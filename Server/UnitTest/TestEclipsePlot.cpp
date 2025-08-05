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

#include "etc/IniReader.h"

//FILE_NAME_FOR_LOG

void TestEclipsePlot()
{
    ////std::cout << "Client Unit Test" << END_LOG;

    KIniReader iniReader( L".\\Logger.ini" );
    //CIniReader iniReader( ".\Logger.ini" );
    std::wstring        m_strIniFilePath;

    //wchar_t buff[MAX_PATH] = {0};
    //::GetCurrentDirectoryW( MAX_PATH, buff );
    //m_strIniFilePath = buff;
    m_strIniFilePath += L".\\";
    m_strIniFilePath += L"Logger.ini";

    int test = ::GetPrivateProfileIntW( L"Login", L"Kairo", 0, m_strIniFilePath.c_str() );

    std::cout << L"Number from INI: "<< test;

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
        std::wstring temp = iniReader.ReadString( L"Login", L"Name", L"");
        std::wcout << temp.c_str();

        KEVENT_VERIFY_ACCOUNT_REQ kReq;
        //kReq.m_strLogin         = "grandchase5";
        kReq.m_strLogin         = KncUtil::toNarrowString( iniReader.ReadString( L"Login", L"Name", L"") );
        kReq.m_strPasswd        = KncUtil::toNarrowString( iniReader.ReadString( L"Login", L"PW", L"") ); //"12345678";
        kReq.m_bMale            = 0;
        kReq.m_iVersion         = iniReader.ReadInteger( L"Login", L"Protocol Version", 0 );
        kReq.m_dwChecksum       = 0;
        kReq.m_nConnectType     = KEVENT_VERIFY_ACCOUNT_REQ::ECT_FIRST_CONNECTION; // 초기접속 | 서버이동의 경우를 확인하기 위한 변수.
        kReq.m_iP2PVersion      = CLIENT_P2P_VER;
        kReq.m_dwAuthType		= iniReader.ReadInteger( L"Login", L"AuthType", 0 );;
        //kReq.m_dwAuthTick       = 7978625; // 7 = 102500937     , 4 = 102993890
        kReq.m_dwAuthTick       = iniReader.ReadInteger(L"Login", L"AuthTick", 0);

        kReq.m_kStatClientInfo.m_bjoystick = false;
        kReq.m_kStatClientInfo.m_iMainMemory = 1024;
        kReq.m_kStatClientInfo.m_iVMemory = 0;
        kReq.m_kStatClientInfo.m_strCPU = L"0";
        kReq.m_kStatClientInfo.m_strDXversion = L"0";
        kReq.m_kStatClientInfo.m_strGraphicChip =L"0";
        KUserProxy *pkUserProxy = new KUserProxy;
        pkUserProxy->SetClassID( 1 );
        pkUserProxy->Init( false );

        KEventPtr spEvent( new KUserEvent );
        spEvent->m_usEventID = KUserEvent::EVENT_VERIFY_ACCOUNT_REQ;
        spEvent->Serialize( kReq );

        // verify account ack를 받기위해 대기하는 동안 Tick이 호출되지 않으므로, timeout이 발생한다.
        KServerInfo kServerInfo;
        kServerInfo.m_strIP = KncUtil::toNarrowString( iniReader.ReadString( L"Login", L"ServerIP", L"") );//"116.120.238.25";
        //kServerInfo.m_usPort = SiKGSNetLayer()->m_usPortMaster;
        kServerInfo.m_usPort = iniReader.ReadInteger( L"Login", L"Port", 9400 );//9410;

        pkUserProxy->ConnectNAuth( kServerInfo, spEvent, 1000 );


        Sleep(8000);

        // Enter Channel
        DWORD dwChannelID = 6;

        pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, dwChannelID );

        Sleep(8000);

        //pkUserProxy->SendID( KUserEvent::EVENT_STAGE_LOAD_COMPLETE_NOT );
        
        KEVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ kPacket;
        kPacket.m_ItemID = 8880;

        while ( 1 )
        {
            pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, dwChannelID );

            Sleep(5000);

            pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_PLOT_INFO_REQ );

            Sleep(5000);

            //pkUserProxy->SendPacket( KUserEvent::EVENT_SOCKS_COLLECT_REQ, vecSocks );

            //Sleep(2000);

            //pkUserProxy->SendID( KUserEvent::EVENT_SOCKS_MATERIAL_EXCHANGE_REQ );

            //Sleep( 2000 );
        }
        ////////////////////////////////////////////////////////////////////////////

        getchar();

        delete pkUserProxy;
        ::WSACleanup();

    }
}