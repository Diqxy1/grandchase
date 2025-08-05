#include "CenterSimLayer.h"
#include "CnAccumulateAttendance.h"
#include "CenterDBThread.h"
#include "KncTimer.h"

ImplementSingleton( KCnAccumulateAttendanceManager );
ImplOstmOperatorW2A( KCnAccumulateAttendanceManager );
NiImplementRootRTTI( KCnAccumulateAttendanceManager );
ImplToStringW( KCnAccumulateAttendanceManager )
{
    KLocker lock( m_csAccumulateAttendance );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_nVersion )
        << TOSTRINGW( m_bChanged );
}

KCnAccumulateAttendanceManager::KCnAccumulateAttendanceManager(void)
: m_kUpdateCheckTimer( 60 * 1000 )
,m_bChanged( true )
{
}

KCnAccumulateAttendanceManager::~KCnAccumulateAttendanceManager(void)
{
}

void KCnAccumulateAttendanceManager::Tick()
{
    if ( m_kUpdateCheckTimer.CheckTime() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_ACCUMULATE_ATTENDANCE_DATA_UPDATE_NOT , L"", 0 );
    }

    if ( CheckChange() ) {
        KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT kPacket;
        GetAccumulateAttendanceSettingData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT, kPacket );

        m_bChanged = false;
    }
}

bool KCnAccumulateAttendanceManager::CheckChange()
{
    KLocker lock( m_csAccumulateAttendance );
    if ( m_bChanged ) {
        m_bChanged = false;
        return true;
    }

    return false;
}

void KCnAccumulateAttendanceManager::GetAccumulateAttendanceSettingData( KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csAccumulateAttendance );
    kPacket_.m_nVersion = m_nVersion;
    kPacket_.m_tmBegin = m_tmBegin;
    kPacket_.m_tmEnd = m_tmEnd;
}

void KCnAccumulateAttendanceManager::UpdateAccumulateAttendanceSettingData( const KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csAccumulateAttendance );
    m_nVersion = kPacket_.m_nVersion;
    m_tmBegin = kPacket_.m_tmBegin;
    m_tmEnd = kPacket_.m_tmEnd;
    m_bChanged = true;
}