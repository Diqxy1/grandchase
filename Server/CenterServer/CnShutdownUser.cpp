#include "NUser.h"
#include "CnShutdownUser.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterSimLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnShutdownUser );
ImplOstmOperatorW2A( KCnShutdownUser );
NiImplementRootRTTI( KCnShutdownUser );

KCnShutdownUser::KCnShutdownUser(void)
{
}

KCnShutdownUser::~KCnShutdownUser(void)
{
}

ImplToStringW( KCnShutdownUser )
{
    KLocker lock( m_csConfig );
    return START_TOSTRINGW
        << TOSTRINGWb( m_kConfig.m_bEnable )
        << TOSTRINGWb( m_kConfig.m_bCenterLoginCheck )
        << TOSTRINGWb( m_kConfig.m_bGameLoginCheck )
        << TOSTRINGWb( m_kConfig.m_bGameTickCheck )
        << TOSTRINGW( m_kConfig.m_prAge.first )
        << TOSTRINGW( m_kConfig.m_prAge.second )
        << TOSTRINGW( m_kConfig.m_prStartTime.first )
        << TOSTRINGW( m_kConfig.m_prStartTime.second )
        << TOSTRINGW( m_kConfig.m_prEndTime.first )
        << TOSTRINGW( m_kConfig.m_prEndTime.second )
        << TOSTRINGW( m_kConfig.m_dwShutdownTick )
        << TOSTRINGW( m_kConfig.m_vecAlarmMsgTime.size() );
}

bool KCnShutdownUser::LoadScript()
{
    KLuaManager kLuaMng;
    KShutdownUserConfig kConfig;

    _JIF( kLuaMng.DoFile( "InitShutdownUser.lua" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "Enable", kConfig.m_bEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CenterLoginCheck", kConfig.m_bCenterLoginCheck ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GameLoginCheck", kConfig.m_bGameLoginCheck ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GameTickCheck", kConfig.m_bGameTickCheck ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "Age" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 1, kConfig.m_prAge.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 2, kConfig.m_prAge.second ) == S_OK, return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "StartTime" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 1, kConfig.m_prStartTime.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 2, kConfig.m_prStartTime.second ) == S_OK, return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "EndTime" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 1, kConfig.m_prEndTime.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 2, kConfig.m_prEndTime.second ) == S_OK, return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.GetValue( "ShutdownTick", kConfig.m_dwShutdownTick ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "AlarmMsgTime" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        USHORT usMin = 0;
        if ( kLuaMng.GetValue( i, usMin ) != S_OK ) break;
        kConfig.m_vecAlarmMsgTime.push_back( usMin );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.GetValue( "TestIPOnly", kConfig.m_bTestIPOnly ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "TestIPRange" ) == S_OK, return false );
    for ( int i = 1 ;; ++i ) {
        std::string strFirst;
        std::string strSecond;
        PAIR_DWORD prIpRange;

        if ( kLuaMng.BeginTable( i ) != S_OK) break;
        _JIF( kLuaMng.GetValue( 1, strFirst ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, strSecond ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        prIpRange.first = ::ntohl( inet_addr( strFirst.c_str() ) );
        prIpRange.second = ::ntohl( inet_addr( strSecond.c_str() ) );;
        kConfig.m_vecTestIPRange.push_back( prIpRange );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Shutdown User Script Load.." )
        << BUILD_LOG( kConfig.m_bEnable )
        << BUILD_LOG( kConfig.m_bCenterLoginCheck )
        << BUILD_LOG( kConfig.m_bGameLoginCheck )
        << BUILD_LOG( kConfig.m_bGameTickCheck )
        << BUILD_LOG( kConfig.m_prAge.first )
        << BUILD_LOG( kConfig.m_prAge.second )
        << BUILD_LOG( kConfig.m_prStartTime.first )
        << BUILD_LOG( kConfig.m_prStartTime.second )
        << BUILD_LOG( kConfig.m_prEndTime.first )
        << BUILD_LOG( kConfig.m_prEndTime.second )
        << BUILD_LOG( kConfig.m_dwShutdownTick )
        << BUILD_LOG( kConfig.m_bTestIPOnly )
        << BUILD_LOG( kConfig.m_vecTestIPRange.size() )
        << BUILD_LOG( kConfig.m_vecAlarmMsgTime.size() ) << END_LOG;

    {
        KLocker lock( m_csConfig );
        m_kConfig = kConfig;
    }

    {
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SHUTDOWN_USER_CONFIG_NOT, kConfig );
        START_LOG( cerr, L"셧다운제 스크립트 로드후, 설정 전송" ) << END_LOG;
    }
    return true;
}

bool KCnShutdownUser::CheckAge( IN const int& nAge_, IN const DWORD& dwIP_ )
{
    KShutdownUserConfig kConfig;
    GetConfig( kConfig );

    in_addr inIP;
    inIP.S_un.S_addr = dwIP_;

    if ( false == kConfig.m_bEnable ) {
        START_LOG( clog, L"셧다운제 체크 안함." ) << END_LOG;
        return true;
    }

    if ( false == kConfig.m_bCenterLoginCheck ) {
        START_LOG( clog, L"셧다운제 로그인 체크 안함." ) << END_LOG;
        return true;
    }

    if ( kConfig.m_bTestIPOnly ) {
        bool bIsTestIP = false;
        std::vector<PAIR_DWORD>::iterator vit;
        for ( vit = kConfig.m_vecTestIPRange.begin() ; vit != kConfig.m_vecTestIPRange.end() ; ++vit ) {
            if ( vit->first <= dwIP_ && vit->second >= dwIP_ ) {
                bIsTestIP = true;
                START_LOG( clog, L"해당 IP는 테스트 IP입니다. IP : " << inet_ntoa(inIP) ) << END_LOG;
                break;
            }
        }

        if ( false == bIsTestIP ) {
            START_LOG( clog, L"테스트 IP만 검사하는데, 테스트IP가 아님. IP : " << inet_ntoa(inIP) ) << END_LOG;
            return true;
        }
    }

    if ( kConfig.m_prAge.first > nAge_ || nAge_ > kConfig.m_prAge.second ) {
        START_LOG( clog, L"대상 연령층이 아니다. Age : " << nAge_ ) << END_LOG;
        return true;
    }

    CTime tmCurrent( CTime::GetCurrentTime() );
    CTime tmStart( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), kConfig.m_prStartTime.first, kConfig.m_prStartTime.second, 0 );
    CTime tmEnd( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), kConfig.m_prEndTime.first, kConfig.m_prEndTime.second, 0 );

    bool bTimeMatching = false;
    if ( tmStart > tmEnd ) {
        // 자정이 포함되는 시간대의 체크
        CTime tmMidStart( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
        CTime tmMidEnd( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 23, 59, 59 );
        if ( ( tmCurrent >= tmStart && tmCurrent <= tmMidEnd ) || ( tmCurrent >= tmMidStart && tmCurrent <= tmEnd ) ) {
            bTimeMatching = true;

            START_LOG( cwarn, L"Shutdown time matching.." )
                << BUILD_LOGtm( tmMidStart )
                << BUILD_LOGtm( tmMidEnd )
                << BUILD_LOGtm( tmCurrent )
                << BUILD_LOGtm( tmStart )
                << BUILD_LOGtm( tmEnd ) << END_LOG;
        }
    }
    else if ( tmCurrent >= tmStart && tmCurrent <= tmEnd ) {
        bTimeMatching = true;

        START_LOG( cwarn, L"Shutdown time matching.." )
            << BUILD_LOGtm( tmCurrent )
            << BUILD_LOGtm( tmStart )
            << BUILD_LOGtm( tmEnd ) << END_LOG;
    }

    if ( false == bTimeMatching ) {
        START_LOG( clog, L"체크하는 시간대가 아니다." )
            << BUILD_LOGtm( tmCurrent )
            << BUILD_LOGtm( tmStart )
            << BUILD_LOGtm( tmEnd ) << END_LOG;
        return true;
    }

    START_LOG( cerr, L"Shutdown Target User.. Age : " << nAge_ )
        << BUILD_LOG( kConfig.m_prAge.first )
        << BUILD_LOG( kConfig.m_prAge.second )
        << BUILD_LOG( inet_ntoa(inIP) )
        << BUILD_LOG( kConfig.m_bTestIPOnly )
        << BUILD_LOGtm( tmCurrent )
        << BUILD_LOGtm( tmStart )
        << BUILD_LOGtm( tmEnd ) << END_LOG;
    return false;
}