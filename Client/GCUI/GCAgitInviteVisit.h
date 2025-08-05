#pragma once

class KGCAgitInviteVisit : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum BTN_TYPE { BTN_REGIST = 0, BTN_FRIEND_INVITE = 1, BTN_AGIT_LIST = 2, BTN_RANDOM_AGIT = 3, BTN_CLOSE = 4, NUM_BTN = 5 };
public:
    DECLARE_CLASSNAME( KGCAgitInviteVisit );
    /// Default constructor
    KGCAgitInviteVisit( void );
    /// Default destructor
    virtual ~KGCAgitInviteVisit( void );
    /// Copy constructor
    KGCAgitInviteVisit( const KGCAgitInviteVisit& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitInviteVisit& operator=( const KGCAgitInviteVisit& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DStatic* m_pkStaticInvite;
    KD3DStatic* m_pkStaticVisit;
    KSafeArray< KD3DWnd*, NUM_BTN > m_apkBtn;
    KSafeArray< KD3DStatic*, NUM_BTN - 1 > m_apkBtnStatic;

public:
    void OnRegister( void );
    void OnFriendInvite( void );
    void OnAgitList( void );
    void OnRandomAgit( void );
    void OnClose( void );
};

DEFINE_WND_FACTORY( KGCAgitInviteVisit );
DECLARE_WND_FACTORY( KGCAgitInviteVisit );
DECLARE_WND_FACTORY_NAME( KGCAgitInviteVisit );
