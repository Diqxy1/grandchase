#include ".\statemoticon.h"
#include <dbg/dbg.hpp>

KStatEmoticon::KStatEmoticon(void)
{
}

KStatEmoticon::~KStatEmoticon(void)
{
}

void KStatEmoticon::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogFile.CloseFile();
    m_klogFile.OpenFile( std::wstring ( L"EmoticonLog" ) );
    ClearData();

}

void KStatEmoticon::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatEmoticon::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapEmoticonStat.size() << std::endl;
}

void KStatEmoticon::Flush()
{
    KLocker lock( m_csStat );
    m_klogFile << KNC_TIME_STRING << GLOG::endl;

    std::map< int, DWORD >::iterator mit;
    m_klogFile << L"Level,Count" << GLOG::endl;
    for( mit = m_mapEmoticonStat.begin() ; mit != m_mapEmoticonStat.end() ; ++mit )
    {
        m_klogFile << mit->first << L"," << mit->second << GLOG::endl;
    }
    m_klogFile << GLOG::endl << GLOG::endl;
    ClearData();
}

void KStatEmoticon::AddStat( int nLv_ )
{
    KLocker lock( m_csStat );
    m_mapEmoticonStat[nLv_] += 1;
}

void KStatEmoticon::ClearData()
{
    m_mapEmoticonStat.clear();
    for( int i = 0 ; i <= 75 ; ++i )
    {
        m_mapEmoticonStat[i] = 0;
    }

}