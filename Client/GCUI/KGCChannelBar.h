#ifndef _GCCHANNELBAR_H_
#define _GCCHANNELBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"
//#include "KNC/UserPacket.h"

class KGCChannelBar: public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChannelBar );
    /// Default constructor
    KGCChannelBar( void );
    /// Default destructor
    virtual ~KGCChannelBar( void );
    /// Copy constructor
    KGCChannelBar( const KGCChannelBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChannelBar& operator=( const KGCChannelBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_CHANNEL_BAR_WIDTH = 60,
        MIN_PERCENT = 10,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DStatic*     m_pkChannelName;
    KD3DWnd*        m_pkProgressBar;

    KChannelInfo m_kChannelInfo;

    D3DXVECTOR4 m_vTextureCoords;

public:
    void SetChannelInfo( KChannelInfo& kChannelInfo, D3DCOLOR kChannelNameColor );
    DWORD GetChannelUID(){ return m_kChannelInfo.m_dwChannelUID; }
    std::wstring GetChannelName(){ return m_kChannelInfo.m_strChannelName; }
};

DEFINE_WND_FACTORY( KGCChannelBar );
DECLARE_WND_FACTORY( KGCChannelBar );
DECLARE_WND_FACTORY_NAME( KGCChannelBar );

#endif // _GCCHANNELBAR_H_