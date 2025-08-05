#ifndef _GCMAINFRAME_H_
#define _GCMAINFRAME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCMainFrame : public KD3DWnd,        // extends
                     public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCMainFrame );
    /// Default constructor
    KGCMainFrame( void );
    /// Default destructor
    virtual ~KGCMainFrame( void );
    /// Copy constructor
    KGCMainFrame( const KGCMainFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMainFrame& operator=( const KGCMainFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );

protected:
    KD3DWnd*    m_pkMinimize;
    KD3DWnd*    m_pkMaximize;
    KD3DWnd*    m_pkClose;
	KD3DWnd*	m_pkAgeLimit;
};

DEFINE_WND_FACTORY( KGCMainFrame );
DECLARE_WND_FACTORY( KGCMainFrame );
DECLARE_WND_FACTORY_NAME( KGCMainFrame );

#endif // _GCMAINFRAME_H_
