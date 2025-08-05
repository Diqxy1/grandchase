#include "stdafx.h"
#include "GCEnchantSubShopPanel.h"

IMPLEMENT_CLASSNAME( KGCEnchantSubShopPanel );
IMPLEMENT_WND_FACTORY( KGCEnchantSubShopPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantSubShopPanel, "gc_enchantsubshoppanel" );

KGCEnchantSubShopPanel::KGCEnchantSubShopPanel( void )
: m_pkBtnPrev(NULL)
, m_pkBtnNext(NULL)
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < ESHOP_ITEM_WND_NUM; ++i )
    {
        m_pkItemWnd[i] = NULL;
        sprintf( str, "shop_item_img%d", i );  
        LINK_CONTROL( str, m_pkItemWnd[i] ); 

        m_pkBtnBuy[i] = NULL;
        sprintf( str, "btn_buy%d", i );
        LINK_CONTROL( str, m_pkBtnBuy[i] );
    }

    LINK_CONTROL( "btn_prev", m_pkBtnPrev ); 
    LINK_CONTROL( "btn_next", m_pkBtnNext );     
}

KGCEnchantSubShopPanel::~KGCEnchantSubShopPanel( void )
{
}


void KGCEnchantSubShopPanel::OnCreate( void )
{
    m_pkBtnPrev->InitState(true, true, this);
    m_pkBtnNext->InitState(true, true, this);    

    for( int i = 0; i < ESHOP_ITEM_WND_NUM; i++ )
    {
        m_pkItemWnd[i]->InitState(true);
        m_pkBtnBuy[i]->InitState(true, true, this);
    }
    InitShopInfo();
}

void KGCEnchantSubShopPanel::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnPrev,			KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevPage );
    GCWND_MSG_MAP( m_pkBtnNext,			KD3DWnd::D3DWE_BUTTON_CLICK, OnNextPage );

    for( int i = 0; i < ESHOP_ITEM_WND_NUM; i++ )
    {
        if( event.m_pWnd == m_pkBtnBuy[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            int iRealIndex = i+m_iCurPage * ESHOP_ITEM_WND_NUM;
            if( iRealIndex < static_cast<int>(m_vecShopItemList.size()) )
                OnBuy(m_vecShopItemList[i+m_iCurPage * ESHOP_ITEM_WND_NUM]/10);
            else
                OnBuy(-1);
        }
    }
}

void KGCEnchantSubShopPanel::FrameMoveInEnabledState( void )
{
}

void KGCEnchantSubShopPanel::LockUI( bool bLock )
{
    for ( int i = 0; i < ESHOP_ITEM_WND_NUM; ++i ) 
    {
        m_pkBtnBuy[i]->Lock( bLock );
    }

    m_pkBtnPrev->Lock( bLock );
    m_pkBtnNext->Lock( bLock );
}

bool KGCEnchantSubShopPanel::RenderItemInfoBox()
{
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    for( int i = 0; i < ESHOP_ITEM_WND_NUM; ++i )
    {
        if( m_pkItemWnd[i]->CheckPosInWindowBound( vMousePos ) )
        {
            // 아이템 정보 표시창 위치 보정.
            D3DXVECTOR2 vecPos =m_pkItemWnd[i]->GetCurrentWindowPos();

            int iRealIndex = i+m_iCurPage * ESHOP_ITEM_WND_NUM;
            if( iRealIndex < static_cast<int>(m_vecShopItemList.size()) )
            {
                vecPos.x *= m_fWindowScaleX;
                vecPos.y *= m_fWindowScaleY;
                vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

                GCItem* pItem = g_pItemMgr->GetItemData(m_vecShopItemList[i+m_iCurPage * ESHOP_ITEM_WND_NUM]/10);

                g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, -1 );
                g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
                g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

                g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
                g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
                return true;
            }            
        }
    }

    return false;
}

void KGCEnchantSubShopPanel::InitShopInfo()
{    
    m_iCurPage = 0;
    UpdateItemWnd();    
}

void KGCEnchantSubShopPanel::OnPrevPage()
{
    if( m_iCurPage == 0 )
        return;

    m_iCurPage--;    
    UpdateItemWnd();
}

void KGCEnchantSubShopPanel::OnNextPage()
{
    if( ( m_iCurPage + 1 ) * ESHOP_ITEM_WND_NUM >= static_cast<int>(m_vecShopItemList.size()) )
        return;

    m_iCurPage++;
    UpdateItemWnd();
}

void KGCEnchantSubShopPanel::OnBuy( GCITEMID dwItemID_ )
{
    GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( dwItemID_ );
    // 인벤토리가 제한 수량을 초과하면 구매를 막는다.
    if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() >= g_kGlobalValue.m_iInvenCapacity )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
            g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", g_kGlobalValue.m_iInvenCapacity ),
            g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    else if( kItem != NULL )
    {        
        if ( kItem->eMoneyType == EMT_CASH ) // 캐쉬 아이템을 경우 캐쉬 구입용 다이얼로그 박스를 바로 띄운다.
        {        
            if ( g_kGlobalValue.m_kUserInfo.iAge < CASH_BUY_LIMIT_AGE ) // 만 7세 미만인지 확인
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY1 ),
                    g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            }
            else
            {
                bool bOpenBuyWnd = true;

                if( bOpenBuyWnd )
                {
#if defined ( USE_VIP_POINT )
                    g_pItemMgr->BuyCashItemS6( kItem->dwGoodsID );
#else
#ifdef GAME_CASH
                    g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false, true );
#else
                    g_MyD3D->m_kItemMgr.BuyItem( kItem->dwGoodsID, kItem->iItemTypeValue  );
#endif
#endif
                }
            }
        }
        else // GP 아이템인 경우 구입 의사를 확인해본다.
        {
            g_pItemMgr->BuyGPItem( kItem, dwItemID_, kItem->iItemTypeValue );
        }
    }
}

void KGCEnchantSubShopPanel::SetEnchantShopItemList( std::set<GCITEMID> setShopItemList )
{
    m_vecShopItemList.clear();
    std::set<GCITEMID>::iterator sit = setShopItemList.begin();

    for( ; sit != setShopItemList.end(); ++sit )
    {
        m_vecShopItemList.push_back(*sit);
    }
}

void KGCEnchantSubShopPanel::UpdateItemWnd()
{    
    int iRealIdx = m_iCurPage * ESHOP_ITEM_WND_NUM;

    for( int i = 0; i < ESHOP_ITEM_WND_NUM; i++ )
    {
        m_pkItemWnd[i]->ToggleRender(false);
        m_pkBtnBuy[i]->ToggleRender(false);
        if( i+iRealIdx >= static_cast<int>(m_vecShopItemList.size()) )
            continue;

        m_pkItemWnd[i]->SetItemInfo(m_vecShopItemList[i+iRealIdx]/10);
        m_pkItemWnd[i]->ToggleRender(true);
        m_pkBtnBuy[i]->ToggleRender(true);
    }
}

bool KGCEnchantSubShopPanel::CheckItemInformation()
{
    for( int i = 0; i < ESHOP_ITEM_WND_NUM; ++i )
    {
        if( m_pkItemWnd[i]->CheckMousePosInWindowBound() )
        {
            // 아이템 정보 표시창 위치 보정.
            D3DXVECTOR2 vecPos =m_pkItemWnd[i]->GetCurrentWindowPos();

            int iRealIndex = i+m_iCurPage * ESHOP_ITEM_WND_NUM;
            if( iRealIndex < static_cast<int>(m_vecShopItemList.size()) )
            {
                vecPos.x *= m_fWindowScaleX;
                vecPos.y *= m_fWindowScaleY;
                vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

                GCItem* pItem = g_pItemMgr->GetItemData(m_vecShopItemList[i+m_iCurPage * ESHOP_ITEM_WND_NUM]/10);

                g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, -1 );
                g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
                g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

                g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
                g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
                return true;
            }            
        }
    }

    return false;
}