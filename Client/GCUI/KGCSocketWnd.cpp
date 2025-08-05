#include "stdafx.h"
#include "KGCSocketWnd.h"
//



//
#include "./MyD3D.h"
//


IMPLEMENT_CLASSNAME( KGCSocketWnd );
IMPLEMENT_WND_FACTORY( KGCSocketWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSocketWnd, "gc_socket_wnd" );

KGCSocketWnd::KGCSocketWnd( void ) : m_pInvenItem(NULL), m_pTexture( NULL )
{
	for( int i = 0; i < (int)KGCSocketWnd::MAX_NUM; i++ )
	{
		m_pIcon[i] = NULL;
	}
	LINK_CONTROL( "icon_weapon", m_pIcon[KGCSocketWnd::WEAPON] );
	LINK_CONTROL( "icon_helmet", m_pIcon[KGCSocketWnd::HELMET] );
	LINK_CONTROL( "icon_armor",  m_pIcon[KGCSocketWnd::ARMOR] );
}
KGCSocketWnd::~KGCSocketWnd( void )
{
	SAFE_RELEASE( m_pTexture );
	m_pInvenItem = NULL;
}
void KGCSocketWnd::OnCreate( void )
{
	SetAllWindowColor( D3DCOLOR_RGBA( 255, 255, 255, 100 ) );
}

void KGCSocketWnd::FrameMoveInEnabledState( void )
{

}

void KGCSocketWnd::PostDraw( void )
{
	if ( m_pTexture == NULL || m_pInvenItem == NULL )
		return;	

	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

	
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	{
		g_pGCDeviceManager2->DrawInScreen(m_pTexture,
			vPos.x , vPos.y , vPos.x + 45.0f * m_fWindowScaleX, vPos.y + 45.0f * m_fWindowScaleY,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);

	}
	g_pGCDeviceManager2->PopState();
}

void KGCSocketWnd::ActionPerformed( const KActionEvent& event )
{

}

void KGCSocketWnd::SetItem( KItem* pInvenItem_ )
{ 
	m_pInvenItem = pInvenItem_; 
	SAFE_RELEASE( m_pTexture );

	if ( m_pInvenItem != NULL )
	{
		m_pTexture = g_pItemMgr->CreateItemTexture( pInvenItem_->m_ItemID, 0 );
	}
}

void KGCSocketWnd::SetIcon( KGCSocketWnd::SocketIcon eIcon_ )
{
	for( int i = 0; i < (int)KGCSocketWnd::MAX_NUM; i++ )
	{
		m_pIcon[i]->ToggleRender( false );
	}
	m_pIcon[eIcon_]->ToggleRender( true );
}