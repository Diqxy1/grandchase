#pragma once
#include "ItemLevelTable.h"
#include <Thread/Locker.h>
#include "PrimitiveTypedef.h"

class KItemLevelDecider
{
public:
    KItemLevelDecider(void);
    ~KItemLevelDecider(void);

    typedef KItemLevelTable         LevelTable;

private:
    mutable KncCriticalSection          m_csLevelDecider;
    std::map< int, LevelTable >    m_mapItemLevelInfo;

    bool LoadItemLevelTable( IN KLuaManager& kLuaMng, OUT std::map< int, LevelTable >& mapItemLevelInfo );

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetItemLevelTable( IN int nTableIndex, OUT LevelTable& kItemLevelTable );
    bool GetItemLevelList( IN int nTableIndex, OUT std::set<int>& setLevelList );
    LevelTable::ItemLevelTableType GetTableType( IN int nTableIndex );
    bool GetItemLevel( IN int nTableIndex, OUT int& nLevel );
    bool CheckItemLevel( IN const int nTableIndex, IN const int nLevel );
    bool CheckItemLevelAttribute( IN const int nLevelTableID, IN OUT int& nAttributeID, IN OUT int& nItemLevel, IN const int nLevelTableType );
    bool IsSelectiveType( IN const int nLevelTableID );
    bool IsRandomType( IN const int nLevelTableID );
};
