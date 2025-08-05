#include "stdafx.h"
#include "GCBuyCashItemDlgS6.h"

IMPLEMENT_CLASSNAME(KGCBuyCashItemDlgS6);
IMPLEMENT_WND_FACTORY(KGCBuyCashItemDlgS6);
IMPLEMENT_WND_FACTORY_NAME(KGCBuyCashItemDlgS6, "gc_buy_cash_item_s6");

KGCBuyCashItemDlgS6::KGCBuyCashItemDlgS6(void)
{
    m_pkBtnClose = NULL;
    m_pkBtnCancel = NULL;
    m_pkBtnBuy = NULL;
    LINK_CONTROL("btn_close", m_pkBtnClose);
    LINK_CONTROL("btn_cancel", m_pkBtnCancel);
    LINK_CONTROL("btn_buy", m_pkBtnBuy);

    m_pkStaticItemName = NULL;
    m_pkStaticItemDesc = NULL;
    LINK_CONTROL("static_item_name", m_pkStaticItemName);
    LINK_CONTROL("static_item_desc", m_pkStaticItemDesc);

    //m_pkStaticGC1 = NULL;
    //m_pkStaticGC2 = NULL;
    //m_pkStaticGC3 = NULL;
    //LINK_CONTROL( "static_GC1", m_pkStaticGC1 );
    //LINK_CONTROL( "static_GC2", m_pkStaticGC2 );
    //LINK_CONTROL( "static_GC3", m_pkStaticGC3 );

    m_pkStaticNecessaryDesc = NULL;
    m_pkStaticCurrentDesc = NULL;
    m_pkStaticNotEnoughDesc = NULL;
    LINK_CONTROL("static_gc_necessary_desc", m_pkStaticNecessaryDesc);
    LINK_CONTROL("static_gc_current_desc", m_pkStaticCurrentDesc);
    LINK_CONTROL("static_gc_not_enough_desc", m_pkStaticNotEnoughDesc);

    m_pkStaticNecessary = NULL;
    m_pkStaticCurrent = NULL;
    m_pkStaticNotEnough = NULL;
    LINK_CONTROL("static_gc_necessary", m_pkStaticNecessary);
    LINK_CONTROL("static_gc_current", m_pkStaticCurrent);
    LINK_CONTROL("static_gc_not_enough", m_pkStaticNotEnough);

    m_pkItemImgWnd = NULL;
    m_pkSetItemImgWnd = NULL;
    LINK_CONTROL("item_img", m_pkItemImgWnd);
    LINK_CONTROL("setitem_img", m_pkSetItemImgWnd);

}

KGCBuyCashItemDlgS6::~KGCBuyCashItemDlgS6(void)
{
}

void KGCBuyCashItemDlgS6::OnCreate(void)
{
    m_pkBtnClose->InitState(true, true, this);
    m_pkBtnClose->SetHotKey(DIK_ESCAPE);

    m_pkBtnCancel->InitState(true, true, this);
    m_pkBtnBuy->InitState(true, true, this);
    m_pkBtnBuy->SetHotKey(DIK_RETURN);

    m_pkStaticItemName->InitState(true);
    m_pkStaticItemName->SetAlign(DT_CENTER);
    m_pkStaticItemName->SetFontSize(SiKGCMultipleLanguages()->GetMiddleFontSize());
    m_pkStaticItemDesc->InitState(true);

    m_pkStaticNecessaryDesc->InitState(true);
    m_pkStaticCurrentDesc->InitState(true);
    m_pkStaticNotEnoughDesc->InitState(true);

    m_pkStaticNecessary->InitState(true);
    m_pkStaticNecessary->SetAlign(DT_RIGHT);
    m_pkStaticCurrent->InitState(true);
    m_pkStaticCurrent->SetAlign(DT_RIGHT);
    m_pkStaticCurrent->SetFontColor(D3DCOLOR_ARGB(255, 0, 255, 0)); // 녹색
    m_pkStaticNotEnough->InitState(true);
    m_pkStaticNotEnough->SetAlign(DT_RIGHT);
    m_pkStaticNotEnough->SetFontColor(D3DCOLOR_ARGB(255, 255, 0, 0)); // 빨강

    m_pkItemImgWnd->InitState(false);
    m_pkSetItemImgWnd->InitState(false);

}

void KGCBuyCashItemDlgS6::OnCreateComplete()
{
    SetGoodsInfo();
}

void KGCBuyCashItemDlgS6::ActionPerformed(const KActionEvent& event)
{
    _GCWND_MSG_MAP(m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose);
    _GCWND_MSG_MAP(m_pkBtnCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose);
    _GCWND_MSG_MAP(m_pkBtnBuy, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBuy);

}

void KGCBuyCashItemDlgS6::FrameMoveInEnabledState(void)
{
}

void KGCBuyCashItemDlgS6::OnClickClose()
{
    SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCBuyCashItemDlgS6::OnClickBuy()
{
    if (g_kGlobalValue.m_dwVirtualCash >= m_kItem.dwPrice) {
        SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_11), L"",
            KGCUIScene::GC_MBOX_USE_BUY_CASH_ITEM_BY_GC, 0, 0, false, true);
    }
    else { // 가상캐쉬가 부족합니다.
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_10), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
    }
}

void KGCBuyCashItemDlgS6::SetGoodsInfo()
{
    // 현재 상품에 대한 정보를 얻자
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData(g_pItemMgr->GetCurrentGoodsID());

    if (pkItem == NULL)
        return;

    // 아이템 정보 저장
    m_kItem = *pkItem;

    // 아이템 이름 & 설명
    m_pkStaticItemName->SetText(m_kItem.strItemName);
    m_pkStaticItemDesc->SetTextAutoMultiline(m_kItem.strItemDesc);

    // GC
    //m_pkStaticGC1->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );
    //m_pkStaticGC2->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );
    //m_pkStaticGC3->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );

    // 스트링
    m_pkStaticNecessaryDesc->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_5));
    m_pkStaticCurrentDesc->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_6));
    m_pkStaticNotEnoughDesc->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_7));

    // 금액
    m_pkStaticNecessary->SetText(m_kItem.dwPrice); // 가상캐쉬로 사도 캐쉬가격이랑 같음
    m_pkStaticCurrent->SetText(g_kGlobalValue.m_dwVirtualCash);
    int nNotEnough = static_cast<int>(m_kItem.dwPrice - g_kGlobalValue.m_dwVirtualCash);
    if (nNotEnough > 0)
        m_pkStaticNotEnough->SetText(nNotEnough);
    else
        m_pkStaticNotEnough->SetText(0);

    // 아이템 이미지
    if (m_kItem.dwSlotPosition & ESP_SET_ITEM)
    {
        m_pkSetItemImgWnd->ToggleRender(true);
        m_pkSetItemImgWnd->SetItemInfo(m_kItem.iSetItemNumber, m_kItem.dwGoodsID, 150.f, 150.f);
        m_pkSetItemImgWnd->ShowItem(true);
        m_pkItemImgWnd->ToggleRender(false);
        m_pkItemImgWnd->ShowItem(true);
    }
    else {
        m_pkItemImgWnd->ToggleRender(true);
        m_pkItemImgWnd->SetItemInfo(m_kItem.dwGoodsID);
        m_pkItemImgWnd->ShowItem(true);
        m_pkSetItemImgWnd->ToggleRender(false);
        m_pkSetItemImgWnd->ShowItem(true);
    }
}

void KGCBuyCashItemDlgS6::BuyCurrentItem()
{
    g_pItemMgr->SendBuyForVirtualCashItem( &m_kItem );
}