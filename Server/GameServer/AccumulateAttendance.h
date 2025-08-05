#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "UserPacket.h"

// 누적 출석 시스템 (발렌타인 이벤트로 추가)
// 스크립트에 누적일 숫자별로 아이템을 세팅한다
// 기간, 버전은 DB에 세팅하고 센터서버에서 퍼온다
// 작업자 :
//    서버 - 손기운
//    클라 - 이재준
//    기획 - 구동현
//    Comment : 

class KLuaManager;

class KAccumulateAttendanceManager
{
    DeclareSingleton( KAccumulateAttendanceManager );
    NiDeclareRootRTTI( KAccumulateAttendanceManager );
    DeclToStringW;
public:
    KAccumulateAttendanceManager(void);
    ~KAccumulateAttendanceManager(void);

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun(); // 이벤트 진행중인지 체크.
    void UpdateInfoFromCenter( IN const time_t& tmBegin, IN const time_t& tmEnd, IN const int& nVersion );
    const int& GetVersion() { KLocker lock( m_csAccumulateAttendance ); return m_nVersion; };
    void GetAccumulateAttendanceScriptInfo( OUT KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK& kPacket );
    bool CheckRewardDayValid( IN const int& nRewardReqDayIndex );
    bool GetReward( IN const int& nRewardReqDayIndex, OUT VEC_REWARD& vecReward );
    bool GetAccumulateAttendanceUserData_FromDB( IN const std::string& strScript );
private:
    bool LoadAttendanceRewardList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, VEC_REWARD>& mapCumulativeAttendanceRewardList );
    bool LoadRewardItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
private:
    mutable KncCriticalSection m_csAccumulateAttendance;

    // 센터에서 얻음
    time_t                    m_tmBegin;                           // 이벤트 시작 날짜
    time_t                    m_tmEnd;                             // 이벤트 종료 날짜
    int                       m_nVersion;                          // 이벤트 버전
    // 스크립트에서 읽음
    bool                      m_bUseImmediatelyRegisterMission;    // 보상 받는 즉시 미션 등록할 것인지
    int                       m_nClientUIType;                     // 클라이언트 UI 선택용 타입 설정
    std::map<int, VEC_REWARD> m_mapCumulativeAttendanceRewardList; // 누적 출석 아이템 정보
};

DefSingletonInline( KAccumulateAttendanceManager );
DeclOstmOperatorA( KAccumulateAttendanceManager );

//////////////////////////////////////////////////////////////////////////
/////////////// ↓↓ 누적 출석 유저 데이터 객체 클래스↓↓ ///////////////
//////////////////////////////////////////////////////////////////////////
class KAccumulateAttendanceUserData
{
public:
    KAccumulateAttendanceUserData(void);
    ~KAccumulateAttendanceUserData(void);
public:
    void UpdateUserAccumulateAttendanceData( IN const KEVENT_ACCUMULATE_ATTENDANCE_ACK& kPacket );
    void GetUserAccumulateAttendanceData( OUT KEVENT_ACCUMULATE_ATTENDANCE_ACK& kPacket );
    void UpdateUserAttendanceRewardCompleteData( IN const std::vector<int>& vecRewardComplete );
    bool IsRewardCompleteDay( IN const int& nRewardReqDayIndex );
private:
    int              m_nCurrentAccumulateDay; // 현재 누적일
    std::vector<int> m_vecRewardCompleteDay;  // 보상 지급 완료 날짜 리스트
};
