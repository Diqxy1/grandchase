#ifndef _GCCHANGEBOX_H_
#define _GCCHANGEBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCChangeBox : public KD3DWnd,        // extends
                     public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCChangeBox );
    /// Default constructor
    KGCChangeBox( void );
    /// Default destructor
    virtual ~KGCChangeBox( void );
    /// Copy constructor
    KGCChangeBox( const KGCChangeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChangeBox& operator=( const KGCChangeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        LEFT_BUTTON,
        RIGHT_BUTTON,
    };

public:
    void SetText( const std::wstring& strText );
    void EnableChangeBox( bool bEnable );

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );

protected:
    KD3DWnd*    m_pkLeft;
    KD3DWnd*    m_pkRight;
    KD3DStatic* m_pkStatic;
};

DEFINE_WND_FACTORY( KGCChangeBox );
DECLARE_WND_FACTORY( KGCChangeBox );
DECLARE_WND_FACTORY_NAME( KGCChangeBox );

#endif // _GCCHANGEBOX_H_