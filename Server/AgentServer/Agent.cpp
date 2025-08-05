#include "Agent.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "AgentServer.h"
#include "AGSimLayer.h"
#include "AGDBLayer.h"
#include "NetError.h"
#include "AgentPacket.h"
#include "SlangFilter.h"
#include "support_Agent.h"
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include "AgitManager.h"
#include "SwitchUserSID.h"
#include <boost/algorithm/string.hpp> //boost::to_lower_copy

//FILE_NAME_FOR_LOG

#define EVENT_TYPE KAgentEvent
#define CLASS_TYPE KAgent

NiImplementRTTI(KAgent, KActor);
ImplementException(KAgent);

KAgentPtr KAgent::Create()
{
	return SiKAGSimLayer()->m_kActorManager.CreateActor<KAgent>();
}

void KAgent::AddManager(KAgentPtr spAGUser_)
{
	JIF(spAGUser_);
	JIF(SiKAGSimLayer()->m_kActorManager.Add(spAGUser_));
}

KAgent::KAgent(void)
{
}

KAgent::~KAgent(void)
{
}

ImplToStringW(KAgent)
{
	return START_TOSTRING_PARENTW(KActor);
}

void KAgent::ProcessEvent(const KEventPtr& spEvent_)
{
	KSerializer ks;
	ks.BeginReading(const_cast<KSerBuffer*>(&spEvent_->m_kbuff));

	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch (spEvent_->m_usEventID)
	{
		_CASE(EAG_VERIFY_ACCOUNT_REQ, KSimpleServerInfo);
		_CASE(EAG_SERVER_RELAY_PACKET_REQ, KServerRelayPacket);
		_CASE(EAG_AGIT_RELAY_PACKET_REQ, KAgitRelayPacket);
		_CASE(EAG_AGIT_ALL_CONFIG_REQ, DWORD);

		_CASE(EAG_ENTER_AGIT_REQ, KEnterAgitReq);
		CASE(EAG_AGIT_LOADING_COMPLETE_REQ);
		CASE(EAG_LEAVE_AGIT_REQ);
		_CASE(EAG_UPDATE_AGIT_USER_INFO_NOT, KEAG_AGIT_LOADING_COMPLETE_REQ);

		CASE(EAG_BUY_AGIT_MAP_REQ);
		_CASE(EAG_BUY_AGIT_MAP_ACK, KBuyAgitMapAck);
		_CASE(EAG_SET_AGIT_OPTION_REQ, KAgitOption);
		_CASE(EAG_SET_AGIT_OPTION_ACK, KSetAgitOptionAck);
		_CASE(EAG_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq);
		_CASE(EAG_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck);
		_CASE(EAG_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq);
		_CASE(EAG_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck);

		_CASE(EAG_AGIT_CHAT_REQ, KChatData);
		_CASE(EAG_AGIT_BAN_USER_REQ, KAgitBanUserReq);
		_CASE(EAG_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq);
		_CASE(EAG_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck);

		_CASE(EAG_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq);
		_CASE(EAG_AGIT_FRIEND_LIST_ACK, KSimpleAgitPageList);
		_CASE(EAG_AGIT_TOP_RANK_LIST_REQ, DWORD);
		_CASE(EAG_AGIT_FAVORITE_LIST_REQ, DWORD);
		_CASE(EAG_AGIT_ADD_FAVORITE_REQ, KAgitUserParam);
		_CASE(EAG_AGIT_ADD_FAVORITE_ACK, KAgitAddFavoriteAck);
		_CASE(EAG_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam);
		_CASE(EAG_AGIT_REMOVE_FAVORITE_ACK, KAgitUserParamAck);

		_CASE(EAG_REGISTER_AGIT_PAGE_REQ, KRegisterAgitPageReq);
		_CASE(EAG_REGISTER_AGIT_PAGE_ACK, KRegisterAgitPageAck);
		_CASE(EAG_UNREGISTER_AGIT_PAGE_REQ, DWORD);
		_CASE(EAG_AGIT_PAGE_LIST_REQ, KAgitPageList);

		_CASE(EAG_EQUIP_BROWNIE_REQ, KEquipBrownieReq);
		_CASE(EAG_EQUIP_BROWNIE_ACK, KEquipBrownieAck);

		_CASE(EAG_WATERING_FAIRY_TREE_REQ, DWORD);
		_CASE(EAG_WATERING_FAIRY_TREE_ACK, KWateringFairyTreeAck);
		_CASE(EAG_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff);
		_CASE(EAG_USE_FAIRY_TREE_FRUIT_REQ, DWORD);
		_CASE(EAG_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck);

		_CASE(EAG_ENTER_AGIT_SID_REQ, KEnterAgitReq);
		_CASE(EAG_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck);

		_CASE(EAG_USER_CONNECT_NOT, KUserTimeParam);
		_CASE(EAG_USER_DISCONNECT_NOT, KUserTimeParam);
		CASE(EAG_LOAD_AGIT_INFO_NOT);
		CASE(EAG_ENTER_AGIT_ACK);
		CASE(EAG_DELETE_FOR_AGIT_NOT);

		_CASE(EAG_GUESTBOOK_PAGE_REQ, KAgitUserParam);
		_CASE(EAG_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq);
		CASE(EAG_GUESTBOOK_WRITE_ACK);
		_CASE(EAG_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq);
		CASE(EAG_GUESTBOOK_DELETE_ACK);
		_CASE(EAG_USE_TRAINING_OBJ_REQ, KUseTrainingObjReq);
		CASE(EAG_UPDATE_TRAINING_OBJ_ACK);

		_CASE(EAG_OPEN_AGIT_CHARACTER_REQ, PAIR_DWORD_INT);
		_CASE(EAG_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck);
		_CASE(EAG_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq);
		_CASE(EAG_SET_AGIT_CHARACTER_POS_ACK, KSetAgitCharPosAck);
		_CASE(EAG_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq);
		_CASE(EAG_SET_AGIT_CHARACTER_MOTION_ACK, KSetAgitCharMotionAck);
		_CASE(EAG_SET_AGIT_CHARACTER_COORDI_REQ, KSetAgitCharCoordiReq);
		_CASE(EAG_SET_AGIT_CHARACTER_COORDI_ACK, KSetAgitCharCoordiAck);
		_CASE(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq);
		_CASE(EAG_SET_AGIT_CHARACTER_SPEECH_ACK, KSetAgitCharSpeechAck);

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

void KAgent::Tick()
{
	KActor::Tick();

	switch (GetStateID()) {
	case KDefaultFSM::STATE_EXIT:
		SiKAGSimLayer()->m_kActorManager.ReserveDelete(m_strName);
		break;
	}
}

void KAgent::OnDestroy()
{
	KSession::OnDestroy();

	switch (GetStateID()) {
	case KDefaultFSM::STATE_CONNECTED:
	case KDefaultFSM::STATE_LOGINED:
		StateTransition(KDefaultFSM::INPUT_EXIT_GAME);
		break;
	}
}

_IMPL_ON_FUNC(EAG_VERIFY_ACCOUNT_REQ, KSimpleServerInfo)
{
	VERIFY_STATE((1, KDefaultFSM::STATE_CONNECTED));

	KAgentPtr spAgent;
	KEAG_VERIFY_ACCOUNT_ACK kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_kServerInfo = kPacket_;
	kPacket.m_bEnterance = true;
	kPacket.m_nSID = SiKAgentServer()->GetServerID();

	SET_ERROR(ERR_UNKNOWN);

	// 보고된 아이피가 소켓에서 얻은 것과 다름.
	if (KncUtil::toWideString(GetIPStr()) != kPacket_.m_strIP) {
		SET_ERR_GOTO(ERR_VERIFY_08, END_PROC); // 불량 유저가 접속 시도
	}

	// 서버/클라이언트간 프로토콜이 다름
	if (kPacket_.m_nProtocolVer != SiKAgentServer()->m_nProtocolVer) {
		SET_ERR_GOTO(ERR_VERIFY_07, END_PROC);
	}

	// 이중접속 시도
	spAgent = SiKAGSimLayer()->m_kActorManager.GetByName<KAgent>(kPacket_.m_strName);
	if (spAgent) {
		SET_ERR_GOTO(ERR_VERIFY_04, END_PROC);
	}

	SetUID(kPacket_.m_dwUID);
	m_kServerInfo = kPacket.m_kServerInfo;

	// rename, UID 등록
	LIF(SiKAGSimLayer()->m_kActorManager.Rename(GetName(), m_kServerInfo.m_strName));
	LIF(SiKAGSimLayer()->m_kActorManager.RegByUID(*this));

	StateTransition(KDefaultFSM::INPUT_VERIFICATION_OK);

	if (SiKAgitManager()->GetCashingSize() >= SiKAgitManager()->GetMaxCashingAgitInfo()) {
		kPacket.m_bEnterance = false;

		START_LOG(cwarn, L"아지트 등록 제한 한계..")
			<< BUILD_LOG(SiKAgitManager()->GetMaxCashingAgitInfo())
			<< BUILD_LOG(SiKAgitManager()->GetCashingSize()) << END_LOG;
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK, L"Ret : " << NetError::GetErrStr(kPacket.m_nOK))
		<< BUILD_LOG(GetIPStr())
		<< BUILD_LOG(SiKAgentServer()->m_nProtocolVer)
		<< BUILD_LOG(kPacket_.m_dwUID)
		<< BUILD_LOG(kPacket_.m_strIP)
		<< BUILD_LOG(kPacket_.m_usPort)
		<< BUILD_LOG(kPacket_.m_strName)
		<< BUILD_LOG(kPacket_.m_nProtocolVer)
		<< BUILD_LOG(kPacket.m_nSID)
		<< BUILD_LOG(kPacket.m_bEnterance)
		<< BUILD_LOG(SiKAgitManager()->GetMaxCashingAgitInfo())
		<< BUILD_LOG(SiKAgitManager()->GetCashingSize()) << END_LOG;

	SEND_PACKET(EAG_VERIFY_ACCOUNT_ACK);

	if (NetError::GetLastNetError() == NetError::NET_OK) {
		KAgitConfig kConfig;
		SiKAgitManager()->GetAgitConfig(kConfig);
		SEND_DATA(EAG_AGIT_CONFIG_NOT, kConfig);

		KAgitMapInfo kMapInfo;
		SiKAgitManager()->GetMapInfo(kMapInfo);
		SEND_DATA(EAG_AGIT_MAP_NOT, kMapInfo);

		KFlowerPotConfig kFlowerPotConfig;
		SiKAgitManager()->GetFlowerPotConfig(kFlowerPotConfig);
		SEND_DATA(EAG_AGIT_FLOWER_NOT, kFlowerPotConfig);

		KFairyTreeConfig kTreeConfig;
		SiKAgitManager()->GetFairyTreeConfig(kTreeConfig);
		SEND_DATA(EAG_AGIT_FAIRY_TREE_NOT, kTreeConfig);

		START_LOG(cout, L"아지트 설정정보 전달.. ServerUID : " << GetUID()) << END_LOG;
	}
}

_IMPL_ON_FUNC(EAG_SERVER_RELAY_PACKET_REQ, KServerRelayPacket)
{
	// 구현 필요함.
	// 용도가 아직 없음.
	// 특정 대상 유저에게 패킷을 압축해서 그대로 전달.
}

_IMPL_ON_FUNC(EAG_AGIT_RELAY_PACKET_REQ, KAgitRelayPacket)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		return;
	}

	spAgit->GetUserServerList(kPacket_.m_mapServerUser);

	if (!kPacket_.m_mapServerUser.empty()) {
		spAgit->SendToAll(KAgentEvent::EAG_AGIT_RELAY_PACKET_REQ, kPacket_);
	}

	START_LOG(clog, L"Agit Relay Packet.. serverUID : " << GetUID())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_usEventID)
		<< BUILD_LOG(kPacket_.m_dwSenderUID)
		<< BUILD_LOG(kPacket_.m_mapServerUser.size()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_AGIT_ALL_CONFIG_REQ, DWORD)
{
	KAgitConfig kConfig;
	SiKAgitManager()->GetAgitConfig(kConfig);
	SEND_DATA(EAG_AGIT_CONFIG_NOT, kConfig);

	KAgitMapInfo kMapInfo;
	SiKAgitManager()->GetMapInfo(kMapInfo);
	SEND_DATA(EAG_AGIT_MAP_NOT, kMapInfo);

	KFlowerPotConfig kFlowerPotConfig;
	SiKAgitManager()->GetFlowerPotConfig(kFlowerPotConfig);
	SEND_DATA(EAG_AGIT_FLOWER_NOT, kFlowerPotConfig);

	START_LOG(cout, L"아지트 설정정보 전달.. ServerUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_ENTER_AGIT_REQ, KEnterAgitReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KEnterAgitAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwEnterUserUID = kPacket_.m_dwEnterUserUID;
	kPacket.m_kInfo.m_dwUID = kPacket_.m_dwAgitUID;
	kPacket.m_strPassword = kPacket_.m_strPassword;
	KAgitInfo kInfo;
	KAgitMapInfo kMapInfo;
	std::map<DWORD, KAgitMap>::iterator mit;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_ENTER_AGIT_REQ, kPacket_);
		START_LOG(clog, L"DB 유저 아지트정보 요청.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		return;
		//SET_ERR_GOTO( ERR_AGIT_01, END_PROC );
	}

	// 아지트 정보 전달
	spAgit->GetInfo(kPacket.m_kInfo);
	spAgit->GetTRServerIPPort(kPacket.m_prTRServerIpPort);
	spAgit->GetURServerIPPort(kPacket.m_prURServerIpPort);
	spAgit->GetFlowerPot(kPacket.m_mapFlowerPot);
	spAgit->GetBrownieInfo(kPacket.m_mapBrownie);
	spAgit->GetFairyTree(kPacket.m_kFairyTree);
	spAgit->GetTrainingObjUpdate(kPacket.m_mapTrainingObj);

	SiKAgitManager()->GetMapInfo(kMapInfo);

	// 아지트 캐릭터 최대 배치 개수 전달
	mit = kMapInfo.find(kPacket.m_kInfo.m_dwMapID);
	if (mit != kMapInfo.end()) {
		kPacket.m_usMaxCharPosCount = mit->second.m_usMaxCharPos;
	}

	// 입장요청시, 삭제안하도록 설정
	spAgit->SetReserveDestroy(false);

	// 주인이 아니면
	if (kPacket_.m_dwEnterUserUID != kPacket.m_kInfo.m_dwUID) {
		// 인기 아지트가 아니고,
		if (false == SiKAgitManager()->IsRankAgit(kPacket.m_kInfo.m_dwUID)) {
			// 주인이 없을때 입장할 수 없음.
			if (false == spAgit->IsOwnerConnect()) {
				START_LOG(cwarn, L"아지트 주인이 게임에 접속해있지않음.. AgitUID : " << kPacket.m_kInfo.m_dwUID)
					<< BUILD_LOG(spAgit->IsOwnerConnect()) << END_LOG;
				SET_ERR_GOTO(ERR_AGIT_09, END_PROC);
			}
		}

		// 비공개 - 비밀번호 체크
		if (kPacket.m_kInfo.m_ucOpenType == KAgitInfo::OT_CLOSE) {
			if (kPacket.m_kInfo.m_strPassword.compare(kPacket_.m_strPassword) != 0) {
				SET_ERR_GOTO(ERR_AGIT_07, END_PROC);
			}
		}

		// 인원체크
		if (spAgit->IsFull()) {
			SET_ERR_GOTO(ERR_AGIT_05, END_PROC);
		}
	}

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_01, kPacket.m_nOK, 1); // 아지트 정보를 DB에서 받아올 수 없음.
		ERR_CASE(ERR_AGIT_05, kPacket.m_nOK, 5); // 입장할 아지트 정원이 초과됨.
		ERR_CASE(ERR_AGIT_06, kPacket.m_nOK, 6); // 입장할 아지트는 친구만 입장 가능.
		ERR_CASE(ERR_AGIT_07, kPacket.m_nOK, 7); // 입장할 아지트의 비밀번호가 틀림.
		ERR_CASE(ERR_AGIT_09, kPacket.m_nOK, 9); // 주인이 접속중이 아니다.

	default:
		START_LOG(cerr, L"내 아지트 입장처리 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"내 아지트 입장 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_kInfo.m_dwUID)
		<< BUILD_LOG(kPacket.m_kInfo.m_strName)
		<< BUILD_LOGc(kPacket.m_kInfo.m_dwMapID)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucOpenType)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucInviteType)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucMaxUser)
		<< BUILD_LOG(kPacket.m_kInfo.m_nTodayVisiter)
		<< BUILD_LOG(kPacket.m_kInfo.m_nTotalVisiter)
		<< BUILD_LOGc(kPacket.m_kInfo.m_kProfile.m_ucTheme)
		<< BUILD_LOG(kPacket.m_kInfo.m_kProfile.m_strProfile)
		<< BUILD_LOG(kPacket.m_kInfo.m_vecObjectPos.size())
		<< BUILD_LOG(kPacket.m_kInfo.m_mapCharacterPos.size())
		<< BUILD_LOGtm(CTime(kPacket.m_kInfo.m_tmLastCleaning))
		<< BUILD_LOG(kPacket.m_kInfo.m_strPassword)
		<< BUILD_LOG(kPacket.m_mapFlowerPot.size())
		<< BUILD_LOG(kPacket.m_mapBrownie.size())
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmUpdateFruit))
		<< BUILD_LOG(kPacket.m_kFairyTree.m_bUseFruit)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_tmUpdateFruit)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nFruitType)
		<< BUILD_LOG(kPacket.m_strPassword)
		<< BUILD_LOG(sizeof(kPacket)) << END_LOG;

	SEND_COMPRESS_PACKET(EAG_ENTER_AGIT_ACK, kPacket);
}

IMPL_ON_FUNC(EAG_AGIT_LOADING_COMPLETE_REQ)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitLoadingCompleteAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_kUserInfo.m_dwUID;
	kPacket.m_mapAgitUser.clear();

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		// 입장할 아지트 정보가 없음.
		// DB에 데이터 요청하고, 실패처리 하자.
		// 유저가 로드한 정보와 DB에서 받아오는 정보가 다를 수 있으니까 실패처리.
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_04, END_PROC);
	}

	// 아지트 입장실패(정원초과)
	if (false == spAgit->Enter(kPacket_.m_kUserInfo, GetThisPtr())) {
		kPacket.m_mapAgitUser.clear();
		SET_ERR_GOTO(ERR_AGIT_05, END_PROC);
	}

	// 아지트에 있는 유저들 정보(자기정보 포함)
	spAgit->GetUserList(kPacket.m_mapAgitUser);

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_ENTER_AGIT_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kUserInfo);

	if (spAgit->IsPageRegister()) {
		SiKAgitManager()->SetPageChange(true);
	}

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_04, kPacket.m_nOK, 4); // 입장할 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_05, kPacket.m_nOK, 5); // 입장할 아지트 정원이 초과됨.

	default:
		START_LOG(cerr, L"로딩완료후 아지트 입장처리 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	in_addr in; // for Log
	in.S_un.S_addr = kPacket_.m_kUserInfo.m_prUserIPPort.first;

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"로딩완료후 아지트 입장처리 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_mapAgitUser.size())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_dwUID)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_strNick)
		<< BUILD_LOGc(kPacket_.m_kUserInfo.m_cCharType)
		<< BUILD_LOGc(kPacket_.m_kUserInfo.m_cPromotion)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_dwLevel)
		<< BUILD_LOG(inet_ntoa(in))
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_prUserIPPort.second)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_nPlayerIndex)
		<< BUILD_LOG(sizeof(kPacket_)) << END_LOG;

	SEND_COMPRESS_PACKET(EAG_AGIT_LOADING_COMPLETE_ACK, kPacket);
}

IMPL_ON_FUNC(EAG_LEAVE_AGIT_REQ)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit != NULL) {
		spAgit->Leave(kPacket_.m_dwUserUID);

		if (spAgit->IsPageRegister()) {
			if (kPacket_.m_dwUserUID == kPacket_.m_dwAgitUID) {
				QueueingEvent(KAgentEvent::EAG_UNREGISTER_AGIT_PAGE_REQ, kPacket_.m_dwAgitUID);
				START_LOG(clog, L"주인이 퇴장한경우, 페이지등록 해제요청.. AgitUID : " << kPacket_.m_dwAgitUID << L"UserUID : " << kPacket_.m_dwUserUID) << END_LOG;
			}
			else {
				SiKAgitManager()->SetPageChange(true);
				START_LOG(clog, L"손님이 나간경우, 페이지 정보변경요청.. AgitUID : " << kPacket_.m_dwAgitUID << L"UserUID : " << kPacket_.m_dwUserUID) << END_LOG;
			}
		}

		START_LOG(clog, L"아지트 퇴장 처리.. AgitUID : " << kPacket_.m_dwAgitUID << L"UserUID : " << kPacket_.m_dwUserUID) << END_LOG;
	}

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_LEAVE_AGIT_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_dwUserUID);

	START_LOG(clog, L"아지트 퇴장 처리.. Name : " << GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_LEAVE_AGIT_ACK);
}

_IMPL_ON_FUNC(EAG_UPDATE_AGIT_USER_INFO_NOT, KEAG_AGIT_LOADING_COMPLETE_REQ)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		return;
	}

	spAgit->SetUserInfo(kPacket_.m_kUserInfo);

	START_LOG(clog, L"아지트 유저정보 갱신.. Name : " << GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_dwUID)
		<< BUILD_LOGc(kPacket_.m_kUserInfo.m_cCharType)
		<< BUILD_LOGc(kPacket_.m_kUserInfo.m_cPromotion)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_dwLevel)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_strNick)
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_vecEquipItems.size())
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_vecLookEquips.size())
		<< BUILD_LOG(kPacket_.m_kUserInfo.m_kPetInfo.m_dwPetID)
		<< BUILD_LOG(SiKAgitManager()->GetCashingSize())
		<< BUILD_LOG(SiKAgitManager()->GetMaxCashingAgitInfo()) << END_LOG;
}

IMPL_ON_FUNC(EAG_BUY_AGIT_MAP_REQ)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitInfo kInfo;
	KBuyAgitMapAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_dwMapID = kPacket_.m_dwMapID;
	kPacket.m_kPayItem = kPacket_.m_kPayItem;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		SET_ERR_GOTO(ERR_AGIT_MAP_05, END_PROC);
	}

	spAgit->GetInfo(kInfo);
	if (kInfo.m_dwMapID == kPacket_.m_dwMapID) {
		SET_ERR_GOTO(ERR_AGIT_MAP_07, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_BUY_AGIT_MAP_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_MAP_05, kPacket.m_nOK, 5); // 맵 구매진행할 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_MAP_07, kPacket.m_nOK, 7); // 현재 자신의 맵과 동일한 맵.

	default:
		START_LOG(cerr, L"아지트 맵 구매중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"아지트 맵 구매 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_dwMapID)
		<< BUILD_LOG(kPacket_.m_kPayItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPayItem.m_nCount) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_BUY_AGIT_MAP_ACK);
	}
}

_IMPL_ON_FUNC(EAG_BUY_AGIT_MAP_ACK, KBuyAgitMapAck)
{
	KAgitPtr spAgit;
	KAgitInfo kInfo;
	KAddItemReq kDropData;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		goto END_PROC;
	}

	// 맵 변경 적용
	spAgit->GetInfo(kInfo);
	kInfo.ResetMapID(kPacket_.m_dwMapID, kPacket_.m_tmLastCleanTime);
	spAgit->SetInfo(kInfo);

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_AGIT_MAP_BROAD, kPacket_.m_dwUserUID, kPacket_.m_dwMapID);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_MAP_06, kPacket_.m_nOK, 6); // 맵구매 DB처리 실패.

	default:
		START_LOG(cerr, L"아지트 맵 구매중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"아지트 맵 구매 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_dwMapID)
		<< BUILD_LOG(kPacket_.m_kPayItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPayItem.m_nCount) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_BUY_AGIT_MAP_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_OPTION_REQ, KAgitOption)
{
	SET_ERROR(ERR_UNKNOWN);

	KSetAgitOptionAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_kOption.m_dwAgitUID = kPacket_.m_dwAgitUID;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_OPT_07, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_OPTION_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_OPT_07, kPacket.m_nOK, 7); // 옵션변경할 아지트 정보가 없음.

	default:
		START_LOG(cerr, L"아지트 옵션변경중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"아지트 옵션변경 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_strName)
		<< BUILD_LOGc(kPacket_.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_strPassword)
		<< BUILD_LOGc(kPacket_.m_ucInviteType)
		<< BUILD_LOG(kPacket_.m_strProfile) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_OPTION_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SET_AGIT_OPTION_ACK, KSetAgitOptionAck)
{
	KAgitPtr spAgit;
	KAgitInfo kInfo;

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_kOption.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_kOption.m_dwAgitUID);
		goto END_PROC;
	}

	spAgit->GetInfo(kInfo);

	// 옵션변경 실패시, 이전 옵션값 넘겨줌.
	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		kInfo.GetOption(kPacket_.m_kOption);
		goto END_PROC;
	}

	boost::replace_all(kPacket_.m_kOption.m_strName, L"''", L"'");
	boost::replace_all(kPacket_.m_kOption.m_strProfile, L"''", L"'");

	// 변경된 옵션을 적용
	spAgit->SetOption(kPacket_.m_kOption);
	if (spAgit->IsPageRegister()) {
		SiKAgitManager()->SetPageChange(true);
	}

	// 방원들에게 알림.
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_AGIT_OPTION_BROAD, kPacket_.m_kOption.m_dwAgitUID, kPacket_.m_kOption);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_OPT_14, kPacket_.m_nOK, 14); // 옵션변경 DB처리 실패.

	default:
		START_LOG(cerr, L"아지트 옵션변경중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"아지트 옵션변경 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_kOption.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kOption.m_strName)
		<< BUILD_LOGc(kPacket_.m_kOption.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_kOption.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_kOption.m_strPassword)
		<< BUILD_LOGc(kPacket_.m_kOption.m_ucInviteType)
		<< BUILD_LOG(kPacket_.m_kOption.m_strProfile) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_OPTION_ACK);
}

_IMPL_ON_FUNC(EAG_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq)
{
	SET_ERROR(ERR_UNKNOWN);

	USHORT usActiveCashPotCount = 0;
	USHORT usActiveNormalPotCount = 0;
	USHORT usAgitCashPotCount = 0;
	USHORT usAgitNormalPotCount = 0;

	std::map<GCITEMUID, KFlowerPot> mapPotList;
	std::map<GCITEMUID, KFlowerPot>::const_iterator cmit;

	KAgitInfo kInfo;
	KAgitMap kMap;
	KSeedFlowerPotAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_SEED_FLOWER_04, END_PROC);
	}

	// 아지트 정보
	spAgit->GetFlowerPot(mapPotList);
	spAgit->GetInfo(kInfo);
	LIF(SiKAgitManager()->GetCurrentMapInfo(kInfo.m_dwMapID, kMap));

	// 화분개수 제약
	usAgitNormalPotCount = kMap.m_usMaxFlowerPot;
	usAgitCashPotCount = SiKAgitManager()->GetMaxAgitCashPotCount();

	// 현재 화분 정보
	usActiveCashPotCount = spAgit->GetActiveCashPotCount();
	usActiveNormalPotCount = spAgit->GetActiveNormalPotCount();

	// 이미 설치해서 사용하고 있는 화분이 아닐경우,
	cmit = mapPotList.find(kPacket_.m_kPotItem.m_ItemUID);
	if (cmit == mapPotList.end()) {
		if (SiKAgitManager()->IsCashPotItemID(kPacket_.m_kPotItem.m_ItemID)) {
			// 캐쉬 화분제한수에 걸리지않나?
			if (usActiveCashPotCount >= usAgitCashPotCount) {
				SET_ERR_GOTO(ERR_SEED_FLOWER_07, END_PROC);
			}
		}
		else if (false == SiKAgitManager()->IsNormalPotItemID(kPacket_.m_kPotItem.m_ItemID)) {
			// 화분 아이템이 아니다.
			SET_ERR_GOTO(ERR_SEED_FLOWER_06, END_PROC);
		}

		// 화분 추가등록
		KFlowerPot kPot;
		kPot.m_UID = kPacket_.m_kPotItem.m_ItemUID;
		kPot.m_ItemID = kPacket_.m_kPotItem.m_ItemID;
		kPot.m_nSeedType = KFlowerPot::ST_NONE;
		kPot.m_usFlowerCount = 0;
		spAgit->UpdateFlowerPot(kPot);

		START_LOG(clog, L"화분 추가등록.. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(kPot.m_UID)
			<< BUILD_LOG(kPot.m_ItemID)
			<< BUILD_LOG(mapPotList.size()) << END_LOG;
	}
	else {
		START_LOG(clog, L"이미 설치된 화분에 씨앗심기를 시도. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemID)
			<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemUID) << END_LOG;
	}

	QUEUEING_EVENT_TO_DB(EAG_SEED_FLOWER_POT_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_SEED_FLOWER_04, kPacket.m_nOK, 4); // 씨앗심을 아지트 정보가 없음.
		ERR_CASE(ERR_SEED_FLOWER_06, kPacket.m_nOK, 6); // 화분 아이템이 아님.
		ERR_CASE(ERR_SEED_FLOWER_07, kPacket.m_nOK, 7); // 캐쉬화분 개수제한에 걸림.
		ERR_CASE(ERR_SEED_FLOWER_08, kPacket.m_nOK, 8); // 일반화분 개수제한에 걸림.

	default:
		START_LOG(cerr, L"씨앗심기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"씨앗심기 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nSeedType)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_nCount)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_nCount)
		<< BUILD_LOG(kPacket_.m_nNeedSeedCount)
		<< BUILD_LOG(kInfo.m_dwMapID)
		<< BUILD_LOG(usActiveCashPotCount)
		<< BUILD_LOG(usActiveNormalPotCount)
		<< BUILD_LOG(usAgitCashPotCount)
		<< BUILD_LOG(usAgitNormalPotCount) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SEED_FLOWER_POT_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck)
{
	KAgitPtr spAgit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		goto END_PROC;
	}

	// 화분정보 갱신
	spAgit->UpdateFlowerPot(kPacket_.m_kPotInfo);

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_FLOWER_POT_STATE_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kPotInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_SEED_FLOWER_05, kPacket_.m_nOK, 5); // 정의되지않은 씨앗심기 타입
		ERR_CASE(ERR_SEED_FLOWER_11, kPacket_.m_nOK, 11); // 화분초기화 DB처리 실패
		ERR_CASE(ERR_SEED_FLOWER_12, kPacket_.m_nOK, 12); // 씨앗심기 DB처리 실패

	default:
		START_LOG(cerr, L"아지트 씨앗심기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"아지트 씨앗심기 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_UID)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_nSeedType)
		<< BUILD_LOGtm(CTime(kPacket_.m_kPotInfo.m_tmSeedTime))
		<< BUILD_LOGtm(CTime(kPacket_.m_kPotInfo.m_tmFlowerTime))
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_usFlowerCount)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kSeedItem.m_nCount) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SEED_FLOWER_POT_ACK);
}

_IMPL_ON_FUNC(EAG_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq)
{
	SET_ERROR(ERR_UNKNOWN);

	std::map<GCITEMUID, KFlowerPot> mapPotList;
	std::map<GCITEMUID, KFlowerPot>::const_iterator cmit;

	int nFreeInvenFlowerCount = 0;
	int nCurrentAddFlowerCount = 0;

	KHarvestFlowerPotAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_nFlowerCount = 0;
	kPacket.m_nBonusFlowerCount = 0;
	kPacket.m_vecItem.clear();

	KFlowerPotOption kOption;
	time_t tmFlower;
	time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime());

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_03, END_PROC);
	}
	spAgit->GetFlowerPot(mapPotList);

	// 해당 화분이 있나?
	cmit = mapPotList.find(kPacket_.m_kPotItem.m_ItemUID);
	if (cmit == mapPotList.end()) {
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_04, END_PROC);
	}

	// 수확 가능한 상태인가?
	if (cmit->second.m_nSeedType < KFlowerPot::ST_FAST || cmit->second.m_nSeedType > KFlowerPot::ST_BEST) {
		START_LOG(cerr, L"화분 수확 가능한 상태가 아님.. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemID)
			<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemUID)
			<< BUILD_LOG(cmit->second.m_nSeedType) << END_LOG;
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_05, END_PROC);
	}

	// 수확 가능한 시간인가?
	tmFlower = cmit->second.m_tmFlowerTime;
	if (tmCurrent < tmFlower) {
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_06, END_PROC);
	}

	// 씨앗심기 옵션
	if (false == SiKAgitManager()->GetFlowerPotOption(cmit->second.m_nSeedType, kOption)) {
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_07, END_PROC);
	}

	// 화폐(민들레)인가?
	if (kPacket_.m_kFlowerItem.m_ItemID != SiKAgitManager()->GetFlowerItemID()) {
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_08, END_PROC);
	}

	// 수확량
	kPacket_.m_nFlowerCount = cmit->second.m_usFlowerCount;

	// 수확량 동기화
	if (cmit->second.m_usFlowerCount == kOption.m_usFlowerCount) {
		START_LOG(cwarn, L"수확하는 꽃의 개수가 서버설정과 일치하지 않음. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(cmit->second.m_nSeedType)
			<< BUILD_LOG(cmit->second.m_usFlowerCount)
			<< BUILD_LOG(kOption.m_usFlowerCount) << END_LOG;

		kPacket_.m_nFlowerCount = kOption.m_usFlowerCount;
	}

	// 보너스 수확량
	kPacket_.m_nBonusFlowerCount = 0;
	if (kPacket_.m_bBonusFlower) {
		kPacket_.m_nBonusFlowerCount = (int)kOption.m_usBonusCount;
		START_LOG(clog, L"수확 보너스 꽃 추가. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(cmit->second.m_nSeedType)
			<< BUILD_LOG(kOption.m_usBonusCount) << END_LOG;
	}

	// 아이템 가지고 있으면
	if (kPacket_.m_kFlowerItem.m_ItemUID > 0) {
		// 보유량 제한
		nFreeInvenFlowerCount = SiKAgitManager()->GetInvenLimitFlowerCount() - kPacket_.m_kFlowerItem.m_nCount;
		nFreeInvenFlowerCount = std::max<int>(nFreeInvenFlowerCount, 0);

		// 수확량 제한
		nCurrentAddFlowerCount = kPacket_.m_nFlowerCount + kPacket_.m_nBonusFlowerCount;
		nCurrentAddFlowerCount = std::min<int>(nCurrentAddFlowerCount, nFreeInvenFlowerCount);
		kPacket_.m_nFlowerCount = std::min<int>(kPacket_.m_nFlowerCount, nCurrentAddFlowerCount);
		kPacket_.m_nBonusFlowerCount = nCurrentAddFlowerCount - kPacket_.m_nFlowerCount;
		kPacket_.m_nBonusFlowerCount = std::max<int>(kPacket_.m_nBonusFlowerCount, 0);
	}

	QUEUEING_EVENT_TO_DB(EAG_HARVEST_FLOWER_POT_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_HARVEST_FLOWER_03, kPacket.m_nOK, 3); // 수확을 진행할 아지트 정보가 않음.
		ERR_CASE(ERR_HARVEST_FLOWER_04, kPacket.m_nOK, 4); // 수확할 화분정보가 없음.
		ERR_CASE(ERR_HARVEST_FLOWER_05, kPacket.m_nOK, 5); // 수확가능한 상태가 아니다.
		ERR_CASE(ERR_HARVEST_FLOWER_06, kPacket.m_nOK, 6); // 수확가능한 시간이 아니다.
		ERR_CASE(ERR_HARVEST_FLOWER_07, kPacket.m_nOK, 7); // 정의되지 않은 씨앗심기 타입.
		ERR_CASE(ERR_HARVEST_FLOWER_08, kPacket.m_nOK, 8); // 수확할 꽃의 정보가 서버와 다름.

	default:
		START_LOG(cerr, L"수확하기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"수확하기 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kPotItem.m_nCount)
		<< BUILD_LOG(kPacket_.m_kFlowerItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kFlowerItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kFlowerItem.m_nCount)
		<< BUILD_LOG(kPacket_.m_bBonusFlower)
		<< BUILD_LOG(kPacket_.m_nFlowerCount)
		<< BUILD_LOG(kPacket_.m_nBonusFlowerCount)
		<< BUILD_LOGtm(CTime(tmCurrent))
		<< BUILD_LOGtm(CTime(tmFlower))
		<< BUILD_LOG(mapPotList.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_HARVEST_FLOWER_POT_ACK);
	}
}

_IMPL_ON_FUNC(EAG_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck)
{
	KAgitPtr spAgit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		goto END_PROC;
	}

	// 화분정보 갱신
	spAgit->UpdateFlowerPot(kPacket_.m_kPotInfo);

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_FLOWER_POT_STATE_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kPotInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_HARVEST_FLOWER_09, kPacket_.m_nOK, 9); // 화분초기화 DB처리 실패

	default:
		START_LOG(cerr, L"수확하기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"수확하기 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nFlowerCount)
		<< BUILD_LOG(kPacket_.m_nBonusFlowerCount)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_UID)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_nSeedType)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_nSeedType)
		<< BUILD_LOG(kPacket_.m_kPotInfo.m_usFlowerCount) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_HARVEST_FLOWER_POT_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_CHAT_REQ, KChatData)
{
	SET_ERROR(ERR_UNKNOWN);

	DWORD dwAgitUID = static_cast<DWORD>(kPacket_.m_iParam);

	KAgitChatAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwSenderUID;

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_CHAT_07, END_PROC);
	}

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_AGIT_CHAT_BROAD, dwAgitUID, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHAT_07, kPacket.m_nOK, 7); // 채팅할 아지트 정보가 없음.
	default:
		START_LOG(cerr, L"아지트채팅 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"아지트채팅 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(dwAgitUID)
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwSenderUID)
		<< BUILD_LOG(kPacket_.m_strSenderNick)
		<< BUILD_LOGc(kPacket_.m_cChatType)
		<< BUILD_LOG(kPacket_.m_dwChatColor)
		<< BUILD_LOG(kPacket_.m_strChatMsg) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_AGIT_CHAT_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_BAN_USER_REQ, KAgitBanUserReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitUserInfo kUserInfo;
	KAgitBanUserAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_dwBanUserUID = kPacket_.m_dwBanUserUID;

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_BAN_02, END_PROC);
	}

	// 추방할 유저가 없음
	if (false == spAgit->GetUserInfo(kPacket.m_dwBanUserUID, kUserInfo)) {
		SET_ERR_GOTO(ERR_AGIT_BAN_03, END_PROC);
	}

	// 추방 알림
	spAgit->SendTo(KAgentEvent::EAG_AGIT_BAN_USER_NOT, kPacket_, kPacket.m_dwBanUserUID, kUserInfo.m_nServerUID);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_BAN_02, kPacket.m_nOK, 2); // 추방처리할 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_BAN_03, kPacket.m_nOK, 3); // 추방할 유저가 내 아지트에 없음.
	default:
		START_LOG(cerr, L"아지트 유저추방 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"아지트 유저추방 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_dwAgitUID)
		<< BUILD_LOG(kPacket.m_dwBanUserUID) << END_LOG;

	SEND_PACKET(EAG_AGIT_BAN_USER_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KSetAgitObjectPosAck kPacket;

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_OBJECT_04, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_OBJECT_POS_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_OBJECT_04, kPacket.m_nOK, 4); // 오브젝트 배치할 아지트 정보가 없음.
	default:
		START_LOG(cerr, L"오브젝트 배치 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"오브젝트 배치 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecObject.size()) << END_LOG;

	SEND_PACKET(EAG_SET_AGIT_OBJECT_POS_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck)
{
	KAgitPtr spAgit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		goto END_PROC;
	}

	// 특수 오브젝트
	{
		std::map<GCITEMUID, KTrainingObj> mapTrainingObj;
		std::vector<KAgitObjectPos> vecOldPos;
		spAgit->GetActiveObjectPos(SiKAgitManager()->TrainingItemID(), vecOldPos);

		std::vector<KAgitObjectPos>::iterator vitNew;
		for (vitNew = kPacket_.m_vecObject.begin(); vitNew != kPacket_.m_vecObject.end(); ++vitNew) {
			if (vitNew->m_ID != SiKAgitManager()->TrainingItemID()) {
				continue;
			}

			if (vitNew->m_nPosX == -1 && vitNew->m_nPosY == -1) {
				START_LOG(clog, L"훈련소 오브젝트 배치 해제됨. ItemID : " << vitNew->m_ID << L", ItemUID : " << vitNew->m_UID) << END_LOG;
				continue;
			}

			std::vector<KAgitObjectPos>::iterator vitOld;
			vitOld = std::find_if(vecOldPos.begin(), vecOldPos.end(),
				boost::bind(&KAgitObjectPos::m_UID, _1) == vitNew->m_UID);

			// 새로 추가된 훈련소
			if (vitOld == vecOldPos.end()) {
				KTrainingObj kObj;
				kObj.m_ItemUID = vitNew->m_UID;
				kObj.m_nAccCount = 0;
				kObj.m_nInitAccCount = 0;

				mapTrainingObj[kObj.m_ItemUID] = kObj;
				START_LOG(clog, L"훈련소 오브젝트 새로 배치됨. ItemID : " << vitNew->m_ID << L", ItemUID : " << vitNew->m_UID) << END_LOG;
				continue;
			}

			START_LOG(clog, L"훈련소 오브젝트 위치만 이동. ItemID : " << vitNew->m_ID << L", ItemUID : " << vitNew->m_UID) << END_LOG;
		}

		if (false == mapTrainingObj.empty()) {
			KEAG_UPDATE_TRAINING_OBJ_REQ kDBPacket;
			kDBPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
			kDBPacket.m_mapTrainingObj = mapTrainingObj;
			QUEUEING_EVENT_TO_DB(EAG_UPDATE_TRAINING_OBJ_REQ, kDBPacket);
			START_LOG(clog, L"활성화되는 훈련소가 있으면, DB에 갱신 요청.. AgitUID : " << kPacket_.m_dwAgitUID << L", size : " << mapTrainingObj.size()) << END_LOG;
		}
	}

	// 오브젝트 배치정보 갱신
	spAgit->SetObjectPos(kPacket_.m_vecObject);

	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_AGIT_OBJECT_POS_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_vecObject);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_OBJECT_05, kPacket_.m_nOK, 5); // 오브젝트 배치 DB처리 실패.

	default:
		START_LOG(cerr, L"오브젝트 배치 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"오브젝트 배치 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecObject.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_OBJECT_POS_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq)
{
	// 자신의 아지트 정보가 없으면 불러오자.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
	}

	QUEUEING_EVENT_TO_DB(EAG_AGIT_FRIEND_LIST_REQ, kPacket_);

	START_LOG(clog, L"친구아지트 목록 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_setFriendUID.size()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_AGIT_FRIEND_LIST_ACK, KSimpleAgitPageList)
{
	START_LOG(clog, L"친구아지트 목록 전달 ")
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_mapPageList.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_AGIT_FRIEND_LIST_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_TOP_RANK_LIST_REQ, DWORD)
{
	KSimpleAgitPageList kPacket;
	kPacket.m_dwUserUID = kPacket_;

	// 인기 아지트 리스트 전달
	SiKAgitManager()->GetRankAgit(kPacket.m_mapPageList);

	START_LOG(clog, L"인기아지트 목록 전달 ")
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_dwUserUID) << END_LOG;

	SEND_PACKET(EAG_AGIT_TOP_RANK_LIST_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_FAVORITE_LIST_REQ, DWORD)
{
	KSimpleAgitPageList kPacket;
	kPacket.m_dwUserUID = kPacket_;
	kPacket.m_mapPageList.clear();

	std::map<DWORD, KAgitPageInfo> mapPage;
	mapPage.clear();

	// 자신의 아지트 정보가 없으면 불러오자.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		goto END_PROC;
	}

	spAgit->GetFavorites(mapPage);
	SiKAgitManager()->BuildPageBuffer(mapPage, kPacket.m_mapPageList);
	START_LOG(clog, L"아지트 즐겨찾기 리스트 받아옴.. AgitUID : " << kPacket_) << END_LOG;

END_PROC:
	START_LOG(clog, L"즐겨찾기 목록 전달 ")
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_)
		<< BUILD_LOG(mapPage.size())
		<< BUILD_LOG(kPacket.m_mapPageList.size()) << END_LOG;

	SEND_PACKET(EAG_AGIT_FAVORITE_LIST_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_ADD_FAVORITE_REQ, KAgitUserParam)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitAddFavoriteAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_kAgitInfo.m_dwUID = kPacket_.m_dwAgitUID;

	std::map<DWORD, KAgitPageInfo> mapFavorite;
	mapFavorite.clear();

	// 내 아지트 정보가 없다.
	{
		KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
		if (spAgit == NULL) {
			QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
			SET_ERR_GOTO(ERR_AGIT_FAVORITE_03, END_PROC);
		}

		// 이미 등록된 아지트
		spAgit->GetFavorites(mapFavorite);
		if (mapFavorite.find(kPacket_.m_dwAgitUID) != mapFavorite.end()) {
			SET_ERR_GOTO(ERR_AGIT_FAVORITE_04, END_PROC);
		}

		// 즐겨찾기 개수 제한
		if (mapFavorite.size() >= (USHORT)SiKAgitManager()->GetMaxFavoriteCount()) {
			SET_ERR_GOTO(ERR_AGIT_FAVORITE_05, END_PROC);
		}
	}

	QUEUEING_EVENT_TO_DB(EAG_AGIT_ADD_FAVORITE_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_FAVORITE_02, kPacket.m_nOK, 2); // 즐겨찾기할 아지트 정보가 없다.
		ERR_CASE(ERR_AGIT_FAVORITE_03, kPacket.m_nOK, 3); // 내 아지트 정보가 없다.
		ERR_CASE(ERR_AGIT_FAVORITE_04, kPacket.m_nOK, 4); // 이미 즐겨찾기에 등록된 아지트.
		ERR_CASE(ERR_AGIT_FAVORITE_05, kPacket.m_nOK, 5); // 더이상 즐겨찾기에 등록할 수 없음.
	default:
		START_LOG(cerr, L"즐겨찾기 추가 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"즐겨찾기 추가 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(mapFavorite.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_AGIT_ADD_FAVORITE_ACK);
	}
}

_IMPL_ON_FUNC(EAG_AGIT_ADD_FAVORITE_ACK, KAgitAddFavoriteAck)
{
	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	// 내 아지트 정보가 없다.
	{
		KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
		if (spAgit == NULL) {
			QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
			goto END_PROC;
		}

		// 즐겨찾기 추가 적용
		spAgit->AddFavorites(kPacket_.m_kAgitInfo);
	}

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_FAVORITE_06, kPacket_.m_nOK, 6); // 즐겨찾기 추가 DB작업 실패.

	default:
		START_LOG(cerr, L"즐겨찾기 추가 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"즐겨찾기 추가 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_kAgitInfo.m_dwUID)
		<< BUILD_LOG(kPacket_.m_kAgitInfo.m_strName)
		<< BUILD_LOGc(kPacket_.m_kAgitInfo.m_ucOpenType) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_AGIT_ADD_FAVORITE_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;

	std::map<DWORD, KAgitPageInfo> mapFavorite;
	mapFavorite.clear();

	// 내 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		SET_ERR_GOTO(ERR_AGIT_FAVORITE_03, END_PROC);
	}

	// 즐겨찾기 목록에 없는 아지트
	spAgit->GetFavorites(mapFavorite);
	if (mapFavorite.find(kPacket_.m_dwAgitUID) == mapFavorite.end()) {
		SET_ERR_GOTO(ERR_AGIT_FAVORITE_07, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_AGIT_REMOVE_FAVORITE_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_FAVORITE_03, kPacket.m_nOK, 3); // 내 아지트 정보가 없다.
		ERR_CASE(ERR_AGIT_FAVORITE_07, kPacket.m_nOK, 7); // 즐겨찾기 목록에 없음.
	default:
		START_LOG(cerr, L"즐겨찾기 제거 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"즐겨찾기 제거 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(mapFavorite.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_AGIT_REMOVE_FAVORITE_ACK);
	}
}

_IMPL_ON_FUNC(EAG_AGIT_REMOVE_FAVORITE_ACK, KAgitUserParamAck)
{
	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	// 내 아지트 정보가 없다.
	{
		KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
		if (spAgit == NULL) {
			QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
			goto END_PROC;
		}

		// 즐겨찾기 제거
		spAgit->RemoveFavorites(kPacket_.m_dwAgitUID);
	}

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_FAVORITE_08, kPacket_.m_nOK, 8); // 즐겨찾기 제거 DB작업 실패.

	default:
		START_LOG(cerr, L"즐겨찾기 제거 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"즐겨찾기 제거 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_dwAgitUID) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_AGIT_REMOVE_FAVORITE_ACK);
}

_IMPL_ON_FUNC(EAG_REGISTER_AGIT_PAGE_REQ, KRegisterAgitPageReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KRegisterAgitPageAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_strNickName = kPacket_.m_strNickName;

	KAgitOption kOption;
	KAgitInfo kInfo;
	bool bChangeOption = false;

	// 내 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_PAGE_02, END_PROC);
	}

	// 아지트 정보
	spAgit->GetInfo(kInfo);
	kInfo.GetOption(kOption);
	kPacket.m_kAgitOption = kOption;

	// 현재 설정과 비교
	if (kOption.m_strName.compare(kPacket_.m_strName) != 0) {
		bChangeOption = true;
		kOption.m_strName = kPacket_.m_strName;
	}

	if (kOption.m_ucMaxUser != kPacket_.m_ucMaxUser) {
		bChangeOption = true;
		kOption.m_ucMaxUser = kPacket_.m_ucMaxUser;
	}

	if (kOption.m_ucOpenType != kPacket_.m_ucOpenType) {
		bChangeOption = true;
		kOption.m_ucOpenType = kPacket_.m_ucOpenType;
	}

	if (kOption.m_strPassword.compare(kPacket_.m_strPassword) != 0) {
		bChangeOption = true;
		kOption.m_strPassword = kPacket_.m_strPassword;
	}

	// 설정변경이 있을 경우, DB쓰기
	if (bChangeOption) {
		KDB_EAG_REGISTER_AGIT_PAGE_REQ kDBPacket;
		kDBPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
		kDBPacket.m_strNickName = kPacket_.m_strNickName;
		kDBPacket.m_kAgitOption = kOption;
		QUEUEING_EVENT_TO_DB(EAG_REGISTER_AGIT_PAGE_REQ, kDBPacket);

		START_LOG(clog, L"아지트 페이지 등록요청을 위한 설정변경 DB에서 쓰기요청.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		return;
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	START_LOG(clog, L"아지트 페이지 등록요청.. ServerUID : " << GetUID())
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOGc(kPacket_.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_strPassword)
		<< BUILD_LOG(kPacket_.m_strNickName) << END_LOG;

	QueueingEvent(KAgentEvent::EAG_REGISTER_AGIT_PAGE_ACK, kPacket);
}

_IMPL_ON_FUNC(EAG_REGISTER_AGIT_PAGE_ACK, KRegisterAgitPageAck)
{
	KAgitPtr spAgit;
	KAgitInfo kInfo;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	// 내 아지트 정보가 없다.
	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_AGIT_PAGE_02, END_PROC);
	}

	// 변경된 옵션을 적용
	spAgit->GetInfo(kInfo);
	spAgit->SetOption(kPacket_.m_kAgitOption);

	// 이미 등록되어있는지
	if (spAgit->IsPageRegister()) {
		SET_ERR_GOTO(ERR_AGIT_PAGE_03, END_PROC);
	}

	// 페이지 등록 요청
	if (false == SiKAgitManager()->RegisterPage(kPacket_.m_dwAgitUID)) {
		SET_ERR_GOTO(ERR_AGIT_PAGE_05, END_PROC);
	}

	// 페이지 등록된 RoomID 전달
	kPacket_.m_usRoomID = spAgit->GetRoomID();
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_PAGE_02, kPacket_.m_nOK, 2); // 내 아지트 정보가 없다.
		ERR_CASE(ERR_AGIT_PAGE_03, kPacket_.m_nOK, 3); // 이미 페이지에 등록되어 있음.
		ERR_CASE(ERR_AGIT_PAGE_04, kPacket_.m_nOK, 4); // 페이지 등록중 DB처리 실패.
		ERR_CASE(ERR_AGIT_PAGE_05, kPacket_.m_nOK, 5); // 페이지 등록중 서버처리 실패.
	default:
		START_LOG(cerr, L"아지트 페이지 등록 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"아지트 페이지 등록 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_strNickName)
		<< BUILD_LOG(kPacket_.m_usRoomID)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strName)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strPassword)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucInviteType)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strProfile) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_REGISTER_AGIT_PAGE_ACK);
}

_IMPL_ON_FUNC(EAG_UNREGISTER_AGIT_PAGE_REQ, DWORD)
{
	// 내 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		START_LOG(cwarn, L"페이지 등록해제하려는 아지트 정보가 서버에 없음. AgitUID : " << kPacket_) << END_LOG;
		goto END_PROC;
	}

	// 등록 해제여부 확인
	if (false == spAgit->IsPageRegister()) {
		START_LOG(cwarn, L"이미 페이지 등록해제 상태. AgitUID : " << kPacket_) << END_LOG;
		goto END_PROC;
	}

	// 페이지 등록해제
	LIF(SiKAgitManager()->UnregisterPage(spAgit->GetRoomID(), kPacket_));

END_PROC:
	START_LOG(clog, L"아지트 페이지 등록해제")
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_UNREGISTER_AGIT_PAGE_ACK);
}

_IMPL_ON_FUNC(EAG_AGIT_PAGE_LIST_REQ, KAgitPageList)
{
	// 인원체크 안할경우, 미리 빌드해둔 페이지를 쓴다.
	if (kPacket_.m_usMinUser == 1) {
		// 옵션별로 페이지 정보 불러오기
		SiKAgitManager()->GetPageBuffer(kPacket_);
	}
	else {
		// 인원수 체크하는 경우, 그때그때 빌드해서 전달
		SiKAgitManager()->GetCustomPage(kPacket_);
	}

	START_LOG(clog, L"아지트 페이지 전달")
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_bOpen)
		<< BUILD_LOG(kPacket_.m_bNotFull)
		<< BUILD_LOG(kPacket_.m_usMinUser)
		<< BUILD_LOG(kPacket_.m_usPageNum)
		<< BUILD_LOG(kPacket_.m_usMaxPageNum) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_AGIT_PAGE_LIST_ACK);
}

_IMPL_ON_FUNC(EAG_EQUIP_BROWNIE_REQ, KEquipBrownieReq)
{
	SET_ERROR(ERR_UNKNOWN);

	std::map<GCITEMUID, KBrownieInfo>::iterator mit;
	std::map<GCITEMUID, KBrownieInfo> mapList;
	KEquipBrownieAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_kEquip = kPacket_.m_kEquip;
	kPacket.m_kBrownieInfo = kPacket_.m_kBrownieInfo;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		SET_ERR_GOTO(ERR_AGIT_BROWNIE_04, END_PROC);
	}

	// 등록된 도우미인가
	spAgit->GetBrownieInfo(mapList);
	mit = mapList.find(kPacket_.m_kBrownieInfo.m_dwUID);
	if (mit == mapList.end()) {
		SET_ERR_GOTO(ERR_AGIT_BROWNIE_05, END_PROC);
	}
	kPacket_.m_kBrownieInfo = mit->second;

	QUEUEING_EVENT_TO_DB(EAG_EQUIP_BROWNIE_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_BROWNIE_04, kPacket.m_nOK, 4); // 도우미 정보수정할 아지트정보가 없음.
		ERR_CASE(ERR_AGIT_BROWNIE_05, kPacket.m_nOK, 5); // 보유하지 않은 도우미.

	default:
		START_LOG(cerr, L"도우미 장착 변경중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"도우미 장착 변경 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_dwUID)
		<< BUILD_LOG(mapList.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_OPTION_ACK);
	}
}

_IMPL_ON_FUNC(EAG_EQUIP_BROWNIE_ACK, KEquipBrownieAck)
{
	KAgitPtr spAgit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		goto END_PROC;
	}

	// 변경된 도우미정보 갱신
	spAgit->UpdateBrownieInfo(kPacket_.m_kBrownieInfo);

	// 아지트 인원에 알림
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_BROWNIE_BROAD, kPacket_.m_dwUserUID, kPacket_.m_kBrownieInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_BROWNIE_06, kPacket_.m_nOK, 6); // 도우미 정보변경 DB처리 실패.

	default:
		START_LOG(cerr, L"도우미 장착 변경중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"도우미 장착 변경결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_EquipItemID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_EquipItemID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_dwUID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_bIsUsed) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_EQUIP_BROWNIE_ACK);
}

_IMPL_ON_FUNC(EAG_WATERING_FAIRY_TREE_REQ, DWORD)
{
	SET_ERROR(ERR_UNKNOWN);

	KEAG_WATERING_FAIRY_TREE_REQ kDBPacket;
	kDBPacket.m_kPacket.m_dwUserUID = kPacket_;
	kDBPacket.m_kPacket.m_dwAddExp = 0;
	kDBPacket.m_bFruitChange = false;

	KWateringFairyTreeAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_;

	CTime tmCurrent = CTime::GetCurrentTime();
	CTime tmLastGrow;
	CTimeSpan tSpan;

	KFairyTreeConfig kConfig;
	SiKAgitManager()->GetFairyTreeConfig(kConfig);

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		SET_ERR_GOTO(ERR_WATERING_FAIRY_TREE_02, END_PROC);
	}

	spAgit->GetFairyTree(kDBPacket.m_kPacket.m_kFairyTree);
	kPacket.m_kFairyTree = kDBPacket.m_kPacket.m_kFairyTree;
	tmLastGrow = CTime(kDBPacket.m_kPacket.m_kFairyTree.m_tmLastGrowTime);
	tSpan = tmCurrent - tmLastGrow;

	// 날짜가 지났는가?
	if (tmLastGrow.GetYear() < tmCurrent.GetYear() ||
		(tmLastGrow.GetYear() == tmCurrent.GetYear() && tmLastGrow.GetMonth() < tmCurrent.GetMonth()) ||
		(tmLastGrow.GetYear() == tmCurrent.GetYear() && tmLastGrow.GetMonth() == tmCurrent.GetMonth() && tmLastGrow.GetDay() < tmCurrent.GetDay())) {
		kDBPacket.m_kPacket.m_dwAddExp = kConfig.m_dwAddExpFirstDay;
		START_LOG(clog, L"요정의 나무 일일 최초 물주기 경험치 획득 AgitUID : " << kPacket_) << END_LOG;
	}
	else if (tSpan.GetTotalMinutes() > kConfig.m_dwWateringTimeTerm) {
		kDBPacket.m_kPacket.m_dwAddExp = kConfig.m_dwAddExpTime;
		START_LOG(clog, L"요정의 나무 시간주기별 물주기 경험치 획득 AgitUID : " << kPacket_) << END_LOG;
	}
	else {
		START_LOG(clog, L"요정의 나무 변화없음 (물주기 주기가 아님) AgitUID : " << kPacket_) << END_LOG;
	}

	// 획득하는 경험치가 있으면
	if (kDBPacket.m_kPacket.m_dwAddExp > 0) {
		DWORD dwTotalExp = kDBPacket.m_kPacket.m_kFairyTree.m_dwExp + kDBPacket.m_kPacket.m_dwAddExp;
		int nLv = SiKAgitManager()->GetFairyTreeLv(kConfig.m_vecLvExp, dwTotalExp);

		if (dwTotalExp < kDBPacket.m_kPacket.m_kFairyTree.m_dwExp) {
			START_LOG(cerr, L"요정의나무 총경험치가 감소. AgitUID : " << kPacket_)
				<< BUILD_LOG(nLv)
				<< BUILD_LOG(dwTotalExp)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_dwExp)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nLv) << END_LOG;
			SET_ERR_GOTO(ERR_WATERING_FAIRY_TREE_03, END_PROC);
		}

		if (nLv < kDBPacket.m_kPacket.m_kFairyTree.m_nLv) {
			START_LOG(cerr, L"요정의나무 레벨이 감소. AgitUID : " << kPacket_)
				<< BUILD_LOG(nLv)
				<< BUILD_LOG(dwTotalExp)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_dwExp)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nLv) << END_LOG;
			SET_ERR_GOTO(ERR_WATERING_FAIRY_TREE_04, END_PROC);
		}

		if (nLv > kDBPacket.m_kPacket.m_kFairyTree.m_nLv) {
			// 열매를 사용하지 않았으면 교체
			if (false == kDBPacket.m_kPacket.m_kFairyTree.m_bUseFruit) {
				kDBPacket.m_bFruitChange = true;
				kDBPacket.m_kPacket.m_kFairyTree.m_nFruitType = SiKAgitManager()->GetTodayFruitType(nLv);
				kDBPacket.m_kPacket.m_kFairyTree.m_tmUpdateFruit = KncUtil::TimeToInt(CTime::GetCurrentTime());

				START_LOG(clog, L"레벨업으로 요정의나무 열매 설정.. AgitUID : " << kPacket_)
					<< BUILD_LOG(nLv)
					<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nLv)
					<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nFruitType) << END_LOG;
			}

			START_LOG(clog, L"요정의나무 레벨이 증가. AgitUID : " << kPacket_)
				<< BUILD_LOG(nLv)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nFruitType)
				<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_bUseFruit)
				<< BUILD_LOGtm(CTime(kDBPacket.m_kPacket.m_kFairyTree.m_tmUpdateFruit)) << END_LOG;
		}

		kDBPacket.m_kPacket.m_kFairyTree.m_nLv = nLv;
		kDBPacket.m_kPacket.m_kFairyTree.m_dwExp = dwTotalExp;
		kDBPacket.m_kPacket.m_kFairyTree.m_tmLastGrowTime = KncUtil::TimeToInt(tmCurrent);

		QUEUEING_EVENT_TO_DB(EAG_WATERING_FAIRY_TREE_REQ, kDBPacket);
		START_LOG(clog, L"요정의 나무 경험치 획득 DB기록 요청. AgitUID : " << kPacket_)
			<< BUILD_LOG(kDBPacket.m_kPacket.m_dwAddExp)
			<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_nLv)
			<< BUILD_LOG(kDBPacket.m_kPacket.m_kFairyTree.m_dwExp)
			<< BUILD_LOGtm(CTime(kDBPacket.m_kPacket.m_kFairyTree.m_tmLastGrowTime))
			<< BUILD_LOGtm(CTime(kDBPacket.m_kPacket.m_kFairyTree.m_tmLastDecreaseTime))
			<< BUILD_LOGtm(tmCurrent) << END_LOG;
		return;
	}

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_WATERING_FAIRY_TREE_02, kPacket.m_nOK, 2); // 물주기 진행할 아지트 정보가 없음.
		ERR_CASE(ERR_WATERING_FAIRY_TREE_03, kPacket.m_nOK, 3); // 요정의나무 총경험치가 감소.
		ERR_CASE(ERR_WATERING_FAIRY_TREE_04, kPacket.m_nOK, 4); // 요정의나무 레벨이 감소.

	default:
		START_LOG(cerr, L"요정의나무 물주기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"요정의나무 물주기 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nFruitType)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmUpdateFruit))
		<< BUILD_LOG(kPacket.m_kFairyTree.m_bUseFruit)
		<< BUILD_LOGtm(tmCurrent)
		<< BUILD_LOG(kConfig.m_dwAddExpFirstDay)
		<< BUILD_LOG(kConfig.m_dwAddExpTime)
		<< BUILD_LOG(kConfig.m_dwWateringTimeTerm)
		<< BUILD_LOG(tSpan.GetTotalMinutes())
		<< BUILD_LOG(kDBPacket.m_bFruitChange) << END_LOG;

	SEND_PACKET(EAG_WATERING_FAIRY_TREE_ACK);
}

_IMPL_ON_FUNC(EAG_WATERING_FAIRY_TREE_ACK, KWateringFairyTreeAck)
{
	KAgitPtr spAgit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
		goto END_PROC;
	}

	// 변경된 나무정보 갱신
	spAgit->SetFairyTree(kPacket_.m_kFairyTree);

	// 아지트 인원에 알림
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_CHANGE_FAIRY_TREE_BROAD, kPacket_.m_dwUserUID, kPacket_.m_kFairyTree);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_WATERING_FAIRY_TREE_05, kPacket_.m_nOK, 5); // 요정의나무 물주기 DB작업 실패.

	default:
		START_LOG(cerr, L"요정의나무 물주기 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"요정의나무 물주기결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_nFruitType)
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_bUseFruit)
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmUpdateFruit)) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_WATERING_FAIRY_TREE_ACK);
}

_IMPL_ON_FUNC(EAG_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwUserUID);
	}

	START_LOG(clog, L"요정의나무 열매버프 정보 전달.. AgitUID : " << kPacket_.m_dwUserUID) << END_LOG;
	SEND_RECEIVED_PACKET(EAG_FAIRY_TREE_BUFF_NOT);
}

_IMPL_ON_FUNC(EAG_USE_FAIRY_TREE_FRUIT_REQ, DWORD)
{
	//요정의 나무 열매먹기
	SET_ERROR(ERR_UNKNOWN);

	KFairyTree kFairyTree;

	KEAG_USE_FAIRY_TREE_FRUIT_REQ kDBPacket;
	kDBPacket.m_dwAgitUID = kPacket_;
	kDBPacket.m_nFruitType = KFairyTreeConfig::FBT_NONE;

	KUseFairyTreeFruitAck kPacket;
	kPacket.m_kFruitBuff.m_dwUserUID = kPacket_;
	kPacket.m_nOK = -99;

	// 아지트 정보가 없다.
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		SET_ERR_GOTO(ERR_USE_FAIRY_TREE_FRUIT_02, END_PROC);
	}

	spAgit->GetFairyTree(kFairyTree);
	// 열매정보 전달
	kPacket.m_kFruitBuff.m_nTreeLv = kFairyTree.m_nLv;
	kPacket.m_kFruitBuff.m_nFruitType = kFairyTree.m_nFruitType;
	kPacket.m_kFruitBuff.m_fBuffRate = SiKAgitManager()->GetFruitBuffRate(kFairyTree.m_nLv, kFairyTree.m_nFruitType);
	kPacket.m_kFruitBuff.m_tmBuffTime = kFairyTree.m_tmUpdateFruit;

	// 이미 열매를 먹었음.
	if (kFairyTree.m_bUseFruit) {
		SET_ERR_GOTO(ERR_USE_FAIRY_TREE_FRUIT_03, END_PROC);
	}

	// 먹을 수 있는 열매가 없음.
	if (kFairyTree.m_nFruitType == KFairyTreeConfig::FBT_NONE) {
		SET_ERR_GOTO(ERR_USE_FAIRY_TREE_FRUIT_04, END_PROC);
	}
	// 먹은 열매타입
	kDBPacket.m_nFruitType = kFairyTree.m_nFruitType;

	QUEUEING_EVENT_TO_DB(EAG_USE_FAIRY_TREE_FRUIT_REQ, kDBPacket);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_USE_FAIRY_TREE_FRUIT_02, kPacket.m_nOK, 2); // 열매먹기 진행할 아지트 정보가 없음.
		ERR_CASE(ERR_USE_FAIRY_TREE_FRUIT_03, kPacket.m_nOK, 3); // 이미 열매를 먹었음.
		ERR_CASE(ERR_USE_FAIRY_TREE_FRUIT_04, kPacket.m_nOK, 4); // 먹을 수 있는 열매가 없음.

	default:
		START_LOG(cerr, L"열매먹기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"열매먹기 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_kFruitBuff.m_nTreeLv)
		<< BUILD_LOG(kPacket.m_kFruitBuff.m_nFruitType)
		<< BUILD_LOG(kPacket.m_kFruitBuff.m_fBuffRate)
		<< BUILD_LOGtm(CTime(kPacket.m_kFruitBuff.m_tmBuffTime))
		<< BUILD_LOG(kFairyTree.m_bUseFruit)
		<< BUILD_LOG(kFairyTree.m_nLv)
		<< BUILD_LOG(kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kFairyTree.m_tmUpdateFruit)) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_USE_FAIRY_TREE_FRUIT_ACK);
	}
}

_IMPL_ON_FUNC(EAG_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck)
{
	KAgitPtr spAgit;
	KFairyTree kFairyTree;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_kFruitBuff.m_dwUserUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_kFruitBuff.m_dwUserUID);
		goto END_PROC;
	}

	// 변경된 나무정보 갱신
	spAgit->GetFairyTree(kFairyTree);
	kFairyTree.m_bUseFruit = true;
	spAgit->SetFairyTree(kFairyTree);

	// 열매정보 전달
	kPacket_.m_kFruitBuff.m_nTreeLv = kFairyTree.m_nLv;
	kPacket_.m_kFruitBuff.m_nFruitType = kFairyTree.m_nFruitType;
	kPacket_.m_kFruitBuff.m_fBuffRate = SiKAgitManager()->GetFruitBuffRate(kFairyTree.m_nLv, kFairyTree.m_nFruitType);
	kPacket_.m_kFruitBuff.m_tmBuffTime = kFairyTree.m_tmUpdateFruit;

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_USE_FAIRY_TREE_FRUIT_05, kPacket_.m_nOK, 5); // 열매먹기 DB작업 실패.

	default:
		START_LOG(cerr, L"요정의나무 열매먹기 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"요정의나무 열매먹기 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_kFruitBuff.m_nTreeLv)
		<< BUILD_LOG(kPacket_.m_kFruitBuff.m_nFruitType)
		<< BUILD_LOG(kPacket_.m_kFruitBuff.m_fBuffRate)
		<< BUILD_LOGtm(CTime(kPacket_.m_kFruitBuff.m_tmBuffTime))
		<< BUILD_LOG(kFairyTree.m_bUseFruit)
		<< BUILD_LOG(kFairyTree.m_nLv)
		<< BUILD_LOG(kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kFairyTree.m_tmUpdateFruit)) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_USE_FAIRY_TREE_FRUIT_ACK);
}

_IMPL_ON_FUNC(EAG_ENTER_AGIT_SID_REQ, KEnterAgitReq)
{
	// Agent Master만 받고 처리해야하는 패킷.
	// 코드적인 제약이나 보완책이 필요할 것.
	KEnterAgitSIDAck kPacket;
	kPacket.m_kEnterAgitReq = kPacket_;
	kPacket.m_nSID = -1;

	// SID 등록되어 있으면 전달
	kPacket.m_nSID = SiKSwitchUserSID()->GetUserSID(kPacket_.m_dwAgitUID);
	if (kPacket.m_nSID > -1) {
		START_LOG(clog, L"아지트 정보가 있는 AgentServer ID 전달.. AgitUID : " << kPacket_.m_dwAgitUID << L", SID : " << kPacket.m_nSID) << END_LOG;
		goto End_PROC;
	}

	// SID 없음 획득 요청
	kPacket.m_nSID = SiKSwitchUserSID()->GetAvailableSlaveSID();
	if (kPacket.m_nSID == -1) {
		START_LOG(cerr, L"아지트 정보를 더이상 생성할 수 있는 에이전트 서버가 없음. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		goto End_PROC;
	}

	// 해당 서버에 아지트 정보 설정 요청
	{
		KEnterAgitSIDReq kAgPacket;
		kAgPacket.m_dwServerUID = GetUID();
		kAgPacket.m_kEnterAgitReq = kPacket_;
		SiKAGSimLayer()->m_kActorManager2.SendTo((DWORD)kPacket.m_nSID, KAgentEvent::EAGS_SET_AGIT_SID_REQ, kAgPacket);
		START_LOG(clog, L"아지트정보 AgentServer에 설정요청.. AgitUID : " << kPacket_.m_dwAgitUID << L", SID : " << kPacket.m_nSID) << END_LOG;
		return;
	}

End_PROC:
	// 생성실패 상태거나, 등록되어 있는 정보를 전달했거나.
	SEND_PACKET(EAG_ENTER_AGIT_SID_ACK);
	START_LOG(clog, L"Send EnterAgitSID Ack.. AgitUID : " << kPacket_.m_dwAgitUID << L", SID : " << kPacket.m_nSID) << END_LOG;
}

_IMPL_ON_FUNC(EAG_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck)
{
	SEND_RECEIVED_PACKET(EAG_ENTER_AGIT_SID_ACK);
	START_LOG(clog, L"Send EnterAgitSID Ack.. AgitUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID << L", SID : " << kPacket_.m_nSID) << END_LOG;
}

_IMPL_ON_FUNC(EAG_USER_CONNECT_NOT, KUserTimeParam)
{
	// Agent Master Packet
	int nSID = SiKSwitchUserSID()->GetUserSID(kPacket_.m_dwUserUID);
	if (nSID > -1) {
		SiKAGSimLayer()->m_kActorManager2.SendTo((DWORD)nSID, KAgentEvent::EAGS_USER_CONNECT_NOT, kPacket_);
	}

	SiKSwitchUserSID()->SetUserConnect(true, kPacket_.m_dwUserUID, kPacket_.m_tmTime);
	START_LOG(clog, L"Connect UserUID : " << kPacket_.m_dwUserUID << L", ServerUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_USER_DISCONNECT_NOT, KUserTimeParam)
{
	// Agent Master Packet
	int nSID = SiKSwitchUserSID()->GetUserSID(kPacket_.m_dwUserUID);
	if (nSID > -1) {
		SiKAGSimLayer()->m_kActorManager2.SendTo((DWORD)nSID, KAgentEvent::EAGS_USER_DISCONNECT_NOT, kPacket_);
	}

	SiKSwitchUserSID()->SetUserConnect(false, kPacket_.m_dwUserUID, kPacket_.m_tmTime);
	START_LOG(clog, L"Disconnect UserUID : " << kPacket_.m_dwUserUID << L", SID : " << nSID) << END_LOG;
}

IMPL_ON_FUNC(EAG_LOAD_AGIT_INFO_NOT)
{
	JIF(SiKAgitManager()->Add(kPacket_.m_kInfo));

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_kInfo.m_dwUID);
	if (spAgit != NULL) {
		spAgit->SetOwnerNickname(kPacket_.m_strNick);
		spAgit->SetFlowerPot(kPacket_.m_mapFlowerPot);
		spAgit->SetFavorites(kPacket_.m_mapFavoritesInfo);
		spAgit->SetBrownieInfo(kPacket_.m_mapBrownie);
		spAgit->SetFairyTree(kPacket_.m_kTree);
		spAgit->SetGuestbook(kPacket_.m_mapGuestbook);
		spAgit->SetTrainingObj(kPacket_.m_mapTrainingObj);
	}

	START_LOG(clog, L"Load Agit Info.. AgitUID : " << kPacket_.m_kInfo.m_dwUID) << END_LOG;
}

IMPL_ON_FUNC(EAG_ENTER_AGIT_ACK)
{
	SET_ERROR(ERR_UNKNOWN);

	KEnterAgitAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwEnterUserUID = kPacket_.m_kEnterAgitReq.m_dwEnterUserUID;
	kPacket.m_kInfo.m_dwUID = kPacket_.m_kEnterAgitReq.m_dwAgitUID;
	kPacket.m_strPassword = kPacket_.m_kEnterAgitReq.m_strPassword;

	KAgitPtr spAgit;

	if (false == SiKAgitManager()->Add(kPacket_.m_kAgitLoadInfo.m_kInfo)) {
		START_LOG(cwarn, L"아지트 가용 한계로 아지트 추가실패.. AgitUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID) << END_LOG;
		SET_ERR_GOTO(ERR_AGIT_10, END_PROC);
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_kAgitLoadInfo.m_kInfo.m_dwUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_);
		START_LOG(cwarn, L"DB에서 아지트 정보 받아왔는데, 아지트 정보가 없음.. AgitUID : " << kPacket_.m_kEnterAgitReq.m_dwAgitUID) << END_LOG;
		SET_ERR_GOTO(ERR_AGIT_01, END_PROC);
	}

	// 아지트 정보 설정
	spAgit->SetOwnerNickname(kPacket_.m_kAgitLoadInfo.m_strNick);
	spAgit->SetFlowerPot(kPacket_.m_kAgitLoadInfo.m_mapFlowerPot);
	spAgit->SetFavorites(kPacket_.m_kAgitLoadInfo.m_mapFavoritesInfo);
	spAgit->SetBrownieInfo(kPacket_.m_kAgitLoadInfo.m_mapBrownie);
	spAgit->SetFairyTree(kPacket_.m_kAgitLoadInfo.m_kTree);
	spAgit->SetGuestbook(kPacket_.m_kAgitLoadInfo.m_mapGuestbook);
	spAgit->SetTrainingObj(kPacket_.m_kAgitLoadInfo.m_mapTrainingObj);
	START_LOG(clog, L"Load Enter Agit Info.. AgitUID : " << kPacket_.m_kAgitLoadInfo.m_kInfo.m_dwUID) << END_LOG;

	// 아지트 정보 전달
	spAgit->GetInfo(kPacket.m_kInfo);
	spAgit->GetTRServerIPPort(kPacket.m_prTRServerIpPort);
	spAgit->GetURServerIPPort(kPacket.m_prURServerIpPort);
	spAgit->GetFlowerPot(kPacket.m_mapFlowerPot);
	spAgit->GetBrownieInfo(kPacket.m_mapBrownie);
	spAgit->GetFairyTree(kPacket.m_kFairyTree);
	spAgit->GetTrainingObjUpdate(kPacket.m_mapTrainingObj);

	// 입장요청시, 삭제안하도록 설정
	spAgit->SetReserveDestroy(false);

	// 주인이 아니면
	if (kPacket_.m_kEnterAgitReq.m_dwEnterUserUID != kPacket.m_kInfo.m_dwUID) {
		// 인기 아지트가 아니고,
		if (false == SiKAgitManager()->IsRankAgit(kPacket.m_kInfo.m_dwUID)) {
			// 주인이 없을때 입장할 수 없음.
			if (false == spAgit->IsOwnerConnect()) {
				START_LOG(cwarn, L"아지트 주인이 게임에 접속해있지않음.. AgitUID : " << kPacket.m_kInfo.m_dwUID)
					<< BUILD_LOG(spAgit->IsOwnerConnect()) << END_LOG;
				SET_ERR_GOTO(ERR_AGIT_09, END_PROC);
			}
		}

		// 비공개 - 비밀번호 체크
		if (kPacket.m_kInfo.m_ucOpenType == KAgitInfo::OT_CLOSE) {
			if (kPacket.m_kInfo.m_strPassword.compare(kPacket_.m_kEnterAgitReq.m_strPassword) != 0) {
				SET_ERR_GOTO(ERR_AGIT_07, END_PROC);
			}
		}

		// 인원체크
		if (spAgit->IsFull()) {
			SET_ERR_GOTO(ERR_AGIT_05, END_PROC);
		}
	}
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_01, kPacket.m_nOK, 1); // 아지트 정보를 DB에서 받아올 수 없음.
		ERR_CASE(ERR_AGIT_05, kPacket.m_nOK, 5); // 입장할 아지트 정원이 초과됨.
		ERR_CASE(ERR_AGIT_06, kPacket.m_nOK, 6); // 입장할 아지트는 친구만 입장 가능.
		ERR_CASE(ERR_AGIT_07, kPacket.m_nOK, 7); // 입장할 아지트의 비밀번호가 틀림.
		ERR_CASE(ERR_AGIT_09, kPacket.m_nOK, 9); // 주인이 접속중이 아니다.
		ERR_CASE(ERR_AGIT_10, kPacket.m_nOK, 10); // 아지트서버 포화상태.

	default:
		START_LOG(cerr, L"내 아지트 입장처리 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"내 아지트 입장 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_kInfo.m_dwUID)
		<< BUILD_LOG(kPacket.m_kInfo.m_strName)
		<< BUILD_LOGc(kPacket.m_kInfo.m_dwMapID)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucOpenType)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucInviteType)
		<< BUILD_LOGc(kPacket.m_kInfo.m_ucMaxUser)
		<< BUILD_LOG(kPacket.m_kInfo.m_nTodayVisiter)
		<< BUILD_LOG(kPacket.m_kInfo.m_nTotalVisiter)
		<< BUILD_LOGc(kPacket.m_kInfo.m_kProfile.m_ucTheme)
		<< BUILD_LOG(kPacket.m_kInfo.m_kProfile.m_strProfile)
		<< BUILD_LOG(kPacket.m_kInfo.m_vecObjectPos.size())
		<< BUILD_LOG(kPacket.m_kInfo.m_mapCharacterPos.size())
		<< BUILD_LOGtm(CTime(kPacket.m_kInfo.m_tmLastCleaning))
		<< BUILD_LOG(kPacket.m_kInfo.m_strPassword)
		<< BUILD_LOG(kPacket.m_mapFlowerPot.size())
		<< BUILD_LOG(kPacket.m_mapBrownie.size())
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmUpdateFruit))
		<< BUILD_LOG(kPacket.m_kFairyTree.m_bUseFruit)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_tmUpdateFruit)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nFruitType)
		<< BUILD_LOG(kPacket.m_strPassword)
		<< BUILD_LOG(kPacket_.m_kAgitLoadInfo.m_mapGuestbook.size())
		<< BUILD_LOG(kPacket.m_mapTrainingObj.size())
		<< BUILD_LOG(sizeof(kPacket)) << END_LOG;

	SEND_COMPRESS_PACKET(EAG_ENTER_AGIT_ACK, kPacket);
}

IMPL_ON_FUNC(EAG_DELETE_FOR_AGIT_NOT)
{
	KAgitInfo kInfo;
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		START_LOG(clog, L"아지트 오브젝트 삭제할 오브젝트 정보가 없음.. 패스 처리.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		return;
	}

	// 아지트 정보
	spAgit->GetInfo(kInfo);

	int nDelCount = 0;
	std::vector<GCITEMUID>::iterator vitDel;
	for (vitDel = kPacket_.m_vecDeleteObject.begin(); vitDel != kPacket_.m_vecDeleteObject.end(); ++vitDel) {
		std::vector<KAgitObjectPos>::iterator vit;
		vit = std::find_if(kInfo.m_vecObjectPos.begin(), kInfo.m_vecObjectPos.end(),
			boost::bind(&KAgitObjectPos::m_UID, _1) == *vitDel);

		if (vit == kInfo.m_vecObjectPos.end()) {
			continue;
		}

		kInfo.m_vecObjectPos.erase(vit);
		++nDelCount;
	}

	spAgit->SetInfo(kInfo);

	START_LOG(clog, L"삭제된 아이템 정보 갱신.. AgitUID : " << kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kInfo.m_vecObjectPos.size())
		<< BUILD_LOG(kPacket_.m_vecDeleteObject.size())
		<< BUILD_LOG(nDelCount) << END_LOG;
}

_IMPL_ON_FUNC(EAG_GUESTBOOK_PAGE_REQ, KAgitUserParam)
{
	SET_ERROR(ERR_UNKNOWN);

	std::map<int, KGuestMsg> mapGusetbook;
	mapGusetbook.clear();

	KGuestBookPageList kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_buffCompList.Clear();

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_GUESTBOOK_02, END_PROC);
	}

	spAgit->GetGuestbook(mapGusetbook);
	SiKAgitManager()->BuildGuestBookBuffer(mapGusetbook, kPacket.m_buffCompList);

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_GUESTBOOK_02, kPacket.m_nOK, 2); // 아지트 정보가 없음.

	default:
		START_LOG(cerr, L"낙서장 리스트 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"낙서장 리스트 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_dwAgitUID)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(mapGusetbook.size()) << END_LOG;

	SEND_PACKET(EAG_GUESTBOOK_PAGE_ACK);
}

_IMPL_ON_FUNC(EAG_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq)
{
	SET_ERROR(ERR_UNKNOWN);

	std::map<int, KGuestMsg> mapGusetbook;
	mapGusetbook.clear();

	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_GUESTBOOK_02, END_PROC);
	}

	// 주인이 아니면 일일 쓰기제한 체크
	if (kPacket_.m_dwAgitUID != kPacket_.m_dwUserUID) {
		spAgit->GetGuestbook(mapGusetbook);
		if (false == SiKAgitManager()->IsWriteEnable(kPacket_.m_dwUserUID, mapGusetbook)) {
			SET_ERR_GOTO(ERR_GUESTBOOK_11, END_PROC);
		}
	}

	QUEUEING_EVENT_TO_DB(EAG_GUESTBOOK_WRITE_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_GUESTBOOK_02, kPacket.m_nOK, 2); // 아지트 정보가 없음.
		ERR_CASE(ERR_GUESTBOOK_11, kPacket.m_nOK, 11); // 일일 쓰기 제한에 걸림

	default:
		START_LOG(cerr, L"낙서장 글쓰기 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"낙서장 글쓰기 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_strNick)
		<< BUILD_LOG(kPacket_.m_strMsg) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_GUESTBOOK_WRITE_ACK);
	}
}

IMPL_ON_FUNC(EAG_GUESTBOOK_WRITE_ACK)
{
	KAgitPtr spAgit;
	KGuestMsg kMsg;
	kMsg.m_dwUserUID = kPacket_.m_kGuestbookWriteReq.m_dwUserUID;
	kMsg.m_strNick = kPacket_.m_kGuestbookWriteReq.m_strNick;
	kMsg.m_strMsg = kPacket_.m_kGuestbookWriteReq.m_strMsg;
	kMsg.m_tmDate = kPacket_.m_tmDate;

	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_kGuestbookWriteReq.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_kGuestbookWriteReq.m_dwUserUID;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket.m_dwAgitUID);
		goto END_PROC;
	}

	// 낙서장 글 추가
	spAgit->AddGuestMsg(kPacket_.m_nNo, kMsg);

	{
		// 아지트내 유저들에게 보낸다
		KGuestBookWriteBroad kAgPacket;
		kAgPacket.m_dwAgitUID = kPacket.m_dwAgitUID;
		kAgPacket.m_nNo = kPacket_.m_nNo;
		kAgPacket.m_kGuestMsg = kMsg;
		SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_GUESTBOOK_WRITE_BROAD, kPacket.m_dwAgitUID, kAgPacket);
	}

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_GUESTBOOK_06, kPacket.m_nOK, 6); // 낙서장 새글쓰기 DB처리 실패.

	default:
		START_LOG(cerr, L"낙서장 글쓰기 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"낙서장 글쓰기 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_dwAgitUID)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_nNo)
		<< BUILD_LOG(kMsg.m_dwUserUID)
		<< BUILD_LOG(kMsg.m_strNick)
		<< BUILD_LOG(kMsg.m_strMsg)
		<< BUILD_LOGtm(CTime(kMsg.m_tmDate)) << END_LOG;

	SEND_PACKET(EAG_GUESTBOOK_WRITE_ACK);
}

_IMPL_ON_FUNC(EAG_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket_.m_dwAgitUID);
		SET_ERR_GOTO(ERR_GUESTBOOK_02, END_PROC);
	}

	if (kPacket_.m_dwAgitUID != kPacket_.m_dwUserUID) {
		std::map<int, KGuestMsg> mapMsg;
		spAgit->GetGuestbook(mapMsg);

		std::set<int>::iterator sit;
		for (sit = kPacket_.m_setDeleteNo.begin(); sit != kPacket_.m_setDeleteNo.begin(); ++sit) {
			std::map<int, KGuestMsg>::iterator mit;
			mit = mapMsg.find(*sit);
			if (mit == mapMsg.end()) {
				continue;
			}

			// 주인이 아닌데 다른사람의 글을 삭제하려고 함.
			if (mit->second.m_dwUserUID != kPacket_.m_dwUserUID) {
				SET_ERR_GOTO(ERR_GUESTBOOK_08, END_PROC);
			}
		}
	}

	QUEUEING_EVENT_TO_DB(EAG_GUESTBOOK_DELETE_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_GUESTBOOK_02, kPacket.m_nOK, 2); // 아지트 정보가 없음.
		ERR_CASE(ERR_GUESTBOOK_08, kPacket.m_nOK, 8); // 아지트 주인이 아닌데 다른 사람의 낙서장 글을 삭제하려고 함.

	default:
		START_LOG(cerr, L"낙서장 글삭제 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"낙서장 글삭제 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_setDeleteNo.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_GUESTBOOK_DELETE_ACK);
	}
}

IMPL_ON_FUNC(EAG_GUESTBOOK_DELETE_ACK)
{
	KAgitPtr spAgit;

	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_kGuestBookDeleteReq.m_dwAgitUID;
	kPacket.m_dwUserUID = kPacket_.m_kGuestBookDeleteReq.m_dwUserUID;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket.m_dwAgitUID);
	if (spAgit == NULL) {
		QUEUEING_EVENT_TO_DB(EAG_LOAD_AGIT_INFO_NOT, kPacket.m_dwAgitUID);
		goto END_PROC;
	}

	// 낙서장 글 삭제
	spAgit->DeleteGuestMsg(kPacket_.m_kGuestBookDeleteReq.m_dwUserUID, kPacket_.m_kGuestBookDeleteReq.m_setDeleteNo);

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_GUESTBOOK_DELETE_BROAD, kPacket.m_dwAgitUID, kPacket_.m_kGuestBookDeleteReq);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_GUESTBOOK_09, kPacket.m_nOK, 9); // 낙서장 글삭제 DB처리 실패.

	default:
		START_LOG(cerr, L"낙서장 글삭제 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"낙서장 글삭제 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_dwAgitUID)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_kGuestBookDeleteReq.m_setDeleteNo.size()) << END_LOG;

	SEND_PACKET(EAG_GUESTBOOK_DELETE_ACK);
}

_IMPL_ON_FUNC(EAG_USE_TRAINING_OBJ_REQ, KUseTrainingObjReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KUseTrainingObjAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kMaterialItem = kPacket_.m_kMaterialItem;
	kPacket.m_kTrainingObj.m_ItemUID = kPacket_.m_TrainingObjUID;
	kPacket.m_nUseMaterialSize = 0;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		// doubt.아지트가 등록이 안되어 있거나, 잘못된 agent slave로 보낸거다.
		// Master에게 해당 Agit의 SID를 구해서 게임서버들에 전달하도록 패킷을 보내자.
		SET_ERR_GOTO(ERR_TRAINING_OBJ_07, END_PROC);
	}

	// 등록된 훈련소가 아니다.
	if (false == spAgit->GetCurrentTrainingObj(kPacket_.m_TrainingObjUID, kPacket.m_kTrainingObj)) {
		SET_ERR_GOTO(ERR_TRAINING_OBJ_08, END_PROC);
	}

	// 훈련할 수 있는 회수
	if (kPacket_.m_TrainingObjUID <= 0) {
		SET_ERR_GOTO(ERR_TRAINING_OBJ_09, END_PROC);
	}

	// 실제 수행가능한 훈련횟수
	kPacket.m_nUseMaterialSize = std::min<int>(kPacket.m_kTrainingObj.m_nAccCount, kPacket_.m_kMaterialItem.m_nCount);
	if (kPacket.m_nUseMaterialSize <= 0) {
		SET_ERR_GOTO(ERR_TRAINING_OBJ_10, END_PROC);
	}

	// 사용된 훈련소 장작 차감
	kPacket.m_kTrainingObj.m_nAccCount -= kPacket.m_nUseMaterialSize;
	kPacket.m_kTrainingObj.m_nAccCount = std::max<int>(kPacket.m_kTrainingObj.m_nAccCount, 0);
	kPacket.m_kTrainingObj.m_tmDate = KncUtil::TimeToInt(CTime::GetCurrentTime());

	// 훈련소 정보 갱신
	spAgit->UpdateTrainingObj(kPacket.m_kTrainingObj);

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_TRAINING_OBJ_BROAD, kPacket.m_dwUserUID, kPacket.m_kTrainingObj);

	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_TRAINING_OBJ_07, kPacket.m_nOK, 7); // 아지트 정보가 없음.
		ERR_CASE(ERR_TRAINING_OBJ_08, kPacket.m_nOK, 8); // 등록된 훈련소가 아니다.
		ERR_CASE(ERR_TRAINING_OBJ_09, kPacket.m_nOK, 9); // 훈련할 수 있는 횟수가 없음.
		ERR_CASE(ERR_TRAINING_OBJ_10, kPacket.m_nOK, 10); // 실제 훈련할 수 있는 횟수가 없음.

	default:
		START_LOG(cerr, L"훈련소 사용요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"훈련소 사용요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_nUseMaterialSize)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_TrainingObjUID)
		<< BUILD_LOG(kPacket_.m_kMaterialItem.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kMaterialItem.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kMaterialItem.m_nCount)
		<< BUILD_LOG(kPacket.m_kTrainingObj.m_ItemUID)
		<< BUILD_LOG(kPacket.m_kTrainingObj.m_nAccCount)
		<< BUILD_LOG(kPacket.m_kTrainingObj.m_nInitAccCount)
		<< BUILD_LOGtm(CTime(kPacket.m_kTrainingObj.m_tmDate)) << END_LOG;

	SEND_PACKET(EAG_USE_TRAINING_OBJ_ACK);
}

IMPL_ON_FUNC(EAG_UPDATE_TRAINING_OBJ_ACK)
{
	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit != NULL) {
		// 훈련소 정보 갱신
		std::map<GCITEMUID, KTrainingObj>::iterator mit;
		for (mit = kPacket_.m_mapTrainingObj.begin(); mit != kPacket_.m_mapTrainingObj.end(); ++mit) {
			spAgit->UpdateTrainingObj(mit->second);
		}

		// 아지트내 유저들에게 보낸다
		SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_TRAINING_OBJ_BROAD, kPacket_.m_dwAgitUID, mit->second);
	}

	START_LOG(clog, L"훈련소 새로 배치된 정보 갱신.. AgitUID : " << kPacket_.m_dwAgitUID << L", size : " << kPacket_.m_mapTrainingObj.size()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_OPEN_AGIT_CHARACTER_REQ, PAIR_DWORD_INT)
{
	SET_ERROR(ERR_UNKNOWN);

	KOpenAgitCharacterAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.first;
	kPacket.m_nCharType = kPacket_.second;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket.m_dwUserUID);
	if (spAgit == NULL) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_05, END_PROC);
	}

	// 이미 오픈되어 있는 캐릭터인가?
	if (spAgit->IsCharacterPosOpen(kPacket.m_nCharType)) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_07, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_OPEN_AGIT_CHARACTER_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_05, kPacket.m_nOK, 5); // 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_CHARACTER_07, kPacket.m_nOK, 7); // 이미 오픈된 캐릭터 타입
	default:
		START_LOG(cerr, L"캐릭터배치 오픈 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"캐릭터배치 오픈 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket.m_nCharType) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_OPEN_AGIT_CHARACTER_ACK);
	}
}

_IMPL_ON_FUNC(EAG_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck)
{
	KAgitPtr spAgit;
	KAgitCharacterPos kCharPos;
	kCharPos.m_nCharType = kPacket_.m_nCharType;

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwUserUID);
	if (spAgit == NULL) {
		goto END_PROC;
	}

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		if (!kPacket_.m_mapCharacterPos.empty()) { // DB에서 다시 가져온 캐릭터 정보 갱신.
			spAgit->UpdateTotalCharacterInfo(kPacket_.m_mapCharacterPos);
		}
		goto END_PROC;
	}

	// 캐릭터배치 오픈적용
	spAgit->UpdateCharacterPos(kPacket_.m_nCharType, kCharPos);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_06, kPacket_.m_nOK, 6); // 캐릭터 오픈 DB처리 실패.

	default:
		START_LOG(cerr, L"캐릭터배치 오픈 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"캐릭터배치 오픈 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kCharPos.m_nCharType)
		<< BUILD_LOG(kPacket_.m_mapCharacterPos.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_OPEN_AGIT_CHARACTER_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq)
{
	SET_ERROR(ERR_UNKNOWN);

	kPacket_.m_bCharPosOver = false;

	KSetAgitCharPosAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_vecCharPos = kPacket_.m_vecCharPos;

	std::vector<KSimpleAgitCharPos>::iterator vit;
	std::map<DWORD, KAgitMap>::iterator mit;

	std::vector<KSimpleAgitCharPos> vecCurrentCharPos;
	vecCurrentCharPos.clear();

	KAgitInfo kInfo;
	KAgitMapInfo kMapInfo;
	SiKAgitManager()->GetMapInfo(kMapInfo);

	USHORT usCharPosCount = 0;
	USHORT usMaxCharPosCount = 0;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_05, END_PROC);
	}

	spAgit->GetInfo(kInfo);
	mit = kMapInfo.find(kInfo.m_dwMapID);
	if (mit != kMapInfo.end()) {
		usMaxCharPosCount = mit->second.m_usMaxCharPos;
	}

	// 오픈되어있지 않은 캐릭터 타입
	for (vit = kPacket_.m_vecCharPos.begin(); vit != kPacket_.m_vecCharPos.end(); ++vit) {
		if (false == spAgit->IsCharacterPosOpen(vit->m_nCharType)) {
			START_LOG(cerr, L"오픈되어있지 않은 캐릭터 타입.. AgitUID : " << kPacket_.m_dwAgitUID << L", CharType : " << vit->m_nCharType) << END_LOG;
			SET_ERR_GOTO(ERR_AGIT_CHARACTER_13, END_PROC);
		}

		if (vit->m_nPosX > -1 && vit->m_nPosY > -1) {
			// 배치할 캐릭터 수 제한 체크
			if (usCharPosCount >= usMaxCharPosCount) {
				// 2012.11.07 nodefeat. 멘티스 338926 이슈로 수정
				// 최대 배치 캐릭터 수를 넘어서면 기존에 설치 된 캐릭터 정보로 ERR ACK
				vecCurrentCharPos.clear();
				for (std::map<int, KAgitCharacterPos>::iterator mitCharPos = kInfo.m_mapCharacterPos.begin(); mitCharPos != kInfo.m_mapCharacterPos.end(); mitCharPos++) {
					KSimpleAgitCharPos kCharPos;
					kCharPos.m_nCharType = mitCharPos->second.m_nCharType;
					kCharPos.m_nPosX = mitCharPos->second.m_nPosX;
					kCharPos.m_nPosY = mitCharPos->second.m_nPosY;
					vecCurrentCharPos.push_back(kCharPos);

					kPacket.m_vecCharPos = vecCurrentCharPos;
				}
				SET_ERR_GOTO(ERR_AGIT_CHARACTER_18, END_PROC);
				/*
				vit->m_nPosX = -1;
				vit->m_nPosY = -1;

				START_LOG( cwarn, L"배치제한수를 초과하는 캐릭터 배치좌표 초기화.. AgitUID : " << kPacket_.m_dwAgitUID << L", CharType : " << vit->m_nCharType ) << END_LOG;
				kPacket_.m_bCharPosOver = true;
				*/
			}
			else {
				++usCharPosCount;
			}
		}

		vecCurrentCharPos.push_back(*vit);
	}

	// 서버에서 체크한 배치정보로 저장하자.
	kPacket_.m_vecCharPos = vecCurrentCharPos;
	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_CHARACTER_POS_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_05, kPacket.m_nOK, 5); // 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_CHARACTER_13, kPacket.m_nOK, 13); // 오픈되어있지 않은 캐릭터 타입
		ERR_CASE(ERR_AGIT_CHARACTER_18, kPacket.m_nOK, 18); // 배치캐릭터 최대 배치제한수 초과.
	default:
		START_LOG(cerr, L"캐릭터배치 위치변경 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"캐릭터배치 위치변경 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecCharPos.size())
		<< BUILD_LOG(kPacket_.m_bCharPosOver)
		<< BUILD_LOG(kInfo.m_dwMapID)
		<< BUILD_LOG(usCharPosCount)
		<< BUILD_LOG(usMaxCharPosCount) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_CHARACTER_POS_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_POS_ACK, KSetAgitCharPosAck)
{
	KAgitPtr spAgit;
	KAgitCharacterPos kCharPos;

	std::vector<KSimpleAgitCharPos>::iterator vit;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK) && !IS_CORRECT(ERR_AGIT_CHARACTER_18)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		goto END_PROC;
	}

	// 배치캐릭터 수정적용
	for (vit = kPacket_.m_vecCharPos.begin(); vit != kPacket_.m_vecCharPos.end(); ++vit) {
		spAgit->GetCurrentCharacterPos(vit->m_nCharType, kCharPos);

		kCharPos.m_nPosX = vit->m_nPosX;
		kCharPos.m_nPosY = vit->m_nPosY;
		START_LOG(clog, L"배치캐릭터 좌표 수정.. AgitUID : " << kPacket_.m_dwAgitUID << L", PosX : " << kCharPos.m_nPosX << L", PosY : " << kCharPos.m_nPosY) << END_LOG;

		spAgit->UpdateCharacterPos(vit->m_nCharType, kCharPos);
	}

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_POS_BROAD, kPacket_.m_dwAgitUID, kPacket_);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_14, kPacket_.m_nOK, 14); // DB처리 실패.
		ERR_CASE(ERR_AGIT_CHARACTER_18, kPacket_.m_nOK, 18); // 배치캐릭터 최대 배치제한수 초과.

	default:
		START_LOG(cerr, L"아지트 캐릭터 배치 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	if (kPacket_.m_nOK == 18) {
		START_LOG(cwarn, L"아지트 캐릭터 배치시, 배치제한개수 초과됨. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(kPacket_.m_vecCharPos.size()) << END_LOG;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == 18, L"아지트 캐릭터 배치 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecCharPos.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_CHARACTER_POS_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KSetAgitCharMotionAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_nMotionID = kPacket_.m_nMotionID;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_05, END_PROC);
	}

	// 오픈되어있지 않은 캐릭터 타입
	if (false == spAgit->IsCharacterPosOpen(kPacket_.m_nCharType)) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_13, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_CHARACTER_MOTION_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_05, kPacket.m_nOK, 5); // 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_CHARACTER_13, kPacket.m_nOK, 13); // 오픈되어있지 않은 캐릭터 타입
	default:
		START_LOG(cerr, L"캐릭터 모션변경 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"캐릭터 모션변경 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_nMotionID) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_CHARACTER_MOTION_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_MOTION_ACK, KSetAgitCharMotionAck)
{
	KAgitPtr spAgit;
	KAgitCharacterPos kCharPos;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		goto END_PROC;
	}

	spAgit->GetCurrentCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kCharPos);
	kCharPos.m_nMotionID = kPacket_.m_kCharInfo.m_nMotionID;
	kPacket_.m_kCharInfo = kCharPos;

	// 배치캐릭터 수정적용
	spAgit->UpdateCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kPacket_.m_kCharInfo);

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kCharInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_15, kPacket_.m_nOK, 15); // DB처리 실패.

	default:
		START_LOG(cerr, L"캐릭터 모션변경 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"캐릭터 모션변경 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_nCharType)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_nMotionID) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_CHARACTER_MOTION_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_COORDI_REQ, KSetAgitCharCoordiReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KSetAgitCharCoordiAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_mapCoordi = kPacket_.m_mapSlotCoordiItemUID;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_05, END_PROC);
	}

	// 오픈되어있지 않은 캐릭터 타입
	if (false == spAgit->IsCharacterPosOpen(kPacket_.m_nCharType)) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_13, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_CHARACTER_COORDI_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_05, kPacket.m_nOK, 5); // 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_CHARACTER_13, kPacket.m_nOK, 13); // 오픈되어있지 않은 캐릭터 타입
	default:
		START_LOG(cerr, L"배치캐릭터 코디설정 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"배치캐릭터 코디설정 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_mapSlotCoordiItemUID.size())
		<< END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_CHARACTER_COORDI_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_COORDI_ACK, KSetAgitCharCoordiAck)
{
	KAgitPtr spAgit;
	KAgitCharacterPos kCharPos;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		goto END_PROC;
	}

	spAgit->GetCurrentCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kCharPos);
	kCharPos.m_mapCoordi.swap(kPacket_.m_kCharInfo.m_mapCoordi);
	kPacket_.m_kCharInfo = kCharPos;

	// 배치캐릭터 수정적용
	spAgit->UpdateCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kPacket_.m_kCharInfo);

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kCharInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_16, kPacket_.m_nOK, 16); // DB처리 실패.

	default:
		START_LOG(cerr, L"배치캐릭터 코디설정 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"배치캐릭터 코디설정 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_nCharType)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_mapCoordi.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_CHARACTER_COORDI_ACK);
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq)
{
	SET_ERROR(ERR_UNKNOWN);

	KSetAgitCharSpeechAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_mapSpeech = kPacket_.m_mapSpeech;

	KAgitPtr spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_05, END_PROC);
	}

	// 오픈되어있지 않은 캐릭터 타입
	if (false == spAgit->IsCharacterPosOpen(kPacket_.m_nCharType)) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_13, END_PROC);
	}

	QUEUEING_EVENT_TO_DB(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, kPacket_);
	SET_ERROR(NET_OK);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_05, kPacket.m_nOK, 5); // 아지트 정보가 없음.
		ERR_CASE(ERR_AGIT_CHARACTER_13, kPacket.m_nOK, 13); // 오픈되어있지 않은 캐릭터 타입
	default:
		START_LOG(cerr, L"배치캐릭터 대사설정 요청 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket.m_nOK == 0, L"배치캐릭터 대사설정 요청 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_mapSpeech.size()) << END_LOG;

	if (kPacket.m_nOK != 0) {
		SEND_PACKET(EAG_SET_AGIT_CHARACTER_SPEECH_ACK);
	}
}

_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_SPEECH_ACK, KSetAgitCharSpeechAck)
{
	KAgitPtr spAgit;
	KAgitCharacterPos kCharPos;

	NetError::SetLastNetError(kPacket_.m_nOK);
	if (!IS_CORRECT(NET_OK)) {
		goto END_PROC;
	}

	spAgit = SiKAgitManager()->GetAgit(kPacket_.m_dwAgitUID);
	if (spAgit == NULL) {
		goto END_PROC;
	}

	spAgit->GetCurrentCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kCharPos);
	kCharPos.m_mapSpeech.swap(kPacket_.m_kCharInfo.m_mapSpeech);
	kPacket_.m_kCharInfo = kCharPos;

	// 배치캐릭터 수정적용
	spAgit->UpdateCharacterPos(kPacket_.m_kCharInfo.m_nCharType, kPacket_.m_kCharInfo);

	// 아지트내 유저들에게 보낸다
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_BROAD, kPacket_.m_dwAgitUID, kPacket_.m_kCharInfo);

END_PROC:
	switch (NetError::GetLastNetError()) {
		ERR_CASE(NET_OK, kPacket_.m_nOK, 0); // 성공
		ERR_CASE(ERR_AGIT_CHARACTER_17, kPacket_.m_nOK, 17); // DB처리 실패.

	default:
		START_LOG(cerr, L"배치캐릭터 대사설정 중 알수 없는 오류 Name : " << GetName())
			<< BUILD_LOG(NetError::GetLastNetError())
			<< BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
		kPacket_.m_nOK = -99;
	}

	_LOG_SUCCESS(kPacket_.m_nOK == 0, L"배치캐릭터 대사설정 결과 : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(GetName())
		<< BUILD_LOG(GetUID())
		<< BUILD_LOG(kPacket_.m_nOK)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_nCharType)
		<< BUILD_LOG(kPacket_.m_kCharInfo.m_mapSpeech.size()) << END_LOG;

	SEND_RECEIVED_PACKET(EAG_SET_AGIT_CHARACTER_SPEECH_ACK);
}