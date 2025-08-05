// code:#END_OF_CASE_GSDBTHREAD
#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "User.h"
#include "GSSimLayer.h"
#include <boost/algorithm/string.hpp> //boost::to_lower_copy
#include "GameServer.h"
#include "GuildManager.h"
#include "GSGash.h"
#include "LogManager.h"
#include "ResultManager.h"
#include <boost/bind.hpp>
#include "support_User.h"
#include "SkillManager.h"
#include "RecomHelper.h" // 이건 나중에 다른곳으로 옮기는게 좋을듯..
#include "PreHackingCheckManager.h"
#include "MissionInfo.h"
#include "MissionManager.h"
#include "IPAdvantage.h"
#include "MissionEvent.h"
#include "DicePlay.h"
#include "GCHelper.h"
#include "EventType.h"
#include "ChannelTypeEvent.h"
#include "StrengthManager.h"
#include "Survey.h"
#include "TickManager.h"
#include "SealedChar.h"
#include "GCHelper.h"
#include "PVPMatchManager.h"
#include "GachaManager.h"
#include "CharSlotManager.h"
#include "VirtualCash.h"

ImplementDBThread( KGSDBThread );
ImplementDBThreadName( KGSDBThread, Stat );

#define CLASS_TYPE KGSDBThread

KGSDBThread::KGSDBThread()
: m_kHelper( m_kODBC )
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() );
    m_kODBCStat.DriverConnect( ms_wstrFileDSNStat.c_str() );
}

KGSDBThread::~KGSDBThread(void)
{
    End();
}

void KGSDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();  // DB 함수 처리 시간 구하기

    switch( kIntEvent_.m_usEventID )
    {
        _INT_CASE( EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ );
        _INT_CASE( EVENT_REGISTER_NICKNAME_REQ, std::wstring );
        INT_CASE_NOPARAM( EVENT_AGREEMENT_PRIVATEINFO_NOT );
        _INT_CASE( EVENT_STRONG_LEVELUP_REQ, KDB_EVENT_STRONG_LEVELUP_REQ );
        _INT_CASE( EVENT_BUY_FOR_GP_REQ, KDB_EVENT_BUY_FOR_GP_REQ );
        _INT_CASE( EVENT_BUY_FOR_GEM_REQ, KDB_EVENT_BUY_FOR_GEM_REQ );
        _INT_CASE( EVENT_BUY_FOR_CRYSTAL_REQ, KDB_EVENT_BUY_FOR_CRYSTAL_REQ );
        //_INT_CASE( EVENT_BUY_COUNT_FOR_GP_REQ, KDB_EVENT_BUY_COUNT_FOR_GP_REQ );
        INT_CASE_NOPARAM( EVENT_QUERY_INVENTORY_INFO_REQ );
        _INT_CASE( EVENT_SELL_ITEM_REQ, KDB_EVENT_SELL_ITEM_INFO );

        _INT_CASE( DB_EVENT_UPDATE_USER_DATA_REQ, KUserData );
        INT_CASE( DB_EVENT_EXIT_GAME_NOT );
        INT_CASE( DB_EVENT_SERVER_INFO_REQ );
        INT_CASE( DB_EVENT_WRITE_MODE_COUNT_NOT );

        INT_CASE( DB_EVENT_INSERT_STATISTICS_NOT );

        _INT_CASE( DB_EVENT_UPDATE_GUILD_POINT_REQ, KGuildGameResult );
        _INT_CASE( DB_EVENT_INSERT_DROP_ITEM_REQ, KInsertDropItem );
        //_INT_CASE( EVENT_GUILD_EDIT_NOTICE_REQ, KGuildNoticeToEdit );

        //050530. microcat.
        _INT_CASE( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, KPacket2Name );
        _INT_CASE( EVENT_QUERY_DURATION_ITEM_LIST_REQ, KPacket2Name );
        _INT_CASE( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, KPacket2Name );
        _INT_CASE( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, KPacket2Name );
        _INT_CASE( EVENT_ADD_NORMAL_ITEM_GP_REQ, KAddItemInfo );
        _INT_CASE( EVENT_ADD_DURATION_ITEM_GP_REQ, KAddItemInfo );
        _INT_CASE( EVENT_ADD_NORMAL_ITEM_CASH_REQ, KAddItemInfo );
        _INT_CASE( EVENT_ADD_DURATION_ITEM_CASH_REQ, KAddItemInfo );
        INT_CASE( EVENT_REMOVE_ITEM_REQ );
        _INT_CASE( EVENT_QUERY_USER_INFO_REQ, KPacket2Name );
        _INT_CASE( EVENT_CHANGE_STRONG_ITEM_REQ, KPacket2Name );
        INT_CASE_NOPARAM( EVENT_QUERY_ITEM_INFO_REQ );
        //INT_CASE( EVENT_CHANGE_USER_INFO_REQ );
        _INT_CASE( EVENT_QUERY_USER_INFO2_REQ, KPacket2Name );
        _INT_CASE( EVENT_REMOVE_USER_INFO_REQ, KPacket2Name );

        _INT_CASE( DB_EVENT_INSERT_P2PFAILRATE_DATA, std::vector<int> );
        _INT_CASE( DB_EVENT_CHECK_EXPIRATION_ITEM_REQ, std::vector<KItem> );
        _INT_CASE( DB_EVENT_INSERT_CLIENTERR_DATA, std::vector<int> );
        INT_CASE( EVENT_USE_CHANGE_NICKNAME_REQ );
        INT_CASE( EVENT_USE_CHANGE_NICKCOLOR_REQ );
        // 060414. kkurrung. 미션 관련
        _INT_CASE( EVENT_REGIST_MISSION_REQ, KDB_EVENT_REGIST_MISSION_REQ );
        _INT_CASE( EVENT_REMOVE_MISSION_REQ, KEVENT_REMOVE_MISSION );
        INT_CASE( EVENT_COMPLETE_MISSION_REQ );
        INT_CASE( DB_EVENT_DUNGEON_MISSION_REGIST_REQ );
        INT_CASE_NOPARAM( DB_EVENT_FINISHED_MISSION_LIST_REQ );
        INT_CASE( EVENT_GMTOOL_AUTHENTICATION_REQ );
        INT_CASE_NOPARAM( DB_EVENT_TIME_LIMIT_EVENT_DATA );
        INT_CASE( DB_EVENT_CREATE_INDIGO_REQ );
        INT_CASE( DB_EVENT_UPDATE_INDIGO_REQ );
        INT_CASE_NOPARAM( DB_EVENT_CHECK_OPEN_TIME_REQ );
        INT_CASE( EVENT_SHANGHAI_DROP_REQ );
        _INT_CASE( EVENT_SHANGHAI_CHECK_REQ, DWORD );
        _INT_CASE( EVENT_RECOMMENDER_REQ, KPacketNameOK );
        INT_CASE_NOPARAM( EVENT_ACCUMULRATION_TIME_NOT );
        INT_CASE( DB_EVENT_KAIRO_REQ );
        INT_CASE( DB_EVENT_KAIRO_ONESHOT_REQ );

        // 2007/1/13. iridology. 펫 관련
        _INT_CASE( EVENT_CREATE_PET_REQ, KDB_EVENT_CREATE_PET );
        _INT_CASE( EVENT_DELETE_PET_REQ, KDB_EVENT_DELETE_PET );
        INT_CASE( EVENT_CHANGE_PET_NAME_REQ );
        INT_CASE_NOPARAM( DB_EVENT_UPDATE_LOUD_MSG );

        _INT_CASE( EVENT_USE_INVENTORY_EXTEND_REQ, KExtendItem );

        INT_CASE( EVENT_STAT_USER_HISTORY_NOT );
        INT_CASE( EVENT_STAT_CLIENT_INFO ); // 클라이언트 사양 조사.

        INT_CASE( EVENT_DB_REPETITION_PROVIDE_REQ ); // 한시간 이벤트
        _INT_CASE( EVENT_TRANSFORMATION_PET_REQ, KEVENT_DB_TRANSFORMATION_PET_REQ );

        //GCPoint and Calendar 
        INT_CASE_NOPARAM( EVENT_LOAD_POINTSYSTEM_INFO_REQ );
        _INT_CASE( EVENT_GCPOINT_RECOMMEND_REQ, std::wstring );
        _INT_CASE( EVENT_LOAD_NEW_CALENDAR_NOT, std::vector<KDailyInfo> );
        //INT_CASE_NOPARAM( DB_EVENT_UPDATE_MONTHLY_REWARD_ITEM );
        _INT_CASE( EVENT_MONTHLY_ATTEND_NOT, KDailyInfo );

        INT_CASE( DB_EVENT_MONTHLY_ATTEND_REWARD_REQ );

        INT_CASE_NOPARAM( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT );

        // 펫 부화
        _INT_CASE( EVENT_HATCHING_EGG_REQ, KEVENT_DB_HATCHING_EGG_REQ );

        INT_CASE_NOPARAM( DB_EVENT_TIME_SYNCRONIZE_NOT );

        // 아이템 구매.
        _INT_CASE( EVENT_BUY_GCPOINT_ITEM_REQ, KDB_EVENT_BUY_GCPOINT_ITEM_REQ );
        _INT_CASE( EVENT_SELL_GCPOINT_ITEM_REQ, KBuySellItemReq );

        _INT_CASE( EVENT_RESET_WIN_LOSE_REQ, KResetWinLoseData );

        INT_CASE( EVENT_LUCKYDAY_NOT );
        INT_CASE_NOPARAM( DB_EVENT_UPDATE_LUCKYDAY_LIST );

        INT_CASE( EVENT_STAT_PROMOTION_COUNT );
        INT_CASE_NOPARAM( EVENT_YAHOO_BENEFIT_NOT );
        _INT_CASE( EVENT_DB_STAT_LOADING_TIME_NOT, KEVENT_MAP_DWORD_DWORD );
        _INT_CASE( EVENT_INSERT_HACK_INFO_NOT, int );
        INT_CASE_NOPARAM( EVENT_GET_NASTY_INFO_REQ );
        INT_CASE( EVENT_REPORT_USER_REQ );
        _INT_CASE( EVENT_ENCHANT_REQ, KDB_EVENT_ENCHANT_REQ );

        INT_CASE_NOPARAM( EVENT_GET_CHATTING_EVENT_MSG_REQ );
        _INT_CASE( DB_EVENT_CASH_ERROR_REPORT, KDB_EVENT_CASH_ERROR_REPORT );
        INT_CASE( DB_EVENT_BONUS_POINT_REFILL_REQ );
        INT_CASE( EVENT_CHANGE_BONUS_COIN_REQ );
        INT_CASE_NOPARAM( EVENT_SLOT_INFO_NOT );
        _INT_CASE( EVENT_STAT_CPL_INSERT, MAP_CPL_STAT );
        _INT_CASE( EVENT_STAT_PLAY_TIME_INSERT, MAP_PLAY_TIME_STAT );
        _INT_CASE( EVENT_STAT_MAP_COUNT, MAP_INT_DWORD );
        INT_CASE_NOPARAM( EVENT_INDIGO_RANK_NOT );
        INT_CASE( EVENT_NEW_CHARACTER_REQ );
        _INT_CASE( EVENT_TW_USER_AGREEMENT_REQ, int );
        INT_CASE( EVENT_EMOTICON_MIX_REQ );
        _INT_CASE( EVENT_STAT_GET_ITEM, MAP_ITEM_STAT );
        INT_CASE_NOPARAM( EVENT_CONNECTION_MSG_REQ );

        INT_CASE_NOPARAM( EVENT_DB_UPDATE_RANK_DATA_REQ );
       _INT_CASE( EVENT_DB_MY_RANK_INFO_REQ, std::set<char> );
        INT_CASE( EVENT_RANK_SEARCH_REQ );
       _INT_CASE( EVENT_GET_HELL_ITEM_REQ, int );
        INT_CASE_NOPARAM( EVENT_DB_UPDATE_SELL_ITEM_LIST_REQ );
       _INT_CASE( EVENT_SELL_COUNT_ITEM_REQ, KDBSellCountItemData );

        INT_CASE_NOPARAM( EVENT_CURRENT_VIRTUAL_CASH_NOT );
       _INT_CASE( EVENT_BUY_VIRTUAL_CASH_REQ, KDB_EVENT_BUY_VIRTUAL_CASH_REQ );
        INT_CASE_NOPARAM( EVENT_SET_ABUSE_TO_DB_REQ );
       _INT_CASE( EVENT_DB_STAT_GAME_LOADING_TIME, KEVENT_MAP_DWORD_DWORD );
       _INT_CASE( EVENT_COLLECTION_COMPLETE_REQ, int );

        INT_CASE_NOPARAM( EVENT_MINIGAME_INFO_NOT ); //  080630  woosh. 미니게임 최고점수 얻기
        INT_CASE( EVENT_DB_UPDATE_MINIGAME_SCORE );
       _INT_CASE( EVENT_STAT_MINIGAME_INSERT, MAP_MINIGAME_STAT );
       _INT_CASE( EVENT_UNLOCK_SKILL_REQ, KDB_EVENT_UNLOCK_SKILL_REQ );
       _INT_CASE( EVENT_STAT_SPX_COUNT, MAP_SPX_STAT );
       _INT_CASE( EVENT_CHANGE_COUPON_REQ, KEVENT_CHANGE_COUPON_ACK );
       _INT_CASE( EVENT_STAT_DISCONNECT_COUNT, MAP_INT_DWORD );
       _INT_CASE( EVENT_GACHA_ACTION_REQ, KDB_EVENT_GACHA_ACTION_REQ );
        INT_CASE( EVENT_GACHA_SET_REWARD_REQ );
       _INT_CASE( EVENT_GACHA_LEVEL_SET_REWARD_REQ, KEVENT_GACHA_SET_REWARD_REQ );
       _INT_CASE( EVENT_STAT_LEAVE_GAME_COUNT, KEVENT_MAP_STAT_LEAVE_GAME );
       _INT_CASE( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_REQ, PAIR_INT );

        INT_CASE_NOPARAM( DB_EVENT_UPDATE_PERIOD_NOTICE );
        INT_CASE_NOPARAM( EVENT_FULL_COUPLE_INFO_REQ );
       _INT_CASE( EVENT_MAKE_COUPLE_REQ, KMakeCoupleDBData_Req );
       _INT_CASE( EVENT_COUPLE_REQ_DENY_SET, DWORD );
       _INT_CASE( EVENT_USE_COUPLE_ITEM_REQ, KCoupleItemData );
        INT_CASE( EVENT_CHANGE_COUPLE_RING_REQ );
       _INT_CASE( EVENT_DIVORCE_COUPLE_REQ, KDiborceDBData );
       _INT_CASE( EVENT_COUPLEITEM_EXPIRED_NOT, std::vector<KSimpleItem> );
        INT_CASE_NOPARAM( EVENT_GET_USER_DONATION_INFO_REQ );
        INT_CASE( EVENT_DONATION_POINT_REQ );
        INT_CASE_NOPARAM( DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM ); //081127.tslayer.주/월개근시 보상아이템 정보 갱신
        INT_CASE( DB_EVENT_WEEKLY_ATTEND_REWARD_REQ );
        INT_CASE_NOPARAM( EVENT_CHECK_LUNA_NEWYEAR_REQ );
        INT_CASE_NOPARAM( EVENT_LUNA_NEWYEAR_REQ );
       _INT_CASE( EVENT_CHAR_POLL_UPDATE_DB, MAP_CHAR_INT );
        INT_CASE_NOPARAM( EVENT_DB_GACHA_COIN_COUNT_REQ );
       _INT_CASE( EVENT_BUY_FOR_VIP_REQ, KDB_EVENT_BUY_FOR_VIP_REQ );
        INT_CASE_NOPARAM( EVENT_CHECK_GACHA_REEL_POINT_REQ );
       _INT_CASE( EVENT_GACHA_REEL_POINT_UPDATE_DB, GAGHA_REEL_POINT );
       
        INT_CASE( DB_EVENT_OPEN_CAPSULE_REQ );
       _INT_CASE( EVENT_UPDATE_GAME_COUPON_FROM_DB, int );
        INT_CASE( EVENT_USE_GAME_COUPON_REQ );
        INT_CASE_NOPARAM( EVENT_GAME_COUPON_FAIL_NOT );
        // 09.04.11. kkurrung. 추천 (5th)
        INT_CASE_NOPARAM( EVENT_CHECK_RECOMMEND_ENABLE_REQ );
        INT_CASE( DB_EVENT_RECOMMEND_USER_REQ );
        INT_CASE_NOPARAM( EVENT_RECOMMEND_FULL_INFO_REQ );
       _INT_CASE( EVENT_RECOMMENDER_ACCEPT_REQ, std::vector<DWORD> );
       _INT_CASE( EVENT_ATTENDANCE_BONUS_REQ, int );
       _INT_CASE( EVENT_RECOM_DAILY_ATTENDANCE_REQ, bool );
       _INT_CASE( EVENT_RECOMMENDER_DENY_REQ, std::vector<DWORD> );
       _INT_CASE( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, KEVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK );
        INT_CASE_NOPARAM( DB_EVENT_SANTA_DATA );
        INT_CASE_NOPARAM( DB_EVENT_VIRTUAL_CASH_RESTRICT_ITEM_LIST );
       _INT_CASE( EVENT_MISSION_PACK_REGIST_REQ, KEVENT_MISSION_PACK_REGIST_ACK );

        INT_CASE( DB_EVENT_DUNGEON_HACKING_USER_NOT );
       _INT_CASE( EVENT_GET_GCCLUB_CONNECT_BONUS_REQ, std::vector<KDropItemInfo> );
       _INT_CASE( EVENT_GET_GCCLUB_ATTEND_BONUS_REQ, std::vector<KDropItemInfo> );
        // 090520 tgkwon. ping 통계
       _INT_CASE( EVENT_STAT_PING_INSERT_DB, MAP_PING_STAT );
       // 09.07.12. kkurrung. RainBow
        INT_CASE( EVENT_RAINBOW_JACKPOT_ITEM_REQ );
        INT_CASE( EVENT_EFFECT_TEX_REPORT );       
       _INT_CASE( EVENT_CO_OP_ITEM_BUY_REQ, KCoopEventItem );
        INT_CASE_NOPARAM( EVENT_GET_USER_HACKING_COUNT_REQ );
        INT_CASE( EVENT_ITEM_TRADE_REQ );
        INT_CASE( EVENT_MISSION_ITEM_NOT );
        INT_CASE_NOPARAM( EVENT_MY_SUBSCRIPTION_INFO_REQ );
        INT_CASE( EVENT_SUBSCRIPT_REQ );
        INT_CASE( EVENT_NPC_GIFTS_REQ );
        // 09.10.07 tgkwon 주사위 게임.
        INT_CASE_NOPARAM( EVENT_GET_USER_BOARD_GAME_INFO );
        INT_CASE( EVENT_DB_DICE_PLAY_REQ );
        INT_CASE( EVENT_DICE_ITME_BUY_CHECK_REQ );
        // 090924 tgkwon 추석 송편 기부 이벤트.
        INT_CASE( EVENT_DB_DONATION_ITEM_GET_REQ );
        INT_CASE_NOPARAM( EVENT_DONATION_INFO_REQ );

        // 091118. kkurrung. 퀴즈 빙고
        INT_CASE_NOPARAM( EVENT_USER_BINGO_DATA_REQ );
       _INT_CASE( EVENT_CHANGE_BINGO_COIN_REQ, int );
       _INT_CASE( EVENT_BINGO_QUESTION_REQ, int );
       _INT_CASE( EVENT_BINGO_ANSWER_REQ, KBingoAnswerReqForDB );
        INT_CASE( EVENT_DB_BINGO_ACC_TIME_UPDATE );

        // 대만 대회참가승리팀 예상 기부
        INT_CASE( EVENT_TOURNAMENT_DONATION_REQ );
       _INT_CASE( EVENT_ITEM_BUY_CHECK_REQ, GCITEMID );
	   
        INT_CASE( EVENT_WEB_GACHA_COIN_TRADE_REQ );
       _INT_CASE( EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, MAP_AD_COUNT );
        // 091210. kkurrung. GEC Event
        INT_CASE_NOPARAM( EVENT_GWC_EVENT_INFO_REQ );
       _INT_CASE( EVENT_GWC_EVENT_DONATION_REQ, int );
 // 길드
        INT_CASE( EVENT_SEARCH_GUILD_LIST_REQ );
        INT_CASE( EVENT_MARK_UPLOAD_REQ );
        INT_CASE( EVENT_CREATE_GUILD_REQ );
       _INT_CASE( EVENT_EDIT_GUILD_NOTICE_REQ, KNGuildNoticeList );
        INT_CASE( EVENT_EDIT_GUILD_MY_COMMENT_REQ );
        INT_CASE( EVENT_EDIT_GUILD_NAME_REQ );
        INT_CASE( EVENT_GUILD_MEMBER_LIST_REQ );
       _INT_CASE( EVENT_EDIT_GUILD_URL_REQ, KNGuildURL );
       _INT_CASE( EVENT_GET_GUILD_INFO_FROM_DB_REQ, DWORD );
       _INT_CASE( EVENT_GET_GUILD_MEMBER_LIST_FROM_DB_REQ, DWORD );
        
        // 091230. tgkwon 새해 소원 성취 이벤트
        INT_CASE( EVENT_NEW_YEAR_DONATION_REQ );
       _INT_CASE( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT, KECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
       _INT_CASE( EVENT_CHECK_UPGRADE_GUILD_GRADE_GIFT_NOT, DWORD );

       _INT_CASE( EVENT_SOCKET_OPEN_REQ, KEVENT_SOCKET_OPEN_ACK );
       _INT_CASE( EVENT_MONSTER_CARD_INSERT_REQ, KEVENT_MONSTER_CARD_INSERT_ACK );
       _INT_CASE( EVENT_MONSTER_CARD_REMOVE_REQ, KEVENT_MONSTER_CARD_REMOVE_ACK );

       INT_CASE( EVENT_ATTRIBUTE_SELECT_REQ  );
        // #EVENT_CASES
        // 09.12.07. tgkwon. 추천 (5th) 추가작업( 신규유저 Levelup시 아이템 지급. )
        INT_CASE( DB_EVENT_RECOMMEND_LEVELUP_NOT );
        INT_CASE_NOPARAM( DB_EVENT_RECOMMEND_LEVELUP_CHECK );
        // 100118 tgkwon. 주사위 이벤트 구조 변경
        INT_CASE( DB_EVENT_INIT_DICE_PLAY_REQ );

        INT_CASE( EVENT_ITEM_BREAKUP_REQ );
        INT_CASE( DB_EVENT_UPDATE_TIME_DROP_NOT );
       _INT_CASE( DB_EVENT_TIME_DROP_REQ, KTimeDropItem );
        INT_CASE_NOPARAM( DB_EVENT_GET_TIME_DROP_INFO_REQ );
       _INT_CASE( EVENT_GET_WEB_CASH_ITEM_REQ, int );
        INT_CASE( EVENT_STAT_RESOLUTION_NOT );
        INT_CASE( EVENT_MANUFACTURES3_REQ );
        // 100223. tgkwon. 선물상자 이벤트
        INT_CASE( EVENT_PRESENTBOX_TRADE_REQ );
        INT_CASE( DB_EVENT_PRESENTBOX_ACTION_REQ );
        // 100310 tgkwon. 구매제한 아이템 Count 증가.
       _INT_CASE( DB_EVENT_UPDATE_BUY_CHECK_COUNT_REQ, GCITEMID );
        INT_CASE( DB_EVENT_GUILD_PAGE_BUILD_NOT );
       _INT_CASE(  EVENT_GUILD_LIST_REQ, KGuildPageBuild );
        INT_CASE( DB_EVENT_ONE_GUILD_PAGE_BUILD_NOT );
       _INT_CASE( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, PAIR_SHORT );
        INT_CASE( DB_EVENT_UPDATE_DUNGEON_STAT_NOT );
       _INT_CASE( DB_EVENT_UPDATE_GUILD_PAGE_INFO_NOT, DWORD );
       _INT_CASE( EVENT_BUY_FOR_GAMBLE_REQ, KDB_EVENT_BUY_FOR_GAMBLE_REQ );

       _INT_CASE( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, KDB_EVENT_CHANGE_NORMAL_TO_LOOK_REQ );
       _INT_CASE( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, KChangeLookItem );
        INT_CASE_NOPARAM( DB_EVENT_FULL_LOOK_INFO_REQ );

        INT_CASE( DB_EVENT_BUNDLE_SELL_ITEM_REQ );
        INT_CASE( DB_CHANNELING_EVENT_CHECK );
        INT_CASE( DB_EVENT_CHAR_PROMOTION_UPDATE_REQ );
        INT_CASE_NOPARAM( EVENT_USER_HERO_DUNGEON_INFO_REQ );
        INT_CASE( DB_EVENT_USER_HERO_DUNGEON_INFO_UPDATE_NOT );
       _INT_CASE( EVENT_BUY_FOR_HERO_REQ, KDB_EVENT_BUY_FOR_HERO_REQ );
       _INT_CASE( DB_EVENT_STAT_LOADING_INFO, KLoadingInfo );
        INT_CASE( EVENT_EXP_UPDATE_TO_DB_NOT );
        INT_CASE( EVENT_PVEXP_UPDATE_TO_DB_NOT );

        INT_CASE_NOPARAM( DB_EVENT_GAWIBAWIBO_INFO_REQ );
       _INT_CASE( DB_EVENT_GAWIBAWIBO_INFO_UPDATE, KUserGawibawiboInfo );
       _INT_CASE( DB_EVENT_GAWIBAWIBO_INIT_INFO_REQ, KUserGawibawiboInfo );
       _INT_CASE( EVENT_STAT_LOGINOUT_COUNT, KCountInfo );
        INT_CASE( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ );
       _INT_CASE( EVENT_SPECIAL_REWARD_REQ, KRewardInfoList );
       _INT_CASE( EVENT_DUNGEON_REWARD_ITEM_REQ, KDB_EVENT_DUNGEON_REWARD_ITEM_REQ );

        INT_CASE_NOPARAM( DB_EVENT_SOCKS_INFO_REQ );
       _INT_CASE( DB_EVENT_SOCKS_INFO_UPDATE, KUserSocksInfo );
       _INT_CASE( DB_EVENT_SOCKS_HANGUP_REQ, KUserSocksInfo );
       _INT_CASE( DB_EVENT_SOCKS_COLLECT_REQ, KUserSocksInfo );
       _INT_CASE( DB_EVENT_SOCKS_INSERT_ITEM, KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT );
       _INT_CASE( DB_EVENT_SOCKS_INSERT_ITEM_NEW, KSocksExchangeData );

       _INT_CASE( EVENT_CHAT_NOT, KChatData );
        INT_CASE_NOPARAM( DB_EVENT_INIT_DICE_DAILY_FREE );
        INT_CASE( DB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ );

        INT_CASE_NOPARAM( EVENT_TODAYS_POPUP_INFO_REQ );
        INT_CASE_NOPARAM( EVENT_DISABLE_TODAYS_POPUP_NOT );
       _INT_CASE( EVENT_WHISPER_REQ, KChatData );
        INT_CASE_NOPARAM( EVENT_FASHION_CATALOG_REQ );
       _INT_CASE( EVENT_USER_SERVER_ROOM_REQ, std::wstring );
       _INT_CASE( EVENT_USER_RECOM_TYPE_REQ, int );
        INT_CASE( DB_EVENT_UPDATE_CHARACTER_STAT_NOT );

        // 110222. tgkwon. 나무나무 이벤트.
        INT_CASE_NOPARAM( EVENT_PLANT_TREE_INFO_REQ );
        INT_CASE( EVENT_PLANT_TREE_ACTION_REQ );
        INT_CASE( DB_EVENT_PLANT_TREE_REWARD_REQ );

       _INT_CASE( EVENT_BUY_FOR_GUILD_REQ, KDB_EVENT_BUY_FOR_GUILD_REQ );
       _INT_CASE( DB_EVENT_UPDATE_GUILD_BATTLE_POINT_REQ, KGuildGameResult );
        INT_CASE_NOPARAM( DB_UPDATE_GUILD_LIST_REQ );

        INT_CASE_NOPARAM( EVENT_SQUARE_LIST_REQ );
       _INT_CASE( EVENT_CHOICE_BOX_OPEN_REQ, KDB_EVENT_CHOICE_BOX_OPEN_REQ );
        INT_CASE_NOPARAM( DB_EVENT_VIPEVENT_INFO_REQ );
       _INT_CASE( EVENT_VIPEVENT_USER_INFO_REQ, USHORT );
       _INT_CASE( DB_EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT, time_t );

       _INT_CASE( EVENT_UNLOCK_CHANGE_WEAPON_REQ, char );
        INT_CASE( EVENT_USER_AUTH_CHECK_REQ );
        INT_CASE( EVENT_ITEM_LEVEL_SELECT_REQ );

       _INT_CASE( DB_EVENT_MAGIC_BOX_LIST_NOT, USHORT );
        INT_CASE_NOPARAM( EVENT_MAGIC_BOX_LIST_REQ );
       _INT_CASE( EVENT_SET_MAGIC_BOX_TARGET_REQ, GCITEMID );
       _INT_CASE( EVENT_USE_MAGIC_BOX_REQ, KDB_EVENT_USE_MAGIC_BOX_REQ );
        INT_CASE_NOPARAM( DB_EVENT_GET_PACKAGE_LIST_NOT );
        INT_CASE( EVENT_RKTORNADO_ACTION_REQ );
        INT_CASE( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ );

        INT_CASE_NOPARAM( EVENT_USER_SPHINX_DATA_REQ );
        INT_CASE( EVENT_SPHINX_QUESTION_REQ );
        INT_CASE( DB_EVENT_SPHINX_ANSWER_REQ );

       _INT_CASE( EVENT_DUNGEON_STAT_INFO, KDungeonStatAddA );
       _INT_CASE( DB_EVENT_SQUARE_STAT_TIME, DWORD );

        INT_CASE( EVENT_STRENGTH_EQUIP_REQ );
        INT_CASE( EVENT_STRENGTH_ACTION_REQ );
        INT_CASE( EVENT_STRENGTH_BREAKUP_REQ );
        INT_CASE( EVENT_STRENGTH_COMPOSE_REQ );

       _INT_CASE( EVENT_BUY_FOR_AGIT_REQ, KDB_EVENT_BUY_FOR_AGIT_REQ );
       _INT_CASE( EVENT_SELL_FOR_AGIT_REQ, KDB_EVENT_SELL_FOR_AGIT_REQ );
       _INT_CASE( EVENT_HARVEST_FLOWER_POT_NOT, KHarvestFlowerPotAck );
       _INT_CASE( EVENT_AGIT_STAY_BONUS_DROP_REQ, KDropItemInfo );
        INT_CASE_NOPARAM( EVENT_TODAYS_SEED_DROP_COUNT_REQ );
       _INT_CASE( EVENT_BUY_FOR_BROWNIE_REQ, KDB_EVENT_BUY_FOR_BROWNIE_REQ );
       _INT_CASE( EVENT_AGIT_INIT_SEED_REQ, KDropItemInfo );
       _INT_CASE( EVENT_DROP_FLOWER_POT_REQ, KAddItemReq );
        INT_CASE_NOPARAM( EVENT_AGIT_TUTORIAL_DONE_REQ );
       _INT_CASE( EVENT_AGIT_TUTORIAL_REQ, DROP_ITEMS );
        INT_CASE_NOPARAM( EVENT_FAIRY_TREE_BUFF_REQ );
        INT_CASE_NOPARAM( EVENT_AGIT_USER_OPTION_REQ );

        INT_CASE( EVENT_ENTER_AGIT_NOT );
        INT_CASE( EVENT_BUY_AGIT_MAP_NOT );
        INT_CASE( EVENT_NMNETCAFE_BONUS_REQ );

        INT_CASE( EVENT_STAT_CHAR_EQUIP_NOT );
        INT_CASE( EVENT_STAT_DUNGEON_STAGE_NOT );
        INT_CASE( EVENT_STAT_CHAR_LV_PT_NOT );

        INT_CASE( DB_EVENT_SURVEY_ANSWER_REQ );
       _INT_CASE( EVENT_SEALLING_CALENDAR_REQ, KEVENT_SEALLING_CALENDAR_ACK );
       _INT_CASE( DB_EVENT_GWC_RANKING_REWARD_REQ, VECTOR_STRUCT_DROPITEM );
        INT_CASE( DB_EVENT_JUST_IN_TIME_REWARD_REQ );

        INT_CASE_NOPARAM( EVENT_CHARISMAS_EVENT_INFO_REQ );
       _INT_CASE( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, std::wstring );
        INT_CASE( DB_EVENT_CHARISMAS_EVENT_PRESENT_REQ );
        INT_CASE( EVENT_CHARISMAS_EVENT_DONATION_REQ );
        INT_CASE( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ );

        INT_CASE( EVENT_STAT_HOTKEY_USED_INFO_NOT );
        INT_CASE( DB_EVENT_ECLIPSE_COLLECT_REWARD_REQ );
       _INT_CASE( DB_EVENT_ECLIPSE_COLLECT_REWARD_CHECK_REQ, time_t );

        INT_CASE_NOPARAM( DB_EVENT_INIT_POST_LETTER_LIST_REQ );
       _INT_CASE( EVENT_SEND_LETTER_REQ, KDB_EVENT_SEND_LETTER_REQ );
       _INT_CASE( EVENT_READ_LETTER_NOT, POSTUID );
        INT_CASE( EVENT_GET_ITEM_FROM_LETTER_REQ );
        INT_CASE( DB_EVENT_DELETE_LETTER_REQ );
        INT_CASE_NOPARAM( EVENT_NEW_POST_LETTER_INFO_REQ );
        INT_CASE( EVENT_DUNGEON_CLEAR_REWARD_REQ );
        INT_CASE( DB_EVENT_UPDATE_DUNGEON_CLEAR_INFO_NOT );
       _INT_CASE( EVENT_EQUIP_LEVEL_DOWN_REQ, KDB_EVENT_EQUIP_LEVEL_DOWN_REQ );
        INT_CASE( EVENT_MONSTER_CARD_MIX_REQ );

        INT_CASE( DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ );
        INT_CASE_NOPARAM( EVENT_ECLIPSE_PLOT_INFO_REQ );
        INT_CASE( DB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ );
        INT_CASE( DB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ );

        INT_CASE( EVENT_DEPOT_CHAR_TAB_INFO_REQ );
        INT_CASE_NOPARAM( EVENT_DEPOT_INFO_REQ );
       _INT_CASE( DB_EVENT_UPDATE_DEPOT_DATA_REQ, KDepotUpdateData );
       _INT_CASE( DB_EVENT_DELETE_DEPOT_DATA_REQ, KDepotUpdateData );
       
       _INT_CASE( EVENT_DEPOT_EXTEND_REQ, KDepotExtend );
       _INT_CASE( EVENT_DEPOT_CREATE_REQ, KDepotInfo );

        INT_CASE_NOPARAM( DB_EVENT_SELECT_GACHA_NOTICE_POPUP_INFO );
        INT_CASE_NOPARAM( EVENT_UPDATE_GACHA_POPUP_INFO );
        INT_CASE( EVENT_ADD_SKILL_SLOT_OPEN_REQ );
        INT_CASE( EVENT_STAT_DUNGEON_CHARACTER_NOT );
        INT_CASE( DB_EVENT_USER_DUNGEON_STAT_NOT );
       _INT_CASE( EVENT_STAT_CHARACTER_SKILL_COUNT, KEVENT_STAT_CHARACTER_SKILL_COUNT );

       _INT_CASE( DB_EVENT_SONGKRAN_USER_INFO_REQ, bool );
       _INT_CASE( EVENT_SONGKRAN_GRADE_REWARD_REQ, KDB_EVENT_SONGKRAN_GRADE_REWARD_REQ );
       _INT_CASE( DB_EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ, KItem );
        INT_CASE( DB_EVENT_SONGKRAN_WATERBOMB_USE_REQ );
        INT_CASE( DB_EVENT_SONGKRAN_WATERBOMB_NOT );

        INT_CASE_NOPARAM( EVENT_ATTEND_GET_CHAR_REQ );
        _INT_CASE( EVENT_STAT_VIEW_GRADE_TIME_NOT, KUserViewGradeTime );

        _INT_CASE( DB_EVENT_ADVENTURE_DATA_REQ, int );
        INT_CASE( EVENT_ADVENTURE_CONTINENT_REWARD_REQ );
        INT_CASE( DB_EVENT_ADVENTURE_FINAL_REWARD_REQ );
        INT_CASE( EVENT_ADVENTURE_USER_COLLECT_DATA_NOT );

        _INT_CASE( DB_EVENT_LEVEL_RESET_EVENT_INFO_REQ, DWORD );
        INT_CASE( DB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ );
        INT_CASE( DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ );
        INT_CASE_NOPARAM( EVENT_LOGIN_TIME_UPDATE_REQ );
        INT_CASE( EVENT_TEST_ADD_ITEM_REQ );
        INT_CASE( EVENT_NEW_CHAR_CHOICE_REQ );
        INT_CASE( DB_EVENT_SEALED_CHARACTER_UPDATE_NOT );
        INT_CASE( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ );
        INT_CASE( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ );
        INT_CASE( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ );
        INT_CASE( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT );
        INT_CASE( EVENT_OLYMPIC_TREEDROP_RESULT_REQ );
        INT_CASE( DB_EVENT_OLYMPIC_JOIN_GAME_REQ );
        INT_CASE( EVENT_OLYMPIC_BALLOON_RESULT_REQ );
        INT_CASE( DB_EVENT_OLYMPIC_INFO_REQ );
        INT_CASE( EVENT_OLYMPIC_REWARD_REQ );
       _INT_CASE( DB_EVENT_USER_DUNGEON_LOADING_INFO, KUserLoadingStat );
        INT_CASE( DB_EVENT_USER_CHANGE_CHAR_STAT );
        INT_CASE( DB_EVENT_USER_MODE_CLEAR_STAT );
        INT_CASE( DB_EVENT_ROOM_MONSTER_KILL_STAT );
        INT_CASE( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ );
        INT_CASE( DB_EVENT_ITEM_COMPOSE_REQ );
        INT_CASE( DB_EVENT_GACHA_LOTTERY_ACTION_REQ );
        INT_CASE( DB_EVENT_SKILL_OPENKEY_USE_REQ );

        INT_CASE( DB_EVENT_GP_ATTRIBUTE_RANDOM_REQ );
        INT_CASE_NOPARAM( DB_EVENT_SKILL_MISSION_INFO_NOT );
       _INT_CASE( DB_EVENT_MONSTER_TYPE_STAT_INFO, KMonsterTypeStat );
        INT_CASE( DB_EVENT_MATCH_CHAR_DATA_REQ );
        INT_CASE( DB_EVENT_DEL_MATCH_REQ );
        INT_CASE( DB_EVENT_MATCH_ELO_RESULT_REQ );

        INT_CASE( DB_EVENT_HERO_ITEM_UPGRADE_REQ );

        INT_CASE_NOPARAM( EVENT_RITAS_CHRISTMAS_USER_INFO_REQ);
        INT_CASE_NOPARAM( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ );
       _INT_CASE( DB_EVENT_RITAS_CHRISTMAS_UPDATE_CONTINUE_COIN_NOT, int );
       _INT_CASE( DB_EVENT_RITAS_CHRISTMAS_UPDATE_RECORD_NOT, int );
        INT_CASE( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ );
       _INT_CASE( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ, KItem );

        INT_CASE( EVENT_STAT_DUNGEON_LEAVE_NOT );
        INT_CASE( EVENT_STAT_USER_PLAY_TIME_NOT );
        INT_CASE( EVENT_STAT_LEVEL_GP_INFO_NOT );

        INT_CASE_NOPARAM( DB_EVENT_UPDATE_MATCH_RANK_DATA_REQ );
       _INT_CASE( DB_EVENT_MY_MATCH_RANK_INFO_REQ, std::set<char> );
        INT_CASE( EVENT_MATCH_RANK_SEARCH_REQ );

       _INT_CASE( EVENT_UPDATE_ITEM_COUNT_NOT, std::vector< KItem > );

        INT_CASE( DB_EVENT_BUFF_DONATION_REQ );
        INT_CASE_NOPARAM( EVENT_BUFF_DONATION_LOGIN_INFO_REQ );

        INT_CASE_NOPARAM( EVENT_ACCUMULATE_ATTENDANCE_REQ );
        INT_CASE( DB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ );
        INT_CASE( DB_EVENT_PVP_BALANCE_STAT_REQ );

        INT_CASE_NOPARAM( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ );

        INT_CASE_NOPARAM( DB_EVENT_ERRAND_USER_INFO_REQ );
        INT_CASE( DB_EVENT_ERRAND_REWARD_REQ );
        INT_CASE( EVENT_INIT_ITEM_ATTRIBUTE_REQ );

        INT_CASE_NOPARAM( DB_EVENT_SYSTEM_GUIDE_COMPLETE_INFO_REQ );
        INT_CASE( DB_EVENT_SYSTEM_GUIDE_ITEM_REQ );
        INT_CASE( DB_EVENT_SYSTEM_GUIDE_COMPLETE_REQ );	   
        INT_CASE( EVENT_LOOK_INVENTORY_EXTEND_REQ );
		
       _INT_CASE( EVENT_DUNGEON_SUBJECT_REWARD_REQ, KEVENT_DUNGEON_SUBJECT_REWARD_ACK );
       _INT_CASE( EVENT_STAT_DUNGEON_SUBJECT_NOT, KDungeonSubjectStat );

        INT_CASE( DB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ );
       _INT_CASE( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT, time_t );
        INT_CASE( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ );		
        INT_CASE( DB_EVENT_CHANGE_CHARACTER_INFO_REQ );
        INT_CASE( DB_EVENT_ADD_SLOT_REQ );
        INT_CASE( DB_EVENT_VITALITY_INFO_REQ );
        INT_CASE( DB_EVENT_VITALITY_RECHARGE_NOT );
        INT_CASE( EVENT_STAT_VITALITY_DUNGEON_PLAY_NOT );
        INT_CASE( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ );

        INT_CASE( DB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ );
        INT_CASE( DB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ );

        INT_CASE_NOPARAM( DB_EVENT_USER_ATTRIBUTE_MIGRATION_INFO_REQ );

        INT_CASE_NOPARAM( EVENT_JUMPING_CHAR_INFO_NOT );
       _INT_CASE( EVENT_JUMPING_CHAR_REWARD_REQ, VECTOR_STRUCT_DROPITEM );
        INT_CASE( DB_EVENT_JUMPING_CHAR_REQ );
        INT_CASE( EVENT_INSERT_DUNGEON_HACK_NOT );
       _INT_CASE( EVENT_SKILL_TRAINING_REQ, int );
        INT_CASE( EVENT_USER_RELAY_SEND_COUNT_REQ );
        INT_CASE( DB_EVENT_SYSTEM_RESOURCE_INFO );
        INT_CASE( DB_EVENT_COORDI_COMPOSE_REQ );

        INT_CASE_NOPARAM( DB_EVENT_VIRTUAL_DEPOT_ITEM_LIST_REQ );
        INT_CASE( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ );

       _INT_CASE( DB_EVENT_CLIENT_NETWORK_REPORT_NOT, MAP_INT_INT );

        INT_CASE_NOPARAM( DB_EVENT_TONG_DONATION_TOTAL_AMOUNT_INFO_REQ );
        INT_CASE_NOPARAM( DB_EVENT_TONG_DONATION_USER_AMOUNT_INFO_REQ );
        INT_CASE( EVENT_TONG_DONATION_DONATE_REQ );

        INT_CASE_NOPARAM( DB_EVENT_SOSCIAL_COMMERCE_BUY_COUNT_INFO );
        INT_CASE( DB_EVENT_SOSCIAL_COMMERCE_BUY_REQ );
       _INT_CASE( DB_EVENT_SOSCIAL_COMMERCE_SELL_STATE_FLAG_UPDATE_REQ, PAIR_DWORD_INT );
        INT_CASE( DB_EVENT_USER_UDP_PORT_INFO_REQ );
        INT_CASE( DB_EVENT_COORDI_GRADE_UPGRADE_REQ );
        INT_CASE( DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ );

       _INT_CASE( EVENT_NICKNAME_VALIDITY_CHECK_REQ, std::wstring );

        INT_CASE( DB_EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ );
        INT_CASE( DB_EVENT_INFINITY_DUNGEON_STAT_NOT );
        INT_CASE_NOPARAM( DB_EVENT_SHANGHAI_MISSION_INFO_NOT );

        INT_CASE_NOPARAM( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );
        INT_CASE_NOPARAM( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ );

        INT_CASE( DB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ );

        INT_CASE( EVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT );

        INT_CASE( DB_EVENT_GACHA_PONG_ACTION_REQ );

        INT_CASE( DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ );

        INT_CASE( EVENT_COORDIVIEWER_GETSLOT_REQ );
        INT_CASE( EVENT_COORDIVIEWER_SETSLOT_REQ );

        INT_CASE( DB_EVENT_BILLBOARD_CHAT_MSG_LOG_REQ );

        INT_CASE( DB_EVENT_USE_HERO_TICKET_REQ );

       // #END_OF_CASE_GSDBTHREAD

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    SiKTickManager()->UpdateDBThreadTime( kIntEvent_.m_usEventID, dwElapTime );
}

_INT_IMPL_ON_FUNC(EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ) // ������ ó�� ���ӽ�
{
    KEVENT_DB_VERIFY_ACCOUNT_ACK kPacket;
    KEVENT_VERIFY_ACCOUNT_ACK& kVerifyPacket = kPacket.m_kVerifyPacket;
    KEVENT_QUERY_INVENTORY_INFO_ACK& kInvenPacket = kPacket.m_kInvenPacket;
    KEVENT_GIFT_ITEM_NOT& kGiftEvent = kPacket.m_kGiftItem;
    KEVENT_BAD_USER_INFO_NOT& kBadUserInfo = kPacket.m_kBadUserInfo;
    KEVENT_COLLECTION_MISSION_NOT& kCollectionMission = kPacket.m_kCollectionMission;
    KFullSPInfo& kFullSPInfo = kPacket.m_kFullSPInfo;

    KRecommenderInfo& kRecomInfo = kPacket.m_kMyRecomInfo;
    kRecomInfo.Clear();
    kPacket.m_bReadOldModwLevel = false; // ���� ��Ÿ���� Mode Level�� �о�����.
    std::vector<KGiftInfo>& vecGiftInfo = kPacket.m_vecGiftInfo;
    KEVENT_USER_CHANGE_WEAPON_NOT& kChangeWeapon = kPacket.m_kChangeWeapon;

    kGiftEvent.m_vecGiftItemList.clear();
    vecGiftInfo.clear();

    kVerifyPacket.m_ucOK = -50;
    kVerifyPacket.m_nPubEvent = kPacket_.m_nPubEvent;
    kVerifyPacket.m_iAge = kPacket_.m_kPacket.m_iAge;
    kVerifyPacket.m_dwAuthType = kPacket_.m_kPacket.m_dwAuthType;// 060626. kkurrung. NxJ �ǽù� ���� NMJ������ ������ ���� �ʵ��� �Ѵ�.(ä�θ�)
    kVerifyPacket.m_dwChannelType = kPacket_.m_kPacket.m_dwChannellingType;
    kVerifyPacket.m_strPassword = kPacket_.m_kPacket.m_strPasswd; // �߱� ������ �ʿ�, �α伭���� ���� �뵵
    std::wstring wstrLogin = KncUtil::toWideString(kPacket_.m_kPacket.m_strLogin);
    std::wstring strReqPasswd = KncUtil::toWideString(kPacket_.m_kPacket.m_strPasswd);

    std::vector< sSanta > vecSanta;
    std::set<char> setOldSkillTreechar;
    std::vector< int > vecVersions;

    CTime tmCurrent = CTime(2022, 1, 1, 12, 0, 0); // �⺻��.
    kVerifyPacket.m_tVirtualEnableDate = KncUtil::TimeToInt(tmCurrent);

    // FinalLoginTime ��µ� �ʿ� �� ���� �߰�
    // DB���� ���� �� �ð� ���� ������ �ƹ� �͵� ���� �ʱ⿡ �˻翡 ��
    CTime tmCurrentTime = CTime::GetCurrentTime();
    time_t tCurrentTimeStamp = KncUtil::TimeToInt(tmCurrentTime);
    std::vector<std::pair<GCITEMUID, GCITEMID>> vecDesignCoordi;

    DBJIF( m_kODBC.Query( L"{ call dbo.gnu_exist_user_check ( N'%s' ) }", wstrLogin.c_str() ) );
    if ( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kVerifyPacket.m_ucOK );
        m_kODBC.EndFetch();
        if ( kVerifyPacket.m_ucOK != 0 )
        {
            SET_ERROR( ERR_NUSER_VERIFY_09 );
            goto verify_end;
        }
    }

    if (kPacket_.m_kPacket.m_strLogin.empty())
    {
        kVerifyPacket.m_ucOK = 7;
        goto verify_end;
    }

    if (SiKGSSimLayer()->CheckAuthTypeFlag(KGSSimLayer::AT_LEVELUP) && !SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP)) {

        std::wstring strSALT(L"");
        m_kHelper.GetCommonSP().GetSALT(wstrLogin, strSALT); // ������ SALT �� ���´�

        boost::to_upper(strReqPasswd);

        strReqPasswd = SiKGSSimLayer()->MakeSHA1(strReqPasswd + strSALT); // encrypted passwd = SHA1( MD5 + SALT )

    }

    kVerifyPacket.m_iSessionInfo = KUser::SI_INVALID_VALUE;

    // NexonID, NexonUID�� ���� �ϴ��� Ȯ�� 
    DBJIF(m_kODBC.Query(L"{ call dbo.up_query_user_passwd ( N'%s' ) }",
        wstrLogin.c_str()));
    if (m_kODBC.BeginFetch()) { //����� ������
        std::wstring wstrPasswdDB;
        VERIFY_QUERY(m_kODBC >> wstrPasswdDB
            >> kVerifyPacket.m_strLogin);
        m_kODBC.EndFetch();
        // CenterServer���� ���� üũ�� �ѹ� �ϱ� ������ ���Ӽ������� 2�� ���� ó���ϴ� �κ�
        // �ּ� ó��.

        // �츮 DB�� ����� PW üũ ����.
        if (!SiKGSSimLayer()->CheckTypeFlag(ST_DEBUG_AUTH) &&
            !SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_GASH) &&
            !SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_FUNBOX_TH) &&
            !SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_LIN) &&
            !SiKGSSimLayer()->CheckAuthTypeFlag(KSimLayer::AT_CYOU)
            )
        {
            std::wstring strLwPasswdDB = boost::to_lower_copy(wstrPasswdDB);
            std::wstring strLwPasswdUser = boost::to_lower_copy(strReqPasswd);
            if (strLwPasswdDB != strLwPasswdUser)
            {
                // �����ϴ� ������̳� passwd�� �ٸ�
                SiKLogManager()->Get(KLogManager::LT_PASSWORD)
                    << L"Login : " << wstrLogin
                    << L", User :" << strReqPasswd
                    << L", DB :" << wstrPasswdDB
                    << L", Time : " << KNC_TIME_STRING
                    << GLOG::endl;

                START_LOG(cerr, L"incorrect passwd. login : " << wstrLogin) << END_LOG;
                kVerifyPacket.m_ucOK = 1;
                goto verify_end;
            }
        }

    }
    else { //����� �������
        if (SiKGSSimLayer()->CheckTypeFlag(ST_DEBUG_AUTH) ||
            SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP))
        {

            /*
            20081108. microcat.
            L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }"

            { call dbo.up_insert_new_user_20130402 ( @01, @02, @03, @04 ) }
            @01: @01strLogin_input      nvarchar( 40 )
            @02: @02strPassword_input   nvarchar( 32 )
            @03: @03bSex_input          bit
            @04: @04iChannelingID_input tinyint = 1

            1 select ( @1 )
            @1: OK int
            0       : ����
            -1      : ������ �̹� ������
            -101����: Ʈ����� ����
            */

            if (!m_kODBC.Query(L"{ call dbo.up_insert_new_user_20130402 ( N'%s', N'%s', %d, %d ) }",
                wstrLogin.c_str(),
                strReqPasswd.c_str(),
                kPacket_.m_kPacket.m_bMale ? 0 : 1,
                kVerifyPacket.m_dwChannelType))
            {
                kVerifyPacket.m_ucOK = 8;
                goto verify_end;
            }

            if (m_kODBC.BeginFetch())
            {
                int nOk = -99;
                VERIFY_QUERY(m_kODBC >> nOk);
                m_kODBC.EndFetch();
                if (nOk != 0)
                {
                    kVerifyPacket.m_ucOK = 8;
                    goto verify_end;
                }
            }
        }
        else
        {
            kVerifyPacket.m_ucOK = 8;
            goto verify_end;
        }
    }

    //���⼭���ʹ� ��Ȯ�� ��ġ�ϴ� ����� ������    //  20080219 woosh 
    kVerifyPacket.m_strLogin = KncUtil::toWideString(kPacket_.m_kPacket.m_strLogin);

    // ���� ������ DB���� �����´�.
    DBJIF(m_kODBC.Query(L"{ call dbo.UGGA_user_gameinfo_select ( N'%s', N'%s', N'%s' ) }",
        wstrLogin.c_str(),
        kPacket_.m_kPacket.m_strGetIP.c_str(),
        SiKGameServer()->GetServerName().c_str())); //050729. microcat. �������� �� IP�� ����Ѵ�.

    START_LOG(clog, L" ���� ���� ���� ����.. ")
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << BUILD_LOG(wstrLogin)
        << BUILD_LOG(kPacket_.m_kPacket.m_strGetIP) << END_LOG;

    int nFirstConnect = 0;  // ���� ����. �� ������ ĳ���� ������ ���� ������ ����д�.
    if (m_kODBC.BeginFetch())
    {
        kVerifyPacket.m_mapDifficulty.clear();

        // 061116. kkurrung. Battle
        //sex, numKills, numDies, gamePoint, firstConnect ������ �о��.
        char cSex = 0;
        int nGamePointTemp = 0;
        VERIFY_QUERY(m_kODBC
            >> cSex
            >> kVerifyPacket.m_dwUserUID
            >> nGamePointTemp
            >> nFirstConnect
            >> kVerifyPacket.m_tFirstLoginTime
            >> kVerifyPacket.m_tLastLoginTime);

        kVerifyPacket.m_bMale = (cSex == 0);
        m_kODBC.EndFetch();
    }

    if (kVerifyPacket.m_iSessionInfo != KUser::SI_NEW_ACCOUNT) { // �ű������� ���� �ǵ帮�� �ȵȴ�.
        switch (nFirstConnect) {
        case 0:     kVerifyPacket.m_iSessionInfo = KUser::SI_TODAY_LOGINED_BEFORE;    break;
        case 2:     kVerifyPacket.m_iSessionInfo = KUser::SI_NEW_ACCOUNT;             break;
        case 1:
            if (kVerifyPacket.m_mapCharacterInfo.find(GC_CHAR_LAS) != kVerifyPacket.m_mapCharacterInfo.end())  // �󽺸� ������ ����
                kVerifyPacket.m_iSessionInfo = KUser::SI_TODAY_FIRST_LOGIN_LAS;
            else
                kVerifyPacket.m_iSessionInfo = KUser::SI_TODAY_FIRST_LOGIN_NO_LAS;
            break;
        default:
            break;
        }
    }

    kVerifyPacket.m_cLastPlayCharacter = GetLastPlayCharacter(kVerifyPacket.m_dwUserUID); //���������� �÷����� ĳ����

    if (kVerifyPacket.m_cLastPlayCharacter != -1) {
        m_kHelper.LowLevelCharEquip(kVerifyPacket.m_dwUserUID);

        GetModeLevel(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapDifficulty);
        if (nFirstConnect != 2 && kVerifyPacket.m_mapDifficulty.empty()) {
            GetOldModeLevel(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapDifficulty);
            kPacket.m_bReadOldModwLevel = true;
        }
        // ���� Ŭ���� ���� �޾ƿ���
        LIF(m_kHelper.UserGameClear2_select_20111213(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapDifficulty));
        // ���� ��Ʈ�� ���ڵ� ���� �޾ƿ��� (��Ʈ�� �ǵ��)
        GetUserControlRecordData(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapDifficulty);
    }

    int nUserCurrentIndexCnt = -1;
    GetUserSlotCnt(kVerifyPacket.m_dwUserUID, nUserCurrentIndexCnt);
    kVerifyPacket.m_dwUserRemainIndexCnt = static_cast<DWORD>(nUserCurrentIndexCnt);

    // ���� ����....
    kVerifyPacket.m_cAuthLevel = 0;
    LIF(m_kHelper.GetCommonSP().User_auth_select(kVerifyPacket.m_dwUserUID, kBadUserInfo.m_cLastGrade, kBadUserInfo.m_cCurrentGrade, kBadUserInfo.m_nPeriodType, kBadUserInfo.m_tmLimit, kBadUserInfo.m_cCause));
    kVerifyPacket.m_cAuthLevel = kBadUserInfo.m_cCurrentGrade;

    if (kVerifyPacket.m_cAuthLevel == KUser::AL_BLOCK) { // ���� ������ ��쿡 ���� ó��.
        kVerifyPacket.m_ucOK = 5;
        goto verify_end;
    }

    // Auth Tick Count üũ �մϴ�.
    // �Ϲ� �����̰�, �����̰�, Auth Tick Check Enable, ���� Ÿ���� ù ���� ������ ��쿡 ���ؼ��� üũ.
    if (kVerifyPacket.m_cAuthLevel != KUser::AL_ADMIN &&
        !SiKGSSimLayer()->CheckTypeFlag(ST_INTERNAL_TESOP) &&
        SiKGSSimLayer()->IsCheckTickCount() &&
        kPacket_.m_kPacket.m_nConnectType == KEVENT_VERIFY_ACCOUNT_REQ::ECT_FIRST_CONNECTION) {
        DBJIF(m_kODBC.Query(L"{ call dbo.gup_user_auth_value_get ( N'%s' ) }", wstrLogin.c_str()));
        if (m_kODBC.BeginFetch())
        {
            DWORD dwAuthTick;
            int* pnAuthTick = (int*)(&dwAuthTick);

            VERIFY_QUERY(m_kODBC >> (*pnAuthTick));
            m_kODBC.EndFetch();
            if (kPacket_.m_kPacket.m_dwAuthTick != dwAuthTick)
            {
                START_LOG(cerr, L"���� Tick count �� DB�� �ٸ���.. Name : " << wstrLogin)
                    << BUILD_LOG(kPacket_.m_kPacket.m_dwAuthTick)
                    << BUILD_LOG(dwAuthTick) << END_LOG;
                kVerifyPacket.m_ucOK = 13;
                goto verify_end;
            }
        }
    }

    // ���� ���� ���� �̰�, ��� Ȥ�� ��ȸ �����ڸ� ���� ����.
    if (SiKGSSimLayer()->CheckTypeFlag(ST_ENTER_LIMIT) &&
        (kVerifyPacket.m_cAuthLevel < 1 || kVerifyPacket.m_cAuthLevel > 3)) // ����� 1, 2, 3 �� �����Ҽ� ����.
    {
        kVerifyPacket.m_ucOK = 14;
        START_LOG(cwarn, L" �ش� ������ �� ������ ���� �ϽǼ� �����ϴ�. Name : " << wstrLogin) << END_LOG;
        goto verify_end;
    }

    // �̱� ��ȸ���� ���� �ð� ����
    if (SiKGSSimLayer()->CheckTypeFlag(ST_CREATE_LIMIT))
    {
        if (SiKGCHelper()->IsTournamentCheck() == false)
        {
            kVerifyPacket.m_ucOK = 15;
            START_LOG(cwarn, L" ��ȸ���� ���� �ð��� �ƴմϴ�.. Name : " << wstrLogin) << END_LOG;
            goto verify_end;
        }
    }

    // ������ ��ȸ�����̰�, ���� ������ ����.
    if (SiKGSSimLayer()->CheckTypeFlag(ST_TOURNAMENT)) {
        if (SiKGCHelper()->IsTournamentCheck() == false) {
            kVerifyPacket.m_ucOK = 15;
            START_LOG(cwarn, L" ��ȸ���� ���� �ð��� �ƴմϴ�.. Name : " << wstrLogin) << END_LOG;
            goto verify_end;
        }
    }

    // Ŭ���̾�Ʈ üũ�� üũ.
    if (!CheckClientChecksum(kPacket_))
    {
        int nType = KPreHackingCheckManager::HT_CHECKSUM;
        ON_EVENT_INSERT_HACK_INFO_NOT(wstrLogin, kVerifyPacket.m_dwUserUID, 0, nType);
        kVerifyPacket.m_ucOK = 10;
        goto verify_end;
    }

    // indigo - ������ �⺻ ���������� ������ �� �� ó���Ǿ�� ��. (���Ǻ� overwrite)
    if (SiKGSSimLayer()->CheckTypeFlag(ST_INDIGO))
    {
        DBJIF(m_kODBC.Query(L"{ call dbo.up_indigo_query_by_uid ( %d ) }",
            kVerifyPacket.m_dwUserUID));

        char cOK = -99;

        if (m_kODBC.BeginFetch())
        {
            VERIFY_QUERY(m_kODBC >> cOK
                >> kVerifyPacket.m_iRP
                >> kVerifyPacket.m_iIndigoWin       // ���� ������ ������ ���� ���������� ���´�.
                >> kVerifyPacket.m_iIndigoLose
                >> kVerifyPacket.m_iRPRank);

            m_kODBC.EndFetch(); // BeginFetch()�� false�� ��ȯ�ϴ��� EndFetch�ؾ���.
        }

        if (cOK != 0)
            START_LOG(clog, L"No Indigo data yet. Name : " << wstrLogin) << END_LOG;
    }

    // �г��� �̱�.
/*
{ call dbo.up_query_user_nickname ( N'%s' ) }

{ call dbo.up_query_user_nickname ( @1 ) }
@1 ; Login nvarchar(20)

1 return ( @1 )
@1 ; Nickname nvarchar(24)
*/
//DBJIF( m_kODBC.Query( L"{ call dbo.up_query_user_nickname ( N'%s' ) }", wstrLogin.c_str() ) );
    DBJIF(m_kODBC.Query(L"{ call dbo.up_query_user_nickname_color ( N'%s' ) }", wstrLogin.c_str()));
    if (m_kODBC.BeginFetch())
    {
        //VERIFY_QUERY( m_kODBC >> kVerifyPacket.m_strNickName );
        VERIFY_QUERY(m_kODBC >> kVerifyPacket.m_strNickName >> kVerifyPacket.m_strNickColor);
        m_kODBC.EndFetch();
    }

    if (kVerifyPacket.m_strNickColor == L"")
        kVerifyPacket.m_strNickColor = L"FFFFFF";

    DBJIF( m_kODBC.Query( L"{ call dbo.LUMA_Get_Font_List ( %d ) }", kVerifyPacket.m_dwUserUID ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMID tempItem;
        VERIFY_QUERY( m_kODBC >> tempItem );
        kVerifyPacket.m_vecFontVector.push_back( tempItem );
    }

    /*DBJIF(m_kODBC.Query(L"{ call dbo.LUMA_PvE_Exp_Get ( %d ) }", kVerifyPacket.m_dwUserUID));
    if (m_kODBC.BeginFetch())
    {
        VERIFY_QUERY(m_kODBC >> kVerifyPacket.m_iPvExp);
        m_kODBC.EndFetch();
    }*/

    if (SiKGCHelper()->IsEventNewCharTerm()) {

        int nGetNewChar = GetTodayEventCount(kVerifyPacket.m_dwUserUID, UET_GET_NEW_CHAR, 0);
        if (nGetNewChar == 0) {
            kVerifyPacket.m_nGetNewCharID = SiKGCHelper()->GetEventNewCharID();
            SetTodayEventDone(kVerifyPacket.m_dwUserUID, UET_GET_NEW_CHAR, 0, 1);
        }
        else {
            kVerifyPacket.m_nGetNewCharID = -1;
        }
    }

    int nSeasonYear = 0;
    int nSeasonVersion = 0;
    SiKPVPMatchManager()->GetSeasonInfo(nSeasonYear, nSeasonVersion);
    // ĳ���� ���� ��������, ��ĳ���� �־��ִ� ����� sp ���� ȣ���. 
    Character_gameinfo_select(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo);
    kVerifyPacket.m_dwUserRemainIndexCnt = kVerifyPacket.m_dwUserRemainIndexCnt - kVerifyPacket.m_mapCharacterInfo.size(); // ���� ������ ������ ����.
    // ������ ��ü ĳ���� �г��� �������� �κ�
    //GetCharNickName( kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo );
    //GetLastPlayCharNickName( kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapCharacterInfo, kVerifyPacket.m_strCharNickName );
    Character_ELOData_select(kVerifyPacket.m_dwUserUID, nSeasonYear, nSeasonVersion, kVerifyPacket.m_mapCharacterInfo);

    // ���� ĳ���� ���� ��Ŷ �����ϱ�
    GetSealedCharInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_kSealedCharInfoNot.m_mapSealedCharInfo);
    // ���� ĳ���� ���� ����(�̹� ���� ������ �Ϸ��� ĳ���� ���� ó��)
    UpdateSealedCharInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo, kVerifyPacket.m_kSealedCharInfoNot.m_mapSealedCharInfo);

    //050915. microcat. �ӽ÷� ����ϴ� �α�
    if (kVerifyPacket.m_mapCharacterInfo.size() > GC_CHAR_NUM)
    {
        START_LOG(cerr, L"ĳ���� ���� ��Ȯ���� �����Ƿ� ���")
            << BUILD_LOG(kVerifyPacket.m_mapCharacterInfo.size())
            << BUILD_LOG(KncUtil::toWideString(kPacket_.m_kPacket.m_strLogin))
            << BUILD_LOG(kVerifyPacket.m_strNickName)
            << BUILD_LOG(kVerifyPacket.m_strNickColor)
            << BUILD_LOG(kVerifyPacket.m_bMale)
            << END_LOG;
    }

    // ��ȭ ���̱׷��̼� ����.
    if (kVerifyPacket.m_cLastPlayCharacter != -1 && SiKStrengthManager()->GetStrengthMigrationEnable()) {
        // ���� ��ȭ ���̱׷��̼� ���� üũ.
        time_t tmDate = KncUtil::TimeToInt(CTime(2011, 8, 18, 0, 0, 0)); // ������ �ش� �ð� ���.
        int nData = GetTodayEventCount(kVerifyPacket.m_dwUserUID, UET_STRENGTH_MIGRATION, tmDate);
        if (nData == 0) {
            if (kVerifyPacket.m_iSessionInfo != KUser::SI_NEW_ACCOUNT) {
                ItemStrengthMigration(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter);
            }
            LIF(SetTodayEventDoneDiff(kVerifyPacket.m_dwUserUID, UET_STRENGTH_MIGRATION, tmDate, 1));
        }
        else {
            START_LOG(clog, L"��ȭ ���̱׷��̼� �̹� �Ͽ���. Login " << kVerifyPacket.m_strLogin) << END_LOG;
        }
    }

    // ���� ī�� ���̱׷��̼� �۾�.
    if (SiKGSSimLayer()->IsMonsterCardMigration()) {
        // ���� ��ȭ ���̱׷��̼� ���� üũ.
        time_t tmDate = KncUtil::TimeToInt(CTime(2013, 7, 18, 0, 0, 0)); // ������ �ش� �ð� ���.
        int nData = GetTodayEventCount(kVerifyPacket.m_dwUserUID, UET_MONSTERCARD_MIGRATION, tmDate);
        if (nData == 0) {
            if (kVerifyPacket.m_iSessionInfo != KUser::SI_NEW_ACCOUNT) {
                MonsterCardMigration(kVerifyPacket.m_dwUserUID);
            }
            LIF(SetTodayEventDoneDiff(kVerifyPacket.m_dwUserUID, UET_MONSTERCARD_MIGRATION, tmDate, 1));
        }
        else {
            START_LOG(clog, L"���� ī�� ���̱׷��̼� �̹� �Ͽ���. Login " << kVerifyPacket.m_strLogin) << END_LOG;
        }
    }

    // ��� ������ ����
    WaitingItemMergeDB(kVerifyPacket.m_dwUserUID);

    // ��Ÿ �޼���, ��ǥ ������ ���.
    GetSantaComment(kVerifyPacket.m_dwUserUID, -1, kGiftEvent.m_vecGiftItemList, kPacket_.m_kPacket.m_nLanguageCode);
    if (SiKPostConfig()->IsPostEnable() && SiKPostConfig()->IsSantaPostEnable()) {
        ApplySantaPost(kVerifyPacket.m_dwUserUID, -1, kGiftEvent.m_vecGiftItemList);
    }
    else {
        ApplySanta(kVerifyPacket.m_dwUserUID, -1, kGiftEvent.m_vecGiftItemList);
    }
    kVerifyPacket.m_cUserBenfitType = 0; // �ʱ�ȭ �Լ� �߰�.
    // PC�� �������� ���� ó�� ( �±� ���� �ڵ� �߰� �Ǹ� �Ʒ� �ּ� Ǯ�� �˴ϴ�. )
    if (kPacket_.m_kPacket.m_dwAuthType == KGSSimLayer::AT_FUNBOX_TH) {
        if (kPacket_.m_kPacket.m_nFunBoxBonus == 1) {
            kVerifyPacket.m_cPCBangType = KIPAdvantage::PCBANG_PUBLISHER;
        }
        kVerifyPacket.m_cUserBenfitType = CheckTH3BBUser(kPacket_.m_kPacket.m_strGetIP, KIPAdvantage::TH3BB_USER);
        START_LOG(clog, L"CheckTH3BBUser")
            << BUILD_LOG(kPacket_.m_kPacket.m_strGetIP)
            << BUILD_LOGc(kVerifyPacket.m_cUserBenfitType) << END_LOG;
    }
    else if (kPacket_.m_kPacket.m_dwAuthType == KGSSimLayer::AT_LIN) {
        char cBenefitType = 0;
        if (SiKIPAdvantage()->GetLINBenefitTestID(kVerifyPacket.m_strLogin, cBenefitType)) {
            // �׽�Ʈ�� �ڵ�. LoginID�� ������ ���� PC�� ������ �����Ѵ�
            kVerifyPacket.m_cUserBenfitType = cBenefitType;
            START_LOG(clog, L"���� PC�� ���� ����(�׽�Ʈ��)")
                << BUILD_LOG(wstrLogin)
                << BUILD_LOG(kPacket_.m_kPacket.m_nLinBonus)
                << BUILD_LOG(kPacket_.m_kPacket.m_strGetIP) << END_LOG;
        }
        else {
            switch (kPacket_.m_kPacket.m_nLinBonus) {
            case 101:
                kVerifyPacket.m_cUserBenfitType = KIPAdvantage::LIN_BENEFIT_TYPE1;
                break;
            case 102:
                kVerifyPacket.m_cUserBenfitType = KIPAdvantage::LIN_BENEFIT_TYPE2;
                break;
            case 103:
                kVerifyPacket.m_cUserBenfitType = KIPAdvantage::LIN_BENEFIT_TYPE3;
                break;
            case 104:
                kVerifyPacket.m_cUserBenfitType = KIPAdvantage::LIN_BENEFIT_TYPE4;
                break;
            default:
                kVerifyPacket.m_cUserBenfitType = KIPAdvantage::PCBANG_NONE;
                break;
            }
            START_LOG(clog, L"���� PC�� ���� ����")
                << BUILD_LOG(kPacket_.m_kPacket.m_nLinBonus)
                << BUILD_LOG(kPacket_.m_kPacket.m_strGetIP) << END_LOG;
        }
    }
    else {
        kVerifyPacket.m_cPCBangType = CheckPCBangUser(kPacket_.m_kPacket.m_strGetIP);
    }

    if (kVerifyPacket.m_cLastPlayCharacter != -1) { // ĳ���� ������ ���� ��쿡 ���ؼ��� ó��.
        if (SiKIPAdvantage()->IsPCBangUser(kVerifyPacket.m_cPCBangType))
        {
            // DB�� �ִ� �������� ������ �������� ���� ����. ���߿� ������ ��ó�� ������.
            InsertIPAdvantageDailyItem(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, std::vector<KItem>()); // �̾ȿ��� ���� �޾Ҵ��� ���� üũ��..
        }

        AddInvenExternalItems(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter); // shared DB ���� ���� Item�� DB �� �ִ´�.. ����.
        m_kHelper.GetGiftInfoList(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, vecGiftInfo);
        m_kHelper.InsertFullWaitItemToInven(kVerifyPacket.m_dwUserUID, KItem::EICT_AGIT, std::vector<KItem>());
        m_kHelper.InsertFullWaitItemToInven(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, std::vector<KItem>());

        // �κ��丮 �о����
        GetItemListFromDB(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kInvenPacket.m_vecInv, vecDesignCoordi);

        // �� ������ ���� ���� �ͼ� ����.
        AdjustLookItemInfo(kVerifyPacket.m_dwUserUID, kInvenPacket.m_vecInv, kVerifyPacket.m_cLastPlayCharacter);

        // ������Ʈ ������ ���� ����
        AdjustObjectItemInfo(kVerifyPacket.m_dwUserUID, kInvenPacket.m_vecInv);

        // ���⼭ �Ǿ��� ��¥ �������� �־� �ش�.
        if (SiKIPAdvantage()->IsPCBangUser(kVerifyPacket.m_cPCBangType))
        {
            std::map<char, KCharacterInfo>::iterator mit;
            for (mit = kVerifyPacket.m_mapCharacterInfo.begin(); mit != kVerifyPacket.m_mapCharacterInfo.end(); ++mit) {
                if (mit->second.m_cCharType == kVerifyPacket.m_cLastPlayCharacter) { // �������� �÷����� ĳ���� ������ �����´�.
                    SiKIPAdvantage()->GetConnectionItem(mit->second.m_cCharType, (int)mit->second.m_dwLevel, kInvenPacket.m_vecInv, KIPAdvantage::PCBANG_PUBLISHER);
                }
            }
        }

        // ���⼭ ���� ������ ��¥ �������� �־��ش�.
        if (SiKIPAdvantage()->IsTH3BBUser(kVerifyPacket.m_cUserBenfitType)) {
            std::map<char, KCharacterInfo>::iterator mit;
            for (mit = kVerifyPacket.m_mapCharacterInfo.begin(); mit != kVerifyPacket.m_mapCharacterInfo.end(); ++mit) {
                if (mit->second.m_cCharType == kVerifyPacket.m_cLastPlayCharacter) { // �������� �÷����� ĳ���������� �����´�.
                    SiKIPAdvantage()->GetConnectionItem(mit->second.m_cCharType, (int)mit->second.m_dwLevel, kInvenPacket.m_vecInv, KIPAdvantage::TH3BB_USER);
                }
            }
        }

        // ĳ���� ���� ���� ���� ����.
        LIF(GetUserEquipInfo(kVerifyPacket.m_dwUserUID, kPacket.m_kInvenPacket.m_vecInv, kVerifyPacket.m_mapCharacterInfo, vecDesignCoordi));
        // ĳ���� �ڵ� ���� ���� ��������.
        m_kHelper.SetUIGACharItemCoordiEquip(kVerifyPacket.m_dwUserUID, kPacket.m_kInvenPacket.m_vecInv, kVerifyPacket.m_mapCharacterInfo, vecDesignCoordi);
        // ����ü���� ���� ��������
        InitChangeWeapon(kVerifyPacket.m_mapCharacterInfo, kPacket.m_kChangeWeapon);
        LIF(m_kHelper.GetChangeWeaponInfoFromDB(kVerifyPacket.m_dwUserUID, kPacket.m_kChangeWeapon));
        SyncChangeWeapon(kPacket.m_kInvenPacket.m_vecInv, kPacket.m_kChangeWeapon, kVerifyPacket.m_mapCharacterInfo);

        // �� ĳ���Ͱ� �����ϰ� �ִ� ���⸦ Ȯ���ؼ� ���� ���õ� ���� ���¸� ���� �Ѵ�.
        {
            std::map<char, KCharacterInfo>::iterator mit;
            for (mit = kVerifyPacket.m_mapCharacterInfo.begin(); mit != kVerifyPacket.m_mapCharacterInfo.end(); ++mit)
            {
                mit->second.m_cCurrentPromotion = 0;
                DWORD dwRet = SiKGameServer()->ChkValidWeapon(mit->second.m_vecEquipItems, mit->second.m_cCharType,
                    mit->second.m_cPromotion, mit->second.m_cCurrentPromotion);
                LOG_SUCCESS(dwRet == NetError::NET_OK)
                    << BUILD_LOGc(mit->second.m_cCharType)
                    << BUILD_LOG(kVerifyPacket.m_strLogin)
                    << BUILD_LOG(NetError::GetErrStr(dwRet)) << END_LOG;
            }
        }
    }

    // 0:��õ�� �̵��, 1:��õ�� �������, 2:��õ�� ����, 3:��õ�� �ý��۾���, 4: ���̿�
    kVerifyPacket.m_cRecommendUser = 3; //��õ��

    if (SiKGSSimLayer()->m_bRecommendEnable)
    {
        DBJIF(m_kODBC.Query(L"{ call dbo.up_recommender_check_status ( %d ) }",
            kVerifyPacket.m_dwUserUID));
        if (m_kODBC.BeginFetch())
        {
            int nRecomend = -10;

            VERIFY_QUERY(m_kODBC >> nRecomend);

            switch (nRecomend)
            {
            case 0: //�̹� ��õ����
                kVerifyPacket.m_cRecommendUser = 1;
                break;
            case -1: // ��õ�� �̵��
                kVerifyPacket.m_cRecommendUser = 0;
                break;
            case -2: // ��õ�� ����
                kVerifyPacket.m_cRecommendUser = 2;
                break;
            default:
                kVerifyPacket.m_cRecommendUser = 4;
            }//end switch
        }
        m_kODBC.EndFetch();
    }

    // ��� ���� �޾ƿ���
    GetUserGuildInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_kGuildUserInfo);

    if (SiKGSSimLayer()->CheckTypeFlag(ST_GUILD) && kVerifyPacket.m_kGuildUserInfo.m_dwGuildUID == 0) {
        kVerifyPacket.m_ucOK = 16;
        START_LOG(cwarn, L" ��� �Ҽ� ������ �ƴմϴ�. Name : " << wstrLogin) << END_LOG;
        goto verify_end;
    }

    if (SiKGSSimLayer()->CheckTypeFlag(ST_GUILD)) { // ���� ��常 ����� ���� ���� ����.
        KGuildPtr spGuild = SiKGuildManager()->GetGuild(kVerifyPacket.m_kGuildUserInfo.m_dwGuildUID);

        UCHAR guildGrade = spGuild->GetGuildGrade();

        if (guildGrade != KNGuildInfo::GG_OFFICIAL) {
            kVerifyPacket.m_ucOK = 17;
            START_LOG(cwarn, L"���� ��尡 �ƴմϴ�. Name : " << wstrLogin) << END_LOG;
            goto verify_end;
        }
    }

    //����� ���� ����/��� ����
    /*
    { call dbo.up_get_exist_user_in_agreement ( N'%s' ) }

    { call dbo.up_get_exist_user_in_agreement ( @1 ) }
    @1 ; Login nvarchar(20)

    1 return ( @1 )
    @1 ; OK int

    OK
    0 : ����
    -1 : �̵���
    */
    DBJIF(m_kODBC.Query(L"{ call dbo.up_get_exist_user_in_agreement ( N'%s' ) }",
        wstrLogin.c_str()));
    kVerifyPacket.m_bAgreePrivateInfo = false;
    if (m_kODBC.BeginFetch())
    {
        int iAgreePrivateInfo = -1;
        VERIFY_QUERY(m_kODBC >> iAgreePrivateInfo);

        switch (iAgreePrivateInfo)
        {
        case -1:
            kVerifyPacket.m_bAgreePrivateInfo = false;
            break;
        case 0:
            kVerifyPacket.m_bAgreePrivateInfo = true;
            break;
        }

        m_kODBC.EndFetch();
    }

    if (kVerifyPacket.m_cLastPlayCharacter != -1) {
        // DB���� ���̱� ���� �̼� �������� ���ν��� ����.
        GetMainMissionInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_vecMissionSlot); // �̼� ���� ��������.
        GetSubMissionInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_vecMissionSlot);

        GetMainCollectionInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kCollectionMission.m_vecCollection); // Īȣ ���� ��������.
        GetSubCollectionInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kCollectionMission.m_vecCollection);

        // �� �⺻ ���� ��������.
        GetPetDefaultInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapPetInfo);

        GetPetExp(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapPetInfo); // �� ������ ����ġ ��������

        // �� ���� ���� ����.
        GetPetInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, kInvenPacket.m_vecInv, kVerifyPacket.m_mapPetInfo);
        // �� �ڽ�Ƭ ���� ����..
        GetPetCostume(kVerifyPacket.m_strLogin, kVerifyPacket.m_mapPetInfo, kInvenPacket.m_vecInv);

        // ĳ���Ͱ� ������ �� ���� ����.
        GetCharacterPetEquip(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo);
        GetCharEquipPetCostume(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo);

        // ���⼭ PC�� �� �������� �־��ش�.
        // 1. User�� Pet������ ��� �ִ� std::map<DWORD, PetInfo>�� ������ �־��ش�.
        // 2. User�� Inven�� �� ������, �� ���� ������, �� �ڽ�Ƭ ������ �־��ش�.
        if (SiKIPAdvantage()->IsPCBangUser(kVerifyPacket.m_cPCBangType)) {
            SiKIPAdvantage()->GetPCBangPetItem(kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapPetInfo, kInvenPacket.m_vecInv, KIPAdvantage::PCBANG_PUBLISHER);
        }

        // �±� 3BB ���� �̺�Ʈ.
        if (SiKIPAdvantage()->IsTH3BBUser(kVerifyPacket.m_cUserBenfitType)) {
            SiKIPAdvantage()->GetPCBangPetItem(kVerifyPacket.m_cLastPlayCharacter, kVerifyPacket.m_mapPetInfo, kInvenPacket.m_vecInv, KIPAdvantage::TH3BB_USER);
        }

        std::map<char, KCharacterInfo>::iterator mit;
        for (mit = kVerifyPacket.m_mapCharacterInfo.begin(); mit != kVerifyPacket.m_mapCharacterInfo.end(); ++mit)
        {
            GetPetGlyph(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_cLastPlayCharacter, mit->second.m_vecPetGlyphCharData);
            mit->second.m_dwPetGlyphSize = mit->second.m_vecPetGlyphCharData.size();
        }
    }

    if (SiKGSSimLayer()->m_bTWUserAgreement) // �븸 ����ȭ üũ ON�̸�.
    {
        DBJIF(m_kODBC.Query(L"{ call dbo.GAS_agreement_system_select ( %d, %d ) }",
            kVerifyPacket.m_dwUserUID,
            TWUSERAGREEMENT));

        kPacket.m_nUserAgreement = -1;
        int iUserUID = -1;
        if (m_kODBC.BeginFetch())
        {
            VERIFY_QUERY(m_kODBC >> iUserUID);
            m_kODBC.EndFetch();
        }

        if (kVerifyPacket.m_dwUserUID == iUserUID) // ���� ������ ������ ��ġ�ϴ� �ڽ��� UID�� ��ȯ
            kPacket.m_nUserAgreement = TWUSERAGREEMENT;  // �ش� ����� ������ ������.
    }

    if (SiKMissionEvent()->IsMissionEventTerm())
    {
        DBJIF(m_kODBC.Query(L"{ call dbo.DQGADailyQuest_select_log ( %d ) }",
            kVerifyPacket.m_dwUserUID));

        kPacket.m_nMissionEventCheck = -1;
        int nMissionEventCheck = -1;
        if (m_kODBC.BeginFetch())
        {
            VERIFY_QUERY(m_kODBC >> nMissionEventCheck);
        }
        m_kODBC.EndFetch();
        kPacket.m_nMissionEventCheck = nMissionEventCheck;
    }

    // �̺�Ʈ ���� ��� ���� Ȯ��
    if (SiKGCHelper()->CheckEventEnable())
    {
        CheckEventInfoEnable(kVerifyPacket.m_dwUserUID, kPacket.m_setStringIDList);
    }

    if (kVerifyPacket.m_cLastPlayCharacter != -1) {
        SiKSkillManager()->GetOldSkillTreeUseCharacter(setOldSkillTreechar);

        GetTrainedSkill(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo, kFullSPInfo, setOldSkillTreechar);
        GetSkillSet(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo, kFullSPInfo, setOldSkillTreechar); // �ʱ� ������ ä���
        // GetEquipSkillSet : ��ų ���������� �������� �ʾƵ� ��.
        GetEquipSkillSet(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_mapCharacterInfo, setOldSkillTreechar);
        GetUnLockGroupID(kVerifyPacket.m_dwUserUID, kFullSPInfo.m_mapUnLockGroupID, setOldSkillTreechar);
        GetSkillSlotOpenInfo(kVerifyPacket.m_dwUserUID, kFullSPInfo.m_mapSlotOpenInfo);

        SiKGachaManager()->GetGachaVersions(vecVersions);
        GetGachaSetObtained(vecVersions, kVerifyPacket.m_dwUserUID, kPacket.m_mapGachaSetObtained);
        GetGachaSetObtained(vecVersions, kVerifyPacket.m_dwUserUID, kPacket.m_mapGachaSetObtainedLevel);
    }

    // 09.04.12. kkurrung. ��õ (5th)
    // �ű� ���� ���� �̸� ó�� ���ش�.
    kRecomInfo.Clear();
    if (!kPacket_.m_kPacket.m_bExpAccount) // ü������̸�..�ȵ�..
        InitialiseRecomInfo(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_iSessionInfo, kVerifyPacket.m_tFirstLoginTime, kVerifyPacket.m_tLastLoginTime, kRecomInfo);

    kVerifyPacket.m_bIsRecommendEvent = SiKRecomHelper()->IsLimitEventTerm();
    kVerifyPacket.m_ucOK = 0;

    // ���� ���� üũ.
    bool bReturnUserCheck = false;
    if (SiKRecomHelper()->IsEventTerm()) {
        if (SiKRecomHelper()->IsReturnUser(kVerifyPacket.m_tLastLoginTime)) {
            bReturnUserCheck = true;
            InsertReturnUserInfo(UET_RECOM_HELPER_EVENT, SiKRecomHelper()->GetVersion(), kVerifyPacket.m_dwUserUID);
        }
        else {
            bReturnUserCheck = CheckReturnUser(UET_RECOM_HELPER_EVENT, SiKRecomHelper()->GetVersion(), kVerifyPacket.m_dwUserUID);
        }
    }

    // 09.12.07. tgkwon. ��õ (5th) �ű�/�������� Ư������ �޼��� ������ ����üũ.
    if (SiKRecomHelper()->IsEventTerm()) {
        if (SiKRecomHelper()->IsNewBeginTerm(kVerifyPacket.m_tFirstLoginTime) || bReturnUserCheck) {
            std::pair<int, int> prRecomRewardInfo;
            if (CheckRecomUserLevelup(kVerifyPacket.m_dwUserUID, kRecomInfo.m_dwLoginUID, prRecomRewardInfo, SiKRecomHelper()->GetVersion())) {
                kPacket.m_prRecomLevelUpCheck = prRecomRewardInfo;
            }
        }
    }
    kPacket.m_bReturnUserCheck = bReturnUserCheck;

    // ä�θ� �̺�Ʈ ( �ӽ� ��� pet ����. )
    // 1. User�� Pet������ ��� �ִ� std::map<DWORD, PetInfo>�� ������ �־��ش�.
    // 2. User�� Inven�� �� ������, �� ���� ������, �� �ڽ�Ƭ ������ �־��ش�.
    if (SiKChannelTypeEvent()->IsEventTerm()) {
        SiKChannelTypeEvent()->GetChannelPetItem(kVerifyPacket.m_dwChannelType, kVerifyPacket.m_mapPetInfo, kInvenPacket.m_vecInv);
    }

    // ���� �̱� ���� ���� ĳ�� ��� ������ ����.
    if (SiKGCHelper()->IsUserPurchaseEventTerm()) {
        GetVirtualCashLog(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_tVirtualEnableDate);
        if (!SiKGCHelper()->IsUserVirtualPurchaseTerm(kVerifyPacket.m_tVirtualEnableDate)) {
            time_t tmDate;
            SiKGCHelper()->GetBeginTime(tmDate);
            int nData = GetTodayEventCount(kVerifyPacket.m_dwUserUID, UDT_USERAUTH_VIRTUALCASH_EVENT, tmDate);
            if (0 == nData) {
                // ���� ĳ�� �ʱ�ȭ.
                int nCurrentVCash = m_kHelper.GetCurrentVirtualCash(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_strLogin);
                m_kHelper.VirtualCashMergeWithType(kVerifyPacket.m_dwUserUID, 0, KVirtualCash::EVCMT_SUB_LA_US_EXPIRED_AUTH_CHECK, 0, nCurrentVCash);
                LIF(SetTodayEventDoneDiff(kVerifyPacket.m_dwUserUID, UDT_USERAUTH_VIRTUALCASH_EVENT, tmDate, 1));
            }
            else {
                START_LOG(clog, L"���� ĳ�� �ʱ�ȭ �̹� �Ͽ���. Login " << kVerifyPacket.m_strLogin) << END_LOG;
            }
        }
    }

    if (SiKSurvey()->IsRun()) { // �̺�Ʈ �Ⱓ�϶��� ��������.
        int nSurveyVersion = SiKSurvey()->GetSurveyVersion();
        GetUserSurveyInfo(nSurveyVersion, kVerifyPacket.m_dwUserUID, kPacket.m_mapUserSurveyInfo);
    }

    if (SiKGCHelper()->IsCharLevelEvent()) {
        GetUserCharLevelEventData(kVerifyPacket.m_dwUserUID, SiKGCHelper()->GetCharLevelEventVersion(), kPacket.m_setCharLevelEvent);
    }
    // 120623. nodefeat. 
    // kVerifyPacket.m_iSessionInfo �� ù ������ �Ǵ��ϴ� �뵵�� �ƴ϶�� ��
    // ������ ���� �ð��� ��� �� �Ŀ� ������(����) �α��� �ð��� ��� ��
    // ������ ���� ��¥�� ���� ��¥�� �ٸ��� ù ����
    /*
    ���� ������ ���� ��¥ ��ȸ
    L"{ call dbo.LLGALastLogin_Select ( %d ) }"

    { call dbo.LLGALastLogin_Select ( @1 ) }
    @1: @iLoginUID_  int


    1 return ( @1, @2 )
    @1: LoginUID  int
    @2: LastLogin datetime
    */
    DBJIF(m_kODBC.Query(L"{ call dbo.LLGALastLogin_Select ( %d ) }",
        kVerifyPacket.m_dwUserUID));

    kVerifyPacket.m_tFinalLoginTime = tCurrentTimeStamp;
    int temp = 0; // return ������ LoginUID �ٽ� �ֱ� ����...

    if (m_kODBC.BeginFetch())
    {
        VERIFY_QUERY(m_kODBC
            >> temp
            >> kVerifyPacket.m_tFinalLoginTime);
    }
    m_kODBC.EndFetch();

    // ���� ������ DB���� �ƹ� �͵� ���� ���� ��. ���� ��¥�� �����Ѵ�.
    if (tCurrentTimeStamp == kVerifyPacket.m_tFinalLoginTime) {
        CTime tmYesterDay = CTime(tmCurrentTime.GetYear(), tmCurrentTime.GetMonth(), tmCurrentTime.GetDay(), 0, 0, 0) - CTimeSpan(1, 0, 0, 0);
        kVerifyPacket.m_tFinalLoginTime = KncUtil::TimeToInt(tmYesterDay);
    }

    // LastLoginTime�� ���� �Ŀ� �ٷ� ������ LastLogin���� ����Ѵ�.
    UpdateFinalLoginTime(kVerifyPacket.m_dwUserUID);
    GetUserTutorialEnable(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_iSessionInfo, kVerifyPacket.m_tFirstLoginTime, UET_NEWUSER_TUTORIAL, kVerifyPacket.m_bTutorialEnable);

    // ���� �������� ���� ������ ��������
    CheckConnectionGiftBoxNewUser(kVerifyPacket.m_dwUserUID, kVerifyPacket.m_tFirstLoginTime, kPacket.m_kConnectionGiftBoxUpdateData.m_bIsNewUser);
    kPacket.m_kConnectionGiftBoxUpdateData.m_bIsEventUser = SiKConnectionGiftBox()->IsGiftEventUserTerm();
    kPacket.m_kConnectionGiftBoxUpdateData.m_nEventUserVersion = SiKConnectionGiftBox()->GetGiftEventUserVersion();
    if (-1 != kVerifyPacket.m_cLastPlayCharacter) {
        Get_ConnectionGiftBox_UserData_FromDB(kVerifyPacket.m_dwUserUID, static_cast<int>(kVerifyPacket.m_cLastPlayCharacter), kPacket.m_kConnectionGiftBoxUpdateData);
    }



verify_end:
    QUEUING_ACK_TO_USER(EVENT_VERIFY_ACCOUNT_ACK);
    START_LOG(clog, L"EVENT_VERIFY_ACCOUNT_REQ sp ó�� ��� : ")
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_REGISTER_NICKNAME_REQ, std::wstring )
{
    KPacketNameOK kPacket;

    KLocker lock( m_kODBC.GetCS() );

    kPacket.m_nOK = -9;
    kPacket.m_strName.clear();

    if ( !m_kODBC.Query( L"{ call dbo.UNGA_user_nickname_insert ( %d, N'%s' ) }", 
        dwUID_, 
        kPacket_.c_str() ) ) 
    {
        START_LOG( cerr, L"UNGA_user_nickname_insert 프로시저 실패" )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( (wchar_t*)m_kODBC.m_szDiagMsg )
            << BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState )
            << END_LOG;
        //return;
    }
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC
            >> kPacket.m_nOK );
        m_kODBC.EndFetch();

        _LOG_SUCCESS( kPacket.m_nOK == 0, L"NickName Insert.. Ret : " << kPacket.m_nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

        if( kPacket.m_nOK == 0 )
        {
            kPacket.m_strName = kPacket_;
        }
    }

    QUEUING_ACK_TO_USER( EVENT_REGISTER_NICKNAME_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_USER_DATA_REQ, KUserData )
{
    START_LOG( clog, strLogin_ ) << END_LOG;
    UpdateUserData( kPacket_ );
}

INT_IMPL_ON_FUNC( DB_EVENT_EXIT_GAME_NOT )
{
    //050711. microcat. 유저가 종료했으니 아이템 빼내야지 ㅎㅎ;
    //일반 아이템은 저장대상이 아니므로 삭제할 필요는 없다.
    std::vector<KItem>::iterator  iter2;
    iter2 = kPacket_.m_vecDurationItemInv.begin();
    while( iter2 != kPacket_.m_vecDurationItemInv.end() )
    {
        if( iter2->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
        {
            //START_LOG( clog, L" 아이템 삭제" << iter2->m_dwGoodsID ) << END_LOG;

            iter2 = kPacket_.m_vecDurationItemInv.erase( iter2 );
            continue;
        }
        iter2++;
    }

    bool bReturn = UpdateUserData( kPacket_ );

    START_LOG( clog , L"Name : " << strLogin_ << L", ok : " << bReturn ) << END_LOG;

    if( kPacket_.m_strWantedServerName.empty() ) // 서버 이동이 아님. 게임 종료시에는 user::Ondestroy()에서 센터서버로 NOT한다.
    {
        DWORD dwAuthTick = ::GetTickCount(); // DB로 부터 TickCount를 받아 온다.
        /*
        { call dbo.gup_user_auth_value_set ( N'%s', %d ) }

        { call dbo.gup_user_auth_value_set ( @1, @2 ) }
        @1 ; Login nvarchar(20)
        @2 ; Value int

        1 return ( @1 )
        @1 ; OK int
        */
        DBJIF( m_kODBC.Query( L"{ call dbo.gup_user_auth_value_set ( N'%s', %d ) }",
            strLogin_.c_str(), (int)(dwAuthTick) ) );
        if( m_kODBC.BeginFetch() )
        {
            int nOK;
            VERIFY_QUERY( m_kODBC >> nOK );
            LOG_SUCCESS( nOK == 0 )
                << BUILD_LOG( nOK )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( dwAuthTick )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << BUILD_LOG( (wchar_t*)(m_kODBC.m_szDiagMsg) )
                << BUILD_LOG( (wchar_t*)(m_kODBC.m_sqlState) ) << END_LOG;
            m_kODBC.EndFetch();
        }
        return ;
    }

    int kPacket = (bReturn ? 0 : -1 ); // 0 = 성공, -1 = 실패.

    if( !bReturn ) // 마이그레이션 이고 디비 싱크 실패일때
    {
        _QUEUING_ACK_TO_USER( EVENT_SERVER_MIGRATION_ACK, kPacket );
        return;
    }

    // 서버에서 개쉬 인증을 지원하고, 해당 유저가 개쉬 인증을 사용하면
    if( SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_GASH ) && kPacket_.m_dwAuthType == KGSSimLayer::AT_GASH )
    {
        KGASH_ACCOUNTING kNot;
        kNot.m_strLogin             = KncUtil::toNarrowString( strLogin_ );
        kNot.m_strIP                = kPacket_.m_strIP;
        kNot.m_strWantedServerName  = kPacket_.m_strWantedServerName;
        kNot.m_dwUserUID            = kPacket_.m_dwUserUID;
        KTThreadManager<KGSGash>::GetInstance()->QueueingEvent( KUserEvent::DB_EVENT_EXIT_GAME_NOT, strLogin_.c_str(), 0, 0, kNot );

        START_LOG( clog, L"LOG" )
            << BUILD_LOG( KncUtil::toWideString( kNot.m_strLogin ) )
            << BUILD_LOG( KncUtil::toWideString( kNot.m_strIP ) )
            << BUILD_LOG( kNot.m_strWantedServerName )
            << BUILD_LOG( kNot.m_dwUserUID ) << END_LOG;
        return;
    }
    _QUEUING_ACK_TO_USER( EVENT_SERVER_MIGRATION_ACK, kPacket );
}

INT_IMPL_ON_FUNC( DB_EVENT_INSERT_STATISTICS_NOT )
{
    std::vector<KCharStat>::iterator vit;
    int nUID = -99;
    int nOK = InsertUserTraceMainInfo( dwUID_, kPacket_.m_strStartDate, kPacket_.m_strEndDate, (int)kPacket_.m_cGrade, kPacket_.m_dwPlayTime, kPacket_.m_bPCBang, kPacket_.m_bGuildServer, nUID );

    if( nOK == 0 && nUID > 0 ) {
        for( vit = kPacket_.m_vecCharData.begin() ; vit != kPacket_.m_vecCharData.end() ; ++vit ) {
            int nRet = InsertUserTraceCharacterInfo( dwUID_, nUID, (int)vit->m_cCharType, vit->m_biDiffExp, vit->m_dwLevel, vit->m_dwPlayCount, vit->m_bTurtleBack, vit->m_biTotalDiffExp );
                LOG_SUCCESS( nRet == 0 )
                    << BUILD_LOG( L" 통계 기록..."  ) 
                    << BUILD_LOG( nRet )
                    << BUILD_LOG( m_kODBC.m_strLastQuery )
                    << END_LOG;
        }
    }

    if( kPacket_.m_bPCBang ) {
        //DBJIF( m_kODBC.Query(L"INSERT INTO UserPCBangCount Values(\'%s\', \'%s\', %d, \'%s\', \'%s\')",
        DBJIF( m_kODBC.Query( L"{ call dbo.sp_executesql "
            L"( N'insert into dbo.UserPCBangCount with (updlock) (Login, IP, PlayTime, ConnectTime, DisconnectTime) select @1, @2, @3, @4, @5', "
            L"N'@1 nvarchar(20), @2 nvarchar(15), @3 int, @4 smalldatetime, @5 smalldatetime', N'%s', N'%s', %d, N'%s', N'%s' ) }",
            strLogin_.c_str(),
            kPacket_.m_strIP.c_str(),
            kPacket_.m_dwPlayTime,
            kPacket_.m_strStartDate.c_str(),
            kPacket_.m_strEndDate.c_str() ) );

        START_LOG( clog, L"PC방 유저 기록 완료" )
            << BUILD_LOG( kPacket_.m_strIP )
            << BUILD_LOGc( kPacket_.m_bPCBang ) << END_LOG;
    }

    // PT/DT 통계 남기는 작업 추가.
    InsertPlayTimeStat( dwUID_, kPacket_.m_dwPlayTime, kPacket_.m_dwPTTime );

    if ( true == SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_NETMARBLE ) ) {
    m_kHelper.PTEVPlayTimeEvent( dwUID_, kPacket_.m_strStartDate, kPacket_.m_strEndDate );
    }
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_AGREEMENT_PRIVATEINFO_NOT )
{
    KLocker lock( m_kODBC.GetCS() );

    DBJIF( m_kODBC.Query( L"{ call dbo.up_insert_user_for_Agreement ( N'%s' ) }",
        strLogin_.c_str() ) );

    START_LOG( clog, L"" )
        << BUILD_LOG( strLogin_.c_str() ) << END_LOG;

    char cOK;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> cOK );

        START_LOG( clog, L"유저 정보 공개 및 사용 동의를 받아냄." )
            << BUILD_LOGc( cOK ) << END_LOG;

        m_kODBC.EndFetch();
    }

}
