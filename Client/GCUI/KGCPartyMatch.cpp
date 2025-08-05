#include "StdAfx.h"
#include "gcui/KGCPartyMatch.h"

IMPLEMENT_CLASSNAME( KGCPartyMatch );
IMPLEMENT_WND_FACTORY( KGCPartyMatch );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyMatch, "gc_party_match_dlg" );

KGCPartyMatch::KGCPartyMatch(void)
:m_pkRoomNameStr ( NULL )
, m_pkPasswordStr ( NULL )
, m_pkTeamStr ( NULL )
, m_pkSurvivalStr ( NULL )
, m_pkTeamModeChkBox ( NULL )
, m_pkSurvModeChkBox ( NULL )
, m_pkMapSelectStr ( NULL )
, m_pkMapSelect ( NULL )
, m_pkConfirm ( NULL )
, m_pkCancel ( NULL )
, m_pkClose (NULL)
, m_pkRoomName ( NULL )
, m_pkPassword ( NULL )
, m_pkMapStr ( NULL )
, m_pkMapMenu ( NULL )
, m_pkItemModeBox ( NULL )
, m_pkMatchModeBox ( NULL )
, m_iSelectedModeIndex ( 0 )
, m_iSelectedSubMode ( 0 )
, m_pkTeamModeCheck(NULL)
, m_pkSurvModeCheck(NULL)
, m_pkModeSelectStr(NULL)
{
    m_mapSubGameModeNameID.clear();
    m_mapGameModeCategoryNameID.clear();
    m_mapValidGameCategory.clear();
    m_mapMapNameID.clear();

    m_eUIGameModeCategory   = GC_GMC_MATCH;
    m_nUISubGameMode        = GC_SGM_NORMAL;
    m_eUIGameMode = GC_GM_TEAM_MODE;

#if defined ( NATION_KOREA) || defined ( NATION_BRAZIL ) || defined( NATION_TAIWAN ) || defined( NATION_THAILAND )
    m_eUIGameStage = GC_GS_FOREST_OF_ELF_REVERS;
#else
	m_eUIGameStage = GC_GS_FOREST_OF_ELF;
#endif

    m_mapGameModeCategoryNameID[GC_GMC_MATCH]               = g_pkStrLoader->GetString( STR_ID_GMC_MATCH );
    m_mapGameModeCategoryNameID[GC_GMC_GUILD_BATTLE]        = g_pkStrLoader->GetString( STR_ID_GMC_GUILD_BATTLE );
    m_mapGameModeCategoryNameID[GC_GMC_DUNGEON]             = g_pkStrLoader->GetString( STR_ID_GMC_QUEST );
    m_mapGameModeCategoryNameID[GC_GMC_INDIGO]              = g_pkStrLoader->GetString( STR_ID_GMC_INDIGO );
    m_mapGameModeCategoryNameID[GC_GMC_TAG_MATCH]           = g_pkStrLoader->GetString( STR_ID_GMC_TAG_MATCH );
    m_mapGameModeCategoryNameID[GC_GMC_MONSTER_CRUSADER]    = g_pkStrLoader->GetString( STR_ID_GMC_MONSTER_CRUSADERS );
    m_mapGameModeCategoryNameID[GC_GMC_MONSTER_HUNT]        = g_pkStrLoader->GetString( STR_ID_GMC_MONSTER_HUNT );
    //m_mapGameModeCategoryNameID[GC_GMC_ANGELS_EGG]          = g_pkStrLoader->GetString( STR_ID_GMC_ANGELS_EGG );
    m_mapGameModeCategoryNameID[GC_GMC_ANGELS_EGG]          = g_pkStrLoader->GetString( STR_ID_GMC_HALLOWEEN_PUMPKIN );
    m_mapGameModeCategoryNameID[GC_GMC_DEATHMATCH]          = g_pkStrLoader->GetString( STR_ID_GMC_DEATHMATCH );
    m_mapGameModeCategoryNameID[GC_GMC_FATAL_DEATHMATCH]          = g_pkStrLoader->GetString( STR_ID_GMC_FATAL_DEATHMATCH );
    m_mapGameModeCategoryNameID[GC_GMC_CAPTAIN]             = g_pkStrLoader->GetString( STR_ID_GMC_CAPTAIN );
    m_mapGameModeCategoryNameID[GC_GMC_DOTA]               = g_pkStrLoader->GetString( STR_ID_GMC_DOTA );

    m_mapSubGameModeNameID[GC_SGM_NORMAL]                   = g_pkStrLoader->GetString( STR_ID_SGM_NORMAL );
    m_mapSubGameModeNameID[GC_SGM_NO_ITEM]                  = g_pkStrLoader->GetString( STR_ID_SGM_NO_ITEM );

#if defined ( NATION_KOREA ) || defined ( NATION_BRAZIL ) || defined( NATION_TAIWAN ) || defined( NATION_THAILAND )
    m_mapMapNameID[GC_GS_FOREST_OF_ELF_REVERS]  = STR_ID_GS_FOREST_OF_ELF_REVERS;
    m_mapMapNameID[GC_GS_VALLEY_OF_OATH_REVERS] = STR_ID_GS_VALLEY_OF_OATH_REVERS;
#endif 

    m_mapMapNameID[GC_GS_FOREST_OF_ELF]         = STR_ID_GS_FOREST_OF_ELF;
    m_mapMapNameID[GC_GS_SWAMP_OF_OBLIVION]     = STR_ID_GS_SWAMP_OF_OBLIVION;
    m_mapMapNameID[GC_GS_FLYING_SHIP]           = STR_ID_GS_FLYING_SHIP;
    m_mapMapNameID[GC_GS_VALLEY_OF_OATH]        = STR_ID_GS_VALLEY_OF_OATH;
    m_mapMapNameID[GC_GS_FOGOTTEN_CITY]         = STR_ID_GS_FOGOTTEN_CITY;
    //m_mapMapNameID[GC_GS_BABEL_OF_X_MAS]        = STR_ID_GS_BABEL_OF_X_MAS;
    m_mapMapNameID[GC_GS_TEMPLE_OF_FIRE]        = STR_ID_GS_TEMPLE_OF_FIRE;
    m_mapMapNameID[GC_GS_SHOOTING_FIELD]        = STR_ID_GS_SHOOTING_FIELD;
    m_mapMapNameID[GC_GS_HELL_BRIDGE]           = STR_ID_GS_HELL_BRIDGE;
    m_mapMapNameID[GC_GS_TEMPLE_OF_ORC]         = STR_ID_GS_TEMPLE_OF_ORC;
    m_mapMapNameID[GC_GS_GUILD]                 = STR_ID_GS_GUILD;
    m_mapMapNameID[GC_GS_CARRIBEACH]            = STR_ID_GS_CARRIBEACH;
    m_mapMapNameID[GC_GS_FOREST_OF_TRIAL]       = STR_ID_GS_TRIAL;
    m_mapMapNameID[GC_GS_GRAVEYARD_OF_DECEASED] = STR_ID_GS_GRAVEYARD_OF_DECEASED;

#if defined( NATION_BRAZIL )
    m_mapMapNameID[GC_GS_BAHIA]                 = STR_ID_GS_BAHIA;
    m_mapMapNameID[GC_GS_AMAZON_FOREST]                 = STR_ID_GS_AMAZON_FOREST;
    m_mapMapNameID[GC_GS_CASTLE_OF_RULE]                = STR_ID_GC_GS_CASTLE_OF_RULE;
#endif

#if defined(NATION_THAILAND) 
    m_mapMapNameID[GC_GS_MARKET_WATER]                 = STR_ID_GS_MARKET_WATER;
#endif
#if defined(NATION_PHILIPPINE) //|| defined(NATION_PHILIPPINE)
    m_mapMapNameID[GC_GS_RIZAL_PARK]                 = STR_ID_GS_RIZAL_PARK;
#endif

    //방제 패스워드
    LINK_CONTROL( "room_name_static", m_pkRoomNameStr );
    LINK_CONTROL( "password_static", m_pkPasswordStr );
    
    //팀모드 서바이벌 모드 스트링 표시
    LINK_CONTROL( "team_static", m_pkTeamStr );
    LINK_CONTROL( "survival_static", m_pkSurvivalStr );

    //체크박스
    LINK_CONTROL( "team_mode_check_box", m_pkTeamModeChkBox );
    LINK_CONTROL( "surv_mode_check_box", m_pkSurvModeChkBox );
    LINK_CONTROL( "team_mode_check", m_pkTeamModeCheck );
    LINK_CONTROL( "surv_mode_check", m_pkSurvModeCheck );
    
    //맵선택
    LINK_CONTROL( "map_select_static", m_pkMapSelectStr );
    //맵선택 팝업창 띄우는 버튼
    LINK_CONTROL( "map_select", m_pkMapSelect );
    //멀티 박스 메뉴
    LINK_CONTROL("popupbox_map" , m_pkMapMenu );
    
    //확인, 취소
    LINK_CONTROL("confirm", m_pkConfirm);
    LINK_CONTROL("cancel", m_pkCancel);
    LINK_CONTROL("close" , m_pkClose);

    //선택 또는 지정한 스트링 보여주는 곳들
    LINK_CONTROL("room_name", m_pkRoomName );
    LINK_CONTROL("password", m_pkPassword );
    LINK_CONTROL("static_map", m_pkMapStr );

    //Change_box
    LINK_CONTROL ("item_mode_change" , m_pkItemModeBox );
    LINK_CONTROL ("match_mode_change" , m_pkMatchModeBox );

    //모드 선택 str
    LINK_CONTROL("mode_select_static", m_pkModeSelectStr );
}

KGCPartyMatch::~KGCPartyMatch(void)
{
}

void KGCPartyMatch::OnCreate()
{
    m_pkRoomNameStr->InitState( true, true, this );
    m_pkRoomNameStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_ROOM_TITLE ) , true );
    m_pkRoomNameStr->SetToolTip(  g_pkStrLoader->GetString( STR_ID_ROOM_TITLE ) );
    m_pkRoomNameStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkPasswordStr->InitState( true, true, this );
    m_pkPasswordStr->SetText( g_pkStrLoader->GetString( STR_ID_PASSWORD ) );
    m_pkPasswordStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkTeamStr->InitState( true, true, this );
    m_pkTeamStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_TEAM ) , true );
    m_pkTeamStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_TEAM ) );
    m_pkTeamStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkSurvivalStr->InitState( true, true, this );
    m_pkSurvivalStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_SURVIVAL ) , true );
    m_pkSurvivalStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_SURVIVAL ) );
    m_pkSurvivalStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkTeamModeChkBox->InitState( true, true, this );
    m_pkTeamModeCheck->InitState( true, true, this );
    m_pkSurvModeChkBox->InitState( true, true, this );
    m_pkSurvModeCheck->InitState( false , true, this);

    m_pkMapSelectStr->InitState( true, true, this );
    m_pkMapSelectStr->SetForceWordWrapText( g_pkStrLoader->GetString ( STR_ID_SELECT_MAP ) , true );
    m_pkMapSelectStr->SetToolTip(  g_pkStrLoader->GetString( STR_ID_SELECT_MAP ) );
    m_pkMapSelectStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkMapSelect->InitState( true, true, this );
    m_pkConfirm->InitState( true, true, this );
    m_pkConfirm->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK , this ,&KGCPartyMatch::OnConfirm);
    
    m_pkCancel->InitState( true, true, this );
    m_pkClose->InitState( true, true, this );

    m_pkRoomName->InitState( true, true, this );
    m_pkRoomName->SetLimitText( MAX_EDIT_LEN, m_pkRoomName->GetWidth(), 12 );
    m_pkRoomName->SetFontColor(0xFFFFFFFF);
    m_pkRoomName->SetText( SiKGCRoomManager()->m_vecRandomRoomNameList[rand() % KGCRoomManager::m_vecRandomRoomNameList.size()].c_str() );

    m_pkPassword->InitState( true, true, this );
    m_pkPassword->SetLimitText( MAX_EDIT_LEN, m_pkRoomName->GetWidth(), 12 );
    m_pkPassword->SetFontColor(0xFFFFFFFF);

    m_pkMapStr->InitState( true, true, this );
    m_pkMapStr->SetText(L"");

    m_pkMapMenu->InitState( false, true, this );

    m_pkItemModeBox->SetSelfInputCheck( false );
    m_pkItemModeBox->AddActionListener( this );
    m_pkItemModeBox->SetText(  m_mapSubGameModeNameID[m_nUISubGameMode] );
    m_pkMatchModeBox->SetSelfInputCheck( false );
    m_pkMatchModeBox->AddActionListener( this );
    m_pkMatchModeBox->SetText( m_mapGameModeCategoryNameID[m_eUIGameModeCategory] );
    
    m_pkModeSelectStr->InitState(true, false, this);

    UpdateMapList();
}

void KGCPartyMatch::OnCreateComplete( void )
{

    m_iGameModeCategory =0;
    m_eUIGameModeCategory = GC_GMC_MATCH;
    m_nUISubGameMode = GC_SGM_NORMAL;;
    m_eUIGameMode = GC_GM_TEAM_MODE;

#if defined ( NATION_KOREA) || defined ( NATION_BRAZIL ) || defined( NATION_TAIWAN ) || defined( NATION_THAILAND )
	m_eUIGameStage = GC_GS_FOREST_OF_ELF_REVERS;
#else
	m_eUIGameStage = GC_GS_FOREST_OF_ELF;
#endif
    
    m_pkItemModeBox->SetText(  m_mapSubGameModeNameID[m_nUISubGameMode] );
    m_pkItemModeBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
    m_pkMatchModeBox->SetText( m_mapGameModeCategoryNameID[m_eUIGameModeCategory] );
    m_pkMatchModeBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
    m_pkPassword->SetText(L"");
    m_pkMapStr->SetForceWordWrapText( g_pkStrLoader->GetString( m_mapMapNameID[m_eUIGameStage]  ) , true);
    m_pkMapStr->SetToolTip( g_pkStrLoader->GetString( m_mapMapNameID[m_eUIGameStage] ) );

    m_pkModeSelectStr->SetForceWordWrapText( g_pkStrLoader->GetString(STR_ID_PARTY_DUNGEON_MODE_SELECT) , true );
    m_pkModeSelectStr->SetToolTip( g_pkStrLoader->GetString(STR_ID_PARTY_DUNGEON_MODE_SELECT) );
    m_pkModeSelectStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    SetValidGameCategory();

    m_pkCancel->SetHotKey( DIK_ESCAPE );
    m_pkCancel->DisconnectAllEventProcedure();
    m_pkCancel->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyMatch::OnCancel );
    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkClose->DisconnectAllEventProcedure();
    m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyMatch::OnCancel );
}

void KGCPartyMatch::SetValidGameCategory( void )
{
    m_mapValidGameCategory = SiKGCRoomManager()->GetChannelCategory();
}

void KGCPartyMatch::OnConfirm( void )
{
    g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
    bool bTemp = true;
    std::wstring wsTemp = m_pkMapStr->GetText();
    if ( wsTemp  == L"" )
        bTemp = false;
    
    if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE && bTemp)
    {
        g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
        this->Destroy();
        g_pkSquareOverlayUI->ExitSquare();
        g_MyD3D->SetMatchChannel(CT_UNLIMITED);
        
        g_KDSound.Play( "31" );
        
        std::wstring wRoomName = m_pkRoomName->GetText( false );
        std::wstring wRoomPass = m_pkPassword->GetText( true );

        HRESULT hr = SiKGCRoomManager()->CreateRoomParty( wRoomName ,wRoomPass,  static_cast<int>( m_eUIGameMode), 0, m_eUIGameModeCategory ,m_nUISubGameMode ,m_eUIGameStage , false );
    }
    else if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && bTemp )
    {
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
        {
            g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
            this->Destroy();
            //	말풍선
            g_pkAgitOverlayUI->DeleteAllSayBalloon();

            // 모션제거
            g_pGraphicsHelper->UnLoadPlayerActionMotion();
            g_pGraphicsHelper->UnLoadPlayerModel();

            // 엑스트라 메시정보 제거
            g_kGlobalValue.m_ExtraMesh.ClearMeshInfo();

            SiKGCAgitGameManager()->SaveAgitObject();
            SiKGCAgitGameManager()->ClearObject();
            SiKGCAgitGameManager()->ClearPlaceCharUID();
            g_pkAgitOverlayUI->OnExitAgit();

            if( SiKGCAgitGameManager()->GetRegisterState() )
                SiKGCAgitGameManager()->UnRegisterAgitList();

            // 현재 아지트 나가고 
            g_pStateGame->OnExitGame(); 
            g_pkUIScene->EndMessageBox();
			g_MyD3D->SetMatchChannel(CT_UNLIMITED);

            g_KDSound.Play( "31" );

            std::wstring wRoomName = m_pkRoomName->GetText( false );
            std::wstring wRoomPass = m_pkPassword->GetText( true );

            HRESULT hr = SiKGCRoomManager()->CreateRoomParty( wRoomName ,wRoomPass,  static_cast<int>( m_eUIGameMode), 0, m_eUIGameModeCategory ,m_nUISubGameMode ,m_eUIGameStage , false );
        }
    }
    else if (bTemp)
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString( STR_ID_PARTY_EMPTY_LIST) ,L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
    }
}

void KGCPartyMatch::ActionPerformed( const KActionEvent& event )
{
    if (event.m_pWnd == m_pkMapSelect && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnClickMapSelect();
    }

    else if( event.m_pWnd == m_pkMapMenu && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
    {
        if( GC_GMC_DOTA == m_eUIGameModeCategory )
        {
            m_pkMapStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_GS_DOTA_PARK ) , true);
            m_pkMapStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_GS_DOTA_PARK ) );
        }
        else
        {
            m_eUIGameStage   = static_cast<EGCGameStage> ( event.m_dwlParam);
            std::map< EGCGameStage, int >::iterator mit = m_mapMapNameID.find ( m_eUIGameStage );
            if( mit != m_mapMapNameID.end() )
            {
                m_pkMapStr->SetForceWordWrapText( g_pkStrLoader->GetString( mit->second ) , true);
                m_pkMapStr->SetToolTip( g_pkStrLoader->GetString( mit->second ) );
            }
        }
    }

    else if( event.m_pWnd == m_pkItemModeBox )
    {
        if( GC_GMC_DOTA != m_eUIGameModeCategory )
        {
            switch ( event.m_dwCode )
            {
            case KGCChangeBox::LEFT_BUTTON:
                {
                    ChangeItemMode( false );
                    break;
                }
            case KGCChangeBox::RIGHT_BUTTON:
                {
                    ChangeItemMode( true );
                    break;
                }
            }
        }
    }
    else if ( event.m_pWnd == m_pkMatchModeBox )
    {
        switch ( event.m_dwCode )
        {
        case KGCChangeBox::LEFT_BUTTON:
            {
                ChangeMatchMode( false );
                break;
            }
        case KGCChangeBox::RIGHT_BUTTON:
            {
                ChangeMatchMode( true );
                break;
            }
        }
    }
    
    else if( event.m_pWnd  == m_pkTeamModeCheck || event.m_pWnd  == m_pkTeamModeChkBox || event.m_pWnd == m_pkTeamStr )
    {
        if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
            g_KDSound.Play( "31" );
            m_pkTeamModeCheck->ToggleRender(true);
            m_pkSurvModeCheck->ToggleRender( false );
            if ( m_eUIGameModeCategory == GC_GMC_TAG_MATCH )
                m_eUIGameMode = GC_GM_TAG_TEAM ; 
            else if ( m_eUIGameModeCategory == GC_GMC_DEATHMATCH ) 
                m_eUIGameMode = GC_GM_DEATH_TEAM;
            else if ( m_eUIGameModeCategory == GC_GMC_FATAL_DEATHMATCH ) 
                m_eUIGameMode = GC_GM_FATAL_DEATH_TEAM;
            else
                m_eUIGameMode = GC_GM_TEAM_MODE;            
        }
    }
    else if( event.m_pWnd  == m_pkSurvModeCheck || event.m_pWnd  == m_pkSurvModeChkBox || event.m_pWnd == m_pkSurvivalStr )
    {
        if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
            g_KDSound.Play( "31" );
            if( GC_GMC_DOTA != m_eUIGameModeCategory )
            {
                m_pkSurvModeCheck->ToggleRender(true);
                m_pkTeamModeCheck->ToggleRender( false );
	            if ( m_eUIGameModeCategory == GC_GMC_TAG_MATCH )
	                m_eUIGameMode = GC_GM_TAG_SURVIVAL ; 
	            else if ( m_eUIGameModeCategory == GC_GMC_DEATHMATCH ) 
	                m_eUIGameMode = GC_GM_DEATH_SURVIVAL;
                else if ( m_eUIGameModeCategory == GC_GMC_FATAL_DEATHMATCH ) 
                    m_eUIGameMode = GC_GM_FATAL_DEATH_SURVIVAL;
	            else
	                m_eUIGameMode = GC_GM_SURVIVAL_MODE;
            }
        }
    }

}

void KGCPartyMatch::FrameMoveInEnabledState( void )
{

}

void KGCPartyMatch::PostChildDraw( void )
{
}

void KGCPartyMatch::OnCancel( void )
{
    this->Destroy();
    g_pkUIScene->EndMessageBox();

    g_KDSound.Play( "31" );
}

void KGCPartyMatch::OnClickMapSelect( void )
{
    if( m_pkMapMenu->IsRenderOn() )
        m_pkMapMenu->ToggleRender(false);
    else
        m_pkMapMenu->ToggleRender(true);

    //m_pkMapStr->SetText(L"");
    g_KDSound.Play( "31" );
}

void KGCPartyMatch::UpdateMapList( void )
{
    m_pkMapMenu->ClearMenu();

    if( GC_GMC_DOTA == m_eUIGameModeCategory )
    {
        m_pkMapMenu->AddMenu( GC_GMC_DOTA, g_pkStrLoader->GetString( STR_ID_GS_DOTA_PARK ) );
        m_pkMapStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_GS_DOTA_PARK ) , true);
        m_pkMapStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_GS_DOTA_PARK ) );
    }
    else
    {
        std::map < EGCGameStage , int > ::iterator mit = m_mapMapNameID.begin();
        for( ; mit != m_mapMapNameID.end() ; ++mit)
        {
            m_pkMapMenu->AddMenu( mit->first, g_pkStrLoader->GetString( mit->second ) );

            if( mit->first == m_eUIGameStage )
            {
                m_pkMapStr->SetForceWordWrapText( g_pkStrLoader->GetString( mit->second ) , true);
                m_pkMapStr->SetToolTip( g_pkStrLoader->GetString( mit->second ) );
            }
        }
    }
}

void KGCPartyMatch::ChangeItemMode ( bool bRightMove_ )
{
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {     
        SiKGCRoomManager()->ChangeSubGameMode( bRightMove_ );

        m_nUISubGameMode = SiKGCRoomManager()->GetSubGameMode();
        m_pkItemModeBox->SetText( m_mapSubGameModeNameID[m_nUISubGameMode] );
        m_pkItemModeBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

        g_KDSound.Play( "31" );
    }
}

void KGCPartyMatch::ChangeMatchMode( bool bRightMove_ )
{
    std::map<int , EGCGameModeCategory>::iterator mit = m_mapValidGameCategory.begin();

    if ( bRightMove_ )
    {
        if ( ++m_iGameModeCategory >= static_cast<int>( m_mapValidGameCategory.size()) ) {
            m_iGameModeCategory = 0;
        }
        m_pkMatchModeBox->SetText( m_mapGameModeCategoryNameID[m_mapValidGameCategory[m_iGameModeCategory]] );
        m_pkMatchModeBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_eUIGameModeCategory = m_mapValidGameCategory[m_iGameModeCategory]; 
        if ( m_eUIGameModeCategory == GC_GMC_TAG_MATCH )
            m_eUIGameMode = GC_GM_TAG_TEAM ; 
        else if ( m_eUIGameModeCategory == GC_GMC_DEATHMATCH ) 
            m_eUIGameMode = GC_GM_DEATH_TEAM;
        else if ( m_eUIGameModeCategory == GC_GMC_FATAL_DEATHMATCH ) 
            m_eUIGameMode = GC_GM_FATAL_DEATH_TEAM;
        else
            m_eUIGameMode = GC_GM_TEAM_MODE;
    }
    else
    {
        if ( --m_iGameModeCategory < 0 ) {
            m_iGameModeCategory = static_cast<int>( m_mapValidGameCategory.size() )- 1;
        }
        m_pkMatchModeBox->SetText( m_mapGameModeCategoryNameID[m_mapValidGameCategory[m_iGameModeCategory]] );
        m_pkMatchModeBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_eUIGameModeCategory = m_mapValidGameCategory[m_iGameModeCategory] ;
        if ( m_eUIGameModeCategory == GC_GMC_TAG_MATCH )
            m_eUIGameMode = GC_GM_TAG_SURVIVAL ; 
        else if ( m_eUIGameModeCategory == GC_GMC_DEATHMATCH ) 
            m_eUIGameMode = GC_GM_DEATH_SURVIVAL;
        else if ( m_eUIGameModeCategory == GC_GMC_FATAL_DEATHMATCH ) 
            m_eUIGameMode = GC_GM_FATAL_DEATH_SURVIVAL;
        else
            m_eUIGameMode = GC_GM_SURVIVAL_MODE;
    }
   
    if( GC_GMC_DOTA == m_eUIGameModeCategory ) 
    {
        // 점령전은 무조건 노템전으로 셋팅되야한다.
        if( 0 == m_nUISubGameMode )
        {
            ChangeItemMode( true );
        }

        // 점령전은 무조건 팀전으로 셋팅되야한다.
        m_pkTeamModeCheck->ToggleRender(true);
        m_pkSurvModeCheck->ToggleRender( false );
    }

    UpdateMapList();

    g_KDSound.Play( "31" );
}