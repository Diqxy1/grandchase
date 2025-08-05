#include "StdAfx.h"
#include ".\gcsubmission.h"

KGCSubMission::KGCSubMission(void)
{
}

KGCSubMission::~KGCSubMission(void)
{
}

void KGCSubMission::AddCondition( KGCCondition& kCondition )
{
    m_sSubMissionInfo.m_vecConditionID.push_back( kCondition.GetConditionInfo().m_dwID );
}

void KGCSubMission::SetAction( int iCount, DWORD dwItemID , bool bMonDrop )
{
    m_sSubMissionInfo.m_sAction.m_iCount = iCount;
    m_sSubMissionInfo.m_sAction.m_dwItemID = dwItemID;
    m_sSubMissionInfo.m_sAction.m_bMonDrop = bMonDrop;
}

void KGCSubMission::SetCompletion( int iCount, DWORD dwItemID )
{
    m_sSubMissionInfo.m_sCompletion.m_iCount = iCount;
    m_sSubMissionInfo.m_sCompletion.m_dwItemID = dwItemID;
}
