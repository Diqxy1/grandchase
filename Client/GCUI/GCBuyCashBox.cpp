#include "StdAfx.h"
#include "GCItemImgWnd.h"
#include "GCBuyCashBox.h"
#include "GCItemManager.h"

#define MAX_GIFT_MSG_SIZE 60

IMPLEMENT_CLASSNAME(KGCBuyCashBox);
IMPLEMENT_WND_FACTORY(KGCBuyCashBox);
IMPLEMENT_WND_FACTORY_NAME(KGCBuyCashBox, "gc_kgcbuycashbox");

KGCBuyCashBox::KGCBuyCashBox(void)
{
    m_pkItemPriceLabel = NULL;
    m_pkCurrentCashLabel = NULL;
    m_pkVirtualCashLabel = NULL;
    m_pkUseVirtualCash = NULL;
    m_pkMixVirtualCash = NULL;

    m_pkCalcItemPriceLabel = NULL;
    m_pkCalcVirtualCashLabel = NULL;
    m_pkCalcPercentCashLabel = NULL;
    m_pkCalcResultCashLabel = NULL;

    m_pkItemImg = NULL;

    m_pkCheckUseVirtualCash = NULL;
    m_pkCheckMixVirtualCash = NULL;

    m_pkComboItemPeriod = NULL;
    m_pkOK = NULL;
    m_pkCancle = NULL;

    m_pkMainframe = NULL;
    m_pkInnerBG = NULL;
    m_pkCalcBG = NULL;

    LINK_CONTROL("label_item_price", m_pkItemPriceLabel);
    LINK_CONTROL("label_current_cash", m_pkCurrentCashLabel);
    LINK_CONTROL("label_current_virtual_cash", m_pkVirtualCashLabel);
    LINK_CONTROL("static_use_virtual_cash", m_pkUseVirtualCash);
    LINK_CONTROL("static_mix_virtual_cash", m_pkMixVirtualCash);

    LINK_CONTROL("label_calc_item_price", m_pkCalcItemPriceLabel);
    LINK_CONTROL("label_calc_virtual_cash", m_pkCalcVirtualCashLabel);
    LINK_CONTROL("label_percent_of_cash", m_pkCalcPercentCashLabel);
    LINK_CONTROL("label_calc_result_cash", m_pkCalcResultCashLabel);

    LINK_CONTROL("img_item", m_pkItemImg);

    LINK_CONTROL("checkbox_use_virtualcash", m_pkCheckUseVirtualCash);
    LINK_CONTROL("checkbox_mix_virtualcash", m_pkCheckMixVirtualCash);

    LINK_CONTROL("price_combobox", m_pkComboItemPeriod);

    LINK_CONTROL("btn_ok", m_pkOK);
    LINK_CONTROL("btn_cancle", m_pkCancle);

    LINK_CONTROL("mainframe", m_pkMainframe);
    LINK_CONTROL("bg_inner", m_pkInnerBG);
    LINK_CONTROL("bg_calc", m_pkCalcBG);

#if defined(NATION_TAIWAN)
    m_pk_use_Gift = NULL;
    m_pkCheckUseGift = NULL;

    m_pkGiftBG = NULL;

    m_pkGiftNicknameBG = NULL;
    m_pkGiftCommentBG = NULL;

    m_pEditGiftID = NULL;
    m_pEditGiftComment = NULL;

    m_pkFontImgNickname = NULL;

    LINK_CONTROL("static_use_gift", m_pk_use_Gift);
    LINK_CONTROL("checkbox_use_gift", m_pkCheckUseGift);
    LINK_CONTROL("bg_gift", m_pkGiftBG);
    LINK_CONTROL("bg_gift_nickname", m_pkGiftNicknameBG);
    LINK_CONTROL("bg_gift_comment", m_pkGiftCommentBG);
    LINK_CONTROL("edit_gift_id", m_pEditGiftID);
    LINK_CONTROL("edit_gift_commet", m_pEditGiftComment);
    LINK_CONTROL("gift_nickname", m_pkFontImgNickname);
#endif

    m_dwItemID = 0;
    m_iOriginalPrice = 0;
    m_iDiscount = 0;
    m_iResultPrice = 0;
    m_pkItem = NULL;

}

KGCBuyCashBox::~KGCBuyCashBox(void)
{
}

void KGCBuyCashBox::OnCreate(void)
{
    std::wostringstream strPercentTextStream;
    std::wstring strPercentText;

    m_pkItemPriceLabel->InitState(true);
    m_pkItemPriceLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX01));
    m_pkCurrentCashLabel->InitState(true);
    m_pkCurrentCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX02));
    m_pkVirtualCashLabel->InitState(true);
    m_pkVirtualCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX03));


    m_pkCalcItemPriceLabel->InitState(true);
    m_pkCalcItemPriceLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX04));
    m_pkCalcVirtualCashLabel->InitState(true);
    m_pkCalcVirtualCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX05));

#if defined( NATION_TAIWAN )
    m_pkCalcResultCashLabel->InitState(true);
    m_pkItemImg->InitState(true);
#if defined( BUY_CASHITEM_WITH_VIRTUAL_CASH ) 
    m_pkMixVirtualCash->InitState(true);
    m_pkMixVirtualCash->SetText(g_pkStrLoader->GetString(STR_ID_BUY_USE_VURTUAL_GASH));
    m_pkCheckMixVirtualCash->InitState(true, true, this);
    m_pkCheckMixVirtualCash->SetCheck(false);
#else
    m_pkMixVirtualCash->InitState(false);
    m_pkCheckMixVirtualCash->InitState(false);
#endif
#else
    m_pkCalcResultCashLabel->InitState(true);
    m_pkCalcResultCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX06));

    m_pkItemImg->InitState(true);

    m_pkUseVirtualCash->InitState(true);
    m_pkUseVirtualCash->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX08));
    m_pkCheckUseVirtualCash->SetCheck(false);
    m_pkCheckUseVirtualCash->InitState(true, true, this);

#if defined( BUY_CASHITEM_MIX_VIRTUAL_CASH )
    m_pkMixVirtualCash->InitState(true);
    m_pkMixVirtualCash->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX09));
    m_pkCheckMixVirtualCash->SetCheck(false);
    m_pkCheckMixVirtualCash->InitState(true, true, this);
#else
    m_pkMixVirtualCash->InitState(false);
    m_pkCheckMixVirtualCash->InitState(false);
#endif
#endif

    m_pkComboItemPeriod->InitState(true, true, this);

    m_pkOK->InitState(true, true, this);
    m_pkCancle->InitState(true, true, this);

    m_pkMainframe->InitState(true);
    m_pkInnerBG->InitState(true);
    m_pkCalcBG->InitState(true);

#if defined(NATION_TAIWAN)
    m_pk_use_Gift->InitState(true);
    m_pk_use_Gift->SetText(g_pkStrLoader->GetString(STR_ID_GIFT_CASH_ITEM));
    m_pkCheckUseGift->InitState(true, true, this);
    m_pkCheckUseGift->SetCheck(false);

    m_pEditGiftID->InitState(true, false, this);
    m_pEditGiftComment->InitState(true, false, this);

    m_pkFontImgNickname->InitState(true);

    // 아이디 입력창에 각 나라별 디비에 저장되는 아이디 길이만큼만 입력할수있게 한다
    m_pEditGiftID->SetLimitText(MAX_LOGIN_ID_SIZE);
    m_pEditGiftComment->SetLimitText(MAX_GIFT_MSG_SIZE);
#endif

    CalcResultPrice();
}

void KGCBuyCashBox::OnCreateComplete()
{
#if defined(NATION_TAIWAN)
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT && g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
    {
        m_pkCheckUseGift->ToggleRender(false);
        m_pk_use_Gift->ToggleRender(false);
    }
    else
    {
        m_pkCheckUseGift->ToggleRender(true);
        m_pk_use_Gift->ToggleRender(true);
    }
#endif
}

void KGCBuyCashBox::FrameMoveInEnabledState(void)
{

}

void KGCBuyCashBox::PostChildDraw(void)
{
    g_pkUIMgr->RenderNumber("very_small_num", D3DXVECTOR2(PRICE_X * m_fWindowScaleX, PRICE_Y * m_fWindowScaleY), m_iOriginalPrice, -1, false, true, -1.0f, true);
    g_pkUIMgr->RenderNumber("very_small_num", D3DXVECTOR2(CURRENT_CASH_X * m_fWindowScaleX, CURRENT_CASH_Y * m_fWindowScaleY), g_kGlobalValue.m_dwCash, -1, false, true, -1.0f, true);
    g_pkUIMgr->RenderNumber("very_small_num", D3DXVECTOR2(CURRENT_VCASH_X * m_fWindowScaleX, CURRENT_VCASH_Y * m_fWindowScaleY), g_kGlobalValue.m_dwVirtualCash, -1, false, true, -1.0f, true);

#if defined( NATION_TAIWAN)
    g_pkUIMgr->RenderNumber("orange_gradation_num", D3DXVECTOR2(CALC_RESULT_ONLY_X * m_fWindowScaleX, CALC_RESULT_ONLY_Y * m_fWindowScaleY), m_iResultPrice, -1, false, true, -1.0f, true);
#else 
    if (m_iDiscount == 0)
    {
        g_pkUIMgr->RenderNumber("orange_gradation_num", D3DXVECTOR2(CALC_RESULT_ONLY_X * m_fWindowScaleX, CALC_RESULT_ONLY_Y * m_fWindowScaleY), m_iResultPrice, -1, false, true, -1.0f, true);
    }
    else
    {
        g_pkUIMgr->RenderNumber("blue_gradation_num", D3DXVECTOR2(CALC_ORIGINAL_PRICE_X * m_fWindowScaleX, CALC_ORIGINAL_PRICE_Y * m_fWindowScaleY), m_iOriginalPrice, -1, false, true, -1.0f, true);
        g_pkUIMgr->RenderNumber("red_gradation_num", D3DXVECTOR2(CALC_DISCOUNT_X * m_fWindowScaleX, CALC_DISCOUNT_Y * m_fWindowScaleY), m_iDiscount, -1, false, true, -1.0f, true);
        g_pkUIMgr->RenderNumber("orange_gradation_num", D3DXVECTOR2(CALC_RESULT_X * m_fWindowScaleX, CALC_RESULT_Y * m_fWindowScaleY), m_iResultPrice, -1, false, true, -1.0f, true);
    }
#endif
}

void KGCBuyCashBox::ActionPerformed(const KActionEvent& event)
{
    _GCWND_MSG_MAP(m_pkOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOK);
    _GCWND_MSG_MAP(m_pkCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancle);

    _GCWND_MSG_MAP(m_pkCheckUseVirtualCash, KD3DWnd::D3DWE_BUTTON_CLICK, OnCheckUseVirtualCash);
    _GCWND_MSG_MAP(m_pkCheckMixVirtualCash, KD3DWnd::D3DWE_BUTTON_CLICK, OnCheckMixVirtualCash);
    _GCWND_MSG_MAP(m_pkComboItemPeriod, KD3DComboBox::D3DWE_COMBO_SELCHANGE, OnChangeItemPeriod);

#if defined( NATION_TAIWAN )
    _GCWND_MSG_MAP(m_pkCheckUseGift, KD3DWnd::D3DWE_BUTTON_CLICK, OnCheckUseGift);
    _GCWND_MSG_MAP(m_pEditGiftID, KD3DWnd::D3DWE_BUTTON_CLICK, m_pEditGiftID->SetFocus);
    _GCWND_MSG_MAP(m_pEditGiftComment, KD3DWnd::D3DWE_BUTTON_CLICK, m_pEditGiftComment->SetFocus);

    if (event.m_pWnd == m_pEditGiftID)
    {
        if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB)
            m_pEditGiftComment->SetFocus();
    }
    else if (event.m_pWnd == m_pEditGiftComment)
    {
        if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB)
            m_pEditGiftID->SetFocus();
    }
#endif
}

void KGCBuyCashBox::OnClickOK()
{
    std::vector<GCItem*>& vecItem = g_pItemMgr->GetCashShopItem(m_dwItemID);
    int iSelectItem = m_pkComboItemPeriod->GetCurSel();
    if (iSelectItem == -1)
        iSelectItem = 0;

    iSelectItem = (vecItem.size() - 1) - iSelectItem;

    m_pkItem = vecItem[iSelectItem];
    RETURN_NIL(m_pkItem);

#if defined( NATION_TAIWAN )
#if defined( BUY_CASHITEM_WITH_VIRTUAL_CASH )
    if (m_pkCheckMixVirtualCash->GetCheck())
    {
        //SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );

        if (m_pkItem->dwPrice <= g_kGlobalValue.m_dwVirtualCash)
        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
            g_pItemMgr->SendBuyForVirtualCashItem(m_pkItem);
        }
        else if (g_kGlobalValue.m_dwVirtualCash > 0)    // 가상캐쉬가 있다면 혼용가능
        {
            // 메세지 창을 띄워 부족한 가상캐쉬만큼 캐쉬를 차감한다고 알린후 확인을 누르면 구매
            if (m_pkItem->dwPrice <= (g_kGlobalValue.m_dwVirtualCash + g_kGlobalValue.m_dwCash))// 단 캐쉬를 합해서 살 수 있는 가격일때만
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_ID_ARE_YOU_SURE_BUY_CASHITEM_MIX_VIRTUAL_CASH)
                    , L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_CASH_INCLUDE_CASH, 0, 0, true, true);
            }
        }
        else
        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT)
                , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        }
    }
    else
#endif
    {
        SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
        bool bPresent = false;

        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT)
        {
            if (!m_pkCheckUseGift->GetCheck()) {
                bPresent = false;
                m_pEditGiftID->SetText(L"");
                m_pEditGiftComment->SetText(L"");
            }
            else
                bPresent = true;
        }
        else
        {
            bPresent = false;
        }

        g_pItemMgr->SendBuyForCashItem(m_pkItem, 0, bPresent, m_pEditGiftID->GetText(), m_pEditGiftComment->GetText());
    }
#else

    if (m_pkCheckUseVirtualCash->GetCheck())
    {
        if (m_pkItem->dwPrice <= g_kGlobalValue.m_dwVirtualCash)
        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
            g_pItemMgr->SendBuyForVirtualCashItem(m_pkItem);
        }
        else
        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT)
                , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        }
    }
    else

#if defined( BUY_CASHITEM_MIX_VIRTUAL_CASH )
        if (m_pkCheckMixVirtualCash->GetCheck())
        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));

            if (m_pkItem->dwPrice <= m_iDiscount)
            {
                g_pItemMgr->SendBuyForVirtualCashItem(m_pkItem);
            }
            else
            {
                g_pItemMgr->SendBuyForCashItem(m_pkItem, m_iDiscount);
            }
        }
        else

#endif

        {
            SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
            g_pItemMgr->SendBuyForCashItem(m_pkItem, 0, false, L"", L"");
        }

#endif

}

void KGCBuyCashBox::OnClickCancle()
{
    SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCBuyCashBox::OnCheckUseVirtualCash()
{
    if ((int)g_kGlobalValue.m_dwVirtualCash < m_iOriginalPrice)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT)
            , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        CalcResultPrice();
        return;
    }
    bool bCheck = m_pkCheckUseVirtualCash->GetCheck();
    m_pkCheckUseVirtualCash->SetCheck(!bCheck);
    if (bCheck == false)
    {
        m_pkCheckMixVirtualCash->SetCheck(false);
    }
    CalcResultPrice();
}

void KGCBuyCashBox::OnCheckMixVirtualCash()
{
    bool bCheck = m_pkCheckMixVirtualCash->GetCheck();

#if !defined( NATION_TAIWAN )
    m_pkCheckMixVirtualCash->SetCheck(!bCheck);
    if (bCheck == false)
    {
        m_pkCheckUseVirtualCash->SetCheck(false);
    }
#else
    if ((int)g_kGlobalValue.m_dwVirtualCash == 0)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT)
            , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        CalcResultPrice();
        return;
    }
    m_pkCheckMixVirtualCash->SetCheck(!bCheck);
    m_pkCheckUseGift->SetCheck(false);
#endif

    CalcResultPrice();
}

void KGCBuyCashBox::SetItemID(DWORD dwItemID)
{
    m_pkComboItemPeriod->DeleteAllItem();
    m_dwItemID = dwItemID;
    GCItem* pkItem = g_pItemMgr->GetItemData(dwItemID);

    std::wostringstream strstream;
    std::vector<GCItem*>& vecItem = g_pItemMgr->GetCashShopItem(pkItem->dwGoodsID);
    for (int i = (int)vecItem.size() - 1; i >= 0; i--)
    {
        if (vecItem[i]->iItemTypeValue != -1)
        {
            strstream << vecItem[i]->dwPrice << L"(" << vecItem[i]->iItemTypeValue <<
                g_pkStrLoader->GetString(pkItem->eItemType == GCIT_PERIOD ? STR_ID_PERIOD_ITEM_DATE : STR_ID_COUNT_ITEM_COUNT) << L")";
        }
        else
        {
            strstream << vecItem[i]->dwPrice << g_pkStrLoader->GetString(STR_ID_NOT_PERIOD_ITEM);
        }

        if (i == (int)vecItem.size() - 1)
        {
            m_pkComboItemPeriod->SetComboStatic(strstream.str(), DT_CENTER);
        }
        m_pkComboItemPeriod->AddString(strstream.str(), DT_CENTER);
        strstream.str(L"");
    }

    m_pkItemImg->SetItemInfo(dwItemID, -1, pkItem->dwSlotPosition & ESP_SET_ITEM, pkItem->iSetItemNumber);
    //m_pkItemPrice->SetText( pkItem->dwPrice );
    m_iOriginalPrice = pkItem->dwPrice;
    m_iDiscount = 0;
    m_iResultPrice = 0;
    CalcResultPrice();
}

void KGCBuyCashBox::OnChangeItemPeriod()
{
    std::vector<GCItem*>& vecItem = g_pItemMgr->GetCashShopItem(m_dwItemID);

    if (vecItem.empty())
    {
        return;
    }

    int iSelectItem = m_pkComboItemPeriod->GetCurSel();
    if (iSelectItem == -1)
        iSelectItem = 0;

    /////////////////////////////////////////////////////////////////////
    // Date :  06-12-21 
    // milennium9@kogstudios.com 권상구
    //*******************************************************************
    // 보이는 순서를 뒤집었기 때문에 선택한 순서도 뒤집어 주어야 합니다.
    /////////////////////////////////////////////////////////////////////
    iSelectItem = (vecItem.size() - 1) - iSelectItem;
    /////////////////////////////////////////////////////////////////////


    GCItem* kItem = vecItem[iSelectItem];
    RETURN_NIL(kItem);

    m_iOriginalPrice = kItem->dwPrice;
    CalcResultPrice();
}

void KGCBuyCashBox::CalcResultPrice()
{
    if (m_pkCheckMixVirtualCash->GetCheck())
    {
#if defined(NATION_TAIWAN)
        if (m_iOriginalPrice <= static_cast<int>(g_kGlobalValue.m_dwVirtualCash))
        {
            m_iDiscount = m_iOriginalPrice;
            m_iResultPrice = m_iOriginalPrice;
        }
        else
        {
            m_iDiscount = static_cast<int>(g_kGlobalValue.m_dwVirtualCash);
            m_iResultPrice = m_iOriginalPrice;
        }

        m_pkCalcResultCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX05));
    }
    else
    {
        m_iDiscount = 0;
        m_iResultPrice = m_iOriginalPrice;

        m_pkCalcResultCashLabel->SetText(g_pkStrLoader->GetString(STR_ID_BUY_CASH_BOX06));
    }
#else
        int AbleDiscPrice = static_cast<int>(m_iOriginalPrice * g_kGlobalValue.m_ivurcashper * 0.01f);

        if (AbleDiscPrice > static_cast<int>(g_kGlobalValue.m_dwVirtualCash))
        {
            m_iDiscount = g_kGlobalValue.m_dwVirtualCash;
            m_iResultPrice = m_iOriginalPrice - m_iDiscount;
        }
        else
        {
            m_iDiscount = AbleDiscPrice;
            m_iResultPrice = m_iOriginalPrice - m_iDiscount;
        }
}
    else
    {
        m_iDiscount = 0;
        m_iResultPrice = m_iOriginalPrice;
    }
#endif

    ResizeWnd();
}

void KGCBuyCashBox::ResizeWnd()
{
#if defined( NATION_TAIWAN )
    if (m_pkCheckUseGift->GetCheck()) // 선물하기 클릭시
    {
        if (!m_pkCheckMixVirtualCash->GetCheck()) //가상캐쉬를 쓰지 않음
        {
            m_pkMainframe->SetSize(m_pkMainframe->GetWidth(), SMALL_MAINFRAME + 70);
            m_pkInnerBG->SetSize(m_pkInnerBG->GetWidth(), SMALL_INNERBG + 70);
            m_pkCalcBG->SetSize(m_pkCalcBG->GetWidth(), SMALL_CALCBG);

            m_pkCalcItemPriceLabel->ToggleRender(false);
            m_pkCalcVirtualCashLabel->ToggleRender(false);
            m_pkCalcResultCashLabel->SetWindowPosDirect(D3DXVECTOR2(CALC_RESULT_LABLE_X, CALC_RESULT_LABLE_Y_AT_SMALL));

            m_pEditGiftID->SetWindowPosY(222);
            m_pEditGiftComment->SetWindowPosY(253);

            m_pkGiftNicknameBG->SetWindowPosY(221);
            m_pkGiftCommentBG->SetWindowPosY(242);

            m_pkGiftBG->SetWindowPosY(219);
            m_pkFontImgNickname->SetWindowPosY(222);
        }

        m_pEditGiftID->ToggleRender(true);
        m_pEditGiftComment->ToggleRender(true);

        m_pkGiftNicknameBG->ToggleRender(true);
        m_pkGiftCommentBG->ToggleRender(true);

        m_pkGiftBG->ToggleRender(true);
        m_pkFontImgNickname->ToggleRender(true);
    }
    else // 일반 구입
    {
        if (m_iDiscount == 0)  // 일반 캐시로 구입
        {
            m_pkMainframe->SetSize(m_pkMainframe->GetWidth(), SMALL_MAINFRAME);
            m_pkInnerBG->SetSize(m_pkInnerBG->GetWidth(), SMALL_INNERBG);
            m_pkCalcBG->SetSize(m_pkCalcBG->GetWidth(), SMALL_CALCBG);

            m_pkCalcItemPriceLabel->ToggleRender(false);
            m_pkCalcVirtualCashLabel->ToggleRender(false);
            m_pkCalcResultCashLabel->SetWindowPosDirect(D3DXVECTOR2(CALC_RESULT_LABLE_X, CALC_RESULT_LABLE_Y_AT_SMALL));
        }
        else if (m_iDiscount == m_iOriginalPrice) // 가상 캐시로 구입 
        {
            m_pkMainframe->SetSize(m_pkMainframe->GetWidth(), SMALL_MAINFRAME);
            m_pkInnerBG->SetSize(m_pkInnerBG->GetWidth(), SMALL_INNERBG);
            m_pkCalcBG->SetSize(m_pkCalcBG->GetWidth(), SMALL_CALCBG);

            m_pkCalcItemPriceLabel->ToggleRender(false);
            m_pkCalcVirtualCashLabel->ToggleRender(false);
            m_pkCalcResultCashLabel->SetWindowPosDirect(D3DXVECTOR2(CALC_RESULT_LABLE_X, CALC_RESULT_LABLE_Y_AT_SMALL));
        }

        m_pEditGiftID->ToggleRender(false);
        m_pEditGiftComment->ToggleRender(false);

        m_pkGiftNicknameBG->ToggleRender(false);
        m_pkGiftCommentBG->ToggleRender(false);

        m_pkGiftBG->ToggleRender(false);
        m_pkFontImgNickname->ToggleRender(false);
    }
#else

    if (m_iDiscount == 0)
    {
        m_pkMainframe->SetSize(m_pkMainframe->GetWidth(), SMALL_MAINFRAME);
        m_pkInnerBG->SetSize(m_pkInnerBG->GetWidth(), SMALL_INNERBG);
        m_pkCalcBG->SetSize(m_pkCalcBG->GetWidth(), SMALL_CALCBG);

        m_pkCalcItemPriceLabel->ToggleRender(false);
        m_pkCalcVirtualCashLabel->ToggleRender(false);
        m_pkCalcResultCashLabel->SetWindowPosDirect(D3DXVECTOR2(CALC_RESULT_LABLE_X, CALC_RESULT_LABLE_Y_AT_SMALL));
    }
    else
    {
        m_pkMainframe->SetSize(m_pkMainframe->GetWidth(), BIG_MAINFRAME);
        m_pkInnerBG->SetSize(m_pkInnerBG->GetWidth(), BIG_INNERBG);
        m_pkCalcBG->SetSize(m_pkCalcBG->GetWidth(), BIG_CALCBG);

        m_pkCalcItemPriceLabel->ToggleRender(true);
        m_pkCalcVirtualCashLabel->ToggleRender(true);
        m_pkCalcResultCashLabel->SetWindowPosDirect(D3DXVECTOR2(CALC_RESULT_LABLE_X, CALC_RESULT_LABLE_X_AT_BIG));
    }
#endif

    m_pkOK->SetWindowPosDirect(m_pkInnerBG->GetFixedWindowLocalPos() + D3DXVECTOR2(OK_GAP_X, OK_GAP_Y + (float)m_pkInnerBG->GetHeight()));
    m_pkCancle->SetWindowPosDirect(m_pkInnerBG->GetFixedWindowLocalPos() + D3DXVECTOR2(CANCLE_GAP_X, CANCLE_GAP_Y + (float)m_pkInnerBG->GetHeight()));
}

bool KGCBuyCashBox::CheckCondition(IN const KDialogInfo& kInfo_)
{
    this->SetItemID(static_cast<GCITEMID>(kInfo_.m_lParam));
    return true;
}

void KGCBuyCashBox::OnCheckUseGift()
{
    bool bCheck = m_pkCheckUseGift->GetCheck();
    m_pkCheckUseGift->SetCheck(!bCheck);
    m_pkCheckMixVirtualCash->SetCheck(false);
    CalcResultPrice();

}

// 가상캐쉬 + 캐쉬 혼용구매
void KGCBuyCashBox::BuyCashItemUseCashAndVirtualCash()
{
    SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
    g_pItemMgr->SendBuyForCashItem(m_pkItem, m_iDiscount, m_pkCheckUseGift->GetCheck(), m_pEditGiftID->GetText(), m_pEditGiftComment->GetText());
    SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
}