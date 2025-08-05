#pragma once
#include "KNCSingleton.h"
#include "Packet.h"
#include "ChannelType.h"
////#include <vector>
////#include <map>


struct SBanInfo
{
    enum
    {
        MAX_BANINFO_NUM = 5,
    };

    EGCGameModeCategory emGameCategory;
    USHORT              usBannedRoomID;
    DWORD               dwBannedTime;

    SBanInfo()
        : emGameCategory(GC_GMC_INVALID)
        , usBannedRoomID(0)
        , dwBannedTime(0)
    {
    }
};

struct KGCGameStage
{
    EGCGameStage                        m_eGameStage;           //게임 지도, 맵.
    int                                 m_nSubGameMode;         //아템전인가, 아닌가 또는 퀘스트 난이도

    KGCGameStage()
        : m_eGameStage(GC_GS_FOREST_OF_ELF)
        , m_nSubGameMode(GC_SGM_INVALID)
    {}

    KGCGameStage(const KGCGameStage& arg_)
    {
        m_eGameStage = arg_.m_eGameStage;
        m_nSubGameMode = arg_.m_nSubGameMode;
    }

    const KGCGameStage& operator = (const KGCGameStage& arg_)
    {
        m_eGameStage = arg_.m_eGameStage;
        m_nSubGameMode = arg_.m_nSubGameMode;
        return *this;
    }


};

struct KGCGameMode
{
    EGCGameMode                         m_eGameMode;            //구체적인 모드
    std::vector<KGCGameStage>           m_vecGameStage;

    KGCGameMode()
        : m_eGameMode(GC_GM_INVALID)
    {}

    KGCGameMode(const KGCGameMode& arg_)
    {
        m_eGameMode = arg_.m_eGameMode;
        m_vecGameStage = arg_.m_vecGameStage;
    }

    const KGCGameMode& operator = (const KGCGameMode& arg_)
    {
        m_eGameMode = arg_.m_eGameMode;
        m_vecGameStage = arg_.m_vecGameStage;
        return *this;
    }

};

struct KGCCategoryInfo
{
    EGCGameModeCategory                         m_eGameModeCategory;            //구체적인 모드
    std::vector<KGCGameStage>           m_vecGameStage;

    bool bRandomMap;

    KGCCategoryInfo()
        : m_eGameModeCategory(GC_GMC_INVALID)
    {}

    KGCCategoryInfo(const KGCCategoryInfo& arg_)
    {
        m_eGameModeCategory = arg_.m_eGameModeCategory;
        m_vecGameStage = arg_.m_vecGameStage;
        bRandomMap = arg_.bRandomMap;
    }

    const KGCCategoryInfo& operator = (const KGCCategoryInfo& arg_)
    {
        m_eGameModeCategory = arg_.m_eGameModeCategory;
        m_vecGameStage = arg_.m_vecGameStage;
        bRandomMap = arg_.bRandomMap;
        return *this;
    }

};

struct KGCGameModeCategory
{
    EGCGameModeCategory                 m_eGameModeCategory;    //대전인지, 퀘스트인지..
    bool                                m_bRandomMap;           //랜덤맵이 가능한가.
    std::vector<KGCGameMode>            m_vecGameMode;

    KGCGameModeCategory()
        : m_eGameModeCategory(GC_GMC_INVALID)
        , m_bRandomMap(false)
    {}

    KGCGameModeCategory(const KGCGameModeCategory& arg_)
    {
        m_eGameModeCategory = arg_.m_eGameModeCategory;
        m_bRandomMap = arg_.m_bRandomMap;
        m_vecGameMode = arg_.m_vecGameMode;
    }

    const KGCGameModeCategory& operator = (const KGCGameModeCategory& arg_)
    {
        m_eGameModeCategory = arg_.m_eGameModeCategory;
        m_bRandomMap = arg_.m_bRandomMap;
        m_vecGameMode = arg_.m_vecGameMode;
        return *this;
    }

};

struct KGCGameModeSet
{
    std::vector<KGCGameModeCategory>    m_vecGameModeCategory;
};

struct KGCStageSpecification
{
    std::set<EGCGameMode> m_setGameMode;
    std::string m_strFootHoldPostString;
    std::string m_strSkyPostString;
};

class KGCRoomManager
{
    DeclareSingleton(KGCRoomManager);

public:
    KGCRoomManager(void);
    ~KGCRoomManager(void);



    // -------------------------------------------------------------------------------------
    // 룸 리스트에서 사용하는 정보들..
    // -------------------------------------------------------------------------------------
public:
    // 랜덤으로 사용될 방이름입니다.	방과 관련된 내용이라서 여기에 둡니다.
    static std::vector< std::wstring > m_vecDungeonRoomNameList;
    static std::vector< std::wstring > m_vecRandomRoomNameList;
protected:
    int     m_iDropItemGettingRule;	//아이템 획득 방식
    bool    m_bChannelRoomList;
    bool    m_bCreateToJoin;

public:

    bool IsCorrectPassword(const std::wstring& strPassword);
    KInDoorUserInfo CreateIndoorUserInfo();

    bool GetCreateToJoin() const { return m_bCreateToJoin; }
    void SetCreateToJoin(bool val) { m_bCreateToJoin = val; }

    HRESULT CreateRoom(const std::wstring& strRoomName, const std::wstring& strPassword, int iMaxPlayer, bool bIsQuickJoin, KRoomOptions kOptions);
    HRESULT CreateRoomParty(const std::wstring& strRoomName, const std::wstring& strPassword, int iGameMode, int iDungeonLv, EGCGameModeCategory cGameCategory, int nSubGameMode, EGCGameStage gameStage, bool bIsDungeon);
    HRESULT CreateRoomDirect(const EGCGameModeCategory cGameCategory, const EGCGameMode nGameMode);
    HRESULT CheckHeroInfo(const EGCGameModeCategory cGameCategory, const EGCGameMode nGameMode);
    KRoomInfo CreateRoomInfoPartyDungeon(const std::wstring& strRoomName, const std::wstring& strPassword, int iGameMode, int nSubGameMode, int iDungeonLv);
    KRoomInfo CreateRoomInfoPartyMatch(const std::wstring& strRoomName, const std::wstring& strPassword, int iGameMode, EGCGameModeCategory cGameCategory, int nSubGameMode, EGCGameStage gameStage);
    KRoomInfo CreateRoomInfo(const std::wstring& strRoomName, const std::wstring& strPassword, int iMaxPlayer);

    HRESULT CheckCreateRoomError();



    enum
    {
        NOT_ENTER_CONDITION,
        ENTER_POSSIBLE,
        ENTER_IMPOSSIBLE,
    };
    int CheckTicketToDungeonEnter(bool bErrorMsg, int iGameMode_ = -1, bool* pbGoldStage = NULL);
    HRESULT CreateRoomInSquare(const std::wstring& strRoomName,
        const std::wstring& strPassword,
        int iMaxPlayer);
    HRESULT JoinRoom(const UINT uiJoinFlag_, short shID_ = -1, const std::wstring& strPasswd = L"", bool bCenterRoom = false, bool bSendPacket = true);
    HRESULT JoinRoom(const UINT uiJoinFlag_, std::map<DWORD, DWORD> mapJoinOption, short shID_ = -1, const std::wstring& strPasswd = L"", bool bCenterRoom = false, bool bSendPacket = true);

    HRESULT JoinRoomInSquare();

    bool IsChannelRoomList() const { return m_bChannelRoomList; }
    void SetChannelRoomList(bool val) { m_bChannelRoomList = val; }

    int GetDropItemGettingRule() const { return m_iDropItemGettingRule; }
    void SetDropItemGettingRule(int val);
    int	SetDropItemGettingRuleToNext();

    // -------------------------------------------------------------------------------------
    // 룸 안에서 사용하는 정보들..
    // -------------------------------------------------------------------------------------

    void SetRecvRoomInfo(bool bRecv, KRoomInfo kInfo);
    bool GetRecvRoomInfo(KRoomInfo& kInfo);

protected:
    // 모든 게임 모드 정보가 담겨있다
    std::vector<KGCGameModeCategory> m_vecGameModeCategory;
    // 해당 채널 안에서 플레이 할 수 있는 게임 모드들만 들어가 있음
    std::vector<KGCGameModeCategory> m_vecShowGameModeCategory;
    // 채널별 플레이 가능한 카테고리
    std::map< EChannelType, std::vector<EGCGameModeCategory> > m_mapChannelCategory;

    std::map< unsigned char, std::vector<KGCStageSpecification> > m_mapStageSpecification;

    int m_iSelectedModeCategoryIndex;
    int m_iSelectedModeIndex;
    int m_iSelectedStageIndex;
    int m_iSelectedSubMode; //서브 모드는 인덱스가 아니다
    bool m_bRandomMap;

    // 서버에서 준 정보를 가지고 있음.
    std::pair<bool, KRoomInfo> m_prRcvRoomInfo;

    // 방안에 들어왔을 때.. 그 방의 정보들이 담겨있음..
    SRoomInfo m_kRoomInfo;
    SRoomInfo m_kPrevRoomInfo; //서버에서 방 정보 안 바꿔줬을 경우 예전 것으로 유지하기 위해.

    std::vector<EGCGameMode>    m_vecCheckDungeonGameMode;

    boost::circular_buffer<SBanInfo>    m_cvBanInfo;

    std::map<DWORD, DWORD> m_mapJoinOption; // 방 입장 제한 사항.

    std::vector<KGCCategoryInfo> m_vecStoreCategoryInfo; //대전 채널 카테고리 정보를 벡터로 저장 한다.
public:
    //길드 방 정보를 얻어오자
    KRoomInfo m_kGuildRoomInfo;
    std::map< int, EGCGameModeCategory > vecValidCategory;

    bool IsEmptyGameModeCategory(EChannelType type_) { if (m_mapChannelCategory.find(type_) == m_mapChannelCategory.end()) return true; return m_mapChannelCategory[type_].empty(); }

    std::vector<EGCGameMode> GetCheckDungeonGameMode() { return m_vecCheckDungeonGameMode; }
    //GameModeTable.lua 에서 정보들을 읽어온다
    void LoadGameMode();

    //게임 모드를 초기화
    void InitGameMode();
    void InitGameModeDirect(const EGCGameModeCategory eGameModeCategory, const EGCGameMode eGameMode);
    void InitGameModeParty(EGCGameModeCategory eGameModeCategory, EGCGameMode eGameMode, EGCGameStage eGameStage, int nSubGameMode, bool IsDungeon);

    // 채널에 따라서 해당 채널에서 플레이 할 수 있는 게임모드를 업데이트 한다!
   // bCategoryFullMode가 true이면 모든 카테고리로 설정.. 튜토리얼이나 리플레이 때 사용하는듯?
    void UpdateShowGameModeCategory(bool bCategoryFullMode, EChannelType eChannelType);
    void ResetGameMode();       // 현재 선택된 카테고리에 따라서 게임모드를 초기화 해 준다
    void ResetSubGameMode();    // 서브 게임 모드 초기화

    // m_vecShowGameModeCategory의 정보를 얻어온다
    std::vector<KGCGameModeCategory> GetGameModeCategoryVector();
    std::vector<KGCGameMode> GetGameModeVector();
    std::vector<KGCGameStage> GetGameStageVector();

public:
    bool IsAvailableGameMode(EGCGameModeCategory eCategory, EGCGameMode eMode);
    bool IsAvailableCategory(EChannelType eChannelType, EGCGameModeCategory eCategory);

    bool SetGameModeCategory(EGCGameModeCategory eCategory);
    bool SetGameMode(EGCGameMode eMode);
    bool SetGameModeStage(EGCGameStage eStage);
    bool SetSubGameMode(int nSubMode);

    void ChangeGameModeCategory(bool bRightMove);
    void ChangeGameMode(bool bRightMove);
    void ChangeSubGameMode(bool bRightMove);
    void ChangeSubGameModeTower(bool bRightMove);
    void ChangeDungeonGameMode(int nGameLevel);
    void ChangeStage(bool bRightMove);

    EGCGameMode GetStagePlayable(std::map<int, KDungeonUserInfo>& mapStagePlayable);


    void RefreshRandomMap();

    int  GetUserCount() { return m_kRoomInfo.usUsers; }
    DWORD GetHostUID() { return m_kRoomInfo.dwHostUID; }

    inline EGCGameModeCategory  GetGameModeCategory(void) const { return m_kRoomInfo.eGameModeCategory; }
    inline EGCGameMode          GetGameMode(void)         const { return m_kRoomInfo.eGameMode; }
    inline int                  GetSubGameMode(void)      const { return m_kRoomInfo.nSubGameMode; }
    inline int                  GetDungeonLevel(void)     const { return m_kRoomInfo.nDungeonLevel; }
    EGCGameStage                GetGameModeStage();

    inline bool IsGameModeCategory(EGCGameModeCategory eGameModeCategory) { return (m_kRoomInfo.eGameModeCategory == eGameModeCategory); }

    void                        SetDungeonLevel(int iDungeonLevel) { m_kRoomInfo.nDungeonLevel = (char)iDungeonLevel; }

    std::string GetGameModeCategoryStr();
    std::string GetGameModeStr();
    std::string GetSubGameModeStr();
    const char* GetGameModeCategoryCStr() { return GetGameModeCategoryStr().c_str(); }
    const char* GetGameModeCStr() { return GetGameModeStr().c_str(); }
    const char* GetSubGameModeCStr() { return GetSubGameModeStr().c_str(); }

    std::string GetStageFootHoldTextureName(IN int iFieldIndex_, IN bool bSpecification_);
    std::string GetStageSkyTextureName(IN int iSkyID_, IN bool bSpecification_);

    void SetPressureState(DWORD dwUID, KPressureState iPrState);
    void SetIdleState(DWORD dwUID, bool bIdle);
    void PressureUser(DWORD dwUID);
    int GetEmptySlot();
    int GetClosedSlot();
    int GetOpenSlotCount();

    EGCGameModeType GetGameModeType(void) const;

    void UpdateRoomInfo(KRoomInfo* pkRoomInfo_ = NULL);
    void UpdateRoomInfo(KChangeRoomInfo kRoomInfo);
    void UpdateRoomInfo(KRoomOptions kOptions);

    void WritePvPRoomOptions();

    SRoomInfo& GetRoomInfo() { return m_kRoomInfo; }
    SRoomInfo& GetPrevRoomInfo() { return m_kPrevRoomInfo; }
    KRoomInfo& GetGuildroomInfo() { return m_kGuildRoomInfo; }
    USHORT GetRoomID() { return m_kRoomInfo.usRoomID; }

    bool All_Players_Ready(void);
    void Kick_Player(char Num);

    void AddBannedRoom(USHORT usBanRoomID, DWORD dwBanTime);
    bool IsBannedRoom(USHORT usRoomID);
    void GetBannedRoomID(std::vector<USHORT>& vecKickedRoomID);

public:
    void UpdateRoomUI();
    void SendRoomInfo();
    bool CanPlayHellmode();
    // 이게.. 판타스틱 맵을 사용한다.. 라는 의미이긴 한데..
    // 몬스터가 등장하면 판타스틱 맵을 사용하기 때문에..
    // 몬스터 등장하는가 안하는가와 동일하다
    bool IsMonsterGameMode();
    bool IsFatalGameMode(); // 페이탈 게임 모드 ( 한방 매치 )

    std::wstring GetGameModeName(EGCGameMode eGameMode);
    void LoadGameModeFromPacket(const KENU_CLIENT_CONTENTS_OPEN_NOT& kRecv);
    void LoadGameModeFromPacket(KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kRecv);
    bool CheckValidDungeon();

    //영웅던전 최적의 캐릭터를 선택해 준다.
//    void SelectHeroDugeonCharacter();

private:
    bool m_bIsPlayedHeroDungeon;

public:
    void SetPlayedHeroDungeon(bool bCheck_) { m_bIsPlayedHeroDungeon = bCheck_; }
    void SetGuildroomInfo(KRoomInfo kRoominfo) { m_kGuildRoomInfo = kRoominfo; }
    void SetValidCategory();
    bool IsPlayedHeroDungeon() { return m_bIsPlayedHeroDungeon; }
    void LoadStageSpecification();
    bool MoveRoom(USHORT usRoomID, bool bPublic, std::wstring wsRoomPass = L"");

    void InitObserver(DWORD dwUserUID = -1);
    bool IsPlayerObserver(DWORD dwUserUID);
    bool IsObserverLive(DWORD dwUserUID = -1);
public:
    bool IsAngelEggModeAbility();
    bool IsDotaModeAbility();

public:// 점령전 관련
    void ChangeDotaStage(bool bRightMove);
    void ResetGameModeParty(EGCGameModeCategory eGamemodeCategory, EGCGameMode eGameMode, EGCGameStage eGameStage, bool IsDungeon);
    const std::map<int, EGCGameModeCategory>& GetChannelCategory() { return vecValidCategory; }


public:// 던전 입장 조건처리
    std::map<int, KDungeonTicket> m_mapDungeonTicket;
    void SetDungeonTicket(std::map<int, KDungeonTicket>& DungeonTicket) { m_mapDungeonTicket = DungeonTicket; }
    std::map<int, KDungeonTicket>& GetDungeonTicket() { return m_mapDungeonTicket; }
    void SetSubDungeonGameMode(int nSubGameMode);
    bool IsHellMode();
    bool IsBreakMode();
    std::set< EGCGameStage > GetGameModeListInChannel(EChannelType eChannelType);
    bool CanStartAutoMachGame();
    void SetRecvRelayServerStatus(KEVENT_RELAY_SERVER_STATUS_ACK& kRecv);
private:
    time_t m_tmPreQuestionStartAutoGame;

public:
    bool IsNotPetSpecial() { return m_kRoomInfo.m_bDisablePetAttackPvP; }
    bool IsNot4Special() { return m_kRoomInfo.m_bDisableSP4SkillsPvP; }
    bool IsMatchBalance() { return m_kRoomInfo.m_bEnablePvPBallance; }
    bool IsBlockedChar(int iCharType)
    {
        return m_kRoomInfo.m_saAllowedChars[iCharType] == false;
    }

    bool IsBlockedRank(int iGrade)
    {
        if (iGrade < 0)
            return false;

        return m_kRoomInfo.m_saAllowedRankings[iGrade] == false;
    }
};

DefSingletonInline(KGCRoomManager);
