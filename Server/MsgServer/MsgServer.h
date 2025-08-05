#pragma once
#include "BaseServer.h"

class KMsgServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KMsgServer );

protected:
    KMsgServer(void);
public:
    virtual ~KMsgServer(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();

    void Tick();

    //////////////////////////////////////////////////////////////////////////
    DWORD GetUID()              { return m_dwUID; }
    void SetUID( DWORD dwUID )  { m_dwUID = dwUID; }

public:
    bool    m_bWriteLogoutLog;

protected:
    DWORD   m_dwUID;
};

DefSingletonInline( KMsgServer );