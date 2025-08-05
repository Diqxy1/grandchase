#pragma once

class KGCSetItemImgWnd;

class KGCBuyCashItemChooseCashTypeDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBuyCashItemChooseCashTypeDlg );
    /// Default constructor
    KGCBuyCashItemChooseCashTypeDlg( void );
    /// Default destructor
    virtual ~KGCBuyCashItemChooseCashTypeDlg( void );
    /// Copy constructor
    KGCBuyCashItemChooseCashTypeDlg( const KGCBuyCashItemChooseCashTypeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBuyCashItemChooseCashTypeDlg& operator=( const KGCBuyCashItemChooseCashTypeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;
    KD3DWnd*                    m_pkBtnUseCash;
    KD3DWnd*                    m_pkBtnUseGC;
    KD3DWnd*                    m_pkDot;
    KD3DStatic*                 m_pkStaticItemName;
    KD3DStatic*                 m_pkStaticDesc1;
    KD3DStatic*                 m_pkStaticDesc2;
    KD3DStatic*                 m_pkStaticDesc3;
    KGCItemImgWnd*              m_pkItemImgWnd;
    KGCSetItemImgWnd*           m_pkSetItemImgWnd;
    GCItem                      m_kItem;  
public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void OnClickUseCash();
    void OnClickUseGC();
    void SetGoodsInfo();
};

DEFINE_WND_FACTORY( KGCBuyCashItemChooseCashTypeDlg );
DECLARE_WND_FACTORY( KGCBuyCashItemChooseCashTypeDlg );
DECLARE_WND_FACTORY_NAME( KGCBuyCashItemChooseCashTypeDlg );
