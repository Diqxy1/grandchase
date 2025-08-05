#pragma once
#include "KGCWorldMapManager.h"

class KGCDungeonTitleS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonTitleS6 );
    /// Default constructor
    KGCDungeonTitleS6( void );
    /// Default destructor
    virtual ~KGCDungeonTitleS6( void );
    /// Copy constructor
    KGCDungeonTitleS6( const KGCDungeonTitleS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonTitleS6& operator=( const KGCDungeonTitleS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetWorldTitle( void );

private:
	KSafeArray<KD3DWnd*, NUM_WORLDMAP> m_arTitleList;
};

DEFINE_WND_FACTORY( KGCDungeonTitleS6 );
DECLARE_WND_FACTORY( KGCDungeonTitleS6 );
DECLARE_WND_FACTORY_NAME( KGCDungeonTitleS6 );
