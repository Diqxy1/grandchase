#pragma once
//////#include <vector>
////#include <set>
#include "KNCSingleton.h"
#include "GCMission.h"
#include "GCCondition.h"
#include "GCSubMission.h"
#include "KGCMissionString.h"
//#include "..\GCLuaManager\KLuaManager.h"

typedef std::map< DWORD,  KGCMission > MissionList;
typedef std::map< int, KGCSubMission > SubMissionList;
typedef std::map< int, KGCCondition >  ConditionList;


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
		GC_MM_DIFFERENT_CHAR,
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

public:
    bool LoadMissionFile( std::string strMissionScriptFile , KLuaManager* pkLuaMgr = NULL);    //미션 스크립트를 로드 합니다.
    bool OutPutMissionFile( std::string strMissionScriptFile );  //미션 스크립트를 파일로 뽑습니다.

    // 미션에 필요한 요소를 추가하는 부분입니다. ( 미션, 서브미션, 컨디션 );
    bool    AddMissionInfo( const SMissionInfo& sInfo );
    int     AddSubMissionInfo( const SSubMissionInfo& sInfo );
    int     AddConditionInfo( const SConditionInfo& sInfo );

    bool    UpdateMissionInfo( const SMissionInfo& sInfo );
    bool    UpdateSubMissionInfo( const SSubMissionInfo& sInfo );
    void    UpdateUniqueMissionList();

    int    CheckCompleteMission( DWORD dwMissionID, std::vector< std::pair< int, int > > vecCompletion );
    int     IsAvailableRegister( DWORD dwMissionID, int iChar, int iLevel, int iPromotion );
    bool    IsUniqueMission( DWORD dwMissionID );

    void    Reset();

    int     GetNewSubMissionID();   //새로 생성될 서브 미션이 가질 ID를 리턴합니다. 아직 추가되지 않은 서브미션 ID
    int     GetNewConditionID();    //새로 생성될 컨디션이 가질 ID를 리턴합니다. 아직 추가되지 않은 컨디션 ID

	bool	CheckSubMissionCondition( int iSubMissionID, const SConditionInfo& sInfo );

    KGCMission* GetMissionWithID( DWORD dwID );
    const SMissionInfo* GetMissionInfo( DWORD dwID );
    const SSubMissionInfo* GetSubMission( int iID );
    const SConditionInfo* GetConditionInfo( int iID );

    bool    IsMissionExist( DWORD dwMissionID ) {
        return (m_mapMissionList.count( dwMissionID ) == 0 ? false : true );
    }
};
DefSingletonInline(KGCMissionManager);
