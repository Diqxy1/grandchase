#ifndef _GCROOMHOVER_H_
#define _GCROOMHOVER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCRoomHover : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCRoomHover );
    /// Default constructor
    KGCRoomHover( void );
    /// Default destructor
    virtual ~KGCRoomHover( void );
    /// Copy constructor
    KGCRoomHover( const KGCRoomHover& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomHover& operator=( const KGCRoomHover& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void PreDraw( void );
    virtual void PostDraw( void );
};

DEFINE_WND_FACTORY( KGCRoomHover );
DECLARE_WND_FACTORY( KGCRoomHover );
DECLARE_WND_FACTORY_NAME( KGCRoomHover );

#endif // _GCROOMHOVER_H_