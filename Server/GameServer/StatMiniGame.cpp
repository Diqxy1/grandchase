#include ".\statminigame.h"
#include <dbg/dbg.hpp>
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatMiniGame::KStatMiniGame(void)
{
}

KStatMiniGame::~KStatMiniGame(void)
{
}

void KStatMiniGame::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatMiniGame::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapMiniGameData.size() << std::endl;

}

void KStatMiniGame::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapMiniGameData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_MINIGAME_INSERT, L"PlayTimeStatistic", dwWriteDB, 0, m_mapMiniGameData );
    m_mapMiniGameData.clear();
}

void KStatMiniGame::AddStat( std::map<KStatKeyMiniGame, std::pair<DWORD,DWORD> >& mapData_ )
{
    MAP_MINIGAME_STAT::iterator mit;
    for( mit = mapData_.begin() ; mit != mapData_.end() ; ++mit )
    {
        KLocker lock( m_csStat );
        m_mapMiniGameData[mit->first].first += mit->second.first; // pair 첫번째 데이터 , users
        m_mapMiniGameData[mit->first].second += mit->second.second; // pair 두번째, min
        
    }
}

void KStatMiniGame::AddStat( KStatKeyMiniGame kKey_, DWORD dwElapsedMinute_ )
{
    MAP_MINIGAME_STAT::iterator mit;
    
    KLocker lock( m_csStat );
    m_mapMiniGameData[kKey_].first += 1; // pair 첫번째 데이터 , users
    m_mapMiniGameData[kKey_].second += dwElapsedMinute_; // pair 두번째, minute
}

DWORD KStatMiniGame::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapMiniGameData.size();
}