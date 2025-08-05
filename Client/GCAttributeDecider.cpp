#include <stdafx.h>
#include "GCAttributeDecider.h"
#include <fstream>
#include <dbg/dbg.hpp>

FILE_NAME_FOR_LOG

KGCAttributeDecider::KGCAttributeDecider(void)
{
}

KGCAttributeDecider::~KGCAttributeDecider(void)
{
}

bool KGCAttributeDecider::GetAttributeList(IN int nIndex_, IN int nGrade, IN int nCount_, OUT std::list<std::pair<int, float>>& AttributeList_)
{
	Table kTable;

	if (!GetTable(nIndex_, nGrade, kTable))
		return false;

	if (GetTableType(nIndex_, nGrade) != Table::ATT_SELECTIVE)
		return kTable.GetAttribute(nCount_, AttributeList_);

	for (int i = 0; i < nCount_; ++i)
	{
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
		START_LOG(cerr, L"File is not opended!!!");
		return false;
	}

	Table::SaveEnum(file);

	file << "\n\nItemAttributeTable = {\n";

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
	m_mapTables.clear();
	KLuaManager luaMgr;

	_JIF(GCFUNC::LoadLuaScript(luaMgr, "ItemAttributeTable.lua"), return false);

	int iIndex = 0;

	_JIF(luaMgr.BeginTable("ItemAttributeTable") == S_OK, return false);

	TableList   mapTables;
	while (++iIndex)
	{
		Table kTable;

		if (kTable.LoadScript(iIndex, luaMgr) == false) {
			break;
		}

		std::pair<int, int> pair;
		pair.first = kTable.GetIndex();
		pair.second = kTable.GetGrade();

		m_mapTables.insert(TableList::value_type(pair, kTable));
	}
	luaMgr.EndTable();

	return true;
}

bool KGCAttributeDecider::GetTable(IN int iTableIndex, IN int nGrade, OUT Table& kTable_)
{
	TableList::iterator mit = m_mapTables.find(std::pair<int, int>(iTableIndex, -1));

	if (mit != m_mapTables.end())
	{
		kTable_ = mit->second;
		return true;
	}

	mit = m_mapTables.find(std::pair<int, int>(iTableIndex, nGrade));

	if (mit != m_mapTables.end())
	{
		kTable_ = mit->second;
		return true;
	}

	return false;
}

int KGCAttributeDecider::AddTable(IN Table& kTable_)
{
	std::pair<int, int> pair;
	pair.first = kTable_.GetIndex();
	pair.second = kTable_.GetGrade();

	m_mapTables.insert(TableList::value_type(pair, kTable_));
	return kTable_.GetIndex();
}

KGCAttributeDecider::Table::AttributeTableType KGCAttributeDecider::GetTableType(IN int nIndex_, IN int nGrade)
{
	Table kTable;

	if (!GetTable(nIndex_, nGrade, kTable))
		return KGCAttributeDecider::Table::ATT_INVAILD;

	return kTable.GetTableType();
}

bool KGCAttributeDecider::GetAttributeFullList(IN int nIndex_, IN int nGrade, OUT std::list<std::pair<int, float>>& AttributeList_)
{
	Table kTable;

	if (!GetTable(nIndex_, nGrade, kTable))
		return false;

	return kTable.GetFullAttribute(AttributeList_);
}