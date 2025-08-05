#pragma once
#include <Thread/Locker.h>
#include <set>
#include <map>
#include "PrimitiveTypedef.h"

class KLuaManager;

class KGCItemConfig
{
public:
    KGCItemConfig(void);
    ~KGCItemConfig(void);

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetNonInvenSizeItemList( OUT std::set<GCITEMID>& setNonInvenSizeItemList );
    bool IsNoneSizeItem( IN const GCITEMID ItemID );
    void GetHideInvenItemList( OUT std::set<GCITEMID>& setHideInvenItemList );
    bool GetNickNameChangeItemInfo( IN const int nType, OUT GCITEMID& ItemID );

private:
    bool LoadSetItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setItemList );
    bool LoadNickNameChangeItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, GCITEMID>& mapNickNameChangeItem );

private:
    mutable KncCriticalSection      m_csItemConfig;
    std::set<GCITEMID>              m_setNonInvenSizeItem; // 인벤토리 차지 하지 않는 아이템 리스트.
    std::set<GCITEMID>              m_setHideInvenItem; // 인벤토리 보이지 않는 아이템 리스트.
    std::map<int,GCITEMID>          m_mapNickNameChangeItem; // 닉네임 변경 아이템 리스트.
};
