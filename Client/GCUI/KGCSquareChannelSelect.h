#ifndef _GCSQUARECHANNELSELECT_H_
#define _GCSQUARECHANNELSELECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// [1/15/2008] breadceo. 나중에 페이지 들어가면 좀 하는일이 많아질듯 -_-

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"

class KGCSquareChannelBar;

#define MAX_SQUARE_CHANNEL_NUM 8

class KGCSquareChannelSelect: public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSquareChannelSelect );
    /// Default constructor
    KGCSquareChannelSelect( void );
    /// Default destructor
    virtual ~KGCSquareChannelSelect( void );
    /// Copy constructor
    KGCSquareChannelSelect( const KGCSquareChannelSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSquareChannelSelect& operator=( const KGCSquareChannelSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState();
    virtual void ActionPerformed( const KActionEvent& event );
    void UpdateData();
    void SetError( int iStrID );

protected:
    virtual void OnCreate( void );

    KSafeArray<KGCSquareChannelBar*,MAX_SQUARE_CHANNEL_NUM> m_pkChannelBar;
    KD3DWnd*                m_pkErrString;
    KD3DStatic*             m_pkErrStatic;
    DWORD                   m_dwErrShow;

	int						m_iChannelNum;
};

DEFINE_WND_FACTORY( KGCSquareChannelSelect );
DECLARE_WND_FACTORY( KGCSquareChannelSelect );
DECLARE_WND_FACTORY_NAME( KGCSquareChannelSelect );

#endif // _GCSQUARECHANNELSELECT_H_