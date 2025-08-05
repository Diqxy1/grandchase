#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include "TRServer.h"
#include "TRSimLayer.h"
#include "TRNetLayer.h"
#include "TRDBLayer.h"
#include "TREvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KTRServer, KBaseServer );
ImplementSingleton( KTRServer );

ImplToStringW( KTRServer )
{
    START_TOSTRING_PARENTW( KBaseServer )
        << TOSTRINGW( m_kTRUserManager.GetCountTot() )
        << TOSTRINGW( m_kTRUserManager.GetCountReg() );

    size_t nPopQueueSize = 0;
    size_t nPushQueueSize = 0;
    {
        KLocker lock( m_csPushQueue );
        nPushQueueSize = m_pkPushQueue->size();
    }
    {
        KLocker lock( m_csPopQueue );
        nPopQueueSize = m_pkPopQueue->size();
    }

    return stm_ << TOSTRINGW( nPushQueueSize )
        << TOSTRINGW( nPopQueueSize );
}

KTRServer::KTRServer(void)
{
    m_pkPushQueue   = &m_queue1;
    m_pkPopQueue    = &m_queue2;
}

KTRServer::~KTRServer(void)
{
    m_hFirstSyncEvent = NULL;
}

ImplOstmOperatorW2A( KTRServer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KTRServer::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get())
    [
        luabind::def( "GetTRServer", &KTRServer::GetInstance ),
        luabind::class_<KTRServer, KBaseServer>( "KTRServer" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetServerProtocol", &KTRServer::SetServerProtocol )
            .def( "SetSID", &KTRServer::SetSID )
            .def( "SetLimitPacketSendCount", &KTRServer::SetLimitPacketSendCount )
    ];
}

bool KTRServer::Init( const wchar_t* szFileName_ )
{
    m_pkNetLayer    = SiKTRNetLayer();
    m_pkSimLayer    = SiKTRSimLayer();
    m_pkDBLayer     = SiKTRDBLayer();

    _JIF( KBaseServer::Init( szFileName_ ), false );

    m_hFirstSyncEvent = ::CreateEvent( NULL, false, false, NULL );
    KDB_EVENT_FIRST_SERVER_INFO_REQ kPacket;
    kPacket.m_bInit = true;
    kPacket.m_strIP = NetCommon::GetLocalIPW();
    kPacket.m_usPort = m_pkNetLayer->m_usPortMaster;
    kPacket.m_nType = KDB_EVENT_FIRST_SERVER_INFO_REQ::ST_TCP;
    SiKTRDBLayer()->QueueingEvent( KTREvent::DB_EVENT_FIRST_SERVER_INFO_REQ, m_strServerName.c_str(), 0, -1, kPacket );

    {
        std::wstringstream stm;
        stm << L"TR_" << SiKTRNetLayer()->m_usPortMaster << L"_" << SiKTRNetLayer()->m_usPortSlave;
        SetServerName( stm.str().c_str() );
        stm.clear();
    }

    switch( ::WaitForSingleObject( m_hFirstSyncEvent, 30000 ) ) {
    case WAIT_OBJECT_0:
        {
            std::cout << "**** Server Initialize Finished.. ****" << std::endl;
            CloseHandle( m_hFirstSyncEvent );
            m_hFirstSyncEvent = NULL;
        }
        break;
    case WAIT_TIMEOUT:
        START_LOG( cerr, L"Getting Server Info from DB failed." ) << END_LOG;
        CloseHandle( m_hFirstSyncEvent );
        return false;
        break;
    default:
        START_LOG( cerr, L"Unexpected Return value from ::WaitForSingleObject()." ) << END_LOG;
        CloseHandle( m_hFirstSyncEvent );
        return false;
    }

    SiKTRSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;
    return true;
}

void KTRServer::Run(  bool bRunInterpreter_ )
{
    KBaseServer::Run( bRunInterpreter_ );
}

void KTRServer::ShutDown()
{
    KDB_EVENT_FIRST_SERVER_INFO_REQ kPacket;
    kPacket.m_strIP = NetCommon::GetLocalIPW();
    kPacket.m_usPort = m_pkNetLayer->m_usPortMaster;
    kPacket.m_nType = KDB_EVENT_FIRST_SERVER_INFO_REQ::ST_TCP;
    SiKTRDBLayer()->QueueingEvent( KTREvent::DB_EVENT_FIRST_SERVER_INFO_REQ, m_strServerName.c_str(), 0, -1, kPacket );

    KBaseServer::ShutDown();
    KTRSimLayer::ReleaseInstance();
    KTRNetLayer::ReleaseInstance();
    KTRDBLayer::ReleaseInstance();
}

void KTRServer::Tick()
{
    m_kTRUserManager.Tick();
    
    KLocker lock( m_csPopQueue );

    {// Locking Scope

        KLocker lock( m_csPushQueue );
        std::swap( m_pkPushQueue, m_pkPopQueue );

    }// Locking Scope

    // HB, UID Reg 등은 모두 처리되었고, Relay 해야할 데이터들만 queue에 들어 있다.
    KRelayDataPtr spData;

    while( !m_pkPopQueue->empty() )
    {
        spData = m_pkPopQueue->front();
        m_pkPopQueue->pop();

        m_kTRUserManager.Relay( spData );
    }
}

const int& KTRServer::GetServerProtocol()
{
    KLocker lock( m_csServerProtocol );
    return m_nServerProtocol;
}

void KTRServer::SetServerProtocol( IN const int& nProtocol_ )
{
    KLocker lock( m_csServerProtocol );
    m_nServerProtocol = nProtocol_;
}

void KTRServer::DumpInfo()
{
    // 서버정보 로그로 출력
    DumpServerList();
}

void KTRServer::SetSID( int nSID_ )
{
    SetServerID( nSID_ );
}

void KTRServer::QueueingRelayData( KRelayDataPtr& spData )
{
    KLocker lock( m_csPushQueue );
    m_pkPushQueue->push( spData );
}

void KTRServer::SetLimitPacketSendCount( IN int nLimitPacketSendCount_ )
{
    m_kTRUserManager.SetLimitPacketSendCount( nLimitPacketSendCount_ );
}