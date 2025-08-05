
#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


#include "windows.h" // Sleep().

#include "CenterServer.h"
#include "CenterSimLayer.h"
#include "NUserProxy.h"
#include "DefaultFSM.h"
#include "etc/IniReader.h"
#include "UserEvent.h"
#include "UserPacket.h"

#include "UserProxy.h"

#include "etc/IniReader.h"

//FILE_NAME_FOR_LOG

void TestLoginServer()
{
    KIniReader iniReader( L".\\Logger.ini" );
    std::wstring        m_strIniFilePath;
    m_strIniFilePath += L".\\";
    m_strIniFilePath += L"Logger.ini";

    int test = ::GetPrivateProfileIntW( L"Login", L"Kairo", 0, m_strIniFilePath.c_str() );

    NetCommon::InitWinSock();
    {
        //std::cout << L"펀박스.." << END_LOG;
        KNUserProxy* pkNUserProxy = new KNUserProxy;
        pkNUserProxy->Init( false );

        bool bIsConnected = pkNUserProxy->Connect( "127.0.0.1", 9501, false );
            BOOST_CHECK_EQUAL( bIsConnected, true );

            Sleep(200);

        pkNUserProxy->Tick();
        pkNUserProxy->Tick();
        pkNUserProxy->Tick();
        pkNUserProxy->Tick();
        pkNUserProxy->Tick();

            Sleep(200);

        KIniReader kReader( L"./LoginInfo.ini" );
        KENU_VERIFY_ACCOUNT_REQ kReq;
        kReq.m_nProtocolVer = 2;
        kReq.m_strLogin         = KncUtil::toNarrowString( kReader.ReadString( L"Info", L"LoginID", L"funbox002" ) );
        kReq.m_strPasswd        = KncUtil::toNarrowString( kReader.ReadString( L"Info", L"PassWD", L"ca33324e395f39166f5f140577d2c39c" ) );
        kReq.m_dwAuthType       = KSimLayer::AT_CYOU;
        kReq.m_nFunBoxPostfix   = 0;
        kReq.m_nFunBoxBonus     = kReader.ReadInteger( L"Info", L"PostFix", 0 ); // fb

        LIF( pkNUserProxy->SendPacket( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, kReq, true, true ) );

        pkNUserProxy->Tick();
        delete pkNUserProxy;
        Sleep(2000);

        // gameserver 
        
        KEVENT_VERIFY_ACCOUNT_REQ kGameReq;
        kGameReq.m_strLogin         = KncUtil::toNarrowString( kReader.ReadString( L"Info", L"LoginID", L"funbox002" ) );
        kGameReq.m_strPasswd        = KncUtil::toNarrowString( kReader.ReadString( L"Info", L"PassWD", L"ca33324e395f39166f5f140577d2c39c" ) );
        kGameReq.m_dwAuthType = KSimLayer::AT_CYOU;
        kGameReq.m_iVersion         = iniReader.ReadInteger( L"Login", L"Protocol Version", 0 );
        
        KUserProxy *pkUserProxy = new KUserProxy;
        pkUserProxy->Init( false );

        KEventPtr spEvent( new KUserEvent );
        spEvent->m_usEventID = KUserEvent::EVENT_VERIFY_ACCOUNT_REQ;
        spEvent->Serialize( kGameReq );

        KServerInfo kServerInfo;
        kServerInfo.m_strIP = KncUtil::toNarrowString( iniReader.ReadString( L"Login", L"ServerIP", L"192.168.61.29") );//"116.120.238.25";
        kServerInfo.m_usPort = iniReader.ReadInteger( L"Login", L"Port", 9400 );//9410;

        pkUserProxy->ConnectNAuth( kServerInfo, spEvent, 1000 );


        // buy
        {
            KEVENT_BUY_FOR_CASH_REQ kBuyReq;
            //kBuyReq.m_dwBuyerUID = kReader.ReadInteger( L"Info", L"LoginUID", 74 );            
            kBuyReq.m_strBuyerLogin = kReader.ReadString( L"Info", L"LoginID", L"funbox002" );
            kBuyReq.m_kBuyInfo.m_ItemID = kReader.ReadInteger( L"Info", L"ItemID", 8880 );
            kBuyReq.m_kBuyInfo.m_nPeriod = -1;
            kBuyReq.m_kBuyInfo.m_nCount = 1;
            kBuyReq.m_cashInfo.m_dwDecreaseCashPoint = kReader.ReadInteger( L"Info", L"Price", 0 );

            //KUserProxy *pkUserProxy = new KUserProxy;
            //pkUserProxy->Init( false );

        //    //KEventPtr spEvent( new KUserEvent );
        //    //spEvent->m_usEventID = KUserEvent::EVENT_BUY_FOR_CASH_REQ;
        //    //spEvent->Serialize( kGameReq );

            //LIF( pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_CASH_REQ, kBuyReq, true, true ) );
    
        }

        {
            //LIF( pkUserProxy->SendID( KUserEvent::EVENT_CYOU_USER_AGREEMENT_REQ ) );
        }

        {
            KEVENT_CYOU_CASH_CHANGE_REQ kChange;
            LIF( pkUserProxy->SendPacket( KUserEvent::EVENT_CYOU_CASH_CHANGE_REQ, kChange, true, false ) );
        }

        {

            LIF( pkUserProxy->SendID( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ ) );
        }

        Sleep(10000);

        delete pkUserProxy;
    }

    // connect to gameserver

    ::WSACleanup();

}