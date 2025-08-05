#include "stdafx.h"
#include "KGCRankIcon.h"

IMPLEMENT_CLASSNAME( KGCRankIcon );
IMPLEMENT_WND_FACTORY( KGCRankIcon );
IMPLEMENT_WND_FACTORY_NAME( KGCRankIcon, "gc_rank_icon" );

KGCRankIcon::KGCRankIcon( void )
{
	m_pkIcon = NULL;
	LINK_CONTROL( "rank_icon", m_pkIcon );

	m_pkDisableIcon = NULL;
	LINK_CONTROL( "disable", m_pkDisableIcon );
}

KGCRankIcon::~KGCRankIcon( void )
{
}

void KGCRankIcon::OnCreate( void )
{
	if( m_pkIcon )
		m_pkIcon->InitState( true );

	if(m_pkDisableIcon)
		m_pkDisableIcon->InitState(false);
}


void KGCRankIcon::ActionPerformed( const KActionEvent& event )
{

}

void KGCRankIcon::InitMark()
{
	if( m_pkIcon )
		m_pkIcon->ToggleRender( true );

	if(m_pkDisableIcon)
		m_pkDisableIcon->ToggleRender(false);
}
