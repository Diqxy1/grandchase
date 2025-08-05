#pragma once

class KGCChallengeErrandPanel;
class KGCChallengeErrandHelp;

class KGCChallengeErrandDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandDlg );
    /// Default constructor
    KGCChallengeErrandDlg( void );
    /// Default destructor
    virtual ~KGCChallengeErrandDlg( void );
    /// Copy constructor
    KGCChallengeErrandDlg( const KGCChallengeErrandDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandDlg& operator=( const KGCChallengeErrandDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    void OnClose();
    void IsClose();
    void OnStart();
    void OnReward();
    void OnHelp();
    void HelpRewardList();
    void GamExitRewardPacket();
private:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnStart;
    KD3DWnd*    m_pkBtnReward;
    KD3DWnd*    m_pkBtnHelp;



    KGCChallengeErrandHelp*                  m_pkErrandHelpDlg;
    KGCChallengeErrandPanel*                 m_pkErrandPanel;
};

DEFINE_WND_FACTORY( KGCChallengeErrandDlg );
DECLARE_WND_FACTORY( KGCChallengeErrandDlg );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandDlg );
