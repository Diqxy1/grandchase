#pragma once
#include "Statistics.h"
#include <map>
SmartPointer( KStatGameLoading );
class KStatGameLoading : public KStatistics
{
public:
    KStatGameLoading(void);
    ~KStatGameLoading(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( DWORD dwElapsedSecond ); // ms

protected:
    DWORD GetDataSize();
    std::map<DWORD, DWORD>  m_mapGameLoadingStat; // <Time, Count>
};
