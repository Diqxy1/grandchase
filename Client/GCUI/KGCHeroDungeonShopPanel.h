#pragma once
#include "GCHeroDungeonManager.h"
#include "KGCHeroDungeonShopItemSlot.h"

class KD3DStatic;
class KGCHeroDungeonShopItemSlot;
class KGCHeroItemPreview;

class KGCHeroDungeonShopPanel : public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCHeroDungeonShopPanel);
	/// Default constructor
	KGCHeroDungeonShopPanel(void);
	/// Default destructor
	virtual ~KGCHeroDungeonShopPanel(void);
	/// Copy constructor
	KGCHeroDungeonShopPanel(const KGCHeroDungeonShopPanel&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCHeroDungeonShopPanel& operator=(const KGCHeroDungeonShopPanel&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

protected:
	virtual void OnCreate(void);
	virtual void OnCreateComplete(void);
	virtual void OnDestroy(void);
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void OnDestroyComplete(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);

public:
	enum EShopTabCategoryType
	{
		ETAB_HERO_DUNGEON_ALL,
		ETAB_HERO_DUNGEON_WEAPON,
		ETAB_HERO_DUNGEON_EQUIP,
		ETAB_HERO_DUNGEON_ACC,
		ETAB_HERO_DUNGEON_PET,
		ETAB_HERO_DUNGEON_ETC,
		ETAB_HERO_DUNGEON_SIZE,
	};

	void UpdateShopPanel(void);
	void UpdateClickShopPanel(void);
	void ChangeTab(EShopTabCategoryType eTab);
	void UpdateAllHeroShop(void);
	std::map<EShopTabCategoryType, std::vector<KHeroItemInfo>> m_mapItemInfo;
private:
	enum { NUM_HERO_ITEM_BOX = 6 };
	EGCCharType				m_eCurrentCharacter;
	EShopTabCategoryType	m_eCurrentShopTabType;
	int						m_nCurrentPage;
	int						m_nTotalPages;



private:
	void SetHeroItemInfo(void);
	void SendHeroItemCatalogReq(void);
	void SendHeroItemMaterialReq(void);
	void PrevPage(void);
	void NextPage(void);
	void PushItemMap(EShopTabCategoryType eTab, KHeroItemInfo heroItem);
	void UpdateFrame(EShopTabCategoryType eTab, int iCurPage);

private:

	KD3DStatic* m_pkStaticPage;
	KD3DStatic* m_pkStaticHeroCoin;
	KGCHeroItemPreview* m_pkHeroItemPreview;
	KGCItemImgWnd* m_pkHeroCoinImg;

	KSafeArray<KD3DComboBox*, ETAB_HERO_DUNGEON_SIZE>				m_pkTabWnd;
	KSafeArray<KD3DWnd*, ETAB_HERO_DUNGEON_SIZE>					m_pkTabWndOn;
	KSafeArray<KGCHeroDungeonShopItemSlot*, NUM_HERO_ITEM_BOX>		m_apkItemBox;

	KD3DWnd* m_pkBtnPrevPage;
	KD3DWnd* m_pkBtnNextPage;
};

DEFINE_WND_FACTORY(KGCHeroDungeonShopPanel);
DECLARE_WND_FACTORY(KGCHeroDungeonShopPanel);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonShopPanel);