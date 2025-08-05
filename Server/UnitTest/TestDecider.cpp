#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

//#include "ActionDecider.h"

void TestDecider()
{
    //KActionDecider kDecider;

    //kDecider.AddAction( 0, 0, 0.2f );
    //kDecider.AddAction( 0, 1, 0.3f );
    //kDecider.AddAction( 0, 2, 0.1f );

    //kDecider.AddAction( 1, 0, 0.2f );
    //kDecider.AddAction( 1, 1, 0.3f );
    //kDecider.AddAction( 1, 2, 0.15f );
    //kDecider.AddAction( 1, 3, 0.25f );
    //kDecider.AddAction( 1, 4, 0.3f );
    //kDecider.AddAction( 1, 5, 0.1f );

    //kDecider.AddAction( 2, 0, 0.09f );
    //kDecider.AddAction( 2, 1, 0.17f );
    //kDecider.AddAction( 2, 2, 0.115f );
    //kDecider.AddAction( 2, 5, 0.7f );

    //float fRoulette;
    //int iAction;
    //
    //fRoulette = 0.9f;
    //iAction = kDecider.MakeDecision( 0, fRoulette );
    //START_LOG( clog, "아무 변화 없음 테스트." )
    //    << BUILD_LOG( fRoulette )
    //    << BUILD_LOG( iAction )
    //    << std::lbreak;
    //BOOST_CHECK_EQUAL( iAction, DEFAULT_INVALID_ACTION );

    //fRoulette=0.5f;
    //iAction=kDecider.MakeDecision( 3, fRoulette );
    //START_LOG( clog, L"이상한 상태를 입력받았을 때의 액션 테스트")
    //    << BUILD_LOG( fRoulette )
    //    << BUILD_LOG( iAction )
    //    << std::lbreak;
    //BOOST_CHECK_EQUAL( iAction, DEFAULT_INVALID_ACTION );

    //for( int i=0; i<4; i++ )
    //{
    //    for( int iState=0; iState<3; iState++ )
    //    {
    //        iAction = kDecider.MakeDecision( iState );
    //        START_LOG( clog, L"룰렛에 따른 액션 테스트" )
    //            << BUILD_LOG( iState )
    //            << BUILD_LOG( iAction );
    //    }
    //}

    //std::clog << std::lbreak;
}
