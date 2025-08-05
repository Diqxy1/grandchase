#include "StatNetwork.h"
#include "GSDBLayer.h"
#include "UserEvent.h"

KStatNetwork::KStatNetwork(void)
{
}

KStatNetwork::~KStatNetwork(void)
{
}

void KStatNetwork::Tick()
{
    if( ::GetTickCount() - m_dwStartTime < m_dwFlushGap ) {
        return;
    }

    Flush();
    ResetTime();
}

void KStatNetwork::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Current Size : " << m_mapNetworkStat.size() << std::endl;
}

void KStatNetwork::Flush()
{
    std::map<int,int> mapNetworkStat;
    mapNetworkStat.clear();
    DWORD dwWriteDB = 0;
    if( m_bWriteToDB ) {
        dwWriteDB = 1;
    }

    {
        KLocker lock( m_csStat );
        if( !m_mapNetworkStat.empty() ) {
            mapNetworkStat.swap( m_mapNetworkStat );
        } else {
            return;
        }
    }

    // DB에 기록을 하겠다고 세팅할 경우에만 DB에 기록하자.
    if( m_bWriteToDB ) {
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_CLIENT_NETWORK_REPORT_NOT, L"NetworkStatistics", dwWriteDB, 0, mapNetworkStat );
    }
}

void KStatNetwork::AddStat( IN const int& nType_, IN const int& nCount_ )
{
    std::map<int,int>::iterator mit;
    KLocker lock( m_csStat );
    mit = m_mapNetworkStat.find( nType_ );
    if( mit == m_mapNetworkStat.end() ) {
        m_mapNetworkStat[nType_] = nCount_;
    } else {
        mit->second += nCount_;
    }
}