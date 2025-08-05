#pragma once
#include "BaseServer.h"
#include "TRUserManager.h"
#include <queue>
#include "RelayData.h"

class KTRServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KTRServer );

protected:
    KTRServer(void);
public:
    virtual ~KTRServer(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();
    void QueueingRelayData( KRelayDataPtr& spData );

    void SetInitEvent() { ::SetEvent( m_hFirstSyncEvent ); };

    void Tick();

    const int& GetServerProtocol();
    void SetServerProtocol( IN const int& nProtocol );
    void DumpInfo();
    void SetSID( int nSID );
    void SetLimitPacketSendCount( IN int nLimitPacketSendCount );

public:
    KTRUserManager                                  m_kTRUserManager;

protected:
    HANDLE                      m_hFirstSyncEvent;

    mutable KncCriticalSection  m_csServerProtocol;
    int                         m_nServerProtocol;

    mutable KncCriticalSection                      m_csPushQueue;
    mutable KncCriticalSection                      m_csPopQueue;   // tostring을 위해선 보호해야 한다.
    std::queue< KRelayDataPtr >*                    m_pkPushQueue;
    std::queue< KRelayDataPtr >*                    m_pkPopQueue;

    std::queue< KRelayDataPtr >                     m_queue1;
    std::queue< KRelayDataPtr >                     m_queue2;
};

DefSingletonInline( KTRServer );