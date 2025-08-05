#include "DropSystem.h"
#include "GCEnum.h"
#include <KncUtilLua.h> // luabind::object
#include "Log4.h"
//FILE_NAME_FOR_LOG

void KDropInfo::InitFromLuaTable(const luabind::object& table_)
{
	Init(); // 컨테이너 초기화 하고 있음.
	JIF(table_.type() == LUA_TTABLE);

	m_iTestGroup = KncUtil::GetLuaData(table_, "TestGroup", static_cast<int>(-1));
	m_kDropItemDetail.m_iNum = KncUtil::GetLuaData(table_, "NumItem", static_cast<int>(1));
	m_kDropItemDetail.m_iPeriod = KncUtil::GetLuaData(table_, "ItemPeriod", static_cast<int>(1));
	m_fProb = KncUtil::GetLuaData(table_, "Probability", 0.0f);
	m_kDropItemDetail.m_iConditionItem = KncUtil::GetLuaData(table_, "ConditionItem", static_cast<int>(-1));
	m_kDropItemDetail.m_iNumRoomMember = KncUtil::GetLuaData(table_, "NumRoomMember", static_cast<int>(0));
	m_kDropItemDetail.m_iDropReceiver = KncUtil::GetLuaData(table_, "Receiver", static_cast<int>(KDropItemDetail::DR_ALL));
	m_kDropItemDetail.m_iDropType = KncUtil::GetLuaData(table_, "DropType", static_cast<int>(KDropItemDetail::DT_NORMAL));
	m_kDropItemDetail.m_iBeginLv = KncUtil::GetLuaData(table_, "BeginLv", static_cast<int>(-1));
	m_kDropItemDetail.m_iEndLv = KncUtil::GetLuaData(table_, "EndLv", static_cast<int>(-1));

	luabind::object::iterator itOuter;
	luabind::object::iterator itInner;

	for (itOuter = table_.begin(); itOuter != table_.end(); ++itOuter)
	{
		std::string key = luabind::object_cast<std::string>(itOuter.key());

		if (key == "ItemList")
		{
			luabind::object ItemListTable = table_["ItemList"];
			for (itInner = ItemListTable.begin(); itInner != ItemListTable.end(); ++itInner)
			{
				int index = luabind::object_cast<int>(itInner.key()) - 1;     // lua : 1-base
				m_kDropItemDetail.m_mapItemList[index] = luabind::object_cast<int>(*itInner);
			}
		}
		else if (key == "MonsterAffectedProb")
		{
			luabind::object MonsterAffectedProbTable = table_["MonsterAffectedProb"];
			for (itInner = MonsterAffectedProbTable.begin(); itInner != MonsterAffectedProbTable.end(); ++itInner)
			{
				int index = luabind::object_cast<int>(itInner.key()) - 1;     // lua : 1-base
				m_mapMonsterAffectedProb[index] = luabind::object_cast<float>(*itInner);
			}
		}
	}
}

// KDropMode

KDropMode::KDropMode(const KDropMode& t_)
{
	*this = t_;
}

void KDropMode::AddDropInfo(const luabind::object& table_)
{
	KDropInfo kDropInfo;

	kDropInfo.InitFromLuaTable(table_);

	std::map<int, KDropInfo>::iterator itDropInfo;

	itDropInfo = m_mapDropInfoSet.find(m_iNextID);
	if (itDropInfo != m_mapDropInfoSet.end())
	{
		START_LOG(cwarn, L"추가하려는 DropInfo의 번호가 이미 존재함.")
			<< BUILD_LOG(m_iNextID)
			<< END_LOG;
	}

	m_mapDropInfoSet[m_iNextID] = kDropInfo;
	m_kActionDecider.AddAction(kDropInfo.m_iTestGroup, m_iNextID, kDropInfo.m_fProb);

	m_iNextID++;
}

void KDropMode::ProcessDrop(OUT std::vector<KDropItemDetail>& vecDropItemDetail_)
{
	KActionDecider kActionDecider = m_kActionDecider;

	std::map<int, KDropInfo>::iterator dropInfoIter;
	std::map<int, int> mapResult;
	std::map<int, int>::iterator resultIter;

	kActionDecider.MakeDecision(mapResult);

	vecDropItemDetail_.clear();

	resultIter = mapResult.begin();
	while (resultIter != mapResult.end())
	{
		dropInfoIter = m_mapDropInfoSet.find(resultIter->second);
		if (dropInfoIter == m_mapDropInfoSet.end())
		{
			START_LOG(cerr, L"Drop 처리의 KDropInfo 인덱스가 이상함.")
				<< BUILD_LOG(resultIter->second)
				<< END_LOG;

			vecDropItemDetail_.clear();
			return;
		}

		START_LOG(clog, L"드랍된 아이템의 DropInfo index : " << resultIter->second) << END_LOG;

		vecDropItemDetail_.push_back(dropInfoIter->second.m_kDropItemDetail);
		resultIter++;
	}
}

KDropMode& KDropMode::operator=(const KDropMode& t)
{
	m_iDropMethod = t.m_iDropMethod;
	m_iNextID = t.m_iNextID;
	m_kActionDecider = t.m_kActionDecider;
	m_mapDropInfoSet = t.m_mapDropInfoSet;

	return *this;
}

ImplOstmOperatorW2A(KDropMode);
ImplOstmOperatorW(KDropMode);
ImplToStringW(KDropMode)
{
	return stm_ << L"KDropMode Dump." << std::endl
		<< TOSTRINGW(m_iDropMethod)
		<< TOSTRINGW(m_iNextID)
		<< TOSTRINGW(m_mapDropInfoSet.size())
		<< m_kActionDecider << std::endl;
}

// KDropSystem

KDropSystem::KDropSystem()
{
	m_bEnableAdminEvent = false;
	m_pairAdminEventItem = std::make_pair(-1, 0);
}

KDropSystem::KDropSystem(const KDropSystem& t)
{
	*this = t;
}

void KDropSystem::ProcessDrop(int iMainKey_, int iSubKey_, int iDifficult_, int ireserve_,
	OUT std::vector<KDropItemDetail>& vecDropItemDetail_)
{
	DWORD dwKey = (iMainKey_ << 24) + (iSubKey_ << 16) + (iDifficult_ << 8) + (0x000000ff & ireserve_); // 생성된 조합키

	{//locking scope
		KLocker lock(m_csDropModeList);

		std::map<DWORD, KDropMode>::iterator mit;

		// 061214. ZzangDoL.
		mit = m_mapDropModeList.find(dwKey);

		if (mit != m_mapDropModeList.end())
		{
			mit->second.ProcessDrop(vecDropItemDetail_);
			return;
		}
	}

	START_LOG(clog, L"드랍 시스템에 등록된 모드가 아님.")
		<< BUILD_LOG(iMainKey_)
		<< BUILD_LOG(iSubKey_)
		<< BUILD_LOG(iDifficult_)
		<< BUILD_LOG(ireserve_)
		<< BUILD_LOG(dwKey) << END_LOG;
}

void KDropSystem::ProcessAdminEvent(std::vector<KDropItemDetail>& vecDropItemDetail_)
{
	if (m_bEnableAdminEvent)
	{
		KDropItemDetail kDropItemDetail;
		int i;

		// there is no explicit constant defined for the number of jobs.
		for (i = 0; i < CID_MAX; i++) {
			kDropItemDetail.m_mapItemList[i] = m_pairAdminEventItem.first;
		}
		kDropItemDetail.m_iNum = m_pairAdminEventItem.second;
		kDropItemDetail.m_iDropType = KDropItemDetail::DT_ADMIN_EVENT;
		kDropItemDetail.m_iPeriod = -1;

		KLocker lock(m_csDropModeList);//locking scope

		vecDropItemDetail_.push_back(kDropItemDetail);
	}
}

KDropSystem& KDropSystem::operator=(const KDropSystem& right_)
{
	m_mapDropModeList = right_.m_mapDropModeList;
	m_bEnableAdminEvent = right_.m_bEnableAdminEvent;
	m_pairAdminEventItem = right_.m_pairAdminEventItem;

	return *this;
}

void KDropSystem::AddMode(int iMainKey_, int iSubKey_, int iDifficult_, int ireserve_, KDropMode& kMode_, int iDropMethod_)
{
	DWORD dwKey = (iMainKey_ << 24) + (iSubKey_ << 16) + (iDifficult_ << 8) + (0x000000ff & ireserve_); // 생성된 조합키

	{//locking scope
		KLocker lock(m_csDropModeList);

		std::map<DWORD, KDropMode>::iterator mit;
		mit = m_mapDropModeList.find(dwKey);

		if (mit != m_mapDropModeList.end())
		{
			START_LOG(cerr, L"드랍 시스템에 이미 등록된 모드를 등록하려고 함.")
				<< BUILD_LOG(iMainKey_)
				<< BUILD_LOG(iSubKey_)
				<< BUILD_LOG(iDifficult_)
				<< BUILD_LOG(ireserve_)
				<< BUILD_LOG(dwKey) << END_LOG;
		}
	}
	kMode_.m_iDropMethod = iDropMethod_;
	m_mapDropModeList[dwKey] = kMode_;
}

bool KDropSystem::GetMode(IN const int iMainKey_, IN const int iSubKey_, IN const int iDifficult_, IN const int ireserve_, OUT KDropMode& kMode_)
{
	DWORD dwKey = (iMainKey_ << 24) + (iSubKey_ << 16) + (iDifficult_ << 8) + (0x000000ff & ireserve_);
	{ //locking scope
		KLocker lock(m_csDropModeList);

		std::map<DWORD, KDropMode>::iterator mit = m_mapDropModeList.find(dwKey);

		if (mit != m_mapDropModeList.end())
		{
			kMode_ = mit->second;
			return true;
		}
	} //locking scope

	START_LOG(clog, L"얻어내려는 드랍 모드가 없음") << END_LOG;
	return false;
}

void KDropSystem::EnableAdminEvent(bool bEnable_, int iItemID_, int iNumItem_)
{
	KLocker lock(m_csDropModeList);

	// suppose that the id of every item is greater than or equal to 0.
	if (bEnable_ && iItemID_ >= 0 && iNumItem_ > 0)
	{
		m_bEnableAdminEvent = true;
		m_pairAdminEventItem = std::make_pair(iItemID_, iNumItem_);
		START_LOG(cerr, L"운영자 기능이 활성화 되었습니다.")
			<< BUILD_LOG(iItemID_)
			<< BUILD_LOG(iNumItem_) << END_LOG;
	}
	else
	{
		m_bEnableAdminEvent = false;
		m_pairAdminEventItem = std::make_pair(-1, 0);
		START_LOG(cerr, L"운영자 기능이 비활성화 되었습니다.") << END_LOG;
	}
}

bool KDropSystem::GetAdminEventInfo(std::pair<int, int>& pairAdminEventItem_)
{
	KLocker lock(m_csDropModeList);

	pairAdminEventItem_ = m_pairAdminEventItem;
	return m_bEnableAdminEvent;
}
void KDropSystem::ClearDropModeList()
{
	KLocker lock(m_csDropModeList);

	m_mapDropModeList.clear();
}

ImplOstmOperatorW(KDropSystem);
ImplOstmOperatorW2A(KDropSystem);

ImplToStringW(KDropSystem)
{
	KLocker lock(m_csDropModeList);

	return stm_ << L"-> KDropSystem ( Admin Event Information )" << std::endl
		<< L"    " << (m_bEnableAdminEvent ? L"Admin Event Enabled." : L"Admin Event Disabled.") << std::endl
		<< L"    Event Item ID : " << m_pairAdminEventItem.first << L", " << L"Number of Items : " << m_pairAdminEventItem.second << std::endl
		<< L"    mapsize : " << m_mapDropModeList.size() << std::endl;
}