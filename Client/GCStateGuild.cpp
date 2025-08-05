#include "stdafx.h"
//
#include "GCStateGuild.h"
#include "MyD3D.h"

#include "GCUI/GCChatBox.h"
#include "GCStateMachine.h"
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
//
//
//
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCGuildMemberBox.h"
#include "KMci.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCInviteManager.h"
#include "GCRenderManager.h"

KGCStateGuild::KGCStateGuild(void)
{
}

KGCStateGuild::~KGCStateGuild(void)
{
}

bool KGCStateGuild::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	// 서버에 채널 퇴장을 알린다.
	if( g_MyD3D->m_pStateMachine->IsJoinedChannel() )
	{
		if ( FAILED( g_MyD3D->LeaveChannel() ) ) 
			return false;

		g_MyD3D->m_pStateMachine->SetJoinChannel( false );
	}

	// 서버에 길드방 입장을 알린다.
	if ( FAILED( g_MyD3D->EnterGuildRoom() ) ) 
		return false;

	g_MyD3D->m_pStateMachine->SetJoinGuild( true );	
	g_MyD3D->m_pStateMachine->Clear_SavedState();

	return true;
}

bool KGCStateGuild::FrameMove(float fElapsedTime)
{
    SiKGCInviteManager()->FrameMove();

	if( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_NUMPADENTER ))
	{
		if( g_pkGuildChatBox != NULL )
			g_pkGuildChatBox->SetFocus();
	}

	// 자동메세지
	if ( !o10 )
	{
		const WCHAR* strMessage = RobotMessage();
		if (strMessage != NULL ) 
		{
			if( wcscmp( strMessage, L" ") )
			{
				if ( g_pkChatManager != NULL )
				{
					g_pkChatManager->AddChatMsg( strMessage, KGCChatManager::CHAT_TYPE_AUTO );
				}
			}
		}
	}

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();

	return true;
}

bool KGCStateGuild::Render()
{
    if ( BeginScene() )
    {
	    g_MyD3D->RenderUI();
	    g_MyD3D->RenderFPS();
        EndScene();
    }

	return true;
}

bool KGCStateGuild::OnInit()
{
	g_pkUIScene->m_pkGuildScene->EnterScene();

	g_pkGuildMemberBox->Activate( true );

    if( g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_SHOP_CASH &&
#ifndef DISABLE_MAP_SHOP // Kawan>
        g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_SHOP_GP &&
#endif
        g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_MY_INFO  )
    {
	    // 길드 채팅 박스를 클리어한다.
	    if ( g_pkGuildChatBox != NULL ) 
	    {
            g_pkGuildChatBox->ClearChatBox( GCPairPlaySystem::PPSCT_GUILD );
		    g_pkGuildChatBox->SetFocus();
	    }
    }

	if ( g_KMci.GetTrack() != 13 )
	{
		// 길드용 BGM을 연주한다.
		g_KMci.Play( GC_BGM_ALLIANCE, true );
	}	

    // WONYWONY
	//g_pkUIScene->m_pkGuildScene->SetButtonLock( BTN_LOBBY , !g_MyD3D->m_pStateMachine->IsJoinedChannel() );
    g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_WAIT_ROOM );
    
	return true;
}

bool KGCStateGuild::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateGuild::OnSetupFSM()
{
	ON_SETUP_FSM( GS_SERVER,		FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_SHOP_CASH,			GS_GUILD );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,			GS_GUILD );
#endif
	ON_SETUP_FSM( GS_MY_INFO,		GS_GUILD );
	ON_SETUP_FSM( GS_WORLDMAP,		GS_GUILD );
    ON_SETUP_FSM( GS_MATCH,			GS_GUILD );
    ON_SETUP_FSM( GS_ROOM,			GS_GUILD );
    ON_SETUP_FSM( GS_SQUARE_LOADING,GS_GUILD );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	m_eStateForFail = GS_SERVER;
}

void KGCStateGuild::OnDestroy()
{
        
}
