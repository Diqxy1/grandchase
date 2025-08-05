#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


//#include "windows.h" // Sleep().
//
//#include "TRUser.h"
//#include "TRServer.h"


void TestTCPRelay()
{
    
    //std::cout << L"....LoginServerTest...." << END_LOG;
    //{
    //    KTRServer& kTRServer = *SiKTRServer();
    //    BOOST_CHECK( kTRServer.Init( "config_TRserver" ) );
    //    kTRServer.Run();

    //    {
    //        KTRUser kTRUserProxy( true );
    //        kTRUserProxy.Init( false );

    //        BOOST_CHECK( kTRUserProxy.Connect( "127.0.0.1", 9520, 1 ) );

    //        Sleep(300);

    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 1 );
    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 1 );
    //        
    //    }

    //    
    //    Sleep(300);
    //    

    //    BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 0 );
    //    BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 0 );

    //    {
    //        KTRUser kTRUserProxy( true );
    //        kTRUserProxy.Init( false );

    //        BOOST_CHECK( kTRUserProxy.Connect( "127.0.0.1", 9520, 2 ) );

    //        Sleep(300);

    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 1 );
    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 1 );
    //        {

    //            KTRUser kTRUserProxy2( true );
    //            kTRUserProxy2.Init( false );

    //            BOOST_CHECK( kTRUserProxy2.Connect( "127.0.0.1", 9520, 3 ) );

    //            Sleep(300);

    //            BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 2 );
    //            BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 2 );

    //            KRelayData kRelayData;
    //            kRelayData.m_cRecvCnt = 1;
    //            kRelayData.m_vecUID.push_back( 2 ); // receiver's uid.
    //            kRelayData.m_buff.push_back( 99 );  // dummy data.

    //            kTRUserProxy2.Send( kRelayData.m_vecUID, &kRelayData.m_buff[0], kRelayData.m_buff.size() );

    //        }

    //        Sleep(300);

    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 1 );
    //        BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 1 );

    //    }

    //    Sleep(300);
    //    BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountTot(), 0 );
    //    BOOST_CHECK_EQUAL( kTRServer.m_kTRUserManager.GetCountReg(), 0 );

    //    kTRServer.ShutDown();
    //    KTRServer::ReleaseInstance();

    //}

}