#pragma once

class KGCPartyUserList;

class KGCPartyWindow : public KD3DDialog,        // extends
                       public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCPartyWindow );
    /// Default constructor
    KGCPartyWindow( void );
    /// Default destructor
    virtual ~KGCPartyWindow( void );
    /// Copy constructor
    KGCPartyWindow( const KGCPartyWindow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyWindow& operator=( const KGCPartyWindow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void OnPartyDlg();
    void OffPartyDlg();
public:
    void OnClickDungeon( void );
    void OnClickMatch( void );
    void UpdatePartyDlg();
public:
    KD3DWnd* m_pkDungeonBtn;
    KD3DWnd* m_pkMatchBtn;
    KGCPartyUserList* m_pkPartyUserList;
};

DEFINE_WND_FACTORY( KGCPartyWindow );
DECLARE_WND_FACTORY( KGCPartyWindow );
DECLARE_WND_FACTORY_NAME( KGCPartyWindow );