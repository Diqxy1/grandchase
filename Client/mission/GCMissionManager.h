#pragma once
//#include <vector>
//#include <set>
#include "KNCSingleton.h"
#include "GCMission.h"
#include "GCCondition.h"
#include "GCSubMission.h"
#include "KGCMissionString.h"
//#include "..\GCLuaManager\KLuaManager.h"

struct MissionCompleteEff {
    std::string strParticle;
    D3DXVECTOR2 dxvPos;

    MissionCompleteEff() : strParticle( "" ), dxvPos( 0.0f, 0.0f ) {}
};

typedef std::map< DWORD,  KGCMission > MissionList;
typedef std::map< int, KGCSubMission > SubMissionList;
typedef std::map< int, KGCCondition >  ConditionList;

#define MAX_MISSION_REWARD_NUM  4

struct SMissionReward
{
	int m_iGp;
	int m_iExp;
	std::vector<std::pair< DWORD, int > >  m_vecItem;
	int m_iNewJob;
	int m_iNewChar;

	bool m_bLevelUp;
	EGCCharType m_eCharType;
	int m_iLevel;

	SMissionReward()
	{
		Init();
	}

	void Init()
	{
		m_iGp = 0; 
		m_iExp = 0;
		m_vecItem.clear();
		m_iNewChar = -1;
		m_iNewJob = -1;

		m_bLevelUp = false;
		m_eCharType = GC_CHAR_INVALID;
		m_iLevel = 0;
	}
};



class KGCMissionManager
{
    DeclareSingleton(KGCMissionManager);
public:
    KGCMissionManager(void);
    ~KGCMissionManager(void);

    enum{
        GC_MM_OK,
        GC_MM_NOT_ENOUGH,
        GC_MM_MISSION_NOT_EXIST,
        GC_MM_SUBMISSION_NOT_EXIST,
        GC_MM_CONDITION_NOT_EXIST,
		GC_MM_NOT_ENOUGH_LEVEL,
		GC_MM_NOT_ENOUGH_PROMOTION,
        GC_MN_NOT_ALREADY_PROMOTION,
		GC_MM_DIFFERENT_CHAR,
    };

    enum
    {
        TYPE_NOUSE       = -1, // 사용안함
        TYPE_RANK        = 0,  // 랭크정보
        TYPE_USE_POTION  = 1,  // 포션 사용횟수 제한
        TYPE_BEATTACK    = 2,  // 피격 횟수( 몬스터에서 맞은 횟수 )
        TYPE_COMBO       = 3,  // 콤보 횟수
        TYPE_BACK_ATTACK = 4,  // 백어택 횟수
        TYPE_SKILL_COUNT = 5,  // 스킬 사용제한
    };

    
    enum
    {   
        EM_LIMIT_LOW_LEVEL = 10,    // 미션 받을수 있는 레벨 제한 조건
    };

    //////////////////////////////////////////////////////////////////////////
    // 미션 스트링 관련 
    //////////////////////////////////////////////////////////////////////////
private:
    KGCMissionString m_kStringMgr;
public:
    bool LoadMissionString( std::string strFileName , FILE* pFile = NULL, bool bForTool = false );
    bool LoadMissionString( FILE* pFile = NULL );
    bool OutPutMissionString( std::string strFileName );
    int AddString( std::wstring str );
    void EraseString( int iID );
    std::wstring GetString ( int iID );
    std::wstring GetTitleString( DWORD dwMissionID );

private:
    KGCMission* operator[] ( DWORD dwID )
    {
        if( m_mapMissionList.count( dwID ) == 0 )
            return NULL;
        return &m_mapMissionList[dwID];
    }
    //////////////////////////////////////////////////////////////////////////
    // 미션 관련
    //////////////////////////////////////////////////////////////////////////
private:
    std::map< DWORD,  KGCMission > m_mapMissionList;
    std::map< int, KGCSubMission > m_mapSubMission;
    std::map< int, KGCCondition > m_mapCondition;
    std::set< DWORD > m_setUniqueMission;
    std::map< DWORD, std::vector< MissionCompleteEff > > m_mapCompleteEff;

    std::map<int, std::vector<DWORD>> m_mapDungeonEpicMissionData;
    std::map<int, std::vector<DWORD>> m_mapDungeonNormalMissionData;

    // 현재캐릭터의 클리어된 에픽미션, 각던전의 첫번째 일반미션
    std::vector<DWORD> m_vecClearMissionData;

    // < 미션ID, 획득하려면 선행으로 클리어되야하는 미션ID >
    std::map< DWORD, DWORD > m_mapGettableCondditionInfo;

public:
    bool LoadScript();
    bool LoadMissionFile( std::string strMissionScriptFile , KLuaManager* pkLuaMgr = NULL);    //미션 스크립트를 로드 합니다.
    //bool OutPutMissionFile( std::string strMissionScriptFile );  //미션 스크립트를 파일로 뽑습니다.

    // 미션에 필요한 요소를 추가하는 부분입니다. ( 미션, 서브미션, 컨디션 );
    bool    AddMissionInfo( const SMissionInfo& sInfo );
    int     AddSubMissionInfo( const SSubMissionInfo& sInfo );
    int     AddConditionInfo( const SConditionInfo& sInfo );

    bool    UpdateMissionInfo( const SMissionInfo& sInfo );
    bool    UpdateSubMissionInfo( const SSubMissionInfo& sInfo );
    void    UpdateUniqueMissionList();

    int    CheckCompleteMission( DWORD dwMissionID, std::vector< std::pair< int, int > > vecCompletion );
    int     IsAvailableRegister( DWORD dwMissionID, SCharInfo& sCharInfo_ );
    bool    IsUniqueMission( DWORD dwMissionID );

    void    Reset();

    int     GetNewSubMissionID();   //새로 생성될 서브 미션이 가질 ID를 리턴합니다. 아직 추가되지 않은 서브미션 ID
    int     GetNewConditionID();    //새로 생성될 컨디션이 가질 ID를 리턴합니다. 아직 추가되지 않은 컨디션 ID

	bool	CheckSubMissionCondition( int iSubMissionID, const SConditionInfo& sInfo, int iMonLevel = -1 );

    std::wstring GetTitleColor( int iCharLevel, int iMissionLevel );

    KGCMission* GetMissionWithID( DWORD dwID );
    const SMissionInfo* GetMissionInfo( DWORD dwID );
    const SSubMissionInfo* GetSubMission( int iID );
    const SConditionInfo* GetConditionInfo( int iID );

    bool    IsMissionExist( DWORD dwMissionID ) {
        return (m_mapMissionList.count( dwMissionID ) == 0 ? false : true );
    }
    bool LoadMissionStringFromKOM( std::string strFileName);
	int GetMissionNPCID( DWORD dwMissionID );
    bool IsEnableDelete( DWORD dwMissionID );

    bool LoadMissionCompleteEff( const char* strFileName_ );  // 미션 완료시 지정 이펙트 파일 읽기 
    bool GetMissionCompleteEff( const DWORD dwMissionID_, OUT std::vector< MissionCompleteEff >& vecEff_ );
    bool IsCompleteEff( const DWORD dwMissionID_ );

    void    SetClearMissionData( std::vector<DWORD> &vecClearMissionData );
    bool    CheckEpicMission( int iGameMode );
    bool    CheckClearCondisionMission( DWORD dwMissionID );
    DWORD   CheckNormalMission( int iGameMode );
    int     GetEpicMissionIconType( EGCGameMode eGameMode, int iCurrentCharLevel );
    void    SetMissionGettableCondditionInfo( std::map< DWORD, DWORD > &mapGettableCondditionInfo );
};
DefSingletonInline(KGCMissionManager);
