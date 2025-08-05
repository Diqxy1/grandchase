#pragma once

class KGCDeathMatch;

class KGCDeathMatchOption : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDeathMatchOption );
    /// Default constructor
    KGCDeathMatchOption( void );
    /// Default destructor
    virtual ~KGCDeathMatchOption( void );
    /// Copy constructor
    KGCDeathMatchOption( const KGCDeathMatchOption& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDeathMatchOption& operator=( const KGCDeathMatchOption& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

public:
    void ChangeGoalKillLeft();
    void ChangeGoalKillRight();
    void ChangeLimitTimeLeft();
    void ChangeLimitTimeRight();
    void OnCheckBalance();
    void GetDeathMatchInfo();
    void InitData();
    void SendDeathMatchInfo();
protected:
    KGCChangeBox* m_pkGoalKill;
    KGCChangeBox* m_pkLimitTime;
    KD3DCheckBox* m_pkBalanceChk;
    KD3DStatic* m_pkBalanceStatic;
    KGCDeathMatch* m_pStateDeathMatch;
    bool m_bSendPacket;
};

DEFINE_WND_FACTORY( KGCDeathMatchOption );
DECLARE_WND_FACTORY( KGCDeathMatchOption );
DECLARE_WND_FACTORY_NAME( KGCDeathMatchOption );
