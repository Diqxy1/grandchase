#include "StdAfx.h"
#include "KGCHeroDungeonShopPanel.h"
#include "KGCHeroDungeonShopItemSlot.h"
#include "KGCHeroDungeonShopS6.h"
#include "Procedure.h"
#include "MyD3D.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonShopPanel);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonShopPanel);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonShopPanel, "gc_herodungeonshoppanel");

KGCHeroDungeonShopPanel::KGCHeroDungeonShopPanel(void) :
	m_eCurrentShopTabType(ETAB_HERO_DUNGEON_ALL),
	m_nCurrentPage(-1),
	m_nTotalPages(-1),
	m_eCurrentCharacter(GC_CHAR_INVALID),
	m_pkStaticHeroCoin(NULL),
	m_pkHeroCoinImg(NULL),
	m_pkBtnPrevPage(NULL),
	m_pkBtnNextPage(NULL),
	m_pkStaticPage(NULL)
{
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
	{
		m_pkTabWnd[i] = NULL;
		m_pkTabWndOn[i] = NULL;

		char buffTab[MAX_PATH];
		sprintf_s(buffTab, MAX_PATH - 1, "btn_tab%d", i);
		LINK_CONTROL(buffTab, m_pkTabWnd[i]);

		char buffTabOn[MAX_PATH];
		sprintf_s(buffTabOn, MAX_PATH - 1, "btn_tab_on%d", i);
		LINK_CONTROL(buffTabOn, m_pkTabWndOn[i]);
	}

	for (int i = 0; i < NUM_HERO_ITEM_BOX; ++i)
	{
		m_apkItemBox[i] = NULL;

		char buffItemSlot[MAX_PATH];
		sprintf_s(buffItemSlot, MAX_PATH - 1, "itemslot%02d", i);
		LINK_CONTROL(buffItemSlot, m_apkItemBox[i]);
	}

	LINK_CONTROL("static_coin_num", m_pkStaticHeroCoin);

	LINK_CONTROL("coin_img", m_pkHeroCoinImg);

	LINK_CONTROL("btn_prev_page", m_pkBtnPrevPage);
	LINK_CONTROL("btn_next_page", m_pkBtnNextPage);
	LINK_CONTROL("static_page", m_pkStaticPage);

	m_mapItemInfo.clear();
}

KGCHeroDungeonShopPanel::~KGCHeroDungeonShopPanel(void)
{
}

void KGCHeroDungeonShopPanel::OnCreate(void)
{
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i) {
		m_pkTabWnd[i]->InitState(true, true, this);
		m_pkTabWndOn[i]->InitState(false, true, this);
	}

	for (int i = 0; i < NUM_HERO_ITEM_BOX; ++i)
	{
		m_apkItemBox[i]->InitState(true, true, this);
	}

	m_pkBtnPrevPage->InitState(true, true, this);
	m_pkBtnNextPage->InitState(true, true, this);

	m_pkStaticPage->SetFontColor(0xffffffff);
	m_pkStaticPage->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticPage->SetAlign(DT_CENTER);
	m_pkStaticPage->InitState(true, false);

	m_pkStaticHeroCoin->SetFontColor(0xffffffff);
	m_pkStaticHeroCoin->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticHeroCoin->SetAlign(DT_RIGHT);
	m_pkStaticHeroCoin->InitState(true, false);

	m_pkHeroCoinImg->InitState(true, false);
	m_pkHeroCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(ITEM_ID_HERO_COIN));
}

void KGCHeroDungeonShopPanel::OnCreateComplete(void)
{
	UpdateAllHeroShop();
}

void KGCHeroDungeonShopPanel::UpdateAllHeroShop(void) {
	SCharInfo charInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
	m_eCurrentCharacter = static_cast<EGCCharType>(charInfo.iCharType);
	SetHeroItemInfo();
	ChangeTab(ETAB_HERO_DUNGEON_ALL);
	UpdateFrame(ETAB_HERO_DUNGEON_ALL, m_nCurrentPage);
}


void KGCHeroDungeonShopPanel::OnDestroy(void)
{
	m_mapItemInfo.clear();
}

void KGCHeroDungeonShopPanel::ActionPerformed(const KActionEvent& event)
{
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; i++) {
		GCWND_MSG_MAP(m_pkTabWnd[i],
			KD3DWnd::D3DWE_BUTTON_CLICK,
			[&]()
			{
				m_eCurrentShopTabType = static_cast<EShopTabCategoryType>(i);
				ChangeTab(m_eCurrentShopTabType);
				UpdateFrame(m_eCurrentShopTabType, m_nCurrentPage);
			});
	}
	GCWND_MSG_MAP(m_pkBtnPrevPage, KD3DWnd::D3DWE_BUTTON_CLICK, PrevPage);
	GCWND_MSG_MAP(m_pkBtnNextPage, KD3DWnd::D3DWE_BUTTON_CLICK, NextPage);
}

void KGCHeroDungeonShopPanel::OnDestroyComplete(void)
{
}

void KGCHeroDungeonShopPanel::FrameMoveInEnabledState(void)
{
	int nHeroCoinCount = 0;

	if (g_kGlobalValue.isCurrentShopVIP) {
		m_pkHeroCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(ITEM_NEW_SPECIAL_EVENT_COIN));
		nHeroCoinCount = (g_pItemMgr->GetInventoryItemDuration(ITEM_NEW_SPECIAL_EVENT_COIN));
	}
	else {
		m_pkHeroCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(ITEM_ID_HERO_COIN));
		nHeroCoinCount = (g_pItemMgr->GetInventoryItemDuration(ITEM_ID_HERO_COIN));
	}

	m_pkStaticHeroCoin->SetText(nHeroCoinCount);
}

void KGCHeroDungeonShopPanel::PostChildDraw(void)
{
}

void KGCHeroDungeonShopPanel::ChangeTab(EShopTabCategoryType eTab)
{
	if (eTab < 0 || eTab >= ETAB_HERO_DUNGEON_SIZE) return;
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
	{
		if (!m_pkTabWnd[i] && !m_pkTabWndOn[i])
			continue;

		if (i == eTab)
		{
			m_pkTabWnd[i]->ToggleRender(false);
			m_pkTabWndOn[i]->ToggleRender(true);
		}
		else
		{
			m_pkTabWnd[i]->ToggleRender(true);
			m_pkTabWndOn[i]->ToggleRender(false);
		}
	}
	m_nCurrentPage = 1;
}

void KGCHeroDungeonShopPanel::SetHeroItemInfo(void)
{
	if (!m_mapItemInfo.empty())
		return;

	SendHeroItemCatalogReq();
	SendHeroItemMaterialReq();

	std::map<PAIR_USHORT_DWORD, KManufactureItem> mapCatalog = g_kGlobalValue.m_kHeroDungeonInfo.m_mapCatalog;
	std::map<PAIR_USHORT_DWORD, KManufactureItem>::iterator mapIterator = mapCatalog.begin();

	for (; mapIterator != mapCatalog.end(); ++mapIterator)
	{
		PAIR_USHORT_DWORD pairTemp = mapIterator->first;
		std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo>>::iterator materialIterator = g_kGlobalValue.m_kHeroDungeonInfo.m_mapMaterial.find(pairTemp);

		if (g_kGlobalValue.m_kHeroDungeonInfo.m_mapMaterial.end() == materialIterator)
			continue;

		if ((true == g_kGlobalValue.isCurrentShopVIP && !mapIterator->second.CheckItemFlag(0x00000040)) || (false == g_kGlobalValue.isCurrentShopVIP && mapIterator->second.CheckItemFlag(0x00000040))) {
			continue;
		}

		GCItem* itemData = g_pItemMgr->GetItemData(static_cast<DWORD>(mapIterator->second.m_ItemID / 10));
		KHeroItemInfo tempHeroItemInfo;
		tempHeroItemInfo.m_pairIdIndex = mapIterator->first;
		tempHeroItemInfo.m_ItemInfo = mapIterator->second;
		tempHeroItemInfo.m_vecMaterialInfo = materialIterator->second;

		PushItemMap(ETAB_HERO_DUNGEON_ALL, tempHeroItemInfo);
		switch (itemData->dwSlotPosition)
		{
		case ESP_WEAPON:
			PushItemMap(ETAB_HERO_DUNGEON_WEAPON, tempHeroItemInfo);
			break;
		case ESP_HELMET:
		case ESP_JACKET:
		case ESP_PANTS:
		case ESP_GLOVES:
		case ESP_SHOES:
		case ESP_MANTLE:
			PushItemMap(ETAB_HERO_DUNGEON_EQUIP, tempHeroItemInfo);
			break;
		case ESP_A_UPPER_HELMET:
		case ESP_A_DOWN_HELMET:
		case ESP_A_NECKLET:
		case ESP_A_JACKET:
		case ESP_A_PANTS:
		case ESP_A_ARM:
		case ESP_A_FINGER:
		case ESP_A_SHOES:
			PushItemMap(ETAB_HERO_DUNGEON_ACC, tempHeroItemInfo);
			break;
		case ESP_PET:
			PushItemMap(ETAB_HERO_DUNGEON_PET, tempHeroItemInfo);
			break;
		default:
			PushItemMap(ETAB_HERO_DUNGEON_ETC, tempHeroItemInfo);
		}
	}
}

void KGCHeroDungeonShopPanel::SendHeroItemCatalogReq(void)
{
	KP2P::GetInstance()->Send_HeroItemCatalogReq();
	Result_HeroItemList = INT_MAX;
	g_MyD3D->WaitForServerAck(Result_HeroItemList, INT_MAX, 3000, INT_MAX);
}

void KGCHeroDungeonShopPanel::SendHeroItemMaterialReq(void)
{
	std::map<PAIR_USHORT_DWORD, KManufactureItem> mapCatalog = g_kGlobalValue.m_kHeroDungeonInfo.m_mapCatalog;

	if (mapCatalog.empty())
		return;

	std::set<PAIR_USHORT_DWORD> setItemID;
	std::map<PAIR_USHORT_DWORD, KManufactureItem>::iterator mapIterator = mapCatalog.begin();

	for (; mapIterator != mapCatalog.end(); ++mapIterator)
		setItemID.insert(mapIterator->first);

	KP2P::GetInstance()->Send_HeroItemMaterialReq(setItemID);
	Result_HeroItemList = INT_MAX;
	g_MyD3D->WaitForServerAck(Result_HeroItemList, INT_MAX, 3000, INT_MAX);
}

void KGCHeroDungeonShopPanel::PushItemMap(EShopTabCategoryType eTab, KHeroItemInfo heroItem)
{
	std::map<EShopTabCategoryType, std::vector<KHeroItemInfo>>::iterator heroItemInfoIterator = m_mapItemInfo.find(eTab);

	if (m_mapItemInfo.end() == heroItemInfoIterator)
	{
		std::vector<KHeroItemInfo> vecHeroItemInfo;
		vecHeroItemInfo.push_back(heroItem);
		m_mapItemInfo.insert(std::make_pair(eTab, vecHeroItemInfo));
	}
	else
	{
		m_mapItemInfo[eTab].push_back(heroItem);
	}
}

void KGCHeroDungeonShopPanel::PrevPage(void)
{
	--m_nCurrentPage;
	if (m_nCurrentPage < 1)
		m_nCurrentPage = 1;
	UpdateFrame(m_eCurrentShopTabType, m_nCurrentPage);
}

void KGCHeroDungeonShopPanel::NextPage(void)
{
	++m_nCurrentPage;
	if (m_nCurrentPage > m_nTotalPages)
		m_nCurrentPage = m_nTotalPages;
	UpdateFrame(m_eCurrentShopTabType, m_nCurrentPage);
}

void KGCHeroDungeonShopPanel::UpdateFrame(EShopTabCategoryType eTab, int iCurPage)
{
	std::map<EShopTabCategoryType, std::vector<KHeroItemInfo>>::iterator itemIterator = m_mapItemInfo.find(eTab);

	if (m_mapItemInfo.end() == itemIterator)
		return;

	int iStartItemIndex = iCurPage;
	--iStartItemIndex;
	iStartItemIndex *= NUM_HERO_ITEM_BOX;

	std::vector<std::pair<GCItem*, KHeroItemInfo>> tempVector;

	if (itemIterator != m_mapItemInfo.end())
	{
		for (int i = 0; i < static_cast<int>(itemIterator->second.size()); ++i)
		{
			KHeroItemInfo heroItemInfo = itemIterator->second[i];
			GCItem* itemData = g_pItemMgr->GetItemData(heroItemInfo.m_ItemInfo.m_ItemID / 10);
			if (itemData->dwCharType & (1 << m_eCurrentCharacter))
			{
				tempVector.push_back(std::make_pair(itemData, itemIterator->second[i]));
			}
		}
	}

	m_nTotalPages = ceil(tempVector.size() * 2 / 2 / (double)NUM_HERO_ITEM_BOX);
	if (m_nTotalPages <= 0) ++m_nTotalPages;

	std::wostringstream strm;
	strm << m_nCurrentPage << L" / " << m_nTotalPages;
	m_pkStaticPage->SetText(strm.str());

	for (int i = 0; i < NUM_HERO_ITEM_BOX; ++i)
	{
		if (iStartItemIndex < tempVector.size())
		{
			if ((int)tempVector.size() <= iStartItemIndex)
				break;

			std::pair<GCItem*, KHeroItemInfo> tempInfo = tempVector[iStartItemIndex++];
			m_apkItemBox[i]->SetEquipped(g_HeroDungeonShopS6.CheckItem(tempInfo.first));
			m_apkItemBox[i]->SetHeroItem(tempInfo.first, tempInfo.second);
			m_apkItemBox[i]->ToggleRender(true);
		}
		else
		{
			m_apkItemBox[i]->ToggleRender(false);
		}
	}
}

void KGCHeroDungeonShopPanel::UpdateClickShopPanel(void)
{
	ChangeTab(ETAB_HERO_DUNGEON_ALL);
	m_eCurrentShopTabType = ETAB_HERO_DUNGEON_ALL;
	UpdateFrame(m_eCurrentShopTabType, m_nCurrentPage);
}

void KGCHeroDungeonShopPanel::UpdateShopPanel(void)
{
	UpdateFrame(m_eCurrentShopTabType, m_nCurrentPage);
}