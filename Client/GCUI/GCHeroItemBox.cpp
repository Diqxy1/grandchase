#include "stdafx.h"
#include "GCHeroItemBox.h"

IMPLEMENT_CLASSNAME( KGCHeroItemBox );
IMPLEMENT_WND_FACTORY( KGCHeroItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroItemBox, "gc_hero_item_box" );

KGCHeroItemBox::KGCHeroItemBox( void )
{
    m_pItem = NULL;
    m_pMaterialItem = NULL;
    
    m_bItemHovered = false;
    m_bCoinHovered = false;
    m_bVIPHovered = false;
    m_bMaterialItemHovered = false;
    m_bPreview = false;  
    m_bLock = false; 

    m_emGameMode = GC_GM_INVALID;

    m_pkBtnBuy = NULL;
    m_pkBtnSelect = NULL;
    m_pkBtnPreView = NULL;
    m_pkItemBackGround = NULL;
    m_pkMaterialItemPriceBack = NULL;
    m_pkMaterialItemBackground = NULL;

    m_pkHeroCoin = NULL;
    m_pkHeroCoinPiece = NULL;
    m_pkHeroVIP = NULL;
    m_pkEventMark = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticItemType = NULL;
    m_pkStaticItemPrice = NULL;
    m_pkStaticPrevItemNum = NULL;

    m_pkItemTexture = NULL;
    m_pkPrevItemTexture = NULL;

    LINK_CONTROL( "btn_buy", m_pkBtnBuy );
    LINK_CONTROL( "btn_select", m_pkBtnSelect );
    LINK_CONTROL( "btn_preview", m_pkBtnPreView );
    LINK_CONTROL( "background2", m_pkItemBackGround );
    LINK_CONTROL( "background4", m_pkMaterialItemPriceBack);
    LINK_CONTROL( "background_item", m_pkMaterialItemBackground );
    
    LINK_CONTROL( "hero_coin", m_pkHeroCoin );
    LINK_CONTROL( "hero_coin_piece", m_pkHeroCoinPiece );
    LINK_CONTROL( "hero_vip_badge", m_pkHeroVIP );
    LINK_CONTROL( "event_mark", m_pkEventMark );
    LINK_CONTROL( "static_item_name", m_pkStaticItemName);
    LINK_CONTROL( "static_item_type", m_pkStaticItemType);
    LINK_CONTROL( "static_item_price", m_pkStaticItemPrice);
    LINK_CONTROL( "static_prev_item_num", m_pkStaticPrevItemNum);
}

KGCHeroItemBox::~KGCHeroItemBox( void )
{
}

void KGCHeroItemBox::OnCreate( void )
{
    if ( m_pkBtnBuy ) 
        m_pkBtnBuy->InitState(true, true, this);
    
    if ( m_pkBtnSelect ) 
        m_pkBtnSelect->InitState( false, true, this );


    m_pkBtnPreView->InitState( false, true, this);

    m_pkHeroCoin->InitState( true, false, this );
    m_pkHeroCoinPiece->InitState( true, false, this );
    m_pkHeroVIP->InitState( true, false, this );

    if (m_pkEventMark)
        m_pkEventMark->InitState( false, false, this );

	if(m_pkMaterialItemPriceBack)
	    m_pkMaterialItemPriceBack->InitState( false, false, NULL );
    if(m_pkMaterialItemBackground)
		m_pkMaterialItemBackground->InitState( false, false, NULL );

    m_pkStaticItemName->SetFontColor(0xffffffff);
    m_pkStaticItemName->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemName->SetAlign(DT_LEFT);
    m_pkStaticItemName->SetShadow(true);
    m_pkStaticItemName->InitState(true, false, this);

    m_pkStaticItemType->SetFontColor(0xffffffff);
    m_pkStaticItemType->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemType->SetAlign(DT_LEFT);
    m_pkStaticItemType->InitState(true, false, this);

    m_pkStaticItemPrice->SetFontColor(0xffffffff);
    m_pkStaticItemPrice->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemPrice->SetAlign(DT_LEFT);
    m_pkStaticItemPrice->InitState(true, false, this);

    m_pkStaticPrevItemNum->SetFontColor(0xffffffff);
    m_pkStaticPrevItemNum->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticPrevItemNum->SetAlign(DT_LEFT);
    m_pkStaticPrevItemNum->InitState(true, false, this);
}

void KGCHeroItemBox::OnDestroy( void )
{
    SAFE_RELEASE( m_pkItemTexture );
    SAFE_RELEASE( m_pkPrevItemTexture );
}

void KGCHeroItemBox::SetBuyType( bool _bBuy )
{
    if ( m_pkBtnBuy ) 
        m_pkBtnBuy->ToggleRender( _bBuy );

    if ( m_pkBtnSelect ) 
        m_pkBtnSelect->ToggleRender( !_bBuy );
}

void KGCHeroItemBox::RenderSelectBtn( bool _bRender )
{
    if ( m_pkBtnSelect )
    {
        m_pkBtnSelect->ToggleRender( _bRender );
        m_pkBtnSelect->Lock( false );
    }
}

void KGCHeroItemBox::FrameMoveInEnabledState( void )
{
    if( m_bLock )
        return;

    POINT   pt = g_pkInput->GetMousePos();
    m_bItemHovered = m_pkItemBackGround->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );
    m_bCoinHovered = m_pkHeroCoin->CheckPosInWindowBound(D3DXVECTOR2((float)pt.x, (float)pt.y));
    m_bVIPHovered = m_pkHeroVIP->CheckPosInWindowBound(D3DXVECTOR2((float)pt.x, (float)pt.y));

    if( m_pkMaterialItemBackground )
    {
        if( m_pkMaterialItemBackground->IsRenderOn() )
            m_bMaterialItemHovered = m_pkMaterialItemBackground->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );
    }    

    if( m_bItemHovered &&  m_pItem != NULL && m_pItem->dwGoodsID != 0 )
    {
        if( g_pItemMgr->IsEquipmentItem( m_pItem->dwGoodsID ) && !(m_pItem->dwSlotPosition & ESP_A_EARRING) && (m_pItem->eItemKind != GIK_MONSTER_CARD) && (m_pItem->eItemKind != GIK_PET))
            m_pkBtnPreView->ToggleRender(true);
    }
    else 
    {
        m_pkBtnPreView->ToggleRender(false);
    }
}

void KGCHeroItemBox::ActionPerformed( const KActionEvent& event )
{
    
    if( event.m_pWnd == m_pkBtnPreView && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        m_bPreview = true;
        m_pkBtnPreView->SetWndMode(D3DWM_LOCK);
        m_pkBtnPreView->ToggleRender(true);
    }

    if( event.m_pWnd == m_pkBtnBuy && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && !m_bLock )
    {
        std::wstring strTemp = m_pkStaticItemPrice->GetText();
        int iPrice = _wtoi( strTemp.c_str() );
        
        strTemp.clear();
        strTemp = m_pkStaticPrevItemNum->GetText();
        int iMaterialItemNum = _wtoi( strTemp.c_str() );

        DWORD dwItemID = m_pItem->dwGoodsID * 10;

		if(m_pkMaterialItemBackground && m_pkMaterialItemBackground->IsRenderOn())
        {
            std::wostringstream strm;
            strm << g_pItemMgr->GetItemGradeColorKey( m_pMaterialItem->cItemGrade );
            strm << g_pItemMgr->GetItemName( m_pMaterialItem->dwGoodsID );
            strm << L"#cX";

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, 
                g_pkStrLoader->GetReplacedString( STR_ID_BUY_HERO_ITEM_COIN_MATERIAL,  "ili", iPrice, strm.str(), iMaterialItemNum ), L"",
                KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, dwItemID, 0, true, true);
        }
        else if( m_pkHeroCoin->IsRenderOn() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, 
                g_pkStrLoader->GetReplacedString( STR_ID_BUY_HERO_ITEM_HERO_COIN,  "i", iPrice ), L"",
                KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, m_kHeroItem.m_pairIdIndex.second, m_kHeroItem.m_pairIdIndex.first, true, true);
        }
        else if(m_pkHeroCoinPiece->IsRenderOn())
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, 
                g_pkStrLoader->GetReplacedString( STR_ID_BUY_HERO_ITEM_HERO_COIN_PIECE,  "i", iPrice ), L"",
                KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, m_kHeroItem.m_pairIdIndex.second, m_kHeroItem.m_pairIdIndex.first, true, true);
        }
        else
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
                g_pkStrLoader->GetReplacedString(STR_ID_BUY_HERO_ITEM_HERO_VIP, "i", iPrice), L"",
                KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, m_kHeroItem.m_pairIdIndex.second, m_kHeroItem.m_pairIdIndex.first, true, true);
        }
    }
    
    if( event.m_pWnd == m_pkBtnSelect && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && !m_bLock )
    {
        std::vector<INDEX_GCITEMID> vecIndexItemID;
        vecIndexItemID.push_back( m_kHeroItem.m_pairIdIndex );
        
        KP2P::GetInstance()->Send_DungeonClearRewardReq( m_emGameMode, vecIndexItemID );
        m_pkBtnSelect->Lock( true );
    }
}

void KGCHeroItemBox::PostChildDraw( void )
{
	float fAddtionValueX = GC_SCREEN_DIV_WIDTH;
	float fAddtionValueY = GC_SCREEN_DIV_HEIGHT;

    // 아이템 이미지 
    float x = 2.0f * fAddtionValueX;
    float y = 0.0f * fAddtionValueY;
    float fw = 76.0f * fAddtionValueX;
    float fh = 76.0f * fAddtionValueY;

    D3DXVECTOR2 dxvItemPos = this->GetRelocatedWindowPos();;
    dxvItemPos.x += x;
    dxvItemPos.y += y;

    float fScaleX = g_pGCDeviceManager->GetWindowScaleX();
    float fScaleY = g_pGCDeviceManager->GetWindowScaleY();
    
    g_pItemMgr->Draw_ShopImage(dxvItemPos,  fw * fScaleX, fh * fScaleY, m_pkItemTexture, false, -1, 0xffffffff);

    // 수량성 아이템 숫자 표시
    if( m_kHeroItem.m_ItemInfo.m_nDuration > 0 )
    {
        D3DXVECTOR2 dxvNumPos;
        dxvNumPos.x = dxvItemPos.x + (8.0f * fAddtionValueX * fScaleX);
        dxvNumPos.y = dxvItemPos.y + (56.0f * fAddtionValueY * fScaleY);
        g_pkUIMgr->RenderNumber("very_small_num", dxvNumPos, static_cast<DWORD>(m_kHeroItem.m_ItemInfo.m_nDuration), D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f, true);
    }

    // 재료 표시
    if( m_pkPrevItemTexture && m_pkMaterialItemBackground )
    {
        float fWidth = 27.0f * fAddtionValueX;
        float fHeight = 27.0f * fAddtionValueY;

        D3DXVECTOR2 dxvMaterialItemPos = m_pkMaterialItemBackground->GetRelocatedWindowPos();
        g_pItemMgr->Draw_ShopImage( dxvMaterialItemPos, fWidth * fScaleX, fHeight * fScaleY, m_pkPrevItemTexture, false, -1, 0xffffffff );
    }

    // 이벤트 표시 
    if( m_pkEventMark->IsRenderOn() )
        m_pkEventMark->Render();

    // 미리보기 버튼
    m_pkBtnPreView->Render();

    // 아이템 정보 창 
    g_pkUIScene->m_pkItemInformationBox->Render();

}

void KGCHeroItemBox::SetItemBox(KHeroItemInfo& kHeroItem)
{
    // 아이템 정보 
    m_kHeroItem = kHeroItem;
    DWORD dwItemID = static_cast<DWORD>(kHeroItem.m_pairIdIndex.second/10);
    m_pItem = g_pItemMgr->GetItemData(dwItemID);
    if( NULL == m_pItem  ) return;

    m_pkHeroCoin->ToggleRender(false);
    m_pkHeroCoinPiece->ToggleRender(false);
    m_pkHeroVIP->ToggleRender(false);
    m_pkStaticItemPrice->SetText(L"");

    // 아이템 이름
    m_pkStaticItemName->SetFontColor( g_pItemMgr->GetItemGradeColorRGB(m_pItem->cItemGrade) );
    m_pkStaticItemName->SetForceWordWrapText( g_pItemMgr->GetItemName( m_pItem->dwGoodsID ), true );

    // 아이템 타입 + 기간
    std::wostringstream strm;
    strm << g_pItemMgr->GetItemTypeString(m_pItem) ;

    if( m_kHeroItem.m_ItemInfo.m_nPeriod != -1 )
        strm << L" ( " << g_pkStrLoader->GetReplacedString( STR_ID_DURATION_PERIOD, "i", m_kHeroItem.m_ItemInfo.m_nPeriod  ) << " )";

    m_pkStaticItemType->SetText( strm.str() );

    for(int i = 0 ; i < static_cast<int>(kHeroItem.m_vecMaterialInfo.size()) ; ++i)
    {
        if ( kHeroItem.m_vecMaterialInfo[i].m_ItemID == ITEM_VIP_BADGE * 10 )
        {
            std::wostringstream strm;
            strm << kHeroItem.m_vecMaterialInfo[i].m_nDuration;
            m_pkHeroCoin->ToggleRender(false);
            m_pkHeroCoinPiece->ToggleRender(false);
            m_pkHeroVIP->ToggleRender(true);
            m_pkStaticItemPrice->SetText( strm.str() );
            break;
        }
        else if ( kHeroItem.m_vecMaterialInfo[i].m_ItemID == ITEM_ID_HERO_COIN * 10 )
        {
            std::wostringstream strm;
            strm << kHeroItem.m_vecMaterialInfo[i].m_nDuration;
            m_pkHeroCoin->ToggleRender(true);
            m_pkHeroCoinPiece->ToggleRender(false);
            m_pkHeroVIP->ToggleRender(false);
            m_pkStaticItemPrice->SetText(strm.str());
            break;
        }
        else
        {
            std::wostringstream strm;
            strm << kHeroItem.m_vecMaterialInfo[i].m_nDuration;
            m_pkHeroCoin->ToggleRender(false);
            m_pkHeroCoinPiece->ToggleRender(true);
            m_pkHeroVIP->ToggleRender(false);
            m_pkStaticItemPrice->SetText( strm.str() );
            break;
        }
    }

    // 해당 아이템 텍스처
    m_pkItemTexture = g_pItemMgr->CreateItemTexture(m_pItem->dwGoodsID);

    // 재료 아이템 텍스처/정보 세팅 
    int iMaterialCount = 0;
    std::vector<KDropItemInfo>::iterator vit = m_kHeroItem.m_vecMaterialInfo.begin();
    for( ; vit != m_kHeroItem.m_vecMaterialInfo.end() ; ++vit )
    {
        if( vit->m_ItemID == (ITEM_ID_HERO_COIN * 10) || vit->m_ItemID == (ITEM_ID_HERO_COIN_PIECE * 10) || vit->m_ItemID == (ITEM_VIP_BADGE * 10)) // 영웅 주화, 주화 조각 제외 
            continue;
        else
        {
            if( NULL == m_pkPrevItemTexture )
            {
                m_pkPrevItemTexture = g_pItemMgr->CreateItemTexture( vit->m_ItemID / 10 );
                m_pMaterialItem = g_pItemMgr->GetItemData(vit->m_ItemID / 10);
            }
            ++iMaterialCount;
        }
    }
    if( m_pkPrevItemTexture )
    {
		if(m_pkMaterialItemBackground)
	        m_pkMaterialItemBackground->ToggleRender( true );
		if(m_pkMaterialItemPriceBack)
	        m_pkMaterialItemPriceBack->ToggleRender( true );

        std::wostringstream strm;
        strm << iMaterialCount;
        m_pkStaticPrevItemNum->SetText( strm.str() );
    }

    if( m_kHeroItem.m_ItemInfo.CheckItemFlag( KManufactureItem::IF_EVENT ) )
        m_pkEventMark->ToggleRender( true );
}

void KGCHeroItemBox::ClearItemBox(void)
{
    SAFE_RELEASE( m_pkItemTexture );
    SAFE_RELEASE( m_pkPrevItemTexture );

    m_pkEventMark->ToggleRender( false );
	if(m_pkMaterialItemBackground)
	    m_pkMaterialItemBackground->ToggleRender( false );
	if(m_pkMaterialItemPriceBack)
		m_pkMaterialItemPriceBack->ToggleRender( false );

    m_pkItemTexture = NULL;
    m_pkPrevItemTexture = NULL;

    m_pItem = NULL;
    m_pMaterialItem = NULL;
    m_bItemHovered = false;
    m_bCoinHovered = false;
    m_bMaterialItemHovered = false;

    m_pkStaticItemName->SetText( L"" );
    m_pkStaticItemType->SetText( L"" );
    m_pkStaticItemPrice->SetText( L"" );
    m_pkStaticPrevItemNum->SetText( L"" );
}

void KGCHeroItemBox::SetPreview(bool bPreview)
{  
    m_bPreview = bPreview; 

    if( !m_bPreview )
        m_pkBtnPreView->SetWndMode(D3DWM_DEFAULT); 
}