#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


//#include "windows.h" // Sleep().
//
//#include "MsgServer.h"
//#include "MsgRouter.h"
//#include "MSPacket.h"
#include "MSProxy.h"
//#include "MSUser.h"
//#include "MSEvent.h"
#include "MSNetLayer.h"
//#include "MSSimLayer.h"
//#include "MRSimLayer.h"
//#include "DefaultFSM.h"

void TestMessenger()
{
    //{
    //    KBaseServer* pkMsgRouter = SiKMsgRouter();
    //    pkMsgRouter->Init( L"config_mr" );
    //    pkMsgRouter->Run( true );

    //    Sleep(200);

    //    KBaseServer* pkMsgServer = SiKMsgServer();
    //    pkMsgServer->Init( L"config_msg" );
    //    pkMsgServer->Run( true );

    //    Sleep(200);
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


    KMSProxyPtr spProxy( new KMSProxy );
    spProxy->Init( false );

    KEMS_S2_VERIFY_ACCOUNT_REQ kReq;
    kReq.m_dwUserUID        = 13350;
    kReq.m_strLogin         = L"woosh";
    //    kReq.m_strPasswd;
    kReq.m_strNickName      = L"개드립";
    kReq.m_nGuildID         = 52;
    //    kReq.m_strServerName    = L"unit test";

    KEventPtr spEvent( new KMSEvent );
    spEvent->m_usEventID = KMSEvent::EMS_S2_VERIFY_ACCOUNT_REQ;
    spEvent->Serialize( kReq );

    KServerInfo kServerInfo;
    kServerInfo.m_strIP = "192.168.130.62";
    kServerInfo.m_usPort = 9300;//SiKMSNetLayer()->m_usPortMaster;

    spProxy->ConnectNAuth( kServerInfo, spEvent, 5000 );

    //    BOOST_CHECK_EQUAL( SiKMSSimLayer()->m_kActorManager.GetCount(), 1 );
    Sleep(5000);

    //KGuildMemberList kGuildMemberReq;
    //kGuildMemberReq.m_dwUserUID = 13350;
    //kGuildMemberReq.m_GuildUID = 52;

    //spEvent->m_usEventID = KMSEvent::EMS_UPDATE_GUILD_MEMBER_LIST_REQ;
    //spEvent->Serialize( kGuildMemberReq );

    //spProxy->SendPacket( KMSEvent::EMS_UPDATE_GUILD_MEMBER_LIST_REQ, kGuildMemberReq );


    //Sleep(50000);

    //    //BOOST_CHECK_EQUAL( SiKMRSimLayer()->GetMSUserSize(), 1 );

    //    KMSUserPtr spUser;
    //    spUser = boost::static_pointer_cast<KMSUser>( SiKMSSimLayer()->m_kActorManager.GetByIndex( 0 ) );

    //    BOOST_CHECK_EQUAL( spUser->GetStateID(), KDefaultFSM::STATE_LOGINED );

    //    if( spUser->GetStateID() != KDefaultFSM::STATE_LOGINED ) return;


    //    //{ // EVENT_BUY_FOR_CRYSTAL_REQ
    //    //    START_LOG( cwarn, L"고르고스 검 200회 구입시도. 크리스탈이 없으므로 구입 실패." )
    //    //        << BUILD_LOG( spUser->GetStateIDString() );
    //    //    KEVENT_BUY_FOR_GEM_REQ kReq;
    //    //    kReq.m_dwGoodsID = 7700; //고르고스 검(200회)

    //    //    LIF( spProxy->SendPacket( KMSEvent::EVENT_BUY_FOR_CRYSTAL_REQ, kReq ) );

    //    //    Sleep( 100 );

    //    //    BOOST_CHECK_EQUAL( spUser->GetStateID(), KMSUserFSM::STATE_CHANNEL );
    //    //}

    //    spUser.reset();
    //    spProxy.reset();

    //    Sleep(1000);

    //    BOOST_CHECK_EQUAL( SiKMSSimLayer()->m_kActorManager.GetCount(), 0 );

    //    Sleep(200);

    //    BOOST_CHECK_EQUAL( SiKMRSimLayer()->GetMSUserSize(), 0 );


    //    pkMsgServer->ShutDown();

    //    Sleep(200);

    //    BOOST_CHECK_EQUAL( SiKMRSimLayer()->m_kActorManager.GetCount(), 0 );

    //    pkMsgRouter->ShutDown();

    //    KMsgServer::ReleaseInstance();
    //    KMsgRouter::ReleaseInstance();
    //}

    //KncSecurity::ReleaseSecurity();
}
