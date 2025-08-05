#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include "MsgRouter.h"
#include "MRSimLayer.h"
#include "MRNetLayer.h"
#include "MRDBLayer.h"
#include "MREvent.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMsgRouter, KBaseServer );
ImplementSingleton( KMsgRouter );

ImplToStringW( KMsgRouter )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

KMsgRouter::KMsgRouter(void)
{
}

KMsgRouter::~KMsgRouter(void)
{
}

ImplOstmOperatorW2A( KMsgRouter );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMsgRouter::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get())
    [
        luabind::def( "GetServer", KMsgRouter::GetInstance ),
        luabind::class_<KMsgRouter, KBaseServer>( "KMsgRouter" )
            .def( luabind::tostring(luabind::self) )
    ];
}

bool KMsgRouter::Init( const wchar_t* szFileName_ )
{
    m_pkNetLayer    = SiKMRNetLayer();
    m_pkSimLayer    = SiKMRSimLayer();
    m_pkDBLayer     = SiKMRDBLayer();
    
    _JIF( KBaseServer::Init( szFileName_ ), false );

    
    // db 데이터 초기작업. (서버 이름 얻기, 동접수 0 초기화)
    // note : db handler와 protocol ver 설정이 마무리 된 후에 호출해야 한다.
    // (dbthread 가동 전이라 하더라도 미리 queueing되었다가 호출된다. but 지금은 thread 가동 된 후임)
    KDB_EVENT_SERVER_INFO_REQ kPacket;
    kPacket.m_nProtocolVer  = m_nProtocolVer;
    kPacket.m_strIP         = NetCommon::GetLocalIPW();
    kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
    kPacket.m_mapChannellingUserNum[0] = m_pkSimLayer->m_kActorManager.GetCount();
    kPacket.m_bFirstDBWork  = true;
    kPacket.m_hSyncEvent    = ::CreateEvent( NULL, false, false, NULL );

    m_pkDBLayer->QueueingEvent( KMREvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

    switch( ::WaitForSingleObject( kPacket.m_hSyncEvent, 3000 ) ) {
    case WAIT_OBJECT_0:
        {
            KLocker lock( m_csServerName );

            START_LOG( cerr, L"** Server Starting Configuration **" << dbg::endl
                << L" Server Name : " << m_strServerName ) << END_LOG;
        }
    	break;
    case WAIT_TIMEOUT:
        START_LOG( cerr, L"Getting Server Info from DB failed." ) << END_LOG;
        CloseHandle( kPacket.m_hSyncEvent );
        return false;
        break;
    default:
        START_LOG( cerr, L"Unexpected Return value from ::WaitForSingleObject()." ) << END_LOG;
        CloseHandle( kPacket.m_hSyncEvent );
        return false;
    }

    CloseHandle( kPacket.m_hSyncEvent );

    SiKMRSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;
    return true;
}

void KMsgRouter::Run( bool bRunInterpreter_ )
{
    KBaseServer::Run( bRunInterpreter_ ); // lua interpreter 시작

    //// 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    //SiKGSSimLayer()->DestroyAllUsers();
    // ^ 이 부분에서 처리하면 Test 프로젝트에서 초기화 이후 바로 실행된다.
}

void KMsgRouter::ShutDown()
{
    // 051012. 서버를 종료할 때 DB의 동접수치를 0으로 내려준다.
    KDB_EVENT_SERVER_INFO_REQ kPacket;
    kPacket.m_nProtocolVer  = -1;
    kPacket.m_strIP         = NetCommon::GetLocalIPW();
    kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
    kPacket.m_mapChannellingUserNum[0] = 0;
    kPacket.m_bFirstDBWork  = false;
    kPacket.m_hSyncEvent    = NULL;

    SiKMRDBLayer()->QueueingEvent( KMREvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

    // 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    SiKMRSimLayer()->DestroyAllUsers();

    KBaseServer::ShutDown();

    KMRSimLayer::ReleaseInstance();
    KMRNetLayer::ReleaseInstance();
    KMRDBLayer::ReleaseInstance();
}

void KMsgRouter::Tick()
{
    if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwSendServerListGap )
    {
        KDB_EVENT_SERVER_INFO_REQ kPacket;
        kPacket.m_strIP         = NetCommon::GetLocalIPW();
        kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
        kPacket.m_mapChannellingUserNum[0] = m_pkSimLayer->m_kActorManager.GetCount();
        kPacket.m_nProtocolVer  = m_nProtocolVer;
        kPacket.m_bFirstDBWork  = false;
        kPacket.m_hSyncEvent    = NULL;

        SiKMRDBLayer()->QueueingEvent( KMREvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

        m_dwServerInfoUpdateTick = ::GetTickCount();
    }

}