#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


//#include "windows.h" // Sleep().

#include "UserEvent.h"
#include "UserFSM.h"
#include "GameServer.h"
//#include "ChannelManager.h"
#include "UserPacket.h"
#include "UserProxy.h"
#include "CenterServer.h"
#include "GSSimLayer.h"
#include "FailRate.h"
#include "GCEnum.h"
#include "GSNetLayer.h"
#include "User.h"
//FILE_NAME_FOR_LOG

// 060411. kkurrung. 지금 NUser를 통해서 Center에 유저가 접속하지 않으면
// Connected Map에 해당 유저가 없어서 강퇴 조취시키므로
// NUser넣기 전까지는 Center Server를 동작시키면 않된다.
// 어찌되었든지 
void TestClientProject()
{
////std::cout << "Client Unit Test" << END_LOG;
{

    Sleep(200);

    KGameServer& KGameServer = *SiKGameServer();
    KGameServer.Init( L"config_server" );
    KGameServer.Run( true );

    Sleep(200); 

    KUserProxy UserProxy;
    UserProxy.Init( false );
    KEVENT_VERIFY_ACCOUNT_REQ kReq;
    kReq.m_strLogin         = "shallom8";
    kReq.m_strPasswd        = "NM0066211740";
    kReq.m_bMale            = 0;
    kReq.m_iVersion         = 243;
    kReq.m_dwChecksum       = 0x00000000;
    kReq.m_nConnectType     = KEVENT_VERIFY_ACCOUNT_REQ::ECT_FIRST_CONNECTION; // 초기접속 | 서버이동의 경우를 확인하기 위한 변수.
    kReq.m_iAge             = 13;
    kReq.m_dwAuthTick       = 156727703;
    kReq.m_dwAuthType       = KSimLayer::AT_NETMARBLE;

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = KUserEvent::EVENT_VERIFY_ACCOUNT_REQ;
    spEvent->Serialize( kReq );

    // verify account ack를 받기위해 대기하는 동안 Tick이 호출되지 않으므로, timeout이 발생한다.
    KServerInfo kServerInfo;
    kServerInfo.m_strIP = "127.0.0.1";
    kServerInfo.m_usPort = SiKGSNetLayer()->m_usPortMaster;

    UserProxy.ConnectNAuth( kServerInfo, spEvent, 5000 );

    {
        BOOST_CHECK_EQUAL( SiKGSSimLayer()->m_kActorManager.GetCount(), 1 );

        KUserPtr spUser;
        spUser = boost::static_pointer_cast<KUser>( SiKGSSimLayer()->m_kActorManager.GetByIndex( 0 ) );

        BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_CHANNELLOBBY );
    }
    
    KGameServer.ShutDown();
    KGameServer::ReleaseInstance();
    }
}