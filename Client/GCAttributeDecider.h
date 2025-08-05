#pragma once
#include "GCAttributeTable.h"
#include <Thread/Locker.h>

class KGCAttributeDecider
{
public:
	KGCAttributeDecider(void);
	~KGCAttributeDecider(void);

	typedef KGCAttributeTable               Table;
	typedef std::map<std::pair<int, int>, KGCAttributeTable> TableList;
	typedef std::list<std::pair<int, float>> AttList;

private:
	std::map<std::pair<int, int>, KGCAttributeTable>    m_mapTables;

public:
	bool LoadScript();
	bool SaveScript(IN std::string strFileName_);

	size_t GetTableCount() { return m_mapTables.size(); }
	const TableList& GetContainer() const { return m_mapTables; }
	bool GetTable(IN int iTableIndex, IN int nGrade, OUT Table& kTable_);
	int AddTable(IN Table& kTable_);

	Table::AttributeTableType GetTableType(IN int nIndex_, IN int nGrade);
	bool GetAttributeList(IN int nIndex_, IN int nGrade, IN int nCount_, OUT std::list<std::pair<int, float>>& AttributeList_);
	bool GetAttributeFullList(IN int nIndex_, IN int nGrade, OUT std::list<std::pair<int, float>>& AttributeList_);
};
