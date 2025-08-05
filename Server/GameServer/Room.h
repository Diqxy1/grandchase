#pragma once
#include "SobManager.h"
#include "FSM/support_FSM.h"
#include "DropSystem.h"
#include "GCEnum.h"
#include "TimeEventManager.h"

SmartPointer( KRoom );
SmartPointer(KUser);
#define MC_MAX_STRING 50
#define MAX_OBSERVER_COUNT 2
// 모드 기본 유저 수.
#define DEFAULT_INDIGO_USER 2
#define DEFAULT_DUNGEON_USER 4
#define DEFAULT_PVP_USER 6

class KRoom : public KSimObject, protected KSobManager<KUserPtr>
{
    DeclareFSM;
    NiDeclareRTTI;
    DeclToStringW;
public:
    struct KAbuseElement : public boost::noncopyable
    {
        CTime       m_time;
        DWORD       m_dwGP;
        __int64     m_biaExp[ GC_CHAR_NUM ];
        CON_COPYCON_ASSIGNOP( KAbuseElement, right )
        {
            m_time  = right.m_time;
            m_dwGP  = right.m_dwGP;
            ::memcpy( m_biaExp, right.m_biaExp, sizeof( m_biaExp ) );
            return *this;
        }

        void Clear()
        {
            m_time  = CTime::GetCurrentTime();
            m_dwGP  = 0;
            ::ZeroMemory( m_biaExp, sizeof(m_biaExp) );
        }
    };

    struct KAbuseData : public boost::noncopyable
    {
        bool            m_bAbuse;
        std::wstring    m_strLogin;
        std::wstring    m_strNickName;
        DWORD           m_dwUID;
        int             m_nCount;
        KAbuseElement   m_aElement[3];  //시간대별 ( 0 = 초기, 1 = 걸렸을때, 2 = 나갈때 )

        // 080509 tslayer.
        std::map<std::wstring, std::pair<std::wstring,int> >                 m_mapPlayerInfo; // 같이 플레이한 유저로그인, 플레이 횟수
        //std::map<int, std::vector<std::wstring> >   m_mapRoomMember; // 그 방에서 몇번째 플레이인지 횟수, 그 방에 있었던 유저로그인(줄이 너무 많이 남는다)

        CON_COPYCON_ASSIGNOP( KAbuseData, right )
        {
            m_bAbuse        = right.m_bAbuse;
            m_strLogin      = right.m_strLogin;
            m_strNickName   = right.m_strNickName;
            m_dwUID         = right.m_dwUID;
            m_nCount        = right.m_nCount;

            m_aElement[0]   = right.m_aElement[0];
            m_aElement[1]   = right.m_aElement[1];
            m_aElement[2]   = right.m_aElement[2];

            m_mapPlayerInfo = right.m_mapPlayerInfo;
            //m_mapRoomMember = right.m_mapRoomMember;
            return *this;
        }

        void Clear()
        {
            m_bAbuse        = false;
            m_strLogin      = L"";
            m_strNickName   = L"";
            m_dwUID         = 0;
            m_nCount        = 0;
            m_aElement[0].Clear();
            m_aElement[1].Clear();
            m_aElement[2].Clear();

            m_mapPlayerInfo.clear();
            //m_mapRoomMember.clear();
        }
    };

public:
    enum {
        LEAVE_SELF_DECISION             = 0,
        LEAVE_BANNED_ON_PLAYING         = 1,
        LEAVE_BANNED_EXCEED_MAXUSER     = 2,
        LEAVE_BANNED_HOST_DECISION      = 3,
        LEAVE_SERVER_FIND_ERR           = 4,
        LEAVE_CLIENT_FIND_ERR           = 5,
        LEAVE_HOST_MIGRATION_FAILED     = 6,
        LEAVE_LOADING_COMPLETE_FAILED   = 7,
        LEAVE_MOVE_AGIT                 = 8,
        NUM_OF_LEAVE_ROOM_REASON,
    };

    enum {
        BROAD_TO_ALL,           // 모든 인원에게 전달
        BROAD_TO_ALL_EX_ME,     // 나를 제외한 모든 인원에게 전달
    };

    enum {
        MI_PVP_1VS1 = 0,
        MI_PVP_2VS2 = 1,
        MI_PVP_3VS3 = 2,
        MI_PVP_MAX,
    };

    enum {
        PT_END_GAME = 0,
        PT_LEAVE_GAME = 1,
        PT_MAX,
    };

    enum {
        MT_MODETYPE_PVP = 0,
        MT_MODETYPE_PVE = 1,
        MT_MODETYPE_MAX,
    };

    KRoom( unsigned short usRoomID );
    virtual ~KRoom(void);

    bool Enter( IN KUser& kUser, IN KInDoorUserInfo& kInDoorUserInfo, IN int& nTeamType );
    bool Leave( KUser& kUser, int nReasonWhy );  // 유저가 방을 나가는 이유를 추가로 받는다.
    bool BanishOther( IN KUser& kRequestUser, IN const std::wstring& strTargetName, IN int nReasonWhy, OUT bool& bIsBanHost );    // 다른 유저를 쫓아낸다.
    size_t GetCount() { return m_mapSob.size(); }   // KSobManager<> 의 함수와 똑같음  
    size_t GetCountExObserver();
    bool IsRoomLastUser(); // 마지막 유저인지 확인.

    void SetRoomInfo( IN KRoomInfo& kInfo, IN char cType, IN int& nModeInfo );
    void ChangeRoomInfo( IN KUser* pkUser, IN OUT KChangeRoomInfo& kInfo ); // SetInfo와 ChangeInfo : 변경하는 데이터 종류/양이 다르다.
    void ChangeRoomExtraOptions(IN KUser* pkUser, IN OUT KRoomOptions& kInfo);
    void GetRoomInfo( OUT KRoomInfo& kInfo );
    unsigned short GetRoomID() { return m_usRoomID; };
    void GetSimpleRoomInfo( OUT KSimpleRoomInfo& kSimpleRoomInfo );
    std::wstring& GetRoomName() { return m_strName; };
    //void GetInfo( OUT KGuildRoomInfo& kInfo );
    void ChangeHostAddress( KUser* pkUser, const std::vector<DWORD>& vecHostIP, const std::vector<USHORT>& vecHostPort );
    KUser* GetHostPtr() { return m_spHostUser.get(); }
    KUser* GetPlayingHost();
    DWORD GetPlayingHostUID();
    void ClearPlayingHost();
    void MigrationForPlaying();
    void RollbackMigrationFromPlaying( DWORD& dwHostUID );
    void SameMigrationForPlaying();

    bool StartGame( IN KUser& kUser );
    bool StartJoinGame( IN KUser& kUser );
    bool EndGame( IN const KEndGameReq& kReq );
    bool LeaveGame( KUser& kUser );
    bool EndResult( KUser& kUser );
    void EndResult();
    DWORD GetLatestPlayTime() const { return m_dwLastGameDuration; }

    bool IsUserJoined( const std::wstring& strLogin ) { return GetByName( strLogin ) != NULL; }

    //GC
    bool DropItem( IN int iEndingMode, IN int iReserve,  OUT std::vector<KDropItemDetail>& vecDropItemDetail );
    bool GetDropMode( IN int iEndingMode, IN int iReserve, OUT KDropMode& kMode );

    KInDoorUserInfo* FindInDoorUserInfo( IN const std::wstring& strName );
    KInDoorUserInfo* FindInDoorUserInfo( IN const DWORD dwUID );
    bool FindInDoorUserInfo( IN const std::wstring& strName, OUT KInDoorUserInfo& kInDoorUserInfo );
    bool FindInDoorUserInfo( IN const DWORD dwUID, OUT KInDoorUserInfo& kInDoorUserInfo );

    const KInDoorUserInfo* GetHostInDoorUserInfo();
    bool Send_ChatMsg( const KChatData& kNot );
    bool ChangeRoomUserInfo( IN OUT KChangeRoomUserInfo& kChangeRoomUserInfo );
    bool ProcChangeRoomUserInfo( IN OUT KInDoorUserInfo& kInDoorUserInfo, IN OUT KChangeRoomUserInfo& kChangeRoomUserInfo );
    void GetChangeRoomUserInfo( IN const KInDoorUserInfo& kInDoorUserInfo, OUT KChangeRoomUserInfo& kChangeRoomUserInfo );
    bool SyncCharInfo( IN std::wstring& strLogin, IN char cCharType = -1 );

    bool AssignInDoorUserInfo( IN KInDoorUserInfo& kInDoorUserInfo );
    void AddInDoorUserInfo( IN KInDoorUserInfo& kInDoorUserInfo, IN int& nTeamType );
    void RemoveInDoorUserInfo( IN const KUser& kUser );
    bool IsNormalUserAllReady(); // 채팅 이벤트를 위해서라도 모든 유저가 레디인지 체크 필요.
    bool IsAdminExist();

    bool TransitionEndGame( IN const KUser& kUser );

    void GetUserUIDList( std::vector<DWORD>& vecUIDs );
    void UpdateCharacterInfo( std::wstring& m_strName, int iGP );

    //펫 관련
    void UpdatePetInfo( std::wstring strLogin, char cChar, KPetInfo& kPetInfo );
    void GetChangeRoomInfo( OUT KChangeRoomInfo& kInfo );
    void ProcessForceEvent( IN OUT KForceChangeRoomUserInfo& kPacket );

    template< typename T >
    bool BroadPacket( int nBroadCastType, std::wstring strReqLogin, unsigned short usEventID, T& kData, bool bCompress = false );
    template< typename T >
    bool BroadPacket( std::vector<DWORD>& vecUser, unsigned short usEventID, T& kData, bool bCompress = false );
    bool BroadID( int nBroadCastType, std::wstring strReqLogin, unsigned short usEventID );

    static void UpdateStatToDB();
    static void InitModeCount();

    bool ChangeRoomOption( IN const KRoomOption& kReq );

    void AddPromotionCount( std::vector< std::pair<char,char> >& vecPromotionCount );

    bool IsMaximumUser(); // 옵저버를 제외 하고 6명이 가득 있는방인지 체크
    
    //채팅 이벤트
    void SetChatEvent( KChatEventInfo& chatEvent ); // 방이 만들어 질때 호출 된다.
    bool GetChatEvent( OUT KChatEventInfo& chatEvent );
    void ClearChatEvent(); // 방에서 유저가 나가거나 레디를 풀때 등등드..
    void ResetChatEvent(); // 방이 Close 될때 호출 된다.
    bool IsChatEventReward(){ return m_bRewardChatEvent; }
    float GetChatEventEXPRatio();
    float GetChatEventGPRatio();
    void RegChatEventCorrect( std::wstring strName, int nUID );
    void CheckEventReward(); // 방원들이 전부 채팅이벤트 조건을 만족하는지 검사 후 플래그 설정
    DWORD GetAllReadyTick() { return m_dwAllReadyTick; }

    // Exp Gp 계산을 위해...
    void GetGameInfo( OUT KGameInfo& kGameInfo );
    int GetNormalUserNum(); // 옵저버가 아닌 유저의 숫자.
    void GetStartingInfo( OUT std::vector<KSimpleInDoorUserInfo>& vecStartingInfo );
    void SetTimeEvent( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList );
    void AddTimeEvent( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList );
    bool GetTimeEvent( OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList );
    void ClearTimeEvent();

    // 드랍 관련
    void SetDropList( IN KDropData& kDropData );
    void GetDropList( OUT KDropData& kDropData );
    void ClearDropInfo();
    void UpdateIndoorUserInfo();
    void SetPartyDropList( IN KDropData& kDropData );

    // GC Club
    void UpdatePremiumData( IN const std::wstring& strLogin, IN const KPremiumInfo& kInfo );
    void GetPremiumInfo( OUT USER_PREMIUM_INFO& mapPremium );

    bool IsShortGame();
    void SetChampInfo( IN std::vector<KChampionInfo>& vecChampInfo );
    void GetChampInfo( OUT std::vector<KChampionInfo>& vecChampInfo );
    void ClearChampInfo();

    void UpdateQuickSlotData();
    void UpdateAuthLevel();

    bool IsPremiumRoom(); // GC 클럽 체크
    bool IsHellMode();
    bool IsBreakMode();
    bool StartHellModeProcess(); // 헬모드 시작 처리..
    bool StartBreakModeProcess(); // 헬모드 시작 처리..
	void GetStartingUser( OUT std::vector<DWORD>& vecStartUser );

	void SetRandomSeed( DWORD dwRandomSeed ) { m_dwRandomSeed = dwRandomSeed; };
	DWORD GetRandomSeed() { return m_dwRandomSeed; };
    void DumpRoomInfo();

    void SetUniqueNumber( DWORD dwUniqueNumber ) { m_dwUniqueNumber = dwUniqueNumber; }
    DWORD GetUniqueNumber() { return m_dwUniqueNumber; }

    bool IsLoading();
    bool IsLeftUser( DWORD dwUserUID );
    void InsertLeftUser( DWORD dwUserUID );
    void ClearLeftRoom();

    void SetPvPEvent( IN const int& nWinTeam, OUT std::vector< std::wstring >& vecStrLogin );

    bool JoinGameEnable();
    void SetDeathScore( KDeathMatchScore& kInfo );
    void GetDeathScore( KDeathMatchScore& kInfo );
    void ClearDeathScore();

    // 스킬 관련
    void UpdateSkillSetInfo( std::wstring& strLogin );
    bool IsSameTeam( DWORD dwUID1, DWORD dwUID2 );

    void DivorceUser( std::wstring& strLogin );

    bool IsBanHost( IN const std::wstring& strTargetName_ );
    bool Send_TeamChatMsg( IN const KChatData& kNot_, IN const int nTeam_ );

    // 09.04.13. kkurrung. 추천(5th)
    void GetTeamInfo( std::vector<std::pair<DWORD,int> >& vecTeamInfo ); // UID:Team

    // 0900701 tgkwon 방장 강퇴 Counting
    void InitBanishCount() { m_dwBanishCount = 3; }; // 방이 생성, 게임이 종료되었을 때 방장 강퇴 Count 초기화
    void DecreaseBanishCount() { m_dwBanishCount--; };     // 유저 강퇴했을 경우 Count 증가
    DWORD GetBanishCount() { return m_dwBanishCount; }; // 현재 방장의 강퇴한 Count 가져오기
    // 유저들의 인벤 정보 가져오기.
    void GetUserInvenInfo( OUT std::map< DWORD, std::pair<int,int> >& mapUsersInvenInfo );
    void UpdateHostOrderByPing();
    void GetPressState( OUT std::vector<std::pair<DWORD, KPressureState> >& vecPressSate_ );
    void GetIdleState( OUT std::vector<std::pair<DWORD, int> >& vecIdleState_ );
    void GetAllMemberPingInfo( OUT std::vector<std::pair<DWORD,DWORD> >& vecPinInfo_ );

    void CheckLoadingCompleteUser( std::wstring LoginID = L"", DWORD UserUID = 0 );

    void SetEventMonInfo( IN std::vector<KEventMonster>& vecList ) { m_vecEventMonster = vecList; }
    void GetEventMonInfo( OUT std::vector<KEventMonster>& vecList ) { vecList = m_vecEventMonster; }
    void ClearEventMonInfo() { m_vecEventMonster.clear(); }

    bool UpdateLookEquip( IN const std::wstring& strLogin_, IN const std::map<char, std::vector<KEquipItemInfo> >& mapLookEqip_ );

    // PlayTime Set, Get func.
    void SetEndGamePlayTime( IN const DWORD dwPlayTime_ ){ m_dwEndGamePlayTime = dwPlayTime_; };
    DWORD GetEndGamePlayTime(){ return m_dwEndGamePlayTime; };
    void SetLeaveGamePlayTime( IN const DWORD dwPlayTime_ ){ m_dwLeaveGamePlayTime = dwPlayTime_; };
    DWORD GetLeaveGamePlayTime() { return m_dwLeaveGamePlayTime; };

    void SetResultGP( IN std::map<DWORD,int>& mapData )     { m_mapResultGP = mapData; }
    void GetResultGP( OUT std::map<DWORD,int>& mapData )    { mapData = m_mapResultGP; }
    void ClearResultGP()                                    { m_mapResultGP.clear(); }

    void SetResultExp( IN std::map< std::pair<DWORD,char>, __int64 >& mapData )     { m_mapResultExp.swap( mapData ); }
    void GetResultExp( OUT std::map< std::pair<DWORD,char>, __int64 >& mapData )    { mapData = m_mapResultExp; }
    void ClearResultExp()                                                           { m_mapResultExp.clear(); }

    DWORD GetRoomPing() { return m_dwHostPing; }

    void CheckStageLoadCompleteUser( IN const std::wstring LoginID, IN const DWORD dwUID );
    void FailStageLoadComplete( std::wstring LoginID_, DWORD UserUID_ );
    void CheckFirstStageLoadComplete();

    void FailLoadingComplete( std::wstring LoginID_, DWORD UserUID_ );
    bool GetStageLoadSwitch() { return m_bStageLoadSwitch; }
    void SetStageLoadSwitch( IN bool bStageLoadSwitch_ ) { m_bStageLoadSwitch = bStageLoadSwitch_; }
    bool FindStageLoadFailUser( DWORD dwUID_ );
    bool IsShortGameByClient();
    DWORD GetClientLatestPlayTime() const { return m_dwClientLastGameDuration; }

    void AddMonDropGP( IN const int& nMonID, IN const int& nTriggerID, IN const DWORD& dwCoinType, IN const DWORD& dwDropGP, IN int nCount = 1 );
    void GetMonDropGP( OUT std::map<PAIR_INT, std::map<PAIR_DWORD,USHORT> >& mapDrop ) { mapDrop = m_mapMonDropGP; }
    void ClearMonDropGP() { m_mapMonDropGP.clear(); }

    bool IsGuildRoomFull( IN const DWORD guildUID_ );
    int GetGuildMemberCount( IN const DWORD guildUID_ );
    bool SetGuildRoomArray( DWORD guildUID_ );
    bool SetGuildMarkName( IN const std::wstring& guildMarkName_ );
    bool SetGuildName( IN const std::wstring& guildName_ );
    bool SetGuildBattlePoint( IN const DWORD guildUID_, IN const DWORD bPoint_ );
    bool FindInDoorUserSameGuild( IN const DWORD dwUID_, IN const DWORD guildUID_ );
    void SetLeaveGuildWhilePlaying( IN const DWORD guildUID_ );
    DWORD GetLeaveGuildWhilePlaying();
    std::wstring GetRoomHostNationCode() { return m_wstrCCode; }
    void SetNationRewardEnable( IN bool bNationReward_ ) { m_bNationReward = bNationReward_; }
    bool GetNationRewardEnable() { return m_bNationReward; }

    void SetDotaInfo( IN KDotaModeInfo& kDotaModeInfo ) { m_kDotaModeInfo = kDotaModeInfo; };

    std::wstring& GetPassword() { return m_strPasswd; };

    void SetLevelUpUser( IN std::map<DWORD, std::pair<int,DWORD>>& mapLevelUpUser ) { m_mapLevelUpUser.swap( mapLevelUpUser ); }
    void GetLevelUpUser( OUT std::map<DWORD, std::pair<int,DWORD>>& mapLevelUpUser ) { mapLevelUpUser = m_mapLevelUpUser; }
    void ClearLevelUpUser() { m_mapLevelUpUser.clear(); }
    bool CheckLevelUpEnable( IN const DWORD& dwUID, OUT int& nCharType, OUT DWORD& dwLevel );

    bool DecreasePvpLife( IN const DWORD& dwUserUID );

    void GetPromotionInfo( OUT MAP_CHAR_JOB& mapPromotionInfo );
    void GetCharLevelInfo( OUT MAP_CHAR_LEVEL& mapCharLevelInfo );
    int GetStartUserNum();
    void SetSpecialMonInfo( IN KSpecialMonster& kSpecialMonster ) { m_kSpecialMonster = kSpecialMonster; }
    void GetSpecialMonInfo( OUT KSpecialMonster& kSpecialMonster ) { kSpecialMonster = m_kSpecialMonster; }
    void UpdateSkillInfo( IN std::wstring& strLogin, IN const char& cCharType );

    void ClearAdventureMissionMonsterData() { m_kAdventureMissionMonsterData.clear(); };
    void SetAdventureMissionMonsterData( IN std::map< int, KAdventureMonster >& kPacket_ ) { m_kAdventureMissionMonsterData = kPacket_; };
    void SetAdventureUserCollectDataPrev();
    int GetFatigueRatio( IN const DWORD dwUID_ );
    void GetFatigueUserList( OUT std::map<DWORD, int>& mapFatigueUser );

    void AddMonsterKillInfo( IN const int& nStageID, IN const int& nMonID );
    void ClearMonsterKillInfo() { m_mapStageMonKillCount.clear(); }
    void GetMonsterKillInfo( OUT std::map<std::pair<int,int>, int>& mapStageMonKillCount ) { mapStageMonKillCount = m_mapStageMonKillCount; }
    void IncreaseMatchFailCount(){ ++m_nMatchFailCount; };
    int GetMatchFailCount(){ return m_nMatchFailCount; };
    void InitMatchFailCount(){ m_nMatchFailCount = 0; };
    bool CheckStartGamePossible();
    bool ELOAvgRPCalc();
    void SetMapID( IN const int& nMapID );
    void GetUserLifeCount( IN const DWORD& dwUserUID, OUT USHORT& usLifeCount );
    void GetTotalUserUIDList( OUT std::vector<DWORD>& vecUIDs );
    void GetModeType( OUT DWORD& dwModeType );

	bool IncreaseMonsterKillCount();
    void ClearMonsterKillCount() { m_nMonsterKillCount = 0; }

    void ClearRemainVitalityOnStartGame() { m_mapRemainVitalityOnStartGame.clear(); };
    bool AddRemainVitalityOnStartGame( IN const DWORD dwLoginUID_, IN const int nCharType_, IN const int nRemainVitality_ );
    bool GetRemainVitalityOnStartGame( IN const DWORD dwLoginUID_, IN const int nCharType_, OUT int& nRemainVitality_ );
    void GetRemainVitalityOnStartGameList( OUT std::map< PAIR_DWORD_INT, int >& mapRemainVitalityOnStartGame_ ) { mapRemainVitalityOnStartGame_ = m_mapRemainVitalityOnStartGame; };

    bool IsCompletedSubjectTrigger( IN const DWORD dwUserUID_, IN const DWORD dwTriggerID_ );
    void AddCompletedSubjectTrigger( IN const DWORD dwUserUID_, IN const DWORD dwTriggerID_ ) { m_mapCompletedSubjectTrigger[ dwUserUID_ ].insert( dwTriggerID_ ); }
    void ClearCompletedSubjectTrigger() { m_mapCompletedSubjectTrigger.clear(); }
    void SetTRServerInfo( IN const DWORD& dwTRelayServerIP, IN const USHORT& usTRelayServerPort );

    void SetPartyRewardInfo( IN std::vector<KDropItemInfo>& vecPartyReward );
    void GetPartyRewardItem( OUT std::vector<KDropItemInfo>& vecPartyReward ) { vecPartyReward = m_vecPartyReward; };

    void SetUserUsePosition( IN const DWORD& dwUID, IN const int& nPositionGroup );
    void GetUserUsePosition( IN const DWORD& dwUID, OUT std::map<int,int>& mapPositionUseCount );
    void ClearUserUsePosition() { m_mapUserPositionUseCount.clear(); }
    void ClearInfinityDungeonInfoOnStartGame();
    DWORD GetIDMonsterLastSummonTime() { return m_dwIDMonsterLastSummonTime; };
    void SetIDMonsterLastSummonTime( IN const DWORD dwIDMonsterLastSummonTime_ ) { m_dwIDMonsterLastSummonTime = dwIDMonsterLastSummonTime_; };
    DWORD GetIDSummonedMonsterLastUID() { return m_dwIDSummonedMonsterLastUID; };
    void SetIDSummonedMonsterLastUID( IN const DWORD dwIDSummonedMonsterLastUID_ ) { m_dwIDSummonedMonsterLastUID = dwIDSummonedMonsterLastUID_; };
    DWORD GetIDReservedDropLastUID() { return m_dwIDReservedDropLastUID; };
    void SetIDReservedDropLastUID( IN const DWORD dwIDReservedDropLastUID_ ) { m_dwIDReservedDropLastUID = dwIDReservedDropLastUID_; };
    void SetIDSummonedMonsters( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_ ) { m_lstIDSummonedMonsters = lstIDSummonedMonsters_; };
    void GetIDSummonedMonsters( OUT std::list< KIDMonsterInfo >& lstIDSummonedMonsters_ ) { lstIDSummonedMonsters_ = m_lstIDSummonedMonsters; };
    void SetIDReservedDrops( IN std::list< KGameDropElement >& lstIDReservedDrops_ ) { m_lstIDReservedDrops = lstIDReservedDrops_; };
    void GetIDReservedDrops( OUT std::list< KGameDropElement >& lstIDReservedDrops_ ) { lstIDReservedDrops_ = m_lstIDReservedDrops; };
    void SetIDReservedGPDrops( IN std::list< KGameDropElement >& lstIDReservedGPDrops_ ) { m_lstIDReservedGPDrops = lstIDReservedGPDrops_; };
    void GetIDReservedGPDrops( OUT std::list< KGameDropElement >& lstIDReservedGPDrops_ ) { lstIDReservedGPDrops_ = m_lstIDReservedGPDrops; };
    void IncreaseIDSummonedMonstersForStats( IN const int nSummonType_ ) { m_mapIDSummonedMonstersForStats[ nSummonType_ ] += 1; };
    void GetIDSummonedMonstersForStats( OUT std::map< int, int >& mapIDSummonedMonstersForStats_ ) { mapIDSummonedMonstersForStats_ = m_mapIDSummonedMonstersForStats; };
    void InitIDPlayTime( IN const std::vector< DWORD >& vecStartingUsers_ );
    void SetIDPlayTime( IN const DWORD dwUserUID_, IN const DWORD dwIDPlayTime_ ) { m_mapIDPlayTime[ dwUserUID_ ] = dwIDPlayTime_; };
    void GetIDPlayTime( OUT std::map< DWORD, DWORD >& mapIDPlayTime_ ) { mapIDPlayTime_ = m_mapIDPlayTime; };

    bool ChangeHost(IN const KUser& kPrevHost, DWORD dwNewHostID);

protected:  // util func
    inline bool CountGameMode( unsigned int uiMode, int nDifficulty );   // static으로 하지 않음. 문제 발생시 방이름 출력.
    inline bool SetNextHost( IN const KUser& kPrevHost );   // 다음 방장을 정하고, 이를 남은 유저에게 전송한다.
    inline bool CloseRoom();
    void RefreshGuildArray();

    void AddAbuseData( IN KUser& kUser );
    void UpdateAbuseData( IN KUser& kUser );
    void RemoveAbuseData( IN KUser& kUser );

    void SetStartingInfo(); // 게임 시작할때 구해놓는 값

    bool PlayableDungeon( IN const int& nGameMode, IN OUT int& nDifficulty ); // 던전 선행 클리어 정보 체크.
    bool IsSinglePlayable();

    void GameStartStateTransition( IN const std::vector<DWORD>& vecUserUIDs ); // 게임 시작하는 유저들의 상태 전이.
    //bool IndigoAvgRPCalc(); // 용사의 섬 제거.

    bool IsDeathMatchReady( OUT std::vector<DWORD>& vecStartUser );
    void GetReadyUserUID( OUT std::vector<DWORD>& vecStartUser );
    int CalcTeam( int nSlotPos );
    int GetEmptySlot( int nTeam, bool bForce = false );
    int GetMinTeam(); // 팀원이 적은 팀.
    int GetPlayingUserCount();
    int GetPCBangUserNum();
    bool StartHeroModeProcess();
    int GetPrevDifficulty( IN const int& nModeID, IN const int& nDifficulty );
    void CheckMonsterCrusaderMonLv( IN const int nModeID, IN const int nTotalUserLevel, IN const int nStartUserNum, OUT int& nMonLv );
    void SetRoomHostPing( DWORD dwHostPing );
    void SetInitStageLoad();
    void SetTickStageLoad();
    
    int GetGuildTeam( DWORD dwGuildUID_ );
    void SetRoomHostNationCode( IN const std::wstring& wstrCCode_ ) { m_wstrCCode = wstrCCode_; }
    void ClearNationReward() { m_bNationReward = false; }
    bool CheckNationCode( IN const std::wstring& wstrHostCCode, IN const std::wstring& wstrUserCCode );

    bool StartDungeonTicketProcess();
    void SetPvpLifeCount( IN const std::vector<DWORD>& vecStartUser );
    void SetDungeonDefaultInfo( IN const int& iGameMode );

public:
    int                         m_nSendResultEndUser;
    DWORD                       m_dwResultTick;
    int                         m_naGuildID[2];//길드전인 경우 두개의 길드만 들어 올 수 있다.
    unsigned short              m_usRoomID;
    unsigned short              m_usMaxUsers;
    bool                        m_abRankBan[PVP_RANK_NUM];
    bool                        m_abCharBan[GC_CHAR_NUM];
    bool                        m_bUsePet;
    bool                        m_bUse4sp;
    bool                        m_bUseBalance;
    bool                        m_bPublic;    // 공개방, 비번방
    std::wstring                m_strPasswd;
    KUserPtr                    m_spPlayingHostUser;
    KUserPtr                    m_spHostUser;
    std::vector<KUserPtr>       m_vecHostOrder;
    std::vector<DWORD>          vecStartUser;
//  bool                        m_bObserverExist;
    int                         m_nObserverCnt;
    int                         m_iP2PVersion;
    char                        m_cGrade;
    char                        m_cGameCategory;
    int                         m_iGameMode;
    int                         m_iSubGameMode;
    int                         m_nDifficulty;
    int                         m_iMapID;
    bool                        m_bRandomableMap;
    int                         m_nMonsterID;       // 몬스터 원정대에 사용.
    int                         m_nMonsterCrusaderMonLv; // 몬스터 원정대. 몬스터 레벨
    int                         m_nMonsterCount;    // 몬스터 원정대에 사용.
    bool                        m_abSlotOpen[MAX_NUM_USER_SLOT]; // User Slot
    char                        m_cRoutingMethod;
    int                         m_nDeathMatchTime;  // 데쓰매치 시간 제한
    int                         m_nDeathKillCount;  // 데쓰매치 목표 Kill
    bool                        m_bJudgePoorP2P;    // p2p 상태 나쁜 유저 제거
    bool                        m_bDeathMatchIntrudable;  // 데쓰매치 방에 난입할수 있다.(게임이 아니라 방)
    bool                        m_bDeathMatchBalancing;   // 데쓰매치 능력치 밸런싱.
    int                         m_nCurrentKill;
    int                         m_nCurrentTime;

    DWORD						m_dwRoomOptions;
    DWORD						m_dwRoomAllowerdChars;
    DWORD						m_dwRoomAllowedRankings;

    std::map<std::wstring, KInDoorUserInfo>  m_mapInDoorUserInfo;
    std::pair<int, int>         m_prAvgRP;

    int                         m_nStartingUserNum;     // 게임 시작 할때 방원 수,옵저버가 있다면 제외 한다.
    __int64                     m_biStartingTotalExp;   // 게임 시작할때 모든 유저의Exp합
    DWORD                       m_dwStartingTotalGp;    // 게임 시작할때 모든 유저의GP합
    int                         m_nTotalLv;             // 게임 시작 할때 모든 캐릭터의 레벨 합.
    char                        m_cChannelType; // 원래라면 룸이 생성될때 알아야 하지만.ㅜㅜ,,, 차후 수정.

    // 050816. jseop. 한 game의 play time 측정용.
    DWORD                   m_dwStartGameTime;
    DWORD                   m_dwLastGameDuration;
    DWORD                   m_dwClientLastGameDuration;
    // 050831. florist. 게임서버를 내리지 않고 릴레이서버를 추가/삭제 가능하도록 서버주소를 방에서 관리.
    DWORD                   m_dwRelayServerIP;
    USHORT                  m_usRelayServerPort;
    // 060301. kkurrung. TR
    DWORD                   m_dwTRServerIP;
    USHORT                  m_usTRServerPort;

    // 채팅 이벤트.
    KChatEventInfo          m_kChatEventInfo; // 이벤트 정보.
    DWORD                   m_dwAllReadyTick; // 모든 유저가 레디한 시각.
    bool                    m_bRewardChatEvent; // 모든 유저가 채팅이벤트 보상받는가(모두 단어 입력했는가)
    std::set< DWORD >       m_setChattedUser; // 채팅 이벤트 수행한 유저들

    DWORD                   m_dwRandomSeed;
    DWORD                   m_dwUniqueNumber;

    mutable KncCriticalSection m_csInDoorUserInfo; // m_mapInDoorUserInfo 보호

    int                     m_iStageClearcount;

    static KDropSystem      ms_kDropSystem;                 ///< do item drop related tasks.
    static KDropSystem* GetDropSystem() { return &ms_kDropSystem; } ///< for luabind.
    static const int        ms_iDifficultyModes[];
    static const wchar_t    ms_szModeName[][MC_MAX_STRING];
    static const int        ms_iDungeonModes[];
    static const int        ms_iPvPModes[];
    static bool IsDifficultyMode( int iMode );
    static int GetMaxDifficulty( int iMode );
    static bool IsDungeonMode( int iMode );
    static bool IsHeroDungeonMode( int iMode_ );
    static KHeroDungeon GetHeroDungeonInfo(int iMode);
    static bool IsPvPMode( int iMode );

    static const int        ms_naMaps[];
    static const wchar_t    ms_szMapName[][MC_MAX_STRING];
    static int GetMapNum();
    int                     m_dwBanishCount; // 방장 강퇴 Count
    std::wstring            m_wstrCCode;     // 방장 국가 정보.

    std::map<DWORD, std::pair<int,DWORD>> m_mapLevelUpUser; // 레벨업한 유저들.
    std::map<DWORD,USHORT>  m_mapLifeCount; // 남은 목숨수 카운트 map[UserUID,LifeCount]
    KDefaultModeInfo        m_kDefaultModeInfo; // 방의 기본 Default 값.

    KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT   m_kAdventureMissionMonsterData;
    std::pair<int, int>     m_prELOTeamRPAvg;

    int                     m_nMonsterKillCount; // 몬스터 Kill Count.

    std::map<DWORD, int> m_mapUserFatigueRatio; // 중국 과몰입 방지 해당 유저의 타입. 해당되는 유저UID, 보상 백분율% ( 0 ~ 100 )

    int                     m_nModeInfo;
    int                     m_nMatchFailCount;

    std::map< DWORD, std::set< DWORD > >    m_mapCompletedSubjectTrigger; // 달성한 과제의 트리거 리스트. map< LoginUID, set< TriggerID > >. 게임 시작시 초기화해야 한다
    std::map< DWORD, std::map<int,int> >    m_mapUserPositionUseCount; // 유저 포션 사용 정보.

    CTime                   m_tmGameStartTime;  // 게임시작한 시간.
    CTime                   m_tmGameEndTime;    // 게임종료한 시간.

protected:
    enum {
        MAX_DIFFICULTY_MONSTER_HUNT = 25,
        MAX_DIFFICULTY_QUEST        = 4, // 헬모드 포함
        MAX_DIFFICULTY_RELAY_HERO   = 30,
    };
    static std::vector< int >           ms_avecModeCount[NUM_GC_GAME_MODE];
    static KEVENT_STAT_PROMOTION_COUNT  ms_mapPromotionCount; // std::map< short, DWORD>
    static unsigned int                 ms_uiUpdateStatTick;

    // Abuser Check;
    std::map<DWORD, KAbuseData>  m_mapAbuseData;

    // Time Event Data
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > m_mapEventList;

    // Drop Data
    KDropData                   m_kDropData;
    std::vector<KChampionInfo>  m_vecChampInfo;
    std::set<DWORD>             m_setUserUID;
    int                         m_nWinTeam;

    DWORD                       m_dwGameStartGap; // 게임 시작 하는데 걸리는 시간 측정을 위한 변수.

    std::vector<KEventMonster>  m_vecEventMonster;

    std::vector<DWORD>          m_vecLoadFailUser; // 로드 컴플릿 시간 초과한 유저

    // PT에 대한 통계를 위한 변수 추가.
    DWORD                       m_dwEndGamePlayTime; // 정상적으로 던전/대전 클리어 시간.(초)
    DWORD                       m_dwLeaveGamePlayTime; // 게임 중간에 나갔을 경우 플레이 시간.(초)

    std::map<DWORD,int>         m_mapResultGP; // UserUID,ResultGP
    std::map< std::pair<DWORD,char>, __int64 > m_mapResultExp; // [ UserUID,CharType], 누적Exp

    DWORD                       m_dwHostPing; // 방장의 핑(평균)

    std::vector<DWORD>          m_vecStageLoadFailUser; // 로드 컴플릿 시간 초과한 유저
    bool                        m_bStageLoadSwitch; // 스테이지 로드 체크(시간) 스위치

    // map< pair<MonID,TriggerID>, map< pair<골드타입,GP>,count> >
    std::map<PAIR_INT, std::map<PAIR_DWORD,USHORT> > m_mapMonDropGP; // 던전내 실제 획득 GP정보

    std::pair<std::wstring,std::wstring> m_pairGuildMarkName; // 길드 마크 이름 ( 양쪽 길드 )
    std::pair<std::wstring,std::wstring> m_pairGuildName; // 길드 이름
    std::pair<DWORD, DWORD> m_pairBattlePoint;

    DWORD m_dwLeaveGuildWhilePlaying; // 게임중 나간 길드 ID
    bool                        m_bNationReward; // 국가 정보 혜택.
    KDotaModeInfo               m_kDotaModeInfo;

    KSpecialMonster             m_kSpecialMonster;
    std::map<std::pair<int,int>, int>     m_mapStageMonKillCount; // map[<Stage,MonID>Count]
    std::map< PAIR_DWORD_INT, int > m_mapRemainVitalityOnStartGame;

    std::vector<KDropItemInfo>  m_vecPartyReward;

    DWORD                           m_dwIDMonsterLastSummonTime; // 무한던전 마지막 몬스터 소환 시간
    DWORD                           m_dwIDSummonedMonsterLastUID; // 무한던전에 소환된 마지막 몬스터 UID(소환될때마다 1씩 순차증가시킴)
    DWORD                           m_dwIDReservedDropLastUID; // 무한던전에 드랍 예정된 마지막 아이템 드랍 UID(소환될때마다 1씩 순차증가시킴), GP와 공용으로 사용함
    std::list< KIDMonsterInfo >     m_lstIDSummonedMonsters; // 무한던전 소환된 몬스터들 정보
    std::list< KGameDropElement >   m_lstIDReservedDrops; // 무한던전 드랍 예정 아이템들 정보(실제로 몬스터를 잡아서 드랍된 아이템인지 판단은 하지 않음)
    std::list< KGameDropElement >   m_lstIDReservedGPDrops; // 무한던전 드랍 예정 GP들 정보(실제로 몬스터를 잡아서 드랍된 GP인지 판단은 하지 않음)
    std::map< int, int >            m_mapIDSummonedMonstersForStats; // 무한던전 통계용 소환된 몬스터 정보
    std::map< DWORD, DWORD >        m_mapIDPlayTime; // 무한던전 통계용 유저별 플레이 시간 정보 (UserUID, PlayTime)
};

template < typename T >
bool KRoom::BroadPacket( int nBroadCastType_, std::wstring strReqLogin_, unsigned short usEventID_, T& kData_, bool bCompress_ /*= false*/ )
{
    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( kData_ );
    ks.EndWriting();

    if( bCompress_ ) spEvent->m_kbuff.Compress();

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    std::wstringstream stm;
    switch( nBroadCastType_ )
    {
    case BROAD_TO_ALL:
        {
            std::map<std::wstring, KUserPtr>::iterator mit;
            KLocker lock( m_csSob );
            for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
            {
                mit->second->SendPacket( kbuff, spEvent->GetEventIDString() );
                stm << mit->second->GetName() << ", ";
            }
        }
        break;
    case BROAD_TO_ALL_EX_ME:
        {     
            std::map<std::wstring, KUserPtr>::iterator mit;
            KLocker lock( m_csSob );
            for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
            {
                if( mit->second->m_strName != strReqLogin_ )
                {
                    mit->second->SendPacket( kbuff, spEvent->GetEventIDString() );
                    stm << mit->second->GetName() << ", ";
                }
            }
        }
        break;
    default:
        START_LOG( cerr, L"지원하지 않는 브로드 캐스트 타입 입니다." )
            << BUILD_LOG( nBroadCastType_ ) << END_LOG;
        return false;

    }

    START_LOG( clog, L"ⓢⓑ " << spEvent->GetEventIDString() << L" (name:" << stm.str() << L")"  ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return true;
}

template< typename T >
bool KRoom::BroadPacket( std::vector<DWORD>& vecUser_, unsigned short usEventID_, T& kData_, bool bCompress_ /*= false*/ )
{
    if( vecUser_.empty() ) return true;

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( kData_ );
    ks.EndWriting();

    if( bCompress_ ) spEvent->m_kbuff.Compress();

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    std::wstringstream stm;
    
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob );
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
    {
        if( std::find( vecUser_.begin(), vecUser_.end(), mit->second->GetUID() ) == vecUser_.end() ) continue;
        mit->second->SendPacket( kbuff, spEvent->GetEventIDString() );
        stm << mit->second->GetName() << ", ";
    }
    START_LOG( clog, L"ⓢⓑ " << spEvent->GetEventIDString() << L" (name:" << stm.str() << L")"  ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return true;
}
