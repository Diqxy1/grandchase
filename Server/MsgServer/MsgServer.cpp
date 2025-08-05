#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include "MsgServer.h"
#include "MSSimLayer.h"
#include "MSNetLayer.h"
#include "MSDBLayer.h"
#include "MSEvent.h"
#include "SlangFilter.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMsgServer, KBaseServer );
ImplementSingleton( KMsgServer );

ImplToStringW( KMsgServer )
{
    return START_TOSTRING_PARENTW( KBaseServer )
        << TOSTRINGWb( m_bWriteLogoutLog );
}

KMsgServer::KMsgServer(void)
:m_bWriteLogoutLog(false)
{
}

KMsgServer::~KMsgServer(void)
{
}

ImplOstmOperatorW2A( KMsgServer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KMsgServer::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get())
    [
        luabind::def( "GetMsgServer", &KMsgServer::GetInstance ),
        luabind::class_<KMsgServer, KBaseServer>( "KMsgServer" )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_bWriteLogoutLog",    &KMsgServer::m_bWriteLogoutLog )
    ];
}

bool KMsgServer::Init( const wchar_t* szFileName_ )
{
    m_pkNetLayer    = SiKMSNetLayer();
    m_pkSimLayer    = SiKMSSimLayer();
    m_pkDBLayer     = SiKMSDBLayer();
    
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

    m_pkDBLayer->QueueingEvent( KMSEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

    _JIF( true == SiKSlangFilter()->LoadFilter( "Curse.txt" ), return false ); // 070723. woosh. 메신저서버를 위한 비속어 필터 로드

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

    SiKMRConnector()->ConnectAll();

    SiKMRConnector()->Begin();

    SiKMSSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;
    return true;
}

void KMsgServer::Run(  bool bRunInterpreter_ )
{
    KBaseServer::Run( bRunInterpreter_ ); // lua interpreter 시작

    //// 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    //SiKGSSimLayer()->DestroyAllUsers();
    // ^ 이 부분에서 처리하면 Test 프로젝트에서 초기화 이후 바로 실행된다.
}

void KMsgServer::ShutDown()
{
    // 051012. 서버를 종료할 때 DB의 동접수치를 0으로 내려준다.
    KDB_EVENT_SERVER_INFO_REQ kPacket;
    kPacket.m_nProtocolVer  = -1;
    kPacket.m_strIP         = NetCommon::GetLocalIPW();
    kPacket.m_usPort        = m_pkNetLayer->m_usPortMaster;
    kPacket.m_mapChannellingUserNum[0] = 0;
    kPacket.m_bFirstDBWork  = false;
    kPacket.m_hSyncEvent    = NULL;

    SiKMSDBLayer()->QueueingEvent( KMSEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );
    
    // 종료처리 하기 전, 모든 유저들의 캐싱 데이터를 업데이트 해야 한다.
    
    SiKMSSimLayer()->DestroyAllUsers(); // 070801. woosh. SiKMRConnector()->ShutDown() 과 순서 바꿈. 071226 다시 원위치
    SiKMRConnector()->ShutDown(); //스레드 종료 순서!(KBaseServer::ShutDown 과 바뀐 것 수정). 070411. woosh
    
    // MRConnector 의 tick 이 도는 중에  MRProxy를 제거하려고 해서 생기는 크래쉬로 추정
    
    KBaseServer::ShutDown();
    
    KMRConnector::ReleaseInstance();
    KMSSimLayer::ReleaseInstance();
    KMSNetLayer::ReleaseInstance();
    KMSDBLayer::ReleaseInstance();
}

void KMsgServer::Tick()
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

        SiKMSDBLayer()->QueueingEvent( KMSEvent::DB_EVENT_SERVER_INFO_REQ, NULL, 0, 0, kPacket );

        m_dwServerInfoUpdateTick = ::GetTickCount();
    }

}