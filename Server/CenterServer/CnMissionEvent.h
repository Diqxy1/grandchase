#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <set>

class KCnMissionEvent
{
    DeclareSingleton( KCnMissionEvent );
    NiDeclareRootRTTI( KCnMissionEvent );
    DeclToStringW;

public:
    KCnMissionEvent(void);
    ~KCnMissionEvent(void);

    void DBMissionEventUpdate( IN const int nMissionEventID, IN const time_t tmBegin, IN const time_t tmEnd, IN int nMissionCheckHour );
    void DBDailyEventUpdate( IN std::vector<KMissionDailyData>& vecDailyEventData );
    void UpdateState();
    bool IsEventTerm() { KLocker lock( m_csMissionEvent ); return m_bEventTerm; }
    bool IsChanged();
    void SetChangeTag( IN const bool bChanged );
    // 날짜가 수정되었는지 체크.
    bool IsDateUpdate();
    void SetChangeDate( IN const bool bDateUpdated );

    void Tick();
    void GetMissionEventPacket( OUT KECN_MISSION_EVENT_UPDATE_NOT& kPacket );

protected:
    bool CheckEventTerm();
    bool GetCurrentDailyEvent( OUT KMissionDailyData& kCurrentDailyEvent );
    bool GetTotalDailyEvent( OUT std::vector<KMissionDailyData>& vecDailyEventData );
    void GetTotalDailyEventItem( OUT std::set<int>& sItemList );
    void QueuingToDB( IN const bool bFirst );
    
    void DumpCurrentEvent( std::wostream& stm ) const ;
    void SendToCenter();
    void SendToNotify( IN const DWORD dwTime );

    void GetTodayMissions( IN std::vector<KMissionDailyData>& vecTotalMissions, OUT std::vector<KMissionDailyData>& vecMissions );
    void GetCurrentMissions( OUT std::vector<KMissionDailyData>& vecMissions );

protected:
    mutable KncCriticalSection      m_csMissionEvent;

    // 전체 이벤트 시간( 시작시작, 종료시간 )
    time_t                  m_tmEventBegin;
    time_t                  m_tmEventEnd;
    int                     m_nMissionEventID;
    // 몇시에 해당 이벤트를 발동할 것인가?
    int                     m_nMissionCheckHour;
    // 서버의 시작 시간을 저장하고 있는 변수.
    time_t                  m_tmStartBegin;

    // 전체 미션 이벤트 리스트
    std::vector<KMissionDailyData>      m_vecMissionDaily;
    // 전체 미션 이벤트 ItemList
    std::set<int>                       m_setMissionList;

    std::vector<KMissionDailyData>      m_vecCurrentMissions;

    bool                    m_bChanged;     // 변경사항이 있었는가..,?
    bool                    m_bTodayChanged;// 하루가 지났는지에 대한 확인.
    bool                    m_bEventTerm;   // 미션 이벤트 기간인가..,?

    // Tick 관련 데이터
    DWORD                   m_dwLastTick;
    DWORD                   m_dwTickGap;

    // 공지 관련 데이터 
    DWORD                   m_dwNotifyTime;             // 이벤트 체크 시간.
    bool                    m_bFirstTimeCheck;          // 10분전 공지에 대해서 체크 변수.
    bool                    m_bSecondTimeCheck;         // 5분전 공지에 대해서 체크 변수.
    bool                    m_bThirdTimeCheck;         // 시간변경에 대해서 체크 변수.
    CTime                   m_tmFirstNofity;            // 오늘변경 시간의 10분전을 저장.
    CTime                   m_tmSecondNofity;            // 오늘변경 시간의 5분전을 저장.
    CTime                   m_tmthirdCheck;             // 시간변경을 해당 변수로 테스트.

    bool                    m_bFirst;                   // 서버 시작시간 업데이트를 위해서..,
};

DefSingletonInline( KCnMissionEvent );
DeclOstmOperatorA( KCnMissionEvent );