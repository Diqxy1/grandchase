#include "MatchConnector.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMatchConnector );

KMatchConnector::KMatchConnector(void)
:m_dwShutdownTimeout( 1000 * 20 )
,m_kConnectTimer( 1000 * 10 )
{
}

KMatchConnector::~KMatchConnector(void)
{
    ShutDown();
}

ImplOstmOperatorW2A( KMatchConnector );

ImplToStringW( KMatchConnector )
{
    KLocker lock1(m_csProxy);
    KLocker lock2(m_csAddress);

    stm_ << TOSTRINGW( m_mapAddress.size() )
        << TOSTRINGW( m_mapMatchPtr.size() )
        << TOSTRINGW( m_vecDisconnectList.size() );

    std::map<int,KMatchProxyPtr>::const_iterator cmit;
    for ( cmit = m_mapMatchPtr.begin() ; cmit != m_mapMatchPtr.end() ; ++cmit ) {
        if ( cmit->second == NULL ) {
            continue;
        }
        cmit->second->ToString( stm_ );
    }
    return stm_;
}

void KMatchConnector::Init()
{
    KLocker lock1(m_csProxy);
    KLocker lock2(m_csAddress);

    std::map<int,KProxyAddress>::iterator mit;
    for ( mit = m_mapAddress.begin() ; mit != m_mapAddress.end() ; ++mit ) {
        KMatchProxyPtr spProxy;
        spProxy.reset( new KMatchProxy );

        m_mapMatchPtr[ mit->first ] = spProxy;
    }

    m_vecDisconnectList.clear();
}

void KMatchConnector::Tick()
{
    KLocker lock(m_csProxy);

    std::map<int,KMatchProxyPtr>::iterator mit;
    for ( mit = m_mapMatchPtr.begin() ; mit != m_mapMatchPtr.end() ; ++mit ) {
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
        mit = m_mapMatchPtr.find( *vit );
        if ( mit == m_mapMatchPtr.end() ) {
            START_LOG( cwarn, L"삭제할 MatchProxy를 찾을 수 없음." ) << END_LOG;
            continue;
        }

        mit->second.reset();
    }
    m_vecDisconnectList.clear();
}

bool KMatchConnector::InitConnect()
{
    std::vector<int> vecSID;
    GetSIDList( vecSID );

    std::vector<int>::iterator vit;
    for ( vit = vecSID.begin() ; vit != vecSID.end() ; ++vit ) {
        Connect( *vit );
    }

    m_spThreadKeepConnect = std::shared_ptr< KTThread<KMatchConnector> >( new KTThread<KMatchConnector>( *this, &KMatchConnector::KeepConnectionThread, 10000 ) );

    if ( m_spThreadKeepConnect ) {
        m_spThreadKeepConnect->Begin();
        START_LOG( cout, L"MatchConnector 재접속 Thread 시작" ) << END_LOG;
        return true;
    }

    START_LOG( cerr, L"MatchConnector 재접속 Thread 생성 실패" ) << END_LOG;
    return false;
}

bool KMatchConnector::Connect( const int& nSID_ )
{
    KProxyAddress kAddress;
    __JIF( GetRemoteAddress( nSID_, kAddress ), return false );
    __JIF( !kAddress.m_strIP.empty(), return false );
    __JIF( kAddress.m_usPort > 0, return false );

    bool bConnect = true;
    KMatchProxyPtr spProxy;
    spProxy.reset( new KMatchProxy );
    if ( !spProxy->Connect( nSID_, kAddress.m_strIP, kAddress.m_usPort ) ) {
        START_LOG( cerr, L"매칭 서버로 접속 실패.. SID : " << nSID_ )
            << BUILD_LOG( KncUtil::toWideString(kAddress.m_strIP) )
            << BUILD_LOG( kAddress.m_usPort ) << END_LOG;
        bConnect = false;
    }

    {
        KLocker lock(m_csProxy);
        m_mapMatchPtr[ nSID_ ] = spProxy;
    }

    return bConnect;
}

void KMatchConnector::ShutDown()
{
    // 일단 재접속 Thread부터 죽인다.
    if ( m_spThreadKeepConnect ) {
        m_spThreadKeepConnect->End( 1000 );
    }

    {
        KLocker lock(m_csProxy);

        std::map<int,KMatchProxyPtr>::iterator mit;
        for ( mit = m_mapMatchPtr.begin() ; mit != m_mapMatchPtr.end() ; ++mit ) {
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

bool KMatchConnector::ShutDownFinished()
{
    KLocker lock(m_csProxy);

    std::map<int,KMatchProxyPtr>::iterator mit;
    for ( mit = m_mapMatchPtr.begin() ; mit != m_mapMatchPtr.end() ; ++mit ) {
        if ( mit->second ) {
            return false;
        }
    }

    return true;
}

void KMatchConnector::SetRemoteAddress( const int& nSID_, const char* szIP_, const unsigned short& usPort_ )
{
    KLocker lock(m_csAddress);
    m_mapAddress[nSID_].m_strIP = szIP_;
    m_mapAddress[nSID_].m_usPort = usPort_;
}

bool KMatchConnector::GetRemoteAddress( const int& nSID_, OUT KProxyAddress& kAddress_ )
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

void KMatchConnector::GetSIDList( OUT std::vector<int>& vecSID_ )
{
    vecSID_.clear();

    KLocker lock(m_csAddress);

    vecSID_.reserve( m_mapAddress.size() );
    std::map<int,KProxyAddress>::iterator mit;
    for ( mit = m_mapAddress.begin() ; mit != m_mapAddress.end() ; ++mit ) {
        vecSID_.push_back( mit->first );
    }
}

size_t KMatchConnector::GetSIDCount()
{
    KLocker lock(m_csAddress);
    return m_mapAddress.size();
}

bool KMatchConnector::SendMatchRelayPacket( const int nSID_, const unsigned short usEventID_, const KSerBuffer& kEventBuff_, const DWORD dwSenderUID_, const DWORD dwAgitUID_ )
{
    KMatchProxyPtr spProxy = GetMatchProxy( nSID_ );
    __JIF( spProxy, return false );
    __JIF( spProxy->IsConnected(), return false );
    __JIF( spProxy->IsAfterVerify(), return false );

    return true;
}

KMatchProxyPtr KMatchConnector::GetMatchProxy( const int& nSID_ )
{
    KLocker lock(m_csProxy);

    std::map<int,KMatchProxyPtr>::iterator mit;
    mit = m_mapMatchPtr.find( nSID_ );
    if ( mit == m_mapMatchPtr.end() ) {
        return KMatchProxyPtr();
    }

    return mit->second;
}

void KMatchConnector::GetDisconnectedProxyList( OUT std::vector<int>& vecList_ )
{
    vecList_.clear();

    KLocker lock(m_csProxy);

    std::map<int,KMatchProxyPtr>::iterator mit;
    for ( mit = m_mapMatchPtr.begin() ; mit != m_mapMatchPtr.end() ; ++mit ) {
        if ( mit->second == NULL ) {
            vecList_.push_back( mit->first );
        }
    }
}

void KMatchConnector::KeepConnectionThread()
{
    if ( !m_kConnectTimer.CheckTime() ) {
        return;
    }

    std::vector<int> vecDisconnectList;
    GetDisconnectedProxyList( vecDisconnectList );

    std::vector<int>::iterator vit;
    for ( vit = vecDisconnectList.begin() ; vit != vecDisconnectList.end() ; ++vit ) {
        if ( Connect( *vit ) ) {
            START_LOG( cout, L" Reconnect To MatchServer. SID : " << *vit ) << END_LOG;
        }
    }
}

bool KMatchConnector::GetFirstSID( OUT int& nMatchSID_ )
{
    std::map<int,KProxyAddress>::iterator mit;
    KLocker lock(m_csAddress);

    if( !m_mapAddress.empty() ) {
        mit = m_mapAddress.begin();
        nMatchSID_ = mit->first;
        return true;
    }
    return false;
}