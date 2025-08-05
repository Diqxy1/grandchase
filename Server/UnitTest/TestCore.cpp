#include <boost/test/unit_test.hpp>

#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

void TestSmartPointer()
{
    //{       
    //    NiRefObject* p = new NiRefObject;
    //    NiRefObjectPtr sp1(p);

    //    BOOST_CHECK_EQUAL( sp1->GetRefCount(), 1 );
    //    {
    //        NiRefObjectPtr sp2 = p;
    //        BOOST_CHECK_EQUAL( sp2->GetRefCount(), 2 );

    //        sp2 = sp1;

    //        BOOST_CHECK_EQUAL( sp2->GetRefCount(), 2 );
    //    }
    //    BOOST_CHECK_EQUAL( p->GetRefCount(), 1 );
    //    BOOST_CHECK_EQUAL( sp1->GetRefCount(), 1 );
    //}
    //// 파괴 되었는지 확인
    //BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 0 );

    //{
    //    NiRefObject* p1 = new NiRefObject;
    //    NiRefObject* p2 = new NiRefObject;

    //    NiRefObjectPtr  sp1(p1);
    //    NiRefObjectPtr  sp2(p2);

    //    BOOST_CHECK_EQUAL( sp1->GetRefCount(), 1 );
    //    BOOST_CHECK_EQUAL( sp2->GetRefCount(), 1 );
    //    BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 2 );

    //    sp1 = sp2;      // p2 ref++

    //    BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 1 );  // 위의 assign에 의해 p1이 파괴 됨
    //    BOOST_CHECK_EQUAL( sp1->GetRefCount(), 2 );
    //    BOOST_CHECK_EQUAL( sp2->GetRefCount(), 2 );
    //    
    //    sp2 = 0;        // p2 ref--
    //    
    //    BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 1 );
    //    
    //    sp1 = 0;        // p2 ref--

    //    BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 0 );  // 위의 assign에 의해 p2가 파괴 됨
    //}
    //// 파괴 되었는지 확인
    //BOOST_CHECK_EQUAL( NiRefObject::GetTotalObjectCount(), 0 );
}

void TestLogLevel()
{
    //std::cout << "default log level is 2" << std::lbreak;

    //std::cout << "std::cout : log level 0 " << std::endl;
    //std::cerr << "std::cerr : log level 0 " << std::endl;
    //std::cwarn << "std::cwran : log level 1 " << std::endl;
    //std::clog << "std::cout : log level 2 " << std::endl;

    //std::logfile::SetLogLevel( 1 );

    //std::cout << "std::cout : log level 0 " << std::endl;
    //std::cerr << "std::cerr : log level 0 " << std::endl;
    //std::cwarn << "std::cwran : log level 1 " << std::endl;
    //std::clog << "std::cout : log level 2 " << std::endl;

    //std::logfile::SetLogLevel( 0 );

    //std::cout << "std::cout : log level 0 " << std::endl;
    //std::cerr << "std::cerr : log level 0 " << std::endl;
    //std::cwarn << "std::cwran : log level 1 " << std::endl;
    //std::clog << "std::cout : log level 2 " << std::endl;

    //std::logfile::SetLogLevel( -1 );

    //std::cout << "std::cout : log level 0 " << std::endl;
    //std::cerr << "std::cerr : log level 0 " << std::endl;
    //std::cwarn << "std::cwran : log level 1 " << std::endl;
    //std::clog << "std::cout : log level 2 " << std::endl;

    //std::logfile::SetLogLevel( 2 );

    //std::cout << "std::cout : log level 0 " << std::endl;
    //std::cerr << "std::cerr : log level 0 " << std::endl;
    //std::cwarn << "std::cwran : log level 1 " << std::endl;
    //std::clog << "std::cout : log level 2 " << std::endl;

    int     a = 1234;
    wchar_t*   b = L"dummy string";

    START_LOG( clog, "make log using macro" )
        << BUILD_LOG( a )
        << BUILD_LOG( b )
        << END_LOG;

    std::wstring m_strName = L"테스트용 m_strName 변수";

    //START_LOG_WITH_NAME( cout );
    //START_LOG_WITH_NAME( cout2 );
    //START_LOG_WITH_NAME( cerr );
    //START_LOG_WITH_NAME( cerr2 );
    //START_LOG_WITH_NAME( cwarn );
    //START_LOG_WITH_NAME( cwarn2 );
    //START_LOG_WITH_NAME( clog );
    //START_LOG_WITH_NAME( clog2 );
}