#pragma once
#include "BaseServer.h"

class KMatchServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KMatchServer );

protected:
    KMatchServer(void);
public:
    virtual ~KMatchServer(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();

    void SetInitEvent() { ::SetEvent( m_hFirstSyncEvent ); };

    void Tick();

    bool LoadScript();
    const int& GetServerProtocol();
    void SetServerProtocol( IN const int& nProtocol );
    void DumpInfo();
    void SetSID( int nSID );

protected:
    HANDLE                      m_hFirstSyncEvent;

    mutable KncCriticalSection  m_csServerProtocol;
    int                         m_nServerProtocol;
};

DefSingletonInline( KMatchServer );