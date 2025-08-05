#include "stdafx.h"

#ifndef DISABLE_MAP_SHOP // Kawan>
//
#include "GCStateGPShop.h"
#include "MyD3D.h"

#include "GCStateMachine.h"
//
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
//
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCMyInfoScene.h"
//
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCInviteManager.h"
#include "GCRenderManager.h"
//
#include "message.h"

KGCStateGPShop::KGCStateGPShop(void)
{
}

KGCStateGPShop::~KGCStateGPShop(void)

{
}

bool KGCStateGPShop::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );
	return true;
}

bool KGCStateGPShop::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    SiKGCInviteManager()->FrameMove();

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();

	return true;
}

bool KGCStateGPShop::Render()
{
    g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);

    if ( BeginScene() )
    {

        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();

        g_ParticleManager->Render(GC_LAYER_UI);
        g_ParticleManager->Render( GC_LAYER_CUSTOM );
        
        EndScene();
    }

	return true;
}

bool KGCStateGPShop::OnInit()
{
#ifdef GAME_CASH
	KP2P::GetInstance()->Send_CurrentCashPoint();
#endif

#if defined( NATION_USA ) || defined( NATION_EU )
	g_kGlobalValue.UpdateCashData();
#endif

    // 첫구매 유저 보너스GC 지급비율 요청
    KP2P::GetInstance()->Send_CashbackExtraRatioInfoReq();


	g_pkUIScene->m_pkGPShopScene->EnterScene();

	//g_pkUIScene->m_pkGPShopScene->SetButtonLock( BTN_LOBBY , !g_MyD3D->m_pStateMachine->IsJoinedChannel() );

    g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_SHOP );
	return true;
}

bool KGCStateGPShop::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateGPShop::OnSetupFSM()
{
	ON_SETUP_FSM( GS_LOGIN,          GS_SERVER );
    ON_SETUP_FSM( GS_ROOM,          GS_SERVER );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
    ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_GUILD,         GS_SERVER );
    ON_SETUP_FSM( GS_MY_INFO,       GS_SERVER );
    ON_SETUP_FSM( GS_SQUARE_LOADING,GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_CHAR_SELECT ,   GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_SERVER );
    
#if defined USE_COORDI_SHOP
    ON_SETUP_FSM( GS_COORDI_SHOP,   GS_SERVER );
#endif

#if defined( USE_AGIT_SYSTEM )
    ON_SETUP_FSM( GS_GAME_LOADING, GS_SERVER );
#endif

    m_eStateForFail = GS_SERVER;
}

void KGCStateGPShop::OnDestroy()
{
    g_pkUIScene->m_pkGPShopScene->LeaveScene();
    g_pkUIScene->HideSpeechBubbleMsg();
}
#endif