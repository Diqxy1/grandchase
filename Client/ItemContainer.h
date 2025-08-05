#pragma once
#include "KncSmartPtr.h"
#include "CommonPacket.h"

//class GCItemManager;

class KItemContainer
{
public:
    KItemContainer(void);
    ~KItemContainer(void);

    typedef std::map< GCITEMUID, KItem > ItemList;
    typedef std::set< GCITEMUID > newitemlist_type;
private:
    std::map< GCITEMUID, KItem > m_mapItems;
    std::set< GCITEMUID > m_setNewItem;

public:
    KItem* FindItemByItemID( IN const GCITEMID ItemID );
    KItem* FindItemByItemIDCoordi( IN const GCITEMID ItemID );
    KItem* FindItemByItemIdAndLevel( IN const GCITEMID ItemID_, int iLevel_ );
    KItem* FindItemByItemUID( IN const GCITEMUID ItemUID );
    const KItem* FindItem( IN const GCITEMID ItemID, IN const GCITEMUID ItemUID );
    bool AddItem( IN const KItem& kItem, IN const bool bConvertToClientStyle = false );
    bool UpdateItem( IN OUT KItem& oldItem, IN const KItem& newItem );
    int AddItemList( IN OUT std::vector< KItem >& vecItemList, IN const bool bConvertToClientStyle = false );
    bool RemoveItem( IN const GCITEMUID ItemUID );
    bool RemoveItem( IN const GCITEMID ItemID, IN const GCITEMUID ItemUID );
    bool RemoveItemList( IN const std::vector< KItem >& vecItemList );
    bool RemoveItemList( IN const std::vector< GCITEMUID >& vecItemUIDList );
    void ClearItem();
    void AdjustQueryInventory( IN OUT std::vector< KItem >& vecList );
    void GetExpiredItemList( OUT std::vector< KItem >& vecList );
    void GetZeroCountItemList( OUT std::vector< KItem >& vecList );
    const ItemList& GetFullItemList() { return m_mapItems; }
    int GetSize( IN const std::set<GCITEMID>& setExceptionItemIDs = std::set<GCITEMID>() );
    int GetInvenItemSize( IN const std::set<GCITEMID>& setExceptionItemIDs = std::set<GCITEMID>() );
    std::vector<KItem> GetItemListInRange( IN const std::set<GCITEMID>& setRange );
    bool IsItemByItemID( IN const GCITEMID ItemID_ );

    void AddNewItem( GCITEMUID itemUID_ );
    void ClearNewItem();
    bool IsNewItem( GCITEMUID itemUID_ );

    void UpdatePremiumInfo();

    bool FindItemListByItemID( IN const GCITEMID itemID_, OUT std::vector< KItem* >& vecItemList_ );

    void LogInventory( IN const std::set<GCITEMID>& setExceptionItemIDs = std::set<GCITEMID>() );
};
