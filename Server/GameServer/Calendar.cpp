#include "GSSimLayer.h"
#include ".\calendar.h"
#include <dbg/dbg.hpp>
#include <KncException.h>
#include <boost/bind.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "NetError.h"
#include "GCPoint.h"
//FILE_NAME_FOR_LOG

namespace{

    class KComp
    {
    public:
        bool operator() ( const KDailyInfo& a, const KDailyInfo& b )
        {
            if( a.m_kDate.m_sYear < b.m_kDate.m_sYear ) return true;
            else if( a.m_kDate.m_sYear  == b.m_kDate.m_sYear )
            {
                if( a.m_kDate.m_cMonth < b.m_kDate.m_cMonth ) return true;
                else if( a.m_kDate.m_cMonth == b.m_kDate.m_cMonth )
                {
                    if( a.m_kDate.m_cDay < b.m_kDate.m_cDay ) return true;
                }
            }
            return false;
        }
    };

    class KEqu
    {
    public:
        KEqu( const KDailyInfo& a ){ b = a;};
        bool operator() ( const KDailyInfo& a )
        {
            if( a.m_kDate.m_sYear  == b.m_kDate.m_sYear &&
                a.m_kDate.m_cMonth == b.m_kDate.m_cMonth &&
                a.m_kDate.m_cDay   == b.m_kDate.m_cDay )
                return true;
            return false;
        }
        KDailyInfo b;
    };
};

KCalendar::KCalendar(void)
:m_bLoaded(false),
m_nAccTime( 0 )
{
    m_tmCurrent   = CTime::GetCurrentTime();
    m_tmStartTime = CTime::GetCurrentTime(); // no use
    m_mapCalendarInfo.clear();
}

KCalendar::~KCalendar(void)
{
}

// 벡터로 넘어온 달력을 '주' 단위로 구분해서 달력의 맵 정보로 구성하는 함수
bool KCalendar::LoadDailyInfo( IN std::vector<KDailyInfo>& vecCalendarInfo_ )
{
    std::vector<KDailyInfo>::iterator vit;
    CTime tmCurrent = CTime::GetCurrentTime();

    m_mapCalendarInfo.clear(); 

    // 이번 달이 몇주차 까지 있는지 주를 빈 벡터로 구성.
    int nWeekNumInMonth = CalcWeekNumber( tmCurrent.GetYear(), tmCurrent.GetMonth() );
    for( int i = 1 ; i <= nWeekNumInMonth ; ++i )
    {
        std::vector<KDailyInfo> vecBlankWeek;
        vecBlankWeek.clear();
        m_mapCalendarInfo.insert( std::make_pair( static_cast<char>(i), vecBlankWeek ) );
    }
    KSimpleDate kFirstDay( tmCurrent.GetYear(), tmCurrent.GetMonth(), 1, tmCurrent.GetDayOfWeek()-1 ); //현재 월1일
    // DB에서 얻어온 달력 정보를 맵에 채워 넣어 준다. 하나 하나..
    for( vit = vecCalendarInfo_.begin() ; vit != vecCalendarInfo_.end() ; ++vit )
    {
        if( (vit->m_kDate < kFirstDay) ) // 이전달의 날짜, 네모->세모 변경
        { 
            //vit->m_cSealType &= OVER_MASK;
            if( (vit->m_cSealType & ST_SQU) > 0)
            {
                vit->m_cSealType = ST_TRI;
            }
        }
        if( nWeekNumInMonth >= vit->m_cWeekID )
            m_mapCalendarInfo[ vit->m_cWeekID ].push_back( *vit );
    }

    // 맵을 순회 하면서 각각의 벡터를 정렬 해준다.
    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    for( mit = m_mapCalendarInfo.begin() ; mit != m_mapCalendarInfo.end() ; ++mit )
    {
        std::sort( mit->second.begin(), mit->second.end(), KComp() );
    }

    AddNewDay( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay() );

    m_bLoaded = true;
    return true;
}

void KCalendar::GetCalendarInfo( OUT std::map<char,std::vector<KDailyInfo> >& mapCalendarInfo_ )
{
    mapCalendarInfo_.clear();
    mapCalendarInfo_ = m_mapCalendarInfo;
}

// 출첵 원하는 날에 도장 찍기
bool KCalendar::SetSealling( IN int nMonthDay_, OUT KDailyInfo& kDailyInfo_ )
{
    KSimpleDate dateToday;
    dateToday.m_sYear   = m_tmCurrent.GetYear();
    dateToday.m_cMonth  = m_tmCurrent.GetMonth();
    dateToday.m_cDay    = m_tmCurrent.GetDay();

    KSimpleDate dateReq;
    dateReq.m_sYear   = m_tmCurrent.GetYear();
    dateReq.m_cMonth  = m_tmCurrent.GetMonth();
    dateReq.m_cDay    = nMonthDay_;

    if( dateToday == dateReq && m_nAccTime < SiKGCPoint()->GetAttendTime() )
    {
        SET_ERROR( ERR_CALENDAR_00 ); //오늘 누적 플레이 시간이 60분이 되지 않았네요.
        return false;
    }
    if( nMonthDay_ == 0 )
    {
        START_LOG( cerr, L"날짜가 0으로 왔음..." )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        SET_ERROR( ERR_CALENDAR_11 );
        return false;
    }

    int nWeekNum = 0;
    nWeekNum = GetWeekNumber( dateReq.m_sYear, dateReq.m_cMonth, dateReq.m_cDay );

    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    mit = m_mapCalendarInfo.find( nWeekNum );
    if( mit == m_mapCalendarInfo.end() )
    {
        START_LOG( cerr, L"업데이트 할려는 날짜의 정보를 얻어 낼수 없음...(주차 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOG( nWeekNum )
            << BUILD_LOG( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        SET_ERROR( ERR_CALENDAR_10 );
        return false;
    }

    std::vector<KDailyInfo>::iterator vit;
    vit = std::find_if( mit->second.begin(), mit->second.end(),
        boost::bind( &KDailyInfo::m_kDate, _1) == dateReq );
    if( vit == mit->second.end() )
    {
        START_LOG( cerr, L"업데이트 할려는 날짜의 정보를 얻어 낼수 없음...(날짜 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOG( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        SET_ERROR( ERR_CALENDAR_11 );
        return false;
    }

    if( ( vit->m_cSealType & ST_SQU ) <= 0 )
    {
        START_LOG( cerr, L"출석체크 실패시 정보")
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( vit->m_cSealType )
            << BUILD_LOG( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        SET_ERROR( ERR_CALENDAR_12 );
        return false;
    }
    // 이 아래로는 성공.
    vit->m_cSealType &= CLEAR_MASK;
    vit->m_cSealType |= ST_CIR;

    kDailyInfo_ = *vit;
    SET_ERROR( NET_OK );
    return true;
}

// 해당 날
void KCalendar::UpdateSeal( IN char cToday_ )
{
    int nWeekNum = 0;
    KSimpleDate dateReq;
    dateReq.m_sYear     = static_cast<short>(m_tmCurrent.GetYear());
    dateReq.m_cMonth    = static_cast<char>(m_tmCurrent.GetMonth());
    dateReq.m_cDay      = cToday_;

    nWeekNum = GetWeekNumber( dateReq.m_sYear, dateReq.m_cMonth, dateReq.m_cDay );

    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    mit = m_mapCalendarInfo.find( nWeekNum );
    if( mit == m_mapCalendarInfo.end() )
    {
        START_LOG( cerr, L"업데이트 할려는 날짜의 정보를 얻어 낼수 없음...(주차 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        return;
    }

    std::vector<KDailyInfo>::iterator vit;
    vit = std::find_if( mit->second.begin(), mit->second.end(),
        boost::bind( &KDailyInfo::m_kDate, _1) == dateReq );
    if( vit == mit->second.end() )
    {
        START_LOG( cerr, L"업데이트 할려는 날짜의 정보를 얻어 낼수 없음...(날짜 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        return;
    }

    if( m_nAccTime < SiKGCPoint()->GetAttendTime() )
    {
        if( vit->m_cSealType > ST_TRI )
        {
            return;
        }
        vit->m_cSealType &= CLEAR_MASK;
        vit->m_cSealType |= ST_TRI;
    }
    else
    {
        if( vit->m_cSealType != ST_TRI )
        {
            return;
        }
        vit->m_cSealType &= CLEAR_MASK;
        vit->m_cSealType |= ST_SQU;
    }
}

bool KCalendar::SetWeeklyAttendance( char cWeekID_, char& cMonth_ )
{
    unsigned int nAttendCount = 0;
    std::map<char,std::vector<KDailyInfo> >::iterator mit;
    mit = m_mapCalendarInfo.find( cWeekID_ );
    if( mit == m_mapCalendarInfo.end() )
    {
        SET_ERROR( ERR_CALENDAR_01 ); //달력에 해당 주 출석 정보가 없다
        return false;
    }
    if( mit->second.size() != 7 )
    {
        SET_ERROR( ERR_CALENDAR_02 ); // 해당 주의 출첵이 주개근 조건에 못미침
        return false;
    }
    if ( !IsLoaded() )
    {
        SET_ERROR( ERR_CALENDAR_09 ); // 달력 정보가 로드되지 않았음
        return false;
    }

    KSimpleDate dateToday;
    dateToday.m_sYear   = m_tmCurrent.GetYear();
    dateToday.m_cMonth  = m_tmCurrent.GetMonth();
    dateToday.m_cDay    = m_tmCurrent.GetDay();

    cMonth_ = dateToday.m_cMonth; // 클라이언트로 보내주기 위해 해당 주의 달 세팅

    std::vector<KDailyInfo>::iterator vit;
    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    {
        if( (vit->m_cSealType & ST_STA) > 0 )
        {
            SET_ERROR( ERR_CALENDAR_08 ); // 이미 주개근 했음
            return false;
        }
        if( (vit->m_cSealType & ST_SQU) > 0 )
        {
            SET_ERROR( ERR_CALENDAR_06 ); // 네모인 날 있음
            return false;
        }
        if( vit->m_kDate == dateToday && 
            (vit->m_cSealType & ST_TRI ) > 0 )
        {
            SET_ERROR( ERR_CALENDAR_07 ); // 오늘 누적시간 한시간 미만
            return false;
        }
        ++nAttendCount;
        //개근 정보 셋팅
    }
    // 일주일 몽땅 세어서 위에 있는 기준들을 통과하면 일주일 모두 주개근으로 체크
    if( nAttendCount < 7 ) // 070425. woosh.
    {
        SET_ERROR( ERR_CALENDAR_02 ); // 주개근 조건에 못미치는 날이 있음
        return false;
    }
    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    {
        vit->m_cSealType |= ST_STA;
    }
    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        Dump();
    }
    SET_ERROR( NET_OK );
    return true;
}
// 달개근 설정
bool KCalendar::SetMonthlyAttendance()
{
    std::vector<KDailyInfo> vecMonth; 
    KSimpleDate StartDate;
    StartDate.m_sYear   = m_tmCurrent.GetYear();
    StartDate.m_cMonth  = m_tmCurrent.GetMonth();
    StartDate.m_cDay    = 1;

    KSimpleDate dateToday;
    dateToday.m_sYear   = m_tmCurrent.GetYear();
    dateToday.m_cMonth  = m_tmCurrent.GetMonth();
    dateToday.m_cDay    = m_tmCurrent.GetDay();

    unsigned int nMonthLastDay = 1;
    nMonthLastDay = GetLastDay( dateToday.m_sYear, dateToday.m_cMonth );
    unsigned int nAttendCount = 0;

    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    for( mit = m_mapCalendarInfo.begin() ; mit != m_mapCalendarInfo.end() ; ++mit )
    {
        if( mit->second.empty() ) // 달의 주 정보가 통째로 비었음
        {
            SET_ERROR( ERR_CALENDAR_01 );
            return false;
        }
        std::vector<KDailyInfo>::iterator vit; // 주 단위로 돌면서 '이번달' 날짜들 찾기
        vit = std::find_if( mit->second.begin(), mit->second.end(),
            boost::bind( &KDailyInfo::m_kDate, _1 ) >= StartDate );

        if( vit == mit->second.end() )
        {
            // 저장된 달력의 어떤 주에 이번달 1일 이후의 날짜가 없다
            SET_ERROR( ERR_CALENDAR_01 );
            return false;
        }

        vecMonth.insert( vecMonth.end(), vit, mit->second.end() );
    }

    //날 개수를 센다.
    if( vecMonth.size() < nMonthLastDay )
    {
        SET_ERROR( ERR_CALENDAR_03 ); // 출석한 날이 이번달의 전체 날보다 적어서 월개근 불가
        return false;
    }

    std::vector<KDailyInfo>::iterator vit;
    for( vit = vecMonth.begin() ; vit != vecMonth.end() ; ++vit )
    {
        if( (vit->m_cSealType & ST_HEA) > 0 ) //이미 월개근을 했나?
        {
            SET_ERROR( ERR_CALENDAR_04 );
            return false;
        }
        if( (vit->m_cSealType & ST_SQU) > 0 ) // 네모이면 않됨.
        {
            SET_ERROR( ERR_CALENDAR_06 );
            return false;
        }
        if( vit->m_kDate == dateToday && 
            (vit->m_cSealType & ST_TRI) > 0 ) //오늘 날짜가 세모이면 않됨.
        {
            SET_ERROR( ERR_CALENDAR_07 );
            return false;
        }
        ++nAttendCount; 
    }
    if( nAttendCount < nMonthLastDay )
    {
        SET_ERROR( ERR_CALENDAR_03 ); // 월개근에 못미치는 날이 있음
        return false;
    }
    for( mit = m_mapCalendarInfo.begin() ; mit != m_mapCalendarInfo.end() ; ++mit )
    {    
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            if( vit->m_kDate >= StartDate )
                vit->m_cSealType |= ST_HEA;
        }
        //월 개근 정보 셋팅
    }
    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        Dump();
    }
    return true;
}

void KCalendar::SetAccTime( int nAccTime_ )
{
    m_nAccTime = nAccTime_;
    START_LOG( clog, L"누적시간은...: " )
        << BUILD_LOG( m_strOwnerLogin )
        << BUILD_LOG( m_nAccTime ) << END_LOG;
}

void KCalendar::GetMonthInfo( IN const int nYear_, IN const int nMonth_,
                                 OUT KSimpleDate& kStartDate_, OUT KSimpleDate& kLastDate_ )
{
    using namespace boost::gregorian;

    kStartDate_.m_sYear    = nYear_;
    kStartDate_.m_cMonth   = nMonth_;
    kStartDate_.m_cDay     = 1;
    kStartDate_.m_cWeekDay = (char)GetWeekDay( nYear_, nMonth_, 1 );

    kLastDate_.m_sYear    = nYear_;
    kLastDate_.m_cMonth   = nMonth_;
    kLastDate_.m_cDay     = (char)GetLastDay( nYear_, nMonth_ );
    kLastDate_.m_cWeekDay = (char)GetWeekDay( nYear_, nMonth_, kLastDate_.m_cDay );
}
int KCalendar::GetWeekDay( IN const int nYear_, IN const int nMonth_, IN const int nDay_ )
{
    using namespace boost::gregorian;
    return static_cast<unsigned char>(date( nYear_, nMonth_, nDay_ ).day_of_week());
    //return 1;
}
// 해당 월이 몇주짜리 인지 구하는 함수
int KCalendar::CalcWeekNumber( int nYear_, int nMonth_ )
{
    int nEndMonthDay    = 30;
    int nStartWeekDay   = 1;
    int nEndWeekDay     = 1;
    int nStartWeekNum   = 1;
    int nEndWeekNum     = 5;

    using namespace boost::gregorian;
    _BEGIN_TRY_BLOCK
    nEndMonthDay    = GetLastDay( nYear_, nMonth_ );

    nStartWeekDay   = GetWeekDay( nYear_, nMonth_, 1 );
    nEndWeekDay     = GetWeekDay( nYear_, nMonth_, nEndMonthDay );

    nStartWeekNum   = gregorian_calendar::week_number( date( nYear_, nMonth_, 1 ).year_month_day() );
    nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nEndMonthDay ).year_month_day() );

    // 꽁수 2 ...-_- 코드 컴플릿2 의 디버깅 챕터가 생각난다.
    if ( nMonth_ == 1 ) {
        nStartWeekNum = 1;
        nEndWeekNum += 1;
    }
    // 꽁수....^^
    if( nMonth_ == 12 && ( nEndWeekNum < nStartWeekNum ) )
    {
        nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nEndMonthDay-7 ).year_month_day() );
        ++nEndWeekNum;
    }
    _END_TRY_BLOCK_NOINFO(L"CalcWeekNumber()");

    if( nStartWeekDay == 0 )
        ++nStartWeekNum;

    if( nEndWeekDay == 0 )
        ++nEndWeekNum;

    return nEndWeekNum - nStartWeekNum +1;
}

int KCalendar::GetWeekNumber( int nYear_, int nMonth_, int nDay_ )
{
    int nStartWeekDay   = 1;
    int nEndWeekDay     = 1;
    int nStartWeekNum   = 1;
    int nEndWeekNum     = 1;

    using namespace boost::gregorian;
    _BEGIN_TRY_BLOCK
    nStartWeekDay   = GetWeekDay( nYear_, nMonth_, 1 );
    nEndWeekDay     = GetWeekDay( nYear_, nMonth_, nDay_ );

    nStartWeekNum   = gregorian_calendar::week_number( date( nYear_, nMonth_, 1 ).year_month_day() );
    nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nDay_ ).year_month_day() );

    // 꽁수....^^ 12월 말에  nEndWeekNum이 내년초 1주에 편입되면서 생기는 문제점 해결. (지난주 주차 +1 )
    if( nMonth_ == 12 && ( nEndWeekNum < nStartWeekNum ) )
    {
        nEndWeekNum     = gregorian_calendar::week_number( date( nYear_, nMonth_, nDay_-7 ).year_month_day() );
        ++nEndWeekNum;
    }

    // ISO 8601 의 주 계산법 ( week_number ) 과 우리 달력의 차이 때문.
    // 1월 첫주가 작년의 마지막 주에 편입된 경우 예외처리.
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

    _END_TRY_BLOCK_NOINFO(L"GetWeekNumber()");

    if( nStartWeekDay == 0 ) // 주의 시작이나 끝이 일요일이면 ...
        ++nStartWeekNum;

    if( nEndWeekDay == 0 )
        ++nEndWeekNum;

    return nEndWeekNum - nStartWeekNum +1;
}

int KCalendar::GetLastDay( int nYear_, int nMonth_ )
{
    int nLastDay = 1;
    using namespace boost::gregorian;
    _BEGIN_TRY_BLOCK
    nLastDay    = gregorian_calendar::end_of_month_day( nYear_, nMonth_ );
    _END_TRY_BLOCK_NOINFO(L"GetLastDay()");

    return nLastDay;
}
void KCalendar::GetToday( KSimpleDate& kToday )
{
    kToday.m_sYear      = m_tmCurrent.GetYear();
    kToday.m_cMonth     = m_tmCurrent.GetMonth(); 
    kToday.m_cDay       = m_tmCurrent.GetDay();   
    kToday.m_cWeekDay   = m_tmCurrent.GetDayOfWeek() -1;
}

int KCalendar::Update() // 070428. woosh. 일 변경, 월 변경 순서, 방법을 개선하면 좀 더 깔끔할 듯..
{
    // 오늘 도장 갱신 
    // 날이 바뀜 체크함->달력 정보에 새로운 날 정보를 넣어주기->누적 시간 초기화 
    // 달이 바뀜 체크 -> 새로운 달력 디비에서 얻어오기
    int ret = UR_NORMAL;
    ++m_nAccTime; // 누적 시간 증가

    if( !m_bLoaded )
    {
        ret = UR_GETFAIL;
        START_LOG( clog, L"달력 정보가 로드되지 않았음" )
            << BUILD_LOG( m_nAccTime )            
            << BUILD_LOG( m_strOwnerLogin ) << END_LOG;
        return ret;
    }
    UpdateSeal( m_tmCurrent.GetDay() );

    CTime tmCurrent = CTime::GetCurrentTime();
    if( tmCurrent.GetDay() != m_tmCurrent.GetDay() )
    {
        //달력 정보에 새로운 날 정보를 넣어주기
        //누적 시간 초기화
        if( tmCurrent.GetMonth() == m_tmCurrent.GetMonth() )
        {
            if( AddNewDay( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay() ) )
            {
                ret = UR_NEXTDAY;
                m_nAccTime = 0;
                m_tmCurrent = tmCurrent;
                START_LOG( clog, L"날짜 변함.. Name : " << m_strOwnerLogin )
                    << BUILD_LOG( m_nAccTime )
                    << BUILD_LOG( tmCurrent.GetYear() )
                    << BUILD_LOG( tmCurrent.GetMonth() )
                    << BUILD_LOG( tmCurrent.GetDay() ) << END_LOG;
            }
            else
            {
                m_nAccTime = 0;
                m_tmCurrent = tmCurrent;
                START_LOG( clog, L"날짜 변경 실패.. Name : " << m_strOwnerLogin ) // -> Error ?
                    << BUILD_LOG( m_nAccTime )
                    << BUILD_LOG( tmCurrent.GetYear() )
                    << BUILD_LOG( tmCurrent.GetMonth() )
                    << BUILD_LOG( tmCurrent.GetDay() ) << END_LOG;
            }
        }
    }
    if( tmCurrent.GetMonth() != m_tmCurrent.GetMonth() )
    {
        ret = UR_RELOAD;
        m_bLoaded = false;
        m_tmCurrent = tmCurrent;
        START_LOG( clog, L"월 변경.. Name : " << m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOG( tmCurrent.GetYear() )
            << BUILD_LOG( tmCurrent.GetMonth() )
            << BUILD_LOG( tmCurrent.GetDay() )
            << BUILD_LOG( m_tmCurrent.GetYear() )
            << BUILD_LOG( m_tmCurrent.GetMonth() )
            << BUILD_LOG( m_tmCurrent.GetDay() ) << END_LOG;
    }
    return ret;
}
bool KCalendar::GetCalendarDBInfo( OUT std::vector<KDailyInfo>& vecCalendarInfo_ )
{
    vecCalendarInfo_.clear();
    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    for( mit = m_mapCalendarInfo.begin() ; mit != m_mapCalendarInfo.end() ; ++mit )
    {
        vecCalendarInfo_.insert( vecCalendarInfo_.end(), mit->second.begin(), mit->second.end() );
    }

    START_LOG( clog, L" 켈린더 정보 DB에 쓰기 위해서 정보 얻음... Name : " << m_strOwnerLogin )
        << BUILD_LOG( vecCalendarInfo_.size() ) << END_LOG;
    return true;
}

void KCalendar::SetOwnerLogin( std::wstring& strOwnerLogin_ )
{
    LIF( !strOwnerLogin_.empty() );
    START_LOG( clog, L" Register Owner Login.. Old : " << m_strOwnerLogin << L" -> New : " << strOwnerLogin_ ) << END_LOG;
    m_strOwnerLogin = strOwnerLogin_;
}

bool KCalendar::AddNewDay( IN int nYear_, IN int nMonth_, IN int nDay_ )
{
    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        Dump();
    }
    //새로 넣고자 하는 날의 주 차 정보 얻기
    //달력 맵에서 해당 주의 벡터의 끝에 입력된 날짜 삽입
    int nWeekNumber = GetWeekNumber( nYear_, nMonth_, nDay_ ); // 주차
    int nWeekDay    = GetWeekDay( nYear_, nMonth_, nDay_ );    // 요일

    KDailyInfo kDailyInfo;
    kDailyInfo.m_kDate = KSimpleDate( nYear_, nMonth_, nDay_, nWeekDay );
    kDailyInfo.m_cSealType = ST_TRI;
    kDailyInfo.m_cWeekID = nWeekNumber;

    std::map< char, std::vector<KDailyInfo> >::iterator mit;
    mit = m_mapCalendarInfo.find( nWeekNumber );

    if( mit == m_mapCalendarInfo.end() )
    {
        START_LOG( cwarn, L"넣고자 하는 날의 해당 주가 없음" ) 
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( nYear_ )
            << BUILD_LOG( nMonth_ )
            << BUILD_LOG( nDay_ )
            << BUILD_LOG( nWeekNumber )
            << BUILD_LOG( nWeekDay ) << END_LOG;
            //<< BUILD_LOGc( mit->first )
            //<< BUILD_LOG( mit->second.size() ) << END_LOG;
        return false;
    }
    if( mit->second.size() >= 7 )
    {
         START_LOG( cwarn, L"주 크기 오버됨")
             << BUILD_LOG( m_strOwnerLogin )
             << BUILD_LOG( nYear_ )
             << BUILD_LOG( nMonth_ )
             << BUILD_LOG( nDay_ )
             << BUILD_LOG( nWeekNumber )
             << BUILD_LOG( nWeekDay )
             << BUILD_LOGc( mit->first )
             << BUILD_LOG( mit->second.size() ) << END_LOG;
         return false;
    }

    KSimpleDate kFirstDayOfMonth( m_tmCurrent.GetYear(), m_tmCurrent.GetMonth(), 1, 0 );
    KSimpleDate kLastDayOfMonth( m_tmCurrent.GetYear(), m_tmCurrent.GetMonth(), 
                     GetLastDay( m_tmCurrent.GetYear(), m_tmCurrent.GetMonth() ) , 0);

    if( kDailyInfo.m_kDate < kFirstDayOfMonth || kDailyInfo.m_kDate > kLastDayOfMonth )
    {
        START_LOG( cerr, L"이번달에 해당하는 날이 아닙니다.. Name :" << m_strOwnerLogin )
            << BUILD_LOG( nMonth_ )
            << BUILD_LOG( nDay_ )
            << BUILD_LOGc( kDailyInfo.m_kDate.m_cMonth )
            << BUILD_LOGc( kDailyInfo.m_kDate.m_cDay ) << END_LOG;
        return false;
    }

    std::vector<KDailyInfo>::iterator vit;
    vit = std::find_if( mit->second.begin(), mit->second.end(),
            boost::bind( &KDailyInfo::m_kDate, _1 ) == kDailyInfo.m_kDate );

    if( vit == mit->second.end() ) // 달력에 더하고자 하는 새 날이 없으면 추가한다.
    {
        mit->second.push_back( kDailyInfo ); 
        std::sort( mit->second.begin(), mit->second.end(), KComp() );
    }
    return true;
}

bool KCalendar::CheckLuckyDay( IN char cToday_ , OUT char& cSealType_ )
{
    KSimpleDate dateReq;
    dateReq.m_sYear     = static_cast<short>(m_tmCurrent.GetYear());
    dateReq.m_cMonth    = static_cast<char>(m_tmCurrent.GetMonth());
    dateReq.m_cDay      = cToday_;

    int nWeekNum = 0;
    nWeekNum = GetWeekNumber( dateReq.m_sYear, dateReq.m_cMonth, dateReq.m_cDay );

    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    mit = m_mapCalendarInfo.find( nWeekNum );
    if( mit == m_mapCalendarInfo.end() )
    {
        START_LOG( cerr, L"행운의 날로 설정할 날짜의 정보를 얻어 낼수 없음...(주차 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        return false;
    }

    std::vector<KDailyInfo>::iterator vit;
    vit = std::find_if( mit->second.begin(), mit->second.end(),
        boost::bind( &KDailyInfo::m_kDate, _1) == dateReq );
    if( vit == mit->second.end() )
    {
        START_LOG( cerr, L"행운의 날로 설정할 날짜의 정보를 얻어 낼수 없음...(날짜 오류)" )
            << BUILD_LOG( m_strOwnerLogin )
            << BUILD_LOG( m_nAccTime )
            << BUILD_LOGc( dateReq.m_sYear )
            << BUILD_LOGc( dateReq.m_cMonth )
            << BUILD_LOGc( dateReq.m_cDay ) << END_LOG;
        return false;
    }

    if( !SiKGSSimLayer()->FindLuckyDay( dateReq ) )
    {
        START_LOG( clog, L"출첵한 날이 럭키데이 아님..." ) << END_LOG; // 오늘은 운좋은 날이 아님
        return false;
    }

    vit->m_cSealType |= ST_LUC;
    cSealType_ = vit->m_cSealType; // 현재 LUCKY_NOT 패킷이 따로 있어서, 사용되지 않을 수 있다.
    // 이 함수의 두 번째 인자를 KDailyInfo 로 해야 할까.
    return true;
}
void KCalendar::Dump()
{
    std::wstringstream stm;
    stm << std::endl << L"------ Dump Calendar ------" << std::endl;
    stm << L" User Login : " << m_strOwnerLogin << std::endl;
    stm << L" Acc Time : " << m_nAccTime << std::endl;
    stm << L" Current Time : " << (LPCTSTR)m_tmCurrent.Format( _T("%y%m%d") ) << std::endl;

    std::map<char, std::vector<KDailyInfo> >::iterator mit;
    for( mit = m_mapCalendarInfo.begin() ; mit != m_mapCalendarInfo.end() ; ++mit )
    {
        stm << static_cast<int>(mit->first) << L" : ";
        std::vector<KDailyInfo>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            CTime tm( vit->m_kDate.m_sYear, vit->m_kDate.m_cMonth, vit->m_kDate.m_cDay, 0, 0, 0 );
            stm << (LPCTSTR)tm.Format( _T("%y%m%d") ) << L" (" <<  static_cast<int>(vit->m_cSealType) << L"), ";
        }
        stm << std::endl;
    }
    START_LOG( clog, L"" << stm.str() ) << END_LOG;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}
