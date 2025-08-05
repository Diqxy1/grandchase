#ifndef _D3DTAB_H_
#define _D3DTAB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"

class KD3DTab : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DTab );
    /// Default constructor
    KD3DTab( void );
    /// Default destructor
    virtual ~KD3DTab( void );
    /// Copy constructor
    KD3DTab( const KD3DTab& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DTab& operator=( const KD3DTab& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnActivate( void );
};

DEFINE_WND_FACTORY( KD3DTab );

#endif // _D3DTAB_H_