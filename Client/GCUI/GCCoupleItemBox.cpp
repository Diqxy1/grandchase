#include "StdAfx.h"

#include "GCCoupleItemBox.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCCoupleItemBox );
IMPLEMENT_WND_FACTORY( KGCCoupleItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleItemBox, "gc_couple_item_box" );

KGCCoupleItemBox::KGCCoupleItemBox(void)
{
	for( int i = 0 ; i < GCCIB_SLOT_NUM ; ++i )
	{
		m_pkItemSlot[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "couplelook_slot%d", i );
		LINK_CONTROL( temp, m_pkItemSlot[i] );
	}

	LINK_CONTROL( "scroll",		m_pkScroll );
	LINK_CONTROL( "sorting",	m_pkComboBox );
}

KGCCoupleItemBox::~KGCCoupleItemBox(void)
{
}

void KGCCoupleItemBox::OnCreate( void )
{
	m_pkScroll->InitState( true, true, this );
	m_pkComboBox->InitState( true, true, this );
	for( int i = 0 ; i < GCCIB_SLOT_NUM ; ++i )
	{
		m_pkItemSlot[i]->InitState( true, true, this );
#if defined( _DEBUG )
		KLuaManager luaMgr;
		GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

		float x = 0.0f, y = 1.0f;
		LUA_GET_VALUE_DEF( "Scalex", x, 1.0f );
		LUA_GET_VALUE_DEF( "Scaley", y, 1.0f );

		float px, py;
		LUA_GET_VALUE_DEF( "Px", px, 1.0f );
		LUA_GET_VALUE_DEF( "Py", py, 1.0f );
#endif
		m_pkItemImage[i].SetScale( 0.19f, 0.19f );
		m_pkItemImage[i].SetPosition( ( m_pkItemSlot[i]->GetFixedWindowPos().x + 4.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -( m_pkItemSlot[i]->GetFixedWindowPos().y + 17.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
		m_pkItemImage[i].CameraMatrixOn();
	}

	Update();
}

void KGCCoupleItemBox::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_pWnd == m_pkScroll )
	{
		int iPos = m_pkScroll->GetScrollPos();

		if ( m_iScrollPos != iPos )
		{
			g_KDSound.Play( "73" );
			OnScrollPos(iPos);
			m_iScrollPos = iPos;
		}
		return;
	}

	for( int i = 0 ; i < GCCIB_SLOT_NUM ; ++i )
	{
		if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
			continue;
		if( event.m_pWnd == m_pkItemSlot[i] && i < (int)m_vecItems.size() )
		{
			SpeakToActionListener( KActionEvent( this, K3DWE_COUPLE_ITEM_CLICK, m_vecItems[i].m_dwID, m_vecItems[i].m_dwUID ) ); 
			Update();
		}
	}
}

void KGCCoupleItemBox::OnScrollPos( int iPos )
{
	m_vecItems = SiKGCCoupleSystem()->GetCoupleItem( 5, 3, iPos );

	for( int i = 0 ; i < GCCIB_SLOT_NUM ; ++i )
	{
		m_pkItemImage[i].RemoveAllTexture();

		if( (int)m_vecItems.size() <= i )
			continue;

		m_pkItemImage[i].AddTexture( g_pItemMgr->CreateItemTexture( m_vecItems[i].m_dwID, 0 ) );
	}
}

void KGCCoupleItemBox::PostChildDraw( void )
{
	for( int i = 0 ; i < GCCIB_SLOT_NUM ; ++i )
	{
		m_pkItemImage[i].Render();
	}
}

void KGCCoupleItemBox::Update()
{
	int iTotalPage = (SiKGCCoupleSystem()->GetCoupleItemNum() + 4 )/5;
	m_pkScroll->SetScrollPageSize( 3 );
	m_pkScroll->SetScrollRangeMax( iTotalPage );
	OnScrollPos( m_pkScroll->GetScrollPos() );
}