#ifndef _KGCINFINITY_DUNGEON_H_
#define _KGCINFINITY_DUNGEON_H_
#pragma once

struct SBossRankGroup
{
    int                 m_nRank;
    int                 m_nLevel;
    bool                m_bRandom;
    std::vector<int>    m_vecBossList;

    SBossRankGroup()
    {
        m_nRank = 0;
        m_nLevel = 1;
        m_bRandom = false;
        m_vecBossList.clear();
    }
};

struct SMonsterRankGroup
{
    int m_nRank;
    int m_nLevel;
    std::vector<std::pair<int,int>> m_vecRegenRate;
    std::vector<int>                m_vecMonsterGroup;

    SMonsterRankGroup()
    {
        m_nRank = 0;
        m_nLevel = 1;
        m_vecRegenRate.clear();
        m_vecMonsterGroup.clear();
    }

    int GetRegenGroup();
    int GetRegenMonster();
    int GetRegenLevel() { return m_nLevel; }
};

class KGCInfinityDungeon 
{
public:
    KGCInfinityDungeon();
    ~KGCInfinityDungeon();

public:

    int m_nGameMode;

    std::vector<std::string> m_vecStageInfo;    // 스테이지 리스트
    std::vector<std::pair<int,int>> m_vecRankScoreInfo; // 랭크별 점수 및 유지 시간


    // Boss 소환 정보
    int m_nBossRegenRank;       // 보스 몬스터 소환되는 랭크
    int m_nBossRegenWaitTime;   // 랭크 달성시 보스 몬스터 소환까지 걸리는 시간
    int m_nMinBossRegenTime;    // 보스 몬스터 최소 소환주기
    int m_nMaxBossRegenTime;    // 보스 몬스터 최대 소환주기

    std::map<int,SBossRankGroup>    m_mapBossRankGroup;   // 등급별 보스 몬스터 정보

    int m_iTotalAtk;

    // 일반 몬스터 소환 정보
    int m_nMonsterRegenTime;    // 일반 몬스터 소환 주기
    int m_nStartMonsterLevel;   // 시작시 생성되는 몬스터 레벨 
    std::vector<int> m_vecMaxMonsterNum;        // 파티인원별 몬스터 개수
    std::vector<int> m_vecStartMonsterGroup;    // 시작시 생성된 몬스터 그룹
    std::map<int,SMonsterRankGroup> m_mapMonsterRankGroup;  // 등급별 몬스터 소환 정보

    int GetTotalATK() { return m_iTotalAtk; }

    std::string GetStageName();
    std::string GetStageName( int nIndex );
    int         GetStageNum( void );

    bool GetRankScore( int nRank, int& nScore, int& nTime );
    int GetRegenBossRank() { return m_nBossRegenRank; }
    int GetRegenBossLevel( int nRank );
    int GetRegenBossWaitTime() { return m_nBossRegenWaitTime; }
    int GetMinBossRegenTime() { return m_nMinBossRegenTime; }
    int GetMaxBossRegenTime() { return m_nMaxBossRegenTime; }
    bool GetRegenBossList( bool bRankGen, int nRank, std::vector<int>& vecBossMon );

    int GetRegenMonsterTime() { return m_nMonsterRegenTime; }
    int GetMaxMonsterNum( int nPlayerNum );
    int GetStartMonsterLevel() { return m_nStartMonsterLevel; }

    // 최초 게임 시작시 생성될 몬스터 그룹을 요청한다.
    bool GetStartMonsterList( int nMonsterNum, std::vector<int>& vecMonsterList );

    // 특정 랭크의 소환될 몬스터를 가져온다.
        bool GetRankMonsterList( int nRank, int nMonsterNum, std::vector<int>& vecMonsterList );
    int  GetRankMonsterLevel( int nRank );

    void GetAllMonsterList( std::set<int>& setMonsterList );

private:
    bool _GetRegenBossList( int nRank, std::vector<int>& vecBossMon );
};

#endif