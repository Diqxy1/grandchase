#include "StatLeaveGame.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatLeaveGame::KStatLeaveGame(void)
{
}

KStatLeaveGame::~KStatLeaveGame(void)
{
}

void KStatLeaveGame::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatLeaveGame::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapStatData.size() << std::endl;
}

DWORD KStatLeaveGame::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapStatData.size();
}

void KStatLeaveGame::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapStatData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_LEAVE_GAME_COUNT, L"LeaveGameStatistics", dwWriteDB, 0, m_mapStatData );
    m_mapStatData.clear();
}

void KStatLeaveGame::AddStat( int nGameMode_, int nDifficulty_, int nPlayTime_, char cLeftLife_, char cCharType_, unsigned char ucLv_  )
{
    KStatKeyLeaveGame kKey;
    kKey.m_nGameMode       = nGameMode_;
    kKey.m_nDifficulty     = nDifficulty_;
    kKey.m_nPlayTime       = nPlayTime_;
    kKey.m_cLeftLife       = cLeftLife_;
    kKey.m_cCharType       = cCharType_;
    kKey.m_ucLv            = ucLv_;

    KLocker lock( m_csStat );
    m_mapStatData[kKey] += 1;
}