#pragma once
#include "Actor.h"
#include "MatchPacket.h"
#include <windows.h>    // ::GetTickCount()
#include "MatchEvent.h"

SmartPointer(KMatchUser);

class KMatchUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareException;

public:
    struct MatchUserPoolTag{};
    KMatchUser(void);
    virtual ~KMatchUser(void);

    static KMatchUserPtr Create();
    static void AddManager( KMatchUserPtr spMatchUser );

    KMatchUserPtr GetThisPtr() { return boost::static_pointer_cast<KMatchUser>( (*this).shared_from_this() );}

    // override
    virtual void Tick();
    virtual KEvent* CreateKEvent() { return new KMatchEvent; }

protected:
    //override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 

protected:  // Packet Handling
   _DECL_ON_FUNC( EVENT_MATCH_VERIFY_ACCOUNT_REQ, KSimpleServerInfo );
    DECL_ON_FUNC( EVENT_MATCH_ADD_USER_REQ );
   _DECL_ON_FUNC( EVENT_MATCH_DEL_USER_REQ, KPartyData );

protected:
    KSimpleServerInfo   m_kServerInfo;
};