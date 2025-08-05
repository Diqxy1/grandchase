#pragma once
#include "Statistics.h"
#include <fstream>
#include <time.h>
#include "GenericLogfile.h"

struct KCheckInInfo
{
    time_t         m_tmStarTime;
    std::map< int, int > m_mapEventCount;

    KCheckInInfo()
    {
        ::time( &m_tmStarTime );
        m_mapEventCount.clear();
    }
};

SmartPointer( KCheckInStat );
class KCheckInStat : public KStatistics
{
public:
    KCheckInStat(void);
    ~KCheckInStat(void);

    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Flush();
    void AddStat( int nCase );

protected:
    void Clear();
    bool                m_bFileExist;
    KCheckInInfo        m_kCountInfo;
    KGenericLogfile     m_kLog_InOut;
};
