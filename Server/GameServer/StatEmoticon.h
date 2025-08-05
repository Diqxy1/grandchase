#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include <map>

SmartPointer( KStatEmoticon );
class KStatEmoticon : public KStatistics
{
public:
    KStatEmoticon(void);
    ~KStatEmoticon(void);
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nLv );
    void ClearData();

protected:
    std::map< int, DWORD >  m_mapEmoticonStat; // BestCharLv : Count
    KGenericLogfile         m_klogFile;
};
