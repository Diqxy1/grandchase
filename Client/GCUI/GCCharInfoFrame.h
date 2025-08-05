#pragma once
#ifndef NEW_CHAR_SELECT_SCENE

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KGCCharIcon;
class KGCAccountNickNameFrame;

class KGCCharInfoFrame : public KD3DWnd    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCCharInfoFrame);
    /// Default constructor
    KGCCharInfoFrame(void);
    /// Default destructor
    virtual ~KGCCharInfoFrame(void);
    /// Copy constructor
    KGCCharInfoFrame(const KGCCharInfoFrame&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCCharInfoFrame& operator=(const KGCCharInfoFrame&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:

    enum {
        ICON_GAP = 3
    };

    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);

    virtual void OnInitialize();
    virtual void OnDestroyComplete(void);

    virtual void FrameMoveInEnabledState(void);
    virtual void PostChildDraw();

    bool OnDblClickChangeCharacter(const KActionEvent& event);
    void OnClickChannelBtn();
    bool OnClickChangeCharacter(const KActionEvent& event);
    void ChangeCharacter();

    //void UpdateData();
    void SettingCharIconPos();
    void SetSelectedChar();
    void SetSelectedCharInfo();
    void OnTutorial();
    void GoChannel();
    void GoRoom();

    void UpdateUIVisible();
    void UpdateCharSlot();
    void UpdateEmptySlot();

    void CreateEmptySlot();
    void OnAddSlot();
    void OnMoveChannel();
    void BuyAddSlotItem(GCITEMID itemID);
    void RecvBuyItem();

    void LockEmptySlot(bool bRender_);
    void SetVitalityTooltip();

    void CreateVDepot();
    void UdateSlotAddIcon();
    void UpdateMeshObject(void);

    void SendChangeChange();

    KD3DWnd* m_pkBackGroundWhite;
    int                 m_iExtraCharNum;
    int                 m_iExtraCharNumOld;

private:
    KGCCharIcon* m_pkCharIcon[GC_CHAR_NUM];

    KD3DWnd* m_pkExpGaugeRect;
    KD3DWnd* m_pkExpGauge;
    KD3DWnd* m_pkExpGaugeBar;
    KD3DStatic* m_pkStaticExpValue;
    //troca de char em sala
    PLAYER m_Player;
    GCObject* m_pMeshKOGRoom;
    GCDeviceMotionFRM* m_pShopMotion;
    GCDeviceTexture* m_pTexTitleItem;

    GCDeviceMotionFRM* m_pWaitMotion;
    GCDeviceMotionFRM* m_pActionMotion;
    void ReloadMotion();
    void SetTitleItemID(int iTitleItemID);


    KD3DWnd* m_pkGuildMarkRect;
    GCImageIconTexture* m_pkGuildMarkTex;
    KD3DWnd* m_pkNoGuildMark;

    KD3DWnd* m_pkCharIconRect;

    KD3DStatic* m_pkStaticRecord;
    KD3DStatic* m_pkStaticAutoRecord;
    KD3DStatic* m_pkStaticCharName;
    KD3DStatic* m_pkStaticJobName;
    KD3DStatic* m_pkStaticUserName;
    KD3DStatic* m_pkStaticCharLV;
    KD3DStatic* m_pkStaticGuildExplain;


    KD3DWnd* m_pkBtn_Confirm;
    KD3DWnd* m_pkBtn_Make_char;
    KD3DWnd* m_pkBtnMoveChannel;

    KD3DWnd* m_pkNoChar1;
    KD3DWnd* m_pkNoChar2;
    KD3DWnd* m_pkInfoTitle;
    KD3DWnd* m_pkInfoTitleMatching;

    KD3DWnd* m_pkCharFocus;

    KGCCharIcon* m_pkFocusingIcon;

    KGCCharWnd* m_pkCharWnd;

    bool m_bEmptyNoHaveChar;

    std::vector<KD3DWnd*>   m_vecEmptySlot;

    KGCAccountNickNameFrame* m_pkNickNameFrame;

    KD3DWnd* m_pkNameBack;
    KD3DWnd* m_pkVdepot;

    KD3DWnd* m_pkSloatAddIcon;
};

DEFINE_WND_FACTORY(KGCCharInfoFrame);
DECLARE_WND_FACTORY(KGCCharInfoFrame);
DECLARE_WND_FACTORY_NAME(KGCCharInfoFrame);
#endif