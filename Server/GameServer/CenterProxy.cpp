#include "CenterProxy.h"
#include "DefaultFSM.h"
#include "GSSimLayer.h"
#include "ChannelManager.h"
#include "User.h"
#include "CenterEvent.h"
#include "CenterPacket.h"
#include "Socket/NetCommon.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "GameServer.h"
#include "SubThread.h"
#include "NetError.h"
#include "GSNetLayer.h"
#include "MiniGameRankManager.h"
#include "DonationManager.h"
#include "userpacket.h"
#include "Rainbow.h"
#include "ItemManager.h"
#include "MissionEvent.h"
#include "SubscriptionManager.h"
#include "NpcGift.h"
#include "DicePlay.h"
#include "SquareManager.h"
#include "UserFSM.h"
#include "GuildManager.h"
#include "GameCouponManager.h"
#include "GWCSupport.h"
#include "GSDBLayer.h"
#include "NewDonationManager.h"
#include "Sphinx.h"
#include "Survey.h"
#include "StrengthManager.h"
#include "ShutdownUser.h"
#include "ResultManager.h"
#include "GWCEvent.h"
#include "Log4.h"
#include "Adventure.h"
#include "OlympicManager.h"
#include "Adventure.h"
#include "BuffDonation.h"
#include "AccumulateAttendance.h"
#include "UpdatePlanManager.h"
#include "ConnectionGiftBox.h"
#include "CYOUBilling/CYOUBilling.h"
#include "CnConnector.h"
#include "VipEventManager.h"
#include "TongDonation.h"
#include "SocialCommerce.h"
#include "DungeonRankManager.h"

NiImplementRTTI( KCenterProxy, KActorProxy );

#define EVENT_TYPE KCenterEvent
#define CLASS_TYPE KCenterProxy

KCenterProxy::KCenterProxy(void)
:m_bAfterVerify(false)
{
    m_hVerifyEvent = ::CreateEvent( NULL, false, false, NULL );
    m_nSavedResult = NetError::NET_OK;
    SetSendQueueSize( 1024 * 512 );

}

KCenterProxy::~KCenterProxy(void)
{
    OnSocketError();
    ::CloseHandle( m_hVerifyEvent );
    
}

ImplOstmOperatorW2A( KCenterProxy );
ImplToStringW( KCenterProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

bool KCenterProxy::Connect( std::string strIP_, unsigned short usPort_ )
{
    // 1. 연결중 다시 호출 되어도 이상이 없도록 한다.
    if( IsConnected() )
    {
        START_LOG( cwarn, L"이미 연결되어 있습니다." ) << END_LOG;
        return true;
    }

    
    if( !KActorProxy::Connect( strIP_.c_str(), usPort_, true ) )
        return false;

    KECN_VERIFY_ACCOUNT_REQ kPacket;
    kPacket.m_nProtocolVer      = SiKGameServer()->m_nProtocolVer2;
    kPacket.m_nUserProtocolVer  = SiKGameServer()->m_nProtocolVer;
    kPacket.m_strName           = SiKGameServer()->GetServerName();
    kPacket.m_strIP             = NetCommon::GetLocalIPW();
    kPacket.m_usPort            = SiKGSNetLayer()->m_usPortMaster;
    kPacket.m_dwUID             = SiKGameServer()->GetServerID();

    START_LOG( clog, L"Send ECN_VERIFY_ACCOUNT_REQ 전송 " )
        << BUILD_LOG( kPacket.m_nProtocolVer )
        << BUILD_LOG( kPacket.m_nUserProtocolVer )
        << BUILD_LOG( kPacket.m_strName )
        << BUILD_LOG( kPacket.m_strIP )
        << BUILD_LOG( kPacket.m_usPort )
        << BUILD_LOG( kPacket.m_dwUID ) << END_LOG;

    _JIF( SendPacket( KCenterEvent::ECN_VERIFY_ACCOUNT_REQ, kPacket ), return false );

    DWORD dwElapsTime = ::GetTickCount();

    while( GetTickCount() - dwElapsTime < 5000 )
    {
        // break 문으로 while loop를 벗어나기 위해 switch문을 사용하지 않고 if ~ else를 썼다.
        DWORD dwReturn  = WaitForSingleObject( m_hVerifyEvent, 200 );

        if( dwReturn == WAIT_TIMEOUT )
            Tick(); // 이 스레드에서 Tick을 호출해 주어야만 ACK 확인처리가 가능하다.

        else if( dwReturn == WAIT_OBJECT_0 )
        {
            if(m_nSavedResult == NetError::NET_OK) 
                return true;
            else
                break;
        }

        else
        {
            START_LOG( cerr, L"Unexpected result of function call." )
                << BUILD_LOG( ::GetLastError() ) << END_LOG;
            break;
        }
    }

    if( m_nSPIndex != 0 )
    {
        START_LOG( cerr, L"SA 삭제. SPI : " << m_nSPIndex ) << END_LOG;
        KncSecurity::GetSADB().Delete( m_nSPIndex );
        m_nSPIndex = 0;
    }

    return false;
}

// Call to ProcessEventQueue (Client)
void KCenterProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
     KSerializer     ks;
     KSerBuffer      kbuff;

     ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
    CASE( ECN_VERIFY_ACCOUNT_ACK );
    CASE( ECN_KICK_USER_NOT );
   _CASE( ECN_LOUD_MSG_ACK, KSerBuffer );
    CASE( ECN_REPETITION_PROVIDE_REQ );
   _CASE( ECN_MINIGAME_RANK_ACK, KMiniGameRankList );
   _CASE( ECN_UPDATE_DONATION_ACK, KDotantionSyncData );
   _CASE( ECN_SERVER_RELAY_PACKET_NOT, KServerRelayPacket );
    CASE( ECN_RAINBOW_UPDATE_NOT );
    CASE( ECN_MISSION_EVENT_UPDATE_NOT );
    CASE( ECN_MISSION_NOTIFY_NOT );
    CASE( ECN_SUBSCRIPTION_INFO_UPDATE_NOT );
   _CASE( ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_ACK, PAIR_INT_DWORD );
    CASE( ECN_NPC_GIFT_UPDATE_NOT );
   _CASE( ECN_UPDATE_DICE_PLAY_ACK, KDicePlaySyncData );
    CASE( ECN_ITEM_DONATION_UPDATE_NOT );
   _CASE( ECN_COUPON_INFO_NOT, std::vector<KGameCouponInfo> );
    CASE( ECN_QUIZ_BINGO_DATA_NOT );
   _CASE( ECN_GWC_CONFIG_DATA_NOT, KGWCSupportConfigData );
   _CASE( ECN_GWC_DATA_NOT, std::vector<KGWCSupportData> );
    CASE( ECN_ENTER_SQUARE_ACK );
   _CASE( ECN_NEW_SQUARE_USER_NOT, KSquareUserInfo );
    CASE( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT );
    CASE( ECN_LEAVE_SQUARE_BROAD );
   _CASE( ECN_SQUARE_RELAY_PACKET_NOT, KSquareRelayPacket );
    CASE( ECN_ENTER_GUILD_SQUARE_ACK );
   _CASE( ECN_OFFLINE_GUILD_USER_BROAD, PAIR_DWORD );
   _CASE( ECN_CHAT_BROAD, KECN_CHAT_REQ );
    CASE( ECN_CHAT_ACK );
    CASE( ECN_GUILD_LIST_NOT );
    CASE( ECN_GUILD_MEMEBER_LIST_NOT );
    CASE( ECN_GUILD_PAGE_NOT );
   _CASE( ECN_ADD_NEW_GUILD_NOT, KNOneGuildData );
    CASE( ECN_DRUMOUT_GUILD_USER_BROAD_NOT );
    CASE( ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT );
   _CASE( ECN_SQUARE_LIST_ACK, KECN_SQUARE_LIST_ACK );
    CASE( ECN_UPDATE_GUILD_NOTICE_NOT );
   _CASE( ECN_GUILD_MARK_STATUS_NOT, KNGuildMarkInfo );
    CASE( ECN_UPDATE_GUILD_NAME_NOT );
    CASE( ECN_JOIN_GUILD_ACK );
    CASE( ECN_ADD_GUILD_USER_BROAD_NOT );
    CASE( ECN_GUILD_RANK_NOT );
    CASE( ECN_CANCEL_JOIN_GUILD_ACK );
    CASE( ECN_REMOVE_GUILD_USER_BROAD_NOT );
    CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT );
    CASE( ECN_SELF_DRUMOUT_GUILD_ACK );
    CASE( ECN_ACCEPT_GUILD_JOINER_ACK );
    CASE( ECN_REJECT_GUILD_JOINER_ACK );
   _CASE( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL );
    CASE( ECN_BREAKUP_GUILD_ACK );
   _CASE( ECN_REMOVE_GUILD_BROAD_NOT, DWORD );
   _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_NOT, KNGuildJoinPolicy );
   _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy );
   _CASE( ECN_GUILD_CONFIG_NOT, KGuildConfig );
   _CASE( ECN_UPGRADE_GUILD_GRADE_BROAD_NOT, DWORD );
    CASE( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
    CASE( ECN_INVITE_GUILD_USER_ACK );
   _CASE( ECN_UPDATE_GUILD_POINT_BROAD, KECN_UPDATE_GUILD_POINT_BROAD );
    CASE( ECN_GUILD_INFO_LIST_NOT );
    CASE( ECN_GUILD_NOTICE_LIST_NOT );
    CASE( ECN_GUILD_MEMBER_LIST_NOT );
    CASE( ECN_SQUARE_USER_SYNC_ACK );
   _CASE( ECN_UPDATE_NEW_DONATION_ACK, MAP_DWORD_INT );
    CASE( ECN_SOCKET_ITEM_GP_TABLE_NOT );
   _CASE( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
   _CASE( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
   _CASE( ECN_INVITE_GUILD_NOT, KGuildInvite );
   _CASE( ECN_CLIENT_PING_CONFIG_NOT, KPingConfig );
    CASE( ECN_USER_SERVER_ROOM_REQ );
    CASE( ECN_USER_SERVER_ROOM_ACK );
    CASE( ECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT );
    CASE( ECN_SPHINX_DATA_NOT );
   _CASE( ECN_CREATE_PARTY_ASK, KPartyData );
    CASE( ECN_SURVEY_DATA_NOT );
   _CASE( ECN_CREATE_PARTY_ACK, KPartyData );
   _CASE( ECN_LEAVE_PARTY_BROAD, KPartyData );
   _CASE( ECN_CHANGE_PARTY_HOST_BROAD, KPartyData );
   _CASE( ECN_INVITE_PARTY_ROOM_REQ, KPartyRoomData );
   _CASE( ECN_INVITE_PARTY_ASK, KPartyData );
   _CASE( ECN_INVITE_PARTY_ACK, KPartyData );
   _CASE( ECN_BREAK_PARTY_NOT, KPartyData );
   _CASE( ECN_BAN_PARTY_USER_NOT, KPartyData );
   _CASE( ECN_NOTICE_NOT, std::vector<KSerBuffer> );
   _CASE( ECN_INVITE_AGIT_NOT, KInviteAgitNot );
   _CASE( ECN_SHUTDOWN_USER_CONFIG_NOT, KShutdownUserConfig );
    CASE( ECN_GWC_EVENT_DATA_NOT );
   _CASE( ECN_GWC_NOTICE_NOT, DWORD );
   _CASE( ECN_ITEM_UPDATE_NOT, bool );
   _CASE( ECN_RECEIVE_LETTER_NOT, DWORD );
    CASE( ECN_ADVENTURE_DATA_NOT );
    CASE( ECN_OLYMPIC_RANK_DATA_NOT );
    CASE( ECN_OLYMPIC_SYNC_SOLO_DATA_NOT );
    CASE( ECN_OLYMPIC_SYNC_TEAM_DATA_ACK );
    CASE( ECN_OLYMPIC_EVENT_DATA_NOT );
   _CASE( ECN_PARTY_USER_READY_BROAD, KPartyData );
    CASE( ECN_BUFF_DONATION_TOTAL_DATA_NOT );
    CASE( ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT );
   _CASE( ECN_CASHBACK_RATIO_INFO_NOT, KCashbackRatioInfo );
   _CASE( ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT );
   _CASE( ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT );
   _CASE( ECN_ITEM_BUY_INFO_NOT, KItemBuyInfo );
   _CASE( ECN_EVENT_CHANGED_EVENT_STATE_INFO_NOT, KEVENT_UPDATE_EVENT_STATE_NOT );
    CASE( ECN_PLAN_SCRIPT_UPDATE_NOT );
   _CASE( ECN_CYOU_CHANGE_CASH_ACK, KCYOUCashPoint );
   _CASE( ECN_CYOU_GET_CURRENT_CASH_ACK, KCYOUCashPoint );
    CASE( ECN_CYOU_USER_LOGIN_ACK );
    CASE( ECN_CYOU_USER_HEART_BEAT_ACK );
    CASE( ECN_CONNECTION_GIFTBOX_INFO_NOT );
    CASE( ECN_EVENT_DB_UPDATE_NOT );
   _CASE( ECN_EVENT_PACKAGE_LIST_CHANGED_NOT, bool );
    CASE( ECN_TONG_DONATION_INFO_NOT );
    CASE( ECN_SOCIAL_COMMERCE_INIT_DATA_NOT );
    CASE( ECN_DUNGEON_RANK_RANKING_CONFIG_NOT );
    CASE( ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT );
    CASE( ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT );
    _CASE(ECN_BILLBOARD_DATA_BROAD, KECNBillboardChat);

    // #END_OF_CASES
    // code:#END_OF_FILE

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_VERIFY_ACCOUNT_ACK )
{
    m_nSavedResult = kPacket_.m_nOK;

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
            << END_LOG;
    }
    else
    {
        START_LOG( clog, L"Connecting with Center Server succeess." )
            << BUILD_LOG( kPacket_.m_kServerInfo.m_dwUID )
            << BUILD_LOG( kPacket_.m_kServerInfo.m_strName )
            << BUILD_LOG( kPacket_.m_kServerInfo.m_nMaxLevel )
            << END_LOG;
    }

    SetEvent( m_hVerifyEvent );
    SiKGameServer()->SetUID( kPacket_.m_kServerInfo.m_dwUID );

    if( SiKSquareManager()->AddSquareList( kPacket_.m_vecSquares ) == false )
    {
        START_LOG( cerr, L"광장 리스트 추가 실패" )
            << BUILD_LOG( kPacket_.m_vecSquares.size() ) << END_LOG;
    }

    KMiniGameRankList kMini;
    kMini.m_mapRank.clear();
    SEND_DATA( ECN_MINIGAME_RANK_REQ, kMini.m_mapRank );

    if ( kPacket_.m_nOK == 0 )
        m_bAfterVerify = true;
}

IMPL_ON_FUNC( ECN_KICK_USER_NOT )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwDestUserUID ) );

    if ( NULL == spUser ) {
        START_LOG( clog, L"대상 유저가 이 서버에는 존재하지 않음.. UID : " << kPacket_.m_dwDestUserUID ) << END_LOG;
        return;
    }

    // 연결이 끊기기 전 메세지를 보낸다.
    KEVENT_CLOSE_CONNECTION_NOT kNot;
    kNot.m_nCloseType   = kPacket_.m_nReason;
    spUser->QueueingEvent( KUserEvent::EVENT_CLOSE_CONNECTION_NOT, kNot );
    START_LOG( clog, L"강제 종료. UID : " << kPacket_.m_dwDestUserUID << L", Reason : " << kPacket_.m_nReason ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_LOUD_MSG_ACK, KSerBuffer )
{
    SiKChannelManager()->QueueingMsg( kPacket_ );
}

IMPL_ON_FUNC( ECN_REPETITION_PROVIDE_REQ )
{
    //채널 매니저에게 큐잉 한다.
    SiKChannelManager()->QueueingRepetition( kPacket_ );
}

_IMPL_ON_FUNC( ECN_MINIGAME_RANK_ACK, KMiniGameRankList )
{    
    SiKMiniGameRankManager()->UpdateRank( kPacket_.m_mapRank );

    START_LOG( clog, L"로긴 서버로부터 미니게임 랭킹데이타 받음")
        << BUILD_LOG( kPacket_.m_mapRank.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_DONATION_ACK, KDotantionSyncData )
{
    SiKDonationManager()->UpdateTotalDonationFromCenter( kPacket_.m_biTotalDonationPoint );
    SiKDonationManager()->UpdateDonationRankFromCenter( kPacket_.m_vecDonationRank );

    START_LOG( clog, L"센터 에서 받은 총 기부 금액.. : " << kPacket_.m_biTotalDonationPoint
        << L", 랭킹 : " << kPacket_.m_vecDonationRank.size() )  << END_LOG;
}

_IMPL_ON_FUNC( ECN_SERVER_RELAY_PACKET_NOT, KServerRelayPacket )
{
    KUserPtr spUser;
    if( kPacket_.m_bUseUID )
        spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwRecverUID );
    else
        spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( kPacket_.m_strRecverLogin );

    LIF( spUser ); // JIF 로 대체할까요
    if( spUser ) {
        if( kPacket_.m_bDirectSend ) {
            spUser->SendPacket( kPacket_.m_kEventBuff, KUserEvent::GetEventIDString( kPacket_.m_usEventID ) );
        }
        else {
            spUser->QueueingEvent( kPacket_.m_kEventBuff );
        }
    }

    START_LOG( clog, L"EventID : " << KUserEvent::GetEventIDString( kPacket_.m_usEventID )
        <<  L", Sender Login : " << kPacket_.m_strSenderLogin << L", Sender UID : " << kPacket_.m_dwSenderUID
        << L", Recver Login : " << kPacket_.m_strRecverLogin << L", Recver UID : " << kPacket_.m_dwRecverUID
        << L", bUseUID : " << kPacket_.m_bUseUID << L", bDirect : " << kPacket_.m_bDirectSend )  << END_LOG;
}

IMPL_ON_FUNC( ECN_RAINBOW_UPDATE_NOT )
{
    SiKRainbow()->UpdateFromCenter( kPacket_ );

    START_LOG( cwarn, L" 레인 보우 이벤트가 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_nRainBowID )
        << BUILD_LOG( kPacket_.m_nDailyEventID )
        << BUILD_LOG( kPacket_.m_nJackPotEventID )
        << BUILD_LOG( kPacket_.m_vecEventList.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_MISSION_EVENT_UPDATE_NOT )
{
    // Center Server --> GameServer
    // 미션 관련 데이터 받기.
    SiKMissionEvent()->UpdateFromCenter( kPacket_ );

    START_LOG( clog, L" 미션지급 이벤트가 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_vecMissions.size() )
        << BUILD_LOG( kPacket_.m_setItemList.size() )
        << BUILD_LOG( kPacket_.m_bDateChanged ) << END_LOG;
}

IMPL_ON_FUNC( ECN_MISSION_NOTIFY_NOT )
{
    // Center Server --> GameServer
    // 미션 이벤트 관련 공지 출력.
    SiKMissionEvent()->NotifyFromCenter( kPacket_ );

    START_LOG( clog, L" 미션이벤트 알림 공지가 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_dwTime ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SUBSCRIPTION_INFO_UPDATE_NOT )
{
    SiKSubscriptionManager()->SetEventInfo( kPacket_ );
    START_LOG( clog, L" 응모 이벤트정보 게임서버로 전달됨. size : " << kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_ACK, PAIR_INT_DWORD )
{
    SiKSubscriptionManager()->SetCurrentGiftNum( kPacket_.first, kPacket_.second );
    START_LOG( clog, L" 응모이벤트 경품수량 게임서버로 전달됨. EventUID : " << kPacket_.first << L", CurrentGiftNum : " << kPacket_.second ) << END_LOG;
}

IMPL_ON_FUNC( ECN_NPC_GIFT_UPDATE_NOT )
{
    SiKNpcGift()->UpdateEventInfo( kPacket_ );
    START_LOG( clog, L" NPC Gift 이벤트 서버로 전달됨. size : " << kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_DICE_PLAY_ACK, KDicePlaySyncData )
{
    SiKDicePlay()->UpdateDicePlayRankFromCenter( kPacket_.m_vecDicePlayRank );

    START_LOG( clog, L" 주사위 랭킹정보가 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_vecDicePlayRank.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ITEM_DONATION_UPDATE_NOT )
{
    // Center Server --> GameServer
    // 아이템 기부 관련 데이터 전송.
    SiKDonationManager()->UpdateFromCenter( kPacket_ );

    START_LOG( clog, L" 아이템 기부 데이터가 서버로 전달됨..," )
        << BUILD_LOG( kPacket_.m_vecGradeInfo.size() ) << END_LOG;

}

_IMPL_ON_FUNC( ECN_COUPON_INFO_NOT, std::vector<KGameCouponInfo> )
{
    if( SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_FUNBOX_TH ) ) {
        SiKGameCouponManager()->SetGameCouponInfo( kPacket_ );
    }
}

IMPL_ON_FUNC( ECN_QUIZ_BINGO_DATA_NOT )
{
    START_LOG( clog, L" 퀴즈 빙고 데이터 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_nEventID )
        << BUILD_LOGtm( CTime(kPacket_.m_tmBegin) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmEnd) )
        << BUILD_LOG( kPacket_.m_mapCompressedQuestionList.size() ) << END_LOG;

    SiKQuizBingo()->UpdateBingoConfig( kPacket_ );

}

_IMPL_ON_FUNC( ECN_GWC_CONFIG_DATA_NOT, KGWCSupportConfigData )
{
    SiKGWCSupport()->UpdateConfigData( kPacket_ );
    START_LOG( clog, L"센터로 부터 GWC Event 설정 전달 받음." ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GWC_DATA_NOT, std::vector<KGWCSupportData> )
{
    SiKGWCSupport()->UpdateGWCData( kPacket_ );
    START_LOG( clog, L"센터로 부터 GWC Event 데이터 전달 받음." ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ENTER_SQUARE_ACK )
{
    KUserPtr spUser;
    KEVENT_ENTER_SQUARE_ACK kPacket;

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID );
    START_LOG( clog, L"광장 입장 응답, User UID : " << kPacket_.m_dwUserUID ) << END_LOG;
    JIF( spUser );
    KSquarePtr spSquare = SiKSquareManager()->GetSquare( kPacket_.m_dwSquareUID );

    // TODO : 옳은지 확인해보자.
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_kMyPos = kPacket_.m_kMyPos;
    kPacket.m_dwRelayServerIP = kPacket_.m_dwRelayServerIP;
    kPacket.m_usRelayServerPort = kPacket_.m_usRelayServerPort;
    kPacket.m_vecUserInfo = kPacket_.m_vecUserInfo;

    if ( kPacket_.m_nOK == 0 ) {

        switch( spUser->GetStateID() ) {
        case KUserFSM::STATE_MINIGAME:
        case KUserFSM::STATE_MINIGAME_PLAYING:
            // 광장으로 유저 넣기
            if ( spSquare == NULL ) {
                kPacket_.m_nOK = 2;
                START_LOG( cerr, L"요청한 광장을 찾을수 없음. SquareID : " << kPacket_.m_dwSquareUID ) << END_LOG;
                break;
            }

            spSquare->BackToSquareFromMiniGame( spUser, kPacket ); // 광장에서의 마지막 나의 위치를 어디에 기억시켜야 하나, 광장에 돌아갔을 때, 그 위치로 가야함

            if ( spUser ) {
                LIF( spUser->SendPacket( KUserEvent::EVENT_EXIT_MINIGAME_ACK, kPacket ) );
            }

            break;
        default:
            SiKSquareManager()->EnterSquareReserve( spUser, kPacket_.m_dwSquareUID );

            if ( spUser ) {
                LIF( spUser->SendPacket( KUserEvent::EVENT_ENTER_SQUARE_ACK, kPacket ) );
            }
        }

        return;
    }
    else  {
        START_LOG( cerr, L"광장 입장 실패, UserUID :" << kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_dwSquareUID ) << END_LOG;
    }

    if ( spUser ) {
        LIF( spUser->SendPacket( KUserEvent::EVENT_ENTER_SQUARE_ACK, kPacket ) );
    }
}

_IMPL_ON_FUNC( ECN_NEW_SQUARE_USER_NOT, KSquareUserInfo )
{
    SiKSquareManager()->QueueingToSquare( kPacket_.m_dwSquareID, KUserEvent::EVENT_NEW_SQUARE_USER_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUID );
    // 로그 너무 많이 남음. 주석처리
    //START_LOG( clog, L"내 이웃의 UID. dwUID : " << kPacket_.m_dwUID << L", vecNeighborhood.size() : " << kPacket_.m_vecNeighborhood.size() ) << END_LOG;

    if( spUser ) {
        LIF( spUser->SendPacket( KUserEvent::EVENT_UPDATE_SQUARE_USER_NOT, kPacket_.m_vecNeighborhood ) );
    }
}

IMPL_ON_FUNC( ECN_LEAVE_SQUARE_BROAD )
{
    START_LOG( clog, L"나간 사람 알리기. SquareUID : " << kPacket_.m_dwSquareUID << L",UserUID : " << kPacket_.m_dwUID ) << END_LOG;

    SiKSquareManager()->LeaveSquare( kPacket_ );
    SiKSquareManager()->QueueingToSquare( kPacket_.m_dwSquareUID, KUserEvent::EVENT_LEAVE_SQUARE_USER_NOT, kPacket_.m_dwUID );        
}

_IMPL_ON_FUNC( ECN_SQUARE_RELAY_PACKET_NOT, KSquareRelayPacket )
{
    std::map<DWORD,std::vector<DWORD> >::iterator mit;
    mit = kPacket_.m_mapReceiverUsers.find( SiKGameServer()->GetUID() ); // 현재 서버에 속한 데이터가 있는지 검색
    if( mit == kPacket_.m_mapReceiverUsers.end() )
    {
        START_LOG( clog, L"해당 서버 릴레이 데이터 없음 : " << KUserEvent::GetEventIDString( kPacket_.m_usEventID ) ) << END_LOG;
        return; // 해당 서버의 데이터 없으므로 리턴
    }

    KSquarePtr spSquare = SiKSquareManager()->GetSquare( kPacket_.m_dwSquareUID ); // 광장 포인터 

    JIF( spSquare );
    std::vector<DWORD>::iterator vit;
    for( vit = mit->second.begin(); vit != mit->second.end() ; ++vit ) // 유저UID.
    {
        spSquare->SendRelayTo( *vit, kPacket_.m_usEventID, kPacket_.m_kEventBuff ); // 각각의 유저에게 데이터 전송
    }

    START_LOG( clog, L"Square Relay Packet.. Packet : " << KUserEvent::GetEventIDString( kPacket_.m_usEventID ) )
        << BUILD_LOG( kPacket_.m_usEventID )
        << BUILD_LOG( mit->second.size() )
        << BUILD_LOG( SiKGameServer()->GetUID() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ENTER_GUILD_SQUARE_ACK )
{
    KSquarePtr spSquare = SiKSquareManager()->GetSquare( kPacket_.m_kSquareInfo.m_dwID );

    do {
        if ( spSquare ) {
            continue;
        }

        if ( SiKSquareManager()->AddSquare( kPacket_.m_kSquareInfo ) == false ) {

            START_LOG( cerr, L"길드 광장 입장 시, 광장 생성 실패" )
                << BUILD_LOG( kPacket_.m_kSquareInfo.m_dwID ) << END_LOG;
                
            kPacket_.m_kEnterSquare.m_nOK;
            continue;
        }

    } while (false);

    QueueingEvent( KCenterEvent::ECN_ENTER_SQUARE_ACK, kPacket_.m_kEnterSquare );

    START_LOG( clog, L"길드 광장 입장 응답 " )
        << BUILD_LOG( kPacket_.m_kEnterSquare.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kEnterSquare.m_vecUserInfo.size() )
        << BUILD_LOG( kPacket_.m_kSquareInfo.m_dwID )
        << BUILD_LOG( kPacket_.m_kEnterSquare.m_dwSquareUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_OFFLINE_GUILD_USER_BROAD, PAIR_DWORD )
{
    // 길드원이 나간것을 센터서버에서 알려줌.
    // 게임서버의 길드 매니저는 유저가 나간 사실을 적용하고,
    // 해당 길드를 찾아서 해당 길드에서는 접속정보를 로그아웃으로 바꾸고,
    // 이 서버에 접속중인 같은 길드원에게 나갔다는 사실을 통보하라.
    DWORD dwUserUID = kPacket_.first;
    DWORD dwGuildUID = kPacket_.second;

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( dwUserUID );
    if ( spUser != NULL ) {
        START_LOG( clog, L"센터서버로부터 길드원 로그오프 알림왔으나 이 서버에 해당 길드원이 존재." )
            << BUILD_LOG( dwGuildUID )
            << BUILD_LOG( dwUserUID ) << END_LOG;
        return;
    }
    // 길드매니저에서 유저 로그오프 처리
    LIF( SiKGuildManager()->LogOffUser( dwGuildUID, dwUserUID ) );

    START_LOG( clog, L"센터서버로부터 길드원 로그오프 알림." )
        << BUILD_LOG( dwGuildUID )
        << BUILD_LOG( dwUserUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CHAT_BROAD, KECN_CHAT_REQ )
{
    START_LOG( clog, L"Center Chat. ChatType : " << (int)kPacket_.m_kChatData.m_cChatType )
        << BUILD_LOG( kPacket_.m_kChatData.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_strReceiverNick ) << END_LOG;

    switch( kPacket_.m_kChatData.m_cChatType )
    {
    case KChatData::MSG_GUILD:
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_CHAT_NOT, kPacket_.m_kChatData.m_strSenderNick.c_str(), kPacket_.m_kChatData.m_dwSenderUID, 0, kPacket_.m_kChatData );
        break;
    case KChatData::MSG_WHISPER:
    case KChatData::MSG_WHISPER_EX:
        {
            KECN_CHAT_ACK kCnPacket;
            kCnPacket.m_nOK = -99;
            kCnPacket.m_kChatData = kPacket_.m_kChatData;
            kCnPacket.m_dwServerUID = kPacket_.m_dwServerUID;

            KUserPtr spkUser;
            spkUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kChatData.m_dwReceiverUID );
            if ( spkUser == NULL ) {
                // 대상유저를 찾을 수 없음.
                kCnPacket.m_nOK = 4;
                __LIF( SendPacket( KCenterEvent::ECN_CHAT_ACK, kCnPacket ) );
                START_LOG( cwarn, L"귓속말 대상 유저가 없음. Nick : " << kPacket_.m_kChatData.m_strReceiverNick ) << END_LOG;
                return;
            }

            kPacket_.m_kChatData.m_dwReceiverUID    = spkUser->GetUID();
            kPacket_.m_kChatData.m_strReceiverNick  = spkUser->GetNickName();

            if ( SiKGSSimLayer()->CheckTypeFlag(ST_BROADCASTING) ) {
                if ( spkUser->GetAuthLevel() == KUser::AL_HIGH_RANKER || spkUser->IsAdmin() ) {
                    kCnPacket.m_nOK = 3;
                    __LIF( SendPacket( KCenterEvent::ECN_CHAT_ACK, kCnPacket ) );
                    START_LOG( clog, L"방송용 서버,초고수나 운영자에게는 귓말을 전송하지 않는다." ) << END_LOG;
                    return;
                }
            }

            spkUser->SendPacket( KUserEvent::EVENT_CHAT_NOT, kPacket_.m_kChatData );

            // 성공
            kCnPacket.m_nOK = 0;
            __LIF( SendPacket( KCenterEvent::ECN_CHAT_ACK, kCnPacket ) );
        }
        break;
    default:
        START_LOG( clog, L"처리가 정의되지 않은 채팅 타입 : " << (int)kPacket_.m_kChatData.m_cChatType ) << END_LOG;
        break;
    }
}

IMPL_ON_FUNC( ECN_CHAT_ACK )
{
    KUserPtr spUser;
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kChatData.m_dwSenderUID );
    if ( spUser != NULL ) {
        KEVENT_WHISPER_ACK kPacket;
        kPacket.m_nOK = kPacket_.m_nOK;
        kPacket.m_kChatData = kPacket_.m_kChatData;
        spUser->SendPacket( KUserEvent::EVENT_WHISPER_ACK, kPacket );
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"귓속말 결과 : " << kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kChatData.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_dwReceiverUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strReceiverNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_strChatMsg ) << END_LOG;
}

IMPL_ON_FUNC( ECN_GUILD_LIST_NOT )
{
    // 미사용 패킷. 제거예정.

    // 길드 정보 생성
//     SiKGuildManager()->AddGuild( kPacket_.m_mapGuildList );
// 
//     // 길드 공지 생성
//     std::map< DWORD, std::vector<KNGuildNotice> >::iterator mit2;
//     for ( mit2 = kPacket_.m_mapGuildNotice.begin() ; mit2 != kPacket_.m_mapGuildNotice.end() ; ++mit2 ) {
//         LIF( SiKGuildManager()->SetGuildNotice( mit2->first, mit2->second ) );
//     }
// 
//     // 온라인 상태의 길드 맴버 정보 설정
//     std::map< DWORD, std::map< DWORD, KNGuildUserInfo > >::iterator mit3;
//     for ( mit3 = kPacket_.m_mapGuildMember.begin() ; mit3 != kPacket_.m_mapGuildMember.end() ; ++mit3 )
//     {
//         LIF( SiKGuildManager()->SetGuildMemberList( mit3->first, mit3->second ) );
//     }
// 
//     START_LOG( clog, L"센터서버에서 전체 길드정보/길드공지/온라인길드맴버리스트 받음." )
//         << BUILD_LOG( kPacket_.m_mapGuildList.size() )
//         << BUILD_LOG( kPacket_.m_mapGuildNotice.size() )
//         << BUILD_LOG( kPacket_.m_mapGuildMember.size() ) << END_LOG;
// 
//     // 센터서버만 재시작될 경우, 게임서버의 길드 맴버리스트/맴버 접속 0여부의 싱크가 다르게 됨으로
//     // 이 패킷을 받으면 센터서버로 게임서버의 맴버리스트 정보를 모아서 보내주도록 한다.
//     KECN_GUILD_MEMEBER_LIST_SYNC_NOT kCnPacket;
//     SiKGuildManager()->GetAllGuildMemberList( kCnPacket );
//     // 센터서버로 전달
//     LIF( SendPacket( KCenterEvent::ECN_GUILD_MEMEBER_LIST_SYNC_NOT, kCnPacket ) );
}

IMPL_ON_FUNC( ECN_GUILD_MEMEBER_LIST_NOT )
{
    START_LOG( clog, L"센터서버에서 온라인길드맴버리스트 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapGuildUserList.size() ) << END_LOG;
    // 아래에서 swap 이 일어나므로 로그를 위에 찍겠습니다.

    // 온라인 상태의 길드 맴버 정보 설정
    LIF( SiKGuildManager()->SetGuildMemberList( kPacket_.m_dwGuildUID, kPacket_.m_mapGuildUserList ) );
}

IMPL_ON_FUNC( ECN_GUILD_PAGE_NOT )
{
    START_LOG( clog, L"센터서버에서 길드 페이지 받음. Sort Type : " << (UINT)kPacket_.m_ucSortType ) << END_LOG;

    // 길드 페이지 정보전달
    SiKGuildManager()->SetGuildPageID( kPacket_.m_ucSortType, kPacket_.m_kPageBuffer );
}

_IMPL_ON_FUNC( ECN_ADD_NEW_GUILD_NOT, KNOneGuildData )
{
    // 센터서버에서 새로 추가된 길드정보를 받아옴.
    if ( SiKGuildManager()->Add( kPacket_.m_kGuildInfo ) == false ) {
        START_LOG( cwarn, L"이미 등록된 길드. GuildUID : " << kPacket_.m_kGuildInfo.m_dwUID ) << END_LOG;
        return;
    }

    // 새로 생성인 경우만.
    LIF( SiKGuildManager()->SetGuildNotice( kPacket_.m_kGuildInfo.m_dwUID, kPacket_.m_vecGuildNotice ) );
    LIF( SiKGuildManager()->SetGuildMemberList( kPacket_.m_kGuildInfo.m_dwUID, kPacket_.m_mapGuildUserList ) );

    START_LOG( clog, L"센터서버에서 새로 추가된 길드정보를 추가. GuildUID : " << kPacket_.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strMasterNick )
        << BUILD_LOG( kPacket_.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket_.m_mapGuildUserList.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_DRUMOUT_GUILD_USER_BROAD_NOT )
{
    // 다른 게임서버에서 길드원이 탈퇴/강퇴된 경우 센터서버를 통해 전달받는다.
    LIF( SiKGuildManager()->DeleteUser( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_cOutType ) );

    START_LOG( clog, L"센터서버로부터 탈퇴된 길드원 정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_cOutType ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT )
{
    // 다른 게임서버에서 길드원이 자기소개를 변경한 경우, 센터서버에서 전달받는다.
    LIF( SiKGuildManager()->SetMemberComment( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_strUserComment ) );

    START_LOG( clog, L"센터서버로부터 길드원 소갯말 변경정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strUserComment ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SQUARE_LIST_ACK, KECN_SQUARE_LIST_ACK )
{
    const DWORD& dwUserUID = kPacket_.first;
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( dwUserUID );
    if ( spUser == NULL ) {
        START_LOG( cerr, L"유저가 존재하지 않음, UserUID: " << dwUserUID ) << END_LOG;
        return;
    }

    spUser->SendPacket( KUserEvent::EVENT_SQUARE_LIST_ACK, kPacket_.second );
    START_LOG( clog, L"광장리스트 size : " << kPacket_.second.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT )
{
    // 다른 게임서버에서 길드원 맴버레벨이 변경된 경우, 센터서버에서 전달.
    LIF( SiKGuildManager()->ChangeGuildMemberLevel( kPacket_.m_dwGuildUID, kPacket_.m_mapChangeMemberLevel ) );

    START_LOG( clog, L"센터서버로부터 길드맴버레벨 변경정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_NOTICE_NOT )
{
    // 다른 게임서버에서 길드공지가 변경된 경우, 센터서버에서 전달.
    LIF( SiKGuildManager()->ModifyNotice( kPacket_.m_dwGuildUID, kPacket_.m_vecNotice ) );

    START_LOG( clog, L"센터서버로부터 변경된 길드공지 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecNotice.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_MARK_STATUS_NOT, KNGuildMarkInfo )
{
    KGuildPtr spGuild = SiKGuildManager()->GetGuild( kPacket_.m_dwGuildUID );
    if( !spGuild ) {
        START_LOG( cerr, L"해당 길드가 존재 하지 않음. UID : " << kPacket_.m_dwGuildUID ) << END_LOG;
        return;
    }

    if ( spGuild->UpdateGuildMarkInfo( kPacket_ ) ) {
        spGuild->SendToAll( KUserEvent::EVENT_GUILD_MARK_STATUS_NOT, kPacket_ );
        START_LOG( clog, L"길드 마크 상태, 리비전 변경이 있음..UID : " << kPacket_.m_dwGuildUID )
            << BUILD_LOGc( kPacket_.m_ucMarkStatus )
            << BUILD_LOG( kPacket_.m_nMarkRevision )
            << BUILD_LOG( kPacket_.m_strFileName ) << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_NAME_NOT )
{
    LIF( SiKGuildManager()->ModifyName( kPacket_.m_dwGuildUID, kPacket_.m_strGuildName ) );

    START_LOG( clog, L"센터서버로부터 변경된 길드 이름 받음. Guild UID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName ) << END_LOG;
}

IMPL_ON_FUNC( ECN_JOIN_GUILD_ACK )
{
    // 길드가입요청에 대한 센터서버 길드매니저로부터의 처리결과 응답.
    // 해당 유저한테 패킷결과를 전달해줘야한다.
    KEVENT_JOIN_GUILD_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_kGuildUserInfo = kPacket_.m_kGuildUserInfo;
    kPacket.m_kGuildInfo = kPacket_.m_kGuildInfo;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_JOIN_GUILD_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드가입요청 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cMemberLevel ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ADD_GUILD_USER_BROAD_NOT )
{
    // 길드에 신규유저가 추가되었을때, 센터서버에서 게임서버로 전달.
    // 길드맴버레벨에 따라 추가적인 동작을 진행하도록 한다.
    if ( SiKGuildManager()->AddUser( kPacket_.m_kGuildUserInfo ) == false ) {
        START_LOG( cwarn, L"이미 길드에 등록된 유저. GuildUID : " << kPacket_.m_dwGuildUID )
            << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_strLogin ) << END_LOG;
    }

    START_LOG( clog, L"센터서버로부터 길드원추가 알림. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_strLogin ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_ACK )
{
    // 길드가입취소 요청에 대한 센터서버 길드매니저로부터의 처리결과 응답.
    // 해당 유저에게 패킷결과 전달
    KEVENT_CANCEL_JOIN_GUILD_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_CANCEL_JOIN_GUILD_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드가입 취소요청 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
}

IMPL_ON_FUNC( ECN_REMOVE_GUILD_USER_BROAD_NOT )
{
    // 길드에 유저가 제거되었을때, 센터서버에서 게임서버로 전달.
    // 길드매니저에서 유저 제거
    LIF( SiKGuildManager()->DeleteUser( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_cOutType ) );

    START_LOG( clog, L"센터서버로부터 길드원제거 알림." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_cOutType ) << END_LOG;
    // 길드맴버레벨에 따라 추가적인 동작이 필요.
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK )
{
    // 길드원 맴버레벨 변경요청에 대한 센터서버 길드매니저로부터의 처리결과 응답.
    // 해당 유저에게 패킷결과 전달
    KEVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwGuildMasterUID = kPacket_.m_dwGuildMasterUID;
    kPacket.m_mapChangeMemberLevel = kPacket_.m_mapChangeMemberLevel;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwGuildMasterUID, KUserEvent::EVENT_CHANGE_GUILD_MEMBER_LEVEL_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드원 맴버레벨변경 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT )
{
    // 길드원 맴버레벨 변경시, 센터서버에서 게임서버로 전달.
    std::map< char, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;

    // 탈퇴처리
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_DRUMOUT );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {
        for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {
            LIF( SiKGuildManager()->DeleteUser( kPacket_.m_dwGuildUID, *sit, kPacket_.m_cOutType ) );
        }
        START_LOG( clog, L"탈퇴 처리유저. GuildUID : " << kPacket_.m_dwGuildUID << L",size : " << mit->second.size() << L",OutType : " << (int)kPacket_.m_cOutType ) << END_LOG;
    }

    // 탈퇴외 처리
    LIF( SiKGuildManager()->ChangeGuildMemberLevel( kPacket_.m_dwGuildUID, kPacket_.m_mapChangeMemberLevel ) );

    START_LOG( clog, L"센터서버로부터 길드원 맴버레벨 변경 알림." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() )
        << BUILD_LOGc( kPacket_.m_cOutType ) << END_LOG;
    // 길드맴버레벨에 따라 추가적인 동작이 필요.
}

IMPL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_ACK )
{
    // 자진 탈퇴 요청에 대한 선터서버 길드매니저로부터의 처리결과 응답.
    // 해당 유저에게 패킷결과 전달
    KEVENT_SELF_DRUMOUT_GUILD_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_SELF_DRUMOUT_GUILD_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드자진탈퇴 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_ACK )
{
    // 길드가입대기자 가입승락 요청결과
    // 요청한 유저에게 패킷결과 전달
    KEVENT_ACCEPT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_setUserUID = kPacket_.m_setUserUID;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_ACCEPT_GUILD_JOINER_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드가입승인 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
}

IMPL_ON_FUNC( ECN_REJECT_GUILD_JOINER_ACK )
{
    // 길드가입 거절요청 결과
    // 요청한 유저에게 패킷결과 전달
    KEVENT_REJECT_GUILD_JOINER_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_setUserUID = kPacket_.m_setUserUID;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_REJECT_GUILD_JOINER_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드가입승인 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_setUserUID.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_GUILD_RANK_NOT )
{
    // 길드 매니저에 셋팅 한다.
    SiKGuildManager()->SetGuildRankData( kPacket_ );
    START_LOG( clog, L"길드 랭킹이 업데이트 되었습니다." ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL )
{
    LIF( SiKGuildManager()->ChangeURL( kPacket_ ) );

    START_LOG( clog, L"센터서버로부터 변경된 길드 URL 받음." )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strURL ) << END_LOG;
}

IMPL_ON_FUNC( ECN_BREAKUP_GUILD_ACK )
{
    // 길드해산요청 결과, 해당유저에게 전달
    KEVENT_BREAKUP_GUILD_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_BREAKUP_GUILD_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드해산 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_REMOVE_GUILD_BROAD_NOT, DWORD )
{
    // 센터에서 길드 삭제 알림.
    SiKGuildManager()->RemoveGuild( kPacket_ );
    START_LOG( clog, L"센터서버로부터 길드삭제 전달받음. GuildUID : " << kPacket_) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_NOT, KNGuildJoinPolicy )
{
   LIF( SiKGuildManager()->ChangeJoinPolicy( kPacket_ ) );

   START_LOG( clog, L"센터서버로부터 길드 가입승인 방법 변경 알림 패킷 받음." )
       << BUILD_LOG( kPacket_.m_nOK )
       << BUILD_LOG( kPacket_.m_dwUID )
       << BUILD_LOGc( kPacket_.m_ucMethod ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy )
{
    KNGuildJoinPolicy kPacket;
    kPacket = kPacket_.m_kPolicy;

    SiKGSSimLayer()->m_kActorManager.QueueingEventTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_CHANGE_GUILD_JOIN_SETTING_ACK, kPacket );

    START_LOG( clog, L"센터서버로부터 길드 가입승인 방법 변경 결과패킷 받음." )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kPolicy.m_nOK )
        << BUILD_LOG( kPacket_.m_kPolicy.m_dwUID )
        << BUILD_LOGc( kPacket_.m_kPolicy.m_ucMethod ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_CONFIG_NOT, KGuildConfig )
{
    // 센터서버 접속시/길드설정 스크립트 다시 읽으면, 길드설정 정보 전달.
    SiKGuildManager()->SetGuildConfig( kPacket_ );

    START_LOG ( cerr, L"길드 설정 정보 전송됨.." )
        << BUILD_LOG( kPacket_.m_strMarkServerURL )
        << BUILD_LOG( kPacket_.m_setExt.size() )
        << BUILD_LOG( kPacket_.m_strDefaultMarkName )
        << BUILD_LOG( kPacket_.m_prMarkSizeLimit.first )
        << BUILD_LOG( kPacket_.m_prMarkSizeLimit.second )
        << BUILD_LOG( kPacket_.m_dwMarkUploadItemID )
        << BUILD_LOG( kPacket_.m_nMarkUploadItemFactor )
        << BUILD_LOG( kPacket_.m_nMarkUploadItemType )
        << BUILD_LOG( kPacket_.m_dwChangGuildNameItemID )
        << BUILD_LOG( kPacket_.m_nGuildPageSize )
        << BUILD_LOG( kPacket_.m_prGuildNameLength.first )
        << BUILD_LOG( kPacket_.m_prGuildNameLength.second )
        << BUILD_LOG( kPacket_.m_dwGuildMasterMinLevel )
        << BUILD_LOG( kPacket_.m_nGuildCreateGP )
        << BUILD_LOG( kPacket_.m_prGuildURLLength.first )
        << BUILD_LOG( kPacket_.m_prGuildURLLength.second )
        << BUILD_LOG( kPacket_.m_nOfficailGuildMemberNum )
        << BUILD_LOG( kPacket_.m_nNewbieGuildRemoveTerm ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_BROAD_NOT, DWORD )
{
    UCHAR ucGrade = KNGuildInfo::GG_OFFICIAL;
    SiKGuildManager()->ChangeGuildGrade( kPacket_, ucGrade );
    START_LOG( cerr, L"길드등급이 신생길드에서 정규길드로 변경됨. GuildUID : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK )
{
    // 길드가 승격 되어 길마에게 아이템을 넣어 준다.
    // 센터에서 대기 아이템에 넣었기 때문에. 여기서는 아이템을 인벤에 넣어 주는 동작을 하고
    // 길마에게 아이템을 전달 해준다.

    // 캐릭터 분리 이후에는 센터에서 대기 아이템을 넣지 않고 SDGA 값만을 변경하고
    // 아이템을 인벤에 넣어주는 동작은 이후 게임 서버에서 전부 처리한다
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( kPacket_.m_dwGuildMasterUID ) );
    if ( NULL != spUser ) {
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT, L"", spUser->GetUID(), spUser->GetCurrentCharType(), kPacket_ );

        START_LOG( clog, L"길드승격에 따른 아이템 보상받음 길마에게 알림." )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
            << BUILD_LOG( kPacket_.m_WUID ) << END_LOG;
    }
    else {
        START_LOG( clog, L"길드승격에 따른 아이템 보상받음 길마에게 알리...려는데 길마가 서버에 없음." )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
            << BUILD_LOG( kPacket_.m_WUID ) << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_INVITE_GUILD_USER_ACK )
{
    for( int i = 0 ; i < (int)kPacket_.m_vecUserUID.size() ; ++i )
    {
        DWORD dwUserUID = kPacket_.m_vecUserUID[i];
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( dwUserUID ) );

        if( spUser == NULL )
            continue;

        spUser->SendPacket( KUserEvent::EVENT_INVITE_NOT, kPacket_.m_kInviteInfo );
    }
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_POINT_BROAD, KECN_UPDATE_GUILD_POINT_BROAD )
{
    //KGuildPointBroad kBroad;// = kPacket_;

    //kBroad.m_GuildUID = kPacket_.m_GuildUID;    
    //kBroad.m_dwUserUID = kPacket_.m_dwUserUID;   
    //kBroad.m_dwDiffPoint = kPacket_.m_dwDiffPoint; 
    //kBroad.m_dwTotalPoint = kPacket_.m_dwTotalPoint;
    //kBroad.m_dwBattlePoint = kPacket_.m_dwBattlePoint;
    //kBroad.m_dwUserGuildPoint = kPacket_.m_dwUserGuildPoint;

    SiKGuildManager()->UpdateGuildPointFromCenter( kPacket_ );
}

IMPL_ON_FUNC( ECN_GUILD_INFO_LIST_NOT )
{
    // 길드 정보 생성
    //SiKGuildManager()->AddGuild( kPacket_ );
    //START_LOG( clog, L"센터서버에서 길드정보 받음. kPacket_.size() : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_GUILD_NOTICE_LIST_NOT )
{
    // 길드 공지 생성
    //std::map< DWORD, std::vector<KNGuildNotice> >::iterator mit;
    //for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
    //    LIF( SiKGuildManager()->SetGuildNotice( mit->first, mit->second ) );
    //}

    //START_LOG( clog, L"센터서버에서 길드공지 받음. kPacket_.size() : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_GUILD_MEMBER_LIST_NOT )
{
    // 온라인 상태의 길드 맴버 정보 설정
    std::map< DWORD, std::map< DWORD, KNGuildUserInfo > >::iterator mit;
    for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        LIF( SiKGuildManager()->SetGuildMemberList( mit->first, mit->second ) );
    }

    START_LOG( clog, L"센터서버에서 길드맴버 받음. kPacket_.size() : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SQUARE_USER_SYNC_ACK )
{
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kUserPos.m_dwUID ) );

    if ( !spUser ) {
        START_LOG( clog, L"대상 유저가 이 서버에는 존재하지 않음.. UID : " << kPacket_.m_kUserPos.m_dwUID ) << END_LOG;
        return;
    }

    spUser->SendPacket( KUserEvent::EVENT_SQUARE_USER_SYNC_ACK, kPacket_.m_vecPosUID );    
}

_IMPL_ON_FUNC( ECN_UPDATE_NEW_DONATION_ACK, MAP_DWORD_INT )
{
    SiKNewDonationManager()->UpdateTotalDonationFromCenter( kPacket_ );
    START_LOG( clog, L"센터 에서 받은 총 기부 NPC Size.. : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SOCKET_ITEM_GP_TABLE_NOT )
{
    SiKItemManager()->m_kSocketCashItem.UpdateItemTable( kPacket_ );
    START_LOG( clog, L"센터에서 받은  소켓 관련 소모 아이템, GP table size : " << kPacket_.m_mapItemUseCount.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName )
{
    LIF( SiKGuildManager()->UpdateMemberNick( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_strNickName, kPacket_.m_strNickColor ) );
    START_LOG( clog, L"길드 맴버 닉네임 변경 알림. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_strNickColor )<< END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter )
{
    LIF( SiKGuildManager()->SendMemberLevelupNotice( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_cCharType, kPacket_.m_cLevel ) );
    START_LOG( clog, L"길드 맴버 레벨업 알림. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cLevel ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_INVITE_GUILD_NOT, KGuildInvite )
{
    // 대상 유저가 있는가?
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwToUserUID );
    if ( !spUser ) {
        START_LOG( cwarn, L"초대할 유저가 이 게임서버에 없음. UserUID : " << kPacket_.m_dwToUserUID ) << END_LOG;
        return;
    }

    // 유저에게 초대 패킷 보냄.
    spUser->SendPacket( KUserEvent::EVENT_INVITE_GUILD_NOT, kPacket_ );

    START_LOG( clog, L"센터서버에서 길드 초대패킷 받음." )
        << BUILD_LOG( kPacket_.m_dwToUserUID )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName )
        << BUILD_LOGc( kPacket_.m_ucGrade )
        << BUILD_LOG( kPacket_.m_strFromUserNickName ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CLIENT_PING_CONFIG_NOT, KPingConfig )
{
    SiKGSSimLayer()->SetPingConfig( kPacket_ );
    START_LOG( clog, L"센터 서버로 부터 Ping 설정 정보 받음." ) << END_LOG;
}

//_IMPL_ON_FUNC( ECN_ONEADAY_UPDATE_ACK, VEC_ONEADAYITEM )
//{
//    SiKOneaday()->SetInfo( kPacket_ );
//}

IMPL_ON_FUNC( ECN_USER_SERVER_ROOM_REQ )
{
    // 이 서버에 있는 특정 유저의 룸 정보 요청.
    // 정보 찾아서 전달해줄수 있도록 하자.
    KChannel* pkChannel;
    KRoom* pkRoom;
    KUserPtr spUser;

    KECN_USER_SERVER_ROOM_ACK kCnPacket;
    kCnPacket.m_nOK = -99;
    kCnPacket.m_kSendUser = kPacket_.m_kSendUser;
    kCnPacket.m_kServerRoomInfo = kPacket_.m_kTargetUser;

    // 대상 유저가 있는가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kTargetUser.m_dwUserUID );
    if( !spUser ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_03, END_PROC );
    }

    // 채널 확인
    pkChannel = spUser->GetChannel();
    if ( pkChannel == NULL ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_01, END_PROC );
    }

    // 룸 확인
    pkRoom = spUser->GetRoom();
    if ( pkRoom == NULL ) {
        SET_ERR_GOTO( ERR_SERVER_ROOM_02, END_PROC );
    }

    kCnPacket.m_kServerRoomInfo.m_dwServerUID = SiKGameServer()->GetUID();
    kCnPacket.m_kServerRoomInfo.m_dwChannelUID = pkChannel->GetUID();
    kCnPacket.m_kServerRoomInfo.m_usRoomID = pkRoom->GetRoomID();
    kCnPacket.m_kServerRoomInfo.m_iGameMode = pkRoom->m_iGameMode;
    kCnPacket.m_kServerRoomInfo.m_bPublic = pkRoom->m_bPublic;
    SET_ERROR( NET_OK );

END_PROC:
    kCnPacket.m_nOK = NetError::GetLastNetError();
    SendPacket( KCenterEvent::ECN_USER_SERVER_ROOM_ACK, kCnPacket );

    _LOG_CONDITION( kCnPacket.m_nOK == 0, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_dwUserUID )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_dwServerUID )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_dwChannelUID )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_usRoomID )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_iGameMode )
        << BUILD_LOG( kCnPacket.m_kServerRoomInfo.m_bPublic ) << END_LOG;
}

IMPL_ON_FUNC( ECN_USER_SERVER_ROOM_ACK )
{
    // 전체 서버에 특정 유저의 서버/ 룸 정보요청에 대한 응답.
    // 에러값 처리를 여기서 하자. 정확한 메세지가 남도록!
    NetError::SetLastNetError( kPacket_.m_nOK );

    KEVENT_USER_SERVER_ROOM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kServerRoomInfo = kPacket_.m_kServerRoomInfo;

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,             kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SERVER_ROOM_00, kPacket.m_nOK, 1 ); // 요청 주기가 짧습니다.
        ERR_CASE( ERR_SERVER_ROOM_01, kPacket.m_nOK, 2 ); // 대상이 채널에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_02, kPacket.m_nOK, 3 ); // 대상이 방에 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_03, kPacket.m_nOK, 4 ); // 대상 유저를 찾을 수 없습니다.
        ERR_CASE( ERR_SERVER_ROOM_04, kPacket.m_nOK, 5 ); // 대상이 같은 서버에 없습니다.
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kSendUser.m_dwUserUID );
    if ( spUser ) {
        spUser->SendPacket( KUserEvent::EVENT_USER_SERVER_ROOM_ACK, kPacket );
    }
    else {
        START_LOG( cwarn, L"정보요청한 유저가 게임서버에 없음. UserUID : " << kPacket_.m_kSendUser.m_dwUserUID ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwUserUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwServerUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_dwChannelUID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_usRoomID )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_iGameMode )
        << BUILD_LOG( kPacket.m_kServerRoomInfo.m_bPublic ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT )
{
    SiKItemManager()->SetBuyLimitItem( kPacket_ );
    START_LOG( clog, L"구매제한 아이템 리스트 업데이트.. size : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SPHINX_DATA_NOT )
{
    START_LOG( cerr, L" 스핑크스 OX 데이터 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOGtm( CTime(kPacket_.m_tmBegin) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmEnd) )
        << BUILD_LOG( kPacket_.m_mapCompressedSphinxQuestionInfo.size() ) << END_LOG;

    SiKSphinx()->UpdateSphinxData( kPacket_ );
}

_IMPL_ON_FUNC( ECN_CREATE_PARTY_ASK, KPartyData )
{
    
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kInvitee.m_dwUID );
    
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_CREATE_PARTY_ASK, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"파티 요청할 유저가 게임서버에 없음. UserUID : " << kPacket_.m_kInvitee.m_dwUID ) << END_LOG;
    }

}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_ASK, KPartyData )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kInvitee.m_dwUID );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_INVITE_PARTY_ASK, kPacket_ );

        START_LOG( clog, L"파티 초대 요청, Party UID : " << kPacket_.m_dwPartyUID )
            << BUILD_LOG( kPacket_.m_kInvitee.m_dwUID ) << END_LOG;
    }
    else {
        START_LOG( cwarn, L"파티 초대할 유저가 게임서버에 없음. UserUID : " << kPacket_.m_kInvitee.m_dwUID ) << END_LOG;
    }

}

_IMPL_ON_FUNC( ECN_CREATE_PARTY_ACK, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_CREATE_PARTY_ACK, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }

}

_IMPL_ON_FUNC( ECN_LEAVE_PARTY_BROAD, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_LEAVE_PARTY_BROAD, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }
    
}

_IMPL_ON_FUNC( ECN_CHANGE_PARTY_HOST_BROAD, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_CHANGE_PARTY_HOST_BROAD, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }

}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_ROOM_REQ, KPartyRoomData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_kPartyData.m_vecPartyPeople.begin(); vit != kPacket_.m_kPartyData.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->SendPacket( KUserEvent::EVENT_INVITE_PARTY_ROOM_BROAD, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }

}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_ACK, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_INVITE_PARTY_BROAD, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }

}

_IMPL_ON_FUNC( ECN_BREAK_PARTY_NOT, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_BREAK_PARTY_NOT, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }    
}

_IMPL_ON_FUNC( ECN_BAN_PARTY_USER_NOT, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_BAN_PARTY_USER_NOT, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"강퇴당할 파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID ) << END_LOG;
        }
    }    
    
}

_IMPL_ON_FUNC( ECN_NOTICE_NOT, std::vector<KSerBuffer> )
{
    SiKStrengthManager()->SetNoticeInfo( kPacket_ );
}

_IMPL_ON_FUNC( ECN_INVITE_AGIT_NOT, KInviteAgitNot )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID );
    if ( spUser != NULL ) {
        spUser->QueueingEvent( KUserEvent::EVENT_INVITE_AGIT_NOT, kPacket_ );
    }

    START_LOG( clog, L"Invite Agit Notice.. AgitUID : " << kPacket_.m_dwAgitUID << L", UserUID : " << kPacket_.m_dwUserUID << L", OwnerNick : " << kPacket_.m_strOwnerNick ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SHUTDOWN_USER_CONFIG_NOT, KShutdownUserConfig )
{
    SiKShutdownUser()->SetConfig( kPacket_ );
    START_LOG( cerr, L"Update Shutdown User Config.." ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SURVEY_DATA_NOT )
{
    START_LOG( cerr, L"설문 데이터 서버로 전달됨..." )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOGtm( CTime(kPacket_.m_tmBegin) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmEnd) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmReturnTime) )
        << BUILD_LOG( kPacket_.m_mapCompressedSurveyInfoPack.size() )
        << BUILD_LOG( kPacket_.m_mapSurveyTypeInfo.size() )
        << BUILD_LOG( kPacket_.m_mapSurveyRewardInfo.size() ) << END_LOG;

    SiKSurvey()->UpdateSurveyData( kPacket_ );
}

IMPL_ON_FUNC( ECN_GWC_EVENT_DATA_NOT )
{
    START_LOG( clog, L"GWC 데이터 서버로 전달됨..." )
        << BUILD_LOGtm( CTime(kPacket_.m_tmBegin) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmEnd) )
        << BUILD_LOGtm( CTime(kPacket_.m_tmDBUpdateTime) )
        << BUILD_LOG( kPacket_.m_mapGradeRewardInfo.size() )
        << BUILD_LOG( kPacket_.m_mapRankingRewardInfo.size() )
        << BUILD_LOG( kPacket_.m_mapTotalCountryInfo.size() ) << END_LOG;

    SiKGWCEvent()->UpdateGWCEventData( kPacket_ );
}

_IMPL_ON_FUNC( ECN_GWC_NOTICE_NOT, DWORD )
{
    SiKGWCEvent()->SetNoticeInfo( kPacket_ );
    START_LOG( clog, L"GWC 이벤트 공지 전달" ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_ITEM_UPDATE_NOT, bool )
{
    SiKGSDBLayer()->QueueingID( KUserEvent::EVENT_DB_UPDATE_SELL_ITEM_LIST_REQ, NULL, 0, 0 );
    START_LOG( clog, L"아이템 업데이트 정보 갱신 알림 전달" ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_RECEIVE_LETTER_NOT, DWORD )
{
    // 대상 유저 찾아서 새 우편 알림
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_ );
    if ( spUser != NULL ) {
        spUser->QueueingEvent( KUserEvent::EVENT_RECEIVE_LETTER_NOT, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"새 우편 알릴 대상유저를 찾을 수 없음. UserUID : " << kPacket_ ) << END_LOG;
    }
    START_LOG( clog, L"새 우편 유저에게 알림.. UserUID : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ADVENTURE_DATA_NOT )
{
    SiKAdventure()->SetAdventureEventData( kPacket_ );
    START_LOG( cerr, L"센터로부터 모험 이벤트 정보 받아 갱신" ) << END_LOG;
}

IMPL_ON_FUNC( ECN_OLYMPIC_RANK_DATA_NOT )
{
    START_LOG( cerr, L"올림픽 이벤트 랭킹 데이터 서버로 전달됨." )
        << BUILD_LOG( kPacket_.m_vecRegDateRank.size() )
        << BUILD_LOG( kPacket_.m_vecTotalRank.size() ) 
        << END_LOG;

    SiKOlympicManager()->UpdateOlympicRankData( kPacket_.m_vecRegDateRank, kPacket_.m_vecTotalRank );
}

IMPL_ON_FUNC( ECN_OLYMPIC_SYNC_SOLO_DATA_NOT )
{
    START_LOG( cerr, L"올림픽 이벤트 개인전 데이터 서버로 전달됨." )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) ) 
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_nTopPlayerData )
        << END_LOG;

    SiKOlympicManager()->UpdateOlympicSoloData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nTopPlayerData, kPacket_.m_strNickName );
}

IMPL_ON_FUNC( ECN_OLYMPIC_SYNC_TEAM_DATA_ACK )
{
    START_LOG( cerr, L"올림픽 이벤트 단체전 데이터 서버로 전달됨." )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) ) 
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_biTotalData )
        << END_LOG;

    SiKOlympicManager()->UpdateOlympicTeamData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_biTotalData );
}

IMPL_ON_FUNC( ECN_OLYMPIC_EVENT_DATA_NOT )
{
    START_LOG( cerr, L"올림픽 이벤트 데이터 서버로 전달됨..." )
        << BUILD_LOGtm( CTime(kPacket_.m_tmRegDate) )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_biTotalData )
        << BUILD_LOG( kPacket_.m_vecRegDateRank.size() )
        << BUILD_LOG( kPacket_.m_vecTotalRank.size() ) << END_LOG;

    SiKOlympicManager()->UpdateOlympicEventData( kPacket_ );
}

_IMPL_ON_FUNC( ECN_PARTY_USER_READY_BROAD, KPartyData )
{
    std::vector<KPartyMember>::iterator vit;
    for ( vit = kPacket_.m_vecPartyPeople.begin(); vit != kPacket_.m_vecPartyPeople.end(); ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( spUser ) {
            spUser->QueueingEvent( KUserEvent::EVENT_PARTY_USER_READY_BROAD, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"파티원이 게임서버에 없음. UserUID : " << vit->m_dwUID )
                << END_LOG;
        }
    }
}

IMPL_ON_FUNC( ECN_BUFF_DONATION_TOTAL_DATA_NOT )
{
    KEVENT_BUFF_DONATION_TOTAL_DATA_NOT kPacket;
    kPacket.m_biTotalDonationPoint = kPacket_.m_biTotalDonationPoint;
    kPacket.m_nBuffStateFlag = kPacket_.m_nBuffStateFlag;

    // 게임서버만 갱신하는 패킷인지 확인
    if ( FLAG_SYNC_ONLY_GAME_SERVER != kPacket_.m_nBuffStateFlag && FLAG_INVALID != kPacket_.m_nBuffStateFlag ) {
        // 플래그가 FLAG_BUFF_START_NOT 일 경우 버프 발동 운영자 공지 후 클라 전달용 플래그 세팅
        if ( FLAG_BUFF_START_NOT == kPacket.m_nBuffStateFlag ) {
            SiKBuffDonation()->NotifyBuffStartMsgFromCenter();
            kPacket.m_nBuffStateFlag = FLAG_BUFF_ON;
        }
        // 발렌타인 객체에 유저 전체한테 NOT 요청
        SiKBuffDonation()->NotifyDonationInfoToAllUser( kPacket );
    }
    else { // 버프ON/OFF/NOT가 아닌 경우 현재 서버가 알고 있는 값으로 적용
        kPacket.m_nBuffStateFlag = SiKBuffDonation()->GetBuffFlag();
    }

    // 게임서버 발렌타인기부 객체에 update
    SiKBuffDonation()->UpdateTotalDonationPointFromCenter( kPacket );

    START_LOG( clog, L"센터로부터 발렌타인 버프 전체 기부 데이터 얻음 " )
        << BUILD_LOG( kPacket.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket.m_nBuffStateFlag )
        << END_LOG;
}

IMPL_ON_FUNC( ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT )
{
    SiKAccumulateAttendanceManager()->UpdateInfoFromCenter( kPacket_.m_tmBegin, kPacket_.m_tmEnd, kPacket_.m_nVersion );

    START_LOG( clog, L"센터로부터 누적 출석 DB세팅 데이터 얻음 " )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOGtm( CTime( kPacket_.m_tmBegin ) )
        << BUILD_LOGtm( CTime( kPacket_.m_tmEnd ) )
        << END_LOG;
}

_IMPL_ON_FUNC( ECN_CASHBACK_RATIO_INFO_NOT, KCashbackRatioInfo )
{
    START_LOG( clog, L"센터로부터 캐시백 비율 정보 얻고 클라에 전달" )
        << BUILD_LOG( kPacket_.m_mapCashbackDefaultRatio.size() )
        << BUILD_LOG( kPacket_.m_mapCashbackEventRatio.size() )
        << END_LOG;

    // 접속중인 모든 클라이언트 들에게도 패킷 전송
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_CASHBACK_RATIO_INFO_NOT, kPacket_, true );
}

_IMPL_ON_FUNC( ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT )
{
    SiKUpdatePlanManager()->UpdateClientContentsOpenInfoNotFromCenter( kPacket_ );

    START_LOG( clog, L"센터로부터 클라이언트 컨텐츠 오픈 정보 얻음 " )
        << BUILD_LOG( kPacket_.m_nSendType )
        << BUILD_LOG( kPacket_.m_prGameCategoryInfo.first.size() )
        << BUILD_LOG( kPacket_.m_prGameCharInfo.first.size() )
        << BUILD_LOG( kPacket_.m_prMonsterInfo.first.size() )
        << BUILD_LOG( kPacket_.m_prMiniEventInfo.first.size() )
        << END_LOG;

    // 데이터 변경인 경우 접속중인 모든 클라이언트 들에게 패킷 전송
    if ( KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_DATA_CHANGED == kPacket_.m_nSendType ) {
        SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_CLIENT_CONTENTS_OPEN_INFO_NOT, kPacket_, true );
    }
}

_IMPL_ON_FUNC( ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT )
{
    SiKUpdatePlanManager()->UpdateClientContentsFirstInitInfoNotFromCenter( kPacket_ );

    START_LOG( clog, L"센터로부터 클라이언트 컨텐츠 초기화 정보 얻음 " )
        << BUILD_LOG( kPacket_.m_nSendType )
        << BUILD_LOG( kPacket_.m_prUsingFilesInfo.first.size() )
        << BUILD_LOG( kPacket_.m_prHackCheckInfo.first.size() )
        << END_LOG;

    // 데이터 변경인 경우 접속중인 모든 클라이언트 들에게 패킷 전송
    if ( KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_DATA_CHANGED == kPacket_.m_nSendType ) {
        SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, kPacket_, true );
    }
}

_IMPL_ON_FUNC( ECN_ITEM_BUY_INFO_NOT, KItemBuyInfo )
{
    SiKUpdatePlanManager()->UpdateItemBuyInfoNotFromCenter( kPacket_ );

    START_LOG( clog, L"센터로부터 아이템 구매 가능 정보 얻음 " )
        << BUILD_LOG( kPacket_.m_bBuyEnable )
        << BUILD_LOG( kPacket_.m_vecItemRange.size() )
        << BUILD_LOG( kPacket_.m_mapRecommInfo.size() )
        << END_LOG;

    // 접속중인 모든 클라이언트 들에게도 패킷 전송
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_ITEM_BUY_INFO_NOT, kPacket_, true );
}

_IMPL_ON_FUNC( ECN_EVENT_CHANGED_EVENT_STATE_INFO_NOT, KEVENT_UPDATE_EVENT_STATE_NOT )
{
    // 이벤트 시작/종료 될 때 받는 패킷이므로 데이터 키핑 후 클라에 바로 쏴주자
    SiKUpdatePlanManager()->UpdateEventStateListFromCenter( kPacket_ );

    START_LOG( clog, L"센터로부터 이벤트 ON/OFF 정보 컨테이너 얻음" )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;

    // 접속중인 모든 클라이언트 들에게도 패킷 전송
    //SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_UPDATE_EVENT_STATE_NOT, kPacket_, true );
}

IMPL_ON_FUNC( ECN_PLAN_SCRIPT_UPDATE_NOT )
{
    std::vector< int > vecEventID;

    SiKUpdatePlanManager()->UpdateScriptFilesFromCenter( kPacket_ , vecEventID );

    START_LOG( clog, L"센터로부터 스크립트 ON/OFF 정보 컨테이너 얻음" )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( vecEventID.size() )
        << END_LOG;

    // 접속중인 모든 클라이언트 들에게도 패킷 전송
    if ( false == vecEventID.empty() ) {
        SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_UPDATE_EVENT_SCRIPT_NOT, vecEventID, true );
    }
}

_IMPL_ON_FUNC( ECN_CYOU_CHANGE_CASH_ACK, KCYOUCashPoint )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_CYOU_CASH_CHANGE_ACK, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"중국 포인트 충전 요청 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwUserUID )
            << END_LOG;
    }

}

_IMPL_ON_FUNC( ECN_CYOU_GET_CURRENT_CASH_ACK, KCYOUCashPoint )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_CYOU_GET_WEB_POINT_ACK, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"중국 잔액조회 요청 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwUserUID )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_CYOU_USER_LOGIN_ACK )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUID );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_CYOU_LOGIN_ACK, kPacket_.m_nOK );
    }
    else {
        START_LOG( cwarn, L"중국 서버 접속 시도 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwUID )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_CYOU_USER_HEART_BEAT_ACK )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUID );
    if ( spUser ) {
        spUser->QueueingEvent( KUserEvent::EVENT_CYOU_USER_HEART_BEAT_ACK, kPacket_.m_nOK );
    }
    else {
        START_LOG( cwarn, L"중국 하트비트 유저가 게임서버에 없음. UserUID : " << kPacket_.m_dwUID )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_CONNECTION_GIFTBOX_INFO_NOT )
{
    START_LOG( clog, L"센터로부터 접속 선물상자 정보 받음" ) << END_LOG;

    SiKConnectionGiftBox()->SetFromConnectionGiftBoxInfoNot( kPacket_ );
}

_IMPL_ON_FUNC( ECN_EVENT_PACKAGE_LIST_CHANGED_NOT, bool )
{
    SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_GET_PACKAGE_LIST_NOT, L"", 0 );

    START_LOG( cerr, L"센터 서버에서 패키지 정보 변경됨 알림 받음" ) << END_LOG;
}

IMPL_ON_FUNC( ECN_EVENT_DB_UPDATE_NOT )
{
    if( kPacket_.m_nEventType == KECN_EVENT_DB_UPDATE_NOT::ET_VIP_EVENT ) {
        SiKVipEventManager()->SendVipEventInfoReqToDB();
    }

    START_LOG( clog, L"센터로부터 DB 이벤트 변경 정보 전달받음" )
        << BUILD_LOG( kPacket_.m_nEventType )
        << END_LOG;
}

IMPL_ON_FUNC( ECN_TONG_DONATION_INFO_NOT )
{
    SiKTongDonation()->SetFromTongDonationInfoNot( kPacket_ );

    START_LOG( clog, L"센터로부터 통합 기부 이벤트 정보 컨테이너 얻음" ) << END_LOG;
}

// 센터에서 받은걸로 데이터 세팅
IMPL_ON_FUNC( ECN_SOCIAL_COMMERCE_INIT_DATA_NOT )
{
    // DB -> Center -> Game 소셜 커머스 설정 정보 세팅
    SiKSocialCommerce()->UpdateSocialCommerceDataFromCenter( kPacket_ );
    // DB에서 현재 아이템별 구매 수량 얻기 요청
    SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_SOSCIAL_COMMERCE_BUY_COUNT_INFO, NULL, 0 );
}

IMPL_ON_FUNC( ECN_DUNGEON_RANK_RANKING_CONFIG_NOT )
{
    SiKDungeonRankManager()->SetRankingConfig( kPacket_ );

    START_LOG( clog, L"센터로부터 던전 랭킹 시스템 설정 정보 얻음" ) << END_LOG;
}

IMPL_ON_FUNC( ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT )
{
    SiKDungeonRankManager()->SetCurrentDungeonRankList( kPacket_ );

    START_LOG( clog, L"센터로부터 던전 랭킹 시스템 현재 시즌 랭킹 데이터 얻음" ) << END_LOG;
}

IMPL_ON_FUNC( ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT )
{
    SiKDungeonRankManager()->SetPreviousDungeonRankList( kPacket_ );

    START_LOG( clog, L"센터로부터 던전 랭킹 시스템 이전 시즌 랭킹 데이터 얻음" ) << END_LOG;

    DWORD dwTemp = 0;
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_UPDATED_NOT, dwTemp );
}

_IMPL_ON_FUNC(ECN_BILLBOARD_DATA_BROAD, KECNBillboardChat)
{
    KEVENT_BILLBOARD_CHAT_MSG_NOT kPacket;
    kPacket.m_wstrNickName = kPacket_.Nickname;
    kPacket.m_dwColor = kPacket_.MessageColor;
    kPacket.m_wstrMessage = kPacket_.Message;
    SiKGSSimLayer()->m_kActorManager.QueueingToAll(KUserEvent::EVENT_BILLBOARD_CHAT_MSG_NOT, kPacket);
}