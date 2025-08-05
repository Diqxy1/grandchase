#include "GSSimLayer.h"
#include "PayLetterBilling.h"
#include "Socket/NetCommon.h"
#include "LogManager.h"
#include "GameServer.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "SlangFilter.h" // IsInQueryInvalidChar
#include "support_User.h"
#include "KncUtil.h"
#include <boost/lexical_cast.hpp> // lexical_cast
#include "Log4.h"
#include "SocialCommerce.h"
//FILE_NAME_FOR_LOG

template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
}



#define CLASS_TYPE KPayLetterBilling


KPayLetterBilling::KCommonData KPayLetterBilling::ms_ComnData = 
{
    KncCriticalSection(),
    std::string(),
    0,
    0,
    0
};


KPayLetterBilling::KPayLetterBilling( void )
{
    KPayLetterEvent::Init();

    m_dwWaitTime = 60000; // 페이레터 60초
    m_dwThreadID = 0;

    InitResultData();

    KLocker lock( ms_ComnData.m_cs );

    for( int i = 0; i < NUM_PORT_TYPE; i++ )
    {
        m_kSock[i].m_pkPayLetter = this; // 
        m_kSock[i].SetKey( i );
        m_kSock[i].m_dwLastHeartBitTime = ::GetTickCount();

        m_hEvents[i * 2 + 0] = CreateEvent( NULL, false, false, NULL ); // 역할은? IO_RECV
        m_hEvents[i * 2 + 1] = CreateEvent( NULL, false, false, NULL ); // IO_SEND
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[i * 2 + 0] );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[i * 2 + 1] );

        LIF( m_kSock[i].Connect( ms_ComnData.m_strBillingServerIP.c_str(), ms_ComnData.m_usPort[i], false ) );
        //InitializeConnection( ( PORT_TYPE )i );

        m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL ); // EndEvent의 역할은?
    }
    KBillDB::DriveConnect();

}

KPayLetterBilling::~KPayLetterBilling( void )
{
    End( 15000 ); // 스레드 종료를 15초 기다려줌?
    int i = 0;
    for( i = 0; i < NUM_PORT_TYPE * 2; i++ )
    {
        ::CloseHandle( m_hEvents[i] ); // 2개의 이벤트
        m_hEvents[i] = NULL;
    }

    for( i = 0; i < NUM_PORT_TYPE; i++ )
    {
        ::CloseHandle( m_hEndEvent[i] ); // 1개의 종료 이벤트?
        m_hEndEvent[i] = NULL;
    }
}

void KPayLetterBilling::InitResultData()
{
    m_kResultEvent.Reset();
}

bool KPayLetterBilling::Begin()
{
    return KThread::Begin();
}

void KPayLetterBilling::Run()
{
    std::cout << "KSubThread::Run(), thread start." << std::endl;
    m_dwThreadID = ::GetCurrentThreadId();

    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT )
        {
            for( int i = 0; i < NUM_PORT_TYPE; i++ )
            {
                KeepAliveNot( ( PORT_TYPE )i );
            }
            Loop();
        }

        else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

    }// while
}

void KPayLetterBilling::InitPayLetterBillingInfo( const char*       szIP_,
                                                  unsigned short    usPort_,
                                                  const char*       szODBC_ )
{
    KPayLetterBilling::InitFileDSN( szODBC_ );

    KLocker lock( ms_ComnData.m_cs );
    ms_ComnData.m_strBillingServerIP       = szIP_;
    ms_ComnData.m_usPort[PT_BILLING]       = usPort_;
}

void KPayLetterBilling::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

    switch( kIntEvent_.m_usEventID )
    {
       _INT_CASE( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );
        INT_CASE( EVENT_BUY_FOR_CASH_REQ );

    default:
        START_LOG( cerr, L"Invalid Event ID : " << kIntEvent_.m_usEventID  ) << END_LOG;
        break;
    }

}

void KPayLetterBilling::InitializeConnection( const PORT_TYPE ePortType_ )
{
    
}

_INT_IMPL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo )
{
    KPayLetterEvent kEvent;

    START_LOG( clog, L"잔액 조회 패킷 도착 ") << END_LOG;

    GetCurrentCashPoint( strLogin_, kPacket_ );
    //kPacket_.m_dwCashPoint = kBalance.m_dwCashRemain; // 아이템 가격은 디비에서 받아옵니다.

   _QUEUING_ACK_TO_USER( EVENT_CURRENT_CASH_POINT_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ )
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_CASH_REQ);

    KPayLetterEvent kEvent;
    KEVENT_BUY_FOR_CASH_ACK kPacket;
    kPacket.m_nOK = 0;
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    kPacket.m_ItemID = kPacket_.m_kBuyInfo.m_ItemID;
    kPacket.m_dwEventUID = kPacket_.m_dwEventUID;

    START_LOG( clog, L"아이템 구매 시작" )
        << BUILD_LOG( kBuyInfo.m_ItemID )
        << BUILD_LOG( kBuyInfo.m_nCount )
        << BUILD_LOG( kBuyInfo.m_nPeriod )
        << BUILD_LOG( kPacket_.m_cashInfo.m_dwDecreaseCashPoint ) << END_LOG;

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

    GetCurrentCashPoint( strLogin_, kPacket_.m_cashInfo );

    if( !KBillDB::CheckBuyLimitItem( dwUID_, kBuyInfo.m_ItemID ) )
    {
        kPacket.m_nOK = 3; // 구매 제한에 걸렸음.
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    kPacket.m_cashInfo.m_dwCashPoint = kPacket_.m_cashInfo.m_dwCashPoint;

    KEVENT_PAYLETTER_BUY kBuyPacket;

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

        int nOwnerCharType = -1;
        if ( false == KBillDB::CharNickToLoginUID( kPacket_.m_strOwnerNickName, dwOwnerUID, nOwnerCharType ) ) {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"선물하려는 대상이 존재하지 않음 name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
    }

    if( m_kHelper.GetPrice( kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, kBuyInfo.m_nPrice, kBuyInfo.m_nMoneyType ) == false )
    {
        kPacket.m_nOK = 1;
        START_LOG( cwarn, L"가격 정보 가져오기 실패" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    // 잔액과 아이템 가격 비교 코드 위치 수정.
    if ( kBuyInfo.m_nPrice > (int)kPacket.m_cashInfo.m_dwCashPoint ) {

        kPacket.m_nOK = -6;
        START_LOG( cerr, L"캐쉬 잔액보다 비싼 아이템 구매시도" )
            << BUILD_LOG( kPacket.m_cashInfo.m_dwCashPoint )
            << BUILD_LOG( kBuyInfo.m_nPrice )
            << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
            << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwCashPoint )
            << END_LOG;
    
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    if( kBuyInfo.m_nMoneyType != KItemManager::EMT_CASH )
    {
        kPacket.m_nOK = -3;
        START_LOG( cwarn, L"캐시가 아닌 아이템 구매 시도" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    __int64 waitItemUID = 0;
    std::wstring wstrItemName;

    if( m_kHelper.BuyItemToWait( kPacket_.m_dwBuyerUID, kPacket_.m_strBuyerLogin, kBuyInfo, L"", waitItemUID, wstrItemName ) != 0 )
    {
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
        kPacket.m_nOK = 2;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    __int64 BillingKey;
    kPacket.m_cashInfo.m_nOK = 0;

    if( kBuyInfo.m_nPrice != 0 ) {
        BuyAndDeduct( strLogin_, kBuyPacket, kPacket_, wstrItemName, kPacket, BillingKey );
    }

    kPacket.m_cashInfo.m_dwDecreaseCashPoint = kBuyInfo.m_nPrice;
//proc_failed:
    if( kPacket.m_cashInfo.m_nOK != 0 ) // 구매  실패했는가?
    {
        // 구매 인증 or 캐쉬 차감 or 구매 확인 실패
        kPacket.m_nOK = kPacket.m_cashInfo.m_nOK;
        // 롤백 하기
        if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
            kPacket.m_nOK = -10;
        }
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
    }

    m_kHelper.UpdateWaitItemToEnabled( waitItemUID );
    if( kBuyInfo.m_nPrice != 0 ) {
        KBillDB::SetBillingKey( waitItemUID, BillingKey );
    }

    if( !kPacket_.m_bPresent ) // 선물이 아닌 경우
    {
        if ( !m_kHelper.InsertWaitItemToInven( dwOwnerUID, kBuyInfo.m_cCharType, KItemManager::EWIT_CASH_BUY, kPacket.m_vecItem ) ) {
            kPacket_.m_cashInfo.m_nOK = -10;
            LogQuery( KBillDB::GetLastQuery(), strLogin_ );
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
    }// end if

    // 성공했을 경우에 아이템의 구매를 DB에 기록하자.( 구매제한 )
    CommitBuyLimitItem( dwUID_, kBuyInfo.m_ItemID );

    // 캐시백을 지급한다
    m_kHelper.CheckCashback( dwUID_, strLogin_, kBuyInfo.m_ItemID, kBuyInfo.m_nPrice, kPacket.m_kVirtualCashInfo.m_dwAddCashPoint, kPacket.m_kVirtualCashInfo.m_dwCashPoint );

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
}

bool KPayLetterBilling::GetCurrentCashPoint( const std::wstring& strLogin_, KCashInfo& kCashInfo_ )
{
    KPayLetterEvent kEvent;
    KEVENT_PAYLETTER_BALANCE kBalance;

    kBalance.m_dwCashRemain = 0;
    kCashInfo_.m_dwCashPoint = 0;
    kCashInfo_.m_nOK = -99;

    kEvent.SetPacketHeader( KPayLetterEvent::EVENT_PAYLETTER_BALANCE, sizeof( KEVENT_PAYLETTER_BALANCE ) + sizeof( KPayLetterPacketHeader ), 0 );

    CopyStringToNCashPacketA( kBalance.m_chUserID, ( int )( sizeof kBalance.m_chUserID ), KncUtil::toNarrowString( strLogin_ ).c_str(), strLogin_.size() );

    kEvent.Write( kBalance );

    START_LOG( clog, L"캐쉬 잔액 요청 보냄." )
        << BUILD_LOG( strLogin_ ) << END_LOG;

    SendPayLetterPacket( PT_BILLING, kEvent );

    if( !WaitEvents( PT_BILLING, strLogin_ ) )
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_TIME_OUT|" << strLogin_ << L"|"<< KNC_TIME_STRING;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return false;
    }

    //if( !HandleErrorNot( m_kResultEvent ) ) // 0 은 성공, 에러 응답일 때 처리..
    if( m_kResultEvent.GetEventID() == KPayLetterEvent::EVENT_PAYLETTER_BALANCE )
    {
        m_kResultEvent.Read( kBalance );
        std::wstring strReceiveLogin = KncUtil::toWideString( kBalance.m_chUserID );

        // LoginID 체크
        if( boost::to_lower_copy(strLogin_) != boost::to_lower_copy(strReceiveLogin) )
        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|CASH_POINT_DIFF_LOGIN|" << strLogin_
                << L"|"<< strReceiveLogin
                << L"|"<< KNC_TIME_STRING
                << L"|EVENT_ID:" << m_kResultEvent.GetEventID();
            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;

            Reconnect();
            return false;
        }

        HandleErrorNot( m_kResultEvent );

        std::string strBalance;
        strBalance.append( (char*)( kBalance.m_chUserID ), sizeof( kBalance.m_chUserID ) );

        kCashInfo_.m_nOK = m_kResultEvent.GetReturnValue();
        kCashInfo_.m_dwCashPoint = kBalance.m_dwCashRemain;

        START_LOG( clog, L"빌링 시스템에서 얻어온 캐쉬 잔액." )
            << BUILD_LOG( kCashInfo_.m_nOK )
            << BUILD_LOG( KncUtil::toWideString(strBalance) )
            << BUILD_LOG( kBalance.m_dwCashRemain )
            << BUILD_LOG( strReceiveLogin ) << END_LOG;
    }
    else
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_DIFF_ID|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|EVENT_ID:" << m_kResultEvent.GetEventID();
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;

        Reconnect();
        return false;
    }
    return true;
}

bool KPayLetterBilling::BuyAndDeduct( const std::wstring& strLogin_, 
                                     IN OUT KEVENT_PAYLETTER_BUY& kBuyPacket_, 
                                     KEVENT_BUY_FOR_CASH_REQ& kPacket_, 
                                     std::wstring& wstrItemName_, 
                                     KEVENT_BUY_FOR_CASH_ACK& kAck_,
                                     __int64& BillingKey_ )
{
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    kAck_.m_cashInfo.m_nOK = -99;
    DWORD dwPacketElapsedTime = ::GetTickCount();
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_REQ|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << kBuyInfo.m_ItemID
            << L"|Count:" << kBuyInfo.m_nCount
            << L"|Period:" << kBuyInfo.m_nPeriod
            << L"|Price:" << kBuyInfo.m_nPrice
            << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint
            << L"|ItemName:" << wstrItemName_ ;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }

    KPayLetterEvent kEvent;
    kEvent.SetPacketHeader( KPayLetterEvent::EVENT_PAYLETTER_BUY, sizeof( KEVENT_PAYLETTER_BUY ) + sizeof( KPayLetterPacketHeader ), 0 );

    CopyStringToNCashPacketA( kBuyPacket_.m_chUserID, ( int )( sizeof kBuyPacket_.m_chUserID ), KncUtil::toNarrowString( strLogin_ ).c_str(), strLogin_.size() );
    CopyStringToNCashPacketA( kBuyPacket_.m_chItemName, ( int )( sizeof kBuyPacket_.m_chItemName), KncUtil::toNarrowString( wstrItemName_ ).c_str(), wstrItemName_.size() );

    ::itoa( ( int )kBuyInfo.m_ItemID, kBuyPacket_.m_chItemID, 10 ); // 숫자를 문자로 변환
    kBuyPacket_.m_dwCashRemain = 0; // 초기화 부분에 넣으면 , 제거해도 됨
    kBuyPacket_.m_nItemPrice = kBuyInfo.m_nPrice;

    kEvent.Write( kBuyPacket_ );

    SendPayLetterPacket( PT_BILLING, kEvent );

    if( !WaitEvents( PT_BILLING, strLogin_ ) )
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_TIME_OUT|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << kBuyInfo.m_ItemID
            << L"|Count:" << kBuyInfo.m_nCount
            << L"|Period:" << kBuyInfo.m_nPeriod
            << L"|Price:" << kBuyInfo.m_nPrice
            << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return false;
    }

    if( m_kResultEvent.GetEventID() == KPayLetterEvent::EVENT_PAYLETTER_BUY )
    {
        KEVENT_PAYLETTER_BUY kAck;

        m_kResultEvent.Read( kAck );
        std::wstring strReciveLogin = KncUtil::toWideString( kBuyPacket_.m_chUserID );

        // LoginID 체크
        if( boost::to_lower_copy(strLogin_) != boost::to_lower_copy(strReciveLogin) )
        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|BUY_DIFF_LOGIN|" << strLogin_
                << L"|"<< strReciveLogin
                << L"|"<< KNC_TIME_STRING
                << L"|EVENT_ID:" << m_kResultEvent.GetEventID()
                << L"|ItemID:" << kPacket_.m_kBuyInfo.m_ItemID
                << L"|Count:" << kPacket_.m_kBuyInfo.m_nCount
                << L"|Period:" << kPacket_.m_kBuyInfo.m_nPeriod
                << L"|Price:" << kPacket_.m_cashInfo.m_dwDecreaseCashPoint
                << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint
                << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl; 

            Reconnect();
            return false;
        }

        HandleErrorNot( m_kResultEvent );

        std::string strBuy;
        std::string strItemName;
        std::string strChargeNo;
        strBuy.append( (char*)( kAck.m_chUserID ), sizeof( kAck.m_chUserID ) );
        strItemName.append( (char*)( kAck.m_chItemName), sizeof( kAck.m_chItemName) );
        strChargeNo.append( (char*)( kAck.m_chChargeNo), sizeof( kAck.m_chChargeNo) );

        kAck_.m_cashInfo.m_dwCashPoint = kAck.m_dwCashRemain;
        kAck_.m_nOK = m_kResultEvent.GetReturnValue();
        kAck_.m_cashInfo.m_nOK = m_kResultEvent.GetReturnValue();
        
        boost::trim( strChargeNo );

        if ( from_string<__int64>( BillingKey_, strChargeNo, std::dec ) == false ) {
            strChargeNo = "0";
        }

        START_LOG( clog, L"아이템 구매 결과" )
            << BUILD_LOG( kAck_.m_nOK )
            << BUILD_LOG( KncUtil::toWideString(strBuy) )
            << BUILD_LOG( KncUtil::toWideString(strItemName) )
            << BUILD_LOG( KncUtil::toWideString(strChargeNo) )
            << BUILD_LOG( kAck.m_dwCashRemain )
            << BUILD_LOG( kAck.m_nItemPrice )
            << BUILD_LOG( strReciveLogin )
            << BUILD_LOG( BillingKey_ ) << END_LOG;

        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|BUY_ACK|" << strLogin_
                << L"|"<< KNC_TIME_STRING
                << L"|OK_1:" << kAck_.m_nOK
                << L"|OK_2:" << kAck_.m_cashInfo.m_nOK
                << L"|GoodsID:" << kBuyInfo.m_ItemID
                << L"|Count:" << kBuyInfo.m_nCount
                << L"|Period:" << kBuyInfo.m_nPeriod
                << L"|Price:" << kBuyInfo.m_nPrice
                << L"|BeforeCash:" << kPacket_.m_cashInfo.m_dwCashPoint
                << L"|CurrentCash:" << kAck_.m_cashInfo.m_dwCashPoint
                << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime
                << L"|Key:" << BillingKey_;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
    else
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_DIFF_ID|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|EVENT_ID:" << m_kResultEvent.GetEventID()
            << L"|GoodsID:" << kBuyInfo.m_ItemID
            << L"|Count:" << kBuyInfo.m_nCount
            << L"|Period:" << kBuyInfo.m_nPeriod
            << L"|Price:" << kBuyInfo.m_nPrice
            << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl; 

        Reconnect();
        return false;
    }
    return true;
}

bool KPayLetterBilling::HandleErrorNot( const KPayLetterEvent& kEvent_ )
{
    switch( kEvent_.GetReturnValue() )
    {
    case KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_01:
        START_LOG( cerr, L"캐쉬 부족" )
            << END_LOG;
        break;
    case KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_02:
        START_LOG( cerr, L"존재하지 않는 유저" )
            << END_LOG;
        break;
    case KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_03:
        START_LOG( cerr, L"DB Internal Error" )
            << END_LOG;
        break;
    case KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_04:
        START_LOG( cerr, L"Billing Server Internal Error" )
            << END_LOG;
        break;
    case KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_05:
        START_LOG( cerr, L"이미 취소된 구매건" )
            << END_LOG;
        break;
    default:
        return true;
    }
    return false;
}


void KPayLetterBilling::SendPayLetterPacket( const PORT_TYPE ePortType_, const KPayLetterEvent& kEvent_ )
{
    START_LOG( clog, L"PayLetter Thread ID : " << m_dwThreadID ) << END_LOG;
    START_LOG( clog, L"PAYLETTER 빌링 서버에 패킷 보냄." )
        << BUILD_LOG( kEvent_.GetEventIDString() )
        << BUILD_LOG( kEvent_.GetPacketSize() ) << END_LOG;

    InitResultData();

    ResetEvent( m_hEndEvent[ePortType_] );

    //kEvent_.DumpNBuffer();
    m_kSock[ePortType_].m_dwLastHeartBitTime = ::GetTickCount();
    LIF( m_kSock[ePortType_].SendData( ( const char* )kEvent_.GetANBuffer(), ( int )kEvent_.GetPacketSize() ) );
}


void KPayLetterBilling::KSocketPayLetter::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    START_LOG( clog, L"패킷 받음.")
        << BUILD_LOG( dwTransfered_ )
        << BUILD_LOG( m_pkPayLetter->m_kResultEvent.GetPacketSize() ) << END_LOG;

    m_ovlRecv.m_dwLeft += dwTransfered_;

    //if( m_pkPayLetter->m_kResultEvent.GetPacketSize() )
    {
        m_pkPayLetter->m_kResultEvent.SetPacketHeader( ( BYTE* )m_ovlRecv.m_pBuffer );
        m_ovlRecv.m_dwLeft -= ( DWORD )HEADER_SIZE_OF_PAYLETTER; // 헤더 추출한 나머지 데이터량

        ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[HEADER_SIZE_OF_PAYLETTER], m_ovlRecv.m_dwLeft );

        if( m_ovlRecv.m_dwLeft >= ( DWORD )( m_pkPayLetter->m_kResultEvent.GetPacketSize() - ( unsigned short )HEADER_SIZE_OF_PAYLETTER ) )
        {
            m_pkPayLetter->m_kResultEvent.CopyToANBuffer( ( BYTE* )m_ovlRecv.m_pBuffer, HEADER_SIZE_OF_PAYLETTER, m_pkPayLetter->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_PAYLETTER );

            m_ovlRecv.m_dwLeft -= ( DWORD )( m_pkPayLetter->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_PAYLETTER );
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[m_pkPayLetter->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_PAYLETTER], m_ovlRecv.m_dwLeft );

            SetEvent( m_pkPayLetter->m_hEndEvent[m_dwKey] );
        }
    }
    LIF( InitRecv() );
}

void KPayLetterBilling::KSocketPayLetter::OnSocketError()
{
    CLOSE_SOCKET( m_sock );
    m_ovlSend.Clear();
    m_ovlRecv.Clear();
    m_ovlSend.m_dwLeft      = 0;
    m_ovlRecv.m_dwLeft      = 0;
    m_ovlSend.m_bSending    = false;
    m_ovlRecv.m_bSending    = false;

    // 너무 무자비하게 reconnect 한다.
    ::Sleep( 1000 );

    START_LOG( clog, L"PayLetter 소켓 끊어짐... Thread ID : " << ::GetCurrentThreadId() ) << END_LOG;
    LIF( Connect( NULL, NULL, false ) );
}

bool KPayLetterBilling::WaitEvents( IN const PORT_TYPE ePortType_, IN const std::wstring& strLogin_ )
{
    DWORD dwRet;
    DWORD dwTotalElapsedTime = ::GetTickCount();
    do
    {
        do
        {
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
            // ePortType_ * 2 + 1의 의미 ->  IO_SEND  // m_hEvents[i * 2 + 1]
            if( dwRet == WAIT_TIMEOUT )
            {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );
        }while( m_kSock[ePortType_].IsSending() ); //send가 완료될 때까지 돈다.

        if( dwRet == WAIT_TIMEOUT ) break;

        do
        {  // 071108. woosh.   m_hEvents[i * 2 + 0] : IO_RECV
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 ], m_dwWaitTime );
            if( dwRet == WAIT_TIMEOUT )
            {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_RECV );

            dwRet = WaitForSingleObject( m_hEndEvent[ ePortType_ ], 1 );

            if( dwRet == WAIT_OBJECT_0 ) break;

        }while( dwRet == WAIT_TIMEOUT );

        if( dwRet == WAIT_TIMEOUT ) break;

        if( ::GetTickCount() - dwTotalElapsedTime > TIME_OUT_ELAPSED_TIME_LIMIT )
        {
            dwRet = WAIT_TIMEOUT;
            break;
        }

    }while(false);

    dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

    if( dwRet == WAIT_TIMEOUT ) // 시간 초과
    {
        START_LOG( cerr, L"Event Processing - time over. elapsed : " << m_dwWaitTime )
            << BUILD_LOG( strLogin_ )
            << END_LOG;

        // Event reset.
        ResetEvent( m_hEvents[ ePortType_ * 2 + 1 ] );
        ResetEvent( m_hEvents[ ePortType_ * 2 ] );
        ResetEvent( m_hEndEvent[ ePortType_ ] );

        for( int i = 0; i < NUM_PORT_TYPE; i++ )
        {
            m_kSock[i].CancelIo();
            m_kSock[i].OnSocketError();
        }
        InitResultData();
        START_LOG( cerr, L"PayLetter ReConnection." ) << END_LOG;
        return false;
    }
    return true;
}

bool KPayLetterBilling::WaitSend( IN const PORT_TYPE ePortType_, IN const std::wstring& strEvent_ )
{ // 보내기만 하는 용도?
    DWORD dwRet;
    DWORD dwTotalElapsedTime = ::GetTickCount();

    do 
    {
        do
        {
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
            if( dwRet == WAIT_TIMEOUT )
            {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );

        }while( m_kSock[ePortType_].IsSending() ); //send 완료할 때까지 도세요

        if( dwRet  == WAIT_TIMEOUT ) break;

        //{
        //    dwRet = WaitForSingleObject( m_hEndEvent[ ePortType_ ], 1 );

        //    if( dwRet == WAIT_OBJECT_0 ) break;

        //}

    } while(false);

    dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

    if( dwRet == WAIT_TIMEOUT ) // 시간 초과
    {
        START_LOG( cerr, L"Event processing - time over. elapsed : " << dwTotalElapsedTime )
            << BUILD_LOG( strEvent_ )
            << END_LOG;

        //이벤트 리셋.
        ResetEvent( m_hEvents[ ePortType_ * 2 + 1 ] );
        ResetEvent( m_hEvents[ ePortType_ * 2 ] );
        ResetEvent( m_hEndEvent[ ePortType_ ] );

        for( int i = 0; i < NUM_PORT_TYPE; i++ )
        {
            m_kSock[i].CancelIo();
            m_kSock[i].OnSocketError();
        }
        InitResultData();
        START_LOG( cerr, L"PayLetter ReConnection" ) << END_LOG;
        return false;
    }
    return true;
}

void KPayLetterBilling::CopyStringToNCashPacketA( char* pbyteDest, int iSizeDest, const char* szSrc, int iLenSrcStr )
{
    //START_LOG( clog, L"스트링 카피." )
    //    << BUILD_LOG( iLenSrcStr )
    //    << BUILD_LOG( iSizeDest )
    //    << END_LOG;

    if( iSizeDest < iLenSrcStr + 1 )
    {
        START_LOG( cerr, L"스트링보다 버퍼가 작음." )
            << BUILD_LOG( iLenSrcStr )
            << BUILD_LOG( iSizeDest )
            << END_LOG;

        iLenSrcStr = iSizeDest - 1;
    }

    ZeroMemory( pbyteDest, iSizeDest );

    int i;
    for( i = 0; i < iLenSrcStr; i++ )
    {
        if( szSrc[i] == 0 )
        {
            break;
        }

        pbyteDest[i] = szSrc[i];
    }
}

void KPayLetterBilling::KeepAliveNot( const PORT_TYPE ePortType_ )
{
    if( ::GetTickCount() - m_kSock[ePortType_].m_dwLastHeartBitTime < HEART_BIT_PERIOD )
    {
        return; // 아직 60초가 지나지 않았다면 넘어감
    }

    //START_LOG( clog, L" ANCash Thread ID : " << ::GetCurrentThreadId() ) << END_LOG;

    if( !m_kSock[ePortType_].IsConnected() )
    {
        return; // 연결 체크
    }

    KPayLetterEvent kEvent;
    KEVENT_PAYLETTER_ALIVE kAlivePacket;

    std::string strBillingName = "BOQGTXLevelUpGC";

    kEvent.SetPacketHeader( KPayLetterEvent::EVENT_PAYLETTER_ALIVE, sizeof( KEVENT_PAYLETTER_ALIVE ) + sizeof( KPayLetterPacketHeader ), 0 );

    CopyStringToNCashPacketA( kAlivePacket.m_chServerIP, ( int )( sizeof kAlivePacket.m_chServerIP ), ms_ComnData.m_strBillingServerIP.c_str(), ms_ComnData.m_strBillingServerIP.size() );
    CopyStringToNCashPacketA( kAlivePacket.m_chServerName, ( int )( sizeof kAlivePacket.m_chServerName ), strBillingName.c_str(), strBillingName.size() );
    kAlivePacket.m_usServerPort = ms_ComnData.m_usPort[PT_BILLING];

    kEvent.Write( kAlivePacket );
    SendPayLetterPacket( PT_BILLING, kEvent );

    JIF( WaitSend( PT_BILLING, std::wstring( L"Keep Alive!" ) ) );

    //START_LOG( clog, L"Keep Alive 신호 보냄" )
    //    << BUILD_LOG( ePortType_ )
    //    << BUILD_LOG( kEvent.GetPacketSize() )
    //    << BUILD_LOG( kEvent.GetEventID() ) << END_LOG;

}

void KPayLetterBilling::LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ )
{
    std::wstringstream stm;
    stm << m_dwThreadID << L"|QUERY|" << strLogin_
        << L"|"<< KNC_TIME_STRING
        << L"|" << strLastQuery_;
    SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
}

void KPayLetterBilling::Reconnect()
{
    // Event reset.
    ResetEvent( m_hEvents[1] );
    ResetEvent( m_hEvents[0] );
    ResetEvent( m_hEndEvent );
    m_kSock[0].CancelIo();
    m_kSock[0].OnSocketError();
}
