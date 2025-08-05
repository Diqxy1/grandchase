#include ".\mapstatistics.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KMapStatistics::KMapStatistics(void)
{
}

KMapStatistics::~KMapStatistics(void)
{
}

void KMapStatistics::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KMapStatistics::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapMapData.size() << std::endl;
}

DWORD KMapStatistics::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapMapData.size();
}

void KMapStatistics::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapMapData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_MAP_COUNT, L"MapStatistics", dwWriteDB, 0, m_mapMapData );
    m_mapMapData.clear();
}

void KMapStatistics::AddStat( int nMapID_ )
{
    KLocker lock( m_csStat );
    m_mapMapData[nMapID_] += 1;
}