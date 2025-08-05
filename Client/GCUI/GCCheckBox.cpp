#include "stdafx.h"
#include "GCCheckBox.h"

IMPLEMENT_CLASSNAME( KGCCheckBox );
IMPLEMENT_WND_FACTORY( KGCCheckBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCheckBox, "gc_checkbox" );

KGCCheckBox::KGCCheckBox( void )
: m_pkCheck(NULL)
, m_pkBox(NULL)
, m_pkStatic(NULL)
, m_bCheck(false)
{
    LINK_CONTROL( "check",  m_pkCheck );
    LINK_CONTROL( "box",    m_pkBox );
    LINK_CONTROL( "title_static",   m_pkStatic );
}

KGCCheckBox::~KGCCheckBox( void )
{
}

void KGCCheckBox::OnCreate( void )
{
    m_pkBox->InitState( true );
    m_pkCheck->InitState( m_pkCheck->IsRenderOn() );

    if( m_pkStatic != NULL )
    {
        m_pkStatic->InitState(true);
        m_pkStatic->SetText(L"");
	}
}

void KGCCheckBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCCheckBox::ToggleCheck()
{
    m_bCheck = !m_bCheck;
    m_pkCheck->ToggleRender( m_bCheck );
}

void KGCCheckBox::SetCheck( bool bCheck )
{
    m_bCheck = bCheck;
    m_pkCheck->ToggleRender( bCheck );
}

void KGCCheckBox::SetText( std::wstring wstr_ )
{
    if( m_pkStatic != NULL )
        m_pkStatic->SetTextAutoMultiline( wstr_ );
}

void KGCCheckBox::InitInfo()
{
    SetCheck(false);
    if( m_pkStatic != NULL )
        m_pkStatic->SetText(L"");
}

void KGCCheckBox::SetLock( bool bLock_ )
{
    SetCheck(false);
    m_pkBox->Lock(bLock_);
}

void KGCCheckBox::SetFontColor( D3DCOLOR dwColor_ )
{
    if ( m_pkStatic != NULL) {
        m_pkStatic->SetFontColor( dwColor_ );
    }
}

void KGCCheckBox::SetFontOutline( bool bFontOutline /*= false*/, D3DCOLOR OutlineColor /*= 0xFF000000 */ )
{
    if ( m_pkStatic != NULL ) {
        m_pkStatic->SetFontOutline( bFontOutline, OutlineColor );
    }
}
