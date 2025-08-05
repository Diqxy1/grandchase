#include "StdAfx.h"

#include "GCCoupleItemBox.h"
#include "GCCoupleInventory.h"
#include "GCCoupleSystem.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCCoupleInventory );
IMPLEMENT_WND_FACTORY( KGCCoupleInventory );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleInventory, "gc_couple_inventory" );

KGCCoupleInventory::KGCCoupleInventory(void)
{
	char strTemp[MAX_PATH];
	for( int i = 0 ; i < GCCI_NUM ; ++i )
	{
		m_pkItemSlot[i] = NULL;
		sprintf( strTemp, "slot%d", i );
		LINK_CONTROL( strTemp,	m_pkItemSlot[i] );
	}
	LINK_CONTROL( "inventory", m_pkItemBox );
}

KGCCoupleInventory::~KGCCoupleInventory(void)
{
}

void KGCCoupleInventory::OnCreate( void )
{
	for( int i = 0 ; i < GCCI_NUM ; ++i )
	{
		m_pkItemSlot[i]->InitState( true, true, this );
		m_kItemImage[i].SetScale( 0.17f, 0.17f );
		m_kItemImage[i].SetPosition( ( m_pkItemSlot[i]->GetFixedWindowPos().x + 5.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -( m_pkItemSlot[i]->GetFixedWindowPos().y + 10.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
		m_kItemImage[i].CameraMatrixOn();
	}
	m_pkItemBox->InitState( true, true, this );
}

void KGCCoupleInventory::ActionPerformed( const KActionEvent& event )
{
	for( int i = 0 ; i < GCCI_NUM ; ++i )
	{
		if( KD3DWnd::D3DWE_BUTTON_CLICK != event.m_dwCode )
			break;

		if( event.m_pWnd != m_pkItemSlot[i] )
			continue;
		
		SiKGCCoupleSystem()->SendEquipItemChange( m_mapItemSlot[i] );
		break;
	}
	OnItemBoxEvent( event );
}

void KGCCoupleInventory::PostChildDraw( void )
{
	for( int i = 0 ; i < GCCI_NUM ; ++i )
	{
		m_kItemImage[i].Render();
	}
}

void KGCCoupleInventory::OnItemBoxEvent( const KActionEvent& event )
{
	if( event.m_pWnd != m_pkItemBox )
		return;

	if( event.m_dwCode != KGCCoupleItemBox::K3DWE_COUPLE_ITEM_CLICK )
		return;

	KSimpleItem kItem;
	kItem.m_cItemType = 0;
	kItem.m_dwID = static_cast<GCITEMID>(event.m_dwlParam);
	kItem.m_dwUID = event.m_dwlParam2;

	SiKGCCoupleSystem()->SendEquipItemChange( kItem );
}

void KGCCoupleInventory::Update()
{
	m_pkItemBox->Update();
	UpdateEquipSlot();
}

void KGCCoupleInventory::UpdateEquipSlot()
{
	KUserItemVector vecEquip = SiKGCCoupleSystem()->GetMyEquipItem();

	for( int i = 0 ; i < GCCI_NUM ; ++i )
	{
		m_kItemImage[i].RemoveAllTexture();
	}

	m_mapItemSlot.clear();
	for( int i = 0 ; i < (int)vecEquip.size() ; ++i )
	{
		if( g_pItemMgr->IsBaseItem(vecEquip[i].iItemID) == true )
			continue;

		GCItem* pItem = g_pItemMgr->GetItemData( vecEquip[i].iItemID );

		GCDeviceTexture* pTexture = g_pItemMgr->CreateItemTexture( vecEquip[i].iItemID, 0 );
		if( pItem->dwSlotPosition & ESP_HELMET )
		{
			m_kItemImage[GCCI_HAT].AddTexture( pTexture );
			m_mapItemSlot[GCCI_HAT] = vecEquip[i].iItemID;
		}
		else if( pItem->dwSlotPosition & ESP_JACKET )
		{
			m_kItemImage[GCCI_SUIT].AddTexture( pTexture );
			m_mapItemSlot[GCCI_SUIT] = vecEquip[i].iItemID;
		}
		else if( pItem->dwSlotPosition & ESP_PANTS )
		{
			m_kItemImage[GCCI_PANTS].AddTexture( pTexture );
			m_mapItemSlot[GCCI_PANTS] = vecEquip[i].iItemID;
		}
		else if( pItem->dwSlotPosition & ESP_SHOES )
		{
			m_kItemImage[GCCI_SHOES].AddTexture( pTexture );
			m_mapItemSlot[GCCI_SHOES] = vecEquip[i].iItemID;
		}
		else if( pItem->dwSlotPosition & ESP_SLOT_ITEM )
		{
			m_kItemImage[GCCI_BG_ITEM].AddTexture( pTexture );
			m_mapItemSlot[GCCI_BG_ITEM] = vecEquip[i].iItemID;
		}
		else if( !(pItem->dwSlotPosition & ESP_A_FINGER) )
		{
			m_kItemImage[GCCI_BG].AddTexture( pTexture );
			m_mapItemSlot[GCCI_BG] = vecEquip[i].iItemID;
		}
	}
}