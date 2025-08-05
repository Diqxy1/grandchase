#pragma once
#include "GCUI/KGCShortMenuCharView.h"


class KGCCharIcon;
class KGCAccountNickNameFrame;

class KGCCharInfoFrame_New : public KD3DWnd    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCCharInfoFrame_New);
    /// Default constructor
    KGCCharInfoFrame_New(void);
    /// Default destructor
    virtual ~KGCCharInfoFrame_New(void);
    /// Copy constructor
    KGCCharInfoFrame_New(const KGCCharInfoFrame_New&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCCharInfoFrame_New& operator=(const KGCCharInfoFrame_New&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    enum {
        ICON_GAP = 15
    };

    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);

    virtual void OnInitialize();
    virtual void OnDestroyComplete(void);

    virtual void FrameMoveInEnabledState(void);
    virtual void PostChildDraw();

    void DisposeInfos();

    // Executed by scene to setup data
    void InitData();

    void LockEmptySlot(bool bRender_);
    void UpdateEmptySlot();
    void CreateEmptySlot();

    void CreateVDepot();

    std::vector<KD3DWnd*>   m_vecEmptySlot;

    KD3DWnd* m_pkBackGroundWhite;

private:
    // Base calculos is made by position of title variable.
    void CreateNewBar(KCharInfoVector::iterator cit, std::vector<KD3DWnd*>& barList, KD3DWnd*& title);
    void GoChannel();
    void GoRoom();
    void OnMoveChannel();
    void OnClickChannelBtn();
    void ChangeCharacter();

    bool OnClickChangeCharacter(const KActionEvent& event);
    bool OnDblClickChangeCharacter(const KActionEvent& event);
    void OnAddSlot();
    void BuyAddSlotItem(GCITEMID itemID);



    std::vector<KD3DWnd*> m_vecSlotList;

    std::vector <KD3DWnd*> m_vecMPSlotBar;
    std::vector<KD3DWnd*> m_vecAPSlotBar;
    std::vector<KD3DWnd*> m_vecHybridSlotBar;

    KD3DWnd* m_TitleMP;
    KD3DWnd* m_TitleAP;
    KD3DWnd* m_TitleHybrid;
    KD3DWnd* m_CharActiveBar;
    KD3DWnd* m_CharBar;


    KD3DWnd* m_pkBtn_Confirm;
    KD3DWnd* m_pkBtn_Make_char;
    KD3DWnd* m_pkBtnMoveChannel;

    KGCCharWnd* m_pkCharWnd;

    KGCAccountNickNameFrame* m_pkNickNameFrame;
    KD3DWnd* m_pkNameBack;
    KD3DWnd* m_pkVdepot;

    KD3DWnd* m_pkSloatAddIcon;
};

DEFINE_WND_FACTORY(KGCCharInfoFrame_New);
DECLARE_WND_FACTORY(KGCCharInfoFrame_New);
DECLARE_WND_FACTORY_NAME(KGCCharInfoFrame_New);
