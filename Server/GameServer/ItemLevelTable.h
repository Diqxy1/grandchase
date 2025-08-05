#pragma once
#include "Lottery.h"
#include "Lua/KLuaManager.h"
#include <vector>
#include <set>

struct KItemLevelRatio
{
    int         m_nType;
    int         m_nAttributeNum;
    float       m_fProb;
};

class KItemLevelTable
{
public:
    KItemLevelTable(void);
    ~KItemLevelTable(void);

    enum ItemLevelTableType
    {
        ILT_INVAILD      = -1,
        ILT_RANDOM       = 0,
        ILT_SELECTIVE    = 1,
        ILT_FIXED        = 2,
        ILT_NUM,
    };

    enum ItemLevelType
    {
        IL_EQUIP_LV = 0, // 장착레벨
        IL_EQUIP_DOWN_LV = 1, // 장착레벨 감소치
    };

private:
    int         m_nIndex;
    int         m_nTableType;
    KLottery    m_kLottery;

    std::vector<KItemLevelRatio> m_vecLevelRatio;

public:
    bool IsRandomType();
    bool IsSelectiveType();
    bool IsFixedType();

    bool LoadScript( IN int nIndex, IN OUT KLuaManager& kLuaMng );
    ItemLevelTableType GetItemLevelType() { return (ItemLevelTableType)m_nTableType; }
    bool GetLevelList( OUT std::set<int>& setLevelList );
    bool GetItemLevel( OUT int& nLevel );
    bool CheckItemLevel( IN const int nLevel );
    bool GetItemAttributeNum( IN const int nLevel, OUT int& nAttributeNum );
};