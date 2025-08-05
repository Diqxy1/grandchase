#include "stdafx.h"
//
#include "GCItemBar.h"
//

//
//
//
//
#include "../MyD3D.h"


#include "../Procedure.h"
//



#include "GCUserItemWnd.h"
#include "GCMyInfoScene.h"
//
#include "mission/GCMissionManager.h"

//
#include "KNeonSign.h"
#include "GCNeonsignSelectBox.h"
#include "KGCEmoticon.h"
#include "../GCStateMachine.h"
#include "GCCoupleSystem.h"
#include "GCGawibawiboGame.h"
#include "GCAttributeSelectBox.h"
#include "GCGawibawiboGame.h"
#include "GCPlantTree.h"
#include "GCLevelResetEventManager.h"
#include "../GCForgeManager.h"
#include "KGCDepotManager.h"
#include "GCPopupAttributeSelect.h"
#include "GCSuperGachaDlg.h"
#include "GCPetMagicStone.h"

#define MAX_SATIATION 6000

IMPLEMENT_CLASSNAME( KGCUserItemWnd );
IMPLEMENT_WND_FACTORY( KGCUserItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCUserItemWnd, "gc_user_item_wnd" );

KGCUserItemWnd::KGCUserItemWnd( void )
:	m_pkRandomInitAttributeButton(NULL)
{
    m_iItemID           = -1;    
    m_iStrong           = 0;    
    m_dwUID         = 0;
    m_bSellState        = false;
    m_bSelectState      = false;

    m_bUnique            = false;
    m_bShow             = false;
    m_bEnchant          = false;
    m_bEnchantBreak     = false;
    m_bPeriodItem        = false;
    m_bEvent            = false;
    m_bPointItem        = false;
    m_bNotSellItem      = false;

    m_pkDeleteButton    = NULL;
    m_pkSellButton      = NULL;
    m_pkEnchantButton   = NULL;
    m_pkConfirmButton   = NULL;
    m_pkEatFoodButton    = NULL;
    m_pkAttributeButton = NULL;
    m_pkCraftingButton = NULL;

    m_pkLevelDownButton = NULL;
    m_pkLevelDownOver = NULL;

    m_pkSocketButton = NULL;

    m_pkItemName    = NULL;
    m_pkRemainTime  = NULL;
    m_pkItemLimit   = NULL;
    m_pkItemBar     = NULL;

    m_pkPlayButton    = NULL;

    m_pkFrame = NULL;
    m_pkActiveFrame = NULL;
    m_pkDurationNum = NULL;
    m_pkSmallDurationNum = NULL;
    m_pkStrongNum = NULL;
    m_pkPriceNum = NULL;
    m_pkLimitIcon = NULL;
    m_pkPocketIcon = NULL;
    m_pkNowSelling = NULL;
    m_pkNowRemove = NULL;
    m_pkCantSell = NULL;
    m_pkTimeRed = NULL;
    m_pkTimeYellow = NULL;
    m_pkTimeGreen = NULL;
    m_pkNowEquip = NULL;
    m_pkSelected = NULL;
    m_pkEquipEnchantItemLv = NULL;

    m_pkImgCoordiCompose = NULL;
	m_pkBtnEquip = NULL;

    m_pkImgCoordiUndo = NULL;

    for( int i = 0 ; i < KItem::GRADE_NUM ; ++i ) {
        m_pkGradeFrame[i] = NULL;
        LINK_CONTROL_STM( "item_frame_grade"<<i, m_pkGradeFrame[i] );
    }

    LINK_CONTROL( "item_frame",         m_pkFrame );
    LINK_CONTROL( "item_active_frame",  m_pkActiveFrame );


    LINK_CONTROL( "deletel_button",     m_pkDeleteButton );
    LINK_CONTROL( "sell_button",        m_pkSellButton );
    LINK_CONTROL( "enchant_button",     m_pkEnchantButton );
    LINK_CONTROL( "confirm_button",     m_pkConfirmButton );
    LINK_CONTROL( "eat_food_button",    m_pkEatFoodButton );
    LINK_CONTROL( "play_button",        m_pkPlayButton );
    LINK_CONTROL( "socket_button",      m_pkSocketButton );
    LINK_CONTROL( "attribute_button",   m_pkAttributeButton );
    LINK_CONTROL( "init_attribute_button",   m_pkRandomInitAttributeButton );
    LINK_CONTROL( "crafting_button",   m_pkCraftingButton );
    

    LINK_CONTROL( "level_down_button",   m_pkLevelDownButton );
    LINK_CONTROL( "level_down_over",   m_pkLevelDownOver );

    LINK_CONTROL( "item_name_static",   m_pkItemName );
    LINK_CONTROL( "item_limit_static",  m_pkItemLimit );
    LINK_CONTROL( "item_bottom_bar",    m_pkItemBar );
    LINK_CONTROL( "remain_time",        m_pkRemainTime );

    LINK_CONTROL( "number_equip_enchant_item",    m_pkEquipEnchantItemLv );
    LINK_CONTROL( "number_duration",    m_pkDurationNum );
    LINK_CONTROL( "number_duration_small", m_pkSmallDurationNum );
    LINK_CONTROL( "number_strong",      m_pkStrongNum );
    LINK_CONTROL( "number_price",       m_pkPriceNum );
    LINK_CONTROL( "icon_limit",         m_pkLimitIcon );
    LINK_CONTROL( "icon_pocket",        m_pkPocketIcon );
    LINK_CONTROL( "icon_nowsell",       m_pkNowSelling );
    LINK_CONTROL( "icon_nowremove",     m_pkNowRemove );
    LINK_CONTROL( "icon_cantsell",      m_pkCantSell );
    LINK_CONTROL( "icon_nowequip",      m_pkNowEquip );
    LINK_CONTROL( "icon_selected",      m_pkSelected );
    

    LINK_CONTROL( "icon_timered", m_pkTimeRed );
    LINK_CONTROL( "icon_timeyellow", m_pkTimeYellow );
    LINK_CONTROL( "icon_timegreen", m_pkTimeGreen );

    LINK_CONTROL( "img_coordi_compose", m_pkImgCoordiCompose );

	LINK_CONTROL ( "equip_button", m_pkBtnEquip );
    LINK_CONTROL ( "unmerge_button", m_pkImgCoordiUndo );
}
    
KGCUserItemWnd::~KGCUserItemWnd( void )
{
}

void KGCUserItemWnd::OnCreate( void )
{
    /*for ( UINT i = 0; i < NUM_LAYER_WND; ++i )
    {
		m_apLayer[i]->SetSelfInputCheck( false );
    }*/

    if( m_pkFrame ) {
        m_pkFrame->SetPredraw( true );
        m_pkFrame->InitState( true );
    }

    if( m_pkActiveFrame ) {
        m_pkActiveFrame->InitState( false );
    }

    if( m_pkDeleteButton ) {
        m_pkDeleteButton->InitState( false, true, this );
        m_pkDeleteButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkSellButton ) {
        m_pkSellButton->SetSelfInputCheck( true );
        m_pkSellButton->AddActionListener( this );
        m_pkSellButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkEnchantButton ) {
        m_pkEnchantButton->SetSelfInputCheck( true );
        m_pkEnchantButton->ToggleRender( false );
        m_pkEnchantButton->AddActionListener( this );
        m_pkEnchantButton->SetIgnoreOffenEvent( true );
    }

    //�̺�Ʈ�� Ȯ�ι�ư
    if( m_pkConfirmButton ) {
        m_pkConfirmButton->SetSelfInputCheck( true );
        m_pkConfirmButton->ToggleRender( true );
        m_pkConfirmButton->AddActionListener( this );
        m_pkConfirmButton->SetIgnoreOffenEvent( true );
		m_pkConfirmButton->SetWndMode(D3DWM_DEFAULT);
    }

    if( m_pkEatFoodButton ) {

        // �� ���� ���̱� ��ư
        m_pkEatFoodButton->SetSelfInputCheck( true );
        m_pkEatFoodButton->ToggleRender( true );
        m_pkEatFoodButton->AddActionListener( this );
        m_pkEatFoodButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkCraftingButton )
    {
        m_pkCraftingButton->SetSelfInputCheck( true );
        m_pkCraftingButton->ToggleRender( true );
        m_pkCraftingButton->AddActionListener( this );
        m_pkCraftingButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkItemName != NULL ) {
        m_pkItemName->SetSelfInputCheck( false );
        m_pkItemName->SetOffset( D3DXVECTOR2( 0.0f, 1.0f ) );
#if defined(NO_USE_WINDOW_MODE)
        m_pkItemName->ToggleRender(false);
        m_pkItemLimit->SetWindowPos(m_pkItemName->GetCurrentWindowLocalPos());
#endif
    }

    if( m_pkItemLimit != NULL ) {
        m_pkItemLimit->SetSelfInputCheck( false );
        m_pkItemLimit->SetFontColor( 0xffff0000 );
    }

    if( m_pkPlayButton ) {
        m_pkPlayButton->InitState( false, true, this );
        m_pkPlayButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkSocketButton ) {
        m_pkSocketButton->InitState( false, true, this );
        m_pkSocketButton->SetIgnoreOffenEvent( true );
    }

    if( m_pkAttributeButton ) {
        m_pkAttributeButton->InitState( false, true, this );
        m_pkAttributeButton->SetIgnoreOffenEvent( true );
    }

    if ( m_pkLevelDownButton )
    {
        m_pkLevelDownButton->InitState( false, true, this );
        m_pkLevelDownButton->SetIgnoreOffenEvent( true );
        m_pkLevelDownOver->InitState( false, true, this );
    }

    if( m_pkItemImg ) {
        m_pkItemImg->InitState( true );
    }

    if( m_pkDurationNum != NULL ) {
        m_pkDurationNum->InitState( true );
        m_pkDurationNum->SetStringType( "small_num" );
    }

    if( m_pkSmallDurationNum ) {
        m_pkSmallDurationNum->InitState( true );
        m_pkSmallDurationNum->SetStringType( "very_small_num" );
    }

    if( m_pkStrongNum != NULL ) {
        m_pkStrongNum->InitState( true );
        m_pkStrongNum->SetStringType( "yellow_num" );
        m_pkStrongNum->SetWriteToRight( false );
    }

    if( m_pkPriceNum != NULL ) {
        m_pkPriceNum->InitState( true );
        m_pkPriceNum->SetStringType( "very_small_num" );
    }

    if( m_pkEquipEnchantItemLv != NULL ) {
        m_pkEquipEnchantItemLv->InitState( true );
        m_pkEquipEnchantItemLv->SetStringType( "normal_num_enchant" );
        m_pkEquipEnchantItemLv->SetWriteToRight(false);
    }

    if( m_pkLimitIcon != NULL ) {
        m_pkLimitIcon->InitState( false );
    }

    if( m_pkPocketIcon != NULL ) {
        m_pkPocketIcon->InitState( false );
    }

    if( m_pkNowSelling != NULL ) {
        m_pkNowSelling->InitState( false );
    }

    if( m_pkNowRemove != NULL ) {
        m_pkNowRemove->InitState( false );
    }

    if( m_pkCantSell != NULL ) {
        m_pkCantSell->InitState( false );
    }

    if( m_pkNowEquip )
        m_pkNowEquip->InitState( false );

    if( m_pkSelected )
        m_pkSelected->InitState( false );

    if( m_pkTimeGreen )
        m_pkTimeGreen->InitState( false );

    if( m_pkTimeYellow )
        m_pkTimeYellow->InitState( false );

    if( m_pkTimeRed )
        m_pkTimeRed->InitState( false );

    if(m_pkRandomInitAttributeButton)
        m_pkRandomInitAttributeButton->InitState(false,true,this);

    if ( m_pkImgCoordiCompose )
        m_pkImgCoordiCompose->InitState( false );
	
	if( m_pkBtnEquip ) {
		m_pkBtnEquip->InitState( false, true, this );
	}

    if (m_pkImgCoordiUndo) {
        m_pkImgCoordiUndo->SetSelfInputCheck(true);
        m_pkImgCoordiUndo->ToggleRender(true);
        m_pkImgCoordiUndo->AddActionListener(this);
        m_pkImgCoordiUndo->SetIgnoreOffenEvent(true);
    }
}

void KGCUserItemWnd::FrameMoveInEnabledState( void )
{
}

void KGCUserItemWnd::PostDraw( void )
{
}

void KGCUserItemWnd::OnConfirmButton()
{
    if( m_pkConfirmButton == NULL )
        return;

#if defined (RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM)
    GCItem* kItem = NULL;
    kItem = g_pItemMgr->GetItemData( m_iItemID );

    if ( kItem->eItemKind == GIK_TREASUREBOX_ITEM ) 
    {
        int iCharType = g_pItemMgr->GetUniqueChartype( kItem->dwCharType);
        int nEquipLevel = kItem->iNeedLevelForEquippingItem;
        //��������� ���
        if (ECT_ALL == kItem->dwCharType)
        {
            if ( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel < nEquipLevel ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_REQUIRE_HIGH_LEVEL ) );
                return;
            }
        }
    }
#endif
    if( g_pkUIScene->m_pkChoiceDrop != NULL )
    {
        if( g_pkUIScene->m_pkChoiceDrop->IsChoiceDropItem( m_iItemID ) ) 
		{
            g_pkUIScene->m_pkChoiceDrop->SetBoxID( (GCITEMID&)m_iItemID );
            KP2P::GetInstance()->Send_ChoiceBoxInfoReq( m_iItemID * 10 );
            return;
        }
    } 

    switch( m_iItemID )
    {
    case KGCItemManager::ITEM_MAGICBOX_GAME_BOARD:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MAGIC_BOX );
            return;
        }
    case KGCItemManager::ITEM_CHANGE_NICK_NAME:
        {
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_NICK_IN_ROOM1),
                    g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_NICK_IN_ROOM2) );
                return;
            }
            g_pkUIScene->m_pkNickNameBox->SetNickRegComplete(false);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME, (DWORD)m_iItemID, m_dwUID, true );
            return;
        }
    case ITEM_VIP_TEMPORARY_ITEM:
        {
            if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_COLOR_IN_ROOM),
                    g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_NICK_IN_ROOM2));
                return;
            }
            g_kGlobalValue.NickColorChangeItemID = m_iItemID;
            g_kGlobalValue.NickColorChangeItemUID = m_dwUID;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NICKNAME_COLOR_DLG);
            return;
        }
    case KGCItemManager::ITEM_CHANGE_PET_NICKNAME_CARD:
        {
            // �� �ȿ����� �� �г��� ����ī�带 ��� �� �� ����
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_NICK_IN_ROOM1),
                    g_pkStrLoader->GetString(STR_ID_CAN_NOT_CHANGE_NICK_IN_ROOM2) );
                return;
            }

            // ���� ���� �����ϰ� ���� �ʴٸ� �г��� ����ī�带 ��� �� �� ����
            if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwUID == 0 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_EQUIP_PET ) );
                return;
            }

            // �����ϰ� �ִ� ���� �� �̶�� �г��� ������ �� �� ����
            // HatchingID �� -1 �̸� �Ϲ� ��.. -1�� �ƴϰ� ���� �ٸ� ���� ����Ű�� ������.. 
            if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_nHatchingID != -1 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANNOT_CHANGE_PET_NAME ));
                return;
            }

            // �߷�(PC�� ��)�� �г����� ������ �� ����.
            if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwUID <= KGCItemManager::MAXIMUM_PCBANG_UID )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANNOT_CHANGE_BEEROPET_NAME ));
                return;
            }

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME, (DWORD)m_iItemID, m_dwUID, true );
            return;
        }
    case KGCItemManager::ITEM_TREASURE_MINGMING_BOX:
    case KGCItemManager::ITEM_SEASON2_CONGRATULATION_BOX:
    case KGCItemManager::ITEM_EASTER_EVENT_TREASURE_BOX:
    case KGCItemManager::ITEM_FORTUNE_CAPSULE_ELA:
    case KGCItemManager::ITEM_FORTUNE_CAPSULE_LRR:
    case KGCItemManager::ITEM_SEVEN_CIRCLE_TREASURE_BOX:
    case KGCItemManager::ITEM_QUEENS_BOX:
    case KGCItemManager::ITEM_PH_CBT_BOX:
        {
            Result_Kid_Gift = -100;

            if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
                return;
            }
            KP2P::GetInstance()->Send_ChangeItemReq( m_dwUID );
            g_MyD3D->WaitForServerAck( Result_Kid_Gift, -100, 15000, -99 );

            if( Result_Kid_Gift == -99 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
            }
            return;
        }
        // 2007.03.15 wony
        // �κ��丮 �뷮�� �ø��� �������Դϴ�
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_10:
    case KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_60:
        {
            // �κ��丮 ũ�Ⱑ 400�̻��̸� ���̻� �κ��丮 Ȯ�� �������� ��� �� �� ����
            int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
            if( iInventoryCapacity >= MAX_INVENTORY_SIZE )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetReplacedString( STR_ID_EXTEND_INVENTORY_MESSAGE5, "i", MAX_INVENTORY_SIZE ), L"" );

                return;
            }

            int iExtendInventorySize = 0;
            if( m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60 || 
                m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_60 )
            {
                iExtendInventorySize = EXTEND_INVENTORY_SIZE_60;
            }
            else if( m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30 )
            {
                iExtendInventorySize = EXTEND_INVENTORY_SIZE_30;
            }
            else if( m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10 ||
                m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2 || 
                m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3 || 
                m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_10 )
            {
                iExtendInventorySize = EXTEND_INVENTORY_SIZE_10;
            }
            else if( m_iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5 )
            {
                iExtendInventorySize = EXTEND_INVENTORY_SIZE_5;
            }

            std::wstring str1 = g_pkStrLoader->GetReplacedString( STR_ID_EXTEND_INVENTORY_MESSAGE1, "i", (int)g_kGlobalValue.m_iInvenCapacity );
            std::wstring str2 = g_pkStrLoader->GetReplacedString( STR_ID_EXTEND_INVENTORY_MESSAGE2, "i", iExtendInventorySize );

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXTEND_INVENTORY, str1, str2,
                KGCUIScene::GC_MBOX_USE_EXTEND_INVENTORY, m_dwUID, m_iItemID );

            return;
        }
    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH5000: // �� ��ȭ ������
    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH3000:
    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_POINT:
    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_GP:
        {
            // ��ȿ����� ��ȭ�� �� �� ����
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_USE_EVOLUTION_ITEM ) );
                return;
            }

            // �켱 �������� �ִ��� �˾ƺ���.. ( �Ϲ���, ���� =��= )
            if( g_MyD3D->m_mapPetInfo.size() > 0 )
            {
                std::map<GCITEMUID, KPetInfo>::iterator mit;
                for( mit = g_MyD3D->m_mapPetInfo.begin(); mit != g_MyD3D->m_mapPetInfo.end(); ++mit )
                {
                    // �Ϲ����� �ϳ��� ������.. + �� �κ��丮�� �ִ� �������̸�
                    if( mit->second.m_nHatchingID == -1 && g_pItemMgr->FindInventory(mit->second.m_dwUID) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PET_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_PET_EVOLUTION_BOX, m_iItemID, mit->second.m_dwUID );
                        return;
                    }
                }
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE1 ), L"" );
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE1 ), L"" );
            }

            return;
        }
    case KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_POINT: // �� ��ȭ ������
    case KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_GP:
        {
            // ��ȿ����� ��ȭ�� �� �� ����
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_USE_DEVOLUTION_ITEM ) );
                return;
            }

            // �켱 �������� �ִ��� �˾ƺ���.. ( �Ϲ���, ���� =��= )
            if( g_MyD3D->m_mapPetInfo.size() > 0 )
            {
                std::map<GCITEMUID, KPetInfo>::iterator mit;
                for( mit = g_MyD3D->m_mapPetInfo.begin(); mit != g_MyD3D->m_mapPetInfo.end(); ++mit )
                {
                    // �Ϲ����� �ϳ��� ������.. + �� �κ��丮�� �ִ� �������̸�
                    if( mit->second.m_nHatchingID == -1 && g_pItemMgr->FindInventory(mit->second.m_dwUID) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PET_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_PET_DEVOLUTION_BOX, m_iItemID, mit->second.m_dwUID );
                        return;
                    }
                }
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE2 ), L"" );
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE2 ), L"" );
            }

            return;
        }
    case KGCItemManager::ITEM_PET_HATCH_ITEM_GP: // �� ��ȭ ������
        {
            // ��ȿ����� ��ȭ�� �� �� ����
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_USE_HATPCH_ITEM ) );
                return;
            }

            // �켱 �������� �ִ��� �˾ƺ���.. ( �Ϲ���, ���� =��= )
            if( g_MyD3D->m_mapPetInfo.size() > 0 )
            {
                std::map<GCITEMUID, KPetInfo>::iterator mit;
                for( mit = g_MyD3D->m_mapPetInfo.begin(); mit != g_MyD3D->m_mapPetInfo.end(); ++mit )
                {
                    // ������ �ϳ��� ������.. + �� �κ��丮�� �ִ� �������̸�
                    if( mit->second.m_nHatchingID != -1 && g_pItemMgr->FindInventory(mit->second.m_dwUID) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PET_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_PET_HATCH_BOX, m_iItemID, mit->second.m_dwUID );
                        return;
                    }
                }
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE12 ), L"" );
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE12 ), L"" );
            }
            return;
        }
    case KGCItemManager::ITEM_RESET_WIN_LOSE_DATA: // �·� �ʱ�ȭ ������
        {
            GCItem* kItem = g_pItemMgr->GetItemData( m_iItemID );
            RETURN_NIL(kItem);

            // �� �ȿ����� �·��ʱ�ȭ �������� ����� �� ����
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetReplacedString( STR_ID_CAN_NOT_USE_RESET_WIN_LOSE_ITEM , "l", kItem->strItemName ) );
                return;
            }

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_ALERT_USE_RESET_WIN_LOSE_ITEM, "l", kItem->strItemName ), L"",
                KGCUIScene::GC_MBOX_USE_RESET_WIN_LOSE_DATA, kItem->dwGoodsID, 0 );
            return;
        }
    case KGCItemManager::ITEM_BONUS_CHARGING_ITEM_POINT: // ���ʽ� ���� ������
    case KGCItemManager::ITEM_BONUS_CHARGING_ITEM_CASH:
        {
            // MessageBox
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BONUS_CHARGE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwUID );
            return;
        }
    case KGCItemManager::ITEM_CHAR_LAS_CARD_CASH:
    case KGCItemManager::ITEM_CHAR_RYAN_CARD_CASH:
    case KGCItemManager::ITEM_CHAR_RONAN_CARD_CASH:
    case KGCItemManager::ITEM_CHAR_JIN_CARD_CASH:
    case KGCItemManager::ITEM_CHAR_SIEG_CARD_CASH:
        {
            Result_New_Character = INT_MAX;
            KP2P::GetInstance()->Send_NewCharacterReq( m_iItemID, m_dwUID );
            if( false == g_MyD3D->WaitForServerAck( Result_New_Character, INT_MAX, 3000, INT_MAX ) )
            {
                // Ÿ�Ӿƿ� ó��
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
            }
            // �������� ���ν������� ó��
            return;
        }
    case KGCItemManager::ITEM_WINTER_VACATION_BOMB_BOX1:
    case KGCItemManager::ITEM_WINTER_VACATION_BOMB_BOX2:
        {
            D3DXVECTOR2 vPos = m_pkConfirmButton->GetFixedWindowPos();

            if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
                return;
            }

            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "boom1_1", (vPos.x - 405.0f) / 400.0f ,  (-vPos.y + 331.0f) / 405.0f, 0.5f ) , GC_LAYER_UI);
            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "boom1_2", (vPos.x - 405.0f) / 400.0f ,  (-vPos.y + 331.0f) / 405.0f, 0.5f ) , GC_LAYER_UI);
            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "boom1_3", (vPos.x - 405.0f) / 400.0f ,  (-vPos.y + 331.0f) / 405.0f, 0.5f ) , GC_LAYER_UI);
            g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "boom1_4", (vPos.x - 405.0f) / 400.0f ,  (-vPos.y + 331.0f) / 405.0f, 0.5f ) , GC_LAYER_UI);
            
            Result_Kid_Gift = -100;
            KP2P::GetInstance()->Send_ChangeItemReq( m_dwUID );
            g_MyD3D->WaitForServerAck( Result_Kid_Gift, -100, 15000, -99 );

            if( Result_Kid_Gift == -99 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
            }
            return;
        }
    case KGCItemManager::ITEM_VIRTUAL_CASH_COUPON0:
    case KGCItemManager::ITEM_VIRTUAL_CASH_COUPON1:
    case KGCItemManager::ITEM_VIRTUAL_CASH_COUPON2:
    case KGCItemManager::ITEM_VIRTUAL_CASH_COUPON3:
    case KGCItemManager::ITEM_VIRTUAL_CASH_COUPON4:
        {
            GCItem* kItem = g_pItemMgr->GetItemData( m_iItemID );
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_USE_COUPON_ITEM, "l", kItem->strItemName), L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_CASH_COUPON, m_dwUID, m_iItemID);
            return;
        }
    case KGCItemManager::ITEM_BINGO_COIN:
        {
            Result_BingoChangeCoin = INT_MAX;
            KP2P::GetInstance()->Send_ChangeBingoCoin();
            g_MyD3D->WaitForServerAck( Result_BingoChangeCoin, INT_MAX, 5000, TIME_OUT_VALUE );
            return;
        }
        break;
    case KGCItemManager::ITEM_HALLOWEEN_DICE:
        {
            KP2P::GetInstance()->Send_DiceItemTradeReq( KGCItemManager::ITEM_HALLOWEEN_DICE * 10 );
            return;
        }
        break;
    case KGCItemManager::ITEM_GAWIBAWIBO_COIN:
        {
            KP2P::GetInstance()->Send_GawibawiboTradeReq( KGCItemManager::ITEM_GAWIBAWIBO_COIN * 10 );
            return;
        }
        break;
    case KGCItemManager::ITEM_GAWIBAWIBO_RETRY:
        {
            KP2P::GetInstance()->Send_GawibawiboTradeReq( KGCItemManager::ITEM_GAWIBAWIBO_RETRY * 10 );
            return;
        }
        break;
    case KGCItemManager::ITEM_PUZZLE_PLATE:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_PUZZLE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL );
            return;
        }
        break;	
    case KGCItemManager::ITEM_GACHA_VIP:
        {
            g_KDSound.Play( "30" );

            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_VIP_GACHA_ERROR2 ) );
            }
            else
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_VIP_GACHA_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }

            return;
        }
        break;
    case KGCItemManager::ITEM_GACHA_COIN:
        {
            KP2P::GetInstance()->Send_WebGachaCoinTrade( m_iItemID*10, m_iDuration );
            return;
        }
        break;
    case KGCItemManager::ITEM_PROMOTION_MAGIC_1:        // 1�� ���� ������
        {
            KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID( KGCItemManager::ITEM_PROMOTION_MAGIC_1 );
            if( pkItem == NULL || pkItem->m_nCount <= 0)
            {   break;   }
            UsePromotionMagic( KGCItemManager::ITEM_PROMOTION_MAGIC_1, PROMOTION_MAGIC_1 );

            return;
        }
        break;
    case KGCItemManager::ITEM_PROMOTION_MAGIC_2:        // 1�� ���� ������
        {
            KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID( KGCItemManager::ITEM_PROMOTION_MAGIC_2 );
            if( pkItem == NULL || pkItem->m_nCount <= 0)
            {   break;   }
            UsePromotionMagic( KGCItemManager::ITEM_PROMOTION_MAGIC_2, PROMOTION_MAGIC_2 );

            return;
        }
        break;
    case KGCItemManager::ITEM_PROMOTION_MAGIC_3:        // 1�� ���� ������
        {
            KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID( KGCItemManager::ITEM_PROMOTION_MAGIC_3 );
            if( pkItem == NULL || pkItem->m_nCount <= 0)
            {   break;   }
            UsePromotionMagic( KGCItemManager::ITEM_PROMOTION_MAGIC_3, PROMOTION_MAGIC_3 );

            return;
        }
        break;
    case KGCItemManager::ITEM_RESET_SKILL:
        {
            int iCharIdx = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[ g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex ].iCharType;
            int iUsageSP = SiGCSKT()->GetUsedSPinPromotion( iCharIdx, 0 );

            if( iUsageSP )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_RESETSKILL_01, "li", GCFUNC::GetCharName(iCharIdx), iUsageSP), 
                                                                     L"", 
                                                                     KGCUIScene::GC_MBOX_RESET_SKILL );
            else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RESETSKILL_02), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, false);

            return;
        }
        break;
    case KGCItemManager::ITEM_OPEN_CHANGE_SKILL_SLOT:
        {
            if( g_kGlobalValue.GetOpenSkillSlotIndex( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ) > KSkillSlot::INDEX_A ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, 
                    g_pkStrLoader->GetReplacedString( STR_ID_OPENED_CHANGE_SKILL_SLOT_MESSAGE, "l", GCFUNC::GetCharName( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ) ), 
                    L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, false);
            } else {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, 
                    g_pkStrLoader->GetReplacedString( STR_ID_OPEN_CHANGE_SKILL_SLOT_QEUSTION, "l", GCFUNC::GetCharName( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType ) ), 
                    L"", KGCUIScene::GC_MBOX_USE_OPEN_CHANGE_SKILL_SLOT, KGCItemManager::ITEM_OPEN_CHANGE_SKILL_SLOT * 10, 0, true, true );
            }

            return;
        }
        break;
    case KGCItemManager::ITEM_SONGKRAN_WATER_BOMB:
        {
            KP2P::GetInstance()->Send_SongkranEventWaterBombExchangeReq();
            return;
        }
        break;
    case KGCItemManager::ITEM_RITAS_CHRISTMAS_CONTINUE_COIN_EXCHANGE_ITEMID:    // ��Ÿ�� ũ�������� �̺�Ʈ �絵�� ���� ���� ������
        {
			Result_ReceiveChristmasExchangeContinueCoin = INT_MAX;
            KP2P::GetInstance()->Send_RitasChristmas_ExchangeContinueCoin_Req();
			g_MyD3D->WaitForServerAck(Result_ReceiveChristmasExchangeContinueCoin,INT_MAX,3000,TIME_OUT_VALUE);
            return;
        }
        break;
    default:
        if ( g_pItemMgr->IsGPCoupon( m_iItemID ) )
        {
            g_kGlobalValue.m_pkCurrentUseItemID = m_iItemID;
            g_kGlobalValue.m_bIsCurrentUsechest = false;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_QUANTITYUSE_DLG);
            return;
        }
#if defined ( USE_VIP_POINT )
        else if ( g_pItemMgr->IsVPCoupon( m_iItemID ) )
        {
            g_kGlobalValue.m_pkCurrentUseItemID = m_iItemID;
            g_kGlobalValue.m_bIsCurrentUsechest = false;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_QUANTITYUSE_DLG);
            return;
        }
#endif
        else if( GIK_TREASUREBOX_ITEM == m_ItemInfo->eItemKind || GIK_COORDIBOX == m_ItemInfo->eItemKind)
        {
            if( false == g_pItemMgr->CheckExtraInventoryExist() && GIK_TREASUREBOX_ITEM == m_ItemInfo->eItemKind )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"" );
                return;
            }

            g_kGlobalValue.m_pkCurrentUseItemID = m_ItemInfo->dwGoodsID;
            g_kGlobalValue.m_bIsCurrentUsechest = true;

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_QUANTITYUSE_DLG );

            return;
        }
        else if( g_kGlobalValue.m_kUserMission.IsMissionPackageItem( m_iItemID ))
        {
            Result_RegistMissionPackage = INT_MAX;
            KP2P::GetInstance()->Send_MissionPackage( (DWORD)m_iItemID, m_dwUID, (char)m_ItemInfo->eItemType );
            g_MyD3D->WaitForServerAck( Result_RegistMissionPackage, INT_MAX, 3000, TIME_OUT_VALUE );

            if( Result_RegistMissionPackage == -99 )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
            }

            return;
        }
        else if( g_pItemMgr->IsExpItemList( m_iItemID ) )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_USE_COUPON_ITEM, "l", kItem->strItemName), L"", KGCUIScene::GC_MBOX_USE_EXP_POTION, m_iItemID, m_dwUID );

            return;
        }

        if(g_MyD3D->m_kItemMgr.IsExistTradeItem(m_iItemID))
        {
            KP2P::GetInstance()->Send_ItemTradeReq(g_MyD3D->m_kItemMgr.GetItemTradeInfo( m_iItemID ).first, 
                g_MyD3D->m_kItemMgr.GetItemTradeInfo( m_iItemID ).second, 0);
            return;
        }

        if( SiKGCLevelResetEventManager()->IsResetItem( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType, m_iItemID ) )
        {
            if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString( STR_ID_LEVER_RESET_ITEM_USE_ERROR02 ) );
                return;
            }

            char iCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
            std::wstring strTemp = g_pkStrLoader->GetReplacedString( STR_ID_LEVER_RESET_ITEM_USE, "l", GCFUNC::GetCharName( iCharType ) );

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_LEVEL_RESET_ITEM, strTemp, L"", KGCUIScene::GC_MBOX_USE_LEVEL_RESET_ITEM, iCharType, m_iItemID );

            return;
        }

		if(g_pItemMgr->IsLookExtendItem( m_iItemID ) )
		{
			// �κ��丮 ũ�Ⱑ 400�̻��̸� ���̻� �κ��丮 Ȯ�� �������� ��� �� �� ����
			int iInventoryCapacity = g_kGlobalValue.m_iCoordiCapacity;
			if( iInventoryCapacity >= g_pItemMgr->GetMaxLookExtendSlot() )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
					g_pkStrLoader->GetReplacedString( STR_ID_CANT_EXPAND_COORDI_INVENTORY, "i", g_pItemMgr->GetMaxLookExtendSlot() ), L"" );
				return;
			}

			int iExtendInventorySize = g_pItemMgr->GetLookExtendSize(m_iItemID);

			if(iExtendInventorySize == 0)
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_FAILED_TO_GET_COORDI_INFO ) );
				return;
			}
			
			std::wstring str1 = g_pkStrLoader->GetReplacedString( STR_ID_COORDI_INVENTORY_SPACE, "i", (int)g_kGlobalValue.m_iCoordiCapacity );
			std::wstring str2 = g_pkStrLoader->GetReplacedString( STR_ID_COORDI_EXPAND_QUESTION, "i", iExtendInventorySize );

			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXTEND_LOOK_INVENTORY, str1, str2, KGCUIScene::GC_MBOX_USE_EXTEND_LOOK_INVENTORY, m_dwUID, m_iItemID );
			return;
		}
        break;
    }



    if( true == SiKNeonSign()->IsServerNeonsign( m_iItemID ) )
    {
        g_pkUIScene->m_pkNeonsignSelectBox->SetSignBoardType( KSignBoardData::SBT_SERVER );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEONSIGN_SELECT );
        return;
    }

    if( m_ItemInfo->eItemKind == GIK_COUPLE_ITEM )
    {
        if( SiKGCCoupleSystem()->Am_I_Solo() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM16), L"" );
        }
        else
        {
            //Ŀ�� �������� ���忡 ������??
            SiKGCCoupleSystem()->ReadyToInsertInven( m_iItemID, m_dwUID, false );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_MESSAGE_OK_CANCLE, g_pkStrLoader->GetString(STR_ID_COUPLE_ITEM_CONFIRM), L"", KGCUIScene::GC_MBOX_USE_COUPLE_ITEM_CONFIRM );
        }
        return;
    }



    if( m_ItemInfo->eItemKind == GIK_MISSION_SCROLL )
    {
        RegisterMission();
        return;
    }

    if(g_pItemMgr->IsGachaList(m_iItemID))
    {
        if( g_kGlobalValue.m_vecGachaUseVersion.empty() ) { 
            return;
        }

        SiKGCDepotManager()->Send_DepotInfoByCharTabReqForGacha();
        if( g_kGlobalValue.IsSingleGacha() ) {  // ���� ������ �ϳ��� ���� ��í 
			if(g_pkUIScene->m_pkSuperGachaDlg->IsRenderOn()) 
			{
				if( g_pkUIScene->GetClearSealDlg()->IsOpenUI() )
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
			} else {
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
			}
        }
        else {  // ���� ������ �������� ���۰�í 
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUPER_GACHA, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType,g_MyD3D->m_TempPlayer.GetCurrentChar().GetCurrentPromotion() );
        }

        return;
    }

    if(g_pItemMgr->IsSkillScroolList(m_iItemID))
    {
        int iSkillID = g_pItemMgr->GetSkillID( m_iItemID );
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_USE_COUPON_ITEM, "l", kItem->strItemName), L"", KGCUIScene::GC_MBOX_USE_SKILL_SCROOL_ITEM_USE, iSkillID, m_dwUID );
        return;
    }
}

void KGCUserItemWnd::OnCraftingButton()
{
    if( NULL == m_pkCraftingButton )
        return;

    GCItem* pItem = NULL;
    pItem = g_pItemMgr->GetItemData( m_iItemID );

    if( NULL == pItem )
        return;

    switch( m_iItemID )
    {
    case KGCItemManager::ITEM_NIGHTMARKET:
        {
            if(SiKP2P()->IsConnectedToGameServer() == true )
            {
                SiGCForgeManager()->Send_GetItemCatalog();
                RecipeSimpleInfo sRecipeSimpleInfo = SiGCForgeManager()->GetCraftRecipeInfo( KGCItemManager::ITEM_NIGHTMARKET_LIGHT );
                if( 0 != sRecipeSimpleInfo.ItemID )
                {
                    // ��� ��� �޾� ���� 
                    std::set< PAIR_USHORT_DWORD > setSendItemIDList;
                    setSendItemIDList.insert( std::make_pair( sRecipeSimpleInfo.usIndex, sRecipeSimpleInfo.ItemID ) );
                    SiGCForgeManager()->Send_GetMeterials( setSendItemIDList );

                    std::vector<SMaterialInfo> vInfo;
                    SiGCForgeManager()->GetMaterialList( sRecipeSimpleInfo.usIndex, sRecipeSimpleInfo.ItemID, vInfo );

                    std::vector< GCITEMUID > vecMaterialUID;
                    std::vector<SMaterialInfo>::iterator  itInfo = vInfo.begin();

                    for( ; itInfo != vInfo.end() ; ++itInfo ) { 
                        KItem* pInvenItem = NULL;
                        g_pItemMgr->FindInventoryForItemID( itInfo->m_ItemID, &pInvenItem );
                        

                        // ��ᰡ �κ��� ���� 
                        if( pInvenItem == NULL ) { 
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ITEM_CREATION_ERROR4), L"" );
                            return;

                        }

                        // ��� ������ ���� 
                        if( pInvenItem->m_nCount < itInfo->m_nDuration ) { 
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ITEM_CREATION_ERROR4), L"" );
                            return;
                        }

                        vecMaterialUID.push_back( pInvenItem->m_ItemUID );
                    }

                    SiGCForgeManager()->CreateItem( sRecipeSimpleInfo, 1, vecMaterialUID );
                    if( g_kGlobalValue.bItemCrafting )
                    {
                        g_kGlobalValue.bItemCrafting = false;
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NIGHTMARKET_FORGE), L"" );
                    }
                }
            }
            else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
        }
        break;
    }
}

void KGCUserItemWnd::RegisterMission()
{
    // ==========================================�̼� ��� �սô�==========================================
    std::wstring str;
    DWORD dwMissionID = m_iItemID * 10 ;
    //�̼� ��ũ��Ʈ�� �̼� ������ ���� �ϴ��� üũ �մϴ�.
    if( SiKGCMissionManager()->IsMissionExist( dwMissionID ) == false )
    {
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR5);
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        return;
    }

    // �̼��� ����Ϸ��� �ɸ��Ͱ� �����ϴ��� �ľ��մϴ�.
    if( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter != -1 &&
        g_kGlobalValue.m_kUserInfo.IsCharExist( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) == false )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVENTORY_ERR_MSG1 ), 
            g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_ERR_MSG2, "l", GCFUNC::GetCharName( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) ) );
        return;
    }

    // �̼� 5�� �̻� ��� ����
//     if( g_kGlobalValue.m_kUserMission.GetCharMissionNum( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType ) >= 5 )
//     {
//         std::wstring str;
//         str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR8);
//         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
//         return;
//     }

    // KK�۾����� ��Ŷ�� ���
    if( Result_Buy == -98 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PACKET_DELAY ),L""
            , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );        
        return;
    }

    // ���� ������ �̼� ��� ������ ������ ���߾����� üũ �մϴ�.
    int iAvailableCount = 0;
    for( int i = 0 ; i < (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size() ; ++i )
    {
        SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo[i];
        if( SiKGCMissionManager()->IsAvailableRegister( dwMissionID, sInfo ) == KGCMissionManager::GC_MM_OK )
            ++iAvailableCount;
    }

    if( iAvailableCount == 0 )
    {
        SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
        switch( SiKGCMissionManager()->IsAvailableRegister(dwMissionID, sInfo ) )
        {
        case KGCMissionManager::GC_MM_DIFFERENT_CHAR:
            str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR17);
            break;
        case KGCMissionManager::GC_MM_NOT_ENOUGH_LEVEL:
            str = g_pkStrLoader->GetReplacedString(STR_ID_REGISTER_MISSION_ERROR12,"i",SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMinLevel() );
            break;
        case KGCMissionManager::GC_MM_NOT_ENOUGH_PROMOTION:
            {

#if defined( NATION_USA )
                int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion + 1;
#else 
                int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion;
#endif 

                str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR16, "i", nPromotion );
            }
            break;
        case KGCMissionManager::GC_MN_NOT_ALREADY_PROMOTION:
            {
                str = g_pkStrLoader->GetString( STR_ID_BUY_CERT_FAILED_JOBLEVEL );
            }
            break;
        }
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        return;
    }

    //���� �̼��� �ΰ��� �ߺ��ؼ� ������ �� ����
    if( g_kGlobalValue.m_kUserMission.IsUserMission( dwMissionID ) == true ||
        g_kGlobalValue.m_kUserMission.IsInvalidMission( dwMissionID ) != 0 )
    {
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR6 );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        return;
    }

    Result_Kid_Gift = 100;

    KP2P::GetInstance()->Send_RegistMission( m_iItemID * 10, m_dwUID );

    g_MyD3D->WaitForServerAck( Result_Kid_Gift, 100, 5000, 200 );

    // ������ Procedure���� �ۼ��ϰ���
    switch( Result_Kid_Gift )
    {
    case 0:  // ���� 
        //ȭ�鿡 ������Ʈ ����
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR0);
        break;
        break;
    case 2:  // ���� ��ϵ� �̼���
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR6 );
        break;
    case 3:  // �̹� �Ϸ��� �̼���
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR7 );
        break;
    case 4:  // �̼� ������ ��á��
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR8 );
        break;
    case 5:  // ��ϵ� �̼ǰ� ������ �̼��̹Ƿ� ��ϺҰ�.
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR15);
        break;
    case 8:  // ĳ���Ͱ� �������� ����
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR2 );
        break;
    case 9:  // ���� ���°� ���� ����.
        {
#if defined( NATION_USA )
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion + 1;
#else 
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion;
#endif 
            str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR16, "i", nPromotion );
        }
        break;
    case 10: // ĳ���� ���� ������ �������� ����
        str = g_pkStrLoader->GetReplacedString(STR_ID_REGISTER_MISSION_ERROR12,"i",SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMinLevel() );
        break;
    case 11: // �����ϴ� �̼��� �ƴ�.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR5);
        break;
    case 14: // �������� �����ϰ� ���� ����.
        str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR11, "i", Result_Kid_Gift );
        g_pItemMgr->m_kInventory.RemoveItem( m_dwUID );
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        break;
    case 15:
        str = g_pkStrLoader->GetString( STR_ID_ALREADY_ACQUIRED_CHARACTER );
        break;
    case 16:
        str = g_pkStrLoader->GetString( STR_ID_BUY_CERT_FAILED_JOBLEVEL );
        break;
    case 7:  // �̼� ���� �������� ����.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR4);
        break;
    case 1:  // ������ �������� ����
    case 6:  // �̼� �������� �ƴѵ� �̼����� ����Ϸ��� ����
    case 12: // Count SubMission ����� ���� ������ ����.
    case 13: // ��Ͻ� DB ����.
    case -99: // �˼����� ����
        str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR11, "i", Result_Kid_Gift );
        break;
    case 200://TimeOut
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR3 );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        break;
    }
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );

    return;
}

void KGCUserItemWnd::OnEnchantButton()
{
    //�ľǵ��� ���� ������ m_ItemInfo�� ���ΰ��� �ͼ� ũ���ð� �Ͼ�� �ִ°����� �����ȴ�. �켱 Crash�� ���� m_ItemInfo�� NULL�� �Ǵ� ��Ȳ�� �ִ��� �ľ�����.
    if ( m_ItemInfo == NULL )   
        return;                 

    //�濡���� ��ȭ�� �� �� ����.
    if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_ENCHANT_ERROR03 ) );
        return;
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEW_NECKLACE_ENCHANT_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwUID, 0 );
}

void KGCUserItemWnd::OnDeleteButton()
{
    // 2007.04.29 wony
    // ����Ʈ �������̸�..
    // �̷���..
    if( m_bPointItem )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_POINT_ITEM_ALERT ), L"",
            KGCUIScene::GC_MBOX_USE_DELETE_POINT_ITEM, m_dwUID );
    }
    else // �ƴϸ� ����ó����Ĵ��-��-��
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
            g_pkStrLoader->GetString( STR_ID_DELETE_ITEM_NO_GEM ),
            g_pkStrLoader->GetString( STR_ID_REALY_DELETE_ITEM ),
            KGCUIScene::GC_MBOX_USE_DELETE_ITEM, m_dwUID );
    }

}

void KGCUserItemWnd::OnDeletePointItemButton()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_POINT_ITEM_ALERT ), L"",
        KGCUIScene::GC_MBOX_USE_DELETE_POINT_ITEM, m_dwUID );
}

void KGCUserItemWnd::OnEatFoodButton()
{
    // �� : �����ֱ�
    KPetInfo kPetInfo = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;

    int iSatiation = g_MyD3D->m_mapPetInfo[kPetInfo.m_dwUID].m_iSatiation;

    // �������� MAX�̸� ���� ������ ���Ѵ�
    if( iSatiation == MAX_SATIATION )
    {
        std::wstring str = g_pkStrLoader->GetString( STR_ID_HUNGRYRATE_MESSAGE5 );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        return;
    }

    // ������ ���� ���ٸ� ���̸� ���� ���Ѵ�
    if( kPetInfo.m_dwUID == 0 )
    {
        // ���� ���� ���� �����ϴ�
        // �� ���� �� ����� �ּ���
        std::wstring str1 = g_pkStrLoader->GetString( STR_ID_PET_EQUIP_ITEM_MESSAGE1 );
        std::wstring str2 = g_pkStrLoader->GetString( STR_ID_PET_EQUIP_ITEM_MESSAGE2 );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str1, str2 );
        return;
    }

    std::wstring str1 = g_pkStrLoader->GetReplacedString( STR_ID_HUNGRYRATE_MESSAGE1, "iil", iSatiation, MAX_SATIATION, GetPetState( iSatiation ) );
    std::wstring str2 = g_pkStrLoader->GetString( STR_ID_HUNGRYRATE_MESSAGE2 );

    g_pkUIScene->MessageBox( KGCUIScene::GC_PET_FEEDING, str1, str2, KGCUIScene::GC_MBOX_USE_PET_FEEDING, m_dwUID );
}

void KGCUserItemWnd::OnCoordiUndoButton()
{
    if (m_pkImgCoordiUndo == NULL)
        return;

    GCItem* pItem = g_pItemMgr->GetItemData(m_ItemInfo->dwGoodsID);
    KItem* DesignItem = NULL;
    g_pItemMgr->FindInventory(m_dwUID, &DesignItem, &pItem);

    std::wstring DesignName = g_pItemMgr->GetItemName(DesignItem->m_DesignCoordiID);

    std::wstringstream stm;
    std::wstringstream strm;
    std::wstring strColorKeyEnd = L"#cX";

    stm << g_pItemMgr->GetItemGradeColorKey(DesignItem->m_cGradeID) << DesignName << strColorKeyEnd;
    strm << g_pItemMgr->GetItemGradeColorKey(m_ItemInfo->cItemGrade) << m_ItemInfo->strItemName << strColorKeyEnd;

    std::wstring strMsg = g_pkStrLoader->GetReplacedString(STR_ID_UNDO_COORDI_CONFIRMATION, "ll", strm.str(), stm.str());

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, strMsg, L"",
        KGCUIScene::GC_MBOX_UNDO_COORDI_ITEM, m_dwUID);
}

void KGCUserItemWnd::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
        g_KDSound.Play( "30" );
        return;
    case KD3DWnd::D3DWE_BUTTON_DOWN:
        g_KDSound.Play( "31" );
        return;
    }

    if ( KD3DWnd::D3DWE_BUTTON_CLICK != event.m_dwCode ) 
	{
		if( KD3DWnd::D3DWE_RBUTTON_CLICK == event.m_dwCode )
		{
			SpeakToActionListener( KActionEvent(this, KD3DWnd::D3DWE_RBUTTON_CLICK, m_dwUID));
		}
		return;
	}

    if( m_pkDeleteButton && event.m_pWnd == m_pkDeleteButton )
    {
        OnDeleteButton();
        return;
    }

    if( m_pkConfirmButton && event.m_pWnd == m_pkConfirmButton )
    {
        OnConfirmButton();
        return;
    }

    if( m_pkCraftingButton && event.m_pWnd == m_pkCraftingButton )
    {
        OnCraftingButton();
        return;
    }


    if (m_pkEatFoodButton && event.m_pWnd == m_pkEatFoodButton)
    {
        OnEatFoodButton();
        return;
    }

    if ( m_pkEnchantButton && event.m_pWnd == m_pkEnchantButton ) // ��ȭ ��ư�� Ŭ���ߴٸ�
    {
        OnEnchantButton();
        return;
    }

    if( m_pkPlayButton && event.m_pWnd == m_pkPlayButton )
    {
        OnPlayButton();
        return;
    }

    if( m_pkSocketButton && event.m_pWnd == m_pkSocketButton )
    {
        OnSocketButton();
        return;
    }

    if (m_pkImgCoordiUndo && event.m_pWnd == m_pkImgCoordiUndo)
    {
        OnCoordiUndoButton();
        return;
    }

    if( m_pkAttributeButton ) {
        GCWND_MSG_MAP( m_pkAttributeButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnAttributeButton );
    }

    if ( m_pkLevelDownButton ) 
    {
        GCWND_MSG_MAP( m_pkLevelDownButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnItemLevelDownClick );
    }

	if (m_pkBtnEquip && event.m_pWnd == m_pkBtnEquip)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBAX_PET_MAGIC_STONE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_iItemID, 0, false, true);
	}
}

KItem* KGCUserItemWnd::GetItemInfo( void )
{
   KItem* pInvenItem = NULL;

   g_pItemMgr->FindInventory( m_dwUID, &pInvenItem);
      
    return pInvenItem;
}

void KGCUserItemWnd::OffItemBtn( )
{
    if( m_pkDeleteButton )
        m_pkDeleteButton->ToggleRender( false );

    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( false );

    if( m_pkEnchantButton )
        m_pkEnchantButton->ToggleRender( false );

    if( m_pkConfirmButton )
        m_pkConfirmButton->ToggleRender( false );

    if( m_pkEatFoodButton )
        m_pkEatFoodButton->ToggleRender( false );

    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );

    if( m_pkAttributeButton )
        m_pkAttributeButton->ToggleRender( false );

    if ( m_pkLevelDownButton ) 
        m_pkLevelDownButton->ToggleRender( false );
    
    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( false );

    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( false );

    if( m_pkDeleteButton )
        m_pkDeleteButton->ToggleRender( false );

    if( m_pkPlayButton)
        m_pkPlayButton->ToggleRender( false );

    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );

    if( m_pkCraftingButton )
        m_pkCraftingButton->ToggleRender( false );
}

void KGCUserItemWnd::ShowItem( bool bShow )
{
    if( m_pkDeleteButton )
        m_pkDeleteButton->ToggleRender( bShow && ( m_bPeriodItem || m_bPointItem ) );

    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( false );

    if( m_pkEnchantButton )
        m_pkEnchantButton->ToggleRender( m_bEvent ? false : ( m_bEnchant && !m_bEnchantBreak && bShow ) );

    if( m_pkConfirmButton )
        m_pkConfirmButton->ToggleRender( false );

    if( m_pkCraftingButton )
        m_pkCraftingButton->ToggleRender( false );

    if( m_pkEatFoodButton ) {

        if( m_ItemInfo != NULL )
        {
            m_pkEatFoodButton->ToggleRender( bShow && ( m_ItemInfo->ePetItemKind == EPET_KIND_FOOD ) );
        }
        else {
            m_pkEatFoodButton->ToggleRender( false );
        }

    }
    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );
    if( m_pkAttributeButton )
        m_pkAttributeButton->ToggleRender( false );

    if ( m_pkLevelDownButton ) 
        m_pkLevelDownButton->ToggleRender( false );

    if( m_pkLevelDownOver ) 
        m_pkLevelDownOver->ToggleRender( false );

    m_bShow = bShow;

    if( m_pkItemName != NULL ) {
        m_pkItemName->ToggleRender( bShow );
    }

    if( m_pkItemBar )
        m_pkItemBar->ToggleRender( bShow );

    if( m_pkItemLimit != NULL )
        m_pkItemLimit->ToggleRender( bShow );

    if( m_pkDurationNum != NULL && !bShow )
        m_pkDurationNum->ToggleRender( false );

    if( m_pkSmallDurationNum && !bShow )
        m_pkSmallDurationNum->ToggleRender( false );

    if( m_pkEquipEnchantItemLv && !bShow )
        m_pkEquipEnchantItemLv->ToggleRender(false);

    if( m_pkStrongNum != NULL && !bShow )
        m_pkStrongNum->ToggleRender( false );

    if( m_pkPriceNum != NULL && !bShow )
        m_pkPriceNum->ToggleRender( false );

    if( m_pkLimitIcon != NULL && !bShow )
        m_pkLimitIcon->ToggleRender( false );

    if( m_pkPocketIcon != NULL && !bShow )
        m_pkPocketIcon->ToggleRender( false );

    if (m_pkImgCoordiCompose != NULL && !bShow) {
        m_pkImgCoordiCompose->ToggleRender(false);
        m_pkImgCoordiUndo->ToggleRender(false);
    }

    if( !bShow )
    {
        if( m_pkItemBar )
            m_pkItemBar->SetCertType( KGCItemBar::CERT_TYPE_NONE );
        if( m_pkItemLimit != NULL )
            m_pkItemLimit->SetText( L"" );
    }

    if (m_pkItemImg)
        m_pkItemImg->ToggleRender( bShow );
    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( bShow && !m_bNotSellItem );

    if( m_ItemInfo && m_pkConfirmButton )
    {
        if ( IsShowConfirmButton( m_iItemID )
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_TREASUREBOX_ITEM)
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_COORDIBOX)
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_MISSION_SCROLL )
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_COUPLE_ITEM && m_ItemInfo->dwSlotPosition != ESP_A_FINGER ) 
            || g_kGlobalValue.m_kUserMission.IsMissionPackageItem( m_iItemID ) 
            || ( g_pkUIScene->m_pkChoiceDrop != NULL && g_pkUIScene->m_pkChoiceDrop->IsChoiceDropItem( m_iItemID ) )
            )
        {
            m_pkConfirmButton->ToggleRender( bShow );
        }
        else
        {
            m_pkConfirmButton->ToggleRender( false );
        }
    }
    else
    {
        if( m_pkConfirmButton )
            m_pkConfirmButton->ToggleRender( false );
    }

    if( m_pkCraftingButton )
    {
        if ( IsShowCraftingButton( m_iItemID ) )
        {
            m_pkCraftingButton->ToggleRender( bShow );
        }
    }

    //�Ⱓ���� �ƹ��׼��� �Ҽ� ������ ����ϴ�
    //������� ����
    if( m_bPeriodItem || m_bPointItem )
    {
        if( m_pkItemBar && m_bPeriodItem )
            m_pkItemBar->ToggleRender( false );

        if( m_pkSellButton )
            m_pkSellButton->ToggleRender( false );

        if( m_pkEnchantButton )
            m_pkEnchantButton->ToggleRender( false );
    }

    if( m_pkRemainTime != NULL )
        m_pkRemainTime->ToggleRender( bShow && m_bPeriodItem );

    if( m_pkTimeRed && !bShow )
        m_pkTimeRed->ToggleRender( bShow );
    if( m_pkTimeYellow && !bShow )
        m_pkTimeYellow->ToggleRender( bShow );
    if( m_pkTimeGreen && !bShow )
        m_pkTimeGreen->ToggleRender( bShow );

    // �ҷ��� �ֻ����� ���� ����
    if( m_iItemID == KGCItemManager::ITEM_HALLOWEEN_BOARD || m_iItemID == KGCItemManager::ITEM_BINGO_BOARD || m_iItemID == KGCItemManager::ITEM_GAWIBAWIBO_BOARD || m_iItemID == KGCItemManager::ITEM_PLANTTREE_BOARD || m_iItemID == KGCItemManager::ITEM_SPHINX_BOARD)
    {
        if( m_pkDeleteButton )
            m_pkDeleteButton->ToggleRender( false );

        if( m_pkPlayButton)
            m_pkPlayButton->ToggleRender( !m_bSellState );
        
    }
    else if( m_iItemID == KGCItemManager::ITEM_BINGO_BOARD )
    {
        if( m_pkDeleteButton )
            m_pkDeleteButton->ToggleRender( false );
        if( m_pkPlayButton)
            m_pkPlayButton->ToggleRender( true );
    }
    else {
        if( m_pkPlayButton )
            m_pkPlayButton->ToggleRender( false );
    }

    SetItemBtn();

#if defined( DISABLE_ITEM_SOCKET )
    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );
#endif
}

void KGCUserItemWnd::UpdateButton( bool bShow )
{
    if( m_pkDeleteButton )
        m_pkDeleteButton->ToggleRender( bShow && ( m_bPeriodItem || m_bPointItem ) );

    if( m_pkSellButton )
        m_pkSellButton->ToggleRender( bShow && !m_bNotSellItem );

    if( m_pkEnchantButton )
        m_pkEnchantButton->ToggleRender( m_bEvent ? false : ( m_bEnchant && !m_bEnchantBreak && bShow ) );


    if( m_pkEatFoodButton ) {

        if( m_ItemInfo != NULL )
        {
            m_pkEatFoodButton->ToggleRender( bShow && ( m_ItemInfo->ePetItemKind == EPET_KIND_FOOD ) );
        }
        else {
            m_pkEatFoodButton->ToggleRender( false );
        }

    }

    if( m_ItemInfo && m_pkConfirmButton )
    {
        //�̺�Ʈ�� Ȯ�ι�ư
        //m_pkConfirmButton->ToggleRender( m_bEquip || !m_bEvent ? false : bShow );
        //if ( IsShowConfirmButton( m_iItemID ) || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_MISSION_SCROLL ) )
        if ( IsShowConfirmButton( m_iItemID )
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_TREASUREBOX_ITEM)
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_COORDIBOX)
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_MISSION_SCROLL )
            || ( m_iItemID != -1 && m_ItemInfo->eItemKind == GIK_COUPLE_ITEM && m_ItemInfo->dwSlotPosition != ESP_A_FINGER ) 
            || g_kGlobalValue.m_kUserMission.IsMissionPackageItem( m_iItemID ) 
            || ( g_pkUIScene->m_pkChoiceDrop != NULL && g_pkUIScene->m_pkChoiceDrop->IsChoiceDropItem( m_iItemID ) )
            )
        {
            m_pkConfirmButton->ToggleRender( bShow );
        }
        else
        {
            m_pkConfirmButton->ToggleRender( false );
        }
    }
    else
    {
        if( m_pkConfirmButton )
            m_pkConfirmButton->ToggleRender( false );
    }

    if( m_pkCraftingButton )
    {
        if ( IsShowCraftingButton( m_iItemID ) )
        {
            m_pkCraftingButton->ToggleRender( bShow );
        }
    }

    //�Ⱓ���� �ƹ��׼��� �Ҽ� ������ ����ϴ�
    //������� ����
    if( m_bPeriodItem || m_bPointItem )
    {
        if( m_pkItemBar && m_bPeriodItem )
            m_pkItemBar->ToggleRender( false );

        if( m_pkSellButton )
            m_pkSellButton->ToggleRender( false );

        if( m_pkEnchantButton )
            m_pkEnchantButton->ToggleRender( false );
    }

    // �ҷ��� �ֻ����� ���� ����
    if( m_iItemID == KGCItemManager::ITEM_HALLOWEEN_BOARD || m_iItemID == KGCItemManager::ITEM_BINGO_BOARD || m_iItemID == KGCItemManager::ITEM_GAWIBAWIBO_BOARD || m_iItemID == KGCItemManager::ITEM_PLANTTREE_BOARD || m_iItemID == KGCItemManager::ITEM_SPHINX_BOARD)
    {
        if( m_pkDeleteButton )
            m_pkDeleteButton->ToggleRender( false );

        if( m_pkPlayButton)
            m_pkPlayButton->ToggleRender( !m_bSellState );

    }
    else if( m_iItemID == KGCItemManager::ITEM_BINGO_BOARD )
    {
        if( m_pkDeleteButton )
            m_pkDeleteButton->ToggleRender( false );
        if( m_pkPlayButton)
            m_pkPlayButton->ToggleRender( true );
    }
    else {
        if( m_pkPlayButton )
            m_pkPlayButton->ToggleRender( false );
    }

#if defined( DISABLE_ITEM_SOCKET )
    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );
#endif
}

void KGCUserItemWnd::OnSetItem()
{
    m_iItemID = -1;
    m_dwUID = 0;
    ShowItem(false);
    m_ItemInfo = NULL;
    SetLeaveCursor();
    UpdateSellIcons();
    SetGradeFrame( KItem::GRADE_NORMAL );
    SetItemDuration( -1 );
}

void KGCUserItemWnd::OnSetItem( KItem* pInventoryItem)
{
    if(pInventoryItem == NULL)
    {
        m_iItemID = -1;
        ShowItem(false);
        return;
    }

    m_ItemInfo = g_MyD3D->m_kItemMgr.GetItemData( pInventoryItem->m_ItemID );
    if( m_ItemInfo == NULL )
    {
        m_iItemID = -1;
        ShowItem( false );
        return;
    }

    m_iStrong = m_ItemInfo->iStrongLevel;
    if( m_pkStrongNum != NULL ) {
        m_pkStrongNum->ToggleRender( false );
        if( m_iStrong != 0 || m_ItemInfo->eItemKind == GIK_STRONG_ITEM ) {
            m_pkStrongNum->SetNumber( m_iStrong );
            m_pkStrongNum->ToggleRender( true );
        } 
    }


    m_iItemID = pInventoryItem->m_ItemID;
    SetItemDuration( pInventoryItem->m_nCount );
    if( m_pkDurationNum != NULL ) {
        if( m_iDuration > -1 ) {
            m_pkDurationNum->ToggleRender( true );
            m_pkDurationNum->SetNumber( m_iDuration );
        }
        else {
            m_pkDurationNum->ToggleRender( false );
        }
    }
    if( m_pkSmallDurationNum != NULL ) {
        if( m_iDuration > -1 ) {
            m_pkSmallDurationNum->ToggleRender( true );
            m_pkSmallDurationNum->SetNumber( m_iDuration );
        }
        else {
            m_pkSmallDurationNum->ToggleRender( false );
        }
    }


    if( m_pkEquipEnchantItemLv != NULL )        
    {
        bool bCheck = false;
        if( m_ItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
            bCheck = true;
        else if( m_ItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM && pInventoryItem->m_EnchantEquipItemUID != 0 )
            bCheck = true;

        if( bCheck )
        {
            m_pkEquipEnchantItemLv->ToggleRender(true);

            if( m_ItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
            {
                if( pInventoryItem->m_cGradeID == KItem::GRADE_NORMAL )
                    m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                else if( pInventoryItem->m_cGradeID == KItem::GRADE_EPIC )
                    m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
            }
            else
            {
                if( pInventoryItem->m_cEnchantEquipGradeID == KItem::GRADE_NORMAL )
                    m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                else if( pInventoryItem->m_cEnchantEquipGradeID == KItem::GRADE_EPIC )
                    m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
            }

            m_pkEquipEnchantItemLv->SetNumber(pInventoryItem->m_cEnchantLevel);
        }
        else
            m_pkEquipEnchantItemLv->ToggleRender(false);
    }


    m_dwUID = pInventoryItem->m_ItemUID;
    UpdateSellIcons();
    SetGradeFrame( pInventoryItem->m_cGradeID );

    m_bEnchant = (m_ItemInfo->eItemKind == GIK_STRONG_ITEM);


    m_bUnique = (m_ItemInfo->eItemKind == GIK_PRINT_UID );

    // ����Ʈ �������ΰ�?
#ifdef ENABLE_POINT_SYSTEM // Kawan>
    if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_ItemInfo->dwGoodsID ) )
        m_bPointItem = true;
    else
#endif
        m_bPointItem = false;

    if( pInventoryItem->m_nPeriod != KItem::UNLIMITED_ITEM )
        m_bPeriodItem = true;
    else 
        m_bPeriodItem = false;

    if( m_pkRemainTime != NULL ) {
        if( m_bPeriodItem )
            m_pkRemainTime->SetText( g_pItemMgr->GetRemainTime( *pInventoryItem ) );
        m_pkRemainTime->ToggleRender( m_bPeriodItem );

    }

    CTimeSpan cSpan = CTime( pInventoryItem->m_tEndDate+ g_kGlobalValue.m_tmServerSyncTime ) - g_kGlobalValue.m_tmServerTime;
    if( m_pkTimeGreen )
        m_pkTimeGreen->ToggleRender( false );
    if( m_pkTimeYellow )
        m_pkTimeYellow->ToggleRender( false );
    if( m_pkTimeRed )
        m_pkTimeRed->ToggleRender( false );

    if( m_bPeriodItem && m_pkTimeGreen && m_pkTimeYellow && m_pkTimeRed ) {
        m_pkTimeGreen->ToggleRender( cSpan.GetTotalHours() > 72 );
        m_pkTimeYellow->ToggleRender( cSpan.GetTotalHours() <= 72 && cSpan.GetTotalHours() > 1 );
        m_pkTimeRed->ToggleRender( cSpan.GetTotalHours() <= 1 );
    }

    int tmpID = pInventoryItem->m_ItemID;

    WCHAR szItemName[MAX_PATH] = { 0, };
    std::size_t len = std::wcslen( m_ItemInfo->strItemName.c_str() );
    for ( std::size_t i = 0; i < len; ++i )
    {
        szItemName[i] = m_ItemInfo->strItemName[i];
        if ( g_pkFontMgr->GetWidth( szItemName ) > (int)( GetWidth() * m_fWindowScaleX ) && i > 0 )
        {
            szItemName[i-1] = szItemName[i] = L'\0';
            break;
        }
    }

    std::wstring strItemGradeColor = g_pItemMgr->GetItemGradeColorKey( pInventoryItem->m_cGradeID );
    std::wstring strItemColorName = strItemGradeColor + szItemName + L"#cX";	

    if( m_pkItemName != NULL ) {
        m_pkItemName->SetForceWordWrapText( strItemColorName, true );
        m_pkItemName->SetFontColor( g_MyD3D->m_kItemMgr.GetItemStringColor( m_ItemInfo->eItemKind ) );
        m_pkItemName->SetShadow( true );
    }

    if( m_pkItemBar )
        m_pkItemBar->SetItem( m_ItemInfo );

    if( m_pkPriceNum != NULL ) {
#ifdef ENABLE_POINT_SYSTEM // Kawan>
        if( g_MyD3D->m_kItemMgr.IsPointShopItem( m_ItemInfo->dwGoodsID ) )
        {
            m_pkPriceNum->SetNumber( g_MyD3D->m_kItemMgr.GetPointItemPrice( m_ItemInfo->dwGoodsID ) );
        }
        else {
            m_pkPriceNum->SetNumber( m_ItemInfo->dwPrice );
        }
#else
        m_pkPriceNum->SetNumber(m_ItemInfo->dwPrice);
#endif
    }

    // ���� ��쿡 �� �̸� ǥ�� & �� ���� ǥ��
	KPetInfo* depotPetInfo = SiKGCDepotManager()->GetPetInfo(pInventoryItem);
    if( g_MyD3D->IsExistPetInfo( m_dwUID ) || depotPetInfo != NULL)
    {
        if( m_ItemInfo->eItemKind == GIK_PET && m_ItemInfo->ePetItemKind == EPET_KIND_PET )
        {

			if( g_MyD3D->IsExistPetInfo( m_dwUID ) )
			{
				*depotPetInfo = g_MyD3D->m_mapPetInfo[ m_dwUID ];
			}

			if( m_pkItemName != NULL )
			{
				int iLevel = GetPetLevel( depotPetInfo->m_dwEXP );
				std::wostringstream strmName;
				strmName << strItemGradeColor << g_pkStrLoader->GetString( STR_ID_LEVEL ) << " " << iLevel << " " << depotPetInfo->m_strName << L"#cX";

				m_pkItemName->SetForceWordWrapText( strmName.str(), true );
			}
		}
	}

    m_bNotSellItem = m_ItemInfo->bNotSellItem;

    // �Ӽ� �ɷ�ġ ����!
    ///////////////////////////////////////////////////////////////////////////////////////
    std::vector< KAttributeInfo > vecAttribute = pInventoryItem->m_vecAttribute;
    std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();

    char cEquipDecLvValue = 0;
    char cEquipWithBlacksmithPermit = 0;
    while (vecAttribute.end() != vitAttribute)
    {
        if ((*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN)
        {
            cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
        }
        ++vitAttribute;
    }

    int iNeedLevelForEquippingItem = g_pItemMgr->GetlItemEquipLevel(m_ItemInfo, pInventoryItem, NULL, true);

    if (g_pItemMgr->FindInventoryForItemID(ITEM_BLACKSMITH_PERMIT)) {
        cEquipWithBlacksmithPermit = 10;
    }
    else {
        cEquipWithBlacksmithPermit = 0;
    }

    if (GIK_MONSTER_CARD != m_ItemInfo->eItemKind && ESP_TITLE != m_ItemInfo->eItemType && g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel < (iNeedLevelForEquippingItem - cEquipDecLvValue) - cEquipWithBlacksmithPermit)
    {
        std::wostringstream strmText;
        strmText << g_pkStrLoader->GetString( STR_ID_NOT_EQUIP_LEVEL ) << iNeedLevelForEquippingItem  << ')';

        if( m_pkItemLimit != NULL )
            m_pkItemLimit->SetText( strmText.str() );

        if( m_pkLimitIcon != NULL )
            m_pkLimitIcon->ToggleRender( true );
    }
    else {
        if( m_pkItemLimit != NULL )
            m_pkItemLimit->SetText( L"" );

        if( m_pkLimitIcon != NULL )
            m_pkLimitIcon->ToggleRender( false );
    }

    if (m_pkImgCoordiCompose && m_pkImgCoordiUndo)
    {
        if (pInventoryItem->m_DesignCoordiID != 0)
        {
            if (g_pItemMgr->CheckEquipItem(&g_MyD3D->m_TempPlayer.m_kUserInfo, m_dwUID)) {
                m_pkImgCoordiUndo->ToggleRender(false);
            }
            else {
                m_pkImgCoordiUndo->ToggleRender(true);
            }
            m_pkImgCoordiCompose->ToggleRender(true);
        }
        else
        {
            m_pkImgCoordiUndo->ToggleRender(false);
            m_pkImgCoordiCompose->ToggleRender(false);
        }
    }

    ShowItem(true);
}

std::wstring KGCUserItemWnd::GetPetState( int iSatiation )
{
    if( iSatiation >= 1000 )
    {
        return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE3 );
    }
    else if( iSatiation >= 250 )
    {
        return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE2 );
    }
    else
    {
        return g_pkStrLoader->GetString( STR_ID_PET_HUNGRY_RATE_STATE1 );
    }
}

bool KGCUserItemWnd::IsShowConfirmButton( int iItemID )
{
    return SiKNeonSign()->IsServerNeonsign( iItemID ) ||
        g_MyD3D->m_kItemMgr.IsExistTradeItem(iItemID) ||
        g_pItemMgr->IsGPCoupon( m_iItemID ) ||
        iItemID == KGCItemManager::ITEM_CHANGE_NICK_NAME || 
        iItemID == KGCItemManager::ITEM_CHAR_LAS_CARD ||
        iItemID == KGCItemManager::ITEM_CHAR_LAS_CARD_CASH ||
        iItemID == KGCItemManager::ITEM_CHAR_JIN_CARD_CASH ||
        iItemID == KGCItemManager::ITEM_CHAR_SWORDMASTER_CARD ||
        iItemID == KGCItemManager::ITEM_CHANGE_PET_NICKNAME_CARD ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_60 ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_30 ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10 ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_2 ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_10_3 ||
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_ITEM_5 ||
        iItemID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH5000 ||
        iItemID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH3000 ||
        iItemID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_GP ||
        iItemID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_POINT ||
        iItemID == KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_GP ||
        iItemID == KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_POINT ||
        iItemID == KGCItemManager::ITEM_PET_HATCH_ITEM_GP ||
        iItemID == KGCItemManager::ITEM_RESET_WIN_LOSE_DATA ||
        iItemID == KGCItemManager::ITEM_BONUS_CHARGING_ITEM_POINT ||
        iItemID == KGCItemManager::ITEM_BONUS_CHARGING_ITEM_CASH || 
        iItemID == KGCItemManager::ITEM_CHAR_RYAN_CARD_CASH ||
        iItemID == KGCItemManager::ITEM_CHAR_RONAN_CARD_CASH ||
        iItemID == KGCItemManager::ITEM_CHAR_SIEG_CARD_CASH ||
        iItemID == KGCItemManager::ITEM_EASTER_EVENT_TREASURE_BOX ||
        iItemID == KGCItemManager::ITEM_FORTUNE_CAPSULE_ELA ||
        iItemID == KGCItemManager::ITEM_QUEENS_BOX || 
        iItemID == KGCItemManager::ITEM_PH_CBT_BOX ||
        iItemID == KGCItemManager::ITEM_WINTER_VACATION_BOMB_BOX1 ||
        iItemID == KGCItemManager::ITEM_WINTER_VACATION_BOMB_BOX2 ||
        iItemID == KGCItemManager::ITEM_WENDY_SECRET_BOX ||
        iItemID == KGCItemManager::ITEM_SEAL_BEAD_LEVEL1_JIN ||
        iItemID == KGCItemManager::ITEM_SEAL_BEAD_LEVEL2_JIN ||
        iItemID == KGCItemManager::ITEM_SEAL_BEAD_LEVEL3_JIN ||
        iItemID == KGCItemManager::ITEM_HALLOWEEN_DICE||
        iItemID == KGCItemManager::ITEM_VIRTUAL_CASH_COUPON0 ||
        iItemID == KGCItemManager::ITEM_VIRTUAL_CASH_COUPON1 ||
        iItemID == KGCItemManager::ITEM_VIRTUAL_CASH_COUPON2 || 
        iItemID == KGCItemManager::ITEM_VIRTUAL_CASH_COUPON3 ||
        iItemID == KGCItemManager::ITEM_VIRTUAL_CASH_COUPON4 ||
        iItemID == KGCItemManager::ITEM_BINGO_COIN||
        iItemID == KGCItemManager::ITEM_PUZZLE_PLATE||
        iItemID == KGCItemManager::ITEM_GACHA_COIN||
        iItemID == KGCItemManager::ITEM_GAWIBAWIBO_COIN||
        iItemID == KGCItemManager::ITEM_GAWIBAWIBO_RETRY||
        iItemID == KGCItemManager::ITEM_PROMOTION_MAGIC_1||
        iItemID == KGCItemManager::ITEM_PROMOTION_MAGIC_2||
        iItemID == KGCItemManager::ITEM_PROMOTION_MAGIC_3||
        iItemID == KGCItemManager::ITEM_RESET_SKILL || 
        iItemID == KGCItemManager::ITEM_MAGICBOX_GAME_BOARD ||
        iItemID == KGCItemManager::ITEM_OPEN_CHANGE_SKILL_SLOT ||
        g_pItemMgr->IsExpItemList(iItemID) ||
		iItemID == KGCItemManager::ITEM_SONGKRAN_WATER_BOMB ||		
        g_pItemMgr->IsGachaList(iItemID) || 
        SiKGCLevelResetEventManager()->IsResetItem( iItemID ) ||
        iItemID == KGCItemManager::ITEM_RITAS_CHRISTMAS_CONTINUE_COIN_EXCHANGE_ITEMID || 
#if defined ( USE_VIP_POINT)
        g_pItemMgr->IsVPCoupon( m_iItemID ) ||
#endif
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_10 || 
        iItemID == KGCItemManager::ITEM_EXTEND_INVENTORY_EVENT_ITEM_60 ||
        g_pItemMgr->IsLookExtendItem( iItemID ) ||
        iItemID == ITEM_VIP_TEMPORARY_ITEM ||
        g_pItemMgr->IsSkillScroolList( iItemID );
}	

bool KGCUserItemWnd::IsShowCraftingButton( int iItemID )
{
    return iItemID == KGCItemManager::ITEM_NIGHTMARKET;
}

void KGCUserItemWnd::OnPlayButton()
{
    //�̴� ������ ������ ���� ó������
    switch(m_iItemID)
    {
    case KGCItemManager::ITEM_HALLOWEEN_BOARD:
        {
            Result_HalloweenBoardGame = INT_MAX;

            KP2P::GetInstance()->Send_BoardGameInfoReq();
            g_MyD3D->WaitForServerAck( Result_HalloweenBoardGame, INT_MAX, 5000, TIME_OUT_VALUE );

            if( Result_HalloweenBoardGame == 0 || Result_HalloweenBoardGame == 2 || Result_HalloweenBoardGame == 3 )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_HALLOWEEN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL);
            break;
        }
    case KGCItemManager::ITEM_BINGO_BOARD:
        {
            if( Result_BingoBoardGame == 0  )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BINGO_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
            else if(Result_BingoBoardGame == 1) //���� ���� �̺�Ʈ �Ⱓ�� �ƴ�.
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_BINGO_ERROR1),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
           break;
        }
    case KGCItemManager::ITEM_GAWIBAWIBO_BOARD:
        {
            Result_GawibawiboGameInfo = INT_MAX;
            if(g_pkUIScene->m_pkGawibawiboGame == NULL)
                break;
            KP2P::GetInstance()->Send_GetUserGwibawiboDataReq(g_pkUIScene->m_pkGawibawiboGame->GetVerInfo());
            g_MyD3D->WaitForServerAck( Result_GawibawiboGameInfo, INT_MAX, 5000, TIME_OUT_VALUE );
            break;
        }
    case KGCItemManager::ITEM_PLANTTREE_BOARD:
        {
            if(g_pkUIScene->m_pkPlantTree == NULL)
                break;
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
                break;

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PLANT_TREE_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
            break;
        }
    case KGCItemManager::ITEM_SPHINX_BOARD:
        {
            if(g_pkUIScene->m_pkSphinxDlg == NULL)
                break;
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
                break;

            Result_SphinxReq = INT_MAX;
            KP2P::GetInstance()->Send_UserSphinxDataReq();
            g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
            break;
        }
    }
    
}

void KGCUserItemWnd::OnSocketButton()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_SOCKET_MANAGEMENT_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwUID );
}

void KGCUserItemWnd::OnAttributeButton()
{
    KItem* kItemInfo = GetItemInfo();

    if( kItemInfo == NULL )
    {
        ASSERT( !"�������� �� ����!!!" );
        return;
    }

    KP2P::GetInstance()->Send_AttributeSelectListReq(kItemInfo->m_ItemUID);
    int temp;
    g_MyD3D->WaitForServerAck( temp, -1, 5000, -1 );
    g_pkUIScene->m_pkAttributeSelectBox->SetItemUID(m_dwUID);
    return;
}

void KGCUserItemWnd::OnRandomInitAttributeButton(const KActionEvent& event)
{
    // �ʱ�ȭ �������� ������ �ִ��� Ȯ���ؾ� ��.
    KItem* kItemInfo = GetItemInfo();
    if( kItemInfo == NULL )
    {
        ASSERT( !"�������� �� ����!!!" );
        return;
    }

    //random_attribute_item : ���� �Ӽ� �������� ���� �Ǿ����� - ���������.
    if(g_pItemMgr->CheckEquipItem(kItemInfo->m_ItemUID))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP1 ), g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,true,true );
        return;
    }


    // Ÿ�� ���� UI
    if( g_pkUIScene->m_pkMyInfoScene->m_pkPopopAttributeSelect->IsRenderOn() ) { 
        g_pkUIScene->m_pkMyInfoScene->m_pkPopopAttributeSelect->ToggleRender( false );
        return;
    }

    bool bMigration = false;

#if defined( USE_ATTRIBUTE_SELECT_EVENT )
    // ���� üũ 
    Result_AttributeMigration = INT_MAX;
    KP2P::GetInstance()->Send_AttributeMigrationInfoReq( kItemInfo->m_ItemUID );
    g_MyD3D->WaitForServerAck( Result_AttributeMigration, INT_MAX, 5000, TIME_OUT_VALUE );

    bMigration = ( 0 == Result_AttributeMigration && g_kGlobalValue.IsEnableAttributeMigration() );
#endif

    const float fOffsetX = 20.0f;
    const float fOffsetY = -47.0f;
    D3DXVECTOR2 dxvPos = m_pkRandomInitAttributeButton->GetFixedWindowPos();
    dxvPos.x += ( fOffsetX * m_fWindowScaleX );
    dxvPos.y += ( fOffsetY * m_fWindowScaleY );

    g_pkUIScene->m_pkMyInfoScene->m_pkPopopAttributeSelect->ShowPopupAttributeSelect( kItemInfo->m_ItemUID, dxvPos, bMigration );

    return;
}

void KGCUserItemWnd::OnItemLevelDownClick()
{
    // �ʱ�ȭ �������� ������ �ִ��� Ȯ���ؾ� ��.
    KItem* kItemInfo = GetItemInfo();
    if( kItemInfo == NULL )
    {
        return;
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_LEVEL_DOWN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kItemInfo->m_ItemUID, 0 );
}

bool KGCUserItemWnd::SetEnterCursor()
{
    if( m_pkActiveFrame )
        m_pkActiveFrame->ToggleRender( true );
    return true;
}

bool KGCUserItemWnd::SetLeaveCursor()
{
    if( m_pkActiveFrame )
        m_pkActiveFrame->ToggleRender( false );
    return true;
}

void KGCUserItemWnd::OnSellStart()
{
    m_bSellState = true;
    UpdateSellIcons();
}

void KGCUserItemWnd::OnSellEnd()
{
    m_bSellState = false;
    UpdateSellIcons();
    ShowItem( true );
}

void KGCUserItemWnd::UpdateSellIcons()
{
    if( m_bSellState == false || m_dwUID == 0 || m_ItemInfo == NULL ) {
        if( m_pkNowSelling )
            m_pkNowSelling->ToggleRender( false );
        if( m_pkCantSell )
            m_pkCantSell->ToggleRender( false );
        if( m_pkNowRemove )
            m_pkNowRemove->ToggleRender( false );
        if( m_pkNowEquip )
            m_pkNowEquip->ToggleRender( false );
        if( m_pkSelected )
            m_pkSelected->ToggleRender( false );
        return;
    }

    if( m_pkNowSelling ) {
        if( g_pItemMgr->IsContainSellItemList( m_dwUID ) && g_pItemMgr->IsSellAvailableItem( m_dwUID )) {
            m_pkNowSelling->ToggleRender( true );
        }
        else {
            m_pkNowSelling->ToggleRender( false );
        }
    }

    if( m_pkNowRemove ) {
        if( g_pItemMgr->IsContainSellItemList( m_dwUID ) && g_pItemMgr->IsRemoveAvailableItem( m_dwUID )) {
            m_pkNowRemove->ToggleRender( true );
        }
        else {
            m_pkNowRemove->ToggleRender( false );
        }
    }

    if( m_pkCantSell ) {
        if( g_pItemMgr->IsSellAvailableItem( m_dwUID ) == false &&
            g_pItemMgr->IsRemoveAvailableItem( m_dwUID ) == false ) {
                m_pkCantSell->ToggleRender( true );
        }
        else {
            m_pkCantSell->ToggleRender( false );
        }
    }

    if( m_pkNowEquip ) {
        if( g_pItemMgr->CheckEquipItem( &g_MyD3D->m_TempPlayer.m_kUserInfo, m_dwUID ) ) {
            m_pkNowEquip->ToggleRender( true );
            m_pkCantSell->ToggleRender(false);
            m_pkNowSelling->ToggleRender(false);
        }
        else {
            m_pkNowEquip->ToggleRender( false );
            
            
        }
    }
}

void KGCUserItemWnd::SetGradeFrame( IN char cGradeID_ )
{
    if( cGradeID_ < 0 || cGradeID_ >= KItem::GRADE_NUM )
        cGradeID_ = KItem::GRADE_NORMAL;

    for( int i = 0 ; i < KItem::GRADE_NUM ; ++i ) {
        if( m_pkGradeFrame[i] == NULL )
            continue;

        m_pkGradeFrame[i]->ToggleRender( i == cGradeID_ );
    }
}

bool KGCUserItemWnd::IsCursorRollover()
{
    if( m_pkActiveFrame )
        return m_pkActiveFrame->IsRenderOn();

    POINT   pt = g_pkInput->GetMousePos();
    return CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );
}

void KGCUserItemWnd::UsePromotionMagic( GCITEMID ItemID_, int iPromotionLevel )
{
    KEVENT_ITEM_CHARPROMOTION_LEVEL_NOT::iterator mIter = g_kGlobalValue.m_mapItemCharpromotionLevel.find( ItemID_*10 );
    if(mIter ==  g_kGlobalValue.m_mapItemCharpromotionLevel.end() ) 
        return;

    // ���� ������ ĳ���Ͱ� �ִ��� Ȯ��
    std::map<char, DWORD> &mIterCharPromotion = mIter->second;
    SCharInfo& sCharInfo = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();
    std::map<char, DWORD>::iterator mIterLevel = mIterCharPromotion.find(static_cast<char>(sCharInfo.iCharType));



    if(  sCharInfo.iCharType, iPromotionLevel < iPromotionLevel ||                                                                     // ���� ���������� ������ �������� ������ Ȯ��
        !g_kGlobalValue.IsOpenedPromotion( sCharInfo.iCharType, iPromotionLevel ) ||             // ������ ������ ���� ĳ���� �ִ� ���� �������� ������ Ȯ��
        mIterLevel == mIterCharPromotion.end() ||                                            // ������������ �ش�ĳ���� ������ ã�Ҵ��� Ȯ��
        mIterLevel->second > static_cast<DWORD>(sCharInfo.iLevel) )                          // ĳ���� ������ �������� �������� ū�� Ȯ��
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_MAGIC_PROMOTION_3 ), L"");
        return;
    }

    g_kGlobalValue.m_iAMPCharType = sCharInfo.iCharType;
    g_kGlobalValue.m_iAMPPromotion = iPromotionLevel;
    g_kGlobalValue.m_iAMPItemID = (ItemID_*10);

    // �����Ұ����� Ȯ�ιޱ�
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
        g_pkStrLoader->GetReplacedString( STR_ID_MAGIC_PROMOTION_1, "s", GCFUNC::GetCharName( sCharInfo.iCharType ).c_str() ), 
        g_pkStrLoader->GetString( STR_ID_MAGIC_PROMOTION_2 ),
        KGCUIScene::GC_MBOX_USE_MAGIC_PROMOTION );
}

void KGCUserItemWnd::SetItemDuration( int iDuration )
{
    m_iDuration = iDuration;
    if( m_pkDurationNum != NULL ) {
        if( m_iDuration > -1 ) {
            m_pkDurationNum->ToggleRender( true );
            m_pkDurationNum->SetNumber( m_iDuration );
        }
        else {
            m_pkDurationNum->ToggleRender( false );
        }
    }
    if( m_pkSmallDurationNum != NULL ) {
        if( m_iDuration > -1 ) {
            m_pkSmallDurationNum->ToggleRender( true );
            m_pkSmallDurationNum->SetNumber( m_iDuration );
        }
        else {
            m_pkSmallDurationNum->ToggleRender( false );
        }
    }
    
}

void KGCUserItemWnd::OnCreateComplete( void )
{
    ConnectEventProcedure( D3DWE_CURSOR_ENTER, this, &KGCUserItemWnd::SetEnterCursor );
    ConnectEventProcedure( D3DWE_CURSOR_LEAVE, this, &KGCUserItemWnd::SetLeaveCursor );
    if(m_pkRandomInitAttributeButton){
        m_pkRandomInitAttributeButton->DisconnectAllEventProcedure();
        m_pkRandomInitAttributeButton->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_DOWN,this,&KGCUserItemWnd::OnRandomInitAttributeButton);
    }
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_START, this, &KGCUserItemWnd::OnSellStart );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_END, this, &KGCUserItemWnd::OnSellEnd );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_ADD, this, &KGCUserItemWnd::UpdateSellIcons );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_REMOVE, this, &KGCUserItemWnd::UpdateSellIcons );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE, this, &KGCUserItemWnd::SetUpdateBtn );
}

bool KGCUserItemWnd::SetAttributeBtn( KItem* pInven )
{
    if( NULL == pInven){
        if( m_pkAttributeButton )
            m_pkAttributeButton->ToggleRender( false );
        return false;
    }

    for( int j = 0; j <  (int)pInven->m_vecAttribute.size(); j++ )
    {
        if( pInven->m_vecAttribute[j].m_cState == static_cast<char>(KAttributeInfo::ATS_OPENED) )
        {
            if( m_pkAttributeButton ){
                m_pkAttributeButton->ToggleRender( true );
                return true;
            }
            return false;
        }
    }
    if(m_pkAttributeButton)
        m_pkAttributeButton->ToggleRender( false );

    return false;
}

// ���� ���� ��ư�� ��Ÿ������ ���Ϲ�ư�� ��Ÿ��������....
void KGCUserItemWnd::SetItemBtn(int state_)
{
    KItem* pInven = NULL;

    if(m_pkAttributeButton)
        m_pkAttributeButton->ToggleRender( false );
    if(m_pkRandomInitAttributeButton)
        m_pkRandomInitAttributeButton->ToggleRender( false );
    if( m_pkSocketButton )
        m_pkSocketButton->ToggleRender( false );

    if ( m_pkLevelDownButton ) 
    {
        m_pkLevelDownButton->ToggleRender( false );
        m_pkLevelDownOver->ToggleRender( false );
    }

	if (m_pkBtnEquip)
	{
		m_pkBtnEquip->ToggleRender(false);
	}

    if( g_pItemMgr->FindInventory( m_dwUID, &pInven) && m_bShow )
    {
		if(state_ == IBS_DEFAULT)
        {
            state_ = g_pkUIScene->m_pkMyInfoScene->GetItemBtnState();
        }

        if ( GC_GLOBAL_DEFINE::bReformCoordi && pInven->CheckItemType( KItem::TYPE_LOOK ) )
            return;

        bool bLevelDown = g_pkUIScene->m_pkMyInfoScene->GetLevelDownState();
		
		bool bIsPetGlyph = SiGCPetMagicStone()->IsPetGlyph(pInven->m_ItemID);

		if (bIsPetGlyph)
		{
			SetEquipBtn(pInven);
		}
        else if ( bLevelDown ) 
        {
            SetLevelDownBtn(pInven);
            UpdateButton( false );
        }
        else
        {
            if(SetAttributeBtn(pInven));	//random_attribute_item : ���� �Ӽ� ���� ��ư�� ��Ÿ�� �� �ִ���.
            else if( SetRandomInitAttributeBtn(pInven,state_) );//random_attribute_item : ���� ���� �Ӽ� ���� ��ư Ȱ��.
			else {
				SetSocketBtn(pInven); //random_attribute_item : �⺻ ���� - ���� ��ư
			}

            UpdateButton( true );
        }
    }
}

bool KGCUserItemWnd::SetSocketBtn( KItem* pInven )
{
    if( NULL == pInven || pInven->m_vecSocket.empty() )
    {
        if( m_pkSocketButton )
            m_pkSocketButton->ToggleRender( false );
        return false;
    }

    if( m_pkSocketButton )
    {
        m_pkSocketButton->SetWndMode( D3DWM_DEFAULT );
        m_pkSocketButton->ToggleRender( m_bShow );
        return true;
    }
    
    return false;
}

bool KGCUserItemWnd::SetRandomInitAttributeBtn( KItem* pInven, int state_ )
{
#if defined( NATION_CHINA ) 
	return false;
#endif

    if(m_pkRandomInitAttributeButton == NULL) 
        return false;

    //random_attribute_item : ĳ����, ���� �� �Ⱓ���� ����
    if( state_ == IBS_RANDOM_INIT_ATTRIBUTE && 
#if !defined( USE_GP_ITEM_ATTRIBUTE_SELECT ) 
        g_pItemMgr->IsCashItem(pInven->m_ItemID) &&
#endif
        pInven->m_nCount == -1  &&
        pInven->m_nPeriod == -1 &&
        pInven->m_vecAttribute.empty() == false &&
        pInven->CheckItemType(KItem::TYPE_LOOK) == false )
    {
        m_pkRandomInitAttributeButton->ToggleRender(true);
        return true;
    }
    
    m_pkRandomInitAttributeButton->ToggleRender(false);
    return false;
}

bool KGCUserItemWnd::SetLevelDownBtn( KItem* pInven )
{
    if ( !m_pkLevelDownButton ) 
        return false;

    if ( g_pItemMgr->IsLevelDownItem( pInven ) ) 
    {
        m_pkLevelDownButton->ToggleRender( true );
        m_pkLevelDownOver->ToggleRender( false );
        return true;
    }
    else
    {
        m_pkLevelDownButton->ToggleRender( false );
        m_pkLevelDownOver->ToggleRender( true );
        return false;
    }
}

bool KGCUserItemWnd::SetEquipBtn(KItem* pInven)
{
	if (!m_pkBtnEquip || !pInven)
	{
		return false;
	}

	m_pkBtnEquip->ToggleRender(true);
	return true;
}

void KGCUserItemWnd::SetUpdateBtn( const KActionEvent& event )
{
    SetItemBtn(static_cast<int>(event.m_dwlParam));
}


void KGCUserItemWnd::UpdateSelectIcon()
{
    if (!m_pkSelected ) 
        return;
 
    m_pkSelected->SetWndMode( D3DWM_DEFAULT );
    m_pkSelected->ToggleRender( m_bSelectState );
    if ( m_bSelectState ) {
        SetEnterCursor();
    }
    else {
        SetLeaveCursor();
    }
}

