#pragma once

#include "GCItemImgWnd.h"

class KGCShopItemWnd;
class KGCSetItemWnd;

class KGCHeroDungeonShopItemSlot : public KD3DWnd, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCHeroDungeonShopItemSlot);
	/// Default constructor
	KGCHeroDungeonShopItemSlot(void);
	/// Default destructor
	virtual ~KGCHeroDungeonShopItemSlot(void);
	/// Copy constructor
	KGCHeroDungeonShopItemSlot(const KGCHeroDungeonShopItemSlot&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCHeroDungeonShopItemSlot& operator=(const KGCHeroDungeonShopItemSlot&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}
public:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);

	void SetHeroItem(GCItem* item, KHeroItemInfo heroItemInfo);
	void SetEquipped(bool render);

private:
	void CheckPreview(void);
	void BuyItem(void);
	void OnPetPreview();

	enum SPetMentState
	{
		PET_MENT_INVALID = -1,
		PET_MENT_HUNGRY = 1,
		PET_MENT_SHOP = 2,
		PET_MENT_ROOM = 3,
		PET_MENT_MYINFO = 4,
		PET_MENT_FEEDING = 5,
		PET_MENT_HUNGRY_ZERO = 6,
		PET_MENT_LEVELUP = 7,
		PET_MENT_CHAR_LEVELUP = 8,
		PET_MENT_FEEDING_3000 = 9,
		PET_MENT_FEEDING_6000 = 10,
		PET_MENT_FIRST_MEETING = 11,
		PET_MENT_PLEASE_HATCH = 12,
	};

private:
	KD3DWnd* m_pkItemFrame;
	KD3DWnd* m_pkItemFrameOn;
	KGCItemImgWnd* m_pkItemImg;
	KD3DStatic* m_pkStaticName;
	KD3DWnd* m_pkBtnItemView;
	KD3DStatic* m_pkStaticCost;
	KD3DWnd* m_pkBtnBuy;
	KGCItemImgWnd* m_pkCoinImg;

	int					m_iPrice;
	wstring				ItemName;
	bool				m_pkIsPreview;
	bool				m_pkHasItem;
	GCItem* m_pkGCItem;
	KHeroItemInfo		m_pkHeroItemInfo;
	bool				m_bItemHovered;
	//GCDeviceTexture* m_pkItemTexture;
};

DEFINE_WND_FACTORY(KGCHeroDungeonShopItemSlot);
DECLARE_WND_FACTORY(KGCHeroDungeonShopItemSlot);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonShopItemSlot);
