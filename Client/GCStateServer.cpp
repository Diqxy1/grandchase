#include "stdafx.h"
//
#include "GCStateServer.h"
#include "MyD3D.h"

#include "GCStateMachine.h"
#include "KMci.h"
//
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameDlg.h"
#include "GCUI/KGCLoginScene.h"
#include "GCUI/KGCRegisterScene.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCNetwork.h"
#include "KGCChannelManager.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "GCUI/GCInventoryFullMsg.h"

KGCStateServer::KGCStateServer( void )
{
}

KGCStateServer::~KGCStateServer( void )
{
}

bool KGCStateServer::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    if( g_MyD3D->m_pStateMachine->IsJoinedGuild() )
    {
        // 서버에 길드방 퇴장을 알린다.
        if ( FAILED( g_MyD3D->LeaveGuildRoom() ) )
            return false;

        g_MyD3D->m_pStateMachine->SetJoinGuild( false );
    }

    // 채널에 접속된 상태라면 채널 퇴장을 알린다.
    if( g_MyD3D->m_pStateMachine->IsJoinedChannel() )
    {
        if ( FAILED( g_MyD3D->LeaveChannel() ) )
            return false;

        g_MyD3D->m_pStateMachine->SetJoinChannel( false );

        SiKGCChannelManager()->SetCurrentChannel( -1 );
    }

	g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );
    return true;
}

bool KGCStateServer::FrameMove( float fElapsedTime )
{
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();

    return true;
}

bool KGCStateServer::Render()
{
    g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );

    if ( BeginScene() )
    {

        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();

        //g_ParticleManager->Render( GC_LAYER_UI );
        
        EndScene();
    }

    return true;
}

bool KGCStateServer::OnInit()
{
    g_MyD3D->m_pStateMachine->Clear_SavedState();

    /*if( g_KMci.GetTrack() != GC_BGM_MAIN_THEME_LYRICS )
        g_KMci.Play( GC_BGM_MAIN_THEME_LYRICS, true );*/
    

    g_pkUIScene->m_pkServerScene->EnterScene();
	
	//bool bConnectGameServer = SiKP2P()->IsConnectedToGameServer(); //g_kGlobalValue.ServerInfo.dwServerType != ST_FORCE_DWORD;

    ShutDown( false );	

	if ( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME &&
		SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
	{
        g_kGlobalValue.SetPlayerSkillInfo( &g_MyD3D->m_TempPlayer, false, true, true );		
	}
	
    return true;
}

bool KGCStateServer::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateServer::OnDestroy()
{
	g_pkUIScene->m_pkServerScene->LeaveScene();
}

void KGCStateServer::OnSetupFSM()
{
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
    ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_SHOP_CASH,          GS_SERVER );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,          GS_SERVER );
#endif
    ON_SETUP_FSM( GS_MY_INFO,       GS_SERVER );
    ON_SETUP_FSM( GS_TUTORIAL,      GS_SERVER );
    ON_SETUP_FSM( GS_REPLAY,        GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT,        GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_GAIN,        GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );

	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
    ON_SETUP_FSM( GS_REGISTER,      GS_SERVER );

    m_eStateForFail = FAIL_N_SHUTDOWN;
}
