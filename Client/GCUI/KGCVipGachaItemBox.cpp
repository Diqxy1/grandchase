#include "stdafx.h"
//#include "P2P.H"
#include "KGCVipGachaItemBox.h"
#include "../uifx/KInput.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DSizingBox.h"
#include "../KDSound.h"
//#include "KGCStringLoader.h"
#include "MyD3D.h"
#include "GCUI/GCCharWnd.h"
//#include "GCUIScene.h"
#include "gcui/KGCVipGachaBox.h"
#include "GCItemManager.h"
#include "Procedure.h"

IMPLEMENT_CLASSNAME( KGCVipGachaItemBox );
IMPLEMENT_WND_FACTORY( KGCVipGachaItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCVipGachaItemBox, "gc_vip_gacha_item" );

KGCVipGachaItemBox::KGCVipGachaItemBox( void )
:m_pkBuyBtn( NULL )
,m_pkAlreadyGet( NULL )
,m_pkMouseOver( NULL )
,m_pkNeedBadgeNum( NULL )
,m_pkEquipedBack( NULL )
,m_bMouseOver( false )
,m_bExistItem( false )
,m_bIsEquip( false )
,m_iItemID( -1 )
,m_iCoinNum( -1 )
,m_iIndex( -1 )
,m_pTextureItem( NULL )
,m_ItemInfo( NULL )
{
    LINK_CONTROL( "buy_button",     m_pkBuyBtn );
    LINK_CONTROL( "get",            m_pkAlreadyGet );
    LINK_CONTROL( "gacha_coin",     m_pkGachaCoin );
    LINK_CONTROL( "coin_num",       m_pkNeedBadgeNum );
    LINK_CONTROL( "mouse_over",     m_pkMouseOver );
    LINK_CONTROL( "equiped",        m_pkEquipedBack );
}

KGCVipGachaItemBox::~KGCVipGachaItemBox( void )
{
    SAFE_RELEASE( m_pTextureItem );
}

void KGCVipGachaItemBox::OnCreate( void )
{
    m_pkBuyBtn->InitState( true, true, this );
    m_pkAlreadyGet->InitState( false );
    m_pkNeedBadgeNum->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_pkNeedBadgeNum->InitState( false );
    m_pkMouseOver->InitState( false );
    m_pkEquipedBack->InitState( false );
    m_pkGachaCoin->InitState( true );
}

void KGCVipGachaItemBox::FrameMoveInEnabledState( void )
{
    if ( m_iItemID != -1 )
        m_pkMouseOver->ToggleRender( m_bMouseOver );
}

void KGCVipGachaItemBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

	GCWND_MSG_MAP( m_pkBuyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBuyBtn );
}

void KGCVipGachaItemBox::OnClickBuyBtn()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_VIP_GACHA_BUY_ITEM, "li", m_ItemInfo->strItemName, m_iCoinNum )
                            , L"", KGCUIScene::GC_MBOX_USE_BUY_VIP , m_iIndex, 0, false, true );
}

void KGCVipGachaItemBox::BuyItemProcedure( void )
{
    // charType, itemID, 수량or기간, 필요한 코인수

    DWORD dwAllChar = 1;

    for( int i = 1; i < GC_CHAR_NUM; i++ )
    {
        dwAllChar = dwAllChar << 1;
        dwAllChar += 1;
    }

    if ( dwAllChar == m_ItemInfo->dwCharType )
    {
        // 공용 아이템이네 -_-;; 후;;
        if( m_ItemInfo->eItemType == GCIT_PERIOD ) {
            KP2P::GetInstance()->Send_BuyForVIPReq( g_pkUIScene->m_pkVipGachaBox->GetCurCharType(), (DWORD)m_iItemID, KItem::UNLIMITED_ITEM, m_ItemInfo->iItemTypeValue, m_iCoinNum );
        }
        else {
            KP2P::GetInstance()->Send_BuyForVIPReq( g_pkUIScene->m_pkVipGachaBox->GetCurCharType(), (DWORD)m_iItemID, m_ItemInfo->iItemTypeValue, KItem::UNLIMITED_ITEM, m_iCoinNum );
        }
    }
    else
    {
        if( m_ItemInfo->eItemType == GCIT_PERIOD ) {
            KP2P::GetInstance()->Send_BuyForVIPReq( m_ItemInfo->GetCharType(), (DWORD)m_iItemID, KItem::UNLIMITED_ITEM, m_ItemInfo->iItemTypeValue, m_iCoinNum );
        }
        else {
            KP2P::GetInstance()->Send_BuyForVIPReq( m_ItemInfo->GetCharType(), (DWORD)m_iItemID, m_ItemInfo->iItemTypeValue, KItem::UNLIMITED_ITEM, m_iCoinNum );
        }
    }

    g_MyD3D->WaitForServerAck( Result_Vip_Gacha_Item, INT_MAX, 3000, INT_MAX );

    // 사고 나면 무조건 있는 아이템이다!!! ㅎ 
    if ( Result_Vip_Gacha_Item == 0 )
    {
        m_pkAlreadyGet->ToggleRender( true );
        m_bExistItem = true;
    }

    Result_Vip_Gacha_Item = INT_MAX;
}

void KGCVipGachaItemBox::IsExistItem( void )
{
    // 인벤 뒤져서 아이템 나오면 다 죽었어! 가 아니고 -_-; get표시 띄운다!
	if ( m_ItemInfo->dwSlotPosition & ESP_SET_ITEM && g_pItemMgr->CheckInventorySetItem( m_iItemID ) )
	{
		m_pkAlreadyGet->ToggleRender( true );
		m_bExistItem = true;
	}
	else if ( g_pItemMgr->FindInventoryForItemID( m_iItemID ) )
	{
		m_pkAlreadyGet->ToggleRender( true );
		m_bExistItem = true;
	}
    else
    {
        m_pkAlreadyGet->ToggleRender( false );
        m_bExistItem = false;
    }
}

void KGCVipGachaItemBox::SetItemInfo( int iItemID, int iCoinNum )
{
    m_iItemID = iItemID;
    m_iCoinNum = iCoinNum;

    if ( m_iItemID != -1 )
    {
        m_pkBuyBtn->ToggleRender( true );
        m_pkGachaCoin->ToggleRender( true );

        m_ItemInfo = g_pItemMgr->GetItemData( (DWORD)m_iItemID );

        int iSetNum = -1;
        if( m_ItemInfo->dwSlotPosition & ESP_SET_ITEM )
            iSetNum = m_ItemInfo->iSetItemNumber;

        if ( iSetNum == -1 )
        {
            m_pTextureItem = g_pItemMgr->CreateShopItemTexture( m_iItemID );
        }
        else
        {
            m_pTextureItem = g_pItemMgr->CreateShopSetItemTexture( iSetNum,m_iItemID );
        }

        WCHAR strTemp[10] = {0,};
        m_pkNeedBadgeNum->SetText( _itow( m_iCoinNum, strTemp, 10 ) );

        IsExistItem();
    }
    else
    {
        m_pkAlreadyGet->ToggleRender( false );
        m_pkMouseOver->ToggleRender( false );
        m_pkEquipedBack->ToggleRender( false );
        m_pkBuyBtn->ToggleRender( false );
        m_pkGachaCoin->ToggleRender( false );
        m_pkNeedBadgeNum->SetText( L"" );
        m_bIsEquip = false;
        m_ItemInfo = NULL;
    }
}

void KGCVipGachaItemBox::PostDraw( void )
{
    if( m_pTextureItem && m_iItemID != -1 )
    {
        m_pTextureItem->SetDeviceTexture();

        D3DXVECTOR2 vPos( GetFixedWindowPos() );
		vPos.x *= m_fWindowScaleX;
		vPos.y *= m_fWindowScaleY;

        if ( m_ItemInfo->dwSlotPosition != ESP_SET_ITEM )
        {
            g_pItemMgr->Draw_ShopImage( vPos + D3DXVECTOR2( 3.0f * GC_SCREEN_DIV_WIDTH, 3.0f * GC_SCREEN_DIV_WIDTH ), 80.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleX, 80.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleY, m_pTextureItem, false );
        }
        else
        {
            g_pItemMgr->Draw_ShopImage( vPos + D3DXVECTOR2( 3.0f * GC_SCREEN_DIV_WIDTH, 3.0f * GC_SCREEN_DIV_WIDTH ), 80.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleX, 80.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleY, m_pTextureItem, true, m_ItemInfo->iSetItemNumber,-1,m_ItemInfo->dwGoodsID );
        }

        g_pkUIMgr->RenderNumber( "blue_gradation_num", vPos + D3DXVECTOR2( 30.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleX, 62.0f * GC_SCREEN_DIV_WIDTH*m_fWindowScaleY ), m_iCoinNum, true, false );
    }
}

void KGCVipGachaItemBox::SetEquiped( bool equip )
{
    m_bIsEquip = equip;

    m_pkEquipedBack->ToggleRender( m_bIsEquip );
}

