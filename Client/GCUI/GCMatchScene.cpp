#include "stdafx.h"
#include "GCMatchScene.h"

//
#include "KGCRoomList.h"
//
#include "KGCChannelManager.h"
#include "KGCMatchSceneUserInfo.h"
//
#include "KGCGradeMark.h"

#include "MyD3D.h"
#include "GCIndigoLobby.h"
//
#include "GCUI/GCChatBox.h"
#include "GCUI/KGCBadUserPopup.h"
#include "Procedure.h"
#include "../BuddySystem/KFriendShip.h"
#include "GCInventoryFullMsg.h"
#include "GCUI/GCMatchWindowS6.h"
#include "KGCMatchManager.h"
#include "KGCPartyManager.h"
#include "KGCBillboardChatBoard.h"


IMPLEMENT_CLASSNAME( KGCMatchScene );
IMPLEMENT_WND_FACTORY( KGCMatchScene );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchScene, "gc_match_scene_s6" );

KGCMatchScene::KGCMatchScene( void )
: m_pkUserInfo(NULL)
, m_pkStatusPanel(NULL)
, m_pkCharWnd(NULL)
, m_pkUserInfoBtn(NULL)
, m_pkStatusBtn(NULL)
, m_pkMatchWndS6(NULL)
{
	LINK_CONTROL("user_info_view",      m_pkUserInfo);
	LINK_CONTROL("user_status_info_view",      m_pkStatusPanel);
	LINK_CONTROL("user_info_button",      m_pkUserInfoBtn);
	LINK_CONTROL("user_status_button",      m_pkStatusBtn);
	LINK_CONTROL("character_window", m_pkCharWnd);
	LINK_CONTROL("match_window_s6", m_pkMatchWndS6);
	
	m_saBlind[0] = NULL;
	m_saBlind[1] = NULL;

	LINK_CONTROL("blind1",      m_saBlind[0] );
	LINK_CONTROL("blind2",      m_saBlind[1] );
	LINK_CONTROL("worldbillboardchat", m_pkBillboardChatBoard);
}

KGCMatchScene::~KGCMatchScene(void)
{
}

void KGCMatchScene::OnCreate()
{
    KGCScene::OnCreate();
	m_pkBillboardChatBoard->InitState(true, true, this);
	m_pkUserInfo->InitState(true);
	m_pkStatusPanel->InitState(false);

	m_pkUserInfoBtn->InitState(true,true,this);
	m_pkUserInfoBtn->Lock(true);
	m_pkStatusBtn->InitState(true,true,this);
	m_pkStatusBtn->Lock(false);

	m_pkCharWnd->InitState(true,true,this);

	m_pkMatchWndS6->InitState(true,true,this);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_CHANGE_CHAR_INFO);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_CHANGE_CHAR_INFO, this, &KGCMatchScene::UpdateGameData);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_UPDATE_MATCH);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_UPDATE_MATCH, this, &KGCMatchScene::UpdateBlind);

	
	for ( int i = 0 ; i < 2 ; ++i ) {
		m_saBlind[i]->InitState(false,true,this);
	}
}

void KGCMatchScene::OnInitialize()
{
    m_pkUserInfo->HideAllCashGashInfo();
	m_pkCharWnd->UpdateMeshObject();
	m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
}

void KGCMatchScene::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkUserInfoBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUserInfoBtn );
	GCWND_MSG_MAP( m_pkStatusBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickStatusBtn );


    KGCScene::ActionPerformed( event ); 
}

void KGCMatchScene::FrameMoveInEnabledState( void )
{
    if( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        if ( false == g_pkUIScene->IsMsgBoxModal() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SHUTDOWN );
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        }
    }

	KGCScene::FrameMoveInEnabledState();
}

void KGCMatchScene::UpdateRoomList()
{

}

void KGCMatchScene::OnEnterScene()
{
    if (g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) 
    {
        SetButtonRender( BTN_GUILD_MATCH, true );
        SetButtonRender( BTN_MATCH, false );
    }
    else 
    {
        SetButtonRender( BTN_GUILD_MATCH, false );
        SetButtonRender( BTN_MATCH, true );
    }        
	UpdateGameData();
	g_pkLobbyChatBox->ClearChatBox( GCPairPlaySystem::PPSCT_LOBBY );
	g_pkLobbyChatBox->InitColorButton();	
	g_pkLobbyChatBox->SetFocus();
	g_pkLobbyChatBox->ToggleRender( true );
	m_pkMatchWndS6->EnterScene();
}

void KGCMatchScene::OnLeaveScene()
{
}

void KGCMatchScene::UpdateGameData()
{
	m_pkStatusPanel->SetChangeUserInfo();
	m_pkUserInfo->SetUserInfo();
	m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
	m_pkStatusPanel->SetText();    

}

void KGCMatchScene::OnClickUserInfoBtn( )
{
	m_pkUserInfoBtn->Lock(true);
	m_pkStatusBtn->Lock(false);

	m_pkUserInfo->ToggleRender(true);
	m_pkStatusPanel->ToggleRender(false);
}

void KGCMatchScene::OnClickStatusBtn( )
{
	m_pkUserInfoBtn->Lock(false);
	m_pkStatusBtn->Lock(true);\

	m_pkUserInfo->ToggleRender(false);
	m_pkStatusPanel->ToggleRender(true);
}

void KGCMatchScene::UpdateBlind()
{
	bool bON = SiKGCMatchManager()->GetCurrentState() == KGCMatchManager::MSTATE_WAIT_STATE ||
		SiKGCPartyManager()->IsMatchReady(false);

	for ( int i = 0 ; i < 2 ; ++i ) {
		m_saBlind[i]->ToggleRender(bON);
	}

}
