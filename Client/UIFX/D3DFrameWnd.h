#ifndef _D3DFRAMEWND_H_
#define _D3DFRAMEWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"

class KD3DFrameWnd : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DFrameWnd );
    /// Default constructor
    KD3DFrameWnd( void ) : m_vMousePosSave( 0.0f, 0.0f ) { m_bDrag = false; }
    /// Default destructor
    virtual ~KD3DFrameWnd( void ) {}
    /// Copy constructor
    KD3DFrameWnd( const KD3DFrameWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DFrameWnd& operator=( const KD3DFrameWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );

private:
    D3DXVECTOR2 m_vInitPos;
    D3DXVECTOR2 m_vMousePosSave;
    bool        m_bDrag;
};

DEFINE_WND_FACTORY( KD3DFrameWnd );

#endif // _D3DFRAMEWND_H_