#include ".\statp2pcritical.h"
#include <dbg/dbg.hpp>

KStatP2PCritical::KStatP2PCritical(void)
{
}

KStatP2PCritical::~KStatP2PCritical(void)
{
}

void KStatP2PCritical::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogRead.CloseFile();
    m_klogRead.OpenFile( std::wstring ( L"P2P_Read" ) );
    m_mapRead.clear();

    m_klogReadComp.CloseFile();
    m_klogReadComp.OpenFile( std::wstring ( L"P2P_ReadComp" ) );
    m_mapReadComp.clear();

    m_klogReadAligned.CloseFile();
    m_klogReadAligned.OpenFile( std::wstring ( L"P2P_ReadAligned" ) );
    m_mapReadAligned.clear();
}

void KStatP2PCritical::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatP2PCritical::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Size : " << m_mapRead.size() << L" : " << m_mapReadComp.size() << L" : " << m_mapReadAligned.size() <<  std::endl;
}

void KStatP2PCritical::Flush()
{
    KLocker lock( m_csStat );
    FlushToFile( m_mapRead,m_klogRead, std::wstring(L"Num,Count") );
    FlushToFile( m_mapReadComp, m_klogReadComp, std::wstring(L"Num,Count") );
    FlushToFile( m_mapReadAligned, m_klogReadAligned, std::wstring(L"Num,Count") );
}

void KStatP2PCritical::AddStat( DWORD dwRead_, DWORD dwReadComp_, DWORD dwReadAligned_ )
{
    KLocker lock( m_csStat );
    if( dwRead_ > 0 ) m_mapRead[dwRead_] += 1;

    if( dwReadComp_ > 0 ) m_mapReadComp[dwReadComp_] += 1;

    if( dwReadAligned_ > 0 ) m_mapReadAligned[dwReadAligned_] += 1;

}

void KStatP2PCritical::FlushToFile( std::map<DWORD,DWORD>& mapData_, KGenericLogfile& logFile_, std::wstring& strTile_ )
{
    if( mapData_.empty() ) return;
    logFile_ << KNC_TIME_STRING << GLOG::endl;
    logFile_ << strTile_ << GLOG::endl;
    std::map<DWORD,DWORD>::iterator mit;
    for( mit = mapData_.begin() ; mit != mapData_.end() ; ++mit )
    {
        logFile_<< mit->first << L"," << mit->second << GLOG::endl;
    }
    mapData_.clear();

}