#ifndef _KGCINFINITY_DUNGEON_MANAGER_H_
#define _KGCINFINITY_DUNGEON_MANAGER_H_
#pragma once


#include "KNCSingleton.h"
#include "KGCInfinityDungeon.h"

class KGCInfinityDungeonManager
{
    DeclareSingleton( KGCInfinityDungeonManager );
public:
    enum
    {
        EM_RANK_F = 0,
        EM_RANK_E = 1,
        EM_RANK_D = 2,
        EM_RANK_C = 3,
        EM_RANK_B = 4,
        EM_RANK_A = 5,
        EM_RANK_S = 6,
        EM_RANK_SS = 7,
        EM_RANK_SIZE = 8,
    };

    enum
    {
        EXP_RANK_VBAD   = 0,
        EXP_RANK_BAD    = 1,
        EXP_RANK_NORMAL = 2,
        EXP_RANK_GOOD   = 3,
        EXP_RANK_VGOOD  = 4,
        EXP_RANK_NONE   = 5,
    };

    enum
    {
        MAX_GP_REWARD_NUM = 100,
    };

public:
    KGCInfinityDungeonManager();
    ~KGCInfinityDungeonManager();

    void Init();
    bool LoadScript();
    void InitBeforeStartGame( void );   // 게임 시작하기 전에 초기화 해야할 것들..

public:
    std::map<int,KGCInfinityDungeon>    m_mapInfinityDungeonInfo;
    
    int m_nGuideGameMode; // 가이드 되는 던전 게임모드

public:    
    bool m_bShowGuide;    // 가이드 출력 여부
    int  m_iStartStage;


    KGCInfinityDungeon* GetInfinityDungeon( int nGameMode );
    std::string GetRandomStageName( int nGameMode );
    std::string GetStageName( int nGameMode, int nindex );
    int GetStageCount( int nGameMode );
    
    bool GetRankScore( int nGameMode, int nRank, int& nScore, int& nTime );
    int GetRegenBossRank( int nGameMode );
    int GetRegenBossLevel( int nGameMode, int nRank );
    int GetRegenBossWaitTime( int nGameMode );
    int GetMinBossRegenTime( int nGameMode );
    int GetMaxBossRegenTime( int nGameMode );

    int GetRegenMonsterTime( int nGameMode );
    int GetMaxMonsterNum( int nGameMode, int nPlayerNum );

    // 소환되어야할 보스 몬스터 리스트
    // 여러 마리 소환되어야 할 경우가 있어서 vector에 담아준다.    
    // 랭크에 의해서 소환할 경우 bRankGen = true 
    // 시간에 의해서 소활할 경우 nRank에 현재 랭크 넣어준다. 
    bool GetRegenBossList( int nGameMode, bool bRankGen, int nRank, std::vector<int>& vecBossMon );      
    
    // 최초 게임 시작시 생성될 몬스터 그룹을 요청한다
    bool GetStartMonsterList( int nGameMode, int nMonsterNum, std::vector<int>& vecMonsterList );
    int  GetStartMonsterLevel( int nGameMode );
    
    // 특정 랭크의 소환될 몬스터를 가져온다.
     bool GetRankMonsterList( int nGameMode, int nRank, int nMonsterNum, std::vector<int>& vecMonsterList );
    int  GetRankMonsterLevel( int nGameMode, int nRank );

    int GetGuideGameMode() { return m_nGuideGameMode; }

private:
    DWORD   m_dwRoundSquence;   // 몇번째 소환인가~

public:
    DWORD   GetRoundSquence( void ) const { return m_dwRoundSquence; }
    bool    SummonStartMonsterReq( const int nGameMode_, const int nNumPlayer_, const int nRank_ );
    bool    SummonMonsterReq( const int nGameMode_, const int nNumPlayer_, const int nRank_, const bool bBoss_, const bool bGenRankBoss_ );
    bool    GenMonster( const std::vector< KIDMonsterInfo > vecMonsterList_ );

public:
    void    CheckShowGuide();
    bool    IsShowGuide();
    void    ShowGuide();
    
public:
    void SetStartStageIndex( int nStage ) { m_iStartStage = nStage; }
    int  GetStartStageIndex( void ) { return m_iStartStage; }

public:
    void PreGeneraterStageMonster( int nGameMode );

public:
    int GetExpRate( const int nGameMode_, const int iMonsterLevel_ );

};

DefSingletonInline( KGCInfinityDungeonManager );

#endif