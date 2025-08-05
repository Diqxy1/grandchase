#include "stdafx.h"
#include "GCVipCollection.h"

IMPLEMENT_CLASSNAME( KGCVipCollection );
IMPLEMENT_WND_FACTORY( KGCVipCollection );
IMPLEMENT_WND_FACTORY_NAME( KGCVipCollection, "gc_vip_collection_container" );

KGCVipCollection::KGCVipCollection( void )
{
	for ( int i = 0; i < VIP_COLLECTION_SIZE; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "vip_%d", i );
		m_vecVipContainer[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecVipContainer[ i ] );
	}
}

KGCVipCollection::~KGCVipCollection( void )
{
}

void KGCVipCollection::ActionPerformed( const KActionEvent& event )
{
}

void KGCVipCollection::OnCreate( void )
{
	for ( int i = 0; i < VIP_COLLECTION_SIZE; i++ )
	{
		if( m_vecVipContainer[ i ] )
		{
			m_vecVipContainer[ i ]->InitState( false, true, this );
			m_vecVipContainer[ i ]->Lock( true );
		}
	}
}

void KGCVipCollection::SetVIPImage( int iIndex, int iFontID )
{
	if( m_vecVipContainer[ iIndex ] )
		m_vecVipContainer[ iIndex ]->ToggleRender( true );
	m_vecVipContainer[ iIndex ]->SetToolTip( g_MyD3D->m_TempPlayer.GetVIPCollectionStringByFontID( iFontID ) );
	KItem* pInventoryItem = NULL;
	const std::vector<GCITEMID> tempFontVec = g_MyD3D->m_TempPlayer.m_kUserInfo.m_vecFontVector;
	std::vector<GCITEMID>::const_iterator itemIT = find( tempFontVec.begin(), tempFontVec.end(), iFontID );
	if ( itemIT != tempFontVec.end() )
    {
		if ( m_vecVipContainer[ iIndex ] )
		{
			m_vecVipContainer[ iIndex ]->Lock( false );
			return;
		}
	}
}

void KGCVipCollection::ResetCharPlates()
{
	for ( int i = 0; i < VIP_COLLECTION_SIZE; ++i )
	{
		if( m_vecVipContainer[ i ] )
		{
			m_vecVipContainer[ i ]->ToggleRender( false );
			m_vecVipContainer[ i ]->Lock( true );
		}
	}
}