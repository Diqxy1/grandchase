#ifndef _KGCCONNECTIONMSG_H_
#define _KGCCONNECTIONMSG_H_

//#include "UserPacket.h"
//#include "Packet.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KGCConnectionMsg: public KD3DDialog,
                             public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCConnectionMsg );

    KGCConnectionMsg(void);
    virtual ~KGCConnectionMsg(void);
    /// Copy constructor
    KGCConnectionMsg( const KGCConnectionMsg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCConnectionMsg& operator=( const KGCConnectionMsg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );

private:
    KD3DWnd*        m_pkOk;
    KD3DStatic*     m_pkInfoTitle;
    KD3DStatic*     m_pkInfoDesc;

    KD3DWnd*        m_pkBar_Extend;
    KD3DStatic*     m_pkInfoDesc_Extend;
    KD3DWnd*        m_pkYellowCard;
    KD3DWnd*        m_pkRedCard;

    KD3DStatic*     m_pkBack;

public:
    void SetInfoText( const std::wstring& str );
    void SetInfoText( KEVENT_BAD_USER_INFO_NOT kRecv );
};

DEFINE_WND_FACTORY( KGCConnectionMsg );
DECLARE_WND_FACTORY( KGCConnectionMsg );
DECLARE_WND_FACTORY_NAME( KGCConnectionMsg );

#endif	//_KGCCONNECTIONMSG_H_
