#pragma once
#include ".\statistics.h"
#include <map>
SmartPointer( KMapStatistics );

class KMapStatistics : public KStatistics
{
public:
    KMapStatistics(void);
    virtual ~KMapStatistics(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nMapID );

protected:
    DWORD GetDataSize();
    std::map<int, DWORD >   m_mapMapData;
};
