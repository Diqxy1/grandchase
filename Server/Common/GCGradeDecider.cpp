#include "GCGradeDecider.h"
#include "lua/KLuaManager.h"
#include <fstream>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG
KGCGradeDecider::KGCGradeDecider(void)
{
}

KGCGradeDecider::~KGCGradeDecider(void)
{
}

bool KGCGradeDecider::LoadScript()
{
	return _LoadScript(std::string());
}

bool KGCGradeDecider::_LoadScript(OUT std::string& strScript_)
{
	KLuaManager luaMgr;

	const std::string strScriptName = "ItemGradeTable.lua";
	strScript_ = strScriptName;
	_JIF(luaMgr.DoFile(strScriptName.c_str()) == S_OK, return false);

	int iIndex = 0;

	_JIF(luaMgr.BeginTable("ItemGradeTable") == S_OK, return false);

	TableList mapTables;
	for (int i = 1; ; ++i) {
		if (luaMgr.BeginTable(i) != S_OK) break;
		KGCGradeTable kTable;
		int nGradeNum;
		_JIF(luaMgr.GetValue("GradeNum", nGradeNum) == S_OK, return false);

		if (kTable.LoadScript(nGradeNum, luaMgr) == false) {
			break;
		}
		_JIF(luaMgr.EndTable() == S_OK, return false);

		mapTables.insert(TableList::value_type(nGradeNum, kTable));
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	KGradeUpgradeInfo kGradeUpgradeInfo;
	_JIF(LoadGradeUpgradeInfo(luaMgr, kGradeUpgradeInfo), return false);

	{
		KLocker lock(m_csTable);
		m_mapTables = mapTables;
		m_kGradeUpgradeInfo = kGradeUpgradeInfo;
	}

	return true;
}

bool KGCGradeDecider::SaveScript(IN std::string strFileName_)
{
	std::ofstream file;

	file.open(strFileName_.c_str());

	if (file.is_open() == false)
	{
		START_LOG(cerr, L"File is not opended!!!") << END_LOG;
		return false;
	}

	Table::SaveEnum(file);

	file << "\n\nItemGradeTable = {\n";

	KLocker lock(m_csTable);
	TableList::iterator mit;
	for (mit = m_mapTables.begin(); mit != m_mapTables.end(); ++mit)
	{
		mit->second.SaveTable(file, 1);
	}

	file << "}\n";
	return true;
}

int KGCGradeDecider::GetGrade(int iTableIndex)
{
	KLocker lock(m_csTable);

	TableList::iterator mit = m_mapTables.find(iTableIndex);

	if (mit == m_mapTables.end())
		return Table::FAIL_GRADE;

	return mit->second.GetGrade();
}

std::string KGCGradeDecider::DumpTables()
{
	TableList::iterator mit;

	KLocker lock(m_csTable);

	std::stringstream stm;
	for (mit = m_mapTables.begin(); mit != m_mapTables.end(); ++mit)
	{
		stm << "===============================================================" << std::endl;
		stm << "Table Index : " << mit->first << std::endl;
		stm << "---------------------------------------------------------------" << std::endl;
		std::map<int, int> mapCounter;
		int iLoop = 10000;
		for (int i = 0; i < iLoop; ++i)
		{
			int iGrade = mit->second.GetGrade();
			++mapCounter[iGrade];
		}

		std::map<int, int>::iterator mit;
		for (mit = mapCounter.begin(); mit != mapCounter.end(); ++mit)
		{
			stm << mit->first << "\t:\t" << mit->second
				<< "( " << static_cast<float>(mit->second) / static_cast<float>(iLoop) * 100.f << "% )" << std::endl;
		}

		stm << "===============================================================" << std::endl;
	}
	return stm.str();
}

bool KGCGradeDecider::GetTable(IN int iTableIndex, OUT Table& kTable_)
{
	KLocker lock(m_csTable);

	TableList::iterator mit = m_mapTables.find(iTableIndex);

	if (mit == m_mapTables.end())
		return false;

	kTable_ = mit->second;
	return true;
}

int KGCGradeDecider::GetFreeIndex() const
{
	TableList::const_iterator mit;

	int iLastIndex = 0;

	KLocker lock(m_csTable);

	for (mit = m_mapTables.begin(); mit != m_mapTables.end(); ++mit)
	{
		if (mit->first > iLastIndex)
			iLastIndex = mit->first;
	}
	return iLastIndex + 1;
}

int KGCGradeDecider::AddTable(IN KGCGradeTable& kTable_)
{
	KLocker lock(m_csTable);
	int iIndex = GetFreeIndex();
	kTable_.SetIndex(iIndex);

	m_mapTables.insert(TableList::value_type(iIndex, kTable_));
	return iIndex;
}

bool KGCGradeDecider::LoadGradeUpgradeInfo(IN KLuaManager& kLuaMng_, OUT KGradeUpgradeInfo& kGradeUpgradeInfo_)
{
	KGradeUpgradeInfo kGradeUpgradeInfo;

	_JIF(kLuaMng_.BeginTable("GradeUpgradeInfo") == S_OK, return false);
	_JIF(kLuaMng_.GetValue("GradeUpgradeItemID", kGradeUpgradeInfo.m_GradeUpgradeItemID) == S_OK, return false);
	_JIF(LoadUseCount(kLuaMng_, std::string("GradeUseCount"), kGradeUpgradeInfo.m_mapGradeUseCount), return false);
	_JIF(kLuaMng_.GetValue("EnableItemType", kGradeUpgradeInfo.m_nEnableItemType) == S_OK, return false);
	_JIF(LoadSetItems(kLuaMng_, std::string("UpgradeBanItem"), kGradeUpgradeInfo.m_setUpgradeBanItem), return false);
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	kGradeUpgradeInfo_ = kGradeUpgradeInfo;
	return true;
}

bool KGCGradeDecider::LoadUseCount(IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<char, int>& mapGradeUseCount_)
{
	mapGradeUseCount_.clear();

	_JIF(kLuaMng_.BeginTable(strTable_.c_str()) == S_OK, return false);
	for (int i = 1; ; ++i) {
		if (kLuaMng_.BeginTable(i) != S_OK) break;

		int nGrade = -1;
		int nCount = -1;
		if (kLuaMng_.GetValue("Grade", nGrade) != S_OK) break;
		if (kLuaMng_.GetValue("UseCount", nCount) != S_OK) break;
		_JIF(mapGradeUseCount_.insert(std::make_pair((char)nGrade, nCount)).second, return false);

		_JIF(kLuaMng_.EndTable() == S_OK, return false);
	}
	_JIF(kLuaMng_.EndTable() == S_OK, return false);
	return true;
}

bool KGCGradeDecider::LoadSetItems(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setItems_)
{
	setItems_.clear();
	__JIF(kLuaMng_.BeginTable(strTable_.c_str()) == S_OK, return false);

	int i = 1;
	while (true)
	{
		GCITEMID ItemID;
		if (kLuaMng_.GetValue(i++, ItemID) != S_OK) break;
		setItems_.insert(ItemID);
	}

	_JIF(kLuaMng_.EndTable() == S_OK, return false);
	return true;
}

bool KGCGradeDecider::CheckGradeUpgradeItem(IN const GCITEMID& ItemID_)
{
	KLocker lock(m_csTable);
	if (m_kGradeUpgradeInfo.m_GradeUpgradeItemID == ItemID_) {
		return true;
	}
	return false;
}

bool KGCGradeDecider::GetGradeUpgradeUseCount(IN const char& cGrade_, OUT int& nUseCount_)
{
	std::map<char, int>::iterator mit;

	KLocker lock(m_csTable);
	mit = m_kGradeUpgradeInfo.m_mapGradeUseCount.find(cGrade_);
	if (mit == m_kGradeUpgradeInfo.m_mapGradeUseCount.end()) {
		return false;
	}
	nUseCount_ = mit->second;
	return true;
}

bool KGCGradeDecider::IsCoordiItemType()
{
	KLocker lock(m_csTable);
	if (m_kGradeUpgradeInfo.m_nEnableItemType != KGradeUpgradeInfo::IT_COORDI) {
		return false;
	}

	return true;
}

bool KGCGradeDecider::CheckUpgradePossibleItem(IN const GCITEMID& ItemID_)
{
	std::set<GCITEMID>::iterator sit;

	KLocker lock(m_csTable);
	sit = m_kGradeUpgradeInfo.m_setUpgradeBanItem.find(ItemID_);
	if (sit != m_kGradeUpgradeInfo.m_setUpgradeBanItem.end()) {
		return false;
	}
	return true;
}

void KGCGradeDecider::GetCoordiUpgradeInfo(OUT KEVENT_COORDI_GRADE_UPGRADE_INFO_ACK& kPacket_)
{
	KLocker lock(m_csTable);
	kPacket_.m_GradeUpgradeItemID = m_kGradeUpgradeInfo.m_GradeUpgradeItemID;
	kPacket_.m_mapGradeUseCount = m_kGradeUpgradeInfo.m_mapGradeUseCount;
	kPacket_.m_nEnableItemType = m_kGradeUpgradeInfo.m_nEnableItemType;
	kPacket_.m_setUpgradeBanItem = m_kGradeUpgradeInfo.m_setUpgradeBanItem;
}