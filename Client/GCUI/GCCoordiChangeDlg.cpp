#include "stdafx.h"
#include "GCUserItemBox.h"
#include "GCCoordiChangeDlg.h"

IMPLEMENT_CLASSNAME( KGCCoordiChangeDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiChangeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiChangeDlg, "gc_coordichangedlg" );

KGCCoordiChangeDlg::KGCCoordiChangeDlg( void )
{
    m_vecChangeItems.push_back( KGCItemManager::ITEM_STYLISH_COUPON );

    m_pkInven = NULL;
    LINK_CONTROL( "itemlist_inventory", m_pkInven );

    m_pkCoordi = NULL;
    LINK_CONTROL( "itemlist_coordi", m_pkCoordi );

    m_pkReadyInven = NULL;
    LINK_CONTROL( "itemlist_ready_to_inven", m_pkReadyInven );

    m_pkReadyCoordi = NULL;
    LINK_CONTROL( "itemlist_ready_to_coordi", m_pkReadyCoordi );

    m_pkClose = NULL;
    LINK_CONTROL( "btn_close", m_pkClose );

    m_pkBtnChangeToCoordi = NULL;
    LINK_CONTROL( "btn_change_to_coordi", m_pkBtnChangeToCoordi );

    m_pkBtnChangeToInven = NULL;
    LINK_CONTROL( "btn_change_to_inven", m_pkBtnChangeToInven );

    m_pkItemCount = NULL;
    LINK_CONTROL( "buy_item_count", m_pkItemCount );

    m_pkItemImg = NULL;
    LINK_CONTROL( "item_img", m_pkItemImg );

    for( int i = 0 ; i < NUM_BUY_ITEM ; ++i ) {
        m_pkBuyItemImg[i] = NULL;
        LINK_CONTROL_STM( "item_img" << i , m_pkBuyItemImg[i] );

        m_pkBtnBuy[i] = NULL;
        LINK_CONTROL_STM( "btn_buy" << i , m_pkBtnBuy[i] );
    }

    m_pkStaticInvenTitle = NULL;
    LINK_CONTROL( "static_inven_list", m_pkStaticInvenTitle );

    m_pkStaticCoordiTitle = NULL;
    LINK_CONTROL( "static_coordi_list", m_pkStaticCoordiTitle );

    m_pkStaticInvenCapacity = NULL;
    LINK_CONTROL( "static_inven_capacity", m_pkStaticInvenCapacity );
    m_pkStaticCoordiCapacity = NULL;
    LINK_CONTROL( "static_coordi_capacity", m_pkStaticCoordiCapacity );

    m_pkStaticInfo = NULL;
    LINK_CONTROL( "static_info", m_pkStaticInfo );

    m_pkCharSelect = NULL;
    LINK_CONTROL( "character_select", m_pkCharSelect );
}

KGCCoordiChangeDlg::~KGCCoordiChangeDlg( void )
{
}

void KGCCoordiChangeDlg::OnCreateComplete( void )
{
    // 여기에 들어가는 상세 설정들을 스크립트에서 할 수 있도록 하면 좋을듯..
    m_pkInven->InitState( true, true );
    m_pkInven->SetCols( NUM_INVEN_COLS );
    m_pkInven->SetRows( NUM_INVEN_ROWS );
    m_pkInven->SetXGap( 4.f );
    m_pkInven->SetYGap( 3.f );
    m_pkInven->SetSorting( false );
    m_pkInven->SetScrollPixel( 17.f );
    m_pkInven->DisconnectAllEventProcedure();

    m_pkCoordi->InitState( true, true );
    m_pkCoordi->SetCols( NUM_INVEN_COLS );
    m_pkCoordi->SetRows( NUM_INVEN_ROWS );
    m_pkCoordi->SetXGap( 4.f );
    m_pkCoordi->SetYGap( 3.f );
    m_pkCoordi->SetSorting( false );
    m_pkCoordi->SetScrollPixel( 17.f );
    m_pkCoordi->DisconnectAllEventProcedure();

    m_iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
    RefreshItemList();

    m_pkReadyInven->InitState( true, true );
    m_pkReadyInven->SetCols( NUM_READY_INVEN_COLS );
    m_pkReadyInven->SetRows( NUM_READY_INVEN_ROWS );
    m_pkReadyInven->SetXGap( 4.f );
    m_pkReadyInven->SetYGap( 3.f );
    m_pkReadyInven->ClearItemList();
    m_pkReadyInven->SetSorting( false );
    m_pkReadyInven->SetScrollPixel( 20.f );
    m_pkReadyInven->DisconnectAllEventProcedure();

    m_pkBtnChangeToCoordi->Lock( true );

    m_pkReadyCoordi->InitState( true, true );
    m_pkReadyCoordi->SetCols( NUM_READY_INVEN_COLS );
    m_pkReadyCoordi->SetRows( NUM_READY_INVEN_ROWS );
    m_pkReadyCoordi->SetXGap( 4.f );
    m_pkReadyCoordi->SetYGap( 3.f );
    m_pkReadyCoordi->SetSorting( false );
    m_pkReadyCoordi->SetScrollPixel( 20.f );
    m_pkReadyCoordi->ClearItemList();
    m_pkReadyCoordi->DisconnectAllEventProcedure();

    m_pkBtnChangeToInven->Lock( true );

    m_pkClose->InitState( true, true );
    m_pkClose->DisconnectAllEventProcedure();
    m_pkClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnChangeToInven->InitState( true, true );
    m_pkBtnChangeToCoordi->InitState( true, true );

    m_pkItemCount->InitState( true );
    m_pkItemCount->SetNumber( GetChangeItemCount() );
    m_pkItemCount->SetStringType( "blue_gradation_num" );
    //m_pkItemCount->SetWriteToRight( false );

    if( m_vecChangeItems.empty() == false ) {
        m_pkItemImg->InitState( true );
        m_pkItemImg->SetItemInfo( m_vecChangeItems[0] );
    }
    else {
        m_pkItemImg->InitState( false );
    }

    m_mapItem.clear();
    for( int i = 0 ; i < NUM_BUY_ITEM ; ++i ) {
        if( i < static_cast<int>( m_vecChangeItems.size() ) ) {
            m_pkBuyItemImg[i]->InitState( true );
            m_pkBuyItemImg[i]->SetItemInfo( m_vecChangeItems[i] );
            m_pkBtnBuy[i]->InitState( true, true );
            m_mapItem[m_pkBtnBuy[i]->GetUID()] = m_vecChangeItems[i];
        }
        else {
            m_pkBuyItemImg[i]->InitState( false );
            m_pkBtnBuy[i]->InitState( false );
        }
    }

    m_pkStaticInvenTitle->InitState( true );
    m_pkStaticInvenTitle->SetText( g_pkStrLoader->GetString( STR_ID_MESSENGER_USER_LOCATION11 ) );
    m_pkStaticInvenTitle->SetAlign( DT_CENTER );
    m_pkStaticCoordiTitle->InitState( true );
    m_pkStaticCoordiTitle->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_CLOSET ) );
    m_pkStaticCoordiTitle->SetAlign( DT_CENTER );
    m_pkStaticInvenCapacity->InitState( true );
    m_pkStaticInvenCapacity->SetAlign( DT_CENTER );
    m_pkStaticCoordiCapacity->InitState( true );
    m_pkStaticCoordiCapacity->SetAlign( DT_CENTER );
    m_pkStaticInfo->InitState( true );
    m_pkStaticInfo->SetMultiLine( true );

    m_pkCharSelect->InitState( true, true );
    m_pkCharSelect->SetPositionEnum( KGCCharacterSelect::POS_LOWER );
    //m_pkCharSelect->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHARACTER_SELECT_UP ), -80.0f );
    m_pkCharSelect->OnSelectChar( m_iCharType );

    ConnectAllChildEventProcedure();
}

void KGCCoordiChangeDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

bool KGCCoordiChangeDlg::OnClickInvenItem( const KActionEvent& event )
{
    if( GetCoordiUseSize() >= GetCoordiFullSize() ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COORDI_INVENTORY_FULL_ERROR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0,true, true );
        return true;
    }
    m_pkInven->RemoveItemList( event.m_dwlParam );
    m_pkReadyCoordi->AddItemList( event.m_dwlParam );
    m_pkBtnChangeToCoordi->Lock( false );

    RefreshCapacity();
    return true;
}

bool KGCCoordiChangeDlg::OnClickCoordiItem( const KActionEvent& event )
{
    //
    KItem* kItem = g_pItemMgr->GetInventoryItem( event.m_dwlParam );
    if ( !kItem ) 
        return true;

    GCItem* pItem = g_pItemMgr->GetItemData( kItem->m_ItemID );
    if ( !pItem || pItem->eItemKind == GIK_COORDI_ITEM ) 
        return true;

    m_pkCoordi->RemoveItemList( event.m_dwlParam );
    m_pkReadyInven->AddItemList( event.m_dwlParam );
    m_pkBtnChangeToInven->Lock( false );

    RefreshCapacity();
    return true;
}

bool KGCCoordiChangeDlg::OnClickReadyInvenItem( const KActionEvent& event )
{
    m_pkCoordi->AddItemList( event.m_dwlParam );
    m_pkReadyInven->RemoveItemList( event.m_dwlParam );
    m_pkBtnChangeToInven->Lock( m_pkReadyInven->GetListSize() == 0 );

    RefreshCapacity();
    return true;
}

bool KGCCoordiChangeDlg::OnClickReadyCoordiItem( const KActionEvent& event )
{
    m_pkInven->AddItemList( event.m_dwlParam );
    m_pkReadyCoordi->RemoveItemList( event.m_dwlParam );
    m_pkBtnChangeToCoordi->Lock( m_pkReadyCoordi->GetListSize() == 0 );

    RefreshCapacity();
    return true;
}

void KGCCoordiChangeDlg::OnClickChangeToCoordi()
{
    std::vector<GCITEMUID> vecItemList = m_pkReadyCoordi->GetItemUIDList();
    std::set<GCITEMUID> setChangeList;
    setChangeList.insert( vecItemList.begin(), vecItemList.end() );

    KP2P::GetInstance()->Send_NormalItemToLookItem( setChangeList );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_RECEIVE_GAME_SERVER_PAPCKET, this, &KGCCoordiChangeDlg::OnReceiveChangeToCoordi );
}

void KGCCoordiChangeDlg::OnClickChangeToInven()
{
    // 현재 인벤토리에 남은 공간을 구하고 
    int iRemainInven = GetInvenFullSize() - GetInvenUseSize();
    if( iRemainInven <= 0 ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    // 변환할 개수와 남은 공간도 비교 
    std::vector<GCITEMUID> vecItemList = m_pkReadyInven->GetItemUIDList();
    if( static_cast< int >( vecItemList.size() ) > iRemainInven ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    std::set<GCITEMUID> setChangeList;
    setChangeList.insert( vecItemList.begin(), vecItemList.end() );

    KP2P::GetInstance()->Send_LookItemToNormalItem( setChangeList );
    //DiconnectAllChildEventProcedure();
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_RECEIVE_GAME_SERVER_PAPCKET, this, &KGCCoordiChangeDlg::OnReceiveChangeToInven );
}

void KGCCoordiChangeDlg::DiconnectAllChildEventProcedure()
{
    for( UINT i = 0 ; i < KD3DWnd::GetNumChild() ; ++i ) {
        KD3DWnd::GetChild( i )->DisconnectAllEventProcedure();
    }

    m_pkItemCount->SetNumber( GetChangeItemCount() );
}

void KGCCoordiChangeDlg::ConnectAllChildEventProcedure()
{
    DiconnectAllChildEventProcedure();
    m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiChangeDlg::OnClickClose );
    m_pkReadyCoordi->ConnectEventProcedure( KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCCoordiChangeDlg::OnClickReadyCoordiItem );
    m_pkReadyInven->ConnectEventProcedure( KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCCoordiChangeDlg::OnClickReadyInvenItem );
    m_pkCoordi->ConnectEventProcedure( KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCCoordiChangeDlg::OnClickCoordiItem );
    m_pkInven->ConnectEventProcedure( KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCCoordiChangeDlg::OnClickInvenItem );

    m_pkBtnChangeToCoordi->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiChangeDlg::OnClickChangeToCoordi );
    m_pkBtnChangeToInven->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiChangeDlg::OnClickChangeToInven );

    for( int i = 0 ; i < NUM_BUY_ITEM ; ++i ) {
        m_pkBtnBuy[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCoordiChangeDlg::OnClickBuyItem );
    }
    m_pkCharSelect->ConnectEventProcedure( KGCUIScene::D3DWE_CHARACTER_SELECT, this, &KGCCoordiChangeDlg::OnCharacterChange );
}

bool KGCCoordiChangeDlg::OnReceiveChangeToCoordi( const KActionEvent& event )
{
    if( event.m_dwlParam != KUserEvent::EVENT_CHANGE_NORMAL_TO_LOOK_ACK )
        return true;

    m_pkReadyCoordi->ClearItemList();
    m_pkBtnChangeToCoordi->Lock( true );
    RefreshItemList();
    m_pkItemCount->SetNumber( GetChangeItemCount() );

    ConnectAllChildEventProcedure();
    RefreshCapacity();
    return true;
}

bool KGCCoordiChangeDlg::OnReceiveChangeToInven( const KActionEvent& event )
{
    if( event.m_dwlParam != KUserEvent::EVENT_CHANGE_LOOK_TO_NORMAL_ACK )
        return true;

    m_pkReadyInven->ClearItemList();
    m_pkBtnChangeToInven->Lock( true );
    RefreshItemList();
    m_pkItemCount->SetNumber( GetChangeItemCount() );

    ConnectAllChildEventProcedure();
    RefreshCapacity();
    return true;
}

void KGCCoordiChangeDlg::OnClickBuyItem( const KActionEvent& event )
{
    std::map<KD3DWnd::uid_type,GCITEMID>::iterator mit = m_mapItem.find( event.m_pWnd->GetUID() );
    if( mit == m_mapItem.end() )
        return;

    GCItem* pItemInfo = g_pItemMgr->GetItemData( mit->second );
    if( pItemInfo == NULL )
        return;

    if (pItemInfo->eMoneyType == EMT_CASH)
    {
        g_pItemMgr->BuyCashItemS6(mit->second);
    }
    else
    {
        g_MyD3D->m_kItemMgr.BuyGPItem(pItemInfo, mit->second, pItemInfo->iItemTypeValue);
    }
}

int KGCCoordiChangeDlg::GetChangeItemCount()
{
    int iCount = 0;
    std::vector<GCITEMID>::iterator vit;
    for( vit = m_vecChangeItems.begin() ; vit != m_vecChangeItems.end() ; ++vit ) {
        iCount += g_pItemMgr->GetInventoryItemDuration( *vit );
    }
    return iCount;
}

void KGCCoordiChangeDlg::RefreshItemList()
{
    DWORD dwItemPosition_ = 0;
    dwItemPosition_ |= ESP_HELMET;
    dwItemPosition_ |= ESP_JACKET;
    dwItemPosition_ |= ESP_PANTS;
    dwItemPosition_ |= ESP_GLOVES;
    dwItemPosition_ |= ESP_SHOES;
    dwItemPosition_ |= ESP_MANTLE;
    dwItemPosition_ |= ESP_A_UPPER_HELMET;
    dwItemPosition_ |= ESP_A_DOWN_HELMET;
    dwItemPosition_ |= ESP_A_JACKET;
    dwItemPosition_ |= ESP_A_PANTS;
    dwItemPosition_ |= ESP_A_ARM;
    dwItemPosition_ |= ESP_A_NECKLET;
    dwItemPosition_ |= ESP_A_SHOES;
    dwItemPosition_ |= ESP_WEAPON;

    std::vector<GCITEMUID> vecInvenList = g_pItemMgr->GetInventoryItemUIDAllList( dwItemPosition_, 0 ); //발찌는 안됨.

    g_pItemMgr->FilterBadCoordiItemList( vecInvenList );
    g_pItemMgr->FilterEquipedItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), vecInvenList );
    g_pItemMgr->FilterRemoveCoordiItem( vecInvenList);
    g_pItemMgr->FilterRemoveOtherChar(vecInvenList, m_iCharType);
    m_pkInven->RefreshItemList( vecInvenList );

    std::vector<GCITEMUID> vecCoordiList = g_pItemMgr->GetInventoryCoordiItemUIDList();
    g_pItemMgr->FilterEquipedItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), vecCoordiList, true );
    g_pItemMgr->FilterRemoveOtherChar(vecCoordiList, m_iCharType);
    m_pkCoordi->RefreshItemList( vecCoordiList );

    RefreshCapacity();
}

void KGCCoordiChangeDlg::OnCharacterChange( const KActionEvent& event )
{
    m_iCharType =static_cast<int>( event.m_dwlParam );
    RefreshItemList();
    m_pkReadyCoordi->ClearItemList();
}

void KGCCoordiChangeDlg::OnDestroy( void )
{
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateInventoryCapacity(g_pkUIScene->m_pkMyInfoScene->m_EquipCategory == UEC_LOOK);
}

void KGCCoordiChangeDlg::RefreshCapacity()
{
    // 인벤토리
    std::wstringstream stm;
    stm << GetInvenUseSize() << L" / " << GetInvenFullSize();
    m_pkStaticInvenCapacity->SetText( stm.str() );

    // 코디
    stm.str(L"");
    stm << GetCoordiUseSize() << L" / " << GetCoordiFullSize();
    m_pkStaticCoordiCapacity->SetText( stm.str() );

    m_pkStaticInfo->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_COORDI_CHANGE0, "i", m_pkReadyCoordi->GetListSize() ) );
}

void KGCCoordiChangeDlg::FrameMoveInEnabledState()
{
    GCItem* kItem = NULL;
    KItem* kInventoryItem = NULL;
    D3DXVECTOR2 vecPos( 0.0f, 0.0f );

    m_pkItemCount->SetNumber( GetChangeItemCount() );

    if( m_pkInven->GetCurrentHoveredItem( kItem, kInventoryItem, vecPos ) == false &&
        m_pkCoordi->GetCurrentHoveredItem( kItem, kInventoryItem, vecPos ) == false &&
        m_pkReadyInven->GetCurrentHoveredItem( kItem, kInventoryItem, vecPos ) == false &&
        m_pkReadyCoordi->GetCurrentHoveredItem( kItem, kInventoryItem, vecPos ) == false ) 
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return;
    }

    if( kInventoryItem == NULL || kItem == NULL ) 
    {
        //ASSERT(!"누가 없는 아이템을 보여 줄려고 했다!!" );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return;
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem, -1, kInventoryItem );

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
}
