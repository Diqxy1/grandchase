#ifndef _KGCBADUSERINFO_H_
#define _KGCBADUSERINFO_H_

//#include "UserPacket.h"
//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class KGCBadUserInfo: public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCBadUserInfo );

    KGCBadUserInfo(void);
    virtual ~KGCBadUserInfo(void);
    /// Copy constructor
    KGCBadUserInfo( const KGCBadUserInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBadUserInfo& operator=( const KGCBadUserInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreateComplete( void );

private:
    KD3DStatic*     m_pkInfoTitle;
    KD3DStatic*     m_pkInfoDesc;

    KD3DWnd*        m_pkBar_Extend;
    KD3DStatic*     m_pkInfoDesc_Extend;

    KD3DWnd*        m_pkBar_Extend_Time;
    KD3DStatic*     m_pkTimeInfo;
    KD3DStatic*     m_pkBack;

public:
    void SetInfoText( KEVENT_BAD_USER_INFO_NOT kRecv );
};

DEFINE_WND_FACTORY( KGCBadUserInfo );
DECLARE_WND_FACTORY( KGCBadUserInfo );
DECLARE_WND_FACTORY_NAME( KGCBadUserInfo );

#endif	//_KGCBADUSERINFO_H_
