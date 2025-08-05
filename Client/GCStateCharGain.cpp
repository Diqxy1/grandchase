#include "stdafx.h"
//
#include ".\GCStateCharGain.h"
#include "MyD3D.h"





#include "GCStateMachine.h"
//
#include "GCUI/KGCLoginScene.h"
#include "GCUI/KGCRegisterScene.h"
#include "GCUI/GCOnGameDlg.h"
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCLoadingScene.h"
#include "GCUI/GCSquareLoadingScene.h"

#include "gcui/GCNickNameBox.h"
#include "GCUI/GCInventoryFullMsg.h"

KGCStateCharGain::KGCStateCharGain(void)
{
}

KGCStateCharGain::~KGCStateCharGain(void)
{
}

bool KGCStateCharGain::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{		
	g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );
	return true;
}

bool KGCStateCharGain::FrameMove(float fElapsedTime)
{
    //g_KDSound.LoadByStep();
	g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
	g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();
	return true;
}
bool KGCStateCharGain::Render()
{
	g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );
	if ( BeginScene() )
	{
		g_MyD3D->RenderUI();
		g_MyD3D->RenderFPS();

		g_ParticleManager->Render( GC_LAYER_UI );
		EndScene();
	}

	return true;
}
bool KGCStateCharGain::OnInit()
{

	g_pkUIScene->m_pkCharGainScene->EnterScene();

	return true;
}

bool KGCStateCharGain::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateCharGain::OnSetupFSM()
{
    ON_SETUP_FSM( GS_LOGIN,      GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_REGISTER,   GS_CHAR_GAIN );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_MATCH,         GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_SHOP_CASH,          GS_CHAR_GAIN );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,          GS_CHAR_GAIN );
#endif
	ON_SETUP_FSM( GS_MY_INFO,       GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_SERVER,        GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_CHAR_SELECT,   GS_CHAR_GAIN );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_CHAR_GAIN );

	ON_SETUP_FSM( GS_CHAR_SELECT,        FAIL_N_SHUTDOWN );

	m_eStateForFail = GS_SERVER;
	
}

void KGCStateCharGain::OnDestroy()
{
	g_pkUIScene->m_pkCharGainScene->LeaveScene();
}
