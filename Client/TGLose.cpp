#include "StdAfx.h"
#include "TGLose.h"
#include "QuestGameManager.h"


#include "MyD3D.h"
#include "Headup Display.h"
#include "gcui/GCGameOverlayUI.h"

CTGLose::CTGLose()
{
    enType              = TG_LOSE;
}

CTGLose::~CTGLose()
{

}

//************************************************************************
// 패배처리
//************************************************************************
void CTGLose::Lose(void)
{
    g_pkQuestManager->m_bWin = false;
    g_pkQuestManager->m_bEndGame = true;

#if defined(NATION_LATINAMERICA)
    g_MyD3D->MyHead->Start_Text( "FALLASTE" );
#else
    g_MyD3D->MyHead->Start_Text( "FAILED" );
#endif
    SetComplete( true );
}

//************************************************************************
// 패배처리 진행
//************************************************************************
void CTGLose::FrameMove(void)
{
    if( IsComplete() == true ) return;
    if( g_pkGameOverlayUI->IsShowDialogue() ) return;       // 다이얼로그 표시중이라면 게임을 끝내지 않는다.

    if( g_kGlobalValue.m_kUserInfo.bHost
        && g_pkQuestManager->IsCompleteEndGameDelayTime()
        )
    {
        Lose();

        if( !m_bSendPacket )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        Lose();
    }
}

HRESULT CTGLose::LoadItemFromLua(KLuaManager &luaMgr)
{
    return S_OK;
}
