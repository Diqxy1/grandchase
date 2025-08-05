#ifndef _GCCHANNELLISTBOX_H_
#define _GCCHANNELLISTBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"

class KGCChannelBar;
class KD3DSizingBox;

class KGCChannelListBox: public KD3DDialog,          // extends
                         public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChannelListBox );
    /// Default constructor
    KGCChannelListBox( void );
    /// Default destructor
    virtual ~KGCChannelListBox( void );
    /// Copy constructor
    KGCChannelListBox( const KGCChannelListBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChannelListBox& operator=( const KGCChannelListBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_CHANNEL = 15,
        CHANNEL_BAR_HEIGHT = 24,
        CHANNEL_LIST_BOX_POS_X = 310,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroyComplete();

    KSafeArray<KGCChannelBar*,NUM_CHANNEL> m_apkChannelBar;
    KD3DSizingBox*  m_pkMainFrame;
    KD3DWnd*        m_pkCloseButton;

    D3DXVECTOR2     m_vBasePos;

    void OnClose();

    DWORD m_dwSelectChannelUID;
    std::wstring m_strSelectChannelName;
    void GoChannel();

public:
    void SetChannelListBox();
};

DEFINE_WND_FACTORY( KGCChannelListBox );
DECLARE_WND_FACTORY( KGCChannelListBox );
DECLARE_WND_FACTORY_NAME( KGCChannelListBox );

#endif // _GCCHANNELLISTBOX_H_
