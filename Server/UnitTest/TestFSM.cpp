#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

//#include "UserEvent.h"
//#include "RoomFSM.h"
//#include "Room.h"
//#include "User.h"
//#include "UserFSM.h"
//
void TestFSM()
{
    //{
    //    // room finite state machine reference
    //    // create room fsm 
    //    FSMclassPtr spRoomFSM( new KRoomFSM );
    //    BOOST_CHECK_EQUAL( spRoomFSM->GetCurrentState(), KRoomFSM::STATE_CLOSE );

    //    // input state translate


    //    // false state input
    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_FULL ), KRoomFSM::STATE_CLOSE  );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_CLOSE )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_OPEN ), KRoomFSM::STATE_WAITING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_WAITING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_FULL ), KRoomFSM::STATE_FULL );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_FULL )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_WAIT ), KRoomFSM::STATE_WAITING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_WAITING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_START_GAME ), KRoomFSM::STATE_PLAYING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_PLAYING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_END_GAME ), KRoomFSM::STATE_WAITING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_WAITING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_START_GAME ), KRoomFSM::STATE_PLAYING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_PLAYING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_WAIT ), KRoomFSM::STATE_FULL );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_FULL )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_WAIT ), KRoomFSM::STATE_WAITING );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_WAITING )  );

    //    BOOST_CHECK_EQUAL( spRoomFSM->StateTransition( KRoomFSM::INPUT_CLOSE ), KRoomFSM::STATE_CLOSE );
    //    START_LOG( clog, L" Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_CLOSE )  );

    //    // force state translate
    //    spRoomFSM->SetCurrentState( KRoomFSM::STATE_WAITING );
    //    BOOST_CHECK_EQUAL( spRoomFSM->GetCurrentState() , KRoomFSM::STATE_WAITING) ;
    //    START_LOG( clog, " Room State :" << spRoomFSM->GetStateIDString( KRoomFSM::STATE_WAITING )  );
    //    
    //}

    ////           example
    //// create room instance
    //unsigned short  usRoomID = 0;       // roomid = 0 createroom
    //KRoomPtr spkRoom( new KRoom( usRoomID ) );

    //// room fsm create, fms of init state is 'STATE_CLOSE'
    //FSMclassPtr spFSMclass( new KRoomFSM );
    //// room instance setting that fsm's 'STATE_CLOSE'
    //spkRoom->SetFSM( spFSMclass );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_CLOSE );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// create user instance
    //KUserPtr spkUser( new KUser );
    //FSMclassPtr spUserFSM( new KUserFSM );
    //spkUser->SetFSM( spUserFSM );

    //// enter user, first user is room's chief
    //KRoomInfo kRoomInfo;
    //kRoomInfo.m_bPublic     = true;
    //kRoomInfo.m_usMaxUsers  = 3;
    //kRoomInfo.m_strRoomName = _T("TestRoom");
    //kRoomInfo.m_strRoomPasswd   = _T("");

    //spkRoom->SetInfo( kRoomInfo );
    //spkRoom->Enter( *spkUser );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_WAITING );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// create user2 instance
    //KUserPtr spkUser2( new KUser );
    //spkUser2->SetFSM( spUserFSM );

    //spkRoom->Enter( *spkUser2 );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_WAITING );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// create user3 instance
    //KUserPtr spkUser3( new KUser );
    //spkUser3->SetFSM( spUserFSM );

    //spkRoom->Enter( *spkUser3 );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_FULL );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// leave room user3 
    //spkRoom->Leave( *spkUser3, KRoom::LEAVE_SELF_DECISION );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_WAITING );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //spkRoom->Enter( *spkUser3 );
    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_FULL );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// start game, end game은 패킷을 송신하는 관계로 network 없이 테스트 하기 곤란하다.

    ////// start game
    ////spkRoom->StartGame();
    ////BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_PLAYING );
    ////START_LOG( clog, " Room State :" << spkRoom->GetStateIDString() );

    ////// end game
    ////spkRoom->EndGame();
    ////BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_FULL );
    ////START_LOG( clog, " Room State :" << spkRoom->GetStateIDString() );

    //// all user leave room
    //spkRoom->Leave( *spkUser3, KRoom::LEAVE_SELF_DECISION );
    //spkRoom->Leave( *spkUser2, KRoom::LEAVE_SELF_DECISION );
    //spkRoom->Leave( *spkUser, KRoom::LEAVE_SELF_DECISION );

    //BOOST_CHECK_EQUAL( spkRoom->GetStateID(), KRoomFSM::STATE_CLOSE );
    //START_LOG( clog, L" Room State :" << spkRoom->GetStateIDString() );

    //// 060105. 릴레이 서버 주소를 셋팅할때 Netlayer 객체가 필요하다.
    //KGSNetLayer::ReleaseInstance();
}
