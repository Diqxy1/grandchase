#include "StatDisconnect.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"



KStatDisconnect::KStatDisconnect(void)
{
}

KStatDisconnect::~KStatDisconnect(void)
{
}

void KStatDisconnect::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap &&
        GetDataSize() < m_dwRowCount )
        return;

    Flush();
    ResetTime();
}

void KStatDisconnect::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapData.size() << std::endl;
}

DWORD KStatDisconnect::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapData.size();
}

void KStatDisconnect::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_DISCONNECT_COUNT, L"DisconnectStatistics", dwWriteDB, 0, m_mapData );

    m_klogFile << KNC_TIME_STRING << L", Exit count : " 
        << m_mapData[DRW_NORMAL_EXIT]
        << GLOG::endl;

    m_mapData.clear();

}

void KStatDisconnect::AddStat( int nType_ )
{
    KLocker lock( m_csStat );
    m_mapData[nType_] += 1;

}

void KStatDisconnect::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    m_klogFile.CloseFile();
    m_klogFile.OpenFile( std::wstring( L"NormalExit" ) );

}