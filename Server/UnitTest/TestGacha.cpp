#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#include <dbg/dbg.hpp>
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

FILE_NAME_FOR_LOG

void TestGacha()
{
    dbg::cout << "Client Unit Test" << END_LOG;

    KIniReader iniReader( L".\\Logger.ini" );
    //CIniReader iniReader( ".\Logger.ini" );
    std::wstring        m_strIniFilePath;

    //wchar_t buff[MAX_PATH] = {0};
    //::GetCurrentDirectoryW( MAX_PATH, buff );
    //m_strIniFilePath = buff;
    m_strIniFilePath += L".\\";
    m_strIniFilePath += L"Logger.ini";

    int test = ::GetPrivateProfileIntW( L"Login", L"Gacha", 0, m_strIniFilePath.c_str() );

    dbg::cout << L"Number from INI: "<< test;


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
        kReq.m_iAge             = 13;
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


        Sleep(4000);

        //KEVENT_REGIST_MISSION_REQ kReq2;
        KEVENT_GACHA_ACTION_REQ kReq2;

        //kReq2.m_dwReqItemID  = 77720; // 77710
        kReq2.m_ItemID = iniReader.ReadInteger( L"Login", L"Gacha", 0);
        //kReq2.m_dwGoodsUID = 10639;
        spEvent->m_usEventID = KUserEvent::EVENT_GACHA_ACTION_REQ;
        spEvent->Serialize( kReq2 );

        //pkUserProxy->SendID( KUserEvent::EVENT_REGIST_MISSION_REQ );
        for( int i = 0 ; i < 1000 ; i++ )
        {
            pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_ACTION_REQ, kReq2 );
            Sleep(300);
        }
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