#include ".\gashbilling.h"
#include "ThreadManager.h"
#include "GSSimLayer.h"
#include "GSDBLayer.h"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "Socket/NetCommon.h"
#include "LogManager.h"
#include "BillingError.h"
#include "VirtualCash.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "SlangFilter.h" // IsInQueryInvalidChar
#include "support_User.h"
#include "Log4.h"
#include "SocialCommerce.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KGashBilling


KGashBilling::KCommonData KGashBilling::ms_ComnData = {
    KncCriticalSection(), std::string(), {0, 0}, std::string(), std::string()
};

KGashBilling::KGashBilling(void)
: m_dwWaitTime( 8000 )
,m_dwThreadID(0)
{
    m_nResult = -1;
    m_strOutErr.assign( "" );
    m_strOutLogin.assign( "" );
    m_strOutAccType.assign( "" );

    KLocker lock( ms_ComnData.m_cs );

    for( int i = 0; i < ACTION_NUM; i++ )
    {
        m_kSock[i].m_pkGash = this;
        m_kSock[i].SetKey( i );

        m_hEvents[ i * 2 + 0 ] = CreateEvent( NULL, false, false, NULL );
        m_hEvents[ i * 2 + 1 ] = CreateEvent( NULL, false, false, NULL );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[ i * 2 + 0 ] );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[ i * 2 + 1 ] );

        LIF( m_kSock[i].Connect( ms_ComnData.m_strGashIP.c_str(), ms_ComnData.m_usPort[i], false ) );

        m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL ); // 완료시 이벤트 생성위해.
    }
    KBillDB::DriveConnect();
}

KGashBilling::~KGashBilling(void)
{
    End( 15000 ); // 너무 빨리 끝내면 타임아웃시간 보다 짧아서 멤버를 참조하다 크래시 난다.
    // for loop에 등호(=)를 넣지 않는다. 맨 마지막 이벤트는 스레드 종료 감지 이벤트로, KThread에서 처리된다.
	int i = 0;
    for( i = 0; i < ACTION_NUM * 2; i++ )
    {
        ::CloseHandle( m_hEvents[i] );
        m_hEvents[i] = NULL;
    }

    for( i = 0 ; i < ACTION_NUM ; ++i )
    {
        ::CloseHandle( m_hEndEvent[i] );
        m_hEndEvent[i] = NULL;
    }
    
}

//---------------------

bool KGashBilling::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
    if( KThread::Begin() )
    {
        // note : 인덱스이므로 +1 하지 않는다.
        m_hEvents[ACTION_NUM * 2] = m_hKillEvent;
        return true;
    }

    return false;
}

void KGashBilling::Run()
{
    DWORD ret       = NULL;
    bool bLoop      = true;

    m_dwThreadID = ::GetCurrentThreadId();

    while( bLoop )
    {
        ret = ::WaitForSingleObject( m_hEvents[ACTION_NUM * 2], 100 );

        switch( ret )
        {
        case WAIT_OBJECT_0: bLoop = false;  return;  // 스레드 종료
        case WAIT_TIMEOUT: break;
        }

        Tick();
    }
}

void KGashBilling::KSocketGash::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;

    DWORD dwIndex = 0;

    // 길이가 최소 4바이트 이상이고, 아직 버퍼의 끝까지 도달하지 않은 경우
    // nIndex + 1 : 현재 가리키는 곳 다음 바이트까지 접근해야 하므로 +1
    // m_ovlRecv.m_dwLeft - 1 : array의 index처럼 접근되므로 길이에서 -1
    while( m_ovlRecv.m_dwLeft > 4 && dwIndex + 1 <= m_ovlRecv.m_dwLeft - 1 )
    {
        // 온전한 데이터 하나를 분리해서 처리한다.
        if( m_ovlRecv.m_pBuffer[dwIndex] == '\r' && m_ovlRecv.m_pBuffer[dwIndex+1] == '\n' )
        {
            char buff[512];
            memset( buff, 0, 512 ); //초기화.
            // index + 1 까지 끊어낸다.
            ::memcpy( buff, m_ovlRecv.m_pBuffer, dwIndex + 2 );
            {
                KLocker lock( m_pkGash->m_csResult );

                std::string strTmp( buff );    
                typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
                boost::char_separator<char> sep(";\n\r");
                tokenizer tokens( strTmp, sep);
                tokenizer::iterator tok_iter = tokens.begin();

                do{
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_nResult = ::atoi( tok_iter->c_str() );

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutErr = *tok_iter;

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutLogin = *tok_iter;

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutAccType = *tok_iter;
                    

                }while( false );
            }
            // 버퍼에 남아있는 데이터를 앞으로 이동한다.
            m_ovlRecv.m_dwLeft -= dwIndex + 2;
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[dwIndex + 2], m_ovlRecv.m_dwLeft );
            dwIndex = 0;

            switch( m_dwKey ) {
                case AUTHOR:    SetEvent( m_pkGash->m_hEndEvent[AUTHOR] ); break;
                case ACCOUNT:   SetEvent( m_pkGash->m_hEndEvent[ACCOUNT] ); break;
            }
        }
        else
        {
            dwIndex++;
        }
    }

    LIF( InitRecv() );
}

void KGashBilling::InitGashBillingInfo( const char* szIP_, 
                                        unsigned short usPortAuthor_, 
                                        unsigned short usPortAccount_, 
                                        int iServiceCode_, 
                                        const char* szRegionCode_,
                                        const char* szODBC )
{
    KBillDB::InitFileDSN( szODBC );
    KLocker lock( ms_ComnData.m_cs );

    char buff[32] = {0};

    ms_ComnData.m_strGashIP         = szIP_;
    ms_ComnData.m_usPort[AUTHOR]    = usPortAuthor_;
    ms_ComnData.m_usPort[ACCOUNT]   = usPortAccount_;
    ms_ComnData.m_strServiceCode    = ::itoa( iServiceCode_, buff, 10 );
    ms_ComnData.m_strRegionCode     = szRegionCode_;
}

void KGashBilling::Tick()
{
    m_spEvent.reset();

    // 새 이벤트를 처리하고자 하면, 현재 전송중인 데이터가 없어야 한다.
    //JIF( !m_kSock[AUTHEN].IsSending() );
    JIF( !m_kSock[AUTHOR].IsSending() );
    JIF( !m_kSock[ACCOUNT].IsSending() );

    if( !m_pkThreadMgr->GetIntEvent( m_spEvent ) )
        return;

    if( m_spEvent == NULL )
    {
        std::cout << "Thread Terminated." << std::endl;
        SetEvent( m_hKillEvent );
        return;
    }

    KSerializer ks;
    ks.BeginReading( &m_spEvent->m_kbuff );
    KIntEvent& kIntEvent_ = *m_spEvent;  // _INT_CASE 를 사용하기 위해서 필요하다.

    // 한개의 데이터 처리, 지금부터 시작.
    DWORD dwElapTime = ::GetTickCount();

    // 이벤트의 종류를 확인 (1. Authentication 2. Authorization 3. Accounting )
    switch( m_spEvent->m_usEventID ) {
        _INT_CASE( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );
        INT_CASE( EVENT_BUY_FOR_CASH_REQ );
        default:
            START_LOG( cerr, L"invalid event id. id : " << m_spEvent->m_usEventID  ) << END_LOG;
            break;
    }
    dwElapTime = ::GetTickCount() - dwElapTime;
    m_pkThreadMgr->CheckMaxProcessingInfo<IDVenderType>( dwElapTime, *m_spEvent );

}

void KGashBilling::KSocketGash::OnSocketError()
{
    CLOSE_SOCKET( m_sock );
    m_ovlSend.Clear();
    m_ovlRecv.Clear();
    m_ovlSend.m_dwLeft      = 0;
    m_ovlRecv.m_dwLeft      = 0;
    m_ovlSend.m_bSending    = false;
    m_ovlRecv.m_bSending    = false;

    LIF( Connect( NULL, NULL, false ) );
}

bool KGashBilling::WaitEvents( IN const ENUM_ACTION_TYPE eAction_, IN const std::wstring& strLogin_  )
{

    DWORD dwRet;
    DWORD dwTotalElpasedTime = ::GetTickCount();
    do{

        do{
            dwRet = WaitForSingleObject( m_hEvents[ eAction_ * 2 + 1 ], 3000 );
            if( dwRet == WAIT_TIMEOUT )
                break;
            m_kSock[eAction_].OnIOCompleted( KOVERLAPPED::IO_SEND );
        }while( m_kSock[eAction_].IsSending() ); //send가 완료할때까지 돈다.

        if( dwRet  == WAIT_TIMEOUT ) break;

        do{
            dwRet = WaitForSingleObject( m_hEvents[ eAction_ * 2 ], m_dwWaitTime );
            if( dwRet == WAIT_TIMEOUT )
                break;
            m_kSock[eAction_].OnIOCompleted( KOVERLAPPED::IO_RECV );

            dwRet = WaitForSingleObject( m_hEndEvent[ eAction_ ], 1 );

            if( dwRet == WAIT_OBJECT_0 ) break;

        }while( dwRet  == WAIT_TIMEOUT );

        if( dwRet  == WAIT_TIMEOUT ) break;

        if( ::GetTickCount() - dwTotalElpasedTime > 10000 )
        {
            dwRet  = WAIT_TIMEOUT;
            break;
        }

    }while(false);

    dwTotalElpasedTime = ::GetTickCount() - dwTotalElpasedTime;

    if( dwRet == WAIT_TIMEOUT ) // 시간 초과
    {
        START_LOG( cerr, L"Event processing - time over. elapsed : " << m_dwWaitTime )
            << BUILD_LOG( strLogin_ ) << END_LOG;

        //이벤트 리셋.
        ResetEvent( m_hEvents[ eAction_ * 2 + 1 ] );
        ResetEvent( m_hEvents[ eAction_ * 2 ] );
        ResetEvent( m_hEndEvent[ eAction_ ] );

        for( int i = 0; i < ACTION_NUM; i++ )
        {
            m_kSock[i].CancelIo();
            m_kSock[i].OnSocketError();
        }
        InitResultData();
        m_nResult = -10;
        START_LOG( cerr, L"Gash ReConnection" ) << END_LOG;
        return false;
    }
    return true;
}

void KGashBilling::GetCurrentCashPoint( const std::wstring& strLogin_, KCashInfo& cashInfo_ )
{
    cashInfo_.m_dwCashPoint = 0;
    cashInfo_.m_nOK = 105;
    InitResultData();

    // 현재의 캐쉬 포인트를 얻어서 GSDBThread로 전달 한다.
    ResetEvent( m_hEndEvent[AUTHOR] );
    boost::format fmter( "%s'%s'%s'0'0'P'0'%s'0'\r\n" );
    //Ex) 300036'TA'testaccount'0'0'P'0'192.168.1.1'\r\n;
    //ServiceCode'ServiceRegion'ServiceAccountID'ServiceAccountPassword'AuthenticationFlag'Type'DuplicateCheck'IPAddress'\r\n

    std::string strLogin = KncUtil::toNarrowString( strLogin_ );
    { // locking scope
        KLocker lock( ms_ComnData.m_cs );
        fmter   % ms_ComnData.m_strServiceCode 
                % ms_ComnData.m_strRegionCode
                % strLogin 
                % cashInfo_.m_strIP;
    } // locking scope

    std::string strData = fmter.str();

    START_LOG( clog, L"[AUTHORIZATION] Check Cash Point. msg : " << KncUtil::toWideString( strData ) ) << END_LOG;

    LIF( m_kSock[AUTHOR].SendData( strData.c_str(), strData.size() ) );

    if( !WaitEvents( AUTHOR, strLogin_ ) )
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_TIME_OUT|" << strLogin_
            << L"|"<< KNC_TIME_STRING << "|" << KncUtil::toWideString(strData);
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return;
    }

    int nResult;
    std::string strOutput;
    std::string strOutLogin;
    std::string strOutAccType;
    std::string strInsert = "B_";
    {
        KLocker lock( m_csResult );
        nResult         = m_nResult;
        strOutput       = m_strOutErr;
        strOutLogin     = m_strOutLogin;
        strOutAccType   = m_strOutAccType;
    }
    // 3(빌링 시스템오류)
    // 2(차단된 계정)

    //if( nResult == -10 )
    //    nResult = -1;

    if( nResult != 1 || strOutLogin != strLogin )
    {
        switch( nResult )
        {
        case -1 : 
			cashInfo_.m_nOK = 101; 
			strInsert = "Gash side Error";
			break; // 게쉬쪽 오류
        case 0 :
            {
                if( strstr( strOutput.c_str(), "Service_Not_Available" ) != NULL ) // 이 문자열이 포함되어 있다면
                {
                    cashInfo_.m_nOK = 104; break; //체험계정이라 캐쉬검색, 구매를 사용할수 없다. 오류를 발생시키지 말자.
                }
                else if( strstr( strOutput.c_str(), "No_Enough_Point" ) != NULL ) // 이 문자열이 포함되어 있다면
                {
                    cashInfo_.m_nOK = 0;
                    cashInfo_.m_dwCashPoint = 0;
                    break; // 잔액부족
                }

                cashInfo_.m_nOK = 102; 
                strInsert += strOutput; // strOutput에 B_  를 문자열 앞에 삽입, B_Unexpected_Error(빌링오류 표시)
                SiKBillingError()->ErrorDetect( strInsert );
            }
        break;

        case -10:
            strOutput = "TimeOut(GetCurrentCash)"; // 071029. woosh. 타임아웃인 경우
            cashInfo_.m_nOK = -99;
            strInsert += strOutput; // strOutput에 B_  를 문자열 앞에 삽입, B_Unexpected_Error(빌링오류 표시)
            SiKBillingError()->ErrorDetect( strInsert );
            break;
        default: cashInfo_.m_nOK = 110; break;
        }
    }
    else
    {
        cashInfo_.m_nOK = 0;
        cashInfo_.m_dwCashPoint = ::atol( strOutput.c_str() );
    }
    
    if( strLogin != strOutLogin )
    {
        START_LOG( cerr, L" Not Eq Login " << KncUtil::toWideString( strLogin ) << L" != " << KncUtil::toWideString( strOutLogin ) ) << END_LOG;
    }
    
    LOG_CONDITION( (strLogin == m_strOutLogin && nResult == 1) || (nResult == 0 && cashInfo_.m_nOK == 104)
                    || ( ( nResult == 0 && cashInfo_.m_nOK == 0 ) ), clog, cerr )
        << "[AUTHORIZATION] Check Cash Point "
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( strOutLogin ) )
        << BUILD_LOG( KncUtil::toWideString( strOutAccType ) )
        << BUILD_LOG( nResult )
        << BUILD_LOG( KncUtil::toWideString( strInsert )) 
        << BUILD_LOG( KncUtil::toWideString( strData )) << END_LOG;
        
}

void KGashBilling::DecreaseCashPoint( const std::wstring& strLogin_, KCashInfo& cashInfo_, __int64 dwItemSetUID_, GCITEMID dwItemID_, int nCount, int nPeriod, std::wstring wstrItemName )
{
    cashInfo_.m_nOK = 101;
    InitResultData();
    ResetEvent( m_hEndEvent[ACCOUNT] );
    //Format : ServiceCode'ServiceRegion'ServiceAccountID'AccountingType'LoginTime'Points'memo'IPAddress'\r\n
    //300036'TA'testaccount'P'2006/05/22 00:00:00'CashPoint'192.168.1.1'TempUniqID,ItemID,ItemFactor'\r\n
    boost::format fmter( "%s'%s'%s'P'%d/%d/%d %d:%d:%d'%d'%s'%d,%d,%d,%d'0'\r\n" );


    std::string strLogin = KncUtil::toNarrowString( strLogin_ );
    { // locking scope
        KLocker lock( ms_ComnData.m_cs );

        SYSTEMTIME  time;
        ::GetLocalTime( & time );

        fmter   % ms_ComnData.m_strServiceCode 
                % ms_ComnData.m_strRegionCode
                % strLogin
                % time.wYear % time.wMonth % time.wDay % time.wHour % time.wMinute % time.wSecond
                % cashInfo_.m_dwDecreaseCashPoint
                % cashInfo_.m_strIP
                % dwItemSetUID_ // 임시 UID
                % dwItemID_     // 구매하려는 아이템 아이디
                % nCount
                % nPeriod;
    }

    std::string strData = fmter.str();
    std::wstring wstrData = KncUtil::toWideString( strData );
    boost::trim( wstrData );
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_REQ|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|" << wstrData;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }

    LIF( m_kSock[ACCOUNT].SendData( strData.c_str(), strData.size() ) );

    if( !WaitEvents( ACCOUNT, strLogin_ ) )
    {
        switch( m_nResult )
        {
        case -1 : cashInfo_.m_nOK = 101; break;
        case 0 : cashInfo_.m_nOK = 103; break;
        case -10 : cashInfo_.m_nOK = -99; break; // 시간 초과
        default: cashInfo_.m_nOK = 110; break;
        }

        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_TIME_OUT|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|" << wstrData;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;

        return;
    }

    int nResult;
    std::string strInsert = "B_";
    std::string strOutput;
    std::string strOutLogin;
    std::string strOutAccType;
    {
        KLocker lock( m_csResult );
        nResult         = m_nResult;
        strOutput       = m_strOutErr;
        strOutLogin     = m_strOutLogin;
        strOutAccType   = m_strOutAccType;
    }
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_ACK|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|" << wstrData
            << L"|OK:" << nResult
            << L"|OutStr:" << KncUtil::toWideString(strOutput)
            << L"|Login:" << KncUtil::toWideString(strOutLogin)
            << L"|AccType:" << KncUtil::toWideString(strOutAccType);
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }


    if( nResult != 1 || strOutLogin != strLogin ) //오류
    {
        switch( nResult )
        {
        case -1 : cashInfo_.m_nOK = 101; break;
        case 0 : cashInfo_.m_nOK = 103; break;
        case -10 : 
            cashInfo_.m_nOK = -99; 
            strOutput = "TimeOut(DecreaseCash)";
            break;
        default: cashInfo_.m_nOK = 110; break;
        }
        strInsert += strOutput;
        SiKBillingError()->ErrorDetect( strInsert );
    }
    else
    {
        cashInfo_.m_nOK = 0;
        cashInfo_.m_dwCashPoint -= cashInfo_.m_dwDecreaseCashPoint;
    }

    if( strLogin != strOutLogin )
    {
        START_LOG( cerr, L" Not Eq Login " << KncUtil::toWideString( strLogin ) << L" != " << KncUtil::toWideString( strOutLogin ) ) << END_LOG;
    }

    LOG_CONDITION( (cashInfo_.m_nOK == 0 && strLogin == m_strOutLogin), clog, cerr )
        << "[ACCOUNT] Cash Decrease . Name : "
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( strOutLogin ))
        << BUILD_LOG( KncUtil::toWideString( strOutAccType ))
        << BUILD_LOG( nResult )
        << BUILD_LOG( KncUtil::toWideString( strOutput ))
        << BUILD_LOG( KncUtil::toWideString( strData ) )
        << BUILD_LOG( cashInfo_.m_dwBillingType )
        << BUILD_LOG( cashInfo_.m_dwCashPoint )
        << BUILD_LOG( cashInfo_.m_dwDecreaseCashPoint )
        << BUILD_LOG( KncUtil::toWideString( cashInfo_.m_strIP ) ) << END_LOG;
}


_INT_IMPL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo )
{
    GetCurrentCashPoint( strLogin_, kPacket_ );    

    _QUEUING_ACK_TO_USER( EVENT_CURRENT_CASH_POINT_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ )
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_CASH_REQ);

    KEVENT_BUY_FOR_CASH_ACK kPacket;

    START_LOG( clog, L"구매 응답 패킷 확인" )
        << BUILD_LOG( kPacket.m_cashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket_.m_cashInfo.m_dwCashPoint )
        << END_LOG;

    kPacket.m_nOK = 0;
    DWORD dwTotalPoint = 0;
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    kPacket.m_ItemID = kBuyInfo.m_ItemID;
    kPacket.m_dwEventUID = kPacket_.m_dwEventUID;

    // 소셜 커머스 아이템일 경우 처리
    if( true == SiKSocialCommerce()->IsRun() && 0 != kPacket_.m_dwEventUID ) {
        // 일단 판매 종료 된 상태인지 확인
        if ( false == SiKSocialCommerce()->CheckSellEnable( kPacket_.m_dwEventUID ) ) {
            kPacket.m_nOK = 8;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
        int nCurrentSellCount = 0;
        // 현재까지 판매 된 수량 가져 옴
        m_kHelper.GetSellCountByEventUID_SocialCommerce( kPacket_.m_dwEventUID, nCurrentSellCount );
        // 수량을 바탕으로 판매 상태 갱신
        SiKSocialCommerce()->UpdateSellCountAtBuyCashItem( kPacket_.m_dwEventUID, nCurrentSellCount );
        // 판매 가능한지 다시 확인
        if ( false == SiKSocialCommerce()->CheckSellEnable( kPacket_.m_dwEventUID ) ) {
            kPacket.m_nOK = 8;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
    }

    {
        kPacket.m_nOK = kPacket_.m_cashInfo.m_nOK; //캐쉬 잔여량 얻기 실패
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    //캐쉬 잔여량 얻기.
    GetCurrentCashPoint( strLogin_, kPacket_.m_cashInfo );

    if( kPacket_.m_cashInfo.m_nOK != 0 )
    {
        kPacket.m_nOK = kPacket_.m_cashInfo.m_nOK; //캐쉬 잔여량 얻기 실패
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    // 대만에서 0원 캐시 아이템의 판매를 위해서 아래 if 문 주석처리함. 2012-01-04 woosh
    //if( kPacket_.m_cashInfo.m_dwCashPoint <= 0 )
    //{
    //    kPacket.m_nOK = -6; //잔액 부족, 클라이언트에서 거르지만 올때도 많다.
    //    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    //    return;
    //}

    kPacket.m_cashInfo = kPacket_.m_cashInfo;

    if( !KBillDB::CheckBuyLimitItem( dwUID_, kBuyInfo.m_ItemID ) )
    {
        kPacket.m_nOK = 3; // 구매 제한에 걸렸음.
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    int nUseVirtualCash = -99;
    kPacket_.m_kVirtualCashInfo.m_dwCashPoint = m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ );
    nUseVirtualCash = CheckVirtualCash( kPacket_.m_kVirtualCashInfo );

    dwTotalPoint = kPacket.m_cashInfo.m_dwCashPoint;

    if( nUseVirtualCash == NOT_ENOUGH_VIRTUAL_CASH )
    {
        kPacket.m_nOK = -6; // 가상캐쉬의 잔액 부족
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }
    else if( nUseVirtualCash == USE_VIRTUAL_CASH )
    {
        if( SiKVirtualCash()->FindRestrictItem( kBuyInfo.m_ItemID ) == true )
        {
            kPacket.m_nOK = -7;
            // 가상캐쉬로 구입할 수 없는 아이템이라면, 에러 처리하고 클라로 알려주기
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
        dwTotalPoint = kPacket.m_cashInfo.m_dwCashPoint + kPacket_.m_kVirtualCashInfo.m_dwCashPoint; // up_buy_cash_item 잔액을 위함
    }

    // 선물일 경우 대상 유저의 UID를 얻어내야 한다.
    DWORD dwOwnerUID = kPacket_.m_dwBuyerUID; //기본으로 구매자의 UID를 셋팅한다.
    kPacket_.m_strBuyerLogin = strLogin_;

    if( kPacket_.m_bPresent )
    {
        boost::replace_all( kPacket_.m_strComment, L"'", L"''" );
        if( kPacket_.m_strOwnerNickName.empty() )
        {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"닉네임이 비어 있음. name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }

        if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strOwnerNickName ) )
        {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"사용 할수 없는 문자 포함 name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }

        bool bSuccessGetOwnerUID = KBillDB::NickToLoginUID( kPacket_.m_strOwnerNickName, dwOwnerUID );
        if ( !bSuccessGetOwnerUID ) {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"선물하려는 대상이 존재하지 않음 name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }

        if ( bSuccessGetOwnerUID && (kPacket_.m_dwBuyerUID == dwOwnerUID ) ) {
            kPacket.m_nOK = 8;
            START_LOG( cwarn, L"선물하려는 대상과 선물받는 대상이 같음, 받는사람 name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }

        kBuyInfo.m_nCharLevel = 0;
    }

    // 아이템 구매 SP를 호출한다. 현재 게임에서는 아나의 아이템을 구매 하게 되어 있지만..
    // 차후 카트 기능이 들어간다면 아래의 sp를 제대로 사용하자.
    //call dbo.up_buy_cash_item OwnerLoginUID, '아이템리스트(ItemID, Factor, ItemID, Factor, ...)', BuyerLoginUID, '코멘트', BuyerCheck(0/1), 보유Gash량
    if( m_kHelper.GetPrice( kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, kBuyInfo.m_nPrice, kBuyInfo.m_nMoneyType ) == false )
    {
        kPacket.m_nOK = 1;
        START_LOG( cwarn, L"가격 정보 가져오기 실패" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    if( dwTotalPoint < static_cast<DWORD>( kBuyInfo.m_nPrice ) ) {
        kPacket.m_nOK = -6;
        START_LOG( cwarn, L"아이템 구매가격보다 보유하고 있는 금액이 적다." )
            << BUILD_LOG( dwTotalPoint )
            << BUILD_LOG( kBuyInfo.m_nPrice )
            << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    if( kBuyInfo.m_nMoneyType != KItemManager::EMT_CASH )
    {
        kPacket.m_nOK = -3;
        START_LOG( cwarn, L"캐시가 아닌 아이템 구매 시도" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    __int64 waitItemUID = 0;
    if ( m_kHelper.BuyItemToWait_Mixed( dwOwnerUID, kPacket_.m_strBuyerLogin, kBuyInfo, KItemManager::EMT_VIRTUALCASH, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint, L"", waitItemUID ) != 0 )
    {
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
        kPacket.m_nOK = -1;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    std::wstring wstrItemName;

    LogQuery( KBillDB::GetLastQuery(), strLogin_ );

    //  090519  woosh. 
    kPacket.m_kVirtualCashInfo.m_dwCashPoint = kPacket_.m_kVirtualCashInfo.m_dwCashPoint; // ack로 줄 가상캐쉬 잔액
    kPacket.m_cashInfo.m_dwDecreaseCashPoint = kBuyInfo.m_nPrice;

    if( nUseVirtualCash == USE_VIRTUAL_CASH )
    {
        if( kPacket.m_cashInfo.m_dwDecreaseCashPoint == kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
        {
            // 물품가 == 가상캐쉬 차감액 -> 캐쉬 안쓰고 날로 먹는다는 거지요. 가상캐쉬 구매로 갔어야지!
            kPacket.m_nOK = -6;
            //롤백 해주자.
            if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
                kPacket.m_nOK = -10;
            }
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );

            START_LOG( cerr, L"물품가 == 가상캐쉬 차감액, 캐쉬없이 날로먹는 경우")
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( kPacket.m_nOK )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwDecreaseCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
                << BUILD_LOG( kBuyInfo.m_ItemID ) << END_LOG;
            return;
        }

        if( SiKVirtualCash()->LimitVirtualCashRatio( kPacket.m_cashInfo.m_dwDecreaseCashPoint, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint ) == false )
        {
            kPacket.m_nOK = OVER_VIRTUAL_CASH_RATIO;
            //롤백 해주자.
            if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
                kPacket.m_nOK = -10;
            }
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );

            START_LOG( cerr, L"가상캐쉬 구매제한")
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( kPacket.m_nOK )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwDecreaseCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
                << BUILD_LOG( kBuyInfo.m_ItemID ) << END_LOG;
            return;
        }

        // Check limit ratio
        DiscountCash( kPacket_.m_kVirtualCashInfo, kPacket.m_cashInfo );
        m_kHelper.VirtualCashMergeWithType( dwUID_, kBuyInfo.m_ItemID, KVirtualCash::EVCMT_SUB_MIXCASH_ITEM_BUY, 0, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint );

        // 가상캐쉬 잔액
        kPacket.m_kVirtualCashInfo.m_dwCashPoint = kPacket_.m_kVirtualCashInfo.m_dwCashPoint - kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint;
    }

    // waitItemUID이 0일 경우에 대한 체크 추가.
    // 해당 부분은 대만에 Gash 서버로그에 ItemUID에 0이 들어간 로그가 발견되서 해당 부분을 확인하기 
    // 위해서 추가.
    if( 0 == waitItemUID ) {
        kPacket.m_cashInfo.m_nOK = -10; // ok값을 롤백 처리된 ok값을 넣어준다.
        START_LOG( cerr, L"ItemUID Value is Zero" )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( kBuyInfo.m_ItemID )
            << BUILD_LOG( waitItemUID ) << END_LOG;
    } 
    else if ( kBuyInfo.m_nPrice > 0 ) {
        DecreaseCashPoint( strLogin_, kPacket.m_cashInfo, waitItemUID, kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, wstrItemName );
    } 
    else if ( kBuyInfo.m_nPrice == 0 ) { //  2012/1/4 woosh , 0원 캐쉬 아이템인 경우에 ok 처리를 한다
        kPacket.m_cashInfo.m_nOK = 0;
    }


    if( kPacket.m_cashInfo.m_nOK != 0 &&  kPacket.m_cashInfo.m_nOK != -99 )
    {
        //캐쉬 차감 실패
        kPacket.m_nOK = kPacket.m_cashInfo.m_nOK;
        //롤백 해주자.
        if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
            kPacket.m_nOK = -10;
        }

        if( nUseVirtualCash == USE_VIRTUAL_CASH ) {
            m_kHelper.VirtualCashMergeWithType( dwUID_, kBuyInfo.m_ItemID, KVirtualCash::EVCMT_ADD_MIXCASH_ITEM_BUY_ROLLBACK, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint, 0 );
        }

        if( kPacket.m_cashInfo.m_dwCashPoint < kPacket.m_cashInfo.m_dwDecreaseCashPoint ) {
            START_LOG( cerr, L"게쉬 잔액보다 비싼 아이템 구매시도" )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwCashPoint )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwDecreaseCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwCashPoint ) << END_LOG;
        }

        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    if( kPacket.m_cashInfo.m_nOK == -99 ) // 이 경우는 타임 아웃이 되었을때다..
    {
        LIF( KBillDB::SetTimeOutMark( waitItemUID ) );
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );

        //캐쉬차감실패
        kPacket.m_nOK = kPacket.m_cashInfo.m_nOK;
        //롤백해주자.
        if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
            kPacket.m_nOK = -10;
        }

        if( nUseVirtualCash == USE_VIRTUAL_CASH ) {
            m_kHelper.VirtualCashMergeWithType( dwUID_, kBuyInfo.m_ItemID, KVirtualCash::EVCMT_ADD_MIXCASH_ITEM_BUY_ROLLBACK, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint, 0 );
        }

        //로그남기자..
        SiKLogManager()->Get( KLogManager::LT_GASH_HK )
            << L"Name : " << strLogin_ << L", UID : " << dwUID_ 
            << L", ItmeInfo : " << kBuyInfo.m_ItemID
            << L", " << kBuyInfo.m_nCount
            << L", " << kBuyInfo.m_nPeriod
            << L", (" <<  wstrItemName
            << L", " << kPacket.m_cashInfo.m_dwDecreaseCashPoint
            << L") , Time : " << KNC_TIME_STRING << GLOG::endl;

        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    m_kHelper.UpdateWaitItemToEnabled( waitItemUID );

     // 선물이 아닌경우만 얻어오자..
    if( !kPacket_.m_bPresent ) {
        if ( !m_kHelper.InsertWaitItemToInven( dwOwnerUID, kBuyInfo.m_cCharType, KItemManager::EWIT_CASH_BUY, kPacket.m_vecItem ) ) {
            kPacket.m_nOK = 4;
            kPacket_.m_cashInfo.m_nOK = -10;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
    }// end if

    // 성공했을 경우에 아이템의 구매를 DB에 기록하자.( 구매제한 )
    CommitBuyLimitItem( dwUID_, kBuyInfo.m_ItemID );

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
}

int KGashBilling::CheckVirtualCash( IN KVirtualCashInfo& kVirtualCashInfo_ )
{
    if( kVirtualCashInfo_.m_dwDeductCashPoint <= 0 )
    {
        START_LOG( clog, L"구매시 가상캐쉬 사용 안함")
            << BUILD_LOG( kVirtualCashInfo_.m_dwDeductCashPoint ) << END_LOG;
        return NO_USE_VIRTUAL_CASH;
    }
    else if( kVirtualCashInfo_.m_dwCashPoint < kVirtualCashInfo_.m_dwDeductCashPoint ) // 잔액 < 사용액
    {
        START_LOG( clog, L"구매시 가상캐쉬 잔액 부족")
            << BUILD_LOG( kVirtualCashInfo_.m_dwDeductCashPoint )
            << BUILD_LOG( kVirtualCashInfo_.m_dwCashPoint ) << END_LOG;
        return NOT_ENOUGH_VIRTUAL_CASH;
    }
    else
    { // 정상, 가상캐쉬 사용
        START_LOG( clog, L"구매시 가상캐쉬 사용")
            << BUILD_LOG( kVirtualCashInfo_.m_dwDeductCashPoint )
            << BUILD_LOG( kVirtualCashInfo_.m_dwCashPoint ) << END_LOG;
        return USE_VIRTUAL_CASH;
    }
}

void KGashBilling::DiscountCash( IN KVirtualCashInfo& kVirtualCashInfo_, IN OUT KCashInfo& cashInfo_ )
{    
    START_LOG( clog, L"구매시 캐쉬 = 캐쉬 - 가상캐쉬 처리전")
        << BUILD_LOG( kVirtualCashInfo_.m_dwDeductCashPoint )
        << BUILD_LOG( kVirtualCashInfo_.m_dwCashPoint )
        << BUILD_LOG( cashInfo_.m_dwDecreaseCashPoint ) << END_LOG;

    cashInfo_.m_dwDecreaseCashPoint -= kVirtualCashInfo_.m_dwDeductCashPoint;

    START_LOG( clog, L"구매시 캐쉬 = 캐쉬 - 가상캐쉬 처리후")
        << BUILD_LOG( kVirtualCashInfo_.m_dwDeductCashPoint )
        << BUILD_LOG( kVirtualCashInfo_.m_dwCashPoint )
        << BUILD_LOG( cashInfo_.m_dwDecreaseCashPoint ) << END_LOG;
}


void KGashBilling::LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ )
{
    std::wstringstream stm;
    stm << m_dwThreadID << L"|QUERY|" << strLogin_
        << L"|"<< KNC_TIME_STRING
        << L"|" << strLastQuery_;
    SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
}
