#include "stdafx.h"
#include "GCAgitCharItemSlot.h"

IMPLEMENT_CLASSNAME( KGCAgitCharItemSlot );
IMPLEMENT_WND_FACTORY( KGCAgitCharItemSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitCharItemSlot, "gc_agit_char_item_slot" );

KGCAgitCharItemSlot::KGCAgitCharItemSlot( void )
{
    m_pkDefaultFrame = NULL;
    m_pkActiveFrame = NULL;
    m_pkItemImg = NULL;
    LINK_CONTROL( "defaultFrame", m_pkDefaultFrame );
    LINK_CONTROL( "activeFrame", m_pkActiveFrame );
    LINK_CONTROL( "itemImg", m_pkItemImg );

    m_pItemData = NULL;
    m_pInvenItem = NULL;
    m_bEquip = false;
}

KGCAgitCharItemSlot::~KGCAgitCharItemSlot( void )
{
}

void KGCAgitCharItemSlot::OnCreate( void )
{
    m_pkDefaultFrame->InitState( true, true, this );
    m_pkActiveFrame->InitState( false, true, this );
    m_pkItemImg->InitState( true );

    m_pItemData = NULL;
    m_pInvenItem = NULL;
    m_bEquip = false;
}

void KGCAgitCharItemSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkDefaultFrame,    KD3DWnd::D3DWE_BUTTON_CLICK,    Onclick );
    GCWND_MSG_MAP( m_pkActiveFrame,     KD3DWnd::D3DWE_BUTTON_CLICK,    Onclick );
}

void KGCAgitCharItemSlot::SetSlot( GCITEMUID itemUID_ )
{
    Clear();
    if( false == g_pItemMgr->FindInventory( itemUID_, &m_pInvenItem ) ) {
        return; 
    }

    m_pItemData = g_pItemMgr->GetItemData( m_pInvenItem->m_ItemID );
    if( NULL == m_pItemData ) {
        return;
    }

    m_pkItemImg->SetItemInfo( m_pInvenItem->m_ItemID, -1, false, -1, 0, -1, m_pInvenItem->m_DesignCoordiID );
    m_pkItemImg->ShowItem( true );
}

void KGCAgitCharItemSlot::Onclick( void )
{
    if( NULL == m_pItemData || NULL == m_pInvenItem ) {
        return; 
    }

    if( false == m_bEquip ) {
        m_bEquip = true;
    }
    else {
        m_bEquip = false;
    }

    m_pkDefaultFrame->ToggleRender( !m_bEquip );
    m_pkActiveFrame->ToggleRender( m_bEquip );

    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );  
}

void KGCAgitCharItemSlot::SetEquipState( bool bEquip_ )
{
    m_bEquip = bEquip_;

    m_pkDefaultFrame->ToggleRender( !m_bEquip );
    m_pkActiveFrame->ToggleRender( m_bEquip );
}

bool KGCAgitCharItemSlot::GetEquipState( void )
{
    return m_bEquip;
}

GCITEMID KGCAgitCharItemSlot::GetItemID( void )
{
    if( NULL == m_pInvenItem ) {
        return 0;
    }

    return m_pInvenItem->m_ItemID;
}

GCITEMUID KGCAgitCharItemSlot::GetItemUID( void )
{
    if( NULL == m_pInvenItem ) {
        return 0; 
    }

    return m_pInvenItem->m_ItemUID;
}

void KGCAgitCharItemSlot::Clear( void )
{
    m_pkDefaultFrame->ToggleRender( true );
    m_pkActiveFrame->ToggleRender( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );

    m_pItemData = NULL;
    m_pInvenItem = NULL;
    m_bEquip = false; 
}

GCItem* KGCAgitCharItemSlot::GetItemData( void )
{
    return m_pItemData;
}

KItem* KGCAgitCharItemSlot::GetInvenItem( void )
{
    return m_pInvenItem;
}