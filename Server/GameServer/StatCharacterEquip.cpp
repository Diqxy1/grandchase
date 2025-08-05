#include "StatCharacterEquip.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatCharacterEquip::KStatCharacterEquip(void)
{
}

KStatCharacterEquip::~KStatCharacterEquip(void)
{
}

void KStatCharacterEquip::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatCharacterEquip::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapCharEquip.size() << std::endl;
}

DWORD KStatCharacterEquip::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapCharEquip.size();
}

void KStatCharacterEquip::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );
    if( m_mapCharEquip.empty() ) {
        return;
    }

    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_CHAR_EQUIP_NOT, L"CharacterEquipStatistic", dwWriteDB, 0, m_mapCharEquip );
    m_mapCharEquip.clear();
}

void KStatCharacterEquip::AddStat( const KCharLvPromotion& kKey_, const std::map<GCITEMID,DWORD>& mapStat_ )
{
    KLocker lock( m_csStat );
    std::map<GCITEMID,DWORD>::const_iterator cmit;
    for ( cmit = mapStat_.begin() ; cmit != mapStat_.end() ; ++cmit ) {
        m_mapCharEquip[kKey_][cmit->first] += cmit->second;
    }
}