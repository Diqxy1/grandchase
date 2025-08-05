#include "GSSimLayer.h"
#include "TH_PayLetterBilling.h"
#include "Socket/NetCommon.h"
#include "LogManager.h"
#include "GameServer.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "SlangFilter.h" // IsInQueryInvalidChar
#include "support_User.h"
#include "Log4.h"
#include "SocialCommerce.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KTH_PayLetterBilling

KTH_PayLetterBilling::KCommonData KTH_PayLetterBilling::ms_ComnData = 
{
    KncCriticalSection(),
    std::string(),
    0,
    0,
    0
};


KTH_PayLetterBilling::KTH_PayLetterBilling( void )
{
    KTHPayLetterEvent::Init();

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

KTH_PayLetterBilling::~KTH_PayLetterBilling( void )
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

void KTH_PayLetterBilling::InitResultData()
{
    m_kResultEvent.Reset();
}

bool KTH_PayLetterBilling::Begin()
{
    return KThread::Begin();
}

void KTH_PayLetterBilling::Run()
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

void KTH_PayLetterBilling::InitPayLetterBillingInfo( const char*       szIP_,
                                                  unsigned short    usPort_,
                                                  const char*       szODBC_ )
{
    KTH_PayLetterBilling::InitFileDSN( szODBC_ );

    KLocker lock( ms_ComnData.m_cs );
    ms_ComnData.m_strBillingServerIP       = szIP_;
    ms_ComnData.m_usPort[TH_BILLING]       = usPort_;
}

void KTH_PayLetterBilling::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

    switch( kIntEvent_.m_usEventID )
    {
       _INT_CASE( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );
        INT_CASE( EVENT_BUY_FOR_CASH_REQ );

    default:
        START_LOG( cerr, L"Invalid Event ID. ID : " << kIntEvent_.m_usEventID ) << END_LOG;
        break;
    }
    return;
}

void KTH_PayLetterBilling::InitializeConnection( const PORT_TYPE ePortType_ )
{
    
}

_INT_IMPL_ON_FUNC( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo )
{
    KTHPayLetterEvent kEvent;

    START_LOG( clog, L"잔액 조회 패킷 도착 ") << END_LOG;

    GetCurrentCashPoint( dwUID_, strLogin_, kPacket_ );

   _QUEUING_ACK_TO_USER( EVENT_CURRENT_CASH_POINT_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ )
{
    EVENT_CHECK_OUT(EVENT_BUY_FOR_CASH_REQ);

    KTHPayLetterEvent kEvent;
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

    GetCurrentCashPoint( dwUID_, strLogin_, kPacket_.m_cashInfo );

    if( !KBillDB::CheckBuyLimitItem( dwUID_, kBuyInfo.m_ItemID ) )
    {
        kPacket.m_nOK = 3; // 구매 제한에 걸렸음.
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    kPacket.m_cashInfo.m_dwCashPoint = kPacket_.m_cashInfo.m_dwCashPoint;

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
    }

    __int64 waitItemUID = 0;
    std::wstring wstrItemName;

    if( m_kHelper.BuyItemToWait( kPacket_.m_dwBuyerUID, kPacket_.m_strBuyerLogin, kBuyInfo, L"", waitItemUID, wstrItemName ) != 0 )
    {
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
        kPacket.m_nOK = -1;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    LogQuery( KBillDB::GetLastQuery(), strLogin_ );
	if( kBuyInfo.m_nPrice != 0 ) {
    BuyAndDeduct( dwUID_, strLogin_, kPacket_, wstrItemName, kPacket );
	} else {
		kPacket.m_cashInfo.m_nOK = 0;
	}
//proc_failed:
    if( kPacket.m_cashInfo.m_nOK != 0 ) // 구매  실패했는가?
    {
        // 구매 인증 or 캐쉬 차감 or 구매 확인 실패
        kPacket.m_nOK = kPacket.m_cashInfo.m_nOK;
        //롤백 해주자.
        if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
            kPacket.m_nOK = -10;
        }
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cerr, L"아이템 구매 실패." )
            << BUILD_LOG( kPacket.m_cashInfo.m_nOK )
            << END_LOG;

        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    m_kHelper.UpdateWaitItemToEnabled( waitItemUID );

    if( !kPacket_.m_bPresent ) // 선물이 아닌 경우
    {
        if ( !m_kHelper.InsertWaitItemToInven( dwOwnerUID, kBuyInfo.m_cCharType, KItemManager::EWIT_CASH_BUY, kPacket.m_vecItem ) ) {
            kPacket.m_nOK = 4;
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

bool KTH_PayLetterBilling::GetCurrentCashPoint( const DWORD dwUID_, const std::wstring& strLogin_, KCashInfo& kCashInfo_ )
{
    KTHPayLetterEvent kEvent;
    KTH_BILL_HEADER kHeader;
    _BILL_PACK_GETBALANCE kBalance;
    std::string strLogin = KncUtil::toNarrowString( strLogin_ );

    kCashInfo_.m_dwCashPoint = 0;
    kCashInfo_.m_nOK = -99;

    kHeader.ReqType = KTHPayLetterEvent::EVENT_TH_GET_BALANCE;
    kHeader.ReqLen  = sizeof( KTH_BILL_HEADER ) + sizeof( _BILL_PACK_GETBALANCE );
    ::memcpy( kBalance.UserID, strLogin.c_str(), strLogin.length() );
    //kBalance.UserID = dwUID_;

    kEvent.WriteHeader( kHeader );
    kEvent.WritePacket( kBalance );

    START_LOG( clog, L"캐쉬 잔액 요청 보냄. Name : " << strLogin_ ) << END_LOG;

    SendPayLetterPacket( TH_BILLING, kEvent );

    if( !WaitEvents( TH_BILLING, strLogin_ ) )
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_TIME_OUT|" << strLogin_ << L"|"<< KNC_TIME_STRING;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return false;
    }

    m_kResultEvent.ReadHeader( kHeader );
    //if( !HandleErrorNot( m_kResultEvent ) ) // 0 은 성공, 에러 응답일 때 처리..
    if( kHeader.ReqType == KTHPayLetterEvent::EVENT_TH_GET_BALANCE )
    {
        m_kResultEvent.ReadPacket( kBalance );

        kCashInfo_.m_nOK = (int)kBalance.RetCode;
        kCashInfo_.m_dwCashPoint = kBalance.BonusCash + kBalance.RealCash;

        START_LOG( clog, L"빌링 시스템에서 얻어온 캐쉬 잔액." )
            << BUILD_LOG( kCashInfo_.m_nOK )
            << BUILD_LOG( KTHPayLetterEvent::GetEventIDString( kBalance.RetCode ) )
            << BUILD_LOG( kBalance.BonusCash + kBalance.RealCash )
            << BUILD_LOG( kCashInfo_.m_dwCashPoint ) << END_LOG;
    }
    else
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_DIFF_ID|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|EVENT_ID:" << kHeader.ReqType;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return false;
    }
    return true;
}

bool KTH_PayLetterBilling::BuyAndDeduct( const DWORD dwUID_, const std::wstring& strLogin_, KEVENT_BUY_FOR_CASH_REQ& kPacket_, std::wstring& wstrItemName_, KEVENT_BUY_FOR_CASH_ACK& kAck_ )
{
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    DWORD dwPacketElapsedTime = ::GetTickCount();
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_REQ|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << kBuyInfo.m_ItemID
            << L"|Count:" << kBuyInfo.m_nCount
            << L"|Period:" << kBuyInfo.m_nPeriod
            << L"|Price:" << kBuyInfo.m_nPrice
            << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }

    KTHPayLetterEvent           kEvent;
    KTH_BILL_HEADER             kHeader;
    _BILL_PACK_PURCHASEITEM     kPurchase;
    kHeader.ReqLen              = sizeof(KTH_BILL_HEADER) + sizeof(_BILL_PACK_PURCHASEITEM);
    kHeader.ReqType             = KTHPayLetterEvent::EVENT_TH_PURCHASE_ITEM;
    kPurchase.UserNo            = dwUID_;
    kPurchase.ClientIP          = inet_addr( kPacket_.m_cashInfo.m_strIP.c_str() );
    //kPurchase.ClientPort;
//    kPurchase.ReqChargeCashAmt  = kPacket_.m_cashInfo.m_dwDecreaseCashPoint;
    kPurchase.ReqChargeCashAmt  = kBuyInfo.m_nPrice;
    //kPurchase.ItemCategory;
    kPurchase.ItemID            = kBuyInfo.m_ItemID;
    kPurchase.ItemCnt           = 1; //(kPacket_.m_iFactor < 0 ? 1 : kPacket_.m_iFactor);
//    kPurchase.ItemUnitPrice     = kPacket_.m_cashInfo.m_dwDecreaseCashPoint;
    kPurchase.ItemUnitPrice     = kBuyInfo.m_nPrice;
    //kPurchase.ItemType;
    //kPurchase.GameServerNo;
    //kPurchase.WorldNo;
    strncpy_s( kPurchase.UserID, sizeof( kPurchase.UserID ), KncUtil::toNarrowString( strLogin_ ).c_str(), _TRUNCATE );
    //kPurchase.CharacterID[30+1];
    strncpy_s( kPurchase.ItemName, sizeof( kPurchase.ItemName ), KncUtil::toNarrowString( wstrItemName_ ).c_str(), _TRUNCATE );
    //kPurchase.StatProperty1[50+1];
    //kPurchase.StatProperty2[50+1];
    //kPurchase.StatProperty3[50+1];
    kPurchase.Location[0] = 'G';

    kEvent.WriteHeader( kHeader );
    kEvent.WritePacket( kPurchase );

    SendPayLetterPacket( TH_BILLING, kEvent );

    if( !WaitEvents( TH_BILLING, strLogin_ ) )
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

    m_kResultEvent.ReadHeader( kHeader );
    if( kHeader.ReqType == KTHPayLetterEvent::EVENT_TH_PURCHASE_ITEM )
    {
        m_kResultEvent.ReadPacket( kPurchase );

        kAck_.m_cashInfo.m_dwCashPoint  = kPurchase.RealCash + kPurchase.BonusCash;
        kAck_.m_nOK                     = kPurchase.RetCode;
        kAck_.m_cashInfo.m_nOK          = kPurchase.RetCode;

        START_LOG( clog, L"아이템 구매 결과" )
            << BUILD_LOG( kAck_.m_nOK )
            << BUILD_LOG( kPurchase.RealCash + kPurchase.BonusCash )
            << BUILD_LOG( kPurchase.ChargeNo ) << END_LOG;

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
                << L"|ChargeNo:" << kPurchase.ChargeNo
                << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
    else
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_DIFF_ID|" << strLogin_
            << L"|"<< KNC_TIME_STRING
            << L"|EVENT_ID:" << kHeader.ReqType
            << L"|GoodsID:" << kBuyInfo.m_ItemID
            << L"|Count:" << kBuyInfo.m_nCount
            << L"|Period:" << kBuyInfo.m_nPeriod
            << L"|Price:" << kBuyInfo.m_nPrice
            << L"|CurrentCash:" << kPacket_.m_cashInfo.m_dwCashPoint
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return false;
    }
    return true;
}

void KTH_PayLetterBilling::SendPayLetterPacket( const PORT_TYPE ePortType_, const KTHPayLetterEvent& kEvent_ )
{
    KTH_BILL_HEADER kHeader;
    kEvent_.ReadHeader( kHeader );
    START_LOG( clog, L"PayLetter Thread ID : " << m_dwThreadID ) << END_LOG;
    START_LOG( clog, L"PAYLETTER 빌링 서버에 패킷 보냄." )
        << BUILD_LOG( KTHPayLetterEvent::GetEventIDString(kHeader.ReqType) )
        << BUILD_LOG( kHeader.ReqLen ) << END_LOG;

    InitResultData();

    ResetEvent( m_hEndEvent[ePortType_] );

    kEvent_.DumpBuffer();
    m_kSock[ePortType_].m_dwLastHeartBitTime = ::GetTickCount();
    LIF( m_kSock[ePortType_].SendData( ( const char* )kEvent_.GetBuffer(), ( int )kHeader.ReqLen ) );
}


void KTH_PayLetterBilling::KSocketPayLetter::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }
    START_LOG( clog, L"패킷 받음. Size : " << dwTransfered_ ) << END_LOG;
    m_ovlRecv.m_dwLeft += dwTransfered_;

    unsigned short usPacketLen;
    do 
    {
        if( m_ovlRecv.m_dwLeft <= sizeof(usPacketLen) ) break;

        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );
        KncUtil::SwapByteOrder( usPacketLen );

        if( m_ovlRecv.m_dwLeft < usPacketLen ) break;

        m_pkPayLetter->m_kResultEvent.WriteFromGTX( (BYTE*)&m_ovlRecv.m_pBuffer[0], usPacketLen );

        m_ovlRecv.m_dwLeft -= usPacketLen;
        ::memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );

    } while(false);

    SetEvent( m_pkPayLetter->m_hEndEvent[m_dwKey] );
    LIF( InitRecv() );
}

void KTH_PayLetterBilling::KSocketPayLetter::OnSocketError()
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

bool KTH_PayLetterBilling::WaitEvents( IN const PORT_TYPE ePortType_, IN const std::wstring& strLogin_ )
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

void KTH_PayLetterBilling::KeepAliveNot( const PORT_TYPE ePortType_ )
{
    if( ::GetTickCount() - m_kSock[ePortType_].m_dwLastHeartBitTime < HEART_BIT_PERIOD )
        return; // 아직 60초가 지나지 않았다면 넘어감

    if( !m_kSock[ePortType_].IsConnected() )
        return; // 연결 체크

    DWORD dwPacketElapsedTime = ::GetTickCount();

    KTHPayLetterEvent kEvent;
    KTH_BILL_HEADER kHeader;
    _BILL_PACK_HEALTH_CHECK kAlivePacket;
    kHeader.ReqType = KTHPayLetterEvent::EVENT_TH_KEEP_ALIVE;
    kHeader.ReqLen = sizeof( KTH_BILL_HEADER ) + sizeof( _BILL_PACK_HEALTH_CHECK );

    kEvent.WriteHeader( kHeader );
    kEvent.WritePacket( kAlivePacket );

    SendPayLetterPacket( TH_BILLING, kEvent );

    if( !WaitEvents( TH_BILLING, std::wstring(L"") ) )
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|KEEP_ALIVE_TIME_OUT|" << KNC_TIME_STRING
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return;
    }

    m_kResultEvent.ReadHeader( kHeader );
    if( kHeader.ReqType == KTHPayLetterEvent::EVENT_TH_KEEP_ALIVE )
    {
        m_kResultEvent.ReadPacket( kAlivePacket );
        std::wstringstream stm;
        stm << m_dwThreadID << L"|KEEP_ALIVE_ACK|" << KNC_TIME_STRING
            << L"|OK:" << kAlivePacket.RetCode
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
    }
    else
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|KEEP_ALIVE_DIFF_ID|" << KNC_TIME_STRING
            << L"|EVENT_ID:" << kHeader.ReqType
            << L"|ElapseTime:" << ::GetTickCount() - dwPacketElapsedTime;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return;
    }

}

void KTH_PayLetterBilling::LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ )
{
    std::wstringstream stm;
    stm << m_dwThreadID << L"|QUERY|" << strLogin_
        << L"|"<< KNC_TIME_STRING
        << L"|" << strLastQuery_;
    SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
}