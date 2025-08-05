#include "stdafx.h"
#include ".\kgccharloadwnd.h"
#include "KGCLoadingStateBar.h"
//


IMPLEMENT_CLASSNAME( KGCCharLoadWnd );
IMPLEMENT_WND_FACTORY( KGCCharLoadWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCharLoadWnd, "gc_char_load_wnd" );

KGCCharLoadWnd::KGCCharLoadWnd(void)
{
    m_pkPingRed         = NULL;
    m_pkPingGreen       = NULL;
    m_pkName            = NULL;
    m_dwUID             = 0;
    m_pkLoadingStateBar = NULL;

    LINK_CONTROL( "ping_red",               m_pkPingRed );
    LINK_CONTROL( "ping_green",             m_pkPingGreen );
    LINK_CONTROL( "name_static",            m_pkName );
    LINK_CONTROL( "loading_state_bar",      m_pkLoadingStateBar );

}

KGCCharLoadWnd::~KGCCharLoadWnd(void)
{
}

void KGCCharLoadWnd::OnCreate( void )
{
	if(m_pkLoadingStateBar != NULL)
	    m_pkLoadingStateBar->InitState(true);
}

void KGCCharLoadWnd::ActionPerformed( const KActionEvent& event )
{
}

void KGCCharLoadWnd::SetName( std::wstring& strName_, DWORD dwUID_, D3DCOLOR dwColor_ )
{
    m_pkName->SetFontColor( dwColor_ );
    m_pkName->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkName->SetAlign( DT_CENTER );
    m_pkName->SetText( strName_ );
    
    
    SetUID( dwUID_ );

    ToggleRender( true );
    SetLoadingComplete( false );
}
void KGCCharLoadWnd::SetLoadingComplete( bool bComplete_ )
{
    m_pkPingRed->ToggleRender( !bComplete_ );
    m_pkPingGreen->ToggleRender( bComplete_ );
}

void KGCCharLoadWnd::SetNickNameAlign( DWORD dwAlign )
{
	m_pkName->SetAlign( dwAlign );
}

void KGCCharLoadWnd::SetLoadingState(std::pair<DWORD, int> prLoadingState_ )
{
	if(m_pkLoadingStateBar != NULL)
		m_pkLoadingStateBar->SetLoadingState(prLoadingState_.first, prLoadingState_.second);
}

void KGCCharLoadWnd::InitLoadingStateBar()
{
	if(m_pkLoadingStateBar != NULL)
		m_pkLoadingStateBar->SetLoadingState( m_dwUID, 1 );
}