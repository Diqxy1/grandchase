#include ".\statitemget.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatItemGet::KStatItemGet(void)
{
}

KStatItemGet::~KStatItemGet(void)
{
}

void KStatItemGet::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatItemGet::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapItemGetData.size() << std::endl;
}

DWORD KStatItemGet::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapItemGetData.size();
}

void KStatItemGet::Flush()
{

    KLocker lock( m_csStat );
    if( !m_mapItemGetData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_GET_ITEM, L"Item Get Stat", (m_bWriteToDB?1:0), 0, m_mapItemGetData );
    m_mapItemGetData.clear();
}

void KStatItemGet::AddStat( int nGameMode_, std::vector<KItem>& vecItem_ )
{
    std::vector<KItem>::iterator vit;
    for( vit = vecItem_.begin() ; vit != vecItem_.end() ; ++vit )
    {
        KStatKeyItem kKey;
        kKey.m_dwItemID = vit->m_ItemID;
        kKey.m_nFactor  = vit->m_nPeriod;
        kKey.m_nMode    = nGameMode_;

        KLocker lock( m_csStat );
        m_mapItemGetData[kKey] += 1;
    }
}