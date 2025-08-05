#include "user.h" // winsock
#include "ShutdownUser.h"
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KShutdownUser );
ImplOstmOperatorW2A( KShutdownUser );
NiImplementRootRTTI( KShutdownUser );

KShutdownUser::KShutdownUser(void)
:m_kAlarmMsgTimer(60*1000)
{
}

KShutdownUser::~KShutdownUser(void)
{
}

ImplToStringW( KShutdownUser )
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

void KShutdownUser::Tick()
{
    if ( false == m_kAlarmMsgTimer.CheckTime() ) {
        return;
    }

    KShutdownUserConfig kConfig;
    GetConfig( kConfig );

    if ( false == kConfig.m_bEnable ) {
        return;
    }

    CTime tmCurrent( CTime::GetCurrentTime() );
    CTime tmStart( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), kConfig.m_prStartTime.first, kConfig.m_prStartTime.second, 0 );

    std::vector<USHORT>::iterator vit;
    for ( vit = kConfig.m_vecAlarmMsgTime.begin() ; vit != kConfig.m_vecAlarmMsgTime.end() ; ++vit ) {
        CTime tmCheck = tmStart - CTimeSpan( 0, 0, *vit, 0 );
        if ( tmCheck.GetHour() == tmCurrent.GetHour() && tmCheck.GetMinute() == tmCurrent.GetMinute() ) {
            KEVENT_SHUTDOWN_ALARM_NOT kPacket;
            kPacket.m_prAge.first = kConfig.m_prAge.first;
            kPacket.m_prAge.second = kConfig.m_prAge.second;
            kPacket.m_prStartTime = kConfig.m_prStartTime;
            kPacket.m_prEndTime = kConfig.m_prEndTime;
            kPacket.m_usPrevTime = *vit;
            SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_SHUTDOWN_ALARM_NOT, kPacket );

            START_LOG( clog, L"셧다운제 시작전 알림 메세지 전달.." )
                << BUILD_LOGtm( tmCurrent )
                << BUILD_LOGtm( tmStart )
                << BUILD_LOG( kPacket.m_usPrevTime ) << END_LOG;
        }
    }
}

bool KShutdownUser::CheckAge( IN const int& nAge_, IN const DWORD& dwIP_, IN int nCheckType_ )
{
    KShutdownUserConfig kConfig;
    GetConfig( kConfig );

    in_addr inIP;
    inIP.S_un.S_addr = dwIP_;

    if ( false == kConfig.m_bEnable ) {
        START_LOG( clog, L"셧다운제 체크 안함." ) << END_LOG;
        return true;
    }

    switch ( nCheckType_ ) {
    case CT_GAME_LOGIN:
        if ( false == kConfig.m_bGameLoginCheck ) {
            START_LOG( clog, L"게임서버 로그인 체크 안함." ) << END_LOG;
            return true;
        }
        break;
    case CT_GAME_TICK:
        if ( false == kConfig.m_bGameTickCheck ) {
            START_LOG( clog, L"게임서버 Tick 체크 안함." ) << END_LOG;
            return true;
        }
        break;
    default:
        START_LOG( cwarn, L"정의되지 않은 게임서버 체크타입.. Type : " << nCheckType_ ) << END_LOG;
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