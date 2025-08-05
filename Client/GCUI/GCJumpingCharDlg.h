#pragma once

class KGCJumpingCharDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
    enum
    {
        NUM_STATIC_DESC = 3,
        ICON_GAP = 1,
    };
        
public:
    DECLARE_CLASSNAME( KGCJumpingCharDlg );
    /// Default constructor
    KGCJumpingCharDlg( void );
    /// Default destructor
    virtual ~KGCJumpingCharDlg( void );
    /// Copy constructor
    KGCJumpingCharDlg( const KGCJumpingCharDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCJumpingCharDlg& operator=( const KGCJumpingCharDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnOK();
    void OnClose();
    void OnItem();
    void OnCharSelect( int nIndex );

    void InitCharIconPos();
    void SetButtonLock( bool bLock );

public:

    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnOk;
    KD3DWnd*    m_pkBtnItem;

    KD3DWnd*    m_pkCharFocus;
    KD3DWnd*    m_pkCharIconRect;
    
    KD3DStatic* m_pkStaticDesc[NUM_STATIC_DESC];
    KGCCharIcon* m_pkCharIcon[GC_CHAR_NUM];

    char    m_cSelectCharType;
};

DEFINE_WND_FACTORY( KGCJumpingCharDlg );
DECLARE_WND_FACTORY( KGCJumpingCharDlg );
DECLARE_WND_FACTORY_NAME( KGCJumpingCharDlg );
