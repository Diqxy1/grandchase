#include "stdafx.h"
#include "GCSetItemWnd.h"
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
#include "GCCashShopScene.h"
#include "GCGPShopScene.h"
#include "GCSearchBar.h"
#include "GCUINumber.h"
#include "../gcui/KGCAskUseVirtualCash.h"



IMPLEMENT_CLASSNAME( KGCSetItemWnd );
IMPLEMENT_WND_FACTORY( KGCSetItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSetItemWnd, "gc_set_item_wnd" );

KGCSetItemWnd::KGCSetItemWnd( void ) : m_kItemInfo(NULL)
{
    m_iIndex        = 0;
    m_iItemID		= -1;

    m_pkBuyButton   = NULL;
    m_pkItemName    = NULL;
    m_pkItemLimit   = NULL;
    m_pkItemBar     = NULL;
    m_pkPriceNum    = NULL;
    m_pkEquip       = NULL;
    m_bShow			= true;
    m_bEquip		= false;

    m_pTextureItem	= NULL;

    LINK_CONTROL( "set_buy_button",      m_pkBuyButton );
    LINK_CONTROL( "present_button",      m_pkPresentButton );
	LINK_CONTROL( "gamble_button",       m_pkGambleButton );
	
    LINK_CONTROL( "item_name_static",    m_pkItemName );
    LINK_CONTROL( "item_limit_static",   m_pkItemLimit );
    LINK_CONTROL( "set_item_bottom_bar", m_pkItemBar );
    LINK_CONTROL( "set_item_equip",      m_pkEquip );
    LINK_CONTROL( "number_price",        m_pkPriceNum );

	m_pkNew = NULL;
	LINK_CONTROL( "new_setgoods",        m_pkNew );
	m_pkFit = NULL;
	LINK_CONTROL( "fit_setgoods",        m_pkFit );
	m_pkHot = NULL;
	LINK_CONTROL( "hot_setgoods",        m_pkHot );
    m_pkVIP = NULL;
	LINK_CONTROL( "vip_setgoods",        m_pkVIP );
	m_pkSale = NULL;
	LINK_CONTROL( "sale_goods",        m_pkSale );

	m_pkLayer = NULL;
	LINK_CONTROL( "background",   m_pkLayer );

	m_pkActiveLayer = NULL;
	LINK_CONTROL( "active_layer",   m_pkActiveLayer );
	m_pkImgDrawRect = NULL;
	LINK_CONTROL( "img_draw_rect",       m_pkImgDrawRect );

}

KGCSetItemWnd::~KGCSetItemWnd( void )
{
    // empty
    SAFE_RELEASE(m_pTextureItem);
}

void KGCSetItemWnd::OnCreate( void )
{

	m_pkImgDrawRect->InitState(false);


    m_pkLayer->SetSelfInputCheck( false );

    m_pkActiveLayer->SetSelfInputCheck( false );
    m_pkActiveLayer->ToggleRender( false );


    m_pkBuyButton->SetSelfInputCheck( true );
    m_pkBuyButton->ToggleRender( true );
    m_pkBuyButton->AddActionListener( this );

	m_pkGambleButton->InitState( true, true , this );
    m_pkItemName->SetSelfInputCheck( false );
    //m_pkItemName->SetOffset( D3DXVECTOR2( 0.0f, 1.0f ) );

	D3DXVECTOR2 v = m_pkBuyButton->GetFixedWindowLocalPos();
	v.y -= m_pkGambleButton->GetHeight();

	m_pkGambleButton->SetWindowPos( v );
#ifndef USE_GAMBLE_SYSTEM
	m_pkGambleButton->ToggleRender( false );
#endif
    m_pkItemLimit->SetSelfInputCheck( false );
    m_pkItemLimit->SetFontColor( 0xffff0000 );

    m_pkEquip->ToggleRender( false );
    //#ifdef GAME_CASH
        m_pkPresentButton->InitState( false, true, this );
    //#else
    //m_pkPresentButton->InitState( false, false );
    //#endif

	if (m_pkNew)
		m_pkNew->InitState(false,false);
	if (m_pkFit)
		m_pkFit->InitState(false,false);
	if (m_pkHot)
		m_pkHot->InitState(false,false);
    if (m_pkVIP)
        m_pkVIP->InitState(false, false);
	if (m_pkSale)
		m_pkSale->InitState(false,false);
}

void KGCSetItemWnd::FrameMoveInEnabledState( void )
{
    bool bMouseHover = CheckPosInWindowBound( D3DXVECTOR2( (float)g_pkInput->GetMousePos().x,
        (float)g_pkInput->GetMousePos().y ) );


    m_pkActiveLayer->ToggleRender( bMouseHover );

}

D3DXVECTOR4 KGCSetItemWnd::GetTextureCoords( int iSetNum )
{
    D3DXVECTOR4 m_vTexCoords;

    if ( 0 == iSetNum % 2 )
    {
        m_vTexCoords.x = 0.0f;
        m_vTexCoords.y = 0.0f;
        m_vTexCoords.z = 0.5f;
        m_vTexCoords.w = 1.0f;
    }
    else
    {
        m_vTexCoords.x = 0.5f;
        m_vTexCoords.y = 0.0f;
        m_vTexCoords.z = 1.0f;
        m_vTexCoords.w = 1.0f;
    }

    return m_vTexCoords;
}

void KGCSetItemWnd::PostDraw( void )
{
    if( m_iItemID == -1 )
        return;

    int shSetNum = m_kItemInfo->iSetItemNumber;

	GCItem *pItem =	g_pItemMgr->GetSameImageIndexItemInfo(m_kItemInfo->dwGoodsID);
	if(pItem != NULL)
		shSetNum = pItem->iSetItemNumber;

    D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
    D3DXVECTOR4 vTexCoords( GetTextureCoords( shSetNum ) );

	vPos.x += m_pkImgDrawRect->GetFixedWindowLocalPos().x;
	vPos.y += m_pkImgDrawRect->GetFixedWindowLocalPos().y;

	g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    {
		g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
			vPos.x, vPos.y, vPos.x + m_pkImgDrawRect->GetWidth() * m_fWindowScaleX, vPos.y + m_pkImgDrawRect->GetHeight() * m_fWindowScaleY,
			vTexCoords.x, vTexCoords.y, vTexCoords.z, vTexCoords.w,false,10.0f,0xffffffff);
    }
	g_pGCDeviceManager2->PopState();	
}

void KGCSetItemWnd::SetIndex( int iIndex )
{
    m_iIndex = iIndex;
}

void KGCSetItemWnd::Equip( bool bEquip)
{
    m_bEquip = bEquip;

    m_pkLayer->SetWndMode( bEquip ? D3DWM_ACTIVE : D3DWM_DEFAULT , false );


    //m_pkEquip->ToggleRender( bEquip );
    //m_pkBuyButton->ToggleRender( bEquip ? false : m_bShow );
}

bool KGCSetItemWnd::IsEquip( void ) const
{
    return m_bEquip;
    //return m_pkEquip->IsRenderOn();
}

void KGCSetItemWnd::ActionPerformed( const KActionEvent& event )
{
    if ( KD3DWnd::D3DWE_BUTTON_CLICK != event.m_dwCode ) return;

    //g_MyD3D->m_kItemMgr.m_ShopCursor = g_MyD3D->m_kItemMgr.m_ShopPitch + m_iIndex;

    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

    if ( event.m_pWnd == m_pkBuyButton ) // 구입 버튼을 클릭했다면
    {
        g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus( false );
        g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus( false );

        int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;

        // 인벤토리가 제한 수량을 초과하면 구매를 막는다.
        if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() > iInventoryCapacity )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
                g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", iInventoryCapacity ),
                g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ) );
        }
        else // 인벤토리에 구매할 여유가 있다면
        {			
            int iItemID = m_iItemID;
            int iPeriod = m_iPeriod;

            GCItem* kItem = m_kItemInfo;

            if ( kItem->eMoneyType == EMT_CASH ) // 캐쉬 아이템인 경우 캐쉬 구입용 다이얼로그 박스를 바로 띄운다.
            {
                // 체험계정이면 캐쉬템 구매가 불가능하다.
                //if( g_kGlobalValue.m_bExpAccount ) //exp_id
                //{
                //    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                //        g_pkStrLoader->GetString( STR_ID_EXPID_NOT_BUY_CASH_ITEM ),
                //        g_pkStrLoader->GetString( STR_ID_EXPID_IMPOSSIBLE ));
                //}
#ifdef _OPEN_TEST_
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY1 ),
                        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY2 ) );
                }
#else // !_OPEN_TEST_
                {
#ifdef NATION_KOREA
                    if ( g_kGlobalValue.m_kUserInfo.iAge < CASH_BUY_LIMIT_AGE )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY1 ),
                            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY2 ) );
                    }
                    else
#endif // NATION_KOREA
                    {
                        bool bOpenBuyWnd = true;
                        #if defined( VIRTUAL_CASH ) && !defined( GAME_CASH )
                        if( g_kGlobalValue.m_dwVirtualCash >= kItem->dwPrice )
                        {
                            g_pkUIScene->m_pkAskUseVirtualCash->SetItemInfo( iItemID, iPeriod );
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_VIRTUAL_CASH );
                            bOpenBuyWnd = false;
                        }
                        #endif

                        if( bOpenBuyWnd )
                        {
#if defined ( USE_VIP_POINT )
                            g_pItemMgr->BuyCashItemS6( iItemID );
#else
                            #ifdef GAME_CASH
                                g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false );
                            #else    
                                g_MyD3D->m_kItemMgr.BuyItem( iItemID, iPeriod  );
                            #endif
#endif
                        }
                    }
                }

#endif // _OPEN_TEST_
            }
            else if ( kItem->eMoneyType == EMT_CRYSTAL )
            {
                if ( g_MyD3D->m_kItemMgr.FindInventoryForItemID( KAZEAZE_MAP_ORIGINAL_ID ) )
                {
                    std::wostringstream strmText1;
                    std::wostringstream strmText2;

                    if ( g_MyD3D->m_kItemMgr.IsExistCrystalDiscountCard() )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                            g_pkStrLoader->GetString( STR_ID_DISCOUNT_CRYSTAL1 ),
                            g_pkStrLoader->GetString( STR_ID_DISCOUNT_CRYSTAL2 ) );

#if defined ( _PORTUGUESE ) || defined( _SPANISH )
                        strmText1 << kItem->strItemName;
                        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "iI", (int)kItem->dwPrice / 2, STR_ID_CRYSTAL_TEXT ) << ' '
                            << g_pkStrLoader->GetString( STR_ID_CRYSTAL_DC_APPLY );
#elif defined( _ENGLISH ) || defined(_THAI_LANG)
                        strmText1 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "li", kItem->strItemName, (int)kItem->dwPrice / 2 )
                            << g_pkStrLoader->GetString( STR_ID_CRYSTAL_TEXT) << L"(s)?" ;
#else
                        strmText1 << kItem->strItemName
                            << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION ) << ' '
                            << g_pkStrLoader->GetString( STR_ID_CRYSTAL_TEXT ) << ' '
                            << kItem->dwPrice / 2;
                        strmText2 << g_pkStrLoader->GetString( STR_ID_BUY_THE_NUMBER ) << ' '
                            << g_pkStrLoader->GetString( STR_ID_CRYSTAL_DC_APPLY );
#endif

                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
                            strmText1.str(), strmText2.str(),
                            KGCUIScene::GC_MBOX_USE_BUY_FOR_CRYSTAL, iItemID, iPeriod );
                    }
                    else
                    {
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
                        strmText1 << kItem->strItemName;
                        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "iI", (int)kItem->dwPrice, STR_ID_CRYSTAL_TEXT );
#elif defined( _ENGLISH ) || defined(_THAI_LANG)
                        strmText1 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "li", kItem->strItemName, (int)kItem->dwPrice )
                            << g_pkStrLoader->GetString( STR_ID_CRYSTAL_TEXT) << L"(s)?";
#else
                        strmText1 << kItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
                        strmText2 << g_pkStrLoader->GetString( STR_ID_CRYSTAL_TEXT ) << ' '
                            << kItem->dwPrice << g_pkStrLoader->GetString( STR_ID_BUY_THE_NUMBER );
#endif

                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
                            strmText1.str(), strmText2.str(),
                            KGCUIScene::GC_MBOX_USE_BUY_FOR_CRYSTAL, iItemID, iPeriod );
                    }
                }
                else
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString( STR_ID_BUY_ORIGINAL_MAP_NEED1 ),
                        g_pkStrLoader->GetString( STR_ID_BUY_ORIGINAL_MAP_NEED2 ) );
                }
            }
            else // GP 아이템인 경우 구입 의사를 확인해본다.
            {
                g_pItemMgr->BuyGPItem( kItem, iItemID, iPeriod );
            }
        }
    }
    else if( event.m_pWnd == m_pkPresentButton )
    {
        int iItemID = m_iItemID;
        int iPeriod = m_iPeriod;
        GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( iItemID );
        RETURN_NIL(kItem);

        if ( EMT_CASH == kItem->eMoneyType )
            g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, true );
    }

	if ( event.m_pWnd == m_pkGambleButton ) // 구입 버튼을 클릭했다면
	{
		g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus( false );
        g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus( false );

		int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;

		// 인벤토리가 제한 수량을 초과하면 구매를 막는다.
		if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() > iInventoryCapacity )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
				g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", iInventoryCapacity ),
				g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ) );
		}
		else // 인벤토리에 구매할 여유가 있다면
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GAMBLE_BOX,
				L"",L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, m_kItemInfo->iSetItemNumber );
		}
	}
}

GCItem*& KGCSetItemWnd::GetItemInfo( void )
{
    return m_kItemInfo;
}

void KGCSetItemWnd::SetItemInfo(int iShopItemIndex)
{
    SAFE_RELEASE(m_pTextureItem);

    if(iShopItemIndex == -1)
    {
        m_iItemID = -1;
        m_pkItemBar->ToggleRender( false );
        m_pkItemName->ToggleRender( false );
        m_pkBuyButton->ToggleRender( false );
        m_pkPresentButton->ToggleRender( false );
        m_pkItemLimit->SetText( L"" );
        m_pkItemName->SetText( L"" );
		m_pkGambleButton->ToggleRender( false );
        m_bShow = false;
		m_pkHot->ToggleRender(false);
		m_pkNew->ToggleRender(false);
		m_pkFit->ToggleRender(false);
        m_pkVIP->ToggleRender(false);
		m_pkSale->ToggleRender(false);

        if( m_pkPriceNum )
            m_pkPriceNum->ToggleRender( false );
        return;
    }

    m_bShow = true;

    if( !g_MyD3D->m_kItemMgr.GetShopItem(iShopItemIndex, m_kItemInfo) )
    {
        m_iItemID = -1;
        return;
    }

    m_pTextureItem = g_pItemMgr->CreateShopSetItemTexture( m_kItemInfo->iSetItemNumber, m_kItemInfo->dwGoodsID );

    m_iItemID = m_kItemInfo->dwGoodsID;
    m_iPeriod = m_kItemInfo->iItemTypeValue;

    m_pkItemBar->ToggleRender( true );
    if( m_pkPriceNum )
        m_pkPriceNum->ToggleRender( true );
    m_pkItemName->ToggleRender( true );
    m_pkBuyButton->ToggleRender( true );

    int iCharType   = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

    // 속성 능력치 적용!
    ///////////////////////////////////////////////////////////////////////////////////////
    std::vector< KAttributeInfo > vecAttribute = m_kItemInfo->m_vecAttribute;
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
    if (  iLevel < m_kItemInfo->iNeedLevelForEquippingItem - cEquipDecLvValue )
    {
        std::wostringstream strmText;
        strmText << g_pkStrLoader->GetString( STR_ID_NOT_EQUIP_LEVEL ) << static_cast<int>( m_kItemInfo->iNeedLevelForEquippingItem ) << ')';
        m_pkItemLimit->SetText( strmText.str() );
    }	else m_pkItemLimit->SetText( L"" );

    WCHAR szItemName[MAX_PATH] = { 0, };
    std::size_t len = std::wcslen( m_kItemInfo->strItemName.c_str() );
    for ( std::size_t i = 0; i < len; ++i )
    {
        szItemName[i] = m_kItemInfo->strItemName[i];
        if ( g_pkFontMgr->GetWidth( szItemName ) > (int)GetWidth() * m_fWindowScaleX && i > 0 )
        {
            szItemName[i-1] = szItemName[i] = L'\0';
            break;
        }
    }

    std::wstring strItemGradeColor = g_pItemMgr->GetItemGradeColorKey( m_kItemInfo->cItemGrade );
    std::wstring strItemColorName = strItemGradeColor + m_kItemInfo->strItemName + L"#cX";
    m_pkItemName->SetForceWordWrapText( strItemColorName, true );
    //m_pkItemName->SetShadow( true );

#if defined(BUY_CASH_ITEM_WITH_POINT)
    if( g_pkUIScene->m_pkCashShopScene->IsPointMode() && m_kItemInfo->eMoneyType == EMT_CASH ||
        g_pkUIScene->m_pkGPShopScene->IsPointMode() && m_kItemInfo->eMoneyType == EMT_CASH 
        )
    {
        m_pkItemBar->SetPointType();
        if( m_pkPriceNum )
        {
            if( g_MyD3D->m_kItemMgr.IsInVaildPointShopItem(m_kItemInfo->dwGoodsID ) )
            {
                //숫자를 쓰지 않는다.
                m_pkPriceNum->ToggleRender( false );
            }
            else if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_kItemInfo->dwGoodsID ) )
            {
                m_pkPriceNum->ToggleRender( true );
                m_pkPriceNum->SetNumber( g_MyD3D->m_kItemMgr.GetPointItemPrice( m_kItemInfo->dwGoodsID ) );
            }
            else
            {
                m_pkPriceNum->ToggleRender( true );
                m_pkPriceNum->SetNumber( static_cast<int>(( m_kItemInfo->dwPrice ) * 10) );
            }
        }
    }
    else
#else
    // NOTE : 세트 아이템은 GP나 캐쉬 상품일 뿐이다.
    if ( m_kItemInfo->eMoneyType == EMT_GEM )
    {
        ASSERT( false && "invalid set item : gem item" );
    }
    else
#endif
    {
#if defined(BUY_CASH_ITEM_WITH_POINT)
        m_pkPriceNum->ToggleRender(true);
#endif
        m_pkItemBar->SetItem( m_kItemInfo, true );
        if( m_pkPriceNum ) {
            if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_kItemInfo->dwGoodsID ) )
            {
                m_pkPriceNum->SetNumber( g_MyD3D->m_kItemMgr.GetPointItemPrice( m_kItemInfo->dwGoodsID ) );
            }
            else {
                m_pkPriceNum->SetNumber( static_cast<int>(m_kItemInfo->dwPrice) );
            }
        }
    }
    m_pkPresentButton->ToggleRender( false );

#ifdef USE_GAMBLE_SYSTEM
	m_pkGambleButton->ToggleRender( m_kItemInfo->eMoneyType == EMT_GAMEPOINT );
#endif
	
	
	
}

void KGCSetItemWnd::SetMask( const int& RecomLevel_ )
{
	m_pkHot->ToggleRender((RecomLevel_ & HOT_MARK_MASK) != 0);
	m_pkNew->ToggleRender((RecomLevel_ & NEW_MARK_MASK) != 0);
	m_pkSale->ToggleRender((RecomLevel_ & SALE_MARK_MASK) != 0);
	m_pkFit->ToggleRender((RecomLevel_ & FIT_MARK_MASK) != 0);
}

void KGCSetItemWnd::SetVIP(bool bVIP)
{
    if (m_pkVIP)
        m_pkVIP->ToggleRender(bVIP);
}