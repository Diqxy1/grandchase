#pragma once
#include "stdafx.h"
#include "MyD3D.h"
#include "KGCRoomManager.h"
#include "GCStateMachine.h"
#include "GCTextColorManager.h"
#include "GCGraphicsHelper.h"
#include "GCGlobal.h"
#include <GCUI\GCCommonItemWnd.h>
#include "../GCUI/GCItemImgWnd.h"

class GCItemImgWnd;

class KGCHeroDungeonUpgradePanel : public KD3DWnd    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCHeroDungeonUpgradePanel);
	/// Default constructor
	KGCHeroDungeonUpgradePanel(void);
	/// Default destructor
	virtual ~KGCHeroDungeonUpgradePanel(void);
	/// Copy constructor
	KGCHeroDungeonUpgradePanel(const KGCHeroDungeonUpgradePanel&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCHeroDungeonUpgradePanel& operator=(const KGCHeroDungeonUpgradePanel&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}
public:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void OnCreateComplete(void);
	virtual void PostChildDraw(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void OnDestroyComplete(void);
	virtual void ActionPerformed(const KActionEvent& event);

	void SetMyHeroCoin(void);
	void SetItemUpgradeInfo(void);
	void SendItemUpgradeInfoReq(void);
	void OnPrevPage(void);
	void OnNextPage(void);
	void ResetPage(int iItemCount_);
	void ResetSlotStatus(void);

	void OnClickHeroUpgradeTab();
	EGCCharType m_eCurrentCharacter;

	enum EShopUpgradeCategoryType
	{
		ETAB_HERO_ALL,
		ETAB_HERO_EPIC,
		ETAB_HERO_RELIC,
		ETAB_HERO_SIZE
	};

	EShopUpgradeCategoryType m_eCurrentShopUpgradeCategoryType; // var pequena ._. kkkk
	int     m_iCurrentPage;
	void UpdateFrame(EShopUpgradeCategoryType eTab, int iCurrentPage);

private:

	std::map<EShopUpgradeCategoryType, std::vector<KHeroItemUpgradeInfo>> m_mapUpgrageInfo;

	enum { NUM_HERO_UPGRADE_BOX = 9 };

	KSafeArray<KD3DWnd*, ETAB_HERO_SIZE> m_pkTabWnd;
	KSafeArray<KD3DWnd*, ETAB_HERO_SIZE> m_pkTabWndOn;
	KSafeArray<KGCHeroDungeonUpgradeSlot*, NUM_HERO_UPGRADE_BOX>  m_apkItemBox;

	KD3DStatic* m_pkStaticHeroCoin;
	KD3DStatic* m_pkStaticPage;

	KD3DWnd* m_pkBtnPrevPage;
	KD3DWnd* m_pkBtnNextPage;

	KGCItemImgWnd* m_pkHeroCoinImg;

	KD3DWnd* m_pkBack;


	int     m_iTotalPage;

	void PushItemMap(EShopUpgradeCategoryType eTab, KHeroItemUpgradeInfo heroItemUpgrade);
	void ChangeTab(EShopUpgradeCategoryType eTab);
};

DEFINE_WND_FACTORY(KGCHeroDungeonUpgradePanel);
DECLARE_WND_FACTORY(KGCHeroDungeonUpgradePanel);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonUpgradePanel);