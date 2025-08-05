#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


//#include "odbc/odbc.h"
//#include "IsCorrect.h"
//#include "GCEnum.h"

void TestCalcGrade()
{
    //KODBC kOdbc;
    //BOOST_CHECK( kOdbc.DriverConnect( L"odbc_internal.dsn" ) );

    //BOOST_CHECK_EQUAL( kOdbc.BeginFetch(), false );

    //// query
    //int iResultGrade = 0;
    //int iResultGradePoint = 0;
    //for( int i = 100; i < 60000000; i++)
    //{
    //    BOOST_CHECK( kOdbc.Query( L"SELECT dbo.fnTransformGrade(%d, %d, %d), dbo.fnTransformGradePoint(%d, %d, %d)", i, i, i, i, i, i ) );

    //    BOOST_CHECK( kOdbc.BeginFetch() );
    //    kOdbc >> iResultGrade >> iResultGradePoint;
    //    kOdbc.EndFetch();

    //    std::vector<DWORD> vecExp;
    //    for(int j = 0; j < GC_CHAR_NUM; j++)  vecExp.push_back( i );
    //    BOOST_CHECK_EQUAL( iResultGrade, static_cast<int>( GetGradeKor( vecExp ) ) );
    //    BOOST_CHECK_EQUAL( iResultGradePoint, static_cast<int>( GetGradePoint( vecExp ) ) );
    //}
}
