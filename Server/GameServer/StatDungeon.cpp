#include "StatDungeon.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatDungeon::KStatDungeon(void)
{
}

KStatDungeon::~KStatDungeon(void)
{
}

void KStatDungeon::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatDungeon::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Data Size : " << m_mapDungeonStat.size() << std::endl;
}

void KStatDungeon::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );
    if( !m_mapDungeonStat.empty() || m_vecBossDungeon.empty() ) {
        KDB_EVENT_UPDATE_DUNGEON_STAT_NOT kDBPacket;
        kDBPacket.m_mapDungeonStat.swap( m_mapDungeonStat );
        kDBPacket.m_vecBossDungeonStat.swap( m_vecBossDungeon );
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_DUNGEON_STAT_NOT, L"DungeonStatics", dwWriteDB, 0, kDBPacket );
    }
}

void KStatDungeon::AddStat( IN const KDungeonStat& kData_ )
{
    KLocker lock( m_csStat );
    ++m_mapDungeonStat[kData_];
}

DWORD KStatDungeon::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapDungeonStat.size();
}

void KStatDungeon::AddBossStat( IN const KBossDungeonStat& kData_ )
{
    KLocker lock( m_csStat );
    m_vecBossDungeon.push_back( kData_ );
}