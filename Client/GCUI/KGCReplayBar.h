#pragma once

class KGCReplayBar : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayBar );
    /// Default constructor
    KGCReplayBar( void );
    /// Default destructor
    virtual ~KGCReplayBar( void );
    /// Copy constructor
    KGCReplayBar( const KGCReplayBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayBar& operator=( const KGCReplayBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

public:
    void SetBarText( std::wstring strFileName);
    void CursorEnter( );
    void CursorLeave( );
protected:
    KD3DStatic* m_pkFileName;

};

DEFINE_WND_FACTORY( KGCReplayBar );
DECLARE_WND_FACTORY( KGCReplayBar );
DECLARE_WND_FACTORY_NAME( KGCReplayBar );