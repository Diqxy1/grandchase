#include "stdafx.h"
#include "GCGameInvenEquipSlot.h"

IMPLEMENT_CLASSNAME( KGCGameInvenEquipSlot );
IMPLEMENT_WND_FACTORY( KGCGameInvenEquipSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCGameInvenEquipSlot, "gc_game_equip_slot" );

KGCGameInvenEquipSlot::KGCGameInvenEquipSlot( void )
{
    m_pkBack = NULL;
    m_pkItemImg = NULL;
    LINK_CONTROL( "back",    m_pkBack );
    LINK_CONTROL( "itemImg", m_pkItemImg );

    m_pItem = NULL;
    m_bMouseBound = false;
}

KGCGameInvenEquipSlot::~KGCGameInvenEquipSlot( void )
{
}

void KGCGameInvenEquipSlot::OnCreate( void )
{
    m_pkBack->InitState( true, true, this );
    m_pkBack->DisconnectAllEventProcedure();
    m_pkBack->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInvenEquipSlot::OnClick );

    m_pkItemImg->InitState( true );
}

void KGCGameInvenEquipSlot::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBack && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) {
        SetMouseBound( true );
    }

    if( event.m_pWnd == m_pkBack && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
        SetMouseBound( false );
    }

}

void KGCGameInvenEquipSlot::SetItemData( GCITEMUID itemUID_ )
{
    m_pItem = NULL;
    g_pItemMgr->FindInventory( itemUID_, &m_pItem );
    if( NULL == m_pItem ) { 
        return;
    }

    m_pkItemImg->SetItemInfo( m_pItem->m_ItemID );
    m_pkItemImg->ShowItem( true );
}

void KGCGameInvenEquipSlot::Clear( void )
{
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
    m_pItem = NULL;
}

void KGCGameInvenEquipSlot::OnClick( void )
{
    if( NULL == m_pItem ) { 
        return;
    }

    SpeakToActionListener( KActionEvent(this, D3DWE_BUTTON_CLICK, m_pItem->m_ItemUID ) );  
}

void KGCGameInvenEquipSlot::FrameMoveInEnabledState( void )
{
    if( m_bMouseBound && m_pItem ) {

        D3DXVECTOR2 dxvPos = GetFixedWindowLocalPos();

        dxvPos.x += ( 150.0f * g_pGCDeviceManager->GetWindowScaleX() );
        dxvPos.y += ( 82.0f * g_pGCDeviceManager->GetWindowScaleY() );

        GCItem* pItem = NULL;
        g_pItemMgr->FindInventory( m_pItem->m_ItemUID, &pItem );
        if( NULL == pItem ) {
            return;
        }

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, m_pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }
}

void KGCGameInvenEquipSlot::SetMouseBound( bool bSet_ )
{
    m_bMouseBound = bSet_;
}