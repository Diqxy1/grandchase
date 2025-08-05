#include "stdafx.h"
#include "GCStateSquare.h"
//
#include "MyD3D.h"

#include "./Square/GCSquare.h"
#include "GCCameraOrtho.h"
//
#include "GCUI/KGCSquareOverlayUI.h"
#include "GCUI/GCLoadingScene.h"
#include "GCUI/GCServerScene.h"

#include "SquareScene.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KStats.h"
#include "KGCInviteManager.h"
#include "GCRenderManager.h"

KGCStateSquare::KGCStateSquare(void)
{
}

KGCStateSquare::~KGCStateSquare(void)
{
}

bool KGCStateSquare::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    g_kCamera.InitSquareCamera();
    return true;
}

bool KGCStateSquare::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    SiKGCInviteManager()->FrameMove();

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_MyD3D->GetMyShortCutSlot()->FrameMove();

    g_KDSound.FrameMove();
    return true;
}

bool KGCStateSquare::Render()
{
    // [1/7/2008] breadceo. square rough
    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();
        EndScene();
    }
    return true;
}

bool KGCStateSquare::OnInit()
{
    g_pkUIScene->m_pkSquareScene->EnterScene();
    g_MyD3D->Story = 0;
    StartGameTime = 0;

	SiKGCRenderManager()->SetOutlineWidth( OUT_LINE_WIDTH_IN_GAME );

#ifndef __PATH__
    g_kStatsManager.InsertPage( L"SQUARE" );
    {
        g_kStatsManager.InsertValue( L"RenderSquarePeopleData", &g_kGlobalValue.m_bRenderSquarePeopleData );
        g_kStatsManager.InsertValue( L"FrameLock", &g_kGlobalValue.m_bFrameLock );
    }
#endif

	//KP2P::GetInstance()->Send_CheckPollTerm();
    return true;
}

bool KGCStateSquare::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateSquare::OnDestroy()
{
	if(!SiKGCInviteManager()->m_bInviteAccept)
	{
		SiKP2P()->DisConnectTCPRelay();
	}
    g_pkUIScene->m_pkSquareOverlayUI->ToggleRender( false );
    SiGCSquare()->ClearResourceOnSquare();
    // 미니게임 덕분에 날라갔음.. 
    //SiKP2P()->Send_LeaveSquareReq();
	
}

void KGCStateSquare::OnSetupFSM()
{
    ON_SETUP_FSM( GS_ROOM,          GS_SERVER ); // 초대 때문에.
    ON_SETUP_FSM( GS_SQUARE_END,    GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN);
    ON_SETUP_FSM( GS_GAME_LOADING,  GS_SERVER );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
    m_eStateForFail = GS_SERVER;
}

