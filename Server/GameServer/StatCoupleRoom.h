#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"

SmartPointer( KStatCoupleRoom );
class KStatCoupleRoom : public KStatistics
{
public:
    KStatCoupleRoom(void);
    ~KStatCoupleRoom(void);
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( bool bCouple );
protected:
    void ClearData();
    DWORD               m_dwCoupleUserOpen;
    DWORD               m_dwOtherUserOpen;
    KGenericLogfile     m_klogFileCouple;

};
