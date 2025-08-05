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

void TestRoom()
{
    //std::cout << "Client Unit Test" << END_LOG;

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


        Sleep(4000);

        // Enter Channel
        DWORD dwChannelID = 6;

        pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, dwChannelID );

        Sleep(4000);


        //KEVENT_REGIST_MISSION_REQ kReq2;
        //KEVENT_KAIRO_DATA kReq2;
        KEVENT_CREATE_ROOM_REQ kCreateRoomReq;

        KRoomInfo kRoomInfo;
        kRoomInfo.m_strRoomName = L"TestGuildRoom";
        kRoomInfo.m_bPublic = true;
        kRoomInfo.m_strRoomPasswd = L"";
        kRoomInfo.m_cGameCategory = 0; // 대전
        kRoomInfo.m_iGameMode = 28; // 
        kRoomInfo.m_iSubGameMode = 1; // GC_SGM_NO_ITEM
        kRoomInfo.m_iMapID = 0;
        kRoomInfo.m_iP2PVersion = CLIENT_P2P_VER;
        //kRoomInfo.m_cDifficulty = 0;
        kRoomInfo.m_usMaxUsers = 6;
        kRoomInfo.m_bRandomableMap = false;
        kRoomInfo.m_cRoutingMethod = DRR_TURN_RANDOM;
        kRoomInfo.m_nMonsterID = MON_INVALID;
        kRoomInfo.m_nMonsterCount = 0;
        kRoomInfo.m_bDeathMatchBalancing = false;
        kRoomInfo.m_bDeathMatchIntrudable = false;
        kRoomInfo.m_nDeathKillCount = 20;
        kRoomInfo.m_nDeathMatchTime = 300;
        kRoomInfo.m_bGuild = true;

        kCreateRoomReq.m_kRoomInfo = kRoomInfo;

        //kReq2.m_dwReqItemID  = 77720; // 77710
        //kReq2.m_dwReqItemID = iniReader.ReadInteger( L"Login", L"Kairo", 0);
        //kReq2.m_dwGoodsUID = 10639;
        spEvent->m_usEventID = KUserEvent::EVENT_KAIRO_REQ;
        spEvent->Serialize( kCreateRoomReq );

        //pkUserProxy->SendID( KUserEvent::EVENT_REGIST_MISSION_REQ );
        //for( int i = 0 ; i < 1000 ; i++ )
        {
            pkUserProxy->SendPacket( KUserEvent::EVENT_CREATE_ROOM_REQ, kCreateRoomReq );
            Sleep(10);
        }
        Sleep(1000);

        // start game
        KStartGameReq kStartGameReq;
        kStartGameReq.m_bGuild = true;
        kStartGameReq.m_dwUserUID = 13350; // woosh's uid

        pkUserProxy->SendPacket( KUserEvent::EVENT_START_GAME_REQ, kStartGameReq );

        Sleep(3000);

        // room ack 를 처리할 수 있나요. -> userproxy 에 추가. 테스트 해야함.
        //
        KEndGameReq kEndGameReq;

        kEndGameReq.m_bGuild = true;
        kEndGameReq.m_dwElapsedSec = 60000;
        kEndGameReq.m_dwUserUID = 13350;
        kEndGameReq.m_usRoomID = pkUserProxy->m_usRoomID;
        
        {
            KGameResultIn kGameResultIn;
            kGameResultIn.m_bWin = true;
            kGameResultIn.m_dwUID = 13350;
            kGameResultIn.m_iNumDies = 1;
            kGameResultIn.m_iNumKills = 5;
            kGameResultIn.m_strLogin = L"woosh";

            KCharacterExpInfo kChar;
            kChar.m_cCharType = 0;
            kChar.m_cCharJob = 0;
            kChar.m_nBaseExp = 0;

            kGameResultIn.m_vecCharExp.push_back( kChar );

            kEndGameReq.m_vecGameResult.push_back( kGameResultIn );
        }

        
        pkUserProxy->SendPacket( KUserEvent::EVENT_END_GAME_REQ, kEndGameReq );

        // end game


        //kReq2.m_dwGoodsID  = 25620;
        //kReq2.m_dwGoodsUID = 25620;
        //spEvent->m_usEventID = KUserEvent::EVENT_REGIST_MISSION_REQ;
        //spEvent->Serialize( kReq2 );

        getchar();

        delete pkUserProxy;
        ::WSACleanup();

    }
}