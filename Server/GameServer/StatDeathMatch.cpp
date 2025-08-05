#include ".\statdeathmatch.h"
#include <dbg/dbg.hpp>

KStatDeathMatch::KStatDeathMatch(void)
{
}

KStatDeathMatch::~KStatDeathMatch(void)
{
}

void KStatDeathMatch::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogFile.CloseFile();
    m_klogFile.OpenFile( std::wstring ( L"DeathMatch" ) );
    m_mapDeathMatch.clear();

    m_klogFileIntrud.CloseFile();
    m_klogFileIntrud.OpenFile( std::wstring ( L"IntrudeTime" ) );
    m_mapIntrudeTime.clear();

    m_klogFileKillGap.CloseFile();
    m_klogFileKillGap.OpenFile( std::wstring ( L"DeathKillGap" ) );
    m_mapKillGap.clear();

}

void KStatDeathMatch::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatDeathMatch::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Size : " << m_mapDeathMatch.size() << L" : " << m_mapIntrudeTime.size() << L" : " << m_mapKillGap.size() <<  std::endl;
}
void KStatDeathMatch::Flush()
{
    KLocker lock( m_csStat );
    if( !m_mapDeathMatch.empty() )
    {
        m_klogFile << KNC_TIME_STRING << GLOG::endl;
        std::map< KDeathKey, DWORD >::iterator mit;
        m_klogFile << L"Intrudable,Balancing,Kill,Time,Count" << GLOG::endl;
        for( mit = m_mapDeathMatch.begin() ; mit != m_mapDeathMatch.end() ; ++mit )
        {
            m_klogFile << mit->first.m_bIntrudable << L","
                << mit->first.m_bBalancing << L","
                << mit->first.m_nKillNum << L","
                << mit->first.m_nTime << L","
                << mit->second << GLOG::endl;
        }
        m_mapDeathMatch.clear();
    }

    if( !m_mapIntrudeTime.empty() )
    {
        m_klogFileIntrud << KNC_TIME_STRING << GLOG::endl;
        std::map< DWORD, DWORD >::iterator mit;
        m_klogFileIntrud << L"IntrudTime,Count" << GLOG::endl;
        for( mit = m_mapIntrudeTime.begin() ; mit != m_mapIntrudeTime.end() ; ++mit )
        {
            m_klogFileIntrud << mit->first << L"," << mit->second << GLOG::endl;
        }
        m_mapIntrudeTime.clear();
    }

    if( !m_mapKillGap.empty() )
    {
        m_klogFileKillGap << KNC_TIME_STRING << GLOG::endl;
        std::map<std::pair<char,char>,DWORD>::iterator mit;
        m_klogFileKillGap << L"serdin,canaban,Count" << GLOG::endl;
        for( mit = m_mapKillGap.begin() ; mit != m_mapKillGap.end() ; ++mit )
        {
            m_klogFileKillGap << (int)mit->first.first << L","<< (int)mit->first.second << L"," << mit->second << GLOG::endl;
        }
        m_mapKillGap.clear();
    }
}

void KStatDeathMatch::AddStat( int nKill_, int nTime_, bool Intrudable_, bool Balancing_ )
{
    KDeathKey kKey;

    kKey.m_bIntrudable  = Intrudable_;
    kKey.m_bBalancing   = Balancing_;
    kKey.m_nKillNum     = nKill_;
    kKey.m_nTime        = nTime_;

    KLocker lock( m_csStat );
    m_mapDeathMatch[kKey] += 1;
}

void KStatDeathMatch::AddStatIntrudeTime( DWORD dwIntrudeTime_ )
{
    KLocker lock( m_csStat );
    m_mapIntrudeTime[dwIntrudeTime_] += 1;
}

void KStatDeathMatch::AddStatGap( DWORD serdin_, DWORD dwCanaban_ )
{
    std::pair<char,char> prKey;
    prKey.first = (char) serdin_;
    prKey.second = (char) dwCanaban_;
    KLocker lock( m_csStat );
    m_mapKillGap[prKey] += 1;
}