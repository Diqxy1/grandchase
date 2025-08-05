#include "stdafx.h"
#include "kncpeermanager.h"
#include "KncPacket.h"
#include "dbg/dbg.hpp"
KncPeerManager::KncPeerManager(void)
{
    m_pMyPeer = NULL;
}

KncPeerManager::~KncPeerManager(void)
{
    CleanUp();
}

void KncPeerManager::CleanUp()
{
    KLocker kLocker( m_mapPeer.csKncMap );
    std::map< DWORD, KncPeer* >::iterator itm;
    for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
    {
        SAFE_DELETE( itm->second );
    }
    m_mapPeer.RemoveAll();
}

KncPeer* KncPeerManager::CreatePeer( DWORD dwUID_, const PlayerID& kRakPeerID_, bool bLocal_, bool bRelay_ )
{
    if ( m_mapPeer.IsIn( dwUID_ ) == true )
        return NULL;

    KncPeer* pPeer = new KncPeer( dwUID_ );
    pPeer->SetRakPeerID( kRakPeerID_ );
    pPeer->SetRelay( bRelay_ );
    pPeer->m_bLocal = bLocal_;
    m_mapPeer.Add( dwUID_, pPeer );

    if ( bLocal_ == true )
        m_pMyPeer = pPeer;
    START_LOG( clog2, pPeer->ToString() );
    return pPeer;
}
KncPeer* KncPeerManager::GetPeer( DWORD dwUID_ )
{
    if ( m_mapPeer.IsIn( dwUID_ ) == false )
        return NULL;
    KncPeer* pPeer = NULL;
    m_mapPeer.Get( dwUID_, &pPeer );
    return pPeer;
}
bool KncPeerManager::IsIn( DWORD dwUID_ )
{
    return m_mapPeer.IsIn( dwUID_ );
}
void KncPeerManager::SetConnectInfo( const KConnectInfo& kConnectInfo_ )
{
    m_kConnectInfo = kConnectInfo_;
}
void KncPeerManager::GetConnectInfo( KConnectInfo* pkConnectInfo_ )
{
    *pkConnectInfo_ = m_kConnectInfo;
}
void KncPeerManager::RemovePeer( DWORD dwUID_ )
{
    KncPeer* pPeer = GetPeer( dwUID_ );
    if ( pPeer != NULL )
        SAFE_DELETE( pPeer );
    m_mapPeer.Remove( dwUID_ );
}
int KncPeerManager::GetUserCount( KncUserCount::EUserCount eUserCount_ )
{
    //====================================================================================
    // 2006.07.24 : Asirion
    // Relay사용 하는 애들 카운팅 하는 모듈만 작성하면 중복 코드 없어지지만,
    // 힘빼지 말자--; 나중에 또 이런일 있으면 모듈화!
    switch( eUserCount_ ) 
    {
        case KncUserCount::ID_TOTAL:
        {
            return m_mapPeer.Size();
        }
        case KncUserCount::ID_RELAY:
        {
            int iUsers = 0;
            KLocker kLocker( m_mapPeer.csKncMap );
            std::map< DWORD, KncPeer* >::iterator itm;
            for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
            {
                if ( itm->second->IsRelay() )
                    iUsers++;
            }
            return iUsers;
        }
        case KncUserCount::ID_P2P:
        {
            int iUsers = 0;
            KLocker kLocker( m_mapPeer.csKncMap );
            std::map< DWORD, KncPeer* >::iterator itm;
            for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
            {
                if ( itm->second->IsRelay() == false )
                    iUsers++;
            }
            return iUsers;
        }
    }
    return 0;
}
std::vector< DWORD > KncPeerManager::GetPeerUID()
{
    std::vector< DWORD > vecUID;
    KLocker kLocker( m_mapPeer.csKncMap );
    std::map< DWORD, KncPeer* >::iterator itm;
    for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
    {
        vecUID.push_back( itm->second->m_dwUID );
    }
    return vecUID;
}

DWORD KncPeerManager::GetAvgPingTime( DWORD dwUID_ )
{
    if ( m_pMyPeer ) 
    {
        // 아직 Ping을 한번도 안 주고 받았다.
        if ( m_pMyPeer->GetLastSendPingTime() == 0 )
            return DEFAULT_INIT_PING;
    }

	KncPeer* pPeer = GetPeer( dwUID_ );
	if ( pPeer != NULL )
		return pPeer->GetPing();		
	return DEFAULT_INIT_PING;
}

DWORD KncPeerManager::GetAvgPingTime( bool bRelay )
{
    if ( m_pMyPeer ) 
    {
        // 아직 Ping을 한번도 안 주고 받았다.
        if ( m_pMyPeer->GetLastSendPingTime() == 0 )
            return DEFAULT_INIT_PING;
    }

	KLocker kLocker( m_mapPeer.csKncMap );
	std::map< DWORD, KncPeer* >::iterator itm;
	DWORD dwAvgPing = 0;
	int   iCount = 0;
	for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
	{
		if( itm->second->IsLocal() == true )
			continue;
        
        if( itm->second->IsRelay() != bRelay )
            continue;

		dwAvgPing += itm->second->GetPing();
		iCount++;
	}
	if ( iCount == 0 )
		return 0;
	
    dwAvgPing = (DWORD)((float)dwAvgPing/(float)iCount);
	return dwAvgPing;
}

DWORD KncPeerManager::GetAvgPartPingTime( DWORD dwUID_ )
{
    if ( m_pMyPeer ) 
    {
        // 아직 Ping을 한번도 안 주고 받았다.
        if ( m_pMyPeer->GetLastSendPingTime() == 0 )
            return DEFAULT_INIT_PING;
    }

    KncPeer* pPeer = GetPeer( dwUID_ );
    if ( pPeer != NULL )
        return pPeer->GetPartPing();		
    return DEFAULT_INIT_PING;
}

DWORD KncPeerManager::GetAvgPartPingTime()
{
    if ( m_pMyPeer ) 
    {
        // 아직 Ping을 한번도 안 주고 받았다.
        if ( m_pMyPeer->GetLastSendPingTime() == 0 )
            return DEFAULT_INIT_PING;
    }

    KLocker kLocker( m_mapPeer.csKncMap );
    std::map< DWORD, KncPeer* >::iterator itm;
    DWORD dwAvgPing = 0;
    int   iCount = 0;
    for ( itm = m_mapPeer.map.begin(); itm != m_mapPeer.map.end(); itm++ )
    {
        if( itm->second->IsLocal() == true )
            continue;
        dwAvgPing += itm->second->GetPartPing();
        iCount++;
    }
    if ( iCount == 0 )
        return 0;

    dwAvgPing = (DWORD)((float)dwAvgPing/(float)iCount);
    return dwAvgPing;
}

bool KncPeerManager::IsRelay( DWORD dwUID_ )
{
    KncPeer* pPeer = GetPeer( dwUID_ );
    if ( pPeer != NULL )
        return pPeer->IsRelay();		
    return false;
}
