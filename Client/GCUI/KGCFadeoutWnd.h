#ifndef _GCFADEOUTWND_H_
#define _GCFADEOUTWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCFadeoutWnd: public KD3DDialog,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCFadeoutWnd );
    /// Default constructor
    KGCFadeoutWnd( void );
    /// Default destructor
    virtual ~KGCFadeoutWnd( void );
    /// Copy constructor
    KGCFadeoutWnd( const KGCFadeoutWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFadeoutWnd& operator=( const KGCFadeoutWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroyComplete( void );
};

DEFINE_WND_FACTORY( KGCFadeoutWnd );
DECLARE_WND_FACTORY( KGCFadeoutWnd );
DECLARE_WND_FACTORY_NAME( KGCFadeoutWnd );

#endif // _GCFADEOUTWND_H_