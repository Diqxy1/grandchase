#pragma once

const int PARTY_RANK_BAR_GAP = 15;

class KGCRankPlayerInfoBar;

class KGCPartyRankInfo : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPartyRankInfo );
    /// Default constructor
    KGCPartyRankInfo( void );
    /// Default destructor
    virtual ~KGCPartyRankInfo( void );
    /// Copy constructor
    KGCPartyRankInfo( const KGCPartyRankInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyRankInfo& operator=( const KGCPartyRankInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    void SetPlayerRankInfo();
protected:
    KSafeArray<KGCRankPlayerInfoBar*,MAX_PLAYER_NUM> m_pPlayerInfoBar;
    KGCRankPlayerInfoBar* m_pPlayerInfoBarWnd;
    
};

DEFINE_WND_FACTORY( KGCPartyRankInfo );
DECLARE_WND_FACTORY( KGCPartyRankInfo );
DECLARE_WND_FACTORY_NAME( KGCPartyRankInfo );
