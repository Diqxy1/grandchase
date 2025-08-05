#include ".\statspx.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatSpx::KStatSpx(void)
{
}

KStatSpx::~KStatSpx(void)
{
}
void KStatSpx::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatSpx::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapSpxData.size() << std::endl;
}

DWORD KStatSpx::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapSpxData.size();
}

void KStatSpx::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapSpxData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_SPX_COUNT, L"SPXStatistics", dwWriteDB, 0, m_mapSpxData );
    m_mapSpxData.clear();
}

void KStatSpx::AddStat( int nGameMode_, char cCharType_, int nLv_, int nSPLv_, int nMaxSPLv_ )
{
    KStatKeySPX kKey;
    kKey.m_nModeID      = nGameMode_;
    kKey.m_cCharType    = cCharType_;
    kKey.m_nLevel       = nLv_;
    kKey.m_nSPLevel     = nSPLv_;
    kKey.m_nMaxSpx      = nMaxSPLv_;

    KLocker lock( m_csStat );
    m_mapSpxData[kKey] += 1;

}