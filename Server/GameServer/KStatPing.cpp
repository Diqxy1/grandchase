#include "KStatPing.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include <dbg/dbg.hpp>
//FILE_NAME_FOR_LOG
KStatPing::KStatPing(void)
{
    m_mapPingData.clear();
}

KStatPing::~KStatPing(void)
{
}

void KStatPing::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    //m_kAvgPing.CloseFile();
    //m_bFileExist = m_kAvgPing.OpenFile( std::wstring( L"AvgPing" ) );
}

void KStatPing::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    START_LOG( clog , L" m_mapPingData : GetDataSize : " << m_mapPingData.size() ) << END_LOG;

    Flush();
    ResetTime();
}

void KStatPing::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapPingData.size() << std::endl;
}

DWORD KStatPing::GetDataSize()
{
    KLocker lock( m_csStat );
    return m_mapPingData.size();
}

void KStatPing::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }
    KLocker lock( m_csStat );
    if( !m_mapPingData.empty() )
        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_PING_INSERT_DB, L"PingStatistics", dwWriteDB, 0, m_mapPingData );

    //JIF( m_bFileExist );

    //if ( !m_mapPingData.empty() ) {
    //    m_kAvgPing << L"[" << (LPCTSTR)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) << L"]";
    //    MAP_PING_STAT ::iterator mit;
    //    for ( mit = m_mapPingData.begin(); mit != m_mapPingData.end(); mit++ ) {        
    //        m_kAvgPing << L" Ping : " << mit->first.m_nPingTime
    //            << L" , Count : " << mit->second << L" | ";
    //    }
    //    m_kAvgPing << GLOG::endl;
    //}

    m_mapPingData.clear();
}

void KStatPing::AddStat( KStatPingInfo& PingStatData )
{
    // PingType에 대한 처리
    PingStatData.m_nPingTime = GetPingType( PingStatData.m_nPingTime );

    KLocker lock( m_csStat );
    // 해당 데이터에 대한 처리를 여기서 진행
    m_mapPingData[PingStatData] += 1;
}

int KStatPing::GetPingType( int nPingType_ )
{
    // Ping Data에 대해서 사용자 지정 타입으로 DB에 저장하기 위해서 구분
    if( nPingType_ < 0 )        return 0;
    if( nPingType_ <= 30)       return 30;
    if( nPingType_ <= 50  )     return 50;
    if( nPingType_ <= 70  )     return 70;
    if( nPingType_ <= 100 )     return 100;
    if( nPingType_ <= 150 )     return 150;
    if( nPingType_ <= 200 )     return 200;
    if( nPingType_ <= 250 )     return 250;
    if( nPingType_ <= 300 )     return 300;
    if( nPingType_ <= 400 )     return 400;
    if( nPingType_ <= 500 )     return 500;
    if( nPingType_ <= 600 )     return 600;
    if( nPingType_ <= 700 )     return 700;
    if( nPingType_ <= 800 )     return 800;
    if( nPingType_ <= 900 )     return 900;
    if( nPingType_ <= 1000 )    return 1000;
    return 2000;
}