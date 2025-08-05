#include "MatchProxy.h"
#include "GSSimLayer.h"
#include "Socket/NetCommon.h"
#include "NetError.h"
#include "GameServer.h"
#include "GSNetLayer.h"
#include "UserEvent.h"
#include "User.h"
#include "Worker.h"
#include "Log4.h"
#include "PVPMatchManager.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMatchProxy, KActorProxy );

#define EVENT_TYPE KMatchEvent
#define CLASS_TYPE KMatchProxy

KMatchProxy::KMatchProxy(void)
:m_bAfterVerify(false)
{
    m_hVerifyEvent = ::CreateEvent( NULL, false, false, NULL );
    m_nSavedResult = NetError::NET_OK;
    SetSendQueueSize( 1024 * 512 );
}

KMatchProxy::~KMatchProxy(void)
{
    OnSocketError();
    ::CloseHandle( m_hVerifyEvent );
}

ImplOstmOperatorW2A( KMatchProxy );
ImplToStringW( KMatchProxy )
{
    return stm_;
}

void KMatchProxy::OnDestroy()
{
    KActorProxy::OnDestroy();

    int nProxyID = GetProxyID();

    SiKPVPMatchManager()->InitModeCount(); // 매칭 리스트 초기화.

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_DISCONNECT_MATCHSERVER_NOT, nProxyID );

    START_LOG( clog, L"매칭서버와의 연결 끊김" )
        << END_LOG;
}
void KMatchProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer     ks;
    KSerBuffer      kbuff;

    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
        CASE( EVENT_MATCH_VERIFY_ACCOUNT_ACK );
        CASE( EVENT_MATCH_ADD_USER_ACK );
        CASE( EVENT_MATCH_DEL_USER_ACK );
        CASE( EVENT_MATCH_FIND_MATCH_NOT );
        // #END_OF_CASES
        // code:#END_OF_FILE

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

bool KMatchProxy::Connect( IN const int& nSID_, IN std::string strIP_, IN unsigned short usPort_ )
{
    if ( IsConnected() ) {
        START_LOG( clog, L"이미 연결되어 있습니다." ) << END_LOG;
        return true;
    }

    if( !KActorProxy::Connect( strIP_.c_str(), usPort_, true ) )
        return false;

    // ProxyID Set.
    KActorProxy::SetProxyID( nSID_ );

    KSimpleServerInfo kPacket;
    kPacket.m_dwUID = SiKGameServer()->GetUID();
    kPacket.m_strIP = NetCommon::GetLocalIPW();
    kPacket.m_usPort = SiKGSNetLayer()->m_usPortMaster;
    kPacket.m_strName = SiKGameServer()->GetServerName();
    kPacket.m_nProtocolVer = SiKGameServer()->m_nProtocolVer;

    START_LOG( clog, L"Send EVENT_MATCH_VERIFY_ACCOUNT_REQ" )
        << BUILD_LOG( kPacket.m_dwUID )
        << BUILD_LOG( kPacket.m_strIP )
        << BUILD_LOG( kPacket.m_usPort )
        << BUILD_LOG( kPacket.m_strName )
        << BUILD_LOG( kPacket.m_nProtocolVer ) << END_LOG;

    _JIF( SendPacket( KMatchEvent::EVENT_MATCH_VERIFY_ACCOUNT_REQ, kPacket ), return false );

    DWORD dwElapsTime = ::GetTickCount();

    while ( GetTickCount() - dwElapsTime < 5000 ) {

        DWORD dwReturn  = WaitForSingleObject( m_hVerifyEvent, 200 );

        if ( dwReturn == WAIT_TIMEOUT ) {
            Tick(); // 이 스레드에서 Tick을 호출해 주어야만 ACK 확인처리가 가능하다.
        }
        else if( dwReturn == WAIT_OBJECT_0 ) {
            if ( m_nSavedResult == NetError::NET_OK ) {
                return true;
            }
            else {
                break;
            }
        }
        else {
            START_LOG( cerr, L"Unexpected result of function call." )
                << BUILD_LOG( ::GetLastError() ) << END_LOG;
            break;
        }
    }

    if ( m_nSPIndex != 0 ) {
        START_LOG( cerr, L"SA 삭제. SPI : " << m_nSPIndex ) << END_LOG;
        KncSecurity::GetSADB().Delete( m_nSPIndex );
        m_nSPIndex = 0;
    }

    return false;
}

bool KMatchProxy::CheckRecvUser( IN const std::map<DWORD,std::vector<DWORD> >& mapServerUser_ )
{
    std::map<DWORD,std::vector<DWORD> >::const_iterator cmit;
    cmit = mapServerUser_.find( SiKGameServer()->GetUID() );
    if ( cmit == mapServerUser_.end() ) {
        START_LOG( clog, L"해당 방에 입장중인 유저의 서버가 아님." )
            << BUILD_LOG( SiKGameServer()->GetUID() ) << END_LOG;
        return false;
    }

    if ( cmit->second.empty() ) {
        START_LOG( clog, L"해당 룸에 입장중인 유저가 없음." )
            << BUILD_LOG( SiKGameServer()->GetUID() ) << END_LOG;
        return false;
    }
    return true;
}

IMPL_ON_FUNC( EVENT_MATCH_VERIFY_ACCOUNT_ACK )
{
    m_nSavedResult = kPacket_.m_nOK;

    if ( kPacket_.m_nOK != NetError::NET_OK ) {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
            << END_LOG;
    }
    else {
        START_LOG( clog, L"Connecting with Match Server succeess." )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }

    SetEvent( m_hVerifyEvent );

    if ( kPacket_.m_nOK == 0 ) {
        m_bAfterVerify = true;
    }
}

IMPL_ON_FUNC( EVENT_MATCH_ADD_USER_ACK )
{
    KEVENT_ADD_MATCH_ACK kPacket;
    KEVENT_ADD_MATCH_BROAD kBroadPacket;

    if( kPacket_.m_nOK == 0 ) { // 매칭 요청 성공시 처리.
        SiKPVPMatchManager()->SetModeCount( kPacket_.m_kPartyData.m_nModeInfo, kPacket_.m_nWaitListCount );
    }

    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_kPartyData = kPacket_.m_kPartyData;
    kPacket.m_nMatchModeCount = SiKPVPMatchManager()->GetModeCount( kPacket_.m_kPartyData.m_nModeInfo );

    kBroadPacket.m_nOK = kPacket_.m_nOK;
    kBroadPacket.m_kPartyData = kPacket_.m_kPartyData;
    kBroadPacket.m_nMatchModeCount = SiKPVPMatchManager()->GetModeCount( kPacket_.m_kPartyData.m_nModeInfo );

    std::vector<KPartyMember>::const_iterator cvit;
    for( cvit = kPacket_.m_kPartyData.m_vecPartyPeople.begin(); cvit != kPacket_.m_kPartyData.m_vecPartyPeople.end(); ++cvit ) {
        if( cvit->m_IsHost ) {
            SiKGSSimLayer()->m_kActorManager.QueueingEventTo( cvit->m_dwUID, KUserEvent::EVENT_ADD_MATCH_ACK, kPacket );
        }
        else {
            SiKGSSimLayer()->m_kActorManager.QueueingEventTo( cvit->m_dwUID, KUserEvent::EVENT_ADD_MATCH_BROAD, kBroadPacket );
        }
    }

    START_LOG( clog, L"매칭서버로부터 매칭 요청에 대한 결과 전달." )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nMatchModeCount )
        << BUILD_LOG( kPacket_.m_kPartyData.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_nWaitListCount )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_MATCH_DEL_USER_ACK )
{
    int kBroadPacket = 0;
    kBroadPacket = kPacket_.m_nOK;

    std::vector<KPartyMember>::const_iterator cvit;
    for( cvit = kPacket_.m_kPartyData.m_vecPartyPeople.begin(); cvit != kPacket_.m_kPartyData.m_vecPartyPeople.end(); ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( cvit->m_dwUID, KUserEvent::EVENT_DEL_MATCH_BROAD, kBroadPacket );
    }

    if( kPacket_.m_nOK == 0 ) { // 매칭 취소시의 처리.
        SiKPVPMatchManager()->SetModeCount( kPacket_.m_kPartyData.m_nModeInfo, kPacket_.m_nWaitListCount );
    }

    START_LOG( clog, L"매칭서버로부터 매칭 취소에 대한 결과 전달." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kPartyData.m_dwPartyUID )
        << BUILD_LOG( kPacket_.m_kPartyData.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_nWaitListCount )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_MATCH_FIND_MATCH_NOT )
{
    KEVENT_FIND_MATCH_NOT kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nModeInfo = kPacket_.m_nModeInfo;
    kPacket.m_nLoopCount = kPacket_.m_nLoopCount;

    if( SiKPVPMatchManager()->CalcMatchTeam( kPacket_.m_nModeInfo, kPacket_.m_vecPartyData, kPacket.m_mapPartyMember ) ) {
        kPacket.m_nOK = 0;
    }

    std::vector<KPartyData>::const_iterator cvitParty;
    std::vector<KPartyMember>::const_iterator cvitMember;
    for( cvitParty = kPacket_.m_vecPartyData.begin(); cvitParty != kPacket_.m_vecPartyData.end(); ++cvitParty ) {
        for( cvitMember = cvitParty->m_vecPartyPeople.begin(); cvitMember != cvitParty->m_vecPartyPeople.end(); ++cvitMember ) {
            SiKGSSimLayer()->m_kActorManager.QueueingEventTo( cvitMember->m_dwUID, KUserEvent::EVENT_FIND_MATCH_NOT, kPacket );
        }
    }

    SiKPVPMatchManager()->SetModeCount( kPacket_.m_nModeInfo, kPacket_.m_nWaitListCount );

    START_LOG( clog, L"매칭서버로부터 매칭된 파티 정보 결과 전달." )
        << BUILD_LOG( kPacket_.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_vecPartyData.size() )
        << BUILD_LOG( kPacket_.m_nModeInfo )
        << BUILD_LOG( kPacket_.m_nWaitListCount )
        << END_LOG;
}