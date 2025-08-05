#include "stdafx.h"
#include "stdafx.h"
#include "MyD3D.h"
#include "GCUI/GCShopItemWnd.h"
#include "GCUI/GCSetItemWnd.h"
#include "KGCHeroDungeonShopS6.h"
#include "KGCHeroDungeonShopItemSlot.h"
#include "GCUI/GCItemInformationBoxPair.h"
#include "GCUI/KGCPetPreview.h"
#include "GCUI/KGCPetMessage.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonShopItemSlot);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonShopItemSlot);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonShopItemSlot, "gc_herodungeonshopitem");

KGCHeroDungeonShopItemSlot::KGCHeroDungeonShopItemSlot(void) :
	m_pkItemFrame(NULL),
	m_pkItemFrameOn(NULL),
	m_pkItemImg(NULL),
	m_pkStaticName(NULL),
	m_pkBtnItemView(NULL),
	m_pkStaticCost(NULL),
	m_pkBtnBuy(NULL),
	m_pkCoinImg(NULL),
	m_pkIsPreview(false),
	m_pkHasItem(false),
	m_bItemHovered(false)
{
	LINK_CONTROL("item_frame", m_pkItemFrame);
	LINK_CONTROL("item_frame_on", m_pkItemFrameOn);
	LINK_CONTROL("item_img", m_pkItemImg);
	LINK_CONTROL("static_name", m_pkStaticName);
	LINK_CONTROL("btn_item_view", m_pkBtnItemView);
	LINK_CONTROL("static_cost", m_pkStaticCost);
	LINK_CONTROL("btn_buy", m_pkBtnBuy);
	LINK_CONTROL("coin_img", m_pkCoinImg);
}

KGCHeroDungeonShopItemSlot::~KGCHeroDungeonShopItemSlot(void)
{
}

void KGCHeroDungeonShopItemSlot::OnCreate(void)
{
	m_pkItemFrame->InitState(true, false);
	m_pkItemFrameOn->InitState(false, false);
	m_pkItemImg->InitState(true, false);
	m_pkStaticName->SetFontColor(0xffffffff);
	m_pkStaticName->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticName->SetAlign(DT_LEFT);
	m_pkStaticName->SetShadow(true);
	m_pkStaticName->InitState(true, false);
	m_pkBtnItemView->InitState(false, true, this);
	m_pkStaticCost->SetFontColor(0xffffffff);
	m_pkStaticCost->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticCost->SetAlign(DT_LEFT);
	m_pkStaticCost->InitState(true, false);
	m_pkBtnBuy->InitState(false, true, this);
	m_pkCoinImg->InitState(false, false);
}

void KGCHeroDungeonShopItemSlot::OnDestroy(void)
{
}

void KGCHeroDungeonShopItemSlot::FrameMoveInEnabledState(void)
{
	POINT mousePos = g_pkInput->GetMousePos();
	m_bItemHovered = CheckPosInWindowBound(D3DXVECTOR2((float)mousePos.x, (float)mousePos.y));
	if (m_bItemHovered && m_pkHasItem)
	{
		if ( g_pItemMgr->IsPackageItemID( m_pkGCItem->dwGoodsID * 10 ) || (g_pItemMgr->IsEquipmentItem( m_pkGCItem->dwGoodsID ) && ( !( m_pkGCItem->dwSlotPosition & ESP_A_EARRING ) && ( m_pkGCItem->eItemKind != GIK_MONSTER_CARD ) ) ) )
		{
			m_pkBtnItemView->ToggleRender(true);
		}
	}
	else
	{
		m_pkBtnItemView->ToggleRender(false);
	}

	const D3DXVECTOR2 vMousePos = D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y);

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

void KGCHeroDungeonShopItemSlot::ActionPerformed(const KActionEvent& event)
{
	if (m_pkGCItem->dwSlotPosition == ESP_PET) {
		GCWND_MSG_MAP(m_pkBtnItemView, KD3DWnd::D3DWE_BUTTON_CLICK, OnPetPreview);
	}
	else {
		GCWND_MSG_MAP(m_pkBtnItemView, KD3DWnd::D3DWE_BUTTON_CLICK, CheckPreview);
	}
	GCWND_MSG_MAP(m_pkBtnBuy, KD3DWnd::D3DWE_BUTTON_CLICK, BuyItem);
}

void KGCHeroDungeonShopItemSlot::OnPetPreview()
{
	if (g_pkUIScene->m_pkPetPreview)
	{
		// 진화한 녀석의 프리뷰를 보여준다
		g_pkUIScene->MessageBox(KGCUIScene::GC_PET_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		g_pkUIScene->m_pkPetPreview->SetMyInfoPreview();
		g_pkUIScene->m_pkPetPreview->SetDescState(2);
		g_pkUIScene->m_pkPetPreview->SetNowPet(g_kGlobalValue.GetPetIDFromItemID(m_pkItemImg->GetItemID()), 0);
		g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState(PET_MENT_FIRST_MEETING);
	}
}

void KGCHeroDungeonShopItemSlot::PostChildDraw(void)
{
	m_pkBtnItemView->Render();
}

void KGCHeroDungeonShopItemSlot::SetHeroItem(GCItem* item, KHeroItemInfo heroItemInfo)
{
	m_pkHasItem = true;
	m_pkGCItem = item;
	m_pkStaticName->SetForceWordWrapText(g_pItemMgr->GetItemName(item->dwGoodsID), true);
	m_pkStaticName->SetFontColor(g_pItemMgr->GetItemGradeColorRGB(item->cItemGrade));
	m_pkItemImg->SetItemInfo(item->dwGoodsID);
	m_pkHeroItemInfo = heroItemInfo;
	m_pkStaticCost->SetText(L"");

	for (int i = 0; i < static_cast<int>(heroItemInfo.m_vecMaterialInfo.size()); ++i)
	{
		if (heroItemInfo.m_vecMaterialInfo[i].m_cEquipType == 0) {
			m_iPrice = heroItemInfo.m_vecMaterialInfo[i].m_nDuration;

			GCItem* kItemName = g_MyD3D->m_kItemMgr.GetItemData(heroItemInfo.m_vecMaterialInfo[i].m_ItemID / 10);

			std::wstring strName;
			strName = kItemName->strItemName;
			ItemName = strName.c_str();
			
			std::wostringstream strprice;
			strprice << m_iPrice;

			m_pkCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(heroItemInfo.m_vecMaterialInfo[i].m_ItemID / 10));
			m_pkBtnBuy->ToggleRender(true);
			m_pkCoinImg->ToggleRender(true);
			m_pkStaticCost->SetText(strprice.str());
			break;
		}
	}
}

void KGCHeroDungeonShopItemSlot::CheckPreview(void)
{
	bool bPackage = g_pItemMgr->IsPackageItemID(m_pkGCItem->dwGoodsID * 10);

	if ((g_pItemMgr->IsEquipmentItem(m_pkGCItem->dwGoodsID) && !(m_pkGCItem->dwSlotPosition & ESP_A_EARRING) && (m_pkGCItem->eItemKind != GIK_MONSTER_CARD)) || bPackage)
	{
		if (bPackage)
		{
			std::vector< KDropItemInfo > vecItems;
			g_pItemMgr->GetPackageElements(m_pkGCItem->dwGoodsID * 10, vecItems);
			for (auto element : vecItems)
				g_HeroDungeonShopS6.PreviewItem( g_pItemMgr->GetItemData( element.m_ItemID / 10 ) );
		}
		else
			g_HeroDungeonShopS6.PreviewItem( m_pkGCItem );
	}

	g_HeroDungeonShopS6.UpdateShopPanel();
}

void KGCHeroDungeonShopItemSlot::BuyItem(void)
{
	if (m_iPrice == NULL) return;
	std::wostringstream strmChatMsg;
	std::wostringstream strItemName;

	if ( m_pkHeroItemInfo.m_ItemInfo.m_nDuration > -1 )
	{
		g_kGlobalValue.m_pkCurrentUseItemID			= m_pkHeroItemInfo.m_ItemInfo.m_ItemID / 10;
		g_kGlobalValue.m_iCurrentUseItemMultiplier	= m_pkHeroItemInfo.m_ItemInfo.m_nDuration;
		g_kGlobalValue.m_pkCurrentUseItemCurrency	= m_pkHeroItemInfo.m_vecMaterialInfo[0].m_ItemID / 10;
		g_kGlobalValue.m_iCurrentUseItemQuantity	= m_pkHeroItemInfo.m_pairIdIndex.first;
		g_kGlobalValue.m_pkCurrentUseItemUID		= m_pkHeroItemInfo.m_pairIdIndex.second;
		g_kGlobalValue.m_iCurrentUseItemPrice		= m_pkHeroItemInfo.m_vecMaterialInfo[0].m_nDuration;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_kGlobalValue.m_pkCurrentUseItemID, KBuySellItemReq::EMT_GEM, true, true );
	}
	else
	{
		g_kGlobalValue.m_pkCurrentUseItemID = m_pkHeroItemInfo.m_ItemInfo.m_ItemID / 10;
		g_kGlobalValue.m_iCurrentUseItemMultiplier = m_pkHeroItemInfo.m_ItemInfo.m_nDuration;
		g_kGlobalValue.m_pkCurrentUseItemCurrency = m_pkHeroItemInfo.m_vecMaterialInfo[0].m_ItemID / 10;
		g_kGlobalValue.m_iCurrentUseItemQuantity = m_pkHeroItemInfo.m_pairIdIndex.first;
		g_kGlobalValue.m_pkCurrentUseItemUID = m_pkHeroItemInfo.m_pairIdIndex.second;
		g_kGlobalValue.m_iCurrentUseItemPrice = m_pkHeroItemInfo.m_vecMaterialInfo[0].m_nDuration;
		GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( m_pkHeroItemInfo.m_ItemInfo.m_ItemID / 10 );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
		g_pkStrLoader->GetReplacedString( STR_ID_BUY_HERO_ITEM_COIN_MATERIAL, "lil", kItem->strItemName, m_iPrice, ItemName ), L"", // alterar depois.
		KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, m_pkHeroItemInfo.m_pairIdIndex.second, m_pkHeroItemInfo.m_pairIdIndex.first, true, true );
	}
}

void KGCHeroDungeonShopItemSlot::SetEquipped(bool render)
{
	m_pkItemFrameOn->ToggleRender(render);
}