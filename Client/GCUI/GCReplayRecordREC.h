#pragma once

class KGCReplayRecordREC : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayRecordREC );
    /// Default constructor
    KGCReplayRecordREC( void );
    /// Default destructor
    virtual ~KGCReplayRecordREC( void );
    /// Copy constructor
    KGCReplayRecordREC( const KGCReplayRecordREC& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayRecordREC& operator=( const KGCReplayRecordREC& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    enum { OFF_TIME = 500, ON_TIME = 1000 };
    KD3DWnd*    m_pkRec;
    DWORD       m_dwTimer;
    bool        m_bShow;

    void RECPlay( void );
    void RECStop( void );
    

};

DEFINE_WND_FACTORY( KGCReplayRecordREC );
DECLARE_WND_FACTORY( KGCReplayRecordREC );
DECLARE_WND_FACTORY_NAME( KGCReplayRecordREC );
