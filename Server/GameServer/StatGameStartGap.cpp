#include "StatGameStartGap.h"
#include <dbg/dbg.hpp>

KStatGameStartGap::KStatGameStartGap(void)
{
    ClearData();
}

KStatGameStartGap::~KStatGameStartGap(void)
{
}

void KStatGameStartGap::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogFileGap.CloseFile();
    m_klogFileGap.OpenFile( std::wstring ( L"GameStartGap" ) );
    ClearData();
}

void KStatGameStartGap::ClearData()
{
    m_mapGapData.clear();
}


void KStatGameStartGap::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatGameStartGap::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Size : " << m_mapGapData.size() << std::endl;
    ClearData();

}


void KStatGameStartGap::Flush()
{
    KLocker lock( m_csStat );
    m_klogFileGap << KNC_TIME_STRING << GLOG::endl;
    std::map<DWORD,DWORD>::const_iterator mit;
    for( mit = m_mapGapData.begin() ; mit != m_mapGapData.end() ; ++mit )
        m_klogFileGap << L"[" << mit->first * 10 << L"] : " << mit->second << GLOG::endl;
    m_klogFileGap << GLOG::endl;
    ClearData();
}

void KStatGameStartGap::AddStat( DWORD dwTimeGap_ )
{
    dwTimeGap_ /= 10000; // 10초 로 나눈다.
    KLocker lock( m_csStat );

    if( dwTimeGap_ > 30 ) // 300초 보다 크면..
        m_mapGapData[ 31 ] += 1;
    else
        m_mapGapData[dwTimeGap_] += 1;
}