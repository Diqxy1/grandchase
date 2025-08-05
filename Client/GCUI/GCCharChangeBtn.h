#ifndef _GCCHARCHANGEBTN_H_
#define _GCCHARCHANGEBTN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCCharChangeBtn : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCCharChangeBtn );
    /// Default constructor
    KGCCharChangeBtn( void );
    /// Default destructor
    virtual ~KGCCharChangeBtn( void );
    /// Copy constructor
    KGCCharChangeBtn( const KGCCharChangeBtn& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharChangeBtn& operator=( const KGCCharChangeBtn& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    DWORD   m_dwWidthSave;
    DWORD   m_dwHeightSave;
};

DEFINE_WND_FACTORY( KGCCharChangeBtn );
DECLARE_WND_FACTORY( KGCCharChangeBtn );
DECLARE_WND_FACTORY_NAME( KGCCharChangeBtn );

#endif // _GCCHARCHANGEBTN_H_