#pragma once

const DWORD dwCountBy= 10;

class KGCChristmasDonateDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasDonateDlg );
    /// Default constructor
    KGCChristmasDonateDlg( void );
    /// Default destructor
    virtual ~KGCChristmasDonateDlg( void );
    /// Copy constructor
    KGCChristmasDonateDlg( const KGCChristmasDonateDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasDonateDlg& operator=( const KGCChristmasDonateDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnIncrease;
    KD3DWnd*            m_pkBtnDecrease;
    KD3DWnd*            m_pkBtnDonate;
    KD3DWnd*            m_pkBtnClose;


    KD3DStatic*         m_pkStaticDonate;
    KD3DStatic*         m_pkStaticDescription;
    KD3DStatic*         m_pkStaticRibbonCount;

    //data
    DWORD               m_dwSledgeCountToDonate;


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );


    void OnClickDonate();
    void OnClickIncrease();
    void OnClickDecrease();
    void OnClickClose();

    void InitEventInfo();
};

DEFINE_WND_FACTORY( KGCChristmasDonateDlg );
DECLARE_WND_FACTORY( KGCChristmasDonateDlg );
DECLARE_WND_FACTORY_NAME( KGCChristmasDonateDlg );
