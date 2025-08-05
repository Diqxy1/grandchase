#pragma once

struct KGCActionTrigger
{
    std::string strAIFunc;

    int iAICheckFrame;
    int iDelayFrame;
    int iRemainAICheckFrame;
    int iRemainDelayFrame;
};

struct KGCMonsterAction
{
    KGCMonsterAction()
    {
        Init();
    }

    int         m_iMotionIndex;
    std::string m_strID;
    std::string m_strMotion;
    std::string m_strFunc;
    bool        m_bEnableMoveCheck;
    std::string m_strMoveFailFunc;

    bool        m_bIsFallDownMotion;

    // 트리거!
    std::vector< KGCActionTrigger > m_vecActionTrigger;

    void Init()
    {
        m_strID             = "";
        m_strMotion         = "";
        m_strFunc           = "";
        m_bEnableMoveCheck  = false;
        m_strMoveFailFunc   = "";
        m_bIsFallDownMotion = false;
        m_vecActionTrigger.clear();
    }
};

struct KMonsterActionScript
{
    std::map< std::string, KGCMonsterAction > m_mapActionList;
    std::map< int, std::string > m_mapStateIndex;
    std::map< std::string, std::string > m_mapCallBack;

    KMonsterActionScript()
    {
        m_mapActionList.clear();
        m_mapCallBack.clear();
        m_mapStateIndex.clear();
    }
};

class MONSTER;

typedef std::map<int,MONSTER*> MAP_INT_PMONSTER;


enum ECREATE_MONSTER_TYPE {
    ECREATE_MONSTER_TYPE_NORMAL,
    ECREATE_MONSTER_TYPE_SUMMON,
    ECREATE_MONSTER_TYPE_EVENT,
};

class KGCMonsterManager
{

public:
	enum KMM_MONSTER_TYPE_INDEX {
		KMMTI_SUMMON_MONSTER_START = 100,
        KMMTI_EVENT_MONSTER_START = 200,
	};

	KGCMonsterManager(void);
	~KGCMonsterManager(void);

public:
    void LoadScript();
    void LoadMonsterTemplate( KLuaManager &luaMgr );
    void LoadActionScript( KLuaManager &luaMgr, SMonsterTemplate &kMonsterTemplate );
    void LoadOneMonsterActionScript( const int iMonsterType_ );

public:// 치트용 리로드
    void ReLoadScript();

public:
    void FrameMove();

//몬스터 생성 소멸 관련 함수.
	void CheckAllMonster();
	void CreateAllMonster(int iNum);
	int CreateMonsters( int iNum );
	void CreateMonster( int iIndex );
	int CreateMonster();
	void DeleteAllMonster();
	void DeleteMonsterAtNumber(int index);
    void AllKillMonster();
    void AllKillInfinityNormalMonster( void );

//몬스터 객체 호출
	void RegisterLuabind();
	MONSTER* GetMonster( int iMonsterIndex );
    MONSTER* GetMonsterType( int iMonsterType );
    MONSTER* GetMonsterTypeToLive( int iMonsterType );

//몬스터 확인 함수.
	bool IsMonsterIndex( const int iMonster );
	bool IsMonsterType( const int iType );
	bool IsSummonMonsterByMonster( int iMonster );
	bool IsSummonMonsterByPlayer( int iMonster );
    bool IsSummonMonsterByTrigger( int iMonster );
	bool IsSummonMonster( int iMonster );
    bool IsLiveMonsterToType(int iType);

	void CreateMonsterAttackIndex( OUT std::map<int, int> &mapSaAttackState );
	void CreateMonsterBoolState( OUT std::map<int, bool> &mapBoolState );
    int GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE eMonsterType = ECREATE_MONSTER_TYPE_NORMAL );

	int GetBaseMonsterLevel(int iMonsterType);
	int CheckFreeSpaceMonsterNUM();
	int GetSummonMonsterNum ( int iMonType );
    int GetLiveMonsterNum();
    int GetLiveMonsterNumExceptSummonMonster();
    int GetMonsterHighHPNum( float fHP );
    int GetLiveSameTeamMonsterNum( int iTeam );
    int GetLiveInfinityMonsterTotalNum();
    int GetLiveInfinityMonsterNormalNum();
    int GetLiveInfinityMonsterBossNum();
    int GetSummonMonsterIndex( DWORD dwPlayerUID_, EGCMonster eMonType_ );
	//몬스터 객체 호출
    int GetLiveMonsterToType(int iType);
public:
    void SetMagicEffectToType(int iType, int iBuffID, float fBuffTime, int iBuffLevel);
    void ClearMagicEffectToType(int iType, int iBuffID);
    void SetMonsterStateToType(int iType, char *strState);
    bool SummonMonster( KSummonInfo* pSummonInfo, int iSummonMonsterLevel );
    bool SummonInfinityMonster( KInfinitySummonInfo* pInfinitySummonInfo );

public:
    bool IsLiveBossMonster();

public:	
    std::map< std::string, KGCMonsterAction >& GetMonsterAction(int iMonsterType);
    std::map< int, std::string >& GetMonsterActionIndex(int iMonsterType);
    std::map< std::string, std::string >& GetMonsterCallback(int iMonsterType);
    std::string GetMonsterStateToIndex(int iMonsterType, int iIndex);
    std::string GetMonsterCallbackToStringKey(int iMonsterType, std::string strKey);

private:
	// 방장이 빈슬롯을 찾아서 방원들에게 보내게 되는데... 여러마리가 같이 등장하게 되면.
	// 방장이 같은 슬롯번호로 보내어 문제가 생김으로 누가 먼저 빈슬롯 번호를 가져가서 쓸때
	// 체크를 하게 하여, 중복되지 않도록 함.
	std::set<int> m_setPreCreateBroadMonsterIndex;
	int m_iMaxSpace;

public:
    // 몬스터 인스턴스
    MAP_INT_PMONSTER m_mapMonster;

    // 몬스터 속성 테이블
    std::vector< SMonsterTemplate >             m_vecMonsterTable;

    // 몬스터 액션 테이블
    std::map< int, KMonsterActionScript >       m_mapMonsterAction;

public:
    std::map< std::string, KGCMonsterAction > mapActionNULL;
    std::map< int, std::string > mapStateNULL;
    std::map< std::string, std::string > mapCallbackNULL;
    std::string strNULL;
};

extern KGCMonsterManager g_kMonsterManager;