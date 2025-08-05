#include "stdafx.h"


#include "KGCGameModeInterface.h"



#include "KDInput.h"
#include "DamageManager.h"
#include "Spark.h"

#include "Controls.h"
#include "Headup Display.h"
#include "KTDGFrustum.h"
#include "Item.h"
#include "Message.h"

//
#include "Stage.h"



#include "PetAI.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCAgitOverlayUI.h"
#include "QuestGameManager.h"
#include "KGCGameBoard.h"
#include "GCCameraOrtho.h"

#include "GCStateGame.h"
#include "Replay.h"
#include "GCRenderManager.h"
#include "./MapSystem/GCWallManager.h"
//
//



//
#include "GCEventManager.h"
#include "ClientErr.h"

#include "MapSystem/KGCLayerManager.h"
#include "GCDropItemManager.h"
#include "KGCRoomManager.h"
#include "KGCElvis.h"
#include "KGC3DObjectMgr.h"
#include "KGCAnnounceMgr.h"
#include "KGCEmoticon.h"

#include "KGCMinigameManager.h"
#include "KGCAngelsEgg.h"

#include "KGCAgitGameManager.h"
#include "gcui/GCAddtionGuide.h"

#include "KGCWeatherSystem.h"
#include "GCUI/KGCPrograssBar.h"
#include "GCAutoMatchManager.h"
#include "KGCSubjectManager.h"


KGCGameModeInterface::KGCGameModeInterface()
{
	m_bRequestRebirth = false;
}
KGCGameModeInterface::~KGCGameModeInterface()
{
}

bool KGCGameModeInterface::CheckExitGame()
{
	// 게임 중 'ESC' 키를 눌렀을 경우
	if (diks[DIK_ESCAPE] && !Old_diks[DIK_ESCAPE])
	{
#if !defined( NO_USE_ADDITION_GUIDE )
		if (g_pkGameOverlayUI->m_pkAddtionGuide &&
			g_pkGameOverlayUI->m_pkAddtionGuide->IsRenderOn() &&
			g_pkGameOverlayUI->m_pkAddtionGuide->GetState() != KGCAddtionGuide::STATE_GAME_OFF)
		{
			g_pkGameOverlayUI->m_pkAddtionGuide->SetAniState(KGCAddtionGuide::ENDING);
			//return false;
		}
#endif

		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
		{
			if (g_pkUIScene->m_pkAgitOverlayUI->GetChatEventHandling() == KGCAgitOverlayUI::EM_CHAT_ESC)
			{
				g_pkUIScene->m_pkAgitOverlayUI->SetChatEventHandling(KGCAgitOverlayUI::EM_CHAT_NONE);
				return false;
			}

			if (g_pkAgitOverlayUI->IsRenderOnAgitUI())
				return false;
		}
		else
		{
			if (g_pkGameOverlayUI->HasChatEventHandling()) // 채팅 이벤트 해들링 중이면 통과
			{
				g_pkGameOverlayUI->SetChatEventHandling(false);
				return false;
			}
		}

		if (false == g_pkUIScene->IsMsgBoxModal())
		{
			if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
			{

				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_GAME_EXIT), L"",
					KGCUIScene::GC_MBOX_USE_EXIT);

			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_EXIT_PRACTICE), L"",
					KGCUIScene::GC_MBOX_USE_EXIT);

			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_DEATHMATCH_SURRENDER_WARNING), L"",
					KGCUIScene::GC_MBOX_USE_EXIT);
			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_EXIT_GAME_STRING_S4), L"",
					KGCUIScene::GC_MBOX_USE_EXIT);
			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
			{
#if defined (NATION_BRAZIL)
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_OLYMPIC_GAME_QUIT_NOTICE), L"",
					KGCUIScene::GC_MBOX_USE_MINIGAME_EXIT);
#else
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_OLYMPIC_GAME_QUIT_NOTICE), g_pkStrLoader->GetString(STR_ID_GAME_EXIT),
					KGCUIScene::GC_MBOX_USE_MINIGAME_EXIT);
#endif

			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME,
					g_pkStrLoader->GetString(STR_ID_EXIT_AGIT),
					L"", KGCUIScene::GC_MBOX_USE_EXIT);
			}
			else if (g_pkGameOverlayUI->IsChapionMonsterTimerOn())
			{
				//아무것도 띄우지 않겠다.
			}
			else
			{
#if defined(EXIT_DUNGEON_MESSAGE)
				std::wostringstream strExit;

#if defined(NATION_TAIWAN)
				strExit << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW1) << L" ";
				strExit << L"#cfa1d2e" << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW2) << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW3) << L"#cX" << L" ";
				strExit << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW4);
#else

				strExit << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW1) << L" ";
#if defined(NATION_THAILAND) || defined( NATION_CHINA )
				strExit << L"#cfa1d2e" << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW2) << L"#cX"; // <--뛰워 쓰기 들어가면 안됨.
#else
				strExit << L"#cfa1d2e" << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW2) << L"#cX" << std::endl;
#endif
				strExit << L"#cfa1d2e" << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW3) << L"#cX" << std::endl;
				strExit << g_pkStrLoader->GetString(STR_ID_GAME_EXIT_NEW4);
#endif
				if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME && g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL) {
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_TUTORIAL_CANCEL_CONFIRM_MSG), L"", KGCUIScene::GC_MBOX_USE_EXIT);
				}
				else
				{
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME, strExit.str(), L"", KGCUIScene::GC_MBOX_USE_EXIT);
				}

#else
				if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME && g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL) {
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_TUTORIAL_CANCEL_CONFIRM_MSG), L"", KGCUIScene::GC_MBOX_USE_EXIT);
				}
				else
				{
					if (KD3DWnd::D3DWS_ENABLED != g_pkUIScene->GetStateUI(KGCUIScene::GC_MBOX_DOTA_SHOP))
						g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_EXIT_GAME_STRING_S4), L"", KGCUIScene::GC_MBOX_USE_EXIT);
				}

#endif
			}
		}
	}
	return false;
}

void KGCGameModeInterface::CheckReplay()
{
	// 2007.02.22 wony
	// 리플레이 막습니다
	//return;

	//퀘스트와 궁수 대회, 몬스터 사냥은 녹화가 불완전하여 지원하지 않는다.
	//리플레이는 녹화를 지원하지 않습니다. 종료시 리플레이를 저장하지 않고 있습니다.
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_CRUSADER &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_HUNT &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_ANGELS_EGG &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DOTA &&
		SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_CAPTAIN)
	{
		if ((SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
			SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH) &&
			g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN)
		{
			return;
		}
		// 녹화 시작
		if ((diks[DIK_LCONTROL] || diks[DIK_RCONTROL]) && diks[DIK_1] && !Old_diks[DIK_1]
#if defined( NATION_HONGKONG)
			|| g_kGlobalValue.IsObserverMode()															// 옵져버 모드라도 리플레이 찍는다.
#endif
			)
		{
			//현재 날짜, 시간을 파일명으로 한다.
			tm* newtime;
			time_t aclock;

			time(&aclock);                 /* Get time in seconds */
			newtime = localtime(&aclock);

			swprintf(g_MyD3D->MyReplay->m_pFileName, MAX_PATH, L"Replay\\%4d_%2d_%2d_%2d_%2d.first",
				newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min);

			g_MyD3D->MyReplay->Save_HeadData();
		}

	}
}

void KGCGameModeInterface::OnRecvGameEndPacket()
{
	// 방장으로부터 게임종료 패킷을 받았음
	if (!g_kGlobalValue.m_kUserInfo.bHost)
	{
		IsDetermineWin = true;
		GameEnd();
		g_kCamera.m_bZoomViewMode = false;
	}
}

HRESULT KGCGameModeInterface::FrameMove(float fElapsedTime)
{
	PROFILE_SET("KGCGameModeInterface::FrameMove");

	KGCPC_BEGIN("CustomFrameMove");
	++StartGameTime;

	int i;
	// 게임 종료체크 / 처리 
	if (!IsDetermineWin && g_kGlobalValue.m_kUserInfo.bHost
		&& g_MyD3D->MyReplay->m_iCurrState != RP_VIEW) //리플레이를 보는 상태라면 종료체크할 필요 없다.
	{
		if (CheckGameEnd())
		{
			GameEndProcess();
			GameEnd();
		}
	}

	// 스토리 관리
	m_iOldNumLife = 0;
	m_iNumLife = 0;

	g_MyD3D->Num_Alive = 0;

	for (i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (g_MyD3D->MyPlayer[i]->m_cLife)
		{
			++g_MyD3D->Num_Alive;
			m_iNumLife += g_MyD3D->MyPlayer[i]->m_cLife;
		}
	}

	m_iNumNet = 0;
	for (i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
			++m_iNumNet;
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_SANTA) == true)
	{
		m_kSanta.FrameMove(fElapsedTime);
	}

	// 하위클래스 프레임무브 처리
	KGCPC_END();

	KGCPC_BEGIN("CustomFrameMove2");
	if (FAILED(CustomFrameMove(fElapsedTime))) {
		KGCPC_END();
		return S_OK;
	}
	else {
		KGCPC_END();
	}

	g_MyD3D->GetMyShortCutSlot()->FrameMove();
	KGCShortCutBox* pShortCutBox = g_pkGameOverlayUI->GetShourtCutBoxInstance();
	if (pShortCutBox)
		pShortCutBox->MacroCommunityFrameMove();

	if (CheckExitGame() == true)
		return S_OK;

#if !defined( NO_USE_ADDITION_GUIDE )
	if (g_pkGameOverlayUI->m_pkAddtionGuide)
		g_pkGameOverlayUI->m_pkAddtionGuide->KeyEvent();
#endif

	// 머하는걸까? ㅡㅡ;
	m_iOldNumLife = m_iNumLife;
	if (g_MyD3D->MyHead->White_Count > 0)
		g_MyD3D->MyHead->White_Count--;

	//대전에서 예전 녹화하기 (Ctrl + 1) 기능 막음. 추후 관련 코드 제거 작업시 처리 예정
	//CheckReplay();

	g_pMagicEffect->FrameMove();

	if (g_MyD3D->IsLocalPlayerObserver() || g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
	{
		// 호스트가 관전 모드라면 특별히 이런 패킷을 보낸다.
		g_MyD3D->ViewMode_FrameMove();
	}

	// 현제 게임에서만 쓴다.
	g_MyD3D->m_pUIAnim->FrameMove(GC_ELAPSED_TIME);

	// 게임이 끝났을때 E_FAIL을 리턴해줍니다.
	if (g_MyD3D->MyHead->Frame_Move() == E_FAIL)
		return S_OK;

	g_kCamera.FrameMove();
	g_MyD3D->MyStg->Frame_Move();    // STAGE
	if (g_pkQuestManager->IsStartedStage())
		SiKGCFantasticMap()->Water_FrameMove();

	g_MyD3D->MyItms->Frame_Move();   // GAME ITEM

	g_MyD3D->MySparks->FrameMove();
	g_MyD3D->m_pDamageManager->FrameMove();
	SiKGC3DObjectMgr()->FrameMove();

	g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);                  // 파티클 처리
	g_AnimManager.FrameMove();                      // 애니메이션 처리
	//g_pMeshEffectManager->FrameMove( GC_ELAPSED_TIME );

	g_MyD3D->m_pMapAnim->FrameMove(GC_ELAPSED_TIME);       // 맵 애니메이션 처리

	g_MyD3D->m_pPetAnim->FrameMove(GC_ELAPSED_TIME);
	//g_MyD3D->m_FireSpace.FrameMove( GC_ELAPSED_TIME );

	//g_MyD3D->m_AITemplet.FrameMove();
	g_pkPetAI->FrameMove();


	float fConstMPRate = 1.0f;
#if defined( NATION_BRAZIL ) 
	fConstMPRate = 0.6f;
#endif

	// MP 회복량 고정에 걸리면 특정 MP로만 회복된다.
	if (g_MyD3D->GetMyPlayer()->IsMPRecoveryFix())
	{
		float fFixMPValue = g_MyD3D->GetMyPlayer()->GetMPRecoveryFixValue();
		g_MyD3D->ChargeLocalPlayerMana(fFixMPValue);
	}
	else
	{
		// 자동 MP 증가 모드 이면
		if (IsMPRecoveryMode())
		{
			float fMPRecoveryAbility = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_MP_RECOVERY_RATE];

			if (g_MyD3D->GetMyPlayer()->IsFatal())
			{
				fMPRecoveryAbility += g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_FATAL_RECOVERY_MP_UP_BUFF] / 100.f;
			}

			float fMPRecoveryRatio = fMPRecoveryAbility * fConstMPRate;
			float fCharManaAutoRecoveryRatio = GetCharManaAutoRecoveryRatio();

			// 이건 보정 처리 안한다.
			float fMPBuffRatio = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE];

#if defined( USE_MATCH_STATUS_RATE )
			fMPRecoveryAbility *= MP_RECOVERY_PERCENT_MATCH;
			if (fMPRecoveryAbility > MP_RECOVERY_MAX_VALUE_MATCH) {
				fMPRecoveryAbility = MP_RECOVERY_MAX_VALUE_MATCH;
			}
#endif

			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
			{
				g_MyD3D->ChargeLocalPlayerMana(g_MyD3D->m_fIncMP * fCharManaAutoRecoveryRatio * 0.5f, fMPBuffRatio);
			}
			else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME) {
				g_MyD3D->ChargeLocalPlayerMana(0);
			}
			else if (SiKGCRoomManager()->IsMonsterGameMode())
			{
				g_MyD3D->ChargeLocalPlayerMana(g_MyD3D->m_fIncMP * (1.0f + fMPRecoveryRatio) * fCharManaAutoRecoveryRatio, fMPBuffRatio);
			}
			else    //  대전
			{
				fMPRecoveryRatio = fMPRecoveryAbility * fConstMPRate;


				float fTempVal = 1.0f;
				if (SiGCAutoMatchManager()->IsAutoMatchBalance() || SiKGCRoomManager()->IsMatchBalance())
				{
					fMPRecoveryRatio *= 0.1f;

					SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_MP_RECORVERY, fTempVal);
					fMPRecoveryRatio *= fTempVal;
				}

				if (GC_GLOBAL_DEFINE::bMatchBalance2014) {
					// 대전에서 MP자동 회복율을 낮춤
					if (g_MyD3D->MyHead->Level <= 1.f) {
						g_MyD3D->ChargeLocalPlayerMana(g_MyD3D->m_fIncMP * (1.0f + fMPRecoveryRatio), fMPBuffRatio);
					}
					else {
						g_MyD3D->ChargeLocalPlayerMana((g_MyD3D->m_fIncMP / 2.f) * (1.0f + fMPRecoveryRatio), fMPBuffRatio);
					}
				}
				else {
					g_MyD3D->ChargeLocalPlayerMana(g_MyD3D->m_fIncMP * (1.0f + fMPRecoveryRatio), fMPBuffRatio);
				}
			}
		}
	}

	if (g_MyD3D->MyCtrl->Mouse_Power > 4)
		g_MyD3D->MyCtrl->Mouse_Power -= 5;

	// 게임이 끝났으면 카메라를 되돌린다5
	if (IsDetermineWin == true && 0.0f != g_kCamera.m_fWideMode)
	{
		g_kCamera.m_fWideMode = 0.0f;
		g_kCamera.SetZoom(1.0f);
	}

	//CheckExitGame();

	g_pkUIMgr->FrameMove(g_MyD3D->m_fTime);
	g_KDSound.FrameMove();

	SiKGCEmoticonManager()->FrameMove();



	g_kCamera.UpdateCameraMatrix();
	return S_OK;
}

HRESULT KGCGameModeInterface::Render()
{
	ClearScene();

	g_MyD3D->m_kAfterImageRenderer.RenderAfterImage();

#if !defined(__PATH__)
	if (g_MyD3D->IsGifRecordMode() == false)
#endif
		g_MyD3D->m_KGCBlurMgr.BeginBlur();

	// [1/4/2008] breadceo. 아 파티클은 z 안키고 그리잖아!!!!
#if !defined( __PATH__ )
	if (g_MyD3D->IsGifRecordMode())
		goto NotRenderBackground;
#endif
	ON_RENDER_GC_LAYER_SKY();                   // GC_LAYER_SKY
	ON_GC_LAYER_SCROLLED_LAYER1();				// GC_LAYER_SCROLLED_LAYER1
	ON_GC_LAYER_SCROLLED_LAYER2();				// GC_LAYER_SCROLLED_LAYER2
	ON_GC_LAYER_SCROLLED_LAYER3();				// GC_LAYER_SCROLLED_LAYER3
	ON_RENDER_GC_LAYER_BETWEEN_SKY_BLOCK();     // GC_LAYER_BETWEEN_SKY_BLOCK
	ON_RENDER_GC_LAYER_BLOCK();                 // GC_LAYER_BLOCK    
	ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR();    // GC_LAYER_BETWEEN_BLOCK_CHAR
#if !defined(__PATH__)
	NotRenderBackground:
#endif
					   ON_RENDER_GC_LAYER_CHAR();                  // GC_LAYER_CHAR
					   ON_RENDER_GC_LAYER_BETWEEN_CHAR_FRONT();    // GC_LAYER_BETWEEN_CHAR_FRONT
					   if (SiKGCRoomManager()->IsMonsterGameMode())
					   {
						   SiKGCLayerManager()->AlphaRender();
					   }
					   ON_RENDER_GC_LAYER_FRONT_MAP();             // GC_LAYER_FRONT_MAP
#if !defined(__PATH__)
					   if (g_MyD3D->IsGifRecordMode() == false)
#endif
						   g_MyD3D->m_KGCBlurMgr.EndBlur();

					   // 2007/2/13. iridology. GC_LAYER_CHAR에서 그리던 글자들을 이부분으로 내렷음.
					   // 이유는 Blur를 먹일 때 512 512에 찍어서 800 600으로 늘려서 그리는데 글자가 있으면 이상해지기 때문~
					   // 앞으로 몬스터 글자 등등 글자 그리는거는 이쪽으로 넣도록 하는걸 권장
#if !defined( __PATH__ )
	// 연속 스크린샷 모드일 때는 그리면 안되는것들 뛰어 넘습니닷
					   if (g_MyD3D->IsGifRecordMode())
						   goto NotRenderTextAndFace;
#endif
					   ON_RENDER_GC_LAYER_WEATHER_SYSTEM();         // GC_WETHER_SYSTEM
					   ON_RENDER_GC_LAYER_TEXT();
					   ON_RENDER_GC_LAYER_FACE();                  // GC_LAYER_FACE    
#if !defined(__PATH__)
					   NotRenderTextAndFace:
#endif
										   ON_RENDER_GC_LAYER_UNDER_UI();
										   ON_RENDER_GC_LAYER_UI();                    // GC_LAYER_UI
										   ON_RENDER_GC_LAYER_ANNOUNCE();              // 방송용
										   ON_RENDER_GC_CHAR_OVER_UI_1();
										   ON_RENDER_GC_CHAR_OVER_UI_2();



										   return S_OK;
}

bool KGCGameModeInterface::BeginScene()
{
	return false;
}

void KGCGameModeInterface::EndScene()
{
}

void KGCGameModeInterface::ClearScene()
{
#if defined( __PATH__ )
	g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0L);
#else
	if (g_MyD3D->IsGifRecordMode())
	{
		g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, g_MyD3D->GetBackGroundColor(), 1.0f, 0L);
	}
	else
	{
		g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0L);
	}
#endif
}

void KGCGameModeInterface::SetPlayerGameResult(int iPlayerIndex_, char cCharType, float fExpWeight_, float fGpWeight_, bool bWin_, bool bLimit30Lv_/* = false  */)
{
	float fGetExp = Get_MyGetExp(iPlayerIndex_, cCharType, bLimit30Lv_, fExpWeight_);
	float fGetGp = Get_MyGetGP(iPlayerIndex_, bLimit30Lv_, fGpWeight_);

	//중간에 나갔다면 처리 안 한다.
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH && g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
	{
		if (bWin_ == false) //졌을 때 GP보상을 따로 계산한다(대전).
		{
			//새 공식 -> 대전 패배 시 획득 GP = 획득 GP/10 + 캐릭터Lv
			fGetGp = Get_MyGetGP(iPlayerIndex_, bLimit30Lv_, 1.0f);
			fGetGp = fGetGp / 10.0f + (float)Exp_2_Level(GetCharacterExp(g_pGameBoard->m_pStartingMember[iPlayerIndex_].m_mapStartExp, cCharType));
		}

		//대전에서 죽이면 나오는 GP더하기 시스템.
		int iGPSum = 0;
		g_MyD3D->MyPlayer[iPlayerIndex_]->GetCatchGPSum(iGPSum);
		fGetGp += (float)iGPSum;
	}

	if (0.0f < fGetExp && fGetExp < 1.0f)
		fGetExp = 1.0f;
	if (0.0f < fGetGp && fGetGp < 1.0f)
		fGetGp = 1.0f;

	//원래 아래 함수를 부르는 시스템이었으나 fGetGP 
	GCEXPTYPE biStartExp = GetCharacterExp(g_pGameBoard->m_pStartingMember[iPlayerIndex_].m_mapStartExp, cCharType);
	if (biStartExp != CHAR_EXP_INVALID)
	{
		// 태그매치에서 2개의 Slot에 하나의 케릭터를 꼽은 경우
		if (g_pGameBoard->Change_How_Exp[iPlayerIndex_][cCharType] != 0)
		{
			// 2007/5/9. iridology. 느무 많은 마이너스 경험치 의심부분..
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] == 0)
			{
				SiGCClientErrorManager()->ErrorCollect(KEventErr::ERR_CLIENTRPT, KClientErr::CE_MINUS_EXP_MISTERY);
				// g_MyD3D->MyHead->Change_How_Exp[iPlayerIndex_][cCharType] != 0 이고
				// g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] == 0 이란 말은
				// 두번째 케릭터 라는 뜻인데 목적 경험치가 0이니까 2배로 줘야 타당하다...
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] = biStartExp + (GCEXPTYPE)(2 * fGetExp);
			}
			else
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] += (GCEXPTYPE)fGetExp;
			}
		}
		else
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] = biStartExp + (GCEXPTYPE)fGetExp;
		}
	}
	g_MyD3D->MyPlayer[iPlayerIndex_]->Target_Score = g_pGameBoard->m_pStartingMember[iPlayerIndex_].Start_Score + (int)fGetGp;

	// 경계값을 체크한다.
	Auto_BoundCheckGPandEXP(iPlayerIndex_);

	// 변화량을 갱신한다.
	GCEXPTYPE biTargetExp = GetCharacterExp(g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp, cCharType);
	if (biStartExp != CHAR_EXP_INVALID)
		g_pGameBoard->Change_How_Exp[iPlayerIndex_][cCharType] = biTargetExp - biStartExp;
	g_pGameBoard->Change_How_Score[iPlayerIndex_] = g_MyD3D->MyPlayer[iPlayerIndex_]->Target_Score - g_pGameBoard->m_pStartingMember[iPlayerIndex_].Start_Score;

	// 승리로 판정한다.
	g_pGameBoard->m_pStartingMember[iPlayerIndex_].IsWin = bWin_;
}

void KGCGameModeInterface::SetPlayerGameResult(int iPlayerIndex_, char cCharType, int iExp_, int iGP_, bool bWin_, bool bLimit30Lv_/* = false  */)
{
	if (3 < g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Score)
		g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Score = 3;

	GCEXPTYPE biStartExp = GetCharacterExp(g_pGameBoard->m_pStartingMember[iPlayerIndex_].m_mapStartExp, cCharType);
	if (biStartExp != CHAR_EXP_INVALID)
	{
		// 태그매치에서 2개의 Slot에 하나의 케릭터를 꼽은 경우
		if (g_pGameBoard->Change_How_Exp[iPlayerIndex_][cCharType] != 0)
		{
			// 2007/5/9. iridology. 느무 많은 마이너스 경험치 의심부분..
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] == 0)
			{
				SiGCClientErrorManager()->ErrorCollect(KEventErr::ERR_CLIENTRPT, KClientErr::CE_MINUS_EXP_MISTERY);
				// g_MyD3D->MyHead->Change_How_Exp[iPlayerIndex_][cCharType] != 0 이고
				// g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] == 0 이란 말은
				// 두번째 케릭터 라는 뜻인데 목적 경험치가 0이니까 2배로 줘야 타당하다...
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] = biStartExp + (GCEXPTYPE)2 * iExp_;
			}
			else
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] += (GCEXPTYPE)iExp_;
			}
		}
		else
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp[cCharType] = biStartExp + (GCEXPTYPE)iExp_;
		}
	}
	g_MyD3D->MyPlayer[iPlayerIndex_]->Target_Score = g_pGameBoard->m_pStartingMember[iPlayerIndex_].Start_Score + iGP_ + g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Score * 10;

	// 경계값을 체크한다.
	Auto_BoundCheckGPandEXP(iPlayerIndex_);

	// 변화량을 갱신한다.
	GCEXPTYPE biTargetExp = GetCharacterExp(g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapEndExp, cCharType);
	if (biTargetExp != CHAR_EXP_INVALID)
		g_pGameBoard->Change_How_Exp[iPlayerIndex_][cCharType] = biTargetExp - biStartExp;
	g_pGameBoard->Change_How_Score[iPlayerIndex_] = g_MyD3D->MyPlayer[iPlayerIndex_]->Target_Score - g_pGameBoard->m_pStartingMember[iPlayerIndex_].Start_Score;

	// 승리로 판정한다.
	g_pGameBoard->m_pStartingMember[iPlayerIndex_].IsWin = bWin_;
}

void KGCGameModeInterface::ShowLevelUp()
{
	return;

	// 레벨이 올랐다면 레벨업 표시를 한다.
	//for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
	//{
	//    if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
	//    {
	//        unsigned char cStart = Exp_2_Level( GetCharacterExp( g_pGameBoard->m_pStartingMember[i].Start_Exp, g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType ) );
	//        unsigned char cResult = Exp_2_Level( GetCharacterExp( g_MyD3D->MyPlayer[i]->Target_Exp, g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType) );
	//        if( g_MyD3D->MyPlayer[i]->Target_Exp.size() <= 0)
	//            g_pGameBoard->m_pStartingMember[i].IsLevelUp = false;
	//        else
	//            g_pGameBoard->m_pStartingMember[i].IsLevelUp = ( cStart < cResult );
	//    }
	//}
}

//void KGCGameModeInterface::RigenMonster( int iMonID_, int iMonLevel_, bool bBoss_, const std::string& strAI_, int iMaxMon_, bool bRegenEffect_, float x, float y, int iLevel /*= -1*/ )
//{
//    //비어있는 몬스터를 찾는다
//    int iIndex = 0;
//    for( iIndex=0 ; iIndex < iMaxMon_ ; ++iIndex )
//    {
//        //if( g_kMonsterManager.m_mapMonster[iIndex]->Now_Monster == iMonID_ && 
//        //    g_kMonsterManager.m_mapMonster[iIndex]->EnableCreate() )
//        //{
//        //    break;
//        //}
//    }
//
//    // 되살아날 자리가 없다
//    if( iIndex >= iMaxMon_ )
//        return;
//
//    // 초기 포지션 설정
//    D3DXVECTOR2 pos = g_MyD3D->MyStg->Check_SafePostionX( randf() * SiKGCFantasticMap()->GetPixelWidth(), randf() );
//
//    if( x != -100.0f )
//        pos.x = x;
//
//    if( y != -100.0f )
//        pos.y = y;
//
//    // 되살려라
//    g_kMonsterManager.m_mapMonster[iIndex]->Wait_Room( iMonID_, pos.x, pos.y, false, false, iLevel );
//    g_kMonsterManager.m_mapMonster[iIndex]->m_bLive = true;
//    //g_kMonsterManager.m_mapMonster[iIndex]->m_iLv = iMonLevel_;
//    //g_kMonsterManager.m_mapMonster[iIndex]->UpateMonsterPoint();
//    //g_kMonsterManager.m_mapMonster[iIndex]->m_bBossMonster = bBoss_;
//
//    //aisequence는 항상 소문자이다.
//    if( strAI_ != "oldai" )
//    {       
//        //g_MyD3D->m_AITemplet.AIEnable( g_kMonsterManager.m_mapMonster[iIndex]->Now_Monster != MON_NEW_SCARECROW, iIndex );
//        //g_MyD3D->m_AITemplet.ResetAIObject( iIndex );       
//
//        // 예전 몬스터들은 나름대로 등장하는 효과가 있음
//        if ( bRegenEffect_ )
//            SendRisenEffectPacket( pos );
//    }
//    else
//    {
//        NPC TransNPC;
//        TransNPC.Action = 7 + iMonID_;
//        TransNPC.IsRight = g_kMonsterManager.m_mapMonster[iIndex]->m_abIsRight[0];
//        TransNPC.What = iIndex+2;
//        TransNPC.x = pos.x;
//        TransNPC.y = pos.y;
//        g_MyD3D->m_NPC.PushNPC_Action( &TransNPC );
//    }
//
//	////이런건 싫지만 ㅡ
//	//if ( g_kMonsterManager.m_mapMonster[iIndex]->m_iMonsterType == MON_NATIVE_SPEARMAN && g_kMonsterManager.m_mapMonster[iIndex]->m_pModelDie )
//	//{
//	//	int iMeshObjNum = g_kMonsterManager.m_mapMonster[iIndex]->m_pModelDie->GetMeshObjectNum();
//	//	for ( int i = 0 ; i < iMeshObjNum; i++ )
//	//	{
//	//		g_kMonsterManager.m_mapMonster[iIndex]->m_pModelDie->GetMeshObject(i)->m_bRender = true;
//	//	}		
//	//}
//}

//void KGCGameModeInterface::ResurrectMonster( int MonsterIndex_ )
//{
//	//if ( !g_kMonsterManager.m_mapMonster[MonsterIndex_]->EnableCreate() )
//	//{
//	//	return;
//	//}
//
//	//g_kMonsterManager.m_mapMonster[MonsterIndex_]->IsMonsterTime = true;
//	//g_kMonsterManager.m_mapMonster[MonsterIndex_]->UpdateMonsterPoint();
//	//g_MyD3D->m_AITemplet.AIEnable( true , MonsterIndex_ );
//	//g_MyD3D->m_AITemplet.GetAIObject( MonsterIndex_ )->motion = EGCSM_STANDUP;
//}

//void KGCGameModeInterface::SendRisenEffectPacket( D3DXVECTOR2& vPos )
//{
//    LTP_MAGICEFF_SYNCH EffPacket;
//    ZeroMemory( &EffPacket, sizeof(EffPacket) );
//    EffPacket.packetType   = ELTP_MAGICEFF;
//    EffPacket.size         = sizeof(EffPacket);
//    EffPacket.latencyTime  = GC_ELAPSED_TIME * 10.0f;
//    EffPacket.iMagicEffType = 1;
//    EffPacket.vecPos = D3DXVECTOR3( vPos.x, vPos.y, 0.0f );
//    g_LatencyTransfer.PushPacket( &EffPacket );
//}

HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_SKY()
{
	if (!g_MyD3D->m_KGCOption.Is1Back())
	{
		return S_OK;
	}

	if (g_MyD3D->MyHead->White_Count != 1 && g_MyD3D->MyHead->White_Count != 2)
	{
		if (SiKGCRoomManager()->IsMonsterGameMode())
		{
			//좀 변태스러운가??;; 판타스틱 맵의 렌더에서는 스카이 박스 인지 아닌지파라미터로 받는다.
			SiKGCFantasticMap()->Render(GC_LAYER_SKY);
		}
		else
		{
			g_MyD3D->MyStg->RenderSky();
		}
		g_ParticleManager->Render(GC_LAYER_SKY);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_SKY);
	}
	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_BETWEEN_SKY_BLOCK()
{
	g_ParticleManager->Render(GC_LAYER_BETWEEN_SKY_BLOCK);
	g_RenderManager->Render(true, NULL, NUM_TECHNIQUE, GC_LAYER_BETWEEN_SKY_BLOCK);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_BETWEEN_SKY_BLOCK);
	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_BLOCK()
{
	if (g_MyD3D->MyHead->White_Count != 1 && g_MyD3D->MyHead->White_Count != 2)
	{
		if (SiKGCRoomManager()->IsMonsterGameMode())
		{
			//좀 변태스러운가??;; 판타스틱 맵의 렌더에서는 스카이 박스 인지 아닌지파라미터로 받는다.
			SiKGCFantasticMap()->Render(GC_LAYER_BLOCK);
		}
		else
		{
			g_MyD3D->MyStg->RenderBack1();
		}
		g_ParticleManager->Render(GC_LAYER_BLOCK);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_BLOCK);
	}
	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR()
{
#if defined( USE_AGIT_SYSTEM )
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
	{
		g_pGCDeviceManager2->PushState();
		{
			g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			SiKGCAgitGameManager()->Render(GC_LAYER_BETWEEN_BLOCK_CHAR);
		}
		g_pGCDeviceManager2->PopState();
	}
#endif
	g_ParticleManager->Render(GC_LAYER_BETWEEN_BLOCK_CHAR);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_BETWEEN_BLOCK_CHAR);
	return S_OK;
}
// 2007/2/13. iridology. Text만 그리는 레이어 입니다.
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_TEXT()
{
	g_pGCDeviceManager2->PushState();
	{
		// 펫의 이름을 표시한다.
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);


		for (int i = 0; i < MAX_PLAYER_NUM; i++)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
			{
				g_MyD3D->MyPlayer[i]->Draw_UserNickName();
				g_MyD3D->MyPlayer[i]->Draw_GuildMark();
				g_MyD3D->m_akPet[i]->RenderPetName();
			}
		}

#if !defined( ANGELS_EGG_REFORM )
		if (GC_GM_ANGELS_EGG == SiKGCRoomManager()->GetGameMode())
			((KGCAngelsEgg*)g_pStateGame->GetCurGameModePtr())->HpRender();
#endif
		for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			mit->second->HPGaugeRender();
			mit->second->TextOutLineRender();
			mit->second->TextRender();
			//mit->second->SetChampionParticle();
		}

		SiKGCEmbarkManager()->TextRender();
		SiKGCWallManager()->TextRender();
		SiKGCSubjectManager()->TextRender();

		g_pGCDeviceManager2->PushState();
		{
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			// 2007/2/23. iridology. 게임 끝났을때 Exp그리는 부분
			g_MyD3D->MyHead->Render();
		}
		g_pGCDeviceManager2->PopState();
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_CHAR()
{
	//g_kCamera.UpdateCameraMatrix();
	//g_MyD3D->Set_ShellMatrix( CHAR_SCALE_RATIO );
	g_MyD3D->m_kAfterImageRenderer.Render();
	g_MyD3D->m_pFrustum->Construct(1000.0f);
	g_RenderManager->Render();
	//g_MyD3D->Set_ShellMatrix( DEFAULT_MONSTER_SCALE_SIZE );
	//for( int i = 0; i < MAX_MONSTER; i++ )
	//{
	//    g_kMonsterManager.m_mapMonster[i]->Render();
	//}

	//for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	//{
	//    g_MyD3D->m_akPet[i]->Render();
	//}

	g_ParticleManager->Render(GC_LAYER_CHAR);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_CHAR);

	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_BETWEEN_CHAR_FRONT()
{
	// Jaeho 2007.12.06
	// Object가 CKTDGAnim::CAnimInstance으로 짜여 져 있다.
	// 캐릭터 그림자 보다 뒷쪽에 그려서 그림자와 Object를 순서대로 겹치게 하였음...
	// (문위에 바닥과 알파블랜딩 된 그림자가 올라와서)
	// Object를 이렇게 뿌리는건 마음에 안들지만...
#if !defined( __PATH__ )
	if (g_MyD3D->IsGifRecordMode())
		goto NotRenderObject;
#endif
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, TRUE);

		for (int i = 0; i < MAX_OBJECT; ++i)
		{
			if (g_MyD3D->m_pObject[i])
				g_MyD3D->m_pObject[i]->Render();
		}
	}
	g_pGCDeviceManager2->PopState();

#if !defined(__PATH__)
	NotRenderObject:
#endif

				   if (g_MyD3D->m_KGCOption.IsShadow())
				   {
					   g_pGCDeviceManager2->PushState();
					   {
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

						   for (int i = 0; i < MAX_PLAYER_NUM; i++)
						   {
							   g_MyD3D->MyPlayer[i]->Shadow_Render();
							   g_MyD3D->m_akPet[i]->RenderShadow();
						   }
						   for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
							   mit->second->ShadowRender();
						   }
					   }
					   g_pGCDeviceManager2->PopState();
				   }

#if defined( USE_AGIT_SYSTEM )
				   if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
				   {
					   g_pGCDeviceManager2->PushState();
					   {
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
						   SiKGCAgitGameManager()->Render(GC_LAYER_BETWEEN_CHAR_FRONT);
					   }
					   g_pGCDeviceManager2->PopState();
				   }
#endif

				   g_pGCDeviceManager2->PushState();
				   {
					   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					   g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
					   g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
					   g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					   g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

					   for (int i = 0; i < MAX_PLAYER_NUM; i++)
					   {
						   if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING)) {
							   g_MyD3D->MyPlayer[i]->Draw_Slash();
#if !defined( __PATH__ )
							   g_MyD3D->MyPlayer[i]->RenderRangeCheckBoundingBox();
#endif
						   }
					   }

					   g_pGCDeviceManager2->PushState();
					   {
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
						   g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
						   //g_MyD3D->m_AITemplet.Render();
						   g_MyD3D->m_pPetAnim->Render();
					   }
					   g_pGCDeviceManager2->PopState();

					   g_MyD3D->m_pMapAnim->Render();

					   // 이거 위치 바꾸지 말그라.
					   // Jaeho.Ready
					   //g_MyD3D->m_pkQuestGameMgr->Render();

					   // 테스트용 메쉬이펙트 
					   //g_pMeshEffectManager->Render();
					   if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
						   SiKGCMinigameManager()->Render();

					   g_MyD3D->MySparks->Render();
					   g_MyD3D->m_pDamageManager->Render();
					   g_AnimManager.Render();
					   g_ParticleManager->Render(GC_LAYER_BETWEEN_CHAR_FRONT);
					   SiKGCInGamePrograssBar()->Render(GC_LAYER_BETWEEN_CHAR_FRONT);

					   SiKGCEmoticonManager()->Render();

					   // 아이템은 제일 아래에 위치 시켜 주도록 하자.
					   g_MyD3D->MyItms->Alpha_Render(); // 땅에 떨어진 아이템.
					   SiKGCDropItemManager()->Alpha_Render();
				   }
				   g_pGCDeviceManager2->PopState();

				   return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_FRONT_MAP()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);

		//g_MyD3D->MyStg->Water_Render();
		if (SiKGCRoomManager()->IsMonsterGameMode())
		{
			SiKGCFantasticMap()->Water_Render();
		}
		else
		{
			g_MyD3D->MyStg->FrontMap_Render();
			g_MyD3D->MyStg->RenderMap();
		}
		g_MyD3D->MyHead->Alpha_Render(); // 궁수대회 전광판 및 화면 번쩍-_-;
		g_ParticleManager->Render(GC_LAYER_FRONT_MAP);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_FRONT_MAP);
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}
HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_FACE()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);

		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//g_MyD3D->MyBuff->Render();
		g_pMagicEffect->RenderHUD();
		g_MyD3D->MyItms->My_Item_Alpha_Render();

#if defined( __PATH__ )
		if (g_MyD3D->m_KGCOption.Is3Back())
#else
		if (g_MyD3D->m_KGCOption.Is3Back() && g_MyD3D->IsGifRecordMode() == false)
#endif
		{
			for (int i = 0; i < MAX_PLAYER_NUM; i++)
			{
				if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
					g_MyD3D->MyPlayer[i]->Draw_Face();
			}
		}

		g_ParticleManager->Render(GC_LAYER_FACE);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_FACE);
	}
	g_pGCDeviceManager2->PopState();
	return S_OK;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_UNDER_UI()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		g_ParticleManager->Render(GC_LAYER_UNDER_UI);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_UNDER_UI);
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_UI()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		SiKGCInGamePrograssBar()->Render(GC_LAYER_UI);
		g_MyD3D->m_pkUIMgr->Render();

		//if( g_pkUIScene->IsMsgBoxModal() )

#if defined( USE_AGIT_SYSTEM )
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
		{
			g_pGCDeviceManager2->PushState();
			{
				g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
				g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				SiKGCAgitGameManager()->RenderAfterUI();
			}
			g_pGCDeviceManager2->PopState();
		}
#endif

#ifdef _OPEN_TEST_
		if (g_MyD3D->MyReplay->m_iCurrState == RP_NONE)
		{
			g_MyD3D->RenderRightBottomText(L"TEST...");
		}
#endif // _OPEN_TEST_

		if (o10 < 12)
		{
			//if( g_MyD3D->MyReplay->m_iCurrState == RP_RECORD ||  g_kGlobalValue.IsCapturing() )
			if (g_MyD3D->MyReplay->m_iCurrState == RP_RECORD)
			{
				g_MyD3D->RenderRightBottomText(L"RECORD...");
			}
			else if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
			{
				g_MyD3D->RenderRightBottomText(L"REPLAY...");
			}
		}

#ifdef _DEBUG
		g_MyD3D->RenderDeviceStats();
#endif // _DEBUG
		g_MyD3D->RenderFPS();
		g_MyD3D->m_pUIAnim->Render();
		g_ParticleManager->Render(GC_LAYER_UI);
		g_MyD3D->MyCtrl->Mouse_Render();
	}
	g_pGCDeviceManager2->PopState();
	return S_OK;
}

void KGCGameModeInterface::CheckRebirth()
{
	int iMe = g_MyD3D->Get_MyPlayer();

	// [12/17/2007] breadceo. 수반 출력 메세지 삭제
	//g_pkGameOverlayUI->EnableRebirthMessage( false );

	if (iMe == 7 || g_pkQuestManager->m_bEndGame == true)
		return;

	// 단축슬롯에 있는 수반을 이용한 부활
	EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
	if (g_MyD3D->GetGameCategoryKind(eGameModeCategory) == GC_GMK_MONSTER)
	{
		if (g_MyD3D->MyCtrl->k_ShortCut_Item != KEY_UP && m_bRequestRebirth == false)
		{
			int iIndex = static_cast<int>(g_MyD3D->MyCtrl->k_ShortCut_Item);

			if (g_MyD3D->MyPlayer[iMe]->IsNotRebirth())
			{
				if (g_MyD3D->GetMyShortCutSlot()->IsGameModeAllowed(SiKGCRoomManager()->GetGameMode(), iIndex) && g_MyD3D->GetMyShortCutSlot()->CanUseItem(iIndex))
				{
					if (g_MyD3D->IsProtectRing(g_MyD3D->GetMyShortCutSlot()->GetItemID(iIndex)))
					{
						if (g_MyD3D->GetMyShortCutSlot()->UseItem(static_cast<int>(iIndex)) == true)
						{
							m_bRequestRebirth = true;
						}
					}
				}
			}
			else
			{
				if (g_MyD3D->IsProtectRing(g_MyD3D->GetMyShortCutSlot()->GetItemID(iIndex)))
				{
					if (g_MyD3D->GetMyShortCutSlot()->UseItem(static_cast<int>(iIndex)) == true)
					{
						m_bRequestRebirth = true;
					}
				}
			}

			g_MyD3D->MyCtrl->k_ShortCut_Item = KEY_UP;
			return;
		}

		// [12/18/2007] breadceo. 여기서 부터는 보너스 포인트를 이용한 부활
		else if ((int)g_MyD3D->MyPlayer[iMe]->m_cLife <= 0 && !g_MyD3D->MyPlayer[iMe]->IsNotRebirth() && g_MyD3D->MyCtrl->k_ShortCut_Item == KEY_UP)
		{

			if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST0) {
				g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Rebirth();
				g_pkChatManager->AddSystemMsg(-1, g_pkStrLoader->GetString(STR_UNLIMITED_REBIRTH_SYSTEM_MSG));
			}

			else if (!g_MyD3D->MyHead->IsCheatMode && (g_pkInput->IsDown(DIK_C)
#ifdef USE_JOYSTICK
				|| g_MyD3D->GetJoystickKey() == EJK_KEY_START
#endif
				)
				&& false == m_bRequestRebirth && VerifyRebirth() == true)
			{
				if (SiKGCElvis()->IsRevivalable())
				{
					// 우정의 반지가 반지가 있다면?
					if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ELESIS)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LIR)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ARME)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LAS)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RYAN)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RONAN)
						|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_AMY)
						)
					{
						g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Rebirth();
						//SiKGCElvis()->IncreaseRevivalCount();

						g_pStateGame->GetCurGameModePtr()->SetRequestRebirth(false);
						g_pStateGame->GetCurGameModePtr()->SetLastRebirthTime(::timeGetTime());
					}
					else
					{
						// 서버로 요청했냐
						m_bRequestRebirth = true;
						KP2P::GetInstance()->Send_UseBonusPoint();
					}
				}
				else
				{
					g_MyD3D->m_kChatManager.AddSystemMsg(-1,
						g_pkStrLoader->GetReplacedString(STR_ID_EXCEED_REBIRTH_COUNT, "i", SiKGCElvis()->GetMaxRevivalCount()),
						KGCChatManager::CHAT_TYPE_ALERT);
				}
			}
		}
	}
}

void KGCGameModeInterface::Rebirth(DWORD dpnid)
{
	int iMyPlayer = g_MyD3D->Get_MyPlayer();

	bool bUseEventItem = false;

	if (dpnid == g_kGlobalValue.m_kUserInfo.dwUID)
	{
		if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING_PC))
		{
			if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING_QUESTPACKAGE))
			{
				if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING_10_DISCOUNT))
				{
					if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING_DISCOUNT))
					{
						if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING_10))
						{
							if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_PROTECT_RING))
							{
								if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_VITTAR_POTION_OF_RESURRECTION))
								{
									if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_LISNAR_PRAY))
									{
										if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEMID_HEITAROS_PRAY))
										{
											if (!g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[iMyPlayer]->GetCurrentChar(), ITEM_HENIR_RESS_POT))
											{

											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (int iPlayerIdx = 0; iPlayerIdx < MAX_PLAYER_NUM; iPlayerIdx++)
	{
		if (g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.dwUID == dpnid)
		{
			if (false == bUseEventItem) {
				g_MyD3D->m_kChatManager.AddSystemMsg(-1, g_pkStrLoader->GetReplacedString(STR_ID_GAMEMSG_REBIRTH, "s", L"#c" + g_MyD3D->MyPlayer[iPlayerIdx]->GetStrUserColor() + g_MyD3D->MyPlayer[iPlayerIdx]->GetStrUserName().c_str() + L"#cx"));
			}
			else {
				g_MyD3D->m_kChatManager.AddSystemMsg(-1, g_pkStrLoader->GetReplacedString(STR_ID_GAMEMSG_REBIRTH, "s", L"#c" + g_MyD3D->MyPlayer[iPlayerIdx]->GetStrUserColor() + g_MyD3D->MyPlayer[iPlayerIdx]->GetStrUserName().c_str() + L"#cx"));
			}
		}
	}

	m_dwLastRebirthTime = timeGetTime();
	g_MyD3D->MyPlayer[iMyPlayer]->Rebirth();
}

bool KGCGameModeInterface::VerifyRebirth()
{
	if (IsDetermineWin)
		return false;

	//5초 안에는 두번 사용되지 못함
	if (timeGetTime() < m_dwLastRebirthTime + 5000)
		return false;

	return true;
}

void KGCGameModeInterface::InitRebirthTimer()
{
	m_bRebirthProcessing = false;
	m_nRebirthTimer = LAST_REBIRTH_CHANCE_TERM;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_ANNOUNCE()
{
	// Ui랑 같게..
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		SiKGCAnnounceMgr()->Render();
		g_ParticleManager->Render(GC_LAYER_CUSTOM);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_CUSTOM);
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_LAYER_WEATHER_SYSTEM()
{
#if defined( USE_WEATHER_SYSTEM )
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		SiKGCWeatherSystem()->Render();
	}

	g_pGCDeviceManager2->PopState();
#endif
	return S_OK;
}

void KGCGameModeInterface::GameEndProcess()
{
	IsDetermineWin = true;
	CalcExpAndGP();
	//ShowLevelUp();

	g_pStateGame->SendEndGamePacket();
	g_kCamera.m_bZoomViewMode = false;
}

HRESULT KGCGameModeInterface::ON_GC_LAYER_SCROLLED_LAYER1()
{
	if (SiKGCRoomManager()->IsMonsterGameMode())
		SiKGCFantasticMap()->Render(GC_LAYER_SCROLLED_LAYER1);
	g_ParticleManager->Render(GC_LAYER_SCROLLED_LAYER1);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_SCROLLED_LAYER1);
	return S_OK;
}

HRESULT KGCGameModeInterface::ON_GC_LAYER_SCROLLED_LAYER2()
{
	if (SiKGCRoomManager()->IsMonsterGameMode())
		SiKGCFantasticMap()->Render(GC_LAYER_SCROLLED_LAYER2);
	g_ParticleManager->Render(GC_LAYER_SCROLLED_LAYER2);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_SCROLLED_LAYER2);
	return S_OK;
}

HRESULT KGCGameModeInterface::ON_GC_LAYER_SCROLLED_LAYER3()
{
	if (SiKGCRoomManager()->IsMonsterGameMode())
		SiKGCFantasticMap()->Render(GC_LAYER_SCROLLED_LAYER3);
	g_ParticleManager->Render(GC_LAYER_SCROLLED_LAYER3);
	SiKGCInGamePrograssBar()->Render(GC_LAYER_SCROLLED_LAYER3);
	return S_OK;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_CHAR_OVER_UI_1()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		g_ParticleManager->Render(GC_LAYER_CHAR_OVER_UI_1);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_CHAR_OVER_UI_1);
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}

HRESULT KGCGameModeInterface::ON_RENDER_GC_CHAR_OVER_UI_2()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		g_ParticleManager->Render(GC_LAYER_CHAR_OVER_UI_2);
		SiKGCInGamePrograssBar()->Render(GC_LAYER_CHAR_OVER_UI_2);
	}
	g_pGCDeviceManager2->PopState();

	return S_OK;
}


float KGCGameModeInterface::GetCharManaAutoRecoveryRatio()
{
	float fManaAutoRecoveryRatio = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fManaAutoRecoveryRatio;

	if (fManaAutoRecoveryRatio == -1.f)
		return 1.0f;

	if (g_kGlobalValue.IsHybridPlayer(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return fManaAutoRecoveryRatio;

	if (g_kGlobalValue.IsCharUseSpecial4(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return fManaAutoRecoveryRatio;

	if (g_kGlobalValue.IsQuickSkillPlayer(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return fManaAutoRecoveryRatio;

	return 1.0f;
}

bool KGCGameModeInterface::IsMPRecoveryMode()
{
	if (SiKGCRoomManager()->IsMonsterGameMode() == false)
		return true;

	if (SiKGCWorldMapManager()->IsNoMPRecoveryMode(SiKGCRoomManager()->GetGameMode()))
		return false;

	if (g_kGlobalValue.IsQuickSkillPlayer(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return true;

	if (g_kGlobalValue.IsHybridPlayer(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return true;

	if (g_kGlobalValue.IsCharUseSpecial4(g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType))
		return true;

	return false;
}

void KGCGameModeInterface::CheckStartPlayer()
{
	//  현재 플레이 인원 확인
	int nPlyerCount = 0;

	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
			nPlyerCount++;
	}

	g_kGlobalValue.m_nPlayerCount = nPlyerCount;
}