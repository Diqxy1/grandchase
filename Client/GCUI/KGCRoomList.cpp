#include "stdafx.h"
//
#include "KGCRoomList.h"

#include "KGCRoomBar.h"
//

//
#include "MyD3D.h"
#include "VersionDef.h"
#include "KGCRoomManager.h"
#include "KGCRoomFilterBox.h"
#include "GCStateMachine.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCRoomList );
IMPLEMENT_WND_FACTORY( KGCRoomList );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomList, "gc_room_list" );

KGCRoomList::KGCRoomList( void )
{
    m_pkCreateRoomButton    = NULL;
    m_pkQuickJoinButton     = NULL;
    m_pkWaitRoomButton      = NULL;
    m_pkAllRoomButton       = NULL;

    m_pkPrevButton          = NULL;
    m_pkNextButton          = NULL;

    m_pkPageInfo            = NULL;
    m_pkRoomFilterBox       = NULL;

	m_pkFilterBtn			= NULL;
	m_pkFilterOnBtn			= NULL;

    m_iRoombarNum           = RLT_MATCH;

    for( int i = 0; i < NUM_ROOM_LIST; ++i )
        m_apkRoomBar[i]     = NULL;

    for( int i = 0; i < 4; ++i )
        m_apkFilterLevelButton[i] = NULL;

    m_pkCloseButton = NULL;

    LINK_CONTROL( "create_room",        m_pkCreateRoomButton );
    LINK_CONTROL( "quick_join",         m_pkQuickJoinButton );
    LINK_CONTROL( "wait_room",          m_pkWaitRoomButton );
    LINK_CONTROL( "all_room",           m_pkAllRoomButton );

    LINK_CONTROL( "room_prev",          m_pkPrevButton );
    LINK_CONTROL( "room_next",          m_pkNextButton );

    LINK_CONTROL( "page_info",          m_pkPageInfo );
    LINK_CONTROL( "room_filter_box",    m_pkRoomFilterBox );

    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < NUM_ROOM_LIST; ++i )
    {
        sprintf( strWndName, "room_bar%02d", i );
        LINK_CONTROL( strWndName,   m_apkRoomBar[i] );
    }

    //LINK_CONTROL( "room_filter_box",    m_apkFilterLevelButton[0] );
    //LINK_CONTROL( "room_filter_box",    m_apkFilterLevelButton[1] );
    //LINK_CONTROL( "room_filter_box",    m_apkFilterLevelButton[2] );
    //LINK_CONTROL( "room_filter_box",    m_apkFilterLevelButton[3] );

    LINK_CONTROL( "filter_btn",			m_pkFilterBtn );
	LINK_CONTROL( "filter_on_btn",		m_pkFilterOnBtn );

    LINK_CONTROL( "close_button",       m_pkCloseButton );

    m_iSelectRoom = -1;
	m_iPrevMaxPage = -1;
	m_iPrevPage = -1;
}

KGCRoomList::~KGCRoomList(void)
{
}

void KGCRoomList::TurnOnRoomFilterBox( bool m_bUse)
{
    //m_pkRoomFilterBox->ToggleRender( m_bUse );

    for( int i = 0; i < NUM_ROOM_LIST; ++i )
    {
        if ( !m_apkRoomBar[i] == NULL)
            m_apkRoomBar[i]->TurnOnCategoryBox( m_bUse);
    }
}

void KGCRoomList::OnCreate()
{
    bool bWaitRoom = SiKGCRoomListManager()->GetWaitRoom();   

    m_pkCreateRoomButton->InitState( true, true, this );
    m_pkQuickJoinButton->InitState( true, true, this );
    m_pkWaitRoomButton->InitState( !bWaitRoom, true, this );
    m_pkAllRoomButton->InitState( bWaitRoom, true, this );

    m_pkPrevButton->InitState( true, true, this );
    m_pkNextButton->InitState( true, true, this );

    for( int i = 0; i < NUM_ROOM_LIST; ++i )
    {
        if(m_apkRoomBar[i])
            m_apkRoomBar[i]->InitState( false, true, this );
    }
    if( m_pkRoomFilterBox )
        m_pkRoomFilterBox->InitState( true, true, this );

    m_pkCloseButton->InitState( true, true, this );
    m_pkCloseButton->SetHotKey( DIK_ESCAPE );

    m_pkPageInfo->InitState( true );
    m_pkPageInfo->SetAlign( DT_CENTER );
    m_pkPageInfo->SetFontOutline( true );

	KRoomOptions options;
	bool bFilterOn = !(options == SiKGCRoomListManager()->GetRoomListFilterOptions());

	m_pkFilterBtn->InitState( !bFilterOn, true, this );
	m_pkFilterBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomList::OnFilterBtnClick);
	m_pkFilterOnBtn->InitState( bFilterOn, true, this );
	m_pkFilterOnBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomList::OnFilterBtnClick);

    m_iSelectRoom = -1;
	m_iPrevMaxPage = -1;
	m_iPrevPage = -1;
}

void KGCRoomList::FrameMoveInEnabledState()
{
    // 매 프레임 부르지만 일정시간이 지나야 패킷을 보낸다( 딜레이가 있음 )
    SiKGCRoomListManager()->SendRoomListReq();

    for ( int i = 0; i < m_iRoombarNum; ++i )
    {
        if ( m_apkRoomBar[i]->IsRenderOn() )
            m_apkRoomBar[i]->SetPremium();
    }   
}

void KGCRoomList::ActionPerformed( const KActionEvent& event )
{
    bool bRoomList = false;
    for( int i = 0; i < m_iRoombarNum; ++i )
    {
        if( event.m_pWnd == m_apkRoomBar[i] )
        {
            bRoomList = true;
            break;
        }
    }

    if( !bRoomList )
    {
        switch( event.m_dwCode )
        {
            case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
            case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); return;
        }
    }

    GCWND_MSG_MAP( m_pkCreateRoomButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnCreateRoom );
    GCWND_MSG_MAP( m_pkQuickJoinButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnQuickJoin );
    GCWND_MSG_MAP( m_pkPrevButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevPage );
    GCWND_MSG_MAP( m_pkNextButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnNextPage );
    GCWND_MSG_MAP( m_pkCloseButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkWaitRoomButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnWaitRoom );
    GCWND_MSG_MAP( m_pkAllRoomButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnAllRoom );
    if( m_pkRoomFilterBox )
        GCWND_MSG_MAP( m_pkRoomFilterBox, KD3DWnd::D3DWE_BUTTON_CLICK, OnRoomFilterBox );

    for( int i = 0; i < m_iRoombarNum; ++i )
    {
        if( event.m_pWnd == m_apkRoomBar[i] &&
            m_apkRoomBar[i]->IsRenderOn() )
        {
            switch( event.m_dwCode )
            {
                case KD3DWnd::D3DWE_CURSOR_ENTER:
                {
                    g_KDSound.Play( "30" );
                    break;
                }
                case KD3DWnd::D3DWE_BUTTON_DOWN:
                {
                    g_KDSound.Play( "31" );
                    m_iSelectRoom = i;
                    break;
                }
                case KD3DWnd::D3DWE_BUTTON_CLICK:
                {
                    if( m_iSelectRoom == i /*&& false == g_pkUIScene->IsMsgBoxModal()*/ )
                    {
                        KSimpleRoomInfo kRoomInfo = m_apkRoomBar[i]->GetRoomInfo();

                        // 입장 티켓조건이 걸린 던전인지 확인
                        if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
                        {
                            if( KGCRoomManager::ENTER_IMPOSSIBLE == SiKGCRoomManager()->CheckTicketToDungeonEnter( true, kRoomInfo.m_nGameMode ) )
                                return;
                        }

                        if( (EGCGameModeCategory)kRoomInfo.m_cGameCategory == GC_GMC_DEATHMATCH ||
                            (EGCGameModeCategory)kRoomInfo.m_cGameCategory == GC_GMC_FATAL_DEATHMATCH )
                        {
                            if ( kRoomInfo.m_bPlaying )
                            {
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_ROOM1 ),
                                    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_ROOM2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                                break;
                            }
                        }

                        if ( !SiKGCWorldMapManager()->IsEnableJoinRoomCheck( static_cast<EGCGameMode>(kRoomInfo.m_nGameMode), kRoomInfo.m_nDifficulty, kRoomInfo.m_bClearLastDifficult ) ) 
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_DIFF_LEVEL ),
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
							return;
                        }


                        SiKGCRoomManager()->GetRoomInfo().usRoomID = kRoomInfo.m_usRoomID;

                        if( kRoomInfo.m_iP2PVersion == CLIENT_P2P_VER ) // P2P 버전이 일치하는 방일 경우
                        {
                            if( kRoomInfo.m_bPublic )
                            {
                                SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_NORMAL, 
                                            SiKGCRoomManager()->GetRoomInfo().usRoomID,
                                            L"", SiKGCRoomManager()->IsChannelRoomList() );
                            }
                            else
                            {
                                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PASSWORD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            }
                        }
                        else if ( kRoomInfo.m_iP2PVersion < CLIENT_P2P_VER ) // P2P 버전이 자신보다 작은 방일 경우
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                g_pkStrLoader->GetString( STR_ID_ROOM_P2P_LESS_THAN1 ),
                                g_pkStrLoader->GetString( STR_ID_ROOM_P2P_LESS_THAN2 ),
                                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        }
                        else // if( kRoomInfo.m_iP2PVersion > CLIENT_P2P_VER ) // P2P 버전이 자신보다 큰 방일 경우
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                g_pkStrLoader->GetString( STR_ID_ROOM_P2P_MORE_THAN1 ),
                                g_pkStrLoader->GetString( STR_ID_ROOM_P2P_MORE_THAN2 ),
                                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        }
                    }
                    else
                        m_iSelectRoom = -1;

                    break;
                }
            }
            return;
        }
    }
}

void KGCRoomList::OnCreateRoom()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CREATE_ROOM_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
}

void KGCRoomList::OnQuickJoin()
{
    if( FAILED( SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_QUICK, -1, L"", SiKGCRoomManager()->IsChannelRoomList() ) ) )
    {
        KRoomOptions options;
        if (FAILED(SiKGCRoomManager()->CreateRoom(KGCRoomManager::m_vecRandomRoomNameList[rand() % KGCRoomManager::m_vecRandomRoomNameList.size()].c_str(), L"", MAX_PLAYER_NUM, true, options)))
        {
            g_MyD3D->m_pStateMachine->GoMatch();
        }
    }
}

void KGCRoomList::UpdateRoomList()
{
    std::vector<KSimpleRoomInfo>& vecRoomList = SiKGCRoomListManager()->GetRoomList();

    int iMaxPage = SiKGCRoomListManager()->GetMaxPage();
    int iCurrentPage = SiKGCRoomListManager()->GetCurrentPage();

    for( int i = 0; i < m_iRoombarNum; ++i )
    {
        //int iRoomIndex = i;

        if( i < (int)vecRoomList.size() )
        {
            m_apkRoomBar[i]->ToggleRender( true );
            m_apkRoomBar[i]->SetRoomInfo( vecRoomList[ i ] );
            m_apkRoomBar[i]->SetPremiumType( vecRoomList[ i ].m_kPremiumInfo.m_dwPremiumType );
            m_apkRoomBar[i]->SetPremium();
        }
        else
        {
            m_apkRoomBar[i]->ToggleRender( false );
        }
    }

    WCHAR strPageInfo[20];
    // iCurrentPage는 0부터 시작이라 보여줄땐 1 더해서 보여준다
    // iMaxPage는 0페이지면 기본 1페이지로 표시해 준다
    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ) // 월드맵일때는 공간이 부족해서 page글자는 뺀다
        swprintf( strPageInfo, 20, L"%d / %d", iCurrentPage + 1, ( iMaxPage == 0 ) ? 1 : iMaxPage );
    else
        swprintf( strPageInfo, 20, L"%d / %d %s", iCurrentPage + 1, ( iMaxPage == 0 ) ? 1 : iMaxPage, g_pkStrLoader->GetString( STR_ID_PAGE ).c_str() );
    m_pkPageInfo->SetText( strPageInfo );

    bool bEnablePrevButton = ( iCurrentPage > 0 );
    bool bEnableNextButton = ( iCurrentPage + 1 < iMaxPage );

	//if( iMaxPage != m_iPrevMaxPage || iCurrentPage != m_iPrevPage )
	{
		m_pkPrevButton->Lock( !bEnablePrevButton );
		m_pkNextButton->Lock( !bEnableNextButton );
		m_pkPrevButton->SetToolTip( bEnablePrevButton ? L"" : g_pkStrLoader->GetString( STR_ID_NOT_EXIST_PREV_PAGE ) );
		m_pkNextButton->SetToolTip( bEnableNextButton ? L"" : g_pkStrLoader->GetString( STR_ID_NOT_EXIST_NEXT_PAGE ) );
	}    

    if( m_pkRoomFilterBox )
        m_pkRoomFilterBox->Update();

    bool bWait = SiKGCRoomListManager()->GetWaitRoom();
    m_pkAllRoomButton->ToggleRender( bWait );
    m_pkWaitRoomButton->ToggleRender( !bWait );

	m_iPrevMaxPage = iMaxPage;
	m_iPrevPage = iCurrentPage;
}

void KGCRoomList::OnPrevPage()
{
    SiKGCRoomListManager()->SetCurrentPage( SiKGCRoomListManager()->GetCurrentPage() - 1 );
}

void KGCRoomList::OnNextPage()
{
    SiKGCRoomListManager()->SetCurrentPage( SiKGCRoomListManager()->GetCurrentPage() + 1 );
}

void KGCRoomList::OnRoomFilterBox()
{
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) && m_pkRoomFilterBox )
        m_pkRoomFilterBox->ShowList();
}

void KGCRoomList::OnClose()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP || RLT_GUILDMATCH == m_eRoomListType)
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCRoomList::OnDestroyComplete()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCRoomList::OnAllRoom()
{
    m_pkWaitRoomButton->ToggleRender( true );
    m_pkAllRoomButton->ToggleRender( false );
    SiKGCRoomListManager()->SetWaitRoom( false );
}

void KGCRoomList::OnWaitRoom()
{
    m_pkWaitRoomButton->ToggleRender( false );
    m_pkAllRoomButton->ToggleRender( true );
    SiKGCRoomListManager()->SetWaitRoom( true );
}

void KGCRoomList::InitRoomList( ROOM_LIST_TYPE eRooListType_ )
{
    m_eRoomListType = eRooListType_;
    
    ROOM_BAR_NUM( static_cast<int>(m_eRoomListType) );
    switch( m_eRoomListType )
    {
    case RLT_DUNGEON:
        m_iRoombarNum = static_cast<int>(RBN_DUNGEON);        
        break;
    case RLT_MATCH:
        m_iRoombarNum = static_cast<int>( RBN_MATCH );        
        break;
    case RLT_GUILDMATCH:
        m_iRoombarNum = static_cast<int>( RBN_GUILDMATCH );
        break;
    }
}

void KGCRoomList::OnFilterBtnClick()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_LIST_FILTER_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}