#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"

class KD3DStatic;
class KGCMinigameCategory : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCMinigameCategory );
	/// Default constructor
	KGCMinigameCategory( void );
	/// Default destructor
	virtual ~KGCMinigameCategory( void );
	/// Copy constructor
	KGCMinigameCategory( const KGCMinigameCategory& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMinigameCategory& operator=( const KGCMinigameCategory& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetGameDesc( const int GameCategory );

protected:
    virtual void OnCreate( void );

private:
    KD3DWnd*        m_pkGameTitle;
    KD3DStatic*     m_pkGameDesc;
};

DEFINE_WND_FACTORY( KGCMinigameCategory );
DECLARE_WND_FACTORY( KGCMinigameCategory );
DECLARE_WND_FACTORY_NAME( KGCMinigameCategory );
