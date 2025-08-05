#include "AGProxy.h"
#include "NetError.h"
#include "AGSimLayer.h"
#include "Socket/NetCommon.h"
#include "AgentEvent.h"
#include "AgentPacket.h"
#include "Agent.h"
#include "AgentServer.h"
#include "AGNetLayer.h"
#include "AGDBLayer.h"
#include "AgitManager.h"

//FILE_NAME_FOR_LOG

NiImplementRTTI(KAGProxy, KActorProxy);

#define EVENT_TYPE KAgentEvent
#define CLASS_TYPE KAGProxy

KAGProxy::KAGProxy(void)
	: m_bAfterVerify(false)
{
	m_hVerifyEvent = ::CreateEvent(NULL, false, false, NULL);
	m_nSavedResult = NetError::NET_OK;
	SetSendQueueSize(1024 * 512);
}

KAGProxy::~KAGProxy(void)
{
	OnSocketError();
	::CloseHandle(m_hVerifyEvent);
}

ImplOstmOperatorW2A(KAGProxy);
ImplToStringW(KAGProxy)
{
	return START_TOSTRING_PARENTW(KActorProxy)
		<< TOSTRINGW(KncUtil::toWideString(GetIPStr()))
		<< TOSTRINGW(GetPort())
		<< TOSTRINGWb(IsConnected());
}

// Call to ProcessEventQueue (Client)
void KAGProxy::ProcessEvent(const KEventPtr& spEvent_)
{
	KSerializer     ks;
	KSerBuffer      kbuff;

	ks.BeginReading(const_cast<KSerBuffer*>(&spEvent_->m_kbuff));

	switch (spEvent_->m_usEventID)
	{
		_CASE(EAGS_VERIFY_ACCOUNT_ACK, int);
		_CASE(EAGS_SET_AGIT_SID_REQ, KEnterAgitSIDReq);
		_CASE(EAGS_USER_CONNECT_NOT, KUserTimeParam);
		_CASE(EAGS_USER_DISCONNECT_NOT, KUserTimeParam);
		_CASE(EAGS_SET_DESTROY_AGIT_NOT, bool);
		_CASE(EAGS_USER_CONNECT_STATUS_ACK, KUserConnectStatus);
		_CASE(EAGS_REMOVE_AGIT_INFO_NOT, DWORD);
	default:
		START_LOG(cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString()) << END_LOG;
	}
}

bool KAGProxy::Connect(std::string strIP_, unsigned short usPort_)
{
	if (IsConnected()) {
		START_LOG(clog, L"이미 연결되어 있습니다.") << END_LOG;
		return true;
	}

	if (false == KActorProxy::Connect(strIP_.c_str(), usPort_, true)) {
		return false;
	}

	KAgentServerInfo kPacket;
	kPacket.m_nSID = SiKAgentServer()->GetServerID();
	kPacket.m_strIP = NetCommon::GetLocalIP();
	kPacket.m_usPort = SiKAGNetLayer()->m_usPortMaster;
	kPacket.m_bAvailable = true;
	kPacket.m_bDestroy = false;

	START_LOG(clog, L"Send EAGS_VERIFY_ACCOUNT_REQ")
		<< BUILD_LOG(kPacket.m_nSID)
		<< BUILD_LOG(KncUtil::toWideString(kPacket.m_strIP))
		<< BUILD_LOG(kPacket.m_usPort)
		<< BUILD_LOG(kPacket.m_bAvailable)
		<< BUILD_LOG(kPacket.m_bDestroy)
		<< BUILD_LOG(SiKAgitManager()->GetMaxCashingAgitInfo()) << END_LOG;

	_JIF(SendPacket(KAgentEvent::EAGS_VERIFY_ACCOUNT_REQ, kPacket), return false);

	DWORD dwElapsTime = ::GetTickCount();

	while (GetTickCount() - dwElapsTime < 5000) {
		DWORD dwReturn = WaitForSingleObject(m_hVerifyEvent, 200);

		if (dwReturn == WAIT_TIMEOUT) {
			Tick(); // 이 스레드에서 Tick을 호출해 주어야만 ACK 확인처리가 가능하다.
		}
		else if (dwReturn == WAIT_OBJECT_0) {
			if (m_nSavedResult == NetError::NET_OK) {
				return true;
			}
			else {
				break;
			}
		}
		else {
			START_LOG(cerr, L"Unexpected result of function call.")
				<< BUILD_LOG(::GetLastError()) << END_LOG;
			break;
		}
	}

	if (m_nSPIndex != 0) {
		START_LOG(cerr, L"SA 삭제. SPI : " << m_nSPIndex) << END_LOG;
		KncSecurity::GetSADB().Delete(m_nSPIndex);
		m_nSPIndex = 0;
	}

	return false;
}

_IMPL_ON_FUNC(EAGS_VERIFY_ACCOUNT_ACK, int)
{
	m_nSavedResult = kPacket_;

	if (kPacket_ != NetError::NET_OK) {
		START_LOG_WITH_NAME(cerr)
			<< BUILD_LOG(kPacket_)
			<< BUILD_LOG(NetError::GetErrStr(kPacket_))
			<< END_LOG;
	}
	else
	{
		START_LOG(clog, L"Connecting with Agent Master succeess.")
			<< BUILD_LOG(kPacket_) << END_LOG;
	}

	SetEvent(m_hVerifyEvent);

	if (kPacket_ == 0) {
		m_bAfterVerify = true;
	}
}

_IMPL_ON_FUNC(EAGS_SET_AGIT_SID_REQ, KEnterAgitSIDReq)
{
	KEnterAgitSIDAck kPacket;
	kPacket.m_nSID = SiKAgentServer()->GetServerID();
	kPacket.m_dwServerUID = kPacket_.m_dwServerUID;
	kPacket.m_kEnterAgitReq = kPacket_.m_kEnterAgitReq;

	SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_LOAD_AGIT_INFO_NOT, L"AgitSlave", kPacket.m_nSID, -1, kPacket_.m_kEnterAgitReq.m_dwAgitUID);
	START_LOG(clog, L"Set AgitSID Req.. AgitUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID << L", SID : " << kPacket.m_nSID << L", Req ServerUID : " << kPacket_.m_dwServerUID) << END_LOG;

	SEND_PACKET(EAGS_SET_AGIT_SID_ACK);
}

_IMPL_ON_FUNC(EAGS_USER_CONNECT_NOT, KUserTimeParam)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		return;
	}

	spAgit->SetOwnerConnect(true, kPacket_.m_tmTime);

	START_LOG(clog, L"아지트 주인 게임접속 알림.. AgitUID :" << kPacket_.m_dwUserUID)
		<< BUILD_LOGtm(CTime(kPacket_.m_tmTime)) << END_LOG;
}

_IMPL_ON_FUNC(EAGS_USER_DISCONNECT_NOT, KUserTimeParam)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		return;
	}

	spAgit->SetOwnerConnect(false, kPacket_.m_tmTime);

	if (SiKAgitManager()->GetCashingSize() >= SiKAgitManager()->GetMaxCashingAgitInfo()) {
		if (false == SiKAgitManager()->IsRankAgit(kPacket_.m_dwUserUID)) {
			if (spAgit->GetUserNum() == 0) {
				spAgit->SetReserveDestroy(true);
				START_LOG(clog, L"아지트 가용 포화상태로 아지트정보 삭제.. AgitUID : " << kPacket_.m_dwUserUID) << END_LOG;
			}
		}
	}

	START_LOG(clog, L"아지트 주인 게임접속 종료알림.. AgitUID :" << kPacket_.m_dwUserUID) << END_LOG;
}

_IMPL_ON_FUNC(EAGS_SET_DESTROY_AGIT_NOT, bool)
{
	SiKAgitManager()->SetDestroyAgit(kPacket_);
	START_LOG(clog, L"AgentMaster에서 아지트정보 삭제여부 설정 전달.. DestroyAgit : " << (kPacket_ == true ? L"True" : L"False")) << END_LOG;
}

_IMPL_ON_FUNC(EAGS_USER_CONNECT_STATUS_ACK, KUserConnectStatus)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		return;
	}

	spAgit->UpdateUserConnectStatus(kPacket_.m_mapUserConnect);
	START_LOG(clog, L"유저 접속상태 확인응답.. AgitUID : " << kPacket_.m_dwAgitUID << L", UserSize : " << kPacket_.m_mapUserConnect.size()) << END_LOG;
}

_IMPL_ON_FUNC(EAGS_REMOVE_AGIT_INFO_NOT, DWORD)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_);
	if (spAgit != NULL) {
		spAgit->SetReserveDestroy(true);
		START_LOG(clog, L"SID재할당으로 기존 아지트정보 삭제.. AgitUID : " << kPacket_) << END_LOG;
	}

	START_LOG(clog, L"SID재할당으로 기존 아지트정보 삭제 요청함.. AgitUID : " << kPacket_) << END_LOG;
}