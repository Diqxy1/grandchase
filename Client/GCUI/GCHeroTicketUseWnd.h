#pragma once

class KGCHeroTicketUseWnd : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroTicketUseWnd );
    /// Default constructor
    KGCHeroTicketUseWnd( void );
    /// Default destructor
    virtual ~KGCHeroTicketUseWnd( void );
    /// Copy constructor
    KGCHeroTicketUseWnd( const KGCHeroTicketUseWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroTicketUseWnd& operator=( const KGCHeroTicketUseWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnExitWindow( void );

private:
    KGCHeroTicketSystem*    m_pkTicketPanel;
};

DEFINE_WND_FACTORY( KGCHeroTicketUseWnd );
DECLARE_WND_FACTORY( KGCHeroTicketUseWnd );
DECLARE_WND_FACTORY_NAME( KGCHeroTicketUseWnd );
