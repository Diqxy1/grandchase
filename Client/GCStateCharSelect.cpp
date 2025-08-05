#include "stdafx.h"
//
#include ".\GCStateCharSelect.h"
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

KGCStateCharSelcet::KGCStateCharSelcet(void)
: m_bFistConnect(true)
{
}

KGCStateCharSelcet::~KGCStateCharSelcet(void)
{
}

bool KGCStateCharSelcet::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{	
	g_MyD3D->m_pStateMachine->Pushfront_SavedState( eNowState_ );
	return true;
}

bool KGCStateCharSelcet::FrameMove(float fElapsedTime)
{
	//g_KDSound.LoadByStep();
	g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
	g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();
	return true;
}
bool KGCStateCharSelcet::Render()
{
	g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );
	if ( BeginScene() )
	{
		g_MyD3D->RenderUI();
		g_MyD3D->RenderFPS();

		g_ParticleManager->Render( GC_LAYER_UI );
		g_ParticleManager->Render( GC_LAYER_CUSTOM );
		EndScene();
	}

	return true;
}

void KGCStateCharSelcet::VerifyCharCardInfo( void )
{
	if ( m_bFistConnect == false ) return;

	m_bFistConnect = false;
	std::map<int,std::pair<GCITEMID, GCITEMUID>> mapCharItemList = g_pItemMgr->GetUserCharCardList();

	for ( std::map<int,std::pair<GCITEMID, GCITEMUID>>::iterator mit = mapCharItemList.begin() ; mit != mapCharItemList.end() ; ++mit ) {
		Result_New_Character = INT_MAX;
		KP2P::GetInstance()->Send_NewCharacterReq( mit->second.first, mit->second.second );
		if( false == g_MyD3D->WaitForServerAck( Result_New_Character, INT_MAX, 3000, INT_MAX ) )
		{
			// 타임아웃 처리
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
		}
	}
}

bool KGCStateCharSelcet::OnInit()
{
//    VerifyCharCardInfo();
	g_pkUIScene->m_pkCharSelectScene->EnterScene();

	return true;
}

bool KGCStateCharSelcet::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateCharSelcet::OnSetupFSM()
{
	ON_SETUP_FSM( GS_LOGIN,						GS_SERVER );
	ON_SETUP_FSM( GS_REGISTER,					GS_SERVER );
	ON_SETUP_FSM( GS_WORLDMAP,					GS_SERVER );
	ON_SETUP_FSM( GS_MATCH,						GS_SERVER );
	ON_SETUP_FSM( GS_SHOP_CASH,					GS_SERVER );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,					GS_SERVER );
#endif
	ON_SETUP_FSM( GS_MY_INFO,					GS_SERVER );
	ON_SETUP_FSM( GS_SERVER,					FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_TUTORIAL,					GS_SERVER );

	ON_SETUP_FSM( GS_CHAR_GAIN,					GS_SERVER );
	ON_SETUP_FSM( GS_ROOM,						GS_SERVER );

	ON_SETUP_FSM( GS_CHAR_SELECT,				FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM,		FAIL_N_SHUTDOWN );

#ifdef AUTO_CITY
	// Kawan> City
	ON_SETUP_FSM(GS_SQUARE_LOADING, GS_CHAR_SELECT);
	ON_SETUP_FSM(GS_SQUARE, GS_CHAR_SELECT);
	ON_SETUP_FSM(GS_SQUARE_END, GS_CHAR_SELECT);
	// End
#endif

	m_eStateForFail = GS_SERVER;
	
}

void KGCStateCharSelcet::OnDestroy()
{
	g_pkUIScene->m_pkCharSelectScene->LeaveScene();
}
