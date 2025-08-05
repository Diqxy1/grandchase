#include "GCAttributeDecider.h"
#include <fstream>
#include <dbg/dbg.hpp>
#include "GCItemAttribute.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

KGCAttributeDecider::KGCAttributeDecider(void)
{
}

KGCAttributeDecider::~KGCAttributeDecider(void)
{
}

bool KGCAttributeDecider::GetAttributeList(IN int nIndex_, IN int nCount_, IN char cGrade_, OUT std::list<std::pair<int, float> >& AttributeList_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = static_cast<int>(cGrade_);
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	if (GetTableType(nIndex_, nGrade) != Table::ATT_SELECTIVE) {
		return mit->second.GetAttribute(nCount_, AttributeList_);
	}

	for (int i = 0; i < nCount_; ++i) {
		AttributeList_.push_back(std::pair<int, float>(Table::AT_INVAILD, 0.0f));
	}
	return true;
}

bool KGCAttributeDecider::SaveScript(IN std::string strFileName_)
{
	std::ofstream file;

	file.open(strFileName_.c_str());

	if (file.is_open() == false)
	{
		START_LOG(cerr, L"File is not opended!!!") << END_LOG;
		return false;
	}

	Table::SaveEnum(file);

	file << "\n\nItemAttributeTable = {\n";

	KLocker lock(m_csTable);
	TableList::iterator mit;
	for (mit = m_mapTables.begin(); mit != m_mapTables.end(); ++mit)
	{
		mit->second.SaveTable(file, 1);
	}

	file << "}\n";
	return true;
}

bool KGCAttributeDecider::LoadScript()
{
	return _LoadScript(std::string());
}

bool KGCAttributeDecider::_LoadScript(OUT std::string& strScript_)
{
	KLuaManager luaMgr;

	const std::string strScriptName = "ItemAttributeTable.lua";
	strScript_ = strScriptName;
	_JIF(luaMgr.DoFile(strScriptName.c_str()) == S_OK, return false);
	_JIF(luaMgr.DoFile("SetItem.stg") == S_OK, return false);

	TableList mapTables;
	_JIF(LoadItemAttributeTable(luaMgr, mapTables), return false);

	std::map< DWORD, std::set<GCITEMID> > mapItemList;
	_JIF(LoadInitAttributeItemList(luaMgr, mapItemList), return false);

	std::map< DWORD, std::set<GCITEMID> > mapGPInitItemList;
	_JIF(LoadInitGP_AttributeItemList(luaMgr, mapGPInitItemList), return false);

	std::map< DWORD, std::set<GCITEMID> > mapRandomGPItemList;
	_JIF(LoadRandomGP_AttributeItemList(luaMgr, mapRandomGPItemList), return false);

	std::map< DWORD, std::set< GCITEMID > > mapSingleRandomAttributeItemList;
	_JIF(LoadSingleRandomAttributeItemList(luaMgr, mapSingleRandomAttributeItemList), return false);

	std::map<GCITEMID, GCITEMID> mapItemSetID;
	std::map<GCITEMID, KSetItemInfo> mapSetItemInfo;
	_JIF(LoadSetItemList(luaMgr, mapItemSetID, mapSetItemInfo), return false);

	KChangeAttributeEventInfo              kChangeAttributeEventInfo;
	_JIF(LoadEventMigrationAttributeInfo(luaMgr, kChangeAttributeEventInfo), return false);

	{
		KLocker lock(m_csTable);

		m_mapTables = mapTables;
		m_mapInitAttributeItemList.swap(mapItemList);
		m_mapInitGPAttributeItemList.swap(mapGPInitItemList); // GP 아이템 속성...
		m_mapRandomGPAttributeItemList.swap(mapRandomGPItemList); // GP 아이템 랜덤 속성.
		m_mapSingleRandomAttributeItemList.swap(mapSingleRandomAttributeItemList);
		m_mapItemSetID.swap(mapItemSetID);
		m_mapSetItemInfo.swap(mapSetItemInfo);
		m_kChangeAttributeEventInfo = kChangeAttributeEventInfo;
	}

	return true;
}

bool KGCAttributeDecider::GetTable(IN int iTableIndex, IN int nGrade_, OUT Table& kTable_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(iTableIndex, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	kTable_ = mit->second;
	return true;
}

int KGCAttributeDecider::AddTable(IN Table& kTable_)
{
	KLocker lock(m_csTable);
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo.first = GetFreeIndex();
	prAttributeInfo.second = DEFAULT_GRADE;

	m_mapTables.insert(TableList::value_type(prAttributeInfo, kTable_));
	return prAttributeInfo.first;
}

int KGCAttributeDecider::GetFreeIndex()
{
	TableList::const_iterator mit;

	int iLastIndex = 0;

	KLocker lock(m_csTable);

	for (mit = m_mapTables.begin(); mit != m_mapTables.end(); ++mit) {
		if (mit->first.first > iLastIndex) {
			iLastIndex = mit->first.first;
		}
	}
	return iLastIndex + 1;
}

KGCAttributeDecider::Table::AttributeTableType KGCAttributeDecider::GetTableType(IN int nIndex_, IN int nGrade_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return KGCAttributeDecider::Table::ATT_INVAILD;
		}
	}

	return mit->second.GetTableType();
}

bool KGCAttributeDecider::GetAttributeFullList(IN int nIndex_, IN int nGrade_, OUT std::list<std::pair<int, float>>& AttributeList_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	return mit->second.GetFullAttribute(AttributeList_);
}

bool KGCAttributeDecider::LoadInitAttributeItemList(IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();
	_JIF(kLuaMng_.BeginTable("InitAttributeItem") == S_OK, return false);
	__LIF(LoadItems(kLuaMng_, std::string("ItemList"), mapItems_));
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

bool KGCAttributeDecider::LoadInitGP_AttributeItemList(IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();
	_JIF(kLuaMng_.BeginTable("InitGP_AttributeItem") == S_OK, return false);
	__LIF(LoadItems(kLuaMng_, std::string("ItemList"), mapItems_));
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

bool KGCAttributeDecider::LoadRandomGP_AttributeItemList(IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();
	_JIF(kLuaMng_.BeginTable("RandomGP_AttributeItem") == S_OK, return false);
	__LIF(LoadItems(kLuaMng_, std::string("ItemList"), mapItems_));
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

bool KGCAttributeDecider::LoadItems(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();
	__JIF(kLuaMng_.BeginTable(strTable_.c_str()) == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (kLuaMng_.BeginTable(i) != S_OK) break;

		DWORD dwGrade = 0;
		std::set<GCITEMID> setItemList;
		if (kLuaMng_.GetValue("Grade", dwGrade) != S_OK) break;            // Grade.
		__LIF(LoadSetItems(kLuaMng_, std::string("ItemID"), setItemList));
		if (!mapItems_.insert(std::make_pair(dwGrade, setItemList)).second) {
			START_LOG(cerr, L"중복된 등급이 있음. ItemID : " << dwGrade) << END_LOG;
			return false;
		}
		_JIF(kLuaMng_.EndTable() == S_OK, return false);
	}
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

void KGCAttributeDecider::GetInitAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();

	KLocker lock(m_csTable);
	mapItems_ = m_mapInitAttributeItemList;
}

bool KGCAttributeDecider::CheckInitAttributeItem(IN const DWORD& dwGrade_, IN const GCITEMID& itemID_)
{
	KLocker lock(m_csTable);

	std::map< DWORD, std::set<GCITEMID> >::iterator mit;
	std::set<GCITEMID>::iterator sit;
	mit = m_mapInitAttributeItemList.find(dwGrade_);
	if (mit == m_mapInitAttributeItemList.end()) {
		return false;
	}
	sit = mit->second.find(itemID_);
	if (sit == mit->second.end()) {
		return false;
	}
	return true;
}

void KGCAttributeDecider::GetInitGPAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();

	KLocker lock(m_csTable);
	mapItems_ = m_mapInitGPAttributeItemList;
}

void KGCAttributeDecider::GetRandomGPAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems_)
{
	mapItems_.clear();

	KLocker lock(m_csTable);
	mapItems_ = m_mapRandomGPAttributeItemList;
}

bool KGCAttributeDecider::CheckInitGP_AttributeItem(IN const DWORD& dwGrade_, IN const GCITEMID& itemID_)
{
	KLocker lock(m_csTable);

	std::map< DWORD, std::set<GCITEMID> >::iterator mit;
	std::set<GCITEMID>::iterator sit;
	mit = m_mapInitGPAttributeItemList.find(dwGrade_);
	if (mit == m_mapInitGPAttributeItemList.end()) {
		return false;
	}
	sit = mit->second.find(itemID_);
	if (sit == mit->second.end()) {
		return false;
	}
	return true;
}

bool KGCAttributeDecider::CheckRandomGP_AttributeItem(IN const DWORD& dwGrade_, IN const GCITEMID& itemID_)
{
	KLocker lock(m_csTable);

	std::map< DWORD, std::set<GCITEMID> >::iterator mit;
	std::set<GCITEMID>::iterator sit;
	mit = m_mapRandomGPAttributeItemList.find(dwGrade_);
	if (mit == m_mapRandomGPAttributeItemList.end()) {
		return false;
	}
	sit = mit->second.find(itemID_);
	if (sit == mit->second.end()) {
		return false;
	}
	return true;
}

bool KGCAttributeDecider::LoadSetItems(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setItems_)
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

bool KGCAttributeDecider::LoadItemAttributeTable(IN KLuaManager& lugMgr_, OUT TableList& mapTables_)
{
	_JIF(lugMgr_.BeginTable("ItemAttributeTable") == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (lugMgr_.BeginTable(i) != S_OK) break;
		Table kTable;
		std::pair<int, int> prAttributeInfo;
		_JIF(lugMgr_.GetValue("AttributeNum", prAttributeInfo.first) == S_OK, return false);
		_JIF(lugMgr_.GetValue("Grade", prAttributeInfo.second) == S_OK, return false);

		if (kTable.LoadScript(prAttributeInfo, lugMgr_) == false) {
			break;
		}
		_JIF(lugMgr_.EndTable() == S_OK, return false);

		if (mapTables_.insert(std::make_pair(prAttributeInfo, kTable)).second == false) {
			START_LOG(cerr, L"중복된 속성정보. AttributeNum : " << prAttributeInfo.first << L",Grade : " << prAttributeInfo.second) << END_LOG;
		}
	}
	_JIF(lugMgr_.EndTable() == S_OK, return false);

	return true;
}

bool KGCAttributeDecider::LoadSetItemList(IN KLuaManager& lugMgr_, OUT std::map<GCITEMID, GCITEMID>& mapItemSetID_, OUT std::map<GCITEMID, KSetItemInfo>& mapSetItemInfo_)
{
	_JIF(lugMgr_.BeginTable("SET_ITEM") == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (lugMgr_.BeginTable(i) != S_OK) break;

		KSetItemInfo kSetInfo;
		GCITEMID SetItemID = 0;
		// 세트아이템ID
		if (lugMgr_.GetValue("NAME", SetItemID) != S_OK) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			continue;
		}
		// 클라이언트 스크립트라서 ItemID에 10을 곱해준다.
		SetItemID *= 10;

		// 아이템ID 리스트
		if (false == LoadPartItemID(lugMgr_, kSetInfo.m_vecItemID)) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			continue;
		}

		// 장착개수별 효과
		if (false == LoadAttributeList(lugMgr_, kSetInfo.m_mapGpBonus, kSetInfo.m_mapExpBonus)) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			continue;
		}
		_JIF(lugMgr_.EndTable() == S_OK, return false);

		// Exp/Gp 효과 있는 세트아이템만 기록
		if (!kSetInfo.m_mapGpBonus.empty() || !kSetInfo.m_mapExpBonus.empty()) {
			mapSetItemInfo_[SetItemID] = kSetInfo;

			std::vector<GCITEMID>::iterator vit;
			for (vit = kSetInfo.m_vecItemID.begin(); vit != kSetInfo.m_vecItemID.end(); ++vit) {
				mapItemSetID_[*vit] = SetItemID;
			}
		}
	}
	_JIF(lugMgr_.EndTable() == S_OK, return false);

	START_LOG(cerr, L"Load SetItem Script..")
		<< BUILD_LOG(mapSetItemInfo_.size())
		<< BUILD_LOG(mapItemSetID_.size()) << END_LOG;
	return true;
}

bool KGCAttributeDecider::LoadPartItemID(IN KLuaManager& lugMgr_, OUT std::vector<GCITEMID>& vecItemID_)
{
	if (lugMgr_.BeginTable("PARTS") != S_OK) {
		return false;
	}

	for (int i = 1; ; ++i) {
		GCITEMID ItemID = 0;
		if (lugMgr_.GetValue(i, ItemID) != S_OK) break;
		// 클라이언트 스크립트라서 ItemID에 10을 곱해준다.
		vecItemID_.push_back(ItemID * 10);
	}

	_JIF(lugMgr_.EndTable() == S_OK, return false);
	return true;
}

bool KGCAttributeDecider::LoadAttributeList(IN KLuaManager& lugMgr_, OUT std::map<int, float>& mapGp_, OUT std::map<int, float>& mapExp_)
{
	if (lugMgr_.BeginTable("ATTRIBUTE_LIST") != S_OK) {
		return false;
	}

	for (int i = 1; ; ++i) {
		if (lugMgr_.BeginTable(i) != S_OK) break;

		int nEquipCount = 0;
		std::map<int, float> mapAttribute;
		if (lugMgr_.GetValue("EQUIP_COUNT", nEquipCount) != S_OK) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			break;
		}
		if (false == LoadAttribute(lugMgr_, mapAttribute)) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			break;
		}

		_JIF(lugMgr_.EndTable() == S_OK, return false);

		// Exp/GP 세트개수별 효과
		mapGp_[nEquipCount] = mapAttribute[ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE];
		mapExp_[nEquipCount] = mapAttribute[ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE];
	}

	_JIF(lugMgr_.EndTable() == S_OK, return false);
	return true;
}

bool KGCAttributeDecider::LoadAttribute(IN KLuaManager& lugMgr_, OUT std::map<int, float>& mapAtrribute_)
{
	if (lugMgr_.BeginTable("ATTRIBUTE") != S_OK) {
		return false;
	}

	for (int i = 1; ; ++i) {
		if (lugMgr_.BeginTable(i) != S_OK) break;

		int nAttrType = 0;
		float fAbility = 0.f;
		if (lugMgr_.GetValue("ATTRIBUTE_TYPE", nAttrType) != S_OK) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			break;
		}

		if (lugMgr_.GetValue("ABILITY", fAbility) != S_OK) {
			_JIF(lugMgr_.EndTable() == S_OK, return false);
			break;
		}
		fAbility *= 0.01f;

		_JIF(lugMgr_.EndTable() == S_OK, return false);

		if (nAttrType != ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE && nAttrType != ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE) {
			continue;
		}

		// Exp,Gp 효과만 기록
		mapAtrribute_[nAttrType] += fAbility;
	}

	_JIF(lugMgr_.EndTable() == S_OK, return false);
	return true;
}

bool KGCAttributeDecider::GetSetID(IN const GCITEMID& ItemID_, OUT GCITEMID& dwSetID_)
{
	KLocker lock(m_csTable);

	std::map<GCITEMID, GCITEMID>::const_iterator cmit;
	cmit = m_mapItemSetID.find(ItemID_);
	if (cmit == m_mapItemSetID.end()) {
		return false;
	}

	dwSetID_ = cmit->second;
	return true;
}

bool KGCAttributeDecider::GetSetInfo(IN const GCITEMID& dwSetID_, OUT KSetItemInfo& kInfo_)
{
	KLocker lock(m_csTable);

	std::map<GCITEMID, KSetItemInfo>::const_iterator cmit;
	cmit = m_mapSetItemInfo.find(dwSetID_);
	if (cmit == m_mapSetItemInfo.end()) {
		return false;
	}

	kInfo_ = cmit->second;
	return true;
}

bool KGCAttributeDecider::GetSetItemList(IN const GCITEMID& dwSetID_, OUT std::vector<GCITEMID>& vecItemID_)
{
	KSetItemInfo kInfo;
	if (false == GetSetInfo(dwSetID_, kInfo)) {
		return false;
	}

	vecItemID_ = kInfo.m_vecItemID;
	return true;
}

bool KGCAttributeDecider::GetSetExpBonus(IN const GCITEMID& dwSetID_, IN const int& nEquipCount_, OUT float& fExpBonus_)
{
	KSetItemInfo kInfo;
	if (false == GetSetInfo(dwSetID_, kInfo)) {
		return false;
	}

	std::map<int, float>::const_iterator cmit;
	for (cmit = kInfo.m_mapExpBonus.begin(); cmit != kInfo.m_mapExpBonus.end(); ++cmit) {
		if (nEquipCount_ < cmit->first) break;

		fExpBonus_ += cmit->second;
	}

	START_LOG(clog, L"Set Exp Bonus.. SetID : " << dwSetID_ << L"Equip count : " << nEquipCount_ << L", Exp bonus : " << fExpBonus_) << END_LOG;
	return true;
}

bool KGCAttributeDecider::GetSetGpBonus(IN const GCITEMID& dwSetID_, IN const int& nEquipCount_, OUT float& fGpBonus_)
{
	KSetItemInfo kInfo;
	if (false == GetSetInfo(dwSetID_, kInfo)) {
		return false;
	}

	std::map<int, float>::const_iterator cmit;
	for (cmit = kInfo.m_mapGpBonus.begin(); cmit != kInfo.m_mapGpBonus.end(); ++cmit) {
		if (nEquipCount_ < cmit->first) break;

		fGpBonus_ += cmit->second;
	}

	START_LOG(clog, L"Set Gp Bonus.. SetID : " << dwSetID_ << L"Equip count : " << nEquipCount_ << L", Gp bonus : " << fGpBonus_) << END_LOG;
	return true;
}

void KGCAttributeDecider::DumpSetItemInfo(IN const GCITEMID& dwSetID_)
{
	KSetItemInfo kInfo;
	if (false == GetSetInfo(dwSetID_, kInfo)) {
		START_LOG(cerr, L"Exp/Gp bonus 정보에 해당하는 SetID 정보가 없음. SetID : " << dwSetID_)
			<< END_LOG;
		return;
	}

	START_LOG(cerr, L"--- Dump SetItem (SetID:" << dwSetID_ << L") ---") << END_LOG;
	START_LOG(cerr, L" - Part ItemID list (size:" << kInfo.m_vecItemID.size() << L")") << END_LOG;

	std::vector<GCITEMID>::iterator vit;
	for (vit = kInfo.m_vecItemID.begin(); vit != kInfo.m_vecItemID.end(); ++vit) {
		START_LOG(cerr, L"" << *vit << L" ") << END_LOG;
	}

	START_LOG(cerr, L" - Set bonus list (size:" << kInfo.m_mapExpBonus.size() << L"," << kInfo.m_mapGpBonus.size() << L")") << END_LOG;
	for (int i = 1; i <= 7; ++i) {
		float fExpBonus = 0.f;
		float fGpBonus = 0.f;

		GetSetExpBonus(dwSetID_, i, fExpBonus);
		GetSetGpBonus(dwSetID_, i, fGpBonus);

		if (fExpBonus <= 0.f && fGpBonus <= 0.f) {
			continue;
		}

		START_LOG(cerr, L"[" << i << L"]" << dbg::endl << L" Exp : " << fExpBonus
			<< L" Gp : " << fGpBonus) << END_LOG;
	}

	START_LOG(cerr, L"--- End Dump ---") << END_LOG;
}

bool KGCAttributeDecider::GetRandomAttributeIndex(IN int nIndex_, IN int nGrade_, OUT std::set<DWORD>& setRandomAttributeIndex_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	return mit->second.GetRandomAttributeIndex(setRandomAttributeIndex_);
}

bool KGCAttributeDecider::CheckRandomAttributeIndex(IN int nIndex_, IN int nGrade_, IN const DWORD dwType_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	return mit->second.CheckRandomAttributeIndex(dwType_);
}

bool KGCAttributeDecider::GetSelectiveAttributeList(IN int nIndex_, IN int nCount_, IN int nGrade_, OUT std::list<std::pair<int, float>>& AttributeList_)
{
	int nGrade = DEFAULT_GRADE;
	std::pair<int, int> prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);
	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (mit == m_mapTables.end()) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (mit == m_mapTables.end()) {
			return false;
		}
	}

	// ItemAttributeTable.lua에 설정된 값들에 대해서만 가져오게 수정.
	// RANDOM_TABLE = 0, SELECTIVE_TABLE = 1, FIXED_TABLE = 2
	// -1값을 ItemAttributeTable.lua에 사용하면 안됨.
	if (GetTableType(nIndex_, nGrade) != Table::ATT_INVAILD) {
		return mit->second.GetAttribute(nCount_, AttributeList_);
	}

	for (int i = 0; i < nCount_; ++i) {
		AttributeList_.push_back(std::pair<int, float>(Table::AT_INVAILD, 0.0f));
	}
	return true;
}

bool KGCAttributeDecider::GetSelectiveAttributeExceptList(IN const int nIndex_, IN const int nCount_, IN const int nGrade_, IN const std::list< std::pair< int, float > >& lstExceptAttributeList_, OUT std::list< std::pair< int, float > >& lstAttributeListOut_)
{
	lstAttributeListOut_.clear();

	int nGrade = DEFAULT_GRADE;
	std::pair< int, int > prAttributeInfo;
	prAttributeInfo = std::make_pair(nIndex_, nGrade);

	KLocker lock(m_csTable);

	TableList::iterator mit = m_mapTables.find(prAttributeInfo);

	if (m_mapTables.end() == mit) {
		nGrade = nGrade_;
		prAttributeInfo.second = nGrade;
		mit = m_mapTables.find(prAttributeInfo);
		if (m_mapTables.end() == mit) {
			return false;
		}
	}

	if (Table::ATT_INVAILD != GetTableType(nIndex_, nGrade)) {
		/* 여기에서 GetFullAttribute() 함수를 이용해서 전체 리스트를 가져온 뒤 except list를 제거하고 속성을 뽑는 로직이 맞는 것 같지만
		   GetFullAttribute() 함수 내에 리턴 처리가 미심쩍고 내용을 고치자니 기존 로직에 영향줄 수 있어 함수를 새로 구현해서 사용한다 */
		return mit->second.GetExceptAttribute(nCount_, lstExceptAttributeList_, lstAttributeListOut_);
	}

	for (int i = 0; i < nCount_; ++i) {
		lstAttributeListOut_.push_back(std::pair< int, float >(Table::AT_INVAILD, 0.0f));
	}

	return false;
}

bool KGCAttributeDecider::LoadEventMigrationAttributeInfo(IN KLuaManager& kLuaMng_, OUT KChangeAttributeEventInfo& kChangeAttributeEventInfo_)
{
	KChangeAttributeEventInfo kChangeAttributeEventInfo;

	_JIF(kLuaMng_.BeginTable("EventMigrationAttributeInfo") == S_OK, return false);
	_JIF(kLuaMng_.GetValue("bUseEnable", kChangeAttributeEventInfo.m_bUseEnable) == S_OK, return false);
	_JIF(LoadTime(kLuaMng_, std::string("BeginTime"), kChangeAttributeEventInfo.m_tmBegin), return false);
	_JIF(LoadTime(kLuaMng_, std::string("EndTime"), kChangeAttributeEventInfo.m_tmEnd), return false);
	_JIF(kLuaMng_.GetValue("bCountCheckEnable", kChangeAttributeEventInfo.m_bUserCountCheckEnable) == S_OK, return false);
	_JIF(LoadTime(kLuaMng_, std::string("RecevieDate"), kChangeAttributeEventInfo.m_tmReceiveTime), return false);
	_JIF(kLuaMng_.GetValue("ItemChangeMaxCount", kChangeAttributeEventInfo.m_dwUserChangeMaxCount) == S_OK, return false);
	_JIF(LoadSetData(kLuaMng_, std::string("ChangeAttributeList"), kChangeAttributeEventInfo.m_setChangeAttributeList), return false);
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	kChangeAttributeEventInfo_ = kChangeAttributeEventInfo;
	return true;
}

bool KGCAttributeDecider::LoadTime(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_)
{
	int nYear, nMonth, nDay, nHour;
	_JIF(kLuaMng_.BeginTable(strTable_.c_str()) == S_OK, return false);
	_JIF(kLuaMng_.GetValue(1, nYear) == S_OK, return false);
	_JIF(kLuaMng_.GetValue(2, nMonth) == S_OK, return false);
	_JIF(kLuaMng_.GetValue(3, nDay) == S_OK, return false);
	_JIF(kLuaMng_.GetValue(4, nHour) == S_OK, return false);
	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	tmTime_ = KncUtil::TimeToInt(CTime(nYear, nMonth, nDay, nHour, 0, 0));
	return true;
}

bool KGCAttributeDecider::LoadSetData(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setInfo_)
{
	setInfo_.clear();
	__JIF(kLuaMng_.BeginTable(strTable_.c_str()) == S_OK, return false);

	int i = 1;
	int nData = 0;
	while (true) {
		if (kLuaMng_.GetValue(i++, nData) != S_OK) break;
		setInfo_.insert(nData);
	}

	_JIF(kLuaMng_.EndTable() == S_OK, return false);
	return true;
}

bool KGCAttributeDecider::IsEventTerm()
{
	time_t tmCurrent = KncUtil::TimeToInt(CTime::GetCurrentTime());

	KLocker lock(m_csTable);
	return (m_kChangeAttributeEventInfo.m_tmBegin <= tmCurrent && m_kChangeAttributeEventInfo.m_tmEnd >= tmCurrent);
}

void KGCAttributeDecider::GetChangeAttributeList(OUT std::set<int>& setChangeAttributeList_)
{
	setChangeAttributeList_.clear();

	KLocker lock(m_csTable);
	setChangeAttributeList_ = m_kChangeAttributeEventInfo.m_setChangeAttributeList;
}

bool KGCAttributeDecider::CheckChangeAttribute(IN char cType_)
{
	std::set<int>::iterator sit;

	KLocker lock(m_csTable);
	int nType = static_cast<int>(cType_);
	sit = m_kChangeAttributeEventInfo.m_setChangeAttributeList.find(nType);
	if (sit == m_kChangeAttributeEventInfo.m_setChangeAttributeList.end()) {
		return false;
	}
	return true;
}

bool KGCAttributeDecider::CheckItemChangePossible(IN const time_t& tmRegDate_)
{
	KLocker lock(m_csTable);

	if (m_kChangeAttributeEventInfo.m_tmReceiveTime < tmRegDate_ || tmRegDate_ == 0) {
		return false;
	}
	return true;
}

bool KGCAttributeDecider::LoadSingleRandomAttributeItemList(IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::set< GCITEMID > >& mapItems_)
{
	mapItems_.clear();

	_JIF(kLuaMng_.BeginTable("SingleRandomAttributeItem") == S_OK, return false);

	_JIF(kLuaMng_.BeginTable("ItemList") == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (kLuaMng_.BeginTable(i) != S_OK) break;

		DWORD dwTargetLevel = 0;
		std::set< GCITEMID > setItemList;

		if (kLuaMng_.GetValue("TargetLevel", dwTargetLevel) != S_OK) break;
		_JIF(LoadSetItems(kLuaMng_, std::string("ItemID"), setItemList), return false);

		if (false == mapItems_.insert(std::make_pair(dwTargetLevel, setItemList)).second) {
			START_LOG(cerr, L"중복된 타겟 레벨이 있음. TargetLevel : " << dwTargetLevel) << END_LOG;

			return false;
		}

		_JIF(kLuaMng_.EndTable() == S_OK, return false);
	}

	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

void KGCAttributeDecider::GetSingleRandomAttributeItemList(OUT std::map< DWORD, std::set< GCITEMID > >& mapItems_)
{
	mapItems_.clear();

	KLocker lock(m_csTable);

	mapItems_ = m_mapSingleRandomAttributeItemList;
}

bool KGCAttributeDecider::CheckSingleRandomAttributeItemLevel(IN const DWORD dwItemLevel_, IN const GCITEMID dwItemID_)
{
	KLocker lock(m_csTable);

	if (true == m_mapSingleRandomAttributeItemList.empty()) {
		return false;
	}

	std::map< DWORD, std::set< GCITEMID > >::iterator mit(m_mapSingleRandomAttributeItemList.begin());
	for (; m_mapSingleRandomAttributeItemList.end() != mit; ++mit) {
		std::set< GCITEMID >::iterator sit = mit->second.find(dwItemID_);
		if (mit->second.end() != sit) {
			if (dwItemLevel_ <= mit->first) {
				return true;
			}
		}
	}

	return false;
}