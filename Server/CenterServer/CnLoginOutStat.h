#pragma once
#include "Statistics.h"
#include <fstream>
#include <time.h>
#include "GenericLogfile.h"

SmartPointer( KCnLoginOutStat );
class KCnLoginOutStat : public KStatistics
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
        STAT_MAX,
    };

    struct KLoginoutCount{
        int     m_nLoginCount;
        int     m_nLogoutCount;
        DWORD   m_dwCCU;
        time_t  m_tmStartTime;

        KLoginoutCount()
            :m_nLoginCount(0)
            ,m_nLogoutCount(0)
            ,m_dwCCU(0)
            ,m_tmStartTime(0)
        {}
    };

public:
    KCnLoginOutStat(void);
    ~KCnLoginOutStat(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Flush();
    void AddStat( int nCase );
    void UpdateCCU( DWORD dwCCU_ );

protected:
    void Clear();

    bool                m_bFileExist;
    KLoginoutCount      m_kCountInfo;
    KGenericLogfile     m_kLog_InOut;
};
