#include "stdafx.h"
#include "GCCharCollection.h"
#include "GCCharCollectionImage.h"

IMPLEMENT_CLASSNAME( KGCCharCollection );
IMPLEMENT_WND_FACTORY( KGCCharCollection );
IMPLEMENT_WND_FACTORY_NAME( KGCCharCollection, "gc_char_collection_container" );

KGCCharCollection::KGCCharCollection( void )
: m_pkStaticCharName( NULL )
{
	for ( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "char_plate_%d", i );
		m_vecCharContainer[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecCharContainer[ i ] );
	}
	for (int i = 0; i < 2; i++)
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "name_%d", i );
		m_vecNameBG[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecNameBG[ i ] );
	}
	LINK_CONTROL( "static_name ", m_pkStaticCharName );
}

KGCCharCollection::~KGCCharCollection( void )
{
}

void KGCCharCollection::ActionPerformed( const KActionEvent& event )
{
}

void KGCCharCollection::OnCreate( void )
{
	if( m_pkStaticCharName )
	{
		m_pkStaticCharName->InitState( true, false );
		m_pkStaticCharName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
		m_pkStaticCharName->SetAlign( DT_LEFT );
	}
	for ( int i = 0; i < GC_CHAR_NUM; i++ )
		m_vecCharContainer[ i ]->InitState( false, true, this );
	for ( int i = 0; i < 2; i++ )
		m_vecNameBG[ i ]->InitState( false, true, this );
}

D3DCOLOR KGCCharCollection::GetCharColor( int iCharType )
{
	switch( iCharType )
	{
		case  0: return 0xffff2424;
		case  1: return 0xff79ff3b;
		case  2: return 0xffd32dfc;
		case  3: return 0xff303bff;
		case  4: return 0xfff07218;
		case  5: return 0xff5314ff;
		case  6: return 0xffff40c9;
		case  7: return 0xffb51010;
		case  8: return 0xff44034a;
		case  9: return 0xff4287ff;
		case 10: return 0xff7a12a3;
		case 11: return 0xff7bb31d;
		case 12: return 0xff8c0b8a;
		case 13: return 0xff80091d;
		case 14: return 0xff1c9e95;
		case 15: return 0xff7358e0;
		case 16: return 0xff67ebb2;
		case 17: return 0xff4b64d1;
		case 18: return 0xffa82eff;
		case 19: return 0xff47b5d6;
		case 20: return 0xff8238f2;
		case 21: return 0xffff91dc;
	}
	return 0xffff2424;
}

void KGCCharCollection::SetCharType( int iCharType, int iPromotion )
{
	for ( int i = 0; i < 2; i++ )
		m_vecNameBG[i]->ToggleRender( true );
	m_vecCharContainer[ iCharType ]->ToggleRender( true );
	if ( m_pkStaticCharName )
	{
		m_pkStaticCharName->ToggleRender( true );
		m_pkStaticCharName->SetText( g_kGlobalValue.GetCharName( iCharType ) );
		m_pkStaticCharName->SetFontColor (GetCharColor( iCharType ) );
		SetCharPromotionTooltip( iCharType );
	}
	if( iPromotion > -1 )
		SetCharPromotion( iCharType, iPromotion );
}

void KGCCharCollection::SetCharPromotion( int iCharType, int iPromotion )
{
	for ( int i = iPromotion; i >= 0; --i )
		m_vecCharContainer[ iCharType ]->SetImageActive( i, true );
}

void KGCCharCollection::SetCharPromotionTooltip( int iCharType )
{
	for ( int i = 3; i >= 0; --i )
		m_vecCharContainer[ iCharType ]->SetImageTooltip( iCharType, i );
}

void KGCCharCollection::ResetCharPlates()
{
	for ( int i = 0; i < 2; i++ )
		m_vecNameBG[i]->ToggleRender( false );
	for ( int i = 0; i < GC_CHAR_NUM; ++i )
	{
		m_vecCharContainer[ i ]->ToggleRender( false );
		if ( m_pkStaticCharName )
		{
			m_pkStaticCharName->ToggleRender( false );
			m_pkStaticCharName->SetText( L"" );
			m_pkStaticCharName->SetFontColor( 0xffffffff );
		}
		for( int k = 3; k >= 0; k-- )
			m_vecCharContainer[ i ]->SetImageActive( k, false );
	}
}