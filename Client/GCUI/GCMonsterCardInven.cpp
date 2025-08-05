#include "stdafx.h"
#include "GCMonsterCardInven.h"

IMPLEMENT_CLASSNAME( KGCMonsterCardInven );
IMPLEMENT_WND_FACTORY( KGCMonsterCardInven );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCardInven, "gc_monstercardinven" );

KGCMonsterCardInven::KGCMonsterCardInven( void )
{
	m_ItemID = 0;
	m_pkItemImg = NULL;
	m_pkDuration = NULL;

	LINK_CONTROL( "item_img", m_pkItemImg );
	LINK_CONTROL( "item_duration", m_pkDuration );
}

KGCMonsterCardInven::~KGCMonsterCardInven( void )
{
}

void KGCMonsterCardInven::ActionPerformed( const KActionEvent& event )
{
}

void KGCMonsterCardInven::OnCreate( void )
{
	m_pkItemImg->InitState( true );
	m_pkDuration->InitState( false );
	m_pkDuration->SetStringType( "blue_gradation_num" );
}

void KGCMonsterCardInven::SetDuration( int nDuration )
{
	m_pkDuration->SetNumber( nDuration );
	ShowDuration( true );
}

void KGCMonsterCardInven::SetItemInfo( GCITEMID _itemID )
{
	m_ItemID = _itemID;
	m_pkItemImg->SetItemInfo( _itemID );
	m_pkItemImg->ShowItem( _itemID != 0 );
}

void KGCMonsterCardInven::ShowDuration( bool _bShow )
{
	m_pkDuration->ToggleRender( _bShow );
}

void KGCMonsterCardInven::SetShowItem( bool _bShow )
{
	m_pkItemImg->ShowItem( _bShow );
}