#include "stdafx.h"
#include "GCSocialCommerceDlg.h"
#include "GCSocialCommerceManager.h"


IMPLEMENT_CLASSNAME( KGCSocialCommerceDlg );
IMPLEMENT_WND_FACTORY( KGCSocialCommerceDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSocialCommerceDlg, "gc_social_commerce_main" );

KGCSocialCommerceDlg::KGCSocialCommerceDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnBuy = NULL;
    m_pkBtnLeft = NULL;
    m_pkBtnRight = NULL;
    m_pkTabLimitedGoods = NULL;
    m_pkTabSocialCommerce = NULL;
    m_pkBtnReward = NULL;
    m_pkWndOnSale = NULL;
    m_pkWndAchievement = NULL;
    m_pkWndSoldOut = NULL;
    m_pkWndNotForSale = NULL;

    m_pkStaticPageNum = NULL;
    m_pkStaticPeriod = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticPriceText = NULL;
    m_pkStaticPrice = NULL;
    m_pkStaticGoalText = NULL;
    m_pkStaticGoal = NULL;

    m_pkItemImg = NULL;

    m_pkSocialCommerceRewardBox = NULL;

    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_buy", m_pkBtnBuy );
    LINK_CONTROL( "btn_left", m_pkBtnLeft );
    LINK_CONTROL( "btn_right", m_pkBtnRight );
    LINK_CONTROL( "btn_order_of_arrival", m_pkTabLimitedGoods );
    LINK_CONTROL( "btn_social_commerce", m_pkTabSocialCommerce );
    LINK_CONTROL( "btn_reward", m_pkBtnReward );
    LINK_CONTROL( "wnd_on_sale", m_pkWndOnSale );
    LINK_CONTROL( "wnd_achievement", m_pkWndAchievement );
    LINK_CONTROL( "wnd_sold_out", m_pkWndSoldOut );
    LINK_CONTROL( "wnd_not_for_sale", m_pkWndNotForSale );

    LINK_CONTROL( "static_page_num", m_pkStaticPageNum );
    LINK_CONTROL( "static_period", m_pkStaticPeriod );
    LINK_CONTROL( "static_item_name", m_pkStaticItemName );
    LINK_CONTROL( "static_price_text", m_pkStaticPriceText );
    LINK_CONTROL( "static_price", m_pkStaticPrice );
    LINK_CONTROL( "static_goal_text", m_pkStaticGoalText );
    LINK_CONTROL( "static_goal", m_pkStaticGoal );

    LINK_CONTROL( "item_img", m_pkItemImg );

    LINK_CONTROL( "social_commerce_reward_box", m_pkSocialCommerceRewardBox );

    // 
    m_eCurrentTab = SCT_LIMITED_GOODS;
    m_dwCurrentGoodsIndex = 0;
    m_dwCountLimitedGoods = 0;
    m_dwCountSocialCommerce = 0;

    m_mapLimitedGoods.clear();
    m_mapSocialCommerce.clear();
}

KGCSocialCommerceDlg::~KGCSocialCommerceDlg( void )
{
}


void KGCSocialCommerceDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBtnBuy->InitState( true, true, this );
    m_pkBtnLeft->InitState( true, true, this );
    m_pkBtnRight->InitState( true, true, this );
    m_pkTabLimitedGoods->InitState( true, true, this );
    m_pkTabSocialCommerce->InitState( true, true, this );
    m_pkBtnReward->InitState( true, true, this );
    m_pkWndOnSale->InitState( false );
    m_pkWndAchievement->InitState( false );
    m_pkWndSoldOut->InitState( false );
    m_pkWndNotForSale->InitState( false );

    m_pkStaticPageNum->InitState( true );
    m_pkStaticPageNum->SetAlign( DT_CENTER );
    m_pkStaticPeriod->InitState( true );
    m_pkStaticPeriod->SetAlign( DT_CENTER );
    m_pkStaticItemName->InitState( true );
    m_pkStaticItemName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_pkStaticItemName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) ); // yellow
    m_pkStaticPriceText->InitState( true );
    m_pkStaticPriceText->SetText( g_pkStrLoader->GetString( STR_ID_SOCIAL_COMMERCE_1 ) );
    m_pkStaticPrice->InitState( true );
    m_pkStaticPrice->SetAlign( DT_RIGHT );
    m_pkStaticGoalText->InitState( true );
    m_pkStaticGoalText->SetText( g_pkStrLoader->GetString( STR_ID_SOCIAL_COMMERCE_2 ) );
    m_pkStaticGoal->InitState( true );
    m_pkStaticGoal->SetAlign( DT_RIGHT );


    m_pkItemImg->InitState( true );

    m_pkSocialCommerceRewardBox->InitState( false, true, this );

}

void KGCSocialCommerceDlg::OnCreateComplete( void )
{
    // 기본텝은 제한구매(선착순)로
    m_pkTabLimitedGoods->SetWndMode( D3DWM_ACTIVE );
    m_pkTabSocialCommerce->SetWndMode( D3DWM_DEFAULT );

    //
    m_dwCurrentGoodsIndex = 0;
    ShowCurrentGoods();
}

void KGCSocialCommerceDlg::FrameMoveInEnabledState( void )
{
    //
}


void KGCSocialCommerceDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,                   KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCloseBtn );
    _GCWND_MSG_MAP( m_pkBtnLeft,                    KD3DWnd::D3DWE_BUTTON_CLICK, OnClickPrevBtn );
    _GCWND_MSG_MAP( m_pkBtnRight,                   KD3DWnd::D3DWE_BUTTON_CLICK, OnClickNextBtn );
    _GCWND_MSG_MAP( m_pkBtnBuy,                     KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBuyBtn );
    _GCWND_MSG_MAP( m_pkBtnReward,                  KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRewardBtn );
    _GCWND_MSG_MAP_PARAM( m_pkTabLimitedGoods,      KD3DWnd::D3DWE_BUTTON_CLICK, OnClickTab, SCT_LIMITED_GOODS );
    _GCWND_MSG_MAP_PARAM( m_pkTabSocialCommerce,    KD3DWnd::D3DWE_BUTTON_CLICK, OnClickTab, SCT_SOCIAL_COMMERCE );
    
}

void KGCSocialCommerceDlg::OnClickCloseBtn()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}


void KGCSocialCommerceDlg::SetGoodsInfo( IN KEVENT_SOSCIAL_COMMERCE_INFO_ACK& kData )
{
    // 기존 데이터는 삭제
    m_mapLimitedGoods.clear();
    m_mapSocialCommerce.clear();

    m_dwCountLimitedGoods = 0;
    m_dwCountSocialCommerce = 0;

    // 분류작업 시작
    // 제한구매 상품( 수량, 기간 )   or   공동구매( 1+1, 보너스 아이템 지급 )
    KEVENT_SOSCIAL_COMMERCE_INFO_ACK::iterator iterData = kData.begin();
    
    for ( ; iterData != kData.end(); ++iterData ){
        if ( iterData->first == SCGT_LIMITED_COUNT || iterData->first == SCGT_LIMITED_PERIOD ) {
            AddtoLimitedGoods( iterData->second );
        } else if ( iterData->first == SCGT_ONE_PLUS_ONE || iterData->first == SCGT_IN_SALE ) {
            AddtoSocialCommerce( iterData->second );
        }
    }

    // 판매하는 아이템이 하나도 없다면 해당 텝은 잠그자
    if( m_dwCountLimitedGoods == 0 )
        m_pkTabLimitedGoods->SetWndMode( D3DWM_LOCK );

    if( m_dwCountSocialCommerce == 0 )
        m_pkTabSocialCommerce->SetWndMode( D3DWM_LOCK );

    // end
}

void KGCSocialCommerceDlg::AddtoLimitedGoods( IN KInformationDataOfSellType& kData_ )
{
    // 제한구매 상품들을 순서대로 재구성
    std::map<DWORD, KSCSellInfo>::iterator iterSI = kData_.m_mapSellItem.begin();

    for( ; iterSI != kData_.m_mapSellItem.end(); ++iterSI ){
        m_mapLimitedGoods.insert( std::make_pair( m_dwCountLimitedGoods, (*iterSI).second ) );
        ++m_dwCountLimitedGoods;
    }
}

void KGCSocialCommerceDlg::AddtoSocialCommerce( IN KInformationDataOfSellType& kData_ )
{
    // 공동구매는 추가보상이 있으니까. 같이 묶어줌
    std::map<DWORD, KSCSellInfo>::iterator iterSI = kData_.m_mapSellItem.begin();
    std::map<DWORD, VEC_REWARD>::iterator iterRI = kData_.m_mapRewardItem.begin();

    for( ; iterSI != kData_.m_mapSellItem.end(); ++iterSI, ++iterRI ){
        m_mapSocialCommerce.insert( std::make_pair( m_dwCountSocialCommerce, std::make_pair( (*iterSI).second, (*iterRI).second ) ) );
        ++m_dwCountSocialCommerce;
    }
}

void KGCSocialCommerceDlg::OnClickTab( IN SocialCommerceTab eTab )
{
    // 현재텝과 같으면
    if( m_eCurrentTab == eTab ){
        return;
    } else {
        m_eCurrentTab = eTab;
        m_dwCurrentGoodsIndex = 0;
    }
}

void KGCSocialCommerceDlg::OnClickNextBtn()
{
    if( m_eCurrentTab == SCT_LIMITED_GOODS ) { 
        if( m_dwCurrentGoodsIndex < m_dwCountLimitedGoods - 1 ) {
            ++m_dwCurrentGoodsIndex;
            ShowCurrentGoods();
        }
    } else if( m_eCurrentTab == SCT_SOCIAL_COMMERCE ) { 
        if( m_dwCurrentGoodsIndex < m_dwCountSocialCommerce - 1 ) {
            ++m_dwCurrentGoodsIndex;
            ShowCurrentGoods();
        }
    }
}

void KGCSocialCommerceDlg::OnClickPrevBtn()
{
    if( m_dwCurrentGoodsIndex > 0 )
        --m_dwCurrentGoodsIndex;

    ShowCurrentGoods();
}

void KGCSocialCommerceDlg::ShowCurrentGoods()
{
    ResetUI();

    // 상품텝이 어디에 있니
    if ( m_eCurrentTab == SCT_LIMITED_GOODS ) { 
        // 여기에서 아이템 세팅
        // m_dwCurrentGoodsIndex 이 녀석이 각 상품의 키값과 동일
        std::map<DWORD, KSCSellInfo>::iterator iterLG = m_mapLimitedGoods.find( m_dwCurrentGoodsIndex );

        if( iterLG == m_mapLimitedGoods.end() )
            return;

        SetItemInfo( (*iterLG).second.m_dwItemID / 10 );
        SetItemCount( (*iterLG).second.m_nCount, (*iterLG).second.m_nLimitCount );
        SetItemPeriod( (*iterLG).second.m_tmBegin, (*iterLG).second.m_tmEnd );

    } else if ( m_eCurrentTab == SCT_SOCIAL_COMMERCE ){ 
        std::map<DWORD, std::pair<KSCSellInfo, VEC_REWARD>>::iterator iterSC = m_mapSocialCommerce.find( m_dwCurrentGoodsIndex );

        if( iterSC == m_mapSocialCommerce.end() )
            return;

        std::pair<KSCSellInfo, VEC_REWARD>& prInfo = (*iterSC).second;

        SetItemInfo( prInfo.first.m_dwItemID / 10 );
        SetItemCount( prInfo.first.m_nCount, prInfo.first.m_nLimitCount );
        SetItemPeriod( prInfo.first.m_tmBegin, prInfo.first.m_tmEnd );
    }
}

void KGCSocialCommerceDlg::SetItemInfo( IN DWORD dwItemID )
{
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( dwItemID );

    m_pkItemImg->SetItemInfo( pkItem->dwGoodsID );
    m_pkStaticItemName->SetText( pkItem->strItemName );
    m_pkStaticPrice->SetText( pkItem->dwPrice );
}

void KGCSocialCommerceDlg::SetItemCount( IN int nCount, IN int nLimitCount )
{
    if( nCount >= nLimitCount ){
        nCount = nLimitCount;
        m_pkWndSoldOut->ToggleRender( true );
        m_pkBtnBuy->SetWndMode( D3DWM_LOCK );
    } else {
        m_pkBtnBuy->SetWndMode( D3DWM_DEFAULT );
    }

    m_pkStaticGoal->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_MIX_PAGE_NUM, "ii", nCount, nLimitCount ) );
}

void KGCSocialCommerceDlg::SetItemPeriod( IN time_t tmBegin, IN time_t tmEnd )
{
    CTime tmTimeBegin( tmBegin );
    CTime tmTimeEnd( tmEnd );

    m_pkStaticPeriod->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SOCIAL_COMMERCE_3, "iiiiii", tmTimeBegin.GetYear(), tmTimeBegin.GetMonth(), tmTimeBegin.GetDay(), 
        tmTimeEnd.GetYear(), tmTimeEnd.GetMonth(), tmTimeEnd.GetDay() ) );

    m_pkStaticPageNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_MIX_PAGE_NUM, "ii", m_dwCurrentGoodsIndex + 1, m_dwCountLimitedGoods ) );

    if( g_kGlobalValue.m_tmServerTime >= tmTimeEnd ){
        // 종료됨
        m_pkWndNotForSale->ToggleRender( true );
        m_pkBtnBuy->SetWndMode( D3DWM_LOCK );
    } else {
        m_pkBtnBuy->SetWndMode( D3DWM_DEFAULT );
    }
}

void KGCSocialCommerceDlg::ResetUI()
{
    m_pkBtnBuy->SetWndMode( D3DWM_LOCK );
    m_pkBtnLeft->SetWndMode( D3DWM_LOCK );
    m_pkBtnRight->SetWndMode( D3DWM_LOCK );
    m_pkBtnReward->SetWndMode( D3DWM_LOCK );
    m_pkWndOnSale->ToggleRender( false );
    m_pkWndAchievement->ToggleRender( false );
    m_pkWndSoldOut->ToggleRender( false );
    m_pkWndNotForSale->ToggleRender( false );
}

void KGCSocialCommerceDlg::OnClickBuyBtn()
{
    if ( m_eCurrentTab == SCT_LIMITED_GOODS ) { 
        std::map<DWORD, KSCSellInfo>::iterator iterLG = m_mapLimitedGoods.find( m_dwCurrentGoodsIndex );
        if( iterLG == m_mapLimitedGoods.end() )
            return;

        Buy( (*iterLG).second.m_dwItemID / 10 );

    } else if ( m_eCurrentTab == SCT_SOCIAL_COMMERCE ) {
        std::map<DWORD, std::pair<KSCSellInfo, VEC_REWARD>>::iterator iterSC = m_mapSocialCommerce.find( m_dwCurrentGoodsIndex );
        if( iterSC == m_mapSocialCommerce.end() )
            return;

        std::pair<KSCSellInfo, VEC_REWARD>& prInfo = (*iterSC).second;
        Buy( prInfo.first.m_dwItemID / 10 );
    }
}

void KGCSocialCommerceDlg::Buy( GCITEMID dwItemID_ )
{
    // 타입을 별도로 설정해야 하는데 뭐였지.. ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ






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
#ifdef GAME_CASH
                g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false, true );
#else    
    #if defined( NEW_GPITEM_BUY_PROCESS )
                g_MyD3D->m_kItemMgr.BuyCashItemWithPoint( kItem, kItem->iItemTypeValue );
    #else
                g_MyD3D->m_kItemMgr.BuyItem( dwItemID_, kItem->iItemTypeValue  );
    #endif
#endif
            }
        }
    }
}

void KGCSocialCommerceDlg::OnClickRewardBtn()
{
    std::map<DWORD, std::pair<KSCSellInfo, VEC_REWARD>>::iterator iterSC = m_mapSocialCommerce.find( m_dwCurrentGoodsIndex );
    if( iterSC == m_mapSocialCommerce.end() )
        return;

    m_pkSocialCommerceRewardBox->SetRewardInfo( (*iterSC).second );
}
