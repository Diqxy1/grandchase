#include "stdafx.h"
#include "GCEnchantSubS6Dlg.h"

IMPLEMENT_CLASSNAME( KGCEnchantSubS6Dlg );
IMPLEMENT_WND_FACTORY( KGCEnchantSubS6Dlg );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantSubS6Dlg, "gc_enchantsubs6dlg" );

KGCEnchantSubS6Dlg::KGCEnchantSubS6Dlg( void )
{
    m_pkBtnClose = NULL;
    m_pkStaticGP = NULL;
    m_pkStaticGPCost = NULL;
    m_pkStaticDesc = NULL;
    m_pkScrollbar = NULL;

    m_pkEnchantPanel = NULL;
    m_pkBreakUPPanel = NULL;
    m_pkAlchemyPanel = NULL;
    m_pkShopPanel = NULL;
    m_nState = EM_STATE_ENCHANT;

    m_vecInvenItemImgList.clear();
    m_vecItemUIDList.clear();

    m_nState = EM_STATE_ENCHANT;
    m_iScrollpos = 0;

    m_nParticleType = EM_PARTICLE_LV1;
    m_nParticleTime = 0;
    m_bSuccess = false;

    m_mapStrengthMaterialInfo.clear();
    m_setStrengthShopItemList.clear();
    m_dwStrengthMaxLevel = 0;
    m_setNonStrengthItem.clear();
    m_mapProtectionInfoList.clear();
    m_mapStrengthGPCost.clear();

    LINK_CONTROL( "btn_close", m_pkBtnClose );

    LINK_CONTROL( "static_gp",      m_pkStaticGP );
    LINK_CONTROL( "static_gp_cost", m_pkStaticGPCost );
    LINK_CONTROL( "static_desc",    m_pkStaticDesc );

    LINK_CONTROL( "scroll", m_pkScrollbar );

    LINK_CONTROL("itemlist_combobox", m_pkItemGroupCombo );

    LINK_CONTROL( "enchant_panel", m_pkEnchantPanel );
    LINK_CONTROL( "breakup_panel", m_pkBreakUPPanel );
    LINK_CONTROL( "alchemy_panel", m_pkAlchemyPanel );
    LINK_CONTROL( "shop_panel", m_pkShopPanel );
}

KGCEnchantSubS6Dlg::~KGCEnchantSubS6Dlg( void )
{
}

void KGCEnchantSubS6Dlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    
    m_pkStaticGP->InitState( true, true, this );
    m_pkStaticGP->SetAlign( DT_RIGHT );
    m_pkStaticGP->SetText( g_pkStrLoader->GetString(STR_ID_ENCHANT_GP) );
    
    m_pkStaticGPCost->InitState( true, true, this );
    m_pkStaticGPCost->SetAlign( DT_RIGHT );
    m_pkStaticGPCost->SetText( g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_GP_COST, "ii", 0, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iGP ) );

    m_pkStaticDesc->InitState( true, true, this );

    m_pkScrollbar->InitState(true,true,this);
    m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );

    m_pkItemGroupCombo->InitState( true, true, this );

    m_pkEnchantPanel->InitState(false,true,this);
    m_pkBreakUPPanel->InitState(false,true,this);
    m_pkAlchemyPanel->InitState(false,true,this);
    m_pkShopPanel->InitState(true,true,this);    

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN, this, &KGCEnchantSubS6Dlg::OnClearInven);

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT, this, &KGCEnchantSubS6Dlg::OnEquipSlot);
    
    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_SEL_ENCHANT_ENCHANT_SLOT);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SEL_ENCHANT_ENCHANT_SLOT, this, &KGCEnchantSubS6Dlg::OnEnchantSlot);
    
    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_SEL_ENCHANT_ASSIST_SLOT);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SEL_ENCHANT_ASSIST_SLOT, this, &KGCEnchantSubS6Dlg::OnAssistSlot);    

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_ENCHANT_OK);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_ENCHANT_OK, this, &KGCEnchantSubS6Dlg::OnOK);    

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_ENCHANT_EQUIP_OK);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_ENCHANT_EQUIP_OK, this, &KGCEnchantSubS6Dlg::OnEquip);        


    CreateInvenSlot();
    InitUIState();
}


void KGCEnchantSubS6Dlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkItemGroupCombo, KD3DWnd::D3DWE_COMBO_SELCHANGE, OnComboSelChange );
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

void KGCEnchantSubS6Dlg::FrameMoveInEnabledState( void )
{
    CheckParticleTime();
    CheckItemInformation();
}

void KGCEnchantSubS6Dlg::PostChildDraw( void )
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

        g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );     
    }
    g_pGCDeviceManager2->PopState();

}

void KGCEnchantSubS6Dlg::OnOK()
{
    switch( m_nState )
    {
    case EM_STATE_ENCHANT:
        {
            if ( !DoEnchantItem() )
                return;
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            if ( !DoBreakUPItem() )
                return;
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            if ( !DoAlchemyItem() )
                return;
        }
        break;
    }

    LockUI( true );
}

void KGCEnchantSubS6Dlg::OnEquip()
{
    if ( m_nState != EM_STATE_ENCHANT )
        return;

    if ( !DoEquipItem() )
        return;

    LockUI( true );
}

bool KGCEnchantSubS6Dlg::DoEnchantItem()
{
    GCITEMUID EquipitemUID = m_pkEnchantPanel->GetEquipItemUID();
    GCITEMUID AssistItemUID = m_pkEnchantPanel->GetAssistItemUID();
    GCITEMID dwAssistItemID = 0;
    DWORD dwAssistType = AT_NOUSE;
    DWORD dwItemSlotType = m_pkItemGroupCombo->GetCurSel() + 1;

    KItem* kItem = g_pItemMgr->GetInventoryItem( EquipitemUID );
    if ( !kItem )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ENCHANT_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KItem* kItemAssist = g_pItemMgr->GetInventoryItem( AssistItemUID );
    if ( kItemAssist )
    {
        dwAssistItemID = kItemAssist->m_ItemID;
    }

    if( kItem->m_nPeriod > 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_PERIODITEM_ERROR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( AssistItemUID != 0 && !CheckValidScroll( dwAssistItemID * 10, dwAssistType ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SCROLL_ERROR01 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( AssistItemUID != 0 && !IsNoBreakLevelCheck( kItem, kItemAssist, dwAssistType ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOBREAK_LEVEL_CHECK ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

        return false;
    }

    if ( kItem->m_cEnchantLevel >= (int)m_dwStrengthMaxLevel )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_MAX_LEVEL_ERROR ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );        
        return false;
    }

    // 보유 GP 확인
    if ( false )// g_kGlobalValue.m_kUserInfo.GetCurrentChar().iGP )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_COST_GP_ERROR ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );        
        return false;
    }


    KEVENT_STRENGTH_ACTION_REQ kPacket;
    kPacket.m_StrengthItemUID = kItem->m_EnchantEquipItemUID;        
    kPacket.m_dwAssistType = dwAssistType;
    kPacket.m_AssistItemID = dwAssistItemID * 10;       
    kPacket.m_dwEquipItemType = dwItemSlotType;
    KP2P::GetInstance()->Send_StrengthActionReq(kPacket);

    // 파티클 타입 셋팅          
    if( kItem->m_cEnchantLevel <= 3 )
    {
        m_nParticleType = EM_PARTICLE_LV1;
    }
    else if( 3 < kItem->m_cEnchantLevel && kItem->m_cEnchantLevel <= 6 )
    {
        m_nParticleType = EM_PARTICLE_LV2;
    }
    else
    {
        m_nParticleType = EM_PARTICLE_LV3;
    }

    return true;
}

bool KGCEnchantSubS6Dlg::DoBreakUPItem()
{
    GCITEMUID EquipitemUID = m_pkBreakUPPanel->GetEquipItemUID();
    GCITEMUID AssistItemUID = m_pkBreakUPPanel->GetAssistItemUID();
    GCITEMID dwAssistItemID = 0;
    DWORD dwAssistType = AT_NOUSE;

    KItem* kItem = g_pItemMgr->GetInventoryItem( EquipitemUID );
    if ( !kItem )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ERROR_00 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KItem* kItemAssist = g_pItemMgr->GetInventoryItem( AssistItemUID );
    if ( !kItemAssist )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ERROR_00 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }
    
    dwAssistItemID = kItemAssist->m_ItemID;


    if( kItem->m_nPeriod > 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_PERIODITEM_ERROR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( AssistItemUID == 0  )
    {
        //  해체 보조 아이템이 없음
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_BREAKUP_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( false == CheckValidScroll( dwAssistItemID * 10, dwAssistType ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SCROLL_ERROR01 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KP2P::GetInstance()->Send_StrengthBreakUpReq(kItem->m_EnchantEquipItemUID, AssistItemUID);

    m_nParticleType = EM_PARTICLE_SMALL;

    return true;
}

bool KGCEnchantSubS6Dlg::DoAlchemyItem()
{
    GCITEMUID EquipitemUID1 = m_pkAlchemyPanel->GetEquipItemUID1();
    GCITEMUID EquipitemUID2 = m_pkAlchemyPanel->GetEquipItemUID2();
    GCITEMUID AssistItemUID = m_pkAlchemyPanel->GetAssistItemUID();
    GCITEMID dwAssistItemID = 0;
    DWORD dwAssistType = AT_NOUSE;

    KItem* kItem1 = g_pItemMgr->GetInventoryItem( EquipitemUID1 );
    if ( !kItem1 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KItem* kItem2 = g_pItemMgr->GetInventoryItem( EquipitemUID2 );
    if ( !kItem2 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KItem* kItemAssist = g_pItemMgr->GetInventoryItem( AssistItemUID );
    if ( !kItemAssist )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    dwAssistItemID = kItemAssist->m_ItemID;

    if( kItem1->m_nPeriod > 0 || kItem2->m_nPeriod > 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_PERIODITEM_ERROR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( EquipitemUID2 == 0  )
    {
        //  연금에 사용될 재료
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( AssistItemUID == 0  )
    {
        //  연금에 사용될 주문서.
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }


    if ( false == CheckValidScroll( dwAssistItemID * 10, dwAssistType ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SCROLL_ERROR01 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KP2P::GetInstance()->Send_StrengthComposeReq(std::make_pair(kItem1->m_EnchantEquipItemUID, kItem2->m_EnchantEquipItemUID), AssistItemUID);

    m_nParticleType = EM_PARTICLE_SMALL;

    return true;
}

bool KGCEnchantSubS6Dlg::DoEquipItem()
{
    GCITEMUID EquipitemUID = m_pkEnchantPanel->GetEquipItemUID();
    GCITEMUID EnchantItemUID = m_pkEnchantPanel->GetEnchantItemUID();
    DWORD dwItemSlotType = m_pkItemGroupCombo->GetCurSel() + 1;

    KItem* kItem = g_pItemMgr->GetInventoryItem( EquipitemUID );
    if ( !kItem )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_EQUIP_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );        
        return false;
    }

    KItem* kItemEnchant = g_pItemMgr->GetInventoryItem( EnchantItemUID );
    if ( !kItemEnchant )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_EQUIP_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );        
        return false;
    }

    if( kItem->m_nPeriod > 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_PERIODITEM_ERROR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    if ( EnchantItemUID == 0  )
    {
        //  강화석 선택해주세요.
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_EQUIP_EMPTY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }

    KP2P::GetInstance()->Send_StrengthEquipReq(EnchantItemUID, EquipitemUID, dwItemSlotType);

    return true;
}

void KGCEnchantSubS6Dlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEnchantSubS6Dlg::OnComboSelChange()
{
    OnClearInven();
    
    switch( m_nState ) 
    {
    case EM_STATE_ENCHANT:
        m_pkEnchantPanel->InitUIState();
        break;
    case EM_STATE_BREAKEUP:
        m_pkBreakUPPanel->InitUIState();
        break;
    case EM_STATE_ALCHEMY:
        m_pkAlchemyPanel->InitUIState();
        break;
    }
}

void KGCEnchantSubS6Dlg::OnScrollPos( void )
{
    int iPos = m_pkScrollbar->GetScrollPos();

    if ( m_iScrollpos != iPos )
    {
        m_iScrollpos = iPos;
        g_KDSound.Play( "73" );

        UpdateInvenItemList();
    }
}
void KGCEnchantSubS6Dlg::OnClearInven()
{
    m_vecItemUIDList.clear();

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i )
    {
        m_vecInvenItemImgList[i]->ResetItem();
    }
}

void KGCEnchantSubS6Dlg::OnEquipSlot()
{
    UpdateEquipSlotItemList();
}

void KGCEnchantSubS6Dlg::OnEnchantSlot()
{
    UpdateEnchantSlotItemList();
}

void KGCEnchantSubS6Dlg::OnAssistSlot()
{
    UpdateAssistSlotItemList();
}

bool KGCEnchantSubS6Dlg::CheckCondition(  IN const KDialogInfo& kInfo_ )
{
    m_nState = (int)kInfo_.m_lParam;
    return true;
}

void KGCEnchantSubS6Dlg::CreateInvenSlot()
{
    float fBasePosX = 540.0f;
    float fBasePosY = 61.0f;

    if(m_vecInvenItemImgList.empty()){     

        std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_enchant_s6_dlg.stg", "",
            "enchant_sub_s6_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );

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

void KGCEnchantSubS6Dlg::InitUIState()
{
    m_pkItemGroupCombo->DeleteAllItem();
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1 ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2 ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HAND ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_MANTLE ) );
    m_pkItemGroupCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON ) );
    m_pkItemGroupCombo->SetCurSel( EM_ITEM_HEAD-1 );

    m_iScrollpos = 0;

    m_nParticleType = EM_PARTICLE_LV1;
    m_nParticleTime = 0;
    m_bSuccess = false;


    m_pkBtnClose->Lock( false );
    m_pkItemGroupCombo->Lock( false );

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i ) 
    {
        m_vecInvenItemImgList[i]->Lock( false );
    }


    UpdateUIState();
}

void KGCEnchantSubS6Dlg::UpdateUIState()
{
    switch( m_nState ) 
    {
    case EM_STATE_ENCHANT:
        {
            m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ENCHANT_ENCHANT_DESC ) );
            m_pkEnchantPanel->ToggleRender( true );
            m_pkBreakUPPanel->ToggleRender( false );
            m_pkAlchemyPanel->ToggleRender( false );
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ENCHANT_BREAKUP_DESC ) );
            m_pkEnchantPanel->ToggleRender( false );
            m_pkBreakUPPanel->ToggleRender( true );
            m_pkAlchemyPanel->ToggleRender( false );
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ENCHANT_ALCHEMY_DESC ) );
            m_pkEnchantPanel->ToggleRender( false );
            m_pkBreakUPPanel->ToggleRender( false );
            m_pkAlchemyPanel->ToggleRender( true );
        }
        break;
    }
}

void KGCEnchantSubS6Dlg::LockUI( bool bLock )
{
    m_pkItemGroupCombo->Lock( bLock );
    m_pkBtnClose->Lock( bLock );

    for ( int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i ) 
    {
        m_vecInvenItemImgList[i]->Lock( bLock );
    }

    m_pkShopPanel->LockUI( bLock );


    switch( m_nState ) 
    {
    case EM_STATE_ENCHANT:
        {
            m_pkEnchantPanel->LockUI( bLock );
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            m_pkBreakUPPanel->LockUI( bLock );
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            m_pkAlchemyPanel->LockUI( bLock );
        }
        break;
    }
}

void KGCEnchantSubS6Dlg::SetResultEquip()
{
    if ( m_nState != EM_STATE_ENCHANT )
        return;

    m_pkEnchantPanel->SetResultEquip();
    UpdateGPCost();

    LockUI( false );
}

void KGCEnchantSubS6Dlg::SetResult()
{

    switch( m_nState ) 
    {
    case EM_STATE_ENCHANT:
        {
            m_pkEnchantPanel->SetResult();
            UpdateGPCost();
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            m_pkBreakUPPanel->SetResult();
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            m_pkAlchemyPanel->SetResult();
        }
        break;
    }

    LockUI( false );
}

void KGCEnchantSubS6Dlg::UpdateInvenItemList()
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

void KGCEnchantSubS6Dlg::UpdateEquipSlotItemList()
{
    GetEquipSlotItemList( true );

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;


    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );

    UpdateInvenItemList();
}

void KGCEnchantSubS6Dlg::UpdateEnchantSlotItemList()
{
    GetEnchantSlotItemList();

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;


    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );

    UpdateInvenItemList();
}

void KGCEnchantSubS6Dlg::UpdateAssistSlotItemList()
{
    GetAssistSlotItemList();

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDList.size();
    int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;


    m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
    m_pkScrollbar->SetScrollRangeMax( nScrollMax );
    m_pkScrollbar->SetScrollPos( m_iScrollpos );

    UpdateInvenItemList();
}

void KGCEnchantSubS6Dlg::OnInvenItemList( int index )
{
    int nIndex = ( m_iScrollpos * EM_INVEN_COLUMN ) + index;
    if ( nIndex < 0 || (int)m_vecItemUIDList.size() <= nIndex )
        return;

    GCITEMUID itemUID = m_vecItemUIDList[nIndex];
    KItem* pkItem = g_pItemMgr->GetInventoryItem( itemUID );
    if ( !pkItem )
        return;   

    switch ( m_nState ) 
    {
    case EM_STATE_ENCHANT:
        {
            m_pkEnchantPanel->OnInvenItemList( itemUID );
            UpdateGPCost();
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            m_pkBreakUPPanel->OnInvenItemList( itemUID );
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            m_pkAlchemyPanel->OnInvenItemList( itemUID );
        }
        break;
    }
}

void KGCEnchantSubS6Dlg::GetAssistSlotItemList()
{
    std::vector<std::pair<DWORD,DWORD>> vecType;
    m_vecItemUIDList.clear();

    switch ( m_nState )
    {
    case EM_STATE_ENCHANT:
        {
            vecType.push_back( std::make_pair(m_nState,AT_UPGRADE) );
            vecType.push_back( std::make_pair(m_nState,AT_NOBREAK) );
            vecType.push_back( std::make_pair(m_nState,AT_SPECIFIC) );
            vecType.push_back( std::make_pair(m_nState,AT_UPGRADE_NOBREAK) );
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            vecType.push_back( std::make_pair(m_nState,AT_UPGRADE) );
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            vecType.push_back( std::make_pair(m_nState,AT_UPGRADE) );
        }
        break;
    }

    for ( int i = 0; i < (int)vecType.size(); ++i ) 
    {
        std::pair<DWORD,DWORD> pairTypeIndex_ = vecType[i];
        std::map<std::pair<DWORD,DWORD>, std::set<GCITEMID>>::iterator mit = m_mapStrengthMaterialInfo.find(pairTypeIndex_);
        if( mit != m_mapStrengthMaterialInfo.end() )
        {
            std::set<GCITEMID>::iterator sit = mit->second.begin();
            for( sit; sit != mit->second.end(); ++sit ) 
            {
                KItem* kItem = g_pItemMgr->m_kInventory.FindItemByItemID((*sit)/10);
                if( kItem != NULL )
                {
                    m_vecItemUIDList.push_back(kItem->m_ItemUID);
                }
            }
        }
    }
}

void KGCEnchantSubS6Dlg::GetEnchantSlotItemList()
{
    m_vecItemUIDList.clear();

    std::vector<GCITEMUID> vecItemUID = g_pItemMgr->GetInventoryItemUIDListByKind(GIK_ENCHANT_ENABLE_ITEM);
    for( int i = 0; i < static_cast<int>(vecItemUID.size()); i++ )
    {
        KItem* kItem = g_pItemMgr->GetInventoryItem(vecItemUID[i]);
        if ( !kItem )
            continue;

        if( kItem->m_EnchantEquipItemUID == 0 )
            m_vecItemUIDList.push_back(kItem->m_ItemUID);
    }

    /*if (g_MyD3D->m_kGuideMgr.IsGuideOngoing()) {
        std::sort(m_vecItemUIDList.begin(), m_vecItemUIDList.end(), boost::bind( SortByEnchantItem, _2, _1));
    } else {
        std::sort(m_vecItemUIDList.begin(), m_vecItemUIDList.end(), boost::bind( SortByEnchantItem, _1, _2));
    }*/
}

void KGCEnchantSubS6Dlg::GetEquipSlotItemList( bool bSort )
{
    ESLOTPOSITION curPart = GetItemPartsPosition( m_pkItemGroupCombo->GetCurSel() + 1 );

    m_vecItemUIDList.clear();
    std::vector<GCITEMUID> vecInvenList =  g_pItemMgr->GetInventoryItemUIDAllList( curPart, 0 );

    for( int i = 0; i < static_cast<int>(vecInvenList.size()); i++ )
    {
        KItem* kItem = g_pItemMgr->GetInventoryItem( vecInvenList[i] );
        if( kItem != NULL )
        {
            GCItem* pItem = g_pItemMgr->GetItemData(kItem->m_ItemID);
            bool bIsEquipedItem = false;

            if( pItem == NULL ){
                continue;
            }

            if( pItem->eItemKind == GIK_MONSTER_CARD || 
                pItem->eItemKind == GIK_COORDI_ITEM  || 
                pItem->eItemKind == GIK_COORDI_PACKAGE7 ){
                    continue;
            }

            std::set< GCITEMID >::iterator sit = m_setNonStrengthItem.find( pItem->dwGoodsID * 10);
            if( sit != m_setNonStrengthItem.end() ) { 
                continue;
            }

            if( kItem->CheckItemType(KItem::TYPE_LOOK) ){
                continue;
            }

            if( kItem->m_nPeriod > 0 )
                continue;

            if( kItem->m_nCount > -1 )
                continue;

            if( kItem->m_ItemUID <= KGCItemManager::MAXIMUM_PCBANG_UID ){
                continue;
            }

            for( int j = 0; j < static_cast<int>(g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size()); j++ )
            {
                if( g_pItemMgr->IsEquipedItem(&g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[j], kItem->m_ItemUID) == true )
                {
                    bIsEquipedItem = true;
                    break;
                }
            }


            switch( m_nState )
            {
            case EM_STATE_ENCHANT:
                {
                    if ( kItem->m_cEnchantLevel >= (int)m_dwStrengthMaxLevel )
                        continue;
                }
                break;
            case EM_STATE_BREAKEUP:
                {
                    if( bIsEquipedItem == true )
                        continue;

                    if ( kItem->m_EnchantEquipItemUID == 0 ) 
                        continue;
                }
                break;
            case EM_STATE_ALCHEMY:
                {
                    if( bIsEquipedItem == true )
                        continue;

                    if ( kItem->m_EnchantEquipItemUID == 0 ) 
                        continue;

                    if ( m_pkAlchemyPanel->IsEquipItem( kItem->m_ItemUID ) )
                        continue;
                }
                break;
            }           


            m_vecItemUIDList.push_back( kItem->m_ItemUID );
        }                
    }

    /*if (bSort)
    {
        std::sort(m_vecItemUIDList.begin(), m_vecItemUIDList.end(), boost::bind( SortByEquipItem, _1, _2));
    }*/
}


void KGCEnchantSubS6Dlg::RenderParticle( bool bSucc )
{
    float fPosX = -0.25f;
    float fPosY = 0.015f;

//     KLuaManager luaMgr;
//     GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
//     LUA_GET_VALUE_DEF( "PosX", fPosX, 0 );
//     LUA_GET_VALUE_DEF( "PosY", fPosY, 0 );

    m_bSuccess = bSucc;


    switch ( m_nParticleType ) 
    {
    case EM_PARTICLE_LV1:
        {
            m_nParticleTime = 190; //110;
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV1_01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV1_02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV1_03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }
        break;
    case EM_PARTICLE_LV2:
        {
            m_nParticleTime = 245; //165 ;
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV2_01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV2_02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV2_03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV2_04", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV2_05", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }
        break;
    case EM_PARTICLE_LV3:
        {
            m_nParticleTime = 300; //220;
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_04", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_05", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_06", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_07", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_08", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_09", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_LV3_10", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }
        break;
    case EM_PARTICLE_SMALL:
        {
            m_nParticleTime = 135;
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Small_01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Small_02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Small_03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Small_04", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Upgrade_Small_05", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }
        break;
    }
}

void KGCEnchantSubS6Dlg::CheckParticleTime()
{
    if( m_nParticleTime == 0)
        return;

    m_nParticleTime--;

    float fPosX = -0.25f;
    float fPosY = 0.015f;

//     KLuaManager luaMgr;
//     GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
//     LUA_GET_VALUE_DEF( "PosX", fPosX, 0 );
//     LUA_GET_VALUE_DEF( "PosY", fPosY, 0 );

    if( m_nParticleTime == 80 )
    {
        //성공파티클
        if( m_bSuccess )
        {
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("success03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }
        else
        {
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
            g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("fail03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
        }        
    }
    else if( m_nParticleTime == 0 )
    {
       SetResult();
    }    

}



void KGCEnchantSubS6Dlg::CheckItemInformation()
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

    if ( m_pkShopPanel->CheckItemInformation() )
    {
        return;
    }

    switch( m_nState )
    {
    case EM_STATE_ENCHANT:
        {
            if( m_pkEnchantPanel->CheckItemInformation() )
                return;
        }
        break;
    case EM_STATE_BREAKEUP:
        {
            if( m_pkBreakUPPanel->CheckItemInformation() )
                return;
        }
        break;
    case EM_STATE_ALCHEMY:
        {
            if( m_pkAlchemyPanel->CheckItemInformation() )
                return;
        }
        break;
    };

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );

}

void KGCEnchantSubS6Dlg::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
{
    if( pItem != NULL && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
    {
        vPos.x += 49.f * GC_SCREEN_DIV_WIDTH;
        vPos.y += 20.f * GC_SCREEN_DIV_WIDTH;
        vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
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

bool KGCEnchantSubS6Dlg::SortByEnchantItem(const GCITEMUID& left_, const GCITEMUID& right_ )
{
    KItem* kItemLeft = g_pItemMgr->GetInventoryItem( left_ );
    KItem* kItemRight = g_pItemMgr->GetInventoryItem( right_ );

    if( kItemLeft->m_cEnchantLevel == kItemRight->m_cEnchantLevel )
    {
        return kItemLeft->m_sEquipLevel > kItemRight->m_sEquipLevel;
    }
    else
    {
        return kItemLeft->m_cEnchantLevel > kItemRight->m_cEnchantLevel;
    }
}

bool KGCEnchantSubS6Dlg::SortByEquipItem( const GCITEMUID& left_, const GCITEMUID& right_ )
{
    KItem* kItemLeft = g_pItemMgr->GetInventoryItem( left_ );
    KItem* kItemRight = g_pItemMgr->GetInventoryItem( right_ );

    if ( !kItemLeft || !kItemRight )
        return false;

    bool bLeftEquip = g_pItemMgr->IsEquipedItem( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), kItemLeft->m_ItemUID );
    bool bRightEquip = g_pItemMgr->IsEquipedItem( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), kItemRight->m_ItemUID );

    if ( bLeftEquip != bRightEquip )
    {
        if ( bLeftEquip )
            return true;
        else 
            return false;

    }
    else
    {
        if( kItemLeft->m_cGradeID == kItemRight->m_cGradeID )
        {
            if( kItemLeft->m_sEquipLevel == kItemRight->m_sEquipLevel )
            {
                if( kItemLeft->m_EnchantEquipItemUID == 0 && kItemRight->m_EnchantEquipItemUID == 0 )
                {
                    return kItemLeft->m_cEnchantLevel >= kItemRight->m_cEnchantLevel;
                }
                else
                {
                    return kItemLeft->m_EnchantEquipItemUID > kItemRight->m_EnchantEquipItemUID;
                }
            }
            else
            {
                return kItemLeft->m_sEquipLevel > kItemRight->m_sEquipLevel;
            }
        }
        else
        {
            return kItemLeft->m_cGradeID > kItemRight->m_cGradeID;
        }
    }
}

void KGCEnchantSubS6Dlg::SetMaterialInfo( KEVENT_STRENGTH_MATERIAL_INFO& kMaterialInfo_ )
{
    m_mapStrengthMaterialInfo = kMaterialInfo_.m_mapStrengthMaterialInfo;
    m_setStrengthShopItemList = kMaterialInfo_.m_setStrengthShopItemList;
    m_dwStrengthMaxLevel = kMaterialInfo_.m_dwStrengthMaxLevel;
    m_setNonStrengthItem = kMaterialInfo_.m_setNonStrengthItem;
    m_mapProtectionInfoList = kMaterialInfo_.m_mapProtectionInfoList;
    m_mapStrengthGPCost = kMaterialInfo_.m_mapStrengthGPCost;
   
    if (m_pkShopPanel)
        m_pkShopPanel->SetEnchantShopItemList(kMaterialInfo_.m_setStrengthShopItemList);
}

ESLOTPOSITION KGCEnchantSubS6Dlg::GetItemPartsPosition( int iIndex_ )
{
    switch(iIndex_)
    {
    case EM_ITEM_HEAD:
        return ESP_HELMET;
    case EM_ITEM_BODY1:
        return ESP_JACKET;
    case EM_ITEM_BODY2:
        return ESP_PANTS;
    case EM_ITEM_HAND:
        return ESP_GLOVES;
    case EM_ITEM_SHOOSE:
        return ESP_SHOES;
    case EM_ITEM_MANTLE:
        return ESP_MANTLE;
    case EM_ITEM_WEAPON:
        return ESP_WEAPON;
    default:
        return ESP_HELMET;
    };
}


bool KGCEnchantSubS6Dlg::CheckValidScroll( GCITEMID _itemID, DWORD& dwAssistType )
{
    if ( _itemID == 0 )
        return false;

    std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >>::iterator iter_beg = m_mapStrengthMaterialInfo.begin();
    std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >>::iterator iter_end = m_mapStrengthMaterialInfo.end();

    for( ; iter_beg != iter_end; ++iter_beg )
    {
        std::pair<DWORD, DWORD> pair = iter_beg->first;

        if ( pair.first != m_nState )
            continue;

        if ( pair.second != AT_UPGRADE && pair.second != AT_NOBREAK && pair.second != AT_SPECIFIC && pair.second != AT_UPGRADE_NOBREAK )
            continue;
            

        if ( iter_beg->second.find( _itemID ) != iter_beg->second.end() )
        {
            dwAssistType = pair.second;
            return true;
        }
    }       

    return false;
}

bool KGCEnchantSubS6Dlg::IsNoBreakLevelCheck( KItem* kItem_, KItem* kItemAssist_, int iType_ )
{
    if ( !kItem_ || !kItemAssist_ )
        return false;

    switch( iType_ )
    {
    case AT_UPGRADE:
    case AT_SPECIFIC:
    case AT_RANDOM:
        return true;
    case AT_UPGRADE_NOBREAK:        
    case AT_NOBREAK:
        break;
    default:    
        return true;
    }

    std::map<GCITEMID, std::pair<int,int>>::iterator pos = m_mapProtectionInfoList.find( kItemAssist_->m_ItemID * 10 );
    if ( pos == m_mapProtectionInfoList.end() )
        return false;

    int nMinLevel = pos->second.first;
    int nMaxLevel = pos->second.second;
    int nCurLevel = kItem_->m_cEnchantLevel + 1;

    if ( nCurLevel < nMinLevel || nCurLevel > nMaxLevel ) 
        return false;

    return true;
}

void KGCEnchantSubS6Dlg::SetAlchemyResultItem( GCITEMUID _itemUID )
{
    m_pkAlchemyPanel->SetAlchemyResultItem( _itemUID );
}

void KGCEnchantSubS6Dlg::UpdateGPCost()
{
    std::wstring strGPCost = g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_GP_COST, "ii", 0, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iGP );    
    
    m_pkStaticGPCost->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
    m_pkStaticGPCost->SetText( strGPCost );

    if ( m_nState != EM_STATE_ENCHANT )
        return;

    KItem* kItem = g_pItemMgr->GetInventoryItem( m_pkEnchantPanel->GetEquipItemUID() );
    if ( !kItem )
        return;

    if ( kItem->m_EnchantEquipItemUID == 0 )
        return;


    int iItemLevel = kItem->m_sEquipLevel;
    if( iItemLevel == 0 || iItemLevel == -1 )
    {
        GCItem* pItem = g_pItemMgr->GetItemData(kItem->m_ItemID);
        if( pItem )
        {
            iItemLevel = pItem->iNeedLevelForEquippingItem;
        }
    }

    int nGPCost = 0;
    
    std::map<DWORD, std::pair<int,int>>::iterator iter = m_mapStrengthGPCost.find( iItemLevel );
    if ( iter != m_mapStrengthGPCost.end() )
    {
        if ( m_pkItemGroupCombo->GetCurSel() == EM_ITEM_WEAPON-1 )
        {
            nGPCost = iter->second.first;
        }
        else
        {
            nGPCost = iter->second.second;
        }
    }

    if ( nGPCost > g_kGlobalValue.m_kUserInfo.GetCurrentChar().iGP )
    {
        m_pkStaticGPCost->SetFontColor( D3DCOLOR_ARGB(255, 255, 0, 0) );
    }
    
    strGPCost = g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_GP_COST, "ii", nGPCost, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iGP );    
    m_pkStaticGPCost->SetText( strGPCost );
}