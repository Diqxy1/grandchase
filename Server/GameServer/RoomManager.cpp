#include "User.h"
#include "Roommanager.h"
#include "RoomFSM.h"
#include "Channel.h"
#include "GSSimLayer.h"
//FILE_NAME_FOR_LOG
#include "HeroDungeonManager.h"
#include <KncUtilLua.h> // boost::bind
#include "GameServer.h"


#define DUNGEON_PAGE_SIZE   7
#define BATTLE_PAGE_SIZE    10
FSMclassPtr KRoomManager::ms_spRoomFSM;

// 080218 tslayer. GCClub 상위 정렬 sort용
namespace
{
	class KRoomGCClubTop
	{
	public:
		bool operator() (const KRoomPtr ptA, const KRoomPtr ptB)
		{
			if (ptA == NULL || ptB == NULL) return false;
			return (ptA->IsPremiumRoom() == ptB->IsPremiumRoom() ? ptA->GetRoomID() < ptB->GetRoomID() : ptA->IsPremiumRoom());
		}
	};

	bool Is_Wait_Room(const KRoomPtr & spRoom)
	{
		if (spRoom == NULL) return false;

		if (spRoom->GetStateID() == KRoomFSM::STATE_FULL ||
			spRoom->GetStateID() == KRoomFSM::STATE_CLOSE ||
			!spRoom->m_bPublic)
			return false;

		if (spRoom->GetStateID() == KRoomFSM::STATE_WAITING) return true;

		if (SiKGSSimLayer()->IsEnableDeathMatchQuickJoin() == false) { return false; }
		// 게임 모드가 데쓰매치 이면 Full, Close만 아니면 가능하다.
		if ((spRoom->m_cGameCategory == GC_GMC_DEATHMATCH && spRoom->m_bDeathMatchIntrudable) &&
			spRoom->m_usMaxUsers > spRoom->GetCount()) return true;

		return false;
	}
};

KRoomManager::KRoomManager(unsigned short usRoomNum)
{
	if (ms_spRoomFSM == NULL)
		ms_spRoomFSM.reset(new KRoomFSM);
	KRoom::InitModeCount();

	ReserveRoomObject(usRoomNum);

	m_dwResultTick = ::GetTickCount();
}

KRoomManager::~KRoomManager(void)
{
	ms_spRoomFSM.reset();    // NULL을 여러번 설정한다. 함수를 따로 불러야 하지만.. 큰 이상 없으므로 우선 그냥 둠.
}

bool KRoomManager::CreateRoom(IN OUT KUser * pkUser_,
	IN KEVENT_CREATE_ROOM_REQ & kReq_,
	OUT KEVENT_CREATE_ROOM_ACK & kAck_)
{
	kAck_.m_nOK = NetError::ERR_UNKNOWN;

	KRoomPtr spRoom;

	if (!FindClosedRoom(spRoom))
	{
		// 071122. kkurrung. 방을 무한히 만들수 있게 변경되었삼. 
		//// 061130. kkurrung. 퀘스트 제로, 용사의 마을 채널일경우 무제한 방생성.
		//if( pkUser_->GetChannel()->GetType() != KChannel::CT_INDIGO ) 
		//{
		//    SET_ERROR( ERR_ROOM_00 );
		//    return false;
		//}

		int nSize = m_vecRoom.size();
		if (!ReserveRoomObject(nSize + 50))
		{
			START_LOG(cerr, L"방 강제 추가 생성 실패") << END_LOG;
			SET_ERROR(ERR_ROOM_00);
			return false;
		}

		if (!FindClosedRoom(spRoom))
		{
			SET_ERROR(ERR_ROOM_00);
			return false;
		}
	}

	// 방이 만들어 졌을때 릴레이 서버 주소를 갱신한다.
	_JIF(spRoom, return false);
	std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
	std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
	SiKGameServer()->GetTCPServerList(vecUseTCPServerInfo, vecNoUseTCPServerInfo);
	SiKGSNetLayer()->GetURServerAddress(spRoom->m_usRoomID, spRoom->m_dwRelayServerIP, spRoom->m_usRelayServerPort);
	SiKGSNetLayer()->GetTRServerAddress(spRoom->m_usRoomID, vecUseTCPServerInfo, vecNoUseTCPServerInfo, spRoom->m_dwTRServerIP, spRoom->m_usTRServerPort);

	spRoom->SetRoomInfo(kReq_.m_kRoomInfo, pkUser_->GetChannel()->GetType(), kReq_.m_nModeInfo);
	spRoom->SetUniqueNumber(SiKGSSimLayer()->GetUniqueNumber());
	spRoom->ClearLeftRoom();

	if (spRoom->Enter(*pkUser_, kReq_.m_kInDoorUserInfo, kReq_.m_nTeamType))
	{
		spRoom->GetRoomInfo(kAck_.m_kRoomInfo);
		kAck_.m_kInDoorUserInfo = kReq_.m_kInDoorUserInfo;
		KChatEventInfo kChatEvent;
		if (SiKGSSimLayer()->GetChatEvent(kChatEvent))
		{
			spRoom->SetChatEvent(kChatEvent);
			START_LOG(clog, L"방생성시 채팅 이벤트 등록")
				<< BUILD_LOG(kChatEvent.m_nUID)
				<< BUILD_LOG(kChatEvent.m_vecEventMsg.size())
				<< BUILD_LOG(kChatEvent.m_fEXPRatio)
				<< BUILD_LOG(kChatEvent.m_fGPRatio) << END_LOG;
		}

		kAck_.m_nOK = NetError::NET_OK;
		spRoom->DumpRoomInfo();
		return true;
	}

	return false;
}

bool KRoomManager::FindClosedRoom(OUT KRoomPtr & spRoom_)
{
	std::vector<KRoomPtr>::iterator vit;
	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->GetStateID() == KRoomFSM::STATE_CLOSE)
		{
			spRoom_ = *vit;
			return true;
		}
	}
	return false;
}


/// @ return 객체가 새로 만들어 졌는지 여부.
bool KRoomManager::ReserveRoomObject(unsigned short usRoomNum_)
{
	if (m_vecRoom.size() >= usRoomNum_)
	{
		START_LOG(clog, L"m_vecRoom.size() >= usRoomNum_")
			<< BUILD_LOG(m_vecRoom.size())
			<< BUILD_LOG(usRoomNum_) << END_LOG;

		return false;
	}

	for (int i = m_vecRoom.size(); i < usRoomNum_; i++)
	{
		m_vecRoom.push_back(KRoomPtr(new KRoom(i)));
		m_vecRoom[i]->SetFSM(ms_spRoomFSM);
	}

	//cassert( m_vecRoom.size() == usRoomNum_ );

	return true;
}

bool KRoomManager::JoinRoom(IN OUT KUser * pkUser_,
	IN KJoinRoomReqInfo & kReq_,
	OUT KJoinRoomAckInfo & kAck_)
{
	if (kReq_.m_usRoomID >= m_vecRoom.size())
	{
		SET_ERROR(ERR_ROOM_04);
		kAck_.m_nOK = NetError::GetLastNetError();
		return false;
	}

	KRoomPtr spRoom = m_vecRoom[kReq_.m_usRoomID];

	_JIF(spRoom, return false);

	spRoom->DumpRoomInfo();
	if (spRoom->IsLeftUser(pkUser_->GetUID()))
	{
		SET_ERROR(ERR_ROOM_57);
		return false;
	}

	// 051031. P2P 버전이 서로 다른 방에는 입장할 수 없다.
	if (spRoom->m_iP2PVersion != pkUser_->GetP2PVersion())
	{
		SET_ERROR(ERR_ROOM_19);
		return false;
	}

	// 누구든 딛힌방에 들어갈수 없다.
	if (spRoom->GetStateID() == KRoomFSM::STATE_CLOSE)
	{
		SET_ERROR(ERR_ROOM_05);
		return false;
	}


	if (spRoom->m_iGameMode == GC_GM_DEATH_TEAM || spRoom->m_iGameMode == GC_GM_DEATH_SURVIVAL)
	{
		if (spRoom->GetStateID() == KRoomFSM::STATE_RESULT)
		{
			SET_ERROR(ERR_ROOM_05);
			return false;
		}

		if (spRoom->GetStateID() == KRoomFSM::STATE_PLAYING &&
			!spRoom->m_bDeathMatchIntrudable)
		{
			SET_ERROR(ERR_ROOM_05);
			return false;
		}
	}
	else
	{
		if (spRoom->GetStateID() == KRoomFSM::STATE_PLAYING ||
			spRoom->GetStateID() == KRoomFSM::STATE_RESULT)
		{
			SET_ERROR(ERR_ROOM_05);
			return false;
		}
	}
	// 데쓰 매치가 플레이 중인 방, 결과 보여 주는 방에는 들어 갈 수 없다.

	if (false == pkUser_->HellModeCheck(spRoom->m_iGameMode, spRoom->m_nDifficulty)) {
		SET_ERROR(ERR_ROOM_50);
		return false;
	}

	// 내부에서 에러값 세팅
	if (SiKHeroDungeonManager()->IsHeroDungeon(spRoom->m_iGameMode)) {
		if (false == pkUser_->HeroDungeonCheck(spRoom->m_iGameMode, spRoom->m_nDifficulty)) {
			return false;
		}

		if (SiKHeroDungeonManager()->IsHeroDungeonDifficultyCheck(spRoom->m_iGameMode)) {
			KDungeonUserInfo kInfo;
			LIF(pkUser_->GetDungeonUserInfo(spRoom->m_iGameMode, kInfo));

			const KInDoorUserInfo* pkInDoorUserInfo = spRoom->GetHostInDoorUserInfo();
			if (pkInDoorUserInfo) {
				std::map<int, KDungeonUserInfo>::const_iterator mitMode;
				mitMode = pkInDoorUserInfo->m_mapDifficulty.find(spRoom->m_iGameMode);
				if (mitMode != pkInDoorUserInfo->m_mapDifficulty.end()) {
					if (mitMode->second.m_kClearData.m_bClear && !kInfo.m_kClearData.m_bClear) {
						START_LOG(cwarn, L"클리어 조건 방인데, 클리어하지 못함..") << END_LOG;
						SET_ERROR(ERR_ROOM_23);
						return false;
					}

					if (!mitMode->second.m_kClearData.m_bClear && kInfo.m_kClearData.m_bClear) {
						START_LOG(cwarn, L"클리어 조건이 아닌 방인데, 클리어함..") << END_LOG;
						SET_ERROR(ERR_ROOM_23);
						return false;
					}
				}
			}
		}
	}

	if (false == pkUser_->DungeonTicketCheck(spRoom->m_iGameMode)) {
		SET_ERROR(ERR_ROOM_69);
		return false;
	}

	// 운영자는 대기방, 비밀방에 들어갈수 있다.
	// 옵저버는 비밀방, 대기방 풀방에 들어갈수 있다.
	// 일반 유저는 대기방에 들어갈수 있고 비밀방은 비번이 맞는경우 들어갈수 있다.

	// 운영자 옵저버가 아니면 비빌방에 비번 모르는 상태에서 들어갈수 없다.
	if (!pkUser_->IsAdmin() && !kReq_.m_kInDoorUserInfo.m_bIsObserver)
	{
		// 비밀방인데 비밀번호가 맞지 않는 경우.
		if (!spRoom->m_bPublic && spRoom->m_strPasswd.compare(kReq_.m_strRoomPasswd) != 0)
		{
			SET_ERROR(ERR_ROOM_06);
			return false;
		}
	}

	//옵저버이면 풀방에도 들어갈수 있다.
	if (!kReq_.m_kInDoorUserInfo.m_bIsObserver)
	{
		if (spRoom->GetStateID() == KRoomFSM::STATE_FULL)
		{
			SET_ERROR(ERR_ROOM_05);
			return false;
		}
	}

	// 060629. florist. 접속하려는 방에 방장이 없는 경우, 접속 불가능.
	if (!spRoom->m_spHostUser)
	{
		SET_ERROR(ERR_ROOM_03);
		return false;
	}

	if (spRoom->GetCount() == 0) {
		// 051003. florist. 로그 분석중 이 메세지가 나온다면, 정말 문제가 심각한 것이다. 반드시 안일어나야 한다.
		START_LOG(cerr, L"방접속 처리중 방이 이상해짐. Name : " << spRoom->GetName())
			<< BUILD_LOG(spRoom->GetCount())
			<< END_LOG;

		SET_ERROR(ERR_ROOM_17);
		return false;
	}

	if (SiKGSSimLayer()->CheckTypeFlag(ST_GUILD)) {
		if (!(pkUser_->IsAdmin() && kReq_.m_kInDoorUserInfo.m_bIsObserver) && spRoom->IsGuildRoomFull(pkUser_->GetGuildUID()) == true) {
			return false;
		}
	}

	// 현재 서버 타입에 입장 가능한 모드인지 체크.
	if (!pkUser_->CheckServerTypeEnable(spRoom->m_iGameMode)) {
		SET_ERROR(ERR_ROOM_38);
		return false;
	}

	if (spRoom->Enter(*pkUser_, kReq_.m_kInDoorUserInfo, kReq_.m_nTeamType))
	{
		kAck_.m_kRoomInfo.m_bGuild = false;
		spRoom->GetRoomInfo(kAck_.m_kRoomInfo);
		if (spRoom->GetHostPtr() != NULL)
		{
			kAck_.m_kRoomInfo.m_bGuild = true;
		}

		std::map<std::wstring, KInDoorUserInfo>::iterator mit;
		for (mit = spRoom->m_mapInDoorUserInfo.begin(); mit != spRoom->m_mapInDoorUserInfo.end(); ++mit)
		{
			kAck_.m_veckInDoorUserInfo.push_back(mit->second);
		}

		return true;
	}
	return false;
}

bool KRoomManager::QuickJoinRoom(IN OUT KUser * pkUser_, IN OUT KJoinRoomReqInfo & kReq_, OUT KJoinRoomAckInfo & kAck_)
{
	if (!pkUser_->HellModeCheck(kReq_.m_nQuickJoinModeID, kReq_.m_nDifficult)) {
		SET_ERROR(ERR_ROOM_50);
		return false;
	}

	// 내부에서 에러값 세팅
	if (false == pkUser_->HeroDungeonCheck(kReq_.m_nQuickJoinModeID, kReq_.m_nDifficult)) {
		return false;
	}

	if (false == pkUser_->DungeonTicketCheck(kReq_.m_nQuickJoinModeID)) {
		SET_ERROR(ERR_ROOM_69);
		return false;
	}

	std::vector<KRoomPtr> vecRoomTemp;

	std::copy_if(m_vecRoom.begin(), m_vecRoom.end(),
		std::insert_iterator<std::vector<KRoomPtr> >(vecRoomTemp, vecRoomTemp.end()), Is_Wait_Room);

	DumpRoomList(vecRoomTemp);
	std::random_shuffle(vecRoomTemp.begin(), vecRoomTemp.end());

	std::vector<KRoomPtr>::iterator vit;
	KQuickJoinOption kQuickJoinOption;

	// 아래 부분이 수정된 퀵조인 , 핑 그룹 맞는 방 찾으면 들어가기.
	for (vit = vecRoomTemp.begin(); vit != vecRoomTemp.end(); ++vit) {
		if ((*vit)->m_bPublic && (*vit)->m_iP2PVersion == pkUser_->GetP2PVersion() &&
			(kReq_.m_cQuickJoinCategory == GC_GMC_INVALID || (*vit)->m_cGameCategory == kReq_.m_cQuickJoinCategory) &&
			(kReq_.m_nQuickJoinModeID == GC_GM_INVALID || (*vit)->m_iGameMode == kReq_.m_nQuickJoinModeID) &&
			((*vit)->m_cGameCategory != GC_GMC_DEATHMATCH || !(*vit)->IsLeftUser(pkUser_->GetUID())) &&
			((kReq_.m_nDifficult == -1 && (*vit)->m_nDifficulty < GC_GMD_CHAMPION_BEGIN) || (*vit)->m_nDifficulty == kReq_.m_nDifficult)  // 난이도 상관없을땐 헬모드가 아니어야 한다.
			)
		{
			if (!pkUser_->IsAdmin() && std::find(kReq_.m_vecKickedRoomID.begin(), kReq_.m_vecKickedRoomID.end(), (*vit)->m_usRoomID) != kReq_.m_vecKickedRoomID.end()) {
				continue;
			}

			if (SiKGSSimLayer()->CheckTypeFlag(ST_GUILD) && (*vit)->IsGuildRoomFull(pkUser_->GetGuildUID()) == true) {
				continue;
			}

			kReq_.m_usRoomID = (*vit)->m_usRoomID;

			if (SiKGSSimLayer()->CheckSamePingGroup(pkUser_->GetPing(), (*vit)->GetRoomPing()) == false) {
				continue;
			}

			if (pkUser_->CheckSameNationGroup(pkUser_->GetCountryCode(), (*vit)->GetRoomHostNationCode()) == false) {
				continue;
			}

			// 룸 옵션 유저 인원 매칭 체크
			if (!kReq_.m_mapJoinOption.empty()) {
				CheckRoomUserMatching((*vit)->m_usRoomID, (*vit)->m_usMaxUsers, static_cast<USHORT>((*vit)->GetCount()), kReq_.m_mapJoinOption, kQuickJoinOption.m_usMaxUser, kQuickJoinOption.m_usRoomID);
				continue;
			}

			if (JoinRoom(pkUser_, kReq_, kAck_)) {
				START_LOG(clog, L"핑 기반 퀵조인 성공" << pkUser_->GetName())
					<< BUILD_LOG(pkUser_->GetPing())
					<< BUILD_LOG((*vit)->GetRoomPing()) << END_LOG;

				return true;
			}
		}
	}

	// 핑기반 퀵조인에서 빠른 입장방을 찾았는가에 대한 체크.
	if (kQuickJoinOption.m_usMaxUser != 0) {
		kReq_.m_usRoomID = kQuickJoinOption.m_usRoomID;
		if (JoinRoom(pkUser_, kReq_, kAck_)) {
			START_LOG(clog, L"핑 기반 퀵조인 성공" << pkUser_->GetName())
				<< BUILD_LOG(pkUser_->GetPing()) << END_LOG;
			return true;
		}
	}

	// 위에서 핑 기반 퀵조인 실패시에  기존 퀵조인을 실행한다.
	for (vit = vecRoomTemp.begin(); vit != vecRoomTemp.end(); ++vit) {
		if ((*vit)->m_bPublic && (*vit)->m_iP2PVersion == pkUser_->GetP2PVersion() &&
			(kReq_.m_cQuickJoinCategory == GC_GMC_INVALID || (*vit)->m_cGameCategory == kReq_.m_cQuickJoinCategory) &&
			(kReq_.m_nQuickJoinModeID == GC_GM_INVALID || (*vit)->m_iGameMode == kReq_.m_nQuickJoinModeID) &&
			((*vit)->m_cGameCategory != GC_GMC_DEATHMATCH || !(*vit)->IsLeftUser(pkUser_->GetUID())) &&
			((kReq_.m_nDifficult == -1 && (*vit)->m_nDifficulty < GC_GMD_CHAMPION_BEGIN) || (*vit)->m_nDifficulty == kReq_.m_nDifficult)  // 난이도 상관없을땐 헬모드가 아니어야 한다.
			)
		{
			if (!pkUser_->IsAdmin() && std::find(kReq_.m_vecKickedRoomID.begin(), kReq_.m_vecKickedRoomID.end(), (*vit)->m_usRoomID) != kReq_.m_vecKickedRoomID.end()) {
				continue;
			}

			if (SiKGSSimLayer()->CheckTypeFlag(ST_GUILD) && (*vit)->IsGuildRoomFull(pkUser_->GetGuildUID()) == true) {
				continue;
			}

			kReq_.m_usRoomID = (*vit)->m_usRoomID;

			// 룸 옵션 유저 인원 매칭 체크
			if (!kReq_.m_mapJoinOption.empty()) {
				CheckRoomUserMatching((*vit)->m_usRoomID, (*vit)->m_usMaxUsers, static_cast<USHORT>((*vit)->GetCount()), kReq_.m_mapJoinOption, kQuickJoinOption.m_usMaxUser, kQuickJoinOption.m_usRoomID);
				continue;
			}

			if (JoinRoom(pkUser_, kReq_, kAck_)) {
				START_LOG(clog, L"일반 퀵조인 성공" << pkUser_->GetName())
					<< BUILD_LOG(pkUser_->GetPing())
					<< BUILD_LOG((*vit)->GetRoomPing()) << END_LOG;
				return true;
			}
		}
	}

	if (kQuickJoinOption.m_usMaxUser != 0) {
		kReq_.m_usRoomID = kQuickJoinOption.m_usRoomID;
		if (JoinRoom(pkUser_, kReq_, kAck_)) {
			START_LOG(clog, L"핑 기반 퀵조인 성공" << pkUser_->GetName())
				<< BUILD_LOG(pkUser_->GetPing()) << END_LOG;
			return true;
		}
	}

	SET_ERROR(ERR_ROOM_07);
	return false;
}

void KRoomManager::BuildDungeonRoomList(OUT std::map<int, std::vector<KListPage> > & mapWaitList_,
	OUT std::map<int, std::vector<KListPage> > & mapFullList_,
	OUT std::map<int, std::vector<KGameModeState> > & mapModeState_)
{
	std::map<int, std::vector<KSimpleRoomInfo> > mapWaitRoomList;
	std::map<int, std::vector<KSimpleRoomInfo> > mapFullRoomList;
	KSimpleRoomInfo kRoomInfo;

	std::vector<KRoomPtr>::iterator vit;
	std::vector<KRoomPtr> vecRoomSort;
	vecRoomSort.clear();

	// 유효한 방
	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE && (*vit)->m_spHostUser && !(*vit)->m_mapInDoorUserInfo.empty())
			vecRoomSort.push_back(*vit);
	}

	std::sort(vecRoomSort.begin(), vecRoomSort.end(), KRoomGCClubTop()); // 080218 tslayer.GC Club 상위로 정렬

	// Union을 이용해서 던전 룸 리스트 키를 만듬.
	union { short m_asKey[2]; int m_nKey; } uniKey;
	union { short m_asKey[2]; int m_nKey; } uniAllKey;

	for (vit = vecRoomSort.begin(); vit != vecRoomSort.end(); vit++)
	{
		(*vit)->GetSimpleRoomInfo(kRoomInfo);

		uniKey.m_asKey[0] = (short)kRoomInfo.m_nGameMode;
		uniKey.m_asKey[1] = (short)kRoomInfo.m_nDifficulty;

		uniAllKey.m_asKey[0] = (short)kRoomInfo.m_nGameMode;
		uniAllKey.m_asKey[1] = -1;

		if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING) // 방 분류 리스트 정리
		{
			mapWaitRoomList[uniKey.m_nKey].push_back(kRoomInfo);
			if (kRoomInfo.m_nDifficulty < GC_GMD_CHAMPION_BEGIN)
				mapWaitRoomList[uniAllKey.m_nKey].push_back(kRoomInfo);
		}

		// 던전 상태.
		if (uniKey.m_asKey[1] >= 0 && uniKey.m_asKey[1] <= GC_GMD_BREAK_BEGIN) {


			if (mapModeState_.find(kRoomInfo.m_nGameMode) == mapModeState_.end()) {
				std::vector<KGameModeState> vecStatus;
				mapModeState_.insert(std::make_pair(kRoomInfo.m_nGameMode, vecStatus));
			}

			std::vector<KGameModeState>::iterator vit2 = std::find_if(mapModeState_[kRoomInfo.m_nGameMode].begin(),
				mapModeState_[kRoomInfo.m_nGameMode].end(),
				boost::bind(&KGameModeState::m_nDifficulty, _1) == uniKey.m_asKey[1]);


			if (vit2 == mapModeState_[kRoomInfo.m_nGameMode].end())
			{
				KGameModeState status;
				status.m_nDifficulty = uniKey.m_asKey[1];

				if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING || (*vit)->GetStateID() == KRoomFSM::STATE_FULL) {

					if ((*vit)->m_bPublic) {
						status.m_nPublicWaitRoomCount = 1;
					}
					else {
						status.m_nPrivateWaitRoomCount = 1;
					}
				}
				else if ((*vit)->GetStateID() == KRoomFSM::STATE_PLAYING) {
					status.m_nPlayingRoomCount = 1;
				}


				mapModeState_[kRoomInfo.m_nGameMode].push_back(status);
			}
			else
			{
				if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING || (*vit)->GetStateID() == KRoomFSM::STATE_FULL) {

					if ((*vit)->m_bPublic) {
						vit2->m_nPublicWaitRoomCount += 1;
					}
					else {
						vit2->m_nPrivateWaitRoomCount += 1;
					}
				}
				else if ((*vit)->GetStateID() == KRoomFSM::STATE_PLAYING) {
					vit2->m_nPlayingRoomCount += 1;
				}
			}
		}

		mapFullRoomList[uniKey.m_nKey].push_back(kRoomInfo);
		if (kRoomInfo.m_nDifficulty < GC_GMD_CHAMPION_BEGIN)
			mapFullRoomList[uniAllKey.m_nKey].push_back(kRoomInfo);
	}

	BuildPages(mapWaitList_, mapWaitRoomList, DUNGEON_PAGE_SIZE);
	BuildPages(mapFullList_, mapFullRoomList, DUNGEON_PAGE_SIZE);
}

void KRoomManager::BuildBattleRoomList(OUT std::map<int, std::vector<KListPage> > & mapWaitList_,
	OUT std::map<int, std::vector<KListPage> > & mapFullList_)
{
	std::map<int, std::vector<KSimpleRoomInfo> > mapWaitRoomList;
	std::map<int, std::vector<KSimpleRoomInfo> > mapFullRoomList;
	KSimpleRoomInfo kRoomInfo;

	// 방을 수집 하자..
	std::vector<KRoomPtr>::iterator vit;
	std::vector<KRoomPtr> vecRoomSort;
	vecRoomSort.clear();

	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE && (*vit)->m_spHostUser && !(*vit)->m_mapInDoorUserInfo.empty())
			vecRoomSort.push_back(*vit);
	}

	std::sort(vecRoomSort.begin(), vecRoomSort.end(), KRoomGCClubTop()); // 080218 tslayer.GC Club 상위로 정렬

	for (vit = vecRoomSort.begin(); vit != vecRoomSort.end(); vit++)
	{
		(*vit)->GetSimpleRoomInfo(kRoomInfo);

		if (SiKGSSimLayer()->IsEnableDeathMatchQuickJoin() == false) {
			if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING) {
				mapWaitRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo); // 대전모드는 카테고리-_- 던전은 게임모드, 헷갈리지 말 것.
				mapWaitRoomList[-1].push_back(kRoomInfo);
			}
		}
		else {
			if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING ||
				((*vit)->m_usMaxUsers > (*vit)->GetCount() && (*vit)->m_cGameCategory == GC_GMC_DEATHMATCH && (*vit)->m_bDeathMatchIntrudable)) {// 방 분류 리스트 정리

				mapWaitRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo); // 대전모드는 카테고리-_- 던전은 게임모드, 헷갈리지 말 것.
				mapWaitRoomList[-1].push_back(kRoomInfo);
			}

		}
		mapFullRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo);
		mapFullRoomList[-1].push_back(kRoomInfo);
	}

	BuildPages(mapWaitList_, mapWaitRoomList, BATTLE_PAGE_SIZE);
	BuildPages(mapFullList_, mapFullRoomList, BATTLE_PAGE_SIZE);
}

void KRoomManager::BuildFilteredBattleRoomList(OUT std::map<int, std::vector<KListPage> >& mapList_,
	IN const KRoomOptions kRoomOptions, IN const char cType)
{
	std::map<int, std::vector<KSimpleRoomInfo> > mapFullRoomList;

	// ¹æÀ» ¼öÁý ÇÏÀÚ..
	std::vector<KRoomPtr>::iterator vit;
	std::vector<KRoomPtr> vecRoomSort;
	vecRoomSort.clear();

	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE && (*vit)->m_spHostUser && !(*vit)->m_mapInDoorUserInfo.empty())
			vecRoomSort.push_back(*vit);
	}

	std::sort(vecRoomSort.begin(), vecRoomSort.end(), KRoomGCClubTop()); // 080218 tslayer.GC Club »óÀ§·Î Á¤·Ä

	bool bBlockPet = (kRoomOptions.m_dwEnabledOptions & 1) == 1;
	bool bSupremeSkillBlock = (kRoomOptions.m_dwEnabledOptions & 2) == 2;
	bool bAttributeCorrection = (kRoomOptions.m_dwEnabledOptions & 4) == 4;
	bool bRankingBlock = (kRoomOptions.m_dwEnabledOptions & 8) == 8;

	for (vit = vecRoomSort.begin(); vit != vecRoomSort.end(); vit++)
	{
		KSimpleRoomInfo kRoomInfo;
		(*vit)->GetSimpleRoomInfo(kRoomInfo);

		bool bBlockPet2 = (kRoomInfo.m_kOptions.m_dwEnabledOptions & 1) == 1;
		bool bSupremeSkillBlock2 = (kRoomInfo.m_kOptions.m_dwEnabledOptions & 2) == 2;
		bool bAttributeCorrection2 = (kRoomInfo.m_kOptions.m_dwEnabledOptions & 4) == 4;

		if (bBlockPet && !bBlockPet2)
		{
			continue;
		}

		if (bSupremeSkillBlock && !bSupremeSkillBlock2)
		{
			continue;
		}
		if (bAttributeCorrection && !bAttributeCorrection2)
		{
			continue;
		}

		if (kRoomInfo.m_kOptions.m_dwDisabledChars && kRoomOptions.m_dwDisabledChars)
		{
			if ((kRoomInfo.m_kOptions.m_dwDisabledChars & kRoomOptions.m_dwDisabledChars) == 0)
			{
				continue;
			}
		}

		if (kRoomInfo.m_kOptions.m_dwAllowedRankings && kRoomOptions.m_dwAllowedRankings)
		{
			if ((kRoomInfo.m_kOptions.m_dwAllowedRankings & kRoomOptions.m_dwAllowedRankings) == 0)
			{
				continue;
			}
		}

		mapFullRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo);
		mapFullRoomList[-1].push_back(kRoomInfo);

		START_LOG(clog, "========= Filtered Room =========")
			<< BUILD_LOG(kRoomInfo.m_strRoomName)
			<< BUILD_LOGc(kRoomInfo.m_cGameCategory)
			<< END_LOG;
	}

	BuildPages(mapList_, mapFullRoomList, BATTLE_PAGE_SIZE);
}


void KRoomManager::BuildGuildBattleRoomList(OUT std::map<int, std::vector<KListPage> > & mapWaitList_,
	OUT std::map<int, std::vector<KListPage> > & mapFullList_,
	IN const DWORD guildUID_)
{
	std::map<int, std::vector<KSimpleRoomInfo> > mapWaitRoomList;
	std::map<int, std::vector<KSimpleRoomInfo> > mapFullRoomList;

	// 방을 수집 하자..
	std::vector<KRoomPtr>::iterator vit;
	std::vector<KRoomPtr> vecRoomSort;
	vecRoomSort.clear();

	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++) {
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE && (*vit)->m_spHostUser && !(*vit)->m_mapInDoorUserInfo.empty()) {
			vecRoomSort.push_back(*vit);
		}
	}

	std::sort(vecRoomSort.begin(), vecRoomSort.end(), KRoomGCClubTop()); // 080218 tslayer.GC Club 상위로 정렬

	for (vit = vecRoomSort.begin(); vit != vecRoomSort.end(); vit++) {

		KSimpleRoomInfo kRoomInfo;
		kRoomInfo.m_bGuildPermitEnter = true;

		(*vit)->GetSimpleRoomInfo(kRoomInfo);

		if ((*vit)->GetStateID() == KRoomFSM::STATE_WAITING &&
			(*vit)->IsGuildRoomFull(guildUID_) == false) {

			mapWaitRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo); // 대전모드는 카테고리-_- 던전은 게임모드, 헷갈리지 말 것.
			mapWaitRoomList[-1].push_back(kRoomInfo);
		}

		if ((*vit)->IsGuildRoomFull(guildUID_) == true) {
			kRoomInfo.m_bGuildPermitEnter = false;
		}
		mapFullRoomList[(int)(kRoomInfo.m_cGameCategory)].push_back(kRoomInfo);
		mapFullRoomList[-1].push_back(kRoomInfo);
	}

	BuildPages(mapWaitList_, mapWaitRoomList, BATTLE_PAGE_SIZE);
	BuildPages(mapFullList_, mapFullRoomList, BATTLE_PAGE_SIZE);
}


void KRoomManager::BuildPages(OUT std::map<int, std::vector<KListPage> > & mapCompList_,
	IN std::map<int, std::vector<KSimpleRoomInfo> > & mapRoomList_, int nPageSize_)
{
	// 다 수집 했다.. 압축하자.
	KSerializer ks;
	KListPage kPage;
	kPage.m_buffComList.Clear();

	std::map<int, std::vector<KSimpleRoomInfo> >::iterator mit;
	for (mit = mapRoomList_.begin(); mit != mapRoomList_.end(); ++mit)
	{
		std::vector<KSimpleRoomInfo> vecRoomPage;
		std::vector<KSimpleRoomInfo>::iterator vit1;
		int nPageNum = 0;
		for (vit1 = mit->second.begin(); vit1 != mit->second.end(); ++vit1)
		{
			vecRoomPage.push_back(*vit1);

			if (vecRoomPage.size() == nPageSize_)
			{
				KSerializer ks;
				ks.BeginWriting(&kPage.m_buffComList);
				ks.Put(vecRoomPage);
				ks.EndWriting();

				kPage.m_nPageNum = nPageNum;
				kPage.m_buffComList.Compress();

				mapCompList_[mit->first].push_back(kPage);

				kPage.m_buffComList.Clear();
				vecRoomPage.clear();
				nPageNum++;
			}
		}

		if (!vecRoomPage.empty())
		{
			KListPage kPage;
			ks.BeginWriting(&kPage.m_buffComList);
			ks.Put(vecRoomPage);
			ks.EndWriting();

			kPage.m_nPageNum = nPageNum;
			kPage.m_buffComList.Compress();

			mapCompList_[mit->first].push_back(kPage);

			vecRoomPage.clear();
			vecRoomPage.reserve(nPageSize_);
			// kPage.m_buffComList.Clear();  필요없는가요;
		}

	}
}

KRoomPtr KRoomManager::GetRoom(IN const unsigned short usRoomID_)
{
	std::vector<KRoomPtr>::iterator vit;
	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->m_usRoomID == usRoomID_)
			return (*vit);
	}

	return KRoomPtr();
}

ImplToStringW(KRoomManager)
{
	std::vector<KRoomPtr>::const_iterator vit;
	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); vit++)
	{
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE)
		{
			stm_ << L"ID : " << (*vit)->m_usRoomID << std::endl
				<< L"    State : " << (*vit)->GetStateIDString() << std::endl
				<< L"    UserNum : (" << (*vit)->GetCount()
				<< L" / " << (*vit)->m_usMaxUsers << L")" << std::endl;
		}
	}

	return stm_;
}

void KRoomManager::DeActive(IN unsigned short usRoomID_)
{
}

void KRoomManager::Tick()
{
	if (::GetTickCount() - m_dwResultTick > 1000) // 1Sec마다 체크한다.
	{
		std::vector<KRoomPtr>::iterator vit = m_vecRoom.begin();

		for (; vit != m_vecRoom.end(); ++vit)
		{
			if ((*vit)->GetStateID() == KRoomFSM::STATE_RESULT && ::GetTickCount() - (*vit)->m_dwResultTick > 1000 * 60)
			{
				(*vit)->EndResult();
			}
		}

		m_dwResultTick = ::GetTickCount(); // 처리 시간을 기록해둔다.
	}

}

void KRoomManager::DumpRoomList(std::vector<KRoomPtr> & vecRoomList_)
{
	if (!SiKGSSimLayer()->CheckTypeFlag(ST_DP_LOG)) return;
	std::vector<KRoomPtr>::const_iterator vit;
	START_LOG(cwarn, L"===================== Start Room List Dump =====================") << END_LOG;
	START_LOG(cwarn, L"*ID, Category, Public, Mode, Intrudable, Balancing*") << END_LOG;
	for (vit = vecRoomList_.begin(); vit != vecRoomList_.end(); ++vit)
	{
		START_LOG(cwarn, L" " << (*vit)->m_usRoomID
			<< L", " << (int)((*vit)->m_cGameCategory)
			<< L", " << ((*vit)->m_bPublic ? L"T" : L"F")
			<< L", " << (*vit)->m_iGameMode
			<< L", " << ((*vit)->m_bDeathMatchIntrudable ? L"T" : L"F")
			<< L", " << ((*vit)->m_bDeathMatchBalancing ? L"T" : L"F")) << END_LOG;
	}
	START_LOG(cwarn, L"===================== End Room List Dump =====================") << END_LOG;
}

void KRoomManager::DumpInfo(std::wostream & stm_) const
{
	DWORD dwTotalRoomNum = 0;
	DWORD dwLiveRoomNum = 0;
	DWORD dwRoomUserNum = 0;
	dwTotalRoomNum = m_vecRoom.size();

	std::vector<KRoomPtr>::const_iterator vit;
	for (vit = m_vecRoom.begin(); vit != m_vecRoom.end(); ++vit)
	{
		if ((*vit)->GetStateID() != KRoomFSM::STATE_CLOSE)
		{
			++dwLiveRoomNum;
			dwRoomUserNum += (*vit)->GetCount();
		}
	}

	stm_ << L", RoomNum : " << dwLiveRoomNum << L" / " << dwTotalRoomNum
		<< L", RoomUser : " << dwRoomUserNum;
}

KRoom* KRoomManager::GetRoomForLua(IN unsigned short usRoomID)
{
	return GetRoom(usRoomID).get();
}

void KRoomManager::CheckRoomUserMatching(IN const USHORT & usRoomID_, IN const USHORT & usMaxUsers_, IN const USHORT & usUsers_, IN std::map<DWORD, DWORD> & mapJoinOption_, IN OUT USHORT & usOPMaxUser_, IN OUT USHORT & usOPRoomID_)
{
	std::map<DWORD, DWORD>::iterator mit;
	mit = mapJoinOption_.find(KJoinRoomReqInfo::OPTION_USERNUM);
	if (mit != mapJoinOption_.end()) {
		if (usMaxUsers_ == mit->second && usUsers_ > usOPMaxUser_) {
			usOPMaxUser_ = usUsers_;
			usOPRoomID_ = usRoomID_;
		}
	}
}

bool KRoomManager::IncreaseMatchFailCount(IN const USHORT & usRoomID_)
{
	if (usRoomID_ >= m_vecRoom.size()) {
		START_LOG(cerr, L"유효하지 하는 룸 번호로 방정보 가져올려고 함")
			<< BUILD_LOG(usRoomID_)
			<< BUILD_LOG(m_vecRoom.size())
			<< END_LOG;
		return false;
	}

	KRoomPtr spRoom = m_vecRoom[usRoomID_];
	_JIF(spRoom, return false);
	spRoom->IncreaseMatchFailCount();
	return true;
}