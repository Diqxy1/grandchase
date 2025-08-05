#pragma once

class KGCEnchantS6Dlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantS6Dlg );
    /// Default constructor
    KGCEnchantS6Dlg( void );
    /// Default destructor
    virtual ~KGCEnchantS6Dlg( void );
    /// Copy constructor
    KGCEnchantS6Dlg( const KGCEnchantS6Dlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantS6Dlg& operator=( const KGCEnchantS6Dlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnClose();
    void OnEnchant();
    void OnBreakup();
    void OnAlchemy();

public:
    KD3DWnd*        m_pkBtnClose;
    KD3DWnd*        m_pkBtnEnchant;
    KD3DWnd*        m_pkBtnBreakup;
    KD3DWnd*        m_pkBtnAlchemy;
};

DEFINE_WND_FACTORY( KGCEnchantS6Dlg );
DECLARE_WND_FACTORY( KGCEnchantS6Dlg );
DECLARE_WND_FACTORY_NAME( KGCEnchantS6Dlg );
