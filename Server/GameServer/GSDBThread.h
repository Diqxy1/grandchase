// code:#END_OF_NORMAL_FUNC
#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "UserPacket.h"
#include "ActionDecider.h"
#include "odbc/Odbc.h"
#include "UserEvent.h"
#include "TimeEventManager.h"
#include "PetHelper.h"
#include "Santa.h"
#include "CenterPacket.h" // KECN_UPGRADE_GUILD_GRADE_GIFT_ACK
#include "SP.h"
#include "VipEventManager.h"

class KGSSimLayer;
class KGSDBLayer;
class KGSDBThread : public KSubThread< KUserEvent >
{
    typedef KGSSimLayer SLayerType;
    DeclareDBThread;
    DeclareDBThreadName( Stat );
public:

    KGSDBThread();
    virtual ~KGSDBThread(void);

    enum {  
        UNKNOWN_ERROR           = -99,  // 051114. KUser에서 기간 만료아이템 처리의 오류확인을 위해 참조.
        
    };

    enum RANK_TYPE
    { 
        RT_DAILY = 1,
        RT_WEEKLY = 2,
        RT_MONTHLY = 3,
        RT_TOTALEXP = 4,
        RT_MAX,
    };

    enum SLOT_TYPE
    {
        ST_ITEM = 0,
        ST_EMOTICON = 1,
        ST_MAX,
    };

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );
    // util
    //void CheckNickName( const char* szNickName );
    int TranslateNicknameToLogin( IN const std::wstring& strNickName, OUT std::wstring& strName );
    int GetNickName( const std::wstring& strName, std::wstring& strNickName );
    bool UpdateUserData( KUserData& kUserData, bool bReturn_ = false );
    bool UpdateUserData( KDB_EVENT_EXIT_GAME_NOT& kNot );
    bool UpdatePetData( const std::map<GCITEMUID, KPetInfo>& mapPetInfo, const std::map<char, KCharacterInfo>& mapCharInfo, const DWORD dwSenderUID, const std::wstring& strLogin );
    bool UpdateDepotPetData( const std::map<GCITEMUID, KPetInfo>& mapPetInfo_, const std::map<char, KCharacterInfo>& mapCharInfo_, const DWORD dwSenderUID_, const std::wstring& strLogin_ );
    bool RegistMission( IN const DWORD dwUID, IN const char& cCharType, IN const GCITEMID dwItemID, OUT KMissionData& kData, OUT int& nRet );
    //전체 메세지.
    void UpdateLoudMsg();
    void UpdateCalendar( IN const DWORD& dwUID, IN const char& cCharType, OUT std::vector<KDailyInfo>& vecCalendar );
    void UpdateRewardItem();
    void GetReserveMonthlyList( IN const DWORD& dwUID, IN const char& cCharType, OUT std::vector<KSimpleDate>& vecRewardList );
    //이벤트 얻어오기.. 
    void UpdateTimeEvent();
    // 통계 초기 처리..
    void StatisticsInit();
    void SelectSellItemList();
    bool GetMsgServerList( OUT std::vector<KServerInfo>& vecServerList );
    // 스킬트리
    //void GetSkillInfo( IN DWORD dwUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo );
    void GetTrainedSkill( IN DWORD dwUID, IN std::map<char, KCharacterInfo>& mapCharacterInfo, OUT KFullSPInfo& kFullSPInfo, IN const std::set<char>& setOldSkillCharacter_ );
    void GetSkillSet( IN DWORD dwUID, IN std::map<char, KCharacterInfo>& mapCharacterInfo, OUT KFullSPInfo& kFullSPInfo, IN const std::set<char>& setOldSkillCharacter_ );
    void GetEquipSkillSet( IN DWORD dwUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo, 
        IN const std::set<char>& setOldSkillCharacter_ );
    void UpdateEquipSkillSet( DWORD dwUID, std::map<char, KCharacterInfo>& mapCharacterInfo ); // 각성에서는 구현하지 않는 기능
    void UpdateSkillSet( IN DWORD dwUID, std::map<std::pair<char,char>, IN std::vector<KSPSetInfo> >& mapSkillSetAdd,
        IN std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSetDel );
    void GetUnLockGroupID( DWORD dwUID, std::map<char, std::set<int> >& mapUnLockGroupID, IN const std::set<char>& setOldSkillCharacter_ );
    bool AddUnLockGroupID( DWORD dwUID, char cCharType, int nUnLockGroupID );
    
    // 스킬 기존 시스템과 스킬개편(각성) 에서 동일하게 사용가능한 함수 
    bool CheckSkillInfo( IN const char cCharType, const char cPromotion, const int nSkillID, const std::map<std::pair<char,char>, KSPInfo>& mapTrainedSkill );

    bool CreateNewChar( IN const DWORD dwUID, IN const std::wstring& strLogin, IN char cCharType, OUT KCharacterInfo& kCharacterInfo );
    bool UpdateCountItems( IN const DWORD dwUserUID, IN const std::vector<KItem>& vecDuration );
    bool UpdateInvenTypeItem( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const std::vector<KItem>& vecDelItem_, IN const char cCharType_ );
    bool DeleteUserItemCharacter( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const char cCharType_ );
    bool InsertUserItemCharacter( DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_, IN const char cCharType_ );

    void GetGachaSetObtained( IN std::vector< int >& vecVersions_, IN DWORD dwUID_, OUT std::map< int, std::set< std::pair< DWORD, DWORD > > >& mapGachaSetObtained_ );
    void GetGachaSetObtained( IN std::vector< int >& vecVersions_, IN DWORD dwUID_, OUT std::map< int, std::map< std::pair< DWORD, int >, std::set< DWORD > > >& mapGachaSetObtainedLevel_ );
    void LeaveGameStat( KEVENT_MAP_STAT_LEAVE_GAME& kPacket );
    //유저UID 얻어오기
    bool LoginToUID( IN const std::wstring& strLogin_, OUT DWORD& dwUID_ );
    bool NickToUID( IN const std::wstring& strNick_, OUT DWORD& dwUID_ );
    bool UIDToNick( IN const DWORD dwUID, OUT std::wstring& strNick );
    void UpdatePeriodNotice();
    // 가차 구매 제한 관련.
    void UpdateCashGachaKeyBuy( DWORD dwUID, bool bCash, int nDiffCount );
    void GetCashGachaKeyBuyCount( DWORD dwUID, PAIR_INT& prData );
    //DB통계기록
    void UpdateDataReportCollet( IN const PAIR_INT prData );
    bool UpdateStatSpxCount( int nGameMode, char cCharType, int nLv, int nSpLv, int nMaxSpx, DWORD dwCount );

    void GetModeLevel( IN const DWORD& dwUID, IN const char& cCharType, OUT std::map<int,KDungeonUserInfo>& mapDifficulty );
    void GetOldModeLevel( IN const DWORD& dwUID, OUT std::map<int,KDungeonUserInfo>& mapDifficulty );
    void UpdateModeLevel( IN const DWORD& dwUID, IN const char& cCharType, IN OUT std::map<char, KncBitStream>& mapOldDifficulty, IN OUT std::map<int, KDungeonUserInfo >& mapNewDifficulty );

    // 커플 관련.
    void SelectCoupleInfo( IN const std::wstring& strLogin, IN const DWORD dwUID, OUT KCoupleInfo& kCoupleInfo );
    void SelectCoupleEquip( IN const std::wstring& strLogin, IN const DWORD dwUID, OUT std::vector<KCoupleEquip>& vecEquip );
    void SelectCoupleReqDenyInfo( IN const std::wstring& strLogin, IN const DWORD dwUID, OUT std::set<DWORD>& setDenyUserList, OUT bool& bAllDeny );
    bool MakeCouple( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const std::wstring& strPartnerLogin, IN const DWORD dwPartnerUID );
    bool UpdateCoupleEquip( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const KCoupleEquip& coupleEquip );
    bool DeleteCoupleEquip( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const KCoupleEquip& coupleEquip );
    bool UpdateCoupleSelectChar( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const char cCharType );
    bool UpdateCoupleDenyInfo( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const DWORD dwDinyUID ); // dwDinyUID == 0 모두 막기
    bool DeleteCouple( IN const std::wstring& strLogin, IN const DWORD dwUID ); // 커플 해제
    void GetWarehouseItems( IN const std::wstring& strLogin, IN const DWORD dwUID, OUT std::vector<KSimpleItem>& vecItems );
    bool InsertWarehouseItems( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const KSimpleItem& kItem );
    void SaveDivorceMsg( DWORD dwReaderUID, IN std::wstring& strMsg );
    void ReadDivorceMsg( DWORD dwReaderUID, OUT std::wstring& strMsg );
    void DeleteWarehouseItem( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const KSimpleItem& kItem );

    bool PureEraseItem( IN const std::wstring& strLogin, IN const DWORD dwLoginUID, IN const GCITEMUID IntemUID, IN const char cCharType_ );
    void GetPetCostume( IN const std::wstring& strLogin, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo, IN const std::vector<KItem>& vecInven );
    bool UpdatePetCostume( IN const std::wstring& strLogin, IN const std::map<GCITEMUID, KPetInfo>& mapPetInfo );

    void GetCharacterPetEquip( IN const DWORD& dwUserUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo );
    void SetCharacterPetEquip( IN const DWORD& dwUserUID, IN const std::wstring& strLogin, IN const GCITEMUID dwPetUID, IN const char cCharType );
    void ResetCharacterPetEquip( IN const std::wstring& strLogin, IN const DWORD dwUserUID );

    // 기부 관련
    bool DonationGP( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const int nGP ); // GP 기부
    void GetDonationPointPerUser( IN const DWORD dwUID, OUT int& nDonationGP ); // 개인 기부 금액 가져 오기.

    //눈꽃 코인.
    void InsertSnowCoin( IN const DWORD& dwUID, IN const int nFactor );

    bool UpdateCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_ );//수량아이템DB기록
    bool UpdatePVExpInfo( IN const DWORD dwUID_ );
    bool UpdateCharacterInfo( IN const std::wstring strLogin, IN const DWORD dwUID, IN const std::map<char, KCharacterInfo>& mapCharacterInfo );

    // 일별 이벤트 관련.
    int GetTodayEventCount( IN const DWORD dwUID, IN const int nEventType ); // 오늘 이벤트 진행 했으면 true
    int GetTodayEventCount( IN const DWORD dwUID, IN const int nEventType, IN const time_t tmDate );
    bool SetTodayEventDoneDiff( IN const DWORD dwUID, IN const int nEventType ); // 오늘 이벤트 셋팅.( Data : 차이값 )
    bool SetTodayEventDoneDiff( IN const DWORD dwUID, IN const int nEventType, IN const time_t tmDate, IN const int nData );
    // 오늘 이벤트 셋팅.( Data : 전체값 )
    bool SetTodayEventDone( IN const DWORD dwUID, IN const int nEventType, IN const time_t tmDate, IN const int nData );
    void WaitingItemMergeDB( IN const DWORD dwUID );
    void GetSantaComment( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector<KSantaInfo>& vecGiftList, IN const int nLanguageCode_ );
    void AddServerCountData( int nData, int nType ); // 카운트 저장.
    void UpdateMissionData( IN const DWORD dwUID, IN const char& cCharType, IN const std::vector<KMissionData>& vecMissionData );

    // 시간 제한 이벤트
    void GetEventCharacter( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventEquipItem( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventLevel( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventModeList( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventParty( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventServer( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetEventPet( IN const DWORD dwUID_, OUT std::vector<KPetBonusInfo>& vecPetBonus_ );
    void GetEventStrengthRatio( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ );
    void GetFreeHellModeEvent( IN const sTimeEvent& sTimeEvent_, OUT std::set<int>& setMode_ );
    void GetPerfectClearBonusEvent( IN const sTimeEvent& sTimeEvent_, OUT std::set<int>& setMode_ );

    bool UpdateCCU( IN KODBC& kODBC, IN const int nProtocol, IN const std::wstring& strIP, IN const int nPort, IN const int nCCU, IN const int nServerID );
    bool GetServerName( IN KODBC& kODBC, IN const std::wstring& strIP, IN const int nPort );
    bool GetServerList( IN KODBC& kODBC, IN const int nProtocol, OUT std::vector<KServerInfo>& vecServerList );
    bool GetServerSort( IN KODBC& kODBC, IN const std::wstring& strIP, IN const int nPort );

    // 게임쿠폰NPC(태국)
     bool CheckGameCoupon( IN const std::wstring& strSerialNum, IN const std::wstring& strPassWD, IN const int nEventUID, OUT int& nCouponNID, OUT int& nItemNID );
     void THGetGameCouponItem( IN const int nItemNID, OUT std::vector<KDropItemInfo>& vecCouponInfo );
     bool UpdateGameCoupon( IN const DWORD dwUID, IN const int nCouponNID );
    // 게임쿠폰NPC
    void GetGameCouponInfoFromDB( IN const int nSet );
    int CheckGameCoupon( IN const DWORD dwUID, IN const int nEventUID, IN const std::wstring& strCouponID, OUT DWORD& dwCouponUID );
    void GetGameCouponItem( IN const int dwEventUID, OUT std::vector<KDropItemInfo>& vecItems );
    int UpdateGameCoupon( IN const DWORD dwUID, IN const int nEventUID, IN const DWORD dwCouponUID );
    bool CheckGameCouponErr( IN const DWORD dwUID_ );       // 쿠폰 오류 정보
    bool UpdateGameCouponFailErr( IN const DWORD dwUID_ );  // 쿠폰 오류 기록

    // 09.04.09. kkurrung. 친구 추천(5th)
    bool GetMyRecomInfo( IN const DWORD dwUID, OUT KRecommenderInfo& kMyRecomInfo, IN const int nVersion );
    bool SetMyRecomInfo( IN const DWORD dwUID, IN const DWORD dwOtherUID, IN const int nState, IN const int nVersion );

    bool GetOtherRecomInfo( IN const DWORD dwUID, OUT std::vector<KRecommenderInfo>& vecRecommender, IN const int nVersion_ );
    bool SetOtherRecomInfo( IN const DWORD dwRecommendeeUID, IN const DWORD dwRecommenderUID, IN const int nState, IN const int nVersion );

    bool GetRecomBonusInfo( IN const DWORD dwUID, OUT int& nState, IN const int nVersion );
    bool SetRecomBonusInfo( IN const DWORD dwUID, IN const int nState, IN const int nVersion );

    void InitialiseRecomInfo( IN const DWORD dwUID, IN const int nSessionInfo, IN const time_t tmFirstLogin, IN const time_t tmLastLogin, OUT KRecommenderInfo& kMyRecomInfo );
    void SelectSanta();
    void GetSantaDormantDate( IN int nEventUID_, OUT sSanta& sData  );
    void SelectSantaItem( IN int nEventUID_, OUT DROP_ITEMS& vecItem_ );
    void GetSantaUserType( IN int nEventUID_, OUT sSanta& sData );
    bool InsertSantaUser( IN DWORD dwUserUID, IN sSanta& sData );
    int CheckSantaRewardRecord( IN DWORD dwUserUID,IN sSanta& sData );
    void ApplySanta( IN DWORD dwUserUID, IN const char cCharType_, OUT std::vector<KSantaInfo>& vecGiftList );
    void ApplySantaPost( IN DWORD dwUserUID, IN const char& cCharType, OUT std::vector<KSantaInfo>& vecGiftList );

    bool RemoveMission( IN const DWORD dwUID, IN const char& cCharType, IN const GCITEMID dwItemID, IN const int nDeleteType );

    // RainBow
    bool CheckJackPotItemGet( IN const DWORD dwUID, IN const int nRainBowID, IN const int nEventID, IN const int nJackPotID );
    bool SetJackPotItemGet( IN const DWORD dwUID, IN const int nRainBowID, IN const int nEventID, IN const int nJackPotID );

    // 09.07.27. kkurrung. 
    bool InsertEffectTexture( int nType, int nIndex, std::wstring& strFile );

    // 누적 해킹횟수 구해오기
    int GetUserHackingCount( IN const DWORD dwUID );
    // 캡슐사용DB기록
    void CapsuleSystemInsertLog( IN const DWORD dwUID, IN const char cCharType, IN const int nItemKind, IN const int nLevel, IN const GCITEMID iteID );
    // 유저의 보드게임 정보 가져오기.
    int GetUserBoardInfo( IN const DWORD dwUID, IN const DWORD dwInitHour, IN const DWORD dwTotalMapCount, OUT KUserBoardInfo& kUserBoard, OUT time_t& tDiceLastTime, IN DWORD dwVersion );
    // 할로윈 보드게임 아이템 획득 로그.
    void HalloweenItemInsertLog( IN DWORD dwUID, IN GCITEMID dwItemID, IN int nCount, IN int nPeriod, IN DWORD dwVersion );
    // 구매 횟수 제한 있는 아이템.
    bool CheckBuyLimitItem( IN const DWORD dwUID, IN const GCITEMID dwItemID );
    void CommitBuyLimitItem( IN const DWORD dwUID, IN const GCITEMID dwItemID );
    // 유저의 보드게임 정보 저장.
    void UpdateUserBoardInfo( IN DWORD dwUID, IN DWORD dwTotalMapCount, IN KUserBoardInfo kUserBoardInfo, IN std::wstring strDiceLastTime, IN DWORD dwVersion );
    // 주사위 이벤트 무료 지급 횟수에 대한 보정 처리.
    void CheckFreeReceiveCount( IN OUT DWORD& dwGiveCount );

    // 09.09.06. kkurrung.  PC방 혜택..
    void InsertIPAdvantageDailyItem( IN const DWORD dwUID, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem_ );

    // 일일 미션 로그.
    void InsertDailyMissionLog( IN const DWORD dwUID, IN const int nEventID, IN const int nOrderID, IN const GCITEMID nItemID );

    // 웹 구매, 선물 받은 아이템 얻어 오기.
    void GetBuyWebAndGiftItems( IN const DWORD dwUID, OUT std::vector<KGiftInfo>& vecGiftInfo );

    void AddInvenExternalItems( DWORD dwUID, IN const char cCharType_ );

    // 퀴즈 빙고
    bool SetBingoCorrectAnswer( IN const DWORD dwUID, IN const int nEventID, IN const int nQuestionID, IN const int nXPos, IN const int nYPos );
    void GetBingoPosition( IN const DWORD dwUID, IN const int nEventID, OUT std::vector<std::pair<int,int> >& vecBingoPos );
    void GetBingoCorrectQuestions( IN const DWORD dwUID, IN const int nEventID, OUT std::vector<int>& vecCorrectQuestions );
    int GetBingoAccTime( IN const DWORD dwUID, IN const int nEventID );
    bool SetBingoAccTime( IN const DWORD dwUID, IN const int nEventID, IN int nAccTime );
    bool SetBingoAccTime( IN const DWORD dwUID, IN const int nEventID, IN const int nAccTime, IN const CTime& tmDate );
    void GetBingoCoinCountInfo( IN const DWORD dwUID, IN const int nEventID, OUT int& nTotalCount, OUT int& nUseCount );
    bool SetBingoCoinCountInfo( IN const DWORD dwUID, IN const int nEventID, IN const int nTotalCount, IN const int nUseCount );

    void CompactionRewards( IN OUT std::vector<KDropItemInfo>& vecRewards );
    bool GetUserEquipInfo( IN const DWORD dwUID, IN const std::vector<KItem>& vecInven, IN OUT std::map<char,KCharacterInfo>& mapCharacterInfo, IN std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi );
    bool GetUserEquipItem( IN const DWORD dwUID_, IN const std::vector<KItem>& vecInven_, IN OUT std::vector<KEquipItemInfo>& vecEquipItem_ ); // 위 함수와 역할은 같으나 조건검사와 출력 컨테이너 차이

    bool UpdateUserEquipInfo( IN const DWORD dwUID, IN const std::map<char, KCharacterInfo>& m_mapCharacterInfo, IN const std::map<char,std::pair<bool,GCITEMUID> >& mapChangeWeapon, IN const std::set<DWORD>& setChangeEquipInfo );
    // 09.11.10 tgkwon. 이벤트 내용 출력 여부 확인.
    void CheckEventInfoEnable( IN const DWORD dwUID, OUT std::set<DWORD>& setStringList );
    // 09.12.07. tgkwon. 추천 (5th) 신규유저 특정레벨 달성시 아이템 지급했는지 체크.
    bool CheckRecomUserLevelup( IN const DWORD dwUID, IN const DWORD dwRecomUserUID, OUT std::pair<int,int>& prRecomRewardInfo, IN const int nVersion );
    void UpdateUserLevelUpInfo( IN const DWORD dwUID, IN const DWORD dwRecomUserUID, IN const int nLevel, IN const int nVersion );
    void UpdateLevelUpRecomReward( IN const DWORD dwUID, IN const DWORD dwRecomUserUID, IN const int nLevel, IN const int nVersion );
    void InsertRecommendUserLevelUp( IN const DWORD dwUID, IN const DWORD dwRecomUID, IN const int nVersion );
    bool CheckRecommendSuppliedItem( IN const DWORD dwUID, OUT std::map<DWORD, std::pair<int,int> >& mapRecomUserList, IN const int nVersion );
    void UpdateRecommendSuppliedItem( IN const DWORD dwUID, IN const DWORD dwRecommendUID, IN const int nRewardLevel, IN const int nVersion );
    // 아이템 교환 로그 정보 저장.
    bool InsertTransformItemLog( IN const DWORD dwUID, IN const GCITEMID dwItemID, IN const int nItemCount, IN const KDropItemInfo kReward );
    // 091210. kkurrung. GWC Event 
    int GetGWCEventUserInfo( IN const DWORD dwUID );
    bool UpdateGWCDonationData( IN const DWORD dwUID, IN const int nCount );

    //void GetGuildTotalRank( OUT std::map< int, std::map< char, std::vector<KWinRateRank> >>& mapGuildRank_ );
    void GetRankingInfo( const std::wstring& strQuery, char eRankType );
    void GetGuildRank( int nType_, IN std::wstring& strName_, OUT KWinRateRank& kRank_ );

    // new guild.
    int UpdateGuildPoint( IN const DWORD dwUserUID, const DWORD dwGuildUID, const DWORD dwGPoint, const int iWin, const int iLose ); // 길드전 전적갱신
    int UpdateGuildBattlePoint( IN const DWORD dwGuildUID_, IN const int BPointDiff_ );
    int UpdateCreateGuild( IN const DWORD dwUserUID, const std::wstring& strGuildName, const std::wstring& strGuildURL, OUT DWORD& dwGuildUID ); // 길드생성 Game
    bool GetGuildCommentNoticeFromDB( IN const DWORD dwGuildUID, OUT std::vector<KNGuildNotice>& vecNotice ); // 길드 생성시 소개 및 공지 설정
    bool GetGuildMemberListFromDB( IN const DWORD dwGuildUID, OUT std::map<DWORD, KNGuildUserInfo>& mapUserList ); // 길드 생성시 맴버정보 설정
    int UpdateGuildMark( IN const DWORD dwUserUID, const DWORD dwGuildUID ); // 길드마크 등록 Game
    int UpdateGuildName( IN const DWORD dwUserUID, const DWORD dwGuildUID, const std::wstring strGuildName ); // 길드이름 변경 Game
    int UpdateGuildURL( IN const DWORD dwUserUID, const DWORD dwGuildUID, const std::wstring strURL ); // 길드URL 변경 Game
    int UpdateGuildCommentNoice( IN const DWORD dwUserUID, const DWORD dwGuildUID, const int iMsgID, const std::wstring strMsg ); // 길드 소개 및 공지 변경 Game
    int UpdateMyGuildComment( IN const DWORD dwUserUID, const DWORD dwGuildUID, const std::wstring strComment ); // 자기소개 변경 Game
    bool SearchGuildListFromDB( IN const std::wstring strKeyword, OUT std::vector<DWORD>& vecGuildUIDList ); // 길드목록에서 이름으로 검색하여 길드UID목록을 받아옴.
    bool GetGuildMarkInfo( IN const DWORD dwGuildUID, OUT int& nStat, OUT int& nRevision, OUT std::wstring& strFileName );
    bool SetGuildMarkRegisted( IN const DWORD dwUID, IN const DWORD dwGuildUID, IN const std::wstring& strExt, IN const bool bCashItem );
    bool GetGuildMasterInfoFromDB( IN const DWORD dwGuildUID, OUT KNGuildUserInfo& kInfo ); // 길드마스터 정보 얻어오기
    DWORD GetGuildUIDFromDB( IN const DWORD dwUserUID ); // DB에서 길드UID 얻어오기
    bool GetGuildInfoFromDB( IN const DWORD dwGuildUID, OUT KNGuildInfo& kInfo ); // 길드 생성시 길드정보 설정. DB에서 길드정보 가져옴
    int UpdateGuildGrade( IN const DWORD dwGuildUID );

    // 091221. kkurrung. 길드 정보 가져 오기.
    void GetAllGuildRankData( OUT std::map<DWORD,KSimpleGuildRank>& mapSimpleRank );
    bool GetAllGuildInfo( OUT std::map< DWORD, KNGuildInfo >& mapGuildList );
    bool GetAllGuildNotice( OUT std::map< DWORD, std::vector<KNGuildNotice> >& mapGuildNotice );

    // 유저의 길드원 정보 받아오기.
    void GetUserGuildInfo( IN const DWORD dwUserUID, OUT KNGuildUserInfo& kUserGuildInfo );
    void UpdateOneGuildInfo( IN const DWORD dwGuildUID ); // DB에서 길드정보 받아와서 등록
    void UpdateOneGuildMemberList( IN const DWORD dwGuildUID );

    // 091230 tgkwon. 새해 소원 성취 이벤트
    bool AddServerCountData( IN int nData, IN int nType, IN time_t tmDate );
    int GetTotalServerCountData( IN time_t tmDate, IN const DWORD dwType );

    // 091223. kkurrung, milennium9. 아이템 관련.
    KSP m_kHelper;

    void GetItemListFromDB( IN const DWORD dwUID, IN const char cCharType_, OUT std::vector<KItem>& vecInven, OUT std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi );
    void GetDepotItemListFromDB( IN const DWORD dwUID_, IN const KDepotInfo& depotInfo_, OUT std::vector<KItem>& vecInven_, OUT std::map<GCITEMUID, GCITEMUID>& mapStrengthItem );

    void GetPetInfo( IN const DWORD dwUID, IN const char& cCharType, IN const std::vector<KItem>& vecInven, OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo );
    // 100118. tgkwon. 주사위 이벤트 구조 변경
    void SetInitDicePlayLog( IN const DWORD dwUID, IN const DWORD dwInitHour );

    // 2010 01 19. woosh 소켓 관련, 아이템의 장착가능 레벨 캐싱
    void UpdateItemEquipLevelList();
    bool UpdateStrongItemLevel( IN const DWORD dwUID, IN const GCITEMID dwItemID, OUT int& nItemKind, OUT int& nNowLevel, OUT int& nCharType );
    int UpdateStrongItem( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const GCITEMID dwItemID, IN const DWORD dwItemUID, IN const int nDecision, IN const GCITEMID dwMaterialItemID, IN const DWORD dwUsrMaterialCount, OUT int& nNextGoodsID );

    // 악세서리 강화
    bool GetStrongItemLevel( IN const GCITEMID dwItemID, OUT int& nItemKind, OUT int& nNowLevel, OUT int& nCharacterType );
    int UpdateEnchantStrongItem( IN const DWORD dwUID, IN const char& cCharType, IN const GCITEMUID dwItemUID, IN const GCITEMID dwItemID, IN const int nDecision, OUT GCITEMID& dwNextItemID );
    // 100225. tgkwon. 선물상자 이벤트.
    void PresentBoxInsertLog( IN DWORD dwUID, IN DWORD dwVersion, IN GCITEMID dwItemID, IN int nDuration, IN int nPeriod );
    // 100408 길드 멤버 정보 불러오기.
    void GetGuildMemeberInfoFromDB( IN DWORD dwGuildUID, IN DWORD dwUserUID, OUT KNGuildUserInfo& kGuildUserInfo, OUT time_t& tmJoinDate, OUT time_t& tmLastLogin );
    void GBGAGuideBook_merge( IN const DWORD dwUID_, IN const std::pair<short,short>& prData_ );
    // 던전 통계
    bool UpdateDungeonStat( IN std::map<KDungeonStat,int>& mapData );
    // 변경된 스킬 정보 저장
    bool UpdateAddSkillTreeSet( IN const DWORD dwUID, IN int nChar, IN int nPromotion, IN int nSetID, IN int nSlotIndex, IN int nSlotID, IN int nSkillID );
    bool UpdateDeleteSkillTreeSet( IN const DWORD dwUID, IN int nChar, IN int nPromotion, IN int nSetID, IN int nSlotIndex, IN int nSlotID, IN int nSkillID );

    // 유저의 달력정보 갱신.
    bool UpdatePointbagCalender( IN const DWORD& dwUID, IN const char& cCharType, IN std::vector<KDailyInfo>& vecDailyInfo );
    // 레벨업 이벤트
    bool UserLevelupCheck( IN const DWORD dwUID );
    bool VerifyCharExpForStat( IN const std::wstring& strLogin_, IN const DWORD dwLoginUID_, IN const KCharacterInfo& kCharInfo_ );
    bool GetCharacterInfo_1( IN const DWORD dwLoginUID_, IN const char cCharType_, OUT KCharacterInfo& kCharInfo_ );

    // 룩 관련.
    void AdjustLookItemInfo( IN const DWORD dwLoginUID_, IN OUT std::vector<KItem>& vecItems_, IN const char cCharType_ );
    void LoadFullLookInfo( IN const DWORD dwLoginUID_, OUT KLookFullInfo& kFullInfo_ );
    void UpdateLookEquipInfo( IN const DWORD dwLoginUID_, IN const std::map<char, std::map<int,KCoordiDiffInfo> >& mapLookEquips_ );

    void GetMainMissionInfo( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector<KMissionData>& vecMissionSlot);
    // 묶어 팔기
    void BundleSellCountItemSync( IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<std::pair<KItem,int> >& vecCountItems_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_ );
    void BundleSellEraseItems( IN const DWORD dwLoginUID_, IN OUT std::vector<GCITEMUID>& vecEraseList_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_, IN const char cCharType_ );
    void BundleSellCountItems( IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<std::pair<KItem,int> >& vecSellCountItem_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_,
        IN OUT int& nGamePoint_, IN OUT std::map<GCITEMUID, KItem>& mapRewardItem_, OUT std::vector<KItem>& vecSellSucceed_, IN const DWORD& dwSlotNum_ );
    void BundleSellNormalItems( IN const DWORD dwLoginUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecSellNormalItem_, IN OUT std::map<GCITEMUID,int>& mapFailInfo_,
        IN OUT int& nGamePoint_, IN OUT std::map<GCITEMUID, KItem>& mapRewardItem_, OUT std::vector<GCITEMUID>& vecSellSucceed_, IN const DWORD& dwSlotNum_ );
    bool UpdateBossDungeonStat( IN std::vector<KBossDungeonStat>& vecBossDungeonStat );
    // 미션 완료 함수
    bool CompleteMissionInfo( IN const DWORD dwUID, IN const char& cCharType, IN const DWORD dwMissionID, IN bool bUnique, IN const int nDeleteType, OUT int& nRet );
    bool UpdateCharPromotion( IN std::wstring strLogin, IN const char cCharType, IN const char cCharPromotion, OUT int& nRet );
    // 캐릭터 전직 시스템 전용 미션 완료 함수
    bool PromotionCompleteMission( IN const DWORD dwUID, IN const char& cCharType, IN const DWORD dwMissionID, OUT int& nRet );
    bool UpdateLoadingInfoStat( IN int nDateType, IN int nLoadingType, IN int nDateID, IN int nCount );

    // 가위바위보
    void GetUserGawibawiboInfo( IN const DWORD dwUID_, IN DWORD dwVersion_, OUT KUserGawibawiboInfo& kUserGawibawibo_ );
    void InitGawibawiboPlayLog( IN const DWORD dwUID_ );
    void GawibawiboItemInsertLog( IN DWORD dwUID_, IN GCITEMID dwItemID_, IN int nDuration_, IN int nPeriod_, IN DWORD dwVersion_ );
    int UpdateUserGawibawiboInfo( IN DWORD dwUID_, IN KUserGawibawiboInfo kUserGawibawiboInfo_ );
    int CheckGawibawiboInit( IN const DWORD dwUID_ );
    void SetSkillInfo( IN DWORD dwUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo );

    // PT/DT 추가.
    void InsertPlayTimeStat( IN const DWORD dwUID, IN const DWORD dwDTTime, IN  DWORD dwPTTime );

    void SelectSocksInfo( IN const DWORD dwUID_, IN const GCITEMID ItemID_, OUT int& nIsHangup_, OUT DWORD& dwRemainSec_ );
    int SocksHangup( IN const DWORD dwUID_, IN const GCITEMID ItemID_, IN const DWORD dwRemainSec_ );
    int SocksCollect( IN const DWORD dwUID_, IN const GCITEMID ItemID_, IN const DWORD dwRemainSec_ );

    void InitChangeWeapon( IN const std::map<char, KCharacterInfo>& mapCharInfo, OUT std::map<char,std::pair<bool,GCITEMUID> >& mapWeaponInfo );
    void SyncChangeWeapon( IN const std::vector<KItem>& vecInven, IN const std::map< char, std::pair<bool,GCITEMUID> >& mapChangeWeapon, IN OUT std::map<char,KCharacterInfo>& mapCharacterInfo );

    bool CheckClientChecksum( IN const KDB_EVENT_VERIFY_ACCOUNT_REQ& kReq_ );
    char CheckPCBangUser( IN const std::wstring& strIP_ );
    void UpdateIndigoRank();
    void InitDiceDailyFree( IN const int dwEventType );
    void SetRandomDiceNum( IN const DWORD& dwRecvUserUID, IN const std::vector<DWORD>& vecUserUID, OUT std::map<DWORD,int>& mapUserDice );
    int InsertGachaSetReward( IN const DWORD dwUID, IN const GCITEMID dwGachaItemID, IN const int nGachaVersion, IN const DWORD dwType  );
    int InsertGachaSetReward( IN const DWORD dwUID, IN const GCITEMID dwGachaItemID, IN const int nGachaVersion, IN const DWORD dwType, IN const int nItemLevel );
    // 110222. tgkwon 나무나무 이벤트.
    void GetPlantTreePosition( IN const DWORD& dwUID, IN const DWORD& dwVersion, OUT DWORD& dwContinentType, OUT DWORD& dwCurrentPosition, OUT DWORD& dwTreeStatus );
    // 110222. tgkwon 나무나무 이벤트.
    int SetPlantTreePosition( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN DWORD& dwContinentType, IN DWORD& dwCurrentPosition, IN DWORD& dwTreeStatus );
    int CheckUserAuthCheck( IN const DWORD& dwType, IN const std::wstring& wstrLogin, IN const std::wstring& wstrNickName, IN const std::wstring& wstrBirthDay, IN const std::wstring& wstrEmail );
    void UpdateVirtualCash( IN const DWORD& dwUID_, IN const DWORD& dwVirtualCash_ );
    void InsertVirtualCashLog( IN const DWORD& dwUID, IN const std::wstring& strAccount, IN const int& nRewardType );
    void GetVirtualCashLog( IN const DWORD& dwUID, OUT time_t& tmVirtualEnableDate );
    char CheckTH3BBUser( IN const std::wstring& strIP, IN const DWORD dwUserType );

    bool CheckEventNewbieReward( IN KVipEvent& kInfo, IN const int& nPoint, IN const time_t& tmFirstLogin, IN const std::map<int,bool>& mapRewardCheck );
    bool CheckEventVipReward( IN KVipEvent& kInfo, IN const int& nPoint, IN const std::map<int,bool>& mapRewardCheck, OUT std::set<USHORT>& setPayTableID );
    void InsertVipEventRewardItemToDB( IN const DWORD& dwUserUID, IN const char cCharType_, IN const USHORT& usVer, IN const std::map<USHORT,std::vector<KDropItemInfo> >& mapReward, OUT std::vector<KItem>& vecItem );
    void GetSphinxUserData( IN const DWORD& dwUID, IN const int& nVersion, OUT int& nRigthAnswerCnt, OUT std::vector<int>& vecQuestionIDs );
    void UpdateUserSphinxAnswerCnt( IN const DWORD dwUID, IN const int nVersion, IN const int nLevel, IN const int nRigthAnswerCnt );
    void InsertUserSphinxQuizInfo( IN const DWORD dwUID, IN const int nVersion, IN const int nLevel, IN const int nQuestionID );
    void InsertDungeonUseItemStat( IN const int nModeID, IN const int nDifficult, IN std::map<GCITEMID, int> mapItemUseCount, IN const DWORD dwLoginUID, IN time_t tmDate );
    void InsertDungeonBuyItemStat( IN const int nModeID, IN const int nDifficult, IN std::map<std::pair<DWORD, GCITEMID>, int> mapItemBuyCount, IN const DWORD dwLoginUID, IN time_t tmDate );
    void InsertDungeonZoneDeathStat( IN const int nModeID, IN const int nDifficult, IN std::map<DWORD,int> mapZoneDeathCount, IN const DWORD dwLoginUID, IN time_t tmDate );
    void InsertDungeonDeathTypeStat( IN const int nModeID, IN const int nDifficult, IN std::map<std::pair<DWORD, DWORD>, int> mapDeathType, IN const DWORD dwLoginUID, IN time_t tmDate );
    void UpdateUserGamePoint( IN const DWORD dwUID_, IN const char cCharType_, IN const int& nSlotID_, IN const int nGamePointDiff_, OUT int& nOK_, OUT int& nGP_ );
    void GetSubMissionInfo( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector<KMissionData>& vecMissionSlot );
    bool CheckUpdateCountItem( IN const DWORD dwUID, IN const GCITEMID ItemID, IN const char cGrade, IN const int nDellFactor, IN const char cCharType_ );

    void GetSantaItemInfo( IN const DWORD dwEventUID, IN const DWORD dwUserType, OUT std::vector<KDropItemInfo>& vecData );
    int UpdateSantaInfo( IN const DWORD dwEventUID, IN const DWORD dwUserUID, IN const char& cCharType );
    bool SetTypeEraseItem( IN const std::wstring strLogin, IN const DWORD dwLoginUID, IN const GCITEMUID IntemUID, IN const DWORD dwDelType, IN const char cCharType_ );

    // 강화석
    void InsertStrengthStat( IN const DWORD dwUID, IN const GCITEMID StrengthItemID, IN const GCITEMUID StrengthItemUID, IN const int nStrengthLevel, IN const GCITEMID EquipItemID, IN const GCITEMUID EquipItemUID, IN DWORD dwType, IN DWORD dwResult, IN GCITEMID AssistItemID );
    void ItemStrengthMigration( IN const DWORD& dwUserUID, IN const char cCharacter_ );

    void AdjustObjectItemInfo( IN const DWORD& dwUserUID, IN OUT std::vector<KItem>& vecItems );
    bool AgitInfo_select( IN const DWORD& dwUserUID, IN OUT KAgitInfo& kInfo );
    bool AgitFairyTree_select( IN const DWORD& dwUserUID, OUT KFairyTree& kInfo );
    bool AgitFairyTreeFruit_select( IN const DWORD& dwUserUID, OUT time_t& tmDate,OUT int& nFruitType );
    bool GetTodayUseFruit( IN const DWORD& dwUserUID );
    bool AgiteFlowerpot_init( IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID );

    bool UpdateGuildMarkInfo( IN const DWORD dwGuildUID );

    bool DailyStageCollect_insert( IN const KEVENT_STAT_DUNGEON_STAGE_NOT& kPacket );
    bool DailyCPLItemCollect_insert( IN const KEVENT_STAT_CHAR_EQUIP_NOT& mapList );
    bool DailyCPLPTCollect_insert( IN const KEVENT_STAT_CHAR_LV_PT_NOT& vecList );

    // 설문 시스템
    bool GetUserSurveyInfo( IN const int& nSurveyVersion, IN const DWORD& dwUID, OUT std::map<int,std::set<std::pair<int,int>>>& mapUserSurveyInfo );
    void InsertUserSurveyAnswer( IN const DWORD& dwUID, IN const int& nVersion, IN const int& nType, IN const int& nQuestionUID, IN const int& nDataFirst, IN const int& nDataSecond, IN const std::wstring& wstrAnswer, IN const int& nLanguageID );

    // 크리스마스 이벤트
    void GetCharismasUserData( IN const DWORD& dwUID, IN const DWORD& nVersion, OUT DWORD& dwGrade, OUT DWORD& dwBonusPoint );
    bool InsertCharismasGift( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const DWORD& dwReceiveUID, IN const DWORD& dwItemID );
    bool UpdateCharismasUserData( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const DWORD& dwGrade, IN const int& nBonusPoint );
    bool UpdateCharismasDonationInfo( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const DWORD& dwDonationCnt );
    // 필살기 단축키 아이템(Hotkey System)
    void InsertHotkeyUsedInfoStat( IN const DWORD& dwUID_, IN const int& nGameMode_, IN const char& cCharType_, IN const char& nUsedType_, IN const char& cSkillLevel_, IN const int& nSkillCount_ );

    bool InsertSystemPost( IN OUT KPostItemInfo& kInfo );
    bool InsertShanghaiPost( IN const DWORD dwLoginUID, IN const char& cCharType, IN const KDropItemInfo& kDropItem );
    bool GetNewPostLetterInfo( IN const std::wstring& strLogin, IN const DWORD dwUID, IN const char cCharType_, IN OUT std::vector<KPostItemInfo>& vecPostItem );
    int InsertUserTraceMainInfo( IN const DWORD& dwUID, IN const std::wstring& strStartDate, IN const std::wstring& strEndDate, IN const int& nGrade, IN const DWORD& dwPlayTime, IN const bool& bPCBang, IN const bool& bGuildServer, OUT int& nUID );
    int InsertUserTraceCharacterInfo( IN const DWORD& dwUID, IN const int& nUID, IN const int& nCharType, IN const __int64& biExp, IN const DWORD& dwLevel, IN const DWORD& dwPlayCount, IN const bool& bTurtleBack, IN const __int64& biTotalExp );
    int InsertPetInfo( IN const DWORD& dwUserUID, IN const char& cCharType, IN const __int64& biPetUID, IN const int& nPetID, IN const std::wstring& wstrPetName, OUT char& cPromotion, OUT int& nSatiation );
    void GetMainCollectionInfo( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector<KCollectionData>& vecCollection );
    void GetSubCollectionInfo( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector<KCollectionData>& vecCollection );
    void UpdateCollectionData( IN const DWORD dwUID, IN const char& cCharType, IN const std::vector<KCollectionData>& vecCollectionSlot );
    //void GetMyRankInfo( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KMyRankInfo& kMyRankInfo );
    bool UpdateStrengthItemReward( IN const DWORD& dwUID, IN OUT KItem& StrengthItem, IN OUT KItem& EquipItem, IN std::list<std::pair<int,float>>& listAttributeList, IN const char cCharType_ );

    // 길드 마크 수정
    void GetRankInfo( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KMyRankInfo& kMyRankInfo );
    // 송크란 이벤트
    void GetMultiEventUserData( IN const DWORD& dwUID, IN const DWORD& dwType,IN const DWORD& dwVersion, OUT DWORD& dwPoint, OUT DWORD& dwGrade, OUT DWORD& dwDuration, OUT DWORD& dwSuppliedDuration, OUT int& nAccTime );
    bool InsertMultiEventGift( IN const DWORD& dwUID, IN const DWORD& dwType_,IN const DWORD& dwVersion, IN const DWORD& dwReceiveUID, IN const DWORD& dwItemID );
    bool UpdateMultiEventUserData( IN const DWORD& dwUID, IN const DWORD& dwType_,IN const DWORD& dwVersion, IN const DWORD& dwPointDiff, IN const DWORD& dwGradeDiff, IN const int& nDurationDiff, IN const DWORD dwSuppliedDuration, IN const int& nAccTime );
    bool UpdateMultiEventDonationInfo( IN const DWORD& dwUID, IN const DWORD& dwType_, IN const DWORD& dwVersion, IN const DWORD& dwDonationCnt );
    bool CheckGradeReward( IN const DWORD& dwUID, IN const DWORD& dwType,IN const DWORD& dwVersion, IN const DWORD& dwGrade );
    bool UpdateWateBombCount( IN const DWORD& dwUID, IN const DWORD& dwType,IN const DWORD& dwVersion, IN const DWORD& dwItemCountDiff );
    bool UpdateSongkranWaterBombUseCount( IN const DWORD& dwUID, IN const DWORD& dwType,IN const DWORD& dwVersion, IN const DWORD& dwUseCount );
    bool UpdateSongkranWaterBombSuppliedCount( IN const DWORD& dwUID, IN const DWORD& dwType,IN const DWORD& dwVersion, IN const DWORD& dwProvide );

    // 모험 이벤트
    bool GetAdventureContinentRewardInfo( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT std::map< int, bool >& mapContinentData_ );
    bool GetAdventureCollectCountInfo( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT std::map< int, std::map< DWORD, int > >& mapCollectCountData_ );
    bool UpdateContinentUserData( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN const int& nContinentID_, IN const int& nSupplied_ );
    bool UpdateModeCountUserData( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN const int& nContinentID_, IN const int& nCollectID_, IN const int& nDiff_ );
    bool UpdateAdventureUserData( IN const DWORD& dwUID_, IN const int& nVersion_,
        IN const std::map< int, KContinentRewardInfo >& mapContinentRewardInfo_ );

    // 송크란 데이터 초기화 시점 얻기/쓰기
    bool GetSongkranEventResetTime( IN const DWORD& dwUID, OUT time_t& tResetTime );
    bool UpdateSongkranEventResetTime( IN const DWORD& dwUID, IN const CTime& tmDate );

    void UpdateFinalLoginTime( IN const DWORD& dwUID );
    int GetLastPlayCharacter( IN const DWORD dwUID_ );

private:
    void Character_gameinfo_select( IN const DWORD& dwUserUID_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ );
    void GetSealedCharInfo( IN const DWORD& dwUserUID_, OUT std::map< int, KSealedCharInfo >& kSealedCharInfo_ );
    void CharacterSeal_Select( IN const DWORD& dwUserUID_, OUT std::set< int >& setSealedChar_ );
    void UpdateSealedCharInfo( IN const DWORD& dwUserUID_, IN std::map< char, KCharacterInfo >& mapCharacterInfo_, IN OUT std::map< int, KSealedCharInfo >& kSealedCharInfo_ );
    void CharacterSeal_Merge( IN const DWORD& dwUserUID_, IN int& nCharType_, IN bool bSealed_ );

    // 창고
    bool DeleteDepotItem( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_ );
    bool DeleteDepotCountItem( IN DWORD const dwUserUID_, IN const char& cCharType, IN const std::vector<KItem>& vecItem_ );
    bool DeleteDepotZeroCountItem( IN DWORD const dwUserUID_, IN const char& cCharType_, IN const std::vector<KItem>& vecItem_ );

    bool MergeDepotItem( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecItem_ );
    bool GetPetExp( IN DWORD const dwUserUID, IN const char& cCharType, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo );
    bool GetPetExp_Depot( IN DWORD const dwUserUID_, IN const KDepotInfo& kDepot_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_ );
    void GetSkillSlotOpenInfo( IN const DWORD& dwUID, OUT std::map<char, int>& mapSlotOpenInfo );
    bool UpdateCharSlotIndex( IN const DWORD& dwUID, IN const char& cCharType, IN const int& nAddSlotIndex );
    bool DungeonCharStat_insert( IN const KEVENT_STAT_DUNGEON_CHARACTER_NOT& vecList );
    bool UserDungeonCollect_insert( IN const KDB_EVENT_USER_DUNGEON_STAT_NOT& vecInfo );
    bool DungeonCharSkillCount_insert( IN const DWORD dwUID, IN const KEVENT_STAT_CHARACTER_SKILL_COUNT& prData );
    bool CheckUpdateClearData( IN const KDungeonClearData& kDungeonClearData );
    bool ViewGradeTime_insert( IN const KUserViewGradeTime& kUserViewGradeTime );

    //  레벨 초기화 이벤트
    void GetLevelResetEventUserData( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN OUT std::map<char,std::pair<bool,bool>>& mapResetCharInfo_ );
    bool SetLevelResetEventChar( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const char& cCharType, IN const bool& bSupplied, IN const bool& bCashType );
    bool SetLevelResetEventExp( IN const std::wstring& strLogin, IN const DWORD& dwUID, IN const char& cCharType, IN __int64 biDiffExp );

    bool OlympicUserData_merge( IN const DWORD& dwUID, IN const int& nIncreasedData, IN time_t& tmTodayDate, IN const int& nGameType, IN const int& nTeamType );
    void GetOlympicUserTimeIndex( IN const DWORD& dwUID, OUT time_t& tmTodayDate, OUT int& nTimeIndex, OUT DWORD& dwGrade );
    void GetOlympicUserAccumulateData( IN const DWORD& dwUID, IN time_t& tmTodayDate, OUT int& nMyAccumulateData );
    bool UpdateOlympicUserTimeIndex( IN const DWORD& dwUID, IN time_t& tmTodayDate, IN int& nTimeIndex );
    bool UpdateOlympicUserGradeData( IN const DWORD& dwUID, IN const DWORD& dwGrade );
    bool InsertDungeonLoadingInfo( IN const DWORD& dwUserUID, IN const KUserLoadingStat& kUserLoadingStat );
    bool InsertUserChangeCharCount( IN const KDB_EVENT_USER_CHANGE_CHAR_STAT& vecList );
    bool InsertUserModeClearInfo( IN const KDB_EVENT_USER_MODE_CLEAR_STAT& vecList );
    bool RoomMonsterKillInfo_insert( IN const KDB_EVENT_ROOM_MONSTER_KILL_STAT& kMonKillPacket );
    void GetUserCharLevelEventData( IN const DWORD& dwUID, IN const DWORD& dwVersion, OUT std::set<char>& setCharLevelEvent );
    bool InsertUserCharLevelEventData( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const char& cCharType );
    bool InsertItemComposeRewardLog( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN std::vector<GCITEMID>& vecList, IN const GCITEMID& dwItemID );
    bool InsertSkillMissionInfo( IN const std::map<DWORD, std::pair<int,DWORD>>& mapDBUpdateSkillMissionInfo );
    bool UpdateGachaLotteryLog( IN const DWORD& dwUID, IN const DWORD& dwVersion, IN const char& cCharType, IN const GCITEMID& dwRewardItemID,  IN const DWORD& dwRewardType, IN const int& nItemRemainCount );
    void RegistGameMode( IN KODBC* pkODBC_ ); // DB_EVENT_SERVER_INFO_REQ에서 함수로 뺌 . woosh
    bool InsertMonsterTypeStatInfo( IN const KMonsterTypeStat& kMonsterTypeStat );
    void GetPVPELOInfo( IN const DWORD& dwUserUID, IN const int& nSeasonYear, IN const int& nSeasonVersion, IN const char& cCharType, IN OUT KCharacterInfo& kCharacterInfo );
    bool UpdatePVPELOInfo( IN const DWORD& dwUID, IN const int& nSeasonYear, IN const int& nSeasonVersion, IN const char& cCharType, IN const int& nRatingPointDiff, IN const int& nWinDiff, IN const int& nLoseDiff, IN const int& nMatchingCntDiff, IN const int& nLastWinLose, IN const int& nConstantK, IN const UCHAR& ucGrade );
    void Character_ELOData_select( IN const DWORD& dwUserUID_, IN const int& nSeasonYear, IN const int& nSeasonVersion, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ );
    bool InsertDelMatchInfo( IN const KDB_EVENT_DEL_MATCH_REQ& kDBPacket );
    bool InsertPVPELOStatInfo( IN const KDB_EVENT_MATCH_ELO_RESULT_REQ& kDBPacket );

    // 리타의 크리스마스 이벤트
    bool Get_RitasChristmas_StartCoin_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nStartCoinCount_, OUT time_t& tmStartCoinReceived_ );
    bool Get_RitasChristmas_ContinueCoin_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nContinueCoinCount_ );
    bool Get_RitasChristmas_PremiumRewardCount_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nPremiumRewardCount_ );
    bool Get_RitasChristmas_TodayMaxPoint_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nTodayMaxPoint_ );
    bool Update_RitasChristmas_NewRecord_ToDB( IN int nVersion_, IN DWORD dwUID_, IN const int nPointDiff_ );
    bool Get_RitasChristmas_RankInfo_FromDB( IN DWORD dwUID_, OUT std::vector< KRitasChristmasRankInfo >& vecRankInfo_ );
    bool Update_RitasChristmas_UserData_ToDB( IN DWORD dwLoginUID_, IN KRitasChristmasEventUserData& kRitasChristmasUserData_ );
    bool Update_RitasChristmas_StartCoin_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nStartCoinDiff_, IN time_t tmStartCoinReceived_ );
    bool Update_RitasChristmas_PremiumRewardCount_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nPremiumRewardDiff_ );
    bool Update_RitasChristmas_ContinueCoin_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nContinueCoinCount_ );
    void GetRPMatchRank();
    void GetWinRecordMatchRank();
    void GetRPMatchRankHOF();
    void GetWinRecordMatchRankHOF();
    void GetExpMatchRank();
    void GetDefaultSlotInfo( IN const DWORD& dwUserUID, OUT char& cDefaultCharType );
    void GetUserSlotCnt( IN const DWORD& dwUserUID, OUT int& nTotalSlotCnt );

    bool UpdateUserSlotCnt( IN const DWORD& dwUserUID, OUT int& nAddSlotCnt );
    int InsertSlotCharInfo( IN const DWORD& dwUserUID, IN const char& cCharType, IN const DWORD& dwSlotNumber, OUT KCharacterInfo& kCharacterInfo );
    int UpdateCharNickName( IN const DWORD& dwUserUID, IN const char& cCharType, IN const DWORD& dwSlotNum, IN const std::wstring& wstrNickName );	
    void GetCharNickName( IN const DWORD& dwUserUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo );
    void GetLastPlayCharNickName( IN const char cLastPlayCharacter_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, OUT std::wstring& wstrLastPlayCharNickName_ );
    void GetMatchRankInfo( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KMyMatchRankInfo& kMyRankInfo );
    void GetUserRPMatchRank( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KMatchRank& kMyRPRankInfo );
    void GetUserWinRecordMatchRank( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KMatchRank& kMyWinRecordRankInfo );
    void GetUserExpMatchRank( IN const DWORD& dwUserUID, IN const int& nTabType, OUT KExpRank& kMyExpRankInfo );
    void FindRPRankUseNickName( IN const char& cCharType, IN const std::wstring& strNickName, OUT KMatchRank& kMatchRank );
    void FindWinRecordRankUseNickName( IN const char& cCharType, IN const std::wstring& strNickName, OUT KMatchRank& kMatchRank );
    void FindExpRankUseNickName( IN const char& cCharType, IN const std::wstring& strNickName, OUT KExpRank& kExpRank );
    void FindRPRankUseNickNameHOF( IN const char& cCharType, IN const std::wstring& strNickName, OUT KMatchRank& kMatchRank );
    void FindWinRecordRankUseNickNameHOF( IN const char& cCharType, IN const std::wstring& strNickName, OUT KMatchRank& kMatchRank );

    void DeleteRemoveStrengthItem( IN const std::wstring& strLogin, IN const char cCharType_, IN const DWORD& dwUID, IN std::vector<GCITEMUID>& vecRemoveItem );

    bool Insert_GachaSystem_RewardLog_ToDB( IN const int nVersion_, IN const DWORD dwLoginUID_, IN const char cSelectedCharType_, IN const int nSelectedCharLevel_, IN const int nSelectedItemLevel_, IN const GCITEMID dwRewardItemID_, IN const int nRewardInfo_, IN const int nRewardItemCount_, IN const int nDecreaseCount_ );
    void GetUserInfo_AccAttendance( IN const DWORD& dwUserUID, IN const int& nVersion, OUT int& nAccumulateCount, OUT time_t& tmLastAttendanceTime );
    bool UpdateAttendance_AccAttendance( IN const DWORD& dwUserUID, IN const int& nVersion, IN const int& nDiff, IN time_t& tmLastAttendanceTime );
    void GetUserRewardStateList_AccAttendance( IN const DWORD& dwUserUID, IN const int& nVersion, OUT std::vector<int>& vecRewardCompleteDay );
    int UpdateUserRewardStateList_AccAttendance( IN const DWORD& dwUserUID, IN const int& nVersion, IN int& nRewardGetDay, IN GCITEMID& dwItemID );
    int InsertPvPBalancePromotion( IN const DWORD& dwUID, IN const char& cCharType, IN const char& cPromotion, IN const DWORD& dwLevel, IN const int& nModeID, IN const int& nPlayTime, IN const int& nResult, IN const int& nLifeCount );
    void InsertPvPBalanceSkillUseCount( IN const DWORD& dwUID, IN const int& nUID, IN const std::map<int,int>& mapSkillUseCount );
    void InsertPvPBalanceEquipSkillSlot( IN const DWORD& dwUID, IN const int& nUID, IN const std::map<int,int>& mapSkillSlotInfo );

	void GetUserData_ErrandEvent( IN const DWORD& dwUserUID, IN const int& nVersion, OUT KErrandUserInfoDataStruct& kPacket );
    int UpdateUserData_ErrandEvent( IN const DWORD& dwUserUID, IN const int& nVersion, IN const KErrandUserInfoDataStruct& kData );
    void GetUserReceivedSpecialRewardInfo_ErrandEvent( IN const DWORD& dwUserUID, IN const int& nVersion, OUT std::vector<PAIR_INT>& vecRecvedSpecialReward );
    int UpdateUserReceivedSpecialRewardInfo_ErrandEvent( IN const DWORD& dwUserUID, IN const int& nVersion, IN const GCITEMID& dwItemID, IN const int& nCount );

    void GetPetDefaultInfo( IN const DWORD& dwUserUID, IN const char& cCharType, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo );
    bool InsertKairoRewardInfo( IN const DWORD& dwUserUID, IN const GCITEMID& ReqItemID, IN const DWORD& dwType, IN KDropItemInfo& kDropItemInfo );
    bool InsertReturnUserInfo( IN const int& nEventType, IN const int& nVersion, IN const DWORD& dwUserUID );
    bool CheckReturnUser( IN const int& nEventType, IN const int& nVersion, IN const DWORD& dwUserUID );

    bool UpdateCharLookItemInvenSize( IN const DWORD& dwUserUID, IN const char& cCharType, IN const DWORD& dwSlotNum, IN const int& nExtendSize );
    bool Update_DungeonSubject_Stat_ToDB( IN KDungeonSubjectStat& kPacket_ );	

    void GetSystemGuideUserData( IN const DWORD& dwUserUID, OUT std::map< int, KGuideCompleteInfo >& mapComplete );
    int UpdateSystemGuideUserData( IN const DWORD& dwUserUID, IN int& nGuideType, IN int& nProvideFlag, IN int& nCompleteFlag );
    void GetUserTutorialEnable( IN const DWORD& dwUID, IN const int& nSessionInfo, IN const time_t& tmFirstLoginTime, IN const int& nEventType, OUT bool& bTutorialEnable );
    void MonsterCardMigration( IN const DWORD& dwUserUID );
    void GetAttributeMigrationInfo( IN const DWORD& dwUID, OUT std::map<GCITEMUID, DWORD>& mapAttributeMigrationItemInfo );

    bool CheckCrashBefore( IN const std::wstring& strIP_, IN const USHORT usPort_ );
    bool InsertDungeonLeaveInfo( IN const KEVENT_STAT_DUNGEON_LEAVE_NOT& kDBPacket );
    bool InsertUserPlayTime( IN const KEVENT_STAT_USER_PLAY_TIME_NOT& kDBPacket );
    bool InsertLevelGPInfo( IN const KEVENT_STAT_LEVEL_GP_INFO_NOT& kDBPacket );
    bool UseCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_ );
    bool AddCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_ );
    int UpdateCharInvenCapacity( IN const DWORD& dwUID, IN const char& cCharType, IN const DWORD& dwSlowNum, IN const int& nInvenCapacity );
    bool Update_VitalitySystem_UserData_ToDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, IN const KVitalityUpdateData& kVitalityUpdateData_ );
    bool Get_VitalitySystem_RechargePoint_FromDB( IN const DWORD dwLoginUID_, OUT int& nRechargePoint_, OUT int& nRechargePointCounter_ );
    bool Get_VitalitySystem_RemainVitality_FromDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, OUT std::map< int, int >& mapRemainVitality_ );
    bool Get_VitalitySystem_LastResetTime_FromDB( IN const DWORD dwLoginUID_, OUT time_t& tmLastInitialize_ );
    bool CheckNeedVitalityReset( IN const time_t tmLastInitialize_ );
    bool Initialize_VitalitySystem_RechargePoint( OUT int& RechargePoint_, OUT int& nRechargePointCounter_ );
    bool Initialize_VitalitySystem_RemainVitality( IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, OUT std::map< int, int >& mapRemainVitality_ );
    bool Update_VitalitySystem_RechargePoint_ToDB( IN const DWORD dwLoginUID_, IN const int nRechargePoint_, IN int nRechargePointCounter_ );
    bool Update_VitalitySystem_RemainVitality_ToDB( IN const DWORD dwLoginUID_, IN const std::map< char, KCharacterInfo >& mapCharacterInfo_, IN const std::map< int, int >& mapRemainVitality_, IN const int nType_ );
    bool Update_VitalitySystem_RemainVitality_Character_ToDB( IN const DWORD dwLoginUID_, IN const KCharacterInfo& kCharacterInfo_, IN const int nRemainVitality_, IN const int nType_ );
    bool Update_VitalitySystem_LastResetTime_ToDB( IN const DWORD dwLoginUID_ );
    bool VitalitySystemDungeonPlayStat_insert( IN const KEVENT_STAT_VITALITY_DUNGEON_PLAY_NOT& kPacket_ );

    int CheckCollectionInfo( IN const DWORD& dwUID, IN const char& cCharType, IN const int& nMainMissionID );
    bool SetTodayCharEventDoneDiff( IN const DWORD dwUID, IN const char& cCharType, IN const int nEventType );
    int GetTodayCharEventCount( IN const DWORD dwUID, IN const char& cCharType, IN const int nEventType );

    void GetEquipPetCostume( IN const GCITEMUID& PetUID, OUT GCITEMUID& CostumeUID, OUT GCITEMID& CostumeID );
    void GetCharEquipPetCostume( IN const DWORD& dwUserUID, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo );
    void GetCharPetInfo( IN const std::wstring strLogin, IN const DWORD dwUID, IN const char& cCharType, OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo, IN OUT std::vector<KItem>& vecInv );
    void UpdateEquipItem( IN const std::wstring& strLogin, IN const DWORD& dwUserUID, IN const char& cCharType, IN const GCITEMUID& ItemUID, IN const GCITEMID& ItemID, IN const int& nSlotIndex, IN const char& cItemType, OUT std::vector< KUserEquipItem >& vecUserEquipItem, IN int nType );
    void UpdateUserCollection( IN const DWORD& dwUserUID, IN const char& cCharType, IN const int& nMainMissionID, IN const int& nSubMissionID, IN const int& nDiff );

    int CheckReserveNewUID( IN const DWORD& dwUID_, IN const char cCharType_, const std::vector<KItem>& vecItem_, IN int nType, OUT std::vector<KItem>& vecNewUIDItem_ );
    int SetNewUID( IN const std::vector<KItem>& vecNewUIDItem_, OUT std::vector<KItem>& vecInvenItem_ );
    void UpdateCharLastLoginDate( IN const DWORD& dwUserUID, IN const char& cCharType, IN const DWORD& dwSlotNum );
    int UpdateCharQuickSlotDelete( IN const DWORD& dwUserUID, IN const char& cCharType );
    void UpdateCharQuickSlotMerge( IN const DWORD& dwUserUID, IN const char& cCharType, IN const int& nItemOrderNo, IN const GCITEMUID& ItemUID, IN const GCITEMID& ItemID );
    void SelectCharQuickSlotInfo( IN const DWORD& dwUserUID, IN const char& cCharType, IN OUT std::vector< KSlotData >& vecItemSlot, IN OUT std::vector< KSlotData >& vecEmoticonSlot );
    bool UpdateUserPointBagAccTime( IN const DWORD& dwUID, IN const char& cCharType, IN const int& nDiffGCPoint, IN const int& nAccTime, IN std::wstring& wstrDate );
    int CheckPointBagItemInfo( IN const DWORD& dwUID, IN const char& cCharType, IN const GCITEMID& ItemID );
    bool GetPointBagItemInfo( IN const GCITEMID& ItemID, OUT int& nPeriod, OUT int& nCount, OUT int& nPrice );
    bool CheckPlayMainMission( IN const DWORD& dwUID, IN const char& cCharType, IN const DWORD& dwMainMissionID );
    bool CheckCompleteMainMission( IN const DWORD& dwUID, IN const char& cCharType, IN const DWORD& dwMainMissionID );
    int InsertMainMission( IN const DWORD& dwUID, IN const char& cCharType, IN const DWORD& dwMainMissionID, IN const int& nLimitDay );
    bool InsertSubMission( IN const DWORD& dwUID, IN const char& cCharType, IN const DWORD& dwMainMissionID, IN const int& nSubMissionID );
    int InsertDungeonClearInfo( IN const DWORD& dwUID, IN const char& cCharType, IN const int& nDungeonID, IN const int& nDifficult );
    int UpdateDefaultBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const int nBaseBonusDiff_ );
    int UpdateSpecialBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const int nSpecialBonusDiff_ );
    bool UpdateBonusLife( IN const DWORD dwUID_, IN const char cCharType_, IN const int nSlotID_, IN const KBonusPointInfo& kBonusLifeDiff_ );

    bool Update_ConnectionGiftBox_UserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ );
    bool Update_ConnectionGiftBox_EventUserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const int nVersion_, IN const int nCurrentGrade_ );
    bool Get_ConnectionGiftBox_UserData_FromDB( IN const DWORD dwUID_, IN const int nCharType_, IN OUT KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ );
    void CheckConnectionGiftBoxNewUser( IN const DWORD dwUID_, IN const time_t tmFirstLoginTime_, OUT bool& bConnectionGiftBoxNewUser_ );

    bool SendUpgradeGuildGradeGiftNot( IN const DWORD dwUID_, IN const char cCharType_, IN const DWORD dwGuildUID_ );

    bool SetJumpingCharInfo( IN const std::wstring& strLogin, IN const DWORD& dwUID, IN const char& cCharType, IN KCharacterInfo& kCharacterInfo );
    void SetJumpingCharLevelPromotion( IN const DWORD& dwLevel, IN const char& cPromotion, IN OUT KCharacterInfo& kCharacterInfo );
    void SetJumpingCharInitLevel( IN OUT KCharacterInfo& kCharacterInfo );
    bool InsertJumpingCharLog( IN const DWORD& dwUID, IN const int& nType, IN const char cCharType, IN const DWORD dwBeforeLevel, IN const DWORD dwCurrentLevel );
    void GetEventDungeonDropRatio( IN const DWORD dwUID, OUT sTimeEvent& sTimeEvent );
    bool InsertUserRelaySendCount( IN const DWORD& dwUID, IN KEVENT_USER_RELAY_SEND_COUNT_REQ& kPacket );

    bool InsertProcessCPUMemoryInfo( IN const DWORD& dwServerGroup, IN const DWORD& dwServerUniqueNum, IN const int& nCPUUsage, IN const KProcessMemoryInfo& kProcessMemoryInfo );
    bool InsertSystemHDDInfo( IN const DWORD& dwServerGroup, IN const DWORD& dwServerUniqueNum, IN const std::vector<KSystemHDDInfo>& vecSystemHDDInfo );
    bool InsertSystemNetworkInfo( IN const DWORD& dwServerGroup, IN const DWORD& dwServerUniqueNum, IN const std::map<std::wstring, __int64>& mapNetworkInfo );
    void GetVirtualDepotItemInfo( IN const DWORD& dwUID, OUT std::vector<KItem>& vecVDepotItemList );
    int UpdateCountItemAtVirtualDepot( IN const DWORD& dwUID, OUT const GCITEMUID& ItemUID, IN const int& nDiff, IN const int& nDelstate );
    void GetTCPServerList( OUT std::vector<KSimpleServerInfo>& vecUseTCPServerList, OUT std::vector<KSimpleServerInfo>& vecNoUseTCPServerList );

    void GetAllSellCount_SocialCommerce( OUT std::vector<PAIR_DWORD_INT>& vecPrBuyCount );
    int UpdateSellCount_SocialCommerce( IN const DWORD& dwUserUID, IN const DWORD& dwEventUID, IN const int& nSellCountDiff );
    bool InsertGroupPurchaseUser_SocialCommerce( IN const DWORD& dwEventUID, IN const DWORD& dwUserUID );
    void UpdateBuyLog_SocialCommerce( IN const DWORD& dwEventUID, IN const int& nType, IN const DWORD& dwUserUID, IN const GCITEMID& dwItemID );
    bool InsertUserUDPPortInfo( IN const KDB_EVENT_USER_UDP_PORT_INFO_REQ& vecList );
    bool CheckItemDuration( IN const DWORD& dwUID, IN const GCITEMUID& ItemUID, IN const int& nInitCount, IN const int& nCount );
 void GetUserControlRecordData( IN const DWORD dwUID, IN const char& cCharType, IN OUT std::map<int,KDungeonUserInfo>& mapList );
    bool UpdateUserControlRecordData( IN const DWORD dwUID, IN const char& cCharType, IN const int& nModeID, IN const SHORT shDifficulty,
        IN KUsersDungeonRecordData& kRecordData );

    bool InsertPresentPost( IN const std::wstring strSendNickName, IN const DWORD dwReceiveUID, IN const KDB_EVENT_BUY_VIRTUAL_CASH_REQ& kPacket );

    bool Update_InfinityDungeon_Stats_ToDB( IN const KDB_EVENT_INFINITY_DUNGEON_STAT_NOT& kDBPacket_ );

    void GetFinishedMissionList( IN const DWORD dwUID, IN const char& cCharType, OUT std::vector< DWORD >& vecFinishedMission );
    bool InsertFinishedMission( IN const DWORD dwUID, IN const char& cCharType, IN const DWORD& dwMissionID );

    bool Get_DungeonPersonalRecord_UserData( IN const DWORD dwUID_, IN const char cCharType_ );
    bool Get_DungeonPersonalRecord_UserData_FromDB( IN const DWORD dwUID_, IN const int nCharType_, OUT std::map< int, std::map< int, KDungeonPersonalRecord > >& mapDungeonPersonalRecord_ );
    bool Update_DungeonPersonalRecord_UserData_ToDB( IN const DWORD dwUID_, IN const int nCharType_, IN const std::map< int, std::map< int, KDungeonPersonalRecord > >& mapDungeonPersonalRecordDiff_ );
    bool Get_DungeonRank_PreviousSeason_UserRank( IN const DWORD dwUID_ );
    bool Get_DungeonRank_PreviousSeason_UserRank_FromDB( IN const DWORD dwUID_, IN const std::set< int >& setRankingModes_, OUT std::map< PAIR_CHAR_INT, KDungeonRankInfo >& mapPreviousDungeonRankPersonal_ );

    bool Insert_PvPPlayCharactersCount_ToDB( IN const KEVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT& kPacket_ );

    bool UpdateGachaPonLog(IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMachineNumber, IN const DWORD& MainRewardItem, IN const DWORD& dwRewardItemID, IN const int& nCapsule, IN const int& nCapsulesInMachine);

	bool GetPetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, OUT std::vector<KPetGlyphCharData>& vecPetGlyph );
	bool EquipPetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, IN const GCITEMUID dwItemUID, IN const GCITEMID dwItemID, IN const int iGlyphType);
	bool RemovePetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, IN const GCITEMUID dwItemUID );

    bool GetCoordiViewerCharSlotItems(IN const int iLoginUID, IN const int iCharType, OUT std::map< int, std::vector< GCITEMID > >& m_mapSlotInfo);
    bool SetCoordiViewerCharSlotItems(IN const int iLoginUID, IN const int iSlotID, IN const int iCharType, IN std::vector< GCITEMID >& m_vecItemList, OUT int iOK);

    void UpdateBillboardLog(IN const DWORD& dwUID_, IN const std::wstring& strNickname_, IN const std::wstring& strMessage_);

    bool UpdateUserHeroDungeonTicket( IN const DWORD dwUserUID, IN const char cCharType_, IN const int nModeID );

    // #END_OF_NORMAL_FUNC
protected:

    enum
    {
        GC_NICKNAME_SIZE        = 24,
        PACKAGE_ITEM            = 1,            //  090319  woosh. 가상 캐쉬 패키지 아이템 구매불가에 사용
    };

   _INT_DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( EVENT_REGISTER_NICKNAME_REQ, std::wstring );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_AGREEMENT_PRIVATEINFO_NOT );
   _INT_DECL_ON_FUNC( EVENT_STRONG_LEVELUP_REQ, KDB_EVENT_STRONG_LEVELUP_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_GP_REQ, KDB_EVENT_BUY_FOR_GP_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_GEM_REQ, KDB_EVENT_BUY_FOR_GEM_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_REQ, KDB_EVENT_BUY_FOR_CRYSTAL_REQ );
   //_INT_DECL_ON_FUNC( EVENT_BUY_COUNT_FOR_GP_REQ, KDB_EVENT_BUY_COUNT_FOR_GP_REQ );

    INT_DECL_ON_FUNC_NOPARAM( EVENT_QUERY_INVENTORY_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_SELL_ITEM_REQ, KDB_EVENT_SELL_ITEM_INFO );
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_USER_DATA_REQ, KUserData );
    INT_DECL_ON_FUNC( DB_EVENT_EXIT_GAME_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_WRITE_MODE_COUNT_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_INSERT_STATISTICS_NOT );
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_GUILD_POINT_REQ, KGuildGameResult );
   _INT_DECL_ON_FUNC( DB_EVENT_INSERT_DROP_ITEM_REQ, KInsertDropItem );
   //_INT_DECL_ON_FUNC( EVENT_GUILD_EDIT_NOTICE_REQ, KGuildNoticeToEdit );
    //050530. microcat.
   _INT_DECL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_REQ, KAddItemInfo );
   _INT_DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_REQ, KAddItemInfo );
   _INT_DECL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_REQ, KAddItemInfo );
   _INT_DECL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_REQ, KAddItemInfo );
    INT_DECL_ON_FUNC( EVENT_REMOVE_ITEM_REQ );
   _INT_DECL_ON_FUNC( EVENT_QUERY_USER_INFO_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_REQ, KPacket2Name );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_QUERY_ITEM_INFO_REQ );
    //INT_DECL_ON_FUNC( EVENT_CHANGE_USER_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_QUERY_USER_INFO2_REQ, KPacket2Name );
   _INT_DECL_ON_FUNC( EVENT_REMOVE_USER_INFO_REQ, KPacket2Name );
    // 051003. florist
   _INT_DECL_ON_FUNC( DB_EVENT_INSERT_P2PFAILRATE_DATA, std::vector<int> );
   _INT_DECL_ON_FUNC( DB_EVENT_CHECK_EXPIRATION_ITEM_REQ, std::vector<KItem> );
   _INT_DECL_ON_FUNC( DB_EVENT_INSERT_CLIENTERR_DATA, std::vector<int> );
    INT_DECL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_REQ );
    INT_DECL_ON_FUNC(EVENT_USE_CHANGE_NICKCOLOR_REQ);
    // 060414. kkurrung. 미션 관련
   _INT_DECL_ON_FUNC( EVENT_REGIST_MISSION_REQ, KDB_EVENT_REGIST_MISSION_REQ );
   _INT_DECL_ON_FUNC( EVENT_REMOVE_MISSION_REQ, KEVENT_REMOVE_MISSION );
    INT_DECL_ON_FUNC( EVENT_COMPLETE_MISSION_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_DUNGEON_MISSION_REGIST_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_FINISHED_MISSION_LIST_REQ );
    INT_DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_TIME_LIMIT_EVENT_DATA );
    INT_DECL_ON_FUNC( DB_EVENT_CREATE_INDIGO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_UPDATE_INDIGO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_CHECK_OPEN_TIME_REQ );
    INT_DECL_ON_FUNC( EVENT_SHANGHAI_DROP_REQ );
   _INT_DECL_ON_FUNC( EVENT_SHANGHAI_CHECK_REQ, DWORD );
   _INT_DECL_ON_FUNC( EVENT_RECOMMENDER_REQ, KPacketNameOK );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_ACCUMULRATION_TIME_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_KAIRO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_KAIRO_ONESHOT_REQ );
   // 2007/1/13. iridology. 펫 관련
   _INT_DECL_ON_FUNC( EVENT_CREATE_PET_REQ, KDB_EVENT_CREATE_PET );
   _INT_DECL_ON_FUNC( EVENT_DELETE_PET_REQ, KDB_EVENT_DELETE_PET );
    INT_DECL_ON_FUNC( EVENT_CHANGE_PET_NAME_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_LOUD_MSG );
   _INT_DECL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_REQ, KExtendItem );
    INT_DECL_ON_FUNC( EVENT_STAT_USER_HISTORY_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_CLIENT_INFO ); // 클라이언트 사양 조사.
    INT_DECL_ON_FUNC( EVENT_DB_REPETITION_PROVIDE_REQ ); // 한시간 이벤트
   _INT_DECL_ON_FUNC( EVENT_TRANSFORMATION_PET_REQ, KEVENT_DB_TRANSFORMATION_PET_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_LOAD_POINTSYSTEM_INFO_REQ );
    // calender and GCPoint system
   _INT_DECL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_REQ, std::wstring);
   _INT_DECL_ON_FUNC( EVENT_LOAD_NEW_CALENDAR_NOT, std::vector<KDailyInfo> );
    //INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_MONTHLY_REWARD_ITEM ); // 달 개근 하면 얻어 내는 아이템 리스트
   _INT_DECL_ON_FUNC( EVENT_MONTHLY_ATTEND_NOT, KDailyInfo );
    INT_DECL_ON_FUNC( DB_EVENT_MONTHLY_ATTEND_REWARD_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT );
    // 펫 부화
   _INT_DECL_ON_FUNC( EVENT_HATCHING_EGG_REQ, KEVENT_DB_HATCHING_EGG_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_TIME_SYNCRONIZE_NOT );
    // 포인트로 아이템을...
   _INT_DECL_ON_FUNC( EVENT_BUY_GCPOINT_ITEM_REQ, KDB_EVENT_BUY_GCPOINT_ITEM_REQ );
   _INT_DECL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_REQ, KBuySellItemReq );
   _INT_DECL_ON_FUNC( EVENT_RESET_WIN_LOSE_REQ, KResetWinLoseData );
    // 운수 좋은 날
    INT_DECL_ON_FUNC( EVENT_LUCKYDAY_NOT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_LUCKYDAY_LIST );
    INT_DECL_ON_FUNC( EVENT_STAT_PROMOTION_COUNT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_YAHOO_BENEFIT_NOT );
   _INT_DECL_ON_FUNC( EVENT_DB_STAT_LOADING_TIME_NOT, KEVENT_MAP_DWORD_DWORD );
   _INT_DECL_ON_FUNC( EVENT_INSERT_HACK_INFO_NOT, int );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GET_NASTY_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_REPORT_USER_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GET_CHATTING_EVENT_MSG_REQ );
   _INT_DECL_ON_FUNC( EVENT_ENCHANT_REQ, KDB_EVENT_ENCHANT_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_CASH_ERROR_REPORT, KDB_EVENT_CASH_ERROR_REPORT );
    INT_DECL_ON_FUNC( DB_EVENT_BONUS_POINT_REFILL_REQ );
    INT_DECL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_SLOT_INFO_NOT );
   _INT_DECL_ON_FUNC( EVENT_STAT_CPL_INSERT, MAP_CPL_STAT );
   _INT_DECL_ON_FUNC( EVENT_STAT_PLAY_TIME_INSERT, MAP_PLAY_TIME_STAT );
   _INT_DECL_ON_FUNC( EVENT_STAT_MAP_COUNT, MAP_INT_DWORD );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_INDIGO_RANK_NOT );
    INT_DECL_ON_FUNC( EVENT_NEW_CHARACTER_REQ );
   _INT_DECL_ON_FUNC( EVENT_TW_USER_AGREEMENT_REQ, int );
    INT_DECL_ON_FUNC( EVENT_EMOTICON_MIX_REQ );
   _INT_DECL_ON_FUNC( EVENT_STAT_GET_ITEM, MAP_ITEM_STAT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CONNECTION_MSG_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DB_UPDATE_RANK_DATA_REQ );
   _INT_DECL_ON_FUNC( EVENT_DB_MY_RANK_INFO_REQ, std::set<char> );
    INT_DECL_ON_FUNC( EVENT_RANK_SEARCH_REQ );
   _INT_DECL_ON_FUNC( EVENT_GET_HELL_ITEM_REQ, int );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DB_UPDATE_SELL_ITEM_LIST_REQ );
   _INT_DECL_ON_FUNC( EVENT_SELL_COUNT_ITEM_REQ, KDBSellCountItemData );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CURRENT_VIRTUAL_CASH_NOT );
   _INT_DECL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_REQ, KDB_EVENT_BUY_VIRTUAL_CASH_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_SET_ABUSE_TO_DB_REQ );
   _INT_DECL_ON_FUNC( EVENT_DB_STAT_GAME_LOADING_TIME, KEVENT_MAP_DWORD_DWORD );
   _INT_DECL_ON_FUNC( EVENT_COLLECTION_COMPLETE_REQ, int );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_MINIGAME_INFO_NOT );
    INT_DECL_ON_FUNC( EVENT_DB_UPDATE_MINIGAME_SCORE );
   _INT_DECL_ON_FUNC( EVENT_STAT_MINIGAME_INSERT, MAP_MINIGAME_STAT );
   _INT_DECL_ON_FUNC( EVENT_UNLOCK_SKILL_REQ, KDB_EVENT_UNLOCK_SKILL_REQ );
   _INT_DECL_ON_FUNC( EVENT_STAT_SPX_COUNT, MAP_SPX_STAT );
   _INT_DECL_ON_FUNC( EVENT_CHANGE_COUPON_REQ, KEVENT_CHANGE_COUPON_ACK );
   _INT_DECL_ON_FUNC( EVENT_STAT_DISCONNECT_COUNT, MAP_INT_DWORD );
   _INT_DECL_ON_FUNC( EVENT_GACHA_ACTION_REQ, KDB_EVENT_GACHA_ACTION_REQ );
    INT_DECL_ON_FUNC( EVENT_GACHA_SET_REWARD_REQ );
   _INT_DECL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_REQ, KEVENT_GACHA_SET_REWARD_REQ );
   _INT_DECL_ON_FUNC( EVENT_STAT_LEAVE_GAME_COUNT, KEVENT_MAP_STAT_LEAVE_GAME );
   _INT_DECL_ON_FUNC( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_REQ, PAIR_INT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_PERIOD_NOTICE );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_FULL_COUPLE_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_MAKE_COUPLE_REQ, KMakeCoupleDBData_Req );
   _INT_DECL_ON_FUNC( EVENT_COUPLE_REQ_DENY_SET, DWORD );
   _INT_DECL_ON_FUNC( EVENT_USE_COUPLE_ITEM_REQ, KCoupleItemData );
    INT_DECL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_REQ );
   _INT_DECL_ON_FUNC( EVENT_DIVORCE_COUPLE_REQ, KDiborceDBData );
   _INT_DECL_ON_FUNC( EVENT_COUPLEITEM_EXPIRED_NOT, std::vector<KSimpleItem> );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GET_USER_DONATION_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_DONATION_POINT_REQ );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM );
    INT_DECL_ON_FUNC( DB_EVENT_WEEKLY_ATTEND_REWARD_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CHECK_LUNA_NEWYEAR_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_LUNA_NEWYEAR_REQ );
   _INT_DECL_ON_FUNC( EVENT_CHAR_POLL_UPDATE_DB, MAP_CHAR_INT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DB_GACHA_COIN_COUNT_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_VIP_REQ, KDB_EVENT_BUY_FOR_VIP_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CHECK_GACHA_REEL_POINT_REQ );
   _INT_DECL_ON_FUNC( EVENT_GACHA_REEL_POINT_UPDATE_DB, GAGHA_REEL_POINT );
    INT_DECL_ON_FUNC( DB_EVENT_OPEN_CAPSULE_REQ );
   _INT_DECL_ON_FUNC( EVENT_UPDATE_GAME_COUPON_FROM_DB, int );
    INT_DECL_ON_FUNC( EVENT_USE_GAME_COUPON_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_FAIL_NOT );

    // 09.04.10. kkurrung. 추천 (5th)
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CHECK_RECOMMEND_ENABLE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_RECOMMEND_USER_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_RECOMMEND_FULL_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_REQ, std::vector<DWORD> );
   _INT_DECL_ON_FUNC( EVENT_ATTENDANCE_BONUS_REQ, int );
   _INT_DECL_ON_FUNC( EVENT_RECOM_DAILY_ATTENDANCE_REQ, bool );
   _INT_DECL_ON_FUNC( EVENT_RECOMMENDER_DENY_REQ, std::vector<DWORD> );

   _INT_DECL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, KEVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SANTA_DATA );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_VIRTUAL_CASH_RESTRICT_ITEM_LIST );


    INT_DECL_ON_FUNC( DB_EVENT_DUNGEON_HACKING_USER_NOT );
   _INT_DECL_ON_FUNC( EVENT_GET_GCCLUB_CONNECT_BONUS_REQ, std::vector<KDropItemInfo> );
   _INT_DECL_ON_FUNC( EVENT_GET_GCCLUB_ATTEND_BONUS_REQ, std::vector<KDropItemInfo> );
   _INT_DECL_ON_FUNC( EVENT_MISSION_PACK_REGIST_REQ, KEVENT_MISSION_PACK_REGIST_ACK );

   // 090519 tgkwon Client ping 통계 남기기
   _INT_DECL_ON_FUNC( EVENT_STAT_PING_INSERT_DB, MAP_PING_STAT );
    INT_DECL_ON_FUNC( EVENT_RAINBOW_JACKPOT_ITEM_REQ );
    INT_DECL_ON_FUNC( EVENT_EFFECT_TEX_REPORT );
   _INT_DECL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_REQ, KCoopEventItem );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GET_USER_HACKING_COUNT_REQ );
    INT_DECL_ON_FUNC( EVENT_ITEM_TRADE_REQ );
    INT_DECL_ON_FUNC( EVENT_MISSION_ITEM_NOT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_MY_SUBSCRIPTION_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_SUBSCRIPT_REQ );
    INT_DECL_ON_FUNC( EVENT_NPC_GIFTS_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GET_USER_BOARD_GAME_INFO );
    INT_DECL_ON_FUNC( EVENT_DB_DICE_PLAY_REQ );
    INT_DECL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_REQ );
    INT_DECL_ON_FUNC( EVENT_DB_DONATION_ITEM_GET_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DONATION_INFO_REQ );
  
    // 091118. kkurrung. 퀴즈 빙고
    INT_DECL_ON_FUNC_NOPARAM( EVENT_USER_BINGO_DATA_REQ );
   _INT_DECL_ON_FUNC( EVENT_CHANGE_BINGO_COIN_REQ, int );
   _INT_DECL_ON_FUNC( EVENT_BINGO_QUESTION_REQ, int );
   _INT_DECL_ON_FUNC( EVENT_BINGO_ANSWER_REQ, KBingoAnswerReqForDB );
    INT_DECL_ON_FUNC( EVENT_DB_BINGO_ACC_TIME_UPDATE );

    // 대만 대회참가승리팀 예상 기부
    INT_DECL_ON_FUNC( EVENT_TOURNAMENT_DONATION_REQ );
   _INT_DECL_ON_FUNC( EVENT_ITEM_BUY_CHECK_REQ, GCITEMID );
   
    INT_DECL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_REQ );
   _INT_DECL_ON_FUNC( EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, MAP_AD_COUNT );

    // 091210. kkurrung. GEC Event
    INT_DECL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_GWC_EVENT_DONATION_REQ, int );

    INT_DECL_ON_FUNC( EVENT_SEARCH_GUILD_LIST_REQ );
    INT_DECL_ON_FUNC( EVENT_MARK_UPLOAD_REQ );
    INT_DECL_ON_FUNC( EVENT_CREATE_GUILD_REQ );
   _INT_DECL_ON_FUNC( EVENT_EDIT_GUILD_NOTICE_REQ, KNGuildNoticeList );
    INT_DECL_ON_FUNC( EVENT_EDIT_GUILD_MY_COMMENT_REQ );
    INT_DECL_ON_FUNC( EVENT_EDIT_GUILD_NAME_REQ );

   _INT_DECL_ON_FUNC( EVENT_EDIT_GUILD_URL_REQ, KNGuildURL );
    INT_DECL_ON_FUNC( EVENT_GUILD_MEMBER_LIST_REQ );
   _INT_DECL_ON_FUNC( EVENT_UPGRADE_GUILD_GRADE_GIFT_NOT, KECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
   _INT_DECL_ON_FUNC( EVENT_CHECK_UPGRADE_GUILD_GRADE_GIFT_NOT, DWORD );
   _INT_DECL_ON_FUNC( EVENT_GET_GUILD_INFO_FROM_DB_REQ, DWORD );
   _INT_DECL_ON_FUNC( EVENT_GET_GUILD_MEMBER_LIST_FROM_DB_REQ, DWORD );
    // 091230 tgkwon. 새해 소원 성취 이벤트
    INT_DECL_ON_FUNC( EVENT_NEW_YEAR_DONATION_REQ );
   _INT_DECL_ON_FUNC( EVENT_SOCKET_OPEN_REQ, KEVENT_SOCKET_OPEN_ACK );
   _INT_DECL_ON_FUNC( EVENT_MONSTER_CARD_INSERT_REQ, KEVENT_MONSTER_CARD_INSERT_ACK );
   _INT_DECL_ON_FUNC( EVENT_MONSTER_CARD_REMOVE_REQ, KEVENT_MONSTER_CARD_REMOVE_ACK );
    INT_DECL_ON_FUNC( EVENT_ATTRIBUTE_SELECT_REQ  );
    // #END_OF_DECL

    INT_DECL_ON_FUNC( DB_EVENT_RECOMMEND_LEVELUP_NOT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_RECOMMEND_LEVELUP_CHECK );
    // 100118 tgkwon. 주사위 이벤트 구조 변경
    INT_DECL_ON_FUNC( DB_EVENT_INIT_DICE_PLAY_REQ );

    INT_DECL_ON_FUNC( EVENT_ITEM_BREAKUP_REQ );
    // 시간단위 아이템 드랍
    INT_DECL_ON_FUNC( DB_EVENT_UPDATE_TIME_DROP_NOT );
   _INT_DECL_ON_FUNC( DB_EVENT_TIME_DROP_REQ, KTimeDropItem );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_GET_TIME_DROP_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_GET_WEB_CASH_ITEM_REQ, int );
    INT_DECL_ON_FUNC( EVENT_STAT_RESOLUTION_NOT );
    INT_DECL_ON_FUNC( EVENT_MANUFACTURES3_REQ );
    // 100223. tgkwon. 선물상자 이벤트
    INT_DECL_ON_FUNC( EVENT_PRESENTBOX_TRADE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_PRESENTBOX_ACTION_REQ );
    // 100310 tgkwon. 구매제한 아이템 Count 증가.
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_BUY_CHECK_COUNT_REQ, GCITEMID );

    INT_DECL_ON_FUNC( DB_EVENT_GUILD_PAGE_BUILD_NOT );
   _INT_DECL_ON_FUNC( EVENT_GUILD_LIST_REQ, KGuildPageBuild );
    INT_DECL_ON_FUNC( DB_EVENT_ONE_GUILD_PAGE_BUILD_NOT );
   _INT_DECL_ON_FUNC( EVENT_GUIDE_BOOL_CHECKPOINT_NOT, PAIR_SHORT );
    INT_DECL_ON_FUNC( DB_EVENT_UPDATE_DUNGEON_STAT_NOT );
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_GUILD_PAGE_INFO_NOT, DWORD );

   _INT_DECL_ON_FUNC( EVENT_CHANGE_NORMAL_TO_LOOK_REQ, KDB_EVENT_CHANGE_NORMAL_TO_LOOK_REQ );
   _INT_DECL_ON_FUNC( EVENT_CHANGE_LOOK_TO_NORMAL_REQ, KChangeLookItem );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_FULL_LOOK_INFO_REQ );

    INT_DECL_ON_FUNC( DB_EVENT_BUNDLE_SELL_ITEM_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_GAMBLE_REQ, KDB_EVENT_BUY_FOR_GAMBLE_REQ );

    INT_DECL_ON_FUNC( DB_CHANNELING_EVENT_CHECK );
    INT_DECL_ON_FUNC( DB_EVENT_CHAR_PROMOTION_UPDATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_USER_HERO_DUNGEON_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_USER_HERO_DUNGEON_INFO_UPDATE_NOT );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_HERO_REQ, KDB_EVENT_BUY_FOR_HERO_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_STAT_LOADING_INFO, KLoadingInfo );
    INT_DECL_ON_FUNC( EVENT_EXP_UPDATE_TO_DB_NOT );
    INT_DECL_ON_FUNC( EVENT_PVEXP_UPDATE_TO_DB_NOT );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_GAWIBAWIBO_INFO_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INFO_UPDATE, KUserGawibawiboInfo );
   _INT_DECL_ON_FUNC( DB_EVENT_GAWIBAWIBO_INIT_INFO_REQ, KUserGawibawiboInfo );

   _INT_DECL_ON_FUNC( EVENT_STAT_LOGINOUT_COUNT, KCountInfo );
    INT_DECL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SOCKS_INFO_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_SOCKS_INFO_UPDATE, KUserSocksInfo );
   _INT_DECL_ON_FUNC( DB_EVENT_SOCKS_HANGUP_REQ, KUserSocksInfo );
   _INT_DECL_ON_FUNC( DB_EVENT_SOCKS_COLLECT_REQ, KUserSocksInfo );
   _INT_DECL_ON_FUNC( DB_EVENT_SOCKS_INSERT_ITEM, KEVENT_SOCKS_MATERIAL_EXCHANGE_NOT );
   _INT_DECL_ON_FUNC( DB_EVENT_SOCKS_INSERT_ITEM_NEW, KSocksExchangeData );

   _INT_DECL_ON_FUNC( EVENT_SPECIAL_REWARD_REQ, KRewardInfoList );
   _INT_DECL_ON_FUNC( EVENT_DUNGEON_REWARD_ITEM_REQ, KDB_EVENT_DUNGEON_REWARD_ITEM_REQ );

   _INT_DECL_ON_FUNC( EVENT_CHAT_NOT, KChatData );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_INIT_DICE_DAILY_FREE );
    INT_DECL_ON_FUNC( DB_EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_GUILD_REQ, KDB_EVENT_BUY_FOR_GUILD_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_GUILD_BATTLE_POINT_REQ, KGuildGameResult );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_TODAYS_POPUP_INFO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DISABLE_TODAYS_POPUP_NOT );

   _INT_DECL_ON_FUNC( EVENT_WHISPER_REQ, KChatData );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_FASHION_CATALOG_REQ );
   _INT_DECL_ON_FUNC( EVENT_USER_SERVER_ROOM_REQ, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_USER_RECOM_TYPE_REQ, int );
    INT_DECL_ON_FUNC( DB_EVENT_UPDATE_CHARACTER_STAT_NOT );

    // 110222. tgkwon. 나무나무 이벤트.
    INT_DECL_ON_FUNC_NOPARAM( EVENT_PLANT_TREE_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_PLANT_TREE_ACTION_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_PLANT_TREE_REWARD_REQ );

    INT_DECL_ON_FUNC_NOPARAM( DB_UPDATE_GUILD_LIST_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_SQUARE_LIST_REQ );
   _INT_DECL_ON_FUNC( EVENT_CHOICE_BOX_OPEN_REQ, KDB_EVENT_CHOICE_BOX_OPEN_REQ );

   _INT_DECL_ON_FUNC( EVENT_UNLOCK_CHANGE_WEAPON_REQ, char );
    INT_DECL_ON_FUNC( EVENT_USER_AUTH_CHECK_REQ );
    INT_DECL_ON_FUNC( EVENT_ITEM_LEVEL_SELECT_REQ );
    INT_DECL_ON_FUNC( EVENT_RKTORNADO_ACTION_REQ );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_VIPEVENT_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_VIPEVENT_USER_INFO_REQ, USHORT );
   _INT_DECL_ON_FUNC( DB_EVENT_VIPEVENT_USER_REWARD_UPDATE_NOT, time_t );

   _INT_DECL_ON_FUNC( DB_EVENT_MAGIC_BOX_LIST_NOT, USHORT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_MAGIC_BOX_LIST_REQ );
   _INT_DECL_ON_FUNC( EVENT_SET_MAGIC_BOX_TARGET_REQ, GCITEMID );
   _INT_DECL_ON_FUNC( EVENT_USE_MAGIC_BOX_REQ, KDB_EVENT_USE_MAGIC_BOX_REQ );

    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_GET_PACKAGE_LIST_NOT );
    INT_DECL_ON_FUNC( EVENT_OPEN_CALENDAR_BONUS_POINT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_USER_SPHINX_DATA_REQ ); // 스핑크스 OX 이벤트.
    INT_DECL_ON_FUNC( EVENT_SPHINX_QUESTION_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SPHINX_ANSWER_REQ );
   _INT_DECL_ON_FUNC( EVENT_DUNGEON_STAT_INFO, KDungeonStatAddA );

   _INT_DECL_ON_FUNC( DB_EVENT_SQUARE_STAT_TIME, DWORD );
    INT_DECL_ON_FUNC( EVENT_STRENGTH_EQUIP_REQ );
    INT_DECL_ON_FUNC( EVENT_STRENGTH_ACTION_REQ );
    INT_DECL_ON_FUNC( EVENT_STRENGTH_BREAKUP_REQ );
    INT_DECL_ON_FUNC( EVENT_STRENGTH_COMPOSE_REQ );


   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_AGIT_REQ, KDB_EVENT_BUY_FOR_AGIT_REQ );
   _INT_DECL_ON_FUNC( EVENT_SELL_FOR_AGIT_REQ, KDB_EVENT_SELL_FOR_AGIT_REQ );
   _INT_DECL_ON_FUNC( EVENT_HARVEST_FLOWER_POT_NOT, KHarvestFlowerPotAck );
   _INT_DECL_ON_FUNC( EVENT_AGIT_STAY_BONUS_DROP_REQ, KDropItemInfo );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_TODAYS_SEED_DROP_COUNT_REQ );
   _INT_DECL_ON_FUNC( EVENT_BUY_FOR_BROWNIE_REQ, KDB_EVENT_BUY_FOR_BROWNIE_REQ );
   _INT_DECL_ON_FUNC( EVENT_AGIT_INIT_SEED_REQ, KDropItemInfo );
   _INT_DECL_ON_FUNC( EVENT_DROP_FLOWER_POT_REQ, KAddItemReq );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_AGIT_TUTORIAL_DONE_REQ );
   _INT_DECL_ON_FUNC( EVENT_AGIT_TUTORIAL_REQ, DROP_ITEMS );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_FAIRY_TREE_BUFF_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_AGIT_USER_OPTION_REQ );

    INT_DECL_ON_FUNC( EVENT_ENTER_AGIT_NOT );
    INT_DECL_ON_FUNC( EVENT_BUY_AGIT_MAP_NOT );
    INT_DECL_ON_FUNC( EVENT_NMNETCAFE_BONUS_REQ );

    INT_DECL_ON_FUNC( EVENT_STAT_CHAR_EQUIP_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_DUNGEON_STAGE_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_CHAR_LV_PT_NOT );

    INT_DECL_ON_FUNC( DB_EVENT_SURVEY_ANSWER_REQ );
   _INT_DECL_ON_FUNC( EVENT_SEALLING_CALENDAR_REQ, KEVENT_SEALLING_CALENDAR_ACK );
   _INT_DECL_ON_FUNC( DB_EVENT_GWC_RANKING_REWARD_REQ, VECTOR_STRUCT_DROPITEM );
   
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CHARISMAS_EVENT_INFO_REQ );
   _INT_DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, std::wstring );
    INT_DECL_ON_FUNC( DB_EVENT_CHARISMAS_EVENT_PRESENT_REQ );
    INT_DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_DONATION_REQ );
    INT_DECL_ON_FUNC( EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ );

    INT_DECL_ON_FUNC( DB_EVENT_JUST_IN_TIME_REWARD_REQ );

    INT_DECL_ON_FUNC( EVENT_STAT_HOTKEY_USED_INFO_NOT );
   
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_INIT_POST_LETTER_LIST_REQ );
   _INT_DECL_ON_FUNC( EVENT_SEND_LETTER_REQ, KDB_EVENT_SEND_LETTER_REQ );
   _INT_DECL_ON_FUNC( EVENT_READ_LETTER_NOT, POSTUID );
    INT_DECL_ON_FUNC( EVENT_GET_ITEM_FROM_LETTER_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_DELETE_LETTER_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_NEW_POST_LETTER_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_DUNGEON_CLEAR_REWARD_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_UPDATE_DUNGEON_CLEAR_INFO_NOT );
   _INT_DECL_ON_FUNC( EVENT_EQUIP_LEVEL_DOWN_REQ, KDB_EVENT_EQUIP_LEVEL_DOWN_REQ );
    INT_DECL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DEPOT_INFO_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_UPDATE_DEPOT_DATA_REQ, KDepotUpdateData );
   _INT_DECL_ON_FUNC( DB_EVENT_DELETE_DEPOT_DATA_REQ, KDepotUpdateData );
   
   _INT_DECL_ON_FUNC( EVENT_DEPOT_EXTEND_REQ, KDepotExtend );
   _INT_DECL_ON_FUNC( EVENT_DEPOT_CREATE_REQ, KDepotInfo );

    INT_DECL_ON_FUNC( DB_EVENT_ECLIPSE_COLLECT_REWARD_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_ECLIPSE_COLLECT_REWARD_CHECK_REQ, time_t );

    INT_DECL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_ECLIPSE_PLOT_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ );

    INT_DECL_ON_FUNC( EVENT_MONSTER_CARD_MIX_REQ );
    INT_DECL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_REQ );


    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SELECT_GACHA_NOTICE_POPUP_INFO );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_UPDATE_GACHA_POPUP_INFO );
    INT_DECL_ON_FUNC( EVENT_STAT_DUNGEON_CHARACTER_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_USER_DUNGEON_STAT_NOT );
    _INT_DECL_ON_FUNC( EVENT_STAT_CHARACTER_SKILL_COUNT, KEVENT_STAT_CHARACTER_SKILL_COUNT );

    _INT_DECL_ON_FUNC( DB_EVENT_SONGKRAN_USER_INFO_REQ, bool );
    _INT_DECL_ON_FUNC( EVENT_SONGKRAN_GRADE_REWARD_REQ, KDB_EVENT_SONGKRAN_GRADE_REWARD_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SONGKRAN_EVENT_UPDATE_WATERBOMB_DURATION_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SONGKRAN_EVENT_WATERBOMB_SUPPLY_REQ );
    _INT_DECL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ, KItem );
    INT_DECL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_USE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SONGKRAN_WATERBOMB_NOT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_ATTEND_GET_CHAR_REQ );
    _INT_DECL_ON_FUNC( EVENT_STAT_VIEW_GRADE_TIME_NOT, KUserViewGradeTime );

    INT_DECL_ON_FUNC_NOPARAM( EVENT_LOGIN_TIME_UPDATE_REQ );   

    _INT_DECL_ON_FUNC( DB_EVENT_ADVENTURE_DATA_REQ, int );
    INT_DECL_ON_FUNC( EVENT_ADVENTURE_CONTINENT_REWARD_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ADVENTURE_FINAL_REWARD_REQ );
    INT_DECL_ON_FUNC( EVENT_ADVENTURE_USER_COLLECT_DATA_NOT );

    _INT_DECL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_INFO_REQ, DWORD );
    INT_DECL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ );

    INT_DECL_ON_FUNC( DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ );

    INT_DECL_ON_FUNC( EVENT_TEST_ADD_ITEM_REQ );
    INT_DECL_ON_FUNC( EVENT_NEW_CHAR_CHOICE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SEALED_CHARACTER_UPDATE_NOT );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_OLYMPIC_JOIN_GAME_REQ );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_OLYMPIC_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_OLYMPIC_REWARD_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_USER_DUNGEON_LOADING_INFO, KUserLoadingStat );
    INT_DECL_ON_FUNC( DB_EVENT_USER_CHANGE_CHAR_STAT );
    INT_DECL_ON_FUNC( DB_EVENT_USER_MODE_CLEAR_STAT );
    INT_DECL_ON_FUNC( DB_EVENT_ROOM_MONSTER_KILL_STAT );
    INT_DECL_ON_FUNC( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ITEM_COMPOSE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_GACHA_LOTTERY_ACTION_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SKILL_OPENKEY_USE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_GP_ATTRIBUTE_RANDOM_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SKILL_MISSION_INFO_NOT );
   _INT_DECL_ON_FUNC( DB_EVENT_MONSTER_TYPE_STAT_INFO, KMonsterTypeStat );
    INT_DECL_ON_FUNC( DB_EVENT_MATCH_CHAR_DATA_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_DEL_MATCH_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_MATCH_ELO_RESULT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_HERO_ITEM_UPGRADE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_USER_INFO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_RITAS_CHRISTMAS_UPDATE_CONTINUE_COIN_NOT, int );
   _INT_DECL_ON_FUNC( DB_EVENT_RITAS_CHRISTMAS_UPDATE_RECORD_NOT, int );
    INT_DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ );
   _INT_DECL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ, KItem );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_MATCH_RANK_DATA_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_MY_MATCH_RANK_INFO_REQ, std::set<char> );
    INT_DECL_ON_FUNC( EVENT_MATCH_RANK_SEARCH_REQ );
   _INT_DECL_ON_FUNC( EVENT_UPDATE_ITEM_COUNT_NOT, std::vector< KItem > );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_BUFF_DONATION_LOGIN_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_BUFF_DONATION_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_ACCUMULATE_ATTENDANCE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_PVP_BALANCE_STAT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_ERRAND_USER_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_ERRAND_REWARD_REQ );
    INT_DECL_ON_FUNC( EVENT_INIT_ITEM_ATTRIBUTE_REQ );
    INT_DECL_ON_FUNC( EVENT_LOOK_INVENTORY_EXTEND_REQ );
   _INT_DECL_ON_FUNC( EVENT_DUNGEON_SUBJECT_REWARD_REQ, KEVENT_DUNGEON_SUBJECT_REWARD_ACK );
   _INT_DECL_ON_FUNC( EVENT_STAT_DUNGEON_SUBJECT_NOT, KDungeonSubjectStat );
    INT_DECL_ON_FUNC( DB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ );	
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SYSTEM_GUIDE_COMPLETE_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SYSTEM_GUIDE_ITEM_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SYSTEM_GUIDE_COMPLETE_REQ );
   _INT_DECL_ON_FUNC( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT, time_t );
    INT_DECL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_USER_ATTRIBUTE_MIGRATION_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_STAT_DUNGEON_LEAVE_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_USER_PLAY_TIME_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_LEVEL_GP_INFO_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_ADD_SLOT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_CHANGE_CHARACTER_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_VITALITY_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_VITALITY_RECHARGE_NOT );
    INT_DECL_ON_FUNC( EVENT_STAT_VITALITY_DUNGEON_PLAY_NOT );
    INT_DECL_ON_FUNC( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_JUMPING_CHAR_INFO_NOT );
   _INT_DECL_ON_FUNC( EVENT_JUMPING_CHAR_REWARD_REQ, VECTOR_STRUCT_DROPITEM );
    INT_DECL_ON_FUNC( DB_EVENT_JUMPING_CHAR_REQ );
    INT_DECL_ON_FUNC( EVENT_INSERT_DUNGEON_HACK_NOT );
   _INT_DECL_ON_FUNC( EVENT_SKILL_TRAINING_REQ, int );
    INT_DECL_ON_FUNC( EVENT_USER_RELAY_SEND_COUNT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SYSTEM_RESOURCE_INFO );
    INT_DECL_ON_FUNC( DB_EVENT_COORDI_COMPOSE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_VIRTUAL_DEPOT_ITEM_LIST_REQ );
    INT_DECL_ON_FUNC( EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_CLIENT_NETWORK_REPORT_NOT, MAP_INT_INT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_TONG_DONATION_TOTAL_AMOUNT_INFO_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_TONG_DONATION_USER_AMOUNT_INFO_REQ );
    INT_DECL_ON_FUNC( EVENT_TONG_DONATION_DONATE_REQ );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SOSCIAL_COMMERCE_BUY_COUNT_INFO );
    INT_DECL_ON_FUNC( DB_EVENT_SOSCIAL_COMMERCE_BUY_REQ );
   _INT_DECL_ON_FUNC( DB_EVENT_SOSCIAL_COMMERCE_SELL_STATE_FLAG_UPDATE_REQ, PAIR_DWORD_INT );
    INT_DECL_ON_FUNC( DB_EVENT_USER_UDP_PORT_INFO_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_COORDI_GRADE_UPGRADE_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ );
   _INT_DECL_ON_FUNC( EVENT_NICKNAME_VALIDITY_CHECK_REQ, std::wstring );
    INT_DECL_ON_FUNC( DB_EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_INFINITY_DUNGEON_STAT_NOT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_SHANGHAI_MISSION_INFO_NOT );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );
    INT_DECL_ON_FUNC_NOPARAM( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ );
    INT_DECL_ON_FUNC( EVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT );
    INT_DECL_ON_FUNC( DB_EVENT_GACHA_PONG_ACTION_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_BILLBOARD_CHAT_MSG_LOG_REQ );
    INT_DECL_ON_FUNC( EVENT_COORDIVIEWER_GETSLOT_REQ );
    INT_DECL_ON_FUNC( EVENT_COORDIVIEWER_SETSLOT_REQ );
    INT_DECL_ON_FUNC( DB_EVENT_USE_HERO_TICKET_REQ );

   // #END_OF_GSDBTHREAD
   // file:./GSDBThread.cpp#END_OF_CASE_GSDBTHREAD
};
