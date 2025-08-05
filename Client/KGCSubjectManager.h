/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2013년 05월 23일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 달성과제 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once
#include "KGCSubject.h"

class KGCSubjectManager
{
    DeclareSingleton( KGCSubjectManager );
public:
    KGCSubjectManager(void);
    ~KGCSubjectManager(void);

#if !defined( __PATH__ )
    void CheckFileMonitor();
    void RegisterFileMoniter( const char *szFileName );
#endif

public:
    void InitStats();

public:
    void LoadScript();
    void LoadSubjectTemplate( KLuaManager &luaMgr );
    void LoadActionScript( KLuaManager &luaMgr, KGCSubjectTemplate &kSubjectTemplate );
    void LoadString();

public:
    void RegisterLuaBind();

public:
    void DestoryAllInstance();
    void ClearInstance( DWORD dwUID );

public:
    void FrameMove();

public:
    void TextRender();

public:
    const KGCSubjectTemplate* GetSubjectTemplate( DWORD dwType );
    const std::map<DWORD, KGCSubject*>& GetAllSubjectInstance();
    KGCSubject* GetSubjectInstance( DWORD dwUID );
    KGCSubject* GetSubjectInstanceFromType( DWORD dwSubjectType );
    int GetMaxSubject();
    KGCStringLoader* GetStringLoader()  {   return &m_kStrLoader;    };

public:
    KGCSubject* CreateSubject( DWORD dwType, int iUserIndex );
    int GetSubjectNum();

public:
    void SetReward( KEVENT_DUNGEON_SUBJECT_REWARD_ACK& kRecv );
    void SetRewardExp( DWORD dwPlayerUID, int iExp );
    void SetRewardItem( std::vector< KItem >& kItem );

public:
    void AddBackAttack();
    void AddSpecial();
    void AddDamageHitCnt();
    void AddDamageVolume( float fDamage );
    void AddDieCnt();
    void SetComboBreak();

private:
    DWORD                                m_dwUIDCounter;
    std::map<DWORD, KGCSubject*>         m_mapSubjectInstance;
    std::map<DWORD, KGCSubjectTemplate>  m_mapSubjectTemplate;

public:
    KDungeonSubjectStat                  m_kSubjectStats;
    KGCStringLoader                      m_kStrLoader;

public:
    DWORD                                m_dwBeforeSubject;
};

DefSingletonInline( KGCSubjectManager );