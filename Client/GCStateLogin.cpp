#include "stdafx.h"
//
#include ".\gcstatelogin.h"
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

KGCStateLogin::KGCStateLogin(void)
{
}

KGCStateLogin::~KGCStateLogin(void)
{
}

bool KGCStateLogin::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{		
	return true;
}

bool KGCStateLogin::FrameMove(float fElapsedTime)
{
    //g_KDSound.LoadByStep();
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();
    return true;
}
bool KGCStateLogin::Render()
{
	g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);

    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        EndScene();
    }

    return true;
}
bool KGCStateLogin::OnInit()
{
    // 로긴 할 때 이것저것 리셋 해 줘야한다 

	// 접속 끊고
    //KP2P::ReleaseInstance();	

    g_KMci.Stop();  //  배경 사운드도 스탑하자.
	g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap(true); // 월드맵 상태를 지워주자 



	g_MyD3D->m_pStateMachine->Clear_SavedState();

	g_pkUIScene->m_pkLoginScene->EnterScene();

    KP2P::GetInstance()->Reset();
    g_pkUIScene->m_pkLoginScene->ConnectLoginServer();
    return true;
}

bool KGCStateLogin::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateLogin::OnSetupFSM()
{
	ON_SETUP_FSM( GS_SERVER,        GS_LOGIN );
    ON_SETUP_FSM( GS_REGISTER,      FAIL_N_SHUTDOWN );

	m_eStateForFail = GS_LOGIN;
}

void KGCStateLogin::OnDestroy()
{
}
