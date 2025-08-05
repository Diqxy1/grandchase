#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"
#include "MatchEvent.h"
#include "MatchPacket.h"
SmartPointer( KMatchProxy );

class KGSSimLayer;
class KUserEvent;
class KMatchConnector;
class KMatchProxy : public KActorProxy
{
    typedef KGSSimLayer SLayerType;
    typedef KUserEvent IDVenderType;
    NiDeclareRTTI;
    DeclToStringW;

public:
    KMatchProxy(void);
    virtual ~KMatchProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    // derived from KSession
    virtual void OnDestroy();
    virtual KEvent* CreateKEvent() { return new KMatchEvent; }

    bool Connect( IN const int& nSID, IN std::string strIP, IN unsigned short usPort );
    bool IsAfterVerify() { return m_bAfterVerify; };
    bool CheckRecvUser( IN const std::map<DWORD,std::vector<DWORD> >& mapServerUser );

protected:
    HANDLE          m_hVerifyEvent;
    int             m_nSavedResult;
    bool            m_bAfterVerify;

    friend class    KMatchConnector;

protected:
    DECL_ON_FUNC( EVENT_MATCH_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EVENT_MATCH_ADD_USER_ACK );
    DECL_ON_FUNC( EVENT_MATCH_DEL_USER_ACK );
    DECL_ON_FUNC( EVENT_MATCH_FIND_MATCH_NOT );
};
DeclOstmOperatorA( KMatchProxy );