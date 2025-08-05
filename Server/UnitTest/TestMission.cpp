#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#include <dbg/dbg.hpp>
#include "Log4.h"

#pragma warning(pop)

#   define CLIENT_P2P_VER   (8)         // 클라이언트 P2P 버전

#include "UserProxy.h"
#include "UserEvent.h"
#include "UserPacket.h"
#include "UserFSM.h"
#include "GameServer.h"
//#include "ChannelManager.h"
#include "UserEvent.h"
#include "GSSimLayer.h"
#include "FailRate.h"
#include "GCEnum.h"
#include "socket/Session.h"

#include "windows.h" // Sleep().
////FILE_NAME_FOR_LOG

void TestMission()
{
    //std::cout << "Client Unit Test" << END_LOG;

    {
        //KBaseServer& kCenterServer = *SiKCenterServer();
        //kCenterServer.Init( L"config_center" );
        //kCenterServer.Run();

        WORD        wVer = MAKEWORD(2, 2);
        WSADATA     wsaData;

        if( ::WSAStartup(wVer, &wsaData) != 0 ) 
        {     
            ::WSACleanup();
        }

        if( wsaData.wVersion != MAKEWORD(2,2) )
        {    
            ::WSACleanup();
        }

        Sleep(200);

        //KGameServer& KGameServer = *SiKGameServer();
        //KGameServer.Init( L"config_server" );
        //KGameServer.Run( true );

        //Sleep(200); 

        KEVENT_VERIFY_ACCOUNT_REQ kReq;
        kReq.m_strLogin         = "ZzangDoL";
        kReq.m_strPasswd        = "ZzangDoL";
        kReq.m_bMale            = 0;
        kReq.m_iVersion         = 237;
        kReq.m_dwChecksum       = 0xabcdef;
        kReq.m_nConnectType     = KEVENT_VERIFY_ACCOUNT_REQ::ECT_FIRST_CONNECTION; // 초기접속 | 서버이동의 경우를 확인하기 위한 변수.
        kReq.m_iAge             = 13;
        kReq.m_iP2PVersion      = CLIENT_P2P_VER;
        kReq.m_dwAuthType		= AT_NETMARBLE;

        KUserProxy *pkUserProxy = new KUserProxy;
        pkUserProxy->SetClassID( 1 );
        pkUserProxy->Init( false );
        
        KEventPtr spEvent( new KUserEvent );
        spEvent->m_usEventID = KUserEvent::EVENT_VERIFY_ACCOUNT_REQ;
        spEvent->Serialize( kReq );

        // verify account ack를 받기위해 대기하는 동안 Tick이 호출되지 않으므로, timeout이 발생한다.
        KServerInfo kServerInfo;
        kServerInfo.m_strIP = "127.0.0.1";
        //kServerInfo.m_usPort = SiKGSNetLayer()->m_usPortMaster;
        kServerInfo.m_usPort = 9400;

        pkUserProxy->ConnectNAuth( kServerInfo, spEvent, 1000 );
       

        Sleep(3000);

        KEVENT_REGIST_MISSION_REQ kReq2;
        kReq2.m_ItemID  = 25980;
        kReq2.m_ItemUID = 10639;
        spEvent->m_usEventID = KUserEvent::EVENT_REGIST_MISSION_REQ;
        spEvent->Serialize( kReq2 );

        pkUserProxy->SendID( KUserEvent::EVENT_REGIST_MISSION_REQ );

        pkUserProxy->SendPacket( KUserEvent::EVENT_REGIST_MISSION_REQ, kReq2 );

        Sleep(1000);
                
        //kReq2.m_dwGoodsID  = 25620;
        //kReq2.m_dwGoodsUID = 25620;
        //spEvent->m_usEventID = KUserEvent::EVENT_REGIST_MISSION_REQ;
        //spEvent->Serialize( kReq2 );

        getchar();

        delete pkUserProxy;
        ::WSACleanup();

    }
}