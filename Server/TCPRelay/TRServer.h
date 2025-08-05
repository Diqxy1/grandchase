#pragma once
#include "NetLayer.h"
#include <KncLua.h>
#include <boost/shared_ptr.hpp>
#include <Thread/Thread.h>
#include "TRUser.h"
#include "TRUserManager.h"
#include <queue>
#include "RelayData.h"

class KTRServer : public KNetLayer
{
    
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KTRServer );
public:
    KTRServer(void);
    virtual ~KTRServer(void);

    bool Init( const char* szFileName );
    void Run();
    //derived from NetLayer
    virtual void ShutDown();

    void QueueingRelayData( KRelayDataPtr& spData );

protected:
    //derived from NetLayer
    virtual void RegToLua( boost::shared_ptr<lua_State> L );

    void Tick();

public:
    KTRUserManager                                  m_kTRUserManager;

protected:
    mutable KncCriticalSection                      m_csPushQueue;
    mutable KncCriticalSection                      m_csPopQueue;   // tostring을 위해선 보호해야 한다.
    std::queue< KRelayDataPtr >*                    m_pkPushQueue;
    std::queue< KRelayDataPtr >*                    m_pkPopQueue;

    std::queue< KRelayDataPtr >                     m_queue1;
    std::queue< KRelayDataPtr >                     m_queue2;

    boost::shared_ptr<lua_State>                    m_sptlua;
    std::string                                     m_strTagVersion;

    boost::shared_ptr< KTThread<KTRServer> >        m_spThreadick;

};
DefSingletonInline( KTRServer );