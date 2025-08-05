#include "stdafx.h"
#include "GCCoordiViewItemWnd.h"
#include "GCItemImgWnd.h"
#include "GCUINumber.h"
#include "GCCustomizarionDlg.h"

IMPLEMENT_CLASSNAME( KGCCoordiViewItemWnd );
IMPLEMENT_WND_FACTORY( KGCCoordiViewItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiViewItemWnd, "gc_coordiview_item_wnd" );

KGCCoordiViewItemWnd::KGCCoordiViewItemWnd( void )
{
    m_itemID = 0;
    m_iType = 0;
    m_bActive = false;

    m_pItemImgWnd = NULL;
    m_pkSelItemImg = NULL;
    m_pkPriceNum = NULL;
    m_pkCoordiVIPImg = NULL;
    m_pkCoordiDiamondImg = NULL;
    m_pkEquipImg = NULL;
    m_pkEquipBtn = NULL;
    m_pkUnEquipBtn = NULL;
    m_pkBuyBtn = NULL;

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        m_pkGradeFrame[i] = NULL;
        LINK_CONTROL_STM( "item_frame_grade"<<i, m_pkGradeFrame[i] );
    }

    LINK_CONTROL( "item_img", m_pItemImgWnd );
    LINK_CONTROL( "inven_slot_sel", m_pkSelItemImg );
    LINK_CONTROL( "number_price", m_pkPriceNum);
    LINK_CONTROL( "img_vip_item", m_pkCoordiVIPImg );
    LINK_CONTROL( "img_diamond_item", m_pkCoordiDiamondImg );
    LINK_CONTROL( "equip_img", m_pkEquipImg );
    LINK_CONTROL( "equip_btn", m_pkEquipBtn );
    LINK_CONTROL( "unequip_btn", m_pkUnEquipBtn );
    LINK_CONTROL( "buy_btn", m_pkBuyBtn );
}

KGCCoordiViewItemWnd::~KGCCoordiViewItemWnd( void )
{
}

void KGCCoordiViewItemWnd::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        if ( m_pkEquipBtn )
            if ( event.m_pWnd == m_pkEquipBtn )
            {
                if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 1 ) )
                {
                    g_pRoomAnimationMgr->EquipRoomAnimation(&g_MyD3D->m_TempPlayer.GetCurrentChar(), m_itemID);
                }
                else if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 2 ) )
                {
                    g_pCutInSkinMgr->EquipCutInSkin(&g_MyD3D->m_TempPlayer.GetCurrentChar(), m_itemID);
                }
                else if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 0 ) )
                {
                    g_pFontSkinMgr->EquipFontSkin(&g_MyD3D->m_TempPlayer.GetCurrentChar(), m_itemID);
                }
            }
        if ( m_pkUnEquipBtn )
            if (event.m_pWnd == m_pkUnEquipBtn)
            {
                if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 1 ) )
                {
                    g_pRoomAnimationMgr->UnEquipRoomAnimation(&g_MyD3D->m_TempPlayer.GetCurrentChar(), true);
                }
                else if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 2 ) )
                {
                    g_pCutInSkinMgr->UnEquipCutInskin(&g_MyD3D->m_TempPlayer.GetCurrentChar(), true);
                }
                else if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 0 ) )
                {
                    g_pFontSkinMgr->UnEquipFontskin(&g_MyD3D->m_TempPlayer.GetCurrentChar(), true);
                }
            }
        if ( m_pkBuyBtn )
            if (event.m_pWnd == m_pkBuyBtn)
                OnClickBuy();
    }
}

void KGCCoordiViewItemWnd::OnCreate( void )
{
    m_pItemImgWnd->InitState( true, false, this );
    m_pItemImgWnd->ShowItem( false );

    if ( m_pkSelItemImg )
    {
        m_pkSelItemImg->InitState( false );
    }

    if (m_pkPriceNum)
    {
        m_pkPriceNum->InitState( false );
        m_pkPriceNum->SetStringType( "very_small_num" );
    }

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->InitState( false );        
    }

    if ( m_pkCoordiVIPImg )
    {
        m_pkCoordiVIPImg->InitState( false );
    }

    if ( m_pkCoordiDiamondImg )
    {
        m_pkCoordiDiamondImg->InitState( false );
    }

    if ( m_pkEquipImg )
    {
        m_pkEquipImg->InitState( false );
    }

    //Sistema de Skin de Dano
    if ( m_pkEquipBtn )
    {
        m_pkEquipBtn->InitState( false, true, this );
    }

    if (m_pkUnEquipBtn)
    {
        m_pkUnEquipBtn->InitState( false, true, this );
    }

    if (m_pkBuyBtn)
    {
        m_pkBuyBtn->InitState( false, true, this );
    }
}

void KGCCoordiViewItemWnd::SetItem( GCITEMID iItemID, int iType, bool bIsBuy )
{
    if( iItemID == 0 )
        return;

    m_itemID = iItemID;
    m_iType = iType;
    m_bIsBuy = bIsBuy;

    GCItem* pItem = g_pItemMgr->GetItemData( m_itemID / 10 );    
    if ( !pItem )
        return;

    bool bSetItem = ( pItem->dwSlotPosition & ESP_SET_ITEM );
    int iSetNumber = pItem->iSetItemNumber;
    char cGrade = pItem->cItemGrade;

    m_pItemImgWnd->SetItemInfo( pItem->dwGoodsID, -1, bSetItem, iSetNumber, 0, -1 );
    
    m_pItemImgWnd->ToggleRender( true );
    m_pItemImgWnd->ShowItem( true );

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->ToggleRender( false );        
    }
    if ( m_pkGradeFrame[cGrade] )
        m_pkGradeFrame[cGrade]->ToggleRender( true );

    SetColor( -1 );
    SetItemType( m_iType );

    //Sistema de Skin de Dano
    SetFontSkinButtons( m_iType );
    SetItemPrice( pItem->dwPrice );
}

void KGCCoordiViewItemWnd::SetItemCustom(GCITEMID iItemID, int iType, bool bIsBuy)
{
    if (iItemID == 0)
        return;

    m_itemID = iItemID;
    m_iType = iType;
    m_bIsBuy = bIsBuy;

    GCItem* pItem = g_pItemMgr->GetItemData(m_itemID / 10);
    if (!pItem)
        return;

    bool bSetItem = (pItem->dwSlotPosition & ESP_SET_ITEM);
    int iSetNumber = pItem->iSetItemNumber;
    char cGrade = pItem->cItemGrade;

    m_pItemImgWnd->SetCustomItemInfo(pItem->dwGoodsID, -1, bSetItem, iSetNumber, 0, -1);

    m_pItemImgWnd->ToggleRender(true);
    m_pItemImgWnd->ShowItem(true);

    for (int i = 0; i < KItem::GRADE_NUM; ++i)
    {
        if (m_pkGradeFrame[i])
            m_pkGradeFrame[i]->ToggleRender(false);
    }
    if (m_pkGradeFrame[cGrade])
        m_pkGradeFrame[cGrade]->ToggleRender(true);

    SetColor(-1);
    SetItemType(m_iType);

    //Sistema de Skin de Dano
    SetFontSkinButtons(m_iType);
    SetItemPrice(pItem->dwPrice);
}

void KGCCoordiViewItemWnd::SetItemPrice( int nPrice )
{
    if( !m_pkPriceNum )
        return;
    if( m_bIsBuy && m_pkBuyBtn )
    {
        if ( !g_pItemMgr->FindInventoryForItemID( m_itemID / 10 ) && m_pkBuyBtn->IsRenderOn() ) {
            m_pkPriceNum->ToggleRender( true );
            m_pkPriceNum->SetNumber( nPrice );
        }
        else
            m_pkPriceNum->ToggleRender( false );
    }
    else
        m_pkPriceNum->ToggleRender( false );
}

void KGCCoordiViewItemWnd::ResetItem()
{
    m_itemID = 0;
    m_iType = 0;
    m_bActive = false;
    m_bIsBuy = false;

    m_pItemImgWnd->ToggleRender( false );
    m_pItemImgWnd->ShowItem( false );

    if (m_pkPriceNum)
        m_pkPriceNum->ToggleRender( false );

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->ToggleRender( false );     

    if ( m_pkCoordiVIPImg )
        m_pkCoordiVIPImg->ToggleRender( false );

    if ( m_pkCoordiDiamondImg )
        m_pkCoordiDiamondImg->ToggleRender( false );

    if ( m_pkEquipImg )
        m_pkEquipImg->ToggleRender( false );

    if (m_pkBuyBtn)
        m_pkBuyBtn->ToggleRender( false );

    if ( m_pkEquipBtn )
        m_pkEquipBtn->ToggleRender( false );

    if ( m_pkUnEquipBtn )
        m_pkUnEquipBtn->ToggleRender( false );
}

void KGCCoordiViewItemWnd::SelectedItem( bool bShow )
{
    if ( m_pkSelItemImg )
        m_pkSelItemImg->ToggleRender( bShow );
    m_bActive = bShow;
}

void KGCCoordiViewItemWnd::SetItemType( int iType )
{
    if( iType == 0 ){
        if ( m_pkCoordiVIPImg )
            m_pkCoordiVIPImg->ToggleRender( false );
        if ( m_pkCoordiDiamondImg )
            m_pkCoordiDiamondImg->ToggleRender( false );
    }
    else if( iType == 1 ){
        if ( m_pkCoordiVIPImg )
            m_pkCoordiVIPImg->ToggleRender( false );
        if ( m_pkCoordiDiamondImg )
            m_pkCoordiDiamondImg->ToggleRender( true );
    }
    else if( iType == 2 ){
        if ( m_pkCoordiVIPImg )
            m_pkCoordiVIPImg->ToggleRender( true );
        if ( m_pkCoordiDiamondImg )
            m_pkCoordiDiamondImg->ToggleRender( false );
    }
    else {
        if (m_pkCoordiVIPImg)
            m_pkCoordiVIPImg->ToggleRender(false);
        if (m_pkCoordiDiamondImg)
            m_pkCoordiDiamondImg->ToggleRender(false);
    }

    if ( m_bIsBuy ) 
    {
        if ( m_pkBuyBtn )
        {
            m_pkBuyBtn->ToggleRender( true );
        }
    }
    else
    {
        if ( m_pkBuyBtn )
        {
            m_pkBuyBtn->ToggleRender( false );
        }
    }

}

void KGCCoordiViewItemWnd::SetColor(IN const DWORD dwColor_)
{
    m_pItemImgWnd->SetColor(dwColor_);
}

//Sistema de Skin de Dano
void KGCCoordiViewItemWnd::SetFontSkinButtons( int iType )
{
    if ( g_pItemMgr->FindInventoryForItemID( m_itemID / 10 ) )
    {
        if ( IsEquipedItem() )
        {
            if( m_pkUnEquipBtn )
                m_pkUnEquipBtn->ToggleRender( true );
            if( m_pkEquipBtn )
                m_pkEquipBtn->ToggleRender( false );
            if( m_pkBuyBtn )
                m_pkBuyBtn->ToggleRender( false );
        }
        else
        {
            if( m_pkUnEquipBtn )
                m_pkUnEquipBtn->ToggleRender( false );
            if( m_pkEquipBtn )
                m_pkEquipBtn->ToggleRender( true );
            if( m_pkBuyBtn )
                m_pkBuyBtn->ToggleRender( false );
        }
    }
    else
    {
        if ( m_pkUnEquipBtn )
            m_pkUnEquipBtn->ToggleRender( false );
        if ( m_pkEquipBtn )
            m_pkEquipBtn->ToggleRender( false );
    }
}

bool KGCCoordiViewItemWnd::IsEquipedItem()
{
    for ( auto& element : g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo )
        if ( element.iItemID == m_itemID / 10 )
            return true;
    return false;
}

void KGCCoordiViewItemWnd::OnClickBuy()
{
    GCItem* pItem = g_pItemMgr->GetItemData( m_itemID / 10 );
    if ( pItem->eMoneyType == EMT_CASH )
        g_MyD3D->m_kItemMgr.BuyCashItemS6( pItem->dwGoodsID );
    else if ( pItem->eMoneyType == EMT_CRYSTAL ) 
    {
        std::wostringstream strmText1;
        std::wostringstream strmText2;
        strmText1 << pItem->strItemName;
        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "iI", (int)pItem->dwPrice, STR_ID_CRYSTAL_TEXT );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_BUY_FONTSKIN, pItem->dwGoodsID, pItem->iItemTypeValue, true, true );
    }
    else
        g_MyD3D->m_kItemMgr.BuyItem( pItem->dwGoodsID, pItem->iItemTypeValue );
}