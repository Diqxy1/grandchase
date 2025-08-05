#include "stdafx.h"
#include "KGCHeroDungeonRewardItemS6.h"
#include "GCUI\KGCHeroDungeonRewardDlg.h"
#include "GCUI/GCItemInformationBoxPair.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonRewardItemS6);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonRewardItemS6);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonRewardItemS6, "gc_herodungeonrewarditem");

KGCHeroDungeonRewardItemS6::KGCHeroDungeonRewardItemS6(void) :
	m_pkItemFrame(NULL)
	, m_pkItemFrameOn(NULL)
	, m_pkItemImg(NULL)
	, m_pkStaticDesc(NULL)
	, m_pkItemSel(NULL)
	, IsSlotSelected(false)
	, m_kCurrentItem(NULL)
{
	LINK_CONTROL("item_frame", m_pkItemFrame);
	LINK_CONTROL("item_frame_on", m_pkItemFrameOn);
	LINK_CONTROL("item_img", m_pkItemImg);
	LINK_CONTROL("static_desc", m_pkStaticDesc);
	LINK_CONTROL("item_sel", m_pkItemSel);
}

KGCHeroDungeonRewardItemS6::~KGCHeroDungeonRewardItemS6() {

}

void KGCHeroDungeonRewardItemS6::OnDestroy(void) {

}

void KGCHeroDungeonRewardItemS6::OnCreate(void) {
	m_pkItemFrame->InitState(true, false, this);
	m_pkItemFrameOn->InitState(false, false, this);
	m_pkItemSel->InitState(false, false, this);

	m_pkItemImg->InitState(true, false);

	m_pkStaticDesc->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticDesc->SetAlign(DT_LEFT);
	m_pkStaticDesc->InitState(true, false, this);
}

void KGCHeroDungeonRewardItemS6::OnCreateComplete(void) {

}

void KGCHeroDungeonRewardItemS6::ActionPerformed(const KActionEvent& event) {

}

void KGCHeroDungeonRewardItemS6::SelectSlot(bool IsSelected) {

	int nSelect = SiKGCWorldMapManager()->GetDungeonRewardNum(GC_GM_QUEST52, g_kGlobalValue.m_kUserInfo.mapStagePlayable);
	bool bOpen = nSelect > 0 ? true : false;

	if (bOpen && IsSelected) {
		IsSlotSelected = true;
		m_pkItemSel->ToggleRender(bOpen);
	}
	else {
		IsSlotSelected = false;
		m_pkItemSel->ToggleRender(false);
	}
}

void KGCHeroDungeonRewardItemS6::FrameMoveInEnabledState(void) {
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y);

	if (m_pkItemFrame->CheckPosInWindowBound(vMousePos)) {
		m_pkItemFrameOn->ToggleRender(true);
	}
	else {
		m_pkItemFrameOn->ToggleRender(false);
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
}

void KGCHeroDungeonRewardItemS6::ClearInfo(void) {
	if(m_pkItemSel)
		m_pkItemSel->ToggleRender(false);
}

void KGCHeroDungeonRewardItemS6::SetItemInfo(DWORD itemID) {

	GCItem* GetItem = g_pItemMgr->GetItemData(itemID);
	m_kCurrentItem = GetItem;
	m_pkItemImg->SetItemInfo(GetItem->dwGoodsID);
	m_pkStaticDesc->SetFontColor(g_pItemMgr->GetItemGradeColorRGB(GetItem->cItemGrade));
	m_pkStaticDesc->SetForceWordWrapText(GetItem->strItemName, true);
	g_pkUIScene->m_pkHeroRewardS6->GetItemSelected(GetItem->dwGoodsID);
	IsSlotSelected = false;
}

void KGCHeroDungeonRewardItemS6::SetHeroItemInfo(KHeroItemInfo HeroItemInfo) {
	m_kCurrenHeroItem = HeroItemInfo;
}