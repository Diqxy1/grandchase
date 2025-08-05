#include "stdafx.h"
//
#include ".\gcstatemyinfofromroom.h"

#include "MyD3D.h"
#include "GCStateMachine.h"

#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCItemInfoBox.h"
//
//

#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
//
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCMyInfoScene.h"
//
#include "Controls.h"


#include "GCRenderManager.h"

GCStateMyInfoFromRoom::GCStateMyInfoFromRoom(void)
{
}

GCStateMyInfoFromRoom::~GCStateMyInfoFromRoom(void)
{
}

bool GCStateMyInfoFromRoom::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{	
	g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );	
	return true;
}

bool GCStateMyInfoFromRoom::FrameMove(float fElapsedTime)
{
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();

    // 내가 자면
    if( true == g_MyD3D->MyCtrl->m_bSleep )
    {
		g_MyD3D->m_pStateMachine->ExitRoom( true );
        g_MyD3D->MyCtrl->NoSleep();
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_SLEEP_OUT ), KGCChatManager::CHAT_TYPE_ADMIN );
    }

	return true;
}

bool GCStateMyInfoFromRoom::Render()
{
    g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);

    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();


	    g_ParticleManager->Render(GC_LAYER_UI);
	    g_ParticleManager->Render(GC_LAYER_CUSTOM);
        EndScene();
    }
	return true;
}

bool GCStateMyInfoFromRoom::OnInit()
{
	KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_MYINFO );

#if defined( NATION_USA ) || defined( NATION_EU )
	g_kGlobalValue.UpdateCashData();
#endif

    // WONYWONY
	//g_pkUIScene->m_pkChannelScene->ToggleRender( false );
	//g_pkUIScene->m_pkLobbyScene->ToggleRender( false );
    g_pkUIScene->m_pkMyInfoScene->Create();
	g_pkUIScene->m_pkMyInfoScene->EnterScene();

    g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_SHOP );

	return true;
}

void GCStateMyInfoFromRoom::OnDestroy()
{
	//g_MyD3D->SendEquipItemList();
    g_MyD3D->SendEquipItemList( &g_MyD3D->m_TempPlayer );
	g_MyD3D->GetMyShortCutSlot()->SendQuickSlotinfo();
	g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();	
    g_pkUIScene->m_pkMyInfoScene->LeaveScene();
    g_pkUIScene->HideSpeechBubbleMsg();
}

void GCStateMyInfoFromRoom::OnSetupFSM()
{
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	ON_SETUP_FSM( GS_ROOM ,         GS_MATCH );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
	ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_CHAR_SELECT ,   GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_SERVER );

	m_eStateForFail = GS_SERVER;
}

bool GCStateMyInfoFromRoom::OnDestroyBeforeStateChange()
{
	return true;
}
