#pragma once
#include "GSSimLayer.h" // for KCheckIn. tslayer
#include "StatisticsManager.h"
#include "CheckInStat.h" // for stat
#include "GSDBLayer.h"
#include <set>

class KUserEventChecker
{
public:
    KUserEventChecker(void);
    ~KUserEventChecker(void);

    bool EventIn( const USHORT& usEventID );
    void EventOut( const USHORT& usEventID );

private:
    std::set<USHORT>    m_setProcessEvent; // 처리중인 EventID
};

// 일종의 크리티컬 섹션..
class KEventCheck
{
public:
    KEventCheck( const USHORT& usEventID, const DWORD& dwUID, const bool& bEnable );
    ~KEventCheck();
protected:
    unsigned short      m_usEventID;
    DWORD               m_dwUID;
};
