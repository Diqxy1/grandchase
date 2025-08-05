#include "stdafx.h"
#include ".\gcstatereplay.h"


#include "Spark.h"
#include "Stage.h"
#include "Replay.h"

#include "GCStateGame.h"
#include "GCCameraOrtho.h"
#include "DamageManager.h"

#include "KGCPlayerCommon.h"

//


#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "KGCRoomSceneS6.h"

#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameDlg.h"
#include "GCUI/KGCLoginScene.h"
#include "GCUI/KGCRegisterScene.h"
#include "KGCChannelManager.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCLoadingScene.h"
#include "KGCRoomManager.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "GCStateMachine.h"
#include "GCUI/GCInventoryFullMsg.h"

KGCStateReplay::KGCStateReplay(void)
{
	m_bFailed = false;
	m_dwLoadCount = 0;
}

KGCStateReplay::~KGCStateReplay(void)
{
}


bool KGCStateReplay::OnInitBeforeStateChange(GAME_STATE eNowState_ )			//스테이트가 바뀌기 이전에 초기화 해주는 부분입니다. 실패하면 스테이트가 바뀌지 않아요.
{
	g_MyD3D->m_pStateMachine->Clear_SavedState();

	m_dwLoadCount = 0;
	m_bFailed = false;
	g_MyD3D->m_bRPLoadFinished = false;	

	std::wstring FileName = g_MyD3D->MyReplay->m_vecFileList[g_MyD3D->m_iRPFileNum];
	swprintf( g_MyD3D->MyReplay->m_pFileName, MAX_PATH /*wcslen(FileName.c_str())*/, FileName.c_str());

	if( !g_MyD3D->MyReplay->Load_HeadData() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_REPLAY_VERSION ));
		return false;
	}

	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_LOADING );

	return true;
}

bool KGCStateReplay::FrameMove(float fElapsedTime)							//프레임마다 호출 됩니다.
{

	if( m_dwLoadCount <= 200 )
		++m_dwLoadCount;

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	g_KDSound.FrameMove();			 

	switch( m_dwLoadCount )
	{
		// 리플레이 오류 대비
	case 1:
		g_MyD3D->MyReplay->m_iCurrState = RP_PREPARE;		
		//g_pStateGame->OnInit();
		break;
	case 10:
		//if( !g_MyD3D->MyReplay->Load_HeadData() )
		//{
		//	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_REPLAY_VERSION ));
		//	StartGameTime += 150;
		//	return;
		//}
        g_MyD3D->m_pDamageManager->MakeSoundLoadList();
		break;
	case 20:
		//리플레이 로딩 화면으로 들어간다.
		g_pkUIScene->m_pkGameOverlayUI->Create();
		g_pkUIScene->m_pkGameOverlayUI->Initialize();
		g_pkUIScene->m_pkGameOverlayUI->ToggleRender(false);
		break;
	case 100:
		//실제 리플레이 로딩 들어간다.
		g_kGlobalValue.m_kUserInfo.bHost = false; //이래야 저절로 끝나는 일 없다.
		g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_PLAYING;
		g_MyD3D->m_bLastFrameSend = false;

		g_pkGameOverlayUI->OnStartGame();

		// 게임 진행용 프레임 카운트를 초기화한다.
		g_kCamera.SetEarthQuake(0);
		g_MyD3D->Next2Stop = false;
		break;
	case 140:
		// 태그매치땜시롱 리플레이는 풀로딩합니다.
		g_pGraphicsHelper->LoadPlayerActionMotionFull();
		g_pGraphicsHelper->LoadPlayerModelFull();
		break;
	case 150:
		// 게임 텍스쳐 로딩
		g_MyD3D->MySparks->Load();
		g_MyD3D->m_pDamageManager->Load();

		g_MyD3D->MyStg->Load_Stage();

		// 맵 화면 스크롤값 초기화
		// 'X-마스의 바벨'에서 사용하는 값들
		g_MyD3D->MyStg->m_fScrollY = 0.0f;
		g_MyD3D->MyStg->m_bUpScroll = true;
		g_MyD3D->MyStg->m_bDeathScrollY = false;
		break;
	case 160:
		for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
		{
			//g_MyD3D->MyPlayer[i]->SetTagSlotAlign();
            if ( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                continue;
            }
			g_MyD3D->MyPlayer[i]->BeforeStartGame();
            g_MyD3D->MyPlayer[i]->CreatePlayer();
            g_MyD3D->MyPlayer[i]->MakeSoundLoadList();
            g_MyD3D->MyPlayer[i]->MakeFireInfo3DObjectLoadList();
            //g_MyD3D->MyPlayer[i]->InitBonusBuff();
			g_MyD3D->MyPlayer[i]->First_Receive = true;
			g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID + 1 + i;
		}
        KGCPlayerCommon::MakeSoundLoadList();
		//카메라를 관전자 모드로 셋팅한다.
		g_kCamera.InitCamera();
		g_kCamera.SetNextTargetPlayer(0);

		// 카메라의 처음 위치 세팅
		g_kCamera.InitCameraPosition();
		// Num_Net=1;
		IsDetermineWin=false;

		//====================================================================================
		// 2006.07.14 : Asirion
		// Comment : 리플레이에서는 내가 운영자 모드로 관찰한다.
		SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo.push_back(g_kGlobalValue.m_kUserInfo);

		break;
	case 170:
		//ResetCard();
		g_MyD3D->ClearMonster();
		break;
	case 180:		
        // 2007/2/26. iridology. 소리 로딩
        g_KDSound.FullLoadfromLoadList();
		break;
	case 190:
		//아래 함수는 로딩 박스가 닫힐 때 불린다.
		g_pkGameOverlayUI->EnableHUD( false );
		break;
	case 200:
		g_MyD3D->m_bRPLoadFinished = true;
		IsResultComplete = false;
        g_MyD3D->MyReplay->m_iCurrState = RP_VIEW;		
		g_pStateGame->OnStartGame();
		g_pStateGame->OnEnterStage();
		break;
	}

	return true;
}

bool KGCStateReplay::Render()												//그려줍니다.
{
    if ( BeginScene() )
    {
	    g_MyD3D->RenderUI();
        EndScene();
    }
	return true;
}

bool KGCStateReplay::OnInit()													//스테이트가 바뀌고 난 후 초기화 해주는 부분입니다.	
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
	return true;
}

bool KGCStateReplay::OnDestroyBeforeStateChange()
{
    if( SiKGCChannelManager()->GetCurrentChannel() != -1 )
	    DeleteReplayData();
	return true;
}

void KGCStateReplay::OnSetupFSM()
{
    ON_SETUP_FSM( GS_GAME,          GS_WORLDMAP );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_WORLDMAP );
	ON_SETUP_FSM( GS_REGISTER,      GS_WORLDMAP );
	ON_SETUP_FSM( GS_REPLAY ,         GS_WORLDMAP );
    m_eStateForFail = GS_SERVER;
}

void KGCStateReplay::OnDestroy()
{
	
}

void KGCStateReplay::DeleteReplayData()
{
	SiKGCRoomManager()->InitObserver(); //옵저버 모드 초기화 하기.(퀘스트 0 시작시에 문제가 되기에.)
	//이 아래 두 줄 순서 바뀌면 안 됩니다... 리플레이 리소스를 정리하고 파일을 닫아야 합니다..
    g_MyD3D->MyReplay->SaveReplayEnd();
	// 임시파일 삭제
	DeleteFile(L"Replay.tmp");
}
