#include "stdafx.h"
#include "GCChangeBox.h"
//

IMPLEMENT_CLASSNAME( KGCChangeBox );
IMPLEMENT_WND_FACTORY( KGCChangeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCChangeBox, "gc_change_box" );

KGCChangeBox::KGCChangeBox( void )
{
    m_pkLeft    = NULL;
    m_pkRight   = NULL;
    m_pkStatic  = NULL;

    LINK_CONTROL( "change_button_left",  m_pkLeft );
    LINK_CONTROL( "change_button_right", m_pkRight );
    LINK_CONTROL( "change_box_static",   m_pkStatic );
}

KGCChangeBox::~KGCChangeBox( void )
{
    // empty
}

void KGCChangeBox::OnCreate( void )
{
    m_pkLeft->SetSelfInputCheck( true );
    m_pkRight->SetSelfInputCheck( true );
    m_pkStatic->SetSelfInputCheck( false );
	m_pkStatic->SetAlign( DT_CENTER );
	m_pkStatic->SetAutoScroll( true );

    m_pkLeft->AddActionListener( this );
    m_pkRight->AddActionListener( this );
}

void KGCChangeBox::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkLeft && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        SpeakToActionListener( KActionEvent( this, LEFT_BUTTON ) );
    }
    else if ( event.m_pWnd == m_pkRight && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        SpeakToActionListener( KActionEvent( this, RIGHT_BUTTON ) );
    }
}

void KGCChangeBox::SetText( const std::wstring& strText )
{
    m_pkStatic->SetText( strText );
}

void KGCChangeBox::EnableChangeBox( bool bEnable )
{
    m_pkLeft->Lock( !bEnable );
    m_pkRight->Lock( !bEnable );
}
