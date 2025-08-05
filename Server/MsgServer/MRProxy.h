#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"
#include "MREvent.h"
#include "MRPacket.h"
#include "CenterPacket.h"

SmartPointer( KMRProxy );

class KGSSimLayer;
class KUserEvent;
class KMRProxy : public KActorProxy
{
    typedef KGSSimLayer SLayerType;
    typedef KUserEvent IDVenderType;
    NiDeclareRTTI;
    DeclToStringW;

public:
    KMRProxy(void);
    virtual ~KMRProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent() { return new KMREvent; }
    void RemoveIOCPKey();

    // derived from KSession
    virtual void OnDestroy();
    virtual void OnSocketError();

    void SetID( int iID )   { m_iID = iID; }

    bool RemoteQueueing( DWORD dwSenderUID, 
        DWORD dwRecverUID, 
        unsigned short usEventID,
        KSerBuffer& kBuf,
        bool bCheckRecv,
        bool bDirectSend );

    bool RemoteQueueingPack( DWORD dwSenderUID,
        std::vector<DWORD> vecRecverUIDList,
        unsigned short usEventID,
        KSerBuffer& kBuf,
        bool bCheckRecv,
        bool bDirectSend );

protected:

   _DECL_ON_FUNC( EMR_VERIFY_ACCOUNT_ACK, int );
    DECL_ON_FUNC( EMR_CLIENT_CONNECT_REPORT_ACK );
    DECL_ON_FUNC( EMR_LOGINNED_FRIEND_INFO_REQ );
    DECL_ON_FUNC( EMR_REMOTE_QUEUEING_ACK );
   _DECL_ON_FUNC( EMR_CLIENT_DISCONNECT_REPORT_ACK, KPacketNameOK );
    DECL_ON_FUNC( EMR_KICK_CLIENT_NOT );
    DECL_ON_FUNC( EMR_REMOTE_QUEUEING_PACK_ACK );

    int             m_iID;

};