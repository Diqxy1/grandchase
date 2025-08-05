#include "StdAfx.h"
#include "TGWin.h"
#include "QuestGameManager.h"


#include "Headup Display.h"

#include "Monster.h"

#include "gcui/GCGameOverlayUI.h"

CTGWin::CTGWin()
{
    enType              = TG_WIN;
}

CTGWin::~CTGWin()
{

}

//************************************************************************
// 승리처리
//************************************************************************
void CTGWin::Win(void)
{

    g_pkQuestManager->DoStageClearProcess();
    
    g_pkQuestManager->m_bWin        = true;
    g_pkQuestManager->m_bEndGame    = true;
    g_pkQuestManager->m_bClearStage = true;
    
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(g_MyD3D->Get_MyPlayer(), mit->first) && pMonster->IsInvincibleNoFrame() == false)
			pMonster->m_fHP = 0.0f;

	}

    for(int i=0;i<MAX_PLAYER_NUM;++i)
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm( true );
            g_MyD3D->MyPlayer[i]->Direct_Motion_Input( g_MyD3D->MyPlayer[i]->GetCurFormTemplate().WIN );
        }
    }
    
    SetComplete( true );

#if defined(NATION_LATINAMERICA)
    g_MyD3D->MyHead->Start_Text( "COMPLETADO" );
#else
    g_MyD3D->MyHead->Start_Text( "COMPLETE" );
#endif
}

//************************************************************************
// 승리처리 진행
//************************************************************************
void CTGWin::FrameMove(void)
{
    if( IsComplete() == true ) return;
    if( g_pkGameOverlayUI->IsShowDialogue() ) return;       // 다이얼로그 표시중이라면 게임을 끝내지 않는다.

    if( g_kGlobalValue.m_kUserInfo.bHost
        && g_pkQuestManager->IsCompleteEndGameDelayTime()
        )
    {
        Win();

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
        Win();
    }
}

HRESULT CTGWin::LoadItemFromLua(KLuaManager &luaMgr)
{
    return S_OK;
}
