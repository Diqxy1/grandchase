#pragma once
#include "ActorProxy.h"
#include "MatchEvent.h"
#include <windows.h>
#include "Thread/Thread.h"

SmartPointer( KMatchSvrProxy );

class KMatchSvrProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;

public:
    KMatchSvrProxy(void);
    virtual ~KMatchSvrProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent() { return new KMatchEvent; }
    void RemoveIOCPKey();

    // derived from KSession
    virtual void OnDestroy();
    virtual void OnSocketError();

    void SetID( int iID )   { m_iID = iID; }

protected:
    int             m_iID;

};