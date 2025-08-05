#include "stdafx.h"
#include "GCItemPopupMenu.h"

IMPLEMENT_CLASSNAME( KGCItemPopupMenu );
IMPLEMENT_WND_FACTORY( KGCItemPopupMenu );
IMPLEMENT_WND_FACTORY_NAME( KGCItemPopupMenu, "gc_itempopupmenu" );


KGCItemPopupMenu::KGCItemPopupMenu( void )
: m_pkBtnMoveDepot( NULL ) 
, m_pkBtnSellItem( NULL ) 
, m_itemUID( 0 )
{
    LINK_CONTROL( "move_depot", m_pkBtnMoveDepot );
    LINK_CONTROL( "sel_item", m_pkBtnSellItem );
}

KGCItemPopupMenu::~KGCItemPopupMenu( void )
{
}

void KGCItemPopupMenu::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnMoveDepot, KD3DWnd::D3DWE_BUTTON_CLICK, OnMoveDepot );
    _GCWND_MSG_MAP( m_pkBtnSellItem, KD3DWnd::D3DWE_BUTTON_CLICK, OnSellItem );
}

void KGCItemPopupMenu::OnCreate( void )
{
    m_pkBtnMoveDepot->InitState( true, true, this );
    m_pkBtnSellItem->InitState( true, true, this );
}

void KGCItemPopupMenu::OnMoveDepot()
{
    bool bActive =  SiKGCVirtualDepotManager()->IsEmptyItemList();

    if( !bActive )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MSG2) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
        return;
    }
    

    if ( m_itemUID == 0 ) 
        return;

    KItem* pkItem = g_pItemMgr->GetInventoryItem( m_itemUID );
    if ( !pkItem ) 
        return;

    GCItem* pItem = g_pItemMgr->GetItemData( pkItem->m_ItemID );
    if ( !pItem ) 
        return;

    if( pItem->eItemType == GCIT_COUNT )
    {
        SiKGCDepotManager()->OnQuantityDepotMove(m_itemUID, 3);
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_itemUID, KGCSellCountItemDlg::EM_MODE_INSERT_DEPOT_FROM_INVENTORY );
    }
    else
    {
        InsertDepotItem( pkItem, m_itemUID );
    }

    ToggleRender( false );

    return;
}

void KGCItemPopupMenu::OnSellItem()
{

    if ( m_itemUID == 0 ) 
        return;

    KItem* pkItem = g_pItemMgr->GetInventoryItem( m_itemUID );
    if ( !pkItem ) 
        return;

    GCItem* pItem = g_pItemMgr->GetItemData( pkItem->m_ItemID );
    if ( !pItem ) 
        return;



    if( pItem->eItemType == GCIT_COUNT )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_itemUID, KGCSellCountItemDlg::EM_MODE_SELL_INVEN );      
       
    }
    else
    {       
        g_pkUIScene->m_pkMyInfoScene->SellInvenItem( *pkItem );
    }    


    ToggleRender( false );

    return;
}

void KGCItemPopupMenu::OpenPopupMenu( GCITEMUID _itemUID, D3DXVECTOR2 vecPos )
{
    ToggleRender( true );

    const float nGAP_X = 5.0f;
    const float nGAP_Y = 5.0f;

    m_itemUID = _itemUID;

    vecPos.x += nGAP_X;
    vecPos.y += nGAP_Y;
    
    SetWindowPosDirect( vecPos );
    AdjustIntoScreen();
}

void KGCItemPopupMenu::InsertDepotItem( KItem* pkItem, GCITEMUID _itemUID )
{
    //  창고에 넣을수 있는 아이템인지 확인
    GCItem* pItem = g_pItemMgr->GetItemData( pkItem->m_ItemID );
    if ( !SiKGCDepotManager()->IsInsertDepotItem( pItem, pkItem ) ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15) , L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
        return;
    }

    bool bOK = SiKGCDepotManager()->InsertDepotItem( pkItem );

    //  창고가 가득 찼습니다.
    if ( !bOK ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_16) , L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
    }

    Result_DepotUpdateItemReq = INT_MAX;
    KP2P::GetInstance()->Send_DepotDBUpdateReq();
    g_MyD3D->WaitForServerAck( Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE );
}