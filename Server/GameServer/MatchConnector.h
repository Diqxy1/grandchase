#pragma once
#include "MatchProxy.h"
#include "KNCSingleton.h"
#include "KncTimer.h"

class KMatchConnector
{
    DeclareSingleton( KMatchConnector );
    DeclToStringW;

public:
    struct KProxyAddress
    {
        bool            m_bEnterance; // 방생성 가능한지
        std::string     m_strIP;
        unsigned short  m_usPort;
    };

    KMatchConnector(void);
    virtual ~KMatchConnector(void);

    // 기본
    void Init();
    void Tick();
    void ShutDown();
    bool ShutDownFinished();

    bool InitConnect();
    inline bool Connect( const int& nSID );

    void SetRemoteAddress( const int& nSID, const char* szIP, const unsigned short& usPort );
    bool GetRemoteAddress( const int& nSID, OUT KProxyAddress& kAddress );
    void GetSIDList( OUT std::vector<int>& vecSID );
    size_t GetSIDCount();

    KMatchProxyPtr GetMatchProxy( const int& nSID );
    void GetDisconnectedProxyList( OUT std::vector<int>& vecList );
    void KeepConnectionThread();

    template < typename T > 
    bool SendPacket( int nSID, unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    bool SendMatchRelayPacket( const int nSID, const unsigned short usEventID, const KSerBuffer& kEventBuff, const DWORD dwSenderUID, const DWORD dwAgitUID );
    bool GetFirstSID( OUT int& nMatchSID );

protected:
    mutable KncCriticalSection      m_csAddress;
    std::map<int,KProxyAddress>     m_mapAddress; // map[MatchServerID, ServerAddress]
    mutable KncCriticalSection      m_csProxy;
    std::map<int,KMatchProxyPtr>     m_mapMatchPtr; // map[MatchServerID,ProxyPtr]
    std::vector<int>                m_vecDisconnectList; // 연결종료된 서버ID리스트 map[AgentServerID]

    DWORD                           m_dwShutdownTimeout;
    KncTimer                        m_kConnectTimer;
    std::shared_ptr< KTThread<KMatchConnector> >  m_spThreadKeepConnect;
};
DefSingletonInline( KMatchConnector );
DeclOstmOperatorA( KMatchConnector );

template < typename T > 
bool KMatchConnector::SendPacket( int nSID_, unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
    KMatchProxyPtr spProxy = GetMatchProxy( nSID_ );
    __JIF( spProxy, return false );
    __JIF( spProxy->IsConnected(), return false );
    __JIF( spProxy->IsAfterVerify(), return false );
    return spProxy->SendPacket( usEventID_, data_, bLogging_, bCompress_ );
}
