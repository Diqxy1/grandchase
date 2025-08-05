#pragma once

class KGCSetItemImgWnd;

class KGCBuyCashItemBuyCompleteDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBuyCashItemBuyCompleteDlg );
    /// Default constructor
    KGCBuyCashItemBuyCompleteDlg( void );
    /// Default destructor
    virtual ~KGCBuyCashItemBuyCompleteDlg( void );
    /// Copy constructor
    KGCBuyCashItemBuyCompleteDlg( const KGCBuyCashItemBuyCompleteDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBuyCashItemBuyCompleteDlg& operator=( const KGCBuyCashItemBuyCompleteDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;
    KD3DWnd*                    m_pkBtnOk;

    KD3DStatic*                 m_pkStaticItemName;
    //KD3DStatic*                 m_pkStaticGC1;
    //KD3DStatic*                 m_pkStaticGC2;
    KD3DStatic*                 m_pkStaticUsedDesc;
    KD3DStatic*                 m_pkStaticCurrentDesc;
    KD3DStatic*                 m_pkStaticUsed;
    KD3DStatic*                 m_pkStaticCurrent;

    KGCItemImgWnd*              m_pkItemImgWnd;
    KGCSetItemImgWnd*           m_pkSetItemImgWnd;

    GCItem                      m_kItem;


public:
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    //virtual void OnCreateComplete( void );
    //virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void OnClickOk();
    void SetBuyResultInfo( IN DWORD nUsedGC );

};

DEFINE_WND_FACTORY( KGCBuyCashItemBuyCompleteDlg );
DECLARE_WND_FACTORY( KGCBuyCashItemBuyCompleteDlg );
DECLARE_WND_FACTORY_NAME( KGCBuyCashItemBuyCompleteDlg );
