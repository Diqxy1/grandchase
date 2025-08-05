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
class KGCHeroDungeonRewardItemS6;

class KGCHeroDungeonRewardDlg : public KD3DDialog    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCHeroDungeonRewardDlg);
    KGCHeroDungeonRewardDlg(void);
    virtual ~KGCHeroDungeonRewardDlg(void);
    KGCHeroDungeonRewardDlg(const KGCHeroDungeonRewardDlg&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    const KGCHeroDungeonRewardDlg& operator=(const KGCHeroDungeonRewardDlg&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    virtual void OnCreate(void);
    virtual void OnCreateComplete(void);
    virtual void OnDestroy(void);
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void FrameMoveInEnabledState(void);
    void InitializeReawrdData(void);
    void SetHeroItemBox(void);
    void ReleaseLock(void);
    void ResetSlots(void);
    void SetSelectedItem();
    void GetItemSelected(DWORD iItemID);
    void UpdateRewardButton();

    void SetLock(bool bLock) { IsRewardAvaliable = !bLock; }
    bool IsLocked() { return IsRewardAvaliable; }

    bool IsRewardAvaliable;
    enum { NUM_HERO_REWARD_BOX = 6 };
    KSafeArray<KGCHeroDungeonRewardItemS6*, NUM_HERO_REWARD_BOX>  m_apkItemBox;
    int m_pkSelectedSlot;
private:
    std::map<DWORD, std::vector<KHeroItemInfo> > m_mapItemInfo;
    KHeroItemInfo  m_IndexSelected;
    KD3DWnd* m_pkClose;
    KD3DWnd* m_pkTitle;
    KD3DWnd* m_pkTitleInfo;
    KD3DWnd* m_pkBtnOk;
    KD3DStatic* m_pkStaticDesc;
    EGCGameMode m_emGameMode;
    GCItem* m_pkSelectedItem;

};

DEFINE_WND_FACTORY(KGCHeroDungeonRewardDlg);
DECLARE_WND_FACTORY(KGCHeroDungeonRewardDlg);
DECLARE_WND_FACTORY_NAME(KGCHeroDungeonRewardDlg);