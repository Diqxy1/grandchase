#include "StdAfx.h"
#include ".\trigger.h"

CTrigger::CTrigger(void)
{
    m_bRepeat                   = true;
    m_bStartWithActive          = false;
    m_bActive                   = 0;
    SetComplete( false );
    m_vtEvent.clear();
    m_vtCondition.clear();
    m_vtAction.clear();
}

CTrigger::~CTrigger(void)
{
    for(int i=0;i<(int)m_vtCondition.size();i++)
        SAFE_DELETE(m_vtCondition[i]);
    m_vtCondition.clear();

    for(int i=0;i<(int)m_vtEvent.size();i++)
        SAFE_DELETE(m_vtEvent[i]);
    m_vtEvent.clear();

    for(int i=0;i<(int)m_vtAction.size();i++)
        SAFE_DELETE(m_vtAction[i]);
    m_vtAction.clear();
}

void CTrigger::FrameMove(void)
{
    if( IsComplete() || !m_bActive ) return;

    //************************************************************************
    // 이벤트 트리거 실행.
    //************************************************************************
    EventTrigger();

    //************************************************************************
    // 컨디션들의 이벤트 진행
    // 컨디션들이 모두 완료 되었다면 Action으 실행한다.
    //************************************************************************
    for(int i=0;i<(int)m_vtCondition.size();i++)
        m_vtCondition[i]->FrameMove();

    for(int i=0;i<(int)m_vtCondition.size();i++)
    {
        if( !m_vtCondition[i]->IsComplete() )
            return;
    }

    ActionTrigger();


    //************************************************************************
    // 액션과 이벤트가 완료 되면 트리거를 완료시킨다...
    //************************************************************************
    for(int i=0;i<(int)m_vtEvent.size();i++)
    {
        if( !m_vtEvent[i]->IsComplete() )
            return;
    }
    for(int i=0;i<(int)m_vtAction.size();i++)
    {
        if( !m_vtAction[i]->IsComplete() )
            return;
    }

    SetComplete( true );
}

void CTrigger::AddEventTrigger(CTriggerCondition *cdt)
{
    //************************************************************************
    // 이벤트 트리거 추가
    //************************************************************************
    if( cdt ) m_vtEvent.push_back(cdt);
}

void CTrigger::AddCondition(CTriggerCondition *cdt)
{
    //************************************************************************
    // 트리거에 컨디션 추가.
    //************************************************************************
    if( cdt ) m_vtCondition.push_back(cdt);
}

void CTrigger::AddActionTrigger(CTriggerCondition *cdt)
{
    //************************************************************************
    // 트리거에 액션 추가
    //************************************************************************
    if( cdt ) m_vtAction.push_back(cdt);
}

void CTrigger::StartTrigger(void)
{
    //************************************************************************
    // 트리거들 초기화
    //************************************************************************
    for(int i=0;i<(int)m_vtEvent.size();i++) m_vtEvent[i]->StartTrigger();
    for(int i=0;i<(int)m_vtCondition.size();i++) m_vtCondition[i]->StartTrigger();
    for(int i=0;i<(int)m_vtAction.size();i++) m_vtAction[i]->StartTrigger();

    if( m_bStartWithActive )
        m_bActive = true;
    else
        m_bActive = false;

    CTriggerCondition::StartTrigger();
}

bool CTrigger::CheckWithConditionToStart(CTriggerCondition* pConditon)
{
    if( IsComplete() || !m_bActive ) return false;

    //************************************************************************
    // 실행대기중인 컨디션과 일치하는 컨디션이 있나 확인합니다.
    //************************************************************************
    for(int i=0;i<(int)m_vtEvent.size();i++)
    {
        if( m_vtEvent[i]->CheckEqualCondition(pConditon) )
            return true;
    }

    for(int i=0;i<(int)m_vtCondition.size();i++)
    {
        if( m_vtCondition[i]->CheckEqualCondition(pConditon) )
            return true;
    }

    for(int i=0;i<(int)m_vtAction.size();i++)
    {
        if( m_vtAction[i]->CheckEqualCondition(pConditon) )
            return true;
    }

    return false;
}

void CTrigger::ForceStartWithOutCheck(void)
{
    //************************************************************************
    // 실행대기중인 컨디션을 확인하지 않고 바로 트리거 실행.
    //************************************************************************
    for(int i=0;i<(int)m_vtEvent.size();i++)
        m_vtEvent[i]->m_bCheckByHost = 1;
    for(int i=0;i<(int)m_vtCondition.size();i++)
        m_vtCondition[i]->m_bCheckByHost = 1;
    for(int i=0;i<(int)m_vtAction.size();i++)
        m_vtAction[i]->m_bCheckByHost = 1;
}

void CTrigger::EventTrigger(void)
{
    //************************************************************************
    // 게임 중 실행되는 이벤트 트리거
    //************************************************************************
    for(int i=0;i<(int)m_vtEvent.size();i++)
        m_vtEvent[i]->FrameMove();
}

void CTrigger::ActionTrigger(void)
{
    //************************************************************************
    // 조건을 완료 하였을 때 실행되는 이벤트 트리거
    //************************************************************************
    for(int i=0;i<(int)m_vtAction.size();i++)
        m_vtAction[i]->FrameMove();
}

void CTrigger::EndTrigger(void)
{
    //************************************************************************
    // 종료 시 종료 부분 다 돌려 줍니다.
    //************************************************************************

    for(int i=0;i<(int)m_vtCondition.size();i++)
    {
        m_vtCondition[i]->EndTrigger(); 
    }
}

CTriggerCondition *CTrigger::FindTriggerConditonByID(const int iTG_ID)
{
    for(int i=0;i<(int)m_vtEvent.size();++i)
    {
        if( iTG_ID == m_vtEvent[i]->m_iTG_ID )
            return m_vtEvent[i];
    }

    for(int i=0;i<(int)m_vtCondition.size();++i)
    {
        if( iTG_ID == m_vtCondition[i]->m_iTG_ID )
            return m_vtCondition[i];
    }

    for(int i=0;i<(int)m_vtAction.size();++i)
    {
        if( iTG_ID == m_vtAction[i]->m_iTG_ID )
            return m_vtAction[i];
    }

    return NULL;
}

void CTrigger::GetTriggerConditionByType(CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition)
{
    for(int i=0;i<(int)m_vtEvent.size();++i)
    {
        if( enTG_Type == m_vtEvent[i]->enType )
            vtTGCondition.push_back(m_vtEvent[i]);
    }

    for(int i=0;i<(int)m_vtCondition.size();++i)
    {
        if( enTG_Type == m_vtCondition[i]->enType )
            vtTGCondition.push_back(m_vtCondition[i]);
    }

    for(int i=0;i<(int)m_vtAction.size();++i)
    {
        if( enTG_Type == m_vtAction[i]->enType )
            vtTGCondition.push_back(m_vtAction[i]);
    }
}

int CTrigger::GetCntOfTriggerType(enTriggerCondition enTG_ID)
{
    int iCnt = 0;
    for(int i=0;i<(int)m_vtEvent.size();++i)
    {
        if( enTG_ID == m_vtEvent[i]->enType )
            ++iCnt;
    }

    for(int i=0;i<(int)m_vtCondition.size();++i)
    {
        if( enTG_ID == m_vtCondition[i]->enType )
            ++iCnt;
    }

    for(int i=0;i<(int)m_vtAction.size();++i)
    {
        if( enTG_ID == m_vtAction[i]->enType )
            ++iCnt;
    }

    return iCnt;
}