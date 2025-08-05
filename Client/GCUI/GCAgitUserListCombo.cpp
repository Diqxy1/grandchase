#include "stdafx.h"
#include "GCAgitUserListCombo.h"

#include "GCAgitListComboMain.h"
#include "GCAgitListComboPopup.h"

IMPLEMENT_CLASSNAME( KGCAgitUserListCombo );
IMPLEMENT_WND_FACTORY( KGCAgitUserListCombo );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitUserListCombo, "gc_agit_user_listcombo" );

KGCAgitUserListCombo::KGCAgitUserListCombo( void )
{
    m_pkComboListMain = NULL;
    m_pkComboListPopup = NULL;
    LINK_CONTROL( "comboListMain", m_pkComboListMain );
    LINK_CONTROL( "comboListPopup", m_pkComboListPopup );
}

KGCAgitUserListCombo::~KGCAgitUserListCombo( void )
{
}

void KGCAgitUserListCombo::OnCreate( void )
{
    m_pkComboListMain->InitState( true, true, this );
    m_pkComboListPopup->InitState( false, true, this );
}

void KGCAgitUserListCombo::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkComboListMain,   KD3DWnd::D3DWE_BUTTON_CLICK,    TogglePopup );
}

void KGCAgitUserListCombo::TogglePopup( void )
{
	m_pkComboListPopup->ToggleRender( !m_pkComboListPopup->IsRenderOn() );
}

void KGCAgitUserListCombo::SetUserList( UCHAR ucMaxUser_, std::map< DWORD, std::wstring >& mapInfo_ )
{   
    std::wstringstream stm;
    stm << static_cast<int>( mapInfo_.size() ) << L"/" << ucMaxUser_;

    m_pkComboListMain->SetMainStatic( stm.str() );
    m_pkComboListPopup->SetUserList( mapInfo_ );
}

void KGCAgitUserListCombo::Clear( void )
{
    m_pkComboListMain->Clear();
    m_pkComboListPopup->Clear();
    m_pkComboListPopup->ToggleRender( false );
}

void KGCAgitUserListCombo::OnClickMain( void )
{
  // SpeakToActionListener( KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK) );
}

bool KGCAgitUserListCombo::CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos )
{ 
	return m_pkComboListPopup->CheckPosPopupBoxBound(vTempPos); 
} 

void KGCAgitUserListCombo::SetPopupBoxRender( bool bSet )
{
	return m_pkComboListPopup->SetPopupBoxRender( bSet );
}

void KGCAgitUserListCombo::SetActive( bool bActive_ )
{
    m_pkComboListMain->SetActive( bActive_ );
}

void KGCAgitUserListCombo::FrameMoveInEnabledState( void )
{
	if(!m_pkComboListPopup->IsRenderOn())
		return;

	if( !g_pkInput->BtnUp( KInput::MBLEFT ) )
		return;	

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	if ( CheckPosInWindowBound( vMousePos ) || m_pkComboListPopup->CheckPosInWindowBound( vMousePos ) ) 
		return;

	m_pkComboListPopup->ToggleRender(false);	
}