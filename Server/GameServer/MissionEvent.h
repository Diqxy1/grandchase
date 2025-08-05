#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <set>

struct KECN_MISSION_EVENT_UPDATE_NOT; // CenterPacket.h
struct KECN_MISSION_NOTIFY_NOT;
struct KMissionCurrentEvent;                 // CommonPacket.h

class KMissionEvent
{
    DeclareSingleton( KMissionEvent );
    NiDeclareRootRTTI( KMissionEvent );
    DeclToStringW;

public:
    KMissionEvent(void);
    ~KMissionEvent(void);

    bool IsEventChanged();
    void SetEventChanged( bool bChange );
    bool IsMissionEventTerm();
    bool GetEventData( OUT KMissionCurrentEvent& kEventData );

    void Tick();

    void UpdateFromCenter( IN KECN_MISSION_EVENT_UPDATE_NOT& kPacket );
    void NotifyFromCenter( IN KECN_MISSION_NOTIFY_NOT& kPacket );

protected:
    void DumpCurrentEvent( std::wostream& stm ) const ;
    void GetItemList( OUT std::set<int>& setItems );


protected:
    mutable KncCriticalSection m_csMissionEvent;

    // 오늘 미션 이벤트 리스트
    std::vector<KMissionDailyData>      m_vecCurrentMissions;
    std::set<int>                       m_setItemList;
    bool                                m_bChanged;
    // 전체 미션 이벤트 ItemList
    std::set<int>           m_setMissionList;

    // Tick 관련 데이터
    DWORD                   m_dwLastTick;
    DWORD                   m_dwTickGap;
};

DefSingletonInline( KMissionEvent );
DeclOstmOperatorA( KMissionEvent );