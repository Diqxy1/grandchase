#pragma once

class KGCChallengeErrandHelp : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandHelp );
    /// Default constructor
    KGCChallengeErrandHelp( void );
    /// Default destructor
    virtual ~KGCChallengeErrandHelp( void );
    /// Copy constructor
    KGCChallengeErrandHelp( const KGCChallengeErrandHelp& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandHelp& operator=( const KGCChallengeErrandHelp& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum{
        NUM_STATIC_HELP = 3,
        NUM_REWARD_VIEW_ITEM = 7
    };
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete();
    virtual void FrameMoveInEnabledState();
    virtual void PostChildDraw();


    void OnClose();
    void SetRewardItemImg();

private:
    KD3DWnd*	m_pkBtnClose;

    KD3DStatic* m_pkStaticHelp[NUM_STATIC_HELP];

    KD3DStatic* m_pkStaticItemGuide;

    KD3DWnd*			m_pkItemImg[NUM_REWARD_VIEW_ITEM];
    GCITEMID			m_GCitemID[NUM_REWARD_VIEW_ITEM];
    GCDeviceTexture*	m_pTexItem[NUM_REWARD_VIEW_ITEM];

};

DEFINE_WND_FACTORY( KGCChallengeErrandHelp );
DECLARE_WND_FACTORY( KGCChallengeErrandHelp );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandHelp );
