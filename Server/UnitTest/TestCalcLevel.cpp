#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)


//#include "odbc/odbc.h"
//#include "IsCorrect.h"

void TestCalcLevel()
{
    //KODBC kOdbc;
    //BOOST_CHECK( kOdbc.DriverConnect( L"odbc_internal.dsn" ) );

    //BOOST_CHECK_EQUAL( kOdbc.BeginFetch(), false );

    //// query
    //int iResult = 0;
    //for( int i = 100; i < 9400; i++)
    //{
    //    BOOST_CHECK( kOdbc.Query( L"SELECT dbo.fnTransformLevel(%d)", i ) );

    //    BOOST_CHECK( kOdbc.BeginFetch() );
    //    kOdbc >> iResult;
    //    kOdbc.EndFetch();

    //    BOOST_CHECK_EQUAL( iResult, static_cast<int>( Exp_2_Level( i ) ) );
    //}
}
