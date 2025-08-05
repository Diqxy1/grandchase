#pragma once
#include ".\statistics.h"
#include <map>
SmartPointer( KPlayTimeStat );

struct KStatKeyPlayTime; //전방 선언
class KPlayTimeStat : public KStatistics
{
public:
    KPlayTimeStat(void);
    virtual ~KPlayTimeStat(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );

    void Flush();
    void AddStat( std::map<KStatKeyPlayTime, std::pair<DWORD,DWORD> >& mapData );

protected:
    DWORD GetDataSize();
    std::map<KStatKeyPlayTime, std::pair<DWORD,DWORD> >    m_mapPlayeTimeData;
};
