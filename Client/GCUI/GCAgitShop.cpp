#include "stdafx.h"
#include "GCAgitShop.h"
#include "GCAgitShopItemBox.h"

#include "KGCAgitGameManager.h"

IMPLEMENT_CLASSNAME( KGCAgitShop );
IMPLEMENT_WND_FACTORY( KGCAgitShop );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitShop, "gc_agit_shop" );

KGCAgitShop::KGCAgitShop( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i] = NULL;

    LINK_CONTROL( "btnClose", m_apkBtn[BTN_CLOSE] );
    LINK_CONTROL( "btnPlace", m_apkBtn[BTN_PLACE] );
    LINK_CONTROL( "btnBuy",   m_apkBtn[BTN_BUY] );
    LINK_CONTROL( "btnFirst", m_apkBtn[BTN_FIRST] );
    LINK_CONTROL( "btnPrev",  m_apkBtn[BTN_PREV] );
    LINK_CONTROL( "btnEnd",   m_apkBtn[BTN_END] );
    LINK_CONTROL( "btnNext",  m_apkBtn[BTN_NEXT] );

#if defined( NATION_PHILIPPINE )
    LINK_CONTROL( "btnCashCharge",  m_apkBtn[BTN_CASH_CHARGE] );
#endif

    LINK_CONTROL( "tabStarDefault", m_apkTab[TAB_STAR_DEFAULT] );
    LINK_CONTROL( "tabStarActive",  m_apkTab[TAB_STAR_ACTIVE] );
    LINK_CONTROL( "tabCashDefault", m_apkTab[TAB_CASH_DEFAULT] );
    LINK_CONTROL( "tabCashActive",  m_apkTab[TAB_CASH_ACTIVE] );

    m_pkStaticPage = NULL;
    LINK_CONTROL( "staticPage", m_pkStaticPage );

    for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
    {
        m_apkItemBox[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "itemBox%d", i );
        LINK_CONTROL( szTemp, m_apkItemBox[i] );
    }

    m_pkImgBack = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticItemDesc = NULL;
    m_pkStaticItemNumDesc = NULL;
    m_pkStaticItemNum = NULL;
    LINK_CONTROL( "imgBack",          m_pkImgBack );
    LINK_CONTROL( "staticItemName",   m_pkStaticItemName );
    LINK_CONTROL( "staticDesc",       m_pkStaticItemDesc );
    LINK_CONTROL( "staticNumDesc",    m_pkStaticItemNumDesc );
    LINK_CONTROL( "staticNum",        m_pkStaticItemNum );

    m_pkIconStar = NULL;
    m_pkIconCash = NULL;
    LINK_CONTROL( "iconStar", m_pkIconStar );
    LINK_CONTROL( "iconCash", m_pkIconCash );

    m_pkItemTex = NULL;

    m_iCurTab = TAB_STAR_ACTIVE;
    m_iCurPage = 0;
    m_iTotalPage = 0;

    m_iCurBoxIndex = -1;
    m_pCurItem  = NULL;
    
    m_iTotalStarItemCount = 0;
    m_iTotalCashItemCount = 0;

    m_vecStarShopItem.clear();
    m_vecCashShopItem.clear();

    m_iSavedCurPage = 0;
    m_bIsSavedCurPage = false;
}

KGCAgitShop::~KGCAgitShop( void )
{
    SAFE_RELEASE( m_pkItemTex );
}

void KGCAgitShop::OnCreate( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i]->InitState( false, true, this );

    for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
        m_apkItemBox[i]->InitState( true, true, this );

    m_pkImgBack->InitState( true, false, NULL );
    m_pkStaticItemName->InitState( true, false, NULL );
    m_pkStaticItemDesc->InitState( true, false, NULL );
    m_pkStaticItemNumDesc->InitState( true, false, NULL );
    m_pkStaticItemNum->InitState( true, false, NULL );
    m_pkStaticItemNum->SetAlign( DT_RIGHT );
    m_pkStaticItemNumDesc->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_ITEM_COUNT ) );
    m_pkStaticItemDesc->SetMultiLine( true );
    
    m_pkStaticPage->InitState( true, false, NULL );
    m_pkStaticPage->SetAlign( DT_CENTER );

    m_pkIconStar->InitState( true );
    m_pkIconCash->InitState( false );

    m_apkBtn[ BTN_CLOSE ]->SetHotKey( DIK_ESCAPE );

    OnPressTab( m_iCurTab );
    SetItemBox();
    SetCurItemInfo( m_iCurBoxIndex );
}

void KGCAgitShop::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_apkTab[ TAB_STAR_DEFAULT ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnPressTab( TAB_STAR_DEFAULT );

    if( event.m_pWnd == m_apkTab[ TAB_CASH_DEFAULT ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnPressTab( TAB_CASH_DEFAULT );

    for( int i=BTN_FIRST ; i<NUM_BTN ; ++i )
    {
        if( event.m_pWnd == m_apkBtn[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            OnPageBtn( i );
            break;
        }
    }

    for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
    {
        if( event.m_pWnd == m_apkItemBox[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            if( m_iCurBoxIndex != -1 && m_iCurBoxIndex != i )
            {
                if( m_apkItemBox[ i ]->GetItemData() != NULL )
                    m_apkItemBox[ m_iCurBoxIndex ]->SetActive( false );
            }
            
            SetCurItemInfo( i );
        }   
    }

    GCWND_MSG_MAP( m_apkBtn[ BTN_CLOSE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_apkBtn[ BTN_BUY ],     KD3DWnd::D3DWE_BUTTON_CLICK,    OnBuy   );
    GCWND_MSG_MAP( m_apkBtn[ BTN_PLACE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnPrePlace );
#if defined( NATION_PHILIPPINE )
    GCWND_MSG_MAP( m_apkBtn[ BTN_CASH_CHARGE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCashCharge );
#endif

}

void KGCAgitShop::InitAgitShop( bool bIsStarTab )
{
    SetStarShopItemInfo( SiKGCAgitGameManager()->GetShopItem(), SiKGCAgitGameManager()->GetShopItemMaterial() );
    SetCashShopItemInfo();
    if ( bIsStarTab ) 
        OnPressTab( TAB_STAR_DEFAULT );
    else 
        OnPressTab( TAB_CASH_DEFAULT );

    m_iCurBoxIndex = -1;
    m_pCurItem = NULL;
}

void KGCAgitShop::SetStarShopItemInfo( std::map<PAIR_USHORT_DWORD,KManufactureItem>& mapItemInfo_, std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterialInfo_ )
{
    if( !m_vecStarShopItem.empty() )
        return;

    std::map<PAIR_USHORT_DWORD,KManufactureItem>::iterator ItemIter;
    std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >::iterator MaterialIter;

    for( ItemIter = mapItemInfo_.begin() ; ItemIter != mapItemInfo_.end() ; ++ItemIter )
    {
        MaterialIter = mapMaterialInfo_.find( ItemIter->first );
        if( MaterialIter == mapMaterialInfo_.end() )
            continue;

        KAgitShopItem tempInfo;
        tempInfo.pairIndex = ItemIter->first;
        tempInfo.itemInfo = ItemIter->second;
        tempInfo.vecMaterial = MaterialIter->second;

        m_vecStarShopItem.push_back( tempInfo );
    }

    m_iTotalStarItemCount = static_cast<int>( m_vecStarShopItem.size() );
}

void KGCAgitShop::SetCashShopItemInfo( void )
{
    if( !m_vecCashShopItem.empty() )
        return;

    g_pItemMgr->MakeAgitCashItemList( m_vecCashShopItem );
    m_iTotalCashItemCount = static_cast<int>( m_vecCashShopItem.size() );
}

void KGCAgitShop::ClearShopInfo( void )
{
    m_vecStarShopItem.clear();
    m_vecCashShopItem.clear();
}

void KGCAgitShop::OnClose( void )
{
    g_pkAgitOverlayUI->ToggleRenderBrownieBtn( true );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAgitShop::OnCashCharge()
{
    CashRechargeWebOpen();
}

void KGCAgitShop::OnPressTab( int iTabType_ )
{
    switch ( iTabType_ )
    {
    case TAB_STAR_DEFAULT:
    case TAB_STAR_ACTIVE:
        m_apkTab[ TAB_STAR_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_STAR_ACTIVE ]->ToggleRender( true );
        m_apkTab[ TAB_CASH_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_CASH_ACTIVE ]->ToggleRender( false );
        m_iCurTab = TAB_STAR_ACTIVE;
        m_pkStaticItemNumDesc->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_ITEM_COUNT ) );

#if defined( NATION_PHILIPPINE )
        m_apkBtn[BTN_CASH_CHARGE]->ToggleRender( false );
#endif

        break;

    case TAB_CASH_DEFAULT:
    case TAB_CASH_ACTIVE:
        m_apkTab[ TAB_STAR_DEFAULT ]->ToggleRender( true );
        m_apkTab[ TAB_STAR_ACTIVE ]->ToggleRender( false );
        m_apkTab[ TAB_CASH_DEFAULT ]->ToggleRender( false );
        m_apkTab[ TAB_CASH_ACTIVE ]->ToggleRender( true );
        m_iCurTab = TAB_CASH_ACTIVE;
        m_pkStaticItemNumDesc->SetText( g_pkStrLoader->GetString( STR_ID_MONEYTYPE_CASH_POINT ) );
#if defined( NATION_USA ) || defined( NATION_EU )
        g_kGlobalValue.UpdateCashData();
#endif
#if defined( NATION_PHILIPPINE )
        m_apkBtn[BTN_CASH_CHARGE]->ToggleRender( true );
#endif

        break;
    }

    InitPage();
    GetSavedCurPage();
    ClearCurItemInfo();
    SetItemBox();
}

void KGCAgitShop::OnPageBtn( int iBtnType_ )
{
    if( iBtnType_ < BTN_FIRST || iBtnType_ > BTN_NEXT )
        return;

    if( 0 == m_iTotalPage )
        return;

    switch( iBtnType_ )
    {
    case BTN_FIRST:
        m_iCurPage = 1;
        ClearCurItemInfo();
        break;

    case BTN_PREV:
        if( m_iCurPage > 1 )
        {
            --m_iCurPage;
            ClearCurItemInfo();
        }
        break;

    case BTN_END:
        m_iCurPage = m_iTotalPage;
        ClearCurItemInfo();
        break;

    case BTN_NEXT:
        if( m_iCurPage < m_iTotalPage )
        {
            ++m_iCurPage;
            ClearCurItemInfo();
        }
        break;
    }

    SetItemBox();
    g_KDSound.Play( "984040" );

    std::wostringstream stm;
    stm << m_iCurPage << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText( stm.str() );
}

void KGCAgitShop::InitPage( void )
{
    int iRemain = 0;
    switch( m_iCurTab )
    {
    case TAB_STAR_ACTIVE:
        iRemain = m_iTotalStarItemCount % NUM_ITEM_BOX;
        m_iTotalPage = m_iTotalStarItemCount / NUM_ITEM_BOX;
        break;

    case TAB_CASH_ACTIVE:
        iRemain = m_iTotalCashItemCount % NUM_ITEM_BOX;
        m_iTotalPage = m_iTotalCashItemCount / NUM_ITEM_BOX;
        break;

    default:
        iRemain = m_iTotalStarItemCount % NUM_ITEM_BOX;
        m_iTotalPage = m_iTotalStarItemCount / NUM_ITEM_BOX;
        break;
    }

    if( iRemain > 0 )
        ++m_iTotalPage;

    if( 0 == m_iTotalPage )
        m_iCurPage = 0;
    else
        m_iCurPage = 1;

    std::wostringstream stm;
    stm << m_iCurPage << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText( stm.str() );
}

void KGCAgitShop::UpdatePage( void )
{
	if ( !IsRenderOn() ) 
		return;

	SetItemBox();

	if( NULL == m_apkItemBox[ m_iCurBoxIndex ]->GetItemData() )
		return;

	SetNowNumStar();
}


void KGCAgitShop::SetItemBox()
{
    if( m_iCurPage < 0 || m_iCurPage > m_iTotalPage ) 
        return;

    for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
        m_apkItemBox[i]->BoxClear();

    int iStartIndex = ( m_iCurPage - 1 ) * NUM_ITEM_BOX;

    if( TAB_STAR_ACTIVE == m_iCurTab && !m_vecStarShopItem.empty() )
    {
        for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
        {
            m_apkItemBox[i]->SetMoneyType( KGCAgitShopItemBox::MONEY_STAR );

            int iIndex = iStartIndex + i;
            if( iIndex >= m_iTotalStarItemCount )
                continue;

            if( iIndex < 0 || iIndex >= static_cast<int>( m_vecStarShopItem.size() ) )
                continue;

            KAgitShopItem tempInfo = m_vecStarShopItem[ iIndex ];
            m_apkItemBox[i]->SetStarItemInfo( tempInfo.pairIndex, tempInfo.itemInfo, tempInfo.vecMaterial );
        }
    }
    else if( TAB_CASH_ACTIVE == m_iCurTab && !m_vecCashShopItem.empty() )
    {
        for( int i=0 ; i<NUM_ITEM_BOX ; ++i )
        {
            m_apkItemBox[i]->SetMoneyType( KGCAgitShopItemBox::MONEY_CASH );

            int iIndex = iStartIndex + i;
            if( iIndex >= m_iTotalStarItemCount )
                continue;

            if( iIndex < 0 || iIndex >= static_cast<int>( m_vecCashShopItem.size() ) )
                continue;

            GCItem* pItem = g_pItemMgr->GetItemData( m_vecCashShopItem[ iIndex ] );
            if( NULL == pItem )
                continue;

            m_apkItemBox[i]->SetCashItemInfo( pItem );
        }   
    }
}

void KGCAgitShop::OnBuy( void )
{
    if( -1 == m_iCurBoxIndex )
        return;

    if( NULL == m_apkItemBox[ m_iCurBoxIndex ]->GetItemData() )
        return;

    if ( g_pkUIScene->m_pkAgitShop ) {
        g_pkUIScene->m_pkAgitShop->SetSavedCurPage();
    }

    SiKGCAgitGameManager()->BuyAgitShopObject( m_apkItemBox[ m_iCurBoxIndex ]->GetItemData()->dwGoodsID, m_apkItemBox[ m_iCurBoxIndex ]->GetPairIndex() );

}

void KGCAgitShop::OnPrePlace( void )
{
    if( -1 == m_iCurBoxIndex )
        return;

    GCItem* pItem = m_apkItemBox[ m_iCurBoxIndex ]->GetItemData();
    if( NULL == pItem )
        return;

    if ( g_pkUIScene->m_pkAgitShop ) {
        g_pkUIScene->m_pkAgitShop->SetSavedCurPage();
    }

    g_pkAgitOverlayUI->ShowPrePlaceUI( true, pItem, m_apkItemBox[ m_iCurBoxIndex ]->GetMoneyType(), m_apkItemBox[ m_iCurBoxIndex ]->GetPrice(), m_apkItemBox[ m_iCurBoxIndex ]->GetPairIndex() );

    SiKGCAgitGameManager()->SetPrePlace( true );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAgitShop::SetCurItemInfo( int iIndex_ )
{
    if( iIndex_ < 0 || iIndex_ >= static_cast<int>(m_apkItemBox.size()) )
        return;

    GCItem* pItem = m_apkItemBox[ iIndex_ ]->GetItemData();
    if( NULL == pItem )
        return;

    m_iCurBoxIndex = iIndex_;
    m_pCurItem = pItem;

    // 아이템 이름, 설명
    m_pkStaticItemName->SetText( m_pCurItem->strItemName );
    m_pkStaticItemDesc->SetTextAutoMultiline( m_pCurItem->strItemDesc );

    // 텍스쳐 
    SAFE_RELEASE( m_pkItemTex );
    m_pkItemTex = g_pItemMgr->CreateItemTexture( m_pCurItem->dwGoodsID );

    // 아이콘 
    if( pItem->eMoneyType == EMT_CASH )
    {
        m_pkIconCash->ToggleRender( true );
        m_pkIconStar->ToggleRender( false );
    }
    else
    {
        m_pkIconCash->ToggleRender( false );
        m_pkIconStar->ToggleRender( true );
    }
    SetNowNumStar();

    // 사운드 
    g_KDSound.Play( "984029" );
}       

void KGCAgitShop::PostChildDraw( void )
{
    if( -1 == m_iCurBoxIndex )
        return;

    if( NULL == m_pkItemTex )
        return;

    // 이미지 그리기 
    float fX = 15.0f * GC_SCREEN_DIV_WIDTH;
    float fY = 3.0f * GC_SCREEN_DIV_WIDTH;
    float fH = 130.0f * GC_SCREEN_DIV_WIDTH;
    float fW = 130.0f * GC_SCREEN_DIV_WIDTH;

    float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();

    D3DXVECTOR2 dxvDrawPos = m_pkImgBack->GetRelocatedWindowPos();
    dxvDrawPos.x += ( fX * fScaleX );
    dxvDrawPos.y += ( fY * fScaleY );

    g_pItemMgr->Draw_ShopImage( dxvDrawPos, fW * fScaleX, fH * fScaleY, m_pkItemTex, false, -1, 0xffffffff );

    // 가격 그리기 
    float fNumOffsetX = 85.0f * GC_SCREEN_DIV_WIDTH;
    float fNumOffsetY = 145.0f * GC_SCREEN_DIV_WIDTH;
    float fGap = 8.0f * GC_SCREEN_DIV_WIDTH;  // very_small_num 
    fNumOffsetX -= g_kGlobalValue.GetCiphers( m_apkItemBox[ m_iCurBoxIndex]->GetPrice() ) * fGap;
    
    dxvDrawPos.x += ( fNumOffsetX * fScaleX );
    dxvDrawPos.y += ( fNumOffsetY * fScaleY );
    g_pkUIMgr->RenderNumber( "very_small_num", dxvDrawPos, m_apkItemBox[ m_iCurBoxIndex]->GetPrice(), D3DXCOLOR( 0.95f, 0.89f, 0.64f, 1.0f ) );
}

void KGCAgitShop::ClearCurItemInfo( void )
{
    if( m_iCurBoxIndex != -1 )
    {
        m_apkItemBox[ m_iCurBoxIndex ]->SetActive( false );
        m_iCurBoxIndex = -1;
        m_pCurItem = NULL;
        m_pkStaticItemName->SetText( L"" );
        m_pkStaticItemDesc->SetText( L"" );
        m_pkStaticItemNum->SetText( L"" );
    }
}

void KGCAgitShop::SetNowNumStar( void )
{
    if( m_iCurTab == TAB_CASH_ACTIVE )
    {
        //m_pkStaticItemNum->SetText( L"" );
        //return;
        m_pkStaticItemNum->SetNumber( g_kGlobalValue.m_dwCash );
    } else {
        KItem* pItem = NULL;
        g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_AGIT_MONEY_STAR, &pItem );

        if( pItem == NULL )
            m_pkStaticItemNum->SetNumber( 0 );
        else
            m_pkStaticItemNum->SetNumber( pItem->m_nCount );
    }
}

void KGCAgitShop::SetSavedCurPage()
{
    m_iSavedCurPage = m_iCurPage;
    m_bIsSavedCurPage = true;
}

void KGCAgitShop::GetSavedCurPage()
{
    if ( m_bIsSavedCurPage ) {
        m_iCurPage = m_iSavedCurPage;
        m_bIsSavedCurPage = false;

        std::wostringstream stm;
        stm << m_iCurPage << L" / " << m_iTotalPage;
        m_pkStaticPage->SetText( stm.str() );
    }
}