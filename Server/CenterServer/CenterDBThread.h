#pragma once
#include "SubThread.h"
#include "CenterEvent.h"
#include "odbc/odbc.h"
#include "CenterPacket.h"
#include "DBLayer.h"
#include "NUserPacket.h"
#include "CommonSP.h"

class KIntEvent;
class KCenterSimLayer;
struct KDotantionRank;
struct KDailyEventData; // CnRainBow.h
struct KJackpotTimeList; // CnRainBow.h
struct KMissionDailyData; // CnMissionEvent.h
struct KItemDonationInfo; // CommonPacket.h

class KCenterDBThread : public KSubThread<KCenterEvent>
{
    typedef KCenterSimLayer SLayerType;
    DeclareDBThread;
    DeclareDBThreadName( Main );
    
public:
    KCenterDBThread();
    virtual ~KCenterDBThread(void);

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent ); 

    enum {
        SHOW_WEB                = 1, // 디비업
        //ONEADAY               = 2, // 원어데이(미사용)
        ITEM_LIMIT              = 3, // 구매제한
        SPHINX                  = 4, // 스핑크스.
        SURVEY                  = 5, // 설문 시스템
        ADVENTURE               = 6, // 모험 이벤트
        OLYMPIC                 = 7, // 올림픽 이벤트.
        ACC_ATTENDANCE          = 8, // 누적출석 이벤트.
        CASHBACK                = 10, // 가상캐시 캐시백 비율 정보 관련
        QUIZ_BINGO              = 11, // 퀴즈빙고 이벤트
        VIPEVENT                = 12, // VIP 이벤트
        SOCIAL_COMMERCE         = 13, // 소셜 커머스(보부상)
        DUNGEON_RANK            = 14, // 던전 랭킹 시즌 변경
    };

    static std::wstring GenInternalPasswd( IN const std::wstring strData, IN const size_t nReturnSize );
    void GetDonationRanking( OUT std::vector<KDotantionRank>& vecRank );
    void AddServerCountData( int nData, int nType );
    bool AddServerCountData( IN __int64 biData, IN int nType, IN time_t tmDate );
    void UpdateChannelNews();
    void GetServerList( IN const int nProtocol, OUT std::vector<KServerInfo>& vecServerList );
    void UpdateMiniGameRank();

    // 09.07.11. kkurrung. Rainbow
    int  CheckRainbowEvent( IN const bool bFirst, OUT time_t& tmBegin, OUT time_t& tmEnd );
    bool GetRainbowDailyEvent( IN const int nRainbowID, OUT std::vector<KDailyEventData>& vecDailyEvent );
    bool GetJackpotEventList( IN const int nRainbowID, OUT std::vector< KRainBowData >& vecJackpotEventList );
    bool GetJackpotTimeList( IN const int nRainbowID, OUT std::vector< KJackpotTimeList >& vecJackpotTimeList );

    // 09.07.26. kkurrung. Show Web
    bool CheckDBUpdate( IN int kind_ );
    bool GetItemBuyInfo( OUT std::vector<std::pair<DWORD,bool> >& vecItemBuyInfo, OUT std::map<DWORD,unsigned char>& mapRecommInfo );

    bool UpdateCouponInfo( IN const bool bAll, OUT std::vector<KGameCouponInfo>& vecCouponInfo );
    // 09.09.07. tgkwon. MissionEvent
    void CheckMissionEvent( OUT int& nEventID, OUT time_t& tmBegin, OUT time_t& tmEnd, OUT int& nMissionCheckHour, OUT int& nUpdated );
    bool GetMissionDailyEvent( IN const int nMissionEventID, OUT std::vector<KMissionDailyData>& vecDailyEvent );
    void UpdateMissionEvent();

    // 09.09.24 tgkwon. Item Donation
    void GetItemDonation( IN const bool bFirst, OUT std::vector<KItemDonationInfo>& vecDoInfo );

    void GetGuildTotalRank( OUT std::map< DWORD, KNGuildRankInfo >& mapGuildRank_ );

    // new guild
    bool GetAllGuildInfo( OUT std::map< DWORD, KNGuildInfo >& mapGuildList ); // 전체 길드 정보 받아오기 center
    bool GetAllGuildNotice( OUT std::map< DWORD, std::vector<KNGuildNotice> >& mapGuildNotice ); // 전체 길드 공지 center
    bool GetAllGuildMemberList( OUT std::map< DWORD, std::vector<KNGuildUserInfo> >& mapGuildMember ); // 전체 길드원 center
    bool GetAllGuildRankData( OUT std::map< DWORD, std::vector<KNGuildRankInfo> >& mapGuildRank, OUT std::map<DWORD,KSimpleGuildRank>& mapSimpleRank ); // 길드랭킹 받아오기, 일/주/월/전체 랭킹이 한번에 리턴 Center
    bool UpdateGuildInfoFromDB( OUT std::map<UCHAR,UCHAR>& mapUpdateCheck ); // 길드정보갱신 Tick단위. Center
    bool GetGuildMarkInfo( OUT std::map< DWORD, std::pair<UCHAR,UCHAR> >& mapMarkStatus ); // 길드마크 읽어가기 Center
    bool UpdateGuildMarkInfo( IN const DWORD dwGuildUID ); // 3,4일 경우 이 SP를 불러서 갱신 Center
    bool UpdateGuildInfoDB( IN const UCHAR ucUpdateType ); // DB에 랭킹정보,길드마크정보 갱신여부 알림
    DWORD GetGuildUIDFromDB( IN const DWORD dwUserUID ); // 유저의 길드ID를 DB에서 받아옴
    bool GetGuildMemberListFromDB( IN const DWORD dwGuildUID, OUT std::map<DWORD, KNGuildUserInfo>& mapUserList ); // 길드맴버 리스트를 DB에서 받아옴
    int UpdateGuildGrade( IN const DWORD dwGuildUID ); // 신규길드 정식길드로 변환
    int UpdateGuildDelete( IN const DWORD dwGuildUID ); // 길드 강제 해산
    bool GetGuildInfoFromDB( IN const DWORD dwGuildUID, OUT KNGuildInfo& kInfo );
    bool GetGuildMarkInfo( IN const DWORD dwGuildUID, OUT UCHAR& ucStat, OUT int& nRevision, OUT std::wstring& strFileName );
    int UpdateDrumoutGuildMember( IN const DWORD dwUserUID, const DWORD dwGuildUID ); // 길드원 자진 탈퇴
    int UpdateAcceptGuildJoiner( IN const DWORD dwCommenderUserUID, const DWORD dwTargetUserUID, const DWORD dwGuildUID ); // 가입대기길드원 일반길드원으로 등급 변경 Game
    int UpdateBreakupGuild( IN const DWORD dwUserUID, const DWORD dwGuildUID ); // 길드 해산 Game
    int UpdateDeleteGuildMember( IN const DWORD dwCommenderUserUID, const DWORD dwTargetUserUID, const DWORD dwGuildUID ); // 길드원 강제 탈퇴 Game
    int UpdateChangeGuildMaster( IN const DWORD dwCommenderUserUID, const DWORD dwTargetUserUID, const DWORD dwGuildUID ); // 길드마스터 이양(일반길드원/제너럴 -> 길드마스터, 길드마스터 -> 제너럴) Game
    int UpdateChangeGuildMemberLevel( IN const DWORD dwCommenderUserUID, const DWORD dwTargetUserUID, const DWORD dwGuildUID, const UCHAR ucGrade ); // 길드원 등급 조정(일반길드원/제너럴 -> 일반길드원/제너럴) Game
    bool UpdateJoinGuildMemberInfo( IN const DWORD dwGuildUID, const DWORD dwUserUID, OUT KNGuildUserInfo& kInfo ); // 길드원 가입시 맴버정보 설정
    int UpdateJoinGuild( IN const DWORD dwUserUID_, IN const DWORD dwGuildUID_, IN const std::wstring& strComment_ ); // 길드원 가입(비길드유저 액션)
    int UpdateGuildJoinSetting( IN const DWORD dwUserUID, const DWORD dwGuildUID, const UCHAR ucJoinSetting ); // 가입 방법 변경 Game

    void IntervalGuildMarkCheck();
    void IntervalGuildRankUpdate();
    bool GetGuildMasterInfoFromDB( IN const DWORD dwGuildUID, OUT KNGuildUserInfo& kInfo ); // 길드마스터 정보 얻어오기
    bool GetRemoveNewbieGuildList( OUT std::set<DWORD>& setGuildUID );

    bool GetGuildCommentNoticeFromDB( IN const DWORD dwGuildUID, OUT std::vector<KNGuildNotice>& vecNotice );
    bool UpdateOneGuildInfo( IN const DWORD dwGuildUID );
    void UpdateOneGuildMemberList( IN const DWORD dwGuildUID );
    void IntervalGuildRemveCheck();
    bool IsExistGuild( IN const DWORD dwGuildUID );
    void GetGuildMemeberInfoFromDB( IN DWORD dwGuildUID, IN DWORD dwUserUID, OUT KNGuildUserInfo& kGuildUserInfo, OUT time_t& tmJoinDate, OUT time_t& tmLastLogin );

    // 091120. kkurrung. 퀴즈 빙고.
    bool UpdateQuizBingoData( IN const bool bForce );
    bool BingoUpdateCheck( OUT int& nEventID, OUT time_t& tmBegin, OUT time_t& tmEnd );
    bool GetBingoQuestions( IN const int nEventID, OUT std::map<int,KBingoQuestionInfo>& mapQuestions );
    // 091208. kkurrung. GWC 응원 이벤트
    bool GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData );
    // 091230 tgkwon. 새해 소원 성취 이벤트
    bool GetTotalDonationInfo( IN time_t tmDate, IN const DWORD dwMinType, IN const DWORD dwMaxType, OUT std::map<DWORD, int>& mapTotalDonation );

    // 단품 아이템 넣기
    bool InsertToWaitItemJob( IN const DWORD dwUID, IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod, IN const int nGetType, OUT __int64& waitUID, IN int nGrade = -1, IN int nItemLevel_ = -1, IN const int nEnchantLevel_ = -1 );
    bool GetCharExpInfo( IN const DWORD dwUID, OUT std::map<char,__int64>& mapCharExp );

    void GetGuideBookList( IN const DWORD dwUID_, OUT std::map<short,short>& mapList_ );
    // 동접 이상 체크.
    void CheckTotalServerUser( IN DWORD dwBeforeUserNum );

    bool IsCurrentBingo( IN const time_t tmBegin_, IN const time_t tmEnd_ );
    
    // 원어데이
    bool GetPackageItem( IN const GCITEMID itemID_, OUT std::vector<KItem>& vecItems_ );
    // 구매제한 아이템리스트
    bool ItemLimit_select( OUT std::set<GCITEMID>& setList );

    // 2011 guild war
    int UpdateGuildBattlePoint( IN const DWORD dwGuildUID_, IN const int BPointDiff_ );
    bool GetGuildPointFromDB( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kInfo_ );
    // 스핑크스 OX 게임.
    bool UpdateSphinxData( IN const bool& bForce, IN const int& nVersion );
    bool SphinxUpdateCheck( IN const int& nVersion );
    bool GetSphinxQuestions( IN const int& nVersion, OUT std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo );

    void InsertBlockIPUserLog( IN const std::wstring& wstrLogin, IN const DWORD dwIP_, IN const std::wstring& wstrIP, IN const bool& bNewUser, IN const std::wstring& wstrNationCode );

    // 설문 시스템
    bool UpdateSurveyData( IN const bool& bForce_, IN const int& nVersion_ );
    bool SurveyUpdateCheck( IN const int& nVersion_ );
    bool GetSurveys( IN const int& nVersion_, IN std::vector< int >& vecLanguageInfo_ , OUT std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyInfo_ );

    KCommonSP m_kHelper;

    //void GetSALT( IN const std::wstring& strLogin_, OUT std::wstring& strSALT_ ); // 2중 암호화. 20110701 woosh
    bool UpdateGWCEventData();
    bool GetGWCTotalCountryInfo( OUT std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo, OUT std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo );

    // 모험 이벤트
    bool UpdateAdventureEventData();
    bool GetAdventureEventVersion( OUT int& nVersion_, OUT time_t& tmStartDate_, OUT time_t& tmEndDate_ );
    bool GetAdventureEventDate( IN const int nVersion_, OUT std::map< int, KAdventureEventDate >& mapAdventureEventDate_ );

    void GetOlympicRegDateRankData( OUT std::vector<KRegDateRankInfo>& vecRegDateRank );
    void GetOlympicTotalRankData( OUT std::vector<KTotalRankInfo>& vecTotalRank );
    bool UpdateOlympicTotalData( IN time_t& tmDate, IN const __int64& biAccumulateData );
    bool UpdateOlympicUserRankData( IN time_t& tmDate, IN const int& nGameType, IN const int& nTeamType, IN const __int64& biAccumulateData, IN const std::wstring& strNickName );
    void GetOlympicUserData( OUT std::map<time_t, KOlympicInfoData>& mapOlympicInfoData );

    // 발렌타인 버프 기부 전체 데이터 가져오기 위해 만듦(센터쪽 기존 함수 사용 하지 않고 타입 하나만 받는거 따로 만듦)
    int GetTotalServerCountData( IN time_t tmDate, IN const DWORD dwType );

    // 누적 출석 세팅 데이터 가져오기(기간, 버전)
    bool GetSettingInfo_AccAttendance( OUT int& nVersion, OUT time_t& tmBegin, OUT time_t& tmEnd );
    void UpdateDBSettingInfo_AccAttendance();

    // 캐시백 비율 정보 가져오기
    bool GetCashbackDefaultAllRatio( OUT std::map< GCITEMID, int >& mapCashbackDefaultRatio_ );
    bool GetCashbackEventAllRatio( OUT time_t& tmStart_, OUT time_t& tmEnd_, OUT std::map< GCITEMID, int >& mapCashbackEventRatio_ );
    void GetTCPServerList( OUT std::vector<KSimpleServerInfo>& vecUseTCPServerList, OUT std::vector<KSimpleServerInfo>& vecNoUseTCPServerList );

    // 소셜 커머스 (보부상)
    void UpdateSocialCommerceData( IN bool bChanged );
    void GetSellSettingInfo_SocialCommerce( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType );
    void GetRewardSettingInfo_SocialCommerce( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType );
    void GetRewardUserInfo_SocialCommerce( IN DWORD& dwEventUID, IN int& nCount, IN VEC_REWARD& vecReward, OUT VEC_GROUP_PURCHASE_REWARD& vecRewardInfo );
    bool UpdateGroupPurchaseRewardComplete_SocialCommerce( IN DWORD& dwEventUID, IN DWORD& dwRewardUID, IN DWORD& dwLoginUID );

    // 던전 랭킹 시스템
    bool Get_DungeonRank_SeasonDelimitTime_FromDB( OUT time_t& tmSeasonDelimit_ );
    bool Get_DungeonRank_CurrentSeasonTopRank_FromDB( IN const std::set< int >& setRankingModes_, OUT std::map< int, std::list< KDungeonRankInfo > >& mapCurrentDungeonRankList_ );
    bool Get_DungeonRank_PreviousSeasonTopRank_FromDB( IN const std::set< int >& setRankingModes_, OUT std::map< int, std::list< KDungeonRankInfo > >& mapPreviousDungeonRankList_ );
    bool Update_DungeonRank_CurrentSeasonNewEntry_ToDB( IN const std::vector< KDungeonRankInfo >& vecNewEntry_, OUT std::vector< KDungeonRankInfo >& vecNewEntryAfterDBWrite_ );
    bool Do_DungeonRank_BuildPreviousSeasonRank_ToDB( IN const CTime& ctCurrent_ );
    bool Get_DungeonRank_PreviousSeason();

protected:
    // handling function
   _INT_DECL_ON_FUNC( DB_EVENT_SERVER_LIST_REQ, int );
    INT_DECL_ON_FUNC( ECN_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( DB_ENU_VERIFY_ACCOUNT_REQ, KENU_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_FIRST_INIT_REQ, int );
   _INT_DECL_ON_FUNC( DB_CONNECTION_COUNT_RECORD_REQ, DWORD );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_MINIGAME_RANK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_GET_TOTAL_DONATION_POINT_REQ );
   _INT_DECL_ON_FUNC( DB_UPDATE_SCRIPT_CHECKSUM_REQ, int );
   _INT_DECL_ON_FUNC( ECN_UPDATE_CONNECTION_COUNT_REQ, DWORD );
    INT_DECL_ON_FUNC_NOPARAM( DB_UPDATE_CHANNEL_NEWS_REQ );
   _INT_DECL_ON_FUNC( ECN_DB_UPDATE_DISCONNECT_TYPE, MAP_INT_INT );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_RAINBOW_UPDATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_UPDATE_CHECK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_MISSION_EVENT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_SUBSCRIPTION_INFO_UPDATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_NPC_GIFTS_UPDATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DICE_PLAY_TOTAL_RANK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_ITEM_DONATION_UPDATE );
    // new guild
    INT_DECL_ON_FUNC_NOPARAM( DB_ECN_UPDATE_GUILD_LIST_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_ECN_UPDATE_GUILD_INFO_CHECK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_GUILD_MARK_INFO_UPDATE_REQ );
    INT_DECL_ON_FUNC( ECN_JOIN_GUILD_REQ );
   _INT_DECL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_REQ, PAIR_DWORD );
   _INT_DECL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_REQ, PAIR_DWORD );
    INT_DECL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ );
    INT_DECL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_REQ );
    INT_DECL_ON_FUNC( ECN_REJECT_GUILD_JOINER_REQ );
    INT_DECL_ON_FUNC( ECN_BREAKUP_GUILD_REQ );
   _INT_DECL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_REQ, KCnNGuildJoinPolicy );
   _INT_DECL_ON_FUNC( ECN_AUTO_CHANGE_GUILD_JOIN_SETTING_NOT, KCnNGuildJoinPolicy );
   _INT_DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_REQ, DWORD );
   _INT_DECL_ON_FUNC( ECN_REMOVE_GUILD_NOT, DWORD );
    INT_DECL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_LIST_REQ );
    INT_DECL_ON_FUNC( ECN_AUTO_REJECT_GUILD_JOINER_REQ );

    INT_DECL_ON_FUNC_NOPARAM( ECN_COUPON_INFO_UPDATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_QUIZ_BINGO_DATA_NOT );
    INT_DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_UPDATE_GWC_DATA );
   _INT_DECL_ON_FUNC( ECN_NEWBIE_GUILD_DESTROY_NOT, DWORD );
    INT_DECL_ON_FUNC_NOPARAM( ECN_GET_TOTAL_NEW_DONATION_REQ );

    INT_DECL_ON_FUNC_NOPARAM( ECN_GUIDE_BOOK_LIST_REQ );
   _INT_DECL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
   _INT_DECL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
    INT_DECL_ON_FUNC( ECN_CHAT_REQ );
    INT_DECL_ON_FUNC( ECN_ADD_GUILD_USER_REQ );
   _INT_DECL_ON_FUNC( ECN_UPDATE_UNIQUE_USER_STAT_REQ, MAP_DWORD_SET );

   _INT_DECL_ON_FUNC( ECN_SQUARE_LIST_REQ, PAIR_DWORD );
   _INT_DECL_ON_FUNC( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_GUILD_POINT_UPDATE_REQ );

    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_SPHINX_DATA_NOT );
   _INT_DECL_ON_FUNC( ECN_UPDATE_LOGIN_CCU, DWORD );

    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_SURVEY_DATA_NOT );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_GWC_EVENT_DATA_REQ );

    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_EVENT_TIME_SYNCRONIZE_NOT );

    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_ADVENTURE_DATA_NOT );

    INT_DECL_ON_FUNC_NOPARAM( ECN_OLYMPIC_RANK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_OLYMPIC_DATA_NOT );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_OLYMPIC_USER_DATA_UPDATE_NOT );
    INT_DECL_ON_FUNC_NOPARAM( ECN_OLYMPIC_DB_USER_DATA_REQ );

   _INT_DECL_ON_FUNC( ECN_BUFF_DONATION_TOTAL_DATA_REQ, KECN_BUFF_DONATION_TOTAL_DATA_NOT );
   _INT_DECL_ON_FUNC( ECN_BUFF_DONATION_DATA_UPDATE_REQ, int );
    INT_DECL_ON_FUNC_NOPARAM( ECN_ACCUMULATE_ATTENDANCE_DATA_UPDATE_NOT );

   _INT_DECL_ON_FUNC( ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT, bool );
    INT_DECL_ON_FUNC( DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ );

    INT_DECL_ON_FUNC( DB_ECN_ITEM_PROVIDE_REQ );

    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_SEASON_DELIMIT_TIME_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_REQ );
    INT_DECL_ON_FUNC_NOPARAM( ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ );
    INT_DECL_ON_FUNC( ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ );
   _INT_DECL_ON_FUNC( ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ, bool );

   _INT_DECL_ON_FUNC( DB_ENU_ACCOUNT_REGISTER_REQ, KENU_ACCOUNT_REGISTER_REQ );
};

inline KTDBLayer<KCenterDBThread>* SiKCenterDBMgr() { return KTDBLayer<KCenterDBThread>::GetInstance(); }

#define QUEUEING_CN_EVENT_TO_DB( id, data )       SiKCenterDBMgr()->QueueingEvent( KCenterEvent::##id, m_strName.c_str(), m_dwUID, data )
#define QUEUEING_CN_ID_TO_DB( id )                SiKCenterDBMgr()->QueueingID( KCenterEvent::##id, m_strName.c_str(), m_dwUID )
