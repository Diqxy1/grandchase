#pragma once
#include "GCGradeTable.h"
#include <Thread/Locker.h>
#include "PrimitiveTypedef.h"
#include <map>
#include <set>
#include "UserPacket.h"

struct KGradeUpgradeInfo
{
	enum {
		IT_TOTAL = 0,
		IT_COORDI = 1,
		IT_MAX,
	};

	GCITEMID                m_GradeUpgradeItemID; // 등급 업그레이드 아이템.
	std::map<char, int>      m_mapGradeUseCount;   // 등급 업그레이드시 아이템 사용 갯수
	int                     m_nEnableItemType;    // 허용 ItemType.
	std::set<GCITEMID>      m_setUpgradeBanItem;  // 업그레이드 불가 아이템.
};

class KGCGradeDecider
{
public:
	KGCGradeDecider(void);
	~KGCGradeDecider(void);

	typedef KGCGradeTable               Table;
	typedef std::map<int, KGCGradeTable> TableList;
private:
	mutable KncCriticalSection      m_csTable;
	std::map<int, KGCGradeTable>     m_mapTables;
	KGradeUpgradeInfo               m_kGradeUpgradeInfo;

	int GetFreeIndex() const;
	bool LoadGradeUpgradeInfo(IN KLuaManager& kLuaMng, OUT KGradeUpgradeInfo& kGradeUpgradeInfo);
	bool LoadUseCount(IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<char, int>& mapGradeUseCount);
	bool LoadSetItems(IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setItems);

public:
	int GetGrade(int iTableIndex);
	size_t GetTableCount() { KLocker lock(m_csTable); return m_mapTables.size(); }
	const TableList& GetContainer() { return m_mapTables; }
	bool GetTable(IN int iTableIndex, OUT Table& kTable_);
	int AddTable(IN Table& kTable_);

	bool LoadScript();
	bool _LoadScript(OUT std::string& strScript);
	bool SaveScript(IN std::string strFileName_);

	std::string DumpTables();
	bool CheckGradeUpgradeItem(IN const GCITEMID& ItemID);
	bool GetGradeUpgradeUseCount(IN const char& cGrade, OUT int& nUseCount);
	bool IsCoordiItemType();
	bool CheckUpgradePossibleItem(IN const GCITEMID& ItemID);
	void GetCoordiUpgradeInfo(OUT KEVENT_COORDI_GRADE_UPGRADE_INFO_ACK& kPacket);
};
