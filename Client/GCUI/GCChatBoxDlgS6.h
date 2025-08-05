#pragma once

class KGCChatBoxDlgS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChatBoxDlgS6 );
    /// Default constructor
    KGCChatBoxDlgS6( void );
    /// Default destructor
    virtual ~KGCChatBoxDlgS6( void );
    /// Copy constructor
    KGCChatBoxDlgS6( const KGCChatBoxDlgS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChatBoxDlgS6& operator=( const KGCChatBoxDlgS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	
	void OnClickChangeSize( const KActionEvent& event );

	void RenderOn();
	void RenderOff();
};

DEFINE_WND_FACTORY( KGCChatBoxDlgS6 );
DECLARE_WND_FACTORY( KGCChatBoxDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCChatBoxDlgS6 );
