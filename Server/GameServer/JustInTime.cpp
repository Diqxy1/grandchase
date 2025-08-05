#include "GSSimLayer.h"
#include "JustInTime.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KJustInTime );
ImplOstmOperatorW2A( KJustInTime );
NiImplementRootRTTI( KJustInTime );

KJustInTime::KJustInTime( void )
: m_kTick( 60000 )
{
    m_mapCurrentEvent.clear();
    m_mapNextEvent.clear();
    m_mapEventList.clear();
    m_mapReceivedRewardStatus.clear();
}

KJustInTime::~KJustInTime( void )
{
}

ImplToStringW( KJustInTime )
{
    KLocker lock( m_csJIT );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapCurrentEvent.size() )
        << TOSTRINGW( m_mapNextEvent.size() )
        << TOSTRINGW( m_kTick.GetInterval() )
        << TOSTRINGW( m_mapEventList.size() )
        << TOSTRINGW( m_mapReceivedRewardStatus.size() );
}

bool KJustInTime::LoadScript()
{
    return _LoadScript(std::string());
}

bool KJustInTime::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    DWORD dwTickGap = 0;
    std::map<USHORT, std::map<time_t,KJITEvent> > mapEventList;
    mapEventList.clear();

    const std::string strScriptName = "InitJustInTime.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( LoadEventList( kLuaMng, std::string("EventList"), mapEventList ), return false );

    START_LOG( cerr, L"JIT 스크립트 읽기 완료." )
        << BUILD_LOG( dwTickGap )
        << BUILD_LOG( mapEventList.size() )
        << END_LOG;

    {
        KLocker lock( m_csJIT );
        m_kTick.SetInterval( dwTickGap );
        m_mapEventList.swap( mapEventList );
        m_mapCurrentEvent.clear();
    }

    for ( USHORT usUserType = EU_ALL_USER ; usUserType < EU_MAX ; ++usUserType ) {
        time_t tmNextEvent;
        GetNextEvent( usUserType, tmNextEvent );
        SetNextEvent( usUserType, tmNextEvent );
        SetCurrentEvent( usUserType, tmNextEvent );
    }

    return true;
}

bool KJustInTime::LoadEventList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<USHORT,std::map<time_t,KJITEvent> >& mapEventList_ )
{
    mapEventList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        USHORT usUserType = EU_ALL_USER;
        std::map<time_t,KJITEvent> mapEvent;

        _JIF( kLuaMng_.GetValue( "UserType", usUserType ) == S_OK, return false );
        _JIF( LoadEvent( kLuaMng_, std::string( "Event" ), mapEvent ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapEventList_[usUserType] = mapEvent;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJustInTime::LoadEvent( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< time_t, KJITEvent >& mapEvent_ )
{
    mapEvent_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KJITEvent kJITEvent;
        _JIF( LoadTime( kLuaMng_, std::string( "Time" ), kJITEvent.m_tmEventTime ), return false );
        _JIF( LoadRewardList( kLuaMng_, std::string( "RewardList" ), kJITEvent.m_vecRewardList ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapEvent_[ kJITEvent.m_tmEventTime ] = kJITEvent;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJustInTime::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime( nYear, nMonth, nDay, nHour, nMin, 0 ) );

    return true;
}

bool KJustInTime::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardList_ )
{
    vecRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardList_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJustInTime::GetNextEvent( IN const USHORT& usUserType_, OUT time_t& tmNextEvent_ )
{
    CTime ctmCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( CTime( ctmCurrent.GetYear(), ctmCurrent.GetMonth(), ctmCurrent.GetDay(), ctmCurrent.GetHour(), ctmCurrent.GetMinute(), 0 ) );

    KLocker lock( m_csJIT );
    std::map<USHORT, std::map<time_t,KJITEvent> >::iterator mit;
    mit = m_mapEventList.find( usUserType_ );
    if ( mit == m_mapEventList.end() ) {
        tmNextEvent_ = 0;
        START_LOG( cwarn, L"JIT 이벤트리스트에 해당하는 유저타입이 없음.. UserType : " << usUserType_ ) << END_LOG;
        return false;
    }

    std::map<time_t,KJITEvent>::iterator mitEvent;
    for ( mitEvent = mit->second.begin(); mitEvent != mit->second.end(); ++mitEvent ) {
        // 가장 가까운 다음 시간정보를 찾아서 세팅. 이후 전송할 때 오늘 날자인 경우에만 전송하게 한다.
        if ( tmCurrent < mitEvent->second.m_tmEventTime ) {
            tmNextEvent_ = mitEvent->second.m_tmEventTime;
            START_LOG( clog, L"JIT 이벤트 다음시간대 설정.. UserType : " << usUserType_ )
                << BUILD_LOGtm( CTime(tmCurrent) )
                << BUILD_LOGtm( CTime(tmNextEvent_) )
                << BUILD_LOGtm( CTime(tmNextEvent_) )
                << END_LOG;
            return true;
        }
    }

    tmNextEvent_ = 0;

    START_LOG( clog, L"JIT 이벤트 다음시간대에 해당하는 시간이 없음.. UserType : " << usUserType_ )
        << BUILD_LOGtm( CTime(tmCurrent) )
        << END_LOG;
    return false;
}

bool KJustInTime::IsHaveNextEvent( IN const USHORT& usUserType_ )
{
    KLocker lock( m_csJIT );

    std::map<USHORT,time_t>::iterator mit;
    mit = m_mapNextEvent.find( usUserType_ );
    if ( mit == m_mapNextEvent.end() ) {
        START_LOG( clog, L"JIT 다음이벤트에 해당하는 유저타입이 없음.. UserType : " << usUserType_ ) << END_LOG;
        return false;
    }

    return ( mit->second != 0 );
}

void KJustInTime::Tick()
{
    if ( !m_kTick.CheckTime() ) {
        return;
    }

    // 이벤트 대상자에게 보상 받으라고 알림.
    for ( USHORT usUserType = EU_ALL_USER ; usUserType < EU_MAX ; ++usUserType ) {
        time_t tmNextEvent = GetNextEvent( usUserType );
        if ( tmNextEvent == 0 ) {
            continue;
        }

        CTime ctmCurrent = CTime::GetCurrentTime();
        time_t tmCurrent = KncUtil::TimeToInt( CTime( ctmCurrent.GetYear(), ctmCurrent.GetMonth(), ctmCurrent.GetDay(), ctmCurrent.GetHour(), ctmCurrent.GetMinute(), 0 ) );

        if ( tmCurrent >= tmNextEvent ) {
            SetCurrentEvent( usUserType, tmNextEvent );

            KEVENT_JUST_IN_TIME_ON_TIME_NOT kPacket;
            kPacket.m_tmJIT = tmNextEvent;
            kPacket.m_usUserType = usUserType;
            LIF( GetRewardList( usUserType, tmNextEvent, kPacket.m_vecItemList ) );

            SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_JUST_IN_TIME_ON_TIME_NOT, kPacket );

            time_t tmNextEventInfo;
            GetNextEvent( usUserType, tmNextEventInfo );
            SetNextEvent( usUserType, tmNextEventInfo );
        }

    }
}

bool KJustInTime::GetRewardList( IN const USHORT& usUserType_, IN const time_t& tmEvent_, OUT std::vector< KDropItemInfo >& vecRewardList_ )
{
    vecRewardList_.clear();

    KLocker lock( m_csJIT );

    std::map<USHORT, std::map<time_t,KJITEvent> >::iterator mit;
    mit = m_mapEventList.find( usUserType_ );
    if ( mit == m_mapEventList.end() ) {
        START_LOG( cwarn, L"JIT 이벤트리스트에 해당하는 유저타입이 없음. UserType : " << usUserType_ ) << END_LOG;
        return false;
    }

    std::map<USHORT,time_t>::iterator mitTime;
    mitTime = m_mapCurrentEvent.find( usUserType_ );
    if ( mitTime == m_mapCurrentEvent.end() ) {
        START_LOG( cwarn, L"JIT 현재이벤트에 해당하는 유저타입이 없음. UserType : " << usUserType_ ) << END_LOG;
        return false;
    }

    if ( tmEvent_ != mitTime->second ) {
        START_LOG( cwarn, L"JIT 이벤트에 해당하는 시간대가 아님." )
            << BUILD_LOG( usUserType_ )
            << BUILD_LOGtm( CTime(tmEvent_) )
            << BUILD_LOGtm( CTime(mitTime->second) )
            << END_LOG;
        return false;
    }

    std::map<time_t,KJITEvent>::iterator mitEvent;
    mitEvent = mit->second.find( mitTime->second );
    if ( mitEvent == mit->second.end() ) {
        START_LOG( cwarn, L"JIT 이벤트에 해당하는 시간대가 없음." )
            << BUILD_LOG( usUserType_ )
            << BUILD_LOGtm( CTime(mitTime->second) )
            << END_LOG;
        return false;
    }

    vecRewardList_ = mitEvent->second.m_vecRewardList;

    START_LOG( clog, L"JIT 이벤트에 해당하는 보상 설정." )
        << BUILD_LOG( usUserType_ )
        << BUILD_LOGtm( CTime(mitTime->second) )
        << BUILD_LOG( vecRewardList_.size() )
        << END_LOG;
    return true;
}

bool KJustInTime::UpdateReceivedRewardStatus( IN const USHORT& usUserType_, IN std::pair< DWORD, time_t >& prReceivedInfo_ )
{
    KLocker lock( m_csJIT );

    std::set< std::pair<DWORD,time_t> >::iterator sit;
    sit = m_mapReceivedRewardStatus[usUserType_].find( prReceivedInfo_ );
    if ( sit != m_mapReceivedRewardStatus[usUserType_].end() ) {
        return false;
    }

    m_mapReceivedRewardStatus[usUserType_].insert( prReceivedInfo_ );
    return true;
}

bool KJustInTime::IsReceivedReward( IN const USHORT& usUserType_, IN std::pair< DWORD, time_t >& prReceivedInfo_ )
{
    KLocker lock( m_csJIT );

    std::set< std::pair< DWORD, time_t > >::iterator sit;
    sit = m_mapReceivedRewardStatus[usUserType_].find( prReceivedInfo_ );
    return ( sit != m_mapReceivedRewardStatus[usUserType_].end() );
}

bool KJustInTime::IsNextEventToday( IN const USHORT& usUserType_ )
{
    CTime ctmCurrent = CTime::GetCurrentTime();

    CTime ctmNext( GetNextEvent( usUserType_ ) );

    return ( ctmCurrent.GetYear() == ctmNext.GetYear()
            && ctmCurrent.GetMonth() == ctmNext.GetMonth()
            && ctmCurrent.GetDay() == ctmNext.GetDay() );
}

time_t KJustInTime::GetNextEvent( IN const USHORT& usUserType_ )
{
    KLocker lock( m_csJIT );

    std::map<USHORT,time_t>::iterator mit;
    mit = m_mapNextEvent.find( usUserType_ );
    if ( mit == m_mapNextEvent.end() ) {
        return 0;
    }

    return mit->second;
}

void KJustInTime::SetCurrentEvent( IN const USHORT& usUserType, IN const time_t& tmCurrentEvent )
{
    {
        KLocker lock( m_csJIT );
        m_mapCurrentEvent[usUserType] = tmCurrentEvent;
    }

    START_LOG( clog, L"JIT Set Current Event." )
        << BUILD_LOG( usUserType )
        << BUILD_LOGtm( CTime(tmCurrentEvent) )
        << END_LOG;
}