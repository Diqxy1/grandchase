#include "StdAfx.h"
#include "KGCGuildLobbyDlg.h"
#include "KGCRoomList.h"

IMPLEMENT_CLASSNAME( KGCGuildLobbyDlg );
IMPLEMENT_WND_FACTORY( KGCGuildLobbyDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildLobbyDlg, "gc_guild_lobby_dlg" );

KGCGuildLobbyDlg::KGCGuildLobbyDlg(void)
:m_pkCloseBtn( NULL )
,m_pkGuildLobbyChat( NULL )
,m_pkGuildRoomList( NULL )
,m_iBeforeFilterSetting( GC_GMC_INVALID )
{
    LINK_CONTROL("close_btn", m_pkCloseBtn );
    LINK_CONTROL("guildlobby_chat_box" , m_pkGuildLobbyChat );
    LINK_CONTROL("guild_room_list" , m_pkGuildRoomList );
}

KGCGuildLobbyDlg::~KGCGuildLobbyDlg(void)
{
}


void KGCGuildLobbyDlg::OnCreate( void )
{
    m_pkGuildRoomList->InitState( true, true, this );
    m_pkGuildRoomList->InitRoomList( KGCRoomList::RLT_GUILDMATCH );
    m_pkCloseBtn->InitState( true, true, this);
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    m_pkGuildLobbyChat->InitState( true, true, this );
    SiKGCRoomManager()->SetChannelRoomList( true );
    SiKGCRoomManager()->UpdateShowGameModeCategory( false, CT_GUILD );

    SiKGCRoomListManager()->ClearRoomFilter();
    SiKGCRoomListManager()->SetFilterEnable( KGCRoomListManager::ECATEGORY_GUILD );

    m_iBeforeFilterSetting = SiKGCRoomListManager()->GetRoomFilter();
    SiKGCRoomListManager()->SetRoomFilter( GC_GMC_INVALID );
}

void KGCGuildLobbyDlg::OnCreateComplete( void )
{
    if( g_pkGuildLobbyChatBox != NULL ) 
    {   
        g_pkGuildLobbyChatBox->Clear();
        g_pkGuildLobbyChatBox->ToggleRender( true );
        g_pkGuildLobbyChatBox->SetFocus();
    }

    m_pkGuildLobbyChat->SetFilteringListInfo( KGCChatBox::CT_GUILD,     true );
    m_pkGuildLobbyChat->SetFilteringListInfo( KGCChatBox::CT_WHISPER,   false );
    m_pkGuildLobbyChat->SetFilteringListInfo( KGCChatBox::CT_GENERAL,   false );
    m_pkGuildLobbyChat->ChangeChatTypePos( KGCChatBox::CT_GUILD, true );

    m_pkGuildRoomList->UpdateRoomList();
}

void KGCGuildLobbyDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn,  KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCGuildLobbyDlg::OnDestroyComplete( void )
{
    g_pkGuildLobbyChatBox->ToggleRender( false );
    g_pkGuildLobbyChatBox->m_pkEdit->Activate( false );
}

void KGCGuildLobbyDlg::FrameMoveInEnabledState( void )
{
    if( g_pkGuildLobbyChatBox != NULL )
        g_pkGuildLobbyChatBox->SetFocus();
}

void KGCGuildLobbyDlg::PostChildDraw( void )
{
}

void KGCGuildLobbyDlg::OnClose()
{
    g_KDSound.Play( "31" );
	if( SiGCSquare()->GetSquareType() != KSquareInfo::ST_GUILD )
	{
		SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
		SiKGCRoomManager()->SetChannelRoomList( false );
		SiKGCRoomListManager()->SendRoomListReq(false);
	}

    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
    {
        g_MyD3D->m_pStateMachine->GoWorldMap();
    }
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );

    //룸필터를 갱신한다.
    SiKGCRoomListManager()->ClearRoomFilter();
    for( int i = 0 ; i < GC_GMC_COUNT ; ++i )
    {
        EChannelType eChannel = (EChannelType)SiKGCChannelManager()->GetCurrentChannelType();
        if( SiKGCRoomManager()->IsAvailableCategory( eChannel, static_cast<EGCGameModeCategory>(i) ) == false )
            continue;

        KGCRoomListManager::EFILTER_CATEGORY eCategory;
        eCategory = SiKGCRoomListManager()->GetFilterEnumFromCategory( (EGCGameModeCategory)i );
        SiKGCRoomListManager()->SetFilterEnable( eCategory );
    }
}

void KGCGuildLobbyDlg::OnDestroy()
{
    SiKGCRoomListManager()->SetRoomFilter( m_iBeforeFilterSetting );
}