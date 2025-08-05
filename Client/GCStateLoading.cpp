#include "stdafx.h"
//
//


#include "KMci.h"


#include "GCUI/GCGameOverlayUI.h"
#include "DamageManager.h"
#include "Spark.h"
#include "Message.h"

#include "GCStateMachine.h"
#include "GCStateLoading.h"
#include "Controls.h"

#include "QuestGameManager.h"
#include "Monster.h"
//

#include "Stage.h"


#include "GCStateGame.h"
#include "GCStateRoom.h"
#include "KGCGameBoard.h"
#include "GCCameraOrtho.h"
//
//
#include "KncP2PLib/KncP2P.h"
#include "KSingleton.h"
#include <KNC/ClientErr.h>

//
#include "GCUI/GCLoadingScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCToolTip.h"
//
#include "GCRenderManager.h"
#include "LatencyTransfer.h"
#include "KGCPlayerCommon.h"
//


#include "GCUI/GCServerScene.h"
#include "KGCRoomManager.h"
#include "MapSystem/KGCLayerManager.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCMinigameManager.h"
#include "buff/KGCBuffManager.h"
#include "GCUI/GCColorDlg.h"
#include "GCUI/GCInventoryFullMsg.h"
#include <dbg/dbg.hpp>
#include "GCClientErrorManager.h"

#define PIXELTOTEXEL(p) ( (p)/512.0f )
#define SCREENTOCLIENTWIDTH(p) ( ( (p)/400.0f) - 1.0f )
#define SCREENTOCLIENTHEIGHT(p) ( 0.75f - ( (p)/300.0f) * 0.75f )

extern bool RecheckFileSHA();

KGCStateLoading::KGCStateLoading(void)
{
	LoadFunc[0] = &KGCStateLoading::InitData;
    LoadFunc[1] = &KGCStateLoading::SparkLoading;
    LoadFunc[2] = &KGCStateLoading::DamageLoading;
    LoadFunc[3] = &KGCStateLoading::DamageLoading;
    LoadFunc[4] = &KGCStateLoading::DamageLoading;
    LoadFunc[5] = &KGCStateLoading::DamageLoading;

    LoadFunc[6] = &KGCStateLoading::StageLoading;
    LoadFunc[7] = &KGCStateLoading::PrevInitPlayer;
    LoadFunc[8] = &KGCStateLoading::InitPlayer;
    LoadFunc[9] = &KGCStateLoading::InitPlayer;
    LoadFunc[10] = &KGCStateLoading::InitPlayer;
    LoadFunc[11] = &KGCStateLoading::InitPlayer;
    LoadFunc[12] = &KGCStateLoading::InitPlayer;
    LoadFunc[13] = &KGCStateLoading::InitPlayer;
    LoadFunc[14] = &KGCStateLoading::AfterInitPlayer;
    LoadFunc[15] = &KGCStateLoading::InitGameMode;
    LoadFunc[16] = &KGCStateLoading::SoundLoading;

    m_pTexLoad = NULL;

    m_bSendP2PConnectionReport = false;
    m_iLoadCount = 0;
    m_iPlayerInitCount = 0;
    m_iDamageLoadCount = 0;
    m_iSoundLoadCount = 0;
    m_bSendLoadingTimeReport = false;
    m_mapUserConnectState.clear();
    m_iSendLoadingCompleteCnt = 0;
    ClearLoadList();

	m_bConnectComplete = false;
    m_bOverLoadingTime = false;
    m_pLoadingTimeFile = NULL;

    m_iStartingMember = 0;
}

KGCStateLoading::~KGCStateLoading(void)
{
    m_mapUserConnectState.clear();
    ClearLoadList();
}

bool KGCStateLoading::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	int iMyPlayer = g_MyD3D->Get_MyPlayer();

	for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
			continue;

		if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
		{
			g_MyD3D->MyPlayer[i]->SetSlotAlign();
		}

		if( i == iMyPlayer && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MINIGAME )
		{
			g_MyD3D->MyPlayer[i]->UpdateGlobalUserInfo();
		}
	}
	m_bConnectComplete = false;

#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
    {
	    if( !KSingleton<KncP2P>::GetInstance()->ConnectP2P() )
	    {
		    //P2P Á¢¼ÓÀ» ¸øÇßÀ½À» ¸Þ½ÃÁö ¹Ú½º·Î ¶ç¿öÁà¾ß ÇÒµí...
#ifndef _IRRUPTION_P2P_
		    ASSERT_MBOX( "You can not connect p2p!!" );
		    return false;
#endif
	    }
    }
#else
	if( !KSingleton<KncP2P>::GetInstance()->ConnectP2P() )
	{
		//P2P Á¢¼ÓÀ» ¸øÇßÀ½À» ¸Þ½ÃÁö ¹Ú½º·Î ¶ç¿öÁà¾ß ÇÒµí...
#ifndef _IRRUPTION_P2P_
		ASSERT_MBOX( "You can not connect p2p!!" );
		return false;
#endif
	}
#endif

    InitAtLoading();

    m_dwLoadingTime = ::timeGetTime();

    if( g_kGlobalValue.m_bUseLoadCheckInServer == false )
        m_kLoadingTimeStopWatch.SetStopWatch(180000); //½ºÅ×ÀÌÆ®·Îµù ´ë±â½Ã°£Àº 3ºÐ

    m_bLoadingComplete = false;

	return true;
}

bool KGCStateLoading::OnDestroyBeforeStateChange()
{
	return true;
}

bool KGCStateLoading::OnInit()
{
#if defined(DLL_INJECTION_CHECK)
    try
    {
		KDLLChecker::GetInstance()->CheckProcessList();
        KDLLChecker::GetInstance()->CheckDll();
    }
    catch(...)
    {

    }
#endif
    
	if (RecheckFileSHA()==false)
	{
		ExitWithHackAlert( L"" );
	}
    

	g_MyD3D->m_pStateMachine->Clear_SavedState();

	g_MyD3D->MyStg->SetStage( SiKGCRoomManager()->GetRoomInfo().ucStage , false );	

	
	// °ÔÀÓ ½ÃÀÛ È­¸é Ç¥½Ã¿ë Ä«¿îÆ®¸¦ ÃÊ±âÈ­ÇÑ´Ù.
	StartGameTime = 0;

	// °ÔÀÓ ½ÃÀÛ À½¾ÇÀ» ¿¬ÁÖÇÑ´Ù.
#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
        g_KMci.Play( GC_BGM_AGIT, true );
    else
#endif
	g_KMci.Play( GC_BGM_GAME_START, false );


    g_pkUIScene->m_pkGameOverlayUI->Create();

	g_pkUIScene->m_pkToolTip->ToggleRender( false );
	g_pkUIScene->EnableMessageBox( false );

	//====================================================================================
	// 2009.02.10 : DD
	// Comment : ´øÀü´ë±â¹æÀÌ³ª ´ëÀü´ë±â¹æ¿¡¼­ ¹æ¿øµéÀÌ Ã¤ÆÃÄÃ·¯ ¹Ú½º¸¦ ÄÑµÐ p»óÅÂ¿¡¼­ ¹æÀåÀÌ
	//			°ÔÀÓÀ» ½ÃÀÛÇÏ°Ô µÇ¸é Ã¤ÆÃÄÃ·¯¹Ú½º´Â »ç¶óÁöÁö ¾Ê°í °ÔÀÓ³» UIµéÀÌ Á¦´ë·Î ³ª¿ÀÁö
	//			¾Ê´Â ¹®Á¦°¡ ¹ß»ýÇØ¼­ Ã¤ÆÃÄÃ·¯¹Ú½ºµµ °­Á¦·Î ²ôµµ·Ï ¼³Á¤
	g_pkUIScene->m_pkColorDlg->CancelDlg();

	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
	{
		g_pkUIScene->m_pkGameOverlayUI->EnterScene();
		g_pkUIScene->m_pkGameOverlayUI->SetWaiting( false );
	}
	else
	{
		g_pkUIScene->m_pkLoadingScene->EnterScene();
		g_pkUIScene->m_pkLoadingScene->SetWaiting( false );
	}

	g_pkUIScene->m_pkLoadingScene->SetGameMode();

	//·Îµù»óÅÂ·Î µé¾î°¡±âÀü¿¡ ¸ðµåº°·Î ¼öÇàÇØ¾ßÇÒ ÇÔ¼ö
	((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetCurGameModePtr()->InitAtLoading();

	m_pTexLoad = g_pGCDeviceManager2->CreateTexture( "gameload.dds" );
    
	//// Æê ÃÊ±âÈ­
	for( int i = 0; i < MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; ++i )
	{
		if( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
		{
			g_MyD3D->m_akPet[i]->EndPet();
			continue;
		}
		KPetInfo kPet = g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar().kPetInfo;
		int iPetLevel = kPet.m_iLevel;
		int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
        int iPetEvolutionLevel = (int)kPet.m_cPromotion;

		// ÀåÂøÇÏ°í ÀÖ´Â ÆêÀÌ ÀÖ´Ù
		if( kPet.m_dwUID != 0 && iPetID != -1 )
		{
            g_MyD3D->m_akPet[i]->StartPet( i, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::GAME_LOADING, kPet.m_strName , true );
            g_MyD3D->m_akPet[i]->CreateParticleEffect();
            g_MyD3D->m_akPet[i]->InitBeforeGameStart();
			g_MyD3D->m_akPet[i]->SetPetMotion( EGCPETMOTION::GAME_LOADING );
			g_MyD3D->m_akPet[i]->cFrame = 0;
		}
		// ÀåÂøÇÏ°í ÀÖ´Â ÆêÀÌ ¾ø´Ù
		else
		{
			g_MyD3D->m_akPet[i]->EndPet();
		}
	}
    
    SiKGCRenderManager()->SetOutlineWidth( OUT_LINE_WIDTH_IN_GAME );
    for ( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        if ( g_MyD3D->MyPlayer[i]->m_pObject )
        {
            g_MyD3D->MyPlayer[i]->m_pObject->SetShellMatrixScale( 0.5f );
            if ( g_MyD3D->m_akPet[i]->m_pObject )
            {
                g_MyD3D->m_akPet[i]->m_pObject->SetShellMatrixScale( 0.26f );
            }
        }        
    }    

    KP2P::GetInstance()->Send_RoomMemberPingInfoReq();

	return true;
}

void KGCStateLoading::OnDestroy()
{

    g_pkUIScene->m_pkLoadingScene->ReleaseTempResource();
        
    g_pkUIScene->m_pkLoadingScene->ToggleRender( false );
    m_dwLoadingTime = ::timeGetTime() - m_dwLoadingTime;
    KP2P::GetInstance()->Send_Stat_FinishLodingTime( m_dwLoadingTime / 1000 );

    m_iSendLoadingCompleteCnt = 0;
}

bool KGCStateLoading::InitAtLoading()
{
    Result_EndGame = 0;
    m_bSendLoadingTimeReport = false;
    m_bSendP2PConnectionReport = false;
    m_iLoadCount = 0;
    m_iPlayerInitCount = 0;
    m_iDamageLoadCount = 0;
	m_iSoundLoadCount = 0;

    m_iSendLoadingCompleteCnt = 0;

    g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->ClearRegenCount();
#if defined( DUNGEONPLAYSTATE )
    g_kGlobalValue.m_dwStageStartTime = timeGetTime();
#endif

	// ÀÌÁ¦ °ÔÀÓÀ» ½ÃÀÛÇÏ¹Ç·Î, °ÔÀÓ¿¡¼­ ½Å°íÇÑÀûÀÌ ¾ø´Ù°í ¼³Á¤ÇÑ´Ù.
	g_kGlobalValue.m_bReportInGame = false;

    // ·Îµù Áß Æ¨±äÀ¯Àú Ä«¿îÆ®
#if defined( DUNGEONPLAYSTATE )
    int iCount = m_iStartingMember - static_cast<int>(GetGameStartingMembers().size());
    g_kGlobalValue.m_kDunPerforState.iLoadingBannedCnt = iCount;
    g_kGlobalValue.m_kDunPerforState.iGameStarttingMember = static_cast<int>(GetGameStartingMembers().size());
#endif
    m_dwTotalTime = 0;
    m_fWaitTime = 0.0f;
    return S_OK; 
}

//·ÎµùÈ­¸é ¹è°æ¿ÜÀÇ ·»´õ¸µ Ã³¸®
//·ÎµùÀÌ ³¡³ª¸é GoGame»óÅÂ·Î º¯°æ
//BeforeStartGameÇÔ¼ö¿¡¼­ ÇÏ´Â ÇàÀ§¸¦ ·ÎµùÈ­¸é¿¡¼­ ¼øÂ÷ÀûÀ¸·Î ½ÇÁ¦ ·ÎµùÀ» ÇØº¸ÀÚ
bool KGCStateLoading::FrameMove(float fElapsedTime)
{
    g_pkUIMgr->FrameMove( fElapsedTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );

    // °ÔÀÓ ·Îµù½Ã°£À» ÃøÁ¤ÇØ¼­ ÆÄÀÏ·Î ³²±âÀÚ!!
    DWORD dwStartTime, dwEndTime;
    bool bLoadingTimeRecord = false;

	// ·Îµù ½ÃÀÛ
	if( m_iLoadCount < LOADCOUNT )
    {
        dwStartTime = ::timeGetTime();
		(this->*LoadFunc[m_iLoadCount])();
        dwEndTime = (::timeGetTime()-dwStartTime);
        m_dwTotalTime += dwEndTime;

        // [1/16/2008] breadceo. ·ÎµùÇÏ´Ù °É¸° ½Ã°£Àº ±»ÀÌ ÇÁ·¹ÀÓ¹«ºê µ¹·ÁÁÙ ÇÊ¿ä ¾ø´Ù
        FRAME_TIMER_RESET;

        //Ã·¿¡´Â ·»´õ ÇÑ´Ù.
        g_MyD3D->m_bKeepFPS60 = (m_iLoadCount > 2); 

        if( g_kGlobalValue.m_bUseLoadCheckInServer == false )
        {
            // ·Îµù¿À¹öÅ¸ÀÓÀ» Ã¼Å©ÇÑ´Ù.
            if( CheckLoadingTime() )    
                return true;
        }        

        //·ÎµùÁøÇà´Ü°è ui¸¦ º¸¿©ÁÖ±âÀ§ÇÑ °ª ¼ÂÆÃ
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
            g_pkUIScene->m_pkGameOverlayUI->DungeonMyLoadingState(m_iLoadCount+1); 
            
        if( ( m_iLoadCount == 0 || (( m_iLoadCount % 4 ) == 0 ) ) && m_mapLoadComplete.size() > 1 )
            KP2P::GetInstance()->Send_RelayLoadingState(std::make_pair( g_kGlobalValue.m_kUserInfo.dwUID, m_iLoadCount+1));
    }
    ++m_iLoadCount;


	// ¾ÆÁ÷ ·ÎµùÀÌ ³¡³ªÁö ¾Ê¾ÒÀ½
	if( m_iLoadCount < LOADCOUNT )
		return false;

    m_fWaitTime += fElapsedTime;


    const int nP2P_CONECT_WAIT_TIME = 20 * 1000;    // 20ÃÊ

    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
    {
        //  ÀÏÁ¤ ½Ã°£¸¸ Ã¼Å©ÇÏ°í ±×·¡µµ ¾ÈµÇ¸é Áö³ª°£´Ù.
        if ( nP2P_CONECT_WAIT_TIME > ( m_fWaitTime * 1000 ) + m_dwTotalTime ) 
        {
            // P2P Á¢¼ÓÀÌ ¿Ï·áµÇÁö ¾ÊÀ¸¸é ·Îµù ¿Ï·á Ã³¸® ¾ÈÇÑ´Ù.
            std::vector<DWORD> vecStartingMembers = GetGameStartingMembers();
            for ( int i = 0; i < (int)vecStartingMembers.size(); i++ )
            {
                if( KSingleton<KncP2P>::GetInstance()->IsIn( vecStartingMembers[i] ) == false )
                {
                    return false;
                }
            }
        }
    }

#if defined(DUNGEONPLAYSTATE)
    g_kGlobalValue.m_kDunPerforState.dwLoadingTime = m_dwTotalTime;
#endif

    if( bLoadingTimeRecord && m_pLoadingTimeFile )
    {
        float fLoadingTime = (m_dwTotalTime/1000.0f);
        fprintf(m_pLoadingTimeFile, "Total\t%f\n\n", fLoadingTime);
        fclose( m_pLoadingTimeFile );
        m_pLoadingTimeFile = NULL;
    }

#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        // ¾ÆÁöÆ® ³»ºÎ ¿ÀºêÁ§Æ®µé ¹èÄ¡ 
        SiKGCAgitGameManager()->InitAgitObjectPos();

        // Ä³¸¯ÅÍ ¹èÄ¡ 
        SiKGCAgitGameManager()->InitAgitCharacter();
    }
#endif
    

	g_pkUIScene->m_pkLoadingScene->SetWaiting( true );
	g_pkUIScene->m_pkGameOverlayUI->SetWaiting( true );

    //Å×½ºÆ®ÄÚµå
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //int comboskillpenalty = 0.0;
    //LUA_GET_VALUE_DEF( "testtest", comboskillpenalty, 0 );

    //if( comboskillpenalty != 0 )
    //    return false;


    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MINIGAME ) // && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
    {
        if ( m_bSendP2PConnectionReport == false )
        {
            KncP2P* pkP2P = KSingleton<KncP2P>::GetInstance();
            SendLoadingComplete( pkP2P->GetUserCount( KncUserCount::ID_P2P ), pkP2P->GetUserCount( KncUserCount::ID_RELAY ) );
            m_bSendP2PConnectionReport =true;
        }

        if( m_bLoadingComplete == false ) 
            return false;

        KP2P::GetInstance()->Send_P2PConnectionCompleteNot();

#if defined(USE_PING_STATE_UI)        
        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        {            
            KP2P::GetInstance()->Send_RoomMemberPingInfoReq();
        }
#endif

#if defined( USE_AGIT_SYSTEM )
        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
#endif
        SendStatisticsInfo();

    }

    // ÀÌÁ¦ °ÔÀÓ ½ÃÀÛÇØ¶ó
#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        g_pkAgitOverlayUI->UpdateAgitInfo();
        g_pkAgitOverlayUI->UpdateMoneyInfo();
    }
#endif
#if defined(__PATH__)
    std::vector<HANDLE>::iterator vecIter;
    std::vector<HANDLE> vecTmp = g_pGCDeviceManager2->GetMassFileManager()->GetfpStreamList();

    for (vecIter = vecTmp.begin() ; vecIter != vecTmp.end() ; ++vecIter ){

        BOOL bCkeck_lock = FALSE;
        bCkeck_lock = LockFile(*vecIter , 0, 0, 0xffffffff, 0xffffffff);
        if (!bCkeck_lock)
            ExitWithHackAlert( L"" );
        else
            UnlockFile(*vecIter , 0, 0, 0xffffffff, 0xffffffff);

    }
#endif
    g_MyD3D->m_pStateMachine->GoGame();

    g_MyD3D->PlayGameBGM();

    InitAtLoading();

    return true;
}


bool KGCStateLoading::Render()
{
    if ( BeginScene() ) 
    {
        g_MyD3D->RenderUI();
        EndScene();
    }

    //if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
    //{
    //    SiKGCRenderManager()->Render( false );
    //}
    return true;	
}




void KGCStateLoading::ReloadScript()
{
	// ¼Ò½º/ ÇÑ±¹ »ç³» ¿¡¼­¸¸ ´Ù½Ã ·ÎµùÇÏµµ·Ï µÇ¾îÀÖ½À´Ï´Ù.
#ifdef __ORIGIN_DEV__ && ( defined(NATION_KOREA)  || defined(NATION_BRAZIL) || defined(NATION_CHINA) )
    KGCGlobal::LoadMonsterStatInfo();

    //°ÔÀÓ ·Îµù½Ã ¸ó½ºÅÍ½ºÅ©¸³Æ® ¸®·Îµå ÇÒ·Á¸é ÁÖ¼®À» Ç®¾î¿ë( ¸ó½ºÅÍ ¸ðµÎ ·ÎµåÇÏ±â¶§¹®¿¡ ·Îµù½Ã°£ÀÌ ±æ¾îÁü )
	//g_kMonsterManager.LoadScript();

	if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() ) 
		g_MyD3D->m_pkQuestGameMgr->ReloadQuestScript();
#endif
}

void KGCStateLoading::SparkLoading()
{
    // ½ºÆÄÅ© ¸®¼Ò½º ·Îµù
    g_MyD3D->MySparks->Load();
}

void KGCStateLoading::DamageLoading()
{
    // µ¥¹ÌÁö ¸®¼Ò½º ·Îµù
    if( m_iDamageLoadCount == 0 )
        g_MyD3D->m_pDamageManager->MakeSoundLoadList();
    g_MyD3D->m_pDamageManager->Load( m_iDamageLoadCount );

    // ¹Ùºü¼­ ÇÏµåÄÚµù -_-
    if( ++m_iDamageLoadCount == 4 )
        m_iDamageLoadCount = 0;
}

void KGCStateLoading::SoundLoading()
{
	// °ÔÀÓ³» »ç¿îµå ·Îµù
	
    g_KDSound.FullLoadfromLoadList();
}

void KGCStateLoading::InitData()
{
	ReloadScript();
	g_MyD3D->m_bLastFrameSend = false;
	g_pStateGame->OnStartGame();
	g_pStateRoom->OnLeaveState();

	g_pkGameOverlayUI->OnStartGame();

	g_kCamera.SetEarthQuake(0);
	g_kCamera.InitCamera();

	g_LatencyTransfer.ClearGamePacket();
	g_pGameBoard->Init();

	//IME ÃÊ±âÈ­ÇÑ´Ù.
	KGCIme::SetImeApi();

	g_MyD3D->Story = 0;
	g_MyD3D->Next2Stop = false;	
	g_MyD3D->MyCtrl->Energy = 0.0f;

    g_MyD3D->m_fIncMPRatio = 1.0f;

    g_kGlobalValue.m_bFrameLock = false;

    // ´øÀü ÆÛÆ÷¸Õ½º Åë°è
#if defined(DUNGEONPLAYSTATE)
    g_kGlobalValue.m_kDunPerforState.Init();
    m_iStartingMember = GetLoadStartingMembers();
    g_kGlobalValue.m_kDunPerforState.iLoadingStarttingMember = GetLoadStartingMembers();
#endif

    // ¸ó½ºÅÍ »óÅÂ Åë°è
    g_kGlobalValue.m_kMonsterTypeStat.Init();
    g_kGlobalValue.m_kMonsterTypeStat.m_nTotalPartyNum = m_iStartingMember;
}

void KGCStateLoading::StageLoading()
{
    EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();

    switch( eGameModeCategory )
    {
        // ´ëÀü Á¾·ù´Â ¿¾³¯ ¹ßÆÇ ½Ã½ºÅÛ
    case GC_GMC_MATCH:
    case GC_GMC_TAG_MATCH:
    case GC_GMC_INDIGO:
    case GC_GMC_GUILD_BATTLE:
    case GC_GMC_DEATHMATCH:
    case GC_GMC_ANGELS_EGG:
    case GC_GMC_CAPTAIN:
    case GC_GMC_AUTOMATCH:
    case GC_GMC_FATAL_DEATHMATCH:
        {
            // ¸Ê ½ºÅ×ÀÌÁö¸¦ ·ÎµùÇÑ´Ù.
            g_MyD3D->MyStg->Load_Stage();
            break;
        }
    case GC_GMC_MINIGAME:
        {
            SiKGCMinigameManager()->LoadStage();
            break;
        }
    case GC_GMC_MONSTER_CRUSADER:
        {
            // À½.. ÀÌ°É ¾²Áö ¾ÊÀ¸´Ï ±×³É ÃÊ±âÈ­..-¤µ - ÀÇ °³³ä?
            g_MyD3D->MyStg->SetStage( GC_GS_FOREST_OF_ELF );

            int iMonsterID = SiKGCRoomManager()->GetRoomInfo().iMonsterID;

            SMonsterCrusaderInfo sMonsterCrusaderInfo;
            for( int i = 0; i < (int)g_kGlobalValue.m_mapMonsterCrusaderInfo.size(); ++i )
            {
                if( iMonsterID == g_kGlobalValue.m_mapMonsterCrusaderInfo[i].iMonsterID )
                {
                    sMonsterCrusaderInfo = g_kGlobalValue.m_mapMonsterCrusaderInfo[i];
                    break;
                }
            }
            SiKGCLayerManager()->LoadScript( sMonsterCrusaderInfo.strMap );

            if( iMonsterID == MON_VS_PATUSEI_HEAD )
            {
                // ¹°À» Ã¤¿î´Ù
                SiKGCFantasticMap()->SetWaterHeight( 927 * 0.0025f );
                SiKGCFantasticMap()->CreateWater();
            }
            else
            {
                SiKGCFantasticMap()->ClearWater();
            }

            break;
        }
    case GC_GMC_MONSTER_HUNT:
        {
            // À½.. ÀÌ°É ¾²Áö ¾ÊÀ¸´Ï ±×³É ÃÊ±âÈ­..-¤µ - ÀÇ °³³ä?
            g_MyD3D->MyStg->SetStage( GC_GS_FOREST_OF_ELF );

            // ¸î´Ü°è ÀÎ°¡?
            int iLevel = SiKGCRoomManager()->GetDungeonLevel();
            int iVecSize = (int)g_kGlobalValue.m_mapMonsterHuntInfo.size(); 

            if( iLevel >= iVecSize )
                iLevel = iVecSize - 1;

            SMonsterHuntInfo kMonsterHuntInfo = g_kGlobalValue.m_mapMonsterHuntInfo[iLevel];
            SiKGCLayerManager()->LoadScript( kMonsterHuntInfo.strMap );

            break;
        }
    case GC_GMC_DOTA:
        {
            g_MyD3D->MyStg->SetStage( GC_GS_FOREST_OF_ELF );

            bool bError=false;
            SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            if( pDotaTemplate )
            {
                if( "" != pDotaTemplate->strMap )
                    bError = !SiKGCLayerManager()->LoadScript( pDotaTemplate->strMap );
                else bError = true;
            }
            else bError = true;

#ifdef __ORIGIN_DEV__
            if (bError)
            {
                char szTemp[1024];
                sprintf(szTemp, "DotaTemplate.stg 오류 : 선택된 맵ID : %d", g_kGlobalValue.m_iCurDotaMapID);
                MessageBoxA(NULL, szTemp, "DotaError", MB_OK);
            }
#endif
            break;
        }
    case GC_GMC_AGIT:   
        {
            std::string strMap = SiKGCAgitGameManager()->GetMapFileName( SiKGCAgitGameManager()->GetAgitInfo().m_dwMapID );
            SiKGCLayerManager()->LoadScript( strMap );
            break;
        }
        // ´øÀüÀº ½ºÅ©¸³Æ®¿¡¼­ ¾Ë¾Æ¼­ ÇÔ..
    case GC_GMC_DUNGEON:
        {
            // ¹«ÇÑ´øÀü ÀÏ°æ¿ì ¼ÒÈ¯µÇ´Â ¸ðµç ¸ó½ºÅÍ¸¦ ¹Ì¸® ·ÎµùÇÑ´Ù.
            if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
            {
                SiKGCInfinityDungeonManager()->PreGeneraterStageMonster( SiKGCRoomManager()->GetGameMode() );
            }
        }
        break;
    }
}

void KGCStateLoading::AdjustPlayerPosition()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        // ´øÀüÀÌ ¾Æ´Ò ¶§.. ´øÀüÀº ½ºÅ©¸³Æ®¿¡¼­ ¼³Á¤ÇØ ÁØ´Ù!
        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        {
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                if( g_MyD3D->MyPlayer[i] && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
                {
                    g_MyD3D->MyPlayer[i]->bIsRight = true;
                    g_MyD3D->MyPlayer[i]->vPos.x = 0.5f;
                    g_MyD3D->MyPlayer[i]->vPos.y = 0.5f;
                }
            }

        }
    }
	else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
	{
		// 2008.06.22 milennium9 ~~minigame~~
		// ¹Ì´Ï°ÔÀÓÀº ½ºÅ©¸³Æ®¿¡¼­ ÄÉ¸¯ÅÍ À§Ä¡¸¦ Àâ¾ÆÁÝ½Ã´Ù.
		SiKGCMinigameManager()->SetPlayerPosition();
	}
    else
    {
        SetPlayerPosition();
    }

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
        if( !pPlayer->m_kUserInfo.bLive )
            continue;

        pPlayer->All_Latency_Equal();
    }
}

void KGCStateLoading::SetPlayerPosition()
{
    bool bFailedLoadScript = false;
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
    {
        bFailedLoadScript = true;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "PVPStartPosition.stg" ) == false )
    {
        bFailedLoadScript = true;
    }

    if( false == bFailedLoadScript )
    {
        int iMapIndex = int(SiKGCRoomManager()->GetRoomInfo().ucStage);
        if( iMapIndex > NUM_TOTAL_STAGE )
            bFailedLoadScript = true;
        else
        {
            char szTableName[50] = "";
            sprintf(szTableName,"Stage%02d", iMapIndex);
            LUA_BEGIN_TABLE_RAII( szTableName )
            {
                if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT )
                {
                    LUA_BEGIN_TABLE_RAII( "TEAM_FIGHT" )
                    {
                        for ( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
                        {
                            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

                            if ( !pPlayer->m_kUserInfo.bLive )
                                continue;

                            LUA_BEGIN_TABLE_RAII( (iLoop+1) )
                            {
                                LUA_GET_VALUE_DEF(1, pPlayer->vPos.x, 0.0f );
                                LUA_GET_VALUE_DEF(2, pPlayer->vPos.y, 0.0f );
                                LUA_GET_VALUE_DEF(3, pPlayer->bIsRight, true);
                            }
                        }
                    }
                    else
                        bFailedLoadScript = true;
                }
                else
                {
                    LUA_BEGIN_TABLE_RAII( "ALONE_FIGHT" )
                    {
                        for ( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
                        {
                            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

                            if ( !pPlayer->m_kUserInfo.bLive )
                                continue;

                            LUA_BEGIN_TABLE_RAII( (iLoop+1) )
                            {
                                LUA_GET_VALUE_DEF(1, pPlayer->vPos.x, 0.0f );
                                LUA_GET_VALUE_DEF(2, pPlayer->vPos.y, 0.0f );
                                LUA_GET_VALUE_DEF(3, pPlayer->bIsRight, true);
                            }
                        }
                    }
                    else
                        bFailedLoadScript = true;
                }
            }
            else
                bFailedLoadScript = true;
        }
    }

    if( bFailedLoadScript )
    {
        if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT )
        {
            // ÆÀº°·Î µû·Î ±¸ºÐÇÏ¿© Ã»ÆÀÀÌ¸é ¿À¸¥ÂÊ¿¡, È«ÆÀÀÌ¸é ¿ÞÂÊ¿¡ À§Ä¡½ÃÅ²´Ù.
            float Left  = 0.1f;
            float Right = g_MyD3D->MyStg->Num_Width * 1.5f - 0.1f;

            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

                if ( !pPlayer->m_kUserInfo.bLive )
                    continue;

                if ( pPlayer->m_kUserInfo.IsBlueTeam() )
                {
                    Right -= 0.15f;
                    pPlayer->bIsRight = false;
                    pPlayer->vPos.x = Right;
                }
                else
                {
                    Left += 0.15f;
                    pPlayer->bIsRight = true;
                    pPlayer->vPos.x = Left;
                }
                // ¹ßÆÇÀ» È®ÀÎÇÏ¿© Á¤È®ÇÑ À§Ä¡·Î ¿Å±ä´Ù.
                D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pPlayer->vPos.x );
                pPlayer->vPos.x = vPos.x;
                pPlayer->vPos.y = vPos.y;
            }
        }
        else
        {
            // ÇÃ·¹ÀÌ¾î ¼ö·ÎºÎÅÍ °£°Ý ¼ö¸¦ ¾ò¾î¿Í¼­ °ñ°í·ç ÆÛÁö°Ô À§Ä¡½ÃÅ²´Ù,
            char cNetPlayer = (char)SiKGCRoomManager()->GetUserCount();
            if ( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() )
                cNetPlayer--;
            if ( 0 == cNetPlayer )
                cNetPlayer = 1;
            float Pitch = (float)(g_MyD3D->MyStg->Num_Width * 1.5f - 0.2f) / (float)cNetPlayer;

            // °è»êµÈ °£°Ý¿¡ µû¶ó ÇÃ·¹ÀÌ¾î¸¦ À§Ä¡½ÃÅ²´Ù.
            int iCount = 0;
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

                if ( !pPlayer->m_kUserInfo.bLive )
                    continue;

                pPlayer->vPos.x = 0.1f + iCount++ * Pitch;

                // ¸ÊÀÇ ¿ÞÂÊ¿¡ ÀÚ¸®ÇÑ´Ù¸é ¿À¸¥ÂÊÀ» ¹Ù¶óº¸°í,
                // ¸ÊÀÇ ¿À¸¥ÂÊ¿¡ ÀÚ¸®ÇÑ´Ù¸é ¿ÞÂÊÀ» ¹Ù¶óº»´Ù.
                pPlayer->bIsRight = ( pPlayer->vPos.x < g_MyD3D->MyStg->Num_Width * 0.75f );

                // ¹ßÆÇÀ» È®ÀÎÇÏ¿© Á¤È®ÇÑ À§Ä¡·Î ¿Å±ä´Ù.
                D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pPlayer->vPos.x );
                pPlayer->vPos.x = vPos.x;
                pPlayer->vPos.y = vPos.y;
            }
        }
    }
}

void KGCStateLoading::PrevInitPlayer()
{
    // PlayerWork¿¡ Contents¸¦ »ðÀÔ
    g_pGameBoard->OnStartGame();

    // 2007/4/30. iridology. ¾îµò°¡ ³ÖÀ»°÷ÀÌ ÀÖÀ»ÅÙµ¥ ¹Ùºü¼­ ÀÌ¸¸..
    //g_MyD3D->MyBuff->Initialize();
    //g_pMagicEffect->ClearMagicEffect();
    g_pMagicEffect->ClearMagicEffect();

    KGCPlayerCommon::MakeSoundLoadList();
}

void KGCStateLoading::InitPlayer()
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
    {
        if( m_iPlayerInitCount == 0 )
        {
            SiKGCMinigameManager()->InitPlayer();
        }
        ++m_iPlayerInitCount;
        return;
    }

    if ( g_MyD3D->MyPlayer[m_iPlayerInitCount]->m_kUserInfo.bLive )
    {
        if( SiKGCRoomManager()->GetGameMode() == GC_GM_DOTA )
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->Calculate_Arena_Pos();

        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
        {
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->BeforeStartGame();
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->CreatePlayer();
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->MakeSoundLoadList();
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->MakeFireInfo3DObjectLoadList();
            g_MyD3D->MyPlayer[m_iPlayerInitCount]->InitBonusBuff( true );
        }

        if( SiKGCRoomManager()->GetGameMode() == GC_GM_INDIGO_TEAM )
        {
            for( int i = 0; i < 3; ++i )
            {
                if( g_MyD3D->MyPlayer[m_iPlayerInitCount]->m_kUserInfo.bLive == false )
                    continue;

                g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex( m_iPlayerInitCount, g_MyD3D->MyPlayer[m_iPlayerInitCount]->m_kUserInfo.aiTagSlot[i] );
                g_pGraphicsHelper->LoadPlayerModel( m_iPlayerInitCount, g_MyD3D->MyPlayer[m_iPlayerInitCount]->m_kUserInfo.aiTagSlot[i] );
            }
        }
    }

    
    if( ++m_iPlayerInitCount == MAX_PLAYER_NUM )
        m_iPlayerInitCount = 0;
}

void KGCStateLoading::AfterInitPlayer()
{
    AdjustPlayerPosition();

	g_MyD3D->GetMyPlayer()->SetIncMP( 0.0015f );
    g_MyD3D->m_fIncMPShield = 0.001f;

    g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->First_Receive = true;

    // Ä«¸Þ¶ó°¡ ÁÖ½ÃÇÒ °÷À» ¼³Á¤ÇÑ´Ù.
    if( g_MyD3D->IsLocalPlayerObserver() )
    {
        // °üÀüÀÚ¶ó¸é ¿ø°Å¸®¿¡¼­ º¸µµ·Ï ÃÊ±âÈ­ÇÑ´Ù.
        g_kCamera.SetNextTargetPlayer(0);
    }
    else
    {
        // ³ª ÀÚ½ÅÀ» Å¸°ÙÀ¸·Î Ä«¸Þ¶ó¸¦ Àâ´Â´Ù.
        g_kCamera.SetTargetPlayer(g_MyD3D->Get_MyPlayer());
    }

    // Ä«¸Þ¶óÀÇ Ã³À½ À§Ä¡ ¼¼ÆÃ
    g_kCamera.InitCameraPosition();

    // ³×Æ®¿öÅ© ÆÐÅ¶ °ü·Ã ÃÊ±âÈ­
	g_kFrameInfoPacket->Life          = 5;
	g_kFrameInfoPacket->Body_Angle    = 0;
    // È£½ºÆ®¸¸ ÁÖ·Î »ç¿ëÇÏ´Â ³»¿ë
    g_kTransNPC->TransNPC.Init();
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_kEffectItem->Eff_Item[i].cWhat = -1;
        g_kEffectItem->Eff_Item[i].sTime = 0;
    }

    //====================================================================================
    // 2007.02.22 : Asirion
    // °ÔÀÓ ½ÃÀÛÇÏ±âÀü g_kTransform ÃÊ±âÈ­.
    // ³ªÁß¿¡ À§Ä¡ ¿Å°Ü¾ß ÇÑ´Ù. 
    // 2007/3/16. iridology. À§Ä¡ ¿Å±è
    memset( g_kTransform, 0, sizeof(KGC_PID_TRANSFORM ) );
    g_kTransform->cTransformType = FORM_NUM;
    g_kTransform->Type = GC_PID_TRANSFORM;

    memset( g_kSwapWeapon, 0, sizeof(KGC_PID_SWAPWEAPON) );
    g_kSwapWeapon->Type = GC_PID_SWAPWEAPON;
    g_kSwapWeapon->ucPlayerIndex = UCHAR_MAX;
    g_kSwapWeapon->ucWeaponType = 0;

	g_kUsePetSkill->bUsePetSkill = false;
    g_kTagCharacter->cPlayerIndex = -1;
    g_kTagCharacter->bArena = false;

	// ½ÂºÎ °áÁ¤ ÇÃ·¡±×¸¦ ²ö´Ù.
	IsDetermineWin = false;
	IsResultComplete = false;
	
	// ÃÊ°í¼ö³ª ¿î¿µÀÚ¶ó¸é ·¹º§ÀÌ °¡Àå ³ôÀº »ç¶÷º¸´Ù Á¶±Ý ´õ ³ôÀº ´É·ÂÄ¡·Î ¼³Á¤ÇØÁØ´Ù.
	Set_HighRanker();

#ifndef __PATH__
	g_MyD3D->InsertTestValue();
#endif
}

void KGCStateLoading::InitGameMode()
{
    if (RecheckFileSHA()==false)
    {
        ExitWithHackAlert( L"" );
    }
    g_pStateGame->GetCurGameModePtr()->m_kSanta.Init();

    EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
    switch( eGameModeCategory )
    {
        // ¸ó½ºÅÍ ¿øÁ¤´ë (º¸½º ·¯½¬) ¸ó½ºÅÍ ·Îµù
        case GC_GMC_MONSTER_CRUSADER:
        {
            g_MyD3D->m_pkQuestGameMgr->EnterQuest();
            // ¸ó½ºÅÍ¸¦ ¿©±â¼­ ·ÎµåÇÒ±î? -¤µ -;;
            g_MyD3D->ClearMonster();
			
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				mit->second->Release();
			}

            int iMonsterID = SiKGCRoomManager()->GetRoomInfo().iMonsterID;

            g_MyD3D->AddMonster( iMonsterID, 0.5f, 0.0f, false, false, true, g_pkQuestManager->GetPlayerAveLv() );
            if( iMonsterID == MON_VS_PATUSEI_HEAD )
            {
                g_MyD3D->AddMonster( MON_VS_PATUSEI_BODY, 0.5f, 0.0f, false, false, true );
                g_MyD3D->AddMonster( MON_VS_PATUSEI_LEG, 0.5f, 0.0f, false, false, true );
            }

            // ¸ó½ºÅÍ ¿øÁ¤´ë´Â ´É·ÂÄ¡ º¸Á¤ÀÌ ÀÖÀ½!
            // ÇÃ·¹ÀÌ¾î ´É·ÂÄ¡¸¦ º¸°í °è»êÇÏ±â ¶§¹®¿¡..
            // ÀÌ ½ÃÁ¡Àº ÇÃ·¹ÀÌ¾î°¡ ¸ðµÎ ·Îµù µÈ »óÅÂ¿©¾ß ÇÑ´Ù
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				MONSTER* pMonster = mit->second;
				if( pMonster->m_bLive )
				{
					pMonster->SetTargetingDelayTime( GC_FPS_LIMIT * 6 );
				}
			}

            break;
        }

        case GC_GMC_MONSTER_HUNT:
        {
            // ¸î´Ü°è ÀÎ°¡?
            int iLevel = SiKGCRoomManager()->GetDungeonLevel();
            int iVecSize = (int)g_kGlobalValue.m_mapMonsterHuntInfo.size(); 

            if( iLevel >= iVecSize )
                iLevel = iVecSize - 1;

            std::vector< SMonsterHuntMonsterInfo > vecMonsterInfo = g_kGlobalValue.m_mapMonsterHuntInfo[iLevel].vecMonsterInfo;

            g_MyD3D->ClearMonster();
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				mit->second->Release();
			}

			for ( std::vector< SMonsterHuntMonsterInfo >::iterator vit = vecMonsterInfo.begin() ; vit != vecMonsterInfo.end() ; ++vit ) {
				int iMonsterIndex = g_MyD3D->AddMonster( vit->iMonsterType, 0.0f, 0.0f, false, true );
				MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
				if( pMonster == NULL )
					break;
                pMonster->SetTargetingDelayTime( GC_FPS_LIMIT * 6 );
            }

            break;
        }
        case GC_GMC_DOTA:
        {
            // ¾Æ·¡ ¿¹Ã³·³ ·ëÁ¤º¸¿¡ µµÅ¸¸Ê(MapID) ÀÎµ¦½ºÁ¤º¸µµ ÇÊ¿ä..
            SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();

            // ¸Ê ÀÎµ¦½º°¡ Àß¸øµÇ¾ú°Å³ª µµÅ¸ÅÛÇÃ¸´ ½ºÅ©¸³Æ® ¿À·ùÀÓ.
            if( pDotaTemplate )
            {
                // ÀÓ½Ã Äù½ºÆ® ³Ö±â
                int iQuestIdx = g_MyD3D->m_pkQuestGameMgr->GetQuestIndexFromGameMode( GC_GM_DOTA );
                SiKGCRoomManager()->GetRoomInfo().ucStage = iQuestIdx;

                // Äù½ºÆ® ·Îµå
                g_MyD3D->m_pkQuestGameMgr->EnterQuest();

                // ¸ó½ºÅÍ ÃÊ±âÈ­

				for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
					mit->second->Release();
				}

                g_MyD3D->ClearMonster();

                // °ÔÀÌÆ® ¼ÒÈ¯¸ó½ºÅÍ ¹Ì¸® ·Îµå( ¾ÈÇÏ¸é ¼ÒÈ¯ÇÒ¶§ ·º°É¸² )
                std::vector<SDotaGateInfo>::iterator vecGateIter = pDotaTemplate->vecsGateInfo.begin();
                for(; vecGateIter != pDotaTemplate->vecsGateInfo.end(); ++vecGateIter)
                {
                    std::vector<int>::iterator vecMonsterIter = vecGateIter->vecMonsterType.begin();
                    for(; vecMonsterIter != vecGateIter->vecMonsterType.end(); ++vecMonsterIter)
                    {
                        g_MyD3D->CreateMeshAnimationForPrevLoad( *vecMonsterIter );
                    }
                }
            }
#ifdef __ORIGIN_DEV__
            else
            {
                char szTemp[1024];
                sprintf(szTemp, "DotaTemplate.stg 오류 : 선택된 맵ID : %d", g_kGlobalValue.m_iCurDotaMapID);
                MessageBoxA(NULL, szTemp, "DotaError", MB_OK);
            }
#endif
        break;
        }
    }
}

void KGCStateLoading::OnSetupFSM()
{
	ON_SETUP_FSM( GS_GAME,		GS_SERVER );
	ON_SETUP_FSM( GS_SERVER,	GS_SERVER );
	ON_SETUP_FSM( GS_MATCH,		GS_SERVER );
    ON_SETUP_FSM( GS_WORLDMAP,  GS_SERVER );

	m_eStateForFail = GS_SERVER;
}

void KGCStateLoading::SetLoadComplete( DWORD dwUID_ )
{
    KLocker lock( m_csWaitToLoad );
	// TODO(´ëµÎ) : ´ëµÎ ¾ø´Â °æ¿ìµéÀÌ »ý°Ü³ª´ÂÁö ¾Ë ¼ö ÀÖµµ·Ï ¼­¹ö·Î ¸®Æ÷ÆÃ ÇÏµµ·Ï ¹Ù²ã¶ó
	if( m_mapLoadComplete.find( dwUID_ ) == m_mapLoadComplete.end() )
	{

	}
	else
	{
		m_mapLoadComplete[dwUID_] = true;
	}
}

void KGCStateLoading::RemoveFromLoadList( DWORD dwUID_ )
{
	KLocker lock( m_csWaitToLoad );

	m_mapLoadComplete.erase( dwUID_ );
	
}

bool KGCStateLoading::IsLoadComplete( DWORD dwUID_ )
{
	// TODO(´ëµÎ) : ¼­¹ö ¸®Æ÷ÆÃ
	if ( m_mapLoadComplete.find( dwUID_ ) == m_mapLoadComplete.end() )
	{
		return false;
	}
	return m_mapLoadComplete[dwUID_];
}

bool KGCStateLoading::IsLoadComplete()
{
	std::map<DWORD, bool>::iterator itm;
	for ( itm = m_mapLoadComplete.begin(); itm != m_mapLoadComplete.end(); itm++ )
	{
		if ( itm->second == false )
			return false;
	}
	return true;
}

bool KGCStateLoading::CheckLoadingTime()
{
    // ·Îµù¿À¹öÅ¸ÀÓÀÌ Ã¼Å©°¡ µÇ¾ú´Ù¸é ¹Ýº¹ÇØ¼­ ¾Æ·¡ °Ë»ç¸¦ ¼öÇàÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
    if( m_bOverLoadingTime )
        return true;

    // ÀÚ½ÅÀÇ ·ÎµùÀÌ ³¡³µ´Ù¸é ¾Æ·¡ °Ë»ç¸¦ ¼öÇàÇÏÁö ¾Ê°Ú´Ù.
    if( IsLoadComplete(g_kGlobalValue.m_kUserInfo.dwUID) )
        return false;    

    if ( m_kLoadingTimeStopWatch.IsStop() )
    {
		KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::EXCEED_LOADING_TIME);
		
        //¹«ÇÑ·ÎµùÀÌ´Ù ¤Ñ        
        g_pStateGame->OnExitGame();
        g_pkUIScene->m_pkGameOverlayUI->HideAllQuestTitle();

#if defined(NATION_KOREA)
        std::wstring strTemp = boost::str(boost::wformat( L"%1% - (%2%)")
            %g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE2 )%KClientErr::EM_LOADING_TIME_OUT3);

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_ROOM,
            g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE1 ),
            strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_ROOM,
            g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE1 ),
            g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );

#endif
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_INFINITLOADING );

        m_bOverLoadingTime = true;

        return true;
    }

    return false;
}

void KGCStateLoading::SendStatisticsInfo()
{
	SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_P2P_LOADING_PACKET_END);
	// °ÔÀÓÀ» ½ÃÀÛÇÏ´Â À¯ÀúµéÀ» Ä«¿îÆ® ÇÏ±â À§ÇÑ °Í. ¸ðµç À¯ÀúÀÇ ·ÎµùÀÌ ³¡³ª¼­ °ÔÀÓ ½ÃÀÛÇÒ ¶§ º¸³»´Â ÆÐÅ¶ÀÌ´Ù.
	SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_P2PFAIL, KFailRate::PLAY_GAME_USER );

	// 2006.11.14 : Asirion
	// 2008.04.25 milennium9
	// ºê¶óÁú¿¡¼­ ÇÏ³ªÀÇ IP¸¦ °øÀ¯ÇØ¼­ »ç¿ëÇÏ´Â È¯°æ¿¡¼­ ¾î¶² À¯Àú°¡ °ÔÀÓÀ» Á¾·á ÇßÀ» ¶§
	// ±× À¯Àú°¡ ¾Æ´Ñ ´Ù¸¥ À¯Àú°¡ °ÔÀÓÀÌ Á¾·á µÇ¾î ¹ö¸®´Â Çö»óÀÌ »ý°Ü¼­ ¾Ë¾Æº¸±â À§ÇÑ Åë°è
	// ÆÐÅ¶
	std::vector<DWORD> vecPlayerUID;
	for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
		{
			continue;
		}
		vecPlayerUID.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
	}
	std::vector<DWORD> vecPeerUID = KSingleton<KncP2P>::GetInstance()->GetPeerUID();
	KP2P::GetInstance()->Send_CheckRoomUserlistNot( vecPeerUID, vecPlayerUID );

	std::vector<DWORD> vecStartingMembers = GetGameStartingMembers();
    bool bSomeThingWrong = false;
	for ( int i = 0; i < (int)vecStartingMembers.size(); i++ )
	{
		if( KSingleton<KncP2P>::GetInstance()->IsIn( vecStartingMembers[i] ) == false )
		{
            std::wstringstream strText;
            strText << " DungeonID : "<< SiKGCRoomManager()->GetGameMode() << ", CurrentStage : " << g_pkQuestManager->m_iCurStage
                << ", Current Dungeon Level : " << SiKGCRoomManager()->GetDungeonLevel()
                << ", g_sTG_MaxQuestID : "      << g_sTG_MaxQuestID << ", IsHost : " << g_kGlobalValue.m_kUserInfo.bHost;

			SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_ST_MEMBER_NOT_ING_P2P, strText.str() );
            bSomeThingWrong = true;
    	}
	}

    if( bSomeThingWrong ) 
    {
        // ÀÌ·± °æ¿ì¸é °ÔÀÓ¿¡ µé¾î°¡´õ¶óµµ 100% ¹®Á¦ »ý±ä´Ù.
        g_MyD3D->m_pStateMachine->ExitRoom( true );
#if defined(NATION_KOREA)
        std::wstring strTemp = boost::str(boost::wformat( L"%1% - (%2%)")
            %g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE2 )%KClientErr::EM_LOADING_P2P_CONNECT);

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_ROOM,
                g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE1 ),
                strTemp );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_ROOM,
#if defined(__PATH__)
            g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE1 ),
#else
			std::wstring(__WFUNCTION__),
#endif
            g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE2 ) );
#endif
    }

	// °ÔÀÓ¿¡¼­ »ç¿ëÇÏ´Â ¸®½ºÆ®¿Í ¤Ñ p2p Á¢¼ÓÀ» À§ÇØ °ü¸®ÇÏ´ø ¸®½ºÆ®°¡ ¸Â°Ô ¸ÅÄ¡ µÇ´ÂÁö È®ÀÎ
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) ||
			g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ||
			g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost )
		{
			if( !KSingleton<KncP2P>::GetInstance()->IsIn(g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID) &&
				!bSomeThingWrong )
			{
                if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH || 
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
                {
				    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_NOT_MATCH_PEERMGR_PLAYER_D );
                }
                else
                {
                    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_NOT_MATCH_PEERMGR_PLAYER );
                }
				break;
			}

		}	
	}

}

std::set<DWORD> KGCStateLoading::GetWaittingUserList()
{
    KLocker lock( m_csWaitToLoad );
	std::set<DWORD> setWaitingUserList;
	std::map< DWORD, bool >::iterator itm;
	for ( itm = m_mapLoadComplete.begin(); itm != m_mapLoadComplete.end(); itm++ )
	{
		if ( itm->second == false )
		{
			setWaitingUserList.insert( itm->first );
		}
	}
	return setWaitingUserList;
}

bool KGCStateLoading::IsConnectComplete( void )
{
	return m_bConnectComplete;

    KLocker lock2( m_csCompleteCon );
	std::map< DWORD, bool >::iterator itm;
	for( itm = m_mapLoadComplete.begin(); itm != m_mapLoadComplete.end(); itm++ )
	{
		{
			if ( m_mapUserConnectState[itm->first] == false )
			{
				return false;
			}
		}
	}
	return true;
}

void KGCStateLoading::SetConnectComplete( DWORD dwUID  )
{
    KLocker lock( m_csCompleteCon );

	std::map<DWORD, bool>::iterator itm = m_mapUserConnectState.find( dwUID );
	if ( itm == m_mapUserConnectState.end() )
	{
	}
	else
	{
		itm->second = true;
	}
}

void KGCStateLoading::AddP2PConnectList( DWORD dwUID )
{
    KLocker lock( m_csCompleteCon );
	if ( m_mapUserConnectState.find( dwUID ) != m_mapUserConnectState.end() )
	{
	}
	else
	{
		m_mapUserConnectState[dwUID] = false;
	}
}

void KGCStateLoading::RemoveP2PConnectList( DWORD dwUID )
{
    KLocker lock( m_csCompleteCon );
	if ( m_mapUserConnectState.find( dwUID ) == m_mapUserConnectState.end() )
	{
	}
	else
	{
		m_mapUserConnectState.erase( dwUID );
	}
}

void KGCStateLoading::SendLoadingComplete( int iP2PCount, int iRelayCount )
{
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
		return;

#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
    {
        // TCP Relay ¿¬°á 
        KSingleton<KncP2P>::GetInstance()->ResetConnectQ();
        KSingleton<KncP2P>::GetInstance()->SetRelayOnly( true );
		
        in_addr in;
		in.S_un.S_addr = SiKGCAgitGameManager()->GetTRIpPort().first;

		KSingleton<KncP2P>::GetInstance()->SetupRelayServer(SiKGCAgitGameManager()->GetURIpPort().first, SiKGCAgitGameManager()->GetURIpPort().second);

		SiKP2P()->DisConnectTCPRelay();
		
        if( !SiKP2P()->m_spTRUserProxy->Connect( inet_ntoa(in), SiKGCAgitGameManager()->GetTRIpPort().second, g_kGlobalValue.m_kUserInfo.dwUID ) )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_TCP_CONNECT_FAIL), L"" );
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TCP_CONNECT_ERROR );        
        }

        SiKP2P()->m_spTRUserProxy->SetPingTimeoutLimit( GetRelayPingLimit() );
        SiKP2P()->m_spTRUserProxy->SetPingSendGap( GetRelayPingInterval() );

        Result_AgitLoadComplete = INT_MAX;
        KP2P::GetInstance()->Send_AgitLoadingCompleteReq();
        g_MyD3D->WaitForServerAck( Result_AgitLoadComplete, INT_MAX, 5000, TIME_OUT_VALUE );

        if( Result_AgitLoadComplete == 0 )
        {
            SiKGCAgitGameManager()->SetP2PConnectAtEnter();
            SetLoadCompleteInServer( true );
        }
        else
            KP2P::GetInstance()->Send_AgitLoadingFailNot();

        return;
    }
    else
#endif
    KP2P::GetInstance()->Send_LoadCompleteNot( iP2PCount, iRelayCount );
}

void KGCStateLoading::ClearLoadList()
{
	m_mapLoadComplete.clear();
}

void KGCStateLoading::InsertLoadList( DWORD dwUID )
{
	//if( dwUID != g_kGlobalValue.m_kUserInfo.dwUID )
		m_mapLoadComplete[dwUID] = false;
}


bool KGCStateLoading::IsStartingMember( DWORD dwUID )
{
	return m_mapLoadComplete.find( dwUID ) != m_mapLoadComplete.end();
}

std::vector<DWORD> KGCStateLoading::GetGameStartingMembers()
{
	std::vector<DWORD> vecStartingMember;
	std::map< DWORD, bool >::iterator itm;
	for ( itm = m_mapLoadComplete.begin(); itm != m_mapLoadComplete.end(); itm++ )
	{
		vecStartingMember.push_back( itm->first );
	}
	return vecStartingMember;
}

int KGCStateLoading::GetLoadStartingMembers()
{
    int iCount = 0;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.bLive )
            ++iCount;
    }
    
    return iCount;
}
