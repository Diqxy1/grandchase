#include "stdafx.h"
#include "GCRoomButtonPanelS6.h"
#include "KGCInfinityDungeonManager.h"
#include "GCStateRoom.h"

IMPLEMENT_CLASSNAME( KGCRoomButtonPanelS6 );
IMPLEMENT_WND_FACTORY( KGCRoomButtonPanelS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomButtonPanelS6, "gc_room_button_panel_s6" );

const int nAUTO_START_TIME = 10;

KGCRoomButtonPanelS6::KGCRoomButtonPanelS6( void )
: m_pkRoomReadyButton( NULL ) 
, m_pkRoomInviteButton( NULL ) 
, m_pkRoomStartButton( NULL )
, m_pkRoomExitButton(NULL)
, m_bExitRoom( false )
{
    LINK_CONTROL( "room_ready_button",  m_pkRoomReadyButton );
    LINK_CONTROL( "room_invite_button", m_pkRoomInviteButton );
    LINK_CONTROL( "room_start_button",  m_pkRoomStartButton );
    LINK_CONTROL( "room_exit_button",  m_pkRoomExitButton );
}

KGCRoomButtonPanelS6::~KGCRoomButtonPanelS6( void )
{
}

void KGCRoomButtonPanelS6::OnCreate( void )
{
    m_pkRoomReadyButton->InitState( true, true, this );
    m_pkRoomInviteButton->InitState( true, true, this );
    m_pkRoomStartButton->InitState( true, true, this );
    m_pkRoomExitButton->InitState( true, true, this );

    m_iStartDelay = 0;
    m_bExitRoom = false;
}

void KGCRoomButtonPanelS6::SetStartDelay( int iDelay )
{
    if( m_bAutoStart == true )
        SetAutoTimer();
    //else if( All_Players_Ready() == true ) // ÀÌ°Å.. ¹®Á¦ »ý±æ ¿äÁö°¡ ÀÖÀ»µí?
    else if( g_kGlobalValue.m_kUserInfo.bHost ) // ¹æÀåÀÏ¶§¸¸ Àû¿ë½ÃÅ°¸é µÇÁö ¾Ê³ª? ¿ÀÅä½ºÅ¸Æ® ¾Æ´Ï¸é..
        m_iStartDelay = iDelay;
    else
        m_iStartDelay = 0;
}

void KGCRoomButtonPanelS6::SetAutoTimer()
{	
    if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() )
    {
        m_iStartDelay = 0;
        return;
    }

    // ÆÀÀÎ¿øÀÌ ¸Â¾Æ¾ß Å¸ÀÌ¸Ó°¡ Èå¸¨´Ï´Ù.
    int iBlue = 0;
    int iRed = 0;
    GetTeamCount( iBlue, iRed );
    if( iBlue == iRed )
        m_iStartDelay = GC_FPS_LIMIT * nAUTO_START_TIME;
    else
        m_iStartDelay = 0;
}

void KGCRoomButtonPanelS6::OnInitialize( void )
{
    if( m_bAutoStart == true )
    {
        m_pkRoomInviteButton->SetSelfInputCheck( false );
        m_pkRoomStartButton->SetSelfInputCheck( false );
        m_pkRoomReadyButton->SetSelfInputCheck( false );

        m_pkRoomInviteButton->SetWindowColor( 0x77ffffff );
        m_pkRoomStartButton->SetWindowColor( 0x77ffffff );
        m_pkRoomReadyButton->SetWindowColor( 0x77ffffff );
    }
    else
    {
        m_pkRoomInviteButton->SetSelfInputCheck( true );
        m_pkRoomStartButton->SetSelfInputCheck( true );
        m_pkRoomReadyButton->SetSelfInputCheck( true );

        m_pkRoomInviteButton->SetWindowColor( 0xffffffff );
        m_pkRoomStartButton->SetWindowColor( 0xffffffff );
        m_pkRoomReadyButton->SetWindowColor( 0xffffffff );
    }

    
    
    m_pkRoomInviteButton->SetSelfInputCheck( true );
    m_pkRoomInviteButton->SetWindowColor( 0xffffffff );

    m_pkRoomExitButton->SetSelfInputCheck( true );
    m_pkRoomExitButton->SetWindowColor( 0xffffffff );
    //m_pkRoomInviteButton->SetToolTip( g_pkStrLoader->GetString( STR_ID_INVITE ) ); ÇÊ¿ä¾øÀ½.

    UpdateGameData();
}

bool KGCRoomButtonPanelS6::IsBlockedCharActive( void )
{
    if ( SiKGCRoomManager()->GetRoomInfo().m_saAllowedChars[ g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType ] == false )
    {
        if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
            OnReady();
         return true;
    }
    return false;
}

bool KGCRoomButtonPanelS6::IsBlockedRankActive( void )
{
    if ( SiKGCRoomManager()->GetRoomInfo().m_saAllowedRankings[ g_MyD3D->m_TempPlayer.GetCurrentChar().kELOUserData.m_ucGrade ] == false )
    {
        if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
            OnReady();
         return true;
    }
    return false;
}

void KGCRoomButtonPanelS6::FrameMoveInEnabledState(void)
{
    if ( SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) )
        return;
    if ( m_bAutoStart == false )
    {
        bool bIsPrivateDungeon = SiKGCWorldMapManager()->IsPrivateDungeon( SiKGCRoomManager()->GetGameMode() );
        m_pkRoomStartButton->SetSelfInputCheck( g_kGlobalValue.m_kUserInfo.bHost && 0 == m_iStartDelay );
        m_pkRoomReadyButton->SetSelfInputCheck( !g_kGlobalValue.m_kUserInfo.bHost );
        m_pkRoomInviteButton->SetSelfInputCheck( g_kGlobalValue.m_kUserInfo.bHost && !bIsPrivateDungeon );
    }
    else
    {
        m_pkRoomStartButton->SetSelfInputCheck( false );
        m_pkRoomReadyButton->SetSelfInputCheck( false );
        m_pkRoomInviteButton->SetSelfInputCheck( false );
    }
    if ( m_iStartDelay == 0 )
    {
        m_pkRoomStartButton->SetWindowColor( D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff ) );
    }
    else if ( m_iStartDelay > GC_FPS_LIMIT )
    {
        m_pkRoomStartButton->SetWindowColor( D3DCOLOR_ARGB( 0xff, 0x80, 0x80, 0x80 ) );
    }
    else
    {
        BYTE a = static_cast<BYTE>( 128.0f + 127.0f * ( static_cast<float>(GC_FPS_LIMIT) - m_iStartDelay ) / static_cast<float>( GC_FPS_LIMIT ) );
        m_pkRoomStartButton->SetWindowColor( D3DCOLOR_ARGB( a, 0x80, 0x80, 0x80 ) );
    }
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME || g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING )
    {
        m_pkRoomReadyButton->Lock( false );
        m_pkRoomStartButton->Lock( false );
        return;
    }
    std::pair<int, int> pairClearInfo = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo( SiKGCRoomManager()->GetGameMode() );
    bool IsHeroTicketUsed = g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeonPlayTicket( SiKGCRoomManager()->GetGameMode() );
    if ( SiKGCWorldMapManager()->IsHeroDungeonMode( SiKGCRoomManager()->GetGameMode() ) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && ( pairClearInfo.first >= pairClearInfo.second && !IsHeroTicketUsed ) )
    {
        if ( g_kGlobalValue.m_kUserInfo.bHost )
        {
            m_pkRoomReadyButton->ToggleRender( false );
            m_pkRoomReadyButton->SetSelfInputCheck( false );
        }
        else
        {
            m_pkRoomStartButton->ToggleRender( false );
            m_pkRoomStartButton->SetSelfInputCheck( false );
        }
        m_pkRoomReadyButton->Lock( true );
        m_pkRoomStartButton->Lock( true );
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
        if ( m_pkRoomReadyButton->CheckPosInWindowBound( vMousePos ) )
        {
            if ( g_pkInput->BtnPressed( KInput::MBLEFT ) )
            {
                std::wstringstream stm;
                std::wstringstream strm;
                std::wstring strColorKey = L"#cff0000";
                std::wstring strColorKeyEnd = L"#cX";
                strm << strColorKey << g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR0 ) << strColorKeyEnd;
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, stm.str(), g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4 ) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            }
        }
    }
    else if ( SiKGCWorldMapManager()->IsEventDungeon( SiKGCRoomManager()->GetGameMode() ) && !g_kGlobalValue.IsOpenDayEventDungeon( SiKGCRoomManager()->GetGameMode() ) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if (g_kGlobalValue.m_kUserInfo.bHost)
        {
            m_pkRoomReadyButton->ToggleRender( false );
            m_pkRoomReadyButton->SetSelfInputCheck( false );
        }
        else
        {
            m_pkRoomStartButton->ToggleRender(false);
            m_pkRoomStartButton->SetSelfInputCheck(false);
        }
        m_pkRoomReadyButton->Lock( true );
        m_pkRoomStartButton->Lock( true );
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
        if ( m_pkRoomReadyButton->CheckPosInWindowBound( vMousePos ) )
        {
            if ( g_pkInput->BtnPressed( KInput::MBLEFT ) )
            {
                std::wstringstream stm;
                std::wstringstream strm;
                std::wstring strColorKey = L"#cff0000";
                std::wstring strColorKeyEnd = L"#cX";
                strm << strColorKey << g_pkStrLoader->GetString( STR_ID_EVENT_DUNGEON_ERROR_02 ) << strColorKeyEnd;
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, stm.str(), g_pkStrLoader->GetString( STR_ID_EVENT_DUNGEON_ERROR_01 ) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            }
        }
    }
    else if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && IsBlockedCharActive() == true )
    {
        if (g_kGlobalValue.m_kUserInfo.bHost)
        {
            m_pkRoomReadyButton->ToggleRender(false);
            m_pkRoomReadyButton->SetSelfInputCheck(false);
        }
        else
        {
            m_pkRoomStartButton->ToggleRender(false);
            m_pkRoomStartButton->SetSelfInputCheck(false);
        }
        m_pkRoomReadyButton->Lock(true);
        m_pkRoomStartButton->Lock(true);
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
        if (m_pkRoomReadyButton->CheckPosInWindowBound(vMousePos))
        {
            if (g_pkInput->BtnPressed(KInput::MBLEFT))
            {
                std::wstringstream stm;
                std::wstringstream strm;
                std::wstring strColorKey = L"#cff0000";
                std::wstring strColorKeyEnd = L"#cX";
                strm << strColorKey << g_pkStrLoader->GetString(STR_ID_START_PVP_ROOM_WITH_BLOCKED_CHAR_FELLOWS) << strColorKeyEnd;
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }
        }
    }
    else if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && IsBlockedRankActive() == true)
    {
        if (g_kGlobalValue.m_kUserInfo.bHost)
        {
            m_pkRoomReadyButton->ToggleRender(false);
            m_pkRoomReadyButton->SetSelfInputCheck(false);
        }
        else
        {
            m_pkRoomStartButton->ToggleRender(false);
            m_pkRoomStartButton->SetSelfInputCheck(false);
        }
        m_pkRoomReadyButton->Lock(true);
        m_pkRoomStartButton->Lock(true);
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
        if (m_pkRoomReadyButton->CheckPosInWindowBound(vMousePos))
        {
            if (g_pkInput->BtnPressed(KInput::MBLEFT))
            {
                std::wstringstream stm;
                std::wstringstream strm;
                std::wstring strColorKey = L"#cff0000";
                std::wstring strColorKeyEnd = L"#cX";
                strm << strColorKey << g_pkStrLoader->GetString(STR_ID_START_PVP_ROOM_WITH_BLOCKED_RANK_FELLOWS) << strColorKeyEnd;
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }
        }
    }
    else
    {
        if ( g_kGlobalValue.m_kUserInfo.bHost )
        {
            m_pkRoomReadyButton->ToggleRender( false );
            m_pkRoomReadyButton->SetSelfInputCheck( false );
        }
        else
        {
            m_pkRoomStartButton->ToggleRender( false );
            m_pkRoomStartButton->SetSelfInputCheck( false );
        }
        if ( m_pkRoomReadyButton->IsLocked() )
        {
            m_pkRoomReadyButton->SetHotKey( DIK_F9 );
#ifdef USE_JOYSTICK
            m_pkRoomReadyButton->SetHotJoystickKey( EJK_KEY_START );
#endif
            m_pkRoomReadyButton->Lock( false );
        }
        if ( m_pkRoomStartButton->IsLocked() )
        {
            m_pkRoomStartButton->SetHotKey( DIK_F9 );
#ifdef USE_JOYSTICK
            m_pkRoomStartButton->SetHotJoystickKey( EJK_KEY_START );
#endif
            m_pkRoomStartButton->Lock( false );
        }
    }
}

void KGCRoomButtonPanelS6::PostChildDraw( void )
{
	if ( SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) )
		return;
    //#ifdef __PATH__
    if ( m_iStartDelay > GC_FPS_LIMIT )	
    {
        D3DXVECTOR2 vPos = m_pkRoomStartButton->GetFixedWindowLocalPos();


        DWORD dwNumberWidth = 0 ;
        DWORD dwNumberHeight = 0 ;

        g_pkUIMgr->GetTypeNumberInfo("big_num",dwNumberWidth,dwNumberHeight);

        float fAddtionPosX = static_cast<float>((m_pkRoomStartButton->GetWidth()-dwNumberWidth)/2) ;
        float fAddtionPosY = static_cast<float>((m_pkRoomStartButton->GetHeight()-dwNumberHeight)/2) ;

        vPos.x += fAddtionPosX;
        vPos.y += fAddtionPosY;

        vPos.x *=m_fWindowScaleX;
        vPos.y *=m_fWindowScaleY;

        g_pkUIMgr->RenderNumber( "big_num", GetRelocatedWindowPos() + vPos, m_iStartDelay / GC_FPS_LIMIT);
    }
    //#endif
}

void KGCRoomButtonPanelS6::ActionPerformed( const KActionEvent& event )
{
	if ( SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) )
		return;

    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER: 
        {
            g_KDSound.Play( "30" ); 
            return;
        }
    case KD3DWnd::D3DWE_BUTTON_CLICK: 
        {
            if( event.m_pWnd == m_pkRoomExitButton )
                m_bExitRoom = true;
        }
        break;
    }

    GCWND_MSG_MAP( m_pkRoomReadyButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnReady );
    GCWND_MSG_MAP( m_pkRoomInviteButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnInvite );
    GCWND_MSG_MAP( m_pkRoomStartButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnStart );
}

void KGCRoomButtonPanelS6::CheckGameStart()
{
    if(!g_kGlobalValue.m_kUserInfo.bHost)
        return;

#if !defined( NATION_KOREA )
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
    {
        if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
            return;
    }
#endif

#ifdef __PATH__
    if ( m_iStartDelay > 55 ) 
        return;
#endif

    bool bReady = false;

    if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if ( !g_MyD3D->MyPlayer[i]->IsLocalPlayer() && g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_READY) )
            {
                bReady = true;
                break;
            }
        }
    }

    else if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if ( !g_MyD3D->MyPlayer[i]->IsLocalPlayer() && g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_READY) && !g_MyD3D->IsMyTeam( i ) )
            {
                bReady = true;
                break;
            }
        }
    }
    else
    {
        bReady = CheckAllReady();		    
    }

    if ( !bReady )
    {
        return;
    }

    if( CheckPlayerNum() == false )
        return;

    if( CheckTeam() == false )
        return;

    if( CheckQuest() == false )
        return;

    GameStart();
}
bool KGCRoomButtonPanelS6::GetExitBtnState()
{
    return m_bExitRoom;
}
void KGCRoomButtonPanelS6::SetExitBtnState( bool bExitState )
{
    m_bExitRoom = bExitState;
}

void KGCRoomButtonPanelS6::OnStart()
{
    g_KDSound.Play( "31" );

    if ( !g_pkUIScene->m_pkRoomSceneS6->GetEnableStart() )
        return;

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) 
    {
        if ( SiKGCWorldMapManager()->IsHeroDungeonMode( SiKGCRoomManager()->GetGameMode() ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) 
        {
            if ( !SiKGCWorldMapManager()->IsHeroDungeonGradeCheck( SiKGCRoomManager()->GetGameMode(), g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR9 ),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                return;
            }
        }
        if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN )
        {
            if ( SiKGCInfinityDungeonManager()->GetInfinityDungeon( SiKGCRoomManager()->GetGameMode() )->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) )
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR9),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return;
            }
        }
    }

    g_pkRoomChatBox->m_pkEdit->Activate(false);
    CheckGameStart();
}

void KGCRoomButtonPanelS6::OnInvite()
{
    g_KDSound.Play( "31" );

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) )
        return;

    if( SiKGCWorldMapManager()->IsRelayHeroDungeon( SiKGCRoomManager()->GetGameMode() ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_INVITE ), L"" );
        return;
    }

	if( SiKGCWorldMapManager()->IsRaidHeroDungeon(SiKGCRoomManager()->GetGameMode()) )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_INVITE ), L"" );
		return;
	}

	if (SiKGCWorldMapManager()->IsEventDungeon(SiKGCRoomManager()->GetGameMode()))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INFINITY_DUNGEON_INVITE), L"");
		return;
	}

    if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SEASON2_INVITE_REQ );
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_HOST_ONLY_INVITE1 ), L"" );
    }
    return;
}

void KGCRoomButtonPanelS6::OnReady()
{
    g_KDSound.Play( "31" );

    if ( g_kGlobalValue.m_kUserInfo.bHost ) 
        return;

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) 
    {
        if ( SiKGCWorldMapManager()->IsHeroDungeonMode( SiKGCRoomManager()->GetGameMode() ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) 
        {            
            if ( !SiKGCWorldMapManager()->IsHeroDungeonGradeCheck( SiKGCRoomManager()->GetGameMode(), g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR9 ),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                return;
            }
        }
    }

    if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN )
    {
        if ( SiKGCInfinityDungeonManager()->GetInfinityDungeon(SiKGCRoomManager()->GetGameMode() )->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR9 ),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        }
    }

    if( 0 == g_pkUIHelper->GetUILockTime( LOCK_ROOM ) )
    {
        g_kGlobalValue.m_kUserInfo.nUserState = ( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY ? GC_RUS_WAIT : GC_RUS_READY );
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( g_kGlobalValue.m_kUserInfo, 
            KChangeRoomUserInfo::RUI_INFO_STATE, g_MyD3D->Get_MyPlayer() );
        Result_Change_Room_UserInfo = INT_MAX;
        g_MyD3D->WaitForServerAck( Result_Change_Room_UserInfo, INT_MAX, 3000, -1 );
        if ( Result_Change_Room_UserInfo == 0 )
        {
            // ÁØºñµÈ »óÅÂ¿¡¼± ¸Þ½ÃÁö¹Ú½º ¾È¶ç¿ò
            g_pkUIScene->EnableMessageBox( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY );

            // Ready ÇÏ±â
            m_pkRoomInviteButton->SetSelfInputCheck( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY );
        }
        else
        {
            g_kGlobalValue.m_kUserInfo.nUserState = ( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY ? GC_RUS_WAIT : GC_RUS_READY );
        }

        g_pkUIHelper->SetUILockTime( LOCK_ROOM , BUTTON_LOCK_FRAME );
    }
}

bool KGCRoomButtonPanelS6::CheckAllReady()
{    
    if( SiKGCRoomManager()->All_Players_Ready() == false )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NOT_START_NOT_READY1 ),
            g_pkStrLoader->GetString( STR_ID_NOT_START_NOT_READY2 ) );

        return false;
    }

    return true;
}

bool KGCRoomButtonPanelS6::CheckPlayerNum()
{
#if defined (NATION_BRAZIL) || defined (NATION_TAIWAN)
    if ( SiKGCRoomManager()->GetGameMode() ==  GC_GM_QUEST58 &&
        SiKGCRoomManager()->GetRoomInfo().usUsers < 1 )
    {
        return false;
    }
#endif
    // ´ëÀåÀü
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN &&
        SiKGCRoomManager()->GetRoomInfo().usUsers < 4 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NOT_START_LESSTHEN_4 ),
            L"" );
        return false;
    }
    else if( SiKGCRoomManager()->GetRoomInfo().usUsers > 1 )
        return true;

    // ¿î¿µÀÚ´Â È¥ÀÚ¼­ ÇÃ·¹ÀÌ °¡´ÉÇÔ
    if( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
    {
        return true;
    }

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
        return true;

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT )
        return true;

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
        g_pkStrLoader->GetString( STR_ID_NOT_START_ONLY_ONE1 ),
        L"" );

    return false;	
}

void KGCRoomButtonPanelS6::GetTeamCount( int& iBlue, int& iRed )
{
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() ) 
                iBlue++;
            else 
                iRed++;
        }
    }
}

bool KGCRoomButtonPanelS6::CheckTeam()
{
    if( GC_GMT_TEAM_FIGHT != SiKGCRoomManager()->GetGameModeType() )
        return true;

    if ( GC_GMC_DEATHMATCH == SiKGCRoomManager()->GetGameModeCategory() )
        return true;


    //ÆÀ¹Ù²Ù°í ¹Ù·Î ½ÃÀÛÇÏ´Â°Å ¸·À½..
    if( g_MyD3D->Get_MyPlayer() < MAX_PLAYER_NUM )
    {
        if( g_kGlobalValue.m_kUserInfo.IsBlueTeam() != g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.IsBlueTeam() )
            return false;
    }		

    int Blue = 0, Red = 0;
    GetTeamCount( Blue, Red );

    if( Blue != Red )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NOT_EQUAL_TEAM_NUM1 ),
            L"" );

        return false;
    }

    // 1 Vs 1, 2 Vs 2, 3 Vs 3 µî µµÅ¸ÅÛÇÃ¸´ ½ºÅ©¸³Æ®¿¡¼­ ¼³Á¤ÇÑ ½ÃÀÛ°¡´É ÆÀ¿ø ¼ö·Î Á¦¾î
    if ( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
    {
        SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
        if( pDotaTemplate )
        {
            if( Blue < pDotaTemplate->iGameStartLimitUser && Red < pDotaTemplate->iGameStartLimitUser )
            {
                // ¸î ´ë ¸î ÀÌÇÏ·Î´Â °ÔÀÓ½ÃÀÛÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù.
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetReplacedString( STR_ID_GAME_START_LIMIT, "ii", pDotaTemplate->iGameStartLimitUser, pDotaTemplate->iGameStartLimitUser),
                    L"" );
                return false;
            }
        }
    }

    if( GC_GMC_GUILD_BATTLE == SiKGCRoomManager()->GetGameModeCategory() )
    {
        int FirstGild=-1;
        int SecondGild=-1;									
        int Num_Player=0;
        for(int i=0 ; i<MAX_PLAYER_NUM ; ++i)
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                bool Found = false;
                ++Num_Player;

                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID == FirstGild )
                    Found=true;
                else if(g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID==SecondGild) 
                    Found=true;

                if(!Found)
                {
                    if(FirstGild == -1) 
                        FirstGild=g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID;
                    else if(SecondGild==-1) 
                        SecondGild=g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID;
                    else
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                            g_pkStrLoader->GetString( STR_ID_ONETEAM_OTHER_GUILD1 ),
                            g_pkStrLoader->GetString( STR_ID_ONETEAM_OTHER_GUILD2 ) );

                        return false;
                    }
                }
            }
        }

        if( Num_Player < 2 )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_ONETEAM_AT_LEAST_2_1 ),
                g_pkStrLoader->GetString( STR_ID_ONETEAM_AT_LEAST_2_2 ) );

            return false;
        }
    }

    return true;
}

bool KGCRoomButtonPanelS6::CheckQuest()
{
    if( GC_GMC_DUNGEON != SiKGCRoomManager()->GetGameModeCategory() )
        return true;

    if( IsEnableCurrentGameMode() == false )
        return false;
    return true;
}

void KGCRoomButtonPanelS6::GameStart()
{
    // ¼­¹ö¿¡ °ÔÀÓ ½ÃÀÛ ¿äÃ»À» ÇÔ.
    Result_StartGame = INT_MAX;

    std::vector< std::pair<char,char> > vecPromotionCount;
    g_MyD3D->GetStatPromotionCount( vecPromotionCount );

    // ÀüÀåÀÏ °æ¿ì ¹æÀåÀÌ ½ÃÀÛ ½ºÅ×ÀÌÁö Á¤ÇØ¼­ ÁØ´Ù. 
    EGCGameMode eCurrentGameMode = SiKGCRoomManager()->GetGameMode();
    if( SiKGCWorldMapManager()->IsInfinityDungeon( eCurrentGameMode ) ) { 
        if( g_kGlobalValue.m_kUserInfo.bHost ) { 
            int iNumStage = SiKGCInfinityDungeonManager()->GetStageCount( eCurrentGameMode );
            if( iNumStage == 0 ) { 
                KP2P::GetInstance()->Send_StartGameReq( vecPromotionCount );
            }
            else { 
                int iStage = rand() % iNumStage;
                KP2P::GetInstance()->Send_StartGameReq( vecPromotionCount, 0, 0, 0, iStage );
            }
        }
    }
    else { 
        KP2P::GetInstance()->Send_StartGameReq( vecPromotionCount );
    }

    g_MyD3D->WaitForServerAck(Result_StartGame, INT_MAX, 5000, TIME_OUT_VALUE );


    if( g_kGlobalValue.m_bCheckChangedKOM )
        return;

    std::wstringstream stm;
    std::wstringstream strm;
    std::wstring strColorKey    = L"#cff0000";
    std::wstring strColorKeyEnd = L"#cX";     
    stm << strColorKey << g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_START_GAME_ERROR ) << strColorKeyEnd;

    // 051102. jseop. ¼­¹ö¿¡¼­ ¿Â ÀÀ´ä¿¡ µû¸¥ Ã³¸®.
    switch( Result_StartGame )
    {
    case 1:
        if ( GC_GM_QUEST6 == SiKGCRoomManager()->GetGameMode() ) // ¹æÀåÀÌ°í ÆÄÀÌ³Î Äù½ºÆ®¶ó¸é ÁöµµÁ¶°¢ 1/2/3/4 ¸ðµÎ ±ð´Â´Ù.
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
        Result_StartGame = 0;
        break;
    case 15:
        Result_StartGame = 0;
        break;
    case 16:
        strColorKey = L"#cffff00";
        strm << strColorKey << L"\n"
            << g_kGlobalValue.m_kHeroDungeonInfo.GetInitTime()
            << strColorKeyEnd;
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,stm.str(), 
            g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4 ) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
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
}

bool KGCRoomButtonPanelS6::IsEnableCurrentGameMode()
{
#if !defined(SERVICE)
    return true;
#endif 
    if( g_pStateRoom->IsEnableCurrentGameMode() )
        return true;

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, 
        g_pkStrLoader->GetString( STR_ID_MONSTER_DUNGEON_NO_LEVEL1 ),
        g_pkStrLoader->GetString( STR_ID_MONSTER_DUNGEON_NO_LEVEL2 ) );

    return false;
}

void KGCRoomButtonPanelS6::DecreaseStartDelay()
{
    // ¹æÀåÀÌ ¸ðµÎ°¡ ·¹µðÇÑ »óÈ²¿¡¼­ °©ÀÚ±â ³»¿ëÀ» º¯°æÇÒ ¼ö ÀÖ´Â Æí¹ýÀ» 
    // ¸·±â À§ÇØ¼­ Ä«¿îÆ®¸¦ ¼¾´Ù.
    if( m_iStartDelay > 0 ) 
        --m_iStartDelay;

    if( g_kGlobalValue.m_kUserInfo.bHost && m_bAutoStart == true && m_iStartDelay == 1 )
    {
        GameStart();
        return;
    }
}

void KGCRoomButtonPanelS6::UpdateGameData()
{
    //¹æÀåÀÌ¸é ½ºÅ¸Æ® ¹öÆ° Ç®°í ¤Ñ ±× ÀÌ¿Ü¿¡´Â ÇÃ·¹ÀÌÁßÀÎ ¹æÀÌ¸é ½ºÅ¸Æ® Ç¬´Ù
    m_pkRoomStartButton->ToggleRender( g_kGlobalValue.m_kUserInfo.bHost );
    
    
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
         SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        for (int i = 0 ; i < MAX_PLAYER_NUM; i++)
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState == GC_RUS_PLAYING )
            {
                m_pkRoomStartButton->InitState( true, true, this);
                break;
            }
        }   
    }

    m_pkRoomReadyButton->InitState( !m_pkRoomStartButton->IsRenderOn(),!m_pkRoomStartButton->IsRenderOn(), this );	

}
