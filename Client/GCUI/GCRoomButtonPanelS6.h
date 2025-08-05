#pragma once

class KGCRoomButtonPanelS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomButtonPanelS6 );
    /// Default constructor
    KGCRoomButtonPanelS6( void );
    /// Default destructor
    virtual ~KGCRoomButtonPanelS6( void );
    /// Copy constructor
    KGCRoomButtonPanelS6( const KGCRoomButtonPanelS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomButtonPanelS6& operator=( const KGCRoomButtonPanelS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void OnInitialize( void );

public:
    bool IsBlockedCharActive(void);
    bool IsBlockedRankActive(void);
    void OnInvite();
    void OnReady();
    void OnStart();
    

public:
    void UpdateGameData();

public:
    void CheckGameStart();

    void SetStartDelay( int iDelay = 0 );
    bool IsAutoStart(){ return m_bAutoStart; }
    void SetAutoStart( bool bAuto ){ m_bAutoStart = bAuto; }
    int GetStartDelay(){ return m_iStartDelay; }

    void GameStart();
    bool IsEnableCurrentGameMode();
    bool CheckAllReady();
    bool CheckPlayerNum();
    bool CheckTeam();
    bool CheckQuest();
    void SetAutoTimer();
    void GetTeamCount( int& iBlue, int& iRed );
    void DecreaseStartDelay();
    bool GetExitBtnState();
    void SetExitBtnState( bool bExitState );

public:
    int     m_iStartDelay;
    bool	m_bAutoStart;
    bool    m_bExitRoom;
public:
    KD3DWnd*        m_pkRoomReadyButton;
    KD3DWnd*        m_pkRoomInviteButton;
    KD3DWnd*        m_pkRoomStartButton;
    KD3DWnd*        m_pkRoomExitButton;
};

DEFINE_WND_FACTORY( KGCRoomButtonPanelS6 );
DECLARE_WND_FACTORY( KGCRoomButtonPanelS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomButtonPanelS6 );
