#include "GCAttributeTable.h"
#include <dbg/dbg.hpp>

KGCAttributeTable::KGCAttributeTable(void)
	: m_iIndex(-1)
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

	if (iCount_ > static_cast<int>(m_vecAttributeList.size()))
	{
		START_LOG(cerr, L"속성 테이블 부족합니다.")
			<< BUILD_LOG(iCount_)
			<< BUILD_LOG(m_iTableType)
			<< BUILD_LOG(m_vecAttributeList.size());
		return false;
	}

	if (GetTableType() == ATT_FIXED)
	{
		for (int i = 0; i < iCount_; ++i)
		{
			if (i >= (int)m_vecAttributeList.size())
				break;

			SAttribute& sAtt = m_vecAttributeList[i];
			AttributeList_.push_back(std::pair<int, float>(sAtt.m_iType, sAtt.m_fValue));
		}
		return true;
	}
	KLottery kLottery = m_kLottery;
	for (int i = 0; i < iCount_; ++i)
	{
		int iIndex = kLottery.Decision();
		kLottery.RemoveCase(iIndex);

		SAttribute& sAtt = m_vecAttributeList[iIndex];
		AttributeList_.push_back(std::pair<int, float>(sAtt.m_iType, sAtt.m_fValue));
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

bool KGCAttributeTable::LoadScript(int iIndex_, KLuaManager& luaMgr)
{
	if (FAILED(luaMgr.BeginTable(iIndex_))) {
		return false;
	}

	if (FAILED(luaMgr.GetValue("AttributeNum", m_iIndex)))
	{
		START_LOG(cerr, L"AttributeNum이 비어 있습니다. Index : " << iIndex_);
		m_iTableType = ATT_INVAILD;
		luaMgr.EndTable();
		return false;
	}

	if (FAILED(luaMgr.GetValue("Grade", m_nGrade)))
	{
		START_LOG(cerr, L"Grade이 비어 있습니다. Index : " << iIndex_);
		m_nGrade = -1;
		luaMgr.EndTable();
		return false;
	}

	if (FAILED(luaMgr.GetValue("Type", m_iTableType)))
	{
		START_LOG(cerr, L"Type이 비어 있습니다. Index : " << iIndex_);
		m_iTableType = ATT_INVAILD;
		luaMgr.EndTable();
		return false;
	}

	if (SUCCEEDED(luaMgr.BeginTable("AttributeInfo"))) {
		int iIndex = 0;
		while (++iIndex)
		{
			if (FAILED(luaMgr.BeginTable(iIndex))) {
				break;
			}

			SAttribute sAtt;
			if (FAILED(luaMgr.GetValue(1, sAtt.m_iType)))
			{
				START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 1");
				luaMgr.EndTable();
				continue;
			}

			if (FAILED(luaMgr.GetValue(2, sAtt.m_fValue)))
			{
				START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 2");
				luaMgr.EndTable();
				continue;
			}

			if (FAILED(luaMgr.GetValue(3, sAtt.m_fProb)))
			{
				START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 3");
				luaMgr.EndTable();
				continue;
			}

			m_vecAttributeList.push_back(sAtt);
			m_kLottery.AddCase(static_cast<int>(m_vecAttributeList.size() - 1), sAtt.m_fProb);

			luaMgr.EndTable();
		}
		luaMgr.EndTable();
	}
	else {
		START_LOG(cerr, L"AttributeInfo 읽기 실패. Index : " << m_iIndex);
	}

	if (SUCCEEDED(luaMgr.BeginTable("RandomAttributeIndex"))) {
		m_setRandomAttributeList.clear();
		int iIndex = 0;
		while (++iIndex)
		{
			int iRandAttributeType = 0;
			if (FAILED(luaMgr.GetValue(iIndex, iRandAttributeType)))
			{
				break;
			}
			m_setRandomAttributeList.insert(iRandAttributeType);
		}
		luaMgr.EndTable();
	}
	else {
		START_LOG(cerr, L"AttributeInfo 읽기 실패. Index : " << m_iIndex);
	}

	luaMgr.EndTable();

	return true;
}

bool KGCAttributeTable::SaveTable(IN OUT std::ofstream& file, int iTab)
{
	file << GetTab(iTab) << "[" << m_iIndex << "] = {\n";
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
		START_LOG(cerr, L"You get wrong enum!!");
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
		START_LOG(cerr, L"You get wrong enum!!");
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