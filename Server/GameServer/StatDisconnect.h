#pragma once
#include "Statistics.h"
#include <map>
#include "GenericLogfile.h"

SmartPointer( KStatDisconnect );
class KStatDisconnect : public KStatistics
{
public:
    KStatDisconnect(void);
    ~KStatDisconnect(void);
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nType );


#undef _ENUM
#define _ENUM( id ) id,
    enum DISCONNECT_REASON_WHY
    {
#include "../Common/Socket/DisconnectReason_def.h"
    };

protected:
    DWORD GetDataSize();
    std::map<int,DWORD>     m_mapData;
    KGenericLogfile         m_klogFile;

};
