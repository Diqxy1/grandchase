#include "stdafx.h"
#include "KGCWeaponChangeBox.h"

IMPLEMENT_CLASSNAME( KGCWeaponChangeBox );
IMPLEMENT_WND_FACTORY( KGCWeaponChangeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCWeaponChangeBox, "gc_weapon_change_box" );

KGCWeaponChangeBox::KGCWeaponChangeBox( void )
: m_pkBtn_Clsoe( NULL )
, m_pkBtn_Ok( NULL )
, m_pkImg_SelectItem( NULL )
, m_pkInven( NULL )
, m_dwSelectItemUID( 0 )
{
    LINK_CONTROL( "btn_close", m_pkBtn_Clsoe );
    LINK_CONTROL( "btn_ok", m_pkBtn_Ok );
    LINK_CONTROL( "img_selectitem", m_pkImg_SelectItem );
    LINK_CONTROL( "static_explain", m_pkStatic_Explain );
    LINK_CONTROL( "itemlist_inventory", m_pkInven );
}

KGCWeaponChangeBox::~KGCWeaponChangeBox( void ){}

void KGCWeaponChangeBox::OnDestroy( void )
{}

void KGCWeaponChangeBox::OnCreateComplete( void )
{
    std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType );
    if ( mit != g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end() )
    {
        m_dwSelectItemUID = (*mit).second.second.second;
    }

    if( m_pkBtn_Clsoe )
    {
        m_pkBtn_Clsoe->InitState( true, true, this );
        m_pkBtn_Clsoe->DisconnectAllEventProcedure();
        m_pkBtn_Clsoe->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCWeaponChangeBox::OnClickClsoe );
        m_pkBtn_Clsoe->SetHotKey(DIK_ESCAPE);
    }

    if( m_pkBtn_Ok )
    {
        m_pkBtn_Ok->InitState( true, true, this );
        m_pkBtn_Ok->DisconnectAllEventProcedure();
        m_pkBtn_Ok->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCWeaponChangeBox::OnClickClsoe );
        m_pkBtn_Ok->SetHotKey(DIK_ESCAPE);
    }

    if( m_pkImg_SelectItem )
    {
        m_pkImg_SelectItem->InitState( true, true );
        m_pkImg_SelectItem->SetItemInfo( 0 );
        m_pkImg_SelectItem->ShowItem( false );
        m_pkImg_SelectItem->DisconnectAllEventProcedure();
        m_pkImg_SelectItem->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCWeaponChangeBox::OnClickSelectItem );
    }

    if( m_pkStatic_Explain )
    {
        m_pkStatic_Explain->SetMultiLine( true );
        m_pkStatic_Explain->SetText( g_pkStrLoader->GetString( STR_ID_WC_STRING6 ) );
    }

    if( m_pkInven )
    {
        m_pkInven->InitState( true, true );
        m_pkInven->SetCols( 4 );
        m_pkInven->SetRows( 3 );
        m_pkInven->SetXGap( 2.f );
        m_pkInven->SetYGap( 2.f );
        m_pkInven->SetSorting( false );
        m_pkInven->SetScrollPixel( 17.f );
        m_pkInven->AddActionListener( this );
        m_pkInven->DisconnectAllEventProcedure();
        m_pkInven->ConnectEventProcedure( KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCWeaponChangeBox::OnClickInvenItem );
    }

    RefreshWeaponItemList();
}

void KGCWeaponChangeBox::ActionPerformed( const KActionEvent& event ){}

void KGCWeaponChangeBox::FrameMoveInEnabledState( void )
{
    GCItem* kItem = NULL;
    KItem* kInventoryItem = NULL;
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vPos, vMousePos( (float)pt.x, (float)pt.y );

    if( m_pkImg_SelectItem && m_pkImg_SelectItem->GetItemID() && m_pkImg_SelectItem->CheckPosInWindowBound( vMousePos ) )
    {
        D3DXVECTOR2 vPos = m_pkImg_SelectItem->GetCurrentWindowPos();
        vPos.y += m_pkImg_SelectItem->GetHeight();
        vPos.x *= m_fWindowScaleX;    vPos.y *= m_fWindowScaleY;

        kItem = g_pItemMgr->GetItemData( GetSelectItemID() );
        g_pItemMgr->FindInventory( GetSelectItemUID(), &kInventoryItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem, -1, kInventoryItem );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
    }
    else if( m_pkInven && m_pkInven->GetCurrentHoveredItem( kItem, kInventoryItem, vPos ) && kItem != NULL )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem, -1, kInventoryItem );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
    }
    else
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCWeaponChangeBox::OnClickClsoe()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

bool KGCWeaponChangeBox::OnClickInvenItem( const KActionEvent& event )
{
    if( NULL == m_pkInven )
        return false;

    if( m_dwSelectItemUID )
        m_pkInven->AddItemList( m_dwSelectItemUID );

    m_pkInven->RemoveItemList( event.m_dwlParam );
    SetSelectItem( event.m_dwlParam );

    return true;
}

void KGCWeaponChangeBox::OnClickSelectItem()
{
    if( m_dwSelectItemUID && m_pkInven && m_pkImg_SelectItem )
    {
        m_pkInven->AddItemList( m_dwSelectItemUID );
        
        m_dwSelectItemUID = 0;
        m_pkImg_SelectItem->SetItemInfo( 0 );
        m_pkImg_SelectItem->ShowItem( false );
    }
}

void KGCWeaponChangeBox::RefreshWeaponItemList()
{
    // 인벤토리 셋팅
    if( m_pkInven ) {
        // 아이템 리스트 얻기
        std::vector< GCITEMUID > vecWeaponItemList;
        vecWeaponItemList = g_pItemMgr->GetInventoryItemUIDList( g_pItemMgr->GetCharFlag( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ), ESP_WEAPON, 0 );

        // 장착가능 여부확인
        std::vector< GCITEMUID >::iterator vecIter = vecWeaponItemList.begin();
        while( vecIter != vecWeaponItemList.end() )
        {
            // 아이템 존재 확인
            KItem* pInvenItem = g_pItemMgr->GetInventoryItem( *vecIter );
            
            if( NULL == pInvenItem ) {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            if ( pInvenItem->CheckItemType( KItem::TYPE_LOOK ) )
            {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            GCItem* pItemInfo = g_pItemMgr->GetItemData( pInvenItem->m_ItemID );
            if( NULL == pItemInfo ) {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            // 무기슬롯에 몬스터카드가 딸려들어온다~ㅠ
            if( pItemInfo->eItemKind == GIK_MONSTER_CARD ) 
            {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            // 아......금괴...
            if( pItemInfo->eItemKind == GIK_ENCHANT_ASIST_ITEM ) { 
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            char cEquipDecLvValue = 0;
            std::vector< KAttributeInfo > vecAttribute = pInvenItem->m_vecAttribute;
            std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();
            int cEquipWithBlacksmithPermit;

            if (g_pItemMgr->FindInventoryForItemID(ITEM_BLACKSMITH_PERMIT)) {
                cEquipWithBlacksmithPermit = 10;
            }
            else {
                cEquipWithBlacksmithPermit = 0;
            }

            // 무기 속성 중 레벨 DOWN속성이 있는지 확인( 무기착용레벨 확인을 위함 )
            for(; vecAttribute.end() != vitAttribute; ++vitAttribute )
            {
                if ( (*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN )
                {
                    cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
                }
            }

            // 무기착용레벨과 전직레벨제한 확인
            int iLevel = g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel;

            if( pItemInfo->iNeedChangeJobLevelForEquippingItem > g_MyD3D->m_TempPlayer.GetCurrentChar().iPromotionLevel ||
                g_pItemMgr->GetlItemEquipLevel(pItemInfo, pInvenItem, NULL, true) - (cEquipDecLvValue + cEquipWithBlacksmithPermit) > iLevel )
            {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            // 현재 선택되어있는 아이템이라면..
            if( m_dwSelectItemUID == (*vecIter) )
            {
                vecIter = vecWeaponItemList.erase( vecIter );
                continue;
            }

            ++vecIter;
        }

        // 장착중이거나 코디인 아이템 필터링
        g_pItemMgr->FilterEquipedItem( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), vecWeaponItemList );
        g_pItemMgr->FilterEquipedItem( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), vecWeaponItemList, true );

        // 여기까지 살아남은 아이템을 인벤토리에 셋팅을 하자!
        m_pkInven->ClearItemList();
        m_pkInven->RefreshItemList( vecWeaponItemList );
    }

    // 선택되어있는 아이템 셋팅
    if( m_dwSelectItemUID ) {
        SetSelectItem( m_dwSelectItemUID );
    }
}

void KGCWeaponChangeBox::SetSelectItem( GCITEMUID dwItemUID )
{
    KItem* pItem = g_pItemMgr->GetInventoryItem( dwItemUID );
    if( pItem && m_pkImg_SelectItem )
    {
        m_dwSelectItemUID = dwItemUID;
        m_pkImg_SelectItem->SetItemInfo( pItem->m_ItemID, -1, false, -1 );
        m_pkImg_SelectItem->ShowItem( true );
    }
    else
        m_dwSelectItemUID = 0;
}

GCITEMID KGCWeaponChangeBox::GetSelectItemID()
{
    if( m_pkImg_SelectItem )
        return m_pkImg_SelectItem->GetItemID();
    return 0;
}

GCITEMUID KGCWeaponChangeBox::GetSelectItemUID()
{
    return m_dwSelectItemUID;
}