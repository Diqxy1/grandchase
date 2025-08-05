#ifndef _KGCWELCOMEBACKREWARD_H_
#define _KGCWELCOMEBACKREWARD_H_

//#include "../uifx/D3DWnd.h"

class KD3DSizingBox;

class KGCWelcomeBackReward :    public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCWelcomeBackReward );
	/// Default constructor
	KGCWelcomeBackReward( void );
	/// Default destructor
	virtual ~KGCWelcomeBackReward( void );
	/// Copy constructor
	KGCWelcomeBackReward( const KGCWelcomeBackReward& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCWelcomeBackReward& operator=( const KGCWelcomeBackReward& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetLock( );

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DWnd*            m_pkDesc;
    KD3DSizingBox*      m_pkBack;
    KD3DWnd*            m_pkCheckMark;
};

DEFINE_WND_FACTORY( KGCWelcomeBackReward );
DECLARE_WND_FACTORY( KGCWelcomeBackReward );
DECLARE_WND_FACTORY_NAME( KGCWelcomeBackReward );

#endif