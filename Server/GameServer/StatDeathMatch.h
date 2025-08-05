#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include <map>


struct KDeathKey
{
    bool    m_bIntrudable;
    bool    m_bBalancing;
    int     m_nKillNum;
    int     m_nTime;

    bool operator < ( const KDeathKey& right ) const
    {
        if( (m_bIntrudable && !right.m_bIntrudable) ) return false;
        else if( (!m_bIntrudable && right.m_bIntrudable) ) return true;

        if( (m_bBalancing && !right.m_bBalancing) ) return false;
        else if( (!m_bBalancing && right.m_bBalancing) ) return true;

        if( m_nKillNum > right.m_nKillNum ) return false;
        else if( m_nKillNum < right.m_nKillNum) return true;

        if( m_nTime > right.m_nTime ) return false;
        else if( m_nTime < right.m_nTime ) return true;
        return false;
    }

private:
    bool operator == ( const KDeathKey& right ) const;
    bool operator >= ( const KDeathKey& right ) const;
    bool operator <= ( const KDeathKey& right ) const;
    bool operator > ( const KDeathKey& right ) const;
};

SmartPointer( KStatDeathMatch );
class KStatDeathMatch : public KStatistics
{
public:
    KStatDeathMatch(void);
    ~KStatDeathMatch(void);

    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nKill, int nTime, bool Intrudable, bool Balancing  ); // StartGame에서 하면 될듯..
    void AddStatIntrudeTime( DWORD dwIntrudeTime );
    void AddStatGap( DWORD serdin, DWORD dwCanaban );

protected:
    std::map< KDeathKey, DWORD >    m_mapDeathMatch; // < [Kill:Time], Count >
    KGenericLogfile                 m_klogFile;

    std::map<DWORD,DWORD>           m_mapIntrudeTime;
    KGenericLogfile                 m_klogFileIntrud;

    std::map<std::pair<char,char>,DWORD>        m_mapKillGap;
    KGenericLogfile                             m_klogFileKillGap;
};
