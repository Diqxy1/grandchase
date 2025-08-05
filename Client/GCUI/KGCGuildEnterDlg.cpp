#include "StdAfx.h"
#include "KGCGuildEnterDlg.h"

IMPLEMENT_CLASSNAME( KGCGuildEnterDlg );
IMPLEMENT_WND_FACTORY( KGCGuildEnterDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildEnterDlg, "gc_guild_enter_dlg" );

KGCGuildEnterDlg::KGCGuildEnterDlg(void)
:m_pkGuildInfoBtn(NULL)
,m_pkGuildISquareBtn(NULL)
,m_pkCloseBtn(NULL)
,m_pkGuildIWarBtn(NULL)
{
    LINK_CONTROL("guildinfo_btn",  m_pkGuildInfoBtn );
    LINK_CONTROL("guildwar_btn",  m_pkGuildIWarBtn );
    LINK_CONTROL("guildsquare_btn",  m_pkGuildISquareBtn );
    LINK_CONTROL("close_btn", m_pkCloseBtn );
}

KGCGuildEnterDlg::~KGCGuildEnterDlg(void)
{
}


void KGCGuildEnterDlg::OnCreate( void )
{
    if ( m_pkGuildInfoBtn )
        m_pkGuildInfoBtn->InitState( true, true, this );

    m_pkGuildIWarBtn->InitState( true, true, this );
    m_pkGuildISquareBtn->InitState( true, true, this );
    m_pkCloseBtn->InitState(true,true, this);
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
}

void KGCGuildEnterDlg::OnCreateComplete( void )
{
}

void KGCGuildEnterDlg::ActionPerformed( const KActionEvent& event )
{
    if ( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
        GCWND_MSG_MAP( m_pkGuildIWarBtn,    KD3DWnd::D3DWE_BUTTON_CLICK, OnClickGuildWarBtn );    
    }
    else {
        GCWND_MSG_MAP( m_pkGuildInfoBtn,    KD3DWnd::D3DWE_BUTTON_CLICK, OnClickGuildWarBtn );
    }
    if (!g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_FREE) && !g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)) {
        GCWND_MSG_MAP(m_pkGuildISquareBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickGuildquareBtn);
    }
    GCWND_MSG_MAP( m_pkCloseBtn,  KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCGuildEnterDlg::OnDestroyComplete( void )
{
}

void KGCGuildEnterDlg::FrameMoveInEnabledState( void )
{
    
}

void KGCGuildEnterDlg::PostChildDraw( void )
{
#if defined (GUILD_RENEW_S4)
    if (g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
        if ( m_pkGuildInfoBtn )
            m_pkGuildInfoBtn->ToggleRender( true );
            m_pkGuildIWarBtn->ToggleRender( false );
    }
    else
    {
        m_pkGuildInfoBtn->ToggleRender( false );
        m_pkGuildIWarBtn->ToggleRender( true );
    }
#endif
}

void KGCGuildEnterDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}
void KGCGuildEnterDlg::OnClickGuildWarBtn()
{
	OnClose();

#if defined ( GUILD_RENEW_S4 ) || defined( NATION_PHILIPPINE )
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_DLG);

    KP2P::GetInstance()->Send_GuildNoticeListReq();
    //if( g_kGlobalValue.m_mapNGuildUserInfo.empty() )
    //{
        KP2P::GetInstance()->Send_GuildMemberListReq();
    //}    
    KP2P::GetInstance()->Send_GuildPointReq();
#else
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_CONNECTION_MSG_TITLE),
        g_pkStrLoader->GetString(STR_ID_NEW_GUILD_BATTLE_COMMING_SOON), KGCUIScene::GC_MBOX_USE_NORMAL, 
        KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0 );
    return;

    SiKGCRoomListManager()->ClearRoomFilter();
    for( int i = 0 ; i < GC_GMC_COUNT ; ++i )
    {
        if( SiKGCRoomManager()->IsAvailableCategory( CT_GUILD, static_cast<EGCGameModeCategory>(i) ) == false )
            continue;

        KGCRoomListManager::EFILTER_CATEGORY eCategory;
        eCategory = SiKGCRoomListManager()->GetFilterEnumFromCategory( (EGCGameModeCategory)i );
        SiKGCRoomListManager()->SetFilterEnable( eCategory );
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_LOBBY_DLG );
#endif

}

void KGCGuildEnterDlg::OnClickGuildquareBtn()
{
    OnClose();
    if (!g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_FREE) && !g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)) {
        g_pkUIScene->m_pkSquareEnterWndS6->SetSquareKind(KSquareInfo::ST_GUILD);
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SQUARE_ENTER_S6);
    }
	
}