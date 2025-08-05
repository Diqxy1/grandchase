#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


#include "windows.h" // Sleep().

#include "UserEvent.h"
#include "UserFSM.h"
#include "GameServer.h"
#include "ChannelManager.h"
#include "UserPacket.h"
#include "UserProxy.h"
//#include "CenterServer.h"
#include "GSSimLayer.h"
#include "FailRate.h"
#include "GCEnum.h"

/* 
시나리오.
1. 초기 접속 이후 m_mapStagePlayable 이 비이었지 낳아야 한다.
2. 초기 접속 이후 각 페어의 첫번째 요소는 셋팅되어 있어야 한다.
3. 게임플레이후 해당 모드의 플레이 이후의 인덱스는 셋팅되어 있어야 한다.
4. 게임종료후 재 접속시 이전 플레이 기록이 남아 있어야 한다.
*/
//이것을 테스트 할려면 KUser 의 m_mapStagePlayable 멤버의 접근 권한을 변경하고 테스트 해야 한다.

void testModeLevel()
{
    friend class KUser;
    dbg::cout << "Client Unit Test" << END_LOG;

    {
        

        KGameServer& KGameServer = *SiKGameServer();
        KGameServer.Init( L"config_server" );
        KGameServer.Run();

        Sleep(200);

        KUserProxy UserProxy;
        UserProxy.Init( false );
        KEVENT_VERIFY_ACCOUNT_REQ kReq;
        kReq.m_strLogin         = "TestRobot";
        kReq.m_strPasswd        = "TestRobot";
        kReq.m_bMale            = 0;
        kReq.m_iVersion         = 191;
        kReq.m_dwChecksum       = 0xabcdef;

        // verify account ack를 받기위해 대기하는 동안 Tick이 호출되지 않으므로, timeout이 발생한다.
        UserProxy.Connect( "127.0.0.1", 9400, false, KUserEvent::EVENT_VERIFY_ACCOUNT_REQ, kReq, 5000 );

        BOOST_CHECK_EQUAL( SiKGSSimLayer()->m_kActorManager.GetCount(), 1 );

        KUserPtr spUser;
        spUser = boost::static_pointer_cast<KUser>( SiKGSSimLayer()->m_kActorManager.GetByIndex( 0 ) );

        BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_CHANNELLOBBY );

        //if( spUser->GetStateID() != KUserFSM::STATE_CHANNELLOBBY ) return;

        //BOOST_CHECK_EQUAL( spUser->m_mapStagePlayable.empty(), false );



        { //ENTERCHANNEL
            KEVENT_ENTER_CHANNEL_REQ kReq;
            kReq.m_strChannelName   = L"자유마을 1";

            UserProxy.SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, kReq );
            UserProxy.Tick();
            Sleep(100);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_CHANNEL );
        }

        { //CREATEROOM
            KEVENT_CREATE_ROOM_REQ kReq;

            kReq.m_kRoomInfo.m_strRoomName  = L"ABCDEFG";
            kReq.m_kRoomInfo.m_bPublic      = false;
            kReq.m_kRoomInfo.m_strRoomPasswd= L"";
            kReq.m_kRoomInfo.m_usMaxUsers    = 6;

            UserProxy.SendPacket( KUserEvent::EVENT_CREATE_ROOM_REQ, kReq );
            UserProxy.Tick();
            Sleep(300);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_JOIN );
            BOOST_CHECK( spUser->GetRoom()->m_spHostUser == spUser );
        }

        { // CREATE_P2P_NOT
            KEVENT_CREATE_P2P_NOT kNot;
            kNot.m_bCreated = true;
            UserProxy.SendPacket( KUserEvent::EVENT_CREATE_P2P_NOT, kNot );

            UserProxy.Tick();
            Sleep(100);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_ROOM );
        }

       

        { //EVENT_START_GAME_REQ                
            KEVENT_START_GAME_REQ kReq;
            kReq.m_iMode = GC_GM_MONSTER_HUNT;  // 051103. 현재 혼자 게임플레이 가능한 유일한 게임모드.
            kReq.m_cModeLevel = 0;
            
            //kReq.m_iMode = GC_GM_QUEST3;
            UserProxy.SendPacket( KUserEvent::EVENT_START_GAME_REQ, kReq );
            UserProxy.Tick();
            Sleep(100);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_PLAYING );
        }

        Sleep( 5000 ); //5 초 간 기다린다.(루아 파일에서 수정)

        { //ENDGAME  
            KEVENT_END_GAME_REQ kReq;
            KGameResult kGameResult;
            kGameResult.m_strLogin  = L"TestRobot";
            kGameResult.m_ucCharType = 1;
            kGameResult.m_nExp = 2;
            kGameResult.m_nGamePoint = 2;
            kGameResult.m_iNumKills = 1;
            kGameResult.m_iNumDies = 0;
            kGameResult.m_cBlessingType = 0;
            kGameResult.m_iBlessing = 0;
            kGameResult.m_cGameType = 0;
            kGameResult.m_iContPoint = 0;
            kGameResult.m_iGuildId = -1;
            kGameResult.m_iGuildPoint = 0;
            kGameResult.m_bIsChangeJob = 0;
            kGameResult.m_bWin = true;
            kReq.m_vecGameResult.push_back( kGameResult );

            UserProxy.SendPacket( KUserEvent::EVENT_END_GAME_REQ, kReq );
            UserProxy.Tick();
            Sleep(100);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_ROOM );
            Sleep( 1000 );

            //std::map<char,KncBitStream>::iterator mit;
            //mit = spUser->m_mapStagePlayable.find( GC_GM_MONSTER_HUNT);
            //BOOST_CHECK( mit != spUser->m_mapStagePlayable.end() );
            //BOOST_CHECK( mit->second.Get( 0 ) );
            //BOOST_CHECK( mit->second.Get( 1 ) );
        }  
        

        { //LEAVEROOM                               
            int nReason = KFailRate::P2P_NO_ERROR;
            UserProxy.SendPacket( KUserEvent::EVENT_LEAVE_ROOM_NOT, nReason );
            UserProxy.Tick();
            Sleep(100);
            UserProxy.Tick();

            BOOST_CHECK_EQUAL( spUser->GetStateID(), KUserFSM::STATE_CHANNEL );
        }


        spUser.reset();

        KUserProxy UserProxy2;
        UserProxy2.Init( false );
        KEVENT_VERIFY_ACCOUNT_REQ kReq2;
        kReq2.m_strLogin         = "TestRobot";
        kReq2.m_strPasswd        = "TestRobot";
        kReq2.m_bMale            = 0;
        kReq2.m_iVersion         = 191;
        kReq2.m_dwChecksum       = 0xabcdef;

        // verify account ack를 받기위해 대기하는 동안 Tick이 호출되지 않으므로, timeout이 발생한다.
        UserProxy2.Connect( "127.0.0.1", 9400, false, KUserEvent::EVENT_VERIFY_ACCOUNT_REQ, kReq2, 5000 );

        BOOST_CHECK_EQUAL( SiKGSSimLayer()->m_kActorManager.GetCount(), 1 );

        KUserPtr spUser2;
        spUser2 = boost::static_pointer_cast<KUser>( SiKGSSimLayer()->m_kActorManager.GetByIndex( 0 ) );

        BOOST_CHECK_EQUAL( spUser2->GetStateID(), KUserFSM::STATE_CHANNELLOBBY );

        //BOOST_CHECK_EQUAL( spUser2->m_mapStagePlayable.empty(), false );
        //std::map< char, KncBitStream >::iterator mit;
        //mit = spUser2->m_mapStagePlayable.find( GC_GM_MONSTER_HUNT );
        //BOOST_CHECK( mit != spUser2->m_mapStagePlayable.end() );
        //BOOST_CHECK( mit->second.Get( 0 ) );
        //BOOST_CHECK( mit->second.Get( 1 ) );



        spUser2.reset();

        KGameServer.ShutDown();

        Sleep(200);

        //kCenterServer.ShutDown();

        //KCenterServer::ReleaseInstance();
        KGameServer::ReleaseInstance();
    }
}