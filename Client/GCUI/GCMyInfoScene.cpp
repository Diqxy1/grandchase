#include "stdafx.h"
//
#include ".\gcmyinfoscene.h"
//
#include "../MyD3D.h"

//

//

#include "GCUI/GCUserItemBox.h"

#include "GCUI/GCInfoPanel.h"
#include "GCUI/KGCStatusInfoPanel.h"
#include "GCUI/GCCharWnd.h"
#include "GCUI/GCShopTab.h"
#include "GCUI/GCItemInformationBoxPair.h"
//
#include "GCUI/KGCUIHelper.h"
//

#include "GCPetWnd.h"
#include "gcui/GCPetInfoBox.h"
#include "GCUI/GCInventoryCapacity.h"
#include "gcui/GCInventoryFullMsg.h"
#include "gcui/KGCWorldMapContainer.h"
//



#include "gcui/KGCTitleWindow.h"
#include "../BuddySystem/KFriendShip.h"
#include "../GCStateMachine.h"

#include "gcui/GCInventoryFullMsg.h"
#include "GCSellInfo.h"

#include "GCSearchBar.h"
#include "GCMyInfoSceneSubMenu.h"
#include "KGCWeaponChangeBox.h"

#include "KGCAddItemInClient.h"

#include "GCItemPopupMenu.h"
#include "GCSellCountItemDlg.h"

#include "./KGCDepotManager.h"

#include "GCStateGame.h"
#include "GCPetMagicStone.h"

IMPLEMENT_CLASSNAME(KGCMyInfoScene);
IMPLEMENT_WND_FACTORY(KGCMyInfoScene);
IMPLEMENT_WND_FACTORY_NAME(KGCMyInfoScene, "gc_my_info_scene");

KGCMyInfoScene::KGCMyInfoScene(void)
    : m_iItemBtnState(0)
    , m_pkMyinfoSceneSubMenu(NULL)
    , m_pkItemPopupMenu(NULL)
{
    LINK_CONTROL("inventory_item_box", m_pkUserItemBox);
    LINK_CONTROL("sell_item_box", m_pkSellItemBox);
    LINK_CONTROL("myinfo_item_box", m_pkUserEquipBox);
    LINK_CONTROL("user_info_view", m_pkInfoPanel);

    for (int i = 0; i < UEC_NUM; i++)
    {
        m_apEquipTab[i] = NULL;
    }

    LINK_CONTROL("armor_tab", m_apEquipTab[UEC_EQUIP1]);
    LINK_CONTROL("accessory_tab", m_apEquipTab[UEC_EQUIP2]);
    LINK_CONTROL("shortcut_tab", m_apEquipTab[UEC_SHORTCUT_SLOT]);
    LINK_CONTROL("pet_tab", m_apEquipTab[UEC_PET]);
    LINK_CONTROL("skill_tab", m_apEquipTab[UEC_SKILL]);
    LINK_CONTROL("skill_tab1", m_apEquipTab[UEC_SKILL_WHITE]);
    LINK_CONTROL("skill_tab2", m_apEquipTab[UEC_SKILL_BLACK]);
    LINK_CONTROL("look_tab", m_apEquipTab[UEC_LOOK]);


    LINK_CONTROL("itemlist_combobox", m_pkItemGroupCombo);
    //LINK_CONTROL("mission_dlg", m_pkMissionDlg );

    LINK_CONTROL("inventory_capacity", m_pkInventoryCapacity);

    char strTemp[MAX_PATH];
    for (int i = 0; i < UIC_NUM; i++)
    {
        m_apInvenTab[i] = NULL;
        sprintf(strTemp, "inven_tab_%d", i + 1);
        LINK_CONTROL(strTemp, m_apInvenTab[i]);
    }
    m_EquipCategory = UEC_EQUIP1;
    m_iItemGroupType = 0;

    m_pkEmoticonWindowButton = NULL;
    LINK_CONTROL("emoticon_button", m_pkEmoticonWindowButton);

    m_pkWelcomeBack_btn = NULL;
    m_pkBtnLevelDown = NULL;
    m_pkBtnLevelDownOn = NULL;
    LINK_CONTROL("welcomeback_btn", m_pkWelcomeBack_btn);
#if defined (USE_LEVEL_DOWN)
    LINK_CONTROL("btn_level_down", m_pkBtnLevelDown);
    LINK_CONTROL("btn_level_down_on", m_pkBtnLevelDownOn);
#endif


    m_bShowUserInfo = false;
    m_bShowStatusInfo = false;
    m_pkBtn_Sell = NULL;
    m_pkBtn_Ordering_Level = NULL;
    m_pkBtn_Ordering_Parts = NULL;
    m_pkSellInfo = NULL;
    m_pkSearchBar = NULL;

    LINK_CONTROL("user_status_info_view", m_pkStatusPanel);

    for (int i = 0; i < TAB_NUM; ++i)
    {
        m_pkInventoryTab[i] = NULL;
    }

    LINK_CONTROL("tab_inventory", m_pkInventoryTab[TAB_INVENTORY]);
    LINK_CONTROL("tab_otheritem", m_pkInventoryTab[TAB_OTHERITEM]);
    LINK_CONTROL("tab_extraitem", m_pkInventoryTab[TAB_EXTRAITEM]);
    LINK_CONTROL("tab_otheritem2", m_pkInventoryTab[TAB_OTHERITEM2]);
    LINK_CONTROL("btn_sell", m_pkBtn_Sell);
    LINK_CONTROL("btn_ordering_level", m_pkBtn_Ordering_Level);
    LINK_CONTROL("btn_ordering_parts", m_pkBtn_Ordering_Parts);
    LINK_CONTROL("user_sell_info", m_pkSellInfo);
    LINK_CONTROL("search_bar", m_pkSearchBar);

    LINK_CONTROL("sub_menu", m_pkMyinfoSceneSubMenu);

    m_pkBntUserInfo = NULL;
    LINK_CONTROL("user_info_button", m_pkBntUserInfo);

    m_pkBntUserStatus = NULL;
    LINK_CONTROL("user_status_button", m_pkBntUserStatus);

    m_pkCharWnd = NULL;
    LINK_CONTROL("character_window", m_pkCharWnd);

    m_pkPopopAttributeSelect = NULL;
    LINK_CONTROL("popup_attribute_select", m_pkPopopAttributeSelect);

    LINK_CONTROL("item_popup", m_pkItemPopupMenu);

    LINK_CONTROL("exit_from_myinfo_room", m_pkBtnExitFromMyinfoRoom);

    LINK_CONTROL("worldbillboardchat", m_pkBillBoardChatBoard);

    //Sistema de Skin de Dano
    m_pkBtnFontSkin = NULL;
    LINK_CONTROL("btn_font_look", m_pkBtnFontSkin);
}

KGCMyInfoScene::~KGCMyInfoScene(void)
{
}

void KGCMyInfoScene::OnCreate()
{
    KGCScene::OnCreate();

    m_pkItemGroupCombo->AddActionListener(this);

    m_pkCharWnd->AddActionListener(this);

    for (int i = 0; i < UEC_NUM; i++)
    {
        if (m_apEquipTab[i])
        {
            m_apEquipTab[i]->InitState(true, true, this);

        }
    }

    for (int i = 0; i < UIC_NUM; i++)
    {
        if (m_apInvenTab[i])
        {
            m_apInvenTab[i]->InitState(false);
        }
    }


#if defined( EMOTICON_SYSTEM )
    m_pkEmoticonWindowButton->InitState(true, true, this);
#else
    m_pkEmoticonWindowButton->InitState(false, true, this);
#endif


#ifdef WELCOMEBACK_FRIEND
    m_pkWelcomeBack_btn->InitState(true, true, this);
#else
    m_pkWelcomeBack_btn->InitState(false);
#endif

#if defined (USE_LEVEL_DOWN)
    if (GC_GLOBAL_DEFINE::bRelayHeroDungeon)
    {
        m_pkBtnLevelDown->InitState(true, true, this);
        m_pkBtnLevelDownOn->InitState(false, true, this);
    }
#endif

    if (m_pkSearchBar != NULL)
        m_pkSearchBar->InitState(true, true, this);

    if (KP2P::GetInstance()->IsConnectedToGameServer() == true)
    {
        m_pkUserEquipBox->LockEquipment(false);
        ActiveInventoryTab(TAB_INVENTORY);
    }
    //m_pkMissionDlg->SetSelfInputCheck( false );

    // define.h 에 신기술 사용가능한 나라등록하세요

    SetInfoButtonState(true, false);

    //아이템의 탭중 인벤토리 탭을 활성화 시킵니다.
    for (int i = 0; i < TAB_NUM; ++i)
    {
        if (m_pkInventoryTab[i] != NULL)
            m_pkInventoryTab[i]->InitState(true, true, this);
    }
    m_pkInventoryTab[TAB_OTHERITEM2]->ToggleRender(false);
    m_pkInventoryTab[TAB_OTHERITEM2]->SetToolTip(g_pkStrLoader->GetString(STR_ID_NOCHAR_ITEM_TAP));

    m_pkInventoryTab[TAB_INVENTORY]->Activate(true);
    m_pkInventoryTab[TAB_OTHERITEM]->Activate(false);
    m_pkInventoryTab[TAB_OTHERITEM]->SetToolTip(g_pkStrLoader->GetString(STR_ID_NOCHAR_ITEM_TAP));

    if (m_pkBtn_Sell) {
        m_pkBtn_Sell->InitState(true, true);
        m_pkBtn_Sell->DisconnectAllEventProcedure();
        m_pkBtn_Sell->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickSell_OpenSellUI);
    }

    m_bSortByLevel = true;
    if (m_pkBtn_Ordering_Level) {
        m_pkBtn_Ordering_Level->InitState(false, true, this);
        m_pkBtn_Ordering_Level->DisconnectAllEventProcedure();
        //m_pkBtn_Ordering->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickSell_OpenSellUI );        
    }

    if (m_pkBtn_Ordering_Parts) {
        m_pkBtn_Ordering_Parts->InitState(false, true, this);
        m_pkBtn_Ordering_Parts->DisconnectAllEventProcedure();
        //m_pkBtn_Ordering->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickSell_OpenSellUI );
    }

    if (m_pkSellInfo) {
        m_pkSellInfo->InitState(false, true);
        m_pkSellInfo->DisconnectAllEventProcedure();
        m_pkSellInfo->ConnectEventProcedure(KGCUIScene::D3DWE_SELL_ITEM, this, &KGCMyInfoScene::OnClickSellBtn);
        m_pkSellInfo->ConnectEventProcedure(KGCUIScene::D3DWE_SELL_EXIT, this, &KGCMyInfoScene::OnClickSell_CloseSellUI);
    }

    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_ADD, this, &KGCMyInfoScene::OnRefreshSellItemList);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_REMOVE, this, &KGCMyInfoScene::OnRefreshSellItemList);

    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_DLG_OPEN_START, this, &KGCMyInfoScene::OnClickSell_OpenSellUI);

    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_EXIT, this, &KGCMyInfoScene::OnClickSell_CloseSellUI);


    m_iItemBtnState = 0;

    if (m_pkMyinfoSceneSubMenu)
        m_pkMyinfoSceneSubMenu->InitState(true, true, NULL);

    m_pkBntUserInfo->InitState(true, true, this);
    m_pkBntUserStatus->InitState(true, true, this);

    //m_pkCharWnd->SetUseRoomObject(true);

    if (m_pkPopopAttributeSelect) {
        m_pkPopopAttributeSelect->InitState(false, true, this);
    }

    m_pkItemPopupMenu->InitState(true, true, this);
    m_pkItemPopupMenu->ToggleRender(false);

    m_pkBtnExitFromMyinfoRoom->InitState(false, true, this);

    m_pkBillBoardChatBoard->InitState(true, true, this);
    m_pkBillBoardChatBoard->SetBlink(false);

    //Sistema de Skin de Dano
    if (m_pkBtnFontSkin)
        m_pkBtnFontSkin->InitState(true, true, this);

}

void KGCMyInfoScene::OnCreateComplete(void)
{

    m_pkUserItemBox->AddActionListener(this);
    m_pkUserItemBox->DisconnectAllEventProcedure();
    m_pkUserItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnClickInventoryItem);
    m_pkUserItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_RCLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnRClickInventoryItem);

    m_pkUserItemBox->SetCols(7);
    m_pkUserItemBox->SetRows(3);
    m_pkUserItemBox->SetXGap(0);
    m_pkUserItemBox->SetYGap(0);

    m_pkUserItemBox->CreateChildWndList();
    m_pkUserItemBox->CalcuateWndPosition();

    m_pkSellItemBox->InitState(false, true);
    m_pkSellItemBox->DisconnectAllEventProcedure();
    m_pkSellItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnClickSellItem);
    m_pkSellItemBox->SetCols(8);
    m_pkSellItemBox->SetRows(3);
    m_pkSellItemBox->SetXGap(1);
    m_pkSellItemBox->SetYGap(1);

    m_pkSellItemBox->CreateChildWndList();
    m_pkSellItemBox->CalcuateWndPosition();

    //m_pkSellItemBox->SetAnimScroll( false );
    m_pkSellItemBox->SetSorting(false);

    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE, this, &KGCMyInfoScene::SetItemBtnState);

    //Sistema de Skin de Dano
    if (m_pkBtnFontSkin)
        m_pkBtnFontSkin->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickBtnFontSkin);

}

void KGCMyInfoScene::OnDestroy()
{
}

void KGCMyInfoScene::OnInitialize()
{
    m_bLevelDown = false;
#if defined (USE_LEVEL_DOWN)
    if (GC_GLOBAL_DEFINE::bRelayHeroDungeon)
    {
        m_pkBtnLevelDown->ToggleRender(true);
        m_pkBtnLevelDownOn->ToggleRender(false);
    }
#endif

    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
    UpdateGameData(true, UEC_EQUIP1);

    m_pkUserEquipBox->LockEquipment(false);
    ActiveInventoryTab(TAB_INVENTORY);

    // 능력치 정보 초기 데이터 
    m_pkStatusPanel->SetStartUserInfo();
    if (m_pkSearchBar != NULL)
        m_pkSearchBar->Init();

    if (m_pkMyinfoSceneSubMenu) {
        if (m_EquipCategory != UEC_LOOK && GetNowInventoryTab() != TAB_OTHERITEM)
            m_pkMyinfoSceneSubMenu->InitSubMenu();
        else {
            m_pkMyinfoSceneSubMenu->ButtonRender(false, false);
        }
    }

    m_pkCharWnd->UpdateMeshObject();
    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
}

void KGCMyInfoScene::OnEnterScene()
{
    m_iItemGroupType = 0;
    m_pkCharWnd->UpdateMeshObject();
    UpdateGameData();
    ShowItemEffect(true);

    if (g_pkUIScene->m_pkTitleCollection->IsCompleteCTMissionAndNotGet())
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_TITLE_COLLECTION, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);
    }

    SetSelfInputCheck(true);
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender(false);
    g_pkUIScene->EnableMessageBox(true);

    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect(D3DXVECTOR2(407.f, 327.0f));
    g_pkUIScene->m_pkInventoryFullMsg->ToggleRender(true);

    if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO) {
        SetButtonRender(BTN_WORLDMAP, true);
#if defined ( GUILD_RENEW_S4 )
        if (g_kGlobalValue.ServerInfo.CheckServerType(ST_GUILD)) {
            SetButtonRender(BTN_GUILD_MATCH, true);
            SetButtonRender(BTN_MATCH, false);
        }
        else {
            SetButtonRender(BTN_GUILD_MATCH, false);
            SetButtonRender(BTN_MATCH, true);
        }
#else
        SetButtonRender(BTN_MATCH, true);
#endif
        SetButtonRender(BTN_SHOP, true);

#if defined USE_COORDI_SHOP
        SetButtonRender(BTN_COORDI_SHOP, true);
#endif
        SetButtonRender(BTN_SKILL_TREE, true);

        SetButtonRender(BTN_MY_INFO, true);

        m_apkButton[BTN_MY_INFO]->Lock(true);
        m_pkBtnExitFromMyinfoRoom->ToggleRender(false);

    }
    else if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM) {
        SetButtonRender(BTN_WORLDMAP, false);
        SetButtonRender(BTN_MATCH, false);
        SetButtonRender(BTN_SHOP, false);
        SetButtonRender(BTN_SKILL_TREE, false);
#if defined ( GUILD_RENEW_S4 )
        SetButtonRender(BTN_GUILD_MATCH, false);
#endif
#if defined USE_COORDI_SHOP
        SetButtonRender(BTN_COORDI_SHOP, false);
#endif
        SetButtonRender(BTN_MY_INFO, false);
        m_pkBtnExitFromMyinfoRoom->ToggleRender(true);
    }

    if (m_pkSellItemBox && m_pkBtn_Sell && m_pkSellItemBox->IsRenderOn()) {
        OnClickSell_CloseSellUI(KActionEvent(m_pkBtn_Sell, KD3DWnd::D3DWE_BUTTON_CLICK));
    }
}

void KGCMyInfoScene::OnLeaveScene()
{
    m_pkUserEquipBox->m_pkPetWnd->m_pkPet->EndPet();
    m_pkCharWnd->DeleteMeshObject();
    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

    g_pkUIScene->OpenCollectionWindow();
    g_pkUIScene->CloseCollectionWindow();
    g_pkUIScene->RemoveWaitMessageBox(KGCUIScene::GC_MBOX_EMOTICON_COLLECTION);

    g_pkUIScene->OpenCollectionWindow();
    g_pkUIScene->CloseCollectionWindow();

    ShowItemEffect(false);
}

void KGCMyInfoScene::UpdateGameData(bool bSaveEquip, USER_EQUIP_CATEGORY eEquipCategory_, int iScrollPos /*= 0*//*, USER_INVEN_CATEGORY eInvenCategory_ */)
{
    if (g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO
        && g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO_FROM_ROOM
        && g_MyD3D->m_pStateMachine->GetState() != GS_MATCH) {
        return;
    }

    if (bSaveEquip)
    {
        //장비가 바뀌었을때만 패킷 던짐
        //서버에 패킷을 최소한만 던지도록..
        g_MyD3D->m_OriginalEquip = g_MyD3D->m_TempPlayer.m_kUserInfo;

    }

    g_MyD3D->m_TempPlayer.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    g_MyD3D->m_mapOriginalSwapWeapon = g_MyD3D->m_TempPlayer.m_mapSwapWeapon;
    m_pkInfoPanel->SetUserInfo();

    {
        //법사인 경우에만 백마법 기술 탭을 활성화시킨다.(현재 백마법 탭이 흑으로 잘못되어 있어서..죄송 ㅡㅜ;)
        if (g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_ARME)
        {
            m_apEquipTab[UEC_SKILL_BLACK]->InitState(true, true, this);
            m_apEquipTab[UEC_SKILL_WHITE]->InitState(true, true, this);
            m_apEquipTab[UEC_SKILL]->InitState(false);
        }
        else
        {
            // 아르메가 아닌데 이 탭이라면 바꿔준다
            if (eEquipCategory_ == UEC_SKILL_BLACK)
            {
                eEquipCategory_ = UEC_SKILL;
            }

            m_apEquipTab[UEC_SKILL_BLACK]->InitState(false);
            m_apEquipTab[UEC_SKILL]->InitState(true, true, this);
            m_apEquipTab[UEC_SKILL_WHITE]->InitState(false);
        }
    }

    UpdatePetWnd();
#if defined( NATION_CHINA )
    m_pkInfoPanel->HideAllCashGashInfo();
#endif
    g_MyD3D->m_TempPlayer.AutoSetExtra();


    //if( m_InvenCategory != eInvenCategory_ )
    //	m_pkUserItemBox->m_iItemGroupType = 0;

    if (eEquipCategory_ != UEC_NUM)
        m_EquipCategory = eEquipCategory_;
    //if ( eInvenCategory_ != UIC_NUM )
    //    m_InvenCategory = eInvenCategory_;


    SetEquipWndCategory(m_EquipCategory, iScrollPos);
    //    SetInvenWndCategory( m_InvenCategory );

#if defined( USE_LOOK_INVEN_EXTEND )
    UpdateInventoryCapacity(m_EquipCategory == UEC_LOOK);
#else if
    UpdateInventoryCapacity();
#endif

    UpdateHotItemCheck();

    //m_pkCharWnd->UpdateMeshObject();
}


void KGCMyInfoScene::OnChangeChar()
{
    g_kGlobalValue.m_kUserInfo = g_MyD3D->m_TempPlayer.m_kUserInfo;
    UpdateGameData(false, (USER_EQUIP_CATEGORY)GetEquipCategory());

    m_pkStatusPanel->ResetInfo();
    m_pkStatusPanel->SetStartUserInfo();
    m_pkStatusPanel->SetChangeUserInfo();
    m_pkStatusPanel->SetText();
}

void KGCMyInfoScene::ActionPerformed(const KActionEvent& event)
{
    if (event.m_dwCode == D3DWE_BUTTON_DOWN)
    {
        // 탭안의 스크롤이 어딧는가?!
        int iScrollPos = 0;

        for (int i = 0; i < TAB_NUM; ++i)
        {
            if (m_pkInventoryTab[i] == event.m_pWnd) {
                ActiveInventoryTab(i);
                return;
            }
        }
    }

    if (event.m_pWnd == m_pkItemGroupCombo &&
        event.m_dwCode == D3DWE_COMBO_SELCHANGE)
    {
        m_pkUserEquipBox->LockEquipment(false);
        //ActiveInventoryTab( TAB_INVENTORY );
        ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
#if defined( USE_LOOK_INVEN_EXTEND )
        UpdateInventoryCapacity(m_EquipCategory == UEC_LOOK);
#endif
    }

    switch (event.m_dwCode)
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
    {
        g_KDSound.Play("30");
        return;
    }
    case KD3DWnd::D3DWE_BUTTON_DOWN:
    {
        g_KDSound.Play("31");
        return;
    }
    }

    if (event.m_dwCode != D3DWE_BUTTON_CLICK)
        return;

    if (event.m_pWnd == m_pkBtn_Ordering_Level)
    {
        m_pkBtn_Ordering_Level->ToggleRender(false);
        m_pkBtn_Ordering_Parts->ToggleRender(true);
        m_bSortByLevel = true;
        ChangeInventoryItem(m_iItemGroupType);
        return;
    }

    if (event.m_pWnd == m_pkBtn_Ordering_Parts)
    {
        m_pkBtn_Ordering_Level->ToggleRender(true);
        m_pkBtn_Ordering_Parts->ToggleRender(false);
        m_bSortByLevel = false;
        ChangeInventoryItem(m_iItemGroupType);
        return;
    }

    if (event.m_pWnd == m_pkCharWnd)
    {
        m_pkUserEquipBox->LockEquipment(false);
        ActiveInventoryTab(TAB_INVENTORY);
        OnChangeChar();
        return;
    }

    for (int i = 0; i < UEC_NUM; i++)
    {
        if (m_apEquipTab[i] == event.m_pWnd)
        {
            m_pkUserEquipBox->LockEquipment(false);
            ActiveInventoryTab(TAB_INVENTORY);
            m_iItemGroupType = 0; //아이템 박스는 전체로 한다.
            SetEquipWndCategory((USER_EQUIP_CATEGORY)i);
            m_pkItemGroupCombo->SetCurSel(i == 0 || i == 3 ? 0 : 1);
            if (i != UEC_EQUIP1)
                ShowItemEffect(false);
            return;
        }
    }

    for (int i = 0; i < UIC_NUM; i++)
    {
        if (m_apInvenTab[i] == event.m_pWnd)
        {
            m_pkUserEquipBox->LockEquipment(false);
            ActiveInventoryTab(TAB_INVENTORY);
            //m_pkUserItemBox->m_iItemGroupType = 0; //아이템 박스는 전체로 한다.
            SetInvenWndCategory((USER_INVEN_CATEGORY)i);
            m_pkItemGroupCombo->SetCurSel(i);

            return;
        }
    }
    if (event.m_pWnd == m_pkBtnExitFromMyinfoRoom)
    {
        if (0 == g_pkUIHelper->GetUILockTime(LOCK_ROOM))
        {
            // wony855 GCStateMyInfo.cpp -> OnDestroy 에서 아래 두줄을 처리해준다
            //g_MyD3D->SendEquipItemList();
            //g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
            {
                g_MyD3D->m_pStateMachine->GoRoom();
            }


            g_pkUIHelper->SetUILockTime(LOCK_ROOM, BUTTON_LOCK_FRAME);
            m_pkUserEquipBox->LockEquipment(false);
            ActiveInventoryTab(TAB_INVENTORY);
        }

        return;
    }

    if (event.m_pWnd == m_apkButton[BTN_MY_INFO])
    {
        /////////////////////////////////////////////////////////////////////
        // Date :  06-12-14 
        // milennium9@kogstudios.com 권상구
        //*******************************************************************
        // Inventory 버튼에 시간 제한을 걸어 줍니다.
        /////////////////////////////////////////////////////////////////////
        if (0 == g_pkUIHelper->GetUILockTime(LOCK_ROOM))
        {
            // wony855 GCStateMyInfo.cpp -> OnDestroy 에서 아래 두줄을 처리해준다
            //g_MyD3D->SendEquipItemList();
            //g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
            {
                g_MyD3D->m_pStateMachine->GoRoom();
            }


            g_pkUIHelper->SetUILockTime(LOCK_ROOM, BUTTON_LOCK_FRAME);
            m_pkUserEquipBox->LockEquipment(false);
            ActiveInventoryTab(TAB_INVENTORY);
        }

        return;
    }

    if (event.m_pWnd == m_pkEmoticonWindowButton)
    {
        if (!SiKGCPrivateDungeonManager()->IsClearPrivateDungeon(g_kGlobalValue.m_kUserInfo.GetCurrentCharType()))
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_CLEAR_PRIVATE_DUNGEON));
            return;
        }


        ShowItemEffect(false);
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EMOTICON_COLLECTION);
        return;
    }

    if (event.m_pWnd == m_pkBntUserStatus)
        SetInfoButtonState(false, true);
    if (event.m_pWnd == m_pkBntUserInfo)
        SetInfoButtonState(true, false);

    if (event.m_pWnd == m_pkWelcomeBack_btn)
    {
        OnRecommend();
        //#if defined( WELCOMEBACK_FRIEND_NEW )
        //        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_WELCOMEBACK_DLG_NEW );
        //#else
        //        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_WELCOMEBACK_DLG );
        //#endif
        return;
    }

    if (event.m_pWnd == m_pkBtnLevelDown)
    {
        UpdateLevelDown(true);
        return;
    }

    if (event.m_pWnd == m_pkBtnLevelDownOn)
    {
        UpdateLevelDown(false);
        return;
    }

    KGCScene::ActionPerformed(event);
}

void KGCMyInfoScene::UpdateLevelDown(bool bLevelDown)
{
    m_pkBtnLevelDown->ToggleRender(!bLevelDown);
    m_pkBtnLevelDownOn->ToggleRender(bLevelDown);
    m_bLevelDown = bLevelDown;

    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE, GetItemBtnState()));
}

void KGCMyInfoScene::SetEquipWndCategory(USER_EQUIP_CATEGORY EquipCategory, int iScrollPos/* = 0*/)
{
    if (m_EquipCategory != EquipCategory)
        m_iItemGroupType = 0;

    m_EquipCategory = EquipCategory;

    m_pkUserEquipBox->SetScrollPos(iScrollPos);
    m_pkUserEquipBox->SetItemCategory(EquipCategory);

    if (EquipCategory == UEC_PET)
    {
        m_pkUserEquipBox->m_pkPetInfoBox->NoMoreFoodAnim();
        KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
        int iPetID = g_kGlobalValue.GetPetIDFromItemID(kPet.m_dwID);
        int iPetEvolutionLevel = (int)kPet.m_cPromotion;
        int iPetLevel = kPet.m_iLevel;
        if (!m_apEquipTab[UEC_PET]->IsActive() && g_MyD3D->IsExistPetInfo(kPet.m_dwUID) && iPetID != -1)
        {
            m_pkUserEquipBox->m_pkPetWnd->m_pkPet->StartPet(-1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, kPet.m_strName, false, kPet.m_kCostume.m_dwID, true);
            m_pkUserEquipBox->m_pkPetWnd->m_pkPet->usMotion = EGCPETMOTION::SHOP;
            m_pkUserEquipBox->m_pkPetWnd->SetPetExp();
            m_pkUserEquipBox->m_pkPetWnd->m_vPetAxisCorrection = m_pkUserEquipBox->m_pkPetWnd->m_pkPet->GetPetAxisCorrection();
        }
    }
    else if (m_apEquipTab[UEC_PET]->IsActive())
    {
        m_pkUserEquipBox->m_pkPetWnd->m_pkPet->EndPet();
        m_pkUserEquipBox->m_pkPetWnd->SetPetExp(false);
    }

    for (int iTab = 0; iTab < UEC_NUM; iTab++)
    {
        if (m_apEquipTab[iTab])
        {
            m_apEquipTab[iTab]->Activate(iTab == m_EquipCategory);
        }
    }

    SetComboPopupList();
    InitInventoryItem();

    if (GC_GLOBAL_DEFINE::bReformCoordi)
    {
        if (m_EquipCategory == UEC_LOOK)
        {
            m_pkInventoryTab[TAB_EXTRAITEM]->ToggleRender(false);
            m_pkInventoryTab[TAB_OTHERITEM]->ToggleRender(false);
            m_pkInventoryTab[TAB_OTHERITEM2]->ToggleRender(true);
            m_pkMyinfoSceneSubMenu->ButtonRender(false, false, false);
        }
        else
        {
            m_pkInventoryTab[TAB_EXTRAITEM]->ToggleRender(true);
            m_pkInventoryTab[TAB_OTHERITEM]->ToggleRender(true);
            m_pkInventoryTab[TAB_OTHERITEM2]->ToggleRender(false);
            m_pkMyinfoSceneSubMenu->ButtonRender(GetNowInventoryTab() != TAB_OTHERITEM, false, !m_pkSellInfo->IsRenderOn());
        }
    }

    ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
    m_pkStatusPanel->SetChangeUserInfo();
    m_pkStatusPanel->SetText();
}

void KGCMyInfoScene::SetInvenWndCategory(USER_INVEN_CATEGORY InvenCategory)
{
    //m_pkUserItemBox->SetItemCategory(InvenCategory); 

    m_InvenCategory = InvenCategory;

    InitInventoryItem();
}

void KGCMyInfoScene::InitInventoryItem()
{
    ChangeInventoryItem(m_iItemGroupType); //현재 그룹 아이템만을 나타낸다.
}

void KGCMyInfoScene::ChangeInventoryItem(int iGroupType)
{
    if (iGroupType != -1)
        m_iItemGroupType = iGroupType;

    int iTab = GetNowInventoryTab();

    m_pkMyinfoSceneSubMenu->ButtonRender(!m_pkSellInfo->IsRenderOn() && m_EquipCategory != UEC_LOOK && iTab != TAB_OTHERITEM, m_EquipCategory == UEC_PET, !m_pkSellInfo->IsRenderOn());
    std::vector<GCITEMUID> vecOriginList;
    if (m_EquipCategory == UEC_LOOK) {
        if (iTab == TAB_INVENTORY) {
            vecOriginList = g_pItemMgr->GetInventoryCoordiItemUIDList(g_pItemMgr->GetCharFlag(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType));
            g_pItemMgr->FilterEquipedItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), vecOriginList, true);
        }
        else if (iTab == TAB_OTHERITEM || iTab == TAB_OTHERITEM2) {
            vecOriginList = g_pItemMgr->GetInventoryCoordiItemUIDList();
            g_pItemMgr->FilterEquipedItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), vecOriginList, true);
            g_pItemMgr->FilterUnuseCoordiItemList(vecOriginList);
        }
    }
    else if (iTab == TAB_EXTRAITEM) {
        g_pItemMgr->MakeInventoryItemList(UEC_EQUIP1, 0, false, &vecOriginList);
        g_pItemMgr->FilterRemainStuffOnly(vecOriginList);
    }
    else {
        g_pItemMgr->MakeInventoryItemList(m_EquipCategory, iGroupType, TAB_INVENTORY != iTab, &vecOriginList, m_bSortByLevel);
        g_pItemMgr->FilterRemoveCoordiItem(vecOriginList);
#ifdef WEAPONCHANGE
        std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
        if (mit != g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end())
        {
            std::vector<GCITEMUID>::iterator vit = std::find(vecOriginList.begin(), vecOriginList.end(), mit->second.second.second);
            if (vit != vecOriginList.end())
                vecOriginList.erase(vit);
        }
#endif
    }

    std::vector<KItem> vecItemList;
    for (int i = 0; i < (int)vecOriginList.size(); i++)
    {
        KItem* kInvenItem = g_pItemMgr->GetInventoryItem(vecOriginList[i]);
        if (kInvenItem == NULL)
            continue;

        GCItem* pItem = g_pItemMgr->GetItemData(kInvenItem->m_ItemID);
        if (pItem)
        {
            if (pItem == NULL)
                continue;

            if (SiKGCCoupleSystem()->IsInCoupleInven(kInvenItem->m_ItemUID))
                continue;

            if (g_pItemMgr->IsNonVisibleItem(pItem->dwGoodsID))
                continue;

            if (GIK_MONSTER_CARD == pItem->eItemKind)
                continue;

            if (GIK_ENCHANT_ENABLE_ITEM == pItem->eItemKind && kInvenItem->m_EnchantEquipItemUID != 0)
                continue;

            if (g_pItemMgr->IsEquipDisableItem(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType, pItem->dwGoodsID)) {
                continue;
            }

            if (GIK_TITLE_ITEM != pItem->eItemKind)
                vecItemList.push_back(*kInvenItem);
        }
    }

    if (m_pkPopopAttributeSelect->IsRenderOn()) {
        m_pkPopopAttributeSelect->ToggleRender(false);
    }

    if (m_pkItemPopupMenu->IsRenderOn())
    {
        m_pkItemPopupMenu->ToggleRender(false);
    }

    m_pkUserItemBox->RefreshItemList(vecItemList, iTab != TAB_OTHERITEM);

    if (TAB_OTHERITEM == iTab)
        m_pkUserEquipBox->LockEquipment(true);

}

void KGCMyInfoScene::SetComboPopupList()
{
    m_pkItemGroupCombo->DeleteAllItem();

    if (m_iItemGroupType == 0)
        m_pkItemGroupCombo->SetComboStatic(g_pkStrLoader->GetString(STR_ID_TOTAL));

    m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_TOTAL));

    switch (m_EquipCategory)
    {
    case UEC_EQUIP1:
    {
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_ARMOR));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_HEAD));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_BODY1));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_BODY2));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_HAND));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_SHOOSE));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_MANTLE));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_WEAPON));
        break;
    }
    case UEC_EQUIP2:
    {
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_ACCESSORY));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_HEAD_UP));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_HEAD_DOWN));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_NECKLACE));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_BODY1_ACC));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_BODY2_ACC));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_ARM_ACC));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_RING));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_SHOOSE_ACC));
        break;
    }
    case UEC_SHORTCUT_SLOT:
    {
        // 상의 후에 결정해야 할 듯 이거 뭐 수정해야 하는거 같은데.
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_QUICK_SLOT_ITEM));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_QUICK_SLOT_POTION));	// 포션이나 버프계열
#if defined(EMOTICON_SYSTEM)
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_QUICK_SLOT_EMOTICON));	// 이모티콘 계열
#endif
        m_EquipCategory = UEC_SHORTCUT_SLOT;
        break;
    }
    case UEC_PET:
    {
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_PET));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_PETITEM));
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_PETSKILL));
        break;
    }
    case UEC_SKILL:
    case UEC_SKILL_WHITE:
    case UEC_SKILL_BLACK:
    {
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetString(STR_ID_ITEM_SPECIAL_SKILL));
#if defined( NATION_USA ) || defined( NATION_EU )
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 2, 1)); //"1차직업 1단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 2, 2)); //"1차직업 2단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 2, 3)); //"1차직업 3단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 3, 1)); //"2차직업 1단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 3, 2)); //"2차직업 2단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "nn", 3, 3)); //"2차직업 3단필살기"
#elif defined(NATION_THAILAND)
#else
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 1, 1)); //"1차직업 1단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 1, 2)); //"1차직업 2단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 1, 3)); //"1차직업 3단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 2, 1)); //"2차직업 1단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 2, 2)); //"2차직업 2단필살기"
        m_pkItemGroupCombo->AddString(g_pkStrLoader->GetReplacedString(STR_ID_ITEM_SPECIAL_SKILL2, "ii", 2, 3)); //"2차직업 3단필살기"
#endif
        break;
    }
    }

    m_pkItemGroupCombo->SetCurSel(m_iItemGroupType);

    if (m_EquipCategory != UEC_EQUIP1 && m_EquipCategory != UEC_PET)
    {
        m_pkItemGroupCombo->SetCurSel(1);
        //m_pkItemGroupCombo->SetComboStatic( ((KD3DStatic*)m_pkItemGroupCombo->)->GetText().c_str() );
        m_pkItemGroupCombo->DirectActionPerform(KActionEvent(m_pkItemGroupCombo->GetComboPopList()->GetChild(m_iItemGroupType + 9), D3DWE_BUTTON_CLICK));
    }
}

void KGCMyInfoScene::FrameMoveInEnabledState(void)
{
#if defined (USE_LEVEL_DOWN)
    if (m_pkInfoPanel->IsRenderOn())
        m_pkBtnLevelDown->Lock(false);
    else
        m_pkBtnLevelDown->Lock(true);
#endif

    //  방안에서는 동작하면 안된다.
    if (g_pkInput->IsDown(DIK_ESCAPE) && g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO)
    {
        if (false == g_pkUIScene->IsMsgBoxModal())
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ESC_MENU_DLG);
            g_pkInput->IgnoreDown(DIK_ESCAPE, 30);
        }
    }

    DrawInfoWindow();
    SiKFriendShip()->CheckAlramAddFriend();
    KGCScene::FrameMoveInEnabledState();


#ifndef __PATH__
    if (g_pkInput->IsDown(DIK_GRAVE))
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ADDITEM_IN_CLIENT);
        g_pkUIScene->m_pkAddItemInClient->OnRenderUpLine(true);
        g_pkUIScene->m_pkAddItemInClient->SetFocus(true);
        g_pkUIScene->m_pkAddItemInClient->ClearEdit();

    }
#endif 

#if defined (USE_JUST_IN_TIME_EVENT)
    KGCScene::JustInTimeFrameMove();
#endif
}

void KGCMyInfoScene::DrawInfoWindow()
{
    if (g_pkUIScene->IsMsgBoxModal() || m_pkPopopAttributeSelect->IsRenderOn() || m_pkItemPopupMenu->IsRenderOn())
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
        return;
    }

    GCItem* kItem = NULL;
    KItem* kInventoryItem = NULL;
    D3DXVECTOR2 vecPos(0.0f, 0.0f);

    if ((m_pkUserItemBox->GetCurrentHoveredItem(kItem, kInventoryItem, vecPos) == false || m_pkUserItemBox->IsRenderOn() == false) &&
        (m_pkUserEquipBox->GetCurrentHoveredItem(kItem, kInventoryItem, vecPos) == false || m_pkUserEquipBox->IsRenderOn() == false) ||
        g_kGlobalValue.m_bShowItemInfoBox)
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
        return;
    }

    if (kInventoryItem == NULL || kItem == NULL)
    {
        //ASSERT(!"누가 없는 아이템을 보여 줄려고 했다!!" );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
        return;
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(kItem, -1, kInventoryItem);

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
}

void KGCMyInfoScene::UpdateData()
{
    InitInventoryItem();
    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
    m_pkUserItemBox->UpdateData();
    UpdatePetWnd();

    m_pkCharWnd->UpdateMeshObject();
    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
}

void KGCMyInfoScene::SetTitleItemToCharWnd(int iItemID)
{
    if (!m_pkCharWnd)
        return;

    m_pkCharWnd->SetTitleItemID(iItemID);
    m_pkStatusPanel->SetChangeUserInfo();
    m_pkInfoPanel->SetUserInfo();

    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
    m_pkStatusPanel->SetText();
}

void KGCMyInfoScene::UpdateCharInfo()
{
    if (!m_pkCharWnd)
        return;

    m_pkStatusPanel->SetChangeUserInfo();
    m_pkInfoPanel->SetUserInfo();
    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
    m_pkStatusPanel->SetText();
}

void KGCMyInfoScene::UpdatePetWnd()
{
    {
        // 펫 : 자기 정보창에서 표시해 주는 펫
        // 펫 보는 창과 캐릭터 보는 창의 펫 정보를 모두 갱신한다
        KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
        int iPetID = g_kGlobalValue.GetPetIDFromItemID(kPet.m_dwID);
        int iPetEvolutionLevel = (int)kPet.m_cPromotion;
        int iPetLevel = kPet.m_iLevel;

        if (g_MyD3D->IsExistPetInfo(kPet.m_dwUID) && iPetID != -1 && m_apEquipTab[UEC_PET]->IsActive())
        {
            m_pkUserEquipBox->m_pkPetWnd->m_pkPet->StartPet(-1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, kPet.m_strName, false, kPet.m_kCostume.m_dwID, true);
            m_pkUserEquipBox->m_pkPetWnd->m_pkPet->usMotion = EGCPETMOTION::SHOP;
            m_pkUserEquipBox->m_pkPetWnd->SetPetExp();
            m_pkUserEquipBox->m_pkPetWnd->m_vPetAxisCorrection = m_pkUserEquipBox->m_pkPetWnd->m_pkPet->GetPetAxisCorrection();
        }
        else
        {
            m_pkUserEquipBox->m_pkPetWnd->m_pkPet->EndPet();
            m_pkUserEquipBox->m_pkPetWnd->SetPetExp(false);
        }

        if (g_MyD3D->IsExistPetInfo(kPet.m_dwUID) && iPetID != -1)
        {
            m_pkCharWnd->m_pkPet->StartPet(-1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID);
            m_pkCharWnd->m_pkPet->SetPetMotion(EGCPETMOTION::WAIT);
        }
        else
        {
            m_pkCharWnd->m_pkPet->EndPet();
        }
    }
}

void KGCMyInfoScene::UpdateInventoryCapacity(bool bCoordi_ /*= false*/)
{
    m_pkInventoryCapacity->UpdateCapacity(bCoordi_);
    g_pkUIScene->m_pkInventoryFullMsg->UpdateMsg();
}

void KGCMyInfoScene::SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_)
{
    m_bShowUserInfo = bUserInfo_;
    m_pkBntUserInfo->Activate(bUserInfo_);
    m_pkInfoPanel->ToggleRender(bUserInfo_);

    m_bShowStatusInfo = bStatusInfo_;
    m_pkBntUserStatus->Activate(bStatusInfo_);
    m_pkStatusPanel->ToggleRender(m_bShowStatusInfo);

    if (bUserInfo_)
        m_pkWelcomeBack_btn->ToggleRender(true);
    else
        m_pkWelcomeBack_btn->ToggleRender(false);
}

bool KGCMyInfoScene::OnClickPetItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;

    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    SCharInfo& SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
    KPetInfo& kPet = SChar.kPetInfo;

    if (SChar.iLevel < g_pItemMgr->GetlItemEquipLevel(itemInfo, kItemInfo, NULL, true))
        return false;//Level제한에 걸렸음

    // 이 펫이 코스튬 펫이면 펫을 장착시키고 옷도 입힌다.
    if (g_kGlobalValue.IsPetCostumeItem(iItemID))
    {
        DWORD dwBasePetID = g_kGlobalValue.GetBasePetItemIDFromPetCostume(iItemID);
        int iPromotion = g_kGlobalValue.GetPromotionFromPetCostume(iItemID);

        KItem* pInvenItem = NULL;

        KPetInfo kUserPet = g_MyD3D->GetMyPet(dwBasePetID, iPromotion);

        if (g_pItemMgr->FindInventory(kUserPet.m_dwUID, &pInvenItem))
        {
            g_MyD3D->m_TempPlayer.UnEquipPet(SChar.iCharType);
            g_MyD3D->m_TempPlayer.SetEquipPetInfo(SChar.iCharType, pInvenItem->m_ItemUID);
            g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID = iItemID;
            g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwUID = kItemInfo->m_ItemUID;
        }
        else
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PET_COSTUME2));
    }
    // 현재 장착하려는 펫이 초기 생성이라면 펫 생성 패킷을 날린다
    // ( 펫 정보가 존재하지 않는 경우 )
    else if (!g_MyD3D->IsExistPetInfo(kItemInfo->m_ItemUID))
    {
#if defined( USE_BIND_PET )
        if (false == g_kGlobalValue.IsVestedItem(kItemInfo->m_ItemID)) {     // 공용 펫이면 
            Result_CreatePet = INT_MAX;

            KP2P::GetInstance()->Send_CreatePetReq(-1, kItemInfo->m_ItemID, kItemInfo->m_ItemUID, L"New Pet");
            g_MyD3D->WaitForServerAck(Result_CreatePet, INT_MAX, 15000, -6);

            switch (Result_CreatePet)
            {
            case 0:
            {
                if (kItemInfo->m_ItemID != 30798) {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_PET_PREVIEW);
                    g_pkUIScene->m_pkPetPreview->SetMyInfoPreview();
                    g_pkUIScene->m_pkPetPreview->SetDescState(2);
                    g_pkUIScene->m_pkPetPreview->SetNowPet(g_kGlobalValue.GetPetIDFromItemID(kItemInfo->m_ItemID), 0);
                    g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState(PET_MENT_FIRST_MEETING);
                }

                g_MyD3D->m_TempPlayer.UnEquipPet(SChar.iCharType);
                g_MyD3D->m_TempPlayer.SetEquipPetInfo(SChar.iCharType, kItemInfo->m_ItemUID);

                break;
            }
            case 2:
            case 1:
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CASH_FAIL_TITLE),
                    g_pkStrLoader->GetString(STR_ID_NO_EXIST_PET));
                break;
            }
            case 3:
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CASH_FAIL_TITLE),
                    g_pkStrLoader->GetString(STR_ID_ALREADY_EXIST_PET));
                break;
            }
            default:
            {
                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_CREATE_PET_ACK, Result_CreatePet);
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, strTemp);
                break;
            }
            }

        }
        else {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ITEM_BIND_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kItemInfo->m_ItemUID, SChar.iCharType);
        }
#else
        Result_CreatePet = INT_MAX;

        // 패킷 세팅
        KP2P::GetInstance()->Send_CreatePetReq(-1, kItemInfo->m_ItemID, kItemInfo->m_ItemUID, L"사용안함");
        g_MyD3D->WaitForServerAck(Result_CreatePet, INT_MAX, 15000, -6);

        // 펫 등록 오류값.
        //1; break; // 펫 존재하지 않음
        //2; break; // 펫 아이템을 가지고 있지 않음
        //3; break; // 신규 생성하려는 펫이 이미 존재함
        switch (Result_CreatePet)
        {
        case 0:
        {
            // 성공했을 경우 펫 미리보기 창을 띄워서 얘들이 연속으로 펫 생성하는것을 막아야 한다
            // 키티 망할.... 젠장..... 죽여버려..... 키티는 프리뷰안뿌려요... 
            if (kItemInfo->m_ItemID != 30798) {
                g_pkUIScene->MessageBox(KGCUIScene::GC_PET_PREVIEW);
                g_pkUIScene->m_pkPetPreview->SetMyInfoPreview();
                g_pkUIScene->m_pkPetPreview->SetDescState(2);
                g_pkUIScene->m_pkPetPreview->SetNowPet(g_kGlobalValue.GetPetIDFromItemID(kItemInfo->m_ItemID), 0); // 처음생성되는 놈이니 진화레벨은 0 이겠지..
                g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState(PET_MENT_FIRST_MEETING);
            }
            // 펫 생성에 성공했다면 원래 끼고 있던 펫은 벗기고 생성된 펫을 끼운다
            g_MyD3D->m_TempPlayer.UnEquipPet(SChar.iCharType);
            g_MyD3D->m_TempPlayer.SetEquipPetInfo(SChar.iCharType, kItemInfo->m_ItemUID);

            break;
        }
        case 2:
        case 1:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CASH_FAIL_TITLE),
                g_pkStrLoader->GetString(STR_ID_NO_EXIST_PET));
            break;
        }
        case 3:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CASH_FAIL_TITLE),
                g_pkStrLoader->GetString(STR_ID_ALREADY_EXIST_PET));
            break;
        }
        default:
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii",
                KUserEvent::EVENT_CREATE_PET_ACK, Result_CreatePet);

            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISCONNECTED1),
                g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISCONNECTED2) + strTemp);
            break;
        }
        }
#endif
    }
    else // 이미 펫정보가 있는 녀석이면
    {
#if defined( USE_BIND_PET ) 
        if (g_pItemMgr->IsSelectCharacterPet(static_cast<char>(SChar.iCharType), kItemInfo->m_ItemUID))
#endif 
        {
            // 우선 펫을 벗기고
            g_MyD3D->m_TempPlayer.UnEquipPet(SChar.iCharType);
            g_MyD3D->m_TempPlayer.SetEquipPetInfo(SChar.iCharType, kItemInfo->m_ItemUID);
            g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID = 0;
            g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwUID = 0;
        }
    }

    // 2007.08.31 wony
    // 펫장착 시.. 해당 펫이 사용할 수 있는 스킬이나..
    // 링겔등을 자동으로 장착시켜준다.
    g_MyD3D->m_TempPlayer.AutoEquipPetItem();
    return true;
}

bool KGCMyInfoScene::OnClickMission(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;

    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, iItemID * 10, true);
    return true;
}

bool KGCMyInfoScene::OnClickPetEquipItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_MyD3D->m_TempPlayer.EquipPetItemByItemUID(kItemInfo->m_ItemUID);
    return true;
}

bool KGCMyInfoScene::OnClickShortCutItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    INT iIndex = 0;
    int iScrollPos = 0;

    if ((iIndex = g_MyD3D->GetMyShortCutSlot()->InsertItem(kItemInfo)) == (-1))
    {
        int itemID = kItemInfo->m_ItemID;
        if (g_MyD3D->GetMyShortCutSlot()->ExchangeItem(kItemInfo) == false)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1), g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2));
        }
    }
    m_pkUserEquipBox->SetScrollPos(0);

    UpdateGameData(false, UEC_SHORTCUT_SLOT, iScrollPos);
    return true;
}

bool KGCMyInfoScene::OnClickEmoticonItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    // 이모티콘 때려넣기
    int iScrollPos = 1;
    m_pkUserEquipBox->SetScrollPos(iScrollPos);

    if (g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem(kItemInfo->m_ItemID)) {
        g_MyD3D->GetMyShortCutSlot()->InsertSpecialSkill(kItemInfo);
    }
    else {
        g_MyD3D->GetMyShortCutSlot()->InsertEmot(kItemInfo);
    }

    UpdateGameData(false, UEC_SHORTCUT_SLOT, iScrollPos);
    return true;
}

bool KGCMyInfoScene::OnClickSkillItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    //스킬 아이템이다...
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT) &&
        itemInfo->eMoneyType == EMT_CASH)
        return false;

    SiGCSKT()->EquipSkill(itemInfo->dwGoodsID);
    g_pItemMgr->EquipInventoryItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), kItemInfo->m_ItemUID);

    return true;
}

bool KGCMyInfoScene::OnClickEquipment(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_pItemMgr->EquipInventoryItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), kItemInfo->m_ItemUID);
    return true;
}

bool KGCMyInfoScene::OnClickItemWnd(GCITEMUID UID_)
{
    ReleaseCapture();
    g_KDSound.Play("31");

    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_MyD3D->m_TempPlayer.RemovePlayerObject();
    g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

    int iCategory = g_pItemMgr->GetItemCategory(kItemInfo->m_ItemID);

    // 장착 불가능한 아이템일 경우에는 장착을 시도했을 때 원래 보고 있던 카테고리를 계속 보고 있도록 한다
    if (iCategory == -1)
    {
        iCategory = GetEquipCategory();
    }

    // 펫 관련 아이템 일 때 펫 탭으로 이동한다
    if (itemInfo->eItemKind == GIK_PET && itemInfo->ePetItemKind != EPET_KIND_INVALID)
    {
        iCategory = UEC_PET;
    }
    g_pStateGame->RemoveEmptyItem();

    if (iCategory != UEC_SHORTCUT_SLOT && kItemInfo->CheckItemType(KItem::TYPE_LOOK) == false) {
        UpdateGameData(false, (USER_EQUIP_CATEGORY)iCategory, 0);
    }
    else if (kItemInfo->CheckItemType(KItem::TYPE_LOOK) == true) {
        UpdateGameData(false, UEC_LOOK, 0);
    }
    return true;
}

bool KGCMyInfoScene::OnClickPVPDisabledItem(GCITEMUID UID_)
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PVP_CONFERENCE_CANT_EQUIP));
    return true;
}

bool KGCMyInfoScene::OnClickNotExistCharItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_ERR_MSG1),
        g_pkStrLoader->GetReplacedString(STR_ID_INVENTORY_ERR_MSG2, "l", GCFUNC::GetCharName(itemInfo->GetCharType())));
    return true;
}

void KGCMyInfoScene::OnClickInventoryItem(const KActionEvent& kEvent_)
{
    GCITEMUID itemUID = kEvent_.m_dwlParam;
    KItem* pInvenItem = g_pItemMgr->GetInventoryItem(itemUID);
    if (pInvenItem == NULL)
        return;

    int iItemID = pInvenItem->m_ItemID;
    if (iItemID == -1)
        return;

    //없는 케릭터 아이템 리스트라면 메시지만 띄워줍니다.
    if (m_pkInventoryTab[TAB_OTHERITEM]->IsActive() == true)
    {
        OnClickNotExistCharItem(itemUID);
        return;
    }

    if (SiGCPetMagicStone()->IsPetGlyph(iItemID))
    {
        return;
    }

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    RETURN_NIL(itemInfo);

    if (pInvenItem->CheckItemType(KItem::TYPE_LOOK)) {
        OnClickLookItem(itemUID);
    }
    // 미션 스크롤 일 경우
    else if (itemInfo->eItemKind == GIK_MISSION_SCROLL)
    {
        OnClickMission(itemUID);
        return;
    }
#if defined( COUPLE_SYSTEM )
    else if (itemInfo->eItemKind == GIK_COUPLE_ITEM)
    {
        //아무것도 하지마!! 일부러 아무 동작 안하기 위한 부분이니 지우지 말것!!!
    }
#endif
    // 펫 : 펫 장착 / 생성
    // 현재 장착하려는 아이템이 펫 이라면
    else if (itemInfo->eItemKind == GIK_PET && itemInfo->ePetItemKind == EPET_KIND_PET)
    {
        OnClickPetItem(itemUID);
    }
    // 펫 : 펫아이템 장착
    else if (itemInfo->eItemKind == GIK_PET && itemInfo->ePetItemKind == EPET_KIND_EQUIP_ITEM)
    {
        OnClickPetEquipItem(itemUID);
    }
    // 단축 슬롯 아이템 : 단축 슬롯에 장착
    else if (itemInfo->eItemKind == GIK_SHORTCUT_SLOT)
    {
        if (itemInfo->dwSlotPosition == ESP_SLOT_ITEM)
        {
            OnClickShortCutItem(itemUID);
        }
        else if (itemInfo->dwSlotPosition == ESP_SLOT_EMOT)
        {
            OnClickEmoticonItem(itemUID);
        }
    }
    else if (itemInfo->dwSkillPosition != ESP_SKC_INVALID)
    {
        OnClickSkillItem(itemUID);
    }
    // 펫이나 펫아이템이 아닐 경우에는 캐릭터 장착정보에 세팅을 한다
    else
    {
        OnClickEquipment(itemUID);
    }

    OnClickItemWnd(itemUID);
    m_pkCharWnd->UpdateMeshObject();
}

void KGCMyInfoScene::OnRClickInventoryItem(const KActionEvent& kEvent_)
{
    //  이미 출력되고 있으면 꺼준다.
    if (m_pkItemPopupMenu->IsRenderOn())
    {
        m_pkItemPopupMenu->ToggleRender(false);
        return;
    }

    GCITEMUID itemUID = kEvent_.m_dwlParam;
    KItem* pInvenItem = g_pItemMgr->GetInventoryItem(itemUID);
    if (pInvenItem == NULL)
        return;

    int iItemID = pInvenItem->m_ItemID;
    if (iItemID == -1)
        return;

    GCItem* pItemInfo = g_pItemMgr->GetItemData(iItemID);
    if (pItemInfo == NULL)
        return;

    if (pItemInfo->bNotSellItem == true)
    {
        return;
    }

    D3DXVECTOR2 vMousePos = D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y);

    vMousePos.x /= g_pGCDeviceManager2->GetWindowScaleX();
    vMousePos.y /= g_pGCDeviceManager2->GetWindowScaleY();

    m_pkItemPopupMenu->OpenPopupMenu(itemUID, vMousePos);
}

void KGCMyInfoScene::ActiveInventoryTab(int iTabID)
{
    for (int i = 0; i < TAB_NUM; ++i)
    {
        if (m_pkInventoryTab[i] != NULL)
            m_pkInventoryTab[i]->Activate(i == iTabID);
    }

    switch (iTabID)
    {
    case TAB_OTHERITEM:
    case TAB_OTHERITEM2:
        ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
        break;
    case TAB_EXTRAITEM:
        ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
        break;
    case TAB_INVENTORY:
    default:
        ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
        m_pkUserEquipBox->LockEquipment(false);
        break;
    }

    m_pkUserItemBox->RefreshScrollMax();
    m_pkUserItemBox->RefreshItemWnd();
    this->FindItemFromName(0);

    if (iTabID != TAB_INVENTORY)
        ShowItemEffect(false);

}

int KGCMyInfoScene::GetNowInventoryTab()
{
    for (int i = 0; i < TAB_NUM; ++i)
    {
        if (m_pkInventoryTab[i] == NULL) continue;
        if (m_pkInventoryTab[i]->IsActive())
            return i;
    }

    return TAB_INVENTORY;
}

bool KGCMyInfoScene::OnClickLookItem(GCITEMUID UID_)
{
    KItem* kItemInfo = g_pItemMgr->GetInventoryItem(UID_);

    if (NULL == kItemInfo)
        return false;
    int iItemID = kItemInfo->m_ItemID;

    GCItem* itemInfo = g_MyD3D->m_kItemMgr.GetItemData(iItemID);
    if (itemInfo == NULL)
        return false;

    g_pItemMgr->EquipInventoryItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), kItemInfo->m_ItemUID, true);
    return true;
}

bool KGCMyInfoScene::OnClickSell_OpenSellUI(const KActionEvent& kEvent_)
{
    if (m_pkPopopAttributeSelect->IsRenderOn()) {
        m_pkPopopAttributeSelect->ToggleRender(false);
    }

    if (m_pkItemPopupMenu->IsRenderOn())
        m_pkItemPopupMenu->ToggleRender(false);

    OnClickOpenSellUI(kEvent_, m_pkBtn_Sell);

    return true;
}

bool KGCMyInfoScene::OnClickSell_CloseSellUI(const KActionEvent& kEvent_)
{
    OnClickCloseSellUI();

    return true;
}

void KGCMyInfoScene::OnClickInventoryItem_ForSell(const KActionEvent& kEvent_)
{
    GCITEMUID itemUID = kEvent_.m_dwlParam;

    if (g_pItemMgr->GetSellListSize() >= m_pkSellItemBox->GetEnabledStateWndNum())
        return;

    //판매 리스트에 있다면 빼고 종료한다.
    if (g_pItemMgr->IsContainSellItemList(itemUID)) {
        g_pItemMgr->RemoveSellItemList(itemUID);
        g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_REMOVE, itemUID));
        return;
    }

    //판매 리스트에 있다면 빼고 종료한다.
    if (g_pItemMgr->IsContainSellItemList(itemUID)) {
        g_pItemMgr->RemoveSellItemList(itemUID);
        g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_REMOVE, itemUID));
        return;
    }

    if (g_pItemMgr->IsSellAvailableItem(itemUID) == false &&
        g_pItemMgr->IsRemoveAvailableItem(itemUID) == false)
        return;

    if (g_pItemMgr->CheckEquipItem(&g_MyD3D->m_TempPlayer.m_kUserInfo, itemUID))
        return;

    KItem* pInvenItem = g_pItemMgr->GetInventoryItem(itemUID);
    if (pInvenItem == NULL)
        return;

    int iItemID = pInvenItem->m_ItemID;
    if (iItemID == -1)
        return;



    //장착 중인 아이템이 있다면, 장착 해제 후 판매를 하게 해준다.
    if (g_MyD3D->GetMyShortCutSlot()->GetCurEquipItemNum(iItemID) > 0)
    {
        g_MyD3D->GetMyShortCutSlot()->DischargeItem(iItemID);
        // 장착 해제 후, 인벤토리 갯수를 갱신해준다. 
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        g_pkUIScene->m_pkMyInfoScene->UpdateData();
    }

    if (g_pItemMgr->IsRemoveAvailableItem(itemUID) == false &&
        pInvenItem->m_nCount != KItem::UNLIMITED_ITEM &&
        pInvenItem->m_nCount > 1) {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, itemUID, KGCSellCountItemDlg::EM_MODE_SELL_LIST);
        return;
    }

    g_pItemMgr->AddSellItemToList(*pInvenItem);
    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_ADD, itemUID));
}

void KGCMyInfoScene::OnClickSellItem(const KActionEvent& kEvent_)
{
    GCITEMUID itemUID = kEvent_.m_dwlParam;
    KItem* pInvenItem = g_pItemMgr->GetInventoryItem(itemUID);
    if (pInvenItem == NULL)
        return;

    int iItemID = pInvenItem->m_ItemID;
    if (iItemID == -1)
        return;

    g_pItemMgr->RemoveSellItemList(itemUID);
    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_REMOVE, itemUID));
}

void KGCMyInfoScene::OnClickSellBtn()
{
    const std::vector<KItem>& vecSellList = g_pItemMgr->GetSellList();
    std::vector<KItem>::const_iterator vit;
    int iGP = 0;
    int iGem = 0;
    int iCrystal = 0;
    for (vit = vecSellList.begin(); vit != vecSellList.end(); ++vit) {
        std::pair<int, EMONEYTYPE> pairPrice = g_pItemMgr->GetItemPrice((*vit).m_ItemUID, (*vit).m_nCount);
        switch (pairPrice.second) {
        case EMT_GAMEPOINT:
            iGP += pairPrice.first;
            break;
        case EMT_GEM:
            iGem += pairPrice.first;
            break;
        case EMT_CRYSTAL:
            iCrystal += pairPrice.first;
            break;
        }
    }

#if defined(NATION_CHINA)
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
        g_pkStrLoader->GetReplacedString(STR_ID_SELL_BUNDLE_ITEM, "ii", iGP, iGem), L"",
        KGCUIScene::GC_MBOX_USE_SELL_BUNDLE_ITEM);
#else
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
        g_pkStrLoader->GetReplacedString(STR_ID_SELL_BUNDLE_ITEM, "iii", iGP, iGem, iCrystal), L"",
        KGCUIScene::GC_MBOX_USE_SELL_BUNDLE_ITEM);
#endif

    //m_pkSellInfo->DisconnectAllEventProcedure();
    DisconnectGlobalEventProcedure(KGCUIScene::D3DWE_RECEIVE_GAME_SERVER_PAPCKET);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_RECEIVE_GAME_SERVER_PAPCKET, this, &KGCMyInfoScene::OnRefreshSellItemList);
    DisconnectGlobalEventProcedure(KD3DWnd::D3DWE_MESSAGEBOX_CLOSE);
    g_pkUIMgr->ConnectGlobalEventProcedure(KD3DWnd::D3DWE_MESSAGEBOX_CLOSE, this, &KGCMyInfoScene::OnRefreshSellItemList);
}

bool KGCMyInfoScene::OnRecvSellPacket(const KActionEvent& kEvent_)
{
    if (kEvent_.m_dwlParam != KUserEvent::EVENT_BUNDLE_SELL_ITEM_ACK)
        return true;

    //m_pkBtn_Sell->SpeakToActionListener( KActionEvent( m_pkBtn_Sell, KD3DWnd::D3DWE_BUTTON_CLICK ) );
    return false;
}

void KGCMyInfoScene::OnRefreshSellItemList()
{
    if (m_pkPopopAttributeSelect->IsRenderOn()) {
        m_pkPopopAttributeSelect->ToggleRender(false);
    }

    if (m_pkItemPopupMenu->IsRenderOn())
        m_pkItemPopupMenu->ToggleRender(false);

    const std::vector<KItem>& vecSellList = g_pItemMgr->GetSellList();
    m_pkSellItemBox->RefreshItemList(vecSellList);
    m_pkSellInfo->SetSellItemNum(g_pItemMgr->GetSellListSize());

    m_pkSellInfo->InitPriceList();
    std::vector<KItem>::const_iterator vit;
    for (vit = vecSellList.begin(); vit != vecSellList.end(); ++vit) {
        std::pair<int, int> pairPrice = g_pItemMgr->GetItemPrice((*vit).m_ItemUID, (*vit).m_nCount);
        m_pkSellInfo->AddReward((*vit).m_ItemUID, pairPrice.first, pairPrice.second);
    }
}

void KGCMyInfoScene::FindItemFromName(const WCHAR* strName_)
{
    //::MessageBoxW( NULL, strName_, __FUNCTIONW__, MB_OK );    //  더미 구현
    if (strName_) {
        m_strFilter = strName_;
    }
    else {
        m_strFilter.clear();
    }

    m_pkUserEquipBox->LockEquipment(false);
    ChangeInventoryItem(m_pkItemGroupCombo->GetCurSel());
}

void KGCMyInfoScene::OnClickOpenSellUI(const KActionEvent& kEvent_, KD3DWnd* pkBtn_Sell)
{
    m_pkSellItemBox->InitState(true, true);
    m_pkSellItemBox->ClearItemList();

    m_pkUserItemBox->DisconnectAllEventProcedure();
    m_pkUserItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnClickInventoryItem_ForSell);

    m_pkSellInfo->InitNumbers();
    m_pkSellInfo->InitPriceList();
    m_pkSellInfo->Update();
    m_pkSellInfo->ToggleRender(true);

    m_pkInfoPanel->ToggleRender(false);
    m_pkStatusPanel->ToggleRender(false);
    m_pkBntUserInfo->ToggleRender(false);
    m_pkWelcomeBack_btn->ToggleRender(false);
    m_pkBntUserStatus->ToggleRender(false);
    if (pkBtn_Sell) {
        pkBtn_Sell->Lock(true);
    }
    m_pkSellInfo->DisconnectAllEventProcedure();
    m_pkSellInfo->ConnectEventProcedure(KGCUIScene::D3DWE_SELL_ITEM, this, &KGCMyInfoScene::OnClickSellBtn);
    m_pkSellInfo->ConnectEventProcedure(KGCUIScene::D3DWE_SELL_EXIT, this, &KGCMyInfoScene::OnClickSell_CloseSellUI);

    m_pkUserEquipBox->ToggleRender(false);
    if (m_pkBtn_Sell != NULL)
        m_pkBtn_Sell->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickSell_CloseSellUI);

    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_START));
}

void KGCMyInfoScene::OnClickCloseSellUI()
{
    m_pkSellItemBox->InitState(false);
    m_pkSellItemBox->ClearItemList();

    m_pkUserItemBox->DisconnectAllEventProcedure();
    m_pkUserItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_CLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnClickInventoryItem);
    m_pkUserItemBox->ConnectEventProcedure(KGCUIScene::D3DWE_RCLICK_INVEN_ITEM, this, &KGCMyInfoScene::OnRClickInventoryItem);
    g_pItemMgr->ClearSellItemList();

    m_pkSellInfo->ToggleRender(false);

    m_pkBntUserStatus->ToggleRender(true);
    if (m_pkBntUserStatus->IsActive())
        m_pkStatusPanel->ToggleRender(true);
    m_pkBntUserInfo->ToggleRender(true);
    if (m_pkBntUserInfo->IsActive())
    {
        m_pkInfoPanel->ToggleRender(true);
#if defined( WELCOMEBACK_FRIEND )
        m_pkWelcomeBack_btn->ToggleRender(true);
#else
        m_pkWelcomeBack_btn->ToggleRender(false);
#endif
    }
    else {
        m_pkWelcomeBack_btn->ToggleRender(false);
    }

    m_pkUserEquipBox->ToggleRender(true);
    if (m_pkBtn_Sell) {
        m_pkBtn_Sell->Lock(false);

        m_pkBtn_Sell->DisconnectAllEventProcedure();
        m_pkBtn_Sell->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoScene::OnClickSell_OpenSellUI);
    }

    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_END));
    ActiveInventoryTab(GetNowInventoryTab());
}


void KGCMyInfoScene::UpdateCharacterSelect(int _iCharType)
{
    m_pkCharWnd->SetChar(_iCharType);
    m_pkCharWnd->UpdateCharacterSelect();
}

void KGCMyInfoScene::ShowItemEffect(bool _bShow)
{
#if defined( USE_NEW_ITEM_CHECK ) 
    m_pkUserItemBox->ShowItemEffect(_bShow);
#endif	
}

void KGCMyInfoScene::UpdateHotItemCheck()
{
#if defined( USE_NEW_ITEM_CHECK )
    std::vector<GCITEMUID> vecOriginList;

    g_pItemMgr->MakeInventoryItemList(UEC_EQUIP1, 0, false, &vecOriginList, true);
    g_pItemMgr->FilterRemoveCoordiItem(vecOriginList);

#ifdef WEAPONCHANGE
    std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
    if (mit != g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end())
    {
        std::vector<GCITEMUID>::iterator vit = std::find(vecOriginList.begin(), vecOriginList.end(), mit->second.second.second);
        if (vit != vecOriginList.end())
            vecOriginList.erase(vit);
    }
#endif

    DWORD dwHotType = EM_HOT_ITEM_NONE_TYPE;

    for (int i = 0; i < (int)vecOriginList.size(); ++i)
    {
        //  무기타인지
        //  방어구인지
        //  Box인지
        KItem* kInvenItem = g_pItemMgr->GetInventoryItem(vecOriginList[i]);
        if (kInvenItem == NULL)
            continue;

        GCItem* pItem = g_pItemMgr->GetItemData(kInvenItem->m_ItemID);
        if (!pItem)
            continue;

        if (vecOriginList[i] <= KGCItemManager::MAXIMUM_PCBANG_UID) {     // 피방 아이템 걸르고 
            continue;
        }

        //  무기타입 체크 아직 안했고 무기이면
        if (!(dwHotType & EM_HOT_ITEM_WEAPON_TYPE) && pItem->dwSlotPosition & ESP_WEAPON)
        {
            if (g_pItemMgr->IsHotItem(vecOriginList[i], &g_MyD3D->m_TempPlayer))
            {
                dwHotType |= EM_HOT_ITEM_WEAPON_TYPE;
            }
        }

        //  방어구 ( 악세포함 ) 체크 아직 안했고 방어구이면        
        if (!(dwHotType & EM_HOT_ITEM_ARMOR_TYPE) && g_pItemMgr->IsArmorEquipItem(kInvenItem->m_ItemID))
        {
            if (g_pItemMgr->IsHotItem(vecOriginList[i], &g_MyD3D->m_TempPlayer))
            {
                dwHotType |= EM_HOT_ITEM_ARMOR_TYPE;
            }
        }

        //  까는상자라면
        if (!(dwHotType & EM_HOT_ITEM_BOX_TYPE) && pItem->eItemKind == GIK_TREASUREBOX_ITEM)
        {
            dwHotType |= EM_HOT_ITEM_BOX_TYPE;
        }

        if (dwHotType == EM_HOT_ITEM_ALL_TYPE)
            break;
    }

    g_pItemMgr->SetHotItemType(dwHotType);
#endif
}

void KGCMyInfoScene::OnRecommend()
{
    g_pkUIScene->m_pkNateOn->SetNew(false);
    Result_recommendFullINfo = INT_MAX;
    KP2P::GetInstance()->Send_RecommendFullInfoReq();
    g_MyD3D->WaitForServerAck(Result_recommendFullINfo, INT_MAX, 3000, 2);

    if (!SiKFriendShip()->m_vecWaitRecommenders.empty())
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_RECOMMEND, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
    else {
#if defined( WELCOMEBACK_FRIEND_NEW )
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_WELCOMEBACK_DLG_NEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#else
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_WELCOMEBACK_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#endif
    }
}

void KGCMyInfoScene::OnBindPet(const GCITEMUID itemUID_)
{
    OnClickItemWnd(itemUID_);
    m_pkCharWnd->UpdateMeshObject();
}

bool KGCMyInfoScene::MakeItemOnFirstSlot(IN GCITEMID dwItemID)
{
    if (g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO)
        return false;

    // 아이템 팔기 상태라면 닫아주자
    g_pkUIScene->m_pkMyInfoScene->OnClickCloseSellUI();

    // 스크롤도 최상단으로 
    m_pkUserItemBox->OnScrollPos(0);

    ShowItemEffect(false);

    return m_pkUserItemBox->MakeItemOnFirstSlot(dwItemID);
}

void KGCMyInfoScene::SellInvenItem(const KItem& kItem)
{
    m_kSellItem = kItem;

    GCItem* pItem = g_pItemMgr->GetItemData(m_kSellItem.m_ItemID);
    if (!pItem)
        return;

    int iGP = 0;
    int iGem = 0;
    int iCrystal = 0;

    std::pair<int, EMONEYTYPE> pairPrice = g_pItemMgr->GetItemPrice(m_kSellItem.m_ItemUID, m_kSellItem.m_nCount);
    switch (pairPrice.second) {
    case EMT_GAMEPOINT:
        iGP += pairPrice.first;
        break;
    case EMT_GEM:
        iGem += pairPrice.first;
        break;
    case EMT_CRYSTAL:
        iCrystal += pairPrice.first;
        break;
    }

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
        g_pkStrLoader->GetReplacedString(STR_ID_SELL_ITEM_QUETION, "lii", pItem->strItemName, iGP, iGem), L"",
        KGCUIScene::GC_MBOX_USE_SELL_INVEN_ITEM);


    DisconnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_INVEN_ITEM);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_SELL_INVEN_ITEM, this, &KGCMyInfoScene::RecieveSellInvenItem);
}

void KGCMyInfoScene::RecieveSellInvenItem()
{
    std::vector<KItem> vecItem;
    vecItem.push_back(m_kSellItem);

    g_pItemMgr->SellItem(vecItem);
}

//Sistema de Skin de Dano
void KGCMyInfoScene::OnClickBtnFontSkin()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_CUSTOMIZATION_DLG);
}