#pragma once

#include "stdafx.h"
#include "MyD3D.h"

class KGCHeroDungeonShopS6
{
private:
	std::map<DWORD, GCItem*>	m_mapEquippedItems;

public:
	void ClearEquippedItems();
	void PreviewItem(GCItem* item);
	bool CheckItem(GCItem* item);
	void UpdateShopPanel();
	void ClearUpgradePanel();
	void SendIDForDrwaItemImg(DWORD item);
};

extern KGCHeroDungeonShopS6 g_HeroDungeonShopS6;