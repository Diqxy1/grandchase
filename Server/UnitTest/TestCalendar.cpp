#include <dbg/dbg.hpp>
#include "Log4.h"

#include <boost/test/unit_test.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
int GetWeekNumber( int nYear_, int nMonth_, int nDay_ )
{
    int nStartWeekDay   = 1;
    int nEndWeekDay     = 1;
    int nStartWeekNum   = 1;
    int nEndWeekNum     = 1;

    using namespace boost::gregorian;
    
    nStartWeekDay   = date( nYear_, nMonth_, 1 ).day_of_week();
    nEndWeekDay     = date( nYear_, nMonth_, nDay_ ).day_of_week();

    nStartWeekNum   = gregorian_calendar::week_number( date( nYear_, nMonth_, 1 ).year_month_day() );
    nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nDay_ ).year_month_day() );

    // 꽁수....^^ 12월 말에  nEndWeekNum이 내년초 1주에 편입되면서 생기는 문제점 해결. (지난주 주차 +1 )
    if( nMonth_ == 12 && (nEndWeekNum < nStartWeekNum) )
    {
        nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nDay_-7 ).year_month_day() );
        ++nEndWeekNum;
    }

    // ISO 8601 의 주 계산법 ( week_number ) 과 우리 달력의 차이 때문.
    // 1월 첫주가 작년의 마지막 주에 편입된 경우 예외처리.
    //if ( nMonth_ == 1 ) { // 1월인 경우
    //    nStartWeekNum = 1; 

    //    if ( nEndWeekNum == 53 || nEndWeekNum == 52 ) { // nEndWeekNum 이 전해의 마지막주에 편입된 경우 해결
    //        nEndWeekNum = 1;
    //    }
    //    else {
    //        if ( 1 != gregorian_calendar::week_number( date( nYear_, 1, 1 ).year_month_day() ) ) {
    //            nEndWeekNum += 1;
    //        }
    //    }
    //}
    //else { // 1월이 아닌 경우
    //    if( 1 != gregorian_calendar::week_number( date( nYear_, 1, 1 ).year_month_day() ) ) { //해당 년도의 1월 첫째주가 1이 아닌 경우.
    //        nStartWeekNum += 1;
    //        nEndWeekNum += 1;
    //    }
    //}

    if( 1 != gregorian_calendar::week_number( date( nYear_, 1, 1 ).year_month_day() ) ) { //해당 년도의 1월 첫째주가 1이 아닌 경우.

        if ( nMonth_ == 1 ) { // 1월 
            nStartWeekNum = 1;

            if ( nEndWeekNum == 52 || nEndWeekNum == 53 ) {
                nEndWeekNum = 1;
            }
            else {
                ++nEndWeekNum;
            }
        }
        else { // 2~ 12월
            ++nStartWeekNum;
            ++nEndWeekNum;
        }    
    }

    // sunday == 0
    if( nStartWeekDay == 0 ) // 주의 시작이나 끝이 일요일이면 ...
        ++nStartWeekNum;

    if( nEndWeekDay == 0 )
        ++nEndWeekNum;

    return nEndWeekNum - nStartWeekNum +1;
}

int CalcWeekNumber( int nYear_, int nMonth_ )
{
    int nEndMonthDay    = 30;
    int nStartWeekDay   = 1;
    int nEndWeekDay     = 1;
    int nStartWeekNum   = 1;
    int nEndWeekNum     = 5;

    using namespace boost::gregorian;

    nEndMonthDay    = gregorian_calendar::end_of_month_day( nYear_, nMonth_ );

    nStartWeekDay   = date( nYear_, nMonth_, 1 ).day_of_week();
    nEndWeekDay     = date( nYear_, nMonth_, nEndMonthDay ).day_of_week();

    nStartWeekNum   = gregorian_calendar::week_number( date( nYear_, nMonth_, 1 ).year_month_day() );
    nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nEndMonthDay ).year_month_day() );

    // 꽁수 2 ...-_- 코드 컴플릿2 의 디버깅 챕터가 생각난다.
    if ( nMonth_ == 1 ) {
        nStartWeekNum = 1;
        nEndWeekNum += 1;
    }
    // 꽁수....^^
    if( nEndWeekNum < nStartWeekNum )
    {
        nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nEndMonthDay-7 ).year_month_day() );
        ++nEndWeekNum;
    }

    if( nStartWeekDay == 0 )
        ++nStartWeekNum;

    if( nEndWeekDay == 0 )
        ++nEndWeekNum;

    return nEndWeekNum - nStartWeekNum +1;
}


void TestCalendar()
{
    int nWeekNum = 0;
    int i = 0;
    int mday[] = { 31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31  };
    int j = 0;
    int oldWeekNum = 0;

    for ( i = 0; i <= sizeof( mday )/sizeof(int) ; ++i ) {
        //std::clog2 << L"Month: " << i+1 << std::endl;
        for( j = 1; j <= mday[i]; ++j ) {
            nWeekNum = GetWeekNumber( 2011, i+1, j );
            
            if ( oldWeekNum != nWeekNum ) {
                oldWeekNum = nWeekNum;
                //std::clog2 << std::endl;
            }
            //std::clog2 << j << L"("<< nWeekNum << L")"<< L",";
        }
    }

    nWeekNum = GetWeekNumber( 2010,01,03 );
    std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2010,02, 28);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2010,04, 01);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2013,01, 01);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2014,01, 01);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2010,12, 31);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2011,12, 31);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2012,12, 31);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2013,12, 31);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2006,01, 01);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2007,01, 01);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2008,1, 1);
    //std::cout << nWeekNum;

    //nWeekNum = GetWeekNumber( 2009,1, 1);
    //std::cout << nWeekNum;

    nWeekNum = GetWeekNumber( 2009,12, 31);
    std::cout << nWeekNum;

    nWeekNum = GetWeekNumber( 2007,12, 31);
    std::cout << nWeekNum << std::endl;

    int nWeekCount = CalcWeekNumber( 2010, 1 );
    std::cout << nWeekCount << std::endl;

    nWeekCount = CalcWeekNumber( 2010, 2 );
    std::cout << nWeekCount << std::endl;

    nWeekCount = CalcWeekNumber( 2010, 3 );
    std::cout << nWeekCount << std::endl;

    nWeekCount = CalcWeekNumber( 2010, 4 );
    std::cout << nWeekCount << std::endl;

    nWeekCount = CalcWeekNumber( 2010, 5 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 6 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 7 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 8 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 9 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 10 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 11 );
    std::cout << nWeekCount<< std::endl;

    nWeekCount = CalcWeekNumber( 2010, 12 );
    std::cout << nWeekCount<< std::endl;


}