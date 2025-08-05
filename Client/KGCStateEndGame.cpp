#include "stdafx.h"
//


#include "./kgcstateendgame.h"
#include "MyD3D.h"
//
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCChatBox.h"
#include "GCUI/KGCUIHelper.h"
#include "KMci.h"

#include "GCNetwork.h"
#include "GCStateMachine.h"
#include "KGCRoomManager.h"
#include "KGCRoomListManager.h"


KGCStateEndGame::KGCStateEndGame(void)
{
}

KGCStateEndGame::~KGCStateEndGame(void)
{
}


bool KGCStateEndGame::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	g_MyD3D->m_pStateMachine->Clear_SavedState();

	return true;
}

bool KGCStateEndGame::FrameMove(float fElapsedTime)
{
    PROFILE_SET("KGCStateEndGame::FrameMove");

	return true;
}

bool KGCStateEndGame::Render()
{
	return true;
}

bool KGCStateEndGame::OnInit()
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if( SiKGCRoomListManager()->IsHellMode() )
        {
            if( g_KMci.GetTrack() != GC_BGM_WORLDMAP_HELL )
                g_KMci.Play( GC_BGM_WORLDMAP_HELL, true );
        }
        else if (SiKGCRoomListManager()->IsBreakMode())
        {
            if (g_KMci.GetTrack() != GC_BGM_WORLDMAP_BREAK)
                g_KMci.Play(GC_BGM_WORLDMAP_BREAK, true);
        }
        else
        {
            int iBGMIndex;
            EGCWorldMap eWorldMap = SiKGCWorldMapManager()->GetCurrentWorldMap();
            iBGMIndex = SiKGCWorldMapManager()->GetWorldMapBgm( eWorldMap );

            if( g_KMci.GetTrack() != iBGMIndex )
                g_KMci.Play( iBGMIndex, true );
        }        
    }
    else
    {
        // 대기방 BGM을 연주한다.
        if( g_KMci.GetTrack() != GC_BGM_LETS_GO )
            g_KMci.Play( GC_BGM_LETS_GO, true );
    }

	if ( g_kGlobalValue.m_kUserInfo.bHost )
	{
		if( !( SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo() ) )
        {
			KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );
        }
        if( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME )
        {
            KP2P::GetInstance()->Send_P2PUniqueNumberReq();
        }
	}

    //if( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME )
	   // KP2P::GetInstance()->Send_EndResultNot();

	g_pkUIScene->RoomSceneUpdateData();
	
    g_pkUIScene->m_pkRoomSceneS6->ToggleRender(true);
    
    g_pkUIScene->m_pkGameOverlayUI->ToggleRender( false );
	g_pkUIScene->EnableMessageBox( true );

	g_pkUIScene->DestroyCurrentMsgBox();

	//채팅 박스에 포커스를~~~!!!
	if ( g_pkRoomChatBox != NULL ) 
	{
		//g_pkRoomChatBox->ClearChatBox();
		g_pkRoomChatBox->SetFocus();
	}	

	g_pkUIHelper->SetUILockTime( 0 , BUTTON_LOCK_FRAME );

	g_MyD3D->m_pStateMachine->GoState( GS_ROOM );
 


	return true;
}

void KGCStateEndGame::Process_Admin_Command( AdminCommandType _ACT )
{
}

bool KGCStateEndGame::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateEndGame::OnSetupFSM()
{
	ON_SETUP_FSM( GS_ROOM, GS_SERVER );

	m_eStateForFail = GS_SERVER;
}

void KGCStateEndGame::OnDestroy()
{

}
