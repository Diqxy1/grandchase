#include "stdafx.h"
#include "GCCoordiItemBox.h"
#include "GCBuyCashItemChooseCashTypeDlg.h"

IMPLEMENT_CLASSNAME( KGCCoordiItemBox );
IMPLEMENT_WND_FACTORY( KGCCoordiItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiItemBox, "gc_coordi_item_box" );

KGCCoordiItemBox::KGCCoordiItemBox( void )
{
    m_pkItemTexture = NULL;

    m_pkItemBackDefault = NULL;
    m_pkItemBackActive = NULL;
    LINK_CONTROL( "item_back_default", m_pkItemBackDefault );
    LINK_CONTROL( "item_back_active", m_pkItemBackActive );

    m_pkStaticItemName = NULL;
    LINK_CONTROL( "static_item_name", m_pkStaticItemName );

    for( int i=0 ; i<NUM_PRICE ; ++i )
    {
        m_apkStaticItemPeriod[i] = NULL;
        m_apkStaticItemPrice[i] = NULL;
        m_apkBtnBuy[i] = NULL;

        char szPeriod[100] = {0, };
        char szPrice[100] = {0, };
        char szBtnBuy[100] = {0, };
        char szBtnCoin[100] = {0, };
        sprintf_s( szPeriod, 99, "static_period%d", i );
        sprintf_s( szPrice,  99, "static_price%d", i );
        sprintf_s( szBtnBuy, 99, "btn_buy%d", i );
        sprintf_s( szBtnCoin, 99, "cash_icon%d", i );
    
        LINK_CONTROL( szPeriod, m_apkStaticItemPeriod[i] );
        LINK_CONTROL( szPrice,  m_apkStaticItemPrice[i] );
        LINK_CONTROL( szBtnBuy, m_apkBtnBuy[i] );
        LINK_CONTROL( szBtnCoin, m_apkCoin[i] );
        
    }

    m_iBoxType = COORDI_ITEM_BOX_NORMAL;
    m_bHovered = false;
    m_bIsEquip = false;
}

KGCCoordiItemBox::~KGCCoordiItemBox( void )
{
}

void KGCCoordiItemBox::OnCreate( void )
{
    m_pkItemBackDefault->InitState( true, true, this );
    m_pkItemBackActive->InitState( false, true, this );

    for( int i=0 ; i<NUM_PRICE ; ++i )
    {
        m_pkStaticItemName->InitState( true, false, NULL );
        m_apkStaticItemPeriod[i]->InitState( true, false, NULL );
        m_apkStaticItemPrice[i]->InitState( true, false, NULL );
        m_apkBtnBuy[i]->InitState(true, true, this );
        m_apkCoin[i]->InitState(true, false, NULL );
        

        m_pkStaticItemName->SetAlign( DT_LEFT );
        m_apkStaticItemPeriod[i]->SetAlign( DT_LEFT );
        m_apkStaticItemPrice[i]->SetAlign( DT_LEFT );
    }

	if ( GC_GLOBAL_DEFINE::bReformCoordi ) 
	{
	    m_apkBtnBuy[LOW_PRICE]->SetToolTip( g_pkStrLoader->GetString( STR_ID_BUY_COORDI_NOTIFY ) );
	}

    m_iBoxType = COORDI_ITEM_BOX_NORMAL;
}

void KGCCoordiItemBox::OnDestroy( void )
{
    SAFE_RELEASE( m_pkItemTexture );
}

void KGCCoordiItemBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_apkBtnBuy[ HIGH_PRICE ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnBuyBtn( HIGH_PRICE );

    if( event.m_pWnd == m_apkBtnBuy[ LOW_PRICE ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnBuyBtn( LOW_PRICE );

    GCWND_MSG_MAP( m_pkItemBackDefault, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquip );
    GCWND_MSG_MAP( m_pkItemBackActive, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquip );
}

void KGCCoordiItemBox::FrameMoveInEnabledState( void )
{
    POINT   pt = g_pkInput->GetMousePos();
    m_bHovered = (m_kCoordiItemInfo.pOriginal != NULL || m_kCoordiItemInfo.pPackage7 != NULL) && 
                 ( (m_pkItemBackDefault->CheckPosInWindowBoundWithChild( D3DXVECTOR2( static_cast<float>(pt.x), static_cast<float>(pt.y) ) )) || 
                   (m_pkItemBackActive->CheckPosInWindowBoundWithChild( D3DXVECTOR2( static_cast<float>(pt.x), static_cast<float>(pt.y) ) ))      );
}

void KGCCoordiItemBox::PostChildDraw( void )
{
    // 샵 이미지 그리기
    float x = 0.0f;
    float y = 0.0f; 
    float fw = 0.0f;
    float fh = 0.0f;

    float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();

    if( m_iBoxType == COORDI_ITEM_BOX_NORMAL )
    {
        x = 10.0f * GC_SCREEN_DIV_WIDTH;
        y = 12.0f * GC_SCREEN_DIV_WIDTH;
        fw = 70.0f * GC_SCREEN_DIV_WIDTH;
        fh = 70.0f * GC_SCREEN_DIV_WIDTH;
    }
    else if ( m_iBoxType == COORDI_ITEM_BOX_HOT )
    {
        x = -25.0f * GC_SCREEN_DIV_WIDTH;
        y = 16.0f * GC_SCREEN_DIV_WIDTH;
        fw = 71.0f * GC_SCREEN_DIV_WIDTH;
        fh = 71.0f * GC_SCREEN_DIV_WIDTH;
    }

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    //LUA_GET_VALUE_DEF( "x", x, 0.0f );
    //LUA_GET_VALUE_DEF( "y", y, 0.0f );
    //LUA_GET_VALUE_DEF( "fw", fw, 102.0f );
    //LUA_GET_VALUE_DEF( "fh", fh, 102.0f );

    D3DXVECTOR2 dxvItemPos = GetRelocatedWindowPos();;
    dxvItemPos.x += (x * fScaleX);
    dxvItemPos.y += (y * fScaleY);


    g_pItemMgr->Draw_ShopImage(dxvItemPos,  fw * fScaleX, fh * fScaleY, m_pkItemTexture, false, -1, 0xffffffff);
}


void KGCCoordiItemBox::Clear( void )
{
    m_kCoordiItemInfo.pOriginal = NULL;
    m_kCoordiItemInfo.pPackage7 = NULL;
    m_bHovered = false;

    SetEquip( false );
    m_pkStaticItemName->SetText( L"" );
    
    for( int i=0 ; i<NUM_PRICE ; ++i )
    {
        m_apkStaticItemPeriod[i]->SetText( L"" );
        m_apkStaticItemPrice[i]->SetText( L"" );
    }

    m_iBoxType = COORDI_ITEM_BOX_NORMAL;
    SAFE_RELEASE( m_pkItemTexture );
}

void KGCCoordiItemBox::SetItemInfo( KCoordiShopItem kItemInfo )
{
    // 코디샵 아이템 맞는지 체크 
    if( kItemInfo.pOriginal && kItemInfo.pOriginal->eItemKind != GIK_COORDI_ITEM && kItemInfo.pOriginal->eItemKind != GIK_COORDIBOX )
        return;
    if( kItemInfo.pPackage7 && kItemInfo.pPackage7->eItemKind != GIK_COORDI_ITEM && kItemInfo.pOriginal->eItemKind != GIK_COORDIBOX )
        return;

    m_kCoordiItemInfo = kItemInfo;
    if(kItemInfo.pOriginal)
    {
        m_pkStaticItemName->SetForceWordWrapText( m_kCoordiItemInfo.pOriginal->strItemName,true);
        m_pkItemTexture = g_pItemMgr->CreateItemTexture( m_kCoordiItemInfo.pOriginal->dwGoodsID);
    }
    else if(kItemInfo.pPackage7)
    {
        m_pkStaticItemName->SetForceWordWrapText( m_kCoordiItemInfo.pPackage7->strItemName,true);
        m_pkItemTexture = g_pItemMgr->CreateItemTexture( m_kCoordiItemInfo.pPackage7->dwGoodsID);
    }

    if(kItemInfo.pOriginal)
    {
        m_apkStaticItemPrice[HIGH_PRICE]->SetNumber( m_kCoordiItemInfo.pOriginal->dwPrice );
        m_apkStaticItemPeriod[HIGH_PRICE]->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_ITEM_PERMANENT ) );
        m_apkBtnBuy[ HIGH_PRICE ]->Lock(false);
        m_apkBtnBuy[ HIGH_PRICE ]->ToggleRender(true);
        m_apkCoin[HIGH_PRICE]->ToggleRender(true);
    }
    else
    {
        m_apkCoin[HIGH_PRICE]->ToggleRender(false);
        m_apkBtnBuy[ HIGH_PRICE ]->ToggleRender(false);
        m_apkBtnBuy[ HIGH_PRICE ]->Lock(true);
        m_apkStaticItemPrice[HIGH_PRICE]->SetText( L"" );
        m_apkStaticItemPeriod[HIGH_PRICE]->SetText( L"");
    }


    if(kItemInfo.pPackage7)
    {
        m_apkStaticItemPrice[LOW_PRICE]->SetNumber( m_kCoordiItemInfo.pPackage7->dwPrice );
        m_apkStaticItemPeriod[LOW_PRICE]->SetText( g_pkStrLoader->GetString(STR_ID_PERIOD7) );
        m_apkBtnBuy[ LOW_PRICE ]->Lock(false);
        m_apkBtnBuy[ LOW_PRICE ]->ToggleRender(true);
        m_apkCoin[LOW_PRICE]->ToggleRender(true);
    }
    else
    {
        m_apkCoin[LOW_PRICE]->ToggleRender(false);
        m_apkBtnBuy[ LOW_PRICE ]->ToggleRender(false);
        m_apkBtnBuy[ LOW_PRICE ]->Lock(true);
        m_apkStaticItemPrice[LOW_PRICE]->SetText( L"" );
        m_apkStaticItemPeriod[LOW_PRICE]->SetText( L"");
    }
}

void KGCCoordiItemBox::OnEquip( void )
{
    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
}

void KGCCoordiItemBox::SetEquip( bool bEquip )
{
    m_bIsEquip = bEquip;

    m_pkItemBackDefault->ToggleRender( m_bIsEquip ? false : true );
    m_pkItemBackActive->ToggleRender( m_bIsEquip ? true : false );
}

void KGCCoordiItemBox::OnBuyBtn( int iBuyType )
{
    // 타입 따라 아이템 정보 가져옴
    GCItem* pItem = NULL;
    switch( iBuyType )
    {
    case HIGH_PRICE:
        pItem = m_kCoordiItemInfo.pOriginal;
        break;

    case LOW_PRICE:
        pItem = m_kCoordiItemInfo.pPackage7;
        break;
    }

    if( pItem == NULL )
        return;

    // 실제 구입
    if( g_kGlobalValue.m_kUserInfo.iAge < CASH_BUY_LIMIT_AGE )  // 7세 미만 확인
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY1 ),
            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY2 ) );
    }
    else
    {
        bool bOpenBuyWnd = true;
#if defined( VIRTUAL_CASH ) && !defined( GAME_CASH ) && !defined( NATION_KOREA )
        if( g_kGlobalValue.m_dwVirtualCash >= pItem->dwPrice )
        {
            g_pkUIScene->m_pkAskUseVirtualCash->SetItemInfo( pItem->dwGoodsID, pItem->iItemTypeValue );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_VIRTUAL_CASH );
            bOpenBuyWnd = false;
        }
#endif

        if( bOpenBuyWnd ){
#if defined ( USE_VIP_POINT )
            g_pItemMgr->BuyCashItemS6( pItem->dwGoodsID );
#else
    #if defined GAME_CASH
            g_pkUIScene->EnableBuyCashItemWnd( pItem->dwGoodsID, false );
    #else
            g_pItemMgr->BuyItem( pItem->dwGoodsID, pItem->iItemTypeValue );
    #endif
#endif
        }
    }
}

void KGCCoordiItemBox::SetBoxType( int iType_ )
{
    if( iType_ < COORDI_ITEM_BOX_NORMAL || iType_ > NUM_COORDI_ITEM_BOX_TYPE )
        return;

    m_iBoxType = iType_;
}