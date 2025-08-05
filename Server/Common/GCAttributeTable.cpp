#include "GCAttributeTable.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

KGCAttributeTable::KGCAttributeTable(void)
	:m_iTableType(-1)
{
	m_kLottery.EnableState(KLottery::ENABLE_NO_FAILURE);
}

KGCAttributeTable::~KGCAttributeTable(void)
{
}

bool KGCAttributeTable::IsFixed()
{
	return m_iTableType == ATT_FIXED;
}

bool KGCAttributeTable::IsSelective()
{
	return m_iTableType == ATT_SELECTIVE;
}

bool KGCAttributeTable::IsRandom()
{
	return m_iTableType == ATT_RANDOM;
}

bool KGCAttributeTable::GetAttribute(IN int iCount_, OUT std::list<std::pair<int, float>>& AttributeList_)
{
	if (m_vecAttributeList.empty())
	{
		return true;
	}

	if (GetTableType() != ATT_FIXED && iCount_ > static_cast<int>(m_vecAttributeList.size()))
	{
		START_LOG(cerr, L"속성 테이블 부족합니다.")
			<< BUILD_LOG(iCount_)
			<< BUILD_LOG(m_iTableType)
			<< BUILD_LOG(m_vecAttributeList.size()) << END_LOG;
		return false;
	}

	if (GetTableType() == ATT_FIXED) { // 고정 속성인 경우 등급과 상관 없이 속성이 모두 적용 된다.
		ElementList::iterator vit;
		for (vit = m_vecAttributeList.begin(); vit != m_vecAttributeList.end(); ++vit) {
			AttributeList_.push_back(std::pair<int, float>(vit->m_iType, vit->m_fValue));
		}
	}
	else {
		KLottery kLottery = m_kLottery;
		for (int i = 0; i < iCount_; ++i)
		{
			int iIndex = kLottery.Decision();
			if (kLottery.RemoveCase(iIndex)) {
				SAttribute& sAtt = m_vecAttributeList[iIndex];
				AttributeList_.push_back(std::pair<int, float>(sAtt.m_iType, sAtt.m_fValue));
			}
		}
	}

	return true;
}

bool KGCAttributeTable::GetExceptAttribute(IN const int nCount_, IN const std::list< std::pair< int, float > >& lstExceptAttributeList_, OUT std::list< std::pair< int, float > >& lstAttributeListOut_)
{
	// lstAttributeListOut_ 는 호출부에서 컨테이너를 비우고 넘어온다고 가정하며 여기에서 따로 clear() 하지 않는다

	if (m_vecAttributeList.empty()) {
		return false;
	}

	if (ATT_FIXED == GetTableType()) { // 이 함수는 FIXED_TABLE 타입에 대해서 사용할 수 없다
		return false;
	}

	if (ATT_FIXED != GetTableType() && nCount_ > static_cast<int>(m_vecAttributeList.size())) {
		START_LOG(cerr, L"속성 테이블 부족합니다.")
			<< BUILD_LOG(nCount_)
			<< BUILD_LOG(m_iTableType)
			<< BUILD_LOG(m_vecAttributeList.size()) << END_LOG;
		return false;
	}

	// except list를 제외시킨 리스트를 만들고 count만큼 랜덤으로 뽑는다
	std::vector< SAttribute > vecAttributeList = m_vecAttributeList;

	std::list< std::pair< int, float > >::const_iterator lit(lstExceptAttributeList_.begin());
	for (; lstExceptAttributeList_.end() != lit; ++lit) {
		std::vector< SAttribute >::iterator vit;
		vit = std::find_if(vecAttributeList.begin(), vecAttributeList.end(),
			boost::bind(&SAttribute::m_iType, _1) == lit->first);
		if (vecAttributeList.end() != vit) {
			vecAttributeList.erase(vit);
		}
	}

	KLottery kLottery;
	kLottery.EnableState(KLottery::ENABLE_NO_FAILURE);
	int nCaseIndex = 0;
	BOOST_FOREACH(SAttribute& rAttribute, vecAttributeList) {
		kLottery.AddCase(nCaseIndex, rAttribute.m_fProb);
		++nCaseIndex;
	}

	for (int i = 0; i < nCount_; ++i) {
		int iIndex = kLottery.Decision();
		if (true == kLottery.RemoveCase(iIndex)) {
			SAttribute& sAtt = vecAttributeList[iIndex];
			lstAttributeListOut_.push_back(std::pair< int, float >(sAtt.m_iType, sAtt.m_fValue));
		}
	}

	return true;
}

bool KGCAttributeTable::GetFullAttribute(OUT std::list<std::pair<int, float>>& AttributeList_)
{
	AttributeList_.clear();
	if (m_vecAttributeList.empty())
	{
		return true;
	}

	for (int i = 0; i < (int)m_vecAttributeList.size(); ++i)
	{
		SAttribute& Att = m_vecAttributeList[i];
		AttributeList_.push_back(std::pair<int, float>(Att.m_iType, Att.m_fValue));
	}
	return true;
}

bool KGCAttributeTable::LoadScript(IN std::pair<int, int> prAttributeInfo_, IN OUT KLuaManager& luaMgr)
{
	_JIF(luaMgr.GetValue("Type", m_iTableType) == S_OK, return false);
	_JIF(luaMgr.BeginTable("AttributeInfo") == S_OK, return false);
	m_prAttributeInfo = prAttributeInfo_;

	int iIndex = 0;
	while (++iIndex)
	{
		if (FAILED(luaMgr.BeginTable(iIndex))) {
			break;
		}

		SAttribute sAtt;
		if (FAILED(luaMgr.GetValue(1, sAtt.m_iType)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 1") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		if (FAILED(luaMgr.GetValue(2, sAtt.m_fValue)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 2") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		if (FAILED(luaMgr.GetValue(3, sAtt.m_fProb)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 3") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		m_vecAttributeList.push_back(sAtt);
		m_kLottery.AddCase(static_cast<int>(m_vecAttributeList.size() - 1), sAtt.m_fProb);

		luaMgr.EndTable();
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	// 랜덤 속성 정보 읽어오기.
	std::set<DWORD> setRandomAttributeIndex;
	LoadRandomAttribute(luaMgr, setRandomAttributeIndex);

	m_setRandomAttributeIndex.swap(setRandomAttributeIndex);

	return true;
}

bool KGCAttributeTable::SaveTable(IN OUT std::ofstream& file, int iTab)
{
	file << GetTab(iTab) << "[" << m_prAttributeInfo.first << "] = {\n";
	file << GetTab(iTab + 1) << "Grade = " << m_prAttributeInfo.second << ",\n";
	file << GetTab(iTab + 1) << "Type = " << GetTableTypeEnumString(m_iTableType) << ",\n";
	for (size_t i = 0; i < m_vecAttributeList.size(); ++i)
	{
		SAttribute& Val = m_vecAttributeList[i];
		file << GetTab(iTab + 1) << "{ "
			<< GetAttributeTypeEnumString(Val.m_iType) << ",\t"
			<< Val.m_fValue << ",\t"
			<< Val.m_fProb << "\t},\n";
	}
	file << GetTab(iTab) << "},\n";

	return true;
}

std::string KGCAttributeTable::GetTableTypeEnumString(int iEnum)
{
	switch (iEnum)
	{
	case ATT_RANDOM:
		return "RANDOM_TABLE";
	case ATT_SELECTIVE:
		return "SELECTIVE_TABLE";
	case ATT_FIXED:
		return "FIXED_TABLE";
	default:
		START_LOG(cerr, L"You get wrong enum!!") << END_LOG;
		return "NULL";
		break;
	}
}

std::string KGCAttributeTable::GetAttributeTypeEnumString(int iEnum)
{
	switch (iEnum)
	{
	case AT_VITALITY:
		return "VITALITY";
	case AT_DEFENCE:
		return "DEFENCE";
	case AT_ATTACK:
		return "ATTACK";
	case AT_CRITICAL:
		return "CRITICAL";
	case AT_COUNTER:
		return "COUNTER";
	default:
		START_LOG(cerr, L"You get wrong enum!!") << END_LOG;
		return "NULL";
		break;
	}
}

void KGCAttributeTable::SaveEnum(IN OUT std::ofstream& file)
{
	for (int i = 0; i < AT_NUM; ++i)
	{
		file << GetAttributeTypeEnumString(i) << "\t= " << i << "\n";
	}

	file << "\n\n";

	for (int i = 0; i < ATT_NUM; ++i)
	{
		file << GetTableTypeEnumString(i) << "\t= " << i << "\n";
	}
}

bool KGCAttributeTable::LoadRandomAttribute(IN KLuaManager& kLuaMng_, OUT std::set<DWORD>& setRandomAttribute_)
{
	setRandomAttribute_.clear();
	int nIndex = 0;

	_JIF(kLuaMng_.BeginTable("RandomAttributeIndex") == S_OK, return false);
	for (int i = 1; ; ++i) {
		DWORD dwAttributeType = 0;
		if (kLuaMng_.GetValue(i, dwAttributeType) != S_OK)
			break;

		setRandomAttribute_.insert(dwAttributeType);
	}

	_JIF(kLuaMng_.EndTable() == S_OK, return false);

	return true;
}

bool KGCAttributeTable::GetRandomAttributeIndex(OUT std::set<DWORD>& setRandomAttributeIndex_)
{
	setRandomAttributeIndex_.clear();
	setRandomAttributeIndex_ = m_setRandomAttributeIndex;
	return true;
}

bool KGCAttributeTable::CheckRandomAttributeIndex(IN const DWORD dwType_)
{
	std::set<DWORD>::iterator sit;
	sit = m_setRandomAttributeIndex.find(dwType_);

	if (sit == m_setRandomAttributeIndex.end()) {
		return true;
	}
	else {
		return false;
	}
}