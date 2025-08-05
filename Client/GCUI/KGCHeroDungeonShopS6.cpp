#include "stdafx.h"
#include "MyD3D.h"
#include "GCUI\GCUIScene.h"
#include "KGCHeroDungeonShopS6.h"
#include "KGCHeroDungeonShopS6Dlg.h"

KGCHeroDungeonShopS6	g_HeroDungeonShopS6;


void KGCHeroDungeonShopS6::ClearEquippedItems()
{
	m_mapEquippedItems.clear();
}

void KGCHeroDungeonShopS6::PreviewItem(GCItem* item)
{
	KGCHeroDungeonShopS6Dlg* gcHeroDungeonShopS6Dlg = g_pkUIScene->m_pkHeroDungeonShopS6;
	std::map<DWORD, GCItem*>::iterator equippedIterator = m_mapEquippedItems.find(item->dwSlotPosition);
	if (m_mapEquippedItems.end() == equippedIterator) {
		gcHeroDungeonShopS6Dlg->EquipItem(item);
		m_mapEquippedItems.insert(std::make_pair(item->dwSlotPosition, item));
		if (item->dwSlotPosition == ESP_WEAPON) gcHeroDungeonShopS6Dlg->ReloadAnimationMotion();
		return;
	}
	GCItem* temp = equippedIterator->second;
	gcHeroDungeonShopS6Dlg->UnequipItem(temp);
	m_mapEquippedItems.erase(temp->dwSlotPosition);
	if (temp->dwGoodsID != item->dwGoodsID)
	{
		gcHeroDungeonShopS6Dlg->EquipItem(item);
		m_mapEquippedItems.insert(std::make_pair(item->dwSlotPosition, item));
		if (item->dwSlotPosition == ESP_WEAPON) gcHeroDungeonShopS6Dlg->ReloadAnimationMotion();
	}
}

bool KGCHeroDungeonShopS6::CheckItem(GCItem* item)
{
	std::map<DWORD, GCItem*>::iterator equippedIterator = m_mapEquippedItems.find(item->dwSlotPosition);
	if (m_mapEquippedItems.end() != equippedIterator)
	{
		return equippedIterator->second->dwGoodsID == item->dwGoodsID;
	}
	return false;
}

void KGCHeroDungeonShopS6::UpdateShopPanel()
{
	KGCHeroDungeonShopS6Dlg* gcHeroDungeonShopS6Dlg = g_pkUIScene->m_pkHeroDungeonShopS6;
	gcHeroDungeonShopS6Dlg->UpdateShopPanel();
}

void KGCHeroDungeonShopS6::ClearUpgradePanel()
{

}

void KGCHeroDungeonShopS6::SendIDForDrwaItemImg(DWORD item)
{
}