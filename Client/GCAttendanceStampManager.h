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

    bool                             m_bUseImmediatelyRegisterMission;    // ���� �޴� ��� �̼� ������� ����(��ũ��Ʈ)
    std::map<int, VEC_REWARD>        m_mapCumulativeAttendanceRewardList; // ���� �⼮ ������ ��ü ����(��ũ��Ʈ)
    KEVENT_ACCUMULATE_ATTENDANCE_ACK m_kUserAccumulateData;               // ������ ���� �⼮ ������

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
