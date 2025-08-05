#include "stdafx.h"
#include "GCChampionElement.h"

IMPLEMENT_CLASSNAME( KGCChampionElement );
IMPLEMENT_WND_FACTORY( KGCChampionElement );
IMPLEMENT_WND_FACTORY_NAME( KGCChampionElement, "gc_champion_element" );

KGCChampionElement::KGCChampionElement( void )
: m_pkText( NULL )
{
	LINK_CONTROL( "text", m_pkText );
}

KGCChampionElement::~KGCChampionElement( void )
{
}

void KGCChampionElement::ActionPerformed( const KActionEvent& event )
{
}

void KGCChampionElement::OnCreate( void )
{
	if ( m_pkText )
	{
		m_pkText->InitState( true, true, this );
	}
}

void KGCChampionElement::SetChampionText( int iIndex )
{
	D3DCOLOR tempColor;
	std:wstring tempString;
	switch( iIndex )
	{
		case 0: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY01 ); break;
		case 1: tempColor = 0xff314ae4; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY02 ); break;
		case 2: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY04 ); break;
		case 3: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY06 ); break;
		case 4: tempColor = 0xff314ae4; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY03 ); break;
		case 5: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY08 ); break;
		case 6: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY07 ); break;
		case 7: tempColor = 0xff314ae4; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY09 ); break;
		case 8: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY10 ); break;
		case 9: tempColor = 0xffe43134; tempString = g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY05 ); break;
	}
	m_pkText->SetText( tempString );
	m_pkText->SetFontColor( tempColor );
}

void KGCChampionElement::ResetChampionText()
{
	m_pkText->SetText( L"" );
	m_pkText->SetFontColor( 0xffffffff );
}