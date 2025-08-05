#pragma once
#include "ActorProxy.h"
#include "NUserEvent.h"
//#include <windows.h>
#include "NUserPacket.h"

//#include <queue>

SmartPointer( KNUserProxy );

class KNUserProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;

public:
    KNUserProxy(void);
    virtual ~KNUserProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent();
        
    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.

    //virtual bool Init( bool bUseIOCP );
    //virtual bool Connect( const TCHAR* szIP, unsigned short usPort );

protected:  // Recv Msg Handler & Util Function
    DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK );
   _DECL_ON_FUNC( ENU_WAIT_TIME_NOT, DWORD );

};
