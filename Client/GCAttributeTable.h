#pragma once
#include "Lottery.h"
#include "KLuaManager.h"
#include <list>

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
	ElementList m_vecAttributeList;
private:
	int         m_iIndex;
	int         m_iTableType;
	int         m_nGrade;
	KLottery    m_kLottery;

	std::set<DWORD> m_setRandomAttributeList;

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
	bool GetFullAttribute(OUT std::list<std::pair<int, float>>& AttributeList_);
	bool LoadScript(IN int iIndex, IN OUT KLuaManager& luaMgr);
	bool SaveTable(IN OUT std::ofstream& file, int iTab);
	const ElementList& GetContainer() const { return m_vecAttributeList; }
	const std::set<DWORD>& GetRandomContaner() const { return m_setRandomAttributeList; }

	AttributeTableType GetTableType() const { return (AttributeTableType)m_iTableType; }
	static void SaveEnum(IN OUT std::ofstream& file);
	static std::string GetTableTypeEnumString(int iEnum);
	static std::string GetAttributeTypeEnumString(int iEnum);

	int GetIndex() { return m_iIndex; }
	int GetGrade() { return m_nGrade; }
	bool IsRandomType(int iAttributeType) const { return m_setRandomAttributeList.find(iAttributeType) != m_setRandomAttributeList.end(); }
};
