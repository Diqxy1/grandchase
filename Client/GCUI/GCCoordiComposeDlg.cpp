#include "stdafx.h"
#include "GCCoordiComposeDlg.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCCoordiComposeDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiComposeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiComposeDlg, "gc_coordicomposedlg" );

KGCCoordiComposeDlg::KGCCoordiComposeDlg( void )
: m_nState( EM_STATE_NONE )
, m_DesignItemUID( 0 )
, m_AbilityItemUID( 0 )
, m_iScrollpos( 0 )
{
    m_pkStaticItemNum = NULL;
    m_pkBtnOk = NULL;
    m_pkBtnClose = NULL;
    m_pkBtnAdd = NULL;
    m_pkBtnDesignSlot = NULL;
    m_pkBtnAbilitySlot = NULL;
    m_pkBtnDesignSlotItem = NULL;
    m_pkBtnAbilitySlotItem = NULL;
    m_pkBtnResultSlot = NULL;

    m_pkDesignItemImg = NULL;
    m_pkAbilityItemImg = NULL;
    m_pkResultItemImg = NULL;

    m_pkComposeImg = NULL;
    m_pkDescImg = NULL;

    m_pkScrollbar = NULL;

    LINK_CONTROL( "static_itemnum", m_pkStaticItemNum );
    LINK_CONTROL( "btn_ok", m_pkBtnOk );
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_add", m_pkBtnAdd );
    LINK_CONTROL( "design_slot_btn", m_pkBtnDesignSlot );
    LINK_CONTROL( "ability_slot_btn", m_pkBtnAbilitySlot );
    LINK_CONTROL( "design_slotitem_btn", m_pkBtnDesignSlotItem );
    LINK_CONTROL( "ability_slotitem_btn", m_pkBtnAbilitySlotItem );
    LINK_CONTROL( "result_slot_btn", m_pkBtnResultSlot );
    LINK_CONTROL( "design_slotitem_img", m_pkDesignItemImg );
    LINK_CONTROL( "ability_slotitem_img", m_pkAbilityItemImg );
    LINK_CONTROL( "result_slotitem_img", m_pkResultItemImg );
    LINK_CONTROL( "compose_img", m_pkComposeImg );
    LINK_CONTROL( "img_desc", m_pkDescImg );
    LINK_CONTROL( "scroll", m_pkScrollbar );
}

KGCCoordiComposeDlg::~KGCCoordiComposeDlg( void )
{
}

void KGCCoordiComposeDlg::OnCreate( void )
{
    m_pkBtnOk->InitState( true, true, this );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBtnAdd->InitState( true, true, this );   

    m_pkBtnDesignSlot->InitState( true, true, this );
    m_pkBtnAbilitySlot->InitState( true, true, this );

    m_pkBtnDesignSlotItem->InitState( true );
    m_pkBtnAbilitySlotItem->InitState( true );
    m_pkBtnResultSlot->InitState( true, true, this );

    m_pkDesignItemImg->InitState( true, true, this );
    m_pkAbilityItemImg->InitState( true, true, this );
    m_pkResultItemImg->InitState( true );
    
    m_pkStaticItemNum->InitState( true, true, this );
    m_pkStaticItemNum->SetAlign( DT_RIGHT );

    m_pkComposeImg->InitState( true, false, this );
    m_pkDescImg->InitState( false );

    m_pkScrollbar->InitState(true,true,this);
    m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );

    CreateInvenSlot();
    InitUIState();

}

void KGCCoordiComposeDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnOk, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnAdd, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddItem );

    GCWND_MSG_MAP( m_pkBtnDesignSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnDesignSlot );
    GCWND_MSG_MAP( m_pkBtnAbilitySlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnAbilitySlot );

    GCWND_MSG_MAP( m_pkDesignItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnDesignItem );
    GCWND_MSG_MAP( m_pkAbilityItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnAbilityItem );

    GCWND_MSG_MAP( m_pkBtnOk, KD3DWnd::D3DWE_CURSOR_ENTER, OnEnterCursor );
    GCWND_MSG_MAP( m_pkBtnOk, KD3DWnd::D3DWE_CURSOR_LEAVE, OnLeaveCursor );

    GCWND_MAP( m_pkScrollbar, OnScrollPos );


    if( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        for ( int i = 0; i < (int) m_vecInvenItemImgList.size(); ++i ) 
        {
            if ( event.m_pWnd == m_vecInvenItemImgList[i] )
            {
                OnInvenItemList(i);
                return;
            }
        }
    }        
}

void KGCCoordiComposeDlg::FrameMoveInEnabledState( void )
{
    CheckItemInformation();
}

void KGCCoordiComposeDlg::InitUIState()
{
    int nCount = g_pItemMgr->GetInventoryItemDuration( KGCItemManager::ITEM_COORDI_COMPOSE_COUPON );
    m_pkStaticItemNum->SetText( nCount );
    
    m_pkBtnOk->Lock( true );
    m_pkComposeImg->Lock( false );
    m_pkBtnDesignSlot->ToggleRender( true );
    m_pkBtnAbilitySlot->ToggleRender( true );
    m_pkBtnDesignSlotItem->ToggleRender( false );
    m_pkBtnAbilitySlotItem->ToggleRender( false );
    m_pkBtnDesignSlot->Lock( false );
    m_pkBtnAbilitySlot->Lock( false );
    m_pkBtnResultSlot->Lock( true );  

    m_pkDesignItemImg->ToggleRender( false );
    m_pkAbilityItemImg->ToggleRender( false );
    m_pkResultItemImg->ToggleRender( false );


    for ( int i = 0; i< (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }

    m_vecItemUIDList.clear();


    m_iScrollpos = 0;
    m_nState = EM_STATE_NONE;
    m_DesignItemUID = 0;
    m_AbilityItemUID = 0;
}

void KGCCoordiComposeDlg::OnOK()
{
    int nResult = g_pItemMgr->DoCoordiCompose( m_AbilityItemUID, m_DesignItemUID );
    if ( nResult != KGCItemManager::EMCOORDI_COMPOSE_OK )
    {
        std::wstring strError;
        switch ( nResult )
        {
        case KGCItemManager::EMCOORDI_COMPOSE_ERR0:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR0);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR1:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR1);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR2:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR2);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR3:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR3);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR4:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR4);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR5:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR5);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR6:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR6);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR7:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR7);
            break;
        case KGCItemManager::EMCOORDI_COMPOSE_ERR8:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_COMPOSE_ERROR8);
            break;
        }

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strError , L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true );

        return;  
    }

    m_pkComposeImg->Lock( true );
}

void KGCCoordiComposeDlg::OnEnterCursor()
{
    m_pkDescImg->ToggleRender( true );
}

void KGCCoordiComposeDlg::OnLeaveCursor()
{
    m_pkDescImg->ToggleRender( false );
}

void KGCCoordiComposeDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCCoordiComposeDlg::OnAddItem()
{
    GCITEMID itemID = KGCItemManager::ITEM_COORDI_COMPOSE_COUPON;

    GCItem* pItem = g_pItemMgr->GetItemData( itemID );
    if ( pItem == NULL ) return;

    if( pItem->eMoneyType == EMT_CASH )
    {
        g_pItemMgr->BuyCashItemS6( itemID );
    }
    else
    {
        g_MyD3D->m_kItemMgr.BuyGPItem( pItem, itemID, pItem->iItemTypeValue );
    }
}

void KGCCoordiComposeDlg::OnDesignSlot()
{
    m_nState = EM_STATE_DESIGN_SLOT;

    m_pkBtnDesignSlot->ToggleRender( false );
    m_pkBtnDesignSlotItem->ToggleRender( true );

    if ( m_AbilityItemUID == 0 )
    {
        m_pkBtnAbilitySlot->ToggleRender( true );
        m_pkBtnAbilitySlotItem->ToggleRender( false );
    }

    // UpdateItemList
    UpdateDesignSlotCoordiList();
}

void KGCCoordiComposeDlg::OnAbilitySlot()
{
    m_nState = EM_STATE_ABILITY_SLOT;

    m_pkBtnAbilitySlot->ToggleRender( false );
    m_pkBtnAbilitySlotItem->ToggleRender( true );

    if ( m_DesignItemUID == 0 )
    {
        m_pkBtnDesignSlot->ToggleRender( true );
        m_pkBtnDesignSlotItem->ToggleRender( false );
    }

    // UpdateItemList
    UpdateAbilitySlotCoordiList();
}

void KGCCoordiComposeDlg::OnDesignItem()
{
    m_nState = EM_STATE_NONE;
    m_DesignItemUID = 0;

    m_pkDesignItemImg->ToggleRender(false);
    m_pkDesignItemImg->ShowItem(false);    
    
    m_pkResultItemImg->ToggleRender(false);
    m_pkResultItemImg->ShowItem(false);

    m_pkBtnResultSlot->Lock( true );
    m_pkBtnOk->Lock( true );

    m_pkBtnDesignSlot->ToggleRender( true );
    m_pkBtnDesignSlot->Lock( false );
    m_pkBtnDesignSlotItem->ToggleRender( false );

    if ( m_AbilityItemUID == 0 )
    {
        m_pkBtnAbilitySlot->ToggleRender( true );
        m_pkBtnAbilitySlotItem->ToggleRender( false );
    }

    m_vecItemUIDList.clear();

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }
}

void KGCCoordiComposeDlg::OnAbilityItem()
{
    m_nState = EM_STATE_NONE;
    m_AbilityItemUID = 0;

    m_pkAbilityItemImg->ToggleRender(false);
    m_pkAbilityItemImg->ShowItem(false);  
    
    m_pkResultItemImg->ToggleRender(false);
    m_pkResultItemImg->ShowItem(false);  

    m_pkBtnResultSlot->Lock( true );
    m_pkBtnOk->Lock( true );

    m_pkBtnAbilitySlot->ToggleRender( true );
    m_pkBtnAbilitySlot->Lock( false );
    m_pkBtnAbilitySlotItem->ToggleRender( false );

    if ( m_DesignItemUID == 0 )
    {
        m_pkBtnDesignSlot->ToggleRender( true );
        m_pkBtnDesignSlotItem->ToggleRender( false );
    }

    m_vecItemUIDList.clear();

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }
}

void KGCCoordiComposeDlg::OnInvenItemList( int index, bool bVerifBypass )
{
    int nIndex = ( m_iScrollpos * EM_INVEN_COLUMN ) + index;
    if ( nIndex < 0 || (int)m_vecItemUIDList.size() <= nIndex )
        return;

    GCITEMUID itemUID = m_vecItemUIDList[nIndex];
    KItem* pkItem = g_pItemMgr->GetInventoryItem( itemUID );
    if ( !pkItem )
        return;   

    if ( pkItem->m_DesignCoordiID > 0 && !bVerifBypass )
    {
        g_pkUIScene->MessageBoxW(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_COMPOSE_CONFIRM_CONFIRM), L"", KGCUIScene::GC_MBOX_USE_COMPOSE_CONFIRM, index, 0i64, false, true);
    }
    else
    {

        if (m_nState == EM_STATE_DESIGN_SLOT)
        {
            m_DesignItemUID = itemUID;

            m_pkDesignItemImg->ToggleRender(true);
            m_pkDesignItemImg->ShowItem(true);
            m_pkDesignItemImg->SetItemInfo(pkItem->m_ItemID, -1, false, -1, pkItem->m_ItemUID, pkItem->m_nCount, pkItem->m_DesignCoordiID);

            m_pkBtnDesignSlot->Lock(true);
            m_pkBtnDesignSlot->ToggleRender(true);
            m_pkBtnDesignSlotItem->ToggleRender(false);
        }
        else
        {
            m_AbilityItemUID = itemUID;

            m_pkAbilityItemImg->ToggleRender(true);
            m_pkAbilityItemImg->ShowItem(true);
            m_pkAbilityItemImg->SetItemInfo(pkItem->m_ItemID, -1, false, -1, pkItem->m_ItemUID, pkItem->m_nCount, pkItem->m_DesignCoordiID);

            m_pkBtnAbilitySlot->Lock(true);
            m_pkBtnAbilitySlot->ToggleRender(true);
            m_pkBtnAbilitySlotItem->ToggleRender(false);
        }

        if (m_DesignItemUID != 0 && m_AbilityItemUID != 0)
        {
            KItem* pkItemAbility = g_pItemMgr->GetInventoryItem(m_AbilityItemUID);
            if (!pkItemAbility)
                return;

            KItem* pkItemDesign = g_pItemMgr->GetInventoryItem(m_DesignItemUID);
            if (!pkItemDesign)
                return;

            GCITEMID _DesignItemID = pkItemDesign->m_ItemID;
            if (pkItemDesign->m_DesignCoordiID != 0)
            {
                _DesignItemID = pkItemDesign->m_DesignCoordiID;
            }

            // CSlot 업데이트
            m_pkResultItemImg->ToggleRender(true);
            m_pkResultItemImg->ShowItem(true);
            m_pkResultItemImg->SetItemInfo(pkItemAbility->m_ItemID, -1, false, -1, pkItemAbility->m_ItemUID, pkItemAbility->m_nCount, _DesignItemID);

            m_pkBtnResultSlot->Lock(false);
            m_pkBtnOk->Lock(false);
        }
    }
}

void KGCCoordiComposeDlg::OnScrollPos( void )
{
    int iPos = m_pkScrollbar->GetScrollPos();

    if ( m_iScrollpos != iPos )
    {
        m_iScrollpos = iPos;
        g_KDSound.Play( "73" );
        
        UpdateInvenItemList();
    }
}

void KGCCoordiComposeDlg::UpdateItemCount()
{
    int nCount = g_pItemMgr->GetInventoryItemDuration( KGCItemManager::ITEM_COORDI_COMPOSE_COUPON );
    m_pkStaticItemNum->SetText( nCount );
}

void KGCCoordiComposeDlg::UpdateDesignSlotCoordiList()
{
    m_vecItemUIDList = g_pItemMgr->GetEnableCoordiComposeList( true, m_AbilityItemUID, m_DesignItemUID );

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;


    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );
    
    
    
    UpdateInvenItemList();
}

void KGCCoordiComposeDlg::UpdateAbilitySlotCoordiList()
{
    m_vecItemUIDList = g_pItemMgr->GetEnableCoordiComposeList( false, m_AbilityItemUID, m_DesignItemUID );
    
    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;

    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );
    
    UpdateInvenItemList();
}

void KGCCoordiComposeDlg::UpdateInvenItemList()
{
    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }

    int nCount = 0;

    for ( int i = 0; i < (int)m_vecItemUIDList.size(); ++i )
    {
        if ( i < ( m_iScrollpos * EM_INVEN_COLUMN ) )
            continue;

        if ( nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW )
            return;

        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_vecItemUIDList[i] );        
        if ( !pkItem )
            return;

        m_vecInvenItemImgList[nCount]->SetItem( pkItem );

        nCount++;
    }
}

void KGCCoordiComposeDlg::CreateInvenSlot()
{
    float fBasePosX = 540.0f;
    float fBasePosY = 61.0f;

    if(m_vecInvenItemImgList.empty()){     
        
        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_coordi_compose_dlg.stg", "",
            "coordi_compose_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );

        for( int i = 0; i < (int)vecTemp.size(); ++i )
        {
            float fPosX = fBasePosX + ( ( i % EM_INVEN_COLUMN ) * EM_INVEN_WIDH_SIZE );
            float fPosY = fBasePosY + ( ( i / EM_INVEN_COLUMN ) * EM_INVEN_HEIGHT_SIZE );

            vecTemp[i]->SetWindowPosDirect(D3DXVECTOR2(fPosX,fPosY));
            vecTemp[i]->ToggleRender(true);
            vecTemp[i]->InitState(true,true,this);

            m_vecInvenItemImgList.push_back( (KGCCommonItemWnd*) vecTemp[i] );
        }  
    }
}

void KGCCoordiComposeDlg::CheckItemInformation()
{
    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->SelectedItem( false );
    }

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        if ( m_vecInvenItemImgList[i]->GetItemID() != 0 && m_vecInvenItemImgList[i]->CheckMousePosInWindowBound() )
        {
            int nIndex = ( m_iScrollpos * EM_INVEN_COLUMN ) + i;
            if ( nIndex < 0 || (int)m_vecItemUIDList.size() <= nIndex )
                return;

            GCITEMUID itemUID = m_vecItemUIDList[nIndex];
            KItem* pkItem = g_pItemMgr->GetInventoryItem( itemUID );
            if ( !pkItem )
                return;

            DrawItemInfo( pkItem, m_vecInvenItemImgList[i]->GetCurrentWindowPos() );

            
            D3DXVECTOR2 vFocusingPos = m_vecInvenItemImgList[i]->GetFixedWindowLocalPos();
            vFocusingPos.x -= 2.0f;
            vFocusingPos.y -= 3.0f;

            m_vecInvenItemImgList[i]->SelectedItem( true );

            return;
        }
    }

    if ( m_pkDesignItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_DesignItemUID );
        if ( !pkItem )
            return;

        DrawItemInfo( pkItem, m_pkDesignItemImg->GetCurrentWindowPos() );
        return;
    }

    if ( m_pkAbilityItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_AbilityItemUID );
        if ( !pkItem )
            return;

        DrawItemInfo( pkItem, m_pkAbilityItemImg->GetCurrentWindowPos() );
        return;
    }

    if ( m_pkResultItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_AbilityItemUID );
        if ( !pkItem )
            return;

        KItem* pkItem2 = g_pItemMgr->GetInventoryItem( m_DesignItemUID );
        if ( !pkItem2 )
            return;        

        KItem kResultItem = *pkItem;
        
        GCITEMID _itemDesignID = pkItem2->m_ItemID;
        if ( pkItem2->m_DesignCoordiID != 0 )
        {
            _itemDesignID = pkItem2->m_DesignCoordiID;
        }
        
        kResultItem.m_DesignCoordiID = _itemDesignID;

        DrawItemInfo( &kResultItem, m_pkResultItemImg->GetCurrentWindowPos() );
        return;
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCCoordiComposeDlg::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
{
    if( pItem != NULL && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
    {
        vPos.x += EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH;
        vPos.y += (EM_INVEN_HEIGHT_SIZE / 2) * GC_SCREEN_DIV_WIDTH;
        vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
        
        if( vPos.x + g_pkUIScene->m_pkItemInformationBox->GetWidth() > static_cast<float>(GC_SCREEN_WIDTH) )
        {
            vPos.x -= ( static_cast<float>( EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH ) * m_fWindowScaleX );
            vPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();      
        }
        
        GCItem* pGCItem = g_pItemMgr->GetItemData(pItem->m_ItemID);

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1, pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}