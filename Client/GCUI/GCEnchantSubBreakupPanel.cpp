#include "stdafx.h"
#include "GCEnchantSubBreakupPanel.h"

IMPLEMENT_CLASSNAME( KGCEnchantSubBreakupPanel );
IMPLEMENT_WND_FACTORY( KGCEnchantSubBreakupPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantSubBreakupPanel, "gc_enchantsubbreakuppanel" );

KGCEnchantSubBreakupPanel::KGCEnchantSubBreakupPanel( void )
{
    m_EquipitemUID = 0;
    m_AssistitemUID = 0;

    m_nState = EM_STATE_NONE;

    m_pkBtnBreakUP = NULL;

    m_pkBtnEquipSlot = NULL;
    m_pkBtnAssistSlot = NULL;

    m_pkSelEquipSlot = NULL;
    m_pkSelAssistSlot = NULL;

    m_pkEquipItemImg = NULL;
    m_pkAssistItemImg = NULL;


    LINK_CONTROL( "btn_ok", m_pkBtnBreakUP );

    LINK_CONTROL( "equip_slot_btn",     m_pkBtnEquipSlot );
    LINK_CONTROL( "assist_slot_btn",    m_pkBtnAssistSlot );

    LINK_CONTROL( "sel_equip_slot",     m_pkSelEquipSlot );
    LINK_CONTROL( "sel_assist_slot",    m_pkSelAssistSlot );

    LINK_CONTROL( "equip_slotitem_img", m_pkEquipItemImg );
    LINK_CONTROL( "assist_slotitem_img", m_pkAssistItemImg );

}

KGCEnchantSubBreakupPanel::~KGCEnchantSubBreakupPanel( void )
{
}

void KGCEnchantSubBreakupPanel::OnCreate( void )
{
    m_pkBtnBreakUP->InitState( true, true, this );

    m_pkBtnEquipSlot->InitState( true, true, this );
    m_pkBtnAssistSlot->InitState( true, true, this );

    m_pkSelEquipSlot->InitState( false );
    m_pkSelAssistSlot->InitState( false );

    m_pkEquipItemImg->InitState( true, true, this );
    m_pkAssistItemImg->InitState( true, true, this );

    InitUIState();
}

void KGCEnchantSubBreakupPanel::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnBreakUP, KD3DWnd::D3DWE_BUTTON_CLICK, OnBreakUP );

    GCWND_MSG_MAP( m_pkBtnEquipSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipSlot );
    GCWND_MSG_MAP( m_pkBtnAssistSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistSlot );

    GCWND_MSG_MAP( m_pkEquipItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipItemImg );
    GCWND_MSG_MAP( m_pkAssistItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistItemImg );
}

void KGCEnchantSubBreakupPanel::InitUIState()
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID = 0;
    m_AssistitemUID = 0;


    m_pkBtnBreakUP->ToggleRender( true );
    m_pkBtnBreakUP->Lock( true );

    m_pkBtnEquipSlot->ToggleRender( true );
    m_pkBtnAssistSlot->ToggleRender( true );

    m_pkBtnEquipSlot->Lock( false );
    m_pkBtnAssistSlot->Lock( true );

    m_pkSelEquipSlot->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkSelEquipSlot->Lock( false );
    m_pkSelAssistSlot->Lock( false );

    m_pkEquipItemImg->ToggleRender( false );
    m_pkAssistItemImg->ToggleRender( false );

    m_pkEquipItemImg->Lock( false );
    m_pkAssistItemImg->Lock( false );
}

void KGCEnchantSubBreakupPanel::UpdateUIState()
{
    bool bEquipItem = ( m_EquipitemUID != 0 );
    bool bAssistItem = ( m_AssistitemUID != 0 );

    m_pkBtnBreakUP->ToggleRender( true );

    m_pkBtnEquipSlot->ToggleRender( !bEquipItem );
    m_pkSelEquipSlot->ToggleRender( bEquipItem );
    m_pkEquipItemImg->ToggleRender( bEquipItem );

    m_pkBtnAssistSlot->ToggleRender( !bAssistItem );
    m_pkSelAssistSlot->ToggleRender( bAssistItem );
    m_pkAssistItemImg->ToggleRender( bAssistItem );
}

void KGCEnchantSubBreakupPanel::LockUI( bool bLock )
{
    if ( bLock )
    {
        m_pkBtnBreakUP->Lock( true );

        m_pkBtnEquipSlot->Lock( true );
        m_pkBtnAssistSlot->Lock( true );

        m_pkEquipItemImg->Lock( true );
        m_pkAssistItemImg->Lock( true );
    }
    else
    {
        if ( m_EquipitemUID != 0 )
        {
            m_pkBtnAssistSlot->Lock( false );

            if ( m_AssistitemUID != 0 )
            {
                m_pkBtnBreakUP->Lock( false );
            }
        }

        m_pkBtnEquipSlot->Lock( false );
        m_pkEquipItemImg->Lock( false );
        m_pkAssistItemImg->Lock( false );
    }
}

void KGCEnchantSubBreakupPanel::SetResult()
{
    // 성공
    if ( Result_EhcnatSystem == 1 )
    {
        m_EquipitemUID = 0;
        m_pkEquipItemImg->ResetItem();
        
        m_AssistitemUID = 0;
        m_pkAssistItemImg->ResetItem();

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_SUCCESS_BREAKUP ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    // 실패인데 안깨짐
    else if ( Result_EhcnatSystem == 4 )
    {

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

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_FAILED_BREAKUP ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    else // 실패
    {
        m_EquipitemUID = 0;
        m_pkEquipItemImg->ResetItem();

        m_AssistitemUID = 0;
        m_pkAssistItemImg->ResetItem();

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENCHANT_SYSTEM_BREAK_BROKEN ), L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );

    UpdateUIState();
}

void KGCEnchantSubBreakupPanel::OnBreakUP()
{
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_OK ) );
}

void KGCEnchantSubBreakupPanel::OnEquipSlot()
{
    m_nState = EM_STATE_EQUIP_SLOT;

    m_pkBtnEquipSlot->ToggleRender( false );
    m_pkSelEquipSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT ) );
}

void KGCEnchantSubBreakupPanel::OnAssistSlot()
{
    m_nState = EM_STATE_ASSIST_SLOT;

    m_pkBtnAssistSlot->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_ASSIST_SLOT ) );
}

void KGCEnchantSubBreakupPanel::OnEquipItemImg()
{
    InitUIState();

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubBreakupPanel::OnAssistItemImg()
{
    m_nState = EM_STATE_NONE;

    m_AssistitemUID = 0;

    m_pkBtnAssistSlot->ToggleRender( true );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkAssistItemImg->ToggleRender(false);
    m_pkAssistItemImg->ResetItem();

    m_pkBtnBreakUP->Lock( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubBreakupPanel::OnInvenItemList( GCITEMUID _itemUID )
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

            m_pkBtnAssistSlot->Lock( false );
        }
        break;
    case EM_STATE_ASSIST_SLOT:
        {
            m_AssistitemUID = _itemUID;

            m_pkAssistItemImg->ToggleRender(true);
            m_pkAssistItemImg->SetItem( kItem );

            m_pkBtnBreakUP->Lock( false );
        }
        break;
    }
}

bool KGCEnchantSubBreakupPanel::CheckItemInformation()
{
    if ( m_pkEquipItemImg->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkEquipItemImg->GetCurrentWindowPos() );
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

void KGCEnchantSubBreakupPanel::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
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