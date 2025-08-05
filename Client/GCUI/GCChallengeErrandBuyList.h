#pragma once

class KGCChallengeErrandBuyList : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandBuyList );
    /// Default constructor
    KGCChallengeErrandBuyList( void );
    /// Default destructor
    virtual ~KGCChallengeErrandBuyList( void );
    /// Copy constructor
    KGCChallengeErrandBuyList( const KGCChallengeErrandBuyList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandBuyList& operator=( const KGCChallengeErrandBuyList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

    void SetBuyList( void );
    void OnClose( void );
    
public:
    KD3DWnd*    m_pkCloseBtn;
    KD3DStatic* m_pkStaticAdvice;
    KSafeArray<KD3DStatic*,NUM_ERRAND_SHOP>                 m_apStaticBuyNum;

};

DEFINE_WND_FACTORY( KGCChallengeErrandBuyList );
DECLARE_WND_FACTORY( KGCChallengeErrandBuyList );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandBuyList );
