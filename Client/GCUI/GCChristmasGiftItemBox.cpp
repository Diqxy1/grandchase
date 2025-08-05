#include "stdafx.h"
#include "GCChristmasGiftItemBox.h"

IMPLEMENT_CLASSNAME( KGCChristmasGiftItemBox );
IMPLEMENT_WND_FACTORY( KGCChristmasGiftItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasGiftItemBox, "gc_christmas_gift_item_box" );

KGCChristmasGiftItemBox::KGCChristmasGiftItemBox( void )
{
    m_pkBtnSend = NULL;
    m_pkPriceNum = NULL;
    m_pkGiftItemImgWnd = NULL;
    LINK_CONTROL( "btn_send", m_pkBtnSend );
    LINK_CONTROL( "price_num", m_pkPriceNum );
    LINK_CONTROL( "item_img_wnd", m_pkGiftItemImgWnd );

    m_nIndex = 0;
    m_dwItemID = 0;
    m_bMouseBound = false;
    m_dwSledgeUseCount = 0;
}

KGCChristmasGiftItemBox::~KGCChristmasGiftItemBox( void )
{
}

void KGCChristmasGiftItemBox::OnCreate( void )
{
    m_pkBtnSend->InitState( true, true, this );
    m_pkPriceNum->InitState( true, false );
    m_pkGiftItemImgWnd->InitState( true, true, this  );

    m_bMouseBound = false;
}

void KGCChristmasGiftItemBox::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnSend,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSendGift );
    
    GCWND_MSG_MAP( m_pkGiftItemImgWnd, KD3DWnd::D3DWE_CURSOR_ENTER, OnMouseEnter );
    GCWND_MSG_MAP( m_pkGiftItemImgWnd, KD3DWnd::D3DWE_CURSOR_LEAVE, OnMouseLeave );
}

void KGCChristmasGiftItemBox::PostChildDraw( void )
{
    if( g_pkUIScene->m_pkChristmasGiftDlg->IsSubDlgRenderOn() == false )
        ShowItemInfo();
}

void KGCChristmasGiftItemBox::OnClickSendGift()
{
    g_KDSound.Play( "31" );

    // 리본 갯수 0이면 설정 안되어 있는 칸 
    if( m_dwSledgeUseCount == 0 ) { 
        return;
    }

    if( m_dwSledgeUseCount > g_pkUIScene->m_pkChristmasGiftDlg->GetCurrentSledgeCount() ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_RIBBON) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);
    } else {
        g_pkUIScene->m_pkChristmasGiftDlg->ActivateSendGiftItemDlg( m_dwItemID );
    }
}

void KGCChristmasGiftItemBox::SetGiftItemBoxInfo( GCITEMID dwItemID, DWORD dwSledgeUseCount )
{
    m_dwItemID = dwItemID;
    m_pkPriceNum->SetNumber( dwSledgeUseCount );
    m_dwSledgeUseCount = dwSledgeUseCount;

    m_pkGiftItemImgWnd->SetItemInfo( dwItemID / 10 );
    m_pkGiftItemImgWnd->ShowItem ( true );
}

void KGCChristmasGiftItemBox::ShowItemInfo( void )
{
    if( m_bMouseBound ) {
        GCItem* pItem = g_pItemMgr->GetItemData( m_dwItemID / 10 );

        if( NULL == pItem ) { 
            return;
        }

        if( pItem->dwGoodsID == KItem::INVALID_ITEM ) { 
            return;
        }

        D3DXVECTOR2 dxvPos = this->GetFixedWindowLocalPos();
        dxvPos.x += ( 20.0f * g_pGCDeviceManager->GetWindowScaleX() );
        dxvPos.y += ( 290.0f * g_pGCDeviceManager->GetWindowScaleY() );

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }
}

void KGCChristmasGiftItemBox::OnMouseEnter( void )
{
    m_bMouseBound = true;
}

void KGCChristmasGiftItemBox::OnMouseLeave( void )
{
    m_bMouseBound = false;
}
