#pragma once

#include "GCUI/GCAttendanceStampSlot.h"
#include "GCUI/GCAttendanceStampDlg.h"

struct SStampImageInfo
{
    int iFileNum;
    int iIndexNum;

    SStampImageInfo()
        : iFileNum( 0 )
        , iIndexNum( 0 )
    {
    }
};

class GCAttendanceStampManager
{
    DeclareSingleton(GCAttendanceStampManager);

    bool                             m_bUseImmediatelyRegisterMission;    // 보상 받는 즉시 미션 등록할지 여부(스크립트)
    std::map<int, VEC_REWARD>        m_mapCumulativeAttendanceRewardList; // 누적 출석 보상의 전체 정보(스크립트)
    KEVENT_ACCUMULATE_ATTENDANCE_ACK m_kUserAccumulateData;               // 유저의 누적 출석 데이터

public:

    GCAttendanceStampManager(void);
    ~GCAttendanceStampManager(void);

    void SetEventInfo( IN KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK& kRecv_ );
    int GetDayCount();
    bool GetSlotReward( IN int nIndex, OUT VEC_REWARD& vecReward );
    AttendanceSlotState GetSlotState( IN int nIndex );
    void OnEventAccumulateAttendanceRewardAck( IN KEVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK& kRecv_ );
    void RegisterMission( IN KItem& kItem );
    void SetUIType( int UiType );
    int GetUIType();
public:
    int m_iUiType;

};
DefSingletonInline(GCAttendanceStampManager);
