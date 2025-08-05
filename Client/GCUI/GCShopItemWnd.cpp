#include "stdafx.h"
#include "GCShopItemWnd.h"
#include "GCItemBar.h"
//

//
//
//
//
#include "../MyD3D.h"

//

#include "../Procedure.h"



//
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCSearchBar.h"
#include "../gcui/KGCAskUseVirtualCash.h"
#include "KGCPetPreview.h"
#include "GCUINumber.h"
#include "gcui/KGCNewClearSealDlg.h"
#include "gcui/KGCCoordiClearSealDlg.h"
#include "gcui/KGCPackagePreview.h"




IMPLEMENT_CLASSNAME( KGCShopItemWnd );
IMPLEMENT_WND_FACTORY( KGCShopItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCShopItemWnd, "gc_shop_item_wnd" );

KGCShopItemWnd::KGCShopItemWnd( void ) : m_kItemData(NULL)
{
    m_iIndex            = 0;
    m_iItemID           = -1;
    m_iPattrn           = 0;
    m_iDuratn           = -1;
    m_iStrong           = 0;
    m_dwUID             = 0;

    m_bShow             = false;
    m_bMouseHoverPrev   = false;
    m_bAlreadyPatternUp = true;
    m_bNotSaleForLevel  = false;
    m_bEvent            = false;

    m_bEquip            = false;

    m_pkItemName    = NULL;
    m_pkItemLimit   = NULL;
    m_pkItemBar     = NULL;
    m_pkEquip       = NULL;
    m_pTextureItem  = NULL;
    m_pkPriceNum    = NULL;

	m_pkLayer = NULL;
	LINK_CONTROL( "background",   m_pkLayer );

	m_pkActiveLayer = NULL;
	LINK_CONTROL( "active_layer",   m_pkActiveLayer );


    LINK_CONTROL( "buy_button",     m_pkBuyButton );
    LINK_CONTROL( "present_button", m_pkPresentButton );
    LINK_CONTROL( "preview_button", m_pkPreviewButton );
	LINK_CONTROL( "gamble_button", m_pkGambleButton );
	
    
    LINK_CONTROL( "item_name_static",   m_pkItemName );
    LINK_CONTROL( "item_limit_static",  m_pkItemLimit );
    LINK_CONTROL( "item_bottom_bar",    m_pkItemBar );
    LINK_CONTROL( "item_equip",         m_pkEquip );
    LINK_CONTROL( "number_price",       m_pkPriceNum );

	m_pkHot = NULL;
	LINK_CONTROL( "hot_goods",       m_pkHot );
	m_pkNew = NULL;
	LINK_CONTROL( "new_goods",       m_pkNew );
	m_pkSale = NULL;
	LINK_CONTROL( "sale_goods",       m_pkSale );
	m_pkFit = NULL;
	LINK_CONTROL( "fit_goods",       m_pkFit );
    m_pkVIP = NULL;
	LINK_CONTROL( "vip_goods",       m_pkVIP );

	m_pkImgDrawRect = NULL;
	LINK_CONTROL( "img_draw_rect",       m_pkImgDrawRect );
}

KGCShopItemWnd::~KGCShopItemWnd( void )
{
    // empty
    SAFE_RELEASE(m_pTextureItem);
}

void KGCShopItemWnd::OnCreate( void )
{   



	m_pkImgDrawRect->InitState(false);
    m_pkActiveLayer->SetSelfInputCheck( false );
    m_pkActiveLayer->ToggleRender( false );

    
    m_pkBuyButton->SetSelfInputCheck( true );
    m_pkBuyButton->ToggleRender( true );
    m_pkBuyButton->AddActionListener( this );
    //#ifdef GAME_CASH
    //{
          m_pkPresentButton->InitState( false, true, this );
    //}
    //#else
    {
    //    m_pkPresentButton->InitState( false, false );
    }
    //#endif

    m_pkItemName->SetSelfInputCheck( false );
    //m_pkItemName->SetOffset( D3DXVECTOR2( 0.0f, 1.0f ) );
    m_pkItemName->SetWidthDirect( 110 );
    m_pkItemName->SetHeightDirect( 15 );

    m_pkItemLimit->SetSelfInputCheck( false );
    m_pkItemLimit->SetFontColor( 0xffff0000 );

    m_pkEquip->SetSelfInputCheck( false );
    m_pkEquip->ToggleRender( false );

    m_pkItemBar->SetSelfInputCheck( false );
    m_pkItemBar->ToggleRender( true );

    m_pkPreviewButton->InitState( false, true, this );    

	D3DXVECTOR2 v = m_pkBuyButton->GetFixedWindowLocalPos();
	v.y -= m_pkGambleButton->GetHeight();

	m_pkGambleButton->SetWindowPos( v );

	m_pkGambleButton->InitState( true, true, this );
#ifndef USE_GAMBLE_SYSTEM
	m_pkGambleButton->ToggleRender( false );
#endif
	
    if( m_pkPriceNum ) {
        m_pkPriceNum->InitState( true );
        m_pkPriceNum->SetStringType( "very_small_num" );
    }

	if (m_pkHot)
		m_pkHot->InitState(false,false);
	if (m_pkNew)
		m_pkNew->InitState(false,false);
	if (m_pkSale)
		m_pkSale->InitState(false,false);
	if (m_pkFit)
		m_pkFit->InitState(false,false);
    if (m_pkVIP)
        m_pkVIP->InitState(false, false);
}

void KGCShopItemWnd::FrameMoveInEnabledState( void )
{
    POINT   pt = g_pkInput->GetMousePos();
    bool    bMouseHover = CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );
        

    m_pkActiveLayer->ToggleRender( bMouseHover );                
        

    bool bPackage = false;
#if defined ( USE_PACKAGE_PREVIEW )
    bPackage = g_pItemMgr->IsPackageItemID( m_iItemID*10 );
#endif

    if( bMouseHover && ((g_kGlobalValue.GetPetIDFromItemID( m_iItemID ) != -1 && g_kGlobalValue.IsPetCostumeItem(m_iItemID)==false) || bPackage) )
    {
        m_pkPreviewButton->ToggleRender(true);
    }
    else
    {
        m_pkPreviewButton->ToggleRender(false);
    }    
}

void KGCShopItemWnd::PostDraw( void )
{	
    
}

void KGCShopItemWnd::SetIndex( int iIndex )
{
    m_iIndex = iIndex;
}

void KGCShopItemWnd::Equip( bool bEquip )
{
    m_bEquip = bEquip;
    //m_pkEquip->ToggleRender( m_bEquip );
	m_pkLayer->SetWndMode( bEquip ? D3DWM_ACTIVE : D3DWM_DEFAULT , false );
}

bool KGCShopItemWnd::IsEquip( void ) const
{
    return m_bEquip;
}

void KGCShopItemWnd::ActionPerformed(const KActionEvent& event)
{
    switch (event.m_dwCode)
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play("30");
        return;

    case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play("31");
        return;
    }

    if (KD3DWnd::D3DWE_BUTTON_CLICK != event.m_dwCode)
        return;

    int iCharIndex = g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex;
    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

    if (event.m_pWnd == m_pkBuyButton)
    {
        g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus(false);
#ifndef DISABLE_MAP_SHOP
        g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus(false);
#endif
        int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;

        if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60 ||
            m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30 ||
            m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10 ||
            m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2 ||
            m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3 ||
            m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5)
        {
            KItem* kInventoryItem = NULL;
            int iExtendCapacity = 0;

            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_60;
            }
            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_30;
            }
            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_10;
            }
            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_10;
            }
            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_10;
            }
            if (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5, &kInventoryItem))
            {
                iExtendCapacity += kInventoryItem->m_nCount * EXTEND_INVENTORY_SIZE_5;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_60;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_30;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_10;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_10;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_10;
            }
            if (m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5)
            {
                iExtendCapacity += EXTEND_INVENTORY_SIZE_5;
            }
            if (iInventoryCapacity + iExtendCapacity > MAX_INVENTORY_SIZE)
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString(STR_ID_EXTEND_INVENTORY_MESSAGE7, "i", MAX_INVENTORY_SIZE));
                return;
            }
        }

        if (g_MyD3D->m_kItemMgr.GetTotalInventorySize() >= iInventoryCapacity &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60 &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30 &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10 &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2 &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3 &&
            m_iItemID != KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL1) +
                g_pkStrLoader->GetReplacedString(STR_ID_INVENTORY_FULL2, "i", iInventoryCapacity),
                g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL3));
        }
        else
        {

#ifdef ENABLE_POINT_SYSTEM
            if (g_MyD3D->m_kItemMgr.IsPointShopItem(m_iItemID))
            {
                GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData(m_iItemID);
                RETURN_NIL(kItem);
                int iPointItemPrice = g_MyD3D->m_kItemMgr.GetPointItemPrice(m_iItemID);

                if (iPointItemPrice <= g_kGlobalValue.m_iGCPoint)
                {
                    if (g_MyD3D->m_kItemMgr.GetPointItemTypeValue(m_iItemID) > 0)
                    {
                        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_iItemID, KBuySellItemReq::EMT_GCPOINT);
                    }
                    else
                    {
                        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
                            g_pkStrLoader->GetReplacedString(STR_ID_BUY_POINT_ITEM, "li", kItem->strItemName, iPointItemPrice), L"",
                            KGCUIScene::GC_MBOX_USE_BUY_POINT_ITEM, m_iItemID, 1);
                    }
                }
                else
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetReplacedString(STR_ID_NOT_ENOUGH_POINT, "l", kItem->strItemName), L"",
                        KGCUIScene::GC_MBOX_USE_NORMAL);
                }

                return;
            }
#endif

            int iItemID = m_iItemID;
            int iPeriod = m_iPeriod;
            GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
            RETURN_NIL(kItem);

            if (GEM_ITEM_ID == iItemID)
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, iItemID, KBuySellItemReq::EMT_GAMEPOINT);
            }
            else if (kItem->eMoneyType == EMT_GEM)
            {
                if (m_bNotSaleForLevel)
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString(STR_ID_NOT_BUY_LV_LIMIT1),
                        g_pkStrLoader->GetString(STR_ID_NOT_BUY_LV_LIMIT2));
                }
                else
                {
                    std::wostringstream strmText1;
                    std::wostringstream strmText2;

                    strmText1 << kItem->strItemName;
                    strmText2 << g_pkStrLoader->GetReplacedString(STR_ID_BUY_THE_NUMBER, "iI", (int)kItem->dwPrice, STR_ID_GEM_TEXT);

                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
                        strmText1.str(), strmText2.str(),
                        KGCUIScene::GC_MBOX_USE_BUY_FOR_GEM, iItemID);
                }
            }
            else if (kItem->eMoneyType == EMT_CRYSTAL)
            {
                std::wostringstream strmText1;
                std::wostringstream strmText2;

                strmText1 << kItem->strItemName;
                strmText2 << g_pkStrLoader->GetReplacedString(STR_ID_BUY_THE_NUMBER, "iI", (int)kItem->dwPrice, STR_ID_CRYSTAL_TEXT);

                if (g_kGlobalValue.IsItemInVIPList(iItemID) && g_pItemMgr->GetInventoryItemFromID(ITEM_VIP_TEMPORARY_ITEM) == NULL)
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOTVIP_NOTBUY));
                }
                else
                {
                    if (kItem->eItemType == GCIT_COUNT && kItem->iItemTypeValue != -1) {
                        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, iItemID, KBuySellItemReq::EMT_CRYSTAL);
                    }
                    else
                    {
                        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_BUY_FOR_CRYSTAL, iItemID, m_iPeriod);
                    }
                }
            }
            else if (kItem->eMoneyType == EMT_CASH)
            {
                if (g_kGlobalValue.m_kUserInfo.iAge < CASH_BUY_LIMIT_AGE)
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString(STR_ID_LESSTHAN12_NOT_BUY1),
                        g_pkStrLoader->GetString(STR_ID_LESSTHAN12_NOT_BUY2));
                }
                else if (iItemID == 15737 && g_MyD3D->m_kItemMgr.GetInventoryItemDuration(iItemID) > 3)
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, L"Limite de compra alcançado", g_pkStrLoader->GetString(STR_ID_LESSTHAN12_NOT_BUY2));
                }
                else
                {
                    bool IsCashQuantityItem = std::count(g_kGlobalValue.m_vecCashQuantityList.begin(), g_kGlobalValue.m_vecCashQuantityList.end(), iItemID);
                    if(IsCashQuantityItem)
                        g_MyD3D->m_kItemMgr.BuyCashItemQuantity(iItemID);
                    else
                        g_MyD3D->m_kItemMgr.BuyCashItemS6(iItemID);
                }
            }
            else
            {
                if (kItem->dwGoodsID == 2693 ||
                    kItem->dwGoodsID == 2694 ||
                    kItem->dwGoodsID == 2695 ||
                    kItem->dwGoodsID == 2696)
                {
                    if ( false == g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_CERT_BROKEN_DURATION))
                    {
                        std::wostringstream strmText1;
                        std::wostringstream strmText2;
                        strmText1 << g_pkStrLoader->GetString(STR_ID_HAVE_NOT_ITEM_CERT_BROKEN1);
                        strmText2 << g_pkStrLoader->GetString(STR_ID_HAVE_NOT_ITEM_CERT_BROKEN2);

                        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                            strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_NORMAL);
                        return;
                    }
                }

                if ((kItem->eItemType == GCIT_COUNT && kItem->iItemTypeValue != -1) &&
                    (kItem->dwGoodsID != 122290 &&
                        kItem->dwGoodsID != 122291 &&
                        kItem->dwGoodsID != 122292 &&
                        kItem->dwGoodsID != 123641 &&
                        kItem->dwGoodsID != 147325) && !g_pItemMgr->IsPackageItemID(iItemID * 10))
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, iItemID, KBuySellItemReq::EMT_GAMEPOINT);
                }
                else
                {
                    g_pItemMgr->BuyGPItem(kItem, iItemID, iPeriod);
                }
            }
        }
    }
    else if (event.m_pWnd == m_pkPreviewButton)
    {
        int iItemID = m_iItemID;
        int iPetID = g_kGlobalValue.GetPetIDFromItemID(iItemID);
        bool bPackage = false;

#if defined( USE_PACKAGE_PREVIEW )
        bPackage = g_pItemMgr->IsPackageItemID(m_iItemID * 10);
#endif
        if (bPackage)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_PACKAGE_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_iItemID * 10, true);
        }
        else
        {
            int iPetID = g_kGlobalValue.GetPetIDFromItemID(iItemID);

            g_pkUIScene->MessageBox(KGCUIScene::GC_PET_PREVIEW);
            g_pkUIScene->m_pkPetPreview->SetShopPreview();
            g_pkUIScene->m_pkPetPreview->SetDescState(1);
            g_pkUIScene->m_pkPetPreview->SetNowPet(iPetID, 0);
        }
    }
}

GCItem*& KGCShopItemWnd::GetItemInfo( void )
{
    return m_kItemData;
}

void KGCShopItemWnd::ShowItem( bool bShow )
{
    m_bShow = bShow;

    m_pkItemName->ToggleRender( bShow );
    m_pkItemBar->ToggleRender( bShow );
    m_pkPriceNum->ToggleRender( bShow );
    m_pkItemLimit->ToggleRender( bShow );
	m_pkHot->ToggleRender(bShow);
	m_pkNew->ToggleRender(bShow);
	m_pkFit->ToggleRender(bShow);
    if(m_pkVIP)
        m_pkVIP->ToggleRender(bShow);
	m_pkSale->ToggleRender(bShow);

    if ( false == bShow )
    {
        m_pkItemBar->SetCertType( KGCItemBar::CERT_TYPE_NONE );
        m_pkItemLimit->SetText( L"" );
		m_pkGambleButton->ToggleRender( false );
    }

    m_pkBuyButton->ToggleRender( bShow );
    //#ifdef GAME_CASH
	if( m_kItemData != NULL )
	{
		if( !g_pkUIScene->m_pkCashShopScene->IsPointMode() && m_kItemData->eMoneyType == EMT_CASH )
			m_pkPresentButton->ToggleRender( bShow );
		else
			m_pkPresentButton->ToggleRender( false );
	}
    //m_pkEquip->ToggleRender( m_bEquip );
}

void KGCShopItemWnd::OnActivateComplete( void )
{    
    /*
    for ( UINT i = 0; i < NUM_LAYER_WND; ++i )
    {
        m_apLayer[i]->SetWndMode( IsActive() ? D3DWM_ACTIVE : m_eMode );
    }    
    */
}

void KGCShopItemWnd::SetItemInfo(int iShopItemIndex)
{
    SAFE_RELEASE(m_pTextureItem);
    if(iShopItemIndex == -1)
    {
        m_iItemID = -1;
        ShowItem( false );        
        return;
    }

    m_iItemID           = -1;
    m_iPattrn           = 0;
    m_iDuratn           = -1;
    m_iStrong           = 0;
    m_dwUID             = 0;
    m_bAlreadyPatternUp = true;

    if( !g_MyD3D->m_kItemMgr.GetShopItem(iShopItemIndex, m_kItemData) )
    {
        ShowItem( false );        
        return;
    }
    m_iItemID = m_kItemData->dwGoodsID;
    m_iPeriod = m_kItemData->iItemTypeValue;

    //========ItemResorceLoading====================
    // 이모티콘 아이템의 경우에는 장비창에 에니메이션 시키기 위해서 다른 텍스쳐 쓴다!!
    m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );

    //===============================================    

    ShowItem( true );

    //현재 플레이어가 장착할 수 있는가???

    //빨간글자로.. 착용불가.......
    m_bNotSaleForLevel = false;

    // 속성 능력치 적용!
    ///////////////////////////////////////////////////////////////////////////////////////
    std::vector< KAttributeInfo > vecAttribute = m_kItemData->m_vecAttribute;
    std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();

    char cEquipDecLvValue = 0;
    while ( vecAttribute.end() != vitAttribute )
    {
        if ( (*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN )
        {
            cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
        }
        ++vitAttribute;
    }

    // 레벨 제한에 걸리면 착용 불가 표시
    int iLevel = g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel;
    if ( iLevel < m_kItemData->iNeedLevelForEquippingItem - cEquipDecLvValue )
    {
        std::wostringstream strmText;
        if ( m_kItemData->eMoneyType == EMT_GEM )
        {
            m_bNotSaleForLevel = true;
            strmText << ( g_pkStrLoader->GetString( STR_ID_NOT_BUY_LEVEL ) )
                << static_cast<int>( m_kItemData->iNeedLevelForEquippingItem ) << ')';
        }
        else
        {
            strmText << g_pkStrLoader->GetString( STR_ID_NOT_EQUIP_LEVEL ) << static_cast<int>( m_kItemData->iNeedLevelForEquippingItem ) << ')';
        }
        m_pkItemLimit->SetText( strmText.str() );
    }
    else m_pkItemLimit->SetText( L"" );

    // 아이템 이름, 가격, 가격 타입을 설정한다.
    //WCHAR szItemName[MAX_PATH] = { 0, };
    //std::size_t len = std::wcslen( m_kItemData->strItemName.c_str() );
    //for ( std::size_t i = 0; i < len; ++i )
    //{
    //    szItemName[i] = m_kItemData->strItemName[i];
    //    if ( g_pkFontMgr->GetWidth( szItemName ) > (int)m_dwWidth && i > 0 )
    //    {
    //        szItemName[i-1] = szItemName[i] = L'\0';
    //        break;
    //    }
    //}


    std::wstring strItemGradeColor = g_pItemMgr->GetItemGradeColorKey( m_kItemData->cItemGrade );
    std::wstring strItemColorName = strItemGradeColor + m_kItemData->strItemName + L"#cX";
    m_pkItemName->SetForceWordWrapText( strItemColorName, true );
    //m_pkItemName->SetShadow( true );

#if defined(BUY_CASH_ITEM_WITH_POINT)
    if( g_pkUIScene->m_pkCashShopScene->IsPointMode() && m_kItemData->eMoneyType == EMT_CASH ||
        g_pkUIScene->m_pkGPShopScene->IsPointMode() && m_kItemData->eMoneyType == EMT_CASH 
        )
    {
        m_pkItemBar->SetPointType();
        if( m_pkPriceNum )
        {
            if( g_MyD3D->m_kItemMgr.IsInVaildPointShopItem(m_kItemData->dwGoodsID ) )
            {
                //숫자를 쓰지 않는다.
                m_pkPriceNum->ToggleRender( false );
            }
            else if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_kItemData->dwGoodsID ) )
            {
                m_pkPriceNum->ToggleRender( true );
                m_pkPriceNum->SetNumber( g_MyD3D->m_kItemMgr.GetPointItemPrice( m_kItemData->dwGoodsID ) );
            }
            else
            {
                m_pkPriceNum->ToggleRender( true );
                m_pkPriceNum->SetNumber( static_cast<int>(( m_kItemData->dwPrice ) * 10) );
            }
        }
    }
    else
#endif
    {
#if defined(BUY_CASH_ITEM_WITH_POINT)
        m_pkPriceNum->ToggleRender(true);
#endif
    m_pkItemBar->SetItem( m_kItemData, true );
    if( m_pkPriceNum ) {
        if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_kItemData->dwGoodsID ) )
        {
            m_pkPriceNum->SetNumber( g_MyD3D->m_kItemMgr.GetPointItemPrice( m_kItemData->dwGoodsID ) );
        }
        else {
            m_pkPriceNum->SetNumber( static_cast<int>( m_kItemData->dwPrice ) );
        }
    }
    }

    bool bPackage = false;
#if defined ( USE_PACKAGE_PREVIEW )
    bPackage = g_pItemMgr->IsPackageItemID( m_iItemID*10 );
#endif

    if( (g_kGlobalValue.GetPetIDFromItemID( m_iItemID ) != -1 && g_kGlobalValue.IsPetCostumeItem(m_iItemID)==false) || bPackage )
    {
        m_pkPreviewButton->ToggleRender(true);
    }
    else
    {
        m_pkPreviewButton->ToggleRender(false);
    }


#ifdef USE_GAMBLE_SYSTEM
    m_pkGambleButton->ToggleRender( false );
    if ( m_kItemData->dwSlotPosition & ESP_HELMET ||
        m_kItemData->dwSlotPosition & ESP_JACKET ||
        m_kItemData->dwSlotPosition & ESP_PANTS ||
        m_kItemData->dwSlotPosition & ESP_GLOVES ||
        m_kItemData->dwSlotPosition & ESP_SHOES	||
        m_kItemData->dwSlotPosition & ESP_MANTLE ||
        m_kItemData->dwSlotPosition & ESP_WEAPON 
)
    {
		if( !g_MyD3D->m_kItemMgr.IsPointShopItem( m_kItemData->dwGoodsID ) )
			m_pkGambleButton->ToggleRender( m_kItemData->eMoneyType == EMT_GAMEPOINT );
    }
#endif
}

void KGCShopItemWnd::SetPointType()
{
    m_pkItemBar->SetPointType();
}

void KGCShopItemWnd::SetMask( const int& RecomLevel_ )
{
	m_pkHot->ToggleRender((RecomLevel_ & HOT_MARK_MASK) != 0 );
	m_pkNew->ToggleRender((RecomLevel_ & NEW_MARK_MASK) != 0 );
	m_pkSale->ToggleRender((RecomLevel_ & SALE_MARK_MASK) != 0);
	m_pkFit->ToggleRender((RecomLevel_ & FIT_MARK_MASK) != 0);
}

void KGCShopItemWnd::SetVIP( bool bVIP )
{
    if(m_pkVIP)
        m_pkVIP->ToggleRender( bVIP );
}

void KGCShopItemWnd::PreDraw()
{
	// 샵 아이템 이미지를 그린다.
	if ( m_bShow && m_iItemID > -1 )
	{
		D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
		vPos.x += m_pkImgDrawRect->GetFixedWindowLocalPos().x;
		vPos.y += m_pkImgDrawRect->GetFixedWindowLocalPos().y;

		g_pItemMgr->Draw_ShopImage( vPos , m_pkImgDrawRect->GetWidth() * m_fWindowScaleX, m_pkImgDrawRect->GetHeight() * m_fWindowScaleY, m_pTextureItem, false );
	}

}
