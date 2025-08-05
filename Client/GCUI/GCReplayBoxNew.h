#pragma once

class KGCReplayBoxNew : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayBoxNew );
    /// Default constructor
    KGCReplayBoxNew( void );
    /// Default destructor
    virtual ~KGCReplayBoxNew( void );
    /// Copy constructor
    KGCReplayBoxNew( const KGCReplayBoxNew& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayBoxNew& operator=( const KGCReplayBoxNew& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    
    KD3DWnd*    m_pkOkBtn;
    KD3DWnd*    m_pkOldReplayBtn;
    KD3DStatic* m_pkStaticDesc;

    void OnOldReplay( void );
    void OnClose( void );

};

DEFINE_WND_FACTORY( KGCReplayBoxNew );
DECLARE_WND_FACTORY( KGCReplayBoxNew );
DECLARE_WND_FACTORY_NAME( KGCReplayBoxNew );
