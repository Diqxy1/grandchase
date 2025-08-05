#pragma once

#include "stdafx.h"
#include "MyD3D.h"
#include "GCHeroDungeonManager.h"

class KD3DStatic;
class KGCCommonEditBox;
class KGCHeroDungeonShopPanel;
class KGCHeroDungeonShopSidePanel;
class KGCHeroDungeonUpgradePanel;
class KGCHeroDungeonUpgradeSidePanel;
class KGCHeroDungeonShopS6Dlg : public KD3DDialog, public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCHeroDungeonShopS6Dlg);
	/// Default constructor
	KGCHeroDungeonShopS6Dlg(void);
	/// Default destructor
	virtual ~KGCHeroDungeonShopS6Dlg(void);
	/// Copy constructor
	KGCHeroDungeonShopS6Dlg(const KGCHeroDungeonShopS6Dlg&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCHeroDungeonShopS6Dlg& operator=(const KGCHeroDungeonShopS6Dlg&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	enum EShopTabType
	{
		ETAB_HERO_DUNGEON_SHOP,
		ETAB_HERO_ITEM_UPGRADE,
		ETAB_HERO_DUNGEON_SIZE
	};

private:
	void ChangeTab(EShopTabType eTab);
	void OnClose(void);

public:
	void EquipItem(GCItem* item);
	void UnequipItem(GCItem* item);
	void UpdateShopPanel(void);
	void ReloadAnimationMotion(void);
	void UpdateUpgradeItemsTab();
	KGCHeroDungeonShopPanel* m_pkHeroShopPanel;
	KGCHeroDungeonUpgradeSidePanel* m_pkHeroUpgradeSidePanel;
	KGCHeroDungeonShopSidePanel* m_pkHeroShopSidePanel;
	KGCHeroDungeonUpgradePanel* m_pkHeroUpgradePanel;

protected:
	virtual void OnCreate(void);
	virtual void OnCreateComplete(void);
	virtual void OnDestroy(void);
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void OnDestroyComplete(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);

private:
	KSafeArray<KD3DWnd*, ETAB_HERO_DUNGEON_SIZE>	m_pkTabWnd;
	KSafeArray<KD3DWnd*, ETAB_HERO_DUNGEON_SIZE>	m_pkTabWndOn;
	KD3DWnd*										m_pkExit;
	KD3DWnd*										m_pkTitleNormal;
	KD3DWnd*										m_pkTitleVIP;
};

DEFINE_WND_FACTORY(KGCHeroDungeonShopS6Dlg);
DECLARE_WND_FACTORY(KGCHeroDungeonShopS6Dlg);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonShopS6Dlg);