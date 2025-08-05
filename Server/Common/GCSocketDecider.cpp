#include "GCSocketDecider.h"
#include <fstream>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KGCSocketDecider::KGCSocketDecider(void)
{
}

KGCSocketDecider::~KGCSocketDecider(void)
{
}

bool KGCSocketDecider::GetSocketList(IN int iIndex, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/)
{
	return _GetSocketList(iIndex, m_mapSocketTables, SocketList_, bDump);
}

bool KGCSocketDecider::GetBasicSocketList(IN int iGrade, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/)
{
	return _GetSocketList(iGrade, m_mapBasicTables, SocketList_, bDump);
}

bool KGCSocketDecider::_GetSocketList(IN int iIndex, IN TableList& mapList_, OUT std::list<int>& SocketList_, IN bool bDump /*= false*/)
{
	KLocker lock(m_csTables);
	TableList::iterator mit = mapList_.find(iIndex);

	if (mit == mapList_.end())
		return false;

	return mit->second.GetSocketList(SocketList_, bDump);
}

bool KGCSocketDecider::LoadScript()
{
	return _LoadScript(std::string());
}

bool KGCSocketDecider::_LoadScript(OUT std::string& strScript_)
{
	KLuaManager luaMgr;
	std::map<int, KGCSocketTable> mapSocketTables;
	std::map<int, KGCSocketTable> mapBasicTables;

	const std::string strScriptName = "ItemSocketTable.lua";
	strScript_ = strScriptName;
	_JIF(luaMgr.DoFile(strScriptName.c_str()) == S_OK, return false);

	_JIF(luaMgr.BeginTable("ItemSocketTable") == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (luaMgr.BeginTable(i) != S_OK) break;
		KGCSocketTable kTable;
		int nSocketNum;
		_JIF(luaMgr.GetValue("SocketNum", nSocketNum) == S_OK, return false);

		if (kTable.LoadScript(nSocketNum, luaMgr) == false) {
			break;
		}
		_JIF(luaMgr.EndTable() == S_OK, return false);

		mapSocketTables.insert(TableList::value_type(nSocketNum, kTable));
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	_JIF(luaMgr.BeginTable("DefaultSocketTable") == S_OK, return false);

	for (int i = 1; ; ++i) {
		if (luaMgr.BeginTable(i) != S_OK) break;
		KGCSocketTable kTable;
		int nSocketNum;
		_JIF(luaMgr.GetValue("SocketNum", nSocketNum) == S_OK, return false);

		if (kTable.LoadScript(nSocketNum, luaMgr) == false) {
			break;
		}
		_JIF(luaMgr.EndTable() == S_OK, return false);

		mapBasicTables.insert(TableList::value_type(nSocketNum, kTable));
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	KLocker lock(m_csTables);
	m_mapSocketTables.swap(mapSocketTables);
	m_mapBasicTables.swap(mapBasicTables);

	return true;
}

bool KGCSocketDecider::SaveScript(IN std::string strFileName_)
{
	std::ofstream file;

	file.open(strFileName_.c_str());

	if (file.is_open() == false)
	{
		START_LOG(cerr, L"File is not opened!!!") << END_LOG;
		return false;
	}

	KGCSocketTable::SaveEnum(file);

	file << "\n\n-- Socket Table for each Grade\n";
	SaveList(file, "DefaultSocketTable", m_mapBasicTables);
	file << "-- Socket Table for special items\n";
	SaveList(file, "ItemSocketTable", m_mapSocketTables);
	return true;
}

bool KGCSocketDecider::SaveList(std::ofstream& file, std::string strTableName, TableList& mapList)
{
	file << strTableName << " = {\n";

	TableList::iterator mit;
	KLocker lock(m_csTables);
	for (mit = mapList.begin(); mit != mapList.end(); ++mit)
	{
		mit->second.SaveTable(file, 1);
	}

	file << "}\n\n\n";

	return true;
}