#pragma once
#include "Statistics.h"
#include <fstream>
#include <time.h>
#include "GenericLogfile.h"
#include "UserPacket.h"

SmartPointer( KLoginOutStat );
class KLoginOutStat : public KStatistics
{
public:
    enum STATTYPE
    {
        CCU = 1,
        LOGIN,
        LOGOUT,
        LOGIN_MIGRATION,
        LOGOUT_MIGRATION,
        ZOMBIE_OUT,
        LOGIN_RECONNECT,
        LOGIN_CCU,
        PCBANG_LOGOUT,
        STAT_MAX,
    };

public:
    KLoginOutStat(void);
    ~KLoginOutStat(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Flush();
    void AddStat( int nCase );
protected:
    void Clear();
    bool                m_bFileExist;
    KCountInfo          m_kCountInfo;
    KGenericLogfile     m_kLog_InOut;
};
