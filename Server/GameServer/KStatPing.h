#pragma once
#include "statistics.h"
#include <map>
#include "GenericLogfile.h"

SmartPointer( KStatPing );

struct KStatPingInfo;
class KStatPing : public KStatistics
{
public:
    KStatPing(void);
    ~KStatPing(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Flush();
    void AddStat( KStatPingInfo& PingStatData );
    int GetPingType( int nPingType );

protected:
    DWORD GetDataSize();
    //bool m_bFileExist;
    std::map< KStatPingInfo, DWORD > m_mapPingData;
    KGenericLogfile m_kAvgPing;
};
