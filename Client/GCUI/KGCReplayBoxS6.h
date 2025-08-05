#pragma once

const int REPLAY_BAR_NUM  = 8;
const int REPLAY_BAR_GAP = 0;

class KGCReplayBar;
class KGCReplayBoxS6 : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayBoxS6 );
    /// Default constructor
    KGCReplayBoxS6( void );
    /// Default destructor
    virtual ~KGCReplayBoxS6( void );
    /// Copy constructor
    KGCReplayBoxS6( const KGCReplayBoxS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayBoxS6& operator=( const KGCReplayBoxS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnCreateComplete();

public:
    void RequestList( void );
    void UpdateReplayList( void );
    void OnClose(void);
    void OnClickReplayBar(void);

protected:
    KD3DScrollbar*      m_pkScrollbar;
    KGCReplayBar*       m_pkReplayBar;
    KD3DWnd*            m_pkExit;
    int                 m_iTopIndex;
    KSafeArray<KGCReplayBar*,REPLAY_BAR_NUM> m_pReplayBar;

};

DEFINE_WND_FACTORY( KGCReplayBoxS6 );
DECLARE_WND_FACTORY( KGCReplayBoxS6 );
DECLARE_WND_FACTORY_NAME( KGCReplayBoxS6 );