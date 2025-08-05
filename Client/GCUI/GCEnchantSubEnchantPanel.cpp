#include "stdafx.h"
#include "GCEnchantSubEnchantPanel.h"

IMPLEMENT_CLASSNAME( KGCEnchantSubEnchantPanel );
IMPLEMENT_WND_FACTORY( KGCEnchantSubEnchantPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantSubEnchantPanel, "gc_enchantsubenchantpanel" );

KGCEnchantSubEnchantPanel::KGCEnchantSubEnchantPanel( void )
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID = 0;
    m_EnchantitemUID = 0;
    m_AssistitemUID = 0;

    m_pkBtnEnchant = NULL;
    m_pkBtnEquip = NULL;

    m_pkBtnEquipSlot = NULL;
    m_pkBtnEnchatSlot = NULL;
    m_pkBtnAssistSlot = NULL;

    m_pkSelEquipSlot = NULL;
    m_pkSelEnchatSlot = NULL;
    m_pkSelAssistSlot = NULL;

    m_pkEquipItemImg = NULL;
    m_pkEnchantItemImg = NULL;
    m_pkAssistItemImg = NULL;

    LINK_CONTROL( "btn_ok", m_pkBtnEnchant );
    LINK_CONTROL( "btn_ok2", m_pkBtnEquip );
    
    LINK_CONTROL( "equip_slot_btn",     m_pkBtnEquipSlot );
    LINK_CONTROL( "enchant_slot_btn",   m_pkBtnEnchatSlot );
    LINK_CONTROL( "assist_slot_btn",    m_pkBtnAssistSlot );

    LINK_CONTROL( "sel_equip_slot",     m_pkSelEquipSlot );
    LINK_CONTROL( "sel_enchant_slot",   m_pkSelEnchatSlot );
    LINK_CONTROL( "sel_assist_slot",    m_pkSelAssistSlot );

    
    LINK_CONTROL( "equip_slotitem_img", m_pkEquipItemImg );
    LINK_CONTROL( "enchant_slotitem_img", m_pkEnchantItemImg );
    LINK_CONTROL( "assist_slotitem_img", m_pkAssistItemImg );

}

KGCEnchantSubEnchantPanel::~KGCEnchantSubEnchantPanel( void )
{
}

void KGCEnchantSubEnchantPanel::OnCreate( void )
{
    m_pkBtnEnchant->InitState( true, true, this );
    m_pkBtnEquip->InitState( false, true, this );

    m_pkBtnEquipSlot->InitState( true, true, this );
    m_pkBtnEnchatSlot->InitState( true, true, this );
    m_pkBtnAssistSlot->InitState( true, true, this );

    m_pkSelEquipSlot->InitState( false );
    m_pkSelEnchatSlot->InitState( false );
    m_pkSelAssistSlot->InitState( false );

    m_pkEquipItemImg->InitState( true, true, this );
    m_pkEnchantItemImg->InitState( true, true, this );
    m_pkAssistItemImg->InitState( true, true, this );

    InitUIState();
}

void KGCEnchantSubEnchantPanel::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnEnchant, KD3DWnd::D3DWE_BUTTON_CLICK, OnEnchant );
    GCWND_MSG_MAP( m_pkBtnEquip, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquip );

    GCWND_MSG_MAP( m_pkBtnEquipSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipSlot );
    GCWND_MSG_MAP( m_pkBtnEnchatSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnEnchantSlot );
    GCWND_MSG_MAP( m_pkBtnAssistSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistSlot );

    GCWND_MSG_MAP( m_pkEquipItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipItemImg );
    GCWND_MSG_MAP( m_pkEnchantItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnEnchantItemImg );
    GCWND_MSG_MAP( m_pkAssistItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistItemImg );
}

void KGCEnchantSubEnchantPanel::InitUIState()
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID = 0;
    m_EnchantitemUID = 0;
    m_AssistitemUID = 0;


    m_pkBtnEnchant->ToggleRender( true );
    m_pkBtnEnchant->Lock( true );

    m_pkBtnEquip->ToggleRender( false );
    m_pkBtnEquip->Lock( true );

    m_pkBtnEquipSlot->ToggleRender( true );
    m_pkBtnEnchatSlot->ToggleRender( true );
    m_pkBtnAssistSlot->ToggleRender( true );

    m_pkBtnEquipSlot->Lock( false );
    m_pkBtnEnchatSlot->Lock( true );
    m_pkBtnAssistSlot->Lock( true );

    m_pkSelEquipSlot->ToggleRender( false );
    m_pkSelEnchatSlot->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkSelEquipSlot->Lock( false );
    m_pkSelEnchatSlot->Lock( false );
    m_pkSelAssistSlot->Lock( false );

    m_pkEquipItemImg->ToggleRender( false );
    m_pkEnchantItemImg->ToggleRender( false );
    m_pkAssistItemImg->ToggleRender( false );

    m_pkEquipItemImg->Lock( false );
    m_pkEnchantItemImg->Lock( false );
    m_pkAssistItemImg->Lock( false );
}

void KGCEnchantSubEnchantPanel::UpdateUIState()
{
    bool bEquipItem = ( m_EquipitemUID != 0 );
    bool bEnchantItem = ( m_EnchantitemUID != 0 );
    bool bAssistItem = ( m_AssistitemUID != 0 );

    if ( bEquipItem )
    {
        KItem* kItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
        if ( kItem )
        {
            if ( kItem->m_EnchantEquipItemUID > 0 )
            {
                m_pkBtnEnchant->ToggleRender( true );
                m_pkBtnEquip->ToggleRender( false );
            }
            else
            {
                m_pkBtnEnchant->ToggleRender( false );
                m_pkBtnEquip->ToggleRender( true );
            }
        }
    }
    else
    {
        m_pkBtnEnchant->ToggleRender( true );
        m_pkBtnEquip->ToggleRender( false );
    }

    m_pkBtnEquipSlot->ToggleRender( !bEquipItem );
    m_pkSelEquipSlot->ToggleRender( bEquipItem );
    m_pkEquipItemImg->ToggleRender( bEquipItem );

    m_pkBtnEnchatSlot->ToggleRender( !bEnchantItem );
    m_pkSelEnchatSlot->ToggleRender( bEnchantItem );
    m_pkEnchantItemImg->ToggleRender( bEnchantItem );

    m_pkBtnAssistSlot->ToggleRender( !bAssistItem );
    m_pkSelAssistSlot->ToggleRender( bAssistItem );
    m_pkAssistItemImg->ToggleRender( bAssistItem );
}

void KGCEnchantSubEnchantPanel::LockUI( bool bLock )
{
    if ( bLock )
    {
        m_pkBtnEnchant->Lock( true );
        m_pkBtnEquip->Lock( true );

        m_pkBtnEquipSlot->Lock( true );
        m_pkBtnEnchatSlot->Lock( true );
        m_pkBtnAssistSlot->Lock( true );

        m_pkEquipItemImg->Lock( true );
        m_pkEnchantItemImg->Lock( true );
        m_pkAssistItemImg->Lock( true );
    }
    else
    {
        if ( m_EquipitemUID != 0 )
        {
            KItem* kItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
            if ( kItem )
            {            
                if ( kItem->m_EnchantEquipItemUID > 0 )
                {
                    m_pkBtnEnchant->Lock( false );
                    m_pkBtnAssistSlot->Lock( false );
                }
                else
                {
                    m_pkBtnEnchatSlot->Lock( false );

                    if ( m_EnchantitemUID != 0 )
                    {
                        m_pkBtnEquip->Lock( false );
                    }
                }

                // 최고레벨이 되었음.
                if ( kItem->m_cEnchantLevel >= (int)g_pkUIScene->m_pkEnchantSubS6Dlg->GetStrengthMaxLevel() )
                {
                    m_pkBtnEnchant->Lock( true );
                }
            }           
        }

        m_pkBtnEquipSlot->Lock( false );
        m_pkEquipItemImg->Lock( false );
        m_pkEnchantItemImg->Lock( false );
        m_pkAssistItemImg->Lock( false );
    }
}

void KGCEnchantSubEnchantPanel::SetResultEquip()
{
    KItem* kItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
    if ( !kItem )
        return;

    m_pkEquipItemImg->SetItem( kItem );

    m_EnchantitemUID = 0;
    m_pkEnchantItemImg->ResetItem();

    m_AssistitemUID = 0;
    m_pkAssistItemImg->ResetItem();

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );

    UpdateUIState();
}

void KGCEnchantSubEnchantPanel::SetResult()
{
    KItem* kItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
    if ( !kItem )
        return;

    m_pkEquipItemImg->SetItem( kItem );

    KItem* kItemAssist = g_pItemMgr->GetInventoryItem( m_AssistitemUID );
    if ( !kItemAssist )
    {
        m_AssistitemUID = 0;
        m_pkAssistItemImg->ResetItem();
    }
    else
    {
        m_pkAssistItemImg->SetItem( kItemAssist );
    }

    // 깨짐
    if ( Result_EhcnatSystem == 3 )
    {
        // 깨질경우 Assist 아이템 초기화 한다.
        m_AssistitemUID = 0;
        m_pkAssistItemImg->ResetItem();

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT3 ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    // 초기화
    else if ( Result_EhcnatSystem == 5 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT5 ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );

    UpdateUIState();
}

void KGCEnchantSubEnchantPanel::OnEnchant()
{
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_OK ) );
}

void KGCEnchantSubEnchantPanel::OnEquip()
{
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_EQUIP_OK ) );
}

void KGCEnchantSubEnchantPanel::OnEquipSlot()
{
    m_nState = EM_STATE_EQUIP_SLOT;

    m_pkBtnEquipSlot->ToggleRender( false );
    m_pkSelEquipSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT ) );
}

void KGCEnchantSubEnchantPanel::OnEnchantSlot()
{
    m_nState = EM_STATE_ENCHANT_SLOT;

    m_pkBtnEnchatSlot->ToggleRender( false );
    m_pkSelEnchatSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_ENCHANT_SLOT ) );
}

void KGCEnchantSubEnchantPanel::OnAssistSlot()
{
    m_nState = EM_STATE_ASSIST_SLOT;

    m_pkBtnAssistSlot->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_ASSIST_SLOT ) );
}

void KGCEnchantSubEnchantPanel::OnEquipItemImg()
{
    InitUIState();

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubEnchantPanel::OnEnchantItemImg()
{
    m_nState = EM_STATE_NONE;

    m_EnchantitemUID = 0;

    m_pkBtnEnchatSlot->ToggleRender( true );
    m_pkSelEnchatSlot->ToggleRender( false );

    m_pkEnchantItemImg->ToggleRender(false);
    m_pkEnchantItemImg->ResetItem();

    m_pkBtnEnchant->Lock( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubEnchantPanel::OnAssistItemImg()
{
    m_nState = EM_STATE_NONE;

    m_AssistitemUID = 0;

    m_pkBtnAssistSlot->ToggleRender( true );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkAssistItemImg->ToggleRender(false);
    m_pkAssistItemImg->ResetItem();

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubEnchantPanel::OnInvenItemList( GCITEMUID _itemUID )
{
    KItem* kItem = g_pItemMgr->GetInventoryItem( _itemUID );
    if ( !kItem )
        return;

    switch( m_nState ) 
    {
    case EM_STATE_EQUIP_SLOT:
        {
            m_EquipitemUID = _itemUID;
            
            m_pkEquipItemImg->ToggleRender(true);
            m_pkEquipItemImg->SetItem( kItem );

            // 강화석 보유 여부 확인
            if ( kItem->m_EnchantEquipItemUID > 0 )
            {
                m_pkBtnEnchant->Lock( false );
                m_pkBtnEnchatSlot->Lock( true );
                m_pkBtnAssistSlot->Lock( false );                
            }
            else
            {
                m_pkBtnEnchatSlot->Lock( false );
                m_pkBtnAssistSlot->Lock( true );
            }
        }
        break;
    case EM_STATE_ENCHANT_SLOT:
        {
            m_EnchantitemUID = _itemUID;

            m_pkEnchantItemImg->ToggleRender(true);
            m_pkEnchantItemImg->SetItem( kItem );

            m_pkBtnEquip->ToggleRender( true );
            m_pkBtnEquip->Lock( false );
        }
        break;
    case EM_STATE_ASSIST_SLOT:
        {
            m_AssistitemUID = _itemUID;

            m_pkAssistItemImg->ToggleRender(true);
            m_pkAssistItemImg->SetItem( kItem );
        }
        break;
    }
}

bool KGCEnchantSubEnchantPanel::CheckItemInformation()
{
    if ( m_pkEquipItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkEquipItemImg->GetCurrentWindowPos() );
        return true;
    }

    if ( m_pkEnchantItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_EnchantitemUID );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkEnchantItemImg->GetCurrentWindowPos() );
        return true;
    }

    if ( m_pkAssistItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_AssistitemUID );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkAssistItemImg->GetCurrentWindowPos() );
        return true;
    }

    return false;
}

void KGCEnchantSubEnchantPanel::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
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