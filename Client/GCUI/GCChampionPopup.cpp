#include "stdafx.h"
#include "GCChampionPopup.h"
#include "GCChampionElement.h"

IMPLEMENT_CLASSNAME( KGCChampionPopup );
IMPLEMENT_WND_FACTORY( KGCChampionPopup );
IMPLEMENT_WND_FACTORY_NAME( KGCChampionPopup, "gc_champion_popup" );

KGCChampionPopup::KGCChampionPopup( void )
{
	for ( int i = 0; i < MAX_STATS; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "champ_element_%d", i );
		m_vecChampionElements[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecChampionElements[ i ] );
	}
}

KGCChampionPopup::~KGCChampionPopup( void )
{
}

void KGCChampionPopup::OnCreate( void )
{
	for ( int i = 0; i < MAX_STATS; i++ )
		if( m_vecChampionElements[ i ] )
			m_vecChampionElements[ i ]->InitState( true, true, this );
}

void KGCChampionPopup::OnCreateComplete( void )
{
	for ( int i = 0; i < MAX_STATS; i++ )
		if( m_vecChampionElements[ i ] )
			m_vecChampionElements[ i ]->SetChampionText( i );
}

void KGCChampionPopup::ActionPerformed( const KActionEvent& event )
{
}

void KGCChampionPopup::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}
