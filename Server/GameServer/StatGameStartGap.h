#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include <map>

SmartPointer( KStatGameStartGap );
class KStatGameStartGap : public KStatistics
{
public:
    KStatGameStartGap(void);
    ~KStatGameStartGap(void);

    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( DWORD dwTimeGap );

protected:
    void ClearData();
    std::map<DWORD,DWORD>   m_mapGapData;
    KGenericLogfile         m_klogFileGap;
};
