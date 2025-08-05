#include <WinSock2.h>
#include "SwitchUserSID.h"
#include <dbg/dbg.hpp >
#include "AgentEvent.h"
#include "AgitManager.h"

//FILE_NAME_FOR_LOG

ImplementSingleton(KSwitchUserSID);
ImplOstmOperatorW2A(KSwitchUserSID);
NiImplementRootRTTI(KSwitchUserSID);

ImplToStringW(KSwitchUserSID)
{
	KLocker lock(m_csSwitch);

	USHORT usConnectCount;
	GetConnectUserCount(usConnectCount);

	return START_TOSTRINGW
		<< TOSTRINGW(m_mapAgentSlave.size())
		<< TOSTRINGW(m_mapUserSID.size())
		<< TOSTRINGW(m_mapConnectUserUID.size())
		<< TOSTRINGW(usConnectCount);
}

KSwitchUserSID::KSwitchUserSID(void)
	:m_kMinTimer(1000 * 60)
	, m_bDestroyAgit(false)
{
}

KSwitchUserSID::~KSwitchUserSID(void)
{
}

void KSwitchUserSID::Tick()
{
	// 1분단위 체크
	if (m_kMinTimer.CheckTime()) {
		// 수용 가능한 아지트가 없으면 삭제작업 예약
		bool bDestroy = (GetAvailableSlaveSID() == -1 ? true : false);
		SetDestroySetting(bDestroy);
	}
}

//void KSwitchUserSID::

void KSwitchUserSID::SetSlave(IN const KAgentServerInfo& kInfo_)
{
	KLocker lock(m_csSwitch);

	std::map<int, KAgentServerInfo>::iterator mit;
	mit = m_mapAgentSlave.find(kInfo_.m_nSID);
	if (mit == m_mapAgentSlave.end()) {
		m_mapAgentSlave.insert(std::map<int, KAgentServerInfo>::value_type(kInfo_.m_nSID, kInfo_));
		START_LOG(clog, L"Add new AgentSlave.. NID : " << kInfo_.m_nSID) << END_LOG;
	}
	else {
		mit->second = kInfo_;
		START_LOG(clog, L"Update AgentSlave.. NID : " << kInfo_.m_nSID) << END_LOG;
	}

	START_LOG(clog, L"Set AgentSlave.. total size : " << m_mapAgentSlave.size()) << END_LOG;
}

bool KSwitchUserSID::GetSlave(IN const int& nSID_, OUT KAgentServerInfo& kInfo_)
{
	KLocker lock(m_csSwitch);

	std::map<int, KAgentServerInfo>::iterator mit;
	mit = m_mapAgentSlave.find(nSID_);
	if (mit == m_mapAgentSlave.end()) {
		START_LOG(cwarn, L"Not exist AgentSlave.. NID : " << nSID_ << L", total size : " << m_mapAgentSlave.size()) << END_LOG;
		return false;
	}

	kInfo_ = mit->second;
	return true;
}

int KSwitchUserSID::GetAvailableSlaveSID()
{
	KLocker lock(m_csSwitch);

	std::map<int, KAgentServerInfo>::iterator mit;
	for (mit = m_mapAgentSlave.begin(); mit != m_mapAgentSlave.end(); ++mit) {
		if (mit->second.m_bAvailable) {
			break;
		}
	}

	if (mit == m_mapAgentSlave.end()) {
		START_LOG(clog, L"수용가능한 AgentSlave가 없음.. AgentSlave size : " << m_mapAgentSlave.size()) << END_LOG;
		return -1;
	}

	return mit->first;
}

bool KSwitchUserSID::UpdateSlaveAvailable(IN const int& nSID_, IN const bool& bAvailable_)
{
	KLocker lock(m_csSwitch);

	std::map<int, KAgentServerInfo>::iterator mit;
	mit = m_mapAgentSlave.find(nSID_);
	if (mit == m_mapAgentSlave.end()) {
		START_LOG(cerr, L"아지트 가용상태를 갱신하려는 AgentSlave가 없음.. SID : " << nSID_) << END_LOG;
		return false;
	}

	mit->second.m_bAvailable = bAvailable_;
	START_LOG(clog, L"Update AgentSlave Available.. nSID : " << nSID_ << L", Available : " << bAvailable_) << END_LOG;
	return true;
}

void KSwitchUserSID::SetUserSID(IN const DWORD& dwUserUID_, IN const int& nSID_, OUT int& nPrevSID_)
{
	KLocker lock(m_csSwitch);

	nPrevSID_ = -1;

	std::map<DWORD, int>::iterator mit;
	mit = m_mapUserSID.find(dwUserUID_);
	if (mit == m_mapUserSID.end()) {
		m_mapUserSID.insert(std::map<DWORD, int>::value_type(dwUserUID_, nSID_));
		START_LOG(clog, L"Add new User SID.. UserUID : " << dwUserUID_ << L", SID : " << nSID_ << L", total size : " << m_mapUserSID.size()) << END_LOG;
	}
	else {
		if (mit->second != nSID_) {
			nPrevSID_ = mit->second;
		}
		mit->second = nSID_;

		START_LOG(clog, L"Update User SID.. UserUID : " << dwUserUID_ << L", SID : " << nSID_ << L", total size : " << m_mapUserSID.size()) << END_LOG;
	}

	PAIR_DWORD_INT kBroadPacket;
	kBroadPacket.first = dwUserUID_;
	kBroadPacket.second = nSID_;
	SiKAgitManager()->SendAgitPacket(KAgentEvent::EAG_SET_AGIT_SID_BROAD, kBroadPacket);

	START_LOG(clog, L"Send Agit SID Broad.. AgitUID : " << dwUserUID_ << L", nSID_ : " << nSID_ << L", nPrevSID_ : " << nPrevSID_) << END_LOG;
}

int KSwitchUserSID::GetUserSID(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csSwitch);

	std::map<DWORD, int>::iterator mit;
	mit = m_mapUserSID.find(dwUserUID_);
	if (mit == m_mapUserSID.end()) {
		START_LOG(cwarn, L"Not exist User SID.. UserUID : " << dwUserUID_ << L", total size : " << m_mapUserSID.size()) << END_LOG;
		return -1;
	}

	return mit->second;
}

void KSwitchUserSID::SetUserConnect(IN bool bVal_, IN const DWORD& dwUserUID_, IN const time_t& tmRecvTime_)
{
	KLocker lock(m_csSwitch);

	std::map<DWORD, std::pair<bool, time_t> >::iterator mit;
	mit = m_mapConnectUserUID.find(dwUserUID_);
	if (mit != m_mapConnectUserUID.end()) {
		if (tmRecvTime_ < mit->second.second) {
			START_LOG(cwarn, L"이전 접속설정 보다 이전의 패킷.. UserUID : " << dwUserUID_)
				<< BUILD_LOGtm(CTime(tmRecvTime_))
				<< BUILD_LOGtm(CTime(mit->second.second))
				<< BUILD_LOG(bVal_) << END_LOG;
			return;
		}
	}

	m_mapConnectUserUID[dwUserUID_] = std::make_pair(bVal_, tmRecvTime_);
}

bool KSwitchUserSID::IsConnectUser(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csSwitch);

	std::map<DWORD, std::pair<bool, time_t> >::iterator mit;
	mit = m_mapConnectUserUID.find(dwUserUID_);
	if (mit == m_mapConnectUserUID.end()) {
		return false;
	}

	return mit->second.first;
}

void KSwitchUserSID::DumpAgentSlave()
{
	KLocker lock(m_csSwitch);

	START_LOG(cerr, L"--- Dump AgitSlvae (size:" << m_mapAgentSlave.size() << L") ---") << END_LOG;

	std::map<int, KAgentServerInfo>::iterator mit;
	for (mit = m_mapAgentSlave.begin(); mit != m_mapAgentSlave.end(); ++mit) {
		START_LOG(cerr, L"SID : " << mit->first
			<< L", IP : " << KncUtil::toWideString(mit->second.m_strIP)
			<< L", Port : " << mit->second.m_usPort
			<< L", Available : " << mit->second.m_bAvailable) << END_LOG;
	}
	START_LOG(cerr, L"--- End Dump ---") << END_LOG;
}

void KSwitchUserSID::DumpUser(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csSwitch);
	std::map<DWORD, int>::iterator mit;
	mit = m_mapUserSID.find(dwUserUID_);
	if (mit == m_mapUserSID.end()) {
		START_LOG(cout, L"Not exist User SID.. UserUID : " << dwUserUID_ << L", ListSize : " << m_mapUserSID.size()) << END_LOG;
	}
	else {
		START_LOG(cerr, L"User SID.. UserUID : " << dwUserUID_ << L", SID : " << mit->second << L", ListSize : " << m_mapUserSID.size()) << END_LOG;
	}

	bool bConnect = false;
	time_t tmLastRecv;
	std::map<DWORD, std::pair<bool, time_t> >::iterator mitUser;
	mitUser = m_mapConnectUserUID.find(dwUserUID_);
	if (mitUser == m_mapConnectUserUID.end()) {
		bConnect = false;
	}
	else {
		bConnect = mitUser->second.first;
		tmLastRecv = mitUser->second.second;
	}

	START_LOG(cerr, L"UserUID : " << dwUserUID_ << L", IsConnect : " << (bConnect ? L"True" : L"False"))
		<< BUILD_LOGtm(CTime(tmLastRecv))
		<< BUILD_LOG(m_mapConnectUserUID.size()) << END_LOG;
}

void KSwitchUserSID::GetConnectUserCount(OUT USHORT& usCount_) const
{
	usCount_ = 0;

	std::map<DWORD, std::pair<bool, time_t> >::const_iterator mit;
	for (mit = m_mapConnectUserUID.begin(); mit != m_mapConnectUserUID.end(); ++mit) {
		if (mit->second.first) {
			++usCount_;
		}
	}
}