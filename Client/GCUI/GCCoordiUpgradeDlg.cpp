#include "stdafx.h"
#include "GCCoordiUpgradeDlg.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCCoordiUpgradeDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiUpgradeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiUpgradeDlg, "gc_coordiupgradedlg" );

KGCCoordiUpgradeDlg::KGCCoordiUpgradeDlg( void )
: m_ItemUID( 0 )
, m_iScrollpos( 0 )
{
    m_pkStaticItemNum = NULL;
    m_pkStaticCostNum = NULL;

    m_pkBtnOk = NULL;
    m_pkBtnClose = NULL;
    m_pkBtnAdd = NULL;

    m_pkCoordiItem = NULL;
    m_pkResultItem = NULL;
    m_nState = EM_COMMON_STATE;

    LINK_CONTROL( "static_itemnum", m_pkStaticItemNum );
    LINK_CONTROL( "static_costnum", m_pkStaticCostNum );
    
    LINK_CONTROL( "btn_ok", m_pkBtnOk );
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_add", m_pkBtnAdd );

    LINK_CONTROL( "coordi_item", m_pkCoordiItem );
    LINK_CONTROL( "result_item", m_pkResultItem );


    LINK_CONTROL( "scroll", m_pkScrollbar );
}

KGCCoordiUpgradeDlg::~KGCCoordiUpgradeDlg( void )
{
}

void KGCCoordiUpgradeDlg::OnCreate( void )
{
    m_pkBtnOk->InitState( true, true, this );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBtnAdd->InitState( true, true, this );   

    m_pkCoordiItem->InitState( true, true, this );
    m_pkResultItem->InitState( true, true, this );

    m_pkStaticItemNum->InitState( true, true, this );
    m_pkStaticItemNum->SetAlign( DT_RIGHT );

    m_pkStaticCostNum->InitState( true, true, this );
    m_pkStaticCostNum->SetAlign( DT_RIGHT );

    m_pkScrollbar->InitState(true,true,this);
    m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );

    CreateInvenSlot();
    InitUIState();    
}

void KGCCoordiUpgradeDlg::OnCreateComplete( void )
{
    UpdateCoordiList();
}

void KGCCoordiUpgradeDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnOk, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnAdd, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddItem );

    GCWND_MSG_MAP( m_pkCoordiItem, KD3DWnd::D3DWE_BUTTON_CLICK, OnCoordiSlot );

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

void KGCCoordiUpgradeDlg::FrameMoveInEnabledState( void )
{
    CheckItemInformation();
    for ( int i = 0; i< (int)m_vecInvenItemImgList.size(); ++i )
    {
//        m_vecInvenItemImgList[i]->m_pItemImgWnd->ToggleRender( true );
        m_vecInvenItemImgList[i]->m_pItemImgWnd->ShowItem( true );
    }
}

void KGCCoordiUpgradeDlg::InitUIState()
{
    m_nState = EM_COMMON_STATE;

    int nCount = g_pItemMgr->GetInventoryItemDuration( g_pItemMgr->m_sCoordiUpgradeInfo.m_GradeUpgradeItemID / 10 );
    m_pkStaticItemNum->SetText( nCount );
    m_pkStaticCostNum->SetText( L"" );

    m_pkBtnOk->Lock( true );

    m_pkCoordiItem->ResetItem();
    m_pkResultItem->ResetItem();


    for ( int i = 0; i< (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }

    m_vecItemUIDList.clear();


    m_iScrollpos = 0;
    m_ItemUID = 0;
}

void KGCCoordiUpgradeDlg::InitResultState()
{
    int nCount = g_pItemMgr->GetInventoryItemDuration( g_pItemMgr->m_sCoordiUpgradeInfo.m_GradeUpgradeItemID / 10 );
    m_pkStaticItemNum->SetText( nCount );
    m_pkStaticCostNum->SetText( L"" );

    m_pkBtnOk->Lock( true );

    m_pkCoordiItem->ResetItem();

    for ( int i = 0; i< (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }

    m_vecItemUIDList.clear();

    m_iScrollpos = 0;
//    m_ItemUID = 0;

    m_nState = EM_RESULT_STATE;

    UpdateCoordiList();    
}

void KGCCoordiUpgradeDlg::OnOK()
{
    int nResult = g_pItemMgr->DoCoordiUpgrade( m_ItemUID );
    if ( nResult != KGCItemManager::EMCOORDI_UPGRADE_OK )
    {
        std::wstring strError;
        switch ( nResult )
        {
        case KGCItemManager::EMCOORDI_UPGRADE_ERR0:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR0);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR1:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR1);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR2:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR2);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR3:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR3);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR4:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR4);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR5:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR5);
            break;
        case KGCItemManager::EMCOORDI_UPGRADE_ERR6:
            strError = g_pkStrLoader->GetString(STR_ID_COORDI_UPGRADE_ERROR6);
            break;
        }

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strError , L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true );

        return;  
    }
}

void KGCCoordiUpgradeDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCCoordiUpgradeDlg::OnAddItem()
{
    GCITEMID itemID = g_pItemMgr->m_sCoordiUpgradeInfo.m_GradeUpgradeItemID / 10;

    GCItem* pItem = g_pItemMgr->GetItemData( itemID );
    if ( pItem == NULL ) return;

    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( itemID );
#else
#ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( itemID, false, false, true );
#else
        g_MyD3D->m_kItemMgr.BuyItem( itemID, pItem->iItemTypeValue );
#endif
#endif
    }
    else
    {
        g_MyD3D->m_kItemMgr.BuyGPItem( pItem, itemID, pItem->iItemTypeValue );
    }
}

void KGCCoordiUpgradeDlg::OnCoordiSlot()
{
}

void KGCCoordiUpgradeDlg::OnInvenItemList( int index )
{
    int nIndex = ( m_iScrollpos * EM_INVEN_COLUMN ) + index;
    if ( nIndex < 0 || (int)m_vecItemUIDList.size() <= nIndex )
        return;

    GCITEMUID itemUID = m_vecItemUIDList[nIndex];
    KItem* pkItem = g_pItemMgr->GetInventoryItem( itemUID );
    if ( !pkItem )
        return;   
      
    m_ItemUID = itemUID;
    m_pkCoordiItem->SetItem( pkItem );

    int nCost = 0;
    std::map<char,int>::iterator iter = g_pItemMgr->m_sCoordiUpgradeInfo.m_mapGradeUseCount.find( pkItem->m_cGradeID + 1 );
    if ( iter != g_pItemMgr->m_sCoordiUpgradeInfo.m_mapGradeUseCount.end() )
    {
        nCost = iter->second;
    }

    m_pkStaticCostNum->SetText( nCost );


    if ( m_ItemUID != 0 )
    {
        // result item 
        KItem kItem = GetResultItem();

        m_pkResultItem->SetItem( &kItem );
        m_pkBtnOk->Lock( false );
    }

    m_nState = EM_COMMON_STATE;
}

KItem KGCCoordiUpgradeDlg::GetResultItem()
{
    KItem kItem;
    KItem* pkItem = g_pItemMgr->GetInventoryItem( m_ItemUID );
    if ( !pkItem )
        return kItem;   

    kItem = *pkItem;
    kItem.m_ItemUID = 0;
    kItem.m_cGradeID += 1;
    kItem.m_vecAttribute.clear();

    int iAttributeTableID = g_pItemMgr->GetAttributeTableID( kItem.m_ItemID );

    if( g_pItemMgr->m_kAttributeDecider.GetTableType( iAttributeTableID, kItem.m_cGradeID ) == KGCAttributeDecider::Table::ATT_FIXED )
    {
        std::list<std::pair<int,float>> listAttributes;
        g_pItemMgr->m_kAttributeDecider.GetAttributeFullList( iAttributeTableID, kItem.m_cGradeID, listAttributes );

        std::list<std::pair<int,float>>::iterator lit;
        int iSlot = 0;
        for( lit = listAttributes.begin() ; lit != listAttributes.end() ; ++lit, ++iSlot)
        {
            KAttributeInfo kAtt;
            kAtt.m_cType = lit->first;
            kAtt.m_fValue = lit->second;
            kAtt.m_cState = KAttributeInfo::ATS_USING;
            kAtt.m_cSlotID = iSlot;
            kItem.m_vecAttribute.push_back( kAtt );
        }
    }

    return kItem;
}

void KGCCoordiUpgradeDlg::OnScrollPos()
{
    int iPos = m_pkScrollbar->GetScrollPos();

    if ( m_iScrollpos != iPos )
    {
        m_iScrollpos = iPos;
        g_KDSound.Play( "73" );

        UpdateInvenItemList();
    }
}

void KGCCoordiUpgradeDlg::UpdateItemCount()
{
    int nCount = g_pItemMgr->GetInventoryItemDuration( g_pItemMgr->m_sCoordiUpgradeInfo.m_GradeUpgradeItemID / 10 );
    m_pkStaticItemNum->SetText( nCount );
}

void KGCCoordiUpgradeDlg::UpdateCoordiList()
{
    g_pItemMgr->GetEnableCoordiUpgradeList( m_vecItemUIDList );

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;

    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );

    UpdateInvenItemList();
}

void KGCCoordiUpgradeDlg::UpdateInvenItemList()
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

void KGCCoordiUpgradeDlg::CreateInvenSlot()
{
    float fBasePosX = 540.0f;
    float fBasePosY = 61.0f;

    if(m_vecInvenItemImgList.empty()){     

        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_coordi_upgrade_dlg.stg", "",
            "coordi_upgrade_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );

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

void KGCCoordiUpgradeDlg::CheckItemInformation()
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

            m_vecInvenItemImgList[i]->SelectedItem( true );

            return;
        }
    }

    if ( m_pkCoordiItem->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_ItemUID );
        if ( !pkItem )
            return;

        DrawItemInfo( pkItem, m_pkCoordiItem->GetCurrentWindowPos() );
        return;
    }

    if ( m_pkResultItem->CheckMousePosInWindowBound() )
    {
        if ( m_ItemUID != 0 )
        {
            if ( m_nState == EM_RESULT_STATE )
            {
                KItem* pkItem = g_pItemMgr->GetInventoryItem( m_ItemUID );
                if ( !pkItem )
                    return;

                DrawItemInfo( pkItem, m_pkCoordiItem->GetCurrentWindowPos() );
            }
            else
            {
                KItem kItem = GetResultItem();

                DrawItemInfo( &kItem, m_pkResultItem->GetCurrentWindowPos() );                
            }

            return;
        }
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );

}

void KGCCoordiUpgradeDlg::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
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

