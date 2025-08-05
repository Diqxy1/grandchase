#ifndef _KGCWORLDMAPOBJECT_H_
#define _KGCWORLDMAPOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include <vector>

class KGCWorldMapObject : public KD3DWnd
{
//public:
//    DECLARE_CLASSNAME( KGCWorldMapObject );
//    /// Default constructor
//    KGCWorldMapObject( void );
//    /// Default destructor
//    virtual ~KGCWorldMapObject( void );
//    /// Copy constructor
//    KGCWorldMapObject( const KGCWorldMapObject& )
//    { ASSERT( false && "Do not use copy constructor!" ); }
//    /// Assignment operator
//    const KGCWorldMapObject& operator=( const KGCWorldMapObject& )
//    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        WORLDMAP_ICON = 0,
        WORLDMAP_PATH = 1,
    };

    std::vector< KGCWorldMapObject* > m_vecNext;
    int m_iObjectType;
};

//DEFINE_WND_FACTORY( KGCWorldMapObject );
//DECLARE_WND_FACTORY( KGCWorldMapObject );
//DECLARE_WND_FACTORY_NAME( KGCWorldMapObject );

#endif // _KGCWORLDMAPOBJECT_H_