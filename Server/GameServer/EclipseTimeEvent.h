#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "Lua/KLuaManager.h"
#include <vector>
#include "UserPacket.h"

class KEclipseTimeEvent
{
    DeclareSingleton( KEclipseTimeEvent );
    NiDeclareRootRTTI( KEclipseTimeEvent );
    DeclToStringW;

public:
    enum ALARM_TYPE
    {
        ALARM_TYPE_START = 0,
        AL_EVENT_START,     // 일식 이벤트 시작 알림
        AL_EVENT_END,       // 일식 이벤트 종료 알림
        ALARM_TYPE_END,
    };

    struct KEclipseTimeInfo {
        time_t                          m_tmBegin;
        time_t                          m_tmEnd;
        DWORD                           m_dwPeriod;     // 초단위로 저장, 클라에게도 초단위로 전송
        DWORD                           m_dwDuration;   // 초단위로 저장, 클라에게도 초단위로 전송
        std::vector< int >              m_vecMonsterID;

        void Clear()
        {
            m_tmBegin = 0;
            m_tmEnd = 0;
            m_dwPeriod = 0;
            m_dwDuration = 0;
            m_vecMonsterID.clear();
        }
    };

    KEclipseTimeEvent( void );
    ~KEclipseTimeEvent( void );

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    void GetEclipseTimeInfo( OUT KEVENT_ECLIPSE_TIME_EVENT_INFO_NOT& kPacket_ );
    void Tick();
    bool IsEventOn();
    bool CheckEclipseEventMonster( IN const int& nMonID_ );
    time_t GetCurrentPeriod();
    bool SendEclipseTimeEventInfoNot( IN const std::string& strScript_ );

private:
    bool LoadEclipseTimeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KEclipseTimeInfo& kEclipseTimeInfo_ );
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadMonsterID( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< int >& vecMonsterID_ );

    bool CheckTick();
    void UpdateStatus();

    void SetNoticeInfo( IN const bool& bBeforeStatus_ );

protected:
    mutable KncCriticalSection      m_csETE;

    DWORD                           m_dwTickGap;
    DWORD                           m_dwLastTick;

    KEclipseTimeInfo                m_kEclipseTimeInfo;

    time_t                          m_tmCurrentPeriod;          // 현재 일식 주기 시작 시간
    time_t                          m_tmNextPeriod;             // 다음 일식 주기 시작 시간
    bool                            m_bEclipseInProgress;       // 일식이 진행 중인지(true) 아닌지(false)
};

DefSingletonInline( KEclipseTimeEvent );
DeclOstmOperatorA( KEclipseTimeEvent );
