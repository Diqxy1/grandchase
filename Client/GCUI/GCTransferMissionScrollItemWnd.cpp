#include "stdafx.h"
#include "GCTransferMissionScrollItemWnd.h"

IMPLEMENT_CLASSNAME( KGCTransferMissionScrollItemWnd );
IMPLEMENT_WND_FACTORY( KGCTransferMissionScrollItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCTransferMissionScrollItemWnd, "gc_mission_scroll_item_wnd" );

KGCTransferMissionScrollItemWnd::KGCTransferMissionScrollItemWnd( void )
{
    m_pkBtnBuy = NULL;
    LINK_CONTROL( "buy_button", m_pkBtnBuy );

    m_pkIconGP = NULL;
    LINK_CONTROL( "gp", m_pkIconGP );

    m_pkIconCash = NULL;
    LINK_CONTROL( "cash", m_pkIconCash );

    m_pkMissionItemImgWnd = NULL;
    LINK_CONTROL( "item_img_wnd", m_pkMissionItemImgWnd );

    m_pkStaticItemName = NULL;
    LINK_CONTROL( "item_name_static", m_pkStaticItemName );

    m_pkStaticItemPrice = NULL;
    LINK_CONTROL( "item_price_static", m_pkStaticItemPrice );

    m_pkStaticItemDesc = NULL;
    LINK_CONTROL( "item_desc_static", m_pkStaticItemDesc );

    //
    m_dwItemID = 0;

}

KGCTransferMissionScrollItemWnd::~KGCTransferMissionScrollItemWnd( void )
{
}

void KGCTransferMissionScrollItemWnd::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnBuy,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBuy );
    _GCWND_MSG_MAP( m_pkMissionItemImgWnd,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickImgWnd );
}

void KGCTransferMissionScrollItemWnd::OnCreate( void )
{
    m_pkBtnBuy->InitState( true, true, this );
    m_pkIconGP->InitState ( false );
    m_pkIconCash->InitState ( false );

    m_pkMissionItemImgWnd->InitState( true, true, this );

    m_pkStaticItemName->InitState( true );
    m_pkStaticItemName->SetFontOutline( true );
    m_pkStaticItemName->SetFontColor( 0xffffcc99 );
    m_pkStaticItemName->SetAutoScroll( true );

    m_pkStaticItemPrice->InitState( true );
    m_pkStaticItemPrice->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );

    m_pkStaticItemDesc->InitState( true );
    m_pkStaticItemDesc->SetMultiLine( true );

    SetItemInfo();
}

void KGCTransferMissionScrollItemWnd::FrameMoveInEnabledState( void )
{
    //
}

void KGCTransferMissionScrollItemWnd::SetItemInfo( )
{
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwItemID );

    if( pkItem == NULL )
        return;

    if( pkItem->eMoneyType == EMT_GAMEPOINT ){
        m_pkIconGP->ToggleRender( true );
        m_pkIconCash->ToggleRender( false );
    } else if( pkItem->eMoneyType == EMT_CASH ) {
        m_pkIconGP->ToggleRender( false );
        m_pkIconCash->ToggleRender( true );
    } else {
        m_pkIconGP->ToggleRender( false );
        m_pkIconCash->ToggleRender( false );
    }

    m_pkMissionItemImgWnd->SetItemInfo( m_dwItemID );
    m_pkMissionItemImgWnd->ShowItem ( true );

    m_pkStaticItemName->SetText( pkItem->strItemName );
    m_pkStaticItemPrice->SetText( pkItem->dwPrice );

    if( pkItem->eMoneyType == EMT_GAMEPOINT ){
        m_pkStaticItemDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_0 ) );
    } else if( pkItem->eMoneyType == EMT_CASH ) {
        m_pkStaticItemDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_1 ) );
    }
}

void KGCTransferMissionScrollItemWnd::OnClickBuy()
{
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwItemID );

    if( pkItem == NULL )
        return;

    if( pkItem->eMoneyType == EMT_GAMEPOINT ) {
        g_pItemMgr->BuyGPItem( pkItem, pkItem->dwGoodsID, pkItem->iItemTypeValue );
    }
    else if( pkItem->eMoneyType == EMT_CASH ) { 
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( pkItem->dwGoodsID );
#else
    #if defined( NATION_KOREA ) 
        g_pItemMgr->BuyItem( pkItem->dwGoodsID, pkItem->iItemTypeValue );
    #else
        g_pkUIScene->EnableBuyCashItemWnd( m_dwItemID, false, true );
    #endif
#endif
    }
}

void KGCTransferMissionScrollItemWnd::OnClickImgWnd()
{
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwItemID );

    if( pkItem == NULL )
        return;

    if( pkItem->eItemKind == GIK_MISSION_SCROLL )
    {
        g_pkUIScene->EndMessageBox();
        g_pkUIScene->m_pkMissionUI_S4->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwItemID * 10, true, true, true);
    }
}