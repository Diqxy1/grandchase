#include "Agent.h"
#include "Agit.h"
#include "AgitManager.h"
#include "AGDBLayer.h"
#include "AGSimLayer.h"
#include <boost/bind/bind.hpp>

NiImplementRTTI(KAgit, KPerformer);

#define EVENT_TYPE KAgentEvent
#define CLASS_TYPE KAgit

//FILE_NAME_FOR_LOG

ImplToStringW(KAgit)
{
	return START_TOSTRING_PARENTW(KPerformer);
}

KAgit::KAgit(void)
	:m_usRoomID(0)
	, m_bPageRegister(false)
	, m_kConnectCheckTimer(1000 * 60 * 5)
	, m_tmPacketRecvTime(0)
{
	for (int i = 0; i < 6; ++i) {
		m_mapUserSlot[i] = 0;
	}
}

KAgit::~KAgit(void)
{
}

void KAgit::Tick()
{
	KPerformer::Tick();

	if (m_kConnectCheckTimer.CheckTime()) {
		// 방원들 접속상태 체크
		KUserConnectStatus kPacket;
		kPacket.m_dwAgitUID = GetUID();
		GetUserUID(kPacket.m_mapUserConnect);
		if (false == kPacket.m_mapUserConnect.empty()) {
			SiKAGConnector()->SendPacket(KAgentEvent::EAGS_USER_CONNECT_STATUS_REQ, kPacket);
		}
	}
}

void KAgit::ProcessEvent(const KEventPtr& spEvent_)
{
	KSerializer ks;
	ks.BeginReading(const_cast<KSerBuffer*>(&spEvent_->m_kbuff));
	switch (spEvent_->m_usEventID) {
		_CASE(EAG_ENTER_AGIT_BROAD, KAgitUserInfo);
		_CASE(EAG_LEAVE_AGIT_BROAD, DWORD);
		_CASE(EAG_CHANGE_AGIT_MAP_BROAD, DWORD);
		_CASE(EAG_CHANGE_FLOWER_POT_STATE_BROAD, KFlowerPot);
		_CASE(EAG_AGIT_CHAT_BROAD, KChatData);
		_CASE(EAG_CHANGE_AGIT_OBJECT_POS_BROAD, VEC_OBJECT_POS);
		_CASE(EAG_CHANGE_BROWNIE_BROAD, KBrownieInfo);
		_CASE(EAG_CHANGE_FAIRY_TREE_BROAD, KFairyTree);
		_CASE(EAG_UPDATE_FAIRY_TREE_REQ, time_t);
		_CASE(EAG_UPDATE_FAIRY_TREE_ACK, KEAG_UPDATE_FAIRY_TREE_REQ);
		_CASE(EAG_UPDATE_AGIT_VISITER_COUNT_NOT, time_t);
		_CASE(EAG_CHANGE_AGIT_OPTION_BROAD, KAgitOption);
		_CASE(EAG_GUESTBOOK_WRITE_BROAD, KGuestBookWriteBroad);
		_CASE(EAG_GUESTBOOK_DELETE_BROAD, KGuestBookDeleteReq);
		_CASE(EAG_UPDATE_TRAINING_OBJ_BROAD, KTrainingObj);
		_CASE(EAG_CHECK_TRAINING_OBJ_NOT, GCITEMID);
		_CASE(EAG_UPDATE_AGIT_CHARACTER_BROAD, KAgitCharacterPos);
		_CASE(EAG_UPDATE_AGIT_CHARACTER_POS_BROAD, KSetAgitCharPosAck);
	default:
		START_LOG(cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString()) << END_LOG;
	}
}

void KAgit::SetInfo(IN const KAgitInfo& kInfo_)
{
	KLocker lock(m_csInfo);
	m_kInfo = kInfo_;
	SetUID(kInfo_.m_dwUID);
	m_tmVisiterCountDate = CTime::GetCurrentTime();
}

void KAgit::GetInfo(OUT KAgitInfo& kInfo_)
{
	KLocker lock(m_csInfo);
	kInfo_ = m_kInfo;
}

void KAgit::IncVisiter()
{
	KLocker lock(m_csInfo);
	++m_kInfo.m_nTodayVisiter;
	++m_kInfo.m_nTotalVisiter;
	START_LOG(clog, L"방문자 수 증가 Today : " << m_kInfo.m_nTodayVisiter << L", Total : " << m_kInfo.m_nTotalVisiter) << END_LOG;
}

void KAgit::GetActiveObjectPos(IN const GCITEMID& ItemID_, OUT std::vector<KAgitObjectPos>& vecInfo_)
{
	KLocker lock(m_csInfo);

	vecInfo_.clear();

	std::vector<KAgitObjectPos>::iterator vit;
	for (vit = m_kInfo.m_vecObjectPos.begin(); vit != m_kInfo.m_vecObjectPos.end(); ++vit) {
		if (vit->m_ID != ItemID_) {
			continue;
		}

		if (vit->m_nPosX == -1 && vit->m_nPosY == -1) {
			continue;
		}

		// 설치된 오브젝트만 구하기
		vecInfo_.push_back(*vit);
	}
}

void KAgit::SetObjectPos(IN const std::vector<KAgitObjectPos>& vecInfo_)
{
	KLocker lock(m_csInfo);

	std::vector<KAgitObjectPos>::const_iterator vitNew;
	for (vitNew = vecInfo_.begin(); vitNew != vecInfo_.end(); ++vitNew) {
		// 오브젝트 정보 갱신
		std::vector<KAgitObjectPos>::iterator vit;
		vit = std::find(m_kInfo.m_vecObjectPos.begin(), m_kInfo.m_vecObjectPos.end(), *vitNew);
		if (vit != m_kInfo.m_vecObjectPos.end()) {
			*vit = *vitNew;
		}
		else {
			m_kInfo.m_vecObjectPos.push_back(*vitNew);
		}
	}
}

bool KAgit::AddFavorites(IN const KAgitPageInfo& kInfo_)
{
	KLocker lock(m_csFavorites);
	return (m_mapFavorites.insert(std::map<DWORD, KAgitPageInfo>::value_type(kInfo_.m_dwUID, kInfo_)).second);
}

void KAgit::RemoveFavorites(IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csFavorites);
	m_mapFavorites.erase(dwAgitUID_);
}

void KAgit::SetTRServerIPPort(IN const DWORD& dwIP_, IN const USHORT& usPort_)
{
	KLocker lock(m_csRelayServer);
	m_prTRServerIPPort.first = dwIP_;
	m_prTRServerIPPort.second = usPort_;
}

void KAgit::GetTRServerIPPort(OUT std::pair<DWORD, USHORT>& prIPPort_)
{
	KLocker lock(m_csRelayServer);
	prIPPort_ = m_prTRServerIPPort;
}

void KAgit::SetURServerIPPort(IN const DWORD& dwIP_, IN const USHORT& usPort_)
{
	KLocker lock(m_csRelayServer);
	m_prURServerIPPort.first = dwIP_;
	m_prURServerIPPort.second = usPort_;
}

void KAgit::GetURServerIPPort(OUT std::pair<DWORD, USHORT>& prIPPort_)
{
	KLocker lock(m_csRelayServer);
	prIPPort_ = m_prURServerIPPort;
}

size_t KAgit::GetUserNum()
{
	KLocker lock(m_csUser);
	return m_mapUserInfo.size();
}

bool KAgit::IsFull()
{
	size_t UserCount = GetUserNum();

	KLocker lock(m_csInfo);
	return (UserCount >= m_kInfo.m_ucMaxUser);
}

bool KAgit::IsExistUser(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csUser);
	return (m_mapUserInfo.find(dwUserUID_) != m_mapUserInfo.end());
}

void KAgit::SetUserInfo(IN const KAgitUserInfo& kUserInfo_)
{
	START_LOG(clog, L"아지트 입장유저.. UserUId : " << kUserInfo_.m_dwUID)
		<< BUILD_LOG(kUserInfo_.m_strNick)
		<< BUILD_LOGc(kUserInfo_.m_cCharType)
		<< BUILD_LOGc(kUserInfo_.m_cPromotion)
		<< BUILD_LOG(kUserInfo_.m_dwLevel) << END_LOG;

	KLocker lock(m_csUser);
	m_mapUserInfo[kUserInfo_.m_dwUID] = kUserInfo_;
	m_mapUserPlayTime[kUserInfo_.m_dwUID] = ::GetTickCount();
}

bool KAgit::GetUserInfo(IN const DWORD& dwUserUID_, OUT KAgitUserInfo& kUserInfo_)
{
	KLocker lock(m_csUser);
	std::map<DWORD, KAgitUserInfo>::const_iterator cmit;
	cmit = m_mapUserInfo.find(dwUserUID_);
	if (cmit == m_mapUserInfo.end()) {
		return false;
	}

	kUserInfo_ = cmit->second;
	return true;
}

void KAgit::GetUserList(OUT std::map<DWORD, KAgitUserInfo>& mapUser_)
{
	KLocker lock(m_csUser);
	mapUser_ = m_mapUserInfo;
}

void KAgit::SetUserList(IN const std::map<DWORD, KAgitUserInfo>& mapUser_)
{
	KLocker lock(m_csUser);
	m_mapUserInfo = mapUser_;
	START_LOG(clog, L"아지트 유저 설정.. size : " << mapUser_.size()) << END_LOG;
}

void KAgit::DeleteUserInfo(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csUser);
	m_mapUserInfo.erase(dwUserUID_);

	DWORD dwPlayTime = 1;

	std::map<DWORD, DWORD>::iterator mit;
	mit = m_mapUserPlayTime.find(dwUserUID_);
	if (mit != m_mapUserPlayTime.end()) {
		DWORD dwCurrentTick = ::GetTickCount();
		DWORD dwEnterTick = mit->second;
		dwEnterTick = std::min<DWORD>(dwEnterTick, dwCurrentTick - 1);
		DWORD dwPlaySec = static_cast<DWORD>((dwCurrentTick - dwEnterTick) / 1000);

		if (dwPlaySec > 0) {
			KEAG_ENTER_AGIT_STAT_NOT kDBPacket;
			kDBPacket.m_dwAgitUID = GetUID();
			kDBPacket.m_dwUserUID = dwUserUID_;
			kDBPacket.m_dwPlayTime = dwPlaySec;
			SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_ENTER_AGIT_STAT_NOT, L"", kDBPacket.m_dwAgitUID, -1, kDBPacket);
		}

		m_mapUserPlayTime.erase(mit);

		START_LOG(clog, L"아지트 플레이시간 기록.. AgitUID : " << GetUID())
			<< BUILD_LOG(dwUserUID_)
			<< BUILD_LOG(dwPlaySec) << END_LOG;
	}

	std::map<USHORT, DWORD>::iterator mitSlot;
	for (mitSlot = m_mapUserSlot.begin(); mitSlot != m_mapUserSlot.end(); ++mitSlot) {
		if (mitSlot->second == dwUserUID_) {
			mitSlot->second = 0;
			START_LOG(clog, L"유저 슬롯 초기화.. SlotID : " << mitSlot->first << L", AgitUID : " << GetUID()) << END_LOG;
		}
	}
}

void KAgit::SetUserServer(IN const DWORD& dwUserUID_, IN const DWORD& dwServerUID_, IN KAgentPtr& spAgent_)
{
	RemoveUserServer(dwUserUID_);

	KLocker lock(m_csServer);
	m_mapServer[dwServerUID_] = spAgent_;
	m_mapServerUser[dwServerUID_].push_back(dwUserUID_);
}

void KAgit::GetUserServerList(OUT std::map<DWORD, std::vector<DWORD> >& mapUserServer_)
{
	mapUserServer_.clear();

	KLocker lock(m_csServer);
	mapUserServer_ = m_mapServerUser;
}

void KAgit::RemoveUserServer(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csServer);

	std::map< DWORD, std::vector<DWORD> >::iterator mit;
	for (mit = m_mapServerUser.begin(); mit != m_mapServerUser.end(); ++mit) {
		std::vector<DWORD>::iterator vit;
		vit = std::find(mit->second.begin(), mit->second.end(), dwUserUID_);
		if (vit == mit->second.end()) {
			continue;
		}

		mit->second.erase(vit);
	}
}

void KAgit::SetOption(IN const KAgitOption& kOption_)
{
	KLocker lock(m_csInfo);
	m_kInfo.SetOption(kOption_);
}

void KAgit::SetFlowerPot(IN std::map<GCITEMUID, KFlowerPot>& mapList_)
{
	KLocker lock(m_csFlowerPot);
	m_mapFlowerPot.swap(mapList_);
}

void KAgit::GetFlowerPot(OUT std::map<GCITEMUID, KFlowerPot>& mapList_)
{
	KLocker lock(m_csFlowerPot);
	mapList_ = m_mapFlowerPot;
}

USHORT KAgit::GetActiveCashPotCount()
{
	KLocker lock(m_csFlowerPot);

	USHORT usCount = 0;

	std::map<GCITEMUID, KFlowerPot>::const_iterator cmit;
	for (cmit = m_mapFlowerPot.begin(); cmit != m_mapFlowerPot.end(); ++cmit) {
		if (cmit->second.m_nSeedType == KFlowerPot::ST_NONE) {
			continue;
		}

		if (false == SiKAgitManager()->IsCashPotItemID(cmit->second.m_ItemID)) {
			continue;
		}

		++usCount;
	}

	return usCount;
}

USHORT KAgit::GetActiveNormalPotCount()
{
	KLocker lock(m_csFlowerPot);

	USHORT usCount = 0;

	std::map<GCITEMUID, KFlowerPot>::const_iterator cmit;
	for (cmit = m_mapFlowerPot.begin(); cmit != m_mapFlowerPot.end(); ++cmit) {
		if (cmit->second.m_nSeedType == KFlowerPot::ST_NONE) {
			continue;
		}

		if (false == SiKAgitManager()->IsNormalPotItemID(cmit->second.m_ItemID)) {
			continue;
		}

		++usCount;
	}

	return usCount;
}

void KAgit::UpdateFlowerPot(IN const KFlowerPot& kPotInfo_)
{
	KLocker lock(m_csFlowerPot);
	m_mapFlowerPot[kPotInfo_.m_UID] = kPotInfo_;
}

bool KAgit::Enter(IN OUT KAgitUserInfo& kUserInfo_, IN KAgentPtr& spAgent_)
{
	KAgitInfo kInfo;
	GetInfo(kInfo);

	// 이미 입장한 유저면 정보만 갱신.
	if (false == IsExistUser(kUserInfo_.m_dwUID)) {
		// 주인은 인원체크 안함
		if (kUserInfo_.m_dwUID != kInfo.m_dwUID) {
			// 인원이 꽉 찼음.
			if (IsFull()) {
				return false;
			}
		}
	}

	// 슬롯 번호 지정
	kUserInfo_.m_nPlayerIndex = GetUserSlot(kUserInfo_.m_dwUID);

	// 유저정보 등록
	SetUserServer(kUserInfo_.m_dwUID, kUserInfo_.m_nServerUID, spAgent_);
	SetUserInfo(kUserInfo_);

	// 방문자수 갱신
	IncVisiter();
	SendAgitVisiterCountBroad();
	return true;
}

void KAgit::Leave(IN const DWORD& dwUserUID_)
{
	RemoveUserServer(dwUserUID_);
	DeleteUserInfo(dwUserUID_);

	// 아지트 가용상태 체크
	if (SiKAgitManager()->GetCashingSize() < SiKAgitManager()->GetMaxCashingAgitInfo()) {
		return;
	}

	// 인기아지트 체크
	if (SiKAgitManager()->IsRankAgit(GetUID())) {
		return;
	}

	if (ClearCheck()) {
		if (IsPageRegister()) {
			DWORD dwAgitUID = GetUID();
			SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_UNREGISTER_AGIT_PAGE_REQ, L"", dwAgitUID, -1, dwAgitUID);
			START_LOG(clog, L"페이지 등록 해제 요청 AgitUID : " << dwAgitUID) << END_LOG;
		}

		SetReserveDestroy(true);
		START_LOG(clog, L"아지트정보 삭제예약 AgitUID : " << GetUID()) << END_LOG;
	}
}

void KAgit::GetPageInfo(OUT KAgitPageInfo& kPageInfo_)
{
	// 페이지 정보
	kPageInfo_.m_usRoomID = GetRoomID();

	{
		KLocker lock(m_csInfo);
		kPageInfo_.m_dwUID = m_kInfo.m_dwUID;
		kPageInfo_.m_strName = m_kInfo.m_strName;
		kPageInfo_.m_ucMaxUser = m_kInfo.m_ucMaxUser;
		kPageInfo_.m_ucOpenType = m_kInfo.m_ucOpenType;
		kPageInfo_.m_strOwnerNick = m_strOwnerNickname;
	}

	{
		KLocker lock(m_csUser);
		std::map<DWORD, KAgitUserInfo>::iterator mit;
		for (mit = m_mapUserInfo.begin(); mit != m_mapUserInfo.end(); ++mit) {
			kPageInfo_.m_mapUser[mit->second.m_dwUID] = mit->second.m_strNick;
		}
	}
}

void KAgit::UpdateCashingData()
{
	// 동기화
	{
		KLocker lock(m_csInfo);

		if (m_kInfo.m_nTodayVisiter > m_kInfo.m_nInitTodayVisiter) {
			KEAG_UPDATE_AGIT_VISITER_COUNT_NOT kPacket;
			kPacket.m_dwAgitUID = GetUID();
			kPacket.m_nTodayVisiterDiff = m_kInfo.m_nTodayVisiter - m_kInfo.m_nInitTodayVisiter;
			SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_VISITER_COUNT_NOT, L"", kPacket.m_dwAgitUID, -1, kPacket);
			// 방문자수 동기화
			m_kInfo.m_nInitTodayVisiter = m_kInfo.m_nTodayVisiter;
		}
	}

	{
		KLocker lock(m_csTrainingObj);

		KEAG_UPDATE_TRAINING_OBJ_REQ kPacket;
		kPacket.m_dwAgitUID = GetUID();
		kPacket.m_mapTrainingObj = m_mapTrainingObj;
		SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_UPDATE_TRAINING_OBJ_REQ, L"", kPacket.m_dwAgitUID, -1, kPacket);
	}
}

void KAgit::SendAgitVisiterCountBroad()
{
	KEAG_AGIT_VISITER_COUNT_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_nTodayVisiterCount = m_kInfo.m_nTodayVisiter;
	kPacket.m_nTotalVisiterCount = m_kInfo.m_nTotalVisiter;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_AGIT_VISITER_COUNT_BROAD, kPacket);
}

void KAgit::SetOwnerConnect(IN const bool& bVal_, IN const time_t& tmRecvTime_)
{
	KLocker lock(m_csUser);

	if (tmRecvTime_ < m_tmPacketRecvTime) {
		START_LOG(cwarn, L"전달받은 패킷이 마지막에 받은 패킷보다 이전 패킷.. AgitUID : " << GetUID())
			<< BUILD_LOGtm(CTime(tmRecvTime_))
			<< BUILD_LOGtm(CTime(m_tmPacketRecvTime)) << END_LOG;
		return;
	}

	m_bOwnerConnect = bVal_;
	m_tmPacketRecvTime = tmRecvTime_;

	START_LOG(clog, L"아지트 주인 접속설정.. AgitUID : " << GetUID())
		<< BUILD_LOG(bVal_)
		<< BUILD_LOGtm(CTime(tmRecvTime_)) << END_LOG;
}

bool KAgit::ClearCheck()
{
	if (IsOwnerConnect()) {
		return false;
	}

	if (GetUserNum() > 0) {
		return false;
	}

	return true;
}

void KAgit::SetGuestbook(IN const std::map<int, KGuestMsg>& mapMsg_)
{
	KLocker lock(m_csGuestbook);
	m_mapGuestbook = mapMsg_;
}

void KAgit::GetGuestbook(OUT std::map<int, KGuestMsg>& mapMsg_)
{
	KLocker lock(m_csGuestbook);
	mapMsg_ = m_mapGuestbook;
}

void KAgit::AddGuestMsg(IN const int& nNo, IN const KGuestMsg& kMsg_)
{
	KLocker lock(m_csGuestbook);
	m_mapGuestbook[nNo] = kMsg_;
}

void KAgit::DeleteGuestMsg(IN const DWORD& dwUserUID_, IN OUT std::set<int>& setNo_)
{
	KLocker lock(m_csGuestbook);

	std::set<int>::const_iterator sit;
	std::set<int> setDeleteNo;
	setDeleteNo.clear();

	if (dwUserUID_ == GetUID()) {
		for (sit = setNo_.begin(); sit != setNo_.end(); ++sit) {
			m_mapGuestbook.erase(*sit);
		}
		START_LOG(clog, L"주인이 낙서장 글 삭제요청.. Delete No size : " << setNo_.size()) << END_LOG;
	}
	else {
		for (sit = setNo_.begin(); sit != setNo_.end(); ++sit) {
			std::map<int, KGuestMsg>::iterator mit;
			mit = m_mapGuestbook.find(*sit);
			if (mit == m_mapGuestbook.end()) {
				START_LOG(cwarn, L"삭제하려는 글이 존재하지 않음.. 글번호 : " << *sit) << END_LOG;
				continue;
			}

			if (dwUserUID_ != mit->second.m_dwUserUID) {
				START_LOG(cwarn, L"주인이 아닌데, 다른사람의 글을 삭제 요청.. 글번호 : " << *sit << L", 삭제요청UserUID : " << dwUserUID_ << L",글쓴UserUID : " << mit->second.m_dwUserUID) << END_LOG;
				continue;
			}

			m_mapGuestbook.erase(mit);
			setDeleteNo.insert(*sit);
		}
		START_LOG(clog, L"손님이 낙서장 글 삭제요청.. Delete No size : " << setNo_.size() << L", Current size : " << setDeleteNo.size()) << END_LOG;
		setNo_.swap(setDeleteNo);
	}

	START_LOG(clog, L"낙서장 글 삭제.. Delete No size : " << setNo_.size() << L", Guestbook size : " << m_mapGuestbook.size()) << END_LOG;
}

void KAgit::SetTrainingObj(IN const std::map<GCITEMUID, KTrainingObj>& mapList_)
{
	KLocker lock(m_csTrainingObj);
	m_mapTrainingObj = mapList_;
}

void KAgit::GetTrainingObjUpdate(OUT std::map<GCITEMUID, KTrainingObj>& mapList_)
{
	GCITEMID ItemID = SiKAgitManager()->TrainingItemID();
	USHORT usTrainingMaterialLimit = SiKAgitManager()->GetTrainingMaterialLimit();
	USHORT usTrainingMaterialRegenTerm = SiKAgitManager()->GetTrainingMaterialRegenTerm();
	usTrainingMaterialRegenTerm = std::max<USHORT>(usTrainingMaterialRegenTerm, 1);
	CTime tmCurrent(CTime::GetCurrentTime());

	std::vector<KAgitObjectPos> vecObjPos;
	GetActiveObjectPos(ItemID, vecObjPos);

	KLocker lock(m_csTrainingObj);

	std::map<GCITEMUID, KTrainingObj>::iterator mit;
	for (mit = m_mapTrainingObj.begin(); mit != m_mapTrainingObj.end(); ++mit) {
		START_LOG(clog, L"TrainingObj ItemUID : " << mit->second.m_ItemUID << L", AccCount : " << mit->second.m_nAccCount) << END_LOG;

		std::vector<KAgitObjectPos>::iterator vit;
		vit = std::find_if(vecObjPos.begin(), vecObjPos.end(),
			boost::bind(&KAgitObjectPos::m_UID, boost::placeholders::_1) == mit->first);

		if (vit == vecObjPos.end()) {
			START_LOG(clog, L"설치안된 훈련소 오브젝트.. AgitUID : " << GetUID() << L", ItemUID : " << mit->first) << END_LOG;
			continue;
		}

		if (mit->second.m_nAccCount >= usTrainingMaterialLimit) {
			START_LOG(clog, L"이미 제한개수만큼 가지고 있음.. AgitUID : " << GetUID())
				<< BUILD_LOG(mit->second.m_nAccCount)
				<< BUILD_LOG(usTrainingMaterialLimit) << END_LOG;
			continue;
		}

		CTime tmLastUpdate(mit->second.m_tmDate);
		CTimeSpan tsTimeSpan = tmCurrent - tmLastUpdate;
		if (tsTimeSpan.GetTotalMinutes() < 1) {
			START_LOG(clog, L"갱신체크한지 1분이 지나지 않음.. AgitUID : " << GetUID()) << END_LOG;
			continue;
		}

		// 쌓인 횟수
		USHORT usCurrentCount = (USHORT)(tsTimeSpan.GetTotalMinutes() / usTrainingMaterialRegenTerm);
		// 장작 쌓이는 시간이 안됨
		if (usCurrentCount == 0) {
			START_LOG(clog, L"장작 쌓이는 시간이 안됨.. AgitUID : " << GetUID())
				<< BUILD_LOG(tsTimeSpan.GetTotalMinutes())
				<< BUILD_LOG(usTrainingMaterialRegenTerm)
				<< BUILD_LOG(usCurrentCount) << END_LOG;
			continue;
		}

		// 시간갱신, 장작 추가
		mit->second.m_tmDate = KncUtil::TimeToInt(tmCurrent);
		mit->second.m_nAccCount += usCurrentCount;
		mit->second.m_nAccCount = std::max<int>(mit->second.m_nAccCount, 0);
		mit->second.m_nAccCount = std::min<int>(mit->second.m_nAccCount, usTrainingMaterialLimit);

		START_LOG(clog, L"훈련소 상태갱신. AgitUID : " << GetUID())
			<< BUILD_LOG(mit->second.m_ItemUID)
			<< BUILD_LOG(mit->second.m_nAccCount)
			<< BUILD_LOG(usCurrentCount)
			<< BUILD_LOG(usTrainingMaterialRegenTerm)
			<< BUILD_LOG(tsTimeSpan.GetTotalMinutes())
			<< BUILD_LOGtm(tmLastUpdate)
			<< BUILD_LOGtm(tmCurrent) << END_LOG;
	}

	mapList_ = m_mapTrainingObj;
}

void KAgit::GetTrainingObj(OUT std::map<GCITEMUID, KTrainingObj>& mapList_)
{
	KLocker lock(m_csTrainingObj);
	mapList_ = m_mapTrainingObj;
}

bool KAgit::GetCurrentTrainingObj(IN GCITEMUID& ItemUID_, OUT KTrainingObj& kObject_)
{
	KLocker lock(m_csTrainingObj);

	std::map<GCITEMUID, KTrainingObj>::iterator mit;
	mit = m_mapTrainingObj.find(ItemUID_);
	if (mit == m_mapTrainingObj.end()) {
		return false;
	}

	kObject_ = mit->second;
	return true;
}

void KAgit::UpdateTrainingObj(IN const KTrainingObj& kObject_)
{
	KLocker lock(m_csTrainingObj);
	m_mapTrainingObj[kObject_.m_ItemUID] = kObject_;
}

USHORT KAgit::GetUserSlot(IN const DWORD& dwUserUID_)
{
	KLocker lock(m_csUser);
	std::map<USHORT, DWORD>::iterator mit;

	// 찾기
	for (mit = m_mapUserSlot.begin(); mit != m_mapUserSlot.end(); ++mit) {
		if (mit->second == dwUserUID_) {
			return mit->first;
		}
	}

	// 할당
	for (mit = m_mapUserSlot.begin(); mit != m_mapUserSlot.end(); ++mit) {
		if (mit->second == 0) {
			mit->second = dwUserUID_;
			return mit->first;
		}
	}

	// 추가
	USHORT usIndex = m_mapUserSlot.size();
	m_mapUserSlot[usIndex] = dwUserUID_;
	return usIndex;
}

void KAgit::GetCharacterPos(OUT std::map<int, KAgitCharacterPos>& mapList_)
{
	KLocker lock(m_csInfo);
	mapList_ = m_kInfo.m_mapCharacterPos;
}

bool KAgit::GetCurrentCharacterPos(IN const int& nCharType_, OUT KAgitCharacterPos& kInfo_)
{
	KLocker lock(m_csInfo);

	std::map<int, KAgitCharacterPos>::iterator mit;
	mit = m_kInfo.m_mapCharacterPos.find(nCharType_);
	if (mit == m_kInfo.m_mapCharacterPos.end()) {
		return false;
	}

	kInfo_ = mit->second;
	return true;
}

void KAgit::UpdateCharacterPos(IN const int& nCharType_, IN const KAgitCharacterPos& kInfo_)
{
	KLocker lock(m_csInfo);
	m_kInfo.m_mapCharacterPos[nCharType_] = kInfo_;
}

bool KAgit::IsCharacterPosOpen(IN const int& nCharType_)
{
	KLocker lock(m_csInfo);
	return (m_kInfo.m_mapCharacterPos.find(nCharType_) != m_kInfo.m_mapCharacterPos.end());
}

void KAgit::GetUserUID(OUT std::map<DWORD, bool>& mapList_)
{
	mapList_.clear();

	KLocker lock(m_csUser);
	std::map<DWORD, DWORD>::iterator mit;
	for (mit = m_mapUserPlayTime.begin(); mit != m_mapUserPlayTime.end(); ++mit) {
		mapList_[mit->first] = true;
	}
}

void KAgit::UpdateUserConnectStatus(IN const std::map<DWORD, bool>& mapList_)
{
	std::map<DWORD, bool>::const_iterator mit;
	for (mit = mapList_.begin(); mit != mapList_.end(); ++mit) {
		if (false == mit->second) {
			DWORD dwUserUID = mit->first;
			Leave(dwUserUID);
			ON_EAG_LEAVE_AGIT_BROAD(dwUserUID, 1);
		}
	}
}

void KAgit::UpdateTotalCharacterInfo(IN const std::map<int, KAgitCharacterPos>& mapCharacterPos_)
{
	KLocker lock(m_csInfo);
	m_kInfo.m_mapCharacterPos = mapCharacterPos_;
}

/*-------------------------------------------------------------------------------------------------------------------------------------*/

_IMPL_ON_FUNC(EAG_ENTER_AGIT_BROAD, KAgitUserInfo)
{
	KEAG_ENTER_AGIT_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kUserInfo = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_ENTER_AGIT_BROAD, kPacket);

	START_LOG(clog, L"아지트 유저 입장 알림. AgitUID : " << GetUID() << L", UserUID : " << kPacket_.m_dwUID)
		<< BUILD_LOG(kPacket_.m_strNick)
		<< BUILD_LOGc(kPacket_.m_cCharType)
		<< BUILD_LOGc(kPacket_.m_cPromotion) << END_LOG;
}

_IMPL_ON_FUNC(EAG_LEAVE_AGIT_BROAD, DWORD)
{
	KEAG_LEAVE_AGIT_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_dwUserUID = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_LEAVE_AGIT_BROAD, kPacket);
	START_LOG(clog, L"아지트 유저 퇴장 알림. AgitUID : " << GetUID() << L", UserUID : " << kPacket_) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_AGIT_MAP_BROAD, DWORD)
{
	KEAG_CHANGE_AGIT_MAP_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_dwMapID = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_AGIT_MAP_BROAD, kPacket);
	START_LOG(clog, L"아지트 맵변경 알림. AgitUID : " << GetUID() << L", MapID : " << kPacket_) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_FLOWER_POT_STATE_BROAD, KFlowerPot)
{
	KEAG_CHANGE_FLOWER_POT_STATE_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kFlowerPotInfo = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_FLOWER_POT_STATE_BROAD, kPacket);
	START_LOG(clog, L"화분 상태변경 알림. AgitUID : " << GetUID() << L", PotUID : " << kPacket_.m_UID << L", SeedType : " << kPacket_.m_nSeedType) << END_LOG;
}

_IMPL_ON_FUNC(EAG_AGIT_CHAT_BROAD, KChatData)
{
	KEAG_AGIT_CHAT_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kChatData = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_AGIT_CHAT_BROAD, kPacket);
	START_LOG(clog, L"아지트채팅 알림. AgitUID : " << GetUID() << L", Msg : " << kPacket_.m_strChatMsg) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_AGIT_OBJECT_POS_BROAD, VEC_OBJECT_POS)
{
	KEAG_CHANGE_AGIT_OBJECT_POS_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_vecObject = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_AGIT_OBJECT_POS_BROAD, kPacket);
	START_LOG(clog, L"아지트 오브젝트변경 알림. AgitUID : " << GetUID() << L", Object size : " << kPacket.m_vecObject.size()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_BROWNIE_BROAD, KBrownieInfo)
{
	KEAG_CHANGE_BROWNIE_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kBrownieInfo = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_BROWNIE_BROAD, kPacket);
	START_LOG(clog, L"아지트 도우미변경 알림. AgitUID : " << GetUID() << L", BrownieUID : " << kPacket.m_kBrownieInfo.m_dwUID) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_FAIRY_TREE_BROAD, KFairyTree)
{
	KEAG_CHANGE_FAIRY_TREE_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kFairyTree = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_FAIRY_TREE_BROAD, kPacket);
	START_LOG(clog, L"아지트 요정의나무 변경 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_REQ, time_t)
{
	KFairyTree kFairyTree;
	GetFairyTree(kFairyTree);

	// 성장치 감소 체크
	SiKAgitManager()->FairyTreeGrowChange(kPacket_, kFairyTree);

	// 날짜가 바뀌었으니, 열매 갱신
	kFairyTree.m_nFruitType = SiKAgitManager()->GetTodayFruitType(kFairyTree.m_nLv);
	kFairyTree.m_bUseFruit = false;
	kFairyTree.m_tmUpdateFruit = KncUtil::TimeToInt(CTime::GetCurrentTime());

	KEAG_UPDATE_FAIRY_TREE_REQ kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kFairyTree = kFairyTree;
	SiKAGDBLayer()->QueueingEvent(KAgentEvent::EAG_UPDATE_FAIRY_TREE_REQ, L"", kPacket.m_dwAgitUID, -1, kPacket);

	START_LOG(clog, L"아지트 요정의나무 상태 갱신. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_ACK, KEAG_UPDATE_FAIRY_TREE_REQ)
{
	// 요정의 나무 정보 갱신
	SetFairyTree(kPacket_.m_kFairyTree);
	// 안에 있는 유저들에게 알림.
	ON_EAG_CHANGE_FAIRY_TREE_BROAD(kPacket_.m_kFairyTree, 1);

	// 주인이 접속중이면,
	KAgitUserInfo kUserInfo;
	if (GetUserInfo(GetUID(), kUserInfo)) {
		// 버프 초기화 알림
		KFairyTreeFruitBuff kPacket;
		kPacket.m_dwUserUID = GetUID();
		kPacket.m_nTreeLv = kPacket_.m_kFairyTree.m_nLv;
		kPacket.m_nFruitType = KFairyTreeConfig::FBT_NONE;
		kPacket.m_fBuffRate = 0.f;
		kPacket.m_tmBuffTime = kPacket_.m_kFairyTree.m_tmUpdateFruit;

		SendTo(KAgentEvent::EAG_FAIRY_TREE_BUFF_NOT, kPacket, kPacket.m_dwUserUID, kUserInfo.m_nServerUID);
		START_LOG(clog, L"요정의나무 열매버프 초기화 전달.. AgitUID : " << kPacket.m_dwUserUID) << END_LOG;
	}

	START_LOG(clog, L"요정의나무 정보 갱신.. AgitUID : " << GetUID())
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_nFruitType)
		<< BUILD_LOGtm(CTime(kPacket_.m_kFairyTree.m_tmUpdateFruit))
		<< BUILD_LOG(kPacket_.m_kFairyTree.m_bUseFruit) << END_LOG;
}

_IMPL_ON_FUNC(EAG_UPDATE_AGIT_VISITER_COUNT_NOT, time_t)
{
	UpdateCashingData();

	CTime tmCurrent = CTime(kPacket_);

	KLocker lock(m_csInfo);

	if (m_tmVisiterCountDate.GetYear() < tmCurrent.GetYear() ||
		(m_tmVisiterCountDate.GetYear() == tmCurrent.GetYear() && m_tmVisiterCountDate.GetMonth() < tmCurrent.GetMonth()) ||
		(m_tmVisiterCountDate.GetYear() == tmCurrent.GetYear() && m_tmVisiterCountDate.GetMonth() == tmCurrent.GetMonth() && m_tmVisiterCountDate.GetDay() < tmCurrent.GetDay())) {
		// 날짜 바뀌면 일일 반문자 수 초기화
		m_kInfo.m_nInitTodayVisiter = 0;
		m_kInfo.m_nTodayVisiter = 0;
		m_tmVisiterCountDate = tmCurrent;
	}

	// 접속중인 아지트 유저들에게 알림.
	SendAgitVisiterCountBroad();
	START_LOG(clog, L"아지트 방문자수 초기화 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHANGE_AGIT_OPTION_BROAD, KAgitOption)
{
	KEAG_CHANGE_AGIT_OPTION_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kOption = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_CHANGE_AGIT_OPTION_BROAD, kPacket);
	START_LOG(clog, L"아지트 옵션변경 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_GUESTBOOK_WRITE_BROAD, KGuestBookWriteBroad)
{
	KEAG_GUESTBOOK_WRITE_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_nNo = kPacket_.m_nNo;
	kPacket.m_kGuestMsg = kPacket_.m_kGuestMsg;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_GUESTBOOK_WRITE_BROAD, kPacket);
	START_LOG(clog, L"아지트 낙서장 새글 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_GUESTBOOK_DELETE_BROAD, KGuestBookDeleteReq)
{
	KEAG_GUESTBOOK_DELETE_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_setDeleteNo = kPacket_.m_setDeleteNo;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_GUESTBOOK_DELETE_BROAD, kPacket);
	START_LOG(clog, L"아지트 낙서장 글삭제 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_UPDATE_TRAINING_OBJ_BROAD, KTrainingObj)
{
	KEAG_UPDATE_TRAINING_OBJ_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kTrainingObj = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_UPDATE_TRAINING_OBJ_BROAD, kPacket);
	START_LOG(clog, L"훈련소 상태변경 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_CHECK_TRAINING_OBJ_NOT, GCITEMID)
{
	// 접속 유저 없으면 return
	if (GetUserNum() == 0) {
		START_LOG(clog, L"접속 유저 없음.. AgitUID : " << GetUID()) << END_LOG;
		return;
	}

	// 훈련소가 없으면 return
	std::map<GCITEMUID, KTrainingObj> mapList;
	GetTrainingObj(mapList);
	if (mapList.empty()) {
		START_LOG(clog, L"등록된 훈련소가 없음.. AgitUID : " << GetUID()) << END_LOG;
		return;
	}

	// 실제 설치된 오브젝트가 없으면 return
	std::vector<KAgitObjectPos> vecObjPos;
	GetActiveObjectPos(kPacket_, vecObjPos);
	if (vecObjPos.empty()) {
		START_LOG(clog, L"설치된 훈련소가 없음.. AgitUID : " << GetUID()) << END_LOG;
		return;
	}

	USHORT usTrainingMaterialLimit = SiKAgitManager()->GetTrainingMaterialLimit();
	USHORT usTrainingMaterialRegenTerm = SiKAgitManager()->GetTrainingMaterialRegenTerm();
	usTrainingMaterialRegenTerm = std::max<USHORT>(usTrainingMaterialRegenTerm, 1);
	CTime tmCurrent(CTime::GetCurrentTime());

	std::vector<KAgitObjectPos>::iterator vitObj;
	for (vitObj = vecObjPos.begin(); vitObj != vecObjPos.end(); ++vitObj) {
		KLocker lock(m_csTrainingObj);
		std::map<GCITEMUID, KTrainingObj>::iterator mit;
		mit = m_mapTrainingObj.find(vitObj->m_UID);
		if (mit == m_mapTrainingObj.end()) {
			START_LOG(cwarn, L"훈련소인데, 훈련소 등록이 안되어있음.. AgitUID : " << GetUID())
				<< BUILD_LOG(vitObj->m_UID)
				<< BUILD_LOG(m_mapTrainingObj.size())
				<< BUILD_LOG(vecObjPos.size()) << END_LOG;
			continue;
		}

		// 이미 제한개수만큼 가지고 있음
		if (mit->second.m_nAccCount >= usTrainingMaterialLimit) {
			START_LOG(clog, L"이미 제한개수만큼 가지고 있음.. AgitUID : " << GetUID())
				<< BUILD_LOG(mit->second.m_nAccCount)
				<< BUILD_LOG(usTrainingMaterialLimit) << END_LOG;
			continue;
		}

		// 마지막 갱신시간
		CTime tmLastUpdate(mit->second.m_tmDate);
		CTimeSpan tsTimeSpan = tmCurrent - tmLastUpdate;
		if (tsTimeSpan.GetTotalMinutes() < 1) {
			START_LOG(clog, L"갱신체크한지 1분이 지나지 않음.. AgitUID : " << GetUID()) << END_LOG;
			continue;
		}

		// 쌓인 횟수
		USHORT usCurrentCount = (USHORT)(tsTimeSpan.GetTotalMinutes() / usTrainingMaterialRegenTerm);
		// 장작 쌓이는 시간이 안됨
		if (usCurrentCount == 0) {
			START_LOG(clog, L"장작 쌓이는 시간이 안됨.. AgitUID : " << GetUID())
				<< BUILD_LOG(tsTimeSpan.GetTotalMinutes())
				<< BUILD_LOG(usTrainingMaterialRegenTerm)
				<< BUILD_LOG(usCurrentCount) << END_LOG;
			continue;
		}

		// 시간갱신, 장작 추가
		mit->second.m_tmDate = KncUtil::TimeToInt(tmCurrent);
		mit->second.m_nAccCount += usCurrentCount;
		mit->second.m_nAccCount = std::max<int>(mit->second.m_nAccCount, 0);
		mit->second.m_nAccCount = std::min<int>(mit->second.m_nAccCount, usTrainingMaterialLimit);
		// 정보 갱신
		UpdateTrainingObj(mit->second);

		KEAG_UPDATE_TRAINING_OBJ_BROAD kPacket;
		kPacket.m_dwAgitUID = GetUID();
		kPacket.m_kTrainingObj = mit->second;
		GetUserServerList(kPacket.m_mapServerUser);

		SendToAll(KAgentEvent::EAG_UPDATE_TRAINING_OBJ_BROAD, kPacket);

		START_LOG(clog, L"훈련소 상태변경 알림. AgitUID : " << GetUID())
			<< BUILD_LOG(kPacket.m_kTrainingObj.m_ItemUID)
			<< BUILD_LOG(kPacket.m_kTrainingObj.m_nAccCount)
			<< BUILD_LOG(usCurrentCount)
			<< BUILD_LOG(usTrainingMaterialRegenTerm)
			<< BUILD_LOG(tsTimeSpan.GetTotalMinutes())
			<< BUILD_LOGtm(tmLastUpdate)
			<< BUILD_LOGtm(tmCurrent) << END_LOG;
	}
}

_IMPL_ON_FUNC(EAG_UPDATE_AGIT_CHARACTER_BROAD, KAgitCharacterPos)
{
	KEAG_UPDATE_AGIT_CHARACTER_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kCharInfo = kPacket_;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_BROAD, kPacket);
	START_LOG(clog, L"배치캐릭터 상태변경 알림. AgitUID : " << GetUID()) << END_LOG;
}

_IMPL_ON_FUNC(EAG_UPDATE_AGIT_CHARACTER_POS_BROAD, KSetAgitCharPosAck)
{
	KEAG_UPDATE_AGIT_CHARACTER_POS_BROAD kPacket;
	kPacket.m_dwAgitUID = GetUID();
	kPacket.m_kCharPosInfo.m_dwAgitUID = GetUID();
	kPacket.m_kCharPosInfo.m_vecCharPos = kPacket_.m_vecCharPos;
	GetUserServerList(kPacket.m_mapServerUser);

	SendToAll(KAgentEvent::EAG_UPDATE_AGIT_CHARACTER_POS_BROAD, kPacket);
	START_LOG(clog, L"배치캐릭터 상태변경 알림. AgitUID : " << GetUID() << L", size : " << kPacket_.m_vecCharPos.size()) << END_LOG;
}