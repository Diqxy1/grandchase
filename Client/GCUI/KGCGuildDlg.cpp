#include "StdAfx.h"
#include "KGCGuildDlg.h"
#include "KGCGuildInfoDlg.h"
#include "KGCGuildMemberList.h"
#include "KGCGuildManageDlg.h"

IMPLEMENT_CLASSNAME( KGCGuildDlg );
IMPLEMENT_WND_FACTORY( KGCGuildDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildDlg, "gc_guilddlg" );

KGCGuildDlg::KGCGuildDlg(void)
:m_pkGuildInfoTabBtn(NULL)
,m_pkGuildInfoTabActive(NULL)
,m_pkGuildMemberTabBtn(NULL)
,m_pkGuildMemberTabActive(NULL)
,m_pkGuildManageTabBtn(NULL)
,m_pkGuildManageTabActive(NULL)
,m_pkCloseBtn(NULL)
,m_pkGuildMemberList(NULL)
,m_pkGuildInfoDlg(NULL)
,m_pkGuildManageDlg(NULL)
,m_pkGuildLevelTabBtn(NULL)
,m_pkGuildLevelTabActive(NULL)
,m_pkGuildLevelList(NULL)
{
    LINK_CONTROL("guild_info_tab_btn",			m_pkGuildInfoTabBtn );
    LINK_CONTROL("guild_info_tab_active",		m_pkGuildInfoTabActive );
    LINK_CONTROL("guild_member_tab_btn",		m_pkGuildMemberTabBtn );
    LINK_CONTROL("guild_member_tab_active",		m_pkGuildMemberTabActive );
	LINK_CONTROL("guild_manage_tab_btn",		m_pkGuildManageTabBtn );
	LINK_CONTROL("guild_manage_tab_active",		m_pkGuildManageTabActive );    
    LINK_CONTROL("guild_level_tab_btn",         m_pkGuildLevelTabBtn );
    LINK_CONTROL("guild_level_tab_active",		m_pkGuildLevelTabActive) ;
    
    LINK_CONTROL("close_btn" ,                  m_pkCloseBtn );
    LINK_CONTROL("guild_info_dlg",				m_pkGuildInfoDlg );

    LINK_CONTROL("guild_member_list",			m_pkGuildMemberList );
	LINK_CONTROL("guild_manage_dlg",			m_pkGuildManageDlg );
    LINK_CONTROL("guild_level_list",            m_pkGuildLevelList );
}

KGCGuildDlg::~KGCGuildDlg(void)
{
}

void KGCGuildDlg::OnCreate( void )
{	
    UpdateMemberList();
    m_pkGuildInfoTabBtn->InitState( true, true, this );
    m_pkGuildInfoTabActive->InitState( true );
    m_pkGuildMemberTabBtn->InitState( true, true, this );
    m_pkGuildMemberTabActive->InitState( false );
	m_pkGuildManageTabBtn->InitState( true, true, this );
	m_pkGuildManageTabActive->InitState( false );
    if ( m_pkGuildLevelTabBtn )
        m_pkGuildLevelTabBtn->InitState( true, true, this );
    if ( m_pkGuildLevelTabActive )
        m_pkGuildLevelTabActive->InitState( false );
    
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    m_pkGuildInfoDlg->InitState( true, true, this );
    m_pkGuildMemberList->InitState( false, true, this );
	m_pkGuildManageDlg->InitState( false, true, this );
    if ( m_pkGuildLevelList )
        m_pkGuildLevelList->InitState( true, true, this );
	SwapGuildTab(GUILD_INFO_TAB);       

}

void KGCGuildDlg::OnCreateComplete( void )
{
	if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_NORMAL )
	{
		m_pkGuildManageTabBtn->ToggleRender(false);
	}

#if defined( NATION_PHILIPPINE )
    m_pkGuildLevelTabBtn->ToggleRender( false );
    m_pkGuildLevelTabActive->ToggleRender( false );
    m_pkGuildLevelList->ToggleRender( false );
#endif
}

void KGCGuildDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkGuildInfoTabBtn,			KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildInfoDlg );
    GCWND_MSG_MAP( m_pkGuildMemberTabBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildMemberListDlg );
	GCWND_MSG_MAP( m_pkGuildManageTabBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildManageDlg );    
    GCWND_MSG_MAP( m_pkCloseBtn,				KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    if ( m_pkGuildLevelTabBtn )
        GCWND_MSG_MAP( m_pkGuildLevelTabBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildLevelDlg );
    
}

void KGCGuildDlg::OnDestroyComplete( void )
{
}

void KGCGuildDlg::FrameMoveInEnabledState( void )
{
}

void KGCGuildDlg::PostChildDraw( void )
{
}

void KGCGuildDlg::OnClose( void )
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCGuildDlg::OnGuildInfoDlg( void )
{
    g_KDSound.Play( "31" );
    SwapGuildTab(GUILD_INFO_TAB);
}

void KGCGuildDlg::OnGuildMemberListDlg( void )
{
	g_KDSound.Play( "31" );
	SwapGuildTab(GUILD_MEMBER_TAB);
}

void KGCGuildDlg::OnGuildManageDlg( void )
{
	g_KDSound.Play( "31" );
	SwapGuildTab(GUILD_MANAGE_TAB);
}

void KGCGuildDlg::OnGuildLevelDlg( void )
{
    g_KDSound.Play( "31" );
    SwapGuildTab(GUILD_LEVEL_TAB);
}

void KGCGuildDlg::SwapGuildTab( EGuildTab eTab_ )
{
	m_pkGuildInfoDlg->ToggleRender(false);
	m_pkGuildMemberList->ToggleRender(false);
	m_pkGuildManageDlg->ToggleRender(false);
    if ( m_pkGuildLevelList )
        m_pkGuildLevelList->ToggleRender(false);

	m_pkGuildInfoTabActive->ToggleRender(false);
	m_pkGuildMemberTabActive->ToggleRender(false);
	m_pkGuildManageTabActive->ToggleRender(false);
    if ( m_pkGuildLevelTabActive )
        m_pkGuildLevelTabActive->ToggleRender( false );

	switch( eTab_ )
	{
	case GUILD_MEMBER_TAB:
		{
			m_pkGuildMemberList->ToggleRender(true);
			m_pkGuildMemberTabActive->ToggleRender(true);			
			m_pkGuildMemberList->SetGuildUserInfo();
			m_pkGuildMemberList->SetCursorBoxOffset(D3DXVECTOR2(0.f, 0.0f));
		}		
		break;
	case GUILD_MANAGE_TAB:
		{
			m_pkGuildManageDlg->ToggleRender(true);
			m_pkGuildManageTabActive->ToggleRender(true);
			if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER )
				m_pkGuildManageDlg->OnSelectDlg(KGCGuildManageDlg::BTN_MEMBER_MANAGE);
			else
			{
				m_pkGuildManageDlg->OnSelectDlg(KGCGuildManageDlg::BTN_JOIN_MANAGE);
			}
		}
		break;
    case GUILD_LEVEL_TAB:
        {
            m_pkGuildLevelList->ToggleRender(true);
            m_pkGuildLevelTabActive->ToggleRender( true );
        }
        break;
	case GUILD_INFO_TAB:    
	default:
		{
			m_pkGuildInfoDlg->ToggleRender(true);
			m_pkGuildInfoDlg->SetGuildInfo();
			m_pkGuildInfoTabActive->ToggleRender(true);
		}		
		break;
	}
}

void KGCGuildDlg::UpdateMemberList()
{
	m_pkGuildMemberList->SetGuildUserInfo();
}

void KGCGuildDlg::UpdateGuildMangeDlg( int iType_ )
{
	m_pkGuildManageDlg->OnSelectDlg((KGCGuildManageDlg::EGUILD_MANAGE_BTN)iType_);
}

void KGCGuildDlg::SetGuildNotice( std::wstring strGuildNotice, int iType_)
{
	m_pkGuildInfoDlg->SetGuildNotice( strGuildNotice, iType_ );
}

void KGCGuildDlg::SetClickSubMenu( bool bClick_ )
{
	m_pkGuildMemberList->SetClickSubMenu(bClick_ );
}

// 모든 정보를 업데이한다.
void KGCGuildDlg::UpdateAll()
{
	UpdateMemberList();
	m_pkGuildInfoDlg->SetGuildInfo();
	m_pkGuildInfoDlg->UpdateGuildMark();
#if defined ( GUILD_RENEW_S4 )
    m_pkGuildInfoDlg->UpdateGuildLevel( (int)g_kGlobalValue.m_kGuildInfo.m_dwPoint );
#endif
	m_pkGuildManageDlg->UpdateAllManageDlg();
}