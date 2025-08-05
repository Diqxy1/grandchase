#pragma once
#include "AgentProxy.h"
#include "KNCSingleton.h"
#include "KncTimer.h"

class KAgentConnector
{
    DeclareSingleton( KAgentConnector );
    DeclToStringW;
public:
    struct KProxyAddress
    {
        bool            m_bEnterance; // 입장가능한지
        std::string     m_strIP;
        unsigned short  m_usPort;
    };

    KAgentConnector(void);
    virtual ~KAgentConnector(void);

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

    void SetAgentMasterSID( const int& nSID );
    int GetAgentMasterSID();

    KAgentProxyPtr GetAgentProxy( const int& nSID );
    void GetDisconnectedProxyList( OUT std::vector<int>& vecList );
    void KeepConnectionThread();

    template < typename T > 
    bool SendPacket( int nSID, unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    bool SendServerRelayPacket( const int nSID, const unsigned short usEventID, const KSerBuffer& kEventBuff, const DWORD dwSenderUID, const std::wstring& strSenderLogin,
        const DWORD dwRecverUID, const std::wstring& strRecverLogin, const bool bUID_, const bool bDirectSend_ );
    bool SendAgitRelayPacket( const int nSID, const unsigned short usEventID, const KSerBuffer& kEventBuff, const DWORD dwSenderUID, const DWORD dwAgitUID );

protected:
    mutable KncCriticalSection      m_csAddress;
    int                             m_nMasterSID; // Agent Master SID
    std::map<int,KProxyAddress>     m_mapAddress; // map[AgentServerID, ServerAddress]

    mutable KncCriticalSection      m_csProxy;
    std::map<int,KAgentProxyPtr>    m_mapAgPtr; // map[AgentServerID,ProxyPtr]
    std::vector<int>                m_vecDisconnectList; // 연결종료된 서버ID리스트 map[AgentServerID]

    DWORD                           m_dwShutdownTimeout;
    KncTimer                        m_kConnectTimer;
    std::shared_ptr< KTThread<KAgentConnector> >  m_spThreadKeepConnect;
};
DefSingletonInline( KAgentConnector );
DeclOstmOperatorA( KAgentConnector );

template < typename T > 
bool KAgentConnector::SendPacket( int nSID_, unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
    KAgentProxyPtr spProxy = GetAgentProxy( nSID_ );
    __JIF( spProxy, return false );
    __JIF( spProxy->IsConnected(), return false );
    __JIF( spProxy->IsAfterVerify(), return false );
    return spProxy->SendPacket( usEventID_, data_, bLogging_, bCompress_ );
}