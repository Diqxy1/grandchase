#include "stdafx.h"
//

#include "GCStateRoom.h"
#include "KMci.h"
#include "MyD3D.h"


#include "KGCRoomSceneS6.h"





#include "GCNetwork.h"


#include "GCUI/GCChatBox.h"
//
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
//
//
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/KGCUIHelper.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameDlg.h"
#include "KGCRoomManager.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCSquareLoadingScene.h"

#include "Controls.h"
#include "../GCStateMachine.h"
#include "GCRenderManager.h"
#include "KGCRoomListManager.h"
#include "KGCMatchManager.h"
#include "KSingleton.h"
#include "KncP2PLib/KncP2P.h"


#include "utf16_cvt.h" // UnitCode �ؽ�Ʈ ������ ����� ����.
KGCStateRoom* g_pStateRoom = NULL;

KGCStateRoom::KGCStateRoom(void)
{
    m_iCurrRoomNum = -1;
    g_pStateRoom = this;
    m_strRoomName.clear();
    m_strRoomPassword.clear();
	m_tmStartGameTimer = 0;
	m_tmExitRoomTimer = 0;
}

KGCStateRoom::~KGCStateRoom(void)
{
}

bool KGCStateRoom::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
    g_MyD3D->m_pStateMachine->Clear_SavedState();
	m_tmStartGameTimer = 0;
	m_tmExitRoomTimer = 0;
    return true;
}

bool KGCStateRoom::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );

    if (( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_NUMPADENTER )) )
    {
        if( g_pkUIScene->GetCurMsgBox()->GetWindowName() != "gc_invite_request_box"
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "mission_dlg"
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "option_tab"
			&& g_pkUIScene->GetCurMsgBox()->GetWindowName() != "auto_match_loading")
        {
            if( g_pkRoomChatBox != NULL )
                g_pkRoomChatBox->SetFocus();
        }
    }

    if( g_pkInput->IsDown( DIK_SYSRQ ) )
    {
        if( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
            Process_Admin_Command( KGCState::PRINT_CURRENT_STATE_USER );
    }

    g_MyD3D->Select_FrameMove();
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();
    g_ParticleManager->FrameMove(g_MyD3D->m_fTime, GC_ELAPSED_TIME);

    //// ���� �ڸ�
    if( true == g_MyD3D->MyCtrl->m_bSleep )
    {
        g_MyD3D->m_pStateMachine->ExitRoom( true );
        g_MyD3D->MyCtrl->NoSleep();
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_SLEEP_OUT ), KGCChatManager::CHAT_TYPE_ADMIN );
    }

	if ( SiKGCMatchManager()->GetCurrentState() ==  KGCMatchManager::MSTATE_ROOM_STATE ) {
		if ( SiKGCRoomManager()->CanStartAutoMachGame() && SiKGCRoomManager()->All_Players_Ready() && m_tmStartGameTimer == 0 && SiKGCRoomManager()->GetHostUID() == g_kGlobalValue.m_kUserInfo.dwUID) {
			m_tmStartGameTimer =  GCUTIL_TIME::GetCurTime() + 3;
		}

		if ( m_tmExitRoomTimer == 0 ) { //15�ʰ� ���۵��� ������ ���� �Ѵ�.
			time_t curTime = GCUTIL_TIME::GetCurTime();
			if ( SiKGCRoomManager()->GetHostUID() == g_kGlobalValue.m_kUserInfo.dwUID )
				m_tmExitRoomTimer = curTime + 13;
			else
				m_tmExitRoomTimer = curTime + 15;
		}

		if ( g_pkUIScene->m_pkAutoMatchLoading && !g_pkUIScene->m_pkAutoMatchLoading->IsCreate() ) {
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AUTOMATCH_LOADING);
		}

	}



	if ( m_tmStartGameTimer != 0 ) {
		time_t curTime = GCUTIL_TIME::GetCurTime();
		if ( m_tmStartGameTimer < curTime) {
			OnGameStart();
			m_tmStartGameTimer = 0;
			return true;
		}
	}
	if ( m_tmExitRoomTimer != 0 ) {
		time_t curTime = GCUTIL_TIME::GetCurTime();
		if ( m_tmExitRoomTimer < curTime) {
			OnExitRoom();
			m_tmExitRoomTimer = 0;
			return true;
		}
	}



#ifdef CHAT_EVENT
	// ä�� �̺�Ʈ�� ���� ��
	if( g_kGlobalValue.m_iChatEventCount != -1 )
	{
		--g_kGlobalValue.m_iChatEventCount;
		if( g_kGlobalValue.m_iChatEventCount == 0 )
		{
			g_kGlobalValue.m_iChatEventCount = -1;
			g_kGlobalValue.m_bChatEventMessageTemp = false;
			g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_CHAT_EVENT_DISABLED3 ) );
		}
	}
#endif

    return true;
}

bool KGCStateRoom::Render()
{
    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        g_ParticleManager->Render( GC_LAYER_UI );
        
        EndScene();
    }

    return true;
}

void KGCStateRoom::OnLeaveState()
{
    SiKGCRoomManager()->GetRoomInfo().iRestBanCount = 3;
    g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_NONE;

    g_pkUIScene->m_pkRoomSceneS6->LeaveScene();
}

bool KGCStateRoom::IsEnableCurrentGameMode()
{
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON &&
        SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_HUNT )
        return true;

	//zstaiji - �系���� ��� AŰ�� ������ ���� ������ Ǭ��.
	//�����н� 
//#if !defined( __PATH__ )
	if( g_kGlobalValue.IsUnlock() || g_kGlobalValue.IsDungeonFreePass() )
		return true;	
//#endif

    SUserInfo* pHostUserInfo = &g_MyD3D->MyPlayer[g_MyD3D->Get_HostPlayer()]->m_kUserInfo;

    switch( SiKGCRoomManager()->GetGameModeCategory() )
    {
        case GC_GMC_DUNGEON:
        {
            int iLv = SiKGCRoomManager()->GetDungeonLevel();
            if( iLv == 0 )
                return true;


            bool bOK = SiKGCWorldMapManager()->IsPlayEnableDungeonLevel( SiKGCRoomManager()->GetGameMode(), 
                                                                         iLv, pHostUserInfo->mapStagePlayable ); 
            return bOK;
       }

        case GC_GMC_MONSTER_HUNT:
        {
            // ���� ������ ���ʹ��������� �ܰ�� ���δ�
            int iLv = SiKGCRoomManager()->GetDungeonLevel();
            if( iLv == 0 )
                return true;

            bool bOK = SiKGCWorldMapManager()->IsPlayEnableDungeonLevel( SiKGCRoomManager()->GetGameMode(), 
                iLv, pHostUserInfo->mapStagePlayable ); 

            return bOK;
        }
    }

    return false;
}
void KGCStateRoom::Process_Admin_Command( AdminCommandType _ACT )
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
                << L"(" << SlotCount << L"_Users)_Room.txt";
            
            std::wstringstream stm;
            stm << L"Room Title : " << (SiKGCRoomManager()->GetRoomInfo().strRoomName.empty()?(L"No Name"):(SiKGCRoomManager()->GetRoomInfo().strRoomName)) << L"\r\n";
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                // �Ⱥ�� ������~
                if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false || ( g_MyD3D->MyPlayer[i]->GetPlayerNameSize() == 0 ) )
                    continue;

                stm << L"NickName : [" << g_MyD3D->MyPlayer[i]->m_kUserInfo.strNickName << L"] Login ID : [" << g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin << L"]" << L"\r\n";
            }
            //�ܼ�â�� �Ѹ��� ���ؼ�.
            wprintf( L"%s \r\n", stmFileName.str().c_str() );
            wprintf( stm.str().c_str() );

            //���� ����.
            std::wofstream wFile;
            IMBUE_NULL_CODECVT( wFile );
            wFile.open( KncUtil::toNarrowString(stmFileName.str()).c_str(), std::ios::binary | std::ios::out );
            if( !wFile.is_open() )
                return;

            wchar_t cBom = 0xfeff;
            wFile.write( &cBom, 1 );

            wFile << stm.str(); // ���� ���Ͽ� ����

            wFile.close();

        }
        break;
    }
}

bool KGCStateRoom::OnInit()
{
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
        KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_READY );
    else
        KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT );


	KP2P::GetInstance()->Send_SetMyIdleState(false);
	KP2P::GetInstance()->Send_GetRoomMenberIdleState();
	KP2P::GetInstance()->Send_GetRoomMenberPressStateReq();

    g_pkUIScene->m_pkRoomSceneS6->EnterScene();


    //ä�� �ڽ��� ��Ŀ����~~~!!!
    if ( g_pkRoomChatBox != NULL ) 
    {
        g_pkRoomChatBox->SetFocus();
    }

    SiKGCRenderManager()->SetOutlineWidth( OUT_LINE_WIDTH_IN_WAIT_ROOM );

    g_pkUIHelper->SetUILockTime( 0 , BUTTON_LOCK_FRAME );

    if( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME ||
        g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_MY_INFO_FROM_ROOM ||
		g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_CHAR_SELECT_FROM_ROOM )
    {
     
        if ( g_kGlobalValue.m_kUserInfo.bHost )
        {
            SiKGCRoomManager()->ResetGameMode();
            if( !( SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo() ) )
            {
                KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );
            }
            if( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME )
            {
                KP2P::GetInstance()->Send_P2PUniqueNumberReq();
            }
        }

        if( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_GAME )
        {
            if ( !( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) ) )
            {
                 // ����� ���̳� 1:1ä�ο����� �Ⱥ����� �ȴ�
                KP2P::GetInstance()->Send_EndResultNot();   // ��� �������� ������ �� �濡 �� �� �ִ�
            }

            std::vector<DWORD> vecPeer = KSingleton<KncP2P>::GetInstance()->GetPeerUID();
            for( int i = 0; i < (int) vecPeer.size(); ++i ) 
            {
                if( vecPeer[i] == g_kGlobalValue.m_kUserInfo.dwUID ) 
                    continue;

                DWORD dwPing = KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
                bool bRelay = KSingleton<KncP2P>::GetInstance()->IsRelay( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
                SiGCClientErrorManager()->Send_PingTimeReport( bRelay, dwPing ); 
            }
 
 			ShutDown( false );
 			for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
 			{
 				AddConnectQ( g_MyD3D->MyPlayer[i]->m_kUserInfo );
 			}
            std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;

            for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
            {
                AddConnectQ( *vit );
            }

 			KSingleton<KncP2P>::GetInstance()->SetupRelayServer( SiKGCRoomManager()->GetRoomInfo().dwRelayServerIP, 
 				SiKGCRoomManager()->GetRoomInfo().usRelayServerPort );

			//���� �ϰ� �濡 ���Դٸ�, ������ �����۵� ������ ����!
            g_pItemMgr->UpdateUserItem( &g_kGlobalValue.m_kUserInfo );
        }

        g_pkUIScene->DestroyCurrentMsgBox();
    }
    else {
        g_pkUIScene->m_pkRoomSceneS6->SetRoomTitle( m_iRoomNum+1, m_strRoomName, m_strRoomPassword );

        m_iCurrRoomNum = m_iRoomNum;
    }

    // [6/3/2008] breadceo. 
	//((KGCStateLoading*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME_LOADING ))->SetConnectComplete( false );
	// ���� ���۵Ǿ����� P2P Ŀ�ؼǵ� ��������.
	//KSingleton<KncP2P>::GetInstance()->ConnectP2P();

    ProcessNotifyMessage();

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if( SiKGCRoomListManager()->IsHellMode() )
        {
            if( g_KMci.GetTrack() != GC_BGM_WORLDMAP_HELL )
                g_KMci.Play( GC_BGM_WORLDMAP_HELL, true );
        }
        else
        {
            int iBGMIndex;
            EGCWorldMap eWorldMap = SiKGCWorldMapManager()->GetCurrentWorldMap();
            iBGMIndex = SiKGCWorldMapManager()->GetWorldMapBgm( eWorldMap );

            if( g_KMci.GetTrack() != iBGMIndex )
                g_KMci.Play( iBGMIndex, true );
        }              
        
        //è�Ǿ� ��忡�� Ƽ�Ͼ����� �ñ���.
        if ( SiKGCRoomManager()->GetDungeonLevel() == GC_DUNGEON_LEVEL_HELL )
        {
            if( !SiKGCRoomManager()->CanPlayHellmode() )
            {
                if ( true == SiKGCWorldMapManager()->IsEventDungeon( SiKGCRoomManager()->GetGameMode() ) ) {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET3 ), L"",  KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                }
                else {
                    g_pkUIScene->ClearWaitMessageBox();
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET1 ), L"",  KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false );
                }
				g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, false );
            }
        }

        // ���������� �ɸ� �������� Ƽ���� ������ �ñ���.
        if( KGCRoomManager::ENTER_IMPOSSIBLE == SiKGCRoomManager()->CheckTicketToDungeonEnter( true ) )
        {
            g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, false );
        }
    }
	else if ( SiKGCMatchManager()->GetCurrentState() == KGCMatchManager::MSTATE_ROOM_STATE )
	{
		//���� ���� ���Ұ���
	}
    else
    {
        // ���� BGM�� �����Ѵ�.
        if( g_KMci.GetTrack() != GC_BGM_LETS_GO )
            g_KMci.Play( GC_BGM_LETS_GO, true );
    }

    // ĳ���� ������� ������Ʈ �ڵ��̵�
    SiKGCPrivateDungeonManager()->MoveStateRun( EM_START_GAME_TYPE );

    return true;
}

void KGCStateRoom::UpdateRoomOption()
{
    if ( g_pkRoomChatBox != NULL ) 
    {
        g_pkRoomChatBox->SetFocus();

		// �븸�� ����� �������� �����ݴϴ�.
	#if defined( NATION_TAIWAN ) || defined( NATION_HONGKONG ) || defined( NATION_THAILAND ) || defined( NATION_IDN ) || defined(NATION_USA) || defined( NATION_EU ) || defined(NATION_CHINA)
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_CHANGE_ROOM_TITLE, "l", m_strRoomName ), KGCChatManager::CHAT_TYPE_ADMIN );
	#else
		g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_CHANGE_ROOM_TITLE ), KGCChatManager::CHAT_TYPE_ADMIN );
	#endif
    }

    g_pkUIScene->m_pkRoomSceneS6->SetRoomTitle( m_iRoomNum+1, m_strRoomName, m_strRoomPassword );

    g_pkUIScene->RoomSceneUpdateData();
}

void KGCStateRoom::SetRoomInfo( int iRoomNum_ , const std::wstring& strRoomName, const std::wstring& strRoomPassword )
{
    m_iRoomNum = iRoomNum_;
    m_strRoomName = strRoomName;
    m_strRoomPassword = strRoomPassword;
}

bool KGCStateRoom::OnDestroyBeforeStateChange()
{
    g_pkUIScene->m_pkRoomSceneS6->LeaveScene();
    return true;
}

void KGCStateRoom::OnSetupFSM()
{
    ON_SETUP_FSM( GS_MATCH,                 GS_SERVER );
    ON_SETUP_FSM( GS_WORLDMAP,              GS_SERVER );
    ON_SETUP_FSM( GS_GAME_LOADING,          GS_MATCH );
    ON_SETUP_FSM( GS_MY_INFO_FROM_ROOM,     GS_MATCH );
    ON_SETUP_FSM( GS_GUILD,                 GS_SERVER );
    ON_SETUP_FSM( GS_SQUARE_LOADING,        GS_MATCH );
    ON_SETUP_FSM( GS_SERVER,                FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT ,   GS_SERVER );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_SERVER );
    m_eStateForFail = GS_SERVER;
}

void KGCStateRoom::OnDestroy()
{

    g_pkUIScene->m_pkRoomSceneS6->LeaveScene();
    g_pkUIScene->HideSpeechBubbleMsg();
    //g_pkUIScene->m_pkItemReceiveBox->ClearItemList();	
}

void KGCStateRoom::OnExitRoom()
{
	
	if ( SiKGCRoomManager()->GetHostUID() == g_kGlobalValue.m_kUserInfo.dwUID ) {
		std::wstringstream strText;
		strText << "SiKGCRoomManager()->CanStartAutoMachGame() : " << (SiKGCRoomManager()->CanStartAutoMachGame() ? "true" : "false")<<"\n";
		strText << "SiKGCRoomManager()->All_Players_Ready() : " << (SiKGCRoomManager()->All_Players_Ready() ? "true" : "false")<<"\n";

		if ( SiKGCRoomManager()->CanStartAutoMachGame() == false ) {
			strText << "���� �� ���� : " << SiKGCMatchManager()->GetRoomUserCnt()<<"\n";
			strText << "�濡 ������ ���� : " << g_MyD3D->GetPlayerNum() <<"\n";		
		}

		if ( SiKGCRoomManager()->All_Players_Ready() == false ) {
			for(unsigned char i=0;i<MAX_PLAYER_NUM;i++)
			{
				if( !g_MyD3D->MyPlayer[i]->IsLocalPlayer() && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
				{
					strText << "g_MyD3D->MyPlayer["<<i<<"]->m_kUserInfo.nUserState : " << (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState == GC_RUS_READY ? "GC_RUS_READY" : "GC_RUS_WAIT")<<"\n";
				}
			}
		}

		SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_USER_START_FAIL_STATE, strText.str() );
	}



	
	
	bool bSubtractRP = false;
	if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) && g_MyD3D->GetPlayerNum() > 1 )
	{
		bSubtractRP = true;
	}

	if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
	{
		g_pkUIScene->ClearWaitMessageBox();
		g_pkUIScene->EnableMessageBox( true );
		g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_WAIT;
	}


	g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, bSubtractRP );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_ERR7));


}
void KGCStateRoom::OnGameStart()
{
	// ������ ���� ���� ��û�� ��.
    Result_StartGame = INT_MAX;

    std::vector< std::pair<char,char> > vecPromotionCount;
    g_MyD3D->GetStatPromotionCount( vecPromotionCount );
    int iMapID = SiKGCMatchManager()->GetStartAutoGameMapID();
    KP2P::GetInstance()->Send_StartGameReq( vecPromotionCount,0,0,iMapID );
    g_MyD3D->WaitForServerAck(Result_StartGame, INT_MAX, 5000, TIME_OUT_VALUE );

    if( g_kGlobalValue.m_bCheckChangedKOM )
		return;

	std::wstringstream stm;
	std::wstringstream strm;
	std::wstring strColorKey    = L"#cff0000";
	std::wstring strColorKeyEnd = L"#cX";     
	stm << strColorKey << g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_START_GAME_ERROR ) << strColorKeyEnd;

	// 051102. jseop. �������� �� ���信 ���� ó��.
	switch( Result_StartGame )
	{
	case 1:
		if ( GC_GM_QUEST6 == SiKGCRoomManager()->GetGameMode() ) // �����̰� ���̳� ����Ʈ��� �������� 1/2/3/4 ��� ��´�.
		{
			std::vector<GCITEMUID> vecItemUID;
			g_pItemMgr->DecInventoryDurationItemForItemID( KAZEAZE_MAP1_ID, &vecItemUID );
			g_pItemMgr->DecInventoryDurationItemForItemID( KAZEAZE_MAP2_ID, &vecItemUID );
			g_pItemMgr->DecInventoryDurationItemForItemID( KAZEAZE_MAP3_ID, &vecItemUID );
			g_pItemMgr->DecInventoryDurationItemForItemID( KAZEAZE_MAP4_ID, &vecItemUID );

			KP2P::GetInstance()->Send_UseDurationItemReq( vecItemUID );
		}
		break;
	case 2:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString( STR_ID_START_GAME_ERROR20 ),
			g_pkStrLoader->GetString( STR_ID_START_GAME_ERROR21 ) );
		break;
	case 11:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,stm.str(), 
			g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR5 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 13:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,stm.str(), 
			g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR1 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 14:    
		strColorKey = L"#cffff00";
		strm << strColorKey << L"\n"
			<< g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3 )
			<< g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime( SiKGCRoomManager()->GetGameMode() )
			<< strColorKeyEnd;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,stm.str(), 
			g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR7 ) + strm.str(), KGCUIScene::GC_MBOX_USE_END_GAME, 0, 0, false, true, true );
		break;
	case 15:
		strColorKey = L"#cffff00";
		strm << strColorKey << L"\n"
			<< g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3 )
			<< g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime( SiKGCRoomManager()->GetGameMode() )
			<< strColorKeyEnd;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,stm.str(), 
			g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR6 ) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 16:
		strColorKey = L"#cffff00";
		strm << strColorKey << L"\n" << g_kGlobalValue.m_kHeroDungeonInfo.GetInitTime() << strColorKeyEnd;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,stm.str(), g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4 ) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		break;
	case 17:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"" );
		break;
	default:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString( STR_ID_START_GAME_ERROR30 ),
			L"" );
		break;
	}

	if ( Result_StartGame != 0 && Result_StartGame != 1 )
		g_MyD3D->m_pStateMachine->ExitRoom( true );

}