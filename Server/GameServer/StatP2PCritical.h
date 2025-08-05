#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include <map>

SmartPointer( KStatP2PCritical );
class KStatP2PCritical : public KStatistics
{
public:
    KStatP2PCritical(void);
    ~KStatP2PCritical(void);

    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( DWORD dwRead, DWORD dwReadComp, DWORD dwReadAligned);

protected:
    void FlushToFile( std::map<DWORD,DWORD>& mapData, KGenericLogfile& logFile, std::wstring& strTile );

protected:
    std::map<DWORD,DWORD>           m_mapRead;
    KGenericLogfile                 m_klogRead;

    std::map<DWORD,DWORD>           m_mapReadComp;
    KGenericLogfile                 m_klogReadComp;

    std::map<DWORD,DWORD>           m_mapReadAligned;
    KGenericLogfile                 m_klogReadAligned;
};
