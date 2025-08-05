#include "stdafx.h"
#include "KGCHeroDungeonShopS6.h"
#include "KGCHeroDungeonUpgradeSlot.h"
#include "GCUI/GCItemInformationBoxPair.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonUpgradeSlot);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonUpgradeSlot);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonUpgradeSlot, "gc_herodungeonupgradeslot");


KGCHeroDungeonUpgradeSlot::KGCHeroDungeonUpgradeSlot(void) :
	m_pkBack(NULL),
	m_pkBackLock(NULL),
	m_pkBackOn(NULL),
	m_pkItemImg(NULL),
	m_pkStaticGrade(NULL),
	m_pkStaticName(NULL),
	m_pkStaticCoinNum(NULL),
	m_pkCoinImg(NULL),
	m_iPrice(0),
	IsSlotActive(false)
{
	LINK_CONTROL("back", m_pkBack);
	LINK_CONTROL("slot_back_lock", m_pkBackLock);
	LINK_CONTROL("slot_back_on", m_pkBackOn);
	LINK_CONTROL("item_img", m_pkItemImg);
	LINK_CONTROL("static_grade", m_pkStaticGrade);
	LINK_CONTROL("static_name", m_pkStaticName);
	LINK_CONTROL("static_coin_num", m_pkStaticCoinNum);
	LINK_CONTROL("coin_img", m_pkCoinImg);
}

KGCHeroDungeonUpgradeSlot::~KGCHeroDungeonUpgradeSlot() {

}

void KGCHeroDungeonUpgradeSlot::OnCreate(void) {
	m_pkBack->InitState(true, true, this);
	m_pkBackLock->InitState(true, true, this);
	m_pkBackOn->InitState(true, true, this);
	m_pkItemImg->InitState(true, true, this);
	m_pkCoinImg->InitState(true, true, this);

	m_pkStaticGrade->SetFontColor(0xffffffff);
	m_pkStaticGrade->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticGrade->SetAlign(DT_CENTER);
	m_pkStaticGrade->InitState(true, true, this);

	m_pkStaticName->SetFontColor(0xffffffff);
	m_pkStaticName->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticName->SetAlign(DT_LEFT);
	m_pkStaticName->SetShadow(true);
	m_pkStaticName->InitState(true, false, this);

	m_pkStaticCoinNum->SetFontColor(0xffffffff);
	m_pkStaticCoinNum->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticCoinNum->SetAlign(DT_LEFT);
	m_pkStaticCoinNum->InitState(true, true, this);
}

void KGCHeroDungeonUpgradeSlot::ClearInfo(void)
{
	m_pkItemImg->ShowItem(false);
	m_pkItemImg->SetItemInfo(KItem::INVALID_ITEM);

	m_kItemInfo.m_ItemID = KItem::INVALID_ITEM;
	m_kItemInfo.m_vecMaterials.clear();

	m_pkStaticName->ToggleRender(false);
}

void KGCHeroDungeonUpgradeSlot::SetItemInfo(const IN KHeroItemUpgradeInfo& Info_)
{
	ClearInfo();

	if (Info_.m_vecMaterials.empty() || Info_.m_ItemID == 0) {
		return;
	}

	m_kItemInfo = Info_;

	m_kCurrentItem = g_pItemMgr->GetItemData(m_kItemInfo.m_ItemID / 10);

	if (m_kCurrentItem == NULL) {
		return;
	}

	m_pkStaticName->SetForceWordWrapText(m_kCurrentItem->strItemName, true);
	m_pkStaticName->ToggleRender(true);

	m_pkItemImg->SetItemInfo(m_kCurrentItem->dwGoodsID);
	m_pkItemImg->ShowItem(true);

	m_pkCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(m_kItemInfo.m_vecMaterials[1].m_ItemID / 10));
	m_iPrice = m_kItemInfo.m_vecMaterials[1].m_nDuration;
	std::wostringstream strm;
	strm << m_iPrice;
	m_pkStaticCoinNum->SetText(strm.str());
	MakeGradeString();

	KItem* pKMoney = g_pItemMgr->m_kInventory.FindItemByItemID(m_kItemInfo.m_vecMaterials[1].m_ItemID / 10);
	KItem* pItemHas = g_pItemMgr->m_kInventory.FindItemByItemID(m_kItemInfo.m_vecMaterials[0].m_ItemID / 10);

	if (pKMoney && pItemHas && pItemHas->m_cGradeID == (char)m_kItemInfo.m_nGrade - 1) {
		m_pkBack->ToggleRender(true);
		m_pkBackLock->ToggleRender(false);
		m_pkBackOn->ToggleRender(false);
		m_pkStaticGrade->SetFontColor(g_pItemMgr->GetItemGradeColorRGB(m_kItemInfo.m_nGrade));
		m_pkStaticName->SetFontColor(g_pItemMgr->GetItemGradeColorRGB(m_kItemInfo.m_nGrade));
		m_pkStaticCoinNum->SetFontColor(0xffffffff);
	}
}

void KGCHeroDungeonUpgradeSlot::MakeGradeString() {
	wstring strGradeString = g_pItemMgr->GetItemGradeString(m_kItemInfo.m_nGrade);
	wstring strToErase = L"Item ";

	size_t pos = std::wstring::npos;
	while ((pos = strGradeString.find(strToErase)) != std::wstring::npos) {
		strGradeString.erase(pos, strToErase.length());
	}

	m_pkStaticGrade->SetText(strGradeString.c_str());
}

void KGCHeroDungeonUpgradeSlot::FrameMoveInEnabledState(void) {
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y);

	KItem* pKMoney = g_pItemMgr->m_kInventory.FindItemByItemID(m_kItemInfo.m_vecMaterials[1].m_ItemID / 10);
	KItem* pItemHas = g_pItemMgr->m_kInventory.FindItemByItemID(m_kItemInfo.m_vecMaterials[0].m_ItemID / 10);

	if (m_pkBack->CheckPosInWindowBound(vMousePos)) {
		m_pkBackOn->ToggleRender(true);
	}
	else {
		if(false == IsSlotActive)
			m_pkBackOn->ToggleRender(false);
	}

	if (!(pKMoney && pItemHas && pItemHas->m_cGradeID == (char)m_kItemInfo.m_nGrade - 1)) {
		m_pkBack->ToggleRender(false);
		m_pkBackLock->ToggleRender(true);
		m_pkBackOn->ToggleRender(false);
		m_pkItemImg->ToggleRender(true);
		m_pkStaticGrade->SetFontColor(D3DCOLOR_ARGB(255, 96, 108, 124));
		m_pkStaticName->SetFontColor(D3DCOLOR_ARGB(255, 96, 108, 124));
		m_pkStaticCoinNum->SetFontColor(D3DCOLOR_ARGB(255, 96, 108, 124));
	}

	if (m_pkItemImg->CheckPosInWindowBound(vMousePos))
	{
		D3DXVECTOR2 vecPos = m_pkItemImg->GetCurrentWindowPos();

		vecPos.x *= m_fWindowScaleX;
		vecPos.y *= m_fWindowScaleY;
		vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

		GCItem* pItem = g_pItemMgr->GetItemData(m_pkItemImg->GetItemID());

		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pItem);
		g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);
	}

	if (true == IsSlotActive) {
		m_pkBack->ToggleRender(false);
		m_pkBackLock->ToggleRender(false);
		m_pkBackOn->ToggleRender(true);
	}

}

void KGCHeroDungeonUpgradeSlot::OnDestroy(void) {

}

void KGCHeroDungeonUpgradeSlot::ActionPerformed(const KActionEvent& event) {
	GCWND_MSG_MAP(m_pkBackOn, KD3DWnd::D3DWE_BUTTON_CLICK, ClickBackOn);
}

void KGCHeroDungeonUpgradeSlot::OnCreateComplete(void) {
}

void KGCHeroDungeonUpgradeSlot::InitializeData() {

}

void KGCHeroDungeonUpgradeSlot::ClickBackOn() {
	g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->InitUIStateSide();
	g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->UpdateItemSlotListSide(m_kItemInfo);

	for (int i = 0; i < (int)g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide.size(); ++i) {
		g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide[i]->SelectedItem(false);
	}

	IsSlotActive = true;
}