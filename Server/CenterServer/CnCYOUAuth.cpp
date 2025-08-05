#include "CenterSimLayer.h"
#include "CnCYOUAuth.h"
#include "CenterDBThread.h"
#include "Log4.h"
#include "CenterServer.h"
#include <boost/lexical_cast.hpp>

/*#pragma comment( lib, "../Common/CYOUBilling/PCI" )

#define CLASS_TYPE KCnCYOUAuth
#define MINUTE 60
#define MULTIPLE_POINTS_FACTOR 0.001
#define MAX_SERVER_KEEP_ALIVE_FAIL_COUNT 3

KCnCYOUAuth::KCommonData KCnCYOUAuth::ms_ComnData = 
{
    KncCriticalSection(),
    std::vector< std::pair<std::string, USHORT> >(),
    0
};

USHORT KCnCYOUAuth::m_usPacketSequence = 0;
std::wstring KCnCYOUAuth::ms_wstrFileDSN_forCashDB;
float KCnCYOUAuth::ms_CashExchangeRatio = 1.0f;

KCnCYOUAuth::KCnCYOUAuth(void)
:m_nServerKeepAliveFailCount(0)
{
    CYOUBillingEvent::Init();

    m_dwWaitTime = TIME_OUT_ELAPSED_TIME_LIMIT; // 페이레터 60초, 창유는 몇 초?
    m_dwThreadID = 0;

    InitResultData();

    KLocker lock( ms_ComnData.m_cs );

    for ( int i = 0; i < NUM_PORT_TYPE; i++ ) {
        m_kSock[i].m_pkCYOUBilling = this;
        m_kSock[i].SetKey( i );
        m_kSock[i].m_dwLastHeartBeatTime = ::GetTickCount();
        m_hEvents[i * 2 + 0] = CreateEvent( NULL, false, false, NULL ); // 역할은? IO_RECV
        m_hEvents[i * 2 + 1] = CreateEvent( NULL, false, false, NULL ); // IO_SEND
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[i * 2 + 0] );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[i * 2 + 1] );

        LIF( m_kSock[i].Connect( ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].first.c_str(), ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].second, false ) );

        m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL ); // EndEvent의 역할은?
    }
    DriveConnect_forCashDB();
}

KCnCYOUAuth::~KCnCYOUAuth(void)
{
}

void KCnCYOUAuth::InitResultData()
{
    m_kResultEvent.Reset();
}

bool KCnCYOUAuth::Begin()
{
    return KThread::Begin();
}

void KCnCYOUAuth::Run()
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
                //KeepAliveNot( ( PORT_TYPE )i );
            }
            Loop();
        }

        else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

    }// while
}

bool KCnCYOUAuth::DriveConnect_forCashDB()
{
    return m_kCashODBC.DriverConnect( ms_wstrFileDSN_forCashDB.c_str() );
}

void KCnCYOUAuth::InitFileDSN_CASH_DB( const char* szFileDSN_ )
{
    ms_wstrFileDSN_forCashDB = KncUtil::toWideString( szFileDSN_ );
}

void KCnCYOUAuth::InitCYOUBillingInfo( const char* szIP_, USHORT usPort_, const char* szODBC_, const USHORT usZoneID_, const USHORT usGroupID_, const float fRatio_ )
{
    KCnCYOUAuth::InitFileDSN_CASH_DB( szODBC_ );

    KLocker lock( ms_ComnData.m_cs );
    ms_ComnData.m_vecBillingServerIP_Port.push_back( std::make_pair( szIP_, usPort_ ) );
    ms_ComnData.m_usZoneID = usZoneID_;
    ms_ComnData.m_usGroupID = usGroupID_;
    ms_CashExchangeRatio = fRatio_;
    //ms_ComnData.m_strBillingServerIP       = szIP_;
    //ms_ComnData.m_usPort[PT_BILLING]       = usPort_;
}

void KCnCYOUAuth::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

    switch( kIntEvent_.m_usEventID )
    {
        INT_CASE( ENU_VERIFY_ACCOUNT_REQ );
       _INT_CASE( ENU_SERVER_LOGIN_REQ, KCYOUServerInfo );
       _INT_CASE( ENU_USER_LOGIN_REQ, KCYOUAuthUserInfo );
       _INT_CASE( ENU_USER_LOGOUT_REQ, KCYOUAuthUserInfo );
       _INT_CASE( ENU_SERVER_HEART_BEAT_REQ, KCYOUServerInfo );
       _INT_CASE( ENU_USER_HEARTBEAT_REQ, KCYOUAuthUserInfo );
       _INT_CASE( ENU_CYOU_CHANGE_CASH_REQ, KCYOUCashData );
       _INT_CASE( ENU_CYOU_GET_REMAIN_CASH_REQ, KCYOUCashData );
       _INT_CASE( ENU_CYOU_BUY_LOG_REQ, KCYOUCashData );
       _INT_CASE( ENU_USER_AGREEMENT_REQ, KCYOUAuthUserInfo );

    default:
        START_LOG( cerr, L"Invalid Event ID : " << kIntEvent_.m_usEventID  ) << END_LOG;
        break;
    }
}

void KCnCYOUAuth::KSocketCYOUBilling::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;

    {
        m_pkCYOUBilling->m_kResultEvent.SetInputStream( m_ovlRecv.m_pBuffer );
        m_pkCYOUBilling->m_kResultEvent.SetPacketHeader( ( BYTE* )m_ovlRecv.m_pBuffer );
        m_ovlRecv.m_dwLeft -= ( DWORD )(HEADER_SIZE_OF_CYOUBILLING); // 헤더 추출한 나머지 데이터량

        ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[HEADER_SIZE_OF_CYOUBILLING], m_ovlRecv.m_dwLeft );

        if( m_ovlRecv.m_dwLeft >= ( DWORD )( m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - static_cast<unsigned short>(HEADER_SIZE_OF_CYOUBILLING) ) )
        {
            m_pkCYOUBilling->m_kResultEvent.CopyToBuffer( ( BYTE* )m_ovlRecv.m_pBuffer, HEADER_SIZE_OF_CYOUBILLING, m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING );

            m_ovlRecv.m_dwLeft -= ( DWORD )( m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING );
            // 아래 문장은 루프 돌지 않는 현재 상태에서는 무의미하다.
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[m_pkCYOUBilling->m_kResultEvent.GetPacketSize() - HEADER_SIZE_OF_CYOUBILLING], m_ovlRecv.m_dwLeft );

            SetEvent( m_pkCYOUBilling->m_hEndEvent[m_dwKey] );
        }
    }
    LIF( InitRecv() );
}

void KCnCYOUAuth::KSocketCYOUBilling::OnSocketError()
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

    START_LOG( clog, L"CYOU 빌링 소켓 끊어짐... Thread ID : " << ::GetCurrentThreadId() ) << END_LOG;
    //if ( Connect( NULL, NULL, false ) == false ) 
    {
        if ( ms_ComnData.m_nCurrentIndex < static_cast<int>( ms_ComnData.m_vecBillingServerIP_Port.size()-1 ) ) {
            ++ms_ComnData.m_nCurrentIndex;
        }
        else {
            ms_ComnData.m_nCurrentIndex = 0;
        } // if-else 는 m_nCurrentIndex를 0 부터 size()-1 개 까지 증가시키고, index가 size-1 까지 갔으면 다시 0부터 시작
        // 빌링 서버와 접속이 종료되면, 다음 빌링 서버로 접속을 시도하기 위함.
        LIF( Connect( ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].first.c_str(), ms_ComnData.m_vecBillingServerIP_Port[ms_ComnData.m_nCurrentIndex].second, false ) );
    }
}

void KCnCYOUAuth::Reconnect()
{
    // Event reset.
    ResetEvent( m_hEvents[1] );
    ResetEvent( m_hEvents[0] );
    ResetEvent( m_hEndEvent );
    m_kSock[0].CancelIo();
    m_kSock[0].OnSocketError();

    {
        KCYOUServerInfo kPacket;
        USHORT usRet = 0;
        kPacket.m_usZoneID = ms_ComnData.m_usZoneID;
        kPacket.m_usGroupID = ms_ComnData.m_usGroupID;
        kPacket.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
        SiKCenterSimLayer()->GetBillConnIP( kPacket.m_strIP );

        if ( true == ServerLoginRequest( kPacket, usRet ) ) {
            START_LOG( cerr, L"CYOU Billing 재연결 성공" ) << END_LOG;
        }
        else {
            START_LOG( cerr, L"CYOU Billing 재연결 실패" ) << END_LOG;
        }
    }
}

bool KCnCYOUAuth::SendCYOUPacket( const PORT_TYPE ePortType_, const char* kBuff_, const int nSize_ )
{
    START_LOG( clog, L"CYOU Billing Thread ID : " << m_dwThreadID ) << END_LOG;
    START_LOG( clog, L"CYOU 빌링 서버에 패킷 보냄." ) 
        << BUILD_LOG( nSize_ )
        << END_LOG;

    InitResultData();

    ResetEvent( m_hEndEvent[ePortType_] );

    m_kSock[ePortType_].m_dwLastHeartBeatTime = ::GetTickCount();
    _JIF( m_kSock[ePortType_].SendData( kBuff_, nSize_ ), return false );

    ++KCnCYOUAuth::m_usPacketSequence;

    return true;
}

bool KCnCYOUAuth::WaitEvents( IN const PORT_TYPE ePortType_, IN const std::wstring& strLogin_ )
{
    DWORD dwRet;
    DWORD dwTotalElapsedTime = ::GetTickCount();
    do
    {
        do
        {
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
            // ePortType_ * 2 + 1의 의미 ->  IO_SEND  // m_hEvents[i * 2 + 1]
            if ( dwRet == WAIT_TIMEOUT ) {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );
        } while ( m_kSock[ePortType_].IsSending() ); //send가 완료될 때까지 돈다.

        if ( dwRet == WAIT_TIMEOUT ) break;

        do
        {  // 071108. woosh.   m_hEvents[i * 2 + 0] : IO_RECV
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 ], m_dwWaitTime );
            if ( dwRet == WAIT_TIMEOUT ) {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_RECV );

            dwRet = WaitForSingleObject( m_hEndEvent[ ePortType_ ], 1 );

            if ( dwRet == WAIT_OBJECT_0 ) break;

        } while( dwRet == WAIT_TIMEOUT );

        if ( dwRet == WAIT_TIMEOUT ) break;

        if ( ::GetTickCount() - dwTotalElapsedTime > TIME_OUT_ELAPSED_TIME_LIMIT ) {
            dwRet = WAIT_TIMEOUT;
            break;
        }

    }while(false);

    dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

    if ( dwRet == WAIT_TIMEOUT ) { // 시간 초과
        START_LOG( cerr, L"Event Processing - time over. elapsed : " << dwTotalElapsedTime )
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

        return false;
    }
    return true;
}

bool KCnCYOUAuth::WaitSend( IN const PORT_TYPE ePortType_, IN const std::wstring& strEvent_ )
{
    DWORD dwRet;
    DWORD dwTotalElapsedTime = ::GetTickCount();

    do 
    {
        do
        {
            dwRet = WaitForSingleObject( m_hEvents[ ePortType_ * 2 + 1 ], m_dwWaitTime );
            if ( dwRet == WAIT_TIMEOUT ) {
                break;
            }
            m_kSock[ePortType_].OnIOCompleted( KOVERLAPPED::IO_SEND );

        } while( m_kSock[ePortType_].IsSending() ); //send 완료할 때까지 도세요

        if ( dwRet  == WAIT_TIMEOUT ) break;

    } while(false);

    dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

    if ( dwRet == WAIT_TIMEOUT ) {// 시간 초과

        START_LOG( cerr, L"Event processing - time over. elapsed : " << dwTotalElapsedTime )
            << BUILD_LOG( strEvent_ )
            << END_LOG;

        return false;
    }
    return true;
}

INT_IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ )
{
    KCYOUAuthUserInfo kCYOUPacket;
    kCYOUPacket.m_strLogin = kPacket_.m_strLogin;
    kCYOUPacket.m_strPassword = kPacket_.m_strPasswd;
    kCYOUPacket.m_strMACAddress = kPacket_.m_strMACMD5;
    kCYOUPacket.m_strPC = kPacket_.m_strPC;

    kCYOUPacket.m_strIP = KncUtil::toNarrowString( kPacket_.m_strIP );
    SiKCenterSimLayer()->GetBillConnIP( kCYOUPacket.m_strServerIP );
    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST ) ) {
        kCYOUPacket.m_strServerIP = "14.45.79.140";
    }
    kCYOUPacket.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;

    UserAuthRequest( kCYOUPacket, kPacket_ );

    START_LOG( cwarn, L"중국 유저의 온라인 누적 시간 : " << KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( kPacket_.m_nAccOnlineTime )
        << BUILD_LOGc( kPacket_.m_cFatigueState )
        << END_LOG;

    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, 0, kPacket_ );
}

_INT_IMPL_ON_FUNC( ENU_SERVER_LOGIN_REQ, KCYOUServerInfo )
{
    USHORT usRet = 0;
    kPacket_.m_usZoneID = ms_ComnData.m_usZoneID;
    kPacket_.m_usGroupID = ms_ComnData.m_usGroupID;
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;

    ServerLoginRequest( kPacket_, usRet );

    if ( usRet != 0 ) { // 빌링 서버에 정상 접속 안되면 게임서버를 종료한다. TODO : 0은 정상이 아닐거야
        SiKCenterServer()->ShutDown(); 
    }

    // A0 전송 후에 A0를 보낸다. 패킷을 한 번 보냈기 때문에 시퀀스가 증가해서 다시 넣어준다.
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    ServerRestart( kPacket_ );

    START_LOG( clog, L"중국 창유 빌링서버 로그인" )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strIP ) )
        << BUILD_LOG( kPacket_.m_usZoneID )
        << BUILD_LOG( kPacket_.m_usGroupID )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( ENU_USER_LOGIN_REQ, KCYOUAuthUserInfo )
{
    KECN_CYOU_USER_LOGIN_ACK kPacket;

    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );

    UserLogin( kPacket_, kPacket );

    QUEUEING_ACK_TO_CENTER( ECN_CYOU_USER_LOGIN_ACK );
}

_INT_IMPL_ON_FUNC( ENU_USER_LOGOUT_REQ, KCYOUAuthUserInfo )
{
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    UserLogout( kPacket_ );

    START_LOG( clog, L"CYOU Logout data : " << kPacket_.m_dwUID )
        << BUILD_LOGc( kPacket_.m_Character )
        << BUILD_LOG( kPacket_.m_usCharacterLevel )
        << END_LOG;
}

_INT_IMPL_ON_FUNC( ENU_SERVER_HEART_BEAT_REQ, KCYOUServerInfo )
{
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    kPacket_.m_usZoneID = ms_ComnData.m_usZoneID;
    kPacket_.m_usGroupID = ms_ComnData.m_usGroupID;

    if( ServerKeepAliveRequest( kPacket_ ) == false ) {
        if ( m_nServerKeepAliveFailCount > MAX_SERVER_KEEP_ALIVE_FAIL_COUNT ) {
            Reconnect(); // keep alive 가 4회 실패하면 접속을 새로 한다.
            m_nServerKeepAliveFailCount = 0;            
        }
        else {
            ++m_nServerKeepAliveFailCount;
        }        
    }
}

_INT_IMPL_ON_FUNC( ENU_USER_HEARTBEAT_REQ, KCYOUAuthUserInfo )
{ // 게임서버에서는 로긴서버로 ECN_CYOU_USER_HEART_BEAT_REQ 를 보낸다.
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );
    KECN_CYOU_USER_HEART_BEAT_ACK kPacket;
    kPacket.m_dwUID = kPacket_.m_dwUID;

    UserHeartBeat( kPacket_, kPacket );

    if ( kPacket.m_nOK == NetError::ERR_NUSER_VERIFY_62 ) {
        UserHeartBeat( kPacket_, kPacket ); // 0x06 일 때 패킷을 한 번 더 전송.
    }

    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST ) ) {
        //kPacket.m_nOK = NetError::ERR_NUSER_VERIFY_60;
    }
    if ( SiKCenterSimLayer()->m_nUserHeartBeatError != 0 ) {
        kPacket.m_nOK = SiKCenterSimLayer()->m_nUserHeartBeatError;
    }
    
    if ( kPacket.m_nOK != 0 ) {
        QUEUEING_ACK_TO_CENTER( ECN_CYOU_USER_HEART_BEAT_ACK );
    }
}

_INT_IMPL_ON_FUNC( ENU_CYOU_CHANGE_CASH_REQ, KCYOUCashData )
{
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;
    kPacket_.m_strSerialNumber = 
        boost::lexical_cast<std::string>( ms_ComnData.m_usZoneID * 1000 + ms_ComnData.m_usGroupID ) 
        + boost::lexical_cast<std::string>( kPacket_.m_usServerID )
        + kPacket_.m_strSerialNumber;

    KCYOUCashPoint kPacket;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

    float fCyouPoint = kPacket_.m_nCashPoint * ms_CashExchangeRatio;

    if ( fCyouPoint > 9999000.0f ) {
        SET_ERR_GOTO( ERR_CASH_BILL_06, END_PROC );
    }

    kPacket_.m_strItemInfo = boost::lexical_cast<std::string>(REFILL_ITEMID) + ","
        + boost::lexical_cast<std::string>(kPacket_.m_nCashPoint) + ","
        + boost::lexical_cast<std::string>(fCyouPoint) + ";";

    ChangeCash( kPacket_, kPacket );

    if ( kPacket.m_nOK == 0 ) {
        UpdateCash( kPacket.m_dwUserUID, REFILL_CASH, kPacket_.m_nCashPoint );
        GetCurrentCash( kPacket.m_dwUserUID, kPacket.m_dwCashPoint );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, NetError::NET_OK );
        ERR_CASE( ERR_NUSER_VERIFY_67, kPacket.m_nOK, NetError::ERR_NUSER_VERIFY_67 );
        ERR_CASE( ERR_CASH_BILL_03, kPacket.m_nOK, NetError::ERR_CASH_BILL_03 );
        ERR_CASE( ERR_CASH_BILL_05, kPacket.m_nOK, NetError::ERR_CASH_BILL_05 );
        ERR_CASE( ERR_CASH_BILL_06, kPacket.m_nOK, NetError::ERR_CASH_BILL_06 );
    default:
        START_LOG( cerr, L"창유 포인트 전환시에 알 수 없는 에러, UserUID : " << kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket.m_nOK )
            << END_LOG;
    }
    QUEUEING_ACK_TO_CENTER( ECN_CYOU_CHANGE_CASH_ACK );

    START_LOG( clog, L"중국 게임내 캐쉬로 교환"  )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strSerialNumber ) )
        << BUILD_LOG( kPacket.m_dwCYOUPoint )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( true == SiKCenterSimLayer()->GetCYOUBillLogEnable() ) {
        KCYOUChangeCashLogData kCYOUChangeCashLogData;
        kCYOUChangeCashLogData.m_nOK                = kPacket.m_nOK;
        kCYOUChangeCashLogData.m_strLogin           = kPacket_.m_strLogin;
        kCYOUChangeCashLogData.m_dwUserUID          = kPacket_.m_dwUserUID;
        kCYOUChangeCashLogData.m_strCharNickname    = kPacket_.m_strCharNickname;
        kCYOUChangeCashLogData.m_dwCashPoint        = kPacket.m_dwCashPoint;
        kCYOUChangeCashLogData.m_dwCYOUPoint        = kPacket.m_dwCYOUPoint;
        kCYOUChangeCashLogData.m_usPacketSequence   = kPacket_.m_usPacketSequence;
        kCYOUChangeCashLogData.m_strServerIP        = kPacket_.m_strServerIP;
        kCYOUChangeCashLogData.m_strUserIP          = kPacket_.m_strUserIP;
        kCYOUChangeCashLogData.m_strSerialNumber    = kPacket_.m_strSerialNumber;
        kCYOUChangeCashLogData.m_strItemInfo        = kPacket_.m_strItemInfo;

        _QUEUEING_ACK_TO_CENTER( ECN_CYOU_CHANGE_CASH_LOG_NOT, kCYOUChangeCashLogData );
    }
}

_INT_IMPL_ON_FUNC( ENU_CYOU_GET_REMAIN_CASH_REQ, KCYOUCashData )
{

    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;

    KCYOUCashPoint kPacket;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

    GetRemainPoint( kPacket_, kPacket );

    QUEUEING_ACK_TO_CENTER( ECN_CYOU_GET_CURRENT_CASH_ACK );
}

_INT_IMPL_ON_FUNC( ENU_CYOU_BUY_LOG_REQ, KCYOUCashData )
{
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;

    BuyLog( kPacket_ );
}

_INT_IMPL_ON_FUNC( ENU_USER_AGREEMENT_REQ, KCYOUAuthUserInfo )
{
    kPacket_.m_usPacketSequence = KCnCYOUAuth::m_usPacketSequence;

    AgreementConfirm( kPacket_ );
}

void KCnCYOUAuth::UserAuthRequest( IN KCYOUAuthUserInfo& kUserInfo_, OUT KENU_VERIFY_ACCOUNT_REQ& kAck_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));
    int nSize = 0;
    if ( false == kEvent.SetPacketAuthUserRequest( kUserInfo_, buf, nSize ) ) {
        kAck_.m_nOK = NetError::ERR_NUSER_VERIFY_36;
        START_LOG( clog, L"UserAuthRequest SetPacketAuthUserRequest()" )
            << END_LOG;
        return;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"UserAuthRequest SendCYOUPacket()" )
            << END_LOG;
        kAck_.m_nOK = NetError::ERR_NUSER_VERIFY_32;

        return;
    }

    if ( false == WaitEvents( PT_BILLING, KncUtil::toWideString( kUserInfo_.m_strLogin ) ) ) {
        kAck_.m_nOK = NetError::ERR_NUSER_VERIFY_32;

        return;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETAUTHUSER ) {
        Packet* pPacket = NULL;
        BLAuthUserRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        int nOnlineTime = packet.GetOnlineTime();
        int nOfflineTime = packet.GetOutlintTime();
        char cIsForbidden = packet.GetForbiddenInf();
        char cForbiddenLevel = packet.GetFCMStatus();
        char cRealNameBind = packet.GetRealNameBindInf();
        char cRealNameAuth = packet.GetBeAuthedInf();
        char cLoginType = packet.GetEType();

        char* strUserAccount = new char[PCI_MAX_CN_LEN];
        strUserAccount = packet.GetCN(); // 인증을 제외하고 전부 CN(유저 계정)을 사용한다고 기억하면 편하다
        char* strLoginAccount = new char[PCI_MAX_CN_LEN];
        strLoginAccount = packet.GetEN();

        kAck_.m_strPasswd = kAck_.m_strLogin; // passwd에 로그인 계정을 저장
        kAck_.m_strLogin = strUserAccount; // login에 유저 계정을 저장
        kAck_.m_strBillingAccount = strUserAccount;

        if ( cIsForbidden == 'Y' ) { // 몰입방지 해당 유저 ( 미성년 )
            kAck_.m_cFatigueState = KENU_VERIFY_ACCOUNT_ACK::FUT_AUTH_CHILDREN;
            kAck_.m_nAccOnlineTime = nOnlineTime / MINUTE; // 중국에서 초 단위로 준다.
        }
        else {
            kAck_.m_cFatigueState = KENU_VERIFY_ACCOUNT_ACK::FUT_AUTH_ADULT;
            kAck_.m_nAccOnlineTime = -1;
        }
        kAck_.m_cFatigueLevel = cForbiddenLevel;
        kAck_.m_cIsRealNameAuth = cRealNameAuth;
        kAck_.m_cIsRealName = cRealNameBind;

        UCHAR ucRet = packet.GetAuthRet();

        START_LOG( clog, L"GetAuthRet , USER :" << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
            << BUILD_LOGc( ucRet )
            << BUILD_LOGc( cLoginType )
            << END_LOG;

        switch( ucRet ) {
            case 0x01:
                SET_ERROR( NET_OK );

                if ( cIsForbidden == 'Y' && cRealNameBind == 'N' && cForbiddenLevel != 0x01 ) {
                    SET_ERROR( ERR_NUSER_VERIFY_65 );
                }
                else if ( cIsForbidden == 'Y' && cRealNameBind == 'Y' && cRealNameAuth == 'F' && cForbiddenLevel == 0x03 ) {
                    SET_ERROR( ERR_NUSER_VERIFY_66 );
                }

                break;
            case 0x02:
                SET_ERROR( ERR_NUSER_VERIFY_11 );

                break;
            case 0x03:
                SET_ERROR( ERR_NUSER_VERIFY_10 );
                START_LOG( cerr, L"Wrong password , User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
            case 0x04:
                SET_ERROR( ERR_NUSER_VERIFY_02 );
                START_LOG( cerr, L"Double access, User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
            case 0x05:
                SET_ERROR( ERR_NUSER_VERIFY_63 ); // 다른 서버군에 이미 접속한 계정. 메시지 팝업 필요.

                break;
            case 0x06:
                SET_ERROR( ERR_NUSER_VERIFY_38 );

                break;
            case 0x07:
                SET_ERROR( ERR_NUSER_VERIFY_64 ); // 블럭 된 유저. ( 불법 조작으로 인해 정지된 계정. )
                START_LOG( cerr, L"blocked account, User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
            case 0xA0:
                SET_ERROR( ERR_NUSER_VERIFY_34 ); // 비활성화 계정

                break;
            case 0xA1:
                SET_ERROR( ERR_NUSER_VERIFY_39 ); // 미가입 계정

                break;
            case 0xB1:
                SET_ERROR( ERR_NUSER_VERIFY_40 ); // 고정(유저가 등록한) IP가 아닙니다

                break;
            case 0xB2:
                SET_ERROR( ERR_NUSER_VERIFY_41 ); // 휴대폰 등록 계정이라 로그인 불가.

                break;
            case 0xC1:
                SET_ERROR( ERR_NUSER_VERIFY_42 ); // 보안카드 미신청. 다시 로그인 필요.

                break;
            case 0xC2:
                SET_ERROR( ERR_NUSER_VERIFY_43 ); // 보안카드 정보 입력하고 다시 로그인

                break;
            case 0xC3:
                SET_ERROR( ERR_NUSER_VERIFY_44 ); // 보안카드 비밀번호 틀림

                break;
            case 0xC7:
                SET_ERROR( ERR_NUSER_VERIFY_35 );
                START_LOG( cerr, L"Not real name user, User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
            case 0xD1:
                SET_ERROR( ERR_NUSER_VERIFY_45 ); // Mac 값 검사 실패, 비고정 Mac 경로

                break;
            case 0xE1:
                SET_ERROR( ERR_NUSER_VERIFY_46 ); // 중국 같은 신분증으로 다른 계정이  이미 로그인 된 상태

                break;
            case 0xF1:
                SET_ERROR( ERR_NUSER_VERIFY_37 );
                START_LOG( cerr, L"User didn't not agree CYOU policy, User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
            case 0x31:
                SET_ERROR( ERR_NUSER_VERIFY_47 );

                break;
            case 0x32:
                SET_ERROR( ERR_NUSER_VERIFY_48 );

                break;
            case 0x33:
                SET_ERROR( ERR_NUSER_VERIFY_49 );

                break;
            case 0x34:
                SET_ERROR( ERR_NUSER_VERIFY_50 );

                break;
            case 0x35:
                SET_ERROR( ERR_NUSER_VERIFY_51 );

                break;
            case 0x36:
                SET_ERROR( ERR_NUSER_VERIFY_52 );

                break;
            case 0x37:
                SET_ERROR( ERR_NUSER_VERIFY_53 );

                break;
            case 0x39:
                SET_ERROR( ERR_NUSER_VERIFY_54 );

                break;
            case 0x40:
                SET_ERROR( ERR_NUSER_VERIFY_55 );

                break;
            default:
                SET_ERROR( ERR_NUSER_VERIFY_38 );
                START_LOG( cerr, L"CYOU ERROR return, User : " << KncUtil::toWideString( kUserInfo_.m_strLogin ) )
                    << BUILD_LOGc( ucRet )
                    << END_LOG;

                break;
        }
    }
    else {
        SET_ERROR( ERR_NUSER_VERIFY_32 );
        Packet* pPacket = NULL;
        BLAuthUserRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        char* strUserAccount = new char[PCI_MAX_CN_LEN];
        strUserAccount = packet.GetCN(); // 인증을 제외하고 전부 CN(유저 계정)을 사용한다고 기억하면 편하다
        char* strLoginAccount = new char[PCI_MAX_CN_LEN];
        strLoginAccount = packet.GetEN();

        std::string strUser = strUserAccount;
        std::string strLogin = strLoginAccount;
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETAUTHUSER가 아님" )
            << BUILD_LOG( dwPacketNum )
            << BUILD_LOG( KncUtil::toWideString( strUser ) )
            << BUILD_LOG( KncUtil::toWideString( strLogin ) )
            << END_LOG;
    }

    kAck_.m_nOK = NetError::GetLastNetError();
}

bool KCnCYOUAuth::ServerLoginRequest( IN const KCYOUServerInfo& kServerInfo_, OUT USHORT& nRet_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketLoginServerConnection( kServerInfo_, buf, nSize ) ) {
        START_LOG( clog, L"ServerLoginRequest() SetPacketLoginServerConnection()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        nRet_ = NetError::ERR_NUSER_VERIFY_67;

        START_LOG( clog, L"ServerLoginRequest() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETCONNECT ) {
        Packet* pPacket = NULL;
        BLConnectRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        nRet_ = packet.GetReturn();

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETCONNECT가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::ServerRestart( IN const KCYOUServerInfo& kServerInfo_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketServerRestart( kServerInfo_, buf, nSize ) ) {
        START_LOG( clog, L"ServerRestart() SetPacketServerRestart()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"ServerRestart() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETKICKALL ) {
        Packet* pPacket = NULL;
        BLServRestartRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        USHORT ret;
        ret = packet.GetRet();

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETKICKALL가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}


bool KCnCYOUAuth::UserLogin( IN const KCYOUAuthUserInfo& kUserInfo_, OUT KECN_CYOU_USER_LOGIN_ACK& kPacket_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketLoginRequest( kUserInfo_, buf, nSize ) ) {
        START_LOG( clog, L"UserLogin SetPacketLoginRequest()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_67;

        START_LOG( clog, L"UserLogin SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETBILLSTART ) {
        Packet* pPacket = NULL;
        BLBillStartRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        USHORT ret;
        ret = packet.GetRet();

        switch ( ret )
        {
        case 0x00:
            SET_ERROR( NET_OK );
            break;
        case 0x01:
            SET_ERROR( ERR_NUSER_VERIFY_56 ); // 유저 로그인 실패
            break;
        case 0x03:
            SET_ERROR( ERR_NUSER_VERIFY_57 ); // 게임시작과 로그인을 다른 서버에서 함. (우리 게임에서는 발생 하지 않을 듯 )
            break;
        case 0x04:
            SET_ERROR( ERR_NUSER_VERIFY_58 ); // 이미 온라인 상태의 계정
            break;
        case 0x05:
        case 0x06:
            SET_ERROR( ERR_NUSER_VERIFY_59 ); // 기타 에러
            break;
        default:
            SET_ERROR( ERR_UNKNOWN );
        }

        delete [] strIP;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETBILLSTART가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;
    }

    kPacket_.m_dwUID = kUserInfo_.m_dwUID;
    kPacket_.m_nOK = NetError::GetLastNetError();

    return true;
}

bool KCnCYOUAuth::UserLogout( IN const KCYOUAuthUserInfo& kUserInfo_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketLogoutRequest( kUserInfo_, buf, nSize ) ) {
        START_LOG( clog, L"UserLogout() SetPacketLogoutRequest()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"UserLogout() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETBILLEND ) {
        Packet* pPacket = NULL;
        BLBillEndRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        USHORT ret;
        ret = packet.GetRet();

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETBILLEND가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::ServerKeepAliveRequest( IN const KCYOUServerInfo& kServerInfo_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    LBKeepAlive kPacket;
    int nSize = 0;
    if ( false == kEvent.SetPacketServerKeepAlive( kServerInfo_, buf, nSize ) ) {
        START_LOG( clog, L"ServerKeepAliveRequest() SetPacketServerKeepAlive()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"ServerKeepAliveRequest() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        START_LOG( cerr, L"ServerKeepAlive Ack Failed. " )
            << BUILD_LOG( KncUtil::toWideString( kServerInfo_.m_strIP ) )
            << END_LOG;

        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETKEEPLIVE ) {
        Packet* pPacket = NULL;
        BLKeepAliveRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];

        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        delete [] strIP;

        USHORT ret;
        ret = packet.GetRet();

        if ( ret != 0 ) {
            return false;
        }
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETKEEPLIVE가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
    return true;
}

bool KCnCYOUAuth::UserHeartBeat( IN const KCYOUAuthUserInfo& kUserInfo_, OUT KECN_CYOU_USER_HEART_BEAT_ACK& kPacket_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));
    kPacket_.m_dwUID = kUserInfo_.m_dwUID;

    int nSize = 0;
    if ( false == kEvent.SetPacketUserHeartbeat( kUserInfo_, buf, nSize ) ) {
        kPacket_.m_nOK = NetError::ERR_UNKNOWN;
        START_LOG( clog, L"UserHeartBeat() SetPacketUserHeartbeat()" ) << END_LOG;
        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_67;
        START_LOG( clog, L"UserHeartBeat() SendCYOUPacket()" ) << END_LOG;
        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        kPacket_.m_nOK = NetError::ERR_UNKNOWN;
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETBILLKEEPING ) {
        Packet* pPacket = NULL;
        BLAutoBillRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        USHORT ret;
        ret = packet.GetAuthRet();

        switch ( ret )
        {
        case 0x00:
            SET_ERROR( NET_OK );
            break;
        case 0x02:
            SET_ERROR( ERR_NUSER_VERIFY_60 );
            break;
        case 0x03:
            SET_ERROR( ERR_NUSER_VERIFY_68 );
        case 0x04:
            SET_ERROR( ERR_NUSER_VERIFY_61 );
            break;
        case 0x06:
            SET_ERROR( ERR_NUSER_VERIFY_62 );
            break;
        default:
            SET_ERROR( ERR_UNKNOWN );
        }

        delete [] strIP;

        kPacket_.m_nOK = NetError::GetLastNetError();
        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        kPacket_.m_nOK = NetError::ERR_UNKNOWN;
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETBILLKEEPING가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::ChangeCash( IN const KCYOUCashData& kCashInfo_, OUT KCYOUCashPoint& kResult_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketChangeCash( kCashInfo_, buf, nSize ) ) {
        START_LOG( clog, L"ChangeCash() SetPacketChangeCash()" ) << END_LOG;

        return false;
    }

    // TODO : 빌링 패킷  전송 실패를 의미하는 ok 값을 정의해야 한다.(공통으로 사용할 수 있는 것으로.)
    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        kResult_.m_nOK = NetError::ERR_NUSER_VERIFY_67;

        START_LOG( clog, L"ChangeCash() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETEXCHANGEYB ) {
        Packet* pPacket = NULL;
        BLExchangeYBRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];
        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );
        pPacket->Read( iStream );

        UCHAR ret;
        ret = packet.GetResult();
        kResult_.m_nOK = static_cast<int>(ret);

        kResult_.m_dwCYOUPoint = packet.GetPoint();

        switch( kResult_.m_nOK )
        {
            case 0x00:
                SET_ERROR( NET_OK );
                break;
            case 0x01:
                break;
            case 0x02:
                SET_ERROR( ERR_CASH_BILL_03 );
                break;
            case 0x09:
                SET_ERROR( ERR_CASH_BILL_05 );
            default:
                break;
        }

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETEXCHANGEYB가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::GetRemainPoint( IN const KCYOUCashData& kCashInfo_, OUT KCYOUCashPoint& kResult_ ) // get web point ( before changed money )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketRemainPoint( kCashInfo_, buf, nSize ) ) {
        kResult_.m_nOK = NetError::ERR_NUSER_VERIFY_36;

        START_LOG( clog, L"GetRemainPoint() SetPacketRemainPoint()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        kResult_.m_nOK = NetError::ERR_NUSER_VERIFY_67;

        START_LOG( clog, L"GetRemainPoint() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETCNPOINT ) {
        Packet* pPacket = NULL;
        BLCnPointRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];

        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP );

        pPacket->Read( iStream );

        UCHAR ret;
        ret = packet.GetRet();

        UINT uiPoint = packet.GetPoint();

        kResult_.m_nOK = static_cast<int>( ret );
        kResult_.m_dwCYOUPoint = static_cast<DWORD>( uiPoint );
        // 중국 빌링 문서의 E2 설명 참조. 1000 포인트를 받으면  1로 만듦. 나누기 1000

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETCNPOINT가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::BuyLog( IN const KCYOUCashData& kCashInfo_ ) 
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketBuyLog( kCashInfo_, buf, nSize ) ) {
        START_LOG( clog, L"BuyLog() SetPacketBuyLog()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"BuyLog() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETBUYLOG ) {
        Packet* pPacket = NULL;
        BLServBuyLogRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];

        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP ); 
        pPacket->Read( iStream );

        USHORT ret;
        ret = packet.GetRet();
        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETBUYLOG가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::AgreementConfirm( IN const KCYOUAuthUserInfo& kUserInfo_ )
{
    CYOUBillingEvent kEvent;

    unsigned char buf[MAX_PACKET_SIZE_OF_CYOUBILLING];
    memset(buf, 0, sizeof(buf));

    int nSize = 0;
    if ( false == kEvent.SetPacketAgreement( kUserInfo_, buf, nSize ) ) {
        START_LOG( clog, L"AgreementConfirm() SetPacketAgreement()" ) << END_LOG;

        return false;
    }

    if ( false == SendCYOUPacket( PT_BILLING, (char*)buf, nSize ) ) {
        START_LOG( clog, L"AgreementConfirm() SendCYOUPacket()" ) << END_LOG;

        return false;
    }

    if ( false == WaitEvents( PT_BILLING, L"" ) ) {
        return false;
    }

    if ( m_kResultEvent.GetEventID() == PACKET_BL_RETCONFIRMPROTOCOL ) {
        Packet* pPacket = NULL;
        BLConfirmProtocolRet packet;
        pPacket = &packet;

        InputStream iStream = InputStream( (char*)m_kResultEvent.GetBuffer() );

        UCHAR ucIPLength = 0;
        iStream.Read( ucIPLength );
        char* strIP = new char[GAMESERVER_IP];

        iStream.Read( strIP, ucIPLength );

        pPacket->SetServerIP( strIP ); 
        pPacket->Read( iStream );

        UCHAR ret;
        ret = packet.GetAuthRet();

        delete [] strIP;

        return true;
    }
    else {
        DWORD dwPacketNum = m_kResultEvent.GetEventID();
        START_LOG( clog, L"m_kResultEvent.GetEventID() == PACKET_BL_RETCONFIRMPROTOCOL가 아님" )
            << BUILD_LOG( dwPacketNum ) << END_LOG;

        return false;
    }
}

bool KCnCYOUAuth::UpdateCash( IN const DWORD dwUserUID_, IN const int nUseType_, IN const int nCash_, IN const GCITEMUID waitUID_ ) // 충전, 사용, 환불
{
    //    충전, 사용, 환불
    //
    //두 번째 인자에 대한 간략 설명입니다
    //충전: 유저가 캐쉬를 충전할 경우
    //사용: 유저가 캐쉬를 이용하여 아이템을 구매할 경우
    //환불: 사용시 에러가 발생했을 경우
    //
    //세 번째 인자는 유저가 캐쉬를 사용시 어떠한 아이템을 구매했는지에 대한 기록을 남기기 위한 용도입니다
    //충전과 환불에서는 사용하지 않기 때문에 0값을 넣어주시고 사용일 경우에는 구매한 아이템의 대기UID값을 넣어주세요
    //
    //네 번째 인자는 사용될 캐쉬의 금액 입니다
    //

    /*
    L"{ call dbo.UCGAUserCashInfo_merge_20121109 ( %d, %I64d, %d, %d ) }"

    { call dbo.UCGAUserCashInfo_merge_20121109 ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_input       int
    @2: @iType_input           int
        1:충전
        2:사용
        3:환불
    @3: @iWIGAUID_input        bigint
        대기 아이템의 UID
        @2가 2인 경우에만 0보다 큰 값을 가지며 1, 3인 경우에는 0을 넣어준다
    @4: @iCashPoint_diff_input int
        충전/환불이든 사용이든 0보다 큰 값으로 넣어야 함
        사용일 경우 내부에서 음수값으로 변경하여 저장

    1 return ( @1 )
    @1: OK int
        0        : 성공
        -1       : 없는 타입
        -2       : 입력된 캐쉬 포인트가 0보다 작음
        -3       : 보유 금액보다 더 많은 금액을 사용하려 했음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    *//*

    _DBJIF( m_kCashODBC.Query( L"{ call dbo.UCGAUserCashInfo_merge_20121109 ( %d, %d, %I64d, %d ) }", 
        dwUserUID_, nUseType_, waitUID_ ,nCash_), return false, ( &m_kCashODBC ) );

    int nOK = -99;

    if ( m_kCashODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kCashODBC >> nOK );
    }
    m_kCashODBC.EndFetch();

    //LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    if ( -899 <= nOK && -801 >= nOK ) {
        nOK = 0;
    }

    return ( 0 == nOK );
}

void KCnCYOUAuth::GetCurrentCash( IN const DWORD dwUserUID_, OUT DWORD& nPoint_ )
{
    //보유 금액 조회

    //유저 존재 유무와는 상관없이 최초 데이터가 없을 경우라도 0을 리턴 합니다
    //보유하고 있다면 그 값이 리턴 됩니다

    /*
    L"{ call dbo.UCGAUserCashInfo_select_20121109 ( %d ) }"

    { call dbo.UCGAUserCashInfo_select_20121109 ( @1 ) }
    @1: @iLoginUID_input int

    1 select ( @1 )
    @1: CashPoint int
    *//*

    _DBJIF( m_kCashODBC.Query( L"{ call dbo.UCGAUserCashInfo_select_20121109 ( %d ) }", dwUserUID_ ), return, (&m_kCashODBC) );

    if ( m_kCashODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kCashODBC >> nPoint_ );
    }
    m_kCashODBC.EndFetch();
}
*/