// warning C4535: _set_se_translator()를 호출하려면 /EHa를 지정해야 합니다.
// 명령줄 옵션 /EHc와 /GX가 충분하지 않습니다.
#pragma warning(push, 2)    // 이런 경고가 뜨기 때문에 warning을 감싸준다.
#include <boost/test/included/unit_test_framework.hpp>
#pragma warning(pop)

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

//#include "ImportNMLink.h"     // 넥슨저팬 인증 라이브러리
//#pragma comment( lib, "CPSSOLib" )  // 넷마블 인증 라이브러리
//
//#pragma comment( lib, "../GameServer/nprotect/ggsrvlib" ) // nprotect.
//#pragma comment( lib, "../GameServer/hshield/AntiCpSvr" ) // hshield.

#include <vld.h>

void TestSmartPointer();
void TestLogLevel();
void TestClientProject();
void TestFSM();
void TestODBC();
void TestDecider();
void TestCalcLevel();
//void TestRankingPoint();
void TestCalcGrade(); 
void TestActorManager();
void Curse();
void TestIni();
void TestLoginServer();
void TestTCPRelay();
//void TestBitStream();
//void testModeLevel();
void TestMessenger();
void TestGpExp();
void TestGenericLog();
void TestMission();
void TestNetmarbleAuth();
void TestCalendar();
void TestODBC2();
void TestEnchant();
void TestKairo();
void TestNetmarbleExpAuth();
void TestGacha();
void TestFTP();
void TestRoom();
void TestGetUser();
void TestGuild_ChangeName();
void TestItemSocket();
void TestGawibawibo();
void TestSocks();
//void TestOneaday();
void TestPlantTree();
void TestEclipsePlot();

void TestDepot();
void TestSkillTree();

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* CoreTestSuite = BOOST_TEST_SUITE( "Core Tests" );
	
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestSmartPointer ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestLogLevel ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestODBC ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestFSM ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestDecider ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestRankingPoint ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestCalcLevel ) ); //050808. microcat. 테스트 완료
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestCalcGrade ) ); //050812. microcat. 테스트 완료
    CoreTestSuite->add( BOOST_TEST_CASE( &TestLoginServer ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestClientProject ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &Curse ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestActorManager ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestIni ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestTCPRelay ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestBitStream ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &testModeLevel ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestMessenger ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGpExp ) ); // exp gp 테스트
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGenericLog ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestMission ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestNetmarbleAuth ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestCalendar ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestODBC2 ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestEnchant ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestKairo ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestNetmarbleExpAuth ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGacha ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestFTP ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestRoom ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGetUser ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGuild_ChangeName ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestItemSocket ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestGawibawibo ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestSocks ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestOneaday ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestPlantTree ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestEclipsePlot ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestDepot ) );
    //CoreTestSuite->add( BOOST_TEST_CASE( &TestSkillTree ) );

    return CoreTestSuite;
}
