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

class KGCHeroDungeonRewardItemS6 : public KD3DDialog    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCHeroDungeonRewardItemS6);
    /// Default constructor
    KGCHeroDungeonRewardItemS6(void);
    /// Default destructor
    virtual ~KGCHeroDungeonRewardItemS6(void);
    /// Copy constructor
    KGCHeroDungeonRewardItemS6(const KGCHeroDungeonRewardItemS6&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCHeroDungeonRewardItemS6& operator=(const KGCHeroDungeonRewardItemS6&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }
public:
    void ClearInfo(void);
    virtual void OnCreate(void);
    virtual void OnDestroy(void);
    virtual void OnCreateComplete(void);
    virtual void FrameMoveInEnabledState(void);
    void SetItemInfo(DWORD itemID);
    void SetHeroItemInfo(KHeroItemInfo HeroItemInfo);
    virtual void ActionPerformed(const KActionEvent& event);
    void SelectSlot(bool IsSelected);
    void SetGameMode(EGCGameMode _emGameMode) { m_emGameMode = _emGameMode; }
    bool IsSlotSelected;
    GCItem* m_kCurrentItem;
    KHeroItemInfo m_kCurrenHeroItem;

private:
    EGCGameMode m_emGameMode;
    KManufactureItem m_kItemInfo;
    KD3DSizingBox* m_pkItemFrame;
    KD3DSizingBox* m_pkItemFrameOn;
    KD3DWnd* m_pkItemSel;
    KGCItemImgWnd* m_pkItemImg;
    KD3DStatic* m_pkStaticDesc;

};

DEFINE_WND_FACTORY(KGCHeroDungeonRewardItemS6);
DECLARE_WND_FACTORY(KGCHeroDungeonRewardItemS6);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonRewardItemS6);