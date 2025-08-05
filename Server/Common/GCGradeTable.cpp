#include "GCGradeTable.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

KGCGradeTable::KGCGradeTable(void)
	: m_iIndex(-1)
{
	m_kLottery.EnableState(KLottery::ENABLE_NO_FAILURE);
}

KGCGradeTable::~KGCGradeTable(void)
{
}

bool KGCGradeTable::IsFixed()
{
	return m_mapGradeList.size() == 1;
}

int KGCGradeTable::GetGrade()
{
	return m_kLottery.Decision();
}

bool KGCGradeTable::LoadScript(int iIndex_, KLuaManager& luaMgr)
{
	_JIF(luaMgr.BeginTable("GradeInfo") == S_OK, return false);
	m_iIndex = iIndex_;

	int iIndex = 0;
	while (++iIndex)
	{
		if (FAILED(luaMgr.BeginTable(iIndex)))
			break;

		int iGrade = 0;
		if (FAILED(luaMgr.GetValue(1, iGrade)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 1") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		float fProb = 0.0f;
		if (FAILED(luaMgr.GetValue(2, fProb)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 2") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		m_kLottery.AddCase(iGrade, fProb);
		m_mapGradeList.insert(ElementList::value_type(iGrade, fProb));

		luaMgr.EndTable();
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	return true;
}

bool KGCGradeTable::SaveTable(IN OUT std::ofstream& file, int iTab)
{
	file << GetTab(iTab) << "[" << m_iIndex << "] = {\n";

	ElementList::iterator mit;
	for (mit = m_mapGradeList.begin(); mit != m_mapGradeList.end(); ++mit)
	{
		file << GetTab(iTab + 1) << "{ "
			<< GetEnumString(mit->first) << ",\t"
			<< mit->second << "\t},\n";
	}
	file << GetTab(iTab) << "},\n";

	return true;
}

void KGCGradeTable::SaveEnum(IN OUT std::ofstream& file)
{
	for (int i = 0; i < GRADE_NUM; ++i)
	{
		file << GetEnumString(i) << "\t= " << i << "\n";
	}
}

std::string KGCGradeTable::GetEnumString(int iEnum)
{
	switch (iEnum)
	{
	case GRADE_NORMAL:
		return "NORMAL";
	case GRADE_RARE:
		return "RARE";
	case GRADE_EPIC:
		return "EPIC";
	case GRADE_LEGEND:
		return "LEGEND";
	case GRADE_MYSTIC:
		return "MYSTIC";
	default:
		START_LOG(cerr, L"You get wrong grade enum!!") << END_LOG;
		return "NULL";
		break;
	}
}

bool KGCGradeTable::SetGrade(int iGrade, float fProb)
{
	if (iGrade < 0 || iGrade >= GRADE_NUM)
		return false;

	if (fProb < 0.0f || fProb >= 101.f)
		return false;

	m_mapGradeList[iGrade] = fProb;
	return true;
}

float KGCGradeTable::GetTotalProb()
{
	float fTotalProb = 0.f;
	ElementList::iterator mit;
	for (mit = m_mapGradeList.begin(); mit != m_mapGradeList.end(); ++mit)
	{
		fTotalProb += mit->second;
	}
	return fTotalProb;
}

float KGCGradeTable::GetExtraProb()
{
	return 100.f - GetTotalProb();
}