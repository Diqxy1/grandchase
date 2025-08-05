#include "stdafx.h"
#include "GCAgitListComboMain.h"

IMPLEMENT_CLASSNAME( KGCAgitListComboMain );
IMPLEMENT_WND_FACTORY( KGCAgitListComboMain );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitListComboMain, "gc_agit_combolist_main" );

KGCAgitListComboMain::KGCAgitListComboMain( void )
{
    m_pkDefaultCombo = NULL;
    m_pkActiveCombo = NULL;
    m_pkBtnPopUp = NULL;
    m_pkStaticMain = NULL;
    LINK_CONTROL( "combo", m_pkDefaultCombo );
    LINK_CONTROL( "activecombo", m_pkActiveCombo );
    LINK_CONTROL( "btnPopup", m_pkBtnPopUp );
    LINK_CONTROL( "staticMain", m_pkStaticMain );
}

KGCAgitListComboMain::~KGCAgitListComboMain( void )
{
}

void KGCAgitListComboMain::OnCreate( void )
{
    m_pkDefaultCombo->InitState( true, false, NULL );
    m_pkActiveCombo->InitState( false, false, NULL );
    m_pkBtnPopUp->InitState( true, true, this );
    m_pkStaticMain->InitState( true, false, NULL );
    m_pkStaticMain->SetAlign( DT_RIGHT );
}

void KGCAgitListComboMain::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnPopUp,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClick ); 
}

void KGCAgitListComboMain::SetMainStatic( std::wstring& strText_ )
{
    m_pkStaticMain->SetText( strText_ );
}

void KGCAgitListComboMain::Clear( void )
{
    m_pkStaticMain->SetText( L"" );
}

void KGCAgitListComboMain::OnClick( void )
{
    SpeakToActionListener( KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK) );
}

void KGCAgitListComboMain::SetActive( bool bActive_ )
{
    m_pkDefaultCombo->ToggleRender( !bActive_ );
    m_pkActiveCombo->ToggleRender( bActive_ );
}