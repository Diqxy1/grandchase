#include "AGNetLayer.h"
#include "AgitManager.h"
#include <dbg/dbg.hpp>
#include <boost/mem_fn.hpp>
#include "Lua/KLuaManager.h"
#include "NetError.h"
#include "AgentEvent.h"
#include "AGDBLayer.h"
#include "AgentServer.h"
#include <boost/bind/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

namespace
{
	class KPageSort
	{
	public:
		bool operator() (const KAgitPtr ptA, const KAgitPtr ptB)
		{
			if (ptA == NULL || ptB == NULL) return false;
			else if (ptA->GetRoomID() == ptB->GetRoomID()) return true;
			else if (ptA->GetRoomID() < ptB->GetRoomID()) return true;
			else if (ptA->GetRoomID() > ptB->GetRoomID()) return false;
			return false;
		}
	};
}

ImplementSingleton(KAgitManager);
NiImplementRootRTTI(KAgitManager);
ImplOstmOperatorW2A(KAgitManager);
ImplToStringW(KAgitManager)
{
	KLocker lock1(m_csAgit);
	KLocker lock2(m_csEventQueue);
	KLocker lock3(m_csPage);
	KLocker lock4(m_csPageSort);
	KLocker lock5(m_csRankAgit);

	START_TOSTRINGW
		<< TOSTRINGW(m_bAgitAvailable)
		<< TOSTRINGW(m_mapAgit.size())
		<< TOSTRINGW(m_dwShutdownTimeLimit)
		<< TOSTRINGW(m_queEvent.size())
		<< TOSTRINGWb(m_bPageChange)
		<< TOSTRINGW(m_nLastReservedRoomID)
		<< TOSTRINGW(m_vecAgitRoomID.size())
		<< TOSTRINGW(m_vecPageAll.size())
		<< TOSTRINGW(m_vecPageOpen.size())
		<< TOSTRINGW(m_vecPageNotFull.size())
		<< TOSTRINGW(m_vecPageOpenNotFull.size())
		<< TOSTRINGW(m_mapPageAll.size())
		<< TOSTRINGW(m_mapPageSortByOpen.size())
		<< TOSTRINGW(m_mapPageSortByNotFull.size())
		<< TOSTRINGW(m_mapPageSortByOpenNotFull.size())
		<< TOSTRINGW(m_mapRankPage.size())
		<< TOSTRINGW(m_mapRankList.size());
	return stm_;
}

KAgitManager::KAgitManager(void)
	:m_bPageChange(false)
	, m_nLastReservedRoomID(0)
	, m_kPageTimer(1000 * 3)
	, m_kMinTimer(1000 * 60)
	, m_dwShutdownTimeLimit(1000 * 60 * 3)
	, m_bAgitAvailable(true)
	, m_bDestroyAgit(false)
	, m_usDBUpdateHour(8)
{
	m_tmToday = CTime::GetCurrentTime();
	m_tmLastCheck = CTime(2000, 1, 1, 8, 0, 0);
}

KAgitManager::~KAgitManager(void)
{
}

void KAgitManager::UpdateAgitRankFromDB()
{
	SiKAGDBLayer()->QueueingID(KAgentEvent::EAG_AGIT_RANK_LIST_NOT, L"", 0);
	START_LOG(clog, L"인기아지트 랭크 DB 요청") << END_LOG;
}

bool KAgitManager::LoadScript()
{
	_JIF(LoadConfig(), return false);
	_JIF(LoadMapInfo(), return false);
	_JIF(LoadFlowerPot(), return false);
	_JIF(LoadFairyTree(), return false);

	START_LOG(cerr, L"Load Agit Script Complete.") << END_LOG;
	return true;
}

bool KAgitManager::LoadConfig()
{
	KLuaManager kLuaMng;
	KAgitConfig kConfig;
	USHORT usDBUpdateHour = 8;

	_JIF(kLuaMng.DoFile("InitAgitConfig.lua") == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxCashingAgitInfo", kConfig.m_nMaxCashingAgitInfo) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxAgitNameLength", kConfig.m_usMaxAgitNameLength) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxProfileLength", kConfig.m_usMaxProfileLength) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxUserCount", kConfig.m_usMaxUserCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxPasswordLength", kConfig.m_usMaxPasswordLength) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxInvenSize", kConfig.m_nMaxInvenSize) == S_OK, return false);
	_JIF(kLuaMng.GetValue("SellRewardFlowerCount", kConfig.m_usSellRewardFlowerCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxFavoriteCount", kConfig.m_nMaxFavoriteCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("ReserveRoomID", kConfig.m_usReserveRoomID) == S_OK, return false);
	_JIF(kLuaMng.GetValue("BuildPageTick", kConfig.m_dwBuildPageTick) == S_OK, return false);
	_JIF(kLuaMng.GetValue("AgitCountOnePage", kConfig.m_usAgitCountOnePage) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("BrownieID") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID BrownieID = 0;
		if (kLuaMng.GetValue(i, BrownieID) != S_OK) break;
		if (BrownieID == 0) continue;
		kConfig.m_setBrownieID.insert(BrownieID);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.BeginTable("TutorialItem") == S_OK, return false);
	for (int i = 1; ; ++i) {
		KDropItemInfo kDopItem;
		kDopItem.m_ItemID = 0;
		kDopItem.m_nDuration = -1;
		kDopItem.m_nPeriod = -1;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue(1, kDopItem.m_ItemID) == S_OK, return false);
		_JIF(kLuaMng.GetValue(2, kDopItem.m_nDuration) == S_OK, return false);
		_JIF(kLuaMng.GetValue(3, kDopItem.m_nPeriod) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (kDopItem.m_ItemID == 0) continue;
		kConfig.m_vecTutorialItem.push_back(kDopItem);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.GetValue("RankDBUpdateHour", usDBUpdateHour) == S_OK, return false);
	_JIF(kLuaMng.GetValue("MaxGuestbookMsgLenth", kConfig.m_usMaxGuestbookMsgLenth) == S_OK, return false);
	_JIF(kLuaMng.GetValue("GuestbookWriteTerm", kConfig.m_dwGuestbookWriteTerm) == S_OK, return false);
	_JIF(kLuaMng.GetValue("GuestBookPageSize", kConfig.m_usGuestBookPageSize) == S_OK, return false);
	_JIF(kLuaMng.GetValue("GuestbookWritePerDay", kConfig.m_usGuestbookWritePerDay) == S_OK, return false);
	_JIF(kLuaMng.GetValue("TrainingMaterialItemID", kConfig.m_TrainingMaterialItemID) == S_OK, return false);
	_JIF(kLuaMng.GetValue("TrainingItemID", kConfig.m_TrainingItemID) == S_OK, return false);
	_JIF(kLuaMng.GetValue("TrainingExpRatio", kConfig.m_fTrainingExpRatio) == S_OK, return false);
	_JIF(kLuaMng.GetValue("TrainingMaterialRegenTerm", kConfig.m_usTrainingMaterialRegenTerm) == S_OK, return false);
	_JIF(kLuaMng.GetValue("TrainingMaterialLimit", kConfig.m_usTrainingMaterialLimit) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("ObjectItemID") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID ObjectItemID = 0;
		if (kLuaMng.GetValue(i, ObjectItemID) != S_OK) break;
		if (ObjectItemID == 0) continue;
		kConfig.m_vecObjectItemID.push_back(ObjectItemID);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.GetValue("CharacterOpenCost", kConfig.m_usCharacterOpenCost) == S_OK, return false);
	_JIF(kLuaMng.GetValue("CharacterSpeechLength", kConfig.m_usCharacterSpeechLength) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("NotResellObjectItemID") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID ItemID = 0;
		if (kLuaMng.GetValue(i, ItemID) != S_OK) break;
		if (ItemID == 0) continue;
		kConfig.m_setNotResellObjectItemID.insert(ItemID);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.BeginTable("ObjectItemInvenLimit") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID ItemID = 0;
		int nLimitCount = 0;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue(1, ItemID) == S_OK, return false);
		_JIF(kLuaMng.GetValue(2, nLimitCount) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (ItemID == 0 || nLimitCount <= 0) continue;
		kConfig.m_mapObjectItemInvenLimit[ItemID] = nLimitCount;
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	START_LOG(cerr, L"Load Agit Config..")
		<< BUILD_LOG(kConfig.m_nMaxCashingAgitInfo)
		<< BUILD_LOG(kConfig.m_usMaxAgitNameLength)
		<< BUILD_LOG(kConfig.m_usMaxProfileLength)
		<< BUILD_LOG(kConfig.m_usMaxUserCount)
		<< BUILD_LOG(kConfig.m_usMaxPasswordLength)
		<< BUILD_LOG(kConfig.m_nMaxInvenSize)
		<< BUILD_LOG(kConfig.m_usSellRewardFlowerCount)
		<< BUILD_LOG(kConfig.m_nMaxFavoriteCount)
		<< BUILD_LOG(kConfig.m_usReserveRoomID)
		<< BUILD_LOG(kConfig.m_dwBuildPageTick)
		<< BUILD_LOG(kConfig.m_usAgitCountOnePage)
		<< BUILD_LOG(kConfig.m_setBrownieID.size())
		<< BUILD_LOG(kConfig.m_vecTutorialItem.size())
		<< BUILD_LOG(usDBUpdateHour)
		<< BUILD_LOG(kConfig.m_usMaxGuestbookMsgLenth)
		<< BUILD_LOG(kConfig.m_dwGuestbookWriteTerm)
		<< BUILD_LOG(kConfig.m_usGuestBookPageSize)
		<< BUILD_LOG(kConfig.m_usGuestbookWritePerDay)
		<< BUILD_LOG(kConfig.m_TrainingMaterialItemID)
		<< BUILD_LOG(kConfig.m_TrainingItemID)
		<< BUILD_LOG(kConfig.m_fTrainingExpRatio)
		<< BUILD_LOG(kConfig.m_usTrainingMaterialRegenTerm)
		<< BUILD_LOG(kConfig.m_usTrainingMaterialLimit)
		<< BUILD_LOG(kConfig.m_vecObjectItemID.size())
		<< BUILD_LOG(kConfig.m_usCharacterOpenCost)
		<< BUILD_LOG(kConfig.m_usCharacterSpeechLength)
		<< BUILD_LOG(kConfig.m_setNotResellObjectItemID.size())
		<< BUILD_LOG(kConfig.m_mapObjectItemInvenLimit.size()) << END_LOG;

	{
		KLocker lock(m_csConfig);
		m_kAgitConfig = kConfig;
	}
	{
		KLocker lock(m_csRankAgit);
		m_usDBUpdateHour = usDBUpdateHour;
	}

	InitRoomID();
	SendAgitPacket(KAgentEvent::EAG_AGIT_CONFIG_NOT, kConfig);
	m_kPageTimer.SetInterval(kConfig.m_dwBuildPageTick);
	return true;
}

bool KAgitManager::LoadMapInfo()
{
	KLuaManager kLuaMng;
	std::map<DWORD, KAgitMap> mapList;
	mapList.clear();

	_JIF(kLuaMng.DoFile("InitAgitMap.lua") == S_OK, return false);

	_JIF(kLuaMng.BeginTable("MapInfo") == S_OK, return false);
	for (int i = 1; ; ++i) {
		KAgitMap kInfo;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue("MapID", kInfo.m_dwMapID) == S_OK, return false);
		_JIF(kLuaMng.GetValue("Price", kInfo.m_nPrice) == S_OK, return false);
		_JIF(kLuaMng.GetValue("MaxFlowerPot", kInfo.m_usMaxFlowerPot) == S_OK, return false);
		_JIF(kLuaMng.GetValue("MaxCharPos", kInfo.m_usMaxCharPos) == S_OK, return false);
		_JIF(kLuaMng.GetValue("MapFile", kInfo.m_strFileName) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		_JIF(mapList.insert(std::map<DWORD, KAgitMap>::value_type(kInfo.m_dwMapID, kInfo)).second, return false);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	START_LOG(cerr, L"Load Agit MapInfo..")
		<< BUILD_LOG(mapList.size()) << END_LOG;

	{
		KLocker lock(m_csMap);
		m_mapMapInfo = mapList;
	}

	SendAgitPacket(KAgentEvent::EAG_AGIT_MAP_NOT, mapList);
	return true;
}

bool KAgitManager::LoadFlowerPot()
{
	KLuaManager kLuaMng;
	KFlowerPotConfig kConfig;

	_JIF(kLuaMng.DoFile("InitFlowerPot.lua") == S_OK, return false);

	_JIF(kLuaMng.GetValue("InvenLimitSeedCount", kConfig.m_nInvenLimitSeedCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("DailyMaxSeedCount", kConfig.m_nDailyMaxSeedCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("DailyInitSeedCount", kConfig.m_nDailyInitSeedCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("SeedDropTime", kConfig.m_nSeedDropTime) == S_OK, return false);
	_JIF(kLuaMng.GetValue("SeedDropCount", kConfig.m_nSeedDropCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("AgitCashPotCount", kConfig.m_usMaxAgitCashPotCount) == S_OK, return false);
	_JIF(kLuaMng.GetValue("InvenLimitFlowerCount", kConfig.m_nInvenLimitFlowerCount) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("FlowerPot") == S_OK, return false);
	for (int i = 1; ; ++i) {
		KFlowerPotOption kInfo;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue("SeedType", kInfo.m_nSeedType) == S_OK, return false);
		_JIF(kLuaMng.GetValue("NeedSeedCount", kInfo.m_nNeedSeedCount) == S_OK, return false);
		_JIF(kLuaMng.GetValue("GrowTime", kInfo.m_nGrowTime) == S_OK, return false);
		_JIF(kLuaMng.GetValue("FlowerCount", kInfo.m_usFlowerCount) == S_OK, return false);
		_JIF(kLuaMng.GetValue("BonusCount", kInfo.m_usBonusCount) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (kInfo.m_nSeedType < KFlowerPot::ST_FAST || kInfo.m_nSeedType > KFlowerPot::ST_BEST) {
			START_LOG(cerr, L"정의되지 않은 SeedType : " << kInfo.m_nSeedType) << END_LOG;
			continue;
		}

		_JIF(kConfig.m_mapFlowerPotOption.insert(std::map<int, KFlowerPotOption>::value_type(kInfo.m_nSeedType, kInfo)).second, return false);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.GetValue("SeedItemID", kConfig.m_SeedItemID) == S_OK, return false);
	_JIF(kLuaMng.GetValue("FlowerItemID", kConfig.m_FlowerItemID) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("NormalPotItemID") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID ItemID = 0;
		if (kLuaMng.GetValue(i, ItemID) != S_OK) break;
		LIF(kConfig.m_setNormalPotItemID.insert(ItemID).second);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.BeginTable("CashPotItemID") == S_OK, return false);
	for (int i = 1; ; ++i) {
		GCITEMID ItemID = 0;
		if (kLuaMng.GetValue(i, ItemID) != S_OK) break;
		LIF(kConfig.m_setCashPotItemID.insert(ItemID).second);
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	START_LOG(cerr, L"Load FlowerPot Info..")
		<< BUILD_LOG(kConfig.m_nInvenLimitSeedCount)
		<< BUILD_LOG(kConfig.m_nDailyMaxSeedCount)
		<< BUILD_LOG(kConfig.m_nSeedDropTime)
		<< BUILD_LOG(kConfig.m_nSeedDropCount)
		<< BUILD_LOG(kConfig.m_usMaxAgitCashPotCount)
		<< BUILD_LOG(kConfig.m_nInvenLimitFlowerCount)
		<< BUILD_LOG(kConfig.m_mapFlowerPotOption.size())
		<< BUILD_LOG(kConfig.m_SeedItemID)
		<< BUILD_LOG(kConfig.m_FlowerItemID)
		<< BUILD_LOG(kConfig.m_setNormalPotItemID.size())
		<< BUILD_LOG(kConfig.m_setCashPotItemID.size()) << END_LOG;

	{
		KLocker lock(m_csFlowerPot);
		m_kFlowerPotConfig = kConfig;
	}

	SendAgitPacket(KAgentEvent::EAG_AGIT_FLOWER_NOT, kConfig);
	return true;
}

bool KAgitManager::LoadFairyTree()
{
	KLuaManager kLuaMng;
	KFairyTreeConfig kConfig;
	std::map<int, KLottery> mapFruitDecision;

	_JIF(kLuaMng.DoFile("InitFairyTree.lua") == S_OK, return false);

	_JIF(kLuaMng.BeginTable("LvExp") == S_OK, return false);
	for (int i = 1; ; ++i) {
		int nLv = 0;
		DWORD dwExp = 0;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue(1, nLv) == S_OK, return false);
		_JIF(kLuaMng.GetValue(2, dwExp) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (nLv <= 0) continue;
		kConfig.m_vecLvExp.push_back(std::make_pair(nLv, dwExp));
		kConfig.m_mapLvExp[nLv] = dwExp;
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.GetValue("AddExpFirstDay", kConfig.m_dwAddExpFirstDay) == S_OK, return false);
	_JIF(kLuaMng.GetValue("AddExpTime", kConfig.m_dwAddExpTime) == S_OK, return false);
	_JIF(kLuaMng.GetValue("WateringTimeTerm", kConfig.m_dwWateringTimeTerm) == S_OK, return false);
	_JIF(kLuaMng.GetValue("DecExpDay", kConfig.m_dwDecExpDay) == S_OK, return false);
	_JIF(kLuaMng.GetValue("DecDayTerm", kConfig.m_dwDecDayTerm) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("FruitBuffByLv") == S_OK, return false);
	for (int i = 1; ; ++i) {
		int nLv = 0;
		float fBuffRate = 0;

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue(1, nLv) == S_OK, return false);
		_JIF(kLuaMng.GetValue(2, fBuffRate) == S_OK, return false);
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (nLv <= 0) continue;
		kConfig.m_mapFruitBuffByLv[nLv] = fBuffRate;
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	_JIF(kLuaMng.GetValue("BuffAll_Rate", kConfig.m_fBuffAllRate) == S_OK, return false);
	_JIF(kLuaMng.GetValue("ExpGp_Rate", kConfig.m_fExpGpRate) == S_OK, return false);

	_JIF(kLuaMng.BeginTable("FruitTypeByLv") == S_OK, return false);
	for (int i = 1; ; ++i) {
		int nLv = 0;
		std::map<int, float> mapFruitRatio; // FruitType, Ratio
		mapFruitRatio.clear();

		if (kLuaMng.BeginTable(i) != S_OK) break;
		_JIF(kLuaMng.GetValue(1, nLv) == S_OK, return false);

		_JIF(kLuaMng.BeginTable("Ratio") == S_OK, return false);
		for (int j = 1; ; j += 2) {
			int nFruitType = 0;
			float fRatio = 0.f;
			if (kLuaMng.GetValue(j, nFruitType) != S_OK) break;
			if (kLuaMng.GetValue(j + 1, fRatio) != S_OK) break;
			mapFruitRatio[nFruitType] = fRatio;
			START_LOG(clog, L"Get Fruit Type : " << nFruitType << L", Ratio : " << fRatio) << END_LOG;
		}
		_JIF(kLuaMng.EndTable() == S_OK, return false);

		_JIF(kLuaMng.EndTable() == S_OK, return false);

		if (nLv <= 0) continue;
		kConfig.m_mapFruitTypeByLv[nLv] = mapFruitRatio;

		// 나무레벨별 열매확률 설정
		KLottery kLottery;
		std::map<int, float>::iterator mitFruit;
		for (mitFruit = mapFruitRatio.begin(); mitFruit != mapFruitRatio.end(); ++mitFruit) {
			kLottery.AddCase(mitFruit->first, mitFruit->second * 100.f);
		}
		mapFruitDecision[nLv] = kLottery;
		START_LOG(clog, L"Set Fruit By Lv.. Lv : " << nLv << L", TotalProb() : " << kLottery.GetTotalProb()) << END_LOG;
	}
	_JIF(kLuaMng.EndTable() == S_OK, return false);

	START_LOG(cerr, L"Load FairyTree Config..")
		<< BUILD_LOG(kConfig.m_vecLvExp.size())
		<< BUILD_LOG(kConfig.m_mapLvExp.size())
		<< BUILD_LOG(kConfig.m_dwAddExpFirstDay)
		<< BUILD_LOG(kConfig.m_dwAddExpTime)
		<< BUILD_LOG(kConfig.m_dwWateringTimeTerm)
		<< BUILD_LOG(kConfig.m_dwDecExpDay)
		<< BUILD_LOG(kConfig.m_dwDecDayTerm)
		<< BUILD_LOG(kConfig.m_mapFruitBuffByLv.size())
		<< BUILD_LOG(kConfig.m_fBuffAllRate)
		<< BUILD_LOG(kConfig.m_fExpGpRate)
		<< BUILD_LOG(kConfig.m_mapFruitTypeByLv.size())
		<< BUILD_LOG(mapFruitDecision.size()) << END_LOG;

	{
		KLocker lock(m_csFairyTree);
		m_kFairyTreeConfig = kConfig;
		m_mapFruitDecision.swap(mapFruitDecision);
	}

	SendAgitPacket(KAgentEvent::EAG_AGIT_FAIRY_TREE_NOT, kConfig);
	return true;
}

KAgitPtr KAgitManager::GetAgit(IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csAgit);

	std::map<DWORD, KAgitPtr>::iterator mit;
	mit = m_mapAgit.find(dwAgitUID_);
	if (mit == m_mapAgit.end()) {
		return KAgitPtr();
	}

	return mit->second;
}

void KAgitManager::EraseAgit(IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csAgit);

	std::map<DWORD, KAgitPtr>::iterator mit;
	mit = m_mapAgit.find(dwAgitUID_);
	if (mit != m_mapAgit.end()) {
		mit->second->UpdateCashingData();
		m_mapAgit.erase(mit);
		SiKAGConnector()->SendPacket(KAgentEvent::EAGS_CLEAR_AGIT_SID_NOT, dwAgitUID_);
	}

	START_LOG(clog, L"아지트 정보 삭제.. SID 초기화 알림. AgitUID : " << dwAgitUID_) << END_LOG;
}

void KAgitManager::Tick()
{
	DistributeEvent();
	EachTick();
	IntervalTick();
}

void KAgitManager::DistributeEvent()
{
	std::deque<KIntEventPtr> queEvent;
	{
		KLocker lock(m_csEventQueue);
		m_queEvent.swap(queEvent);
	}

	if (queEvent.empty()) return;

	std::deque<KIntEventPtr>::iterator qit;
	for (qit = queEvent.begin(); qit != queEvent.end(); ++qit) {
		KAgitPtr spAgit = GetAgit((*qit)->m_dwSenderUID);
		if (spAgit == NULL) {
			continue;
		}

		KEventPtr spEvent(new KAgentEvent);
		spEvent->m_usEventID = (*qit)->m_usEventID;
		spEvent->m_kbuff = (*qit)->m_kbuff;

		spAgit->QueueingEvent(spEvent);
	}
}

void KAgitManager::EachTick()
{
	KLocker lock(m_csAgit);

	std::map<DWORD, KAgitPtr>::iterator mit;
	for (mit = m_mapAgit.begin(); mit != m_mapAgit.end(); ++mit) {
		if (mit->second == NULL) {
			continue;
		}

		mit->second->Tick();
	}
}

void KAgitManager::IntervalTick()
{
	if (m_kPageTimer.CheckTime()) {
		if (IsPageChange()) {
			SetPageChange(false);
			BuildPage();
			BuildPageByOption();
		}
	}

	if (m_kMinTimer.CheckTime()) {
		CTime tmCurrent = CTime::GetCurrentTime();
		time_t tmTime = KncUtil::TimeToInt(tmCurrent);
		bool bDayChange = false;
		bool bDestroyAgit = IsDestroyAgit();

		GCITEMID TrainingObjItemID = SiKAgitManager()->TrainingItemID();
		std::vector<DWORD> vecClearAgitList;
		vecClearAgitList.clear();

		// 날짜 변경 체크
		if (m_tmToday.GetYear() < tmCurrent.GetYear() ||
			(m_tmToday.GetYear() == tmCurrent.GetYear() && m_tmToday.GetMonth() < tmCurrent.GetMonth()) ||
			(m_tmToday.GetYear() == tmCurrent.GetYear() && m_tmToday.GetMonth() == tmCurrent.GetMonth() && m_tmToday.GetDay() < tmCurrent.GetDay())) {
			bDayChange = true;
			m_tmToday = tmCurrent;
		}

		{
			KLocker lock(m_csAgit);
			std::map<DWORD, KAgitPtr>::iterator mit;
			for (mit = m_mapAgit.begin(); mit != m_mapAgit.end(); ++mit) {
				if (mit->second == NULL) {
					continue;
				}

				if (bDayChange) {
					// 방문자 수 갱신 요청
					QueueingEvent(KAgentEvent::EAG_UPDATE_AGIT_VISITER_COUNT_NOT, mit->first, tmTime);
					// 요정의나무 상태확인 요청
					QueueingEvent(KAgentEvent::EAG_UPDATE_FAIRY_TREE_REQ, mit->first, tmTime);
				}

				if (bDestroyAgit) {
					// 삭제예정 아지트 수집
					if (mit->second->IsReserveDestroy()) {
						vecClearAgitList.push_back(mit->first);
					}
				}

				if (mit->second->GetUserNum() > 0) {
					// 훈련소 체크
					QueueingEvent(KAgentEvent::EAG_CHECK_TRAINING_OBJ_NOT, mit->first, TrainingObjItemID);
				}
			}

			// 삭제 실행
			if (false == vecClearAgitList.empty()) {
				std::vector<DWORD>::iterator vit;
				for (vit = vecClearAgitList.begin(); vit != vecClearAgitList.end(); ++vit) {
					EraseAgit(*vit);
				}
				START_LOG(clog, L"아지트 삭제 실행.. vecClearAgitList.size() : " << vecClearAgitList.size() << L", m_mapAgit.size() : " << m_mapAgit.size()) << END_LOG;
			}
		}

		// 랭킹 검사시간인가?
		if (tmCurrent.GetHour() == GetRankDBUpdateHour()) {
			if (m_tmLastCheck.GetYear() <= tmCurrent.GetYear() && m_tmLastCheck.GetMonth() <= tmCurrent.GetMonth() && m_tmLastCheck.GetDay() < tmCurrent.GetDay()) {
				UpdateAgitRankFromDB();
				m_tmLastCheck = tmCurrent;

				START_LOG(clog, L"인기 아지트 갱신 요청.. CheckTime(hour) : " << GetRankDBUpdateHour()) << END_LOG;
			}
		}

		// 아지트 가용상태 체크
		bool bIsAvailable = (GetCashingSize() < GetMaxCashingAgitInfo() ? true : false);
		SetAgitAvailable(bIsAvailable);

		// Master에 현재 상태 전달
		PAIR_INT_BOOL prSlaveState(SiKAgentServer()->GetServerID(), bIsAvailable);
		SiKAGConnector()->SendPacket(KAgentEvent::EAGS_UPDATE_SLAVE_STATE_NOT, prSlaveState);
	}
}

void KAgitManager::BuildPage()
{
	bool IsOpen = false; // 공개방인가?
	bool IsNotFull = false; // 인원이 꽉차지않은 방인가?

	std::vector<KAgitPtr> vecAll;
	std::vector<KAgitPtr> vecOpen;
	std::vector<KAgitPtr> vecNotFull;
	std::vector<KAgitPtr> vecOpenNotFull;

	{
		KLocker lock(m_csAgit);

		std::map<DWORD, KAgitPtr>::iterator mit;
		for (mit = m_mapAgit.begin(); mit != m_mapAgit.end(); ++mit) {
			if (mit->first == 0) {
				continue;
			}

			if (mit->second == NULL) {
				continue;
			}

			if (false == mit->second->IsPageRegister()) {
				continue;
			}

			// 옵션별로 분류
			IsOpen = (mit->second->GetOpenType() == KAgitInfo::OT_OPEN ? true : false);
			IsNotFull = (mit->second->IsFull() == false ? true : false);

			if (IsOpen) {
				vecOpen.push_back(mit->second);
			}

			if (IsNotFull) {
				vecNotFull.push_back(mit->second);
			}

			if (IsOpen && IsNotFull) {
				vecOpenNotFull.push_back(mit->second);
			}

			vecAll.push_back(mit->second);
		}
	}

	{
		KLocker lock(m_csPage);
		m_vecPageAll.swap(vecAll);
		m_vecPageOpen.swap(vecOpen);
		m_vecPageNotFull.swap(vecNotFull);
		m_vecPageOpenNotFull.swap(vecOpenNotFull);
	}
}

void KAgitManager::BuildPageByOption()
{
	std::map<int, KSerBuffer> mapAll;
	std::map<int, KSerBuffer> mapOpen;
	std::map<int, KSerBuffer> mapNotFull;
	std::map<int, KSerBuffer> mapOpenNotFull;

	{
		KLocker lock(m_csPage);
		BuildPageBufferPtr(m_vecPageAll, mapAll);
		BuildPageBufferPtr(m_vecPageOpen, mapOpen);
		BuildPageBufferPtr(m_vecPageNotFull, mapNotFull);
		BuildPageBufferPtr(m_vecPageOpenNotFull, mapOpenNotFull);
	}

	{
		KLocker lock(m_csPageSort);
		m_mapPageAll.swap(mapAll);
		m_mapPageSortByOpen.swap(mapOpen);
		m_mapPageSortByNotFull.swap(mapNotFull);
		m_mapPageSortByOpenNotFull.swap(mapOpenNotFull);
	}
}

void KAgitManager::BuildPageBufferPtr(IN std::vector<KAgitPtr> vecAgitPtr_, OUT std::map<int, KSerBuffer>& mapPage_)
{
	mapPage_.clear();

	if (vecAgitPtr_.empty()) {
		return;
	}

	std::vector<KAgitPageInfo> vecPage;
	vecPage.clear();

	USHORT usAgitCountOnePage = GetAgitCountOnePage();
	if (usAgitCountOnePage == 0) {
		return;
	}

	std::vector<KAgitPtr>::iterator vit;
	for (vit = vecAgitPtr_.begin(); vit != vecAgitPtr_.end(); ++vit) {
		// 개수 만족되면 페이지 빌드
		if (vecPage.size() >= usAgitCountOnePage) {
			KSerBuffer kBuff;
			if (CompressPage(vecPage, kBuff)) {
				mapPage_[mapPage_.size() + 1] = kBuff;
			}
			vecPage.clear();
		}

		// 페이지 정보 채우기
		KAgitPageInfo kPage;
		(*vit)->GetPageInfo(kPage);
		vecPage.push_back(kPage);
	}

	// 남은 개수도 페이지 빌드
	if (false == vecPage.empty()) {
		KSerBuffer kBuff;
		if (CompressPage(vecPage, kBuff)) {
			mapPage_[mapPage_.size() + 1] = kBuff;
		}
	}
}

void KAgitManager::BuildPageBuffer(IN std::map<DWORD, KAgitPageInfo> mapInfo_, OUT std::map<int, KSerBuffer>& mapPage_)
{
	mapPage_.clear();

	if (mapInfo_.empty()) {
		return;
	}

	std::vector<KAgitPageInfo> vecPage;
	vecPage.clear();

	USHORT usAgitCountOnePage = GetAgitCountOnePage();
	if (usAgitCountOnePage == 0) {
		return;
	}

	std::map<DWORD, KAgitPageInfo>::iterator mit;
	for (mit = mapInfo_.begin(); mit != mapInfo_.end(); ++mit) {
		// 개수 만족되면 페이지 빌드
		if (vecPage.size() >= usAgitCountOnePage) {
			KSerBuffer kBuff;
			if (CompressPage(vecPage, kBuff)) {
				mapPage_[mapPage_.size() + 1] = kBuff;
			}
			vecPage.clear();
		}

		// 페이지 정보 채우기
		vecPage.push_back(mit->second);
	}

	// 남은 개수도 페이지 빌드
	if (false == vecPage.empty()) {
		KSerBuffer kBuff;
		if (CompressPage(vecPage, kBuff)) {
			mapPage_[mapPage_.size() + 1] = kBuff;
		}
	}
}

void KAgitManager::BuildOnePageBuffer(IN std::vector<KAgitPageInfo> vecInfo_, IN OUT std::map<int, KSerBuffer>& mapPageBuff_)
{
	if (vecInfo_.empty()) {
		mapPageBuff_.clear();
	}

	KSerBuffer kBuffer;
	kBuffer.Clear();
	__LIF(CompressPage(vecInfo_, kBuffer));
	mapPageBuff_[1] = kBuffer;
}

void KAgitManager::BuildOnePageBufferPtr(IN std::vector<KAgitPtr> vecAgitPtr_, IN OUT KAgitPageList& kPageList_)
{
	if (vecAgitPtr_.empty()) {
		kPageList_.m_usPageNum = 0;
		kPageList_.m_usMaxPageNum = 0;
	}

	std::vector<KAgitPageInfo> vecPage;
	vecPage.clear();

	bool bPageBuild = false;
	USHORT usCurrentPageNum = 0;

	USHORT usAgitCountOnePage = GetAgitCountOnePage();
	if (usAgitCountOnePage == 0) {
		return;
	}

	std::vector<KAgitPtr>::iterator vit;
	for (vit = vecAgitPtr_.begin(); vit != vecAgitPtr_.end(); ++vit) {
		// 개수 만족되면 페이지 빌드
		if (vecPage.size() >= usAgitCountOnePage) {
			++usCurrentPageNum;

			// 원하는 페이지 인가?
			if (kPageList_.m_usPageNum == usCurrentPageNum) {
				__LIF(CompressPage(vecPage, kPageList_.m_buffCompList));
				bPageBuild = true;
			}

			vecPage.clear();
		}

		// 페이지 정보 채우기
		KAgitPageInfo kPage;
		(*vit)->GetPageInfo(kPage);
		vecPage.push_back(kPage);
	}

	// 페이지 채우기
	if (!bPageBuild && !vecPage.empty()) {
		__LIF(CompressPage(vecPage, kPageList_.m_buffCompList));
		bPageBuild = true;
		++usCurrentPageNum;
	}

	kPageList_.m_usMaxPageNum = usCurrentPageNum;

	if (false == bPageBuild) {
		START_LOG(cwarn, L"요청한 페이지를 찾을 수 없음(생성실패)")
			<< BUILD_LOG(kPageList_.m_usPageNum)
			<< BUILD_LOG(kPageList_.m_usMaxPageNum)
			<< BUILD_LOG(vecAgitPtr_.size()) << END_LOG;
		kPageList_.m_usPageNum = 0;
	}
}

bool KAgitManager::CompressPage(IN std::vector<KAgitPageInfo> vecPage_, OUT KSerBuffer& kBuff_)
{
	kBuff_.Clear();

	if (vecPage_.empty()) {
		return false;
	}

	KSerializer ks;
	ks.BeginWriting(&kBuff_);
	ks.Put(vecPage_);
	ks.EndWriting();
	kBuff_.Compress();

	return true;
}

void KAgitManager::GetPageBuffer(IN OUT KAgitPageList& kPageList_)
{
	KLocker lock(m_csPageSort);

	if (kPageList_.m_bOpen && kPageList_.m_bNotFull) {
		_LIF(GetCurrentPage(m_mapPageSortByOpenNotFull, kPageList_));
	}
	else if (kPageList_.m_bOpen) {
		_LIF(GetCurrentPage(m_mapPageSortByOpen, kPageList_));
	}
	else if (kPageList_.m_bNotFull) {
		_LIF(GetCurrentPage(m_mapPageSortByNotFull, kPageList_));
	}
	else {
		_LIF(GetCurrentPage(m_mapPageAll, kPageList_));
	}
}

bool KAgitManager::GetCurrentPage(IN std::map<int, KSerBuffer>& mapList_, IN OUT KAgitPageList& kPageList_)
{
	if (mapList_.empty()) {
		kPageList_.m_usPageNum = 0;
		kPageList_.m_usMaxPageNum = 0;
	}

	kPageList_.m_usMaxPageNum = mapList_.size();

	std::map<int, KSerBuffer>::iterator mit;
	mit = mapList_.find(kPageList_.m_usPageNum);
	if (mit == mapList_.end()) {
		START_LOG(clog, L"요청한 페이지가 없음. MaxPage : " << kPageList_.m_usMaxPageNum)
			<< BUILD_LOG(kPageList_.m_bOpen)
			<< BUILD_LOG(kPageList_.m_bNotFull)
			<< BUILD_LOG(kPageList_.m_usMinUser)
			<< BUILD_LOG(kPageList_.m_usPageNum) << END_LOG;

		kPageList_.m_usPageNum = std::min<USHORT>(1, kPageList_.m_usMaxPageNum);
		mit = mapList_.find(kPageList_.m_usPageNum);
		if (mit == mapList_.end()) {
			return false;
		}
	}

	kPageList_.m_buffCompList = mit->second;
	return true;
}

void KAgitManager::GetCustomPage(IN OUT KAgitPageList& kPageList_)
{
	KLocker lock(m_csPage);

	if (kPageList_.m_bOpen && kPageList_.m_bNotFull) {
		BuildCutomPage(m_vecPageOpenNotFull, kPageList_);
	}
	else if (kPageList_.m_bOpen) {
		BuildCutomPage(m_vecPageOpen, kPageList_);
	}
	else if (kPageList_.m_bNotFull) {
		BuildCutomPage(m_vecPageNotFull, kPageList_);
	}
	else {
		BuildCutomPage(m_vecPageAll, kPageList_);
	}

	START_LOG(clog, L"Get CustomPage.")
		<< BUILD_LOG(kPageList_.m_bOpen)
		<< BUILD_LOG(kPageList_.m_bNotFull)
		<< BUILD_LOG(m_vecPageAll.size())
		<< BUILD_LOG(m_vecPageOpen.size())
		<< BUILD_LOG(m_vecPageNotFull.size())
		<< BUILD_LOG(m_vecPageOpenNotFull.size()) << END_LOG;
}

void KAgitManager::BuildCutomPage(IN std::vector<KAgitPtr> vecAgitPtr_, IN OUT KAgitPageList& kPageList_)
{
	std::vector<KAgitPtr> vecSort;

	std::vector<KAgitPtr>::iterator vit;
	for (vit = vecAgitPtr_.begin(); vit != vecAgitPtr_.end(); ++vit) {
		if ((*vit)->GetUserNum() < kPageList_.m_usMinUser) {
			continue;
		}

		vecSort.push_back(*vit);
	}

	BuildOnePageBufferPtr(vecSort, kPageList_);
}

bool KAgitManager::Add(IN const KAgitInfo& kInfo_)
{
	KAgitPtr spAgit = GetAgit(kInfo_.m_dwUID);
	if (spAgit != NULL) {
		spAgit->SetInfo(kInfo_); // 갱신만 하자.
		START_LOG(clog, L"이미 등록된 유저 아지트 정보. UserUId : " << kInfo_.m_dwUID) << END_LOG;
		return true;
	}

	DWORD dwTRServerIP = 0;
	USHORT usTRServerPort = 0;
	LIF(SiKAGNetLayer()->GetTRServerAddress(kInfo_.m_dwUID, dwTRServerIP, usTRServerPort));

	DWORD dwURServerIP = 0;
	USHORT usURServerPort = 0;
	LIF(SiKAGNetLayer()->GetURServerAddress(kInfo_.m_dwUID, dwURServerIP, usURServerPort));

	spAgit.reset(new KAgit);
	spAgit->SetInfo(kInfo_);
	spAgit->SetTRServerIPPort(dwTRServerIP, usTRServerPort);
	spAgit->SetURServerIPPort(dwURServerIP, usURServerPort);
	spAgit->SetOwnerConnect(true, KncUtil::TimeToInt(CTime::GetCurrentTime()));

	// 캐싱데이터 사이즈 보다 작으면 추가
	if (IsAgitAvailable()) {
		KLocker lock(m_csAgit);
		m_mapAgit[kInfo_.m_dwUID] = spAgit;
		START_LOG(clog, L"아지트 정보 추가.. AgitUID : " << kInfo_.m_dwUID) << END_LOG;
	}
	else {
		// 넣기 실패하면 실패한 아지트UID값 마스터에 전달.
		SiKAGConnector()->SendPacket(KAgentEvent::EAGS_CLEAR_AGIT_SID_NOT, kInfo_.m_dwUID);
		return false;
	}

	// 가용포화상태 체크 마스터에 전달
	if (GetCashingSize() >= GetMaxCashingAgitInfo()) {
		SetAgitAvailable(false);
		PAIR_INT_BOOL prSlaveState(SiKAgentServer()->GetServerID(), IsAgitAvailable());
		SiKAGConnector()->SendPacket(KAgentEvent::EAGS_UPDATE_SLAVE_STATE_NOT, prSlaveState);

		START_LOG(cerr, L"아지트정보 가용 포화상태.. SID : " << SiKAgentServer()->GetServerID())
			<< BUILD_LOG(GetMaxCashingAgitInfo())
			<< BUILD_LOG(GetCashingSize()) << END_LOG;
	}

	in_addr in; // for Log
	in.S_un.S_addr = dwTRServerIP;

	START_LOG(clog, L"아지트 등록. UserUID : " << kInfo_.m_dwUID << L", TCP IP : " << inet_ntoa(in) << L" / Port : " << usTRServerPort) << END_LOG;
	return true;
}

bool KAgitManager::GetCurrentMapInfo(IN const DWORD& dwMapID_, OUT KAgitMap& kInfo_)
{
	KLocker lock(m_csMap);

	std::map<DWORD, KAgitMap>::const_iterator cmit;
	cmit = m_mapMapInfo.find(dwMapID_);
	if (cmit == m_mapMapInfo.end()) {
		return false;
	}

	kInfo_ = cmit->second;
	return true;
}

bool KAgitManager::GetFlowerPotOption(IN const int& nSeedType_, OUT KFlowerPotOption& kOption_)
{
	KLocker lock(m_csFlowerPot);

	std::map<int, KFlowerPotOption>::const_iterator cmit;
	cmit = m_kFlowerPotConfig.m_mapFlowerPotOption.find(nSeedType_);
	if (cmit == m_kFlowerPotConfig.m_mapFlowerPotOption.end()) {
		return false;
	}

	kOption_ = cmit->second;
	return true;
}

bool KAgitManager::IsNormalPotItemID(IN const GCITEMID& PotItemID_)
{
	KLocker lock(m_csFlowerPot);
	return (m_kFlowerPotConfig.m_setNormalPotItemID.find(PotItemID_) != m_kFlowerPotConfig.m_setNormalPotItemID.end());
}

bool KAgitManager::IsCashPotItemID(IN const GCITEMID& PotItemID_)
{
	KLocker lock(m_csFlowerPot);
	return (m_kFlowerPotConfig.m_setCashPotItemID.find(PotItemID_) != m_kFlowerPotConfig.m_setCashPotItemID.end());
}

void KAgitManager::InitRoomID()
{
	USHORT usReserveSize = GetAgitRoomIdSize() + GetReserveRoomID();
	__LIF(ReserveRoomID(usReserveSize));
}

bool KAgitManager::ReserveRoomID(IN const USHORT& usSize_)
{
	size_t szRoomIdSize = GetAgitRoomIdSize();
	USHORT usCurrentLastRoomID = 0;

	KLocker lock(m_csPage);

	if (szRoomIdSize >= usSize_) {
		START_LOG(clog, L"이미 해당 RoomID 이상이 할당되어 있음.")
			<< BUILD_LOG(usSize_)
			<< BUILD_LOG(szRoomIdSize) << END_LOG;

		m_nLastReservedRoomID = szRoomIdSize;
		return false;
	}

	for (USHORT i = szRoomIdSize + 1; i <= usSize_; ++i) {
		m_vecAgitRoomID.push_back(std::make_pair(i, 0));
	}

	m_nLastReservedRoomID = m_vecAgitRoomID.size();

	if (false == m_vecAgitRoomID.empty()) {
		usCurrentLastRoomID = m_vecAgitRoomID.rbegin()->first;
	}

	START_LOG(clog, L"아지트 페이지 RoomID 추가할당됨. 요청된 총size : " << usSize_)
		<< BUILD_LOG(m_nLastReservedRoomID)
		<< BUILD_LOG(usCurrentLastRoomID) << END_LOG;

	return true;
}

USHORT KAgitManager::FindEmptyRoomID()
{
	KLocker lock(m_csPage);

	std::vector<PAIR_INT_DWORD>::iterator vit;
	vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
		boost::bind(&PAIR_INT_DWORD::second, boost::placeholders::_1) == 0);

	// 비어있는 방번호가 없는 경우, 방번호 추가생성
	if (vit == m_vecAgitRoomID.end()) {
		USHORT usReserveSize = GetAgitRoomIdSize() + GetReserveRoomID();
		__LIF(ReserveRoomID(usReserveSize));

		vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
			boost::bind(&PAIR_INT_DWORD::second, boost::placeholders::_1) == 0);
		if (vit == m_vecAgitRoomID.end()) {
			START_LOG(cerr, L"비어있는 방번호를 찾을 수 없음. (추가생성실패)") << END_LOG;
			return 0;
		}
	}

	return vit->first;
}

bool KAgitManager::RegisterPage(IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csPage);

	std::vector<PAIR_INT_DWORD>::iterator vit;
	KAgitPtr spAgit;
	USHORT usRoomID = 0;

	spAgit = SiKAgitManager()->GetAgit(dwAgitUID_);
	_JIF(spAgit != NULL, return false);

	usRoomID = FindEmptyRoomID();
	if (usRoomID == 0) {
		return false;
	}

	// 이미 할당되어 있는게 있으면 해제시켜주자.
	vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
		boost::bind(&PAIR_INT_DWORD::second, boost::placeholders::_1) == dwAgitUID_);
	if (vit == m_vecAgitRoomID.end()) {
		vit->second = 0;
	}

	vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
		boost::bind(&PAIR_INT_DWORD::first, boost::placeholders::_1) == usRoomID);
	if (vit == m_vecAgitRoomID.end()) {
		// RoomID가 없을경우, 추가 생성
		USHORT usReserveSize = GetAgitRoomIdSize() + GetReserveRoomID();
		__LIF(ReserveRoomID(usReserveSize));

		vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
			boost::bind(&PAIR_INT_DWORD::first, boost::placeholders::_1) == usRoomID);
		if (vit == m_vecAgitRoomID.end()) {
			START_LOG(cerr, L"등록할 방번호를 찾을 수 없음. (추가생성실패)") << END_LOG;
			return false;
		}
	}

	// 해당 룸 번호에 AgitUID 할당
	vit->second = dwAgitUID_;
	SetPageChange(true);

	// 아지트 정보 갱신
	spAgit->SetRoomID(usRoomID);
	spAgit->SetPageRegister(true);
	return true;
}

bool KAgitManager::UnregisterPage(IN const USHORT& usRoomID_, IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csPage);

	std::vector<PAIR_INT_DWORD>::iterator vit;
	KAgitPtr spAgit;

	vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
		boost::bind(&PAIR_INT_DWORD::first, boost::placeholders::_1) == usRoomID_);
	if (vit == m_vecAgitRoomID.end()) {
		// 해제하려는 RoomID를 못찾았다면, AgitUID로 찾아서 해제
		vit = std::find_if(m_vecAgitRoomID.begin(), m_vecAgitRoomID.end(),
			boost::bind(&PAIR_INT_DWORD::second, boost::placeholders::_1) == dwAgitUID_);
		_JIF(vit != m_vecAgitRoomID.end(), return false);
	}

	// 페이지 등록 해제
	vit->second = 0;
	SetPageChange(true);

	// 아지트 정보 갱신
	spAgit = SiKAgitManager()->GetAgit(dwAgitUID_);
	_JIF(spAgit != NULL, return false);
	spAgit->SetRoomID(0);
	spAgit->SetPageRegister(false);
	return true;
}

int KAgitManager::GetFairyTreeLv(IN std::vector<PAIR_INT_DWORD>& vecLvExp_, IN const DWORD& dwTotalExp_)
{
	std::vector<PAIR_INT_DWORD>::iterator vit;
	vit = std::find_if(vecLvExp_.begin(), vecLvExp_.end(),
		boost::bind(&PAIR_INT_DWORD::second, boost::placeholders::_1) > dwTotalExp_);

	if (vit != vecLvExp_.begin()) {
		--vit;
	}

	return vit->first;
}

int KAgitManager::GetTodayFruitType(IN const int& nFairyTreeLv_)
{
	KLocker lock(m_csFairyTree);

	std::map<int, KLottery>::iterator mit;
	mit = m_mapFruitDecision.find(nFairyTreeLv_);
	if (mit == m_mapFruitDecision.end()) {
		START_LOG(clog, L"해당 나무레벨에 해당하는 열매 정보가 없음.. Lv : " << nFairyTreeLv_) << END_LOG;
		return KFairyTreeConfig::FBT_NONE;
	}

	int nFruitType = mit->second.Decision();
	if (nFruitType < KFairyTreeConfig::FBT_NONE || nFruitType >= KFairyTreeConfig::FBT_MAX) {
		START_LOG(cwarn, L"정의되지않은 열매타입.. FruitType : " << nFruitType) << END_LOG;
		nFruitType = KFairyTreeConfig::FBT_NONE;
	}

	START_LOG(clog, L"일일 나무열매 결정.. Lv : " << nFairyTreeLv_ << L", FruitType : " << nFruitType << L", GetTotalProb : " << mit->second.GetTotalProb()) << END_LOG;
	return nFruitType;
}

float KAgitManager::GetFruitBuffRate(IN const int& nFairyTreeLv_, IN const int& nFruitType_)
{
	KFairyTreeConfig kConfig;
	GetFairyTreeConfig(kConfig);

	if (nFruitType_ == KFairyTreeConfig::FBT_BUFF_ALL) {
		return kConfig.m_fBuffAllRate;
	}

	if (nFruitType_ == KFairyTreeConfig::FBT_EXP_GP) {
		return kConfig.m_fExpGpRate;
	}

	std::map<int, float>::iterator mit;
	mit = kConfig.m_mapFruitBuffByLv.find(nFairyTreeLv_);
	if (mit == kConfig.m_mapFruitBuffByLv.end()) {
		START_LOG(clog, L"해당하는 버프 증가량이 없음.. Lv : " << nFairyTreeLv_) << END_LOG;
		return 0.f;
	}

	START_LOG(clog, L"나무열매 버프 증가량.. Lv : " << nFairyTreeLv_ << L", BuffRate : " << mit->second) << END_LOG;
	return mit->second;
}

void KAgitManager::SetRankAgit(IN const std::map<int, KSerBuffer>& mapPage_, IN const std::map<int, std::vector<KRankAgit> >& mapRankList_)
{
	KLocker lock(m_csRankAgit);
	m_mapRankPage = mapPage_;
	m_mapRankList = mapRankList_;
}

void KAgitManager::GetRankAgit(OUT std::map<int, KSerBuffer>& mapPage_)
{
	KLocker lock(m_csRankAgit);
	mapPage_ = m_mapRankPage;
}

bool KAgitManager::IsRankAgit(IN const DWORD& dwAgitUID_)
{
	KLocker lock(m_csRankAgit);
	return (m_mapRankList.find(dwAgitUID_) != m_mapRankList.end());
}

void KAgitManager::DumpAgit(IN const DWORD& dwAgitUID_)
{
	KAgitPtr spAgit;
	spAgit = GetAgit(dwAgitUID_);
	if (spAgit == NULL) {
		START_LOG(cerr, L"등록되지않은 아지트.. AgitUID : " << dwAgitUID_) << END_LOG;
		return;
	}

	KAgitInfo kInfo;
	spAgit->GetInfo(kInfo);

	START_LOG(cerr, L"--- Dump Agit (UID:" << dwAgitUID_ << L") ---" << dbg::endl
		<< L" AgitName : " << kInfo.m_strName << dbg::endl
		<< L" MapID : " << kInfo.m_dwMapID << dbg::endl
		<< L" OpenType : " << (int)kInfo.m_ucOpenType << dbg::endl
		<< L" InviteType : " << (int)kInfo.m_ucInviteType << dbg::endl
		<< L" MaxUser : " << (int)kInfo.m_ucMaxUser << dbg::endl
		<< L" TodayVisiter : " << kInfo.m_nTodayVisiter << dbg::endl
		<< L" InitTodayVisiter : " << kInfo.m_nInitTodayVisiter << dbg::endl
		<< L" TotalVisiter : " << kInfo.m_nTotalVisiter << dbg::endl
		<< L" Profile : " << kInfo.m_kProfile.m_strProfile << dbg::endl
		<< L" LastCleaning : " << (LPCWSTR)CTime(kInfo.m_tmLastCleaning).Format(KNC_TIME_FORMAT) << dbg::endl
		<< L" Password : " << kInfo.m_strPassword << dbg::endl
		<< L" ObjectPos size : " << kInfo.m_vecObjectPos.size()) << END_LOG;

	std::vector<KAgitObjectPos>::iterator vitObj;
	for (vitObj = kInfo.m_vecObjectPos.begin(); vitObj != kInfo.m_vecObjectPos.end(); ++vitObj) {
		START_LOG(cerr, L"ItemID : " << vitObj->m_ID
			<< L",ItemUID : " << vitObj->m_UID
			<< L",PosX : " << vitObj->m_nPosX
			<< L",PosY : " << vitObj->m_nPosY) << END_LOG;
	}

	START_LOG(cerr, L" CharacterPos size : " << kInfo.m_mapCharacterPos.size()) << END_LOG;
	std::map<int, KAgitCharacterPos>::iterator mitChar;
	for (mitChar = kInfo.m_mapCharacterPos.begin(); mitChar != kInfo.m_mapCharacterPos.end(); ++mitChar) {
		START_LOG(cerr, L"CharType : " << mitChar->second.m_nCharType
			<< L",MotionID : " << mitChar->second.m_nMotionID
			<< L",PosX : " << mitChar->second.m_nPosX
			<< L",PosY : " << mitChar->second.m_nPosY
			<< L",Costume size : " << mitChar->second.m_mapCoordi.size()
			<< L",Speech size : " << mitChar->second.m_mapSpeech.size()) << END_LOG;

		START_LOG(cerr, L" Costume ItemID : ") << END_LOG;

		std::map<DWORD, KSimpleCoordiItem>::iterator mitCoordi;
		for (mitCoordi = mitChar->second.m_mapCoordi.begin(); mitCoordi != mitChar->second.m_mapCoordi.end(); ++mitCoordi) {
			START_LOG(cerr, L"[" << mitCoordi->first << L"](" << mitCoordi->second.m_ItemID << L"," << mitCoordi->second.m_ItemUID << mitCoordi->second.m_DesignCoordiID << L"), ") << END_LOG;
		}

		START_LOG(cerr, L" Speech Msg : ") << END_LOG;

		std::map<USHORT, std::wstring>::iterator mitMsg;
		for (mitMsg = mitChar->second.m_mapSpeech.begin(); mitMsg != mitChar->second.m_mapSpeech.end(); ++mitMsg) {
			START_LOG(cerr, L"(" << mitMsg->first << L" : " << mitMsg->second << L"), ") << END_LOG;
		}
	}

	START_LOG(cerr, L" IsPageRegister : " << spAgit->IsPageRegister() << L", RoomID : " << spAgit->GetRoomID()) << END_LOG;

	std::map<DWORD, std::vector<DWORD> > mapUserServer;
	spAgit->GetUserServerList(mapUserServer);

	START_LOG(cerr, L" UserServer List (size:" << mapUserServer.size() << L")") << END_LOG;
	std::map<DWORD, std::vector<DWORD> >::iterator mitServer;
	for (mitServer = mapUserServer.begin(); mitServer != mapUserServer.end(); ++mitServer) {
		START_LOG(cerr, L"ServerUID : " << mitServer->first << L", User size : " << mitServer->second.size() << L" (") << END_LOG;
		std::vector<DWORD>::iterator vitUser;
		for (vitUser = mitServer->second.begin(); vitUser != mitServer->second.end(); ++vitUser) {
			START_LOG(cerr, *vitUser << L",") << END_LOG;
		}
		START_LOG(cerr, L")") << END_LOG;
	}

	START_LOG(cerr, L" IsOwnerConnect() : " << spAgit->IsOwnerConnect()) << END_LOG;

	START_LOG(cerr, L"--- End Dump ---") << END_LOG;
}

void KAgitManager::UpdateCashingData()
{
	KLocker lock(m_csAgit);

	START_LOG(cerr, L"서버 종료. 상주된 아지트수 : " << m_mapAgit.size()) << END_LOG;

	std::map<DWORD, KAgitPtr>::iterator mit;
	for (mit = m_mapAgit.begin(); mit != m_mapAgit.end(); ++mit) {
		if (mit->first == 0) {
			continue;
		}

		if (mit->second == NULL) {
			continue;
		}

		mit->second->Tick();
		mit->second->UpdateCashingData();
	}

	KDBLayer* pkDBLayer = SiKAgentServer()->GetDBLayer();
	JIF(pkDBLayer);

	DWORD dwBeginTick = ::GetTickCount();
	while (pkDBLayer->GetQueueSize() != 0 && ::GetTickCount() - dwBeginTick < m_dwShutdownTimeLimit) {
		::Sleep(500);
	}

	if (pkDBLayer->GetQueueSize() != 0) {
		START_LOG(cerr, L"캐싱 정보를 모두 저장하지 못하고 종료")
			<< L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
			<< L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
			<< L"Agit count : " << m_mapAgit.size() << dbg::endl
			<< L"non-processed DB event : " << pkDBLayer->GetQueueSize() << END_LOG;
		return;
	}

	START_LOG(cerr, L"모든 아지트 캐싱 데이터 저장.")
		<< L"Elapsed Time : " << ::GetTickCount() - dwBeginTick << dbg::endl
		<< L"Time Limit : " << m_dwShutdownTimeLimit << dbg::endl
		<< L"Agit count : " << m_mapAgit.size() << END_LOG;
}
//GetAgitManager():TestFunc_CreateAgit(100)
void KAgitManager::TestFunc_CreateAgit(IN const int& nCount_)
{
	int nSize = 0;
	{
		KLocker lock(m_csAgit);
		nSize = (int)m_mapAgit.size();
	}

	KAgitInfo kDummy;
	for (int i = nSize + 1; i <= nSize + nCount_; ++i) {
		kDummy.m_dwUID = i;
		{
			std::wstringstream stm;
			stm << i << L"테스트 아지트";
			kDummy.m_strName = stm.str().c_str();
			stm.clear();
		}
		kDummy.m_kProfile.m_strProfile = L"일이삼사오육칠팔구십십일십이십삼십사십오십육십칠십팔십구이십";
		kDummy.m_ucOpenType = (i % 2 > 0 ? KAgitInfo::OT_OPEN : KAgitInfo::OT_CLOSE);
		kDummy.m_ucMaxUser = (i % 6);

		for (int j = 0; j < 30; ++j) {
			KAgitObjectPos kTemp;
			kDummy.m_vecObjectPos.push_back(kTemp);
		}

		for (int k = 0; k < 20; ++k) {
			KAgitCharacterPos kTemp;
			KSimpleCoordiItem kSimpleCoordiItem;

			for (int l = 0; l < 9; ++l) {
				kTemp.m_mapCoordi[l] = kSimpleCoordiItem;
			}

			kTemp.m_mapSpeech[0] = L"134141441414241424141431432412";
			kTemp.m_mapSpeech[1] = L"asfdasfasdfsdafasfdasfadsfadsfs";
			kTemp.m_mapSpeech[2] = L"4j32h4kj3h2j4h32j4hjk32h4jk23h4k";

			kDummy.m_mapCharacterPos[k] = kTemp;
		}

		kDummy.m_strPassword = L"일이삼사";

		LIF(Add(kDummy));

		/*
		KAgitPtr spAgit = GetAgit( kDummy.m_dwUID );
		if ( spAgit ) {
			std::map<DWORD,KAgitUserInfo> mapUser;
			int nCount = ::rand() % 5 + 1;
			for ( int j = 1 ; j <= nCount ; ++j ) {
				std::wstringstream stm;
				stm << L"유저" << j;

				KAgitUserInfo kInfo;
				kInfo.m_dwUID = j;
				kInfo.m_strNick = stm.str().c_str();
				stm.clear();
				mapUser.insert( std::make_pair(j, kInfo) );
			}

			{
				std::wstringstream stm;
				stm << L"주인" << i;
				spAgit->SetOwnerNickname( stm.str().c_str() );
				spAgit->SetUserList(mapUser);
				spAgit->SetPageRegister( true );
			}
		}
		*/
	}
	SetPageChange(true);

	{
		KLocker lock(m_csAgit);
		nSize = (int)m_mapAgit.size();
	}

	START_LOG(clog, L"size of KAgitInfo : " << sizeof(KAgitInfo) << L", size of KAgit : " << sizeof(KAgit) << L", PtrSize : " << nSize) << END_LOG;
}

void KAgitManager::TestFunc_BuildDummyPage(IN const int& nCount_, OUT std::map<int, KSerBuffer>& mapPage_)
{
	mapPage_.clear();

	KLocker lock(m_csAgit);

	std::vector<KAgitPtr> vecPtr;
	vecPtr.clear();

	std::map<DWORD, KAgitPtr>::iterator mit;
	for (mit = m_mapAgit.begin(); mit != m_mapAgit.end(); ++mit) {
		if ((int)vecPtr.size() >= nCount_) {
			break;
		}

		vecPtr.push_back(mit->second);
	}

	BuildPageBufferPtr(vecPtr, mapPage_);
}

void KAgitManager::BuildGuestBookBuffer(IN const std::map<int, KGuestMsg>& mapList_, OUT KSerBuffer& kBuff_)
{
	kBuff_.Clear();
	std::map<int, KGuestMsg> mapTemp;
	mapTemp.clear();

	std::map<int, KGuestMsg>::const_reverse_iterator cmit;
	for (cmit = mapList_.rbegin(); cmit != mapList_.rend(); ++cmit) {
		if (mapTemp.size() > GetGuestBookPageSize()) {
			break;
		}

		mapTemp[cmit->first] = cmit->second;
	}

	if (false == mapTemp.empty()) {
		KSerializer ks;
		ks.BeginWriting(&kBuff_);
		ks.Put(mapTemp);
		ks.EndWriting();
		kBuff_.Compress();
	}

	START_LOG(clog, L"Build GuestBook Buffer..")
		<< BUILD_LOG(mapList_.size())
		<< BUILD_LOG(mapTemp.size())
		<< BUILD_LOG(GetGuestBookPageSize()) << END_LOG;
}

bool KAgitManager::IsWriteEnable(IN const DWORD& dwUserUID_, IN const std::map<int, KGuestMsg>& mapList_)
{
	CTime tmToday(CTime::GetCurrentTime());
	int nWriteCount = 0;

	std::map<int, KGuestMsg>::const_iterator cmit;
	for (cmit = mapList_.begin(); cmit != mapList_.end(); ++cmit) {
		CTime tmCurrent(cmit->second.m_tmDate);

		if (tmCurrent.GetYear() == tmToday.GetYear() && tmCurrent.GetMonth() == tmToday.GetMonth() && tmCurrent.GetDay() == tmToday.GetDay()) {
			if (cmit->second.m_dwUserUID == dwUserUID_) {
				++nWriteCount;
			}
		}

		if (nWriteCount >= GetGuestbookWritePerDay()) {
			break;
		}
	}

	_LOG_CONDITION(nWriteCount < GetGuestbookWritePerDay(), clog, cwarn, L"일일 낙서장 글쓰기 제한.. UserUID : " << dwUserUID_)
		<< BUILD_LOG(GetGuestbookWritePerDay())
		<< BUILD_LOG(nWriteCount) << END_LOG;

	return (nWriteCount < GetGuestbookWritePerDay());
}

bool KAgitManager::FairyTreeGrowChange(IN const time_t& tmToday_, IN OUT KFairyTree& kFairyTree_)
{
	KFairyTreeConfig kConfig;
	SiKAgitManager()->GetFairyTreeConfig(kConfig);

	// 최소값 보정
	time_t tmDefault = KncUtil::TimeToInt(CTime(2000, 1, 1, 0, 0, 0, 0));

	if (kFairyTree_.m_tmLastGrowTime < tmDefault) {
		kFairyTree_.m_tmLastGrowTime = tmDefault;
		START_LOG(cerr, L"나무 마지막 자란 시간 최소값 보정..")
			<< BUILD_LOG(kFairyTree_.m_tmLastGrowTime)
			<< BUILD_LOG(tmDefault) << END_LOG;
	}

	if (kFairyTree_.m_tmLastDecreaseTime < tmDefault) {
		kFairyTree_.m_tmLastDecreaseTime = tmDefault;
		START_LOG(cerr, L"나무 마지막 경험치값 보정..")
			<< BUILD_LOG(kFairyTree_.m_tmLastDecreaseTime)
			<< BUILD_LOG(tmDefault) << END_LOG;
	}

	CTime tmToday = CTime(tmToday_);
	CTime tmCurrent = CTime(tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay(), 0, 0, 0);

	CTime tmLastGrow = CTime(kFairyTree_.m_tmLastGrowTime);
	CTime tmPast = CTime(tmLastGrow.GetYear(), tmLastGrow.GetMonth(), tmLastGrow.GetDay(), 0, 0, 0);
	tmPast += CTimeSpan(1, 0, 0, 0);

	CTime tmDecreaseTime = CTime(kFairyTree_.m_tmLastDecreaseTime);
	CTime tmDecrease = CTime(tmDecreaseTime.GetYear(), tmDecreaseTime.GetMonth(), tmDecreaseTime.GetDay(), 0, 0, 0);

	// 마지막으로 물 준 시간과 마지막으로 경험치 보정한 시간값과 비교하여 더 높은 시간으로 체크한다.
	tmPast = std::max<CTime>(tmPast, tmDecrease);

	CTimeSpan tsTimeSpan = tmCurrent - tmPast;
	DWORD dwDayTerm = 0;

	if (tsTimeSpan.GetTotalHours() > 0) {
		dwDayTerm = static_cast<DWORD>(tsTimeSpan.GetTotalHours() / 24);
	}

	if (dwDayTerm == 0) {
		START_LOG(clog, L"요정의나무 성장치 변경없음..")
			<< BUILD_LOG(kFairyTree_.m_nLv)
			<< BUILD_LOG(kFairyTree_.m_dwExp) << END_LOG;
		return false;
	}

	// 요정의나무 하루이상 물을 안주면 성장치 감소
	DWORD dwDecCount = dwDayTerm / std::max<DWORD>(kConfig.m_dwDecDayTerm, 1);
	DWORD dwDecExp = kConfig.m_dwDecExpDay * dwDecCount;
	DWORD dwCurruntDecExp = std::min<DWORD>(dwDecExp, kFairyTree_.m_dwExp);
	kFairyTree_.m_dwExp -= dwCurruntDecExp;
	kFairyTree_.m_nLv = SiKAgitManager()->GetFairyTreeLv(kConfig.m_vecLvExp, kFairyTree_.m_dwExp);
	kFairyTree_.m_tmLastDecreaseTime = KncUtil::TimeToInt(tmCurrent); // 오늘치 감소했으니, 날짜 갱신

	START_LOG(clog, L"요정의나무 성장치 감소..")
		<< BUILD_LOG(kFairyTree_.m_nLv)
		<< BUILD_LOG(kFairyTree_.m_dwExp)
		<< BUILD_LOG(kConfig.m_dwDecExpDay)
		<< BUILD_LOG(dwDayTerm)
		<< BUILD_LOG(kConfig.m_dwDecDayTerm)
		<< BUILD_LOG(dwDecExp)
		<< BUILD_LOG(dwCurruntDecExp) << END_LOG;

	return true;
}