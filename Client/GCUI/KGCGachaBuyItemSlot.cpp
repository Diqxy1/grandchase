#include "stdafx.h"
#include "KGCGachaBuyItemSlot.h"

IMPLEMENT_CLASSNAME( KGCGachaBuyItemSlot );
IMPLEMENT_WND_FACTORY( KGCGachaBuyItemSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCGachaBuyItemSlot, "gc_gachabuyitemslot" );

KGCGachaBuyItemSlot::KGCGachaBuyItemSlot()
: m_uiPrice( 0 )
, m_kItem( NULL )
, m_pkName( NULL )
, m_pkPrice( NULL )
, m_pkGPMark( NULL )
, m_pkBtnBuy( NULL )
, m_pkItemImg( NULL )
, m_pkCashMark( NULL )
{
	LINK_CONTROL( "cash_mark",		m_pkCashMark );
	LINK_CONTROL( "gp_mark",		m_pkGPMark	 );
	LINK_CONTROL( "static_name",	m_pkName	 );
	LINK_CONTROL( "static_price",	m_pkPrice	 );
	LINK_CONTROL( "btn_buy",		m_pkBtnBuy	 );
	LINK_CONTROL( "item_img",		m_pkItemImg	 );
}

KGCGachaBuyItemSlot::~KGCGachaBuyItemSlot()
{
}

void KGCGachaBuyItemSlot::ActionPerformed( const KActionEvent& event)
{
}

void KGCGachaBuyItemSlot::OnCreate()
{
	if( m_pkCashMark )
	{
		m_pkCashMark->InitState( false, true, this );
	}

	if( m_pkGPMark )
	{
		m_pkGPMark->InitState( false, true, this );
	}

	if( m_pkPrice )
	{
		m_pkPrice->InitState( false, true, this );
	}

	if( m_pkBtnBuy )
	{
		m_pkBtnBuy->InitState( true, true, this );
		m_pkBtnBuy->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGachaBuyItemSlot::OnBuy );
	}
}

void KGCGachaBuyItemSlot::SetItem( DWORD dwGoodsID )
{
	m_kItem = g_MyD3D->m_kItemMgr.GetItemData( dwGoodsID );

	if( m_kItem->eMoneyType == EMT_GAMEPOINT )
		m_pkGPMark->ToggleRender( true );
	else
		m_pkCashMark->ToggleRender( true );

	m_pkPrice->SetText( m_kItem->dwPrice );
	m_pkPrice->ToggleRender( true );
	m_pkName->SetTextAutoMultiline( m_kItem->strItemName );
	m_pkName->ToggleRender( true );
	m_pkItemImg->SetItemInfo( m_kItem->dwGoodsID );
	m_pkItemImg->ToggleRender( true );
}

void KGCGachaBuyItemSlot::OnBuy()
{
	if( m_kItem->eMoneyType == EMT_GAMEPOINT )
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_kItem->dwGoodsID, KBuySellItemReq::EMT_GAMEPOINT, false, true );
	else
		g_MyD3D->m_kItemMgr.BuyCashItemS6( m_kItem->dwGoodsID );
}