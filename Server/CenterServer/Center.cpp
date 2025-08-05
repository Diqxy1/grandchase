#include "Center.h"
#include "FSM/FSMclass.h"
#include "dbg/dbg.hpp"
#include "CenterEvent.h"
#include "DefaultFSM.h"
#include "CenterSimLayer.h"
#include "CenterServer.h"
#include "CenterDBThread.h"
#include "CnDonationManager.h"
#include "CnRainBow.h"
#include "CnMissionEvent.h"
#include "CnSubscriptionManager.h"
#include "CnNpcGift.h"
#include "CnDicePlay.h"
#include "CnSquare.h"
#include "CnSquareManager.h"
#include "CnGuildManager.h"
#include "CnQuizBingo.h"
#include "CnFBAuth.h"
#include "CnCouponManager.h"
#include "CnGWCSupport.h"
#include "CnNewDonationManager.h"
#include "CnItemManager.h"
#include "CnSphinx.h"
#include "CnSurvey.h"
#include "CnPartyManager.h"
#include "CnShutdownUser.h"
#include "NUSAuth.h"
#include "CnClientContentsManager.h"
#include "CnGWCEvent.h"
#include "Log4.h"
#include "CnAdventure.h"
#include "CnOlympicManager.h"
#include "CnAdventure.h"
#include "CnCYOUAuth.h"
#include "CnBuffDonation.h"
#include "CnAccumulateAttendance.h"
#include "CnUpdatePlanManager.h"
#include "CnConnectionGiftBox.h"
#include "CenterLogManager.h"
#include "CnTongDonation.h"
#include "CnSocialCommerce.h"
#include "CnDungeonRankManager.h"

namespace {

    struct KGetConnectServerUID
    {
        DWORD m_dwServerUID;
        DWORD m_dwUID;

        KGetConnectServerUID( DWORD dwUID ):m_dwUID(dwUID),m_dwServerUID(0){};
        void operator() ( const KActorPtr spActor )
        {
            if( !spActor ) return;
            KCenterPtr spCenter = std::static_pointer_cast<KCenter>( spActor );
            if( spCenter->IsConnectedUser( m_dwUID ) )
            {
                m_dwServerUID = spCenter->GetUID();
            }
        }
        operator DWORD() const { return m_dwServerUID; }
    };
}

NiImplementRTTI( KCenter, KActor );
#define EVENT_TYPE KCenterEvent
#define CLASS_TYPE KCenter

#define QUEUEING_EVENT_TO_DB( id, data ) \
    SiKCenterDBMgr()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, -1, data )
#define QUEUEING_ID_TO_DB( id ) \
    SiKCenterDBMgr()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, -1 )

int KCenter::ms_iUID = INT_MAX;

KCenterPtr KCenter::Create()
{
    return SiKCenterSimLayer()->m_kActorManager.CreateActor<KCenter>();
}

void KCenter::AddManager( KCenterPtr spCenter_ )
{
    JIF( spCenter_ );
    spCenter_->SetUID( --ms_iUID );
    JIF( SiKCenterSimLayer()->m_kActorManager.Add( spCenter_ ) );
    START_LOG( clog, L"게임서버의 temp UID : " << ms_iUID ) << END_LOG;
}

KCenter::KCenter(void)
{
    SetSendQueueSize( 1024 * 512 );
}

KCenter::~KCenter(void)
{
}

void KCenter::OnDestroy()
{
    KSession::OnDestroy();

    START_LOG( clog, L"Name : " << m_strName ) << END_LOG;

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_LOGINED:    // 올바르게 종료될 수 있는 상태
        StateTransition( KDefaultFSM::INPUT_EXIT_GAME );
    	break;
    default:                            // 상태 변환이 올바르지 않음. 로그를 남기고 강제 상태 변경.
        START_LOG( cwarn, GetStateIDString() ) << END_LOG;
        ForceStateTransitionTo( KDefaultFSM::STATE_EXIT );
    }
}

void KCenter::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
    CASE( ECN_VERIFY_ACCOUNT_REQ );
    CASE( ECN_VERIFY_ACCOUNT_ACK );
   _CASE( ECN_LOUD_MSG_REQ, KSerBuffer );
   _CASE( ECN_REPETITION_PROVIDE_CANCEL_REQ, KECN_REPETITION_PROVIDE_REQ );
   _CASE( ECN_MINIGAME_RANK_REQ, KMiniGameRankList );
   _CASE( ECN_UPDATE_DONATION_REQ, KDotantionSyncData );
   _CASE( ECN_SERVER_RELAY_PACKET_REQ, KServerRelayPacket );
    CASE( ECN_ADD_USER_REQ );
    CASE( ECN_DEL_USER_REQ );
   _CASE( ECN_ADD_MULTI_USER_REQ, std::vector<DWORD> );
   _CASE( ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_REQ, PAIR_INT );
   _CASE( ECN_UPDATE_DICE_PLAY_REQ, KDicePlaySyncData );
   _CASE( ECN_ENTER_SQUARE_REQ, KSquareUserInfo );
   _CASE( ECN_LEAVE_SQUARE_NOT, DWORD );
   _CASE( ECN_UPDATE_USER_SQUARE_POS_NOT, KSquarePos );
   _CASE( ECN_NEW_SQUARE_USER_NOT, KSquareUserInfo );
    CASE( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT );
    CASE( ECN_LEAVE_SQUARE_BROAD );
   _CASE( ECN_SQUARE_RELAY_PACKET_REQ, KSquareRelayPacket );
    CASE( ECN_ENTER_GUILD_SQUARE_REQ );
   _CASE( ECN_ENTER_MINIGAME_NOT, DWORD );
   _CASE( ECN_EXIT_MINIGAME_NOT, DWORD );
    CASE( ECN_ADD_GUILD_USER_REQ );
   _CASE( ECN_ADD_GUILD_USER_ACK, KECN_ADD_GUILD_USER_REQ );
    CASE( ECN_CHAT_REQ );
    CASE( ECN_CHAT_ACK );
   _CASE( ECN_ADD_NEW_GUILD_NOT, KNOneGuildData );
    CASE( ECN_DRUMOUT_GUILD_USER_NOT );
    CASE( ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT );
   _CASE( ECN_GUILD_MARK_REG_STATUS_REQ, PAIR_DWORD_UCHAR );
   _CASE( ECN_SQUARE_LIST_REQ, PAIR_DWORD );
    CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT );
    CASE( ECN_UPDATE_GUILD_NOTICE_NOT );
    CASE( ECN_UPDATE_GUILD_NAME_NOT );
   _CASE( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL );

    CASE( ECN_JOIN_GUILD_REQ );
    CASE( ECN_JOIN_GUILD_ACK );
   _CASE( ECN_CANCEL_JOIN_GUILD_REQ, PAIR_DWORD );
    CASE( ECN_CANCEL_JOIN_GUILD_ACK );
    CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ );
    CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
   _CASE( ECN_SELF_DRUMOUT_GUILD_REQ, PAIR_DWORD );
    CASE( ECN_SELF_DRUMOUT_GUILD_ACK );
    CASE( ECN_ACCEPT_GUILD_JOINER_REQ );
    CASE( ECN_ACCEPT_GUILD_JOINER_ACK );
    CASE( ECN_REJECT_GUILD_JOINER_REQ );
    CASE( ECN_REJECT_GUILD_JOINER_ACK );
    CASE( ECN_BREAKUP_GUILD_REQ );
    CASE( ECN_BREAKUP_GUILD_ACK );
   _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_NOT, KNGuildJoinPolicy );
   _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, KCnNGuildJoinPolicy );
   _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy );
    CASE( ECN_LOG_OUT_NOT );
    CASE( ECN_GUILD_MEMEBER_LIST_SYNC_NOT );
    CASE( ECN_INVITE_GUILD_USER_REQ );
   _CASE( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad );
   _CASE( ECN_SQUARE_USER_SYNC_REQ, KSquarePos );

   _CASE( ECN_UPDATE_NEW_DONATION_REQ, MAP_DWORD_INT );
   _CASE( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
   _CASE( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
   _CASE( ECN_INVITE_GUILD_NOT, KGuildInvite );
   _CASE( ECN_GUILD_PAGE_REQ, UCHAR );

    CASE( ECN_USER_SERVER_ROOM_REQ );
    CASE( ECN_USER_SERVER_ROOM_ACK );
   _CASE( ECN_SQUARE_LIST_ACK, PAIR_DWORD );

   _CASE( ECN_CREATE_PARTY_REQ, KPartyData );
   _CASE( ECN_CREATE_PARTY_ANSWER, KPartyData );
   _CASE( ECN_INVITE_PARTY_REQ, KPartyData );
   _CASE( ECN_INVITE_PARTY_ANSWER, KPartyData );
   _CASE( ECN_CHANGE_PARTY_HOST_REQ, KPartyData );
   _CASE( ECN_LEAVE_PARTY_REQ, KPartyData );
   _CASE( ECN_INVITE_PARTY_ROOM_REQ, KPartyRoomData );
   _CASE( ECN_BAN_PARTY_USER_REQ, KPartyData );
   _CASE( ECN_INVITE_AGIT_REQ, KInviteAgitReq );

   _CASE( ECN_NOTICE_BROAD, std::vector<KSerBuffer> );
    CASE( ECN_USER_EXIT_GAME_NOT );
   _CASE( ECN_FIND_USER_SEND_LETTER_NOT, DWORD );
    CASE( ECN_OLYMPIC_SYNC_SOLO_DATA_NOT );
    CASE( ECN_OLYMPIC_SYNC_TEAM_DATA_REQ );
   _CASE( ECN_CYOU_USER_HEART_BEAT_REQ, KCYOUAuthUserInfo );
   _CASE( ECN_CYOU_CASH_CHANGE_REQ, KCYOUCashData );
   _CASE( ECN_CYOU_CHANGE_CASH_ACK, KCYOUCashPoint );
   _CASE( ECN_CYOU_CHANGE_CASH_LOG_NOT, KCYOUChangeCashLogData );

   _CASE( ECN_CYOU_USER_AGREEMENT_REQ, KCYOUAuthUserInfo );

   _CASE( ECN_PARTY_USER_READY_REQ, KPartyData );
   _CASE( ECN_CYOU_GET_CURRENT_CASH_REQ, KCYOUCashData );

   _CASE( ECN_BUFF_DONATION_DATA_UPDATE_REQ, int );

   _CASE( ECN_CYOU_GET_CURRENT_CASH_ACK, KCYOUCashPoint );
   _CASE( ECN_CYOU_BUY_LOG_REQ, KCYOUCashData );
    
    CASE( ECN_CYOU_USER_HEART_BEAT_ACK );

   _CASE( ECN_CYOU_USER_LOGIN_REQ, KCYOUAuthUserInfo );
    CASE( ECN_CYOU_USER_LOGIN_ACK );
   _CASE( ECN_CYOU_USER_LOGOUT, KCYOUAuthUserInfo );

    CASE( ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ );

    _CASE(ECN_BILLBOARD_DATA_BROAD_REQ, KECNBillboardChat);

    // #CENTER_EVENT_CASES
    
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

void KCenter::Tick()
{
    KActor::Tick();

    switch( m_pkCurrentState->GetID() )
    {
    case KDefaultFSM::STATE_EXIT:
        SiKCenterSimLayer()->m_kActorManager.ReserveDelete( m_strName );  // 자기 자신을 스스로 삭제할 수 없음.
        break;
    case KDefaultFSM::STATE_LOGINED:
        break;
    }    
}

bool KCenter::AddConnectedUser( DWORD dwUID_ )
{
    KConnectedUser  kData;
    kData.m_dwUID = dwUID_;
    std::map< DWORD, KConnectedUser >::iterator mit;
    mit = m_mapConnectedUser.find( dwUID_ );
    if ( mit == m_mapConnectedUser.end() )
        return m_mapConnectedUser.insert( std::make_pair( dwUID_, kData) ).second;
    return false;
}

bool KCenter::DelConnectedUser( DWORD dwUID_ )
{
    std::map< DWORD, KConnectedUser >::iterator mit;
    mit = m_mapConnectedUser.find( dwUID_ );
    if( mit == m_mapConnectedUser.end() ) return false;
    m_mapConnectedUser.erase( mit );
    return true;
}

bool KCenter::IsConnectedUser( DWORD dwUID_ )
{
    return (m_mapConnectedUser.find( dwUID_ ) != m_mapConnectedUser.end() );
}

IMPL_ON_FUNC( ECN_VERIFY_ACCOUNT_REQ )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_nUserProtocolVer )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_usPort )
        << BUILD_LOG( kPacket_.m_dwUID ) << END_LOG;

    KCenterPtr spCenter;
    KECN_VERIFY_ACCOUNT_ACK kPacket;        // 전처리에서 문제가 발생한 경우 이 패킷을 보낸다.
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    // 보고된 아이피가 소켓에서 얻은 것과 다름.
    if ( KncUtil::toWideString( GetIPStr() ) != kPacket_.m_strIP ) {
        SET_ERR_GOTO( ERR_VERIFY_08, END_PROC );  // 불량 유저가 접속 시도
    }

    // 서버/클라이언트간 프로토콜이 다름
    if ( kPacket_.m_nProtocolVer != SiKCenterServer()->m_nProtocolVer ) {
        SET_ERR_GOTO( ERR_VERIFY_07, END_PROC );
    }

    // 센터서버 같은 경우 전처리 단계에서 미리 서버의 이름을 알 수 있다. 이중접속 확인을 전처리에서 한다.
    // 이중접속 시도
    spCenter = SiKCenterSimLayer()->m_kActorManager.GetByName<KCenter>( kPacket_.m_strName );
    if ( spCenter ) {
        SET_ERR_GOTO( ERR_VERIFY_04, END_PROC ); 
    }

    QUEUEING_EVENT_TO_DB( ECN_VERIFY_ACCOUNT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Ret : " << NetError::GetErrStr( kPacket.m_nOK ) )
        << BUILD_LOG( GetIPStr() )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( SiKCenterServer()->m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_nUserProtocolVer )
        << BUILD_LOG( kPacket_.m_usPort )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_dwUID ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( ECN_VERIFY_ACCOUNT_ACK );
    }
}

//-1 : 서버 리스트에 서버가 등록되어 있지 않음
//-2 : 서버 ip가 다름
//-3 : 서버 port가 다름
//-4 : 서버 프로토콜이 다름
IMPL_ON_FUNC( ECN_VERIFY_ACCOUNT_ACK )
{
    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) ) << END_LOG;

        SEND_RECEIVED_PACKET( ECN_VERIFY_ACCOUNT_ACK );
        return;
    }

    SetUID( kPacket_.m_kServerInfo.m_dwUID );
    m_kServerInfo = kPacket_.m_kServerInfo;

    // rename, UID 등록
    LIF( SiKCenterSimLayer()->m_kActorManager.Rename( GetName(), kPacket_.m_kServerInfo.m_strName ) );
    LIF( SiKCenterSimLayer()->m_kActorManager.RegByUID( *this ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_kServerInfo.m_dwUID )
        << BUILD_LOG( m_kServerInfo.m_iServerPart )
        << BUILD_LOG( m_kServerInfo.m_strName )
        << BUILD_LOG( KncUtil::toWideString( m_kServerInfo.m_strIP ) )
        << BUILD_LOG( m_kServerInfo.m_usPort )
        << BUILD_LOG( m_kServerInfo.m_nUserProtocolVer ) << END_LOG;

    kPacket_.m_vecSquares.clear();
    SiKCnSquareManager()->GetSquareList( kPacket_.m_vecSquares );
    kPacket_.m_kServerInfo.m_nMaxLevel = SiKCnClientContentsManager()->GetMaxLevel();

    SEND_RECEIVED_PACKET( ECN_VERIFY_ACCOUNT_ACK );

    // 레인 보우 이벤트 여부 상관 없이 접속시 보낸다..
    // 중간에 끝어지거나 해서 재갱신 할때 좀더 안전하게 하기 위해 
    {
        KECN_RAINBOW_UPDATE_NOT kPacket;
        SiKCnRainBow()->GetRainbowPacket( kPacket );
        SEND_DATA( ECN_RAINBOW_UPDATE_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 레인 보우 데이터 전송." ) << END_LOG;
    }

    {
        std::vector<KGameCouponInfo> vecCouponInfo;
        SiKCnCouponManager()->GetCouponInfo( vecCouponInfo );
        SEND_DATA( ECN_COUPON_INFO_NOT, vecCouponInfo );
        START_LOG( cerr, L"Center 쿠폰 정보 전송. size : " << vecCouponInfo.size() ) << END_LOG;
    }

    // 미션 이벤트 관련해서 접속시 보낸다.
    {
        KECN_MISSION_EVENT_UPDATE_NOT kPacket;
        SiKCnMissionEvent()->GetMissionEventPacket( kPacket );
        SEND_DATA( ECN_MISSION_EVENT_UPDATE_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 Mission Event Data 전송." ) << END_LOG;
    }

    // 응모권 이벤트 정보 보냄.
    {
        KECN_SUBSCRIPTION_INFO_UPDATE_NOT kPacket;
        SiKCnSubscriptionManager()->GetEventInfo( kPacket );
        SEND_COMPRESS_PACKET( ECN_SUBSCRIPTION_INFO_UPDATE_NOT, kPacket );
    }

    // 접속시, NPC 선물 이벤트 정보 보냄.
    {
        KECN_NPC_GIFT_UPDATE_NOT kPacket;
        SiKCnNpcGift()->GetEventInfo( kPacket );
        SEND_DATA( ECN_NPC_GIFT_UPDATE_NOT, kPacket );
    }

    // 아이템( 송편 ) 기부 관련해서 접속시 보낸다.
    {
        KECN_ITEM_DONATION_UPDATE_NOT kPacket;
        SiKCnDonationManager()->GetTotalGradeInfo( kPacket );
        SEND_DATA( ECN_ITEM_DONATION_UPDATE_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 아이템 기부 등급 정보 전송.") << END_LOG;
    }

    {
        KDicePlaySyncData kPacket;
        SiKCnDicePlay()->GetDicePlayRank( kPacket.m_vecDicePlayRank );
        SEND_PACKET( ECN_UPDATE_DICE_PLAY_ACK );
        START_LOG( cerr, L"Center 접속시 주사위 랭킹 데이터 전송." ) << END_LOG;
    }

    {// 길드 설정.
        KGuildConfig kPacket;
        SiKCnGuildManager()->GetGuildConfig( kPacket );
        SEND_COMPRESS_PACKET( ECN_GUILD_CONFIG_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 길드 설정 데이터 전송." ) << END_LOG;

    }

    {//길드 랭크 데이터.
        std::map< DWORD, KSerBuffer > mapGuildRank;
        SiKCnGuildManager()->GetGuildRankData( mapGuildRank );
        SEND_COMPRESS_PACKET( ECN_GUILD_RANK_NOT, mapGuildRank );
    }

    {
        KECN_QUIZ_BINGO_DATA_NOT kPacket;
        SiKCnQuizBingo()->GetQuizBingoData( kPacket );
        SEND_COMPRESS_PACKET( ECN_QUIZ_BINGO_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 퀴즈 빙고 데이터 전송." ) << END_LOG;
    }

    {
        KGWCSupportConfigData kPacket;
        SiKCnGWCSupport()->GetConfigData( kPacket );
        SEND_COMPRESS_PACKET( ECN_GWC_CONFIG_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 GWC 설정 데이터 전송." ) << END_LOG;
    }

    {
        std::vector<KGWCSupportData> kPacket;
        SiKCnGWCSupport()->GetGWCData( kPacket );
        SEND_COMPRESS_PACKET( ECN_GWC_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 GWC 데이터 전송." ) << END_LOG;

    }

    {
        KECN_SOCKET_ITEM_GP_TABLE_NOT kPacket;
        SiKCnItemManager()->m_kSocketConsume.GetTableInfo( kPacket );
        SEND_COMPRESS_PACKET( ECN_SOCKET_ITEM_GP_TABLE_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 소켓 아이템 소모 테이블 전송" ) << END_LOG;
    }

    {
        KPingConfig kPacket;
        SiKCenterSimLayer()->GetPingConfig( kPacket );
        SEND_PACKET( ECN_CLIENT_PING_CONFIG_NOT );
        START_LOG( cerr, L"Center 접속시 핑 설정 전송" ) << END_LOG;
    }

    {
        std::set<GCITEMID> setList;
        SiKCnItemManager()->GetBuyLimitItem( setList );
        SiKCnItemManager()->Send_BuyLimitItemListNot( setList );
        START_LOG( cerr, L"Center 접속시 구매제한 아이템 리스트 전송" ) << END_LOG;
    }

    {
        KECN_SPHINX_DATA_NOT kPacket;
        SiKCnSphinx()->GetSphinxData( kPacket );
        SEND_COMPRESS_PACKET( ECN_SPHINX_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 스핑크스 OX 데이터 전송." ) << END_LOG;
    }

    {
        KShutdownUserConfig kPacket;
        SiKCnShutdownUser()->GetConfig( kPacket );
        SEND_PACKET( ECN_SHUTDOWN_USER_CONFIG_NOT );
        START_LOG( cerr, L"Center 접속시 셧다운제 설정 전송" ) << END_LOG;
    }

    {
        KECN_SURVEY_DATA_NOT kPacket;
        SiKCnSurvey()->GetSurveyData( kPacket );
        SEND_COMPRESS_PACKET( ECN_SURVEY_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 설문 데이터 전송." ) << END_LOG;
    }

    {
        KECN_GWC_EVENT_DATA_NOT kPacket;
        SiKCnGWCEvent()->GetGWCEventData( kPacket );
        SEND_PACKET( ECN_GWC_EVENT_DATA_NOT );
        START_LOG( cerr, L"Center 접속시 GWC 이벤트 데이터 전송." ) << END_LOG;
    }

    {
        KECN_ADVENTURE_DATA_NOT kPacket;
        SiKCnAdventure()->GetAdventureData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ADVENTURE_DATA_NOT, kPacket );
        START_LOG( cerr, L"Center 접속시 모험 이벤트 데이터 전송." ) << END_LOG;
    }

    {
        KECN_OLYMPIC_EVENT_DATA_NOT kPacket;
        SiKCnOlympicManager()->GetOlympicEventData( kPacket );
        SEND_PACKET( ECN_OLYMPIC_EVENT_DATA_NOT );
        START_LOG( cerr, L"Center 접속시 올림픽 데이터 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 발렌타인 기부 데이터 알려준다 (기부 수량, 버프여부)
        KECN_BUFF_DONATION_TOTAL_DATA_NOT kPacket;
        SiKCnBuffDonation()->GetDonationTotalData( kPacket );
        SEND_PACKET( ECN_BUFF_DONATION_TOTAL_DATA_NOT );
        START_LOG( cerr, L"Center 접속시 발렌타인 기부 이벤트 데이터 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 누적 출석 DB세팅 데이터 알려준다 (버전, 기간)
        KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT kPacket;
        SiKCnAccumulateAttendanceManager()->GetAccumulateAttendanceSettingData( kPacket );
        SEND_PACKET( ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT );
        START_LOG( cerr, L"Center 접속시 누적 출석 데이터 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 캐시백 비율 정보 전달
        KCashbackRatioInfo kPacket;
        SiKCnItemManager()->GetCashbackRatioInfo( kPacket );
        SEND_PACKET( ECN_CASHBACK_RATIO_INFO_NOT );
        START_LOG( cerr, L"Center 접속시 캐시백 비율 정보 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 새로 읽어야 할 스크립트 목록을 알려준다 (UpdatePlanManager)
        // 게임서버 연결 시 전달되는 데이터는 연결 요청을 한 서버에서만 전달하게 수정.
        KECN_PLAN_SCRIPT_UPDATE_NOT kPacket;
        KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT kPacketCCOIN;
        KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT kPacketCCFIIN;
        SiKCnUpdatePlanManager()->GetScriptUpdateInfo( kPacket );
        SEND_PACKET( ECN_PLAN_SCRIPT_UPDATE_NOT );

        SiKCnUpdatePlanManager()->GetCurrentICCFullPacket( kPacketCCOIN, kPacketCCFIIN );
        SEND_DATA( ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, kPacketCCOIN );
        SEND_DATA( ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, kPacketCCFIIN );
        START_LOG( cerr, L"Center 접속시 스크립트 업데이트 정보 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 접속 선물상자 정보 전달
        KECN_CONNECTION_GIFTBOX_INFO_NOT kPacket;
        SiKCnConnectionGiftBox()->GetConnectionGiftBoxInfoNot( kPacket );
        SEND_PACKET( ECN_CONNECTION_GIFTBOX_INFO_NOT );
        START_LOG( cerr, L"Center 접속시 접속 선물상자 정보 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결시 통합 기부 이벤트 정보 전달
        KECN_TONG_DONATION_INFO_NOT kPacket;
        SiKCnTongDonation()->GetTongDonationInfoNot( kPacket );
        SEND_PACKET( ECN_TONG_DONATION_INFO_NOT );
        START_LOG( cerr, L"Center 접속시 통합 기부 이벤트 정보 전송." ) << END_LOG;
    }

    {
        // 게임서버 연결 시 소셜커머스 DB세팅 데이터 알려 준다.
        KECN_SOCIAL_COMMERCE_INIT_DATA_NOT kPacket;
        SiKCnSocialCommerce()->GetSocialCommerceSettingData( kPacket );
        SEND_PACKET( ECN_SOCIAL_COMMERCE_INIT_DATA_NOT );
        START_LOG( cerr, L"Center 접속시 소셜커머스 데이터 전송." ) << END_LOG;
    }

    {
        // 던전 랭킹 시스템 랭킹 정보 전달
        KECN_DUNGEON_RANK_RANKING_CONFIG_NOT kRankingConfig;
        SiKCnDungeonRankManager()->GetDungeonRankRankingConfigNotPacket( kRankingConfig );
        SEND_DATA( ECN_DUNGEON_RANK_RANKING_CONFIG_NOT, kRankingConfig );

        KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT kCurrentRankPacket;
        if ( true == SiKCnDungeonRankManager()->GetDungeonRankCurrentSeasonTopRankNotPacket( kCurrentRankPacket ) ) {
            SEND_COMPRESS_PACKET( ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT, kCurrentRankPacket );
        }

        KECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT kPreviousRankPacket;
        if ( true == SiKCnDungeonRankManager()->GetDungeonRankPreviousSeasonTopRankNotPacket( kPreviousRankPacket ) ) {
            SEND_COMPRESS_PACKET( ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT, kPreviousRankPacket );
        }

        START_LOG( cerr, L"Center 접속시 던전 랭킹 시스템 데이터 전송." ) << END_LOG;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( KncUtil::toWideString( m_kServerInfo.m_strIP ) )
        << BUILD_LOG( m_kServerInfo.m_usPort )
        << BUILD_LOG( m_kServerInfo.m_nUserProtocolVer )
        << BUILD_LOG( m_kServerInfo.m_dwUID ) << END_LOG;

    StateTransition( KDefaultFSM::INPUT_VERIFICATION_OK );
}

_IMPL_ON_FUNC( ECN_LOUD_MSG_REQ, KSerBuffer )
{   
    SiKCenterSimLayer()->m_kActorManager.SendToAllExMe( m_strName, KCenterEvent::ECN_LOUD_MSG_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ECN_REPETITION_PROVIDE_CANCEL_REQ, KECN_REPETITION_PROVIDE_REQ )
{
    SiKCenterServer()->ProcessRepetitionEvent();
}

_IMPL_ON_FUNC( ECN_MINIGAME_RANK_REQ, KMiniGameRankList )
{
    // 게임 서버가 보내온 랭크 정보를 업데이트 하고, 정렬된 결과를 다시 게임서버로 보내준다.
    KMiniGameRankList kPacket;
    //std::map< char, std::vector<KMiniGameRankInfo> > kPacket;

    SiKCenterServer()->AddMiniGameRank( kPacket_.m_mapRank ); // 서버에서 온 랭킹 정보를  로긴이 저장하고 있는 정보에 더하고, 정렬, 클리핑

    SiKCenterServer()->GetMiniGameRank( kPacket.m_mapRank ); // 정렬된 데이터를 다시 가져오기
    
    // 원래 있던 랭킹 데이터와 합친 다음에 정렬하고, 100등에서 컷?

    SEND_PACKET( ECN_MINIGAME_RANK_ACK );
}

_IMPL_ON_FUNC( ECN_UPDATE_DONATION_REQ, KDotantionSyncData )
{
    KDotantionSyncData kPacket;

    SiKCnDonationManager()->UpdateDeltaDonation( kPacket_.m_biTotalDonationPoint );
    SiKCnDonationManager()->UpdateDonationRankServer( kPacket_.m_vecDonationRank );

    kPacket.m_biTotalDonationPoint = SiKCnDonationManager()->GetTotalDonation();
    SiKCnDonationManager()->GetDonationRank( kPacket.m_vecDonationRank );

    SEND_PACKET( ECN_UPDATE_DONATION_ACK );

    START_LOG( clog, L"서버로 부터 랭크 정보 업데이트 받음. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_vecDonationRank.size() )
        << BUILD_LOG( kPacket.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket.m_vecDonationRank.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_NEW_DONATION_REQ, MAP_DWORD_INT )
{
    MAP_DWORD_INT mapTotalDonation;
    mapTotalDonation.clear();

    SiKCnNewDonationManager()->UpdateDeltaDonation( kPacket_ );
    SiKCnNewDonationManager()->GetTotalDonation( mapTotalDonation );

    SEND_DATA( ECN_UPDATE_NEW_DONATION_ACK, mapTotalDonation );

    START_LOG( clog, L"서버로 부터 기부 정보 업데이트 받음. Name : " << m_strName )
        << BUILD_LOG( mapTotalDonation.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SERVER_RELAY_PACKET_REQ, KServerRelayPacket )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAllExMe( m_strName, KCenterEvent::ECN_SERVER_RELAY_PACKET_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_ADD_USER_REQ )
{
    LIF( AddConnectedUser( kPacket_.m_dwUID ) );

    START_LOG( clog, L" Add Connect User : " << kPacket_.m_dwUID ) 
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strName ) )
        << END_LOG;
}

IMPL_ON_FUNC( ECN_DEL_USER_REQ )
{
	__LIF( SiKCnGuildManager()->OfflineUser( kPacket_.m_dwUserUID ) );
	__LIF( SiKCnGuildManager()->BroadGuildUserOffline( kPacket_.m_dwUserUID ) );
	__LIF( SiKCnSquareManager()->LeaveSquare( kPacket_.m_dwUserUID ) );
	__LIF( SiKCnSquareManager()->ForceExitMinigame( kPacket_.m_dwUserUID ) ); // false 인 경우 없음

    if ( DelConnectedUser( kPacket_.m_dwUserUID ) == false ) {
        START_LOG( cwarn, L"DelConnectedUser failed. UserUID : " << kPacket_.m_dwUserUID << L",reason : " << kPacket_.m_nReason << L",ServerUID :" << GetUID() ) << END_LOG;
    }

    START_LOG( clog, L" Remove Connect User : " << kPacket_.m_dwUserUID << L",reason : " << kPacket_.m_nReason << L",ServerUID :" << GetUID() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_ADD_MULTI_USER_REQ, std::vector<DWORD> )
{
    std::vector<DWORD>::iterator vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
        AddConnectedUser( *vit );

    START_LOG( clog, L" Add Connect Multi User.. : " << kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_REQ, PAIR_INT )
{
    SiKCnSubscriptionManager()->UpdateCurrentGiftNum( kPacket_.first, kPacket_.second );
}

_IMPL_ON_FUNC( ECN_UPDATE_DICE_PLAY_REQ, KDicePlaySyncData )
{
    KDicePlaySyncData kPacket;

    // GameServer -> CenterServer 랭킹 정보 전송.
    SiKCnDicePlay()->UpdateDicePlayRankServer( kPacket_.m_vecDicePlayRank );

    SiKCnDicePlay()->GetDicePlayRank( kPacket.m_vecDicePlayRank );
    SEND_PACKET( ECN_UPDATE_DICE_PLAY_ACK );

    START_LOG( clog, L"게임서버로부터 주사위 이벤트 Ranking 정보 업데이트 받음. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_vecDicePlayRank.size() ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_ENTER_SQUARE_REQ, KSquareUserInfo )
{
    START_LOG( clog, L"광장 진입 유저 정보. SquareID : " << kPacket_.m_dwSquareID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strNick )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwServerUID )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwPetID )
        << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetLV )
        << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetPromotion ) << END_LOG;

    SET_ERROR( ERR_UNKNOWN );
    KECN_ENTER_SQUARE_ACK kPacket;
    kPacket.m_dwUserUID = kPacket_.m_dwUID;
    kPacket.m_dwSquareUID = kPacket_.m_dwSquareID;
    if( SiKCnSquareManager()->EnterSquare( kPacket_, GetThisPtr(), kPacket ) )
    {
        SET_ERROR( NET_OK );
    }

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SQUARE_01, kPacket.m_nOK, 2 );  // 요청한 광장을 찾을수 없음. )
        ERR_CASE( ERR_SQUARE_02, kPacket.m_nOK, 3 );  // 입장 하려는 광장에 사람이 꽉 찼습니다. )
        ERR_CASE( ERR_SQUARE_03, kPacket.m_nOK, 4 );  // 입장하려는 광장에 이미 들어 있음. 게임종료 유도 )
        ERR_CASE( ERR_SQUARE_06, kPacket.m_nOK, 5 );  // 아직 닉네임을 만들지 않았음
    default:
        START_LOG( cerr, L"알수 없는 오류  Name : " << m_strName << L", Msg : "<< NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    in_addr in;
    in.S_un.S_addr = kPacket.m_dwRelayServerIP;

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"광장 진입 결과 : " << kPacket.m_nOK )
        << BUILD_LOG( inet_ntoa(in) )
        << BUILD_LOG( kPacket.m_usRelayServerPort ) << END_LOG;

    SEND_PACKET( ECN_ENTER_SQUARE_ACK );
}

_IMPL_ON_FUNC( ECN_LEAVE_SQUARE_NOT, DWORD )
{
    SiKCnSquareManager()->LeaveSquare( kPacket_ );
}

_IMPL_ON_FUNC( ECN_UPDATE_USER_SQUARE_POS_NOT, KSquarePos )
{
    KCnSquarePtr spSquare = SiKCnSquareManager()->GetSquare( kPacket_.m_dwSquareID );

    if( !spSquare )
    {
        START_LOG( cerr, L"유저 위치 변화를 전송할 광장 없음, Square ID : " << kPacket_.m_dwSquareID ) << END_LOG;
        return;
    }

    spSquare->UpdateUserPos( kPacket_ );

    KECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT kPacket;
    kPacket.m_dwUID = kPacket_.m_dwUID;
    spSquare->GetNeighborhood( (int)kPacket_.m_fXpos, kPacket.m_vecNeighborhood );
    // 로그 너무 많이 남음. 주석처리
    //START_LOG( clog, L"광장 위치정보 갱신. UserUID : " << kPacket_.m_dwUID << L",SquareID : " << kPacket_.m_dwSquareID << L",Pos(x,y) : " << kPacket_.m_fXpos << L"," << kPacket_.m_fYpos ) << END_LOG;
    SEND_PACKET( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT );
}

_IMPL_ON_FUNC( ECN_NEW_SQUARE_USER_NOT, KSquareUserInfo )
{
    START_LOG( clog, L"광장에 새로운 유저 입장. SquareID : " << kPacket_.m_dwSquareID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strNick )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwServerUID )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwPetID )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwPetID )
        << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetLV )
        << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetPromotion ) << END_LOG;

    SEND_RECEIVED_PACKET( ECN_NEW_SQUARE_USER_NOT );
}

IMPL_ON_FUNC( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT )
{
    // 로그 너무 많이 남음. 주석처리
    //START_LOG( clog, L"내 이웃의 UID. dwUID : " << kPacket_.m_dwUID << L", vecNeighborhood.size() : " << kPacket_.m_vecNeighborhood.size() ) << END_LOG;
    SEND_RECEIVED_PACKET(ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT);
}

IMPL_ON_FUNC( ECN_LEAVE_SQUARE_BROAD )
{
    SEND_RECEIVED_PACKET(ECN_LEAVE_SQUARE_BROAD);
}

_IMPL_ON_FUNC( ECN_SQUARE_RELAY_PACKET_REQ, KSquareRelayPacket )
{
    KCnSquarePtr spSquare = SiKCnSquareManager()->GetSquare( kPacket_.m_dwSquareUID );

    if ( !spSquare ) {
        START_LOG( cerr, L"릴레이 패킷하려는 광장 없음, Square ID: " << kPacket_.m_dwSquareUID ) << END_LOG;
        return;
    }

    /* 근접한 유저에게만 보낸다.
    if ( kPacket_.m_bNeighborhoodSend ) {
        spSquare->GetNeighborhoodEachServer( kPacket_.m_dwSenderUID, kPacket_.m_mapReceiverUsers );
    } */

    // 광장의 전체 유저에게  전달하기 
    spSquare->GetAllUserEachServer( kPacket_.m_dwSenderUID, kPacket_.m_mapReceiverUsers );
    spSquare->SendToAll( KCenterEvent::ECN_SQUARE_RELAY_PACKET_NOT, kPacket_ );

    START_LOG( clog, L"광장 릴레이 패킷 요청.. SquareUID : " << kPacket_.m_dwSquareUID )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_mapReceiverUsers.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ENTER_GUILD_SQUARE_REQ )
{
    KECN_ENTER_GUILD_SQUARE_ACK kPacket;
    SET_ERROR( NET_OK );
    SiKCnSquareManager()->EnterGuildSquare( kPacket_, GetThisPtr(), kPacket );

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_kEnterSquare.m_nOK, 0 );
        ERR_CASE( ERR_SQUARE_01, kPacket.m_kEnterSquare.m_nOK, 2 );  // 요청한 광장을 찾을수 없음. )
        ERR_CASE( ERR_SQUARE_02, kPacket.m_kEnterSquare.m_nOK, 3 );  // 입장 하려는 광장에 사람이  찼습니다. )
        ERR_CASE( ERR_SQUARE_03, kPacket.m_kEnterSquare.m_nOK, 4 );  // 입장하려는 광장에 이미 들어 있음. 게임종료 유도 )
        ERR_CASE( ERR_SQUARE_06, kPacket.m_kEnterSquare.m_nOK, 5 );  // 아직 닉네임을 만들지 않았음
    default:
        START_LOG( cerr, L"알수 없는 오류  Name : " << m_strName << L", Msg : "<< NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_kEnterSquare.m_nOK = -99;
    }

    SEND_PACKET( ECN_ENTER_GUILD_SQUARE_ACK );
}

_IMPL_ON_FUNC( ECN_ENTER_MINIGAME_NOT, DWORD )
{
    SiKCnSquareManager()->EnterMinigame( kPacket_ );
}

_IMPL_ON_FUNC( ECN_EXIT_MINIGAME_NOT, DWORD )
{
    SiKCnSquareManager()->ExitMinigame( kPacket_, GetThisPtr() );
}

IMPL_ON_FUNC( ECN_ADD_GUILD_USER_REQ )
{
    // Game -> Center 길드에 유저 추가
    // 길드유저가 게임서버에 접속을 하면 자기가 있는 게임서버 정보를 센터에 기록
    // 용도는 길드원이 있는 게임서버에만 길드 관련 패킷들을 처리하기 위함.
    // 신규 유저 혹은 길드 가입자의 처리도 여기서 하게됨(예정)
    QUEUEING_EVENT_TO_DB( ECN_ADD_GUILD_USER_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ECN_ADD_GUILD_USER_ACK, KECN_ADD_GUILD_USER_REQ )
{
    SiKCnGuildManager()->AddUser( kPacket_.m_dwGuildUID, kPacket_.m_kUserInfo, GetThisPtr() );
}

IMPL_ON_FUNC( ECN_CHAT_REQ )
{
    KCenterPtr spCenter;
    DWORD dwServerUID = 0;

    switch ( kPacket_.m_kChatData.m_cChatType ) {
    case KChatData::MSG_GUILD:
        QUEUEING_EVENT_TO_DB( ECN_CHAT_REQ, kPacket_ );
        break;
    case KChatData::MSG_WHISPER:
    case KChatData::MSG_WHISPER_EX:
        // 대상유저 접속서버 찾기
        dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_kChatData.m_dwReceiverUID ) );
        spCenter = SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID );
        if ( spCenter != NULL ) {
            spCenter->SendPacket( KCenterEvent::ECN_CHAT_BROAD, kPacket_ );
        }
        else {
            KECN_CHAT_ACK kPacket;
            kPacket.m_nOK = 4; // 대상 유저를 찾을 수 없음.
            kPacket.m_kChatData = kPacket_.m_kChatData;
            kPacket.m_dwServerUID = kPacket_.m_dwServerUID;
            SEND_PACKET( ECN_CHAT_ACK );
            START_LOG( cwarn, L"귓속말 대상 유저가 없음. Nick : " << kPacket_.m_kChatData.m_strReceiverNick ) << END_LOG;
        }
        break;
    default:
        START_LOG( clog, L"정의되지 않은 채팅 타입 : " << (int)kPacket_.m_kChatData.m_cChatType ) << END_LOG;
        break;
    }

    START_LOG( clog, L"채팅 타입 : " << (int)kPacket_.m_kChatData.m_cChatType << L",GuildUID : " << kPacket_.m_kChatData.m_iParam )
        << BUILD_LOG( kPacket_.m_kChatData.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_strChatMsg )
        << BUILD_LOG( kPacket_.m_kChatData.m_strReceiverNick ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CHAT_ACK )
{
    KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( kPacket_.m_dwServerUID ) );
    if ( spCenter != NULL ) {
        spCenter->SendPacket( KCenterEvent::ECN_CHAT_ACK, kPacket_ );
    }
}

_IMPL_ON_FUNC( ECN_ADD_NEW_GUILD_NOT, KNOneGuildData )
{
    // 게임서버에서 새로 길드생성시 센터로 전달.
    // 센터에선 새로 생성된 길드정보를 더하고,
    // 목록을 갱신하고, 페이지를 갱신하고, 각 게임서버에 전달함.
    SiKCnGuildManager()->AddNewGuildInfo( kPacket_.m_kGuildInfo, kPacket_.m_vecGuildNotice, kPacket_.m_mapGuildUserList );

    START_LOG( clog, L"게임서버로부터 새로 생성된 길드 정보를 받음. GuildUID : " << kPacket_.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strMasterNick )
        << BUILD_LOG( kPacket_.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket_.m_mapGuildUserList.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_DRUMOUT_GUILD_USER_NOT )
{
    // 길드원이 탈퇴한 경우/길드에서 추방(강퇴)당한 경우에 게임서버에서 센터서버로 전달.

    // 센터길드 매니저에서 탈퇴한 유저의 정보 제거/각 게임서버로 정보 전달.
    SiKCnGuildManager()->DrumoutUser( kPacket_.m_dwGuildUID, kPacket_.m_setUserUID, kPacket_.m_cOutType );

    START_LOG( clog, L"게임서버로부터 탈퇴된 길드원 정보를 받음. GuildUID : " << kPacket_.m_dwGuildUID << L", size: " << kPacket_.m_setUserUID.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT )
{
    // 길드원이 자기 소개글을 변경한 경우, 게임서버에서 센터서버로 전달.
    SiKCnGuildManager()->ModifyMemberComment( kPacket_.m_dwGuildUID, kPacket_.m_dwUserUID, kPacket_.m_strUserComment );

    START_LOG( clog, L"게임서버로부터 길드원 변경된 소갯글을 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strUserComment ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_MARK_REG_STATUS_REQ, PAIR_DWORD_UCHAR )
{
    // DB 가서 얻어 와서 업데이트 한다.
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GUILD_MARK_INFO_UPDATE_REQ, L"", kPacket_.first );
}

_IMPL_ON_FUNC( ECN_SQUARE_LIST_REQ, PAIR_DWORD )
{
    const DWORD& dwUserUID = kPacket_.first;
    const DWORD& dwGuildUID = kPacket_.second;

    START_LOG( clog, L"광장 리스트 요청받음. UserUID : " << dwUserUID << L",GuildUID : " << dwGuildUID ) << END_LOG;

    if ( dwGuildUID != 0 ) {
        QUEUEING_EVENT_TO_DB( ECN_SQUARE_LIST_REQ, kPacket_ );
        return;
    }

    KECN_SQUARE_LIST_ACK kPacket;
    kPacket.first = dwUserUID;
    SiKCnSquareManager()->GetSquareList( kPacket.second );
    SiKCnSquareManager()->GetGuildSquareList( dwGuildUID, kPacket.second );
    SEND_PACKET( ECN_SQUARE_LIST_ACK );
}

_IMPL_ON_FUNC( ECN_SQUARE_LIST_ACK, PAIR_DWORD )
{
    const DWORD& dwUserUID = kPacket_.first;
    const DWORD& dwGuildUID = kPacket_.second;

    KECN_SQUARE_LIST_ACK kPacket;
    kPacket.first = dwUserUID;
    SiKCnSquareManager()->GetSquareList( kPacket.second );
    SiKCnSquareManager()->GetGuildSquareList( dwGuildUID, kPacket.second );
    SEND_PACKET( ECN_SQUARE_LIST_ACK );
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT )
{
    // 길드원 맴버레벨이 변경된 경우, 게임서버에서 센터서버로 전달.
    SiKCnGuildManager()->ChangeGuildMemberLevel( kPacket_.m_dwGuildUID, kPacket_.m_mapChangeMemberLevel );

    START_LOG( clog, L"게임서버로부터 길드원 맴버레벨 변경정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_NOTICE_NOT )
{
    // 길드공지가 변경된 경우, 게임서버에서 센터서버로 전달함.
    SiKCnGuildManager()->ChangeGuildNotice( kPacket_.m_dwGuildUID, kPacket_.m_vecNotice );

    START_LOG( clog, L"게임서버로부터 변경된 공지 받음. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_vecNotice.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_UPDATE_GUILD_NAME_NOT )
{
    // 길드 이름이 변경된 경우, 게임서버에서 센터서버로 전달함.
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPDATE_GUILD_NAME_NOT, kPacket_.m_dwGuildUID, kPacket_.m_strGuildName );

    START_LOG( clog, L"게임서버로부터 변경된 길드 이름 받음. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName ) << END_LOG;
}

IMPL_ON_FUNC( ECN_JOIN_GUILD_REQ )
{
    // 게임서버로부터 길드가입요청 패킷을 받음.
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_JOIN_GUILD_REQ, kPacket_ );

    START_LOG( clog, L"게임서버로부터 길드가입요청 패킷을 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strMyComment ) << END_LOG;
}

IMPL_ON_FUNC( ECN_JOIN_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입 성공
    // 성공이면, 길드매니저에 길드원 추가작업 이벤트 큐잉
    // 큐 돌아서 찾아간 길드에서 인원 추가후 길드가 브로드 패킷 보내주고.
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_JOIN_GUILD_ACK, kPacket_.m_dwGuildUID, kPacket_ );
    SiKCnGuildManager()->AddUserInGuildInfo( kPacket_.m_dwUserUID, kPacket_.m_dwGuildUID );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cMemberLevel )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strName ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_JOIN_GUILD_ACK );
}

_IMPL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_REQ, PAIR_DWORD )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_CANCEL_JOIN_GUILD_REQ, kPacket_ );
    START_LOG( clog, L"게임서버로부터 길드가입 취소요청. GuildID : "<< kPacket_.second << L",UserUID : " << kPacket_.first ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입취소 성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_CANCEL_JOIN_GUILD_ACK, kPacket_.m_dwGuildUID, kPacket_ );
    SiKCnGuildManager()->DelUserInGuildInfo( kPacket_.m_dwUserUID );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_CANCEL_JOIN_GUILD_ACK );
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ, kPacket_ );

    START_LOG( clog, L"길드 맴버등급 변경 요청" )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드원 맴버레벨 변경 성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK, kPacket_.m_dwGuildUID, kPacket_ );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
        << BUILD_LOG( kPacket_.m_mapChangeMemberLevel.size() ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
}

_IMPL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_REQ, PAIR_DWORD )
{
    // kPacket_.first : UserUID
    // kPacekt_.second : GuildUID
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_SELF_DRUMOUT_GUILD_REQ, kPacket_ );
    START_LOG( clog, L"길드자진탈퇴신청. GuildID : "<< kPacket_.second << L", UserUID : " << kPacket_.first ) << END_LOG;
}

IMPL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입취소 성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_SELF_DRUMOUT_GUILD_ACK, kPacket_.m_dwGuildUID, kPacket_ );
    SiKCnGuildManager()->DelUserInGuildInfo(kPacket_.m_dwUserUID );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_SELF_DRUMOUT_GUILD_ACK );
}

IMPL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_REQ )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_ACCEPT_GUILD_JOINER_REQ, kPacket_ );

    START_LOG( clog, L"길드가입대기자 가입승인처리" )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입취소 성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_ACCEPT_GUILD_JOINER_ACK, kPacket_.m_dwGuildUID, kPacket_ );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_ACCEPT_GUILD_JOINER_ACK );
}

IMPL_ON_FUNC( ECN_REJECT_GUILD_JOINER_REQ )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_REJECT_GUILD_JOINER_REQ, kPacket_ );

    START_LOG( clog, L"길드가입거절" )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_REJECT_GUILD_JOINER_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드가입거절 성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_REJECT_GUILD_JOINER_ACK, kPacket_.m_dwGuildUID, kPacket_ );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_setUserUID.size() ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_REJECT_GUILD_JOINER_ACK );
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL )
{
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_UPDATE_GUILD_URL_NOT, kPacket_.m_dwUID, kPacket_ );

    START_LOG( clog, L"게임서버로부터 변경된 길드 URL 받음." )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strURL ) << END_LOG;
}


IMPL_ON_FUNC( ECN_BREAKUP_GUILD_REQ )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_BREAKUP_GUILD_REQ, kPacket_ );
    START_LOG( clog, L"길드해산. GuildID : "<< kPacket_.m_dwGuildUID << L",UserUID : " << kPacket_.m_dwUserUID ) << END_LOG;
}

IMPL_ON_FUNC( ECN_BREAKUP_GUILD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 여기서부터는 길드해산 성공
    SiKCnGuildManager()->RemoveGuild( kPacket_.m_dwGuildUID );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_BREAKUP_GUILD_ACK );
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_NOT, KNGuildJoinPolicy )
{
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_NOT, kPacket_.m_dwUID, kPacket_ );

    START_LOG( clog, L"게임서버로부터 변경된 길드 가입승인 정책 받음." )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_ucMethod ) << END_LOG;   
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, KCnNGuildJoinPolicy )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, kPacket_ );

    START_LOG( clog, L"길드가입 승인방법 변경." )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kPolicy.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy )
{
    NetError::SetLastNetError( kPacket_.m_kPolicy.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 길드가입설정 변경성공
    SiKCnGuildManager()->QueueingEvent( KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_ACK, kPacket_.m_kPolicy.m_dwUID, kPacket_ );

END_PROC:
    // OK값 가공은 GameServer 해당 유저의 ack 처리부분에서 해주자.
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kPolicy.m_nOK )
        << BUILD_LOG( kPacket_.m_kPolicy.m_dwUID )
        << BUILD_LOGc( kPacket_.m_kPolicy.m_ucMethod ) << END_LOG;

    // 해당 게임서버로 전송.
    SEND_RECEIVED_PACKET( ECN_CHANGE_GUILD_JOIN_SETTING_ACK );
}

IMPL_ON_FUNC( ECN_LOG_OUT_NOT )
{
    switch( kPacket_.m_dwAuthType )
    {
    case KSimLayer::AT_FUNBOX_TH:
        {
            //if( kPacket_.m_strLogin.find( L".PP" ) == std::wstring::npos )
            //{
            //    if( KTThreadManager<KCnFBAuth>::GetInstance()->GetThreadNum() > 0 )
            //        KTThreadManager<KCnFBAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_LOG_OUT_NOT, m_strName.c_str(), kPacket_.m_dwUID, kPacket_ );
            //}
            //else
            //{
            //    if( KTThreadManager<KCnPPAuth>::GetInstance()->GetThreadNum() > 0 )
            //        KTThreadManager<KCnPPAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_LOG_OUT_NOT, m_strName.c_str(), kPacket_.m_dwUID, kPacket_ );
            //}
            }
        return;

    default:
        START_LOG( cerr, L"undefined Authenticate type." )
            //<< BUILD_LOG( SiKCenterSimLayer()->GetAuthTypeStr( kPacket_.m_dwAuthType ) )
            << BUILD_LOG( kPacket_.m_dwAuthType )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_GUILD_MEMEBER_LIST_SYNC_NOT )
{
    // 센터서버만 재시작된 경우,
    // 게임서버와의 길드 맴버리스트 정보 동기화를 위해 게임서버에서 보냄.
    std::map< DWORD, std::map<DWORD,KNGuildUserInfo> >::iterator mit;
    for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
        LIF( SiKCnGuildManager()->SetGuildMemberInfo( mit->first, mit->second ) );
    }
    START_LOG( clog, L"게임서버 길드 맴버리스트와 센터서버 길드 맴버리스트 동기화함. Guild size : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( ECN_INVITE_GUILD_USER_REQ )
{
    KCnGuildPtr spGuild = SiKCnGuildManager()->GetGuild( kPacket_.m_dwGuildUID );

    if( spGuild == NULL )
    {
        START_LOG( cerr, L"길드 객체가 NULL 입니다. GuildUID : "<<kPacket_.m_dwGuildUID ) << END_LOG;
        return;
    }

    std::map<DWORD, KNGuildUserInfo> mapGuildUser;
    spGuild->GetMemberList( mapGuildUser );

    std::map<DWORD,std::vector<DWORD>> mapUsersInServer;
    std::map<DWORD, KNGuildUserInfo>::iterator mit;
    for( int i = 0 ; i < (int)kPacket_.m_vecInviteList.size() ; ++i )
    {
        mit = mapGuildUser.find( kPacket_.m_vecInviteList[i] );
        if( mit == mapGuildUser.end() )
            continue;

        mapUsersInServer[mit->second.m_cServerUID].push_back( mit->second.m_dwUserUID );
    }

    std::map<DWORD,std::vector<DWORD>>::iterator mitServers;
    for( mitServers = mapUsersInServer.begin() ; mitServers != mapUsersInServer.end() ; ++mitServers )
    {
        DWORD dwServerUID = mitServers->first;
        std::vector<DWORD>& vecUsers = mitServers->second;

        KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );

        if( spCenter == NULL )
            continue;

        KECN_INVITE_GUILD_USER_ACK kPacket;
        kPacket.m_kInviteInfo = kPacket_.m_kInviteInfo;
        kPacket.m_nOK = 0;
        kPacket.m_vecUserUID = vecUsers;
        spCenter->SendPacket( KCenterEvent::ECN_INVITE_GUILD_USER_ACK, kPacket );
    }
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad )
{
    QUEUEING_EVENT_TO_DB( ECN_UPDATE_GUILD_POINT_REQ, kPacket_ );

    START_LOG( clog, L"게임서버에서 길드 포인트 변화량 받음" )
        << BUILD_LOG( kPacket_.m_GuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_dwDiffPoint ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SQUARE_USER_SYNC_REQ, KSquarePos )
{
    KECN_SQUARE_USER_SYNC_ACK kPacket;
    kPacket.m_kUserPos = kPacket_;

    SiKCnSquareManager()->UserPositionSync( kPacket_, kPacket.m_vecPosUID );

    SEND_PACKET( ECN_SQUARE_USER_SYNC_ACK );
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, kPacket_ );

    START_LOG( clog, L"게임서버로부터 변경된 길드 맴버 닉네임 정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter )
{
    // DB 큐잉 queuing
    QUEUEING_EVENT_TO_DB( ECN_GUILD_MEMBER_LEVELUP_NOT, kPacket_ );

    START_LOG( clog, L"게임서버로부터 길드 맴버 레벨업 정보 받음." )
        << BUILD_LOG( kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cLevel ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_INVITE_GUILD_NOT, KGuildInvite )
{
    // 각 게임서버로 길드초대 패킷 전달
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_INVITE_GUILD_NOT, kPacket_ );

    START_LOG( clog, L"게임서버로부터 길드 초대 패킷 받음. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_strGuildName )
        << BUILD_LOGc( kPacket_.m_ucGrade )
        << BUILD_LOG( kPacket_.m_dwToUserUID )
        << BUILD_LOG( kPacket_.m_strFromUserNickName ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_GUILD_PAGE_REQ, UCHAR )
{
    // 길드 페이지 정보 요청
    std::map<UCHAR, KSerBuffer > mapPageInfo;
    SiKCnGuildManager()->GetGuildPageIDList( mapPageInfo );

    std::map<UCHAR, KSerBuffer >::iterator mit;
    mit = mapPageInfo.find( kPacket_ );
    if ( mit == mapPageInfo.end() ) {
        START_LOG( cwarn, L"없는 페이지 Sort 정보. SortType : " << kPacket_ )
            << BUILD_LOG( mapPageInfo.size() ) << END_LOG;
        return;
    }

    KECN_GUILD_PAGE_NOT kPacket;
    kPacket.m_ucSortType = kPacket_;
    kPacket.m_kPageBuffer = mit->second;
    SEND_COMPRESS_PACKET( ECN_GUILD_PAGE_NOT, kPacket );

    START_LOG( clog, L"요청한 길드 Page정보 전송. SortType : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( ECN_USER_SERVER_ROOM_REQ )
{
    KECN_USER_SERVER_ROOM_ACK kPacket;
    kPacket.m_nOK = NetError::ERR_UNKNOWN;
    kPacket.m_kSendUser = kPacket_.m_kSendUser;
    kPacket.m_kServerRoomInfo = kPacket_.m_kTargetUser;

    DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_kTargetUser.m_dwUserUID ) );
    KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
    if ( spCenter != NULL ) {
        // 대상 유저가 있는 서버에 정보 요청.
        spCenter->SendPacket( KCenterEvent::ECN_USER_SERVER_ROOM_REQ, kPacket_ );
        return;
    }
    else {
        START_LOG( cwarn, L"대상 유저가 접속중인 서버가 없습니다. 대상 UserUID : " << kPacket_.m_kTargetUser.m_dwUserUID ) << END_LOG;
    }

    kPacket.m_nOK = NetError::ERR_SERVER_ROOM_03;
    SEND_PACKET( ECN_USER_SERVER_ROOM_ACK );
}

IMPL_ON_FUNC( ECN_USER_SERVER_ROOM_ACK )
{
    KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( kPacket_.m_kSendUser.m_dwServerUID ) );
    JIF( spCenter );
    spCenter->SendPacket( KCenterEvent::ECN_USER_SERVER_ROOM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ECN_CREATE_PARTY_REQ, KPartyData )
{ 
    if ( kPacket_.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티 생성 패킷이 비었음" << kPacket_.m_dwPartyUID ) << END_LOG;
        return;
    }

    {
        DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_kInvitee.m_dwUID ) );
        KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
        if ( spCenter != NULL ) {
            // 대상 유저가 있는 서버에 정보 요청.
            //kPacket_.m_dwPartyUID = dwPartyUID;
            // 파티장 세팅

            spCenter->SendPacket( KCenterEvent::ECN_CREATE_PARTY_ASK, kPacket_ );
        }
        else {
            START_LOG( cwarn, L"대상 파티원이 접속중인 서버가 없습니다. 대상 UserUID : " << kPacket_.m_kInvitee.m_dwUID ) << END_LOG;
        }
    }
}

// EVENT_CREATE_PARTY_ANSWER 로부터 온 패킷
_IMPL_ON_FUNC( ECN_CREATE_PARTY_ANSWER, KPartyData )
{
    kPacket_.m_nOK = -99;

    if ( kPacket_.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티생성 패킷에 파티원 데이터가 없음" ) << END_LOG;
        return;
    }

    if ( kPacket_.m_bAnswer == true ) {

        DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_vecPartyPeople.front().m_dwUID ) );
        KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
        if ( spCenter == NULL ) {
            START_LOG( cerr, L"파티장이 접속중인 서버가 없음, User UID : " << kPacket_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
            return;
        }

        if ( SiKCnPartyManager()->CreateParty( kPacket_, GetThisPtr(), spCenter ) == true ) { // 유저 말고 서버만 등록
            kPacket_.m_nOK = 0;
        }

        if ( SiKCnPartyManager()->SendToAll( KCenterEvent::ECN_CREATE_PARTY_ACK, kPacket_, kPacket_.m_dwPartyUID ) ) {
            START_LOG( clog, L"파티 생성 응답 성공, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;
        }
        else {
            START_LOG( clog, L"파티 생성 응답 실패, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;
        }
    }
    else {
        kPacket_.m_nOK = 1;

        DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_vecPartyPeople.front().m_dwUID ) );
        KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
        if ( spCenter == NULL ) {
            START_LOG( cerr, L"파티장이 접속중인 서버가 없음, User UID : " << kPacket_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
            return;
        }

        spCenter->SendPacket( KCenterEvent::ECN_CREATE_PARTY_ACK, kPacket_ );
        START_LOG( clog, L"파티 거절 성공, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;
    }
}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_REQ, KPartyData )
{
    DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_kInvitee.m_dwUID ) );
    KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
    if ( spCenter != NULL ) {
        // 대상 유저가 있는 서버에 정보 요청.
        spCenter->SendPacket( KCenterEvent::ECN_INVITE_PARTY_ASK, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"대상 파티장이 접속중인 서버가 없습니다. 대상 UserUID : " << kPacket_.m_kInvitee.m_dwUID ) << END_LOG;
    }
    
}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_ANSWER, KPartyData )
{
    if ( kPacket_.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티 생성 응답 패킷이 비었음" << kPacket_.m_dwPartyUID ) << END_LOG;
        return;
    }

    if ( kPacket_.m_bAnswer == true ) {

        kPacket_.m_kInvitee.m_ServerUID = GetUID();
        
        if ( SiKCnPartyManager()->JoinParty( kPacket_, GetThisPtr() ) == true ) {
            SET_ERROR( NET_OK );
        }

    }
    else {
        SET_ERROR( ERR_PARTY_04 );        
    }

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_PARTY_04, kPacket_.m_nOK, 1 ); // 파티 초대 거부 )
        ERR_CASE( ERR_PARTY_02, kPacket_.m_nOK, 2 ); // 이미 존재하는 파티원 초대 )
        ERR_CASE( ERR_PARTY_03, kPacket_.m_nOK, 3 ); // 파티 제한 인원 초과 )
        ERR_CASE( ERR_PARTY_07, kPacket_.m_nOK, 4 ); // 파티 포인터 없음
        
    default:
        START_LOG( cerr, L"알수 없는 오류  Name : " << m_strName << L", Msg : "<< NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }


    {
        if ( kPacket_.m_vecPartyPeople.empty() ) {
            return;
        }
        DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_vecPartyPeople.front().m_dwUID ) );
        KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
        if ( spCenter != NULL && kPacket_.m_nOK != 0 ) {
            spCenter->SendPacket( KCenterEvent::ECN_INVITE_PARTY_ACK, kPacket_ );
            return;            
        }
        else {
            START_LOG( cwarn, L"대상 파티장이 접속중인 서버가 없습니다. 대상 UserUID : " << kPacket_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
        }
        
        if ( SiKCnPartyManager()->SendToAll( KCenterEvent::ECN_INVITE_PARTY_ACK, kPacket_, kPacket_.m_dwPartyUID ) ) {
            START_LOG( clog, L"파티 초대 응답 성공, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;
        }
        else {
            START_LOG( clog, L"파티 초대 응답 실패, Party UID : " << kPacket_.m_dwPartyUID ) << END_LOG;
        }
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwPartyUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_CHANGE_PARTY_HOST_REQ, KPartyData )
{
    SiKCnPartyManager()->SetNextHost( kPacket_ );
}

_IMPL_ON_FUNC( ECN_LEAVE_PARTY_REQ, KPartyData )
{
    SiKCnPartyManager()->LeaveParty( kPacket_, GetThisPtr() );
}

_IMPL_ON_FUNC( ECN_INVITE_PARTY_ROOM_REQ, KPartyRoomData )
{
    if ( kPacket_.m_kPartyData.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티방 초대 패킷이 비었음" << kPacket_.m_kPartyData.m_dwPartyUID ) << END_LOG;
        return;
    }

    if ( SiKCnPartyManager()->SendToAll( KCenterEvent::ECN_INVITE_PARTY_ROOM_REQ, kPacket_, kPacket_.m_kPartyData.m_dwPartyUID ) ) {
        START_LOG( clog, L"파티방으로 초대 요청 성공, Party UID : " << kPacket_.m_kPartyData.m_dwPartyUID )
            << BUILD_LOG( kPacket_.m_kPartyData.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
    }
    else {
        START_LOG( clog, L"파티방으로 초대 요청 실패, Party UID : " << kPacket_.m_kPartyData.m_dwPartyUID ) << END_LOG;
    }
}

_IMPL_ON_FUNC( ECN_BAN_PARTY_USER_REQ, KPartyData )
{
    if ( kPacket_.m_vecPartyPeople.empty() ) {
        START_LOG( cerr, L"파티 강퇴 패킷이 비었음" << kPacket_.m_dwPartyUID ) << END_LOG;
        return;
    }

    SiKCnPartyManager()->BanPartyUser( kPacket_ );

    DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_.m_vecPartyPeople.front().m_dwUID ) );
    KCenterPtr spCenter( SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID ) );
    if ( spCenter != NULL ) {
        spCenter->SendPacket( KCenterEvent::ECN_BAN_PARTY_USER_NOT, kPacket_ );
    }
    else {
        START_LOG( cwarn, L"대상 파티장이 접속중인 서버가 없습니다. 대상 UserUID : " << kPacket_.m_vecPartyPeople.front().m_dwUID ) << END_LOG;
    }
}


_IMPL_ON_FUNC( ECN_NOTICE_BROAD, std::vector<KSerBuffer> )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_NOTICE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ECN_INVITE_AGIT_REQ, KInviteAgitReq )
{
    SET_ERROR( ERR_UNKNOWN );

    DWORD dwServerUID = 0;
    KCenterPtr spCenter;
    int nInviteCount = 0;

    std::vector<DWORD>::iterator vit;
    for ( vit = kPacket_.m_vecUserUID.begin() ; vit != kPacket_.m_vecUserUID.end() ; ++vit ) {
        dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( *vit ) );
        spCenter = SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID );

        if ( spCenter == NULL ) {
            START_LOG( cwarn, L"대상 유저가 접속중이 아님. UserUID : " << *vit ) << END_LOG;
            continue;
        }

        KInviteAgitNot kPacket;
        kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
        kPacket.m_strOwnerNick = kPacket_.m_strOwnerNick;
        kPacket.m_dwUserUID = *vit;
        kPacket.m_ucAgitOpenType = kPacket_.m_ucAgitOpenType;
        kPacket.m_strAgitPassword = kPacket_.m_strAgitPassword;
        kPacket.m_bIsOwnerInvite = kPacket_.m_bIsOwnerInvite;

        spCenter->SendPacket( KCenterEvent::ECN_INVITE_AGIT_NOT, kPacket );
        ++nInviteCount;
    }

    START_LOG( clog, L"아지트 초대 요청 전달" )
        << BUILD_LOG( kPacket_.m_dwAgitUID )
        << BUILD_LOG( kPacket_.m_strOwnerNick )
        << BUILD_LOG( kPacket_.m_vecUserUID.size() )
        << BUILD_LOGc( kPacket_.m_ucAgitOpenType )
        << BUILD_LOG( kPacket_.m_strAgitPassword )
        << BUILD_LOG( kPacket_.m_bIsOwnerInvite )
        << BUILD_LOG( nInviteCount ) << END_LOG;
}

IMPL_ON_FUNC( ECN_USER_EXIT_GAME_NOT )
{
    if( KTThreadManager<KNUSAuth>::GetInstance()->GetThreadNum() > 0 ) {
        KTThreadManager<KNUSAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_LOG_OUT_NOT, m_strName.c_str(), 0, 0, kPacket_ );
    }
}

_IMPL_ON_FUNC( ECN_FIND_USER_SEND_LETTER_NOT, DWORD )
{
    // 대상유저 접속서버 찾기
    DWORD dwServerUID = SiKCenterSimLayer()->m_kActorManager.ForEach( KGetConnectServerUID( kPacket_ ) );
    KCenterPtr spCenter = SiKCenterSimLayer()->m_kActorManager.GetByUID<KCenter>( dwServerUID );
    if ( spCenter != NULL ) {
        spCenter->SendPacket( KCenterEvent::ECN_RECEIVE_LETTER_NOT, kPacket_ );
        START_LOG( clog, L"대상 유저에게 새 우편 알림. ToUser UserUID : " << kPacket_ ) << END_LOG;
    }
    else {
        START_LOG( clog, L"새 우편 알릴 대상 유저가 없음. ToUser UserUID : " << kPacket_ ) << END_LOG;
    }
}

IMPL_ON_FUNC( ECN_OLYMPIC_SYNC_SOLO_DATA_NOT )
{
    SiKCnOlympicManager()->UpdateUserSoloData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_strNickName, kPacket_.m_nTopPlayerData );

    START_LOG( clog, L"게임서버로 부터 올림픽 개인전 정보 받음. Name : " << m_strName )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_nTopPlayerData )
        << END_LOG;
}

IMPL_ON_FUNC( ECN_OLYMPIC_SYNC_TEAM_DATA_REQ )
{
    SiKCnOlympicManager()->UpdateUserTeamData( kPacket_.m_tmTodayDate, kPacket_.m_nGameType, kPacket_.m_nTeamType, kPacket_.m_nDeltaData );

    START_LOG( clog, L"게임서버로 부터 올림픽 개인전 정보 받음. Name : " << m_strName )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_nDeltaData )
        << END_LOG;
}

_IMPL_ON_FUNC( ECN_CYOU_USER_HEART_BEAT_REQ, KCYOUAuthUserInfo )
{
    // TODO : 제거하시오.
    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST) ) {
        kPacket_.m_strServerIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
    }    

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_HEARTBEAT_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );
}

IMPL_ON_FUNC( ECN_CYOU_USER_HEART_BEAT_ACK )
{
    SEND_RECEIVED_PACKET( ECN_CYOU_USER_HEART_BEAT_ACK );
}

_IMPL_ON_FUNC( ECN_CYOU_CASH_CHANGE_REQ, KCYOUCashData )
{
    //kPacket_.m_strServerIP = NetCommon::GetLocalIP();
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );

    // TODO : 제거하시오.
    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST) ) {
        kPacket_.m_strServerIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
    }    

    //SiKCenterSimLayer()->MakeCYOUSerialNumber( kPacket_.m_strSerialNumber );

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_CHANGE_CASH_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );

}

_IMPL_ON_FUNC( ECN_CYOU_CHANGE_CASH_ACK, KCYOUCashPoint )
{
    kPacket_;
    SEND_RECEIVED_PACKET( ECN_CYOU_CHANGE_CASH_ACK );
}

_IMPL_ON_FUNC( ECN_CYOU_CHANGE_CASH_LOG_NOT, KCYOUChangeCashLogData )
{
    std::wstringstream stm;

    stm << L"[" << KNC_TIME_STRING << L"] ";
    stm << kPacket_.m_nOK << L"|";
    stm << KncUtil::toWideString( kPacket_.m_strLogin.c_str() ) << L"|";
    stm << kPacket_.m_dwUserUID << L"|";
    stm << KncUtil::toWideString( kPacket_.m_strCharNickname.c_str() ) << L"|";
    stm << kPacket_.m_dwCashPoint << L"|";
    stm << kPacket_.m_dwCYOUPoint << L"|";
    stm << kPacket_.m_usPacketSequence << L"|";
    stm << KncUtil::toWideString(kPacket_.m_strServerIP.c_str() ) << L"|";
    stm << KncUtil::toWideString(kPacket_.m_strUserIP.c_str() ) << L"|";
    stm << KncUtil::toWideString(kPacket_.m_strSerialNumber.c_str() ) << L"|";
    stm << KncUtil::toWideString(kPacket_.m_strItemInfo.c_str() ) << L"|";

    SiKCenterLogManager()->Get( KCenterLogManager::LT_CYOU_CHANGE_CASH ) << stm.str() << GLOG::endl;
    stm.clear();
    stm.str(L"");

    START_LOG( clog, L"CYOU 화폐전환 로그 기록" ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_PARTY_USER_READY_REQ, KPartyData )
{
    SiKCnPartyManager()->ChangeUserState( kPacket_, GetThisPtr() );
}

_IMPL_ON_FUNC( ECN_BUFF_DONATION_DATA_UPDATE_REQ, int )
{
    QUEUEING_EVENT_TO_DB( ECN_BUFF_DONATION_DATA_UPDATE_REQ, kPacket_ );

    START_LOG( clog, L"게임서버로 부터 기부 Diff값 얻음. Name : " << m_strName )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

_IMPL_ON_FUNC( ECN_CYOU_USER_AGREEMENT_REQ, KCYOUAuthUserInfo )
{
    //kPacket_.m_strServerIP = NetCommon::GetLocalIP();
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );
    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_AGREEMENT_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );

}

_IMPL_ON_FUNC( ECN_CYOU_GET_CURRENT_CASH_REQ, KCYOUCashData )
{
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );

    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST) ) {
        kPacket_.m_strServerIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
    }

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_GET_REMAIN_CASH_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );
}

_IMPL_ON_FUNC( ECN_CYOU_GET_CURRENT_CASH_ACK, KCYOUCashPoint )
{
    kPacket_;
    START_LOG( clog, L"중국 잔액 조회 결과, User UID : " << kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwCYOUPoint )
        << END_LOG;

    SEND_RECEIVED_PACKET( ECN_CYOU_GET_CURRENT_CASH_ACK );
}

_IMPL_ON_FUNC( ECN_CYOU_BUY_LOG_REQ, KCYOUCashData )
{
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );

    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST) ) {
        kPacket_.m_strServerIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
    }    
    SiKCenterSimLayer()->MakeCYOUSerialNumber( kPacket_.m_strSerialNumber );

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_BUY_LOG_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );
}

IMPL_ON_FUNC( ECN_CYOU_USER_LOGIN_ACK )
{
    SEND_RECEIVED_PACKET( ECN_CYOU_USER_LOGIN_ACK );
}

_IMPL_ON_FUNC( ECN_CYOU_USER_LOGIN_REQ, KCYOUAuthUserInfo )
{
    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP ) ;

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_LOGIN_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );
}

_IMPL_ON_FUNC( ECN_CYOU_USER_LOGOUT, KCYOUAuthUserInfo )
{
    // 중국 CYOU 인증에서 유저 로그아웃시 센터에 남아있는 유저일 경우 접종 시켜준다
    KNUserPtr spUser = KNUserPtr( SiKCenterSimLayer()->m_kActorManager2.GetByName< KNUser >( KncUtil::toWideString( kPacket_.m_strLogin ) ) );
    if ( NULL != spUser ) {
        KEVENT_CLOSE_CONNECTION_NOT kNot;
        kNot.m_nCloseType   = KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION_CYOU;
        kNot.m_strMessage   = SiKCenterServer()->m_stringTable.GetValue( L"str_02" );
        spUser->SendPacket( KNUserEvent::EVENT_CLOSE_CONNECTION_NOT, kNot );
        spUser->ReserveDestroy();

        KECN_DEL_USER_REQ kCnPacket;
        kCnPacket.m_dwUserUID = kPacket_.m_dwUID;
        kCnPacket.m_nReason = KECN_DEL_USER_REQ::DR_DUPLICATION_CENTER;
        SiKCenterSimLayer()->m_kActorManager.QueueingToAll( KCenterEvent::ECN_DEL_USER_REQ, kCnPacket );
    }

    SiKCenterSimLayer()->GetBillConnIP( kPacket_.m_strServerIP );

    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST)) {
        kPacket_.m_strServerIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
    }

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_LOGOUT_REQ, GetName().c_str() , GetUID(), 0, kPacket_ );
}

IMPL_ON_FUNC( ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ )
{
    SiKCnDungeonRankManager()->AddCurrentSeasonNewEntry( kPacket_ );

    START_LOG( clog, L"던전 랭킹 새 기록 데이터 얻음. size: " << kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC(ECN_BILLBOARD_DATA_BROAD_REQ, KECNBillboardChat)
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_BILLBOARD_DATA_BROAD, kPacket_ );
}