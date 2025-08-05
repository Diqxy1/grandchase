#include "StdAfx.h"
#include ".\gccondition.h"

KGCCondition::KGCCondition(void)
{  
}

KGCCondition::~KGCCondition(void)
{
}

void KGCCondition::SetConditionInfo( SConditionInfo sConditionInfo )
{
    m_sCondition = sConditionInfo;
}

void KGCCondition::MonsterKill( int iMonsterID ) //몬스터를 죽여야 하는 조건을 건다.
{
    m_sCondition.m_iMonsterID = iMonsterID;
}

void KGCCondition::GameModeClear( int iGameMode, bool bWin , std::pair<int, int> pairDifficultRange )
{
    m_sCondition.m_iGameMode = iGameMode;
    m_sCondition.m_bWin = bWin;
    m_sCondition.m_pairDifficultRange = pairDifficultRange;
}

void KGCCondition::ArriveSpecialStage( int iGameMode, int iStage, std::pair<int, int> pairDifficultRange )
{
    m_sCondition.m_iGameMode = iGameMode;
    m_sCondition.m_iStage = iStage;
    m_sCondition.m_pairDifficultRange = pairDifficultRange;
}

void KGCCondition::SetTimeLimit( int iTime )
{
    m_sCondition.m_iTime = iTime;
}

void KGCCondition::SetKillLimit( int iKill )
{
    m_sCondition.m_iKill = iKill;
}

void KGCCondition::SetDieLimit( int iDie )
{
    m_sCondition.m_iDie = iDie;
}

void KGCCondition::SetExLifeLimit( int iExLife )
{
    m_sCondition.m_iExLife = iExLife;
}

void KGCCondition::SetConditionRatio( float fRatio )
{
    m_sCondition.m_fConditionRatio = fRatio;
}