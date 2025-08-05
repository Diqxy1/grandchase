#include "GCItemCombinationManager.h"

ImplementSingleton(GCItemCombinationManager);

GCItemCombinationManager::GCItemCombinationManager(void)
	: m_isEventPeriod(false)
	, m_bEndParticle(false)
	, m_bEnbaleStuffClick(true)
{
	m_dwMaxMaterialListNum = 0;
	m_vecMaterialList.clear();
	m_vecRewardList.clear();
	m_vecCombinationList.clear();
}

GCItemCombinationManager::~GCItemCombinationManager(void)
{
}

void GCItemCombinationManager::InitData()
{
	m_dwMaxMaterialListNum = 0;
	m_bEndParticle = false;
	m_bEnbaleStuffClick = true;
	m_vecMaterialList.clear();
	m_vecRewardList.clear();
	m_vecCombinationList.clear();
}

void GCItemCombinationManager::OnItemComposeInfoNOT(KEVENT_ITEM_COMPOSE_INFO_NOT& kRecv)
{
	InitData();

	m_dwMaxMaterialListNum = kRecv.m_dwMaxMaterialListNum;
	m_vecMaterialList = kRecv.m_vecMaterialList;
	m_vecRewardList = kRecv.m_vecRewardList;
}

bool GCItemCombinationManager::MakeMaterialList(OUT std::vector<std::pair<GCITEMID, GCITEMUID>>& vecInvenList)
{
	std::vector<GCITEMID>::iterator vit = m_vecMaterialList.begin();

	if (vit == m_vecMaterialList.end() || m_vecMaterialList.empty())
		return false;

	for (; vit != m_vecMaterialList.end(); vit++)
	{
		std::vector< KItem* > vecItemList;
		if (g_pItemMgr->GetInventoryItemListFromItemID((*vit) / 10, vecItemList)) {
			std::vector<KItem*>::iterator vit2 = vecItemList.begin();
			for (; vit2 != vecItemList.end(); vit2++)
			{
				GCItem* pItemInfo = g_pItemMgr->GetItemData((*vit2)->m_ItemID);
				if (NULL == pItemInfo) {
					continue;
				}

				// 재료가 강화석일 때 처리 - 강화석이고, 장착된 상태면 재료로 사용하지 않는다.
				if (pItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM &&
					(*vit2)->m_EnchantEquipItemUID != 0) {
					continue;
				}

				vecInvenList.push_back(std::make_pair(((*vit2)->m_ItemID), (*vit2)->m_ItemUID));
			}
		}
	}
	return true;
}

bool GCItemCombinationManager::GetRewardItemList(OUT std::vector<GCITEMID>& vecRewardList)
{
	std::vector<GCITEMID>::iterator vit = m_vecRewardList.begin();

	if (vit == m_vecRewardList.end() || m_vecRewardList.empty())
		return false;
	else
		vecRewardList = m_vecRewardList;

	return true;
}

void GCItemCombinationManager::SetEventPeriod(bool isEventPeriod)
{
	m_isEventPeriod = isEventPeriod;
}

void GCItemCombinationManager::SetResultItemUID(IN std::vector<GCITEMUID>& vecCombinationList)
{
	if (!vecCombinationList.empty() && static_cast<int> (vecCombinationList.size()) == m_dwMaxMaterialListNum) {
		m_vecCombinationList.clear();
		m_vecCombinationList = vecCombinationList;
	}
}

void GCItemCombinationManager::Send_ItemComposeReq()
{
	if (!m_vecCombinationList.empty() && static_cast<int> (m_vecCombinationList.size()) == m_dwMaxMaterialListNum) {
		Result_Item_Combination = INT_MAX;
		KP2P::GetInstance()->Send_ItemComposeReq(m_vecCombinationList);
		if (false == g_MyD3D->WaitForServerAck(Result_Item_Combination, -1, 3000, INT_MAX))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1));
		}
	}
};

void GCItemCombinationManager::SetEndParticleTime(bool bEndParticle)
{
	m_bEndParticle = bEndParticle;
}

void GCItemCombinationManager::SetStuffSelect(bool bEnbaleStuffClick)
{
	m_bEnbaleStuffClick = bEnbaleStuffClick;
}