#include "stdafx.h"
//
#include "GCStateGame.h"
#include "KGCGuildWar.h"
#include "KGCMatch.h"
#include "KGCDeathMatch.h"
#include "KGCQuest.h"
#include "KGCAgit.h"

#include "Spark.h"
#include "DamageManager.h"
//
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameDlg.h"

#include "GCUI/GCLoadingScene.h"


#include "Procedure.h"


#include "Stage.h"
#include "Headup Display.h"
#include "Item.h"
#include "Buff/KGCBuffManager.h"

#include "PetAI.h"
#include "KGCGameBoard.h"
#include "QuestGameManager.h"
#include "Controls.h"
#include "KGCIndigo.h"
#include "KGCTagMatch.h"
#include "KGCModeTutorial.h"

//
#include "Replay.h"
#include "GCStateMachine.h"


#include "utf16_cvt.h" // UnitCode ÅØ½ºÆ® ÆÄÀÏÀ» ¸¸µé±â À§ÇØ.

#include "GCDropItemManager.h"
#include "GCUI/GCServerScene.h"
#include "KGCRoomManager.h"
#include "KGC3DObjectMgr.h"
#include "KGCMonCrusader.h"
#include "KGCMonsterHunt.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCChannelManager.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCMiniGame.h"
#include "GCRenderManager.h"
#include "GCSKT.h"
#include "KSingleton.h"

#include "KncP2PLib/KncP2P.h"
#include "KGCAngelsEgg.h"
#include "KGCGuildMatch_CapTain.h"
#include "gcui/GCItemReceiveBox.h"
//#include "GCUI/GCUIScene.h"
#include "gcui/GCMyInfoScene.h"
#include "gcui/GCAgitOverlayUI.h"
#include "KGCAgitGameManager.h"
#include "gcui/KGCSurveyDlg.h"
#include "GCClientErrorManager.h"

#include "KGCDota.h"
#include "gcui/KGCPrograssBar.h"
#include "KGCEmbarkManager.h"
#include "./MapSystem/GCWallManager.h"
#include "KGCSubjectManager.h"
#include "KGCNetworkStatManager.h"
#include "GCUI/GCTutorialDlg.h"
#include "KGCQALogManager.h"

KGCStateGame* g_pStateGame = NULL;

KGCStateGame::KGCStateGame(void)
: m_uiBadPingCnt(0)
{
    g_pStateGame = this;
    for(int i=0 ; i<(int)GC_GMC_COUNT ; ++i)
        m_ppGameMode[i] = nullptr;

    m_ppGameMode[GC_GMC_MATCH] = new KGCMatch;
    m_ppGameMode[GC_GMC_GUILD_BATTLE] = new KGCGuildWar;
    m_ppGameMode[GC_GMC_DUNGEON] = new KGCQuest;
    m_ppGameMode[GC_GMC_INDIGO] = new KGCIndigo;
    m_ppGameMode[GC_GMC_TUTORIAL] = new KGCModeTutorial;
    m_ppGameMode[GC_GMC_TAG_MATCH] = new KGCTagMatch;
    m_ppGameMode[GC_GMC_MONSTER_CRUSADER] = new KGCMonCrusader;
    m_ppGameMode[GC_GMC_MONSTER_HUNT] = new KGCMonsterHunt;
    m_ppGameMode[GC_GMC_DEATHMATCH] = new KGCDeathMatch;
	m_ppGameMode[GC_GMC_MINIGAME] = new KGCMiniGame;
	m_ppGameMode[GC_GMC_ANGELS_EGG] = new KGCAngelsEgg;
	m_ppGameMode[GC_GMC_CAPTAIN] = new KGCGuildMatch_CapTain;
    m_ppGameMode[GC_GMC_DOTA] = new KGCDota;
    m_ppGameMode[GC_GMC_AGIT] = new KGCAgit;
	m_ppGameMode[GC_GMC_AUTOMATCH] = new KGCMatch;
    m_ppGameMode[GC_GMC_FATAL_DEATHMATCH] = new KGCDeathMatch;

	m_vecStatPingInfo.clear();
}

KGCStateGame::~KGCStateGame(void)
{
	for(int i=0 ; i<(int)GC_GMC_COUNT ; ++i)
		SAFE_DELETE( m_ppGameMode[i] );

    g_pStateGame = nullptr;
}

bool KGCStateGame::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	g_MyD3D->m_pStateMachine->Clear_SavedState();
	return true;
}

bool KGCStateGame::OnInit()
{
	g_kMonsterManager.CheckAllMonster();

    m_bOnExit = false;
	g_pkUIScene->m_pkGameOverlayUI->EnterScene();


    g_MyD3D->GetMyPlayer()->m_kUserInfo.nUserState = GC_RUS_PLAYING;
    g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_PLAYING;

    // ¹ÞÀº ¾ÆÀÌÅÛ ¸®½ºÆ® »Ñ·ÁÁÖ´Â ¾ÆÀÌ ÃÊ±âÈ­ ÇÏ°í ½ÃÀÛ 
    g_pkUIScene->m_pkItemReceiveBox->ClearItemList();

	//°ÔÀÓ»óÅÂ·Î µé¾î°¡±âÀü¿¡ ¸ðµåº°·Î ¼öÇàÇØ¾ßÇÒ ÇÔ¼ö
	GetCurGameModePtr()->InitAtGame();

	//Æ÷Ä¿½º¸¦ ¸ÞÀÎÀ©µµ¿ì·Î ³Ñ±ä´Ù
	//¤»¤»¤» ¹ß»ý¹®Á¦¿Í ¸Þ½ÃÁö¹Ú½º »ý¼º½Ã À©µµ¿ì°¡ ÃÖ¼ÒÈ­µÇ´Â ¹®Á¦¸¦ ¾ø¾ÖÁØ´Ù.
	//g_pkUIScene->m_pkGameOverlayUI->SetEditFocus();
	SetFocus( g_hUIMainWnd );

	m_eCurrGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();

	for( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
        if ( g_MyD3D->m_KGCOption.GetBasicEquipEnable() )
            if ( !g_MyD3D->MyPlayer[i]->IsLocalPlayer() )
                g_MyD3D->MyPlayer[i]->UpdatePlayerResource();

		if(g_MyD3D->MyPlayer[i]->IsUseItemEffect(ITM_EFF_AP_DP_HP_15_PER))
		{
			if(g_MyD3D->IsSameTeam(i,g_MyD3D->Get_MyPlayer()))
			{
#if defined ( LEVEL_DESIGN_STAT_CALC ) 
				g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_ATTACK_PERCENT] += 0.15f;
				g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_DEFENCE_PERCENT] += 0.15f;
#else
                g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_ATK] *= 1.15f;
                g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_DEF] *= 1.15f;
#endif 
				g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_HP] *= 1.15f;
				g_MyD3D->GetMyPlayer()->SetHPFull();

			}
		}	
	}

	for (int i = 0; i < MAX_PLAYER_NUM ; i++ )
	{
		if ( !g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL) )
		{
			continue;
		}
		if ( g_MyD3D->IsMyTeam(i) )
		{
			if( g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL) )
				g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL)->SetOutlineColor( EDGE_COLOR_FRIEND );
		}
		else
		{
			if( g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL) )
				g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL)->SetOutlineColor( EDGE_COLOR_ENEMY );
		}
		if( g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL) )
			g_MyD3D->MyPlayer[i]->GetMetaForm(FORM_NORMAL)->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE );

		if ( g_MyD3D->m_akPet[i]->m_pObject )
		{
			//g_MyD3D->m_akPet[i]->m_pObject->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE / 2 );        
		}
	}

    for (int i = 0; i < MAX_PLAYER_NUM ; i++ )
    {
        for(int j=0;j<FORM_NUM;++j)
        {
            if ( !g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j] )
            {
                continue;
            }
            if ( g_MyD3D->IsMyTeam(i) )
            {
                if( g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j] )
                    g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j]->SetOutlineColor( EDGE_COLOR_FRIEND );
            }
            else
            {
                if( g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j] )
                    g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j]->SetOutlineColor( EDGE_COLOR_ENEMY );
            }
            if( g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j] )
                g_MyD3D->MyPlayer[i]->m_pObjMetaForm[j]->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE );
        }
    }

    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() &&
        SiKGCWorldMapManager()->IsSkillListGuideDungeon( SiKGCRoomManager()->GetGameMode() ) )
    {
        g_pkTutorialDlg->InitSkillList();
        g_pkTutorialDlg->Enable();
    }

	g_RenderManager->SetOutlineWidth(OUT_LINE_WIDTH_IN_GAME);

    //SiGCPairPlaySystem()->SaveDummyReplayHeadData();

    //if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        StartGameTime = 0;

    g_kGlobalValue.m_bAIEnable = true;
    g_kGlobalValue.m_bRenderMonster = true;

    memset( g_kEnterGame, 0, sizeof(KGC_PID_ENTER_GAME) );
    g_kEnterGame->Type = GC_PID_ENTER_GAME;
    g_kEnterGame->ucPlayerIndex = g_MyD3D->Get_MyPlayer();
    g_pGameBoard->OnEnterPlayer( g_MyD3D->Get_MyPlayer() );

    m_uiBadPingCnt = 0;

    SiKGCNetworkStatManager()->ClearStatModeDataAll();

	return true;
}

void KGCStateGame::OnDestroy()
{

	//³¡³¯¶§ µå¶ø¾ÆÀÌÅÛµµ ÃÊ±âÈ­ ÇØÁÖÀÚ 
	// ±Ç»ó±¸ Á×ÀÌ·Î °¡°Ú´Ù 
	SiKGCDropItemManager()->Clear();
    g_kGlobalValue.m_bMUHANLife = false;
	g_pkGameOverlayUI->DeleteBossList();
    g_pkGameOverlayUI->SetShowPlayTime( true );
    if( m_eCurrGameModeCategory == GC_GMC_TUTORIAL )
        g_kGlobalValue.m_bNewUser_Tutorial = true;
	
    for (int i = 0; i < MAX_PLAYER_NUM ; i++ )
    {
        g_MyD3D->MyPlayer[i]->SetEPlayerState( PS_ALIVE );
        g_MyD3D->MyPlayer[i]->SetMeshToggleState( false );
        g_MyD3D->MyPlayer[i]->SetEmbarkTypeOfInPortal( INT_MAX );
        g_MyD3D->MyPlayer[i]->SetEmbarkUIDOfInPortal( INT_MAX );
        if ( g_MyD3D->MyPlayer[i]->m_pObject )
        {
            g_MyD3D->MyPlayer[i]->m_pObject->ResetMeshState();
            g_MyD3D->MyPlayer[i]->m_pObject->SetCartoon( CARTOON_DEFAULT );
            g_MyD3D->MyPlayer[i]->m_pObject->SetRender( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive );
            g_MyD3D->MyPlayer[i]->m_pObject->SetOutlineColor( EDGE_COLOR_FRIEND );
        }
    }

	g_MyD3D->Clear2DObject();

    // ¹ÞÀº ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ ¶ç¿öÁØ´Ù.
    if( !g_pkUIScene->m_pkItemReceiveBox->GetItemVector().empty() ) {
        //°¡ÀÌµå ÁøÇàÁßÀÎÁö Ã¼Å©
        if ( g_pkUIScene->m_bFirstTutorialPlaying )
            g_pkUIScene->EndMessageBox();
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_RECEIVE);
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        g_pkUIScene->m_pkMyInfoScene->UpdateData();
        g_pkUIScene->m_bFirstTutorialPlaying = false;
    }

	g_kMonsterManager.DeleteAllMonster();
    SiKGCEmbarkManager()->DestoryAllInstance( true );
    SiKGCWallManager()->DestoryAllInstance();
    SiKGCSubjectManager()->DestoryAllInstance();

#if !defined( __PATH__ )
    g_kGlobalValue.SetDamageRaito( 1.0f );
#endif
}

void KGCStateGame::Process_Admin_Command( AdminCommandType _ACT )
{
	switch( _ACT )
	{
	case KGCState::PRINT_CURRENT_STATE_USER:
		{
            std::wstringstream stmFileName;

            int SlotCount = 0;
            for(int i = 0; i < MAX_PLAYER_NUM; i++)
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
                    SlotCount++;
            }

            stmFileName << (LPCTSTR)CTime::GetCurrentTime().Format( _T("%Y-%m-%d %H_%M_%S") )
                << L"_[" << (SiKGCRoomManager()->GetRoomInfo().bPublicRoom ? (L"Public") : (L"Locked")) << L"]"
                << L"(" << SlotCount << L"_Users)_Game.txt";

            std::wstringstream stm;
            stm << L"Room Tile : " << (SiKGCRoomManager()->GetRoomInfo().strRoomName.empty()?(L"No Name"):(SiKGCRoomManager()->GetRoomInfo().strRoomName)) << L"\r\n";
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                // ¾Èºñ¾î ÀÖÀ¸¸é~
                if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false || ( g_MyD3D->MyPlayer[i]->GetPlayerNameSize() == 0 ) )
                    continue;

                stm << L"NickName : [" << g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName << L"] Login ID : [" << g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin << L"]" << L"\r\n";
            }
            //ÄÜ¼ÖÃ¢¿¡ »Ñ¸®±â À§ÇØ¼­.
            wprintf( L"%s \r\n", stmFileName.str().c_str() );
            wprintf( stm.str().c_str() );

            //ÆÄÀÏ »ý¼º.
            std::wofstream wFile;
            IMBUE_NULL_CODECVT( wFile );
            wFile.open( KncUtil::toNarrowString(stmFileName.str()).c_str(), std::ios::binary | std::ios::out );
            if( !wFile.is_open() )
                return;

            wchar_t cBom = 0xfeff;
            wFile.write( &cBom, 1 );

            wFile << stm.str(); // ³»¿ë ÆÄÀÏ¿¡ ¾²±â

            wFile.close();
		}
		break;
	}
}

bool KGCStateGame::FrameMove(float fElapsedTime)
{	
	if (m_bOnExit==true)
		return true;

	if( g_pkInput->IsDown( DIK_SYSRQ ) )
	{
		if( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
			Process_Admin_Command( KGCState::PRINT_CURRENT_STATE_USER );
	}

	if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
    {
        g_MyD3D->MyReplay->Load_FrameData();
        if( g_MyD3D->MyReplay->m_bEndOfFile == true )
        {
            return true;
        }
    }
	//PROFILE_END()

    KGCPC_BEGIN("DropItem");
    SiKGCDropItemManager()->FrameMove();
    KGCPC_END();
    KGCPC_BEGIN("GameMode");
    m_ppGameMode[m_eCurrGameModeCategory]->FrameMove(fElapsedTime);
    KGCPC_END();

#if defined( USE_ONGAME_INVENTORY )
    if( g_MyD3D->MyCtrl->k_Game_Inventory && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && false == g_pkGameOverlayUI->IsShowDialogue() &&
        !g_pkGameOverlayUI->HasEditFocus() && !g_pkUIScene->IsMsgBoxModal() )
    {
        //KP2P::GetInstance()->Send_InventoryInfoReq( "Server" );
        g_MyD3D->SetEnableKeyInput( false );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GAME_INVENTORY );
    }
    else if( false == g_MyD3D->MyCtrl->k_Game_Inventory && KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_GAME_INVENTORY ) ) { 
        g_pkUIScene->CloseGameInventory();
    }
#endif

    //PROFILE_BEGIN("Replay")
    //KGCPC_BEGIN("ReplayDump");
    SiGCPairPlaySystem()->SaveDummyReplayData();
    //KGCPC_END();

    CheckTCPRelayServer();
    SiKGCNetworkStatManager()->FrameMove();

	return true;
}

bool KGCStateGame::Render()
{
    if ( BeginScene() )
    {
        m_ppGameMode[m_eCurrGameModeCategory]->Render();        
        
        EndScene();
    }
	return true;
}

KGCGameModeInterface* KGCStateGame::GetCurGameModePtr()
{
    return m_ppGameMode[SiKGCRoomManager()->GetGameModeCategory()];
}

void KGCStateGame::ClearStageResource(bool bLoad)
{
    //게임 끝날때랑 스테이지 바뀔때 수시로 클리어 할것들 넣어주세요
    g_MyD3D->m_pMapAnim->ClearInstList();

    g_MyD3D->m_NPC.ClearNPC_Action();
    //g_MyD3D->m_AITemplet.ClearAIObject();	
    g_MyD3D->MyHead->Clear();
    g_MyD3D->MyItms->Clear_All();
    g_MyD3D->m_pDamageManager->RemoveAll();
    g_MyD3D->MySparks->RemoveAll();
    g_MyD3D->MyStg->Release();

    g_pkTutorialDlg->Disable();

    // 버프 클리어 할 때, 이번은 스테이지 넘어가는 중이라고 알려준다!!
    // bLoad가 true이면 게임시작이나 스테이지 이동일 때이다.
    g_pMagicEffect->ClearMagicEffect(false, bLoad);

    {
        KEVENT_STAT_END_GAME_INFO kPacket;
        kPacket.m_vecDungeonStat.clear();

        // 몬스터 정보를 초기화한다.
        for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;
            if (pMonster == NULL) continue;
            if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) && bLoad)	continue;

            KBossDungeonStat tmp;
            // 초기화 하기 전에.. 통계 남길 것들 남겨보쟈~
            pMonster->SendBossObjectStat(tmp);
            pMonster->Release();

            if (tmp.m_dwClearType != -1)
            {
                kPacket.m_vecDungeonStat.push_back(tmp);
            }

        }

        if (g_kGlobalValue.m_kUserInfo.bHost)
        {
            if (kPacket.m_vecDungeonStat.size() > 0)
                KP2P::GetInstance()->Send_BossDungeonStat(kPacket);
        }
    }

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        g_MyD3D->MyPlayer[i]->EffectClear();
        g_MyD3D->MyPlayer[i]->ClearMotionParticleList();
        g_MyD3D->MyPlayer[i]->SetEmbarkArena();
        SiGCSKT()->ReleaseResource(g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID);
    }

    for (int i = 0; i < MAX_OBJECT; i++)
    {
        if (g_MyD3D->m_pObject[i])
        {
            g_MyD3D->m_pObject[i]->Destroy();
            SAFE_DELETE(g_MyD3D->m_pObject[i]);
        }
    }

    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        g_MyD3D->m_akPet[i]->ClearParticleEffect();
    }

    g_ParticleManager->DeleteAllInstance();
    //g_pMeshEffectManager->ClearInstance();

    g_AnimManager.StartGame();
    g_pkPetAI->ClearAIObject();

    g_MyD3D->ClearMonster();

    SiKGC3DObjectMgr()->ReleaseStage();
    g_MyD3D->m_fIncMPShield = 0.001f;

    SiKGCEmbarkManager()->CheckStageMoveEmbarkation();
    SiKGCEmbarkManager()->DestoryAllInstance(true);
    SiKGCWallManager()->DestoryAllInstance();

    SiKGCInGamePrograssBar()->ClearALLData();

    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        if (FORM_NORMAL != g_MyD3D->MyPlayer[i]->MetamorphosisForm && true == g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
        {
            if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory())
            {
                g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm(g_pkQuestManager->m_bEndGame);
            }
            else
            {
                g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm();
            }
        }
    }
}

void KGCStateGame::ClearGameResource()
{
    //게임 종료시 클리어 할것들 넣어주세요
    ClearStageResource();
    SiKGC3DObjectMgr()->ReleaseGame();
    g_pMagicEffect->ClearMagicEffect(false, false);

    // 2007/3/30. iridology. 자꾸 뒤저싸서 안되겠다
    if (m_eCurrGameModeCategory > 0 && m_eCurrGameModeCategory < GC_GMC_COUNT) {
        if (m_ppGameMode[m_eCurrGameModeCategory])
            m_ppGameMode[m_eCurrGameModeCategory]->m_kSanta.Delete();
    }
    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        g_MyD3D->MyPlayer[i]->EndGame();
    }

    g_pGameBoard->Init();
    g_MyD3D->m_pPetAnim->ClearInstList();

    // 게임 텍스쳐 해제
    g_MyD3D->MySparks->UnLoad();
    g_MyD3D->m_pDamageManager->UnLoad();
    g_pGraphicsHelper->UnLoadPlayerActionMotion();
    g_pGraphicsHelper->UnLoadPlayerModel();

    g_pkGameOverlayUI->Clear();
    g_pkGameOverlayUI->SetWndState(KD3DWnd::D3DWS_DISABLED);
    g_pkOnGameDlg->OnInitialize();

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        g_MyD3D->m_akPet[i]->EndPet();
    }

    // 단축슬롯 수량을 조절함둥
    g_MyD3D->GetMyShortCutSlot()->SlotRefill();
    g_MyD3D->GetMyShortCutSlot()->SendQuickSlotinfo();

    // 게임내 사운드를 삭제합니다.
    g_KDSound.FreeGameSound();

    g_kGlobalValue.m_kChattingEvent.SetSuccessEvent(false);
    g_kGlobalValue.m_bRisingSunOrMoonEvent = false;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER)
        SiKGCFantasticMap()->ClearWater();

    SiKGCObjectEgoMgr()->ClearObjectStateInfo();
}

void KGCStateGame::RemoveEmptyItem()		//´Ù¾´ ¼ö·®¼º ¾ÆÀÌÅÛÀ» Á¦°ÅÇÏ°í ¹Ù²¼´Ù°í ÀüÃ¼¿¡°Ô ¾Ë¸²
{
    // [2/5/2008] breadceo. ÀÇ¹Ì ¾ø´Ù
    return;

    //if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL || g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
    //    return;

    //SUserInfo& kUserInfo = g_kGlobalValue.m_kUserInfo;
    //PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    //if( pPlayer != NULL )
    //    kUserInfo = pPlayer->m_kUserInfo;
    ////====================================================================================
    //// 2006.07.11 : Asirion
    //// ÀåÂøµÈ ¾ÆÀÌÅÛ Áß ¼ö·®ÀÌ ¼ÒÁøµÈ ¾ÆÀÌÅÛÀº Á¦°ÅÇÑ´Ù, ´Ü °üÀüÀÚ¸ðµå ÀÏ¶§´Â Á¦°Å ÇÏÁö ¾Ê´Â´Ù.
    //if( g_MyD3D->IsLocalPlayerObserver() == false )
    //{
    //    //´Ù¾´ ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸® ¹× ÀåÂø Á¤º¸¿¡¼­ Áö¿ö¹ö¸²
    //    std::vector<KSimpleItem> vecDeletedItem;
    //    if( g_pItemMgr->RemoveEmptyItem( &kUserInfo, &vecDeletedItem ) )
    //    {
    //        if( pPlayer != NULL ){
    //            pPlayer->UpdatePlayerResource();
    //            pPlayer->UpdateGlobalUserInfo();
    //        }
    //        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
    //        {
    //            KEquipUser kEquip;
    //            kEquip.m_strLogin = kUserInfo.strLogin;
    //            kEquip.m_cCharType = (char)kUserInfo.GetCurrentChar().iCharType;
    //            //¼­¹ö·Î º¸³¾ µ¥ÀÌÅÍ´Ï±î UID¸¦ ¼­¹ö¿ëÀ¸·Î ¹Ù²ÙÀÚ.
    //            for ( int i = 0 ; i < (int)vecDeletedItem.size() ; i++ )
    //            {
    //                vecDeletedItem[i].m_dwUID =  vecDeletedItem[i].m_dwUID ;
    //            }
    //            KP2P::GetInstance()->Send_EquipItemReq( kEquip );
    //        }
    //    }
    //}
}

void KGCStateGame::CountStatPing()
{
	KStatPingInfo kPingInfo;
	bool bSinglePlay = true;

#if defined(USE_P2P_OR_TCP_RELAY)
    kPingInfo.m_nPingTime = 0;
    kPingInfo.m_wstrSendCCode = g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.strNation;

    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if((g_kGlobalValue.m_kUserInfo.dwUID != g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID) && 
            g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {                                
            //p2p Àü¼ÛÀ» ÅÃÇÑ »óÈ²¿¡¼­ À¯Àú°£ °¡Àå ÇÎÀÌ ¾ÈÁÁÀº À¯ÀúÀÇ ÇÎÀ» ¼±ÅÃ
            int iPing = (int)KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );                
            if( iPing >= kPingInfo.m_nPingTime )
            {
                kPingInfo.m_nPingTime = iPing;
                kPingInfo.m_wstrRecvCCode = g_MyD3D->MyPlayer[i]->m_kUserInfo.strNation;
            }   

            bSinglePlay = false;
        }
    }

    if( !bSinglePlay )
    {
        if( g_eP2PConMode == P2P_TCP_RELAY_ONLY )
        {
            kPingInfo.m_nType = 1;
            kPingInfo.m_wstrRecvCCode = g_kGlobalValue.m_kUserInfo.strNation;
            kPingInfo.m_nPingTime = (int)g_kGlobalValue.GetTRAvgPing();

            m_vecStatPingInfo.push_back(kPingInfo);

            KP2P::GetInstance()->Send_StatPingInfo(m_vecStatPingInfo);
            m_vecStatPingInfo.clear();
        }
        else
        {
            // ÇÎÀÇ Åë°èº¸´Ù´Â P2P ¹× TR Á¢¼ÓÀÇ Åë°è»óÅÂ¸¦ ¾Ë±âÀ§ÇÔÀÌ±â ¶§¹®¿¡ °¢ »óÈ²¿¡¼­ ÃÖ¼ÒÇÑÀÇ Á¤º¸¸¸À» ³²±âµµ·Ï ÇÔ.
            kPingInfo.m_nType = 0;
    
            m_vecStatPingInfo.push_back(kPingInfo);
            KP2P::GetInstance()->Send_StatPingInfo(m_vecStatPingInfo);
            m_vecStatPingInfo.clear();
        }
    }    
#elif defined(USE_ONLY_TCP_RELAY)
    kPingInfo.m_nType = 1;
    kPingInfo.m_wstrRecvCCode = g_kGlobalValue.m_kUserInfo.strNation;
    kPingInfo.m_nPingTime = (int)g_kGlobalValue.GetTRAvgPing();

    m_vecStatPingInfo.push_back(kPingInfo);

    KP2P::GetInstance()->Send_StatPingInfo(m_vecStatPingInfo);
    m_vecStatPingInfo.clear();
#else    
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if((g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.dwUID != g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID) && 
            g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            kPingInfo.m_nType = 0;
            kPingInfo.m_wstrRecvCCode = g_MyD3D->MyPlayer[i]->m_kUserInfo.strNation;
            kPingInfo.m_nPingTime = (int)KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );

            m_vecStatPingInfo.push_back(kPingInfo);

            bSinglePlay = false;
        }
    }

    if( !bSinglePlay)
    {
        KP2P::GetInstance()->Send_StatPingInfo(m_vecStatPingInfo);
        m_vecStatPingInfo.clear();
    }
#endif

#if defined(USE_ONLY_TCP_RELAY)

    kPingInfo.m_nType = 1;
    kPingInfo.m_wstrRecvCCode = g_kGlobalValue.m_kUserInfo.strNation;
    kPingInfo.m_nPingTime = (int)g_kGlobalValue.GetTRAvgPing();

    m_vecStatPingInfo.push_back(kPingInfo);

    KP2P::GetInstance()->Send_StatPingInfo(m_vecStatPingInfo);
    m_vecStatPingInfo.clear();
#endif


}

void KGCStateGame::SendEndGamePacket()
{
	if(!g_kGlobalValue.m_kUserInfo.bHost)
		return;

	// ´ëÈ¸¿ë ¼­¹öÀÎ °æ¿ì °á°ú¸¦ ³¯¸°´Ù. // ³¡
	KEndGameReq kNot;
	MakeAllGameResult( kNot );

    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
        {
            kNot.m_bIsBlueWin          = IsBlueWin;
            kNot.m_vecIsPlayerWin.push_back(g_pGameBoard->m_pStartingMember[i].IsWin);
            kNot.m_uiMVP               = g_MyD3D->MVP;
            kNot.m_bIsEscortMonsterAlive = false;

            // ¸ó½ºÅÍ Áß¿¡ È£À§ ¸ó½ºÅÍ¸¦ Ã£¾Æ¼­ »ì¾Ò³ª Á×¾ú³ª º¸ÀÚ
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				MONSTER* pMonster = mit->second;
				if( pMonster && pMonster->GetMonsterType() == g_kGlobalValue.GetEscortMonsterID() ) {
					kNot.m_bIsEscortMonsterAlive = pMonster->IsLive();
					break;
				}

			}

            // È£À§ ÀÌº¥Æ® ÁßÀÎÁö 
            kNot.m_bIsActiveEscortEvent = g_kGlobalValue.IsEscortEventActive();
            kNot.m_dwEscortMonsterID = g_kGlobalValue.GetEscortMonsterID();

        }
        else
        {
            kNot.m_vecIsPlayerWin.push_back(false);
        }
    }
	
	// °æ±â °á°ú¸¦ º¸³½´Ù.		
	Result_EndGame = 0;
	KP2P::GetInstance()->Send_EndGameReq("Server", kNot);
	g_MyD3D->WaitForServerAck(Result_EndGame, 0, 5000, 2);
}

void KGCStateGame::EndGame()
{
    g_MyD3D->Pause( TRUE );
    //¸®ÇÃ·¹ÀÌ Àç»ý ¶§´Â ¸®ÇÃ·¹ÀÌ°¡ Á¾·áÇØ¾ßÁö ³»°¡ Á¾·á½ÃÅ°¸é ¾ÈµÈ´Ù.
    if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
        return;

    //³ìÈ­ Áß¿¡´Â (RP_RECORD)¿©±â¼­ ¸®ÇÃ·¹ÀÌ Á¾·áÇØ Áà¾ß.
    //°¢Á¾ º¯¼öµé ÃÊ±âÈ­
    g_MyD3D->MyReplay->m_eReplay_Speed = RS_NORMAL;
    g_MyD3D->MyReplay->SaveReplayEnd();

    g_pkUIScene->DestroyMsgBox();

    ClearGameResource();

    g_MyD3D->bSlotFrameRestart=true;
    for( int i=0; i<8 ;i++)
    {
		g_MyD3D->MyPlayer[i]->vPos.x = 0;
        g_MyD3D->MyPlayer[i]->y_Ex=0;
		g_MyD3D->MyPlayer[i]->cFrame =0;
        for ( int j = 0; j < GC_GAME_ITEM_NUM; ++j ) 
            g_MyD3D->MyPlayer[i]->Item_Remain_Time[j] = 0;
    }

    // ·£´ý¸ÊÀÌ¾ú´Ù¸é ¸ÊÀ» ´Ù½Ã °»½ÅÇÑ´Ù.
    if ( g_kGlobalValue.m_kUserInfo.bHost && SiKGCRoomManager()->GetRoomInfo().bRandomMapMode )
    {
        //g_MyD3D->RefreshRandomMap();
        SiKGCRoomManager()->RefreshRandomMap();
    }

    if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    {
        g_MyD3D->m_pkQuestGameMgr->OnEndGame();
    }

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) == true || GC_GMC_AUTOMATCH == SiKGCRoomManager()->GetGameModeCategory() ){
        g_MyD3D->m_pStateMachine->ExitRoom( true );
    }
    else{

        if ( SiKGCRoomManager()->CheckValidDungeon() )
        {
            g_MyD3D->m_pStateMachine->GoRoom();
        }
        else
        {
            g_MyD3D->m_pStateMachine->ExitRoom( true );
        }
        //g_MyD3D->m_pStateMachine->GoWorldMap();
        
        /*if( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) && 
            SiKGCChannelManager()->GetCurrentChannelType() != CT_GUILD_UNLIMITED &&
            SiKGCChannelManager()->GetCurrentChannelType() != g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade )
        {
            SiKGCChannelManager()->MoveChannelType(static_cast<int>(g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade));
        }*/
#if defined(USE_P2P_OR_TCP_RELAY)
        int iMaxPlayerNum = 0;
        if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
            iMaxPlayerNum = MAX_DUNGEON_PLAYER_NUM;
        else
            iMaxPlayerNum = MAX_PLAYER_NUM;

        for( int i = 0; i < iMaxPlayerNum; i++ )
        {
            if( g_kGlobalValue.m_kUserInfo.dwUID != g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID &&
                g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                AddConnectQ(g_MyD3D->MyPlayer[i]->m_kUserInfo, true);
            }
        }
#endif       

        SiKGCPrivateDungeonManager()->MoveStateRun( EM_EXIT_ROOM_TYPE );
    }
	g_KDSound.Stop("Dungeon_Result_Score");
    g_KDSound.Play( "32" );

    g_kGlobalValue.MakeErrorMotionToFile( "ErrorMotion.txt" );

    g_MyD3D->Pause( FALSE );

#if defined(PING_STATISTICS) && !defined(USE_P2P_OR_TCP_RELAY)
	//ÇÃ·¹ÀÌ¾î°£ ÇÎ °ªÀ» °è»ê ÈÄ ÆÐÅ¶À» º¸³½´Ù.
	CountStatPing();
#endif
}

void KGCStateGame::OnExitReplay()
{
	//°¢Á¾ º¯¼öµé ÃÊ±âÈ­
	g_MyD3D->MyReplay->m_eReplay_Speed = RS_NORMAL;
	SiKGCRoomManager()->InitObserver(); //¿ÉÀú¹ö ¸ðµå ÃÊ±âÈ­ ÇÏ±â.(Äù½ºÆ® 0 ½ÃÀÛ½Ã¿¡ ¹®Á¦°¡ µÇ±â¿¡.)
	//ÀÌ ¾Æ·¡ µÎ ÁÙ ¼ø¼­ ¹Ù²î¸é ¾È µË´Ï´Ù... ¸®ÇÃ·¹ÀÌ ¸®¼Ò½º¸¦ Á¤¸®ÇÏ°í ÆÄÀÏÀ» ´Ý¾Æ¾ß ÇÕ´Ï´Ù..
	g_MyD3D->MyReplay->SaveReplayEnd();
	// ÀÓ½ÃÆÄÀÏ »èÁ¦
	DeleteFile(L"Replay.tmp");

	g_pkUIScene->DestroyMsgBox();
	
	g_MyD3D->m_pStateMachine->GoWorldMap();

	//DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();

	//if( dwChannelUID != 0xFFFFFFFF && dwChannelUID != SiKGCChannelManager()->GetDungeonChannelUID() )
	//{
	//	// ÀÌ¹Ì Ã¤³Î¿¡ Á¢¼Ó ÁßÀÌ¶ó¸é!
	//	g_MyD3D->m_pStateMachine->GoWorldMap();
	//}
	//else
	//{
	//	g_MyD3D->m_pStateMachine->GoServer();
	//}
}

void KGCStateGame::OnExitTutorial()
{
    char cCharIndex = g_MyD3D->MyPlayer[0]->m_kUserInfo.cCharIndex;
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        g_MyD3D->MyPlayer[i]->ClearPlayerData();

        g_kEffectItem->Eff_Item[i].cWhat = -1;
        g_kEffectItem->Eff_Item[i].sTime = 0;

    }
	
	DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
	if( dwChannelUID != 0xFFFFFFFF && dwChannelUID != SiKGCChannelManager()->GetDungeonChannelUID() )
	{
		// ÀÌ¹Ì Ã¤³Î¿¡ Á¢¼Ó ÁßÀÌ¶ó¸é!
		g_MyD3D->m_pStateMachine->GoMatch();
	}
	else
	{
		g_MyD3D->m_pStateMachine->GoServer();
	}
}

void KGCStateGame::OnExitMinigame()
{
	g_MyD3D->MyPlayer[0]->ClearPlayerData();
	g_MyD3D->MyPlayer[0]->EndGame();
	//g_MyD3D->m_pStateMachine->GoMatch();
}

void KGCStateGame::OnExitGame()
{
    m_bOnExit = true;
    g_MyD3D->Pause( true );
    
	KGameResultIn kgr;
	if( IsDetermineWin == false && !g_MyD3D->IsLocalPlayerObserver() )
	{
		MakeGameResult( kgr, g_MyD3D->Get_MyPlayer(), true );
	}
	ClearGameResource();
	// µÎ º¯¼ö°¡ µ¿½Ã¿¡ ¸¸Á·ÇÏ´Â °æ¿ì´Â ¾ø´Ù.
	if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
	{	
		OnExitReplay();
        goto ret;
	}

	switch( m_eCurrGameModeCategory )
	{
		case GC_GMC_MINIGAME:
		{
			OnExitMinigame();
			goto ret;
		}
		case GC_GMC_TUTORIAL:
		{
			OnExitTutorial();
			goto ret;
		}
		case GC_GMC_DUNGEON:
		{
			g_MyD3D->m_pkQuestGameMgr->OnEndGame();
            break;
		}
        case GC_GMC_DOTA:
        case GC_GMC_AGIT:
        {
            KGCGameModeInterface* pMode = GetCurGameModePtr();
            if( pMode )
                pMode->GameEnd();
            break;
        }
    }

    // ½ÌÅ©¿ÀºêÁ§Æ® ÃÊ±âÈ­
    SiKGCSyncObjectManager()->InitCalss();

    // ³ìÈ­ ÁßÀÌ¶ó¸é ÁßÁö
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
	g_MyD3D->MyReplay->SaveReplayEnd();
	g_pkGameOverlayUI->SetWndState(KD3DWnd::D3DWS_DISABLED);
	//===========================================================================================//

	// °ÔÀÓ µµÁß¿¡ ³ª°¥¶§ Ã³¸®
	// ÀÌ¹Ì ¹æÀå¿¡ ÀÇÇØ Á¾·áÃ³¸®°¡ µÆ´Ù¸é Ã³¸®ÇÏÁö ¾Ê´Â´Ù.
#if defined ( USE_AGIT_SYSTEM )
    if( IsDetermineWin == false && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
#else
	if( IsDetermineWin == false )
#endif
	{
		Result_LeaveGame = 0;
#if defined(PING_STATISTICS) && !defined(USE_P2P_OR_TCP_RELAY)
		CountStatPing();
#endif
		KP2P::GetInstance()->Send_LeaveGameReq( kgr );
		//g_MyD3D->WaitForServerAck(Result_LeaveGame, 0, 1000 );
		g_MyD3D->WaitForServerAck(Result_LeaveGame, 0, 1 );
	}

#if defined ( USE_AGIT_SYSTEM )
    if( IsDetermineWin == false && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        Result_LeaveAgit = INT_MAX;
        KP2P::GetInstance()->Send_LeaveAgitReq();
        //g_MyD3D->WaitForServerAck( Result_LeaveAgit, INT_MAX, 5000, TIME_OUT_VALUE );
        g_MyD3D->WaitForServerAck( Result_LeaveAgit, INT_MAX, 1, TIME_OUT_VALUE );
        SiKGCAgitGameManager()->SetExitGame( true );
        g_pkAgitOverlayUI->ToggleRender( false );
    }
#endif

	// ³ª°¡±â Ã³¸®
	g_MyD3D->m_pStateMachine->ExitRoom( IsDetermineWin );
	
ret:
    g_MyD3D->Pause( false );
}

void KGCStateGame::OnStartGame()
{
}

//Stage¿¡ µé¾î°¥¶§ ºÒ¸®´Â ÇÔ¼öÀÔ´Ï´Ù.
void KGCStateGame::OnEnterStage()
{
	// ¹öÇÁ Å¬¸®¾î ÇÒ ¶§, ÀÌ¹øÀº ½ºÅ×ÀÌÁö ³Ñ¾î°¡´Â ÁßÀÌ¶ó°í ¾Ë·ÁÁØ´Ù!!
	g_pMagicEffect->ClearMagicEffect( false, true );

	g_MyD3D->Story = 0;
    g_MyD3D->Next2Stop = false;	
    g_MyD3D->MyCtrl->Energy = 0.0f;

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
                continue;

            g_MyD3D->MyPlayer[i]->EffectInit();
            g_MyD3D->MyPlayer[i]->LoadPet();
            SiGCSKT()->LoadingResource( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        }
    }
}

bool KGCStateGame::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateGame::OnSetupFSM()
{
    ON_SETUP_FSM( GS_ROOM,          GS_MATCH );
    ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
    ON_SETUP_FSM( GS_SQUARE_LOADING,GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
    m_eStateForFail = GS_SERVER;
}

KGCGameModeInterface* KGCStateGame::GetGameModePtr( EGCGameModeCategory eGameMode )
{
    return m_ppGameMode[eGameMode];    
}

void KGCStateGame::CheckTCPRelayServer()
{
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
        return;


    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && !SiKP2P()->m_spTRUserProxy->IsConnected() )
    {
        // ÃÖÃÊ ÇÑ¹ø¸¸ ¿¡·¯ Ä«¿îÆ® º¸³½´Ù.
        if ( SiKP2P()->m_spTRUserProxy->IsConnectCheck()  )
        {
            #if !defined( __PATH__ )
                MessageBoxA( NULL, "TRServer Connect Fail", "Error", MB_OK );
            #endif

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TCP_CONNECT_ERROR2 );    
        }

		if (SiKP2P()->m_spTRUserProxy->IsReConnectCheck())
		{
			SiKP2P()->DisConnectTCPRelay();

			std::string strServerIP = "";
			USHORT nServerPort = 0;

			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
			{
				in_addr in;
				in.S_un.S_addr = SiKGCAgitGameManager()->GetTRIpPort().first;
				strServerIP = inet_ntoa(in);
				nServerPort = SiKGCAgitGameManager()->GetTRIpPort().second;
			}
			else
			{
				SRoomInfo& sRoomInfo = SiKGCRoomManager()->GetRoomInfo();

				in_addr in;
				in.S_un.S_addr = sRoomInfo.dwTRelayServerIP;
				strServerIP = inet_ntoa(in);
				nServerPort = sRoomInfo.usTRelayServerPort;
			}

            if ( !SiKP2P()->m_spTRUserProxy->Connect( strServerIP.c_str(), nServerPort, g_kGlobalValue.ServerInfo.dwUserUID ) )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TCP_CONNECT_ERROR3 );        
            }
        }
    }
}

void KGCStateGame::DungeonRewardExp( std::map<DWORD,KCharExpReward>& mapRewardExp )
{
    std::map<DWORD,KCharExpReward>::iterator mit;
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
        mit = mapRewardExp.find( pPlayer->m_kUserInfo.dwUID );
        if ( mit != mapRewardExp.end() )
        {
            if ( pPlayer->IsLocalPlayer() )
            {
                // ÇØ´ç Ä³¸¯ÅÍ¿¡°Ô °æÇèÄ¡ ¤¡¤¡
                SCharInfo* pCharInfo = &pPlayer->m_kUserInfo.GetCurrentChar( mit->second.m_cCharType );
                pCharInfo->biExp = mit->second.m_biExp;
                pCharInfo->iLevel = static_cast<int>(mit->second.m_dwLevel);
                pCharInfo->kSkillInfo = mit->second.m_kSkillInfo;
            } else {
                SCharInfo* pCharInfo = &pPlayer->m_kUserInfo.GetCurrentChar( mit->second.m_cCharType );
                pCharInfo->iLevel = static_cast<int>(mit->second.m_dwLevel);
            }

            // ·¹º§¾÷ ÀÌÆåÆ®!
            if (static_cast<int>(static_cast<int>(mit->second.m_dwLevel) - static_cast<int>(mit->second.m_dwOldLevel)) > 0 )
            {
                pPlayer->AddDamage( DT_LEVELUP_EFF, 0 );
#if defined(NATION_KOREA)
                pPlayer->Calcualte_Point( false, true, true );
                pPlayer->ReloadBuff();
                g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
#endif				
                g_kGlobalValue.m_kUserMission.AddVirtualMissionToLevelUp();
                g_pkUIScene->m_pkGameOverlayUI->InitMinimap();
            }
        }
    }
}

void KGCStateGame::DungeonRewardItem( DWORD dwRecvUserUID, std::vector<KItem>& vecDropItem, std::vector<std::pair<DWORD,std::vector<KItem>>>& vecPartyDropItem, std::map< DWORD, int >& mapUserDice )
{
    // ÁÖ»çÀ§ ±¼¸®´Â ÆÄÆ¼Å¬ + ÁÖ»çÀ§ Ã¤ÆÃ ¸Þ½ÃÁö ¸¸µé±â 
    std::wstring strDiceString = L"#cff0000" + g_pkStrLoader->GetString( STR_ID_DICE_STRING0 );
    for(int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->IsLive() && g_MyD3D->MyPlayer[i]->m_kUserInfo.iInvenCapacity > g_MyD3D->MyPlayer[i]->m_kUserInfo.iUserItemCount )
        {
            if( false == mapUserDice.empty() )
            {
                // ÁÖ»çÀ§ ÆÄÆ¼Å¬ Ãâ·Â
                g_MyD3D->MyPlayer[i]->AddDiceParticle();

                DWORD dwDiceNumber= 1;
                std::map<DWORD, int>::iterator mitDice = mapUserDice.find( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );

                if( mapUserDice.end() != mitDice)
                    dwDiceNumber = mitDice->second;

                strDiceString += g_pkStrLoader->GetReplacedString(STR_ID_DICE_STRING1, "li", L"#cX#c" + g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickColor + g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName + L"#cX#cff0000", dwDiceNumber);
                g_KDSound.Play( "991022" );

#if defined( USE_QA_LOG_SYSTEM )
                SiKGCQALogManager()->DungeonDropDiceLog( g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName, dwDiceNumber );
#endif
            }
        }
    }

    if( false == mapUserDice.empty() )
    {
        strDiceString += L"#cX";
        g_pkChatManager->AddChatMsg( strDiceString );
    }

    // ¾ÆÀÌÅÛ ¸ÔÀº »ç¶÷ÀÌ ³ª¶ó¸é....
    if ( g_kGlobalValue.m_kUserInfo.dwUID == dwRecvUserUID )
    {
        int iReceiveCount;
        std::vector<KItem>::iterator vit = vecDropItem.begin();
        for ( ; vit != vecDropItem.end(); ++vit )
        {
            iReceiveCount = (*vit).m_nCount;
            KItem* pkInvenItem = g_pItemMgr->m_kInventory.FindItemByItemUID( (*vit).m_ItemUID );

            if( iReceiveCount != -1 && pkInvenItem != NULL )
            {
                iReceiveCount -= pkInvenItem->m_nCount;
            }
            GCITEMUID shortcutItemUID;
            int iShortcutItemDuration = 0;
            for (int slotIndex = 0; slotIndex < ITEM_SLOT_SIZE; ++slotIndex ){
                shortcutItemUID = g_MyD3D->GetMyShortCutSlot()->GetItemUID( slotIndex );

                if ( shortcutItemUID == (*vit).m_ItemUID ){
                    iShortcutItemDuration = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity( slotIndex );
                    break;
                }
                else
                    iShortcutItemDuration = 0;
            }
            iReceiveCount -= iShortcutItemDuration;
            g_pItemMgr->m_kInventory.AddItem( (*vit), true );
            g_pkUIScene->m_pkItemReceiveBox->AddItem( (*vit).m_ItemUID, iReceiveCount );

#if defined( USE_ONGAME_INVENTORY )
            if( g_pItemMgr->IsEquipmentItem( (*vit).m_ItemID / 10 ) )
            { 
                g_pItemMgr->AddNewItem( (*vit).m_ItemUID );
            }

            if( g_pItemMgr->IsHotItem( (*vit).m_ItemUID, g_MyD3D->GetPlayerByUID( dwRecvUserUID ) ) ) { 
                g_pkGameOverlayUI->ShowInvenKey( true );                        
            }
#endif
        }

    }

    // ÆÄÆ¼ ÀüÃ¼ È¹µæ ¾ÆÀÌÅÛ ÀÎº¥¿¡ ³Ö¾î ÁÖ±â 
    std::vector< std::pair< DWORD, std::vector<KItem> > >::iterator vitPartyRewardInfo;
    for ( vitPartyRewardInfo = vecPartyDropItem.begin(); vitPartyRewardInfo != vecPartyDropItem.end(); vitPartyRewardInfo++ ) {
        if ( g_MyD3D->GetPlayerByUID( vitPartyRewardInfo->first ) && g_MyD3D->GetPlayerByUID( vitPartyRewardInfo->first )->IsLocalPlayer() )
        {
            int iReceiveCount;
            std::vector<KItem>::iterator vit = vitPartyRewardInfo->second.begin();
            for ( ; vit != vitPartyRewardInfo->second.end(); ++vit )
            {
                iReceiveCount = (*vit).m_nCount;
                KItem* pkInvenItem = g_pItemMgr->m_kInventory.FindItemByItemUID( (*vit).m_ItemUID );

                if( iReceiveCount != -1 && pkInvenItem != NULL )
                {
                    iReceiveCount -= pkInvenItem->m_nCount;
                }
                GCITEMUID shortcutItemUID;
                int iShortcutItemDuration = 0;
                for (int slotIndex = 0; slotIndex < ITEM_SLOT_SIZE; ++slotIndex ){
                    shortcutItemUID = g_MyD3D->GetMyShortCutSlot()->GetItemUID( slotIndex );

                    if ( shortcutItemUID == (*vit).m_ItemUID ){
                        iShortcutItemDuration = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity( slotIndex );
                        break;
                    }
                    else
                        iShortcutItemDuration = 0;
                }
                iReceiveCount -= iShortcutItemDuration;
                g_pItemMgr->m_kInventory.AddItem( (*vit), true );
                g_pkUIScene->m_pkItemReceiveBox->AddItem( (*vit).m_ItemUID, iReceiveCount );

#if defined( USE_ONGAME_INVENTORY )
                if( g_pItemMgr->IsEquipmentItem( (*vit).m_ItemID / 10 ) )
                { 
                    g_pItemMgr->AddNewItem( (*vit).m_ItemUID );
                }

                if( g_pItemMgr->IsHotItem( (*vit).m_ItemUID, g_MyD3D->GetPlayerByUID( dwRecvUserUID ) ) ) { 
                    g_pkGameOverlayUI->ShowInvenKey( true );                        
                }
#endif
            }

        }
    }

    // ¾ÆÀÌÅÛ È¹µæ Ã¼ÆÃ ¸Þ½ÃÁö ¸¸µé±â 
	//FILE * f = fopen( "DropTrace.txt", "a");
    for( std::vector<KItem>::iterator vit = vecDropItem.begin() ; vit != vecDropItem.end() ; ++vit )
    {
		
        GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );

		if( pItem == NULL )
		{
			//printf("ItemID = %d Fudeu aqui oh\n", vit->m_ItemID);
			continue;
		}
		//fprintf( f, "vit->m_ItemID = %d, ItemID = %d - ItemName = %s\n",vit->m_ItemID, pItem->dwGoodsID, KncUtil::toNarrowString(pItem->strItemName).c_str() );
		

        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwRecvUserUID );
        if( pPlayer &&  pPlayer->IsLive() && pPlayer->m_kUserInfo.iInvenCapacity > pPlayer->m_kUserInfo.iUserItemCount)
        {
            std::wstring strDropMsgColor = L"#c43f4a7";
            std::wstring strNickName = pPlayer->m_kUserInfo.strNickName;

            if( dwRecvUserUID == g_kGlobalValue.m_kUserInfo.dwUID )
            {
                if (pItem->eItemKind == GIK_MONSTER_CARD)
                    strNickName = L"#cX#cffff00#cX#c" + pPlayer->m_kUserInfo.strNickColor + pPlayer->m_kUserInfo.strNickName + L"#cX#cff28da";
                else
                    strNickName = L"#cX#cffff00#c" + pPlayer->m_kUserInfo.strNickColor + pPlayer->m_kUserInfo.strNickName + L"#cX#c43f4a7";
            }


            g_KDSound.Play( "991024" );
            pPlayer->AddParticleNoDirectionPosWithTrace( "Dice_Item_Get_01", 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_BODY_CENTER ) );

            std::wstring strItemCharType = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );

#ifdef COMMON_MANUAL
            DWORD dwCommonCharType = ECT_ALL;
#else
            DWORD dwCommonCharType = 0;
#
            for (int i = 0; i < GC_CHAR_NUM; ++i) {
                dwCommonCharType = dwCommonCharType | (1 << i);
            }
#endif

            if(dwCommonCharType != pItem->dwCharType )  
                strItemCharType = GCFUNC::GetCharName( pItem->GetCharType() );

            std::wstring strDropMsg = strDropMsgColor + g_pkStrLoader->GetReplacedString( STR_ID_DROPITEM0, "lll" , strNickName, pItem->strItemName, strItemCharType ) + L"#cX";
            g_pkChatManager->AddChatMsg( strDropMsg );

#if defined( USE_QA_LOG_SYSTEM )
            SiKGCQALogManager()->DungeonDropGetLog( pPlayer->m_kUserInfo.strNickName, pItem, strItemCharType );
#endif
        }

    }
	//fclose( f );
	
    for ( vitPartyRewardInfo = vecPartyDropItem.begin(); vitPartyRewardInfo != vecPartyDropItem.end(); vitPartyRewardInfo++ ) {
        // ÆÄÆ¼ ÀüÃ¼ ¾ÆÀÌÅÛ È¹µæ Ã¼ÆÃ ¸Þ½ÃÁö ¸¸µé±â 
        for( std::vector<KItem>::iterator vit = vitPartyRewardInfo->second.begin() ; vit != vitPartyRewardInfo->second.end() ; ++vit )
        {
            GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
            PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( vitPartyRewardInfo->first );
            if( pPlayer &&  pPlayer->IsLive() && pPlayer->m_kUserInfo.iInvenCapacity > pPlayer->m_kUserInfo.iUserItemCount)
            {
                std::wstring strDropMsgColor = L"#c43f4a7";
                std::wstring strNickName = pPlayer->m_kUserInfo.strNickName;

                if( dwRecvUserUID == g_kGlobalValue.m_kUserInfo.dwUID )
                {
                    if (pItem->eItemKind == GIK_MONSTER_CARD)
                        strNickName = L"#cX#cffff00#cX#c" + pPlayer->m_kUserInfo.strNickColor + pPlayer->m_kUserInfo.strNickName + L"#cX#cff28da";
                    else
                        strNickName = L"#cX#cffff00#cX#c" + pPlayer->m_kUserInfo.strNickColor + pPlayer->m_kUserInfo.strNickName + L"#cX#c43f4a7";
                }


                g_KDSound.Play( "991024" );
                pPlayer->AddParticleNoDirectionPosWithTrace( "Dice_Item_Get_01", 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_BODY_CENTER ) );

                std::wstring strItemCharType = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );

#ifdef COMMON_MANUAL
                DWORD dwCommonCharType = ECT_ALL;
#else
                DWORD dwCommonCharType = 0;
#
                for (int i = 0; i < GC_CHAR_NUM; ++i) {
                    dwCommonCharType = dwCommonCharType | (1 << i);
                }
#endif

                if(dwCommonCharType != pItem->dwCharType )  
                    strItemCharType = GCFUNC::GetCharName( pItem->GetCharType() );

                std::wstring strDropMsg = strDropMsgColor + g_pkStrLoader->GetReplacedString( STR_ID_DROPITEM0, "lll" , strNickName, pItem->strItemName, strItemCharType ) + L"#cX";
                g_pkChatManager->AddChatMsg( strDropMsg );

#if defined( USE_QA_LOG_SYSTEM )
                SiKGCQALogManager()->DungeonDropGetLog( pPlayer->m_kUserInfo.strNickName, pItem, strItemCharType );
#endif
            }
        }
    }
}