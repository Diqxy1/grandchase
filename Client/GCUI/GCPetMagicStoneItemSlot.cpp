#include "stdafx.h"
#include "GCPetMagicStoneItemSlot.h"
#include "GCPetMagicStone.h"

IMPLEMENT_CLASSNAME(KGCPetMagicStoneItemSlot);
IMPLEMENT_WND_FACTORY(KGCPetMagicStoneItemSlot);
IMPLEMENT_WND_FACTORY_NAME(KGCPetMagicStoneItemSlot, "gc_petmagicstoneitemslot" );

KGCPetMagicStoneItemSlot::KGCPetMagicStoneItemSlot()
: m_pkItemImg(nullptr)
, m_pkItemName(nullptr)
, m_pkBtnEquip(nullptr)
, m_pkItemDesc(nullptr)
{
	LINK_CONTROL("item_img", m_pkItemImg);
	LINK_CONTROL("static_name", m_pkItemName);
	LINK_CONTROL("btn_equip", m_pkBtnEquip);
	LINK_CONTROL("static_desc", m_pkItemDesc);
}

KGCPetMagicStoneItemSlot::~KGCPetMagicStoneItemSlot(void)
{
}

void KGCPetMagicStoneItemSlot::ActionPerformed( const KActionEvent& event )
{
	if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		if (event.m_pWnd == m_pkBtnEquip)
		{
			g_KDSound.Play("73");
			SpeakToActionListener(KActionEvent(this, KGCUIScene::D3DWE_PET_MAGIC_STONE_EQUIP_CLICK, m_iIndex, m_dwItemUID));
			return;
		}
	}
}

void KGCPetMagicStoneItemSlot::OnCreate()
{
	if (m_pkItemImg)
	{
		m_pkItemImg->InitState(true);
	}

	if (m_pkItemName)
	{
		m_pkItemName->InitState(true);
		m_pkItemName->SetFontColor(D3DCOLOR_RGBA(51, 25, 0, 255));
	}

	if (m_pkItemDesc)
	{
		m_pkItemDesc->InitState(true);
		m_pkItemDesc->SetFontColor(D3DCOLOR_RGBA(102, 51, 0, 255));
	}

	if (m_pkBtnEquip)
	{
		m_pkBtnEquip->InitState(true, true, this);
	}
}

void KGCPetMagicStoneItemSlot::AddItem(GCITEMUID dwItemUID)
{
	m_dwItemUID = dwItemUID;

	auto pItem = g_pItemMgr->GetInventoryItem(dwItemUID);
	auto gItem = g_pItemMgr->GetItemData(pItem->m_ItemID);

	m_pkItemImg->SetItemInfo(pItem->m_ItemID);
	m_pkItemName->SetText(gItem->strItemName);	
	m_pkItemDesc->SetText(SiGCPetMagicStone()->GetEffectDesc(pItem->m_ItemID));
	m_pkItemDesc->SetFontSize(SiKGCMultipleLanguages()->GetMiddleFontSize());
}

void KGCPetMagicStoneItemSlot::FrameMoveInEnabledState()
{
	if (CheckMousePosInWindowBound())
	{
		KItem* pInvItem = NULL;
		GCItem* pItem = NULL;

		if (g_pItemMgr->FindInventory(m_dwItemUID, &pInvItem, &pItem) == false)
		{
			return;
		}

		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pItem, -1, pInvItem, NULL, NULL, -1, -1);
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

		D3DXVECTOR2 vecPos(GetCurrentWindowPos());
		vecPos.x += GetWidth();
		vecPos.x *= m_fWindowScaleX;
		vecPos.y *= m_fWindowScaleY;

		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
		g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
	}
}