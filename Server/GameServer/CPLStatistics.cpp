#include ".\cplstatistics.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KCPLStatistics::KCPLStatistics(void)
{
}

KCPLStatistics::~KCPLStatistics(void)
{
}


void KCPLStatistics::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
         GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KCPLStatistics::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapCPLData.size() << std::endl;
}

DWORD KCPLStatistics::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapCPLData.size();
}

void KCPLStatistics::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapCPLData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_CPL_INSERT, L"CPLStatistics", dwWriteDB, 0, m_mapCPLData );
    m_mapCPLData.clear();
}

void KCPLStatistics::AddStat( std::map<KStatKeyCPL, std::pair<DWORD,UINT64> >& mapData_ )
{
    MAP_CPL_STAT::iterator mit;
    for( mit = mapData_.begin() ; mit != mapData_.end() ; ++mit )
    {
        KLocker lock( m_csStat );
        m_mapCPLData[mit->first].first += mit->second.first;
        m_mapCPLData[mit->first].second += mit->second.second;
    }
}