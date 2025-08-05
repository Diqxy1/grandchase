#include "stdafx.h"
#include ".\gcp2pmanager.h"

GCP2PManager::GCP2PManager(void)
{
}

GCP2PManager::~GCP2PManager(void)
{
}

void GCP2PManager::OnCreatePlayer(DWORD dpnid)
{
	__PEER_INFO info;
	info.dpnid = dpnid;	

	m_mapPeer[dpnid] = info;
}

void GCP2PManager::OnDestoryPlayer( DWORD dpnid )
{
	std::map<DWORD,__PEER_INFO>::iterator itor = m_mapPeer.find(dpnid);
	if( m_mapPeer.end() == itor )
		return;

	m_mapPeer.erase( dpnid );
}

void GCP2PManager::SetPlayerLoadingComplete( DWORD dpnid )
{
	std::map<DWORD,__PEER_INFO>::iterator itor = m_mapPeer.find(dpnid);
	if( m_mapPeer.end() == itor )
		return;

	itor->second.bLoadingComplete = true;
}

bool GCP2PManager::GetPlayerLoadingComplete( DWORD dpnid )
{
	std::map<DWORD,__PEER_INFO>::iterator itor = m_mapPeer.find(dpnid);
	if( m_mapPeer.end() == itor )
		return false;

	return itor->second.bLoadingComplete;
}

void GCP2PManager::ClearLoadingComplete()
{
	std::map<DWORD,__PEER_INFO>::iterator itor = m_mapPeer.begin();

	while( itor != m_mapPeer.end() )
	{
		itor->second.bLoadingComplete = false;
		itor++;
	}
}

bool GCP2PManager::IsAllPlayerLoadingComplete()
{
	std::map<DWORD,__PEER_INFO>::iterator itor = m_mapPeer.begin();

	while( itor != m_mapPeer.end() )
	{
		if( !itor->second.bLoadingComplete )
			return false;

		itor++;
	}
	return true;
}