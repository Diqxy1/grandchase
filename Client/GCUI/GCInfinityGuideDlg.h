#pragma once

class KGCInfinityGuideDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCInfinityGuideDlg );
    /// Default constructor
    KGCInfinityGuideDlg( void );
    /// Default destructor
    virtual ~KGCInfinityGuideDlg( void );
    /// Copy constructor
    KGCInfinityGuideDlg( const KGCInfinityGuideDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInfinityGuideDlg& operator=( const KGCInfinityGuideDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnClose();
    void OnQuickJoin();

public:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnQuickJoin;
};

DEFINE_WND_FACTORY( KGCInfinityGuideDlg );
DECLARE_WND_FACTORY( KGCInfinityGuideDlg );
DECLARE_WND_FACTORY_NAME( KGCInfinityGuideDlg );
