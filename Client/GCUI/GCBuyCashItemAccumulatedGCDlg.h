#pragma once

class KGCBuyCashItemAccumulatedGCDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBuyCashItemAccumulatedGCDlg );
    /// Default constructor
    KGCBuyCashItemAccumulatedGCDlg( void );
    /// Default destructor
    virtual ~KGCBuyCashItemAccumulatedGCDlg( void );
    /// Copy constructor
    KGCBuyCashItemAccumulatedGCDlg( const KGCBuyCashItemAccumulatedGCDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBuyCashItemAccumulatedGCDlg& operator=( const KGCBuyCashItemAccumulatedGCDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;
    KD3DWnd*                    m_pkBtnOk;

    //KD3DStatic*                 m_pkStaticGC1;
    //KD3DStatic*                 m_pkStaticGC2;
    KD3DStatic*                 m_pkStaticGainedDesc;
    KD3DStatic*                 m_pkStaticTotalDesc;
    KD3DStatic*                 m_pkStaticGained;
    KD3DStatic*                 m_pkStaticTotal;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    //virtual void OnCreateComplete( void );
    //virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void SetAccumulateInfo( IN KVirtualCashInfo& m_kVirtualCashInfo_ );


};

DEFINE_WND_FACTORY( KGCBuyCashItemAccumulatedGCDlg );
DECLARE_WND_FACTORY( KGCBuyCashItemAccumulatedGCDlg );
DECLARE_WND_FACTORY_NAME( KGCBuyCashItemAccumulatedGCDlg );
