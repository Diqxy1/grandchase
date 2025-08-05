#include "stdafx.h"
//
#include "KGCQuest.h"

#include "QuestGameManager.h"

#include "Headup Display.h"
#include "Monster.h"

#include "KStats.h"
#include "Message.h"
#include "KGCGameBoard.h"

#include "gcui/GCGameOverlayUI.h"
#include "gcui/GCQuestTileMap.h"

#include "Controls.h"
#include "EveryTriggerManager.h"
#include "gcui/GCGameOverlayUI.h"
#include "KGCElvis.h"
#include "gcportal.h"
#include "KGCSyncObject.h"
#include "gcui/KGCPrograssBar.h"
#include "KGCEmbarkManager.h"
#include "./MapSystem/GCWallManager.h"

#include "GCBuffDonationManager.h"
#include "KGCSubjectManager.h"
#include "GCUI/GCTutorialDlg.h"


KGCQuest::KGCQuest(void)
{
	m_fAccumulateTime = 0.0f;
	m_fEventTime = (float)INT_MAX;
	
    InitRebirthTimer();	
	
}

KGCQuest::~KGCQuest(void)
{
}

HRESULT KGCQuest::InitAtGame()
{	
    #if defined( NATION_KOREA )
    {
        m_fAccumulateTime = 0.0f;
        m_fEventTime = 60.0f;
        #ifndef __PATH__
        {
            g_kStatsManager.InsertPage( L"GAME TIME" );
            g_kStatsManager.InsertValue( L"AccumulateTime", &m_fAccumulateTime );
        }
        #endif
    }
    #endif

    //m_iRebirthCount = 0;
    m_dwLastRebirthTime = 0;

    m_bRequestRebirth = false;
    SiKGCElvis()->SetRevivalCount(0);
	
    // 보드에서 플레이 시간 초기화
    g_pGameBoard->InitPlayTime();

#if defined( USE_ONGAME_INVENTORY ) 
    g_pItemMgr->ClearNewItem();
    g_MyD3D->MyCtrl->k_Game_Inventory = false;
#endif

    // 다음 던전을위해 소환 초기화
    g_kGlobalValue.SetEscortMonsterSummonState( false );

    CheckStartPlayer();

    return S_OK;
}

void KGCQuest::GameEnd()
{
    m_bRequestRebirth = false;
    SiKGCElvis()->SetRevivalCount(0);
}

HRESULT KGCQuest::CalcExpAndGP()
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Description by parkch
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // 퀘스트 모드의 승패 결정 방식
    // 승자 - 게임 도중에 나가지 않은 사람
    // 패자 - 게임 도중에 나간 사람
    //
    // 퀘스트 모드의 점수 배분 방식
    // 1. 클리어한 스테이지의 레벨을 확인하여 가산치를 계산한다.
    // 2. 승자는 각 레벨별로 받을 수 있는 Exp/GP에 가산치를 부가해서 받는다.
    // 3. 패자는 아무 변화가 없다.
    // 4. MVP는 선정하지 않는다.
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
	{
		if( g_pGameBoard->m_pStartingMember[i].IsLive )
		{
			// 승리로 판정한다.
			g_pGameBoard->m_pStartingMember[i].IsWin = g_MyD3D->m_pkQuestGameMgr->m_bWin;
		}
	}

	return S_OK;
}

bool KGCQuest::CheckGameEnd()
{
    // 룰렛 정보를 보내지 않았다면 대기.
    if( !g_pkQuestManager->m_bSendRouletteList ) return false;

    if( g_pkQuestManager->m_bRouletteList 
        || (timeGetTime() - g_pkQuestManager->m_dwRouletteWaitDelay) > KQuestGameManager::ROULETTE_DEALY_TIME )
    {
        // 확인사살...
        SendNetworkData(GC_PID_QUEST_REAULT_ITEM_DICE, 0);

        g_MyD3D->SetEnableKeyInput(true);

		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			mit->second->SetMovement(true);
		}
        return true;
    }
    else
    {
        if( g_pkQuestManager->m_bEndGame )
        {
            g_pkQuestManager->CheckCompleteRouletteWaitList();
        }
    }

    //테스트 코드
    return true;
	//return false;
}

void KGCQuest::FrameMove_GameEnd(void)
{
    if( !g_pkQuestManager->m_bEndGame ) return;

#if defined( USE_ONGAME_INVENTORY )
    if( KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_GAME_INVENTORY ) ) { 
        g_pkUIScene->CloseGameInventory();
    }
#endif

    // 플레이어의 게임 결과 정보(랭크관련)를 보내고, 다른 사람들의 정보가 수신되었나 확인한다.
    if( !g_pkQuestManager->m_bGetEndGameInfo )
    {
        g_pkQuestManager->CheckCompleteEndGameInfo();
        if( !g_pkQuestManager->m_bSendGetEndGameInfo )
        {
            m_dwEndTimer = 0;
            g_pkQuestManager->m_bSendGetEndGameInfo = true;

			if( !g_MyD3D->IsLocalPlayerObserver() )
			{
				g_pkQuestManager->SaveToQuestPointList(g_pkQuestManager->m_iCurStage
					, g_MyD3D->Get_MyPlayer()
					, g_pkQuestManager->m_PlayerQuestPoint[g_MyD3D->Get_MyPlayer()]);
                SendNetworkData(GC_PID_BROAD_CONTROL_MISSION_DATA, 0);
				SendNetworkData(GC_PID_QUEST_STAGE_RESULT_SCORE, 0);
			}
            g_pkQuestManager->m_dwEndGameInfoDelayTime = timeGetTime();            
        }

        // 이건 반드시 여기 있어야 한다. m_bSendGetEndGameInfo 를 지나면서 timeGetTime()         
        // 값을 넣어줘야지 딜레이 시간체크가 되기 때문이다.
        if( (timeGetTime() - g_pkQuestManager->m_dwEndGameInfoDelayTime) > KQuestGameManager::ENDGAMEINFO_DEALY_TIME )
            g_pkQuestManager->m_bGetEndGameInfo = true;
    }
	else if( m_dwEndTimer < GC_FPS_LIMIT && !g_MyD3D->IsLocalPlayerObserver() )      // 1초정도 텀을 둔다.
    {
        // 확인 사살...
        if( 0 == m_dwEndTimer )
        {
            g_pkQuestManager->SaveToQuestPointList(g_pkQuestManager->m_iCurStage
                , g_MyD3D->Get_MyPlayer()
                , g_pkQuestManager->m_PlayerQuestPoint[g_MyD3D->Get_MyPlayer()]);
            SendNetworkData(GC_PID_BROAD_CONTROL_MISSION_DATA, 0);
            SendNetworkData(GC_PID_QUEST_STAGE_RESULT_SCORE, 0);
        }
        ++m_dwEndTimer;
    }
    else if( !g_pkGameOverlayUI->GetQuestResultS3()->IsRenderOn() )        // 결과창을 생성한다.
    {
        g_pkGameOverlayUI->EndMinimap();
        g_pkGameOverlayUI->StartQuestResult();
    }
}

HRESULT KGCQuest::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCQuest::CustomFrameMove");

	SendP2PPacket( GC_PID_RELAY_PING );
    g_MyD3D->Story++;    

    // 게이트 이동 체크
    g_pkQuestManager->GateMoveGateCheck();	


    FrameMove_GameEnd();
    g_pkQuestManager->FrameMove_WaterPole();

	if(iGC_FPS == 0)
		g_pkQuestManager->CheckMonsterExistInScreen();
    
	g_pkQuestManager->CheckPortalInScreen();

	//퀘스트
	if (g_kEveryTriggerManager.MovetoNextStage() == true)
	{
		if (g_pkGameOverlayUI->m_pkAddtionGuide)
        {
            if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
                g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(false);
            else
			    g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(true);
        }

        //게임서버로 자신의 로딩이 끝났음을 알린다.
        KP2P::GetInstance()->Send_StageLoadCompleteNot();
		SendNetworkData( GC_PID_UDP_RELAYSERVER_HEART_BEAT, 0 );        
		SendP2PPacket( GC_PID_RELAY_PING );
#if defined(USE_PING_STATE_UI)
        KP2P::GetInstance()->Send_RoomMemberPingInfoReq();

    #if defined(USE_ONLY_TCP_RELAY) || defined(USE_P2P_OR_TCP_RELAY)
        KP2P::GetInstance()->Send_PingInfoNot(g_kGlobalValue.GetTRAvgPing());
    #else
        SendPingToTCPRelay();
    #endif
#endif

		// 로딩단계를 18단계로 구분하기 때문에 18을 넣어줌
		g_pkUIScene->m_pkGameOverlayUI->DungeonMyLoadingState(18);    
		KP2P::GetInstance()->Send_RelayLoadingState(std::make_pair( g_kGlobalValue.m_kUserInfo.dwUID, 18));

        // 매 스테이지마다 화령의 축복 버프를 걸어줌(이벤트)
       if( SiGCBuffDonationManager()->IsBuffOn() )
           SiGCBuffDonationManager()->SetRandomBuff();
	}

    // 스테이지 로딩이 완료되었나 확인해본다.
    if( !g_pkQuestManager->IsStageLoadComplete() )
    {

        // 30초가 지나도 로딩이 끝나지 않는 경우를 체크해서 서버로 리포팅한다. 30초동안 스테이지로딩이 끝나지 않으면 분명 문제가 있다.
        if( g_pkQuestManager->m_kStageLoadingStopWatch.IsStop() )
        {
            g_pkQuestManager->m_kStageLoadingStopWatch.SetStopCheck(true);
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_STAGE_LOADING_OVER_TIME );
 
        }	
        return S_OK;
    }
		
    // 퀘스트트리거를 구동한다.
    g_kEveryTriggerManager.FrameMove();
    if( g_kEveryTriggerManager.FrameMove_NotMatchTrigger() )      // 트리거가 매치가 안되었을 경우 타이머(10초) 후 퀘스트에서 나간다.
        return E_FAIL;

    g_pkGameOverlayUI->RenderDungeonLoadingState(false);

    // 로딩 화면이 켜져 있는 상태거나, 스테이지 시작 딜레이 시간이 남은 상태에서는 FrameMove를 돌지 않는다.
    if( !g_pkQuestManager->IsStartedStage() && g_pkQuestManager->m_iCurStage == 0 )
    {
        // Jeaho.Test UDP Connect Test
        // 릴레이 서버에서 제명되었을 수도 있기 때문에 릴레이 서버를 사용한다.
        if( !o30)
            SendNetworkData( GC_PID_UDP_RELAYSERVER_HEART_BEAT, 0 );
        //ASSERT( false && "UDP_DisConnect Test" );
        return S_OK;
    }    

    if( !g_pkQuestManager->m_bQuestStarted )
    {
        g_pkQuestManager->SetEnableMovement(true);
        g_pkQuestManager->m_bQuestStarted = true;

        if( g_kCamera.GetQuestStartedOfResetCamera() )
        {
            g_kCamera.InitCamera();
            bool bReSetWideMode = true;
            int iMyPlayer = g_MyD3D->Get_MyPlayer();
            if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
            {
                SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
                if( 0 == g_kEveryTriggerManager.GetActiveStage() )
                {
                    if( pCameraZoom && 0 < pCameraZoom->m_fFristZoom )
                    {
#if defined( USE_CAMERA_ZOOM_STATE )
                        if( g_MyD3D->IsPlayerIndex( iMyPlayer ) )
                        {
                            g_MyD3D->MyPlayer[iMyPlayer]->AddCameraZoomState( g_kCamera.m_fWideMode, pCameraZoom->m_fFristZoom );
                        }
#endif
                        g_kCamera.m_fWideMode = pCameraZoom->m_fFristZoom;
                        g_kCamera.SavePrevState();
                        bReSetWideMode = false;

                        if( 0 < int(pCameraZoom->m_vecZoomStep.size()) )
                        {
                            int iStep=0;
                            std::vector<float>::iterator vecIter = pCameraZoom->m_vecZoomStep.begin();
                            for(; vecIter != pCameraZoom->m_vecZoomStep.end();  vecIter++ )
                            {
                                if( pCameraZoom->m_fFristZoom == *vecIter )
                                    break;
                                iStep++;
                            }

                            g_pkGameOverlayUI->ShowZoomUI( !g_kGlobalValue.m_bNoCameraZoom );
                            g_pkGameOverlayUI->SetZoomScrollPos( iStep );
                        }
                    }
                }
                else
                {
                    if( pCameraZoom && (0 < int(pCameraZoom->m_vecZoomStep.size()) || 0 < pCameraZoom->m_fFristZoom) )
                    {
#if defined( USE_CAMERA_ZOOM_STATE )
                        if( g_MyD3D->IsPlayerIndex( iMyPlayer ) )
                        {
                            g_MyD3D->MyPlayer[iMyPlayer]->AddCameraZoomState( g_kCamera.m_fWideMode, g_kCamera.m_fPrevWideMode );
                        }
#endif
                        if( 0 < int(pCameraZoom->m_vecZoomStep.size()) )
                            g_pkGameOverlayUI->ShowZoomUI( !g_kGlobalValue.m_bNoCameraZoom );
                        g_kCamera.BackToPrevState();
                        bReSetWideMode = false;
                    }
                }

                SMiniMapInfo* pMiniMapInfo = SiKGCWorldMapManager()->GetMiniMapInfo( SiKGCRoomManager()->GetGameMode() );
                if( pMiniMapInfo && pMiniMapInfo->m_bIsShow )
                {
                    g_pkGameOverlayUI->SetRealMiniMapInfo( pMiniMapInfo );
                    g_pkGameOverlayUI->ShowRealMiniMap( true );
                }
                else
                    g_pkGameOverlayUI->ShowRealMiniMap( false );

                if ( SiKGCWorldMapManager()->IsSkillListGuideDungeon( SiKGCRoomManager()->GetGameMode() ) )
                {
                    if ( !g_pkGameOverlayUI->IsRenderSkillList() )
                        g_pkTutorialDlg->Enable();
            }
            }

            if( bReSetWideMode )
            {
                if(g_pkQuestManager->IsPlatformer()) // 플래포머인지 확인해보고 카메라 세팅해 보자 
                    g_kCamera.m_fWideMode = 8.0f;
                else
                    g_kCamera.m_fWideMode = 10.0f;
            }
        }
        g_kCamera.SetQuestStartedOfResetCamera( true );

        // 플랫포머 맵에서는 콤보 쿨타임을 20초 준다...
        if( g_pkQuestManager->IsPlatformer() )
            g_MyD3D->MyHead->SetComboTimer( true, 60000 );
        else
            g_MyD3D->MyHead->SetComboTimer( true, 2000 );

        g_pkGameOverlayUI->StartNumberCount(-1);
    }

#if defined(ENABLE_DUGEON_HACK_CHECKING)
	// 해킹체크
	g_pkQuestManager->CheckHackAction();
#endif

    SiKGCEmbarkManager()->FrameMove();

    // 플레이어에 대한 프레임 업데이트 및 충돌처리용 영역을 계산한다.
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        // 퀘스트 클리어 상태면 무적으로 설정.
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING )
            && g_pkQuestManager->m_bClearStage
            )
        {
            g_MyD3D->MyPlayer[i]->Super = 255;
        }

        g_MyD3D->MyPlayer[i]->Calculate_Body_Rect();
        g_MyD3D->MyPlayer[i]->Frame_Move();
    }

    // 몬스터에 대한 프레임 업데이트
    g_kMonsterManager.FrameMove();	

    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Crash_Check();
    }

	// 펫에 대한 프레임 업데이트
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		g_MyD3D->m_akPet[i]->FrameMove();
	}

    // 벽 프레임무브
    SiKGCWallManager()->FrameMove();

    // 싱크오브젝트 프레임무브
    SiKGCSyncObjectManager()->FrameMove();

    // 프로그래스바 프레임무브
    SiKGCInGamePrograssBar()->FrameMove();

	for(int i=0;i<MAX_OBJECT;i++)
	{
		if( g_MyD3D->m_pObject[i] )
		{
			g_MyD3D->m_pObject[i]->FrameMove();			
		}
	}

    // 포탈 로직
	KGCPortal::UpdatePortal();

    // Dialogue 표시상태면 행동을 제한한다.
    if( g_pkGameOverlayUI->IsShowDialogue() )
    {
        if( g_pkQuestManager->IsEnableMovement() )
            g_pkQuestManager->SetEnableMovement(false);
    }

    // 살아있는 사람이 없으면 게임 종료
    int iTotalLife = 0;
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            iTotalLife += g_MyD3D->MyPlayer[i]->m_cLife;
    }

    // [12/18/2007] breadceo. 다 죽었으면 RebirthProcess
    if( !iTotalLife )
    {
        ProcessLastRebirthChance();
    }
    else
    {
        InitRebirthTimer();
		if( g_pkQuestManager->IsStartedStage() && !g_pkGameOverlayUI->IsShowDialogue())
		{
			g_pGameBoard->AdvancePlayTime();
            g_pkQuestManager->SetStartUserCount( g_pGameBoard->GetStartingMemberCount() );
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                if( !g_MyD3D->MyPlayer[i]->IsSpecialSkillMotion() ) {
                    float fDecreaseScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 1 );
                    int iPlayerStartingCnt = g_pGameBoard->GetStartingMemberCount();
                    float fCorrectionValue = 0.f; 
                    float fInfiniteDungeonDecreateValue = 0.f;
                    g_pkQuestManager->GetRankCorrectionValue( iPlayerStartingCnt , 1 , fCorrectionValue);
                    g_pkQuestManager->GetRankCorrectionValue( iPlayerStartingCnt , 3 , fInfiniteDungeonDecreateValue);
                    
                    int iTotalScore = 0;
                    g_pkQuestManager->GetComboDamage( iTotalScore );

                    float fInfiniteCorrectionValue = iTotalScore <= 0 ? fCorrectionValue: fInfiniteDungeonDecreateValue;
                    
                    //if ( !g_pkQuestManager->NotDecreaseCheck() && g_MyD3D->MyPlayer[i]->m_cLife != 0)
                    if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
                        g_pkQuestManager->SetTotalDamagePoint_S6( -(fDecreaseScore * iPlayerStartingCnt * fInfiniteCorrectionValue ), 0, false, i );
                    else
                        g_pkQuestManager->SetTotalDamagePoint_S6( -(fDecreaseScore * iPlayerStartingCnt * fCorrectionValue), 0, false, i );
                }
            }
		}
    }
	CheckRebirth();

    // 다이얼로그 제어
	g_pkQuestManager->FrameMove_Dialogue();
	g_pkQuestManager->FrameMove_Msg();

    // 호위 몬스터 소환
    if( g_kGlobalValue.IsEscortEventActive() == true && 
        g_kGlobalValue.IsEscortMonsterSummoned() == false ){

        g_MyD3D->GetMyPlayer()->SummonEscortEventMonster();

        g_kGlobalValue.SetEscortMonsterSummonState( true );
    }
	
    SiKGCSubjectManager()->FrameMove();


    // 랭크 갱신
    int nCurRank = g_pkQuestManager->GetRankNum();
    int nBeRank = g_pkQuestManager->GetPlayerRank( g_MyD3D->Get_MyPlayer() );
    
    if ( nCurRank != nBeRank ) 
    {
        g_pkQuestManager->CalcPlayerRank();
    }

	return S_OK;
}

HRESULT KGCQuest::Render()
{	
    // NOTE : 퀘스트 로딩 화면 표시 중일 때는 UI만 렌더링한다.
    //if( !g_pkQuestManager->m_bLoadStageByPlayers && g_pkQuestManager->IsStartedStage() )
    if( !g_pkQuestManager->m_bLoadStageByPlayers && g_pkQuestManager->m_iCurStage == 0 )
    {
        return S_OK;
    }

    #if defined( _DEBUG ) || !defined( __PATH__ )
    //////////////////////////////////////////////////////////////////////////
    // Jaeho.Test
    // 기획측의 요청으로 결과 랭크 점수를 마우스 커서에 붙인 것임.
    static bool bShowComboInfo = false;
    if( g_pkInput->IsUp(DIK_0) ) bShowComboInfo = !bShowComboInfo;

    if( bShowComboInfo )
    {
        int iCritical = 0;
        int iDouble = 0;
        int iBack = 0;
        int iCounter = 0;
        int iAerial = 0;
        int iAttacked = 0;
        int iAttack = 0;
        int iSpecial[3] = { 0, };
        int iCombo = 0;

		const int iMyPlayerIndex = g_MyD3D->Get_MyPlayer();
		if( g_MyD3D->IsPlayerIndex( iMyPlayerIndex ) )
		{
			for(int j=0;j<g_kEveryTriggerManager.GetTotalStageCnt();++j)
			{
				if( g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex].count( j ) > 0 )
				{
					iCritical += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].CriticalAttack;
					iDouble += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].DoubleAttack;
					iBack += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].BackAttack;
					iCounter += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].CounterAttack;
					iAerial += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].AerialAttack;
					iAttacked += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].Attacked;
					iAttack += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].Attack;
					iSpecial[0] += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].iSpecial1;
					iSpecial[1] += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].iSpecial2;
					iSpecial[2] += g_pkQuestManager->m_alQuestPoint[iMyPlayerIndex][j].iSpecial3;
				}
			}
		}

        WCHAR szReMainTime[100];
        wsprintf(szReMainTime, L"Critical : %d, Double : %d, Back : %d, Counter : %d"
            , iCritical, iDouble, iBack, iCounter);
        //g_MyD3D->MyHAN_Draw( g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f*0, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );
		g_pGCDeviceManager2->Draw_Text(g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f*0, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true);

        //WCHAR szReMainTime[100];
        wsprintf(szReMainTime, L"Aerial : %d, Attacked : %d, Attack : %d, Special1 : %d"
            , iAerial, iAttacked, iAttack, iSpecial[0]);
		g_pGCDeviceManager2->Draw_Text(g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*1, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );
        //g_MyD3D->MyHAN_Draw( g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*1, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );

        //WCHAR szReMainTime[100];
        wsprintf(szReMainTime, L"Special2 : %d, Special3 : %d, Combo : %d"
            , iSpecial[1], iSpecial[2], iCombo);
		g_pGCDeviceManager2->Draw_Text(g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*2, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true);
        //g_RenderManager->DrawText( g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*2, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );

        //WCHAR szReMainTime[100];
        wsprintf(szReMainTime, L"Power : %d, Control : %d, Attacked : %d, TotalScore : %d"
            , g_pkQuestManager->GetPowerPoint(), g_pkQuestManager->GetControlPoint(), g_pkQuestManager->GetAttackedPoint(), g_pkQuestManager->GetTotalScorePoint());
		g_pGCDeviceManager2->Draw_Text(g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*3, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true);
        //g_MyD3D->MyHAN_Draw( g_MyD3D->MyCtrl->m_x + 0.15f, g_MyD3D->MyCtrl->m_y + 0.30f + 0.1f*3, szReMainTime, 0xffffffff, SiKGCMultipleLanguages()->GetDefualtFontSize(), true );
    }
    //////////////////////////////////////////////////////////////////////////
    #endif
    

    KGCGameModeInterface::Render();

    SiKGCSyncObjectManager()->Render();

    return S_OK;
}

HRESULT KGCQuest::InitAtLoading()
{
    g_MyD3D->m_pkQuestGameMgr->EnterQuest();
    return S_OK; 
}

void KGCQuest::ProcessLastRebirthChance()
{
    if( m_bRebirthProcessing == false )
    {
        m_bRebirthProcessing = true;
        m_nRebirthTimer = LAST_REBIRTH_CHANCE_TERM;
        return;
    }

    m_nRebirthTimer--;

    if( m_nRebirthTimer <= 0 )
    {
        // 게임을 끝낸다
        if( !g_pkQuestManager->m_bEndGame )
        {
            g_pkQuestManager->m_bEndGame = true;
            IsDetermineWin = false;
            g_pkQuestManager->SetShowDialogue(false);
            g_MyD3D->MyHead->Start_Text( "FAILED" );
        }
        InitRebirthTimer();
    }
}