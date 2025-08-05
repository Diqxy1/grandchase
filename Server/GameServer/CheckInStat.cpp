#include ".\checkinstat.h"
#include <dbg/dbg.hpp>
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

KCheckInStat::KCheckInStat(void)
:m_bFileExist(false)
{
}

KCheckInStat::~KCheckInStat(void)
{
}

void KCheckInStat::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_kLog_InOut.CloseFile();
    m_bFileExist = m_kLog_InOut.OpenFile( std::wstring ( L"CheckInCount" ) );

}

void KCheckInStat::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KCheckInStat::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );

    stm_ << L"CheckIn Err Kind : " <<  m_kCountInfo.m_mapEventCount.size() << std::endl;
}

void KCheckInStat::Flush()
{
    // 파일에다 기록 한다.
    KLocker lock( m_csStat );
    JIF( m_bFileExist );
    m_kLog_InOut << L"[" << (LPCTSTR)CTime( m_kCountInfo.m_tmStarTime ).Format( KNC_TIME_FORMAT )
        << "-" << KNC_TIME_STRING << L"]"
        << GLOG::endl;

    // EventID : 해당 이벤트 ID
    std::map< int, int >::const_iterator mit;
    for( mit = m_kCountInfo.m_mapEventCount.begin() ; mit != m_kCountInfo.m_mapEventCount.end() ; ++mit )
    {
        m_kLog_InOut << KUserEvent::GetEventIDString( static_cast<unsigned short>( mit->first ) ) << L", count : " << mit->second << GLOG::endl;
    }

    // 클리어 한다.
    Clear();
}

void KCheckInStat::AddStat( int nCase_ )
{
    JIF( m_bFileExist );
    KLocker lock( m_csStat );

    if( nCase_ < 0 )
        return;

    std::map< int, int >::iterator mit;
    mit = m_kCountInfo.m_mapEventCount.find( nCase_ );

    if( mit != m_kCountInfo.m_mapEventCount.end() )
    {
        ++mit->second;
    }
    else
    {
        m_kCountInfo.m_mapEventCount.insert( std::make_pair(nCase_,1) );
    }
}


void KCheckInStat::Clear()
{
    KLocker lock( m_csStat );

    m_kCountInfo.m_mapEventCount.clear();
    ::time( &m_kCountInfo.m_tmStarTime );
}