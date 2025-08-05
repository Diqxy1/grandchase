#include "stdafx.h"
#include "GCCharCollectionImage.h"

IMPLEMENT_CLASSNAME( KGCCharCollectionImage );
IMPLEMENT_WND_FACTORY( KGCCharCollectionImage );
IMPLEMENT_WND_FACTORY_NAME( KGCCharCollectionImage, "gc_char_collection_image" );

KGCCharCollectionImage::KGCCharCollectionImage( void )
{
	for ( int i = 0; i < 4; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "char_img_%d", i );
		m_vecCharImage[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecCharImage[ i ] );
		m_vecCharSelect[ i ] = NULL;
		sprintf( strTemp, "select_%d", i );
		LINK_CONTROL( strTemp, m_vecCharSelect[i] );
	}
}

KGCCharCollectionImage::~KGCCharCollectionImage( void )
{
}

void KGCCharCollectionImage::ActionPerformed( const KActionEvent& event )
{
}

void KGCCharCollectionImage::FrameMoveInEnabledState( void )
{
	for ( int i = 0; i < 4; ++i )
	{
		if( !(m_vecCharImage[ i ]->GetWindowColor() == D3DCOLOR_ARGB( 0xff, 0x80, 0x80, 0x80 ) ) && m_vecCharImage[i]->CheckMousePosInWindowBound() )
			m_vecCharSelect[ i ]->ToggleRender( true );
		else
			m_vecCharSelect[ i ]->ToggleRender( false );
	}
}

void KGCCharCollectionImage::OnCreate( void )
{
	for ( int i = 0; i < 4; i++ )
	{
		m_vecCharImage[ i ]->InitState( true,true,this );
		m_vecCharImage[ i ]->SetWindowColor( D3DCOLOR_ARGB( 0xff, 0x80, 0x80, 0x80 ) );
		m_vecCharSelect[ i ]->InitState( false, true, this );
	}
}

void KGCCharCollectionImage::SetImageActive( int iCharPromotion, bool bIsActive )
{
	if( bIsActive )
		m_vecCharImage[ iCharPromotion ]->SetWindowColor( D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
	else
		m_vecCharImage[ iCharPromotion ]->SetWindowColor( D3DCOLOR_ARGB( 0xff, 0x80, 0x80, 0x80 ) );
}

void KGCCharCollectionImage::SetImageTooltip( int iCharType, int iCharPromotion )
{
	m_vecCharImage[ iCharPromotion ]->SetToolTip( g_MyD3D->m_TempPlayer.GetCharacterCollectionStringByPromotion( iCharType, iCharPromotion ) );
}