#include <winsock2.h>
#include <iomanip>
#include <sstream>
#include <dbg/dbg.hpp>
#include "AGDBThread.h"
#include "AgentServer.h"
#include "AGSimLayer.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "NetError.h"
#include "AgitManager.h"
#include "Log4.h"

ImplementDBThread(KAGDBThread);
ImplementDBThreadName(KAGDBThread, Stat);

#define CLASS_TYPE KAGDBThread

KAGDBThread::KAGDBThread()
{
	m_kODBC.DriverConnect(ms_wstrFileDSN.c_str());
	m_kODBCStat.DriverConnect(ms_wstrFileDSNStat.c_str());
}

KAGDBThread::~KAGDBThread(void)
{
	End();
}

void KAGDBThread::ProcessIntEvent(const KIntEvent& kIntEvent_)
{
	KSerializer ks;
	ks.BeginReading(const_cast<KSerBuffer*>(&kIntEvent_.m_kbuff));

	switch (kIntEvent_.m_usEventID)
	{
		_INT_CASE(DB_EVENT_FIRST_INIT_REQ, int);
		_INT_CASE(DB_EVENT_SERVER_LIST_REQ, int);

		INT_CASE(EAG_BUY_AGIT_MAP_REQ);
		_INT_CASE(EAG_SET_AGIT_OPTION_REQ, KAgitOption);
		_INT_CASE(EAG_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq);
		_INT_CASE(EAG_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq);
		_INT_CASE(EAG_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq);
		_INT_CASE(EAG_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq);
		_INT_CASE(EAG_AGIT_ADD_FAVORITE_REQ, KAgitUserParam);
		_INT_CASE(EAG_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam);
		_INT_CASE(EAG_REGISTER_AGIT_PAGE_REQ, KDB_EAG_REGISTER_AGIT_PAGE_REQ);
		_INT_CASE(EAG_EQUIP_BROWNIE_REQ, KEquipBrownieReq);
		INT_CASE(EAG_WATERING_FAIRY_TREE_REQ);
		INT_CASE(EAG_USE_FAIRY_TREE_FRUIT_REQ);
		INT_CASE(EAG_UPDATE_FAIRY_TREE_REQ);
		INT_CASE_NOPARAM(EAG_AGIT_RANK_LIST_NOT);
		INT_CASE(EAG_UPDATE_AGIT_VISITER_COUNT_NOT);
		INT_CASE(EAG_ENTER_AGIT_STAT_NOT);

		_INT_CASE(EAG_LOAD_AGIT_INFO_NOT, DWORD);
		_INT_CASE(EAG_ENTER_AGIT_REQ, KEnterAgitReq);
		_INT_CASE(EAG_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq);
		_INT_CASE(EAG_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq);
		INT_CASE(EAG_UPDATE_TRAINING_OBJ_REQ);
		_INT_CASE(EAG_OPEN_AGIT_CHARACTER_REQ, PAIR_DWORD_INT);
		_INT_CASE(EAG_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq);
		_INT_CASE(EAG_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq);
		_INT_CASE(EAG_SET_AGIT_CHARACTER_COORDI_REQ, KSetAgitCharCoordiReq);
		_INT_CASE(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq);
	default:
		START_LOG(cerr, L"이벤트 핸들러가 정의되지 않았음. "
			<< IDVenderType::GetEventIDString(kIntEvent_.m_usEventID))
			<< END_LOG;
	}
}

_INT_IMPL_ON_FUNC(DB_EVENT_FIRST_INIT_REQ, int)
{
	SetServerListFromDB(kPacket_);

	// 서버 초기정보 받아오기
	// 뭘 받아와야할까..

	SiKAgentServer()->SetInitEvent();
}

_INT_IMPL_ON_FUNC(DB_EVENT_SERVER_LIST_REQ, int)
{
	SetServerListFromDB(kPacket_);
}

INT_IMPL_ON_FUNC(EAG_BUY_AGIT_MAP_REQ)
{
	KBuyAgitMapAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwMapID = kPacket_.m_dwMapID;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_kPayItem = kPacket_.m_kPayItem;
	const int& nNeedItemCount = kPacket_.m_nPrice;

	SET_ERROR(ERR_UNKNOWN);

	// 맵 타입 변경
	if (false == AgitInfo_agittype_update(kPacket.m_dwUserUID, kPacket.m_dwMapID)) {
		SET_ERR_GOTO(ERR_AGIT_MAP_06, END_PROC);
	}

	// 화폐 차감
	UseCountItem(kPacket.m_dwUserUID, kPacket.m_kPayItem.m_ItemUID, nNeedItemCount);

	kPacket.m_kPayItem.m_nCount -= nNeedItemCount;
	kPacket.m_kPayItem.m_nCount = std::max<int>(kPacket.m_kPayItem.m_nCount, 0);
	kPacket.m_kPayItem.m_nInitCount = kPacket.m_kPayItem.m_nCount;

	// 청소시간 갱신
	LIF(AgitInfo_lastcleaning_update(kPacket.m_dwUserUID));
	kPacket.m_tmLastCleanTime = KncUtil::TimeToInt(CTime::GetCurrentTime());

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket.m_dwMapID)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket.m_kPayItem.m_ItemID)
		<< BUILD_LOG(kPacket.m_kPayItem.m_ItemUID)
		<< BUILD_LOG(kPacket.m_kPayItem.m_nCount)
		<< BUILD_LOG(nNeedItemCount)
		<< BUILD_LOGtm(CTime(kPacket.m_tmLastCleanTime)) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_BUY_AGIT_MAP_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_OPTION_REQ, KAgitOption)
{
	KSetAgitOptionAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_kOption = kPacket_;

	SET_ERROR(ERR_UNKNOWN);

	// 아지트 옵션 변경
	if (false == AgitInfo_option_update(kPacket_.m_dwAgitUID, kPacket_)) {
		SET_ERR_GOTO(ERR_AGIT_OPT_14, END_PROC);
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_strName)
		<< BUILD_LOGc(kPacket_.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_strPassword)
		<< BUILD_LOGc(kPacket_.m_ucInviteType)
		<< BUILD_LOG(kPacket_.m_strProfile) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_OPTION_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq)
{
	CTime tmFlowerTime;
	KFlowerPotOption kOption;
	KSeedFlowerPotAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kPotInfo.m_UID = kPacket_.m_kPotItem.m_ItemUID;
	kPacket.m_kPotInfo.m_ItemID = kPacket_.m_kPotItem.m_ItemID;
	kPacket.m_kPotInfo.m_nSeedType = kPacket_.m_nSeedType;
	kPacket.m_kSeedItem = kPacket_.m_kSeedItem;

	SET_ERROR(ERR_UNKNOWN);

	// 씨앗심기 타입확인
	if (false == SiKAgitManager()->GetFlowerPotOption(kPacket_.m_nSeedType, kOption)) {
		SET_ERR_GOTO(ERR_SEED_FLOWER_05, END_PROC);
	}

	// 화분 초기화
	if (false == AgiteFlowerpot_init(kPacket_.m_dwAgitUID, kPacket_.m_kPotItem.m_ItemUID)) {
		SET_ERR_GOTO(ERR_SEED_FLOWER_11, END_PROC);
	}

	// 씨앗심기
	if (false == AgiteFlowerpot_seed(kPacket_.m_dwAgitUID, kPacket_.m_kPotItem.m_ItemUID, kPacket_.m_nSeedType, kPacket.m_kPotInfo.m_tmSeedTime)) {
		SET_ERR_GOTO(ERR_SEED_FLOWER_12, END_PROC);
	}

	// 꽃피는 시간
	tmFlowerTime = CTime(kPacket.m_kPotInfo.m_tmSeedTime) + CTimeSpan(0, 0, kOption.m_nGrowTime, 0);
	kPacket.m_kPotInfo.m_tmFlowerTime = KncUtil::TimeToInt(tmFlowerTime);
	// 꽃개수
	kPacket.m_kPotInfo.m_usFlowerCount = kOption.m_usFlowerCount;

	// 씨앗 차감
	UseCountItem(kPacket.m_dwAgitUID, kPacket.m_kSeedItem.m_ItemUID, kPacket_.m_nNeedSeedCount);
	kPacket.m_kSeedItem.m_nCount -= kPacket_.m_nNeedSeedCount;
	kPacket.m_kSeedItem.m_nInitCount = kPacket.m_kSeedItem.m_nCount;

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nNeedSeedCount)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_UID)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_ItemID)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_nSeedType)
		<< BUILD_LOGtm(CTime(kPacket.m_kPotInfo.m_tmSeedTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kPotInfo.m_tmFlowerTime))
		<< BUILD_LOG(kPacket.m_kPotInfo.m_usFlowerCount)
		<< BUILD_LOG(kPacket.m_kSeedItem.m_ItemID)
		<< BUILD_LOG(kPacket.m_kSeedItem.m_ItemUID)
		<< BUILD_LOG(kPacket.m_kSeedItem.m_nCount) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SEED_FLOWER_POT_ACK);
}

_INT_IMPL_ON_FUNC(EAG_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq)
{
	KHarvestFlowerPotAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kFlowerItem = kPacket_.m_kFlowerItem;
	kPacket.m_nFlowerCount = kPacket_.m_nFlowerCount;
	kPacket.m_nBonusFlowerCount = kPacket_.m_nBonusFlowerCount;
	kPacket.m_kPotInfo.m_UID = kPacket_.m_kPotItem.m_ItemUID;
	kPacket.m_kPotInfo.m_ItemID = kPacket_.m_kPotItem.m_ItemID;

	SET_ERROR(ERR_UNKNOWN);

	// 화분 초기화
	if (false == AgiteFlowerpot_init(kPacket_.m_dwAgitUID, kPacket_.m_kPotItem.m_ItemUID)) {
		SET_ERR_GOTO(ERR_HARVEST_FLOWER_09, END_PROC);
	}

	kPacket.m_kPotInfo.m_nSeedType = KFlowerPot::ST_NONE;
	kPacket.m_kPotInfo.m_usFlowerCount = 0;
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nFlowerCount)
		<< BUILD_LOG(kPacket_.m_nBonusFlowerCount)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_UID)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_ItemID)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_nSeedType)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_nSeedType)
		<< BUILD_LOG(kPacket.m_kPotInfo.m_usFlowerCount) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_HARVEST_FLOWER_POT_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq)
{
	KSetAgitObjectPosAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_vecObject.clear();

	SET_ERROR(ERR_UNKNOWN);

	std::vector<KAgitObjectPos>::const_iterator vit;
	for (vit = kPacket_.m_vecObject.begin(); vit != kPacket_.m_vecObject.end(); ++vit) {
		// 아지트 오브젝트 배치
		if (false == AgiteObjPos_merge(kPacket_.m_dwAgitUID, vit->m_UID, vit->m_nPosX, vit->m_nPosY)) {
			START_LOG(cerr, L"아지트 오브젝트 배치 DB작업 실패. AgitUID : " << kPacket_.m_dwAgitUID)
				<< BUILD_LOG(vit->m_UID)
				<< BUILD_LOG(vit->m_nPosX)
				<< BUILD_LOG(vit->m_nPosY) << END_LOG;
			continue;
		}

		kPacket.m_vecObject.push_back(*vit);
	}

	// DB처리 실패
	if (kPacket.m_vecObject.empty()) {
		SET_ERR_GOTO(ERR_AGIT_OBJECT_05, END_PROC);
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecObject.size())
		<< BUILD_LOG(kPacket.m_vecObject.size()) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_OBJECT_POS_ACK);
}

_INT_IMPL_ON_FUNC(EAG_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq)
{
	KSimpleAgitPageList kPacket;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_mapPageList.clear();

	std::set<DWORD>::iterator sit;
	std::wstring strNick;
	KAgitInfo kInfo;
	std::map<DWORD, KAgitPageInfo> mapPage;
	mapPage.clear();

	// 친구의 아지트 정보 불러오기
	for (sit = kPacket_.m_setFriendUID.begin(); sit != kPacket_.m_setFriendUID.end(); ++sit) {
		// 친구 아지트 정보는 DB에서 받아오기
		if (false == AgitInfo_select(*sit, kInfo)) {
			continue;
		}
		LIF(LoginUIDtoNickname_select(*sit, strNick));

		KAgitPageInfo kPage;
		kPage.m_dwUID = kInfo.m_dwUID;
		kPage.m_strName = kInfo.m_strName;
		kPage.m_strOwnerNick = strNick;
		kPage.m_ucMaxUser = kInfo.m_ucMaxUser;
		kPage.m_ucOpenType = kInfo.m_ucOpenType;
		mapPage[kPacket.m_dwUserUID] = kPage;
	}

	// 페이지화
	SiKAgitManager()->BuildPageBuffer(mapPage, kPacket.m_mapPageList);

	START_LOG(clog, L"친구아지트 리스트 전달")
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_setFriendUID.size())
		<< BUILD_LOG(mapPage.size())
		<< BUILD_LOG(kPacket.m_mapPageList.size()) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_AGIT_FRIEND_LIST_ACK);
}

_INT_IMPL_ON_FUNC(EAG_AGIT_ADD_FAVORITE_REQ, KAgitUserParam)
{
	KAgitAddFavoriteAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_kAgitInfo.m_dwUID = kPacket_.m_dwAgitUID;

	SET_ERROR(ERR_UNKNOWN);

	// 즐겨찾기 추가
	if (false == AgitFavorites_insert(kPacket_.m_dwUserUID, kPacket_.m_dwAgitUID)) {
		SET_ERR_GOTO(ERR_AGIT_FAVORITE_06, END_PROC);
	}

	// 다른 에이전트서버의 아지트 정보일 경우도 있으니, 임시로 정보를 받아오자.
	{
		KAgitInfo kInfo;
		std::wstring strNick;
		LIF(AgitInfo_select(kPacket_.m_dwAgitUID, kInfo));
		LIF(AgiteObjPos_select(kPacket_.m_dwAgitUID, kInfo.m_vecObjectPos));
		LIF(AgiteCharInfo_select(kPacket_.m_dwAgitUID, kInfo.m_mapCharacterPos));
		LIF(AgiteCharCoordi_select(kPacket_.m_dwAgitUID, kInfo.m_mapCharacterPos));
		LIF(AgitCharDesignCoordi_select(kPacket_.m_dwUserUID, kInfo.m_mapCharacterPos));
		LIF(LoginUIDtoNickname_select(kPacket_.m_dwAgitUID, strNick));

		kPacket.m_kAgitInfo.m_dwUID = kInfo.m_dwUID;
		kPacket.m_kAgitInfo.m_strName = kInfo.m_strName;
		kPacket.m_kAgitInfo.m_ucMaxUser = kInfo.m_ucMaxUser;
		kPacket.m_kAgitInfo.m_ucOpenType = kInfo.m_ucOpenType;
		kPacket.m_kAgitInfo.m_strOwnerNick = strNick;
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_AGIT_ADD_FAVORITE_ACK);
}

_INT_IMPL_ON_FUNC(EAG_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam)
{
	KAgitUserParamAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;

	SET_ERROR(ERR_UNKNOWN);

	// 즐겨찾기 제거
	if (false == AgitFavorites_delete(kPacket_.m_dwUserUID, kPacket_.m_dwAgitUID)) {
		SET_ERR_GOTO(ERR_AGIT_FAVORITE_08, END_PROC);
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_AGIT_REMOVE_FAVORITE_ACK);
}

_INT_IMPL_ON_FUNC(EAG_REGISTER_AGIT_PAGE_REQ, KDB_EAG_REGISTER_AGIT_PAGE_REQ)
{
	KRegisterAgitPageAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_strNickName = kPacket_.m_strNickName;
	kPacket.m_kAgitOption = kPacket_.m_kAgitOption;

	SET_ERROR(ERR_UNKNOWN);

	// 아지트 옵션 변경
	if (false == AgitInfo_option_update(kPacket_.m_dwAgitUID, kPacket_.m_kAgitOption)) {
		SET_ERR_GOTO(ERR_AGIT_PAGE_04, END_PROC);
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_strNickName)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strName)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucMaxUser)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucOpenType)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strPassword)
		<< BUILD_LOGc(kPacket_.m_kAgitOption.m_ucInviteType)
		<< BUILD_LOG(kPacket_.m_kAgitOption.m_strProfile) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_REGISTER_AGIT_PAGE_ACK);
}

_INT_IMPL_ON_FUNC(EAG_EQUIP_BROWNIE_REQ, KEquipBrownieReq)
{
	KEquipBrownieAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
	kPacket.m_kEquip = kPacket_.m_kEquip;
	kPacket.m_kBrownieInfo = kPacket_.m_kBrownieInfo;

	SET_ERROR(ERR_UNKNOWN);

	kPacket_.m_kBrownieInfo.m_EquipItemID = kPacket_.m_kEquip.m_ItemID;
	kPacket_.m_kBrownieInfo.m_EquipItemUID = kPacket_.m_kEquip.m_ItemUID;

	// 도우미 장착아이템 변경
	if (false == AgitHelper_merge(kPacket_.m_dwUserUID, kPacket_.m_kBrownieInfo)) {
		SET_ERR_GOTO(ERR_AGIT_BROWNIE_06, END_PROC);
	}

	// 정보갱신
	kPacket.m_kBrownieInfo = kPacket_.m_kBrownieInfo;

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket.m_kBrownieInfo.m_EquipItemID)
		<< BUILD_LOG(kPacket.m_kBrownieInfo.m_EquipItemUID)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemID)
		<< BUILD_LOG(kPacket_.m_kEquip.m_ItemUID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_dwUID)
		<< BUILD_LOG(kPacket_.m_kBrownieInfo.m_bIsUsed) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_EQUIP_BROWNIE_ACK);
}

INT_IMPL_ON_FUNC(EAG_WATERING_FAIRY_TREE_REQ)
{
	KWateringFairyTreeAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.m_kPacket.m_dwUserUID;
	kPacket.m_kFairyTree = kPacket_.m_kPacket.m_kFairyTree;

	SET_ERROR(ERR_UNKNOWN);

	// 요정의 나무 정보 갱신
	if (false == AgitFairyTree_merge(kPacket.m_dwUserUID, kPacket.m_kFairyTree)) {
		SET_ERR_GOTO(ERR_WATERING_FAIRY_TREE_05, END_PROC);
	}

	// 열매 교체시, 열매도 기록
	if (kPacket_.m_bFruitChange) {
		LIF(AgitFairyTreeFruit_merge(kPacket.m_dwUserUID, kPacket.m_kFairyTree.m_nFruitType, kPacket.m_kFairyTree.m_tmUpdateFruit));
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_kPacket.m_dwAddExp)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nLv)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_dwExp)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastGrowTime))
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmLastDecreaseTime))
		<< BUILD_LOG(kPacket_.m_bFruitChange)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_bUseFruit)
		<< BUILD_LOG(kPacket.m_kFairyTree.m_nFruitType)
		<< BUILD_LOGtm(CTime(kPacket.m_kFairyTree.m_tmUpdateFruit)) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_WATERING_FAIRY_TREE_ACK);
}

INT_IMPL_ON_FUNC(EAG_USE_FAIRY_TREE_FRUIT_REQ)
{
	KUseFairyTreeFruitAck kPacket;
	kPacket.m_kFruitBuff.m_dwUserUID = kPacket_.m_dwAgitUID;
	kPacket.m_nOK = -99;

	time_t tmLastFruitTime;
	int nLastFruitType = KFairyTreeConfig::FBT_NONE;

	SET_ERROR(ERR_UNKNOWN);

	// 열매먹기 DB기록
	if (false == SetTodayUseFruit(kPacket_.m_dwAgitUID)) {
		SET_ERR_GOTO(ERR_USE_FAIRY_TREE_FRUIT_05, END_PROC);
	}

	if (false == AgitFairyTreeFruit_select(kPacket_.m_dwAgitUID, tmLastFruitTime, nLastFruitType)) {
		START_LOG(cerr, L"요정의나무 열매정보 불러오기 DB처리 실패.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
	}
	else {
		// 열매 갱신시간 체크
		CTime tmUpdateFruit = CTime(tmLastFruitTime);
		CTime tmCurrent = CTime::GetCurrentTime();
		time_t tmCurrentTime = KncUtil::TimeToInt(tmCurrent);

		bool bUpdateFruit = false;

		// 이전 날짜면 갱신할것
		if (tmUpdateFruit.GetYear() < tmCurrent.GetYear() ||
			(tmUpdateFruit.GetYear() == tmCurrent.GetYear() && tmUpdateFruit.GetMonth() < tmCurrent.GetMonth()) ||
			(tmUpdateFruit.GetYear() == tmCurrent.GetYear() && tmUpdateFruit.GetMonth() == tmCurrent.GetMonth() && tmUpdateFruit.GetDay() < tmCurrent.GetDay())) {
			bUpdateFruit = true;
		}

		// 과일 타입이 없으면 갱신할 것
		if (nLastFruitType == KFairyTreeConfig::FBT_NONE) {
			bUpdateFruit = true;
		}

		if (bUpdateFruit) {
			LIF(AgitFairyTreeFruit_merge(kPacket_.m_dwAgitUID, kPacket_.m_nFruitType, tmCurrentTime));
			START_LOG(cerr, L"열매 먹는데, 열매정보가 DB에 없어서 현재 서버가 가지고 있는 열매정보를 DB에 기록함.. AgitUID : " << kPacket_.m_dwAgitUID)
				<< BUILD_LOG(kPacket_.m_nFruitType)
				<< BUILD_LOGtm(tmCurrent) << END_LOG;
		}
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nFruitType) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_USE_FAIRY_TREE_FRUIT_ACK);
}

INT_IMPL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_REQ)
{
	// 요정의 나무 정보 갱신
	JIF(AgitFairyTree_merge(kPacket_.m_dwAgitUID, kPacket_.m_kFairyTree));
	JIF(AgitFairyTreeFruit_merge(kPacket_.m_dwAgitUID, kPacket_.m_kFairyTree.m_nFruitType, kPacket_.m_kFairyTree.m_tmUpdateFruit));
	SiKAgitManager()->QueueingEvent(KAgentEvent::EAG_UPDATE_FAIRY_TREE_ACK, kPacket_.m_dwAgitUID, kPacket_);
}

INT_IMPL_ON_FUNC_NOPARAM(EAG_AGIT_RANK_LIST_NOT)
{
	std::wstring strNick;
	KAgitInfo kInfo;
	std::vector<KAgitPageInfo> vecPage;
	std::map<int, std::vector<KRankAgit> > mapRankList; // map[RankType, vector[랭킹아지트] ]
	mapRankList.clear();

	LIF(AgitRank_select(mapRankList));

	std::map<int, std::vector<KRankAgit> >::iterator mit;
	for (mit = mapRankList.begin(); mit != mapRankList.end(); ++mit) {
		std::vector<KRankAgit>::iterator vit;
		for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
			// 아지트 정보 받아오기
			if (false == AgitInfo_select(vit->m_dwAgitUID, kInfo)) {
				continue;
			}
			LIF(LoginUIDtoNickname_select(vit->m_dwAgitUID, strNick));

			KAgitPageInfo kPage;
			kPage.m_dwUID = kInfo.m_dwUID;
			kPage.m_strName = kInfo.m_strName;
			kPage.m_strOwnerNick = strNick;
			kPage.m_ucMaxUser = kInfo.m_ucMaxUser;
			kPage.m_ucOpenType = kInfo.m_ucOpenType;

			vecPage.push_back(kPage);
		}
	}

	std::map<int, KSerBuffer> mapPageBuff;
	SiKAgitManager()->BuildOnePageBuffer(vecPage, mapPageBuff);
	SiKAgitManager()->SetRankAgit(mapPageBuff, mapRankList);

	START_LOG(cerr, L"일일 랭킹 아지트정보 DB로부터 갱신.")
		<< BUILD_LOGtm(CTime::GetCurrentTime())
		<< BUILD_LOG(mapPageBuff.size())
		<< BUILD_LOG(mapRankList.size()) << END_LOG;
}

INT_IMPL_ON_FUNC(EAG_UPDATE_AGIT_VISITER_COUNT_NOT)
{
	// 아지트 방문자 수 갱신
	if (kPacket_.m_nTodayVisiterDiff > 0) {
		JIF(AgitInfo_visitor_update(kPacket_.m_dwAgitUID, kPacket_.m_nTodayVisiterDiff));
	}
}

INT_IMPL_ON_FUNC(EAG_ENTER_AGIT_STAT_NOT)
{
	// 아지트 입장유저 통계
	LIF(AgitSystemCollect_insert(kPacket_.m_dwAgitUID, kPacket_.m_dwUserUID, kPacket_.m_dwPlayTime));
}

_INT_IMPL_ON_FUNC(EAG_LOAD_AGIT_INFO_NOT, DWORD)
{
	const DWORD& dwAgitUID = kPacket_;
	KEAG_LOAD_AGIT_INFO_NOT kPacket;

	GetAgitInfoFromDB(dwAgitUID, kPacket.m_kInfo, kPacket.m_strNick);
	AgiteFlowerpot_select(dwAgitUID, kPacket.m_mapFlowerPot);
	GetFavoritesInfoFromDB(dwAgitUID, kPacket.m_mapFavoritesInfo);
	AgitHelper_select(dwAgitUID, kPacket.m_mapBrownie);
	GetFairyTreeInfoFromDB(dwAgitUID, kPacket.m_kTree);
	AgitGuestBook_select(dwAgitUID, kPacket.m_mapGuestbook);
	AgitTrainingOBJ_select(dwAgitUID, kPacket.m_mapTrainingObj);

	START_LOG(clog, L"Load AgitInfo.. AgitUID : " << kPacket_) << END_LOG;
	_QUEUING_ACK_TO_USER(EAG_LOAD_AGIT_INFO_NOT, kPacket);
}

_INT_IMPL_ON_FUNC(EAG_ENTER_AGIT_REQ, KEnterAgitReq)
{
	const DWORD& dwAgitUID = kPacket_.m_dwAgitUID;
	KEAG_ENTER_AGIT_ACK kPacket;
	kPacket.m_kEnterAgitReq = kPacket_;

	GetAgitInfoFromDB(dwAgitUID, kPacket.m_kAgitLoadInfo.m_kInfo, kPacket.m_kAgitLoadInfo.m_strNick);
	AgiteFlowerpot_select(dwAgitUID, kPacket.m_kAgitLoadInfo.m_mapFlowerPot);
	GetFavoritesInfoFromDB(dwAgitUID, kPacket.m_kAgitLoadInfo.m_mapFavoritesInfo);
	AgitHelper_select(dwAgitUID, kPacket.m_kAgitLoadInfo.m_mapBrownie);
	GetFairyTreeInfoFromDB(dwAgitUID, kPacket.m_kAgitLoadInfo.m_kTree);
	AgitGuestBook_select(dwAgitUID, kPacket.m_kAgitLoadInfo.m_mapGuestbook);
	AgitTrainingOBJ_select(dwAgitUID, kPacket.m_kAgitLoadInfo.m_mapTrainingObj);

	START_LOG(clog, L"Load Enter AgitInfo.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
	_QUEUING_ACK_TO_USER(EAG_ENTER_AGIT_ACK, kPacket);
}

_INT_IMPL_ON_FUNC(EAG_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq)
{
	KEAG_GUESTBOOK_WRITE_ACK kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_nNo = -1;
	kPacket.m_kGuestbookWriteReq = kPacket_;
	kPacket.m_tmDate = KncUtil::TimeToInt(CTime::GetCurrentTime());

	SET_ERROR(ERR_UNKNOWN);

	// 낙서장 새글 쓰기 DB기록
	if (false == AgitGuestBook_insert(kPacket_.m_dwAgitUID, kPacket_.m_dwUserUID, kPacket_.m_strMsg, kPacket.m_nNo)) {
		SET_ERR_GOTO(ERR_GUESTBOOK_06, END_PROC);
	}
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_strNick)
		<< BUILD_LOG(kPacket_.m_strMsg)
		<< BUILD_LOG(kPacket.m_nNo)
		<< BUILD_LOGtm(CTime(kPacket.m_tmDate)) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_GUESTBOOK_WRITE_ACK);
}

_INT_IMPL_ON_FUNC(EAG_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq)
{
	KEAG_GUESTBOOK_DELETE_ACK kPacket;
	kPacket.m_kGuestBookDeleteReq = kPacket_;
	kPacket.m_nOK = -99;

	std::set<int> setDeleteNo;
	setDeleteNo.clear();

	SET_ERROR(ERR_UNKNOWN);

	// 낙서장 글삭제 DB기록
	std::set<int>::iterator sit;
	for (sit = kPacket_.m_setDeleteNo.begin(); sit != kPacket_.m_setDeleteNo.end(); ++sit) {
		if (false == AgitGuestBook_delete(kPacket_.m_dwAgitUID, kPacket_.m_dwUserUID, *sit)) {
			START_LOG(cerr, L"낙서장 글삭제 실패.. AgitUID : " << kPacket_.m_dwAgitUID)
				<< BUILD_LOG(kPacket_.m_dwUserUID)
				<< BUILD_LOG(*sit) << END_LOG;
			continue;
		}

		setDeleteNo.insert(*sit);
	}

	if (setDeleteNo.empty()) {
		SET_ERR_GOTO(ERR_GUESTBOOK_09, END_PROC);
	}

	kPacket.m_kGuestBookDeleteReq.m_setDeleteNo.swap(setDeleteNo);
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_dwUserUID)
		<< BUILD_LOG(kPacket_.m_setDeleteNo.size())
		<< BUILD_LOG(kPacket.m_kGuestBookDeleteReq.m_setDeleteNo.size()) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_GUESTBOOK_DELETE_ACK);
}

INT_IMPL_ON_FUNC(EAG_UPDATE_TRAINING_OBJ_REQ)
{
	time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime());

	// 훈련소 DB기록 갱신
	std::map<GCITEMUID, KTrainingObj>::iterator mit;
	for (mit = kPacket_.m_mapTrainingObj.begin(); mit != kPacket_.m_mapTrainingObj.end(); ++mit) {
		int nAccDiff = mit->second.m_nAccCount - mit->second.m_nInitAccCount;
		if (false == AgitTrainingOBJ_merge(kPacket_.m_dwAgitUID, mit->second.m_ItemUID, nAccDiff, tmCurrent)) {
			START_LOG(cerr, L"훈련소 DB동기화 실패.. AgitUId : " << kPacket_.m_dwAgitUID)
				<< BUILD_LOG(mit->second.m_ItemUID)
				<< BUILD_LOG(mit->second.m_nAccCount)
				<< BUILD_LOG(mit->second.m_nInitAccCount)
				<< BUILD_LOG(nAccDiff)
				<< BUILD_LOGtm(CTime(mit->second.m_tmDate))
				<< BUILD_LOGtm(CTime(tmCurrent)) << END_LOG;
		}

		// 동기화
		mit->second.m_tmDate = tmCurrent;
		mit->second.m_nInitAccCount = mit->second.m_nAccCount;
	}

	START_LOG(clog, L"훈련소 재설치 DB기록 갱신.. AgitUID : " << kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_mapTrainingObj.size())
		<< BUILD_LOGtm(CTime(tmCurrent)) << END_LOG;

	_QUEUING_ACK_TO_USER(EAG_UPDATE_TRAINING_OBJ_ACK, kPacket_);
}

_INT_IMPL_ON_FUNC(EAG_OPEN_AGIT_CHARACTER_REQ, PAIR_DWORD_INT)
{
	KOpenAgitCharacterAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwUserUID = kPacket_.first;
	kPacket.m_nCharType = kPacket_.second;
	kPacket.m_mapCharacterPos.clear();

	SET_ERROR(ERR_UNKNOWN);

	// 캐릭터 배치 오픈요청
	if (false == AgitCharInfo_insert(kPacket.m_dwUserUID, kPacket.m_nCharType)) {
		START_LOG(cerr, L"캐릭터 배치 오픈요청.. AgitUID : " << kPacket.m_dwUserUID)
			<< BUILD_LOG(kPacket.m_dwUserUID)
			<< BUILD_LOG(kPacket.m_nCharType) << END_LOG;
		// 캐릭터 정보 가져오기가 실패했을 때 DB에서 현재 저장된 캐릭터 리스트 전달.
		if (false == AgiteCharInfo_select(kPacket.m_dwUserUID, kPacket.m_mapCharacterPos)) {
			START_LOG(cerr, L" 캐릭터 배치 오픈중 아지트 캐릭터 정보 다시 가져오기 실패.. AgitUID : " << kPacket.m_dwUserUID) << END_LOG;
		}
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_06, END_PROC);
	}
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket.m_dwUserUID)
		<< BUILD_LOG(kPacket.m_nCharType) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_OPEN_AGIT_CHARACTER_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq)
{
	KSetAgitCharPosAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_vecCharPos = kPacket_.m_vecCharPos;

	SET_ERROR(ERR_UNKNOWN);

	std::vector<KSimpleAgitCharPos>::iterator vit;
	for (vit = kPacket_.m_vecCharPos.begin(); vit != kPacket_.m_vecCharPos.end(); ++vit) {
		// 아지트 캐릭터 배치
		if (false == AgitCharInfo_pos_update(kPacket_.m_dwAgitUID, vit->m_nCharType, vit->m_nPosX, vit->m_nPosY)) {
			START_LOG(cerr, L"아지트 캐릭터 배치 DB처리 실패.. AgitUID : " << kPacket_.m_dwAgitUID << L", CharType : " << vit->m_nCharType) << END_LOG;
			SET_ERR_GOTO(ERR_AGIT_CHARACTER_14, END_PROC);
		}
	}

	// 배치캐릭터 최대 배치수 초과된 경우, 에러값 설정해서 전달
	if (kPacket_.m_bCharPosOver) {
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_18, END_PROC);
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_vecCharPos.size())
		<< BUILD_LOG(kPacket_.m_bCharPosOver) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_CHARACTER_POS_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq)
{
	KSetAgitCharMotionAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_nMotionID = kPacket_.m_nMotionID;

	SET_ERROR(ERR_UNKNOWN);

	// 캐릭터 모션 변경.
	if (false == ASGAAgitCharInfo_motion_update(kPacket_.m_dwAgitUID, kPacket_.m_nCharType, kPacket_.m_nMotionID)) {
		START_LOG(cerr, L"캐릭터 모션 변경 DB처리 실패.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_15, END_PROC);
	}
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_nMotionID) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_CHARACTER_MOTION_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_COORDI_REQ, KSetAgitCharCoordiReq)
{
	KSetAgitCharCoordiAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_mapCoordi.clear();

	SET_ERROR(ERR_UNKNOWN);

	std::map<int, KAgitCharacterPos> mapList;
	if (false == AgiteCharCoordi_select(kPacket_.m_dwAgitUID, mapList)) {
		START_LOG(cerr, L"DB에서 아지트 배치캐릭터 코디정보 읽어오기 실패.. AgitUID : " << kPacket_.m_dwAgitUID) << END_LOG;
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_16, END_PROC);
	}
	else {
		// 코디 디자인 아이템 가져오기.
		LIF(AgitCharDesignCoordi_select(kPacket_.m_dwAgitUID, mapList));
		KAgitCharacterPos& kAgitCharPos = mapList[kPacket_.m_nCharType];
		// 장착해제할 코디 설정
		std::map<DWORD, KSimpleCoordiItem>::iterator mitCoordi;
		for (mitCoordi = kAgitCharPos.m_mapCoordi.begin(); mitCoordi != kAgitCharPos.m_mapCoordi.end(); ++mitCoordi) {
			if (kPacket_.m_mapSlotCoordiItemUID.find(mitCoordi->first) == kPacket_.m_mapSlotCoordiItemUID.end()) {
				mitCoordi->second.m_ItemID = 0;
				kPacket_.m_mapSlotCoordiItemUID[mitCoordi->first] = mitCoordi->second;

				START_LOG(clog, L"장착해제할 배치캐릭터 코디아이템 값 설정.. AgitUID : " << kPacket_.m_dwAgitUID)
					<< BUILD_LOG(mitCoordi->first) // SlotID
					<< BUILD_LOG(mitCoordi->second.m_ItemID) // ItemID
					<< BUILD_LOG(mitCoordi->second.m_ItemUID) // ItemUID
					<< BUILD_LOG(mitCoordi->second.m_DesignCoordiID) // DesignCoordiID
					<< END_LOG;
			}
		}
	}

	if (false == kPacket_.m_mapSlotCoordiItemUID.empty()) {
		// 배치캐릭터 코디 갱신
		std::map<DWORD, KSimpleCoordiItem >::iterator mit;
		for (mit = kPacket_.m_mapSlotCoordiItemUID.begin(); mit != kPacket_.m_mapSlotCoordiItemUID.end(); ++mit) {
			// 아이템ID가 0이면 장착 해제. 아니면 장착
			bool bEquip = (mit->second.m_ItemID == 0 ? false : true);

			int nRet = -99;
			nRet = AgiteCharCoordi_merge(kPacket_.m_dwAgitUID, kPacket_.m_nCharType, mit->first, mit->second.m_ItemUID, bEquip);

			_LOG_SUCCESS(nRet == 0 || nRet == -1, L"배치캐릭터 코디 갱신 DB처리 결과 : " << nRet)
				<< BUILD_LOG(kPacket_.m_dwAgitUID)
				<< BUILD_LOG(kPacket_.m_nCharType)
				<< BUILD_LOG(mit->first) // SlogID
				<< BUILD_LOG(mit->second.m_ItemID) // ItemID
				<< BUILD_LOG(mit->second.m_ItemUID) // ItemUID
				<< BUILD_LOG(mit->second.m_DesignCoordiID) // DesignCoordiID
				<< BUILD_LOG(bEquip) << END_LOG;

			if (nRet != 0) {
				SET_ERR_GOTO(ERR_AGIT_CHARACTER_16, END_PROC);
			}

			if (bEquip) {
				kPacket.m_kCharInfo.m_mapCoordi[mit->first] = mit->second;
			}
		}
	}

	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_mapSlotCoordiItemUID.size())
		<< BUILD_LOG(kPacket.m_kCharInfo.m_mapCoordi.size()) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_CHARACTER_COORDI_ACK);
}

_INT_IMPL_ON_FUNC(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq)
{
	KSetAgitCharSpeechAck kPacket;
	kPacket.m_nOK = -99;
	kPacket.m_dwAgitUID = kPacket_.m_dwAgitUID;
	kPacket.m_kCharInfo.m_nCharType = kPacket_.m_nCharType;
	kPacket.m_kCharInfo.m_mapSpeech = kPacket_.m_mapSpeech;

	std::map<USHORT, std::wstring>::iterator mit;
	for (mit = kPacket_.m_mapSpeech.begin(); mit != kPacket_.m_mapSpeech.end(); ++mit) {
		boost::replace_all(mit->second, L"'", L"''");
	}

	SET_ERROR(ERR_UNKNOWN);

	// 배치캐릭터 대사 설정
	bool bRet = AgitCharInfo_dialogic_update(kPacket_.m_dwAgitUID,
		kPacket_.m_nCharType,
		kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_DEFAULT],
		kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLOSE],
		kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLICK]);

	if (false == bRet) {
		START_LOG(cerr, L"배치캐릭터 대사설정 DB처리실패.. AgitUID : " << kPacket_.m_dwAgitUID)
			<< BUILD_LOG(kPacket_.m_nCharType)
			<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_DEFAULT])
			<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLOSE])
			<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLICK]) << END_LOG;
		SET_ERR_GOTO(ERR_AGIT_CHARACTER_17, END_PROC);
	}
	SET_ERROR(NET_OK);

END_PROC:
	kPacket.m_nOK = NetError::GetLastNetError();

	_LOG_SUCCESS(kPacket.m_nOK == NetError::NET_OK, L"Ret : " << NetError::GetLastNetErrMsg())
		<< BUILD_LOG(dwUID_)
		<< BUILD_LOG(kPacket_.m_dwAgitUID)
		<< BUILD_LOG(kPacket_.m_nCharType)
		<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_DEFAULT])
		<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLOSE])
		<< BUILD_LOG(kPacket_.m_mapSpeech[KAgitCharacterPos::SPT_CLICK]) << END_LOG;

	QUEUING_ACK_TO_USER(EAG_SET_AGIT_CHARACTER_SPEECH_ACK);
}