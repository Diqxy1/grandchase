#ifndef _QUESTGAMEMANAGER_H_
#define _QUESTGAMEMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <d3dx9.h>
//#include <map>
//#include <set>
//#include "GCEnum.h"
//#include "KLuabinder.h"
#include "structofquest.h"
//#include "define.h"
#include "TGAddDamage.h"
#include "TGWeather.h"
#include "KncP2PLib/KncCommonDefine.h"

const int RANK_GRADE_NUM = 8;

class CTriggerManager;
class CTriggerCondition;
class KLuaManager;
class MONSTER;
class KGCDrama;
struct LTP_PLAYER_QUEST_RANK;
struct LTP_PLAYER_DAMAGE;

struct QuestFileInfo
{
public:
    QuestFileInfo()
    {
        iLowLevel           = 0;
        iHighLevel          = 0;
        iNameStringID       = 0;
        iDescStringID       = 0;
        strTGFileName       = "";
        strExTGFileName     = "";
		bLoading			= false;
        bReviseCharLevel    = false;
        fRankTotalScore     = 0.f;
        fHittedByMonNum     = 0.f;
        fDungeonDecreaseNum = 0.f;
        bIsCorrupted        = false;
        iCorruptionIndex    = 0;
    }

    int             iLowLevel;
    int             iHighLevel;
    int             iNameStringID;
    int             iDescStringID;
    std::string     strTGFileName;
    std::string     strExTGFileName;
	bool			bLoading;
    bool            bReviseCharLevel;
    float           fRankTotalScore;
    float           fHittedByMonNum;
    float           fDungeonDecreaseNum;
    bool            bIsCorrupted;
    int             iCorruptionIndex;
};

// 퀘스트들의 스크립트 정보 구조체
struct QuestSign
{
public:
    QuestSign()
    {
        iQuestID            = 0;
    }


    int                     iQuestID;

    QuestFileInfo GetQuestDifficultInfo(int iLevel)
    {
        MAP_QUESTFILE_INFO_ITER pos = mapQuestDifficult.find( iLevel );
        if ( pos != mapQuestDifficult.end() )
        {
            return pos->second;
        }

        QuestFileInfo blank;

        return blank;
    }

    void SetQuestDifficultInfo(QuestFileInfo Qfi, int iLevel)
    {
        mapQuestDifficult.insert( std::make_pair( iLevel, Qfi ) );
    }

	bool GetQuestDifficultAllow( int iLevel )
	{
        MAP_QUESTFILE_INFO_ITER pos = mapQuestDifficult.find( iLevel );
        if ( pos == mapQuestDifficult.end() )
        {
            return false;
        }

		return pos->second.bLoading;
	}

protected:
    typedef std::map<int,QuestFileInfo> MAP_QUESTFILE_INFO;
    typedef MAP_QUESTFILE_INFO::iterator   MAP_QUESTFILE_INFO_ITER;

    MAP_QUESTFILE_INFO mapQuestDifficult;
};

// 퀘스트 게임정보(랭크포인트) 구조체
struct SQuestPoint
{
    SQuestPoint()
    {
        Init();
    }

    void Init(void)
    {
        CriticalAttack      = 0;
        DoubleAttack        = 0;
        BackAttack          = 0;
        CounterAttack       = 0;
        AerialAttack        = 0;
        Attacked            = 0;
        Attack              = 0;
        iSpecial1           = 0;
        iSpecial2           = 0;
        iSpecial3           = 0;
    }

    int CriticalAttack;
    int DoubleAttack;
    int BackAttack;
    int CounterAttack;
    int AerialAttack;
    int Attacked;       // 공격 받은 카운트
    int Attack;         // 공격 카운트
    int iSpecial1;
    int iSpecial2;
    int iSpecial3;
};

// 다이얼로그 구조체
struct SDialogue
{
    bool            bRight;
    DWORD           dwDelayTime;
    int				dwTime;
    std::wstring    strSpeaker;
    std::wstring    strSpeech;
    std::string     strExtra;
    int             iSpeakerID;
    int             iSpeechID;

    SDialogue( void )
    {
        bRight          = false;
        dwDelayTime     = 0;
        dwTime			= -1;
        iSpeakerID      = -1;
        iSpeechID       = -1;
        strSpeaker.erase();
        strSpeech.erase();
        strExtra.erase();
    }
    SDialogue( const SDialogue& src )
    {
        bRight      = src.bRight;
        dwDelayTime = src.dwDelayTime;
        strSpeaker  = src.strSpeaker;
        strSpeech   = src.strSpeech;
        strExtra    = src.strExtra;
        iSpeakerID  = src.iSpeakerID;
        iSpeechID   = src.iSpeechID;
        dwTime		= src.dwTime;
    }
    ~SDialogue( void )
    {
        // empty
    }
    const SDialogue& operator=( const SDialogue& src )
    {
        bRight      = src.bRight;
        dwDelayTime = src.dwDelayTime;
        strSpeaker  = src.strSpeaker;
        strSpeech   = src.strSpeech;
        strExtra    = src.strExtra;
        iSpeakerID  = src.iSpeakerID;
        iSpeechID   = src.iSpeechID;
        dwTime		= src.dwTime;
        return *this;
    }
};

struct SQuestRankInfoS6
{
    float fComboTotalDamage;   //콤보 break 전까지의 데미지 포인트(combo break시 표기)
    float fRankPoint;          //랭크 데미지 포인트 (총 데미지 포인트)
    int m_MaxCombo;
    int m_MaxComboDamage;
    int m_ComboDamage;
    int m_TotlaDamageFromMonster; //몬스터에게 맞은 총 데미지    
    int m_TotalBackPlusAerialAttack;
    int m_KillCombo;
    int m_MaxKillCombo;
    int m_BossKillCombo;
    float fTotalDamage; //내가 몬스터에게 입힌 총 데미지
    int m_InfinityDungeonPartyRank;
    int m_InfinityDungeonMonsterLevel;

    SQuestRankInfoS6(void)
    {
        Init();
    }

    void Init(void)
    {
        fComboTotalDamage = 0.f;
        fRankPoint = 0.f;
        m_MaxCombo = 0;
        m_MaxComboDamage =0;
        m_TotlaDamageFromMonster = 0;
        m_ComboDamage =0;
        m_TotalBackPlusAerialAttack = 0;
        m_KillCombo = 0;
        m_MaxKillCombo = 0;
        m_BossKillCombo = 0;
        fTotalDamage = 0.f;
        m_InfinityDungeonPartyRank = 0;
        m_InfinityDungeonMonsterLevel = 0;
    }
};


class KQuestGameManager : public lua_tinker::lua_value
{	
	friend class KGCDebugCommand;
public:
	KDeclareLuaInvoke
	static void RegisterLuabind();
public:
    KQuestGameManager();
    virtual ~KQuestGameManager( void );

    enum
    {
        END_GAME_DELAY_TIME             = ( GC_FPS_LIMIT * 5 /*초*/ ),
        GAME_START_TARGETING_DELAY_TIME = ( GC_FPS_LIMIT * 6 /*초*/ ), // 게임시작 시 타게팅 잡는 딜레이 설정
        DIALOGUE_MIN_DEALY_TIME         = ( 1000 * 1 /*초*/ ),
        DIALOGUE_DEALY_TIME             = ( 1000 * 7 /*초*/ ),
        ENDGAMEINFO_DEALY_TIME          = ( 1000 * 10 /*초*/ ),
        ROULETTE_DEALY_TIME             = ( 1000 * 10 /*초*/ ),
    };

    enum { DEFAULT_POWER_POINT = 20, DEFAULT_CONTROL_POINT = 20 };      // 기본 랭크 점수 - 2008/01/21 기획측 추가 요청으로 추가.
    enum { RANK_STAR0 = 0, RANK_STAR1 = 40, RANK_STAR2 = 50, RANK_STAR3 = 60, RANK_STAR4 = 75, RANK_STAR5 = 90 };
    enum { TOTAL_RANK = 0, RANK_DECREASE_NUM = 1, RANK_HITTED_BY_MON = 2 };

public:
    Quest           m_Quest;                // 퀘스트 스크립트 정보
    int             m_iBeforeDirection;     // 이전 스테이지에서 깬문의 방향
    int             m_iCurStage;            // 현재 스테이지, 현재 위치
    int             m_iCurTileX;
    int             m_iCurTileY;
    int             m_iBossStageNum;        // 몇번이 보스 스테이지니?
    bool            m_bQuestStarted;        // 퀘스트 시작
    bool            m_bWin;                 // 퀘스트 완수 여부
    bool            m_bEndGame;             // 게임 종료 여부
    bool            m_bClearStage;          // 스테이지 클리어 여부
    bool            m_bLoading;             // 로딩중인가? 여부
    DWORD           m_dwEndGameDelay;       // 게임 종료(TGWin, TGLose 발동 후 딜레이)
    bool            m_bDramaStage;          // 드라마가 있는 스테이지인가( 있다면 전체 스킵을 할 수 없다. )
	
	DWORD			m_dwPlayFrame;			// 퀘스트 시작한 뒤 흐른만큼의 Frame
	bool			m_bShowStartMsg;		// 퀘스트 시작 메시지 표시 완료 여부

    // 090424 치트키
    bool            m_bPerfectMode;

private:
	//던전해킹방지작업
	bool				m_bMonLoadComplete;		// 몬스터의 로딩 완료여부
	int					m_iMonsterNum;			// 로딩된 몬스터 수	
	bool				m_bIsHackedDungeon;		// 올킬핵을 사용했는지
	bool				m_bIsMonsterHPHack;		// 타격없이 몬스터의 hp변화가 생겼는지
	bool				m_bIsAttackedMon;
	bool				m_bBossStage;			// 보스스테이지인지
public:
	int					m_iCurMonsterNum;		// 남은 몬스터 수
	std::vector<std::vector<float>>	m_vecCurAtk;
	std::vector<std::vector<float>>	m_vecIncAtkRatio;

	void InitHackingCheckData();
	void CheckHackAction();
	bool GetHackedDungeon() { return m_bIsHackedDungeon; }
	void SetHackedDungeon( bool bIsHackedDungeon ) { m_bIsHackedDungeon = bIsHackedDungeon; }
	bool GetMonsterHPHack() { return m_bIsMonsterHPHack; }
	void SetMonsterHPHack( bool bIsMonsterHPHack ) { m_bIsMonsterHPHack = bIsMonsterHPHack; }
	bool GetAttackedMonster() { return m_bIsAttackedMon; }
	void SetAttackedMonster( bool bIsAttackedMon ) { m_bIsAttackedMon = bIsAttackedMon; }
	void SetCurIncAtkRatio();
	bool IsBossStage() { return m_bBossStage; }
    bool GetIsCorrupted(void);
    int GetCorruptedIndex(void);
public:
    // 현재 스테이지 관리
    Stage *GetCurStage(void);
    void SetCurStage(int iStage);

    bool IsStartedStage(void);

    //************************************************************************
    // QuestSign Script (퀘스트 스크립트 정보들을 관리하는 스크립트)
    //************************************************************************
    std::vector<QuestSign>   m_vtQuestSign;
    QuestSign           GetQuestSign(int iStage);
    QuestFileInfo       GetQuestFileInfo(int iStage, int iLevel);
    std::wstring        GetCurQuestName(void);
    std::wstring        GetCurQuestDesc(void);
    std::string         GetCurQuestTrigger(void);
    bool                GetReviseCharLevel(void);
    HRESULT             LoadQuestSignScript(std::string strFileName);
    bool                ReloadQuestScript(void);
    bool                IsExistenceStage(int iStage);

    // 게임모드를 넣으면 해당 퀘스트의 정보를 돌려준다.
    bool                IsQuestLevelFromInputLevel(int iLowLevel, int iHighLevel, EGCGameMode eGameMode = EGCGameMode(-1), int iDungeonLevel = -1, int iMonLevel = -1 );         // 던전레벨에 전달된 레벨값이 포함되는지 확인
    int                 GetQuestIndexFromGameMode( EGCGameMode eGameMode );                 // 스트립트에서 몇번째 퀘스트인지 돌려줌
    QuestSign           GetQuestSignFromGameMode( EGCGameMode eGameMode );                  // 퀘스트 전체 정보를 리턴
    QuestFileInfo       GetQuestFileInfoFromGameMode( EGCGameMode eGameMode, int iLevel );  // 퀘스트 파일(레벨) 정보 리턴

    int                 GetQuestCorruptionIndex( EGCGameMode eGameMode );

    // 퀘스트 스크립트 로드 함수
    std::string CreateQuestFileName( QuestFileInfo &questFileInfo );
    HRESULT LoadQuestScript_New(std::string strFileName);
	HRESULT LoadDungeon_ExpGp(int stage,int difficult);

    // 퀘스트를 시작하는 함수
    void EnterQuest(void);
    void OnEndGame( void );

	bool IsPlatformer();
    bool IsFallingDie();
    //************************************************************************
    // 스테이지를 넘어갈 시 로딩이 완료된 플레이어 확인
    //************************************************************************
    void LoadStage(TileMap tilemap);
    void MoveGate(int iDirection, int iDelayTime = 0);          // 게이트로 이동합니다.
    void GateMoveGateCheck(void);                                // 게이트로 이동해야하는 상태인지 체크

    bool                 m_bLoadStageByPlayers;  // 플레이어들의 스테이지 로딩 완료여부
    std::vector<DWORD>   m_vecWaitToLoad;        // 로딩이 완료된 플레이어의 목록
    void SetPlayerPositionByGate(void);          // 이동해온 게이트로 플레이어를 이동함
    void SetSummonMonsterPosition(void);         // 소환 몬스터의 위치를 주인 플레이어에게로 옮긴다.
    void SetStageLoadComplete( DWORD dwUID_ );
    void AllStageLoadComplete();
    bool IsPlayerStageLoadComplete( DWORD dwUID_ );
    bool IsStageLoadComplete(void);

    void SetPlayerPosition(int iPlayerIndex, float iX, float iY);        // 픽셀좌표로 캐릭터를 해당위치로 이동
    void AllPlayerToReady(void);        // 모든 플레이어가 대기 모션으로 모션을 변경. (스테이지 넘어갈때 씀)
    void PlayerToReady(int iPlayer);    // iPlayer 를 대기 모션으로 변경.

    
    //************************************************************************
    // 다이얼로그 관려 함수
    //************************************************************************
    int                                                     m_iDialogueTG_ID;        // 현재 표시중인 다이얼로그 트리거 아이디
    DWORD                                                   m_dwDialogueDelay;       // 다이얼로그 지연시간용 변수
    std::map< std::string, std::vector<SDialogue> >         m_mapDialogueList;       // 다이얼로그 리스트
    std::vector<SDialogue>                                  m_vecDialogue;           // 현재 다이얼로그
    void SetShowDialogue(bool bShow);
    bool SetDialogueList(std::string strDialogue);          // stdDialogue를 표시할 다이얼로그로 선택한다.
    void AddDialogueList( std::string strName, std::vector<SDialogue> &sDialogue );
    HRESULT LoadDialogueTable(void);                        // 다이얼로그 스크립트를 불러온다.

    std::vector< std::string > m_vecEndDialogList;
    bool IsEndDialogList( std::string strName );
    void AddEndDialogList( std::string strName );
    void ClearEndDialogList(void);

    // 다이얼로그 제어
    int GetCurDialogeIndex(void) const;
    SDialogue* GetDialogue( int iIdx );
    void NextDialogue(void);
    void PrevDialogue(void);
    void SetDialogue(int iIdx);
    void FrameMove_Dialogue(void);

	void FrameMove_Msg(void);


    // 플레이어와 몬스터의 움직임 제어
    void SetEnableMovement(bool bEnable);
    bool IsEnableMovement(void);

    //************************************************************************
    // 게임이 끝난 후 게임 정보 송, 수신 관리 (퀘스트 포인트)
    //************************************************************************
    bool                                        m_bSendGetEndGameInfo;      // 아의 게임정보를 다른플레이어에 송신 여부
    bool                                        m_bGetEndGameInfo;          // 다른 플레이어의 게임 정보를 수신 여부
    DWORD                                       m_dwEndGameInfoDelayTime;   // 게임 종료 딜레이 시간.
    std::vector<DWORD>                          m_vecEndGameInfo;           // 로딩이 완료된 플레이어의 목록
    void CheckCompleteEndGameInfo(void);                                    // 게임정보 수신 완료를 체크하여, m_bGetEndGameInfo 변수 값 변경

    //************************************************************************
    // 게임이 끝난 후 아이템 룰렛 정보 송, 수신 관리
    //************************************************************************
    bool                                        m_bSendRouletteList;    // 내 아이템 룰렛 리스트를 다른플레이어에 송신 여부
    bool                                        m_bRouletteList;        // 다른 플레이어의 아이템 룰렛 리스트 수신 여부
    DWORD                                       m_dwRouletteWaitDelay;  // 룰렛 정보를 받는 대기 시간.
    std::vector<DWORD>                          m_vecRouletteWaitList;  // 아이템 룰렛 리스트 받은 플레이어의 목록
    std::map< DWORD, std::vector<DWORD> >       m_mapRouletteList;      // 아이템별 플레이어 아이템 룰렛 참가 리스트
    void AddRouletteList(DWORD iUIDItem, DWORD  iUIDPlayer);            // 룰렛 아이템, 룰렛 플레이어 정보 추가
    void CheckCompleteRouletteWaitList(void);                           // 룰렛 수신 완료를 체크하여, m_bRouletteList 변수 값 변경

    bool IsCompleteEndGameDelayTime(void);

    //************************************************************************
    // 게임 정보 관리
    //************************************************************************
    KSafeArray<int,MAX_PLAYER_NUM> m_PlayerRankPoint;  // 플레이어 등급 관리
    KSafeArray<float,MAX_PLAYER_NUM> m_PlayerDamage;  // 플레이어 데미지
    KSafeArray<SQuestPoint,MAX_PLAYER_NUM> m_PlayerQuestPoint;  // 현재 사용하는 스테이지 퀘스트 포인트
	KSafeArray<std::map<int,SQuestPoint>,MAX_PLAYER_NUM> m_alQuestPoint;      // 스테이지별 퀘스트 포인트 관리
    
    std::vector<std::map< int, KDungeonPersonalRecord >>   m_vecDungeonPersonalRecord;  // 플레이어 스테이지별 클리어 기록 관리 관리
    
    void DoStageClearProcess();
    
    void LoadFromQuestPointList(int iStage, int iPlayer, SQuestPoint &sQuestPoint);    // 스테이지별 퀘스트 포인트를 불러온다.
    void SaveToQuestPointList(int iStage, int iPlayer, SQuestPoint &sQuestPoint);      // 스테이지별 퀘스트 포인트를 저장한다.
    void CalcQuestPointByDamageType(int iPlayer, int iType);
	void IncreaseAttackPoint(int iPlayer);
	void IncreaseAttackedPoint(int iPlayer);
	void IncreaseSpecial1Point(int iPlayer);
	void IncreaseSpecial2Point(int iPlayer);
	void IncreaseSpecial3Point(int iPlayer);

    int GetPowerPoint(void);
    int GetControlPoint(void);
    int GetAttackedPoint(void);
    int GetTotalScorePoint(void);
	int GetTotalScoreStar(void);

	// 시작 위치를 얻어낸다.
	D3DXVECTOR2 & GetArenaPos(void);
	void		SetArenaPos(float x,float y);
	

    //************************************************************************
    // 미니맵에서 지나온 스테이지에 체크를 해둔다.
    //************************************************************************
    std::map< std::pair<int, int>, bool > m_mapStageHistory;        // <Row, Col>, Direction
    void CheckMinimapArrowColor(int iOldRow, int iOldCol, int iOldStage, int iNewRow, int iNewCol, int iNewStage);
    int GetNumOfClearStage();
	int GetTotalStageNum();
	std::set<int>	GetClearStageSet();

    // 스테이지 있는 몬스터 리스트 미리 생성
    void PreGeneraterStageMonster( int iStage );


    // 물 막대기를 일정 타임 동안 화면에 표시하는 부분
    std::vector<DWORD>                                  m_vtWaterPoleTime;	    
    std::vector<CTGAddDamage>                           m_vtCurWaterPole;
    void StartWaterPole(CTGAddDamage tgWaterPole);
    void FrameMove_WaterPole(void);

protected:
    HRESULT LoadQuestFromLua(KLuaManager &luaMgr, Quest *quest);              // 루아로 저장된 Quest 정보를 로드합니다.
    HRESULT LoadStageFromLua(KLuaManager &luaMgr, std::string strStage, Stage *stage, CTriggerManager **pTriggerManager);  // 루아로 저장된 Stage 정보를 로드합니다.
    HRESULT LoadFromFile_Trigger(KLuaManager &luaMgr, CTriggerManager **pTriggerManager);
    HRESULT LoadFromFile_EAction(KLuaManager &luaMgr, std::vector<CTriggerCondition*> &Trigger);
    HRESULT LoadFromFile_Condition(KLuaManager &luaMgr, std::vector<CTriggerCondition*> &pTrigger);

protected:
    int                                 m_iCurDialoge;          // 다이얼로그 현재 인덱스
    bool                                m_bMovement;            // 행동제어 상태인지 확인
    
    //////////////////////////////////////////////////////////////////////////
    // 드라마 관련!!!!
public:
		bool								IsSkipping(){return m_bSkipping;}
		void								SkipDialogue();
protected:
    KGCDrama*                           m_pkDrama;

	D3DXVECTOR2							m_vecArena;	
	int									m_iMaxStage;
    int                                 m_iAllLevelMaxStage;
    int                                 m_iComboBreakCount;
	bool								m_bSkipping;

private:
    SQuestRankInfoS6                    m_sQuestRankInfoS6;

public:
    int GetMaxStage() { return m_iMaxStage; }
    int GetAllLevelMaxStage() { return m_iAllLevelMaxStage; }
    SQuestRankInfoS6& GetQuestRankInfo_S6() { return m_sQuestRankInfoS6; }
    void CountComboBreak();

    bool IsAllDieMonsterToStageClearRank();
    void InitAllPlayerComboState();


	bool m_bExistMonsterInScreen;
	bool CheckMonsterExistInScreen();

    bool m_bPortalInScreen;
    bool CheckPortalInScreen();
    void ClearDungeonKillMon( void );

public:
    KncStopWatch m_kStageLoadingStopWatch;
    int m_iState;
    std::vector<float> m_vecRankCorrectionValue_Attack;
    std::vector<float> m_vecRankCorrectionValue_Decrease;
    std::vector<float> m_vecRankGradeRatioValue;
    std::vector<float> m_vecRankCorrectionValue_AttackInInfinite;
    std::vector<float> m_vecRankCorrectionValue_DecreaseInInfinite;

private:
    int m_iHeroDungeonLevel;
    int m_iPlayersAveLv;

    unsigned char m_ucPlayerNum;
    bool m_bRankPortalCheck;
    bool m_bComboBreak;
    typedef std::vector<std::pair<int, int> > vectorRanking;
    vectorRanking m_vecPlayerRanking;

public:
    int GetPlayerAveLv( void );
    int GetHeroDungeonLevel( void ) { return m_iHeroDungeonLevel; }
    void CalPlayerInfo( void );

private:
    std::vector<int> m_mapMonsterRefreshTaget; 

public:
    void SetMonsterRefreshTaget(int iMonsterIndex_ ) { m_mapMonsterRefreshTaget.push_back(iMonsterIndex_); }
    bool IsEmptyMonsterTagetRefresh() { return m_mapMonsterRefreshTaget.empty(); }
    bool IsExistMonsterTagetRefresh(KGC_PID_MONSTER_TARGET_REFRESH* kMonsterTagetRefresh_);
    bool IsNextSceneKeyUp();
    void SetTotalDamagePoint_S6( float fPoint_, float fMaxHP_, bool bComboState , int iIndex  = -1);
    void RankPointCalcState( bool bGoing );
    void CalcMaxCombo( int iCombo );
    void CalcDamageFromMonster( float fDamage , float fMaxHP_);
    void GetDungeonDetailInfo(OUT int& iMaxCombo, OUT int& iMaxComboDamage, OUT int& iContributionPercent, OUT int& iTotalDamageFromMon , OUT int& iTotalBackAerial , OUT int& iMaxKillCombo );
    void ClearDungeonDetailInfo();
    void CalcBackArielCount(int iPlayer, int iType);
    void ComboBreakCheck( bool bComboBreak );
    void SetAttackTypeState( int iState);
    int GetAttackTypeState(){ return m_iState; }
    void GetComboDamage(OUT int& iComboDamage );
    bool GetComboBreakState(){ return m_bComboBreak; }
    void SetComboKillPoint( bool bClear);
    void GetComboKillPoint(OUT int& iKillPoint );

    void SetComboBossKillPoint();
    void GetComboBossKillPoint( OUT int& iBossKillPoint );

    int GetRankNum();
    int GetRankGradeMaxNum(){return RANK_GRADE_NUM;}
    void CalcPlayerRank();
    void SetPlayerRank( LTP_PLAYER_QUEST_RANK* kPacket);
    bool GetPlayerRank( IN int iIndex , OUT std::wstring& wstrNickName, OUT int& iRankGrade );
    int GetPlayerRank( IN int iIndex );
    int GetPartyAvgRank();
    int GetPartyHighestRank();

    void SetInfinityDungeonPartyRank( const int iRank_ );
    int  GetInfinityDungeonPartyRank( void );
    void SetInfinityDungeonMonsterLevel( const int iLevel_ );
    int  GetInfinityDungeonMonsterLevel( void );
    
    void GetPlyaerIdxForRanking( IN int iRanking, OUT int& iPlayerIdx );
    float GetDungeonRankInfo( EGCGameMode eGameMode , int iLevel, int iDataType);
    int GetRankIndex( float fRankPoint);
    void GetRankScroeTermPercent(IN int iIndex, IN float fRankPoint , OUT float& fPercent );
    bool LoadRankCorrectionScript();
    void GetRankCorrectionValue(IN int iPlayerCnt , IN int iType, OUT float& fCorrectionValue );
    bool NotDecreaseCheck();
    void CalcTotalDamageForMonster(float fDamage);
    int GetQuestStartUserCnt(){ return m_iQuestStartUserCnt; }
    void SetStartUserCount ( int iCnt);
    void SetPlayerDamage( IN int iIndex, IN float fDamage );
    void GetRankGradeRatioValue(OUT float& fRankRatio );
    // 각 스테이지별 먹은 GP UID를 저장함!
    std::set<DWORD> m_setGpUID;
    std::list<std::pair<int , int>> m_listPlayerRank;
    int m_iQuestStartUserCnt;
    bool m_bFirstStartUserRecord;
};

extern KQuestGameManager* g_pkQuestManager;

#endif // _QUESTGAMEMANAGER_H_
