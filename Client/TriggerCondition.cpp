#include "StdAfx.h"
#include ".\TriggerCondition.h"
//
//
#include "EveryTriggerManager.h"

//************************************************************************
// 컨디션들의 아이디 관련 변수와 함수
//************************************************************************
int g_iTG_ID            = 0;
short g_sTG_MaxQuestID    = 0;
void InitTG_ID(void) { g_iTG_ID = 0; }
void SetMaxQuestID(int iMaxQuestID) { if( iMaxQuestID > g_sTG_MaxQuestID ) g_sTG_MaxQuestID = iMaxQuestID; }


//************************************************************************
// 변수 초기화, 컨디션 아이디 증가
//************************************************************************
CTriggerCondition::CTriggerCondition()
{
    enType          = TG_DEFAULT;
    m_bCheckByHost  = false;
    SetComplete( false );
    m_bSendPacket   = false;
    m_bRepeat       = false;
    m_iTG_ID        = 0;
    //SetConditionID(g_iTG_ID++);
}

CTriggerCondition::~CTriggerCondition()
{

}

//************************************************************************
// 트리거의 ID를 설정합니다.
//************************************************************************
void CTriggerCondition::SetConditionID(int iTG_ID)
{
    m_iTG_ID = iTG_ID;
}

//************************************************************************
// 트리거를 초기화 합니다.
//************************************************************************
void CTriggerCondition::StartTrigger(void)
{
    if( m_bRepeat )
    {
        m_bCheckByHost      = false;
        m_bSendPacket       = false;
        SetComplete( false );
    }
}

//************************************************************************
// 실행대기중인 컨디션과 동일한 컨디션인지 체크합니다.
//************************************************************************
bool CTriggerCondition::CheckEqualCondition(CTriggerCondition *pCondition)
{
    if( !pCondition ) return false;
    //if( pCondition->enType != enType ) return false;

    if( pCondition->m_sMaxTG_ID != g_sTG_MaxQuestID )
    {
        g_kEveryTriggerManager.SetNotMatchTrigger( true );

        if( !g_pkUIScene->IsMsgBoxModal() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME,
                g_pkStrLoader->GetString( STR_ID_NOT_EQUAL_QUEST_INFO_1 ),
                g_pkStrLoader->GetString( STR_ID_NOT_EQUAL_QUEST_INFO_2 ),
                KGCUIScene::GC_MBOX_USE_END_GAME, 0, 0, 0, 1 );
        }
        return false;
    }

    if( pCondition->m_iTG_ID == m_iTG_ID )
        m_bCheckByHost = true;
    else
        return false;

    return true;
}

//************************************************************************
// 트리거 종료시에 후촐되는 함수
//************************************************************************
void CTriggerCondition::EndTrigger(void)
{

}

//************************************************************************
// 트리거 발동중에 호출되는 함수
//************************************************************************
void CTriggerCondition::FrameMove(void)
{

}

//************************************************************************
// 컨디션 조건이 완료 되었나?
//************************************************************************
bool CTriggerCondition::IsComplete(void)
{
    return m_bComplete;
}

void CTriggerCondition::SetComplete(bool bComplete)
{
    if( bComplete == true )
    {
        EndTrigger();
    }

    m_bComplete = bComplete;
}