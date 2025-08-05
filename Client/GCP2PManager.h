#pragma once

//#include <Windows.h>
//#include <map>

class GCP2PManager
{
	struct __PEER_INFO
	{
		DWORD dpnid;
		bool  bLoadingComplete;
		__PEER_INFO()
		{
			dpnid = 0;
			bLoadingComplete = false;
		}
	};
public:
	GCP2PManager(void);
	~GCP2PManager(void);

	// Create & Destory Player
	void OnCreatePlayer(DWORD dpnid);
	void OnDestoryPlayer( DWORD dpnid );
	void ClearAllPeer() { m_mapPeer.clear(); }	

	// Size of Peer
	int GetPeerSize() { return (int)m_mapPeer.size(); }	

	// About Loading Complete
	void SetPlayerLoadingComplete( DWORD dpnid );
	bool GetPlayerLoadingComplete( DWORD dpnid );
	void ClearLoadingComplete();	
	bool IsAllPlayerLoadingComplete();

	// Host DPNID
	void SetHostDPNID(DWORD dpnid) { m_dwHostDPNID = dpnid; }
	DWORD GetHostDPNID() { return m_dwHostDPNID; }
	
	void SetLocalDPNID( DWORD dpnid ) { m_dwLocalDPNID = dpnid; }
	DWORD GetLocalDPNID() { return m_dwLocalDPNID; }

private:
	std::map<DWORD,__PEER_INFO> m_mapPeer;
	DWORD m_dwLocalDPNID;
	DWORD m_dwHostDPNID;
};
