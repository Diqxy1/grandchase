#pragma once

#include "KNCSingleton.h"

class GCEclipseEventManager
{
    DeclareSingleton( GCEclipseEventManager );

public:
    GCEclipseEventManager(void);
    ~GCEclipseEventManager(void);

    void SetEventTime(time_t tmBeginTime,time_t tmEndTime, DWORD dwPeriod, DWORD dwExecise);

    bool IsEventTime ( ); // 전체 이벤트.
    bool IsActiveEvent(); // 특수 효과 발동 이벤트 
    int GetLeftEventTime();
    int GetLeftEventTimePercent();
    bool IsHoverTime();
    bool IsVeiwMode();
    bool IsValidDungeon( const EGCGameMode eGameMode_ );

    DWORD GetEventTimePeriod() { return m_dwEventPeriod; }

private:
    time_t m_tmEventBegin; //이벤트 전체 시작타임
    time_t m_tmEventEnd; //이벤트 전체 종료 타임

    DWORD m_dwEventPeriod;//이벤트 발동 주기, 단위 : s 30분 - 1,800
    DWORD m_dwExeciseEventTime; //이벤트 발동 시간, 단위 : s
};
DefSingletonInline( GCEclipseEventManager );
