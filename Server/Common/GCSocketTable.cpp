#include "GCSocketTable.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG
KGCSocketTable::KGCSocketTable(void)
	: m_iTotalCount(0)
	, m_iIndex(0)
{
}

KGCSocketTable::~KGCSocketTable(void)
{
}

bool KGCSocketTable::LoadScript(IN int iIndex_, IN OUT KLuaManager& luaMgr)
{
	std::map<SocketType, SSocket> mapSocketList;
	_JIF(luaMgr.BeginTable("SocketInfo") == S_OK, return false);

	m_iIndex = iIndex_;

	int iIndex = 0;
	while (++iIndex)
	{
		if (FAILED(luaMgr.BeginTable(iIndex)))
			break;

		SSocket sSocket;
		if (FAILED(luaMgr.GetValue(1, sSocket.m_iType)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 1") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		if (FAILED(luaMgr.GetValue(2, sSocket.m_iCount)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 2") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		if (FAILED(luaMgr.GetValue(3, sSocket.m_fProb)))
		{
			START_LOG(cerr, L"스크립트를 체크 해 보세요. Index : " << iIndex << ", value : 3") << END_LOG;
			luaMgr.EndTable();
			continue;
		}

		mapSocketList.insert(std::map<SocketType, SSocket>::value_type(static_cast<SocketType>(sSocket.m_iType), sSocket));
		m_iTotalCount += sSocket.m_iCount;

		luaMgr.EndTable();
	}
	_JIF(luaMgr.EndTable() == S_OK, return false);

	m_mapSocketList.swap(mapSocketList);
	return true;
}

bool KGCSocketTable::SaveTable(IN OUT std::ofstream& file, IN int iTab)
{
	file << GetTab(iTab) << "[" << m_iIndex << "] = {\n";

	std::map<SocketType, SSocket>::iterator mit;
	for (mit = m_mapSocketList.begin(); mit != m_mapSocketList.end(); ++mit)
	{
		SSocket& Val = mit->second;
		file << GetTab(iTab + 1) << "{ "
			<< GetEnumString(Val.m_iType) << ",\t"
			<< Val.m_iCount << ",\t"
			<< Val.m_fProb << "\t},\n";
	}
	file << GetTab(iTab) << "},\n";
	return true;
}

bool KGCSocketTable::GetSocketList(OUT std::list<int>& SocketList_, IN bool bDump /*= false*/)
{
	if (m_mapSocketList.empty()) {
		return true;
	}

	if (bDump) {
		START_LOG(clog, L"=== Socket Info Dump ===") << END_LOG;
	}

	std::map<SocketType, SSocket>::iterator mit;
	for (mit = m_mapSocketList.begin(); mit != m_mapSocketList.end(); ++mit)
	{
		for (int i = 0; i < mit->second.m_iCount; ++i)
		{
			int iType = mit->second.m_iType;

			if (iType == ST_RANDOM)
			{
				KLottery kLottery;
				kLottery.EnableState(KLottery::ENABLE_NO_FAILURE);
				kLottery.AddCase(ST_OPENED, mit->second.m_fProb);
				kLottery.AddCase(ST_CLOSED, 100.f - mit->second.m_fProb);

				iType = kLottery.Decision();
			}
			if (bDump)
			{
				//std::clog<<L"Listed Socket Type : "<<iType<<ENDL;
			}
			SocketList_.push_back(iType);
		}
	}

	return true;
}

std::string KGCSocketTable::GetEnumString(int iEnum)
{
	switch (iEnum)
	{
	case ST_OPENED:
		return "OPENED_SOCKET";
	case ST_CLOSED:
		return "CLOSED_SOCKET";
	case ST_RANDOM:
		return "RANDOM_SOCKET";
	default:
		START_LOG(cerr, L"You get wrong grade enum!!") << END_LOG;
		return "NULL";
		break;
	}
}

void KGCSocketTable::SaveEnum(IN OUT std::ofstream& file)
{
	for (int i = 0; i < ST_NUM; ++i)
	{
		file << GetEnumString(i) << "\t= " << i << "\n";
	}
}