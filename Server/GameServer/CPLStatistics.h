#pragma once
#include ".\statistics.h"
#include <map>

SmartPointer( KCPLStatistics );
struct KStatKeyCPL; // 전방 선언
class KCPLStatistics : public KStatistics
{
public:
    KCPLStatistics(void);
    ~KCPLStatistics(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( std::map<KStatKeyCPL, std::pair<DWORD,UINT64> >& mapData );

protected:
    DWORD GetDataSize();
    std::map<KStatKeyCPL, std::pair<DWORD,UINT64> >    m_mapCPLData;
};
