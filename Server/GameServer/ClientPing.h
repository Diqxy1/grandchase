#pragma once
#include <WTypes.h>
#include <boost/array.hpp>
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

/*
해야 하는 것이..
1. 평균 핑이 특정 수치를 넘어섰을 경우 강제 종료.
2. 특정 시간 동안 핑이 오지 않을 경우 강제로 게임 나가도록.
*/
class KClientPing
{
public:

    enum {
        MAX_PING_COUNT = 20,
        INIT_VALUE = 300,
    };

    KClientPing(void);
    ~KClientPing(void);
    void AddPing( IN const DWORD dwClientPing_ );
    DWORD GetPing(); // 평균 핑 구하기
    DWORD GetPingGap();
    void UpdateLastTick();
    bool IsDelayPingTick( IN const DWORD dwUpdateTickGap_ );
    bool IsLowSpeedPing( IN const DWORD dwLimitPingSpeed_ );
    void GetAvgPingInfo( boost::array<DWORD,MAX_PING_COUNT>& avgPingInfo );
private:
    void InitPingInfo( IN const DWORD dwClinetPing_ );

private:
    bool    m_bFirst;
    int     m_nCurrentIndex; // 평균 구할 때 사용하기
    DWORD   m_dwLastRecvPingTick;
    boost::array<DWORD,MAX_PING_COUNT> m_aPingInfo;
    bool    m_bOverOneCycle;
};
