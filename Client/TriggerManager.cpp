#include "StdAfx.h"
#include ".\triggermanager.h"

CTriggerManager::CTriggerManager(void)
{
    m_bActive       = false;
    m_bComplete     = false;
}

CTriggerManager::~CTriggerManager(void)
{
    ClearTrigger();
}

void CTriggerManager::FrameMove(void)
{
    if( !m_bActive || m_bComplete ) return;

    //************************************************************************
    // 트리거들을 실행 시킨다.
    //************************************************************************
    int iTotalTGCnt = (int)m_vtTriggerList.size();
    int iTGCnt = 0;
    std::vector<CTrigger*>::iterator itTG = m_vtTriggerList.begin();
    while( itTG != m_vtTriggerList.end() )
    {
        CTrigger *pTrigger = (*itTG);
        if( pTrigger )
        {
            if( pTrigger->IsActive() )
            {
                pTrigger->FrameMove();
                if( pTrigger->IsComplete() )
                    iTGCnt++;
            }
        }
        itTG++;
    }
    if( iTotalTGCnt == iTGCnt ) m_bComplete = true;
}

void CTriggerManager::AddTrigger(CTrigger *pTrigger)
{
    if( pTrigger ) m_vtTriggerList.push_back(pTrigger);
}

void CTriggerManager::ClearTrigger(void)
{
    //************************************************************************
    // 트리거를 초기화한다.
    //************************************************************************
    std::vector<CTrigger*>::iterator itTG = m_vtTriggerList.begin();
    while( itTG != m_vtTriggerList.end() )
    {
        SAFE_DELETE((*itTG));
        itTG++;
    }
    m_vtTriggerList.clear();
}

CTrigger *CTriggerManager::FindTrigger(std::string &strTGName)
{
    //************************************************************************
    // strTGName 의 이름을 가진 트리거를 찾는다.
    //************************************************************************
    for(int i=0;i<(int)m_vtTriggerList.size();i++)
    {
        if( m_vtTriggerList[i]->m_strTGName == strTGName )
            return m_vtTriggerList[i];
    }

    return NULL;
}


bool CTriggerManager::CheckWithConditionForStart(CTriggerCondition *pCondition)
{
    //************************************************************************
    // Host로 부터 받은 컨디션 중에 실행 할 수 있는 것이 있느 확인
    //************************************************************************
    std::vector<CTrigger*>::iterator itTG = m_vtTriggerList.begin();
    while( itTG != m_vtTriggerList.end() )
    {
        CTrigger *pTrigger = (*itTG);
        if( pTrigger )
        {
            if( pTrigger->IsActive() )
            {
                if( pTrigger->CheckWithConditionToStart(pCondition) )
                    return true;
            }
        }
        itTG++;
    }

    return false;
}

void CTriggerManager::StartTrigger(void)
{
    m_bComplete = false;
    
    for(int i=0;i<(int)m_vtTriggerList.size();++i)
        m_vtTriggerList[i]->StartTrigger();
}

CTriggerCondition *CTriggerManager::FindTriggerConditonByID(const int iTG_ID)
{
    for(int i=0;i<(int)m_vtTriggerList.size();++i)
    {
        CTriggerCondition *pTCondition = m_vtTriggerList[i]->FindTriggerConditonByID(iTG_ID);
        if( pTCondition ) return pTCondition;
    }

    return NULL;
}

void CTriggerManager::GetTriggerConditionByType(CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition)
{
    for(int i=0;i<(int)m_vtTriggerList.size();++i)
    {
        m_vtTriggerList[i]->GetTriggerConditionByType(enTG_Type, vtTGCondition);
    }
}

int CTriggerManager::GetCntOfTriggerType(CTrigger::enTriggerCondition enTG_Type)
{
    int iCnt = 0;
    for(int i=0;i<(int)m_vtTriggerList.size();++i)
    {
        iCnt += m_vtTriggerList[i]->GetCntOfTriggerType(enTG_Type);
    }

    return iCnt;
}