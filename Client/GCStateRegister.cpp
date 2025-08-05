#include "stdafx.h"
#include ".\gcstateregister.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
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

KGCStateRegister::KGCStateRegister(void)
{
}

KGCStateRegister::~KGCStateRegister(void)
{
}

bool KGCStateRegister::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{		
	return true;
}

bool KGCStateRegister::FrameMove(float fElapsedTime)
{
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();
    return true;
}
bool KGCStateRegister::Render()
{
	g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);

    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        EndScene();
    }

    return true;
}
bool KGCStateRegister::OnInit()
{
    g_KMci.Stop();
	g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap(true);

	g_MyD3D->m_pStateMachine->Clear_SavedState();

	g_pkUIScene->m_pkRegisterScene->EnterScene();

    KP2P::GetInstance()->Reset();
    g_pkUIScene->m_pkRegisterScene->ConnectLoginServer();
    return true;
}

bool KGCStateRegister::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateRegister::OnSetupFSM()
{
	ON_SETUP_FSM( GS_SERVER, FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN, FAIL_N_SHUTDOWN );

	m_eStateForFail = GS_REGISTER;
}

void KGCStateRegister::OnDestroy()
{
}
