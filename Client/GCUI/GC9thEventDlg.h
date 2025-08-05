#pragma once

class KGC9thEventDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGC9thEventDlg );
    /// Default constructor
    KGC9thEventDlg( void );
    /// Default destructor
    virtual ~KGC9thEventDlg( void );
    /// Copy constructor
    KGC9thEventDlg( const KGC9thEventDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGC9thEventDlg& operator=( const KGC9thEventDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

private:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnLink;

    KD3DStatic* m_pkStaticCount;
    KD3DStatic* m_pkStaticDesc0;
    KD3DStatic* m_pkStaticDesc1;
    KD3DStatic* m_pkStaticDesc2;
    KD3DStatic* m_pkStaticLink0;
    KD3DStatic* m_pkStaticLink1;

    KGCUINumber*    m_pkCountUI;
    int             m_iTicketCount;

public:
    int  GetTickCount( void ) { return m_iTicketCount; }

private:
    void SetTicketCount( void );
    void SetCountUI( void );
    void OnClickClose( void );
    void OnClickLink( void );
};

DEFINE_WND_FACTORY( KGC9thEventDlg );
DECLARE_WND_FACTORY( KGC9thEventDlg );
DECLARE_WND_FACTORY_NAME( KGC9thEventDlg );
