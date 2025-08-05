#include "stdafx.h"
#include "GCBuyCashItemBuyCompleteDlg.h"

IMPLEMENT_CLASSNAME( KGCBuyCashItemBuyCompleteDlg );
IMPLEMENT_WND_FACTORY( KGCBuyCashItemBuyCompleteDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCBuyCashItemBuyCompleteDlg, "gc_buy_complete" );

KGCBuyCashItemBuyCompleteDlg::KGCBuyCashItemBuyCompleteDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnOk = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_ok", m_pkBtnOk );

    m_pkStaticItemName = NULL;
    //m_pkStaticGC1 = NULL;
    //m_pkStaticGC2 = NULL;
    LINK_CONTROL( "static_item_name", m_pkStaticItemName );
    //LINK_CONTROL( "static_GC1", m_pkStaticGC1 );
    //LINK_CONTROL( "static_GC2", m_pkStaticGC2 );

    m_pkStaticUsedDesc = NULL;
    m_pkStaticCurrentDesc = NULL;
    m_pkStaticUsed = NULL;
    m_pkStaticCurrent = NULL;
    LINK_CONTROL( "static_gc_used_desc", m_pkStaticUsedDesc );
    LINK_CONTROL( "static_gc_current_desc", m_pkStaticCurrentDesc );
    LINK_CONTROL( "static_gc_used", m_pkStaticUsed );
    LINK_CONTROL( "static_gc_current", m_pkStaticCurrent );

    m_pkItemImgWnd = NULL;
    m_pkSetItemImgWnd = NULL;
    LINK_CONTROL( "item_img", m_pkItemImgWnd );
    LINK_CONTROL( "setitem_img", m_pkSetItemImgWnd );

}

KGCBuyCashItemBuyCompleteDlg::~KGCBuyCashItemBuyCompleteDlg( void )
{
}

void KGCBuyCashItemBuyCompleteDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey ( DIK_ESCAPE );

    m_pkBtnOk->InitState( true, true, this );
    m_pkBtnOk->SetHotKey( DIK_RETURN );

    m_pkStaticItemName->InitState( true );
    m_pkStaticItemName->SetAlign( DT_CENTER );
    m_pkStaticItemName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );

    //m_pkStaticGC1->InitState( true );
    //m_pkStaticGC2->InitState( true );

    m_pkStaticUsedDesc->InitState( true );
    m_pkStaticCurrentDesc->InitState( true );
    m_pkStaticUsed->InitState( true );
    m_pkStaticUsed->SetAlign( DT_RIGHT );
    m_pkStaticCurrent->InitState( true );
    m_pkStaticCurrent->SetAlign( DT_RIGHT );
    m_pkStaticCurrent->SetFontColor( D3DCOLOR_ARGB(255, 0, 255, 0) ); // 녹색

    m_pkItemImgWnd->InitState( false );
    m_pkSetItemImgWnd->InitState( false );

}

void KGCBuyCashItemBuyCompleteDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    _GCWND_MSG_MAP( m_pkBtnOk,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOk );

}

void KGCBuyCashItemBuyCompleteDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCBuyCashItemBuyCompleteDlg::OnClickOk()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

bool KGCBuyCashItemBuyCompleteDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->SetBuyResultInfo( static_cast<DWORD>( kInfo_.m_lParam ) );
    this->SetWindowZOrder( D3DWZ_TOPMOST );
    return true;
}

void KGCBuyCashItemBuyCompleteDlg::SetBuyResultInfo( IN DWORD nUsedGC )
{
    // 서버에서 준 아이템 리스트 안 쓰는 이유는 패키지 이미지 때문임
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( g_pItemMgr->GetCurrentGoodsID() );

    if( pkItem == NULL )
        return;

    m_pkStaticItemName->SetTextAutoMultiline( pkItem->strItemName );
    
    //m_pkStaticGC1->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );
    //m_pkStaticGC2->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );

    m_pkStaticUsedDesc->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_9 ) );
    m_pkStaticCurrentDesc->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_6 ) );
    m_pkStaticUsed->SetText( static_cast<int>(nUsedGC) );
    m_pkStaticCurrent->SetText( static_cast<int>(g_kGlobalValue.m_dwVirtualCash) );

    // 아이템 이미지
    if ( pkItem->dwSlotPosition & ESP_SET_ITEM )
    {
        m_pkSetItemImgWnd->ToggleRender( true );
        m_pkSetItemImgWnd->SetItemInfo( pkItem->iSetItemNumber, pkItem->dwGoodsID , 150.f, 150.f);
        m_pkSetItemImgWnd->ShowItem( true );
        m_pkItemImgWnd->ToggleRender( false );
        m_pkItemImgWnd->ShowItem( true );
    } else {
        m_pkItemImgWnd->ToggleRender( true );
        m_pkItemImgWnd->SetItemInfo( pkItem->dwGoodsID );
        m_pkItemImgWnd->ShowItem( true );
        m_pkSetItemImgWnd->ToggleRender( false );
        m_pkSetItemImgWnd->ShowItem( true );
    }
}

