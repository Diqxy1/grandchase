#ifndef _KGCWORLDMAPPATH_H_
#define _KGCWORLDMAPPATH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCWorldMapObject.h"
//#include "../uifx/ActionListener.h"
//#include <vector>

class KGCWorldMapPath : public KGCWorldMapObject
{
public:
    DECLARE_CLASSNAME( KGCWorldMapPath );
    /// Default constructor
    KGCWorldMapPath( void );
    /// Default destructor
    virtual ~KGCWorldMapPath( void );
    /// Copy constructor
    KGCWorldMapPath( const KGCWorldMapPath& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapPath& operator=( const KGCWorldMapPath& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate();

public:
    void RenderPath( bool bpath );
private:
    KD3DWnd* m_pkPath; 
};

DEFINE_WND_FACTORY( KGCWorldMapPath );
DECLARE_WND_FACTORY( KGCWorldMapPath );
DECLARE_WND_FACTORY_NAME( KGCWorldMapPath );

#endif // _KGCWORLDMAPPATH_H_