#include "StatLoadingInfo.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"


KStatLoadingInfo::KStatLoadingInfo(void)
{
}

KStatLoadingInfo::~KStatLoadingInfo(void)
{
}

void KStatLoadingInfo::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap )
        return;

    Flush();
    ResetTime();
}

void KStatLoadingInfo::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " 
         << m_mapTypeSecond.size() 
         << L" / " 
         << m_mapTypeMapID.size() 
         << L" / " 
         << m_mapTypeTotalUser.size() 
         << L" / " 
         << m_mapFailTypeInfo.size() 
         << std::endl;
}

void KStatLoadingInfo::Flush()
{
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB )
    {
        dwWriteDB = 1;
    }

    KLocker lock( m_csStat );
    KLoadingInfo kLoadingInfo;

    if( !m_mapTypeSecond.empty() || !m_mapTypeMapID.empty() || !m_mapTypeTotalUser.empty() || !m_mapFailTypeInfo.empty() ) {
        KLoadingInfo kLoadingInfo;
        kLoadingInfo.mapTypeSecond.swap( m_mapTypeSecond );
        kLoadingInfo.mapTypeMapID.swap( m_mapTypeMapID );
        kLoadingInfo.mapTypeTotalUser.swap( m_mapTypeTotalUser );
        kLoadingInfo.mapFailTypeInfo.swap( m_mapFailTypeInfo );

        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_STAT_LOADING_INFO, L"StatLoadingInfo", dwWriteDB, 0, kLoadingInfo );
    }
}

void KStatLoadingInfo::AddStat( IN DWORD dwType_, IN int nSecond_, IN DWORD dwTotalUser_, IN DWORD dwMapID_, IN int dwFailType_, IN int dwFailReason_ )
{
    // 일반의 경우와 트리거로 인한 에러 이유에 대한 2가지 경우가 있음.
    if( dwFailType_ > 0 ) {    // 트리거로 실패한 경우.
        AddStatMap( dwType_, dwMapID_ );
        AddStatTotalUser( dwType_, dwTotalUser_ );
        AddStatFailInfo( dwFailType_, dwFailReason_ );
    } else {    // 그렇지 않은 모든 경우.
        AddStatSecond( dwType_, nSecond_ );
        AddStatMap( dwType_, dwMapID_ );
        AddStatTotalUser( dwType_, dwTotalUser_ );
    }
}

void KStatLoadingInfo::AddStatSecond( IN DWORD dwType_, IN int nSecond_ )
{
    KLocker lock( m_csStat );
    std::pair< DWORD, int > prKey( dwType_, nSecond_ );
    m_mapTypeSecond[prKey] += 1;
}

void KStatLoadingInfo::AddStatMap( IN DWORD dwType_, IN DWORD dwMapID_ )
{
    KLocker lock( m_csStat );
    std::pair< DWORD, DWORD > prKey( dwType_, dwMapID_ );
    m_mapTypeMapID[prKey] += 1;
}

void KStatLoadingInfo::AddStatTotalUser( IN DWORD dwType_, IN DWORD dwTotalUser_ )
{
    KLocker lock( m_csStat );
    std::pair< DWORD, DWORD > prKey( dwType_, dwTotalUser_ );
    m_mapTypeTotalUser[prKey] += 1;
}

void KStatLoadingInfo::AddStatFailInfo( IN int nFailType_, IN DWORD dwFailReason_ )
{
    KLocker lock( m_csStat );
    std::pair< int, DWORD > prKey( nFailType_, dwFailReason_ );
    m_mapFailTypeInfo[prKey] += 1;
}