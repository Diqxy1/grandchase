#include "stdafx.h"
#include "GCHeroTicketUseWnd.h"

IMPLEMENT_CLASSNAME( KGCHeroTicketUseWnd );
IMPLEMENT_WND_FACTORY( KGCHeroTicketUseWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroTicketUseWnd, "gc_hero_ticket_use" );

KGCHeroTicketUseWnd::KGCHeroTicketUseWnd( void )
: m_pkTicketPanel( NULL )
{
	LINK_CONTROL( "room_use_ticket", m_pkTicketPanel );	
}

KGCHeroTicketUseWnd::~KGCHeroTicketUseWnd( void )
{
}

void KGCHeroTicketUseWnd::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroTicketUseWnd::OnCreate( void )
{
	if( m_pkTicketPanel )
		m_pkTicketPanel->InitState( true, true, this );
}

void KGCHeroTicketUseWnd::OnExitWindow( void )
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}