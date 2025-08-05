#include "GCPoint.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGCPoint );
ImplOstmOperatorW2A( KGCPoint );
NiImplementRootRTTI( KGCPoint );

ImplToStringW( KGCPoint )
{
    KLocker lock1(m_csConfig);
    KLocker lock2(m_csDaySetting);
    KLocker lock3(m_csWeekSetting);
    KLocker lock4(m_csOpenCalendarSetting);

    return START_TOSTRINGW
        << TOSTRINGWb( m_bGCPointSwitch )
        << TOSTRINGW( m_dwAccUpdateTime )
        << TOSTRINGW( m_nAttendTime )
        << TOSTRINGW( m_nGCPointRecommend )
        << TOSTRINGW( m_nGCPointDay )
        << TOSTRINGW( m_mapEventGCPointDay.size() )
        << TOSTRINGW( m_nGCPointWeek )
        << TOSTRINGW( m_mapEventGCPointWeek.size() )
        << TOSTRINGW( m_nGCPointOpenCalendar )
        << TOSTRINGW( m_mapEventGCPointOpenCalendar.size() );
}

KGCPoint::KGCPoint(void)
:m_bGCPointSwitch( true )
,m_dwAccUpdateTime( 60 * 1000 )
,m_nAttendTime( 70 )
,m_nGCPointRecommend( 0 )
,m_nGCPointDay( 100 )
,m_nGCPointWeek( 100 )
,m_nGCPointOpenCalendar( 0 )
{
    m_mapEventGCPointDay.clear();
    m_mapEventGCPointWeek.clear();
    m_mapEventGCPointOpenCalendar.clear();
}

KGCPoint::~KGCPoint(void)
{
}

bool KGCPoint::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGCPoint::_LoadScript( OUT std::string& strScript_ )
{
    int nGCPointDay = 0;
    int nGCPointWeek = 0;
    int nGCPointOpenCalendar = 0;
    std::map<KTimeKey,int> mapEventGCPointDay;
    std::map<PAIR_INT,int> mapEventGCPointWeek;
    std::map<KTimeKey,int> mapEventGCPointOpenCalendar;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitGCPoint.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadConfig( kLuaMng ), return false );
    _JIF( LoadSetting( kLuaMng, std::string("GCPointDay"), nGCPointDay, mapEventGCPointDay ), return false );
    _JIF( LoadWeekSetting( kLuaMng, nGCPointWeek, mapEventGCPointWeek ), return false );
    _JIF( LoadSetting( kLuaMng, std::string("GCPointOpenCalendar"), nGCPointOpenCalendar, mapEventGCPointOpenCalendar ), return false );

    START_LOG( cerr, L"Load Complete.. GCPoint Info script." )
        << BUILD_LOG( nGCPointDay )
        << BUILD_LOG( nGCPointWeek )
        << BUILD_LOG( nGCPointOpenCalendar )
        << BUILD_LOG( mapEventGCPointDay.size() )
        << BUILD_LOG( mapEventGCPointWeek.size() )
        << BUILD_LOG( mapEventGCPointOpenCalendar.size() ) << END_LOG;

    {
        KLocker lock( m_csDaySetting );
        m_nGCPointDay = nGCPointDay;
        m_mapEventGCPointDay.swap( mapEventGCPointDay );
    }

    {
        KLocker lock( m_csWeekSetting );
        m_nGCPointWeek = nGCPointWeek;
        m_mapEventGCPointWeek.swap( mapEventGCPointWeek );
    }

    {
        KLocker lock( m_csOpenCalendarSetting );
        m_nGCPointOpenCalendar = nGCPointOpenCalendar;
        m_mapEventGCPointOpenCalendar.swap( mapEventGCPointOpenCalendar );
    }
    return true;
}

bool KGCPoint::LoadConfig( IN KLuaManager& kLuaMng_ )
{
    bool bGCPointSwitch = true;
    DWORD dwAccUpdateTime = 60 * 1000;
    int nAttendTime = 70;
    int nGCPointRecommend = 0;

    _JIF( kLuaMng_.GetValue( "GCPointSwitch", bGCPointSwitch ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "AccUpdateTime", dwAccUpdateTime ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "AttendTime", nAttendTime ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "GCPointRecommend", nGCPointRecommend ) == S_OK, return false );

    START_LOG( clog, L"Load GCPoint Config.." )
        << BUILD_LOG( bGCPointSwitch )
        << BUILD_LOG( dwAccUpdateTime )
        << BUILD_LOG( nAttendTime )
        << BUILD_LOG( nGCPointRecommend ) << END_LOG;

    KLocker lock(m_csConfig);
    m_bGCPointSwitch = bGCPointSwitch;
    m_dwAccUpdateTime = dwAccUpdateTime;
    m_nAttendTime = nAttendTime;
    m_nGCPointRecommend = nGCPointRecommend;
    return true;
}

bool KGCPoint::LoadWeekSetting( IN KLuaManager& kLuaMng_, OUT int& nDefault_, OUT std::map<PAIR_INT,int>& mapEvent_ )
{
    nDefault_ = 0;
    mapEvent_.clear();

    _JIF( kLuaMng_.BeginTable( "GCPointWeek" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Default", nDefault_ ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "Event" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMonthID = 0;
        int nWeekID = 0;
        int nGCPoint = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nMonthID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nWeekID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nGCPoint ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        PAIR_INT prKey( nMonthID, nWeekID );
        _JIF( mapEvent_.insert( std::make_pair( prKey, nGCPoint ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load Week Setting.." )
        << BUILD_LOG( nDefault_ )
        << BUILD_LOG( mapEvent_.size() ) << END_LOG;
    return true;
}

bool KGCPoint::LoadSetting( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT int& nDefault_, OUT std::map<KTimeKey,int>& mapEvent_ )
{
    nDefault_ = 0;
    mapEvent_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Default", nDefault_ ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "Event" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KTimeKey kIndex;
        int nGCPoint = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( LoadTime( kLuaMng_, 1, kIndex.m_tmStart ), return false );
        _JIF( LoadTime( kLuaMng_, 2, kIndex.m_tmEnd ), return false );
        _JIF( kLuaMng_.GetValue( 3, nGCPoint ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( mapEvent_.insert( std::make_pair( kIndex, nGCPoint ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load Setting.. " << KncUtil::toWideString( strTable_ ) )
        << BUILD_LOG( nDefault_ )
        << BUILD_LOG( mapEvent_.size() ) << END_LOG;
    return true;
}

bool KGCPoint::LoadTime( IN KLuaManager& kLuaMng_, IN const int nIndex_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    nHour = 0;
    nMin = 0;

    _JIF( kLuaMng_.BeginTable( nIndex_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    kLuaMng_.GetValue( 4, nHour );
    kLuaMng_.GetValue( 5, nMin );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, nMin, 0 ) );
    return true;
}

int KGCPoint::GetGCPointDay( IN const time_t& tmDate_ )
{
    KLocker lock(m_csDaySetting);
    return GetCurrentGCPoint( m_nGCPointDay, m_mapEventGCPointDay, tmDate_ );
}

int KGCPoint::GetGCPointWeek( IN const int& nMonth_, IN const int& nWeekID_ )
{
    KLocker lock(m_csWeekSetting);

    PAIR_INT prKey( nMonth_, nWeekID_ );
    std::map<PAIR_INT,int>::const_iterator cmit;
    cmit = m_mapEventGCPointWeek.find( prKey );
    if ( cmit != m_mapEventGCPointWeek.end() ) {
        START_LOG( clog, L"EventWeek GCPoint : " << cmit->second << L", MonthID : " << nMonth_ << L", WeekID : " << nWeekID_ ) << END_LOG;
        return cmit->second;
    }

    START_LOG( clog, L"Default GCPoint : " << m_nGCPointWeek << L", MonthID : " << nMonth_ << L", WeekID : " << nWeekID_ ) << END_LOG;
    return m_nGCPointWeek;
}

int KGCPoint::GetGCPointOpenCalendar( IN const time_t& tmDate_ )
{
    KLocker lock(m_csOpenCalendarSetting);
    return GetCurrentGCPoint( m_nGCPointOpenCalendar, m_mapEventGCPointOpenCalendar, tmDate_ );
}

int KGCPoint::GetCurrentGCPoint( IN const int& nDefault_, IN const std::map<KTimeKey,int>& mapEvent_, IN const time_t& tmCurrent_ )
{
    std::map<KTimeKey,int>::const_iterator cmit;
    for ( cmit = mapEvent_.begin() ; cmit != mapEvent_.end() ; ++cmit ) {

        if ( cmit->first.m_tmStart <= tmCurrent_ && tmCurrent_ <= cmit->first.m_tmEnd ) {
            START_LOG( clog, L"Event Day.. GCPoint : " << cmit->second )
                << BUILD_LOGtm( CTime(tmCurrent_) )
                << BUILD_LOGtm( CTime(cmit->first.m_tmStart) )
                << BUILD_LOGtm( CTime(cmit->first.m_tmEnd) ) << END_LOG;
            return cmit->second;
        }
    }

    START_LOG( clog, L"Default GCPoint : " << nDefault_ )
        << BUILD_LOGtm( CTime(tmCurrent_) ) << END_LOG;
    return nDefault_;
}

void KGCPoint::DumpInfo()
{
    //std::cout << L"--- Dump GCPoint Info ---" << dbg::endl;

    //{
    //    KLocker lock(m_csDaySetting);
    //    std::cout << L"GCPointDay : " << m_nGCPointDay 
    //        << L", Event size : " << m_mapEventGCPointDay.size() << dbg::endl;
    //    DumpEventSetting( m_mapEventGCPointDay );
    //}

    //{
    //    KLocker lock(m_csWeekSetting);
    //    std::cout << L"GCPointWeek : " << m_nGCPointWeek
    //        << L", Event size : " << m_mapEventGCPointWeek.size() << dbg::endl;

    //    std::map<PAIR_INT,int>::const_iterator cmit;
    //    for ( cmit = m_mapEventGCPointWeek.begin() ; cmit != m_mapEventGCPointWeek.end() ; ++cmit ) {
    //        std::cout << std::tab << L"[" << cmit->first.first << L"-" << cmit->first.second << L", GCPoint : " << cmit->second << L"]" << dbg::endl;
    //    }
    //}

    //{
    //    KLocker lock(m_csOpenCalendarSetting);
    //    std::cout << L"GCPointOpenCalendar : " << m_nGCPointOpenCalendar
    //        << L", Event size : " << m_mapEventGCPointOpenCalendar.size() << dbg::endl;
    //    DumpEventSetting( m_mapEventGCPointOpenCalendar );
    //}

    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KGCPoint::DumpEventSetting( IN const std::map<KTimeKey,int>& mapEvent_ )
{
    //std::map<KTimeKey,int>::const_iterator cmit;
    //for ( cmit = mapEvent_.begin() ; cmit != mapEvent_.end() ; ++cmit ) {
    //    std::cout << std::tab << L"[" << (LPCWSTR)CTime(cmit->first.m_tmStart).Format( KNC_TIME_FORMAT )
    //        << L" - " << (LPCWSTR)CTime(cmit->first.m_tmEnd).Format( KNC_TIME_FORMAT )
    //        << L", GCPoint : " << cmit->second << L"]" << dbg::endl;
    //}
}