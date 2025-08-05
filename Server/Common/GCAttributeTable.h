#pragma once
#include "Lottery.h"
#include "Lua/KLuaManager.h"
#include <list>
#include <set>

class KGCAttributeTable
{
public:
	KGCAttributeTable(void);
	~KGCAttributeTable(void);

	enum AttributeTableType {
		ATT_INVAILD = -1,
		ATT_RANDOM = 0,
		ATT_SELECTIVE = 1,
		ATT_FIXED = 2,
		ATT_NUM,
	};

	enum AttributeType {
		AT_INVAILD = -1,
		AT_VITALITY = 0,
		AT_DEFENCE = 1,
		AT_ATTACK = 2,
		AT_CRITICAL = 3,
		AT_COUNTER = 4,

		AT_NUM,
	};

	struct SAttribute {
		int     m_iType;
		float   m_fValue;
		float   m_fProb;
	};

	typedef std::vector<SAttribute> ElementList;
private:
	std::pair<int, int> m_prAttributeInfo;
	int         m_iTableType;
	KLottery    m_kLottery;

	ElementList m_vecAttributeList;

	std::set<DWORD> m_setRandomAttributeIndex;

	std::string GetTab(int iTab)
	{
		std::stringstream stm;
		while (iTab--)
		{
			stm << "\t";
		}
		return stm.str();
	}

public:
	bool IsFixed();
	bool IsSelective();
	bool IsRandom();

	bool GetAttribute(IN int iCount_, OUT std::list<std::pair<int, float>>& AttributeList_);
	bool GetExceptAttribute(IN const int nCount_, IN const std::list< std::pair< int, float > >& lstExceptAttributeList_, OUT std::list< std::pair< int, float > >& lstAttributeListOut_);
	bool GetFullAttribute(OUT std::list<std::pair<int, float>>& AttributeList_);
	bool LoadScript(IN std::pair<int, int> prAttributeInfo_, IN OUT KLuaManager& luaMgr);
	bool SaveTable(IN OUT std::ofstream& file, int iTab);
	const ElementList& GetContainer() const { return m_vecAttributeList; }

	AttributeTableType GetTableType() { return (AttributeTableType)m_iTableType; }
	static void SaveEnum(IN OUT std::ofstream& file);
	static std::string GetTableTypeEnumString(int iEnum);
	static std::string GetAttributeTypeEnumString(int iEnum);

	bool LoadRandomAttribute(IN KLuaManager& kLuaMng, OUT std::set<DWORD>& setRandomAttribute);
	bool GetRandomAttributeIndex(OUT std::set<DWORD>& setRandomAttributeIndex);
	bool CheckRandomAttributeIndex(IN const DWORD dwType);
};
