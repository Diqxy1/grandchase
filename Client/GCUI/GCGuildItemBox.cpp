#include "stdafx.h"
#include "GCGuildItemBox.h"
#include "../MyD3D.h"
#include "../Procedure.h"

IMPLEMENT_CLASSNAME( KGCGuildItemBox );
IMPLEMENT_WND_FACTORY( KGCGuildItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildItemBox, "gc_guild_item_box" );

KGCGuildItemBox::KGCGuildItemBox( void )
:m_pkBtnBuy(NULL)
,m_pkStaticItemnum(NULL)
,m_pkStaticItemPrice(NULL)
,m_pkImageCoin(NULL)
,m_pkImageGP(NULL)
{
    m_pkItemImage = NULL;
    m_pkManufacture = NULL;
    m_iTabIndex = -1;
    m_bDisplay = false;

    m_iBuyIndex = -1;

    LINK_CONTROL("btn_buy",m_pkBtnBuy);

    LINK_CONTROL("static_prev_item_num",m_pkStaticItemnum);
    LINK_CONTROL("static_item_price",m_pkStaticItemPrice);


    LINK_CONTROL("img",m_pkItemImage);
    LINK_CONTROL("image_coin",m_pkImageCoin);
    LINK_CONTROL("image_gp",m_pkImageGP);

}

KGCGuildItemBox::~KGCGuildItemBox( void )
{
}

void KGCGuildItemBox::ActionPerformed(const KActionEvent &event)
{
    GCWND_MSG_MAP( m_pkBtnBuy, KD3DWnd::D3DWE_BUTTON_CLICK, OnBuyButton );

}

void KGCGuildItemBox::OnCreate()
{
    m_pkBtnBuy->InitState(false,true,this);

    m_pkStaticItemnum->SetFontColor(0xffffffff);
    m_pkStaticItemnum->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemnum->SetAlign(DT_RIGHT);
    m_pkStaticItemnum->SetShadow(true);
    m_pkStaticItemnum->InitState(false,false,this);
    m_pkStaticItemnum->SetText(L"");

    m_pkStaticItemPrice->SetFontColor(0xffffffff);
    m_pkStaticItemPrice->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemPrice->SetAlign(DT_RIGHT);
    m_pkStaticItemPrice->InitState(false,false,this);
    m_pkStaticItemPrice->SetText(L"");

    m_pkImageCoin->InitState(false);
    m_pkImageGP->InitState(false);

    m_pkItemImage->InitState(false,true,this);

}

void KGCGuildItemBox::OnBuyButton()
{
 
    Result_BuyForGuildItem = INT_MAX;

    KP2P::GetInstance()->Send_BuyForGuildReq(m_iTabIndex , m_pkManufacture->m_Index , m_pkManufacture->m_ItemID/10);

    g_MyD3D->WaitForServerAck( Result_BuyForGuildItem, INT_MAX, 500, TIME_OUT_VALUE);

}

void KGCGuildItemBox::SetItemBox(KManufactureItem* pManufacture_ , int iTabIndex_)
{
    m_pkManufacture = pManufacture_;
    m_iTabIndex = iTabIndex_;

    DWORD dwItemID = static_cast<DWORD>(m_pkManufacture->m_ItemID/10);
    GCItem *pItem = g_pItemMgr->GetItemData(dwItemID);
    if (pItem == NULL)
        return;

    m_pkStaticItemnum->SetText(m_pkManufacture->m_nDuration);

    m_pkStaticItemPrice->SetText(m_pkManufacture->m_nPrice);

    m_pkItemImage->SetItemInfo(dwItemID);

    Lock(true);
    SetToolTip(pItem->strItemName);
    Lock(false);
          
    ToggleDisPlay(true);
}

void KGCGuildItemBox::ClearBox()
{
    ToggleDisPlay(false);
    m_iTabIndex = -1;
    m_pkStaticItemnum->SetText(L"");
    m_pkStaticItemPrice->SetText(L"");
    m_pkManufacture = NULL;
    Lock(true);
    SetToolTip(L"");
    Lock(false);
}

void KGCGuildItemBox::ToggleDisPlay(bool bToggle_)
{
    m_pkItemImage->ToggleRender(bToggle_);
    m_pkImageGP->ToggleRender(bToggle_);
    m_pkStaticItemnum->ToggleRender(bToggle_);
    m_pkStaticItemPrice->ToggleRender(bToggle_);
    m_pkBtnBuy->ToggleRender(bToggle_); 
    m_bDisplay = bToggle_;
}

void KGCGuildItemBox::ToggleName(bool bToggle_)
{

}

