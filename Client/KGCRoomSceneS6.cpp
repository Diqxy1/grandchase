#include "stdafx.h"
#include "KGCRoomSceneS6.h"
#include "KDInput.h"
#include "KGCWorldMapManager.h"
#include "GCUI/GCHeroTicketSystem.h"

IMPLEMENT_CLASSNAME( KGCRoomSceneS6 );
IMPLEMENT_WND_FACTORY( KGCRoomSceneS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomSceneS6, "gc_room_scene_s6" );

KGCRoomSceneS6::KGCRoomSceneS6(void)
: /*m_pkRoomExitButton( NULL )*/
 m_pkMyInfoButton( nullptr ) 
, m_pkSkilltreeButton(nullptr)
, m_pkRoomTitle(nullptr)
, m_pkRoomUserPanel(nullptr)
, m_pkRoomDungeonPanel(nullptr)
, m_pkRoomMatchPanel(nullptr)
, m_pkRoomButtonPanel(nullptr)
, m_bEnableStart( false )
, m_dwMissionOpenTime( 0 )
, m_bShowMissionUI( false )
, m_pkTicketPanel(nullptr)
{
    //LINK_CONTROL( "room_exit_button",   m_pkRoomExitButton );
    LINK_CONTROL( "my_info_button",     m_pkMyInfoButton );
    LINK_CONTROL( "skilltree_button",     m_pkSkilltreeButton);
    
    LINK_CONTROL( "room_title",   m_pkRoomTitle );
    LINK_CONTROL( "user_panel",   m_pkRoomUserPanel );
    LINK_CONTROL( "dungeon_panel",   m_pkRoomDungeonPanel );
    LINK_CONTROL( "match_panel",   m_pkRoomMatchPanel );
    LINK_CONTROL( "room_button_panel", m_pkRoomButtonPanel );
    LINK_CONTROL( "room_use_ticket", m_pkTicketPanel );
}

KGCRoomSceneS6::~KGCRoomSceneS6( void )
{
}

void KGCRoomSceneS6::OnInitialize( void )
{
}

void KGCRoomSceneS6::OnCreate( void )
{
    m_pkMyInfoButton->InitState( true, true, this );
    m_pkSkilltreeButton->InitState( true, true, this );
    m_pkRoomTitle->InitState( true, true, this );
    m_pkRoomUserPanel->InitState( true, true, this );
    m_pkRoomDungeonPanel->InitState( true, true, this );
    m_pkRoomMatchPanel->InitState( true, true, this );
    m_pkRoomButtonPanel->InitState( true, true, this );
    m_pkTicketPanel->InitState( false, true, this );

    ToggleRender( false );

#ifdef CALENDAR_RECOMMAND
    g_pkUIScene->m_pkAttendanceInfoMsg->SetWindowPosDirect( D3DXVECTOR2( 385.0f * GC_SCREEN_DIV_WIDTH, 2.0f * GC_SCREEN_DIV_WIDTH ) );
    g_pkUIScene->m_pkAttendanceInfoMsg->SetMsgType( KGCInventoryFullMsg::CALENDAR_INFO );
#endif
}

void KGCRoomSceneS6::HeroTicketPanelManager( bool iActive )
{
    if( iActive )
    {
        m_pkTicketPanel->ToggleRender( true );
        m_pkRoomButtonPanel->ToggleRender( false );
    }
    else
    {
        m_pkTicketPanel->ToggleRender( false );
        m_pkRoomButtonPanel->ToggleRender( true );
    }
}

void KGCRoomSceneS6::FrameMoveInEnabledState( void )
{   
    std::pair<int, int> pairClearInfo = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo( SiKGCRoomManager()->GetGameMode() );
    bool IsHeroTicketUsed = g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeonPlayTicket( SiKGCRoomManager()->GetGameMode() );

    if ( ( pairClearInfo.first >= pairClearInfo.second && pairClearInfo.second > 0 ) && !IsHeroTicketUsed )
    {
        m_pkTicketPanel->UpdateDialog();
        g_pkUIScene->m_pkRoomSceneS6->HeroTicketPanelManager( true );
    }
    else
    {
        g_pkUIScene->m_pkRoomSceneS6->HeroTicketPanelManager( false );
    }

    if( m_pkRoomButtonPanel->GetExitBtnState() )
    {
        OnExitRoom();
        m_pkRoomButtonPanel->SetExitBtnState( false );
    }

    if( 0 == g_pkUIHelper->GetUILockTime( LOCK_ROOM ) )
    {
        m_bEnableStart = true;
    }

    SiKFriendShip()->CheckAlramAddFriend();

    // 패치가 덜 받혔는지 체크한다. ( 체크 기준은 모델 파일이나 모션파일만이 없을 때를 기준으로 함 )
    if( g_kGlobalValue.m_bIncompletePatch )
    {
        g_kGlobalValue.m_bIncompletePatch = false;
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INCOMPLETE_PATCH_MSG), 
            L"", KGCUIScene::GC_MBOX_USE_INCOMPLETE_PATCH_MSG, 0, 0,true, true );		
    }

    if( diks[DIK_LCONTROL] && g_kGlobalValue.IsAdmin() )
    {
        int iPlayerIndex = -1;
        for(int i=0;i<MAX_PLAYER_NUM;++i)
        {
            if( g_pkInput->IsUp( DIK_1 + i ) )
            {
                iPlayerIndex = i;
                break;
            }
        }

        if( -1 != iPlayerIndex )
        {
            if ( g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.bLive && g_MyD3D->Get_MyPlayer() != iPlayerIndex )
            {
                g_KDSound.Play( "74" );
                SiKGCRoomManager()->Kick_Player( iPlayerIndex );	
            }
        }
    }

    if ( m_bShowMissionUI )
    {
        g_kGlobalValue.m_kUserMission.SetShowMissionUI(true);
        m_bShowMissionUI = false;
    }
}

void KGCRoomSceneS6::OnEnterScene( void )
{
    for ( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
    {
        if (g_MyD3D->m_KGCOption.GetBasicEquipEnable())
            if (!g_MyD3D->MyPlayer[i]->IsLocalPlayer())
                g_MyD3D->MyPlayer[i]->UpdatePlayerResource();

        if ( g_MyD3D->MyPlayer[i]->m_pObject )
        {
            g_MyD3D->MyPlayer[i]->m_pObject->ResetMeshState();
            g_MyD3D->MyPlayer[i]->m_pObject->SetRender( true );
        }
    }

    m_pkRoomTitle->SetCountryGroup();
    m_pkRoomUserPanel->SetUserSlot(); 

    g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );

#if !defined( USE_COORDI_SHOP )
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 394.0f, 0.0f ) );
#else
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 502.0f, 0.0f ) );
#endif

    g_pkUIScene->m_pkInventoryFullMsg->ToggleRender( true );    

    if( g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_GAME &&
        g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_MY_INFO_FROM_ROOM &&
		g_MyD3D->m_pStateMachine->GetPrevGameState() != GS_CHAR_SELECT_FROM_ROOM )
    {
        if ( g_pkRoomChatBox != NULL ) 
        {
            g_pkRoomChatBox->ClearChatBox( GCPairPlaySystem::PPSCT_ROOM );
            g_pkRoomChatBox->InitColorButton();
        }
    }

    // 획득가능한 에픽미션이 있는지 확인
    if ( false == SiKGCMissionManager()->CheckEpicMission( SiKGCRoomManager()->GetGameMode() ) )
    {
        // 획득가능한 일반미션 확인
        DWORD dwNormalMissionID = SiKGCMissionManager()->CheckNormalMission( SiKGCRoomManager()->GetGameMode() );
        m_pkRoomDungeonPanel->SetNormalMission(dwNormalMissionID);
    }
    else
        m_pkRoomDungeonPanel->SetNormalMission(0);

    UpdateGameData();
	SiKGCRoomManager()->WritePvPRoomOptions();
    g_pkUIScene->EnableMessageBox( true );
}

void KGCRoomSceneS6::OnLeaveScene( void )
{
    //m_pkRoomUserPanel->m_pkUserSlotPanel_4->RemovePvEGradeParticles();
    //m_pkRoomUserPanel->m_pkUserSlotPanel_6->RemovePvEGradeParticles();

    m_pkRoomUserPanel->ClearUserEnchant();

    if ( g_pkUIScene->IsRenderOn(KGCUIScene::GC_MBOX_ROOM_USER_INFO_POPUP) )
    {
        g_pkUIScene->m_pkRoomUserInfoPopup->OnClose();
    }

#ifdef CALENDAR_RECOMMAND
    g_pkUIScene->m_pkAttendanceInfoMsg->ToggleRender( false );
#endif

    if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
    {
        g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_WAIT;
        g_pkUIScene->EnableMessageBox( true );
    }    
}

void KGCRoomSceneS6::PostChildDraw()
{
}

void KGCRoomSceneS6::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER: 
        {
            g_KDSound.Play( "30" ); 
            return;
        }
    
    }

	//GCWND_MSG_MAP( m_pkRoomExitButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnExitRoom );
    GCWND_MSG_MAP( m_pkMyInfoButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnEnterMyInfo );
    GCWND_MSG_MAP( m_pkSkilltreeButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnSkillTree ); 
    
}

void KGCRoomSceneS6::OnSkillTree()
{
    g_MyD3D->OnSkillTreeButton();
}

void KGCRoomSceneS6::UpdateDungeonLevel()
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if( g_kGlobalValue.m_kUserInfo.bHost && !g_kGlobalValue.IsUnlock() )
        {
            SUserInfo* pHostUserInfo = &g_kGlobalValue.m_kUserInfo;
            
            if ( SiKGCRoomManager()->IsHellMode() ) 
            {
                SiKGCRoomManager()->SetSubGameMode( GC_SGM_DUNGEON_HELL );
            }
            else
            {
				EGCGameMode eGameMode = SiKGCRoomManager()->GetGameMode();
                int nEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( eGameMode, pHostUserInfo->mapStagePlayable );
                nEnableLevel += GC_SGM_DUNGEON_NORMAL;
                if( nEnableLevel < SiKGCRoomManager()->GetSubGameMode() )
                    SiKGCRoomManager()->SetSubGameMode( nEnableLevel );
            }                 
        }
    }
}

void KGCRoomSceneS6::UpdateGameData( bool bUpdateExtraModel )
{
    // 던전방 일때는 기존의 대전방 UI가 보이지 않는다
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        //  대전 패널 끈다.
        m_pkRoomMatchPanel->ToggleRender( false );

        // 방장이 바뀌거나 할 떄.. 레벨 세팅 내가 할 수 있는 최고레벨로 수정..
        UpdateDungeonLevel();

        //  던전 선택 버튼 갱신
        m_pkRoomDungeonPanel->ToggleRender( true );
        
        m_pkRoomUserPanel->UpdateGameData(bUpdateExtraModel);
        m_pkRoomDungeonPanel->UpdateGameData();
    }
    else // 대전방일 경우..
    {
        m_pkRoomDungeonPanel->ToggleRender( false );
        
        m_pkRoomMatchPanel->ToggleRender( true );
        
        m_pkRoomUserPanel->UpdateGameData(bUpdateExtraModel);
        m_pkRoomMatchPanel->UpdateGameData();
    }

    m_pkRoomTitle->SetPrimium();

    bool bShowWorldMap = g_pkUIScene->m_pkWorldMapContainer->IsRenderOn();

    m_pkMyInfoButton->Lock( m_pkRoomButtonPanel->IsAutoStart() || g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY || bShowWorldMap );
    m_pkRoomTitle->LockOptionBtn( !g_kGlobalValue.m_kUserInfo.bHost || g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) );
    m_pkRoomButtonPanel->UpdateGameData();
}

void KGCRoomSceneS6::SetRoomTitle( int nRoomNum, const std::wstring& strRoomName, const std::wstring& strRoomPassword )
{
    m_pkRoomTitle->SetRoomName( nRoomNum, strRoomName );
    m_pkRoomTitle->SetRoomPassword( strRoomPassword );
}

void KGCRoomSceneS6::OnEnterMyInfo()
{
    if( 0 == g_pkUIHelper->GetUILockTime( LOCK_ROOM ) )
    {
        g_MyD3D->m_pStateMachine->GoMyInfoFromRoom();

        //g_pkUIHelper->SetUILockTime( LOCK_ROOM , BUTTON_LOCK_FRAME );
        SetEnableStart( false );
    }
}

void KGCRoomSceneS6::OnExitRoom()
{
    // 정상적으로 방을 나감. (대회서버에선 RP깎아달라고 요청합니다.)
    bool bSubtractRP = false;
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) && g_MyD3D->GetPlayerNum() > 1 )
    {
        bSubtractRP = true;
    }

    // 2010-10-12, shmhlove, 레디 상태였다면 메시지박스와 유저상태를 변경합니다.
    if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
    {
        g_pkUIScene->ClearWaitMessageBox();
        g_pkUIScene->EnableMessageBox( true );
        g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_WAIT;
    }

    g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, bSubtractRP );
    SiKGCRoomManager()->SetPlayedHeroDungeon(false);
}

void KGCRoomSceneS6::ShowWorldMap( bool bShow )
{
    // 방에서 던전 선택.. 월드맵이 나온다..
    // 연출이 있었으면 좋겠음..=ㅅ =ㅋ
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( bShow );
    
    if( bShow )
    {
        g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
        g_pkRoomChatBox->m_pkEdit->Activate( false );
        g_pkRoomChatBox->ToggleRender( false );
        g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_OFF_GCMENU, true ) );
    }
    else
    {
        g_pkRoomChatBox->SetFocus();
        g_pkRoomChatBox->ToggleRender( true );
        g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_OFF_GCMENU, false ) );
    }

    m_pkRoomTitle->ToggleRender( !bShow );
    m_pkRoomUserPanel->ToggleRender( !bShow );
    m_pkRoomDungeonPanel->ToggleRender( !bShow );
    m_pkRoomButtonPanel->ToggleRender( !bShow );
//    m_pkRoomExitButton->ToggleRender( !bShow );
	m_pkMyInfoButton->ToggleRender(!bShow);
    m_pkMyInfoButton->Lock( bShow );
    m_pkSkilltreeButton->ToggleRender(!bShow);
    //m_pkSkilltreeButton->Lock( bShow );


}

void KGCRoomSceneS6::ChangeCurrentLevel( int iLevel, bool bRight )
{
    int fLevel = iLevel;

#ifdef __PANHO__
	if ( iLevel >= GC_DUNGEON_LEVEL_HELL ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_PANHO_MESSAGE_1), L"" );
		return;
	}
#endif

	if (!g_kGlobalValue.m_kUserInfo.bHost)
	{
		return;
	}

	if ( SiKGCWorldMapManager()->IsHeroDungeonMode(SiKGCRoomManager()->GetGameMode()) && SiKGCWorldMapManager()->GetGameMode() != GC_GM_QUEST98 )
	{
        if ( SiKGCWorldMapManager()->GetGameMode() == GC_GM_QUEST52 )
        {
            int iEnableLevel = 0;
            int iLimitLevel = 0;

            std::map<int, KDungeonUserInfo> mapStagePlayable;

            EGCGameMode eGameMode = SiKGCRoomManager()->GetStagePlayable( mapStagePlayable );

            iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel( eGameMode );
            iEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( eGameMode, mapStagePlayable );


            if (g_kGlobalValue.IsUnlock() || g_kGlobalValue.IsDungeonFreePass())
                iEnableLevel = iLimitLevel - 1;

            if (iEnableLevel >= iLimitLevel)
            {
                iEnableLevel = iLimitLevel - 1;
            }

            iEnableLevel = ( GC_SGM_DUNGEON_NORMAL + iEnableLevel );

            if ( bRight )
            {
                if ( ++fLevel > iEnableLevel )
                {
                    fLevel = GC_SGM_NORMAL;
                }
            }
            else
            {
                if ( --fLevel < GC_SGM_NORMAL )
                {
                    fLevel = iEnableLevel;
                }
            }
        }
        else
            return;
	}

    if( 0 == g_pkUIHelper->GetUILockTime( LOCK_ROOM ) )
    {
        SiKGCRoomManager()->ChangeDungeonGameMode( fLevel );
        
        UpdateGameData();

		if (!(SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo()))
		{
			KP2P::GetInstance()->Send_ChangeRoomInfoReq(&SiKGCRoomManager()->GetRoomInfo());
		}

        g_pkUIHelper->SetUILockTime( LOCK_ROOM , BUTTON_LOCK_FRAME );
    }
}

void KGCRoomSceneS6::SetRoomMemberPingInfo(std::vector<std::pair<DWORD,DWORD>>& vecRoomMemberPingInfo_ )
{
    m_pkRoomUserPanel->SetRoomMemberPingInfo(vecRoomMemberPingInfo_);
}

void KGCRoomSceneS6::SetLevelUpPlayer( const int i_ )
{
    m_pkRoomUserPanel->SetLevelUpPlayer(i_);
}

void KGCRoomSceneS6::InitLevelupEffect()
{
    m_pkRoomUserPanel->InitLevelupEffect();
}

void KGCRoomSceneS6::UpdateMissionData()
{
    m_pkRoomDungeonPanel->UpdateMissionData();
}

bool KGCRoomSceneS6::IsMaxPlayerNumSix()
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        int nMaxPlayerNum = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum( SiKGCRoomManager()->GetGameMode() );

        if ( nMaxPlayerNum > OLD_MAX_DUNGEON_PLAYER_NUM ) 
            return true;
        else
            return false;
    }
    else
    {
        return true;
    }

}