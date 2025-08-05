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
#include "GCUIScene.h"

class GCItemImgWnd;
class KGCItemInformationBoxPair;
class KHeroItemUpgradeInfo;

class KGCHeroDungeonUpgradeSidePanel : public KD3DDialog    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCHeroDungeonUpgradeSidePanel);
    /// Default constructor
    KGCHeroDungeonUpgradeSidePanel(void);
    /// Default destructor
    virtual ~KGCHeroDungeonUpgradeSidePanel(void);
    /// Copy constructor
    KGCHeroDungeonUpgradeSidePanel(const KGCHeroDungeonUpgradeSidePanel&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCHeroDungeonUpgradeSidePanel& operator=(const KGCHeroDungeonUpgradeSidePanel&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    enum
    {
        EM_STATE_NONE = 0,
        EM_STATE_READY = 1,
        EM_STATE_PROCESSING = 2,
        EM_STATE_SUCCESS = 3,
        EM_INVEN_COLUMN = 4,
        MAX_MATERIAL = 5,
        EM_INVEN_ROW = 3,
        EM_INVEN_WIDH_SIZE = 64,
        EM_INVEN_HEIGHT_SIZE = 64,
    };

public:
    virtual void OnCreate(void);
    virtual void OnDestroy(void);
    virtual void OnCreateComplete(void);
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void FrameMoveInEnabledState(void);

    void InitUIStateSide(void);
    void CreateInvenSlotSide(void);
    void UpdateItemSlotListSide(KHeroItemUpgradeInfo HeroItemSide);
    void UpdateInvenItemListSide(void);
    void CheckItemInformationSide(void);
    void OnselectItemSide(KGCCommonItemWnd*, KItem*);
    void DrawItemInfoSide(KItem* pItem, D3DXVECTOR2 vPos);
    void DrawItemInfoSide(GCITEMID pItemID, D3DXVECTOR2 vPos);
    void OnScrollPos(void);
    void OnInvenItemList(int index);
    void OnClickUpgradeSide(void);
    std::pair< GCITEMID, int > GetUpgradeItemKey(void);
    bool GetUpgradeMaterialUIDSideFirst(IN const std::pair< GCITEMID, int > prKey_, OUT std::vector< GCITEMUID >& vecUIDList_);
    bool GetUpgradeMaterialUIDSide(OUT std::vector< GCITEMUID >& vecUIDList_);

    std::vector<KGCCommonItemWnd*> m_vecInvenItemImgListSide;
    std::vector<GCITEMUID>         m_vecItemUIDListSide;
    std::vector<KItem*>            tempHeroUpgradeSlotItemSide;
    std::vector<KD3DWnd*>          tempHeroUpgradeSlotImgSide;
    std::vector<GCITEMUID>         m_vecMaterialUID;
    int                            m_nState;

private:

    GCITEMUID                        m_EquipUID;
    int                              m_iScrollpos;
    KD3DWnd* m_pkBack;
    KD3DWnd* m_pkUpgradeButton;
    KD3DWnd* m_pkUpgradeSucces;
    KGCCommonItemWnd* m_pkMaterialSlot;
    KGCCommonItemWnd* m_pkResultSlot;
    KD3DScrollbar* m_pkScrollbar;
    KItem* m_pkEquipToUpgrade;
    GCITEMID                        m_pkMaterialIDSide;
    KHeroItemUpgradeInfo            m_pkHeroItemSide;
};

DEFINE_WND_FACTORY(KGCHeroDungeonUpgradeSidePanel);
DECLARE_WND_FACTORY(KGCHeroDungeonUpgradeSidePanel);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonUpgradeSidePanel);