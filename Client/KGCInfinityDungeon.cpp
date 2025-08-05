#include "stdafx.h"
#include "KGCInfinityDungeon.h"


int SMonsterRankGroup::GetRegenGroup()
{
    if ( m_vecRegenRate.empty() )
        return -1;

    int nRankRand = rand() % 100;
    int nTotal = 0;

    for ( int i = 0; i < (int)m_vecRegenRate.size(); i ++ )
    {
        nTotal += m_vecRegenRate[i].second;

        // 그룹 선택 체크
        if ( nRankRand < nTotal )
        {
            return m_vecRegenRate[i].first;
        }
    }

    return m_vecRegenRate[0].first;
}

int SMonsterRankGroup::GetRegenMonster()
{
    if ( m_vecMonsterGroup.empty() )
        return -1;

    int nRand = rand() % m_vecMonsterGroup.size();

    return m_vecMonsterGroup[nRand];
}



KGCInfinityDungeon::KGCInfinityDungeon()
{
    m_nGameMode = GC_GM_INVALID;
    m_nBossRegenRank = 0;
    m_nBossRegenWaitTime = 0;   
    m_nMinBossRegenTime = 0;    
    m_nMaxBossRegenTime = 0;    

    m_vecStageInfo.clear();
    m_vecRankScoreInfo.clear();
    m_mapBossRankGroup.clear();

    m_nMonsterRegenTime = 0;
    m_nStartMonsterLevel = 1;

    m_vecMaxMonsterNum.clear();        // 파티인원별 몬스터 개수
    m_vecStartMonsterGroup.clear();    // 시작시 생성된 몬스터 그룹
    m_mapMonsterRankGroup.clear();  // 등급별 몬스터 소환 정보    
}

KGCInfinityDungeon::~KGCInfinityDungeon()
{
}

bool KGCInfinityDungeon::GetRankScore( int nRank, int& nScore, int& nTime )
{
    if ( nRank < 0 || nRank >= (int)m_vecRankScoreInfo.size() )
        return false;

    nScore = m_vecRankScoreInfo[nRank].first;
    nTime = m_vecRankScoreInfo[nRank].second;

    return true;
}

bool KGCInfinityDungeon::GetRegenBossList( bool bRankGen, int nRank, std::vector<int>& vecBossMon )
{
    // 특정 랭크에 의해서 불러오는 거라면 전체 리스트에서 한마리 가져온다.
    if ( bRankGen )
    {
        int nSize = m_mapBossRankGroup.size();
        int nSelRank = rand() % nSize;

        return _GetRegenBossList( nSelRank, vecBossMon );
    }
    // 시간에 의해서 소환되는 거라면 해당 랭크에 맞게 소환한다.
    else
    {
        return _GetRegenBossList( nRank, vecBossMon );
    }

    return true;
}

bool KGCInfinityDungeon::_GetRegenBossList( int nRank, std::vector<int>& vecBossMon )
{
    vecBossMon.clear();
    
    if ( m_mapBossRankGroup.find( nRank ) == m_mapBossRankGroup.end() )
        return false;

    bool bRandom = m_mapBossRankGroup[nRank].m_bRandom;

    if ( bRandom ) 
    {
        int nSelBoss = rand() % m_mapBossRankGroup[nRank].m_vecBossList.size();
        vecBossMon.push_back( m_mapBossRankGroup[nRank].m_vecBossList[nSelBoss] );
    }
    else
    {
        vecBossMon = m_mapBossRankGroup[nRank].m_vecBossList;
    }

    return true;
}

int KGCInfinityDungeon::GetRegenBossLevel( int nRank )
{
    if ( m_mapBossRankGroup.find( nRank ) == m_mapBossRankGroup.end() )
        return 1;

    return m_mapBossRankGroup[nRank].m_nLevel;
}

int KGCInfinityDungeon::GetMaxMonsterNum( int nPlayerNum )
{
    if ( nPlayerNum <= 0 || nPlayerNum > (int)m_vecMaxMonsterNum.size() )
        return -1;

    return m_vecMaxMonsterNum[nPlayerNum-1];
}

// 최초 게임 시작시 생성될 몬스터 그룹을 요청한다.
bool KGCInfinityDungeon::GetStartMonsterList( int nMonsterNum, std::vector<int>& vecMonsterList )
{
    vecMonsterList.clear();
    
    for( int i = 0; i < nMonsterNum; ++i ) 
    {
        int nRand = rand() % m_vecStartMonsterGroup.size();
        
        vecMonsterList.push_back( m_vecStartMonsterGroup[nRand] );
    }

    return true;
}

// 특정 랭크의 소환될 몬스터를 가져온다.
bool KGCInfinityDungeon::GetRankMonsterList( int nRank, int nMonsterNum, std::vector<int>& vecMonsterList )
{
    vecMonsterList.clear();

    if ( m_mapMonsterRankGroup.find( nRank ) == m_mapMonsterRankGroup.end() )
        return false;

    SMonsterRankGroup& sMonsterRankGroup = m_mapMonsterRankGroup[nRank];


    for ( int i = 0; i < nMonsterNum; ++i ) 
    {
        int nRankGroup = sMonsterRankGroup.GetRegenGroup();
        
        if ( m_mapMonsterRankGroup.find( nRankGroup ) == m_mapMonsterRankGroup.end() )
            return false;

        vecMonsterList.push_back ( m_mapMonsterRankGroup[nRankGroup].GetRegenMonster() );
    }

    return true;
}

int KGCInfinityDungeon::GetRankMonsterLevel( int nRank )
{
    if ( m_mapMonsterRankGroup.find( nRank ) == m_mapMonsterRankGroup.end() )
        return 1;

    return m_mapMonsterRankGroup[nRank].GetRegenLevel();
}

std::string KGCInfinityDungeon::GetStageName()
{
    if ( m_vecStageInfo.empty() )
        return std::string();
    
    int nRand = rand() % m_vecStageInfo.size();

    return m_vecStageInfo[nRand];
}

std::string KGCInfinityDungeon::GetStageName( int nIndex )
{
    if( m_vecStageInfo.empty() ) { 
        return std::string();
    }

    if( static_cast<int>( m_vecStageInfo.size() ) <= nIndex ) { 
        return std::string();
    }

    return m_vecStageInfo[nIndex];
}

int KGCInfinityDungeon::GetStageNum( void )
{
    return static_cast<int>( m_vecStageInfo.size() );
}

void KGCInfinityDungeon::GetAllMonsterList( std::set<int>& setMonsterList )
{
    setMonsterList.clear();

    std::map<int,SBossRankGroup>::iterator iter = m_mapBossRankGroup.begin();
    for ( ; iter != m_mapBossRankGroup.end(); ++iter )
    {
        SBossRankGroup& sBossRankGroup = iter->second;
                
        for ( int i = 0; i < (int)sBossRankGroup.m_vecBossList.size(); ++i )
        {
            setMonsterList.insert( sBossRankGroup.m_vecBossList[i] );
        }
    }

    for ( int i = 0; i < (int)m_vecStartMonsterGroup.size(); ++i )
    {
        setMonsterList.insert( m_vecStartMonsterGroup[i] );
    }

    std::map<int,SMonsterRankGroup>::iterator iterRank = m_mapMonsterRankGroup.begin();
    for ( ; iterRank != m_mapMonsterRankGroup.end(); ++iterRank )
    {
        SMonsterRankGroup& sMonRankGroup = iterRank->second;

        for ( int i = 0; i < (int)sMonRankGroup.m_vecMonsterGroup.size(); ++i )
        {
            setMonsterList.insert( sMonRankGroup.m_vecMonsterGroup[i] );
        }
    }

    return;
}

