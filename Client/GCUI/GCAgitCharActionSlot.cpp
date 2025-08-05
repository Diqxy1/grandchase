#include "stdafx.h"
#include "GCAgitCharActionSlot.h"

IMPLEMENT_CLASSNAME( KGCAgitCharActionSlot );
IMPLEMENT_WND_FACTORY( KGCAgitCharActionSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitCharActionSlot, "gc_agit_char_action_slot" );

KGCAgitCharActionSlot::KGCAgitCharActionSlot( void )
{
    m_pkBtnDefault = NULL;
    m_pkBtnActive = NULL;
    m_pkStaticAction = NULL;
    LINK_CONTROL( "btnDefault",   m_pkBtnDefault );
    LINK_CONTROL( "btnSelect",    m_pkBtnActive );
    LINK_CONTROL( "staticAction", m_pkStaticAction );

    m_iActionIndex = -1;
    m_iStringIndex = -1;
}

KGCAgitCharActionSlot::~KGCAgitCharActionSlot( void )
{
}

void KGCAgitCharActionSlot::OnCreate( void )
{
    m_pkBtnDefault->InitState( true, true, this );
    m_pkBtnActive->InitState( true, true, this );
    m_pkStaticAction->InitState( true );

    m_iActionIndex = -1;
    m_iStringIndex = -1;
}

void KGCAgitCharActionSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnDefault,  KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickBtn );
    GCWND_MSG_MAP( m_pkBtnActive,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickBtn );
}

void KGCAgitCharActionSlot::SetSlot( int iActionIndex_, int iStringIndex_ )
{
    m_iActionIndex = iActionIndex_;
    m_iStringIndex = iStringIndex_;

    m_pkStaticAction->SetText( g_pkStrLoader->GetString( m_iStringIndex ) );
}

void KGCAgitCharActionSlot::Clear( void )
{
    m_iActionIndex = -1;
    m_iStringIndex = -1;

    SetActive( false );
    m_pkStaticAction->SetText( L"" );
}

void KGCAgitCharActionSlot::OnClickBtn( void )
{
    SpeakToActionListener( KActionEvent(this, KD3DWnd::D3DWE_BUTTON_CLICK) );
}

void KGCAgitCharActionSlot::SetActive( bool bActive_ )
{
    m_pkBtnDefault->ToggleRender( !bActive_ );
    m_pkBtnActive->ToggleRender( bActive_ );
}