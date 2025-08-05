#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "CommonPacket.h"
#include "KncTimer.h"

class KSwitchUserSID
{
	DeclareSingleton(KSwitchUserSID);
	NiDeclareRootRTTI(KSwitchUserSID);
	DeclToStringW;

public:
	KSwitchUserSID(void);
	~KSwitchUserSID(void);

	void Tick();

	void SetSlave(IN const KAgentServerInfo& kInfo);
	bool GetSlave(IN const int& nSID, OUT KAgentServerInfo& kInfo);
	int GetAvailableSlaveSID();
	bool UpdateSlaveAvailable(IN const int& nSID, IN const bool& bAvailable);

	void SetUserSID(IN const DWORD& dwUserUID, IN const int& nSID, OUT int& nPrevSID);
	int GetUserSID(IN const DWORD& dwUserUID);

	void SetUserConnect(IN bool bVal, IN const DWORD& dwUserUID, IN const time_t& tmRecvTime);
	bool IsConnectUser(IN const DWORD& dwUserUID);

	void DumpAgentSlave();
	void DumpUser(IN const DWORD& dwUserUID);
	void GetConnectUserCount(OUT USHORT& usCount) const;

	void SetDestroySetting(IN const bool& bVal) { KLocker lock(m_csSwitch); m_bDestroyAgit = bVal; }
	bool IsDestroySetting() { KLocker lock(m_csSwitch); return m_bDestroyAgit; }

private:
	KncTimer                                m_kMinTimer;

	mutable KncCriticalSection              m_csSwitch;
	std::map<int, KAgentServerInfo>          m_mapAgentSlave;
	std::map<DWORD, int>                     m_mapUserSID; // map[UserUID, AgentSID]
	std::map<DWORD, std::pair<bool, time_t> > m_mapConnectUserUID; // map[UserUID, pair[IsConnect,LastRecvTime] ]
	bool                                    m_bDestroyAgit;
};
DefSingletonInline(KSwitchUserSID);
DeclOstmOperatorA(KSwitchUserSID);