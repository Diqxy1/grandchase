#pragma once
#include "Actor.h"
#include "MRPacket.h"
#include <windows.h>    // ::GetTickCount()
#include "MREvent.h"

SmartPointer(KMRUser);

class KMRUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareException;

public:
    struct MRUserPoolTag{};
    KMRUser(void);
    virtual ~KMRUser(void);

    static KMRUserPtr Create();
    static void AddManager( KMRUserPtr spMRUser );

    // override
    virtual void Tick();
    virtual KEvent* CreateKEvent() { return new KMREvent; }
    
    DWORD GetUserUID()  { return m_dwUserUID; }

protected:
    DWORD   m_dwUserUID;        // 050825. florist. 유저의 고유 ID.

protected:  // util function, Packet Handling
   _DECL_ON_FUNC( EMR_VERIFY_ACCOUNT_REQ, KPacketNameOK );
    DECL_ON_FUNC( EMR_CLIENT_CONNECT_REPORT_REQ );
    DECL_ON_FUNC( EMR_REMOTE_QUEUEING_REQ );
   _DECL_ON_FUNC( EMR_CLIENT_DISCONNECT_REPORT_REQ, DWORD );
    DECL_ON_FUNC( EMR_REMOTE_QUEUEING_PACK_REQ );

    //new
//    _DECL_ON_FUNC( EMR_S2_VERIFY_ACCOUNT_REQ, KPacketNameOK );
//     DECL_ON_FUNC( EMR_S2_CLIENT_CONNECT_REPORT_REQ );
//     DECL_ON_FUNC( EMR_S2_REMOTE_QUEUEING_REQ );
//    _DECL_ON_FUNC( EMR_S2_CLIENT_DISCONNECT_REPORT_REQ, DWORD );
//     DECL_ON_FUNC( EMR_S2_REMOTE_QUEUEING_PACK_REQ );

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 

};