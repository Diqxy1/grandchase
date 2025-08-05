#include "MRConnector.h"
#include "MsgServer.h"
#include "MSSimLayer.h"
#include "MSEvent.h"
//FILE_NAME_FOR_LOG
ImplementSingleton( KMRConnector );

DWORD KMRConnector::ms_dwReconnectGap( 1000 * 60 * 2 );  // 2분.

KMRConnector::KMRConnector(void)
: m_bOnClosing( false )
{
    m_dwReconnectTick = ::GetTickCount();
}

KMRConnector::~KMRConnector(void)
{
}

ImplToStringW( KMRConnector )
{
    KLocker lock1( m_csMRProxy );
    KLocker lock2( m_csMRAddress );

    int nAliveProxyCount = 0;
    int nMRProxyQueSize  = 0;
    std::map<int,KMRProxyPtr>::const_iterator mit;
    for( mit = m_mapMRProxy.begin(); mit != m_mapMRProxy.end(); mit++ )
    {
        if( mit->second && mit->second->IsConnected() )
        {
            nAliveProxyCount++;
            nMRProxyQueSize += mit->second->GetQueueSize();
        }
    }

    return START_TOSTRING2W( KMRConnector )
        << TOSTRINGWb( m_bOnClosing )
        << TOSTRINGW( m_mapMRAddress.size() )
        << TOSTRINGW( m_mapMRProxy.size() )
        << TOSTRINGW( nAliveProxyCount )
        << TOSTRINGW( nMRProxyQueSize );
}

void KMRConnector::SetMRAddress( IN int iID_, IN const char* szIP_, IN unsigned short usPort_ )
{
    KServerInfo kInfo;
    kInfo.m_strIP   = szIP_;
    kInfo.m_usPort  = usPort_;

    // []연산자를 쓰면 동일한 id의 셋팅이 시도되는 경우도 그냥 덮어쓰게 된다.
    SetAddress( iID_, kInfo );

    {
        KLocker lock( m_csMRProxy );
        // 초기에도 무효한 포인터로 mapProxy를 우선 채워둔다.
        m_mapMRProxy[ iID_ ] = KMRProxyPtr();
    }

    START_LOG( clog, L"[SetMRAddress]" )
        << BUILD_LOG( iID_ )
        << BUILD_LOG( szIP_ )
        << BUILD_LOG( usPort_ ) << END_LOG;
}

void KMRConnector::ConnectAll()
{
    std::map<int,KServerInfo> mapAddress;
    GetAllAddress( mapAddress );

    std::map<int,KServerInfo>::iterator mit;
    for ( mit = mapAddress.begin() ; mit != mapAddress.end() ; ++mit ) {
        Connect( mit->first );
    }
}

bool KMRConnector::Connect( int iID_ )
{
    KServerInfo kMRInfo;
    _JIF( GetAddress( iID_, kMRInfo ), return false );

    KMRProxyPtr spMRProxy( new KMRProxy );
    spMRProxy->SetID( iID_ );

    KPacketNameOK kPacket;
    kPacket.m_nOK       = SiKMsgServer()->GetUID();
    SiKMsgServer()->GetServerName( kPacket.m_strName );

    KEventPtr spEvent( new KMREvent );
    spEvent->m_usEventID = KMREvent::EMR_VERIFY_ACCOUNT_REQ;
    spEvent->Serialize( kPacket );

    int nRet = spMRProxy->ConnectNAuth( kMRInfo, spEvent, 5000 );

    if ( nRet == 0 ) {
        {
            KLocker lock( m_csMRProxy );
            m_mapMRProxy[ iID_ ] = spMRProxy;
        }

        // 재접속이 성공한 경우, 해당 router를 사용하는 유저들이 재등록을 해야한다.
        SiKMSSimLayer()->m_kActorManager.QueueingToAll( KMSEvent::EMS_S2_LOGON_REPORT_REQ, iID_ );
    }

    _LOG_SUCCESS( nRet == 0, L"Result : " << nRet )
        << BUILD_LOG( iID_ )
        << BUILD_LOG( KncUtil::toWideString( kMRInfo.m_strIP ) )
        << BUILD_LOG( kMRInfo.m_usPort ) << END_LOG;

    return (nRet == 0);
}

int KMRConnector::GetOwnerMRid( DWORD dwUserUID_ )
{
    KLocker lock( m_csMRProxy );

    if( m_mapMRProxy.empty() ) return -1;

    return dwUserUID_ % m_mapMRProxy.size();
}

KMRProxyPtr KMRConnector::GetOwnerMRProxy( DWORD dwUserUID_ )
{
    int iMRUid = GetOwnerMRid( dwUserUID_ );

    if( iMRUid < 0 )
        START_LOG( cerr, L"m_mapMRProxy 가 비어있음!! iMRUid : " << iMRUid ) << END_LOG;

    KLocker lock( m_csMRProxy );

    std::map<int,KMRProxyPtr>::iterator mit;
    mit = m_mapMRProxy.find( iMRUid );

    if( mit == m_mapMRProxy.end() )
    {
        START_LOG( cerr, L"m_mapMRProxy 가 비어있지는 않은데 문제가 있음!! iMRUid : " << iMRUid )
            << BUILD_LOG( m_mapMRProxy.size() ) << END_LOG;

        return KMRProxyPtr();
    }

    return mit->second;
}


void KMRConnector::RemoveMRProxy( IN const int iID_ )
{
    std::map<int,KMRProxyPtr>::iterator mit;

    KLocker lockMR( m_csMRProxy );

    mit = m_mapMRProxy.find( iID_ );
    if( mit == m_mapMRProxy.end() )
    {
        START_LOG( cerr, L"삭제하고자 하는 mrproxy를 찾을 수 없음" )
            << BUILD_LOG( iID_ )
            << BUILD_LOG( m_mapMRProxy.size() ) << END_LOG;
    }
    else
    {
        if( mit->second )
        {
            mit->second->RemoveIOCPKey();
            mit->second.reset();
        }
    }
}

void KMRConnector::Tick()
{
    //KMRProxyPtr spProxy( new KMRProxy );
    if ( m_bOnClosing == false ) {
        // kkurrung. 살짝 무책임한 행동인다.. SetMRAddress 함수가 실행시 1회 호출되기 때문에
        // 이렇게 할수 있다.. 나중에 주의 해야 한다.
        EnterCriticalSection( &m_csMRProxy );

        std::map<int,KMRProxyPtr>::iterator mit;
        for( mit = m_mapMRProxy.begin() ; mit != m_mapMRProxy.end() ; ++mit )
        {
            KMRProxyPtr spProxy;
            spProxy = mit->second;
            LeaveCriticalSection( &m_csMRProxy );

            if( spProxy )
                spProxy->Tick();

            EnterCriticalSection( &m_csMRProxy );
        }
        LeaveCriticalSection( &m_csMRProxy );
    }

    // 객체 삭제가 예약된 uid가 있으면 찾아서 지운다. (객체만을 지우고, map에는 그대로 남겨둔다.)
    {
        std::set<int> setMRid;
        GetDestroyMRProxy( setMRid );

        std::set<int>::iterator sit;
        for ( sit = setMRid.begin(); sit != setMRid.end(); ++sit ) {
            RemoveMRProxy( *sit );
        }

        ClearDestroyMRProxy();
    }
    
    // 일정 주기에, 종료처리중이 아닐 때에만.
    if( GetTickCount() - m_dwReconnectTick >= ms_dwReconnectGap && !m_bOnClosing )
    {
        std::vector<int> vecMRid;
        CheckMRConnection( vecMRid );

        std::vector<int>::iterator vit;
        for ( vit = vecMRid.begin() ; vit != vecMRid.end() ; ++vit ) {
            Connect( *vit );
        }

        m_dwReconnectTick = ::GetTickCount();
    }

    // MR큐 패킷 처리
    SendQueData();
}

void KMRConnector::DestroyMRProxy( int iID_ )
{
    KLocker lock( m_csDestroyList );
    m_setDestroyList.insert( iID_ );
}

void KMRConnector::StartShutdown()
{
    m_bOnClosing = true;

    KLocker lock( m_csMRProxy );

    std::map<int,KMRProxyPtr>::iterator mit;
    for( mit = m_mapMRProxy.begin(); mit != m_mapMRProxy.end(); mit++ )
    {
        if( mit->second )
            mit->second->ReserveDestroy();
    }
}

bool KMRConnector::ShutdownFinished()
{
    // 아직 종료처리중이 아니라면 false를 리턴한다.
    if( !m_bOnClosing ) return false;

    KLocker lock( m_csMRProxy );

    std::map<int,KMRProxyPtr>::iterator mit;
    for( mit = m_mapMRProxy.begin(); mit != m_mapMRProxy.end(); mit++ )
    {
        // 아직 살아있는 포인터가 있다면 false.
        if( mit->second )
        {
            mit->second->RemoveIOCPKey();
            m_mapMRProxy.erase( mit );
            return false;
        }
    }
    //End();
    return true;
}
void KMRConnector::Run()
{
    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Tick();

        else    START_LOG( cerr, L"*** WaitForSingleObject() - return : " << ret ) << END_LOG;
    }
}

void KMRConnector::ShutDown()
{
    End( INFINITE ); // Thread 멈춤..

    {
        KLocker lockMR( m_csMRProxy );
        m_mapMRProxy.clear();
    }
    {
        KLocker lock( m_csMRAddress );
        m_mapMRAddress.clear();
    }

    ClearDestroyMRProxy();
}

bool KMRConnector::GetAddress( IN const int nID_, OUT KServerInfo& kInfo_ )
{
    KLocker lock( m_csMRAddress );

    std::map<int,KServerInfo>::iterator mit;
    mit = m_mapMRAddress.find( nID_ );
    if ( mit == m_mapMRAddress.end() ){
        START_LOG( cerr, L"접속하고자 하는 MR의 주소정보를 찾을 수 없음. MR ID : " << nID_ << L",size : " << m_mapMRAddress.size() ) << END_LOG;
        return false;
    }

    kInfo_ = mit->second;
    return true;
}

void KMRConnector::SendQueData()
{
    std::deque< KMRPacket > queData;

    {
        KLocker lock( m_csQuePacket );
        m_quePacket.swap( queData );
    }

    if ( queData.empty() ) {
        return;
    }

    std::deque< KMRPacket >::iterator qit;
    for ( qit = queData.begin() ; qit != queData.end() ; ++qit ) {
        LIF( RemoteQueueingPack( qit->m_dwSenderUID, qit->m_vecRecverUID, qit->m_usEventID, qit->m_kBuff, qit->m_bCheckRecv, qit->m_bDirectSend ) );
    }
}

bool KMRConnector::RemoteQueueingPack( DWORD dwSenderUID_,
                                       std::vector<DWORD>& vecRecverUIDList_,
                                       unsigned short usEventID_,
                                       KSerBuffer& kBuff_,
                                       bool bCheckRecv_,
                                       bool bDirectSend_ )
{
    std::map< int, std::vector<DWORD> > mapMRProxyUIDList;

    if ( vecRecverUIDList_.empty() ) {
        return true;
    }

    _JIF( IsUsingRouter(), return false );

    std::vector<DWORD>::iterator vit;
    for ( vit = vecRecverUIDList_.begin() ; vit != vecRecverUIDList_.end() ; ++vit ) {
        int iMRUID = GetOwnerMRid( *vit );

        if ( iMRUID < 0 ) {
            START_LOG( cerr, L"MR 포인터를 찾을 수 없음. UserUID :" << *vit )
                << BUILD_LOG( usEventID_ )
                << BUILD_LOG( KMSEvent::GetEventIDString( usEventID_ ) )
                << BUILD_LOG( iMRUID ) << END_LOG;

            __LIF( Connect( GetOwnerMRid( *vit ) ) );    // 무효한 연결을 발견하면 재접속 시도.
            continue;
        }

        mapMRProxyUIDList[iMRUID].push_back( *vit );
    }

    std::map< int, std::vector<DWORD> >::iterator mit;
    for ( mit = mapMRProxyUIDList.begin() ; mit != mapMRProxyUIDList.end() ; ++mit ) {
        KLocker lock( m_csMRProxy );

        std::map<int,KMRProxyPtr>::iterator mitProxy;
        mitProxy = m_mapMRProxy.find( mit->first );

        if ( mitProxy->second != NULL ) {
            mitProxy->second->RemoteQueueingPack( dwSenderUID_, mit->second, usEventID_, kBuff_, bCheckRecv_, bDirectSend_ );
        }
    }
    return true;
}

void KMRConnector::CheckMRConnection( OUT std::vector<int>& vecMRid_ )
{
    vecMRid_.clear();

    {
        KLocker lock( m_csMRProxy );

        std::map<int,KMRProxyPtr>::iterator mit;
        for ( mit = m_mapMRProxy.begin() ; mit != m_mapMRProxy.end() ; ++mit ) {
            if ( !mit->second ) {
                vecMRid_.push_back( mit->first );
            }
        }
    }

    START_LOG( clog, L"MR 접속상태 체크.. Connection Fail size : " << vecMRid_.size() ) << END_LOG;
}