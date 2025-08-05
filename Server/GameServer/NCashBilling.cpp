#include "GSSimLayer.h"
#include "NCashBilling.h"
#include "GSDBLayer.h"
#include "Socket/NetCommon.h"
#include "LogManager.h"
#include "GameServer.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "SlangFilter.h" // IsInQueryInvalidChar
#include "support_User.h"
#include "BillingError.h"
#include "VirtualCash.h"
#include "Log4.h"
#include "SocialCommerce.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KNCashBilling

KNCashBilling::KCommonData KNCashBilling::ms_ComnData =
{
    KncCriticalSection(),
    std::string(),
    0,
    0,
    0,
};


KNCashBilling::KNCashBilling( void )
:m_dwThreadID(0)
{
    // NCash event init.
    KNCashEvent::Init();

    m_dwWaitTime = 8000;
    InitResultData();

    KLocker lock( ms_ComnData.m_cs );

    for( int i = 0; i < NUM_PORT_TYPE; i++ )
    {
        m_kSock[i].m_pkNCash = this;
        m_kSock[i].SetKey( i );
        m_kSock[i].m_dwLastHeartBitTime = ::GetTickCount();

        m_hEvents[i * 2 + 0] = CreateEvent( NULL, false, false, NULL );
        m_hEvents[i * 2 + 1] = CreateEvent( NULL, false, false, NULL );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[i * 2 + 0] );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[i * 2 + 1] );

        LIF( m_kSock[i].Connect( ms_ComnData.m_strBSIP.c_str(), ms_ComnData.m_usPort[i], false ) );
        InitializeConnection( ( PORT_TYPE )i );

        m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL );
    }
    KBillDB::DriveConnect();
}

KNCashBilling::~KNCashBilling( void )
{
    End( 15000 );

	int i = 0;
    for( i = 0; i < NUM_PORT_TYPE * 2; i++ )
    {
        ::CloseHandle( m_hEvents[i] );
        m_hEvents[i] = NULL;
    }

    for( i = 0; i < NUM_PORT_TYPE; i++ )
    {
        ::CloseHandle( m_hEndEvent[i] );
        m_hEndEvent[i] = NULL;
    }
}

bool KNCashBilling::Begin()
{
    return KThread::Begin();
}

void KNCashBilling::Run()
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
                CheckConnection( ( PORT_TYPE )i );
            }
            Loop();
        }

        else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

    }// while
}

void KNCashBilling::InitNCashBillingInfo( const char*       szIP_,
                                          unsigned short    usPort_,
                                          unsigned short    usServiceID_,
                                          unsigned short    usCPID_,
                                          const char*       szODBC_ )
{
    KNCashBilling::InitFileDSN( szODBC_ );

    KLocker lock( ms_ComnData.m_cs );
    ms_ComnData.m_strBSIP               = szIP_;
    ms_ComnData.m_usPort[PT_BILLING]    = usPort_;
    ms_ComnData.m_usServiceID           = usServiceID_;
    ms_ComnData.m_usCPID                = usCPID_;

    return;
}

void KNCashBilling::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
        _INT_CASE( EVENT_CURRENT_CASH_POINT_REQ, KCashInfo );
        INT_CASE( EVENT_BUY_FOR_CASH_REQ );
        default:
            START_LOG( cerr, L"invalid event id. id : " << kIntEvent_.m_usEventID ) << END_LOG;
            break;
    }
    return;
}

void KNCashBilling::InitResultData()
{
    m_kResultEvent.Reset();
}

bool KNCashBilling::WaitEvents( IN const PORT_TYPE ePortType_, IN const std::wstring& strLogin_ )
{
    DWORD dwRet;
    DWORD dwTotalElpasedTime = ::GetTickCount();
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
        }while( m_kSock[ePortType_].IsSending() ); //send가 완료할때까지 돈다.

        if( dwRet  == WAIT_TIMEOUT ) break;

        do
        {
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 ], m_dwWaitTime );
            if( dwRet == WAIT_TIMEOUT )
            {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_RECV );

            dwRet = WaitForSingleObject( m_hEndEvent[ ePortType_ ], 1 );

            if( dwRet == WAIT_OBJECT_0 ) break;

        }while( dwRet  == WAIT_TIMEOUT );

        if( dwRet  == WAIT_TIMEOUT ) break;

        if( ::GetTickCount() - dwTotalElpasedTime > TIME_OUT_TOLERANCE_OF_CLIENT )
        {
            dwRet  = WAIT_TIMEOUT;
            break;
        }

    }while(false);

    dwTotalElpasedTime = ::GetTickCount() - dwTotalElpasedTime;

    if( dwRet == WAIT_TIMEOUT ) // 시간 초과
    {
        START_LOG( cerr, L"Event processing - time over. elapsed : " << m_dwWaitTime )
            << BUILD_LOG( strLogin_ )
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
        START_LOG( cerr, L"NCash ReConnection" ) << END_LOG;
        return false;
    }
    return true;
}

void KNCashBilling::GetCurrentCashPoint( const std::wstring& strLogin_, KCashInfo& cashInfo_ )
{
    cashInfo_.m_dwCashPoint = 0;
    cashInfo_.m_nOK = 105;

    KNCashEvent kEvent;
    kEvent.SetPacketHeader( KNCashEvent::EVENT_NCASH_CURRENT_MONEY_REQ, sizeof( KEVENT_NCASH_CURRENT_MONEY_REQ ) );
    KEVENT_NCASH_CURRENT_MONEY_REQ kReq;
    kReq.m_usGameServiceID = ms_ComnData.m_usServiceID;
    kReq.m_usCPID = ms_ComnData.m_usCPID;
    strncpy_s( kReq.m_aucUserID, sizeof( kReq.m_aucUserID ), KncUtil::toNarrowString( strLogin_ ).c_str(), _TRUNCATE );
    kEvent.Write( kReq );

    START_LOG( clog, L"캐쉬 잔액 요청 보냄." )
        << BUILD_LOG( strLogin_ ) << END_LOG;

    SendNCashPacket( PT_BILLING, kEvent );

    if( !WaitEvents( PT_BILLING, strLogin_ ) ) {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|CASH_POINT_TIME_OUT|" << strLogin_ << L"|"<< KNC_TIME_STRING;
        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        return;
    }

    if( m_kResultEvent.GetEventID() == KNCashEvent::EVENT_NCASH_CURRENT_MONEY_ACK ) {
        KEVENT_NCASH_CURRENT_MONEY_ACK kAck;
        m_kResultEvent.Read( kAck );
        cashInfo_.m_nOK = static_cast<int>( kAck.m_usResult );
        cashInfo_.m_dwCashPoint = static_cast<DWORD>( kAck.m_nCashBalance );
        // NCash 잔액 가져오기 Return Value.
        /*
        100 ( 성공 )
        -100 ( 유저없음 )
        -400 ( Sys Error )
        */
        if( cashInfo_.m_nOK == 100 ) {
            cashInfo_.m_nOK = 0;
        } 
        else if ( cashInfo_.m_nOK == -100 ) {
            cashInfo_.m_nOK = 0;
        }
        else {
            START_LOG( cerr, L"캐쉬 조회 요청 에러." )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( cashInfo_.m_nOK )
                << BUILD_LOG( cashInfo_.m_dwCashPoint )
                << BUILD_LOG( m_kResultEvent.GetEventID() )
                << BUILD_LOG( m_kResultEvent.GetEventIDString() )
                << END_LOG;
        }
    }
    else {
        START_LOG( cerr, L"캐쉬 조회 요청에 대하여 이상한 패킷이 도착함." )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( m_kResultEvent.GetEventID() )
            << BUILD_LOG( m_kResultEvent.GetEventIDString() )
            << END_LOG;

        cashInfo_.m_nOK = 110;
    }
}

void KNCashBilling::DecreaseCashPoint( IN const std::wstring& strLogin, IN OUT KCashInfo& cashInfo, IN __int64 biItemSetUID, IN GCITEMID dwItemID, IN int nCount, IN int nPeriod, IN std::wstring wstrItemName, OUT int& nTransactionID )
{
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_REQ|" << strLogin
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << dwItemID
            << L"|Factor:" << nCount
            << L"|Factor:" << nPeriod
            << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
            << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }
    cashInfo.m_nOK = 101;

    KNCashEvent kEvent;
    kEvent.SetPacketHeader( KNCashEvent::EVENT_NCASH_BUY_REQ, sizeof( KEVENT_NCASH_BUY_REQ ) );

    KEVENT_NCASH_BUY_REQ kPacket;
    kPacket.m_usGameServiceID = ms_ComnData.m_usServiceID;
    kPacket.m_usCPID = ms_ComnData.m_usCPID;
    strncpy_s( kPacket.m_aucUserID, sizeof( kPacket.m_aucUserID ), KncUtil::toNarrowString( strLogin ).c_str(), _TRUNCATE );
    kPacket.m_nProductCode = dwItemID;
    strncpy_s( kPacket.m_aucProductName, sizeof( kPacket.m_aucProductName ), KncUtil::toNarrowString( wstrItemName ).c_str(), _TRUNCATE );
    kPacket.m_nAmount = cashInfo.m_dwDecreaseCashPoint;
    kPacket.m_biItemUID = biItemSetUID;

    kEvent.Write( kPacket );

    START_LOG( clog, L"아이템 구매 요청 보냄." )
        << BUILD_LOG( biItemSetUID )
        << BUILD_LOG( dwItemID )
        << BUILD_LOG( nCount )
        << BUILD_LOG( nPeriod ) << END_LOG;

    SendNCashPacket( PT_BILLING, kEvent );

    if( !WaitEvents( PT_BILLING, strLogin ) ) {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|BUY_TIME_OUT|" << strLogin
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << dwItemID
            << L"|Count:" << nCount
            << L"|Period:" << nPeriod
            << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
            << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }


    if( m_kResultEvent.GetEventID() == KNCashEvent::EVENT_NCASH_BUY_ACK ) {
        KEVENT_NCASH_BUY_ACK kAck;
        m_kResultEvent.Read( kAck );
        nTransactionID = kAck.m_nTransactionID;

        switch( kAck.m_sResult )
        {
        case 100:
            cashInfo.m_nOK = 0;
            cashInfo.m_dwCashPoint -= cashInfo.m_dwDecreaseCashPoint;
            break;
        case -201:
            START_LOG( cerr, L"사용자 ID가 없어서 결제 실패." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -400:
            START_LOG( cerr, L"시스템 에러." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 101;
            break;
        case -401:
            START_LOG( cerr, L"DB처리 에러." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 101;
            break;
        case -503:
            START_LOG( cerr, L"금액부족." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -504:
            START_LOG( cerr, L"금액부족." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -600:
            START_LOG( cerr, L"상품정보 없음." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 111;
            break;
        case -700:
            START_LOG( cerr, L"DB에 등록된 가격과 상품 가격이 다름." )
                << BUILD_LOG( strLogin ) << END_LOG;
            cashInfo.m_nOK = 111;
            break;
        default:
            START_LOG( cerr, L"캐쉬 차감 실패." )
                << BUILD_LOG( strLogin )
                << BUILD_LOG( kAck.m_sResult ) << END_LOG;
            cashInfo.m_nOK = 110;
            break;
        }

        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|BUY_ACK|" << strLogin
                << L"|"<< KNC_TIME_STRING
                << L"|RecvOK:"<< kAck.m_sResult
                << L"|OK:" << cashInfo.m_nOK
                << L"|GoodsID:" << dwItemID
                << L"|Count:" << nCount
                << L"|Period:" << nPeriod
                << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
                << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
    else {
        START_LOG( cerr, L"캐쉬 차감 요청에 대하여 이상한 패킷이 도착함." )
            << BUILD_LOG( strLogin )
            << BUILD_LOG( m_kResultEvent.GetEventID() )
            << BUILD_LOG( m_kResultEvent.GetEventIDString() )
            << END_LOG;

        cashInfo.m_nOK = 110;
        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|BUY_ACK_DIFF_ID|" << strLogin
                << L"|"<< KNC_TIME_STRING
                << L"|ErrID:"<< m_kResultEvent.GetEventID()
                << L"|GoodsID:" << dwItemID
                << L"|Count:" << nCount
                << L"|Period:" << nPeriod
                << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
                << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
}

int KNCashBilling::HandleErrorNot( const KNCashEvent& kEvent )
{
    if( kEvent.GetEventID() == KNCashEvent::EVENT_NCASH_ERROR_NOT )
    {
        KNCashAckPacket kPacket;
        m_kResultEvent.Read( kPacket );
        switch( kPacket.m_uiResult )
        {
        case 1:
            START_LOG( cerr, L"초기화 되지 않은 상태에서 요청함." )
                << END_LOG;
            break;
        case 2:
            START_LOG( cerr, L"메시지 형식이 틀림." )
                << END_LOG;
            break;
        case 3:
            START_LOG( cerr, L"지원하지 않는 패킷 타입." )
                << END_LOG;
            break;
        }

        START_LOG( cerr, L"Error : " << kPacket.m_uiResult ) << END_LOG;

        return ( int )kPacket.m_uiResult;
    }
    else
    {
        return 0;
    }
}

void KNCashBilling::SendNCashPacket( const PORT_TYPE ePortType, const KNCashEvent& kEvent )
{
    START_LOG( clog, L"NCASH 빌링 서버에 패킷 보냄." )
        << BUILD_LOG( kEvent.GetEventIDString() ) << END_LOG;

    InitResultData();

    ResetEvent( m_hEndEvent[ePortType] );

    //kEvent.DumpNBuffer();
    m_kSock[ePortType].m_dwLastHeartBitTime = ::GetTickCount();
    LIF( m_kSock[ePortType].SendData( ( const char* )kEvent.GetNBuffer(), ( int )kEvent.GetPacketSize() ) );
}

void KNCashBilling::InitializeConnection( const PORT_TYPE ePortType )
{
    KNCashEvent kEvent;
    kEvent.SetPacketHeader( KNCashEvent::EVENT_NCASH_INIT_REQ, sizeof( KEVENT_NCASH_INIT_REQ ) );
    KEVENT_NCASH_INIT_REQ kReq;
    kReq.m_usGameServiceID = ms_ComnData.m_usServiceID;
    kReq.m_usCPID = ms_ComnData.m_usCPID;
    kEvent.Write( kReq );

    SendNCashPacket( ePortType, kEvent );

    JIF( WaitEvents( ePortType, std::wstring( L"Handshake Init" ) ) );

    if( HandleErrorNot( m_kResultEvent ) != 0 )
    {
        START_LOG( cerr, L"초기화 요청 실패." )
            << END_LOG;
    }
    else if( m_kResultEvent.GetEventID() == KNCashEvent::EVENT_NCASH_INIT_ACK )
    {
        START_LOG( clog, L"초기화 성공." ) << END_LOG;
    }
    else
    {
        START_LOG( cerr, L"초기화 요청에 대하여 이상한 패킷이 도착함." )
            << BUILD_LOG( m_kResultEvent.GetEventID() )
            << BUILD_LOG( m_kResultEvent.GetEventIDString() )
            << END_LOG;
    }
}

void KNCashBilling::CheckConnection( const PORT_TYPE ePortType )
{
    if( ::GetTickCount() - m_kSock[ePortType].m_dwLastHeartBitTime < HEART_BIT_PERIOD )
    {
        return;
    }

    if( !m_kSock[ePortType].IsConnected() )
    {
        return;
    }

    KNCashEvent kEvent;
    kEvent.SetPacketHeader( KNCashEvent::EVENT_NCASH_HEART_BIT_REQ, sizeof( KEVENT_NCASH_HEART_BIT_REQ ) );
    KEVENT_NCASH_HEART_BIT_REQ kReq;
    kReq.m_usGameServiceID = ms_ComnData.m_usServiceID;
    kReq.m_usCPID = ms_ComnData.m_usCPID;
    kEvent.Write( kReq );

    SendNCashPacket( ePortType, kEvent );
// NCash에서 HeartBit에 대한 Ack처리를 해줘야 한다.

    JIF( WaitEvents( ePortType, std::wstring( L"Heart bit" ) ) );

    if( m_kResultEvent.GetEventID() == KNCashEvent::EVENT_NCASH_HEART_BIT_ACK ) {
        START_LOG( clog, L"허트빗 성공." ) << END_LOG;
    }
    else {
        START_LOG( cerr, L"허트빗 요청에 대하여 이상한 패킷이 도착함." )
            << BUILD_LOG( m_kResultEvent.GetEventIDString() )
            << END_LOG;
    }
}

void KNCashBilling::CopyStringToNCashPacketA( BYTE* pbyteDest, int iSizeDest, const char* szSrc, int iLenSrcStr )
{
    START_LOG( clog, L"스트링 카피." )
        << BUILD_LOG( iLenSrcStr )
        << BUILD_LOG( iSizeDest )
        << END_LOG;

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

void KNCashBilling::CopyStringToNCashPacketW( BYTE* pbyteDest, int iSizeDest, const wchar_t* szSrc, int iLenSrcStr )
{
    START_LOG( clog, L"스트링 카피." )
        << BUILD_LOG( iLenSrcStr )
        << BUILD_LOG( iSizeDest )
        << END_LOG;

    if( iSizeDest < ( iLenSrcStr + 1 ) * 2 )
    {
        START_LOG( cerr, L"스트링보다 버퍼가 작음." )
            << BUILD_LOG( iLenSrcStr )
            << BUILD_LOG( iSizeDest )
            << END_LOG;

        iLenSrcStr = ( iSizeDest - 2 ) / 2;
    }

    ZeroMemory( pbyteDest, iSizeDest );

    int i;
    for( i = 0; i < iLenSrcStr; i++ )
    {
        if( szSrc[i] == 0 )
        {
            break;
        }

        ( wchar_t& )( pbyteDest[i * 2] ) = szSrc[i];
    }
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
    kPacket.m_nOK = 0;
    DWORD dwTotalPoint = 0;
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    kPacket.m_ItemID = kPacket_.m_kBuyInfo.m_ItemID;
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

    //캐쉬 잔여량 얻기.
    GetCurrentCashPoint( strLogin_, kPacket_.m_cashInfo );
    if( kPacket_.m_cashInfo.m_nOK != 0 ) {
        kPacket.m_nOK = kPacket_.m_cashInfo.m_nOK; //캐쉬 잔여량 얻기 실패
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    if( kPacket_.m_cashInfo.m_dwCashPoint < 0 ) {
        kPacket.m_nOK = -6; //잔액 부족, 클라이언트에서 거르지만 올때도 많다.
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    kPacket.m_cashInfo = kPacket_.m_cashInfo;

    if( !CheckBuyLimitItem( dwUID_, kBuyInfo.m_ItemID ) ) {
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
    if( kPacket_.m_bPresent ) {
        boost::replace_all( kPacket_.m_strComment, L"'", L"''" );
        if( kPacket_.m_strOwnerNickName.empty() ) {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"닉네임이 비어 있음. name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }

        if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strOwnerNickName ) ) {
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

    if( m_kHelper.GetPrice( kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, kBuyInfo.m_nPrice, kBuyInfo.m_nMoneyType ) == false ) {
        kPacket.m_nOK = -1;
        START_LOG( cwarn, L"가격 정보 가져오기 실패" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    if( kBuyInfo.m_nMoneyType != KItemManager::EMT_CASH ) {
        kPacket.m_nOK = -1;
        START_LOG( cwarn, L"캐시가 아닌 아이템 구매 시도" ) << END_LOG;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    __int64 waitItemUID = 0;
    std::wstring wstrItemName;
/*
    if ( m_kHelper.BuyItemToWait_Mixed( dwOwnerUID, kPacket_.m_strBuyerLogin, kBuyInfo, KItemManager::EMT_VIRTUALCASH, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint, L"", waitItemUID ) != 0 )
    {
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
        kPacket.m_nOK = -1;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );

    }
*/
    if( m_kHelper.BuyItemToWait( kPacket_.m_dwBuyerUID, kPacket_.m_strBuyerLogin, kBuyInfo, L"", waitItemUID, wstrItemName ) != 0 ) {
        LogQuery( KBillDB::GetLastQuery(), strLogin_ );
        START_LOG( cwarn, L"구매해서 대기 리스트로 넣기 실패" ) << END_LOG;
        kPacket.m_nOK = -1;
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
    }

    LogQuery( KBillDB::GetLastQuery(), strLogin_ );
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

    int nTransactionID = 0;
    if( !kPacket_.m_bPresent ) {
        // waitItemUID이 0일 경우에 대한 체크 추가.
        if( 0 == waitItemUID ) {
            kPacket.m_cashInfo.m_nOK = -10; // ok값을 롤백 처리된 ok값을 넣어준다.
            START_LOG( cerr, L"ItemUID Value is Zero" )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( kBuyInfo.m_ItemID )
                << BUILD_LOG( waitItemUID ) << END_LOG;
        }
        else {
            if ( kBuyInfo.m_nPrice != 0 ) {
                DecreaseCashPoint( strLogin_, kPacket.m_cashInfo, waitItemUID, kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, wstrItemName, nTransactionID );
            }
        }
    }
    else {
        std::wstring wstrReceiverID;
        if( false == KBillDB::NickNameToLogin( kPacket_.m_strOwnerNickName, wstrReceiverID ) ) {
            kPacket.m_nOK = -1;
            START_LOG( cwarn, L"선물받는 사람 ID없음. name : " << kPacket_.m_strOwnerNickName ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
            return;
        }
        SendGiftItem( strLogin_, wstrReceiverID, kPacket.m_cashInfo, waitItemUID, kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, wstrItemName, nTransactionID );
    }

    if( kPacket.m_cashInfo.m_nOK != 0 && kPacket.m_cashInfo.m_nOK != 110 ) {
        //캐쉬 차감 실패
        kPacket.m_nOK = kPacket_.m_cashInfo.m_nOK;
        //롤백 해주자.
        if( !m_kHelper.RollbackBuyItem( waitItemUID ) ) {
            kPacket.m_nOK = -10;
        }
        if( nUseVirtualCash == USE_VIRTUAL_CASH ) {
            m_kHelper.VirtualCashMergeWithType( dwUID_, kBuyInfo.m_ItemID, KVirtualCash::EVCMT_ADD_MIXCASH_ITEM_BUY_ROLLBACK, kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint, 0 );
        }

        if( kPacket.m_cashInfo.m_dwCashPoint < kPacket.m_cashInfo.m_dwDecreaseCashPoint ) {
            START_LOG( cerr, L"잔액보다 비싼 아이템 구매시도" )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwCashPoint )
                << BUILD_LOG( kPacket.m_cashInfo.m_dwDecreaseCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwDeductCashPoint )
                << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwCashPoint )
                << END_LOG;
        }
        QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    m_kHelper.UpdateWaitItemToEnabled( waitItemUID );
    __int64 biTransactionID = static_cast<__int64>( nTransactionID );
    KBillDB::SetBillingKey( waitItemUID, biTransactionID );

    if( !kPacket_.m_bPresent ) { // 선물이 아닌경우만 얻어오자..
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

    QUEUING_ACK_TO_USER( EVENT_BUY_FOR_CASH_ACK );
}

void KNCashBilling::KSocketNCash::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    START_LOG( clog, L"패킷 받음." )
        << BUILD_LOG( dwTransfered_ )
        << BUILD_LOG( m_pkNCash->m_kResultEvent.GetPacketSize() ) << END_LOG;

    m_ovlRecv.m_dwLeft += dwTransfered_;
    
    if( m_pkNCash->m_kResultEvent.GetPacketSize() == HEADER_SIZE_OF_NCASH )
    {
        // 헤더를 받는 중이다.
        // 헤더 사이즈 이상 받았는지 체크한다.
        if( m_ovlRecv.m_dwLeft >= HEADER_SIZE_OF_NCASH )
        {
            // 헤더를 추출한다.
            m_pkNCash->m_kResultEvent.SetPacketHeader( ( BYTE* )m_ovlRecv.m_pBuffer );

            m_ovlRecv.m_dwLeft -= ( DWORD )HEADER_SIZE_OF_NCASH;
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[HEADER_SIZE_OF_NCASH], m_ovlRecv.m_dwLeft );

            // 헤더만 있는 패킷이면 다 받았다고 알린다.
            if( m_pkNCash->m_kResultEvent.GetPacketSize() == HEADER_SIZE_OF_NCASH )
            {
                SetEvent( m_pkNCash->m_hEndEvent[m_dwKey] );
            }
            // 바디가 달려있는 패킷이면 추출한다.
            else
            {
                if( m_ovlRecv.m_dwLeft >= ( DWORD )( m_pkNCash->m_kResultEvent.GetPacketSize() - ( unsigned short )HEADER_SIZE_OF_NCASH ) )
                {
                    m_pkNCash->m_kResultEvent.CopyToNBuffer( ( BYTE* )m_ovlRecv.m_pBuffer, HEADER_SIZE_OF_NCASH, m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH );

                    m_ovlRecv.m_dwLeft -= ( DWORD )( m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH );
                    ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH], m_ovlRecv.m_dwLeft );

                    SetEvent( m_pkNCash->m_hEndEvent[m_dwKey] );
                }
            }
        }
    }
    else
    {
        // 패킷 바디를 받는 중이다.
        // 패킷 바디를 다 받았으면 추출한다.
        if( m_ovlRecv.m_dwLeft >= ( DWORD )( m_pkNCash->m_kResultEvent.GetPacketSize() - ( unsigned short )HEADER_SIZE_OF_NCASH ) )
        {
            m_pkNCash->m_kResultEvent.CopyToNBuffer( ( BYTE* )m_ovlRecv.m_pBuffer, HEADER_SIZE_OF_NCASH, m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH );

            m_ovlRecv.m_dwLeft -= ( DWORD )( m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH );
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[m_pkNCash->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_NCASH], m_ovlRecv.m_dwLeft );

            SetEvent( m_pkNCash->m_hEndEvent[m_dwKey] );
        }
    }

    LIF( InitRecv() );
}

void KNCashBilling::KSocketNCash::OnSocketError()
{
    CLOSE_SOCKET( m_sock );
    m_ovlSend.Clear();
    m_ovlRecv.Clear();
    m_ovlSend.m_dwLeft          = 0;
    m_ovlRecv.m_dwLeft          = 0;
    m_ovlSend.m_bSending        = false;
    m_ovlRecv.m_bSending        = false;

    // 너무 무자비하게 reconnect 한다.
    ::Sleep( 1000 );

    LIF( Connect( NULL, NULL, false ) );
    m_pkNCash->InitializeConnection( ( KNCashBilling::PORT_TYPE )m_dwKey );
}

void KNCashBilling::LogQuery( IN const std::wstring& strLastQuery_, IN const std::wstring& strLogin_ )
{
    std::wstringstream stm;
    stm << m_dwThreadID << L"|QUERY|" << strLogin_
        << L"|"<< KNC_TIME_STRING
        << L"|" << strLastQuery_;
    SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
}

void KNCashBilling::SendGiftItem( IN const std::wstring& strSenderLogin, IN const std::wstring& strReceiverLogin, IN OUT KCashInfo& cashInfo, IN __int64 biItemSetUID, IN GCITEMID dwItemID, IN int nCount, IN int nPeriod, IN std::wstring wstrItemName, OUT int& nTransactionID )
{
    {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|PRESENT_REQ|" << strSenderLogin
            << L"|"<< strReceiverLogin
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << dwItemID
            << L"|Factor:" << nCount
            << L"|Factor:" << nPeriod
            << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
            << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }
    cashInfo.m_nOK = 101;

    KNCashEvent kEvent;
    kEvent.SetPacketHeader( KNCashEvent::EVENT_NCASH_PRESENT_REQ, sizeof( KEVENT_NCASH_PRESENT_REQ ) );

    KEVENT_NCASH_PRESENT_REQ kPacket;
    kPacket.m_usGameServiceID = ms_ComnData.m_usServiceID;
    kPacket.m_usCPID = ms_ComnData.m_usCPID;
    strncpy_s( kPacket.m_aucSUserID, sizeof( kPacket.m_aucSUserID ), KncUtil::toNarrowString( strSenderLogin ).c_str(), _TRUNCATE );
    strncpy_s( kPacket.m_aucRUserID, sizeof( kPacket.m_aucRUserID ), KncUtil::toNarrowString( strReceiverLogin ).c_str(), _TRUNCATE );
    kPacket.m_nProductCode = dwItemID;
    strncpy_s( kPacket.m_aucProductName, sizeof( kPacket.m_aucProductName ), KncUtil::toNarrowString( wstrItemName ).c_str(), _TRUNCATE );
    kPacket.m_nAmount = cashInfo.m_dwDecreaseCashPoint;
    kPacket.m_biItemUID = biItemSetUID;

    kEvent.Write( kPacket );

    START_LOG( clog, L"아이템 선물 요청 보냄." )
        << BUILD_LOG( biItemSetUID )
        << BUILD_LOG( dwItemID )
        << BUILD_LOG( nCount )
        << BUILD_LOG( nPeriod ) << END_LOG;

    SendNCashPacket( PT_BILLING, kEvent );

    if( !WaitEvents( PT_BILLING, strSenderLogin ) ) {
        std::wstringstream stm;
        stm << m_dwThreadID << L"|PRESENT_TIME_OUT|" << strSenderLogin
            << L"|"<< strReceiverLogin
            << L"|"<< KNC_TIME_STRING
            << L"|GoodsID:" << dwItemID
            << L"|Count:" << nCount
            << L"|Period:" << nPeriod
            << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
            << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

        SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
    }

    if( m_kResultEvent.GetEventID() == KNCashEvent::EVENT_NCASH_PRESENT_ACK ) {
        KEVENT_NCASH_BUY_ACK kAck;
        m_kResultEvent.Read( kAck );
        nTransactionID = kAck.m_nTransactionID;

        switch( kAck.m_sResult )
        {
        case 100:
            cashInfo.m_nOK = 0;
            cashInfo.m_dwCashPoint -= cashInfo.m_dwDecreaseCashPoint;
            break;
        case -201:
            START_LOG( cerr, L"사용자 ID가 없어서 결제 실패." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -400:
            START_LOG( cerr, L"시스템 에러." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 101;
            break;
        case -500:
            START_LOG( cerr, L"보내는 사람과 받는 사람 동일." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -501:
            START_LOG( cerr, L"보내는 사람의 정보가 잘못되었을 경우." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -502:
            START_LOG( cerr, L"받는 사람의 정보가 잘못되었을 경우." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -503:
            START_LOG( cerr, L"보내는 사람 금액부족." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -504:
            START_LOG( cerr, L"받는 사람 구매 불가능." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -505:
            START_LOG( cerr, L"금액부족." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -506:
            START_LOG( cerr, L"금액부족." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 103;
            break;
        case -600:
            START_LOG( cerr, L"상품정보 없음." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 111;
            break;
        case -700:
            START_LOG( cerr, L"DB에 등록된 가격과 상품 가격이 다름." )
                << BUILD_LOG( strSenderLogin ) << END_LOG;
            cashInfo.m_nOK = 111;
            break;
        default:
            START_LOG( cerr, L"선물하기 실패." )
                << BUILD_LOG( strSenderLogin )
                << BUILD_LOG( kAck.m_sResult ) << END_LOG;
            cashInfo.m_nOK = 110;
            break;
        }

        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|PRESENT_ACK|" << strSenderLogin
                << L"|"<< strReceiverLogin
                << L"|"<< KNC_TIME_STRING
                << L"|RecvOK:"<< kAck.m_sResult
                << L"|OK:" << cashInfo.m_nOK
                << L"|GoodsID:" << dwItemID
                << L"|Count:" << nCount
                << L"|Period:" << nPeriod
                << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
                << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
    else {
        START_LOG( cerr, L"선물하기 요청에 대하여 이상한 패킷이 도착함." )
            << BUILD_LOG( strSenderLogin )
            << BUILD_LOG( m_kResultEvent.GetEventID() )
            << BUILD_LOG( m_kResultEvent.GetEventIDString() )
            << END_LOG;

        cashInfo.m_nOK = 110;
        {
            std::wstringstream stm;
            stm << m_dwThreadID << L"|BUY_ACK_DIFF_ID|" << strSenderLogin
                << L"|"<< strReceiverLogin
                << L"|"<< KNC_TIME_STRING
                << L"|ErrID:"<< m_kResultEvent.GetEventID()
                << L"|GoodsID:" << dwItemID
                << L"|Count:" << nCount
                << L"|Period:" << nPeriod
                << L"|Price:" << cashInfo.m_dwDecreaseCashPoint
                << L"|CurrentCash:" << cashInfo.m_dwCashPoint;

            SiKLogManager()->Get( KLogManager::LT_BILLING_LOG ) << stm.str() << GLOG::endl;
        }
    }
}

int KNCashBilling::CheckVirtualCash( IN KVirtualCashInfo& kVirtualCashInfo_ )
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

void KNCashBilling::DiscountCash( IN KVirtualCashInfo& kVirtualCashInfo_, IN OUT KCashInfo& cashInfo_ )
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
