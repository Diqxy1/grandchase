#pragma once
#include <KNCSingleton.h>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include <UserPacket.h>
namespace luabind { class object; } // 전방선언

class KFatigueSystem
{
    DeclareSingleton( KFatigueSystem );
    DeclToStringW;
public:
    KFatigueSystem(void);
    virtual ~KFatigueSystem(void);

    void SetRewardLimit(    bool bEnableRewardLimit,
                            int m_nClearAccOfflineTime,
                            int m_nThresholdTime,
                            int m_nUnlimitedTime,
                            int m_nLimitedTime,
                            const luabind::object& table );

    bool IsRun() { return m_bEnableRewardLimit; };
    int GetRewardRatio( int m_nAccOnlineTime, const std::wstring& strLogin );
    int GetThresholdTime(){ return m_nThresholdTime; };
    int GetClearAccOfflineTime(){ return m_nClearAccOfflineTime; };
    void GetInfo( OUT std::map< int, KFatigueTable >& mapTable, OUT int& nMaxOnlineTime_, OUT int& nResetOfflineTime_ );
public:
    //struct KRewardLimitTuple // Ex. m_nBeginMin < CurrentAccMin <= m_nEndMin
    //{
    //    KRewardLimitTuple()
    //    : m_nBeginMin(0), m_nEndMin(0), m_fRatio(0.f) {}
    //    int         m_nType; // 피로도 타입. 시간에 따른 sign : 녹색,노랑,빨강, userpacket에 enum 참고 ( EVENT_ACCUMULRATION_TIME_NOT, FATIGUE_TIME )
    //    int         m_nBeginMin;    // Ratio가 적용되는 시작 시간
    //    int         m_nEndMin;      // Ratio가 적용되는 마지막 시간
    //    int         m_fRatio;       // 제한 비율, 0 ~ 100
    //    int         m_nPeriod; // 공지 시간 주기
    //};
protected:

    bool        m_bEnableRewardLimit;       // 보상 제한 적용여부
    int         m_nClearAccOfflineTime;     // 누적 online 시간을 클리어할 누적 offline시간. 중국 기준으로 오프라인 5시간 누적 시에 온라인 누적 시간이 초기화됨.
    int         m_nThresholdTime;           // offline 시간을 누적 시기는 문턱 값
    int         m_nUnlimitedTime;           // 보상 제한이 없는 누적 on line 시간, 중국 기준 3시간.
    int         m_nLimitedTime;             // 보상이 전혀 없는 누적 on line 시간, 중국 기준 5시간.


    mutable KncCriticalSection      m_csPenaltyTable; // PenaltyTable을 보호한다.
    //std::vector< KFatigueTable > m_vecPenaltyTable;
    std::map< int, KFatigueTable > m_mapFatigueTable; // m_vecPenaltyTable 을 대체
};

DefSingletonInline( KFatigueSystem );
DeclOstmOperatorA( KFatigueSystem );