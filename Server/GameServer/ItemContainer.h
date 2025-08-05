#pragma once
#include "KncSmartPtr.h"
#include "PrimitiveTypedef.h"
#include <map>
#include <vector>
#include "CommonPacket.h"
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

SmartPointerStruct( KItem );
WeakPointerStruct( KItem );

class KItemContainer
{
public:
    KItemContainer(void);
    ~KItemContainer(void);

    KItemPtr FindItemByItemID( IN const GCITEMID ItemID );
    KItemPtr FindItemByItemIDExItemUID( IN const GCITEMID ItemID, IN const std::vector<GCITEMUID>& vecExItemUID );
    KItemPtr FindItemByItemUID( IN const GCITEMUID ItemUID );
    KItemPtr FindItem( IN const GCITEMID ItemID, IN const GCITEMUID ItemUID );
    KItemPtr FindDurationItem( IN const GCITEMID ItemID, IN const GCITEMUID ItemUID );
    KItemPtr FindDurationItemByItemID( IN const GCITEMID ItemID );
    KItemPtr FindDurationItemByItemUID( IN const GCITEMUID ItemUID );
    KItemPtr FindItemByItemIDandLevel( IN const GCITEMID ItemID_, IN const int nLevel_ );

    bool AddItem( IN const KItem& kItem );
    int AddItemList( IN OUT std::vector< KItem >& vecItemList );
    int AddItemListNoUpdate( IN OUT std::vector< KItem >& vecItemList );    
    bool RemoveItem( IN const GCITEMUID ItemUID );
    bool RemoveItem( IN const GCITEMID ItemID, IN const GCITEMUID ItemUID );
    void AdjustQueryInventory( IN const int nType, IN OUT std::vector< KItem >& vecList );
    void GetExpiredItemList( OUT std::vector< KItem >& vecList );

    void GetDiffDurationItemAndUpdate( OUT std::vector< KItem >& vecDiffItems );

    DWORD GetTotalItemNum() { return m_mapItems.size(); }
    int GetTotalEnableSize();
    int GetLookItemNum();
    int GetAgitObjectItemNum();

    void FindItemUIDsByItemID( IN const GCITEMID ItemID, OUT std::vector<GCITEMUID>& vecItemUIDs );
    int GetItemCount( IN const GCITEMID ItemID );

    KItemPtr FindDepotItemByKey( IN const KDepotKey& depotKey_ );

    bool MoveInvenToDepot( IN KItem& kItem_, IN const KDepotKey& depotKey_ );
    bool MoveInvenToDepotCount( IN KItem& kItem_, IN const KDepotItem& kDepotItem_, OUT KItem& kDepotResultItem_ );
    bool CopyInvenToDepot( IN KItem& kItem_, IN const KDepotKey& depotKey_, IN const int& nCount_, OUT KItem& kDepotItem_ );

    bool ChangeDepotSlot( IN KItem& kItem_, IN KDepotKey& depotKey_, IN KDepotKey& newDepotKey_ );
    void FindDepotItemByTab( IN const char tabID_, OUT std::vector<KItem>& vecItem_ );

    int AddDepotItemList( IN OUT std::vector<KItem>& vecItemList_ );
    KItemPtr FindDepotItemByItemUID( IN const GCITEMUID ItemUID_ );
    bool AddDepotItem( IN const KItem& kItem_ );
    bool MoveDepotToInven( IN KItem& kItem_, IN const KDepotKey& depotKey_ );
    bool MoveDepotToInvenCountItem( IN KItem& kItem_, IN const KDepotKey& depotKey_ );
    bool MoveDepotToInvenCount( IN KItem& kInvenItem_, IN const KDepotItem& kDepotItem_, OUT KItem& kDepotResultItem_ );
    bool CopyDepotToInven( IN KItem& kItem_, IN const KDepotKey& depotKey_, IN const int& nCount_, OUT KItem& kInvenItem_ );

    bool RemoveDepotItem( IN const GCITEMUID ItemUID_ );
    bool RemoveDepotItemFrommmap( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ );
    void GetDiffInvenTypeItem( OUT std::vector< KItem >& vecDiffItems_, OUT std::vector< KItem >& vecDeleteDepotItems_, 
        OUT std::vector< KItem >& vecDiffCountItems_, OUT std::vector< KItem >& vecDeleteDepotCountItems_ );

    void GetStrengthItemList( IN const GCITEMID ItemID, IN const int nGrade, IN const int nLv, OUT std::map<char,std::map<char,std::set<GCITEMUID> > >& mapList );
    KItemPtr FindDepotItemByItemID( IN const GCITEMID ItemID );
    KItemPtr FindDepotItemByItemIDandLevel( IN const GCITEMID ItemID, IN const int nLevel );

    void DumpInventory( IN const std::wstring& wstrName_, IN const DWORD dwUID_ );
    void MakeCharacterInventory( IN const char charType_, std::vector<KItem>& vecList_ ); // 테스트용. 캐릭터 타입의 아이템만 인벤토리에 남기기.
    bool DeleteDepotItem( IN KItem& kItem_, IN const KDepotKey& depotKey_ );

private:
    void ClearItem();
    bool RemoveItemFrommmap( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ );
    bool UpdateItem( IN OUT KItem& oldItem, IN const KItem& newItem );
    bool UpdateItemNoUpdate( IN OUT KItem& oldItem, IN const KItem& newItem );
    bool UpdateCountItem( IN OUT KItem& oldItem, IN const KItem& newItem );

private:
    typedef std::map< GCITEMUID, KItemPtr > ItemMap;
    typedef std::multimap< GCITEMID, KItemWPtr > ItemMMap;
    typedef std::map< KDepotKey, KItemWPtr > DepotItemKeyMap;
    typedef std::map< GCITEMID, KItemWPtr > DepotItemIDMap;

    ItemMap m_mapItems;
    ItemMMap m_mmapItems;

    ItemMap m_mapDepotItems; // ItemUID 로 검색하기 위한 용도
    ItemMMap m_mmapDepotItems;

    DepotItemKeyMap m_mapDepotItem; // 창고 아이템 리스트 관리

    ItemMap m_mapReserveDelDepotItem; // 창고에서 삭제할 수량 아이템 임시보관
    
};
