#include "stdafx.h"
#include "GCAgitShopItemBox.h"

IMPLEMENT_CLASSNAME( KGCAgitShopItemBox );
IMPLEMENT_WND_FACTORY( KGCAgitShopItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitShopItemBox, "gc_agit_item_box" );

KGCAgitShopItemBox::KGCAgitShopItemBox( void )
{
    m_iMoneyType = MONEY_STAR;
    m_pkImgBackDefault = NULL;
    m_pkImgBackActive = NULL;
    m_pkIconStar = NULL;
    m_pkIconCash = NULL;
    m_pkStaticName = NULL;
    m_pkStaticPeriod = NULL;
    m_pkStaticCount = NULL;
    m_pkPriceBack = NULL;

    LINK_CONTROL( "imgBackDefault", m_pkImgBackDefault );
    LINK_CONTROL( "imgBackActive", m_pkImgBackActive );
    LINK_CONTROL( "iconStar", m_pkIconStar );
    LINK_CONTROL( "iconCash", m_pkIconCash );
    LINK_CONTROL( "staticName", m_pkStaticName );
    LINK_CONTROL( "staticPeriod", m_pkStaticPeriod );
    LINK_CONTROL( "staticCount", m_pkStaticCount );
    LINK_CONTROL( "priceBack", m_pkPriceBack );

    m_pkItemTex = NULL;
    m_pItemData = NULL;

    m_iPrice = 0;

    m_iNowCount = 0;
    m_iLimitCount = 0;
}

KGCAgitShopItemBox::~KGCAgitShopItemBox( void )
{
    SAFE_RELEASE( m_pkItemTex );
}

void KGCAgitShopItemBox::OnCreate( void )
{
    m_pkImgBackDefault->InitState( true, true, this );
    m_pkImgBackActive->InitState( false, true, this );
    m_pkIconStar->InitState( true, false, NULL );
    m_pkIconCash->InitState( false, false, NULL );
    
    m_pkStaticName->InitState( true, false, NULL );
    m_pkStaticPeriod->InitState( true, false, NULL );
    m_pkStaticCount->InitState( true, false, NULL );
    m_pkStaticName->SetAlign( DT_LEFT );
    m_pkStaticPeriod->SetAlign( DT_RIGHT );
    m_pkStaticCount->SetAlign( DT_LEFT );

    m_pkPriceBack->InitState( true, false, NULL );

    m_iNowCount = 0;
    m_iLimitCount = 0;
}

void KGCAgitShopItemBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkImgBackDefault, KD3DWnd::D3DWE_BUTTON_CLICK, OnClick );
}

void KGCAgitShopItemBox::PostChildDraw( void )
{
    if( m_pkItemTex == NULL )
        return;

    // 샵 이미지 그리기 
    float fX = 1.0f * GC_SCREEN_DIV_WIDTH;
    float fY = 0.0f * GC_SCREEN_DIV_WIDTH;
    float fH = 68.0f * GC_SCREEN_DIV_WIDTH;
    float fW = 68.0f * GC_SCREEN_DIV_WIDTH;

    float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();

    D3DXVECTOR2 dxvDrawPos = m_pkImgBackDefault->GetRelocatedWindowPos();
    dxvDrawPos.x += ( fX * fScaleX );
    dxvDrawPos.y += ( fY * fScaleY );
    g_pItemMgr->Draw_ShopImage( dxvDrawPos, fW * fScaleX, fH * fScaleY, m_pkItemTex, false, -1, 0xffffffff );

    // 가격 그리기 
    D3DXVECTOR2 dxvPricePos = m_pkPriceBack->GetRelocatedWindowPos();
    float fNumOffsetX = 87.0f * GC_SCREEN_DIV_WIDTH;
    float fNumOffsetY = 7.0f * GC_SCREEN_DIV_WIDTH;
    float fGap = 8.0f * GC_SCREEN_DIV_WIDTH; // very_small_num
    fNumOffsetX -= g_kGlobalValue.GetCiphers( m_iPrice ) * fGap;

    dxvPricePos.x += ( fNumOffsetX * fScaleX );
    dxvPricePos.y += ( fNumOffsetY * fScaleY );
    g_pkUIMgr->RenderNumber( "very_small_num", dxvPricePos, m_iPrice, D3DXCOLOR( 0.95f, 0.89f, 0.64f, 1.0f ) );
}

void KGCAgitShopItemBox::OnDestroy( void )
{
    BoxClear();
}

void KGCAgitShopItemBox::SetStarItemInfo( PAIR_USHORT_DWORD pariIndex_, KManufactureItem ItemInfo_, std::vector<KDropItemInfo>& vecMaterial_ )
{
    m_pItemData = g_pItemMgr->GetItemData( pariIndex_.second / 10 );
    if( NULL == m_pItemData )
        return ;

    m_PairIndex = pariIndex_;
    m_ItemInfo = ItemInfo_;
    m_vecMaterial = vecMaterial_;

    m_pkItemTex = g_pItemMgr->CreateItemTexture( m_PairIndex.second / 10 );

    for( std::vector<KDropItemInfo>::iterator it = m_vecMaterial.begin() ; it != m_vecMaterial.end() ; ++it )
    {
        if( it->m_ItemID == KGCItemManager::ITEM_AGIT_MONEY_STAR * 10 )  // 구매재료 - 일단 8880으로 되있음
        {
            m_iPrice = it->m_nDuration;
            break;
        }
    }
    
    m_iNowCount = SiKGCAgitGameManager()->GetInvenItemCount( m_pItemData->dwGoodsID * 10 );
    m_iLimitCount = ItemInfo_.m_nInvelLimit;
    SetBoxDetail();
}

void KGCAgitShopItemBox::SetCashItemInfo( GCItem* pItem )
{
    m_pItemData = pItem;
    m_pkItemTex = g_pItemMgr->CreateItemTexture( m_pItemData->dwGoodsID );

    m_iPrice = m_pItemData->dwPrice;
    m_iNowCount = SiKGCAgitGameManager()->GetInvenItemCount( pItem->dwGoodsID * 10 );
    m_iLimitCount = SiKGCAgitGameManager()->GetCashItemLimitCount( pItem->dwGoodsID * 10 );
    SetBoxDetail();
}

void KGCAgitShopItemBox::BoxClear( void )
{
    m_pItemData = NULL;
    SAFE_RELEASE( m_pkItemTex );

    KManufactureItem temp;
    m_PairIndex.first = 0;
    m_PairIndex.second = 0;
    m_ItemInfo = temp;
    m_vecMaterial.clear();

    m_pkStaticName->SetText( L"" );
    m_pkStaticPeriod->SetText( L"" );
    m_pkStaticCount->SetText( L"" );

    m_iPrice = 0;
}    

void KGCAgitShopItemBox::OnClick( void )
{
    if( m_pItemData != NULL )
        SetActive( true );

    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );  
}

void KGCAgitShopItemBox::SetBoxDetail( void )
{
    m_pkStaticName->SetForceWordWrapText( m_pItemData->strItemName , true );
    m_pkStaticName->SetToolTip( m_pItemData->strItemName );

    switch( m_iMoneyType )
    {
    case MONEY_STAR:
        {
#if defined (NATION_KOREA)
            m_pkStaticPeriod->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_ITEM_PERMANENT ) );
#else
            m_pkStaticPeriod->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_UNLIMITED_PERIOD ) );
#endif
        }
        break;

    case MONEY_CASH:
        {
            if( m_pItemData->eItemType == GCIT_PERIOD ) // 타입 기간제 
            {
                if( m_pItemData->iItemTypeValue == -1 )     // 영구
#if defined (NATION_KOREA)
                    m_pkStaticPeriod->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_ITEM_PERMANENT ) );
#else
                    m_pkStaticPeriod->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_UNLIMITED_PERIOD ) );
#endif
                else
                    m_pkStaticPeriod->SetText( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_SHOP_PERIOD, "i", m_pItemData->iItemTypeValue ) );
            }
            //m_pkStaticCount->SetText( L"" );
        }
        break;
    }

    std::wostringstream stm;
    stm << m_iNowCount << L" / " << m_iLimitCount;
    m_pkStaticCount->SetText( stm.str() );
}

void KGCAgitShopItemBox::SetActive( bool bActive_ )
{
    m_pkImgBackDefault->ToggleRender( !bActive_ );
    m_pkImgBackActive->ToggleRender( bActive_ );
}

void KGCAgitShopItemBox::SetMoneyType( int iType_ )
{
    switch ( iType_ )
    {
    case MONEY_STAR:
        m_iMoneyType = MONEY_STAR;
        m_pkIconStar->ToggleRender( true );
        m_pkIconCash->ToggleRender( false );
        break;

    case MONEY_CASH:
        m_iMoneyType = MONEY_CASH;
        m_pkIconStar->ToggleRender( false );
        m_pkIconCash->ToggleRender( true );
        break;
    }
}