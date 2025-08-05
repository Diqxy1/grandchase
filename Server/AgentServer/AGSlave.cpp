#include "AGSlave.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "AgentServer.h"
#include "AGSimLayer.h"
#include "AGDBLayer.h"
#include "NetError.h"
#include "AgentPacket.h"
#include "AgitManager.h"
#include "SwitchUserSID.h"
#include "support_Agent.h"
//FILE_NAME_FOR_LOG

#define EVENT_TYPE KAgentEvent
#define CLASS_TYPE KAGSlave

NiImplementRTTI(KAGSlave, KActor);
ImplementException(KAGSlave);

KAGSlavePtr KAGSlave::Create()
{
	return SiKAGSimLayer()->m_kActorManager2.CreateActor<KAGSlave>();
}

void KAGSlave::AddManager(KAGSlavePtr spAGSlave_)
{
	JIF(spAGSlave_);
	JIF(SiKAGSimLayer()->m_kActorManager2.Add(spAGSlave_));
}

KAGSlave::KAGSlave(void)
{
}

KAGSlave::~KAGSlave(void)
{
}

ImplToStringW(KAGSlave)
{
	return START_TOSTRING_PARENTW(KActor);
}

void KAGSlave::ProcessEvent(const KEventPtr& spEvent_)
{
	KSerializer ks;
	ks.BeginReading(const_cast<KSerBuffer*>(&spEvent_->m_kbuff));

	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch (spEvent_->m_usEventID)
	{
		_CASE(EAGS_VERIFY_ACCOUNT_REQ, KAgentServerInfo);
		_CASE(EAGS_SET_AGIT_SID_ACK, KEnterAgitSIDAck);
		_CASE(EAGS_CLEAR_AGIT_SID_NOT, DWORD);
		_CASE(EAGS_UPDATE_SLAVE_STATE_NOT, PAIR_INT_BOOL);
		_CASE(EAGS_USER_CONNECT_STATUS_REQ, KUserConnectStatus);

	default:
		START_LOG(cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString()) << END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if (dwElapTime > 3000)
	{
		START_LOG_WITH_NAME(cwarn)
			<< BUILD_LOG(spEvent_->GetEventIDString())
			<< BUILD_LOG(spEvent_->m_usEventID)
			<< BUILD_LOG(dwElapTime) << END_LOG;
	}
}

void KAGSlave::Tick()
{
	KActor::Tick();

	switch (GetStateID()) {
	case KDefaultFSM::STATE_EXIT:
		SiKAGSimLayer()->m_kActorManager2.ReserveDelete(m_strName);
		break;
	}
}

void KAGSlave::OnDestroy()
{
	KSession::OnDestroy();

	switch (GetStateID()) {
	case KDefaultFSM::STATE_CONNECTED:
	case KDefaultFSM::STATE_LOGINED:
		StateTransition(KDefaultFSM::INPUT_EXIT_GAME);
		break;
	}
}

_IMPL_ON_FUNC(EAGS_VERIFY_ACCOUNT_REQ, KAgentServerInfo)
{
	VERIFY_STATE((1, KDefaultFSM::STATE_CONNECTED));

	KAGSlavePtr spSlave;
	std::wstring strSlaveName;
	int kPacket = -99;

	SET_ERROR(ERR_UNKNOWN);

	// 보고된 아이피가 소켓에서 얻은 것과 다름.
	if (GetIPStr() != kPacket_.m_strIP) {
		SET_ERR_GOTO(ERR_VERIFY_08, END_PROC); // 불량 유저가 접속 시도
	}

	// 이중접속 시도
	spSlave = SiKAGSimLayer()->m_kActorManager2.GetByUID<KAGSlave>((DWORD)kPacket_.m_nSID);
	if (spSlave) {
		SET_ERR_GOTO(ERR_VERIFY_04, END_PROC);
	}

	SetUID((DWORD)kPacket_.m_nSID);
	m_kSlaveInfo = kPacket_;
	SiKSwitchUserSID()->SetSlave(kPacket_);

	// rename, UID 등록
	{
		std::wstringstream stm;
		stm << KncUtil::toWideString(kPacket_.m_strIP) << L"_" << kPacket_.m_usPort;
		strSlaveName = stm.str().c_str();
		stm.clear();
	}

	LIF(SiKAGSimLayer()->m_kActorManager2.Rename(GetName(), strSlaveName));
	LIF(SiKAGSimLayer()->m_kActorManager2.RegByUID(*this));

	StateTransition(KDefaultFSM::INPUT_VERIFICATION_OK);
	SET_ERROR(NET_OK);

END_PROC:
	kPacket = NetError::GetLastNetError();

	_LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK, L"Ret : " << NetError::GetErrStr(kPacket))
		<< BUILD_LOG(GetIPStr())
		<< BUILD_LOG(kPacket_.m_nSID)
		<< BUILD_LOG(KncUtil::toWideString(kPacket_.m_strIP))
		<< BUILD_LOG(kPacket_.m_usPort)
		<< BUILD_LOG(kPacket_.m_bAvailable)
		<< BUILD_LOG(strSlaveName)
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID()) << END_LOG;

	SEND_PACKET(EAGS_VERIFY_ACCOUNT_ACK);
}

_IMPL_ON_FUNC(EAGS_SET_AGIT_SID_ACK, KEnterAgitSIDAck)
{
	int nPrevSID = -1;
	SiKSwitchUserSID()->SetUserSID(kPacket_.m_kEnterAgitReq.m_dwAgitUID, kPacket_.m_nSID, nPrevSID);

	if (kPacket_.m_nSID > -1) {
		if (SiKSwitchUserSID()->IsConnectUser(kPacket_.m_kEnterAgitReq.m_dwAgitUID)) {
			KUserTimeParam kPacket;
			kPacket.m_dwUserUID = kPacket_.m_kEnterAgitReq.m_dwAgitUID;
			kPacket.m_tmTime = KncUtil::TimeToInt(CTime::GetCurrentTime());
			SEND_DATA(EAGS_USER_CONNECT_NOT, kPacket);
			START_LOG(clog, L"아지트 정보 추가된 AgentSlave에 주인 접속하고 있음을 알림.. AgitUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID) << END_LOG;
		}
	}

	SiKAGSimLayer()->m_kActorManager.QueueingEventTo(kPacket_.m_dwServerUID, KAgentEvent::EAG_ENTER_AGIT_SID_ACK, kPacket_, 1);

	START_LOG(clog, L"아지트 상주할 에이전트서버 SID 전달.. ServerUID : " << kPacket_.m_dwServerUID)
		<< BUILD_LOG(nPrevSID)
		<< BUILD_LOG(kPacket_.m_nSID)
		<< BUILD_LOG(kPacket_.m_kEnterAgitReq.m_dwAgitUID) << END_LOG;

	if (nPrevSID > -1) {
		SiKAGSimLayer()->m_kActorManager2.SendTo(nPrevSID, KAgentEvent::EAGS_REMOVE_AGIT_INFO_NOT, kPacket_.m_kEnterAgitReq.m_dwAgitUID);

		START_LOG(clog, L"이전 에이전트서버에 아지트 정보 삭제를 요청..")
			<< BUILD_LOG(nPrevSID)
			<< BUILD_LOG(kPacket_.m_nSID)
			<< BUILD_LOG(kPacket_.m_kEnterAgitReq.m_dwAgitUID) << END_LOG;
	}
}

_IMPL_ON_FUNC(EAGS_CLEAR_AGIT_SID_NOT, DWORD)
{
	DWORD& dwAgitUID = kPacket_;
	int nSID = -1;
	int nPrevSID = -1;

	SiKSwitchUserSID()->SetUserSID(dwAgitUID, nSID, nPrevSID);

	if (nPrevSID > -1) {
		SiKAGSimLayer()->m_kActorManager2.SendTo(nPrevSID, KAgentEvent::EAGS_REMOVE_AGIT_INFO_NOT, dwAgitUID);

		START_LOG(clog, L"에이전트서버에 아지트 정보 삭제를 요청..")
			<< BUILD_LOG(nPrevSID)
			<< BUILD_LOG(nSID)
			<< BUILD_LOG(dwAgitUID) << END_LOG;
	}
}

_IMPL_ON_FUNC(EAGS_UPDATE_SLAVE_STATE_NOT, PAIR_INT_BOOL)
{
	const int& nSID = kPacket_.first;
	const bool& bState = kPacket_.second;

	LIF(SiKSwitchUserSID()->UpdateSlaveAvailable(nSID, bState));
	START_LOG(clog, L"Update AgentSlave State.. SID : " << nSID << L", Available : " << bState) << END_LOG;

	bool bDestroyAgit = SiKSwitchUserSID()->IsDestroySetting();
	SEND_DATA(EAGS_SET_DESTROY_AGIT_NOT, bDestroyAgit);
}

_IMPL_ON_FUNC(EAGS_USER_CONNECT_STATUS_REQ, KUserConnectStatus)
{
	std::map<DWORD, bool>::iterator mit;
	for (mit = kPacket_.m_mapUserConnect.begin(); mit != kPacket_.m_mapUserConnect.end(); ++mit) {
		mit->second = SiKSwitchUserSID()->IsConnectUser(mit->first);
	}

	START_LOG(clog, L"유저 접속상태 확인.. AgitUID : " << kPacket_.m_dwAgitUID << L", UserSize : " << kPacket_.m_mapUserConnect.size()) << END_LOG;
	SEND_RECEIVED_PACKET(EAGS_USER_CONNECT_STATUS_ACK);
}