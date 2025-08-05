#pragma once

class KGCEscMenuDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEscMenuDlg );
    /// Default constructor
    KGCEscMenuDlg( void );
    /// Default destructor
    virtual ~KGCEscMenuDlg( void );
    /// Copy constructor
    KGCEscMenuDlg( const KGCEscMenuDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEscMenuDlg& operator=( const KGCEscMenuDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    KD3DWnd*    m_pkBtnChangeChar;
    KD3DWnd*    m_pkBtnChangeServer;
    KD3DWnd*    m_pkBtnChangeOption;
    KD3DWnd*    m_pkBtnExitGame;
    KD3DWnd*    m_pkBtnClose;

public:
    void OnChangeChar();
    void OnChangeServer();
    void OnChangeOption();
    void OnExitGame();
    void OnClose();
};

DEFINE_WND_FACTORY( KGCEscMenuDlg );
DECLARE_WND_FACTORY( KGCEscMenuDlg );
DECLARE_WND_FACTORY_NAME( KGCEscMenuDlg );
