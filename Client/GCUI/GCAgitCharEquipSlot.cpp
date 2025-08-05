#include "stdafx.h"
#include "GCAgitCharEquipSlot.h"

IMPLEMENT_CLASSNAME( KGCAgitCharEquipSlot );
IMPLEMENT_WND_FACTORY( KGCAgitCharEquipSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitCharEquipSlot, "gc_agit_char_equip_slot" );

KGCAgitCharEquipSlot::KGCAgitCharEquipSlot( void )
{
    m_pkFrame = NULL;
    m_pkItemImg = NULL;
    LINK_CONTROL( "defaultFrame", m_pkFrame );
    LINK_CONTROL( "itemImg", m_pkItemImg );

    m_bEquip = false;
    m_ItemUID = 0;
    m_dwSlotPosition = 0;
}

KGCAgitCharEquipSlot::~KGCAgitCharEquipSlot( void )
{
}

void KGCAgitCharEquipSlot::OnCreate( void )
{
    m_pkFrame->InitState( true, true, this );
    m_pkItemImg->InitState( true );
}

void KGCAgitCharEquipSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkFrame, KD3DWnd::D3DWE_BUTTON_CLICK, OnClick );
}

void KGCAgitCharEquipSlot::SetSlot( GCITEMUID itemUID_ )
{
    Clear();

    KItem* pInvenItem = NULL;
    g_pItemMgr->FindInventory( itemUID_, &pInvenItem );
    if( NULL == pInvenItem ) { 
        return;
    }

    GCItem* pItem = g_pItemMgr->GetItemData( pInvenItem->m_ItemID );
    if( 0x00000000 == (pItem->dwSlotPosition & m_dwSlotPosition) ) {
        return;
    }

    m_pkItemImg->SetItemInfo( pInvenItem->m_ItemID, -1, false, -1, 0, -1, pInvenItem->m_DesignCoordiID );
    m_pkItemImg->ShowItem( true );

    m_ItemUID = pInvenItem->m_ItemUID;
    m_bEquip = true;
}

void KGCAgitCharEquipSlot::OnClick( void )
{
    if( 0 == m_ItemUID ) {
        return; 
    }

    if( false == m_bEquip ) {
        return; 
    }

    Clear();
    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );   
}

void KGCAgitCharEquipSlot::Clear( void )
{
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
    m_bEquip = false;
    m_ItemUID = 0;
}

void KGCAgitCharEquipSlot::SetSlotPosition( DWORD dwSlotPos_ )
{
    m_dwSlotPosition = dwSlotPos_;
}

DWORD KGCAgitCharEquipSlot::GetSlotPosition( void )
{
    return m_dwSlotPosition;
}