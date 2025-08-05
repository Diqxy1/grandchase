#include "stdafx.h"
#include "KGCHeroDungeonUpgradeSidePanel.h"
#include "GCItemInformationBoxPair.h"
#include "gcui/GCCharacterSelectBox.h"
#include "MyD3D.h"
#include "Procedure.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonUpgradeSidePanel);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonUpgradeSidePanel);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonUpgradeSidePanel, "gc_herodungeonupgradesidepanel");


KGCHeroDungeonUpgradeSidePanel::KGCHeroDungeonUpgradeSidePanel(void) :
    m_pkBack(NULL),
    m_pkMaterialSlot(NULL),
    m_pkResultSlot(NULL),
    m_pkUpgradeButton(NULL),
    m_pkUpgradeSucces(NULL),
    m_pkScrollbar(NULL),
    m_pkMaterialIDSide(0)
{
    LINK_CONTROL("back", m_pkBack); //BackGround
    LINK_CONTROL("material_slot", m_pkMaterialSlot); 	//Material necessario para fabricação
    LINK_CONTROL("result_slot", m_pkResultSlot); //Resultado do craft (item que sera construido).
    LINK_CONTROL("btn_upgrade", m_pkUpgradeButton); //Botão de confirmação de Upgrade.
    LINK_CONTROL("img_succ", m_pkUpgradeSucces); //Imagem que aparece após ter sucesso na criação do item.
    LINK_CONTROL("scroll", m_pkScrollbar);
}

KGCHeroDungeonUpgradeSidePanel::~KGCHeroDungeonUpgradeSidePanel() {
}

void KGCHeroDungeonUpgradeSidePanel::OnCreate(void) {
    m_pkBack->InitState(true, true, this);
    m_pkMaterialSlot->InitState(true, true, this);
    m_pkResultSlot->InitState(true, true, this);
    m_pkUpgradeButton->InitState(true, true, this);
    m_pkUpgradeSucces->InitState(true, false, this);
    m_pkScrollbar->InitState(true, true, this);
    CreateInvenSlotSide();
    InitUIStateSide();
}

void KGCHeroDungeonUpgradeSidePanel::OnDestroy(void) {

}

void KGCHeroDungeonUpgradeSidePanel::ActionPerformed(const KActionEvent& event)
{

    if (event.m_dwCode == D3DWE_BUTTON_CLICK)
    {
        for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i)
        {
            if (event.m_pWnd == m_vecInvenItemImgListSide[i])
            {
                OnInvenItemList(i);
                return;
            }
        }
    }

    GCWND_MAP(m_pkScrollbar, OnScrollPos);
    GCWND_MSG_MAP(m_pkUpgradeButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUpgradeSide);
    GCWND_MSG_MAP(m_pkMaterialSlot, KD3DWnd::D3DWE_BUTTON_CLICK, InitUIStateSide);
}

void KGCHeroDungeonUpgradeSidePanel::OnInvenItemList(int index)
{
    int nIndex = (m_iScrollpos * EM_INVEN_COLUMN) + index;
    if (nIndex < 0 || (int)m_vecItemUIDListSide.size() <= nIndex)
        return;

    GCITEMUID itemUID = m_vecItemUIDListSide[nIndex];

    KItem* pkItem = g_pItemMgr->GetInventoryItem(itemUID);

    if (!pkItem)
        return;

    for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i) {
        m_vecInvenItemImgListSide[i]->SelectedItem(false);
        OnselectItemSide(m_vecInvenItemImgListSide[i], pkItem);
    }

    m_vecInvenItemImgListSide[index]->SelectedItem(true);
}

void KGCHeroDungeonUpgradeSidePanel::OnScrollPos(void)
{
    for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i) {
        m_vecInvenItemImgListSide[i]->SelectedItem(false);
    }

    int iPos = m_pkScrollbar->GetScrollPos();

    if (m_iScrollpos != iPos)
    {
        m_iScrollpos = iPos;
        g_KDSound.Play("73");

        UpdateInvenItemListSide();
    }
}

void KGCHeroDungeonUpgradeSidePanel::UpdateItemSlotListSide(KHeroItemUpgradeInfo HeroItemSide)
{
    InitUIStateSide();

    m_pkHeroItemSide = HeroItemSide;
    m_pkMaterialIDSide = m_pkHeroItemSide.m_vecMaterials[0].m_ItemID;

    g_pItemMgr->GetInventoryItemListFromItemID(m_pkMaterialIDSide / 10, tempHeroUpgradeSlotItemSide);

    for (auto& element : tempHeroUpgradeSlotItemSide) {
        if((int)element->m_cGradeID < m_pkHeroItemSide.m_nGrade)
            m_vecItemUIDListSide.push_back(element->m_ItemUID);
    }

    g_pItemMgr->FilterEquipedItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), m_vecItemUIDListSide);

    m_iScrollpos = 0;

    int nItemCount = (int)m_vecItemUIDListSide.size();
    int nScrollMax = ((nItemCount - 1) / EM_INVEN_COLUMN) + 1;

    m_pkScrollbar->SetScrollPageSize(EM_INVEN_ROW);
    m_pkScrollbar->SetScrollRangeMax(nScrollMax);
    m_pkScrollbar->SetScrollPos(m_iScrollpos);

    UpdateInvenItemListSide();
}

void KGCHeroDungeonUpgradeSidePanel::InitUIStateSide()
{
    for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i)
    {
        m_vecInvenItemImgListSide[i]->ResetItem();
    }

    m_iScrollpos = 0;
    m_EquipUID = 0;
    m_nState = EM_STATE_NONE;
    m_pkMaterialIDSide = 0;

    m_pkMaterialSlot->ResetItem();
    m_pkResultSlot->ResetItem();

    m_vecItemUIDListSide.clear();

    g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->ResetSlotStatus();
    g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->UpdateFrame(g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->m_eCurrentShopUpgradeCategoryType, g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradePanel->m_iCurrentPage);
}

void KGCHeroDungeonUpgradeSidePanel::OnselectItemSide(KGCCommonItemWnd* ItemSlot, KItem* m_pkItemSelected) {
    m_nState = EM_STATE_READY;
    m_EquipUID = ItemSlot->GetItemUID();

    m_pkEquipToUpgrade = m_pkItemSelected;

    m_pkMaterialSlot->SetItem(m_pkItemSelected);
    m_pkResultSlot->SetItem(m_pkItemSelected, (char)m_pkHeroItemSide.m_nGrade);
}

void KGCHeroDungeonUpgradeSidePanel::UpdateInvenItemListSide()
{
    for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i)
    {
        m_vecInvenItemImgListSide[i]->ResetItem();
    }

    int nCount = 0;

    for (int i = 0; i < (int)m_vecItemUIDListSide.size(); ++i)
    {
        if (i < (m_iScrollpos * EM_INVEN_COLUMN))
            continue;

        if (nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW)
            return;

        KItem* pkItem = g_pItemMgr->GetInventoryItem(m_vecItemUIDListSide[i]);

        if (!pkItem)
            return;

        m_vecInvenItemImgListSide[nCount]->SetItem(pkItem);

        nCount++;
    }
}

void KGCHeroDungeonUpgradeSidePanel::CreateInvenSlotSide()
{
    float fBasePosX = 13.0f;
    float fBasePosY = 56.0f;

    if (m_vecInvenItemImgListSide.empty()) {

        tempHeroUpgradeSlotImgSide = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_hero_dungeon_dlg_s6.stg", "",
            "hero_dungeon_shop_s6_dlg\\hero_upgrade_side_panel\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW);

        for (int i = 0; i < (int)tempHeroUpgradeSlotImgSide.size(); ++i)
        {
            float fPosX = fBasePosX + ((i % EM_INVEN_COLUMN) * EM_INVEN_WIDH_SIZE);
            float fPosY = fBasePosY + ((i / EM_INVEN_COLUMN) * EM_INVEN_HEIGHT_SIZE);

            tempHeroUpgradeSlotImgSide[i]->SetWindowPosDirect(D3DXVECTOR2(fPosX, fPosY));
            tempHeroUpgradeSlotImgSide[i]->ToggleRender(true);
            tempHeroUpgradeSlotImgSide[i]->InitState(true, true, this);

            m_vecInvenItemImgListSide.push_back((KGCCommonItemWnd*)tempHeroUpgradeSlotImgSide[i]);
        }
    }
}

void KGCHeroDungeonUpgradeSidePanel::OnCreateComplete(void) {

}

void KGCHeroDungeonUpgradeSidePanel::FrameMoveInEnabledState(void) {
    CheckItemInformationSide();
    if (m_nState == EM_STATE_SUCCESS) {
        m_pkUpgradeSucces->ToggleRender(true);
    }
    else {
        m_pkUpgradeSucces->ToggleRender(false);
    }

    if (m_nState == EM_STATE_READY) {
        m_pkUpgradeButton->Lock(false);
    }
    else {
        m_pkUpgradeButton->Lock(true);
    }
}

void KGCHeroDungeonUpgradeSidePanel::CheckItemInformationSide()
{

    for (int i = 0; i < (int)m_vecInvenItemImgListSide.size(); ++i)
    {
        if (m_vecInvenItemImgListSide[i]->GetItemID() != 0 && m_vecInvenItemImgListSide[i]->CheckMousePosInWindowBound())
        {
            int nIndex = (m_iScrollpos * EM_INVEN_COLUMN) + i;
            if (nIndex < 0 || (int)m_vecItemUIDListSide.size() <= nIndex)
                return;

            GCITEMUID itemUID = m_vecItemUIDListSide[nIndex];
            KItem* pkItem = g_pItemMgr->GetInventoryItem(itemUID);
            if (!pkItem)
                return;

            DrawItemInfoSide(pkItem, m_vecInvenItemImgListSide[i]->GetCurrentWindowPos());

            D3DXVECTOR2 vFocusingPos = m_vecInvenItemImgListSide[i]->GetFixedWindowLocalPos();
            vFocusingPos.x -= 2.0f;
            vFocusingPos.y -= 3.0f;

            return;
        }
    }

    if (m_pkMaterialSlot->m_itemID != 0 && m_pkMaterialSlot->CheckMousePosInWindowBound())
    {
        DrawItemInfoSide(m_pkEquipToUpgrade, m_pkMaterialSlot->GetCurrentWindowPos());

        D3DXVECTOR2 vFocusingPos = m_pkMaterialSlot->GetFixedWindowLocalPos();
        vFocusingPos.x -= 2.0f;
        vFocusingPos.y -= 3.0f;

        return;
    }

    if (m_pkResultSlot->m_itemID != 0 && m_pkResultSlot->CheckMousePosInWindowBound())
    {
        DrawItemInfoSide(m_pkResultSlot->m_itemID, m_pkResultSlot->GetCurrentWindowPos());

        D3DXVECTOR2 vFocusingPos = m_pkResultSlot->GetFixedWindowLocalPos();
        vFocusingPos.x -= 2.0f;
        vFocusingPos.y -= 3.0f;

        return;
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
}

void KGCHeroDungeonUpgradeSidePanel::DrawItemInfoSide(KItem* pItem, D3DXVECTOR2 vPos)
{
    if (pItem != NULL && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false)
    {
        vPos.x += EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH;
        vPos.y += (EM_INVEN_HEIGHT_SIZE / 2) * GC_SCREEN_DIV_WIDTH;
        vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

        if (vPos.x + g_pkUIScene->m_pkItemInformationBox->GetWidth() > static_cast<float>(GC_SCREEN_WIDTH))
        {
            vPos.x -= (static_cast<float>(EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH) * m_fWindowScaleX);
            vPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
        }

        GCItem* pGCItem = g_pItemMgr->GetItemData(pItem->m_ItemID);

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pGCItem, -1, pItem);
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vPos);
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
    }
}

void KGCHeroDungeonUpgradeSidePanel::DrawItemInfoSide(GCITEMID pItemID, D3DXVECTOR2 vPos)
{
    if (pItemID > 0 && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false)
    {
        vPos.x += EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH;
        vPos.y += (EM_INVEN_HEIGHT_SIZE / 2) * GC_SCREEN_DIV_WIDTH;
        vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
        vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

        if (vPos.x + g_pkUIScene->m_pkItemInformationBox->GetWidth() > static_cast<float>(GC_SCREEN_WIDTH))
        {
            vPos.x -= (static_cast<float>(EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH) * m_fWindowScaleX);
            vPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
        }

        GCItem* pGCItemSingle = g_pItemMgr->GetItemData(pItemID);
        pGCItemSingle->cItemGrade = (char)m_pkHeroItemSide.m_nGrade;
        pGCItemSingle->iGrade = m_pkHeroItemSide.m_nGrade;

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pGCItemSingle, -1);
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vPos);
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
    }
}

void KGCHeroDungeonUpgradeSidePanel::OnClickUpgradeSide(void)
{
    m_nState = EM_STATE_PROCESSING;
    m_vecMaterialUID.clear();

    m_vecMaterialUID.push_back(m_pkEquipToUpgrade->m_ItemUID);
    m_vecMaterialUID.push_back(g_pItemMgr->GetInventoryItemFromID(ITEM_ID_HERO_COIN)->m_ItemUID);

    if (m_pkHeroItemSide.m_vecMaterials.empty() || m_vecMaterialUID.size() > m_pkHeroItemSide.m_vecMaterials.size()) {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ITEM_CREATION_ERROR4), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        return;
    }

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_ID_HERO_ITEM_UPGRADE_CONFIRM), L"", KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM_UPGRADE, m_pkHeroItemSide.m_ItemID, m_pkHeroItemSide.m_nGrade, true, true);
}

std::pair< GCITEMID, int > KGCHeroDungeonUpgradeSidePanel::GetUpgradeItemKey(void)
{
    if (m_pkHeroItemSide.m_ItemID == KItem::INVALID_ITEM) {
        return std::pair< GCITEMID, int >(0, 0);
    }

    return std::pair< GCITEMID, int >(m_pkHeroItemSide.m_ItemID, m_pkHeroItemSide.m_nGrade);
}

bool KGCHeroDungeonUpgradeSidePanel::GetUpgradeMaterialUIDSideFirst(IN const std::pair< GCITEMID, int > prKey_, OUT std::vector< GCITEMUID >& vecUIDList_)
{
    std::pair< GCITEMID, int > prBox = GetUpgradeItemKey();

    if (prBox.first != prKey_.first && prBox.second != prKey_.second) {
        return false;
    }

    return GetUpgradeMaterialUIDSide(vecUIDList_);
}

bool KGCHeroDungeonUpgradeSidePanel::GetUpgradeMaterialUIDSide(OUT std::vector< GCITEMUID >& vecUIDList_)
{
    vecUIDList_.clear();

    if (m_pkHeroItemSide.m_ItemID == KItem::INVALID_ITEM) {
        return false;
    }

    if (m_vecMaterialUID.empty()) {
        return false;
    }

    vecUIDList_ = m_vecMaterialUID;
    return true;
}