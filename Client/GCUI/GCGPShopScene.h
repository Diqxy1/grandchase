#ifndef _GCGPSHOPSCENE_H_
#define _GCGPSHOPSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"
#include "GCUI/GCShopItemBox.h"
//#include "GCUI/KGCPetPreview.h"
//#include "GCEnum.h"

class KGCShopTab;
class KD3DComboBox;
class KGCInfoPanel;
class KGCStatusInfoPanel;
class KGCCharWnd;
class KGCSearchBar;

//Alto-Falante
class KGCBillBoardChatBoard;

class KGCGPShopScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME(KGCGPShopScene);
    /// Default constructor
    KGCGPShopScene(void);
    /// Default destructor
    virtual ~KGCGPShopScene(void);
    /// Copy constructor
    KGCGPShopScene(const KGCGPShopScene&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCGPShopScene& operator=(const KGCGPShopScene&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    void ActionPerformed(const KActionEvent& event);
    void SetCharItem(EGCCharType iChar);
    void OnInitialize();
    inline ShopTabType GetShopTabType(void) const { return m_ShopTabType; }
    void UpdateUserInfo();
    void ResetEquipItems();
    void Update()
    {
        m_pkItemBox->UpdateEquip();
    }
    void RefreshtemBox()
    {
        m_pkItemBox->RefreshList();
    }
    void FindItemFromName(const WCHAR* strName_);
    void OnLeaveScene(void);
    void UpdateItemView();

    virtual void OnCreate(void);

protected:

    virtual void OnDestroy(void);
    virtual void FrameMoveInEnabledState(void);

    void OnChangeItemInfo();
    virtual void OnActivateComplete(void);
    virtual void OnEnterScene(void);
    void FilterGoodsItem(ShopTabType TabType);
    void SetActiveShopTab(ShopTabType TabType, bool bInit_ = true);
    void OnCreateComplete();
    void ChangeSortType(int iCategory, int iSortType, int iMoneyType);
    void SetComboPopupList(bool bResetCategory_ = true, bool bResetSortType_ = true, bool bResetMoneyType_ = true);
    void DrawInfoWindow();
    void SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_);

protected:
    KGCShopItemBox* m_pkItemBox;
    KSafeArray<KD3DWnd*, NUM_TAB> m_apkShopTab;
    KD3DComboBox* m_pkItemCombo;
    KD3DComboBox* m_pkSortCombo;
    KD3DComboBox* m_pkMoneyTypeCombo;
    KGCInfoPanel* m_pkUserInfo;
    KGCStatusInfoPanel* m_pkStatusPanel;

    UCHAR				m_ucCharNum;
    KGCShopTab* m_pkCurActiveTab;
    ShopTabType			m_ShopTabType;

    int                 m_iCurCategory;
    int                 m_iCurSortType;
    int					m_iCurMoneyType;

    KD3DWnd* m_pkWelcomeBack_btn;

    bool m_bShowUserInfo;
    bool m_bShowStatusInfo;
    KD3DWnd* m_pkActiveUserInfoBtn;
    KD3DWnd* m_pkDeactiveUserInfoBtn;
    KD3DWnd* m_pkActiveStatusInfoBtn;
    KD3DWnd* m_pkDeactiveStatusInfoBtn;


    KD3DWnd* m_pkBntUserInfo;
    KD3DWnd* m_pkBntUserStatus;


    KD3DWnd* m_pkBtnPointItemToggle;
    KD3DWnd* m_pkBtnCashItemToggle;
    bool                m_bPointItemToggle;
    int                 m_iCurTabCategory;

    KD3DWnd* m_pkBtnExit;
    KGCCharWnd* m_pkCharWnd;
public:
    bool IsPointMode() { return m_bPointItemToggle; };
    void InitPointshopTap();
    void UpdateUserPanel();

public:
    KGCSearchBar* m_pkSearchBar;
    //Alto-falante
    KGCBillBoardChatBoard* m_pkBillboardChatBoard;
};

DEFINE_WND_FACTORY(KGCGPShopScene);
DECLARE_WND_FACTORY(KGCGPShopScene);
DECLARE_WND_FACTORY_NAME(KGCGPShopScene);

#endif // _GCGPSHOPSCENE_H_
