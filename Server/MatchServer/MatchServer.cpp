#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include "MatchServer.h"
#include "MatchSimLayer.h"
#include "MatchNetLayer.h"
#include "MatchDBLayer.h"
#include "MatchEvent.h"
#include "MatchManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMatchServer, KBaseServer );
ImplementSingleton( KMatchServer );

ImplToStringW( KMatchServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

KMatchServer::KMatchServer(void)
{
}

KMatchServer::~KMatchServer(void)
{
    m_hFirstSyncEvent = NULL;
}

ImplOstmOperatorW2A( KMatchServer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMatchServer::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get())
    [
        luabind::def( "GetMatchServer", &KMatchServer::GetInstance ),
        luabind::class_<KMatchServer, KBaseServer>( "KMatchServer" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetServerProtocol", &KMatchServer::SetServerProtocol )
            .def( "SetSID", &KMatchServer::SetSID ),
        luabind::def( "GetMatchManager", &KMatchManager::GetInstance ),
        luabind::class_<KMatchManager>( "KMatchManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KMatchManager::LoadScript )
    ];
}

bool KMatchServer::Init( const wchar_t* szFileName_ )
{
    m_pkNetLayer    = SiKMatchNetLayer();
    m_pkSimLayer    = SiKMatchSimLayer();
    m_pkDBLayer     = SiKMatchDBLayer();

    _JIF( KBaseServer::Init( szFileName_ ), false );
    _JIF( LoadScript(), return false );

    m_hFirstSyncEvent = ::CreateEvent( NULL, false, false, NULL );
    SiKMatchDBLayer()->QueueingEvent( KMatchEvent::DB_EVENT_FIRST_INIT_REQ, m_strServerName.c_str(), 0, -1, GetServerProtocol() );

    {
        std::wstringstream stm;
        stm << L"Match_" << SiKMatchNetLayer()->m_usPortMaster << L"_" << SiKMatchNetLayer()->m_usPortSlave;
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

    SiKMatchSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;
    return true;
}

void KMatchServer::Run(  bool bRunInterpreter_ )
{
    KBaseServer::Run( bRunInterpreter_ );
}

void KMatchServer::ShutDown()
{
    KBaseServer::ShutDown();
    KMatchSvrConnector::ReleaseInstance();
    KMatchSimLayer::ReleaseInstance();
    KMatchNetLayer::ReleaseInstance();
    KMatchDBLayer::ReleaseInstance();
    KMatchManager::ReleaseInstance();
}

void KMatchServer::Tick()
{
    if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwSendServerListGap )
    {
        m_dwServerInfoUpdateTick = ::GetTickCount();
    }

    // MatchManager Tick. Find Match를 위한 Tick.
    SiKMatchManager()->Tick();
}

const int& KMatchServer::GetServerProtocol()
{
    KLocker lock( m_csServerProtocol );
    return m_nServerProtocol;
}

void KMatchServer::SetServerProtocol( IN const int& nProtocol_ )
{
    KLocker lock( m_csServerProtocol );
    m_nServerProtocol = nProtocol_;
}

void KMatchServer::DumpInfo()
{
    // 서버정보 로그로 출력
    DumpServerList();
}

void KMatchServer::SetSID( int nSID_ )
{
    SetServerID( nSID_ );
}

bool KMatchServer::LoadScript()
{
    _JIF( SiKMatchManager()->LoadScript(),      return false );
    return true;
}
