#include "stdafx.h"
#include ".\gcstatetutorial.h"

#include "GCStateMachine.h"
#include "KGCModeTutorial.h"
#include "GCStateGame.h"

#include "DamageManager.h"
#include "KGCRoomManager.h"
//
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"

#include "GCUI/GCOnGameDlg.h"
#include "GCUI/KGCLoginScene.h"
#include "GCUI/KGCRegisterScene.h"

#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCLoadingScene.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "buff/KGCBuffManager.h"
#include "GCEventManager.h"

KGCStateTutorial::KGCStateTutorial(void)
{
	m_iCharacter = -1;
}

KGCStateTutorial::~KGCStateTutorial(void)
{
}


bool KGCStateTutorial::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	return true;
}

bool KGCStateTutorial::FrameMove(float fElapsedTime)
{
    return true;
}

bool KGCStateTutorial::Render()
{
	return true;
}

bool KGCStateTutorial::OnInit()
{
	//UI 초기화
	g_pkUIScene->m_pkServerScene->ToggleRender( false );
	g_pkUIScene->m_pkWorldMapScene->ToggleRender( false );
	g_pkUIScene->m_pkGuildScene->ToggleRender( false );
	g_pkUIScene->m_pkMyInfoScene->ToggleRender( false );
	
    g_pkUIScene->m_pkRoomSceneS6->ToggleRender( false );
    
    g_pkUIScene->m_pkCashShopScene->ToggleRender( false );
#ifndef DISABLE_MAP_SHOP // Kawan>
    g_pkUIScene->m_pkGPShopScene->ToggleRender( false );
#endif
	g_pkUIScene->m_pkLoginScene->ToggleRender( false );
	g_pkUIScene->m_pkRegisterScene->ToggleRender( false );
	g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );
	g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
	g_pkUIScene->m_pkLoadingScene->ToggleRender( false );
    g_pkUIScene->m_pkSquareLoadingScene->ToggleRender( false );
	g_pkUIScene->m_pkMatchScene->ToggleRender( false );

    // 튜토리얼 모드로 세팅
    SiKGCRoomManager()->UpdateShowGameModeCategory( true, SiKGCChannelManager()->GetCurrentChannelType()  );
    SiKGCRoomManager()->InitGameMode();
    SiKGCRoomManager()->SetGameModeCategory( GC_GMC_TUTORIAL );
    SiKGCRoomManager()->SetGameMode( GC_GM_TUTORIAL );

	g_MyD3D->MyPlayer[0]->m_kUserInfo.cCharIndex = m_iCharacter;

    g_pMagicEffect->ClearBuffInstace();

	KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
	pMode->InitAtLoading();

    // 2007/4/30. iridology. 어딘가 넣을곳이 있을텐데 바빠서 이만..
    //g_MyD3D->MyBuff->Initialize();
    //g_pMagicEffect->ClearMagicEffect();    

    g_KDSound.FullLoadfromLoadList();
	g_MyD3D->m_pStateMachine->GoGame();

    // 쓸데없는 이벤트 따위 초기화 하쟈;
    SiKGCEventManager()->ClearEvents();

	return true;
}

bool KGCStateTutorial::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateTutorial::OnSetupFSM()
{
    ON_SETUP_FSM( GS_GAME,          GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	ON_SETUP_FSM( GS_REGISTER,      GS_SERVER );
    m_eStateForFail = GS_SERVER;
}

void KGCStateTutorial::OnDestroy()
{

}
