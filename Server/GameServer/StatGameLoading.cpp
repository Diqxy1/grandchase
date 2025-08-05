#include ".\statgameloading.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatGameLoading::KStatGameLoading(void)
{
}

KStatGameLoading::~KStatGameLoading(void)
{
}

void KStatGameLoading::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}
void KStatGameLoading::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapGameLoadingStat.size() << std::endl;
}
void KStatGameLoading::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );
    if( !m_mapGameLoadingStat.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_DB_STAT_GAME_LOADING_TIME, L"StatGameLoading", dwWriteDB, 0, m_mapGameLoadingStat );
    m_mapGameLoadingStat.clear();

}
void KStatGameLoading::AddStat( DWORD dwElapsedSecond_ )
{
    dwElapsedSecond_ /= 1000;
    if( dwElapsedSecond_ > 30 )
        dwElapsedSecond_ = 31;

    KLocker lock( m_csStat );
    m_mapGameLoadingStat[dwElapsedSecond_] += 1;
}

DWORD KStatGameLoading::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapGameLoadingStat.size();
}