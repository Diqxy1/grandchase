#include "stdafx.h"
#include ".\gcsinglemenu.h"
//

//


IMPLEMENT_CLASSNAME( KGCSingleMenu );
IMPLEMENT_WND_FACTORY( KGCSingleMenu );
IMPLEMENT_WND_FACTORY_NAME( KGCSingleMenu, "gc_fairplay_report_menu" );

KGCSingleMenu::KGCSingleMenu(void)
{
    m_pkReport = NULL;

    LINK_CONTROL( "report_static", m_pkReport );
    
}

KGCSingleMenu::~KGCSingleMenu(void)
{
}

void KGCSingleMenu::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        event.m_pWnd->SetFontColor( 0xffffff00 );
    else if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
        event.m_pWnd->SetFontColor( 0xffffffff );
    else if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_UP 
        || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkReport )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FAIRPLAY_REPORT, g_kGlobalValue.m_kUserInfo.strNickName, m_strBadGuy );
        }
        ToggleRender( false );
    }
}

void KGCSingleMenu::OnCreate( void )
{
    m_pkReport->InitState( true, true, this );
    m_pkReport->SetText( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ) );
}

void KGCSingleMenu::SetBadGuy( std::wstring strNickName_ )
{
    m_strBadGuy = strNickName_;
}