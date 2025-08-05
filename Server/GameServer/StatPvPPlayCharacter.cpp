#include "StatPvPPlayCharacter.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatPvPPlayCharacter::KStatPvPPlayCharacter(void)
{
    m_vecPvPPlayCharacterCountData.clear();
}

KStatPvPPlayCharacter::~KStatPvPPlayCharacter(void)
{
}

void KStatPvPPlayCharacter::Tick()
{
    if ( ::GetTickCount() - m_dwStartTime < m_dwFlushGap && GetDataSize() < m_dwRowCount ) {
        return;
    }

    Flush();
    ResetTime();
}

void KStatPvPPlayCharacter::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );

    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_vecPvPPlayCharacterCountData.size() << std::endl;
}

DWORD KStatPvPPlayCharacter::GetDataSize()
{
    KLocker lock( m_csStat );

    return m_vecPvPPlayCharacterCountData.size();
}

void KStatPvPPlayCharacter::Flush()
{
    DWORD dwWriteDB = 0;

    if ( true == m_bWriteToDB ) {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );

    if ( false == m_vecPvPPlayCharacterCountData.empty() ) {
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT, L"PvPPlayCharacterStatistics", dwWriteDB, 0, m_vecPvPPlayCharacterCountData );

        m_vecPvPPlayCharacterCountData.clear();
    }
}

void KStatPvPPlayCharacter::AddStat( IN const time_t tmStartTime_, IN const int nModeID_, IN const std::vector< int >& vecCharTypes_ )
{
    KLocker lock( m_csStat );

    KStatPvPPlayCharacterInfo kStat;
    kStat.m_tmStartTime     = tmStartTime_;
    kStat.m_nModeID         = nModeID_;
    kStat.m_vecCharTypes    = vecCharTypes_;

    m_vecPvPPlayCharacterCountData.push_back( kStat );
}
