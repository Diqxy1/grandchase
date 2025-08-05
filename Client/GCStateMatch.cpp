#include "stdafx.h"
#include "GCStateMatch.h"
#include "MyD3D.h"

#include "GCStateMachine.h"
#include "KMci.h"
//
#include "GCUI/GCServerScene.h"
#include "GCUI/GCWorldMapScene.h"
#include "GCUI/GCMatchScene.h"
#include "GCUI/GCGuildScene.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameDlg.h"
#include "GCUI/KGCLoginScene.h"
#include "GCUI/KGCRegisterScene.h"
#include "KGCChannelManager.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/GCLoadingScene.h"
#include "GCUI/GCSquareLoadingScene.h"
#include "GCUI/GCItemReceiveBox.h"
#include "KGCInviteManager.h"
#include "GCUI/GCChatBox.h"
#include "KGCRoomManager.h"
#include "GCRenderManager.h"

KGCStateMatch::KGCStateMatch(void)
{
    m_dwChannelUID = -1;
    m_bFirstInit = false;
}

KGCStateMatch::~KGCStateMatch(void)
{
}

bool KGCStateMatch::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
	if( g_MyD3D->m_pStateMachine->IsJoinedGuild() )
    {
        // 서버에 길드방 퇴장을 알린다.
        if ( FAILED( g_MyD3D->LeaveGuildRoom() ) )
            return false;
    }

    // 채널에 접속된 상태라면 채널 퇴장을 알린다.
    if( g_MyD3D->m_pStateMachine->IsJoinedChannel() )
    {
        if ( FAILED( g_MyD3D->LeaveChannel() ) )
            return false;
    }

    if ( m_dwChannelUID == 0xFFFFFFFF )
    {
        m_dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
    }

    // 서버에 채널 입장을 알린다.
    if ( FAILED( g_MyD3D->EnterChannel( m_dwChannelUID ) ) )
	{
		if ( FAILED( g_MyD3D->EnterChannel( SiKGCChannelManager()->GetCurrentChannel() ) ) )
		{
			//왕따다.
			g_MyD3D->m_pStateMachine->GoServer();			
			m_dwChannelUID = -1;
			return false;
		}
		else
		{
			m_dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
		}
        
	}


    g_MyD3D->m_pStateMachine->Clear_SavedState();
    return true;
}

bool KGCStateMatch::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    // 응? 왜 이랬음?
    if ( ( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_NUMPADENTER )) )
    {
        if( g_pkUIScene->GetCurMsgBox()->GetWindowName() != "gc_invite_request_box"
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "gc_invite_info"
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "mission_dlg"
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "option_tab" 
            && g_pkUIScene->GetCurMsgBox()->GetWindowName() != "guild_dlg" )
        {
            if( g_pkLobbyChatBox != NULL )
                g_pkLobbyChatBox->SetFocus();
        }
    }

    SiKGCInviteManager()->FrameMove();

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();

    return true;
}

bool KGCStateMatch::Render()
{
    g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );

    if ( BeginScene() )
    {

        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();

        g_ParticleManager->Render( GC_LAYER_UI );
        g_ParticleManager->Render(GC_LAYER_CUSTOM);
        EndScene();
    }

    return true;
}

bool KGCStateMatch::OnInit()
{
    /*if( g_KMci.GetTrack() != GC_BGM_MAIN_THEME_LYRICS )
        g_KMci.Play( GC_BGM_MAIN_THEME_LYRICS, true );*/
    if ( g_KMci.GetTrack() != GC_BGM_MAIN_THEME_LYRICS || g_KMci.GetTrack() != GC_BGM_MAIN_THEME_LOCK )
    {
        
        g_KMci.Play( GC_BGM_MAIN_THEME_LYRICS, true );
    }

    g_MyD3D->m_pStateMachine->SetJoinChannel( true );
    if( m_dwChannelUID != -1 )
    {
        SiKGCChannelManager()->SetCurrentChannel( m_dwChannelUID );
        SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
    }

    g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
    g_pkUIScene->EnableMessageBox( true );

    g_RenderManager->SetOutlineWidth( OUT_LINE_WIDTH_IN_CHANNEL );
    g_pkUIScene->m_pkMatchScene->EnterScene();

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        g_MyD3D->MyPlayer[i]->ReleasePlayerResource();

    // 기본은 대기방상태임..
    SiKGCRoomListManager()->SetWaitRoom( true );
    SiKGCRoomManager()->SetChannelRoomList( false );
    SiKGCRoomListManager()->ClearRoomList();

    // 처음 초기화되었거나 이전에 셋팅된 필터값이 던전용 이였거나 필터범위를 벗어났을 때 초기화 하여라...ㅠ
    if( (false == m_bFirstInit) || SiKGCRoomListManager()->GetFilterSetGameMode() || 
        (GC_GMC_INVALID > SiKGCRoomListManager()->GetRoomFilter() || SiKGCRoomListManager()->GetRoomFilter() >= GC_GMC_COUNT) )
    {
	SiKGCRoomListManager()->SetRoomFilter( GC_GMC_INVALID );
    }

	//	길드전인데 길드서버가 아닐경우 초기화
	if ( SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_GUILD_BATTLE && !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) 
	{
		SiKGCRoomListManager()->SetRoomFilter( GC_GMC_INVALID );
	}

    //채팅 박스에 포커스를~~~!!!
    if( g_pkLobbyChatBox != NULL ) 
    {
        g_pkLobbyChatBox->SetFocus();
    }

    if( g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_MATCH )
    {
        if( g_pkLobbyChatBox != NULL ) 
        {
            g_pkLobbyChatBox->ClearChatBox( GCPairPlaySystem::PPSCT_LOBBY );
        }
    }

	if (g_kGlobalValue.m_bIsRainbowEvent &&
		g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_SERVER )
	{
		if(  g_kGlobalValue.m_bJackPot )
#if defined(FULLMOON_EVENT)
            g_pkLobbyChatBox->AddChat( g_pkStrLoader->GetString(STR_ID_FULLMOON7 ), KGCChatManager::CHAT_TYPE_ADMIN );
#else
			g_pkLobbyChatBox->AddChat( g_pkStrLoader->GetString(STR_ID_RAINBOW7 ), KGCChatManager::CHAT_TYPE_ADMIN );
#endif
        else if( g_kGlobalValue.m_iRainbowEventID != 0 )
#if defined(FULLMOON_EVENT)
			g_pkLobbyChatBox->AddChat( g_pkStrLoader->GetReplacedString(STR_ID_FULLMOONEVENT, "L", g_pkStrLoader->GetString( g_kGlobalValue.m_iRainbowEventID) ), KGCChatManager::CHAT_TYPE_ADMIN );
#else
			g_pkLobbyChatBox->AddChat( g_pkStrLoader->GetReplacedString(STR_ID_RAINBOWEVENT, "L", g_pkStrLoader->GetString( g_kGlobalValue.m_iRainbowEventID) ), KGCChatManager::CHAT_TYPE_ADMIN );
#endif
	}

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) == true )
    {
        ProcessNotifyMessage();
    }

    m_bFirstInit = true;
    return true;
}

bool KGCStateMatch::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateMatch::OnDestroy()
{
	//버그로 작용하고 있는듯 해서 주석 처리 합니다.
    //g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
    g_pkUIScene->m_pkMatchScene->LeaveScene();
    // [2/27/2008] breadceo. 용사의섬 드랍 때문에..
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) == true )
    {
        g_pkUIScene->m_pkItemReceiveBox->ClearItemList();
    }
    g_pkUIScene->HideSpeechBubbleMsg();
}

void KGCStateMatch::SetChannelUID( DWORD dwChannelUID )
{
    if( SiKGCChannelManager()->GetChannel( dwChannelUID ) == NULL )
        m_dwChannelUID = -1;
    else
        m_dwChannelUID = dwChannelUID;
}


void KGCStateMatch::OnSetupFSM()
{
    ON_SETUP_FSM( GS_ROOM,          GS_MATCH );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_MATCH );
    ON_SETUP_FSM( GS_GUILD,         GS_MATCH );
    ON_SETUP_FSM( GS_SHOP_CASH,          GS_MATCH );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,          GS_MATCH );
#endif
	ON_SETUP_FSM( GS_MY_INFO,       GS_MATCH );
	ON_SETUP_FSM( GS_TUTORIAL,      GS_MATCH );
    ON_SETUP_FSM( GS_LOGIN,         GS_MATCH );
	ON_SETUP_FSM( GS_REPLAY,        GS_MATCH );
	ON_SETUP_FSM( GS_SQUARE_LOADING,GS_MATCH );
    ON_SETUP_FSM( GS_MATCH,         GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN);
	ON_SETUP_FSM( GS_CHAR_SELECT,   GS_MATCH);
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_MATCH );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
    
#if defined USE_COORDI_SHOP
    ON_SETUP_FSM( GS_COORDI_SHOP,   GS_MATCH );
#endif
    
#if defined( USE_AGIT_SYSTEM )
    ON_SETUP_FSM( GS_GAME_LOADING, GS_SERVER );
#endif
    
    m_eStateForFail = GS_SERVER;
}
