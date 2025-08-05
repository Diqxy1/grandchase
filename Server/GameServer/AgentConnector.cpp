#include "AgentConnector.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KAgentConnector );

KAgentConnector::KAgentConnector(void)
:m_dwShutdownTimeout( 1000 * 20 )
,m_kConnectTimer( 1000 * 10 )
,m_nMasterSID( -1 )
{
}

KAgentConnector::~KAgentConnector(void)
{
    ShutDown();
}

ImplOstmOperatorW2A( KAgentConnector );

ImplToStringW( KAgentConnector )
{
    KLocker lock1(m_csProxy);
    KLocker lock2(m_csAddress);

    stm_ << TOSTRINGW( m_mapAddress.size() )
         << TOSTRINGW( m_mapAgPtr.size() )
         << TOSTRINGW( m_vecDisconnectList.size() );

    std::map<int,KAgentProxyPtr>::const_iterator cmit;
    for ( cmit = m_mapAgPtr.begin() ; cmit != m_mapAgPtr.end() ; ++cmit ) {
        if ( cmit->second == NULL ) {
            continue;
        }

        cmit->second->ToString( stm_ );
    }

    return stm_;
}

void KAgentConnector::Init()
{
    KLocker lock1(m_csProxy);
    KLocker lock2(m_csAddress);

    std::map<int,KProxyAddress>::iterator mit;
    for ( mit = m_mapAddress.begin() ; mit != m_mapAddress.end() ; ++mit ) {
        KAgentProxyPtr spProxy;
        spProxy.reset( new KAgentProxy );

        m_mapAgPtr[ mit->first ] = spProxy;
    }

    m_vecDisconnectList.clear();
}

void KAgentConnector::Tick()
{
    KLocker lock(m_csProxy);

    std::map<int,KAgentProxyPtr>::iterator mit;
    for ( mit = m_mapAgPtr.begin() ; mit != m_mapAgPtr.end() ; ++mit ) {
        if ( mit->second == NULL ) {
            continue;
        }

        if ( false == mit->second->IsConnected() ) {
            m_vecDisconnectList.push_back( mit->first );
        }

        mit->second->Tick();
    }

    std::vector<int>::iterator vit;
    for ( vit = m_vecDisconnectList.begin() ; vit != m_vecDisconnectList.end() ; ++vit ) {
        mit = m_mapAgPtr.find( *vit );
        if ( mit == m_mapAgPtr.end() ) {
            START_LOG( cwarn, L"삭제할 AgentProxy를 찾을 수 없음." ) << END_LOG;
            continue;
        }

        mit->second.reset();
    }
    m_vecDisconnectList.clear();
}

bool KAgentConnector::InitConnect()
{
    std::vector<int> vecSID;
    GetSIDList( vecSID );

    std::vector<int>::iterator vit;
    for ( vit = vecSID.begin() ; vit != vecSID.end() ; ++vit ) {
        Connect( *vit );
    }

    m_spThreadKeepConnect = std::shared_ptr< KTThread<KAgentConnector> >( new KTThread<KAgentConnector>( *this, &KAgentConnector::KeepConnectionThread, 10000 ) );

    if ( m_spThreadKeepConnect ) {
        m_spThreadKeepConnect->Begin();
        START_LOG( cerr, L"AgentConnector 재접속 Thread 시작" ) << END_LOG;
        return true;
    }

    START_LOG( cerr, L"AgentConnector 재접속 Thread 생성 실패" ) << END_LOG;
    return false;
}

bool KAgentConnector::Connect( const int& nSID_ )
{
    KProxyAddress kAddress;
    __JIF( GetRemoteAddress( nSID_, kAddress ), return false );
    __JIF( !kAddress.m_strIP.empty(), return false );
    __JIF( kAddress.m_usPort > 0, return false );

    bool bConnect = true;
    KAgentProxyPtr spProxy;
    spProxy.reset( new KAgentProxy );
    if ( !spProxy->Connect( kAddress.m_strIP, kAddress.m_usPort ) ) {
        START_LOG( cerr, L"에이전트 서버로 접속 실패.. SID : " << nSID_ )
            << BUILD_LOG( KncUtil::toWideString(kAddress.m_strIP) )
            << BUILD_LOG( kAddress.m_usPort ) << END_LOG;
        bConnect = false;
    }

    {
        KLocker lock(m_csProxy);
        m_mapAgPtr[ nSID_ ] = spProxy;
    }

    return bConnect;
}

void KAgentConnector::ShutDown()
{
    // 일단 재접속 Thread부터 죽인다.
    if ( m_spThreadKeepConnect ) {
        m_spThreadKeepConnect->End( 1000 );
    }

    {
        KLocker lock(m_csProxy);

        std::map<int,KAgentProxyPtr>::iterator mit;
        for ( mit = m_mapAgPtr.begin() ; mit != m_mapAgPtr.end() ; ++mit ) {
            if ( mit->second ) {
                mit->second->ReserveDestroy();
            }
        }
    }

    bool bShutdownFinished = ShutDownFinished();
    DWORD dwTick = ::GetTickCount();
    while ( ::GetTickCount() - dwTick < m_dwShutdownTimeout && !bShutdownFinished ) {
        Sleep( 500 );
        bShutdownFinished = ShutDownFinished();
    }

    LOG_SUCCESS( bShutdownFinished )
        << BUILD_LOG( m_dwShutdownTimeout ) << END_LOG;
}

bool KAgentConnector::ShutDownFinished()
{
    KLocker lock(m_csProxy);

    std::map<int,KAgentProxyPtr>::iterator mit;
    for ( mit = m_mapAgPtr.begin() ; mit != m_mapAgPtr.end() ; ++mit ) {
        if ( mit->second ) {
            return false;
        }
    }

    return true;
}

void KAgentConnector::SetRemoteAddress( const int& nSID_, const char* szIP_, const unsigned short& usPort_ )
{
    KLocker lock(m_csAddress);
    m_mapAddress[nSID_].m_strIP = szIP_;
    m_mapAddress[nSID_].m_usPort = usPort_;
}

bool KAgentConnector::GetRemoteAddress( const int& nSID_, OUT KProxyAddress& kAddress_ )
{
    KLocker lock(m_csAddress);

    std::map<int,KProxyAddress>::iterator mit;
    mit = m_mapAddress.find( nSID_ );
    if ( mit == m_mapAddress.end() ) {
        START_LOG( cwarn, L"해당하는 Proxy Address정보가 없음. SID : " << nSID_ ) << END_LOG;
        return false;
    }

    kAddress_ = mit->second;
    return true;
}

void KAgentConnector::GetSIDList( OUT std::vector<int>& vecSID_ )
{
    vecSID_.clear();

    KLocker lock(m_csAddress);

    vecSID_.reserve( m_mapAddress.size() );
    std::map<int,KProxyAddress>::iterator mit;
    for ( mit = m_mapAddress.begin() ; mit != m_mapAddress.end() ; ++mit ) {
        vecSID_.push_back( mit->first );
    }
}

size_t KAgentConnector::GetSIDCount()
{
    KLocker lock(m_csAddress);
    return m_mapAddress.size();
}

void KAgentConnector::SetAgentMasterSID( const int& nSID_ )
{
    KLocker lock(m_csAddress);
    m_nMasterSID = nSID_;
}

int KAgentConnector::GetAgentMasterSID()
{
    KLocker lock(m_csAddress);
    return m_nMasterSID;
}

bool KAgentConnector::SendServerRelayPacket( const int nSID_, const unsigned short usEventID_, const KSerBuffer& kEventBuff_, const DWORD dwSenderUID_, const std::wstring& strSenderLogin_,
                                         const DWORD dwRecverUID_, const std::wstring& strRecverLogin_, const bool bUID_, const bool bDirectSend_ )
{
    KAgentProxyPtr spProxy = GetAgentProxy( nSID_ );
    __JIF( spProxy, return false );
    __JIF( spProxy->IsConnected(), return false );
    __JIF( spProxy->IsAfterVerify(), return false );

    KServerRelayPacket kPacket;
    kPacket.m_dwSenderUID       = dwSenderUID_;
    kPacket.m_strSenderLogin    = strSenderLogin_;
    kPacket.m_dwRecverUID       = dwRecverUID_;
    kPacket.m_strRecverLogin    = strRecverLogin_;
    kPacket.m_bUseUID           = bUID_;
    kPacket.m_bDirectSend       = bDirectSend_;
    kPacket.m_usEventID         = usEventID_;
    kPacket.m_kEventBuff        = kEventBuff_;

    return spProxy->SendPacket( KAgentEvent::EAG_SERVER_RELAY_PACKET_REQ, kPacket );
}

bool KAgentConnector::SendAgitRelayPacket( const int nSID_, const unsigned short usEventID_, const KSerBuffer& kEventBuff_, const DWORD dwSenderUID_, const DWORD dwAgitUID_ )
{
    KAgentProxyPtr spProxy = GetAgentProxy( nSID_ );
    __JIF( spProxy, return false );
    __JIF( spProxy->IsConnected(), return false );
    __JIF( spProxy->IsAfterVerify(), return false );

    KAgitRelayPacket kPacket;
    kPacket.m_dwAgitUID         = dwAgitUID_;
    kPacket.m_dwSenderUID       = dwSenderUID_;
    kPacket.m_usEventID         = usEventID_;
    kPacket.m_kEventBuff        = kEventBuff_;

    return spProxy->SendPacket( KAgentEvent::EAG_AGIT_RELAY_PACKET_REQ, kPacket );
}

KAgentProxyPtr KAgentConnector::GetAgentProxy( const int& nSID_ )
{
    KLocker lock(m_csProxy);

    std::map<int,KAgentProxyPtr>::iterator mit;
    mit = m_mapAgPtr.find( nSID_ );
    if ( mit == m_mapAgPtr.end() ) {
        return KAgentProxyPtr();
    }

    return mit->second;
}

void KAgentConnector::GetDisconnectedProxyList( OUT std::vector<int>& vecList_ )
{
    vecList_.clear();

    KLocker lock(m_csProxy);

    std::map<int,KAgentProxyPtr>::iterator mit;
    for ( mit = m_mapAgPtr.begin() ; mit != m_mapAgPtr.end() ; ++mit ) {
        if ( mit->second == NULL ) {
            vecList_.push_back( mit->first );
        }
    }
}

void KAgentConnector::KeepConnectionThread()
{
    if ( !m_kConnectTimer.CheckTime() ) {
        return;
    }

    std::vector<int> vecDisconnectList;
    GetDisconnectedProxyList( vecDisconnectList );
    //START_LOG( clog, L"vecDisconnectList.size() : " << vecDisconnectList.size() ) << END_LOG;

    std::vector<int>::iterator vit;
    for ( vit = vecDisconnectList.begin() ; vit != vecDisconnectList.end() ; ++vit ) {
        if ( Connect( *vit ) ) {
            START_LOG( cerr, L" Reconnect To AgentServer. SID : " << *vit ) << END_LOG;
        }
    }
}