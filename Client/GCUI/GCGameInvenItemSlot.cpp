#include "stdafx.h"
#include "GCGameInvenItemSlot.h"

IMPLEMENT_CLASSNAME( KGCGameInvenItemSlot );
IMPLEMENT_WND_FACTORY( KGCGameInvenItemSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCGameInvenItemSlot, "gc_ongame_item_slot" );

KGCGameInvenItemSlot::KGCGameInvenItemSlot( void )
{
    m_pkBack = NULL;
    m_pkActive = NULL;
    m_pkNewIcon = NULL;
    m_pkHotIcon = NULL;
    m_pkItemImg = NULL;
    LINK_CONTROL( "back",    m_pkBack );
    LINK_CONTROL( "active",  m_pkActive );
    LINK_CONTROL( "newIcon", m_pkNewIcon );
    LINK_CONTROL( "hotIcon", m_pkHotIcon );
    LINK_CONTROL( "itemImg", m_pkItemImg );

    m_pItem = NULL;
    m_bNew = false;
    m_bHot = false;
}

KGCGameInvenItemSlot::~KGCGameInvenItemSlot( void )
{

}

void KGCGameInvenItemSlot::OnCreate( void )
{
    m_bNew = false;
    m_bHot = false;

    m_pkBack->InitState( true, true, this );
    m_pkActive->InitState( false );
    m_pkNewIcon->InitState( false );
    m_pkHotIcon->InitState( false );
    m_pkItemImg->InitState( true );
}

void KGCGameInvenItemSlot::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBack && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
        SetActiveFrame( true );
    }

    if( event.m_pWnd == m_pkBack && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
        SetActiveFrame( false );
    }

    GCWND_MSG_MAP( m_pkBack, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSlot );
}

void KGCGameInvenItemSlot::Clear( void )
{
    m_pkActive->ToggleRender( false );
    m_pkNewIcon->ToggleRender( false );
    m_pkHotIcon->ToggleRender( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
    m_pItem = NULL;
}

void KGCGameInvenItemSlot::SetItemInfo( GCITEMUID itemUID_, bool bNew_ /*= false*/, bool bHot_ /*= false*/ )
{
    Clear();

    g_pItemMgr->FindInventory( itemUID_, &m_pItem );
    if( NULL == m_pItem ) { 
        return; 
    }

    m_pkItemImg->SetItemInfo( m_pItem->m_ItemID );
    m_pkItemImg->ShowItem( true );

    m_bNew = bNew_;
    m_bHot = bHot_;

    if( m_bNew ) { 
        m_pkNewIcon->ToggleRender( true );
    }
    else if( m_bHot ) { 
        m_pkHotIcon->ToggleRender( true );
    }
}

void KGCGameInvenItemSlot::SetActiveFrame( bool bSet_ )
{
    m_pkActive->ToggleRender( bSet_ );
}

void KGCGameInvenItemSlot::OnClickSlot( void )
{
    if( NULL == m_pItem ) { 
        return;
    }

    SpeakToActionListener( KActionEvent(this, D3DWE_BUTTON_CLICK, m_pItem->m_ItemUID ) );  
}

void KGCGameInvenItemSlot::FrameMoveInEnabledState( void )
{
    if( m_pkActive->IsRenderOn() && m_pItem ) { 

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