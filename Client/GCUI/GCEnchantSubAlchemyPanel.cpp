#include "stdafx.h"
#include "GCEnchantSubAlchemyPanel.h"

IMPLEMENT_CLASSNAME( KGCEnchantSubAlchemyPanel );
IMPLEMENT_WND_FACTORY( KGCEnchantSubAlchemyPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantSubAlchemyPanel, "gc_enchantsubalchemypanel" );

KGCEnchantSubAlchemyPanel::KGCEnchantSubAlchemyPanel( void )
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID1 = 0;
    m_EquipitemUID2 = 0;
    m_AssistitemUID = 0;
    m_ResultItemUID = 0;

    m_pkBtnAlchemy = NULL;

    m_pkBtnEquipSlot1 = NULL;
    m_pkBtnEquipSlot2 = NULL;
    m_pkBtnAssistSlot = NULL;

    m_pkSelEquipSlot1 = NULL;
    m_pkSelEquipSlot2 = NULL;
    m_pkSelAssistSlot = NULL;

    m_pkEquipItemImg1 = NULL;
    m_pkEquipItemImg2 = NULL;
    m_pkAssistItemImg = NULL;

    LINK_CONTROL( "btn_ok", m_pkBtnAlchemy );

    LINK_CONTROL( "equip_slot_btn1",    m_pkBtnEquipSlot1 );
    LINK_CONTROL( "equip_slot_btn2",    m_pkBtnEquipSlot2 );
    LINK_CONTROL( "assist_slot_btn",    m_pkBtnAssistSlot );

    LINK_CONTROL( "sel_equip_slot1",    m_pkSelEquipSlot1 );
    LINK_CONTROL( "sel_equip_slot2",    m_pkSelEquipSlot2 );
    LINK_CONTROL( "sel_assist_slot",    m_pkSelAssistSlot );


    LINK_CONTROL( "equip_slotitem_img1", m_pkEquipItemImg1 );
    LINK_CONTROL( "equip_slotitem_img2", m_pkEquipItemImg2 );
    LINK_CONTROL( "assist_slotitem_img", m_pkAssistItemImg );

}

KGCEnchantSubAlchemyPanel::~KGCEnchantSubAlchemyPanel( void )
{
}

void KGCEnchantSubAlchemyPanel::OnCreate( void )
{
    m_pkBtnAlchemy->InitState( true, true, this );

    m_pkBtnEquipSlot1->InitState( true, true, this );
    m_pkBtnEquipSlot2->InitState( true, true, this );
    m_pkBtnAssistSlot->InitState( true, true, this );

    m_pkSelEquipSlot1->InitState( false );
    m_pkSelEquipSlot2->InitState( false );
    m_pkSelAssistSlot->InitState( false );

    m_pkEquipItemImg1->InitState( true, true, this );
    m_pkEquipItemImg2->InitState( true, true, this );
    m_pkAssistItemImg->InitState( true, true, this );

    InitUIState();

}

void KGCEnchantSubAlchemyPanel::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnAlchemy, KD3DWnd::D3DWE_BUTTON_CLICK, OnAlchemy );

    GCWND_MSG_MAP( m_pkBtnEquipSlot1, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipSlot1 );
    GCWND_MSG_MAP( m_pkBtnEquipSlot2, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipSlot2 );
    GCWND_MSG_MAP( m_pkBtnAssistSlot, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistSlot );

    GCWND_MSG_MAP( m_pkEquipItemImg1, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipItemImg1 );
    GCWND_MSG_MAP( m_pkEquipItemImg2, KD3DWnd::D3DWE_BUTTON_CLICK, OnEquipItemImg2 );
    GCWND_MSG_MAP( m_pkAssistItemImg, KD3DWnd::D3DWE_BUTTON_CLICK, OnAssistItemImg );
}


void KGCEnchantSubAlchemyPanel::InitUIState()
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID1 = 0;
    m_EquipitemUID2 = 0;
    m_AssistitemUID = 0;

    m_pkBtnAlchemy->ToggleRender( true );
    m_pkBtnAlchemy->Lock( true );

    m_pkBtnEquipSlot1->ToggleRender( true );
    m_pkBtnEquipSlot2->ToggleRender( true );
    m_pkBtnAssistSlot->ToggleRender( true );

    m_pkBtnEquipSlot1->Lock( false );
    m_pkBtnEquipSlot2->Lock( false );
    m_pkBtnAssistSlot->Lock( true );

    m_pkSelEquipSlot1->ToggleRender( false );
    m_pkSelEquipSlot2->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkSelEquipSlot1->Lock( false );
    m_pkSelEquipSlot2->Lock( false );
    m_pkSelAssistSlot->Lock( false );

    m_pkEquipItemImg1->ToggleRender( false );
    m_pkEquipItemImg2->ToggleRender( false );
    m_pkAssistItemImg->ToggleRender( false );

    m_pkEquipItemImg1->Lock( false );
    m_pkEquipItemImg2->Lock( false );
    m_pkAssistItemImg->Lock( false );
}

void KGCEnchantSubAlchemyPanel::UpdateUIState()
{
    bool bEquipItem1 = ( m_EquipitemUID1 != 0 );
    bool bEquipItem2 = ( m_EquipitemUID2 != 0 );
    bool bAssistItem = ( m_AssistitemUID != 0 );

    m_pkBtnAlchemy->ToggleRender( true );

    m_pkBtnEquipSlot1->ToggleRender( !bEquipItem1 );
    m_pkSelEquipSlot1->ToggleRender( bEquipItem1 );
    m_pkEquipItemImg1->ToggleRender( bEquipItem1 );


    m_pkBtnEquipSlot2->ToggleRender( !bEquipItem2 );
    m_pkSelEquipSlot2->ToggleRender( bEquipItem2 );    
    m_pkEquipItemImg2->ToggleRender( bEquipItem2 );   

    
    m_pkBtnAssistSlot->ToggleRender( !bAssistItem );
    m_pkSelAssistSlot->ToggleRender( bAssistItem );
    m_pkAssistItemImg->ToggleRender( bAssistItem );
    

}

void KGCEnchantSubAlchemyPanel::LockUI( bool bLock )
{
    if ( bLock )
    {
        m_pkBtnAlchemy->Lock( true );

        m_pkBtnEquipSlot1->Lock( true );
        m_pkBtnEquipSlot2->Lock( true );
        m_pkBtnAssistSlot->Lock( true );

        m_pkEquipItemImg1->Lock( true );
        m_pkEquipItemImg2->Lock( true );
        m_pkAssistItemImg->Lock( true );
    }
    else
    {
        if ( m_EquipitemUID1 != 0 && m_EquipitemUID2 != 0 )
        {
            m_pkBtnAssistSlot->Lock( false );

            if ( m_AssistitemUID != 0 )
            {
                m_pkBtnAlchemy->Lock( false );
            }
        }

        m_pkBtnEquipSlot1->Lock( false );
        m_pkBtnEquipSlot2->Lock( false );

        m_pkEquipItemImg1->Lock( false );
        m_pkEquipItemImg2->Lock( false );
        m_pkAssistItemImg->Lock( false );
    }

}

void KGCEnchantSubAlchemyPanel::SetResult()
{
    // 노멀등급 이면
    KItem* kItem = g_pItemMgr->GetInventoryItem( m_ResultItemUID );
    if ( kItem )
    {
        if( kItem->m_cGradeID == KItem::GRADE_NORMAL  )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX, 
                g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_SYSTEM_ALCHEMY_NORMAL, "i", kItem->m_cEnchantLevel), 
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, kItem->m_ItemID * 10, m_ResultItemUID, false, true );
        }
        else
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX, 
                g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_SYSTEM_ALCHEMY_EPIC, "i", kItem->m_cEnchantLevel), 
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, kItem->m_ItemID * 10, m_ResultItemUID, false, true );
        }
    }


    m_EquipitemUID1 = 0;
    m_pkEquipItemImg1->ResetItem();

    m_EquipitemUID2 = 0;
    m_pkEquipItemImg2->ResetItem();

    m_AssistitemUID = 0;
    m_pkAssistItemImg->ResetItem();

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
    
    UpdateUIState();
}

void KGCEnchantSubAlchemyPanel::OnAlchemy()
{
    m_ResultItemUID = 0;
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_OK ) );
}

void KGCEnchantSubAlchemyPanel::OnEquipSlot1()
{
    m_nState = EM_STATE_EQUIP_SLOT1;

    m_pkBtnEquipSlot1->ToggleRender( false );
    m_pkSelEquipSlot1->ToggleRender( true );

    if ( m_EquipitemUID2 == 0 )
    {
        m_pkBtnEquipSlot2->ToggleRender( true );
        m_pkSelEquipSlot2->ToggleRender( false );
    }

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT ) );
}

void KGCEnchantSubAlchemyPanel::OnEquipSlot2()
{
    m_nState = EM_STATE_EQUIP_SLOT2;

    m_pkBtnEquipSlot2->ToggleRender( false );
    m_pkSelEquipSlot2->ToggleRender( true );

    if ( m_EquipitemUID1 == 0 )
    {
        m_pkBtnEquipSlot1->ToggleRender( true );
        m_pkSelEquipSlot1->ToggleRender( false );
    }

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_EQUIP_SLOT ) );
}

void KGCEnchantSubAlchemyPanel::OnAssistSlot()
{
    m_nState = EM_STATE_ASSIST_SLOT;

    m_pkBtnAssistSlot->ToggleRender( false );
    m_pkSelAssistSlot->ToggleRender( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SEL_ENCHANT_ASSIST_SLOT ) );
}

void KGCEnchantSubAlchemyPanel::OnEquipItemImg1()
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID1 = 0;

    m_pkBtnEquipSlot1->ToggleRender( true );
    m_pkSelEquipSlot1->ToggleRender( false );

    m_pkEquipItemImg1->ToggleRender(false);
    m_pkEquipItemImg1->ResetItem();

    if ( m_EquipitemUID2 == 0 )
    {
        m_pkBtnEquipSlot2->ToggleRender( true );
        m_pkSelEquipSlot2->ToggleRender( false );
    }

    m_AssistitemUID = 0;

    m_pkBtnAssistSlot->ToggleRender( true );
    m_pkSelAssistSlot->ToggleRender( false );
    m_pkBtnAssistSlot->Lock( true );

    m_pkAssistItemImg->ToggleRender(false);
    m_pkAssistItemImg->ResetItem();

    m_pkBtnAlchemy->Lock( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubAlchemyPanel::OnEquipItemImg2()
{
    m_nState = EM_STATE_NONE;

    m_EquipitemUID2 = 0;

    m_pkBtnEquipSlot2->ToggleRender( true );
    m_pkSelEquipSlot2->ToggleRender( false );

    m_pkEquipItemImg2->ToggleRender(false);
    m_pkEquipItemImg2->ResetItem();

    if ( m_EquipitemUID1 == 0 )
    {
        m_pkBtnEquipSlot1->ToggleRender( true );
        m_pkSelEquipSlot1->ToggleRender( false );
    }


    m_AssistitemUID = 0;

    m_pkBtnAssistSlot->ToggleRender( true );
    m_pkSelAssistSlot->ToggleRender( false );
    m_pkBtnAssistSlot->Lock( true );

    m_pkAssistItemImg->ToggleRender(false);
    m_pkAssistItemImg->ResetItem();

    m_pkBtnAlchemy->Lock( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubAlchemyPanel::OnAssistItemImg()
{
    m_nState = EM_STATE_NONE;

    m_AssistitemUID = 0;

    m_pkBtnAssistSlot->ToggleRender( true );
    m_pkSelAssistSlot->ToggleRender( false );

    m_pkAssistItemImg->ToggleRender(false);
    m_pkAssistItemImg->ResetItem();

    m_pkBtnAlchemy->Lock( true );

    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_ENCHANT_CLEAR_INVEN ) );
}

void KGCEnchantSubAlchemyPanel::OnInvenItemList( GCITEMUID _itemUID )
{
    KItem* kItem = g_pItemMgr->GetInventoryItem( _itemUID );
    if ( !kItem )
        return;

    switch( m_nState ) 
    {
    case EM_STATE_EQUIP_SLOT1:
        {
            m_EquipitemUID1 = _itemUID;

            m_pkEquipItemImg1->ToggleRender(true);
            m_pkEquipItemImg1->SetItem( kItem );

            if ( m_EquipitemUID1 != 0 && m_EquipitemUID2 != 0 )
            {
                m_pkBtnAssistSlot->Lock( false );
            }
        }
        break;
    case EM_STATE_EQUIP_SLOT2:
        {
            m_EquipitemUID2 = _itemUID;

            m_pkEquipItemImg2->ToggleRender(true);
            m_pkEquipItemImg2->SetItem( kItem );

            if ( m_EquipitemUID1 != 0 && m_EquipitemUID2 != 0 )
            {
                m_pkBtnAssistSlot->Lock( false );
            }
        }
        break;
    case EM_STATE_ASSIST_SLOT:
        {
            m_AssistitemUID = _itemUID;

            m_pkAssistItemImg->ToggleRender(true);
            m_pkAssistItemImg->SetItem( kItem );

            m_pkBtnAlchemy->Lock( false );
        }
        break;
    }
}

bool KGCEnchantSubAlchemyPanel::IsEquipItem( GCITEMUID _EquipitemUID )
{
    if ( m_EquipitemUID1 == _EquipitemUID || m_EquipitemUID2 == _EquipitemUID )
        return true;

    return false;
}


bool KGCEnchantSubAlchemyPanel::CheckItemInformation()
{

    if ( m_pkEquipItemImg1->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID1 );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkEquipItemImg1->GetCurrentWindowPos() );
        return true;
    }

    if ( m_pkEquipItemImg2->CheckMousePosInWindowBound() )
    {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( m_EquipitemUID2 );
        if ( !pkItem )
            return false;

        DrawItemInfo( pkItem, m_pkEquipItemImg2->GetCurrentWindowPos() );
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

void KGCEnchantSubAlchemyPanel::DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos)
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