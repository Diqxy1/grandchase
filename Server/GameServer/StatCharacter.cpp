#include "StatCharacter.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatCharacter::KStatCharacter(void)
{
}

KStatCharacter::~KStatCharacter(void)
{
}

void KStatCharacter::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatCharacter::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Data Size : " << m_mapStat.size() << std::endl;
}

void KStatCharacter::Flush()
{
    DWORD dwWriteDB = 0;
    if ( m_bWriteToDB ) {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );
    if( !m_mapStat.empty() ) {
        KDB_EVENT_UPDATE_CHARACTER_STAT_NOT kDBPacket;
        kDBPacket.swap( m_mapStat );
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_CHARACTER_STAT_NOT, L"CharacterStatics", dwWriteDB, 0, kDBPacket );
    }
}

void KStatCharacter::AddStat( IN const std::map<std::pair<int,int>, KCharacterStats>& mapStat_ )
{
    std::map< std::pair<int,int>, KCharacterStats >::const_iterator cmit;
    for ( cmit = mapStat_.begin() ; cmit != mapStat_.end() ; ++cmit ) {
        const int& nSkillID = cmit->first.first;
        const int& nCharLv = cmit->first.second;
        const int& nModeType = cmit->second.m_nModeType;
        const int& nAccCount = cmit->second.m_nAccCount;

        if ( nAccCount <= 0 ) {
            continue;
        }

        KLocker lock( m_csStat );
        m_mapStat[ std::make_pair(nSkillID,nCharLv) ][nModeType] += nAccCount;
    }
}

DWORD KStatCharacter::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapStat.size();
}
