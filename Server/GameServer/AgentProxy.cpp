#include "AgentProxy.h"
#include "GSSimLayer.h"
#include "Socket/NetCommon.h"
#include "NetError.h"
#include "GameServer.h"
#include "GSNetLayer.h"
#include "UserEvent.h"
#include "User.h"
#include "AgitHelper.h"
#include "AgentConnector.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KAgentProxy, KActorProxy );

#define EVENT_TYPE KAgentEvent
#define CLASS_TYPE KAgentProxy

KAgentProxy::KAgentProxy(void)
:m_bAfterVerify(false)
{
    m_hVerifyEvent = ::CreateEvent( NULL, false, false, NULL );
    m_nSavedResult = NetError::NET_OK;
    SetSendQueueSize( 1024 * 512 );
}

KAgentProxy::~KAgentProxy(void)
{
    OnSocketError();
    ::CloseHandle( m_hVerifyEvent );
}

ImplOstmOperatorW2A( KAgentProxy );
ImplToStringW( KAgentProxy )
{
    return stm_;
}

// Call to ProcessEventQueue (Client)
void KAgentProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer     ks;
    KSerBuffer      kbuff;

    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
        CASE( EAG_VERIFY_ACCOUNT_ACK );
       _CASE( EAG_SERVER_RELAY_PACKET_REQ, KServerRelayPacket );
       _CASE( EAG_AGIT_RELAY_PACKET_REQ, KAgitRelayPacket );

       _CASE( EAG_AGIT_CONFIG_NOT, KAgitConfig );
       _CASE( EAG_AGIT_MAP_NOT, KAgitMapInfo );
       _CASE( EAG_AGIT_FLOWER_NOT, KFlowerPotConfig );
       _CASE( EAG_ENTER_AGIT_ACK, KEnterAgitAck );
       _CASE( EAG_AGIT_LOADING_COMPLETE_ACK, KAgitLoadingCompleteAck );
        CASE( EAG_ENTER_AGIT_BROAD );
        CASE( EAG_LEAVE_AGIT_ACK );
        CASE( EAG_LEAVE_AGIT_BROAD );

       _CASE( EAG_BUY_AGIT_MAP_ACK, KBuyAgitMapAck );
        CASE( EAG_CHANGE_AGIT_MAP_BROAD );
       _CASE( EAG_SET_AGIT_OPTION_ACK, KSetAgitOptionAck );
       _CASE( EAG_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck );
        CASE( EAG_CHANGE_FLOWER_POT_STATE_BROAD );
       _CASE( EAG_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck );

       _CASE( EAG_AGIT_CHAT_ACK, KAgitChatAck );
        CASE( EAG_AGIT_CHAT_BROAD );
       _CASE( EAG_AGIT_BAN_USER_ACK, KAgitBanUserAck );
       _CASE( EAG_AGIT_BAN_USER_NOT, KAgitBanUserReq );

       _CASE( EAG_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck );
        CASE( EAG_CHANGE_AGIT_OBJECT_POS_BROAD );
       _CASE( EAG_AGIT_FRIEND_LIST_ACK, KSimpleAgitPageList );
       _CASE( EAG_AGIT_TOP_RANK_LIST_ACK, KSimpleAgitPageList );
       _CASE( EAG_AGIT_FAVORITE_LIST_ACK, KSimpleAgitPageList );
       _CASE( EAG_AGIT_ADD_FAVORITE_ACK, KAgitAddFavoriteAck );
       _CASE( EAG_AGIT_REMOVE_FAVORITE_ACK, KAgitUserParamAck );

       _CASE( EAG_REGISTER_AGIT_PAGE_ACK, KRegisterAgitPageAck );
       _CASE( EAG_UNREGISTER_AGIT_PAGE_ACK, DWORD );
       _CASE( EAG_AGIT_PAGE_LIST_ACK, KAgitPageList );

       _CASE( EAG_EQUIP_BROWNIE_ACK, KEquipBrownieAck );
        CASE( EAG_CHANGE_BROWNIE_BROAD );

       _CASE( EAG_WATERING_FAIRY_TREE_ACK, KWateringFairyTreeAck );
        CASE( EAG_CHANGE_FAIRY_TREE_BROAD );
       _CASE( EAG_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff );
       _CASE( EAG_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck );

        CASE( EAG_AGIT_VISITER_COUNT_BROAD );
       _CASE( EAG_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck );
       _CASE( EAG_SET_AGIT_SID_BROAD, PAIR_DWORD_INT );
       _CASE( EAG_AGIT_FAIRY_TREE_NOT, KFairyTreeConfig );
        CASE( EAG_CHANGE_AGIT_OPTION_BROAD );

       _CASE( EAG_GUESTBOOK_PAGE_ACK, KGuestBookPageList );
       _CASE( EAG_GUESTBOOK_WRITE_ACK, KAgitUserParamAck );
       _CASE( EAG_GUESTBOOK_DELETE_ACK, KAgitUserParamAck );
        CASE( EAG_GUESTBOOK_WRITE_BROAD );
        CASE( EAG_GUESTBOOK_DELETE_BROAD );

       _CASE( EAG_USE_TRAINING_OBJ_ACK, KUseTrainingObjAck );
        CASE( EAG_UPDATE_TRAINING_OBJ_BROAD );

       _CASE( EAG_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck );
       _CASE( EAG_SET_AGIT_CHARACTER_POS_ACK, KSetAgitCharPosAck );
       _CASE( EAG_SET_AGIT_CHARACTER_MOTION_ACK, KSetAgitCharMotionAck );
       _CASE( EAG_SET_AGIT_CHARACTER_COORDI_ACK, KSetAgitCharCoordiAck );
       _CASE( EAG_SET_AGIT_CHARACTER_SPEECH_ACK, KSetAgitCharSpeechAck );
        CASE( EAG_UPDATE_AGIT_CHARACTER_BROAD );
        CASE( EAG_UPDATE_AGIT_CHARACTER_POS_BROAD );

        // #END_OF_CASES
        // code:#END_OF_FILE

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

bool KAgentProxy::Connect( std::string strIP_, unsigned short usPort_ )
{
    if ( IsConnected() ) {
        START_LOG( clog, L"이미 연결되어 있습니다." ) << END_LOG;
        return true;
    }

    if( !KActorProxy::Connect( strIP_.c_str(), usPort_, true ) )
        return false;

    KSimpleServerInfo kPacket;
    kPacket.m_dwUID = SiKGameServer()->GetUID();
    kPacket.m_strIP = NetCommon::GetLocalIPW();
    kPacket.m_usPort = SiKGSNetLayer()->m_usPortMaster;
    kPacket.m_strName = SiKGameServer()->GetServerName();
    kPacket.m_nProtocolVer = SiKGameServer()->m_nProtocolVer;

    START_LOG( clog, L"Send EAG_VERIFY_ACCOUNT_REQ" )
        << BUILD_LOG( kPacket.m_dwUID )
        << BUILD_LOG( kPacket.m_strIP )
        << BUILD_LOG( kPacket.m_usPort )
        << BUILD_LOG( kPacket.m_strName )
        << BUILD_LOG( kPacket.m_nProtocolVer ) << END_LOG;

    _JIF( SendPacket( KAgentEvent::EAG_VERIFY_ACCOUNT_REQ, kPacket ), return false );

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

bool KAgentProxy::CheckRecvUser( IN const std::map<DWORD,std::vector<DWORD> >& mapServerUser_ )
{
    std::map<DWORD,std::vector<DWORD> >::const_iterator cmit;
    cmit = mapServerUser_.find( SiKGameServer()->GetUID() );
    if ( cmit == mapServerUser_.end() ) {
        START_LOG( clog, L"해당 아지트에 입장중인 유저의 서버가 아님." )
            << BUILD_LOG( SiKGameServer()->GetUID() ) << END_LOG;
        return false;
    }

    if ( cmit->second.empty() ) {
        START_LOG( clog, L"해당 아지트에 입장중인 유저가 없음." )
            << BUILD_LOG( SiKGameServer()->GetUID() ) << END_LOG;
        return false;
    }
    return true;
}

IMPL_ON_FUNC( EAG_VERIFY_ACCOUNT_ACK )
{
    m_nSavedResult = kPacket_.m_nOK;

    if ( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
            << END_LOG;
    }
    else
    {
        START_LOG( clog, L"Connecting with Agent Server succeess." )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }

    SetEvent( m_hVerifyEvent );

    if ( kPacket_.m_nOK == 0 ) {
        m_bAfterVerify = true;

        DWORD dwServerUID = SiKGameServer()->GetUID();
        int nSID = SiKAgentConnector()->GetAgentMasterSID();
		__LIF( SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_AGIT_ALL_CONFIG_REQ, dwServerUID ) );
        START_LOG( clog, L"에이전트 마스터에 아지트 설정정보 요청. ServerUID : " << dwServerUID ) << END_LOG;

        // 초기화
        SiKAgitHelper()->ClearAgitSID();
    }
}

_IMPL_ON_FUNC( EAG_SERVER_RELAY_PACKET_REQ, KServerRelayPacket )
{
    // 구현 필요함
    // 용도가 아직 없음.
}

_IMPL_ON_FUNC( EAG_AGIT_RELAY_PACKET_REQ, KAgitRelayPacket )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {

        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *cvit ) );
        spUser->SendPacket( kPacket_.m_kEventBuff , KUserEvent::GetEventIDString( kPacket_.m_usEventID ) );
    }

    START_LOG( clog, L"Agit Relay Packet.. serverUID : " << dwServerUID )
        << BUILD_LOG( KUserEvent::GetEventIDString( kPacket_.m_usEventID ) )
        << BUILD_LOG( kPacket_.m_usEventID )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwSenderUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_CONFIG_NOT, KAgitConfig )
{
    SiKAgitHelper()->UpdateAgitConfig( kPacket_ );
    START_LOG( cerr, L"에이전트 서버에서 아지트 설정정보 받음. ServerUID : " << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_MAP_NOT, KAgitMapInfo )
{
    SiKAgitHelper()->UpdateMapInfo( kPacket_ );
    START_LOG( cerr, L"에이전트 서버에서 아지트 맵정보 받음. ServerUID : " << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_FLOWER_NOT, KFlowerPotConfig )
{
    SiKAgitHelper()->UpdateFlowerPotConfig( kPacket_ );
    START_LOG( cerr, L"에이전트 서버에서 아지트 화분정보 받음. ServerUID : " << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_ENTER_AGIT_ACK, KEnterAgitAck )
{
    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwEnterUserUID, KUserEvent::EVENT_ENTER_AGIT_NOT, kPacket_ );

    START_LOG( clog, L"에이전트 서버로부터 아지트 입장 결과패킷 받음. UserUID : " << kPacket_.m_dwEnterUserUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kInfo.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_LOADING_COMPLETE_ACK, KAgitLoadingCompleteAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser == NULL ) {
        KEAG_LEAVE_AGIT_REQ kAgPacket;
        kAgPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
        kAgPacket.m_dwUserUID = kPacket_.m_dwUserUID;
        SendPacket( KAgentEvent::EAG_LEAVE_AGIT_REQ, kAgPacket );

        START_LOG( cwarn, L"해당 유저가 이미 이 서버에 없음.. 아지트 퇴장 처리." )
            << BUILD_LOG( kPacket_.m_dwAgitUID )
            << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
        return;
    }

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_AGIT_LOADING_COMPLETE_ACK, kPacket_ );

    START_LOG( clog, L"에이전트 서버로부터 로딩완료후 아지트입장 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_mapAgitUser.size() ) << END_LOG;
}

IMPL_ON_FUNC( EAG_ENTER_AGIT_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 새유저 입장 알림
    KEVENT_ENTER_AGIT_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kUserInfo = kPacket_.m_kUserInfo;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        // 자신을 제외하고 보낸다.
        if ( kPacket_.m_kUserInfo.m_dwUID == *cvit ) {
            continue;
        }

        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_ENTER_AGIT_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 유저입장 알림." )
        << BUILD_LOG( kPacket.m_dwAgitUID )
        << BUILD_LOG( kPacket.m_kUserInfo.m_nServerUID )
        << BUILD_LOG( kPacket.m_kUserInfo.m_dwUID )
        << BUILD_LOG( kPacket.m_kUserInfo.m_strNick )
        << BUILD_LOGc( kPacket.m_kUserInfo.m_cCharType )
        << BUILD_LOGc( kPacket.m_kUserInfo.m_cPromotion ) << END_LOG;
}

IMPL_ON_FUNC( EAG_LEAVE_AGIT_ACK )
{
    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_LEAVE_AGIT_ACK, kPacket_.m_dwAgitUID );

    START_LOG( clog, L"에이전트 서버로부터 아지트 퇴장 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_LEAVE_AGIT_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        // 자신을 제외하고 보낸다.
        if ( kPacket_.m_dwUserUID == *cvit ) {
            continue;
        }

        KEVENT_LEAVE_AGIT_BROAD kPacket;
        kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
        kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_LEAVE_AGIT_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 유저입장 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_BUY_AGIT_MAP_ACK, KBuyAgitMapAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_BUY_AGIT_MAP_NOT, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 맵 구매결과 전달받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwMapID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_AGIT_MAP_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 맵변경 알림
    KEVENT_CHANGE_AGIT_MAP_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_dwMapID = kPacket_.m_dwMapID;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        // 자신을 제외하고 보낸다.
        if ( kPacket_.m_dwAgitUID == *cvit ) {
            continue;
        }

        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_AGIT_MAP_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 맵변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwMapID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_OPTION_ACK, KSetAgitOptionAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kOption.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_SET_AGIT_OPTION_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 옵션결과 전달받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kOption.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kOption.m_strName )
        << BUILD_LOGc( kPacket_.m_kOption.m_ucMaxUser )
        << BUILD_LOGc( kPacket_.m_kOption.m_ucOpenType )
        << BUILD_LOG( kPacket_.m_kOption.m_strPassword )
        << BUILD_LOGc( kPacket_.m_kOption.m_ucInviteType )
        << BUILD_LOG( kPacket_.m_kOption.m_strProfile ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_SEED_FLOWER_POT_ACK, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 씨앗심기결과 전달받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_FLOWER_POT_STATE_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 화분상태 변경 알림
    KEVENT_CHANGE_FLOWER_POT_STATE_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kFlowerPot = kPacket_.m_kFlowerPotInfo;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        // 자신을 제외하고 보낸다.
        if ( kPacket_.m_dwAgitUID == *cvit ) {
            continue;
        }

        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_FLOWER_POT_STATE_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 화분상태 변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_HARVEST_FLOWER_POT_NOT, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 수확하기결과 전달받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_CHAT_ACK, KAgitChatAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_AGIT_CHAT_ACK, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트채팅 결과 전달받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_AGIT_CHAT_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_AGIT_CHAT_BROAD, kPacket_.m_kChatData );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트채팅 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strChatMsg ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_BAN_USER_ACK, KAgitBanUserAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_AGIT_BAN_USER_ACK, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 추방요청 결과받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwBanUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_BAN_USER_NOT, KAgitBanUserReq )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwBanUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_AGIT_BAN_USER_NOT, kPacket_.m_dwAgitUID );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 추방당함 전달받음." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwBanUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_SET_AGIT_OBJECT_POS_ACK, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 오브젝트배치 결과받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_AGIT_OBJECT_POS_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 오브젝트 변경 알림
    KEVENT_CHANGE_AGIT_OBJECT_POS_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_vecObject = kPacket_.m_vecObject;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_AGIT_OBJECT_POS_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 오브젝트 변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecObject.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_FRIEND_LIST_ACK, KSimpleAgitPageList )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_FRIEND_LIST_ACK, kPacket_, true, true );
    }

    START_LOG( clog, L"에이전트 서버로부터 친구아지트 리스트 받음." )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_mapPageList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_TOP_RANK_LIST_ACK, KSimpleAgitPageList )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_TOP_RANK_LIST_ACK, kPacket_, true, true );
    }

    START_LOG( clog, L"에이전트 서버로부터 인기아지트 리스트 받음." )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_mapPageList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_FAVORITE_LIST_ACK, KSimpleAgitPageList )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_FAVORITE_LIST_ACK, kPacket_, true, true );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 즐겨찾기 리스트 받음." )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_mapPageList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_ADD_FAVORITE_ACK, KAgitAddFavoriteAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_ADD_FAVORITE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 즐겨찾기 추가 결과 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kAgitInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kAgitInfo.m_strName )
        << BUILD_LOGc( kPacket_.m_kAgitInfo.m_ucOpenType ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_REMOVE_FAVORITE_ACK, KAgitUserParamAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_REMOVE_FAVORITE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 즐겨찾기 삭제 결과 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_REGISTER_AGIT_PAGE_ACK, KRegisterAgitPageAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_REGISTER_AGIT_PAGE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 페이지 등록 결과 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_usRoomID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_UNREGISTER_AGIT_PAGE_ACK, DWORD )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_ ) );
    if ( spUser ) {
        spUser->SendID( KUserEvent::EVENT_UNREGISTER_AGIT_PAGE_ACK );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 페이지 등록해제 결과 받음. AgitUID : " << kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_PAGE_LIST_ACK, KAgitPageList )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_AGIT_PAGE_LIST_ACK, kPacket_ );
        START_LOG( clog, L"Send AgitPage List.. UserUID : " << kPacket_.m_dwUserUID << L", Login : " << spUser->GetName() ) << END_LOG;
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 페이지 전달. UserUID : " << kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nSID )
        << BUILD_LOG( kPacket_.m_bOpen )
        << BUILD_LOG( kPacket_.m_bNotFull )
        << BUILD_LOG( kPacket_.m_usMinUser )
        << BUILD_LOG( kPacket_.m_usPageNum )
        << BUILD_LOG( kPacket_.m_usMaxPageNum ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_EQUIP_BROWNIE_ACK, KEquipBrownieAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_EQUIP_BROWNIE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 도우미 아이템장착 결과 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kEquip.m_ItemID )
        << BUILD_LOG( kPacket_.m_kEquip.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_EquipItemID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_EquipItemID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_bIsUsed ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_BROWNIE_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 오브젝트 변경 알림
    KEVENT_CHANGE_BROWNIE_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kBrownieInfo = kPacket_.m_kBrownieInfo;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_BROWNIE_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 도우미 변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kBrownieInfo.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_WATERING_FAIRY_TREE_ACK, KWateringFairyTreeAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_WATERING_FAIRY_TREE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"에이전트 서버로부터 아지트 요정의나무 물주기 결과 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_nLv )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_dwExp )
        << BUILD_LOG( kPacket_.m_kFairyTree.m_tmLastGrowTime ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_FAIRY_TREE_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 요정의나무 변경 알림
    KEVENT_CHANGE_FAIRY_TREE_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kFairyTree = kPacket_.m_kFairyTree;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_FAIRY_TREE_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 요정의나무 변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_FAIRY_TREE_BUFF_NOT, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 요정의나무 열매버프 정보 받음. UserUID : " << kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nFruitType )
        << BUILD_LOG( kPacket_.m_fBuffRate )
        << BUILD_LOGtm( CTime(kPacket_.m_tmBuffTime) ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kFruitBuff.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_USE_FAIRY_TREE_FRUIT_ACK, kPacket_ );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 요정의나무 열매먹기 결과 받음. UserUID : " << kPacket_.m_kFruitBuff.m_dwUserUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_AGIT_VISITER_COUNT_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 방문자수 알림
    KEVENT_AGIT_VISITER_COUNT_BROAD kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_nTodayVisiterCount = kPacket_.m_nTodayVisiterCount;
    kPacket.m_nTotalVisiterCount = kPacket_.m_nTotalVisiterCount;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_AGIT_VISITER_COUNT_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 방문자수 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_nTodayVisiterCount )
        << BUILD_LOG( kPacket_.m_nTotalVisiterCount ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kEnterAgitReq.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_ENTER_AGIT_SID_ACK, kPacket_ );
    }

    START_LOG( clog, L"Recive EnterAgitSIDAck.. UserUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_SID_BROAD, PAIR_DWORD_INT )
{
    DWORD dwAgitUID = kPacket_.first;
    int nSID = kPacket_.second;

    SiKAgitHelper()->SetAgitSID( dwAgitUID, nSID );
    START_LOG( clog, L"Set Agit SID.. AgitUID : " << dwAgitUID << L", SID : " << nSID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_AGIT_FAIRY_TREE_NOT, KFairyTreeConfig )
{
    SiKAgitHelper()->UpdateFairyTreeConfig( kPacket_ );
    START_LOG( cerr, L"에이전트 서버에서 요정의나무 설정정보 받음. ServerUID : " << GetUID() ) << END_LOG;
}

IMPL_ON_FUNC( EAG_CHANGE_AGIT_OPTION_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    // 해당 아지트 유저들에게 맵변경 알림
    KAgitOption kPacket;
    kPacket = kPacket_.m_kOption;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_CHANGE_AGIT_OPTION_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 아지트 옵션변경 알림." )
        << BUILD_LOG( kPacket_.m_kOption.m_dwAgitUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_GUESTBOOK_PAGE_ACK, KGuestBookPageList )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_GUESTBOOK_PAGE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GuestBook Page Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_GUESTBOOK_WRITE_ACK, KAgitUserParamAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_GUESTBOOK_WRITE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GuestBook Write Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_GUESTBOOK_DELETE_ACK, KAgitUserParamAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_GUESTBOOK_DELETE_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GuestBook Delete Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_GUESTBOOK_WRITE_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    KGuestBookWriteBroad kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_nNo = kPacket_.m_nNo;
    kPacket.m_kGuestMsg = kPacket_.m_kGuestMsg;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_GUESTBOOK_WRITE_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 낙서장 새글 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID ) << END_LOG;
}

IMPL_ON_FUNC( EAG_GUESTBOOK_DELETE_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    KGuestBookDeleteBroad kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_setDeleteNo = kPacket_.m_setDeleteNo;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_GUESTBOOK_DELETE_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 낙서장 글삭제 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_setDeleteNo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_USE_TRAINING_OBJ_ACK, KUseTrainingObjAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_USE_TRAINING_OBJ_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Use TrainingObj Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID) << END_LOG;
}

IMPL_ON_FUNC( EAG_UPDATE_TRAINING_OBJ_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    KUpdateTrainingObjBroad kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kObject = kPacket_.m_kTrainingObj;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_UPDATE_TRAINING_OBJ_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 훈련소 상태변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kTrainingObj.m_nAccCount )
        << BUILD_LOG( kPacket_.m_kTrainingObj.m_ItemUID ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_OPEN_AGIT_CHARACTER_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Open AgitCharacter Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nCharType ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_CHARACTER_POS_ACK, KSetAgitCharPosAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_SET_AGIT_CHARACTER_POS_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 18, L"Set AgitCharPos Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_vecCharPos.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_CHARACTER_MOTION_ACK, KSetAgitCharMotionAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_SET_AGIT_CHARACTER_MOTION_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Set AgitCharMotion Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nCharType )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nMotionID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosX )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosY )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapCoordi.size() )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapSpeech.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_CHARACTER_COORDI_ACK, KSetAgitCharCoordiAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_SET_AGIT_CHARACTER_COORDI_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Set AgitCharCostume Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nCharType )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nMotionID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosX )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosY )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapCoordi.size() )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapSpeech.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EAG_SET_AGIT_CHARACTER_SPEECH_ACK, KSetAgitCharSpeechAck )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwAgitUID ) );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_SET_AGIT_CHARACTER_SPEECH_ACK, kPacket_ );
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"Set AgitCharSpeech Ack.. Ret : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nCharType )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nMotionID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosX )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosY )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapCoordi.size() )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapSpeech.size() ) << END_LOG;
}

IMPL_ON_FUNC( EAG_UPDATE_AGIT_CHARACTER_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    KUpdateAgitCharBroad kPacket;
    kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
    kPacket.m_kCharInfo = kPacket_.m_kCharInfo;

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_UPDATE_AGIT_CHARACTER_BROAD, kPacket );
    }

    START_LOG( clog, L"에이전트 서버로부터 배치캐릭터 상태변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nCharType )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nMotionID )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosX )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_nPosY )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapCoordi.size() )
        << BUILD_LOG( kPacket_.m_kCharInfo.m_mapSpeech.size() ) << END_LOG;
}

IMPL_ON_FUNC( EAG_UPDATE_AGIT_CHARACTER_POS_BROAD )
{
    if ( false == CheckRecvUser( kPacket_.m_mapServerUser ) ) {
        return;
    }

    DWORD dwServerUID = SiKGameServer()->GetUID();

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = kPacket_.m_mapServerUser[dwServerUID].begin() ; cvit != kPacket_.m_mapServerUser[dwServerUID].end() ; ++cvit ) {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( *cvit, KUserEvent::EVENT_UPDATE_AGIT_CHARACTER_POS_BROAD, kPacket_.m_kCharPosInfo );
    }

    START_LOG( clog, L"에이전트 서버로부터 배치캐릭터 위치변경 알림." )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharPosInfo.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_kCharPosInfo.m_vecCharPos.size() ) << END_LOG;
}