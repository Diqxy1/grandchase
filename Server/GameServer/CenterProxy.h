#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"
#include "CenterEvent.h"
#include "CenterPacket.h"
#include <ToString.h>
SmartPointer( KCenterProxy );

class KGSSimLayer;
class KUserEvent;
class KCnConnector;
struct KDotantionSyncData;
class KCenterProxy : public KActorProxy
{
    typedef KGSSimLayer SLayerType;
    typedef KUserEvent IDVenderType;
    NiDeclareRTTI;
    DeclToStringW;

public:
    KCenterProxy(void);
    virtual ~KCenterProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent() { return new KCenterEvent; }

    // 050328. LUA에서 접속을 제어하기 위한 함수가 필요하다.
    bool Connect( std::string strIP, unsigned short usPort );
    bool IsAfterVerify() { return m_bAfterVerify; };

protected:
    
    DECL_ON_FUNC( ECN_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( ECN_KICK_USER_NOT );
    _DECL_ON_FUNC( ECN_LOUD_MSG_ACK, KSerBuffer );
    DECL_ON_FUNC( ECN_REPETITION_PROVIDE_REQ );   
   _DECL_ON_FUNC( ECN_MINIGAME_RANK_ACK, KMiniGameRankList );
   _DECL_ON_FUNC( ECN_UPDATE_DONATION_ACK, KDotantionSyncData );
   _DECL_ON_FUNC( ECN_SERVER_RELAY_PACKET_NOT, KServerRelayPacket );
    DECL_ON_FUNC( ECN_RAINBOW_UPDATE_NOT );
    DECL_ON_FUNC( ECN_MISSION_EVENT_UPDATE_NOT );
    DECL_ON_FUNC( ECN_MISSION_NOTIFY_NOT );
    DECL_ON_FUNC( ECN_SUBSCRIPTION_INFO_UPDATE_NOT );
   _DECL_ON_FUNC( ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_ACK, PAIR_INT_DWORD );
    DECL_ON_FUNC( ECN_NPC_GIFT_UPDATE_NOT );
   _DECL_ON_FUNC( ECN_UPDATE_DICE_PLAY_ACK, KDicePlaySyncData );
    DECL_ON_FUNC( ECN_ITEM_DONATION_UPDATE_NOT );
    DECL_ON_FUNC( ECN_ENTER_SQUARE_ACK );
   _DECL_ON_FUNC( ECN_NEW_SQUARE_USER_NOT, KSquareUserInfo );
    DECL_ON_FUNC( ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT );
    DECL_ON_FUNC( ECN_LEAVE_SQUARE_BROAD );
   _DECL_ON_FUNC( ECN_SQUARE_RELAY_PACKET_NOT, KSquareRelayPacket );
    DECL_ON_FUNC( ECN_ENTER_GUILD_SQUARE_ACK );
   _DECL_ON_FUNC( ECN_OFFLINE_GUILD_USER_BROAD, PAIR_DWORD );
   _DECL_ON_FUNC( ECN_CHAT_BROAD, KECN_CHAT_REQ );
    DECL_ON_FUNC( ECN_CHAT_ACK );
    DECL_ON_FUNC( ECN_GUILD_LIST_NOT );
    DECL_ON_FUNC( ECN_GUILD_MEMEBER_LIST_NOT );
    DECL_ON_FUNC( ECN_GUILD_PAGE_NOT );
   _DECL_ON_FUNC( ECN_ADD_NEW_GUILD_NOT, KNOneGuildData );
    DECL_ON_FUNC( ECN_DRUMOUT_GUILD_USER_BROAD_NOT );
    DECL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT );
    DECL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT );
   _DECL_ON_FUNC( ECN_SQUARE_LIST_ACK, KECN_SQUARE_LIST_ACK );
    DECL_ON_FUNC( ECN_UPDATE_GUILD_NOTICE_NOT );
   _DECL_ON_FUNC( ECN_GUILD_MARK_STATUS_NOT, KNGuildMarkInfo );
    DECL_ON_FUNC( ECN_UPDATE_GUILD_NAME_NOT );
    DECL_ON_FUNC( ECN_JOIN_GUILD_ACK );
    DECL_ON_FUNC( ECN_ADD_GUILD_USER_BROAD_NOT );
    DECL_ON_FUNC( ECN_GUILD_RANK_NOT );
    DECL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_ACK );
    DECL_ON_FUNC( ECN_REMOVE_GUILD_USER_BROAD_NOT );
    DECL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    DECL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT );
    DECL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_ACK );
    DECL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_ACK );
    DECL_ON_FUNC( ECN_REJECT_GUILD_JOINER_ACK );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL );
    DECL_ON_FUNC( ECN_BREAKUP_GUILD_ACK );
   _DECL_ON_FUNC( ECN_REMOVE_GUILD_BROAD_NOT, DWORD );
   _DECL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_NOT, KNGuildJoinPolicy );
   _DECL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy );
   _DECL_ON_FUNC( ECN_GUILD_CONFIG_NOT, KGuildConfig );
   _DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_BROAD_NOT, DWORD );
   _DECL_ON_FUNC( ECN_COUPON_INFO_NOT, std::vector<KGameCouponInfo> );
    DECL_ON_FUNC( ECN_QUIZ_BINGO_DATA_NOT );
    DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
    DECL_ON_FUNC( ECN_INVITE_GUILD_USER_ACK );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_POINT_BROAD, KECN_UPDATE_GUILD_POINT_BROAD );
    DECL_ON_FUNC( ECN_GUILD_INFO_LIST_NOT );
    DECL_ON_FUNC( ECN_GUILD_NOTICE_LIST_NOT );
    DECL_ON_FUNC( ECN_GUILD_MEMBER_LIST_NOT );
    DECL_ON_FUNC( ECN_SQUARE_USER_SYNC_ACK );
   _DECL_ON_FUNC( ECN_GWC_CONFIG_DATA_NOT, KGWCSupportConfigData );
   _DECL_ON_FUNC( ECN_GWC_DATA_NOT, std::vector<KGWCSupportData> );
   _DECL_ON_FUNC( ECN_UPDATE_NEW_DONATION_ACK, MAP_DWORD_INT );
    DECL_ON_FUNC( ECN_SOCKET_ITEM_GP_TABLE_NOT );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
   _DECL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
   _DECL_ON_FUNC( ECN_INVITE_GUILD_NOT, KGuildInvite );
   _DECL_ON_FUNC( ECN_CLIENT_PING_CONFIG_NOT, KPingConfig );
    DECL_ON_FUNC( ECN_USER_SERVER_ROOM_REQ );
    DECL_ON_FUNC( ECN_USER_SERVER_ROOM_ACK );
    DECL_ON_FUNC( ECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT );
    DECL_ON_FUNC( ECN_SPHINX_DATA_NOT );
   _DECL_ON_FUNC( ECN_CREATE_PARTY_ASK, KPartyData );
   _DECL_ON_FUNC( ECN_INVITE_PARTY_ASK, KPartyData );
   _DECL_ON_FUNC( ECN_CREATE_PARTY_ACK, KPartyData );
   _DECL_ON_FUNC( ECN_LEAVE_PARTY_BROAD, KPartyData );
   _DECL_ON_FUNC( ECN_CHANGE_PARTY_HOST_BROAD, KPartyData );
   _DECL_ON_FUNC( ECN_INVITE_PARTY_ROOM_REQ, KPartyRoomData );
   _DECL_ON_FUNC( ECN_INVITE_PARTY_ACK, KPartyData );
   _DECL_ON_FUNC( ECN_BREAK_PARTY_NOT, KPartyData );
   _DECL_ON_FUNC( ECN_BAN_PARTY_USER_NOT, KPartyData );
   _DECL_ON_FUNC( ECN_NOTICE_NOT, std::vector<KSerBuffer> );
   _DECL_ON_FUNC( ECN_INVITE_AGIT_NOT, KInviteAgitNot );
   _DECL_ON_FUNC( ECN_SHUTDOWN_USER_CONFIG_NOT, KShutdownUserConfig );
    DECL_ON_FUNC( ECN_SURVEY_DATA_NOT );
    DECL_ON_FUNC( ECN_GWC_EVENT_DATA_NOT );
   _DECL_ON_FUNC( ECN_GWC_NOTICE_NOT, DWORD );
   _DECL_ON_FUNC( ECN_ITEM_UPDATE_NOT, bool );
   _DECL_ON_FUNC( ECN_RECEIVE_LETTER_NOT, DWORD );
    DECL_ON_FUNC( ECN_ADVENTURE_DATA_NOT );
    DECL_ON_FUNC( ECN_OLYMPIC_RANK_DATA_NOT );
    DECL_ON_FUNC( ECN_OLYMPIC_SYNC_SOLO_DATA_NOT );
    DECL_ON_FUNC( ECN_OLYMPIC_SYNC_TEAM_DATA_ACK );
    DECL_ON_FUNC( ECN_OLYMPIC_EVENT_DATA_NOT );
   _DECL_ON_FUNC( ECN_PARTY_USER_READY_BROAD, KPartyData );
    DECL_ON_FUNC( ECN_BUFF_DONATION_TOTAL_DATA_NOT );
    DECL_ON_FUNC( ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT );
   _DECL_ON_FUNC( ECN_CASHBACK_RATIO_INFO_NOT, KCashbackRatioInfo );
   _DECL_ON_FUNC( ECN_CLIENT_CONTENTS_OPEN_INFO_NOT, KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT );
   _DECL_ON_FUNC( ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT, KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT );
   _DECL_ON_FUNC( ECN_ITEM_BUY_INFO_NOT, KItemBuyInfo );
   _DECL_ON_FUNC( ECN_EVENT_CHANGED_EVENT_STATE_INFO_NOT, KEVENT_UPDATE_EVENT_STATE_NOT );
    DECL_ON_FUNC( ECN_PLAN_SCRIPT_UPDATE_NOT );
    DECL_ON_FUNC( ECN_EVENT_DB_UPDATE_NOT );
   _DECL_ON_FUNC( ECN_CYOU_CHANGE_CASH_ACK, KCYOUCashPoint );
   _DECL_ON_FUNC( ECN_CYOU_GET_CURRENT_CASH_ACK, KCYOUCashPoint );
    DECL_ON_FUNC( ECN_CYOU_USER_LOGIN_ACK );
    DECL_ON_FUNC( ECN_CYOU_USER_HEART_BEAT_ACK );
    DECL_ON_FUNC( ECN_CONNECTION_GIFTBOX_INFO_NOT );
   _DECL_ON_FUNC( ECN_EVENT_PACKAGE_LIST_CHANGED_NOT, bool );
    DECL_ON_FUNC( ECN_TONG_DONATION_INFO_NOT );
    DECL_ON_FUNC( ECN_SOCIAL_COMMERCE_INIT_DATA_NOT );
    DECL_ON_FUNC( ECN_DUNGEON_RANK_RANKING_CONFIG_NOT );
    DECL_ON_FUNC( ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT );
    DECL_ON_FUNC( ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT );
    _DECL_ON_FUNC( ECN_BILLBOARD_DATA_BROAD, KECNBillboardChat );
   HANDLE           m_hVerifyEvent;
   int              m_nSavedResult;
   bool             m_bAfterVerify;

   friend class KCnConnector;

};
DeclOstmOperatorA( KCenterProxy );


#define CASE_REL( id ) case KDBAEvent##::##REL_##id : \
                        { \
                            KREL_##id kPacket; \
                            if( ks.Get( kPacket ) ) \
                            { \
                                unsigned short usEventID = KUserEvent::##id; \
                                START_LOG( clog, L"ⓗ REL_" #id " (uid:" << kEvent_.GetUID() \
                                    << ", name:" << m_strName << ")" << END_LOG; \
                                KSerBuffer kDataBuff; \
                                ks.BeginWriting( &kDataBuff ); \
                                ks.Put( kPacket.m_kPacket ); \
                                ks.EndWriting(); \
                                ks.BeginWriting( &kbuff ); \
                                ks.Put( usEventID ); \
                                ks.Put( kDataBuff ); \
                                ks.Put( kPacket.m_bExistClone ); \
                                ks.Put( kPacket.m_uiCloneUID ); \
                                ks.EndWriting(); \
                                SiKGSSimLayer()->QueueingDBEvent( kPacket.m_strUserName, kbuff ); \
                            } \
                        } \
                        break

#define CASE_INTER( id ) case KDBAEvent##::##id : \
                         {  \
                            KPacketNameOK kPacket; \
                            if( ks.Get( kPacket ) ) \
                            { \
                                unsigned short usEventID  = KUserEvent::##id; \
                                START_LOG( clog, L"ⓗ REL_" #id " (uid:" << kEvent_.GetUID() \
                                    << ", name:" << m_strName << ")" << END_LOG; \
                                KEventPtr spEvent       = CreateKEvent(); \
                                spEvent->m_usEventID    = usEventID; \
                                ks.BeginWriting( &spEvent->m_kbuff ); \
                                ks.Put( kPacket ); \
                                ks.EndWriting(); \
                                ks.BeginWriting( &kbuff ); \
                                ks.Put( *spEvent ); \
                                ks.EndWriting(); \
                                SiKGSSimLayer()->QueueingDBEvent( kPacket.m_strUserName, kbuff ); \
                            } \
                         } \
                         break;
#define CASE_EXIT( id ) case KDBAEvent##::##id : \
                       {  \
                           START_LOG( clog, L"ⓗ REL_" #id " (uid:" << kEvent_.GetUID() \
                               << ", name:" << m_strName << ")" << END_LOG; \
                           KPacketNameOK kPacket; \
                           if( ks.Get( kPacket ) ) \
                           { \
                            ks.BeginWriting( &kbuff ); \
                            ks.Put( kEvent_ ); \
                            ks.EndWriting(); \
                           } \
                       } \
                       break;
