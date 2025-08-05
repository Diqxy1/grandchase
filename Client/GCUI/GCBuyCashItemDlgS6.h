#pragma once

class KGCBuyCashItemDlgS6 : public KD3DDialog    // extends
    , public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCBuyCashItemDlgS6);
    /// Default constructor
    KGCBuyCashItemDlgS6(void);
    /// Default destructor
    virtual ~KGCBuyCashItemDlgS6(void);
    /// Copy constructor
    KGCBuyCashItemDlgS6(const KGCBuyCashItemDlgS6&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCBuyCashItemDlgS6& operator=(const KGCBuyCashItemDlgS6&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }


private:
    KD3DWnd* m_pkBtnClose;
    KD3DWnd* m_pkBtnCancel;
    KD3DWnd* m_pkBtnBuy;

    KD3DStatic* m_pkStaticItemName;
    KD3DStatic* m_pkStaticItemDesc;

    //KD3DStatic*                 m_pkStaticGC1;
    //KD3DStatic*                 m_pkStaticGC2;
    //KD3DStatic*                 m_pkStaticGC3;

    KD3DStatic* m_pkStaticNecessaryDesc;
    KD3DStatic* m_pkStaticCurrentDesc;
    KD3DStatic* m_pkStaticNotEnoughDesc;

    KD3DStatic* m_pkStaticNecessary;
    KD3DStatic* m_pkStaticCurrent;
    KD3DStatic* m_pkStaticNotEnough;

    KGCItemImgWnd* m_pkItemImgWnd;
    KGCSetItemImgWnd* m_pkSetItemImgWnd;

    GCItem                      m_kItem;



public:
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void OnCreate(void);
    virtual void OnCreateComplete(void);
    virtual void FrameMoveInEnabledState(void);

    void OnClickClose();
    void OnClickBuy();
    void SetGoodsInfo();
    void BuyCurrentItem();

};

DEFINE_WND_FACTORY(KGCBuyCashItemDlgS6);
DECLARE_WND_FACTORY(KGCBuyCashItemDlgS6);
DECLARE_WND_FACTORY_NAME(KGCBuyCashItemDlgS6);
