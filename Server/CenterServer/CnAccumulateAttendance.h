#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KncTimer;

class KCnAccumulateAttendanceManager
{
    DeclareSingleton( KCnAccumulateAttendanceManager );
    NiDeclareRootRTTI( KCnAccumulateAttendanceManager );
    DeclToStringW;
public:
    KCnAccumulateAttendanceManager(void);
    ~KCnAccumulateAttendanceManager(void);

    void Tick();
    bool CheckChange();
    void GetAccumulateAttendanceSettingData( KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT& kPacket );
    void UpdateAccumulateAttendanceSettingData( const KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT& kPacket );

private:
    mutable KncCriticalSection  m_csAccumulateAttendance;

    KncTimer m_kUpdateCheckTimer; // 이벤트 변동이 있는지 확인할 타이머(1분)
    int      m_nVersion;
    time_t   m_tmBegin;
    time_t   m_tmEnd;
    bool     m_bChanged;
};

DefSingletonInline( KCnAccumulateAttendanceManager );
DeclOstmOperatorA( KCnAccumulateAttendanceManager );