#pragma once

class KGCScenMenuS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCScenMenuS6 );
    /// Default constructor
    KGCScenMenuS6( void );
    /// Default destructor
    virtual ~KGCScenMenuS6( void );
    /// Copy constructor
    KGCScenMenuS6( const KGCScenMenuS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCScenMenuS6& operator=( const KGCScenMenuS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
};

DEFINE_WND_FACTORY( KGCScenMenuS6 );
DECLARE_WND_FACTORY( KGCScenMenuS6 );
DECLARE_WND_FACTORY_NAME( KGCScenMenuS6 );
