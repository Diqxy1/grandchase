#pragma once
#include "BaseServer.h"

class KMsgRouter : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KMsgRouter );

protected:
    KMsgRouter(void);
public:
    virtual ~KMsgRouter(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();

    void Tick();

    //////////////////////////////////////////////////////////////////////////
    DWORD GetUID()              { return m_dwUID; }
    void SetUID( DWORD dwUID )  { m_dwUID = dwUID; }

protected:
    DWORD   m_dwUID;
};

DefSingletonInline( KMsgRouter );
