#ifndef _KGCNumBERCONTROL_H_
#define _KGCNumBERCONTROL_H_

#include <stdafx.h>

//#include "../uifx/D3DWnd.h"

class KGCNumberControl: public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCNumberControl );

    KGCNumberControl(void);
    virtual ~KGCNumberControl(void);
    /// Copy constructor
    KGCNumberControl( const KGCNumberControl& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNumberControl& operator=( const KGCNumberControl& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );	

    KSafeArray<KD3DWnd*,10> m_apkNumber;
    
    void AllRenderOff();

public:
    void SetNumber( int iNumber );
};

DEFINE_WND_FACTORY( KGCNumberControl );
DECLARE_WND_FACTORY( KGCNumberControl );
DECLARE_WND_FACTORY_NAME( KGCNumberControl );

#endif	//_KGCNumBERCONTROL_H_