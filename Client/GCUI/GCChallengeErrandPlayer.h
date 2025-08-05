#pragma once

const float ERRAND_PLAYER_SPEED = 11.0f;

class KGCChallengeErrandPlayer : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandPlayer );
    /// Default constructor
    KGCChallengeErrandPlayer( void );
    /// Default destructor
    virtual ~KGCChallengeErrandPlayer( void );
    /// Copy constructor
    KGCChallengeErrandPlayer( const KGCChallengeErrandPlayer& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandPlayer& operator=( const KGCChallengeErrandPlayer& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    void SetCurrentLocation( int iLocation )            { m_iCurrentLocation = iLocation; }
    int  GetCurrentLocation( void )                     { return m_iCurrentLocation; }
    float GetPlayerMoveSpeed( void )                    { return ERRAND_PLAYER_SPEED; }
    std::vector<ErrandItemBuyList> GetPlayerBuyInfo()   { return m_vecPlayerBuyList; }

    void AddSuccesCount( void ) { m_iSuccessCount++; }
    int GetSuccesCount( void )  { return m_iSuccessCount; }

    void SetPlayerEmotion(bool bRender, ED3DWndMode eMode = D3DWM_DEFAULT );
    void PlayerDirection( bool bRight );
    void SetAnimDefalult( void );
    void MoveAnim( void );

    int AddBuyList( ErrandItemBuyList& eBuyList );
    ErrandItemBuyList LostItem( void );

    void ResetEndGame( void );
    void ResetPlayer( void );

    void SetShowSuccesParticle( );


private:
    KD3DWnd*    m_pkPlayerLeft;
    KD3DWnd*    m_pkPlayerRight;
    KD3DWnd*    m_pkPlayer;

    KD3DWnd*    m_pkPlayerEmotion;

    std::vector<ErrandItemBuyList> m_vecPlayerBuyList;
    int         m_iSuccessCount;

    int         m_iCurrentLocation;
    int         m_iAniFrame;
    bool        m_bRight;

};

DEFINE_WND_FACTORY( KGCChallengeErrandPlayer );
DECLARE_WND_FACTORY( KGCChallengeErrandPlayer );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandPlayer );
