#pragma once
#include "stdafx.h"
#include "MyD3D.h"
#include "KGCRoomManager.h"
#include "GCStateMachine.h"
#include "GCTextColorManager.h"
#include "GCGraphicsHelper.h"
#include "GCGlobal.h"
#include "GCUI\GCCommonItemWnd.h"
#include "GCItemImgWnd.h"
#include <GCUI/KGCHeroDungeonUpgradeSidePanel.h>

class GCItemImgWnd;

class KGCHeroDungeonUpgradeSlot : public KD3DDialog    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCHeroDungeonUpgradeSlot);
	/// Default constructor
	KGCHeroDungeonUpgradeSlot(void);
	/// Default destructor
	virtual ~KGCHeroDungeonUpgradeSlot(void);
	/// Copy constructor
	KGCHeroDungeonUpgradeSlot(const KGCHeroDungeonUpgradeSlot&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCHeroDungeonUpgradeSlot& operator=(const KGCHeroDungeonUpgradeSlot&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}
public:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void OnCreateComplete(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void ActionPerformed(const KActionEvent& event);
	void InitializeData();
	void ClearInfo(void);
	void SetItemInfo(const IN KHeroItemUpgradeInfo& Info_);
	void MakeGradeString();
	void SetBoxNumber(int iBoxNumber_) { m_iBoxNumber = iBoxNumber_; }
	void ClickBackOn();
	bool IsSlotActive;

private:
	KD3DWnd*				m_pkBack;
	KD3DWnd*				m_pkBackLock;
	KD3DWnd*				m_pkBackOn;
	KGCItemImgWnd*			m_pkItemImg;
	KGCItemImgWnd*			m_pkCoinImg;
	KD3DStatic*				m_pkStaticGrade;
	KD3DStatic*				m_pkStaticName;
	KD3DStatic*				m_pkStaticCoinNum;
	KHeroItemUpgradeInfo	m_kItemInfo;
	GCItem*					m_kCurrentItem;
	int						m_iBoxNumber;
	int						m_iPrice;
	GCITEMID				m_iMaterial;
};

DEFINE_WND_FACTORY(KGCHeroDungeonUpgradeSlot);
DECLARE_WND_FACTORY(KGCHeroDungeonUpgradeSlot);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonUpgradeSlot);