#pragma once
#include "GCAttributeTable.h"
#include <Thread/Locker.h>
#include "PrimitiveTypedef.h"

#define DEFAULT_GRADE -1

struct KSetItemInfo
{
	std::vector<GCITEMID>   m_vecItemID;    // 구성 ItemID List
	std::map<int, float>     m_mapGpBonus;   // 세트개수, 효과
	std::map<int, float>     m_mapExpBonus;  // 세트개수, 효과

	KSetItemInfo()
	{
		m_vecItemID.clear();
		m_mapGpBonus.clear();
		m_mapExpBonus.clear();
	}
};

struct KChangeAttributeEventInfo
{
	bool                    m_bUseEnable;         // 기능 사용 여부.
	bool                    m_bUserCountCheckEnable; // 계정 Count별 체크 여부.
	time_t                  m_tmBegin;           // 속성 이벤트 시작 날짜.
	time_t                  m_tmEnd;             // 속성 이벤트 종료 날짜.
	DWORD                   m_dwUserChangeMaxCount; // 유저 속성 변경 MAX COUNT.
	std::set<int>           m_setChangeAttributeList; // 변경 가능한 속성 리스트.
	time_t                  m_tmReceiveTime; // 아이템 획득날짜 제어변수.

	KChangeAttributeEventInfo()
		:m_bUseEnable(false)
		, m_bUserCountCheckEnable(false)
		, m_tmBegin(1)
		, m_tmEnd(1)
		, m_dwUserChangeMaxCount(0)
		, m_tmReceiveTime(1)
	{
		m_setChangeAttributeList.clear();
	}
};

class KGCAttributeDecider
{
public:
	KGCAttributeDecider(void);
	~KGCAttributeDecider(void);

	typedef KGCAttributeTable               Table;
	typedef std::map<std::pair<int, int>, KGCAttributeTable> TableList;
	typedef std::list<std::pair<int, float>> AttList;

private:
	mutable KncCriticalSection          m_csTable;
	std::map<std::pair<int, int>, KGCAttributeTable>    m_mapTables;
	std::map< DWORD, std::set<GCITEMID> >  m_mapInitAttributeItemList; // map[ Grade, set[ItemID] ]
	std::map< DWORD, std::set<GCITEMID> >  m_mapInitGPAttributeItemList; // map[ Grade, set[ItemID] ] GP 속성 초기화 아이템
	std::map< DWORD, std::set<GCITEMID> >  m_mapRandomGPAttributeItemList; // map[ Grade, set[ItemID] ] GP 속성 초기화 아이템
	std::map< DWORD, std::set< GCITEMID > >  m_mapSingleRandomAttributeItemList; // 개별 랜덤 속성 주문서 아이템( TargetLevel, < ItemID > ). TargetItemLevel 이하의 대상 아이템의 속성만 변경할 수 있다

	// 세트 아이템 정보
	std::map<GCITEMID, GCITEMID>            m_mapItemSetID;     // ItemID, 세트ID
	std::map<GCITEMID, KSetItemInfo>        m_mapSetItemInfo;   // 세트ID, 세트정보(구성Item,보너스)

	// 속성 재설정 시스템 정보.
	KChangeAttributeEventInfo              m_kChangeAttributeEventInfo;

	int GetFreeIndex();
	bool LoadInitAttributeItemList(IN KLuaManager& kLuaMng, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	bool LoadInitGP_AttributeItemList(IN KLuaManager& kLuaMng, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	bool LoadRandomGP_AttributeItemList(IN KLuaManager& kLuaMng, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);

	bool LoadItems(IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	bool LoadSetItems(IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setItems);
	bool LoadItemAttributeTable(IN KLuaManager& lugMgr, OUT TableList& mapTables);
	bool LoadSetItemList(IN KLuaManager& lugMgr, OUT std::map<GCITEMID, GCITEMID>& mapItemSetID, OUT std::map<GCITEMID, KSetItemInfo>& mapSetItemInfo);
	bool LoadPartItemID(IN KLuaManager& lugMgr, OUT std::vector<GCITEMID>& vecItemID);
	bool LoadAttributeList(IN KLuaManager& lugMgr, OUT std::map<int, float>& mapGp, OUT std::map<int, float>& mapExp);
	bool LoadAttribute(IN KLuaManager& lugMgr, OUT std::map<int, float> & vecAtrribute);
	bool LoadEventMigrationAttributeInfo(IN KLuaManager& kLuaMng, OUT KChangeAttributeEventInfo& kChangeAttributeEventInfo);
	bool LoadTime(IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime);
	bool LoadSetData(IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setInfo);
	bool LoadSingleRandomAttributeItemList(IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::set< GCITEMID > >& mapItems_);

public:
	bool LoadScript();
	bool _LoadScript(OUT std::string& strScript);
	bool SaveScript(IN std::string strFileName_);

	size_t GetTableCount() { return m_mapTables.size(); }
	const TableList& GetContainer() { return m_mapTables; }
	bool GetTable(IN int iTableIndex, IN int nGrade, OUT Table& kTable);
	int AddTable(IN Table& kTable_);

	Table::AttributeTableType GetTableType(IN int nIndex, IN int nGrade);
	bool GetAttributeList(IN int nIndex, IN int nCount, IN char cGrade, OUT std::list<std::pair<int, float>>& AttributeList);
	bool GetAttributeFullList(IN int nIndex, IN int nGrade, OUT std::list<std::pair<int, float>>& AttributeList);
	void GetInitAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	void GetInitGPAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	void GetRandomGPAttributeItemList(OUT std::map< DWORD, std::set<GCITEMID> >& mapItems);
	bool CheckInitAttributeItem(IN const DWORD& dwGrade, IN const GCITEMID& itemID);
	bool GetRandomAttributeIndex(IN int nIndex, IN int nGrade, OUT std::set<DWORD>& setRandomAttributeIndex);
	bool CheckRandomAttributeIndex(IN int nIndex, IN int nGrade, IN const DWORD dwType);
	bool GetSelectiveAttributeList(IN int nIndex, IN int nCount, IN int nGrade, OUT std::list<std::pair<int, float>>& AttributeList);
	bool GetSelectiveAttributeExceptList(IN const int nIndex_, IN const int nCount_, IN const int nGrade_, IN const std::list< std::pair< int, float > >& lstExceptAttributeList_, OUT std::list< std::pair< int, float > >& lstAttributeListOut_);

	bool GetSetID(IN const GCITEMID& ItemID, OUT GCITEMID& dwSetID);
	bool GetSetInfo(IN const GCITEMID& dwSetID, OUT KSetItemInfo& kInfo);
	bool GetSetItemList(IN const GCITEMID& dwSetID, OUT std::vector<GCITEMID>& vecItemID);
	bool GetSetExpBonus(IN const GCITEMID& dwSetID, IN const int& nEquipCount, OUT float& fExpBonus);
	bool GetSetGpBonus(IN const GCITEMID& dwSetID, IN const int& nEquipCount, OUT float& fGpBonus);
	void DumpSetItemInfo(IN const GCITEMID& dwSetID);

	bool CheckInitGP_AttributeItem(IN const DWORD& dwGrade, IN const GCITEMID& itemID);
	bool CheckRandomGP_AttributeItem(IN const DWORD& dwGrade, IN const GCITEMID& itemID);

	bool CheckMigrationAttributeEventEnable() { KLocker lock(m_csTable); return m_kChangeAttributeEventInfo.m_bUseEnable; }
	bool IsEventTerm();
	bool CheckUserCountCheckEnable() { KLocker lock(m_csTable); return m_kChangeAttributeEventInfo.m_bUserCountCheckEnable; }
	DWORD GetUserChangeMaxCount() { KLocker lock(m_csTable); return m_kChangeAttributeEventInfo.m_dwUserChangeMaxCount; }
	void GetChangeAttributeList(OUT std::set<int>& setChangeAttributeList);
	bool CheckChangeAttribute(IN char cType);
	bool CheckItemChangePossible(IN const time_t& tmRegDate);
	void GetSingleRandomAttributeItemList(OUT std::map< DWORD, std::set< GCITEMID > >& mapItems_);
	bool CheckSingleRandomAttributeItemLevel(IN const DWORD dwItemLevel_, IN const GCITEMID dwItemID_);
};
