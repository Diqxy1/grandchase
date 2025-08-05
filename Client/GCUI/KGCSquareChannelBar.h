#ifndef _GCSQUARECHANNELBAR_H_
#define _GCSQUARECHANNELBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"
//#include <UserPacket.h>

class KGCSquareChannelBar: public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSquareChannelBar );
    /// Default constructor
    KGCSquareChannelBar( void );
    /// Default destructor
    virtual ~KGCSquareChannelBar( void );
    /// Copy constructor
    KGCSquareChannelBar( const KGCSquareChannelBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSquareChannelBar& operator=( const KGCSquareChannelBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_SERVER_BAR_WIDTH = 60,
        MIN_PERCENT = 6,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );

    void SetSquareChannelInfo( const KSquareInfo& kInfo, int iSelectIndex_ );
    void SetSquareID( DWORD dwID ) { m_dwSquareID = dwID; }
    void SetChannelName( const std::wstring& strChannelName );
    void SetGage( DWORD dwCurrent, DWORD dwMaximum );
    DWORD GetSquareID() const { return m_dwSquareID; }
    void OnClickChannel();

protected:
    virtual void OnCreate( void );

    DWORD           m_dwSquareID;
    KD3DStatic*     m_pkChannelName;
    KD3DWnd*        m_pkGage;
    D3DXVECTOR4     m_vTextureCoords;
    KSquareInfo     m_kSquareInfo;
	int				m_iSelectIndex;
};

DEFINE_WND_FACTORY( KGCSquareChannelBar );
DECLARE_WND_FACTORY( KGCSquareChannelBar );
DECLARE_WND_FACTORY_NAME( KGCSquareChannelBar );

#endif // _GCSQUARECHANNELBAR_H_