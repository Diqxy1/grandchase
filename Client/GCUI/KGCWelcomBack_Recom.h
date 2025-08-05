#ifndef _KGCWELCOMEBACKRECOM_H_
#define _KGCWELCOMEBACKRECOM_H_

//#include "../uifx/D3DWnd.h"

class KGCWelcomeBackRecom :  public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCWelcomeBackRecom );
	/// Default constructor
	KGCWelcomeBackRecom( void );
	/// Default destructor
	virtual ~KGCWelcomeBackRecom( void );
	/// Copy constructor
	KGCWelcomeBackRecom( const KGCWelcomeBackRecom& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCWelcomeBackRecom& operator=( const KGCWelcomeBackRecom& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetSupply( void );
    void SetMode( KD3DWnd::ED3DWndMode eMode);

protected:
    virtual void OnCreate( void );

private:
    KD3DWnd*            m_pkCheck_Mark;
    KD3DWnd*            m_pkBack;
};

DEFINE_WND_FACTORY( KGCWelcomeBackRecom );
DECLARE_WND_FACTORY( KGCWelcomeBackRecom );
DECLARE_WND_FACTORY_NAME( KGCWelcomeBackRecom );

#endif