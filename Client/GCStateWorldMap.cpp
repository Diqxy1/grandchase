#include "stdafx.h"
//
#include "GCStateWorldMap.h"
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
#include "GCUI/GCSquareLoadingScene.h"
#include "KGCInviteManager.h"
#include "KGCRoomManager.h"
#include "GCUI/GCChatBox.h"
#include "KGCRoomManager.h"
#include "KGCRoomListManager.h"

KGCStateWorldMap::KGCStateWorldMap(void)
{
}

KGCStateWorldMap::~KGCStateWorldMap(void)
{
}

bool KGCStateWorldMap::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    ////////////////////////////////////////////////////////////////////////////////////////start
    // 우선 그냥 아무 채널이나 접속하도록 하겠음..
    DWORD dwDungeonChannelUID = SiKGCChannelManager()->GetDungeonChannelUID();

    if( SiKGCChannelManager()->GetChannel( dwDungeonChannelUID ) == NULL )
    {
        KP2P::GetInstance()->Send_ChannelInfoListReq();
        return false;
    }

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

    // 서버에 채널 입장을 알린다.
    if ( FAILED( g_MyD3D->EnterChannel( dwDungeonChannelUID ) ) )
        return false;

    g_MyD3D->m_pStateMachine->SetJoinChannel( true );
    SiKGCChannelManager()->SetCurrentChannel( dwDungeonChannelUID );
    SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );

    g_MyD3D->m_pStateMachine->Clear_SavedState();
    ////////////////////////////////////////////////////////////////////////////////////////end	
	
    return true;
}

bool KGCStateWorldMap::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    SiKGCInviteManager()->FrameMove();

    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
    g_KDSound.FrameMove();

    return true;
}

bool KGCStateWorldMap::Render()
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

bool KGCStateWorldMap::OnInit()
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

    //g_pkUIScene->EnableMessageBox( true );

    // UI 스크립트를 보면 알겠지만.. 월드맵씬은 그냥 테두리만있음
    g_pkUIScene->m_pkWorldMapScene->EnterScene();

    // 혹시 내가 플레이 할 수 있는 레벨보다 높은 레벨이 선택되어 있다면
    // 내가 플레이 할 수 있는 최대 레벨로 세팅해 준다
    SUserInfo* pHostUserInfo = &g_kGlobalValue.m_kUserInfo;
    EGCGameMode eGameMode = SiKGCWorldMapManager()->GetGameMode();
    int nLimiteLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel( eGameMode );
    int nEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( eGameMode, pHostUserInfo->mapStagePlayable );

    if ( nEnableLevel >= nLimiteLevel ) 
    {
        nEnableLevel = nLimiteLevel - 1;
    }

    int nSubGameMode = SiKGCRoomManager()->GetSubGameMode();    
    nEnableLevel += GC_SGM_DUNGEON_NORMAL;


    // 내가 플레이 할 수 있는 레벨이 현재 세팅된 레벨보다 낮다면?
    if ( SiKGCRoomListManager()->IsHellMode() )
    {
        SiKGCRoomManager()->SetSubGameMode( GC_SGM_DUNGEON_HELL );
    }
    else
    {
        if( nEnableLevel < nSubGameMode )
        {
            SiKGCRoomManager()->SetSubGameMode( nEnableLevel );
        }  
    }
    
    // 월드맵 컨테이너에 월드맵이 담겨있다..( 월드맵과 방안에서 모두 보여줘야하기때문에-ㅅ-)
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( true );
    g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
    //g_pkUIScene->m_pkWorldMapContainer->ShowChatBox( false );
    int iCurrentWorldMap = static_cast<int>(SiKGCWorldMapManager()->GetCurrentWorldMap());

    if ( iCurrentWorldMap != WORLDMAP_UPPERMAP &&  g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
        g_pkUIScene->m_pkChatBoxDlgS6->RenderOn();
    else
        g_pkUIScene->m_pkChatBoxDlgS6->RenderOff();

	g_pkWorldmapChatBox->ChangeChatSize(KGCChatBox::CTSIZE_MINIMUM);

    // 기본은 대기방상태임..
    SiKGCRoomListManager()->SetWaitRoom( true );
    SiKGCRoomManager()->SetChannelRoomList( false );

	// 레인보우 공지 출력
	if (g_kGlobalValue.m_bIsRainbowEvent && 
		g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_SERVER )
	{
		if(  g_kGlobalValue.m_bJackPot )
		{
			//g_pkUIScene->m_pkWorldMapContainer->ToggleChatList(true);
#if defined(FULLMOON_EVENT)
            g_pkWorldmapChatBox->AddChat( g_pkStrLoader->GetString(STR_ID_FULLMOON7 ), KGCChatManager::CHAT_TYPE_ADMIN );
#else
            g_pkWorldmapChatBox->AddChat( g_pkStrLoader->GetString(STR_ID_RAINBOW7 ), KGCChatManager::CHAT_TYPE_ADMIN );
#endif
        }
		else if( g_kGlobalValue.m_iRainbowEventID != 0 )
		{
			//g_pkUIScene->m_pkWorldMapContainer->ToggleChatList(true);
#if defined(FULLMOON_EVENT)
            g_pkWorldmapChatBox->AddChat( g_pkStrLoader->GetReplacedString(STR_ID_FULLMOONEVENT, "L", g_pkStrLoader->GetString( g_kGlobalValue.m_iRainbowEventID) ), KGCChatManager::CHAT_TYPE_ADMIN );
#else
            g_pkWorldmapChatBox->AddChat( g_pkStrLoader->GetReplacedString(STR_ID_RAINBOWEVENT, "L", g_pkStrLoader->GetString( g_kGlobalValue.m_iRainbowEventID) ), KGCChatManager::CHAT_TYPE_ADMIN );
#endif
        }
	}
	//dragon 프리패스 이용권이 있을 시 모든 던젼 개방 와울~!
	if((g_MyD3D->m_kItemMgr.FindInventoryForItemID(  KGCItemManager::ITEM_FREEPATH_TICKET ) && !g_kGlobalValue.IsDungeonFreePass() )||
		(!g_MyD3D->m_kItemMgr.FindInventoryForItemID(  KGCItemManager::ITEM_FREEPATH_TICKET ) && g_kGlobalValue.IsDungeonFreePass())
		)
	{
		g_kGlobalValue.SetDungeonFreePass(!g_kGlobalValue.IsDungeonFreePass());
		g_kGlobalValue.m_iUseHellModeBtn = g_kGlobalValue.IsDungeonFreePass();
	}
#ifdef MARI_SHADOW_EVENT
	if(!(rand()%20)){
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SHADOW_EVENT, L"", 
			L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
#endif


    // 캐릭터 전용던전 스테이트 자동이동
    SiKGCPrivateDungeonManager()->MoveStateRun( EM_CREATE_ROOM_TYPE );
    SiKGCPrivateDungeonManager()->MoveStateRun( EM_CLEAR_DUNGEON );
    SiKGCPrivateDungeonManager()->SetNextLevel( false );

    return true;
}

bool KGCStateWorldMap::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateWorldMap::OnDestroy()
{
    // 맵 이펙트 없에자
    g_pkUIScene->m_pkWorldMapContainer->ClearMapEffect();
    g_pkUIScene->HideSpeechBubbleMsg();
	g_pkUIScene->m_pkChatBoxDlgS6->RenderOff();

}

void KGCStateWorldMap::OnSetupFSM()
{
    ON_SETUP_FSM( GS_ROOM,          GS_WORLDMAP );
    ON_SETUP_FSM( GS_MATCH,         GS_WORLDMAP );
    ON_SETUP_FSM( GS_GUILD,         GS_WORLDMAP );
    ON_SETUP_FSM( GS_SHOP_CASH,          GS_WORLDMAP );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,          GS_WORLDMAP );
#endif
    ON_SETUP_FSM( GS_MY_INFO,       GS_WORLDMAP );
    ON_SETUP_FSM( GS_SQUARE_LOADING,GS_WORLDMAP );
    ON_SETUP_FSM( GS_LOGIN,         GS_WORLDMAP );
    ON_SETUP_FSM( GS_REGISTER,      GS_WORLDMAP );
    ON_SETUP_FSM( GS_WORLDMAP,      GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN);
	ON_SETUP_FSM( GS_LOGIN ,         GS_WORLDMAP );
	ON_SETUP_FSM( GS_CHAR_SELECT ,   GS_WORLDMAP );
	ON_SETUP_FSM( GS_CHAR_SELECT_FROM_ROOM ,   GS_WORLDMAP );
	ON_SETUP_FSM( GS_REPLAY ,   GS_WORLDMAP );
    
#if defined USE_COORDI_SHOP
    ON_SETUP_FSM( GS_COORDI_SHOP,   GS_WORLDMAP );
#endif

#if defined( USE_AGIT_SYSTEM )
    ON_SETUP_FSM( GS_GAME_LOADING, GS_SERVER );
#endif

    m_eStateForFail = GS_SERVER;
}
