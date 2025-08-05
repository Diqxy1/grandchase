#include "stdafx.h"
//
#include ".\gcstatemyinfo.h"

#include "MyD3D.h"


#include "GCStateMachine.h"

#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
//
//

#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
//
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCMyInfoScene.h"


#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCInviteManager.h"
#include "GCRenderManager.h"

KGCStateMyInfo::KGCStateMyInfo(void)
{
}

KGCStateMyInfo::~KGCStateMyInfo(void)
{
}

bool KGCStateMyInfo::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
    g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );
    return true;
}

bool KGCStateMyInfo::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    SiKGCInviteManager()->FrameMove();

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();

    return true;
}

bool KGCStateMyInfo::Render()
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

bool KGCStateMyInfo::OnInit()
{
#if defined( NATION_USA ) || defined( NATION_EU )
    if ( false == g_kGlobalValue.m_bIsGetWebCashOnStateMyInfoFirstInit ) {
        g_kGlobalValue.UpdateCashData();
        g_kGlobalValue.m_bIsGetWebCashOnStateMyInfoFirstInit = true;
    }
#endif
    g_pkUIScene->m_pkMyInfoScene->EnterScene();

    g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_SHOP );

    return true;
}

void KGCStateMyInfo::OnDestroy()
{
    //g_MyD3D->SendEquipItemList();
    g_MyD3D->SendEquipItemList( &g_MyD3D->m_TempPlayer );
	g_MyD3D->GetMyShortCutSlot()->SendQuickSlotinfo();
    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
    g_pkUIScene->m_pkMyInfoScene->LeaveScene();
    g_pkUIScene->HideSpeechBubbleMsg();
}

bool KGCStateMyInfo::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateMyInfo::OnSetupFSM()
{
    ON_SETUP_FSM( GS_ROOM,          GS_SERVER );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
    ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_GUILD,         GS_SERVER );
    ON_SETUP_FSM( GS_SHOP_CASH,          GS_SERVER );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,          GS_SERVER );
#endif
    ON_SETUP_FSM( GS_SQUARE_LOADING,GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT ,   GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_SERVER );
    
#if defined USE_COORDI_SHOP
    ON_SETUP_FSM( GS_COORDI_SHOP,    GS_SERVER );
#endif

#if defined( USE_AGIT_SYSTEM )
    ON_SETUP_FSM( GS_GAME_LOADING, GS_SERVER );
#endif

    m_eStateForFail = GS_SERVER;
}
