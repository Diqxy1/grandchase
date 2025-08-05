#include ".\playtimestat.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KPlayTimeStat::KPlayTimeStat(void)
{
}

KPlayTimeStat::~KPlayTimeStat(void)
{
}

void KPlayTimeStat::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KPlayTimeStat::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapPlayeTimeData.size() << std::endl;
}

DWORD KPlayTimeStat::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapPlayeTimeData.size();
}

void KPlayTimeStat::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapPlayeTimeData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_PLAY_TIME_INSERT, L"PlayTimeStatistic", dwWriteDB, 0, m_mapPlayeTimeData );
    m_mapPlayeTimeData.clear();
}

void KPlayTimeStat::AddStat( std::map<KStatKeyPlayTime, std::pair<DWORD,DWORD> >& mapData_ )
{
    MAP_PLAY_TIME_STAT::iterator mit;
    for( mit = mapData_.begin() ; mit != mapData_.end() ; ++mit )
    {
        KLocker lock( m_csStat );
        m_mapPlayeTimeData[mit->first].first += mit->second.first;
        m_mapPlayeTimeData[mit->first].second += mit->second.second;
    }
}