#ifndef _GCROOMBUTTON_H_
#define _GCROOMBUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCRoomButton : public KD3DMoveWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCRoomButton );
    /// Default constructor
    KGCRoomButton( void );
    /// Default destructor
    virtual ~KGCRoomButton( void );
    /// Copy constructor
    KGCRoomButton( const KGCRoomButton& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomButton& operator=( const KGCRoomButton& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    DWORD   m_dwWidthSave;
    DWORD   m_dwHeightSave;
};

DEFINE_WND_FACTORY( KGCRoomButton );
DECLARE_WND_FACTORY( KGCRoomButton );
DECLARE_WND_FACTORY_NAME( KGCRoomButton );

#endif // _GCROOMBUTTON_H_