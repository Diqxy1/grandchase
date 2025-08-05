#include "ItemContainer.h"
#include <dbg/dbg.hpp>
#include "IPAdvantage.h"
#include <boost/bind.hpp>
#include "CommonPacket.h"
#include "ItemManager.h"
#include "LookEquipment.h"
#include <boost/foreach.hpp>
#include "ThreadManager.h"
#include "Worker.h"
#include "Log4.h"

KItemContainer::KItemContainer(void)
{
}

KItemContainer::~KItemContainer(void)
{
}

KItemPtr KItemContainer::FindItemByItemID( IN const GCITEMID ItemID_ )
{
    ItemMap::iterator mit = m_mapItems.begin();
    for( ; mit != m_mapItems.end(); ++mit)
    {
        if( ItemID_ == mit->second->m_ItemID )
            return mit->second;
    }
   
    return KItemPtr();
}

KItemPtr KItemContainer::FindItemByItemIDandLevel( IN const GCITEMID ItemID_, IN const int nLevel_ )
{
    ItemMap::iterator mit = m_mapItems.begin();
    for ( ; mit != m_mapItems.end(); ++mit) {
        if ( ItemID_ == mit->second->m_ItemID && mit->second->m_sEquipLevel == nLevel_ ) {
            return mit->second;
        }
    }

    return KItemPtr();
}

KItemPtr KItemContainer::FindItemByItemIDExItemUID( IN const GCITEMID ItemID_, IN const std::vector<GCITEMUID>& vecExItemUID_ )
{
    // 해당 ItemUID를 제외한 ItemID로 아이템 정보 찾기
    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end(); ++mit) {
        if ( ItemID_ != mit->second->m_ItemID ) {
            continue;
        }

        if ( std::find( vecExItemUID_.begin(), vecExItemUID_.end(), mit->first ) != vecExItemUID_.end() ) {
            continue;
        }

        return mit->second;
    }

    return KItemPtr();
}

KItemPtr KItemContainer::FindItemByItemUID( IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit = m_mapItems.find( ItemUID_ );
    
    if( m_mapItems.end() == mit)
        return KItemPtr();
    
    return mit->second;
}

KItemPtr KItemContainer::FindDepotItemByItemUID( IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit = m_mapDepotItems.find( ItemUID_ );

    if( m_mapDepotItems.end() == mit)
        return KItemPtr();

    return mit->second;
}


bool KItemContainer::RemoveItemFrommmap( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ )
{
    std::pair<ItemMMap::iterator, ItemMMap::iterator> prRange;
    prRange = m_mmapItems.equal_range( ItemID_ );
    if ( prRange.first == prRange.second )
        return false;
    ItemMMap::iterator mmit;
    for ( mmit = prRange.first ; mmit != prRange.second ; ++mmit ) {

        KItemPtr spItem = mmit->second.lock();
        if ( spItem && spItem->m_ItemUID == ItemUID_ )
        {
            m_mmapItems.erase( mmit );
            return true;
        }
    }
    return false;
}

bool KItemContainer::RemoveDepotItemFrommmap( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ )
{
    std::pair<ItemMMap::iterator, ItemMMap::iterator> prRange;
    prRange = m_mmapDepotItems.equal_range( ItemID_ );
    if ( prRange.first == prRange.second )
        return false;
    ItemMMap::iterator mmit;
    for ( mmit = prRange.first ; mmit != prRange.second ; ++mmit ) {

        KItemPtr spItem = mmit->second.lock();
        if ( spItem && spItem->m_ItemUID == ItemUID_ )
        {
            m_mmapDepotItems.erase( mmit );
            return true;
        }
    }
    return false;
}

bool KItemContainer::RemoveItem( IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit = m_mapItems.find( ItemUID_ );

    if( m_mapItems.end() == mit)
        return false;

    LIF( RemoveItemFrommmap( mit->second->m_ItemID, mit->second->m_ItemUID ) );
    m_mapItems.erase( mit );
    return true;
}

bool KItemContainer::RemoveDepotItem( IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit = m_mapDepotItems.find( ItemUID_ );

    if( m_mapDepotItems.end() == mit)
        return false;

    LIF( RemoveDepotItemFrommmap( mit->second->m_ItemID, mit->second->m_ItemUID ) );
    m_mapDepotItems.erase( mit );
    return true;
}


bool KItemContainer::RemoveItem( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ )
{

    ItemMap::iterator mit = m_mapItems.find( ItemUID_ );

    if( m_mapItems.end() == mit)
        return false;

    if( mit->second->m_ItemID != ItemID_ )
        return false;

    LIF( RemoveItemFrommmap( mit->second->m_ItemID, mit->second->m_ItemUID ) );
    m_mapItems.erase( mit );
    return true;
}

bool KItemContainer::AddItem( IN const KItem& kItem_ )
{
    KItemPtr spItem( new KItem );
    *spItem = kItem_;
    if ( !m_mapItems.insert( ItemMap::value_type( spItem->m_ItemUID, spItem ) ).second ) {
        return false;
    }

    KItemWPtr wpItem( spItem );
    m_mmapItems.insert( ItemMMap::value_type( spItem->m_ItemID, wpItem ) );

    return true;
}

bool KItemContainer::AddDepotItem( IN const KItem& kItem_ )
{
    if ( kItem_.m_cInvenType != KItem::DEPOT ) {
        return false;
    }

    KItemPtr spItem( new KItem );
    *spItem = kItem_;
    KItemWPtr wpItem( spItem );
    if ( !m_mapDepotItems.insert( ItemMap::value_type( spItem->m_ItemUID, spItem ) ).second ) {
        return false;
    }

    KDepotKey depotKey( spItem->m_cTabID, spItem->m_cSlotID );
    if ( !m_mapDepotItem.insert( DepotItemKeyMap::value_type( depotKey, wpItem ) ).second ) {
        ItemMap::iterator mit;
        mit = m_mapDepotItems.find( spItem->m_ItemUID );
        if( mit != m_mapDepotItems.end() ) {
            m_mapDepotItems.erase( mit );
        }
        return false;
    }

    m_mmapDepotItems.insert( ItemMMap::value_type( spItem->m_ItemID, wpItem ) );

    return true;
}


bool KItemContainer::UpdateItem( IN OUT KItem& oldItem, IN const KItem& newItem )
{
    if ( oldItem.m_cInvenType != KItem::INVENTORY ) {
        START_LOG( cerr, L"인벤토리 아이템이 아닌데(창고), 수량/기간 합치려고 함, ItemID : " << oldItem.m_ItemID )
            << END_LOG;
        return false;
    }
    if ( oldItem.m_nInitCount != KItem::UNLIMITED_ITEM && newItem.m_nCount != KItem::UNLIMITED_ITEM ) {
        oldItem.m_nInitCount += newItem.m_nCount;
        oldItem.m_nCount += newItem.m_nCount;
    }

    if ( oldItem.m_nPeriod != KItem::UNLIMITED_ITEM && newItem.m_nPeriod != KItem::UNLIMITED_ITEM ) {
        oldItem.m_nPeriod += newItem.m_nPeriod;
        oldItem.m_tEndDate = newItem.m_tEndDate;
    }
    return true;
}

bool KItemContainer::UpdateItemNoUpdate( IN OUT KItem& oldItem, IN const KItem& newItem )
{
    if ( oldItem.m_cInvenType != KItem::INVENTORY ) {
        START_LOG( cerr, L"인벤토리 아이템이 아닌데(창고), 수량/기간 합치려고 함, ItemID : " << oldItem.m_ItemID )
            << END_LOG;
        return false;
    }
    if ( oldItem.m_nInitCount != KItem::UNLIMITED_ITEM && newItem.m_nCount != KItem::UNLIMITED_ITEM ) {
        //oldItem.m_nInitCount += newItem.m_nCount;
        oldItem.m_nCount += newItem.m_nCount;
    }

    if ( oldItem.m_nPeriod != KItem::UNLIMITED_ITEM && newItem.m_nPeriod != KItem::UNLIMITED_ITEM ) {
        oldItem.m_nPeriod += newItem.m_nPeriod;
        oldItem.m_tEndDate = newItem.m_tEndDate;
    }
    return true;
}

bool KItemContainer::UpdateCountItem( IN OUT KItem& oldItem_, IN const KItem& newItem_ )
{
    if ( oldItem_.m_nInitCount != KItem::UNLIMITED_ITEM && newItem_.m_nCount != KItem::UNLIMITED_ITEM ) {
        oldItem_.m_nInitCount += newItem_.m_nCount;
        oldItem_.m_nCount += newItem_.m_nCount;
    }
    return true;
}

int KItemContainer::AddItemList( IN OUT std::vector<KItem>& vecItemList_ )
{
    int nCount = 0;
    std::vector<KItem>::iterator vit;

    std::vector<KItem> vecRestructureList;
    std::vector<KItem>::iterator vitRestructure;
    vecRestructureList.clear();

    for ( vit = vecItemList_.begin() ; vit != vecItemList_.end(); ++vit )
    {
        if ( vit->m_cInvenType != KItem::INVENTORY) { continue; }

        KItemPtr pkItem = FindItemByItemUID( vit->m_ItemUID );
        
        if ( pkItem == NULL ) {
            if ( AddItem( *vit ) ) {
                ++nCount;
            }
        }
        else if ( SiKLookEquipment()->IsLookItemID( pkItem->m_ItemID ) && SiKLookEquipment()->GetEnableAvatar() == true ) {
            if ( AddItem( *vit ) ) {
                ++nCount;
            }
        }
        else {
            UpdateItem( *pkItem, *vit );
            *vit = *pkItem;
            ++nCount;
        }

        // 동일한 UID를 가진 수량 아이템은 최종 결과 값만 client 데이터로 설정 해준다
        vitRestructure = std::find_if( vecRestructureList.begin(), vecRestructureList.end(),
            boost::bind( &KItem::m_ItemUID, _1 ) == vit->m_ItemUID );

        if ( vecRestructureList.end() != vitRestructure ) {
            // 동일 UID 수량 아이템 발견 시 지워줌
            vitRestructure = vecRestructureList.erase( vitRestructure );
    }

        vecRestructureList.push_back( *vit );
    }

    // 최종 결과 데이터만 모은 컨테이너로 세팅
    vecItemList_.clear();
    vecItemList_ = vecRestructureList;

    return nCount;
}
int KItemContainer::AddItemListNoUpdate( IN OUT std::vector<KItem>& vecItemList_ )
{
    int nCount = 0;
    std::vector<KItem>::iterator vit;

    for ( vit = vecItemList_.begin() ; vit != vecItemList_.end(); ++vit )
    {
        if ( vit->m_cInvenType != KItem::INVENTORY) { continue; }

        KItemPtr pkItem = FindItemByItemUID( vit->m_ItemUID );

        if ( pkItem == NULL ) {
            if ( AddItem( *vit ) ) {
                ++nCount;
            }
        }
        else if ( SiKLookEquipment()->IsLookItemID( pkItem->m_ItemID ) && SiKLookEquipment()->GetEnableAvatar() == true ) {
            if ( AddItem( *vit ) ) {
                ++nCount;
            }
        }
        else {
            UpdateItemNoUpdate( *pkItem, *vit );
            *vit = *pkItem;
            ++nCount;
        }
    }

    return nCount;
}

int KItemContainer::AddDepotItemList( IN OUT std::vector<KItem>& vecItemList_ )
{
    int nCount = 0;
    std::vector<KItem>::iterator vit;

    for ( vit = vecItemList_.begin() ; vit != vecItemList_.end(); ++vit )
    {
        if ( vit->m_nPeriod != KItem::UNLIMITED_ITEM ) {
            START_LOG( cerr, L"창고에 기간제 아이템을 넣으려고 시도" )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_ItemUID )
                << BUILD_LOGc( vit->m_cInvenType )
                << END_LOG;
            continue;
        }
        KItemPtr pkItem = FindDepotItemByItemUID( vit->m_ItemUID );
        KItemPtr pkCountItem = FindDepotItemByItemID( vit->m_ItemID );

        if ( pkItem == NULL ) {
            if ( AddDepotItem( *vit ) )
                ++nCount;
        }
        else {
            START_LOG( cerr, L"창고에 이미 존재하는 아이템, 버그!")
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_ItemUID )
                << END_LOG;
        }
    }

    return nCount;
}

bool KItemContainer::MoveInvenToDepot( IN KItem& kItem_, IN const KDepotKey& depotKey_ )
{
    KItemPtr spItem = FindItemByItemUID( kItem_.m_ItemUID );
    if ( spItem == NULL ) {
        return false;
    }
    // 인벤토리에서 이동할 아이템의 정보를 창고 Type으로 세팅하자.
    spItem->m_cInvenType = KItem::DEPOT;
    spItem->m_cTabID = depotKey_.m_cTabID;
    spItem->m_cSlotID = depotKey_.m_cSlotID;

    KItemWPtr wpItem( spItem );
    if ( m_mapDepotItem.insert( std::make_pair( depotKey_, wpItem ) ).second == false ) {
        return false;
    }

    if ( m_mapDepotItems.insert( std::make_pair( spItem->m_ItemUID, spItem ) ).second == false ) {
        // 인벤토리 타입으로 다시 롤백.
        spItem->m_cInvenType = KItem::INVENTORY;
        spItem->m_cTabID = 0;
        spItem->m_cSlotID = 0;

        DepotItemKeyMap::iterator mit;
        mit = m_mapDepotItem.find( depotKey_ ); 
    
        if( mit != m_mapDepotItem.end() ) { // 창고 아이템 리스트 관리하는 컨테이너에서 제거.
            m_mapDepotItem.erase( mit );
        }
        return false;
    }
        
    m_mmapDepotItems.insert( std::make_pair( spItem->m_ItemID, wpItem ) );

    RemoveItem( kItem_.m_ItemUID );


   _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}

bool KItemContainer::CopyInvenToDepot( IN KItem& kItem_, IN const KDepotKey& depotKey_, IN const int& nCount_, OUT KItem& kDepotItem_ )
{ // 기존 인벤토리의 아이템을 지우지 않고 창고에 일부 수량을 복사하면서 창고에 새 아이템을 만들게 된다.
    KItemPtr spItem = FindItemByItemUID( kItem_.m_ItemUID );
    if ( spItem == NULL ) {
        return false;
    }

    //KItemPtr spDepotItem( new KItem );
    //*spDepotItem = kItem_;

    KItem kItem;
    kItem = kItem_;

    // 인벤토리에서 이동할 아이템의 정보를 창고 Type으로 세팅하자.
    kItem.m_cInitInvenType = KItem::INVENTORY;
    kItem.m_cInvenType = KItem::DEPOT;
    kItem.m_cTabID = depotKey_.m_cTabID;
    kItem.m_cSlotID = depotKey_.m_cSlotID;
    kItem.m_nInitCount = kItem.m_nCount = nCount_;
    kItem.m_bReserveNewUID = true;

    kDepotItem_ = kItem;

    if ( AddDepotItem( kItem ) == true ) {
        kItem_.m_nCount -= nCount_;
    }

    _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}

bool KItemContainer::CopyDepotToInven( IN KItem& kItem_, IN const KDepotKey& depotKey_, IN const int& nCount_, OUT KItem& kInvenItem_ )
{
    KItemPtr spItem = FindDepotItemByItemUID( kItem_.m_ItemUID );
    if ( spItem == NULL ) {
        return false;
    }

    KItem kItem;
    kItem = kItem_;

    // 인벤토리에서 이동할 아이템의 정보를 창고 Type으로 세팅하자.
    kItem.m_cInitInvenType = KItem::DEPOT;
    kItem.m_cInvenType = KItem::INVENTORY;
    kItem.m_cTabID = depotKey_.m_cTabID;
    kItem.m_cSlotID = depotKey_.m_cSlotID;
    kItem.m_nInitCount = kItem.m_nCount = nCount_;
    kItem.m_bReserveNewUID = true;

    kInvenItem_ = kItem;
    std::vector<KItem> vecItem;
    vecItem.push_back( kItem );
    //if ( AddItem( kItem ) == true ) {
    //    kItem_.m_nCount -= nCount_;
    //}
    if ( AddItemListNoUpdate( vecItem ) == 1 ) {
        kItem_.m_nCount -= nCount_;
    }

    _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}

bool KItemContainer::MoveInvenToDepotCount( IN KItem& kInvenItem_, IN const KDepotItem& kDepotItem_, OUT KItem& kDepotResultItem_ )
{
    KItemPtr spItem = FindItemByItemUID( kInvenItem_.m_ItemUID ); // 인벤토리의 아이템
    KItemPtr spDepotItem = FindDepotItemByItemID( kInvenItem_.m_ItemID ); // 창고의 아이템
    if ( spItem == NULL ) {
        return false;
    }

    // 크게 신규로 창고 아이템을 넣는 경우와  기존 창고 아이템에 수량을 더하는 경우로 나눈다. 그런데 들여쓰기
    // 절약을 위해서 아래처럼 네가지 경우처럼 구현 --> 별로다. 그냥  신규/기존 케이스로 나누자.

    // 1. 창고 아이템으로 전체 수량 옮기는 경우 (창고에 동일 아이템 없는 경우)
    if ( spDepotItem == NULL && kDepotItem_.m_nCount == kInvenItem_.m_nCount  ) {
        MoveInvenToDepot( kInvenItem_, kDepotItem_.m_kDepotKey ); // 영구 아이템처럼 옮긴다.
        KItemPtr spNewDepotItem = FindDepotItemByItemID( kInvenItem_.m_ItemID ); // 창고의 아이템
        if ( spNewDepotItem != NULL ) {
            kDepotResultItem_ = *spNewDepotItem;
        }        
    }
    // 일부 수량의 창고 이동으로 신규 아이템이 생성되는 경우
    else if ( spDepotItem == NULL && kDepotItem_.m_nCount < kInvenItem_.m_nCount ) {
        CopyInvenToDepot( kInvenItem_, kDepotItem_.m_kDepotKey, kDepotItem_.m_nCount, kDepotResultItem_ );
    }
    // 2. 인벤토리 아이템의 전체 수량이 동일 ItemID의 창고 아이템 수량에 합쳐지는 경우
    else if ( spDepotItem != NULL && kDepotItem_.m_nCount == kInvenItem_.m_nCount ) {
        kInvenItem_.m_nCount = 0;
        spDepotItem->m_nCount += kDepotItem_.m_nCount;
        kDepotResultItem_ = *spDepotItem;
        // removeitem if kinvenitem is new uid 
    }

    // 일부 수량이 기존 창고 아이템에 합쳐지는 경우.
    else if ( spDepotItem != NULL && kDepotItem_.m_nCount < kInvenItem_.m_nCount ) { // 일부 수량을 창고로 옮기는 경우
        kInvenItem_.m_nCount -= kDepotItem_.m_nCount;
        spDepotItem->m_nCount += kDepotItem_.m_nCount;
        kDepotResultItem_ = *spDepotItem; 
    }

    return true;
}

bool KItemContainer::MoveDepotToInvenCount( IN KItem& kItem_, IN const KDepotItem& kDepotItem_, OUT KItem& kDepotResultItem_ )
{
    KItemPtr spInvenItem = FindItemByItemID( kItem_.m_ItemID ); // 인벤토리의 아이템
    KItemPtr spDepotItem = FindDepotItemByItemUID( kItem_.m_ItemUID ); // 창고의 아이템
    if ( spDepotItem == NULL ) {
        return false;
    }

    // 크게 신규로 인벤토리에 아이템을 넣는 경우와  기존 창고 아이템에 수량을 더하는 경우로 나눈다. 그런데 들여쓰기
    // 절약을 위해서 아래처럼 네가지 경우처럼 구현 --> 별로다. 그냥  신규/기존 케이스로 나누자.

    // 1. 아이템을 통째로 옮기는 경우
    if ( spInvenItem == NULL && kDepotItem_.m_nCount == kItem_.m_nCount  ) {

        MoveDepotToInvenCountItem( kItem_, kDepotItem_.m_kDepotKey );
        KItemPtr spNewInvenItem = FindItemByItemUID( kItem_.m_ItemUID ); // 인벤토리 아이템
        if ( spNewInvenItem != NULL ) {
            kDepotResultItem_ = *spNewInvenItem;
        }
    }
    // 일부 수량의 창고에서 인벤토리 이동으로 신규 아이템이 생성되는 경우
    else if ( spInvenItem == NULL && kDepotItem_.m_nCount < kItem_.m_nCount ) {
        CopyDepotToInven( kItem_, kDepotItem_.m_kDepotKey, kDepotItem_.m_nCount, kDepotResultItem_ );
    }
    // 2. 전체 수량이 기존 인벤토리 아이템에 합쳐지는 경우
    else if ( spInvenItem != NULL && kDepotItem_.m_nCount == kItem_.m_nCount ) {
        kItem_.m_nCount = 0;
        spInvenItem->m_nCount += kDepotItem_.m_nCount;
        //spDepotItem->m_nCount -= kDepotItem_.m_nCount;

        kDepotResultItem_ = *spInvenItem;

        if ( kItem_.m_bReserveNewUID == false ) {
            if ( !m_mapReserveDelDepotItem.insert( ItemMap::value_type( spDepotItem->m_ItemUID, spDepotItem ) ).second ) {
                START_LOG( cerr, L" 창고 아이템 삭제 예약 컨테이너 삽입 실패 ItemUID : " << kItem_.m_ItemUID )
                    << END_LOG;
                //return false;
            }
        }

        if( DeleteDepotItem( kItem_, kDepotItem_.m_kDepotKey ) == false ) {
            return false;
        }
    }

    // 일부 수량이 기존 창고 아이템에 합쳐지는 경우.
    else if ( spInvenItem != NULL && kDepotItem_.m_nCount < kItem_.m_nCount ) { // 일부 수량을 창고로 옮기는 경우
        kItem_.m_nCount -= kDepotItem_.m_nCount;
        spInvenItem->m_nCount += kDepotItem_.m_nCount;
        kDepotResultItem_ = *spInvenItem; 
    }

    return true;
}


bool KItemContainer::MoveDepotToInven( IN KItem& kItem_, IN const KDepotKey& depotKey_ )
{
    DepotItemKeyMap::iterator mit = m_mapDepotItem.find( depotKey_ );
    if ( mit == m_mapDepotItem.end() ) {
        return false; // 창고에서 인벤토리로 옮기려는 아이템 없음.
    }

    kItem_.m_cInvenType = KItem::INVENTORY;
    //kItem_.m_cTabID = 0; // !주의 : 초기화는 캐싱 데이터를 DB에 쓸 때만 해야 됨.
    //kItem_.m_cSlotID = 0;
    //kItem_.m_cInitTabID = 0;
    //kItem_.m_cInitSlotID = 0;

    if( !RemoveDepotItem( kItem_.m_ItemUID ) ) {
        START_LOG( cerr, L" 창고 아이템 삭제 실패 ItemUID : " << kItem_.m_ItemUID )
            << END_LOG;
        return false;
    }
    m_mapDepotItem.erase( mit );

    std::vector<KItem> vecItem;
    vecItem.push_back( kItem_ );
    if ( AddItemList( vecItem ) < 1 ) {
        START_LOG( cerr, L" 창고 아이템 이동 실패 ItemUID : " << kItem_.m_ItemUID )
            << END_LOG;
        return false;
    }
    //if( !AddItem( kItem_ ) ) {
    //    START_LOG( cerr, L" 창고 아이템 이동 실패 ItemUID : " << kItem_.m_ItemUID )
    //        << END_LOG;
    //    return false;
    //}

    _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}

bool KItemContainer::MoveDepotToInvenCountItem( IN KItem& kItem_, IN const KDepotKey& depotKey_ )
{
    DepotItemKeyMap::iterator mit = m_mapDepotItem.find( depotKey_ );
    if ( mit == m_mapDepotItem.end() ) {
        return false; // 창고에서 인벤토리로 옮기려는 아이템 없음.
    }

    kItem_.m_cInvenType = KItem::INVENTORY;
    //kItem_.m_cTabID = 0; // !주의 : 초기화는 캐싱 데이터를 DB에 쓸 때만 해야 됨.
    //kItem_.m_cSlotID = 0;
    //kItem_.m_cInitTabID = 0;
    //kItem_.m_cInitSlotID = 0;

    if( !RemoveDepotItem( kItem_.m_ItemUID ) ) {
        START_LOG( cerr, L" 창고 아이템 삭제 실패 ItemUID : " << kItem_.m_ItemUID )
            << END_LOG;
        return false;
    }
    m_mapDepotItem.erase( mit );

    std::vector<KItem> vecItem;
    vecItem.push_back( kItem_ );
    if ( AddItemListNoUpdate( vecItem ) < 1 ) {
        START_LOG( cerr, L" 창고 아이템 이동 실패 ItemUID : " << kItem_.m_ItemUID )
            << END_LOG;
        return false;
    }
    //if( !AddItem( kItem_ ) ) {
    //    START_LOG( cerr, L" 창고 아이템 이동 실패 ItemUID : " << kItem_.m_ItemUID )
    //        << END_LOG;
    //    return false;
    //}

    _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}


bool KItemContainer::DeleteDepotItem( IN KItem& kItem_, IN const KDepotKey& depotKey_ )
{
    DepotItemKeyMap::iterator mit = m_mapDepotItem.find( depotKey_ );
    if ( mit == m_mapDepotItem.end() ) {
        return false; // 창고에서 인벤토리로 옮기려는 아이템 없음.
    }

    //kItem_.m_cInvenType = KItem::INVENTORY;
    //kItem_.m_cTabID = 0; // !주의 : 초기화는 캐싱 데이터를 DB에 쓸 때만 해야 됨.
    //kItem_.m_cSlotID = 0;
    //kItem_.m_cInitTabID = 0;
    //kItem_.m_cInitSlotID = 0;

    if( !RemoveDepotItem( kItem_.m_ItemUID ) ) {
        START_LOG( cerr, L" 창고 아이템 삭제 실패 ItemUID : " << kItem_.m_ItemUID )
            << END_LOG;
        return false;
    }
    m_mapDepotItem.erase( mit );

    _JIF( m_mapDepotItem.size() == m_mapDepotItems.size(), return false );

    return true;
}

bool KItemContainer::ChangeDepotSlot( IN KItem& kItem_, IN KDepotKey& depotKey_, IN KDepotKey& newDepotKey_ )
{
    DepotItemKeyMap::iterator mit = m_mapDepotItem.find( depotKey_ );
    if ( mit == m_mapDepotItem.end() ) {
        // 창고 슬롯 이동할 아이템 없음
        return false;
    }

    if( false == m_mapDepotItem.insert( DepotItemKeyMap::value_type( newDepotKey_, mit->second ) ).second ) {
        return false;
    }

    m_mapDepotItem.erase( mit );

    //kItem_.m_cTabID = newDepotKey_.m_cTabID;
    kItem_.m_cSlotID = newDepotKey_.m_cSlotID;

    return true;

}

KItemPtr KItemContainer::FindItem( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit;
    mit = m_mapItems.find( ItemUID_ );

    if( mit == m_mapItems.end() )
        return KItemPtr();

    if( mit->second->m_ItemID != ItemID_ )
        return KItemPtr();

    return mit->second;
}

void KItemContainer::AdjustQueryInventory( IN const int nType_, IN OUT std::vector<KItem>& vecList_ )
{
    // TODO : 나중에 사라질지도 모르지만. 넷마블 캐시 아이템 구매후 호출 된다.


    // 기본적으로 유저에게 전달을 해줘야 하기 때문에
    // In Param 에 우선 적용하고 내 인벤에 적용 한다.
    // TODO : 방법에 대해서는 한번 생각 해보자.
    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit ) {

        // PC방 Fake Item에 대한 적용이 필요 하다.
        if ( nType_ == 0 && mit->second->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) { 
            vecList_.push_back( *mit->second );
            continue;
        } // IPAdvantage 의 종속성을 제거 하려고 했으나 사용하는 곳이 많아져서 수정 못하고 있음

        // 수량 아이템 변화 상태에 대한 적용이 필요 하다.
        int nDiff = mit->second->m_nCount - mit->second->m_nInitCount;
        if ( nDiff < 0 ) {
            std::vector<KItem>::iterator vit;
            vit = std::find_if( vecList_.begin(), vecList_.end(),
                boost::bind( &KItem::m_ItemUID, _1 ) == mit->first );
            if ( vit == vecList_.end() ) {
                // Log
                continue;
            }
            vit->m_nCount += nDiff;
        }
    }

    // 인벤토리에 적용 한다.
    ClearItem();
    AddItemList( vecList_ );
}

void KItemContainer::ClearItem()
{
    m_mapItems.clear();
    m_mmapItems.clear();
}

void KItemContainer::GetExpiredItemList( OUT std::vector< KItem >& vecList )
{
    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit ) {
        if( mit->second->IsExpired() == false )
            continue;

        vecList.push_back( *mit->second );
    }
}

KItemPtr KItemContainer::FindDurationItem( IN const GCITEMID ItemID_, IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit;
    mit = m_mapItems.find( ItemUID_ );

    if( mit == m_mapItems.end() )
        return KItemPtr();

    if( mit->second->m_ItemID != ItemID_ )
        return KItemPtr();

    if ( mit->second->m_nCount <= 0 )
        return KItemPtr();

    return mit->second;
}

KItemPtr KItemContainer::FindDurationItemByItemID( IN const GCITEMID ItemID_ )
{
    ItemMap::iterator mit = m_mapItems.begin();
    for( ; mit != m_mapItems.end(); ++mit)
    {
        if( ItemID_ == mit->second->m_ItemID && mit->second->m_nCount > 0 )
            return mit->second;
    }

    return KItemPtr();
}

KItemPtr KItemContainer::FindDurationItemByItemUID( IN const GCITEMUID ItemUID_ )
{
    ItemMap::iterator mit;
    mit = m_mapItems.find( ItemUID_ );

    if( mit == m_mapItems.end() )
        return KItemPtr();

    if ( mit->second->m_nCount <= 0 )
        return KItemPtr();

    return mit->second;
}

void KItemContainer::GetDiffDurationItemAndUpdate( OUT std::vector< KItem >& vecDiffItems_ )
{
    vecDiffItems_.clear();

    ItemMap::iterator mit;
    for( mit = m_mapItems.begin() ; mit != m_mapItems.end(); ++mit ) {

        if ( mit->second->m_nCount != mit->second->m_nInitCount ) {
            vecDiffItems_.push_back( *mit->second );
            mit->second->m_nInitCount = mit->second->m_nCount;
        }
    }
}

int KItemContainer::GetTotalEnableSize()
{
    int nCount = 0;
    ItemMap::iterator mit;

    for( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit )
    {
        if( mit->first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
            continue;

        if( SiKItemManager()->m_kItemConfig.IsNoneSizeItem( mit->second->m_ItemID ) )
            continue;

        if( mit->second->CheckItemType(KItem::TYPE_LOOK) )
            continue;

        if( mit->second->CheckItemType(KItem::TYPE_AGIT_OBJ) )
            continue;

        ++nCount;
    }

    return nCount;
}

int KItemContainer::GetLookItemNum()
{
    int nCount = 0;
    ItemMap::iterator mit;
    for( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit )
    {
        if( mit->first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
            continue;

        if( !mit->second->CheckItemType(KItem::TYPE_LOOK) )
            continue;

        if( SiKItemManager()->m_kItemConfig.IsNoneSizeItem( mit->second->m_ItemID ) )
            continue;

        ++nCount;
    }
    return nCount;
}

void KItemContainer::FindItemUIDsByItemID( IN const GCITEMID ItemID_, OUT std::vector<GCITEMUID>& vecItemUIDs_ )
{
    vecItemUIDs_.clear();

    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end(); ++mit) {

        if ( ItemID_ == mit->second->m_ItemID ) {
            vecItemUIDs_.push_back( mit->second->m_ItemUID );
        }
    }
}

int KItemContainer::GetAgitObjectItemNum()
{
    int nPCItemCount = 0;
    int nNormalCount = 0;
    int nNoneSizeItem = 0;

    int nCount = 0;
    ItemMap::iterator mit;
    for( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit )
    {
        if( mit->first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            ++nPCItemCount;
            continue;
        }

        if( false == mit->second->CheckItemType(KItem::TYPE_AGIT_OBJ) ) {
            ++nNormalCount;
            continue;
        }

        if( SiKItemManager()->m_kItemConfig.IsNoneSizeItem( mit->second->m_ItemID ) ) {
            ++nNoneSizeItem;
            continue;
        }

        ++nCount;
    }

    //std::clog << L"All ItemCount : " << m_mapItems.size()
    //    << L", PCItemCount : " << nPCItemCount
    //    << L", NormalCount : " << nNormalCount
    //    << L", NoneSizeItem : " << nNoneSizeItem
    //    << L", AgitObjCount : " << nCount << dbg::endl;

    return nCount;
}

int KItemContainer::GetItemCount( IN const GCITEMID ItemID_ )
{
    int nCount = 0;
    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit ) {

        if( mit->second->m_ItemID != ItemID_ )
            continue;

        ++nCount;
    }

    int nCount_mmap = m_mmapItems.count( ItemID_ );
    _LOG_SUCCESS( nCount == nCount_mmap, L"nCount = " << nCount << L", mmap_count = " << nCount_mmap ) << END_LOG;

    return nCount;
}

KItemPtr KItemContainer::FindDepotItemByKey( IN const KDepotKey& depotKey_ )
{
    DepotItemKeyMap::iterator mit;
    mit = m_mapDepotItem.find( depotKey_ );

    if ( mit != m_mapDepotItem.end() ) {
        return mit->second.lock();
    }

    return KItemPtr();
}

void KItemContainer::FindDepotItemByTab( IN const char tabID_, OUT std::vector<KItem>& vecItem_ )
{
    DepotItemKeyMap::iterator mit;
    for ( mit = m_mapDepotItem.begin() ; mit != m_mapDepotItem.end() ; ++mit ) {
        KItemPtr spItem = mit->second.lock();
        if ( !spItem ) {
            continue;
        }
        vecItem_.push_back( *spItem );
        /*
        if ( tabID_ == spItem->m_cTabID ) { //if ( charType_ == spItem->m_cCharType && tabID_ == spItem->m_cTabID ) {
            vecItem_.push_back( *spItem );
        }
        */
    }

    START_LOG( clog, L"Char 와 Tab 으로 창고 아이템 리스트 가져오기 , Size : " << vecItem_.size() )
        << END_LOG;
}

//실시간으로 DB에 창고 변경사항을 저장하지 않는다.

void KItemContainer::GetDiffInvenTypeItem( OUT std::vector< KItem >& vecDiffItems_,
                                           OUT std::vector< KItem >& vecDeleteDepotItems_,
                                           OUT std::vector< KItem >& vecDiffCountItems_, 
                                           OUT std::vector< KItem >& vecDeleteDepotCountItems_ )
{
    vecDiffItems_.clear();
    vecDeleteDepotItems_.clear();
    vecDiffCountItems_.clear();
    vecDeleteDepotCountItems_.clear();

    std::map< KDepotKey, KItem > deleteItem;

    DepotItemKeyMap::iterator mitDepot;
    for ( mitDepot = m_mapDepotItem.begin(); mitDepot != m_mapDepotItem.end() ; ++mitDepot ) {
        KItemPtr depotItem = mitDepot->second.lock();

        if ( depotItem->m_cInitInvenType == KItem::INVENTORY && depotItem->m_cInvenType == KItem::DEPOT ) {

            if ( depotItem->m_nCount == KItem::UNLIMITED_ITEM ) {
                vecDiffItems_.push_back( *depotItem ); // 인벤토리 -> 창고로 넣은 경우
            }
            else {
                vecDiffCountItems_.push_back( *depotItem );
            }

            depotItem->m_cInitInvenType = depotItem->m_cInvenType; // 초기화.
            depotItem->m_cInitTabID = depotItem->m_cTabID;
            depotItem->m_cInitSlotID = depotItem->m_cSlotID;
            depotItem->m_nInitCount = depotItem->m_nCount;

            continue;
        }

        if ( depotItem->m_cInitInvenType == KItem::DEPOT && depotItem->m_cInvenType == KItem::DEPOT ) { // 창고 안에서의 탭/슬롯 이동
            
            if ( depotItem->m_cTabID != depotItem->m_cInitTabID ) {
                if ( depotItem->m_nCount == KItem::UNLIMITED_ITEM ) { // 영구 아이템
                    vecDiffItems_.push_back( *depotItem );
                }
                else if( depotItem->m_nCount > 0 ) { // 수량 아이템
                    vecDiffCountItems_.push_back( *depotItem );
                }               

                depotItem->m_cInitInvenType = depotItem->m_cInvenType; // 초기화.
                depotItem->m_cInitTabID = depotItem->m_cTabID;
                depotItem->m_cInitSlotID = depotItem->m_cSlotID;
                depotItem->m_nInitCount = depotItem->m_nCount;
            }
            else if ( depotItem->m_cSlotID != depotItem->m_cInitSlotID ) {
                if ( depotItem->m_nCount == KItem::UNLIMITED_ITEM ) {
                    vecDiffItems_.push_back( *depotItem );
                }
                else if ( depotItem->m_nCount > 0 ) {
                    vecDiffCountItems_.push_back( *depotItem );
                }

                depotItem->m_cInitInvenType = depotItem->m_cInvenType; // 초기화.
                depotItem->m_cInitTabID = depotItem->m_cTabID;
                depotItem->m_cInitSlotID = depotItem->m_cSlotID;
                depotItem->m_nInitCount = depotItem->m_nCount;
            }
            
            else if ( depotItem->m_nInitCount != depotItem->m_nCount ) { // 위 if 조건과 else 가 아니라는 것에 주의.
                vecDiffCountItems_.push_back( *depotItem );

                depotItem->m_cInitInvenType = depotItem->m_cInvenType; // 초기화.
                depotItem->m_cInitTabID = depotItem->m_cTabID;
                depotItem->m_cInitSlotID = depotItem->m_cSlotID;
                depotItem->m_nInitCount = depotItem->m_nCount;
            }

            if ( depotItem->m_nCount == 0  ) { // 실시간으로 창고 컨테이너에서 아이템 제거용
                deleteItem.insert( std::make_pair( mitDepot->first, *depotItem ) );
                
            }
        }
    } // for ( mitDepot =

    ItemMap::iterator mitNormal;
    for( mitNormal = m_mapItems.begin() ; mitNormal != m_mapItems.end(); ++mitNormal ) {
        // 인벤토리 타입(인벤,창고) , 슬롯 둘 다 바뀐 경우나 하나씩만 바뀐 경우라도 vecDiffItems_ 에는 1회만 들어감.
        if ( mitNormal->second->m_cInitInvenType == KItem::DEPOT && mitNormal->second->m_cInvenType == KItem::INVENTORY ) { 
            if ( mitNormal->second->m_nCount == KItem::UNLIMITED_ITEM ) { // 영구 아이템
                vecDeleteDepotItems_.push_back( *mitNormal->second );
            }
            else { // 수량 아이템, 창고에서 인벤토리로 옮기는 경우
                vecDeleteDepotCountItems_.push_back( *mitNormal->second );
            }     
            
            mitNormal->second->m_cInitInvenType = mitNormal->second->m_cInvenType; // 초기화.
            mitNormal->second->m_cInitTabID = mitNormal->second->m_cTabID;
            mitNormal->second->m_cInitSlotID = mitNormal->second->m_cSlotID;
        }
    }

    std::map< KDepotKey, KItem >::iterator mitDel;
    for ( mitDel = deleteItem.begin(); mitDel != deleteItem.end(); ++mitDel ) {
        RemoveDepotItem( mitDel->second.m_ItemUID );

        m_mapDepotItem.erase( mitDel->first );
    }

    ItemMap::iterator mitDelReserve;
    for ( mitDelReserve = m_mapReserveDelDepotItem.begin() ; mitDelReserve != m_mapReserveDelDepotItem.end() ; ++mitDelReserve ) {
        vecDeleteDepotCountItems_.push_back( *mitDelReserve->second );
    }
    
    m_mapReserveDelDepotItem.clear();
}

void KItemContainer::GetStrengthItemList( IN const GCITEMID ItemID_, IN const int nGrade_, IN const int nLv_, OUT std::map<char,std::map<char,std::set<GCITEMUID> > >& mapList_ )
{
    // ItemID_ : 강화석 ItemID
    // nGrade_ : 전달받은 등급값 이하의 강화석 리스트를 뽑는다. (-1은 체크안함)
    // nLv_ : 전달받은 레벨값 이하의 강화석 리스트를 뽑는다. (-1은 체크안함)
    mapList_.clear();

    ItemMap::iterator mit;
    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit ) {
        if ( mit->second == NULL ) {
            continue;
        }

        if( mit->second->m_ItemID != ItemID_ ) {
            continue;
        }

        //if ( nGrade_ > -1 && nGrade_ < mit->second->m_cEnchantEquipGradeID ) {
        if ( nGrade_ > -1 && nGrade_ < mit->second->m_cGradeID ) {
            continue;
        }

        if ( nLv_ > -1 && nLv_ < mit->second->m_cEnchantLevel ) {
            continue;
        }

        // 장착중인 강화석은 제외한다
        if ( mit->second->m_EnchantEquipItemUID > 0 ) {
            continue;
        }

        mapList_[mit->second->m_cEnchantEquipGradeID][mit->second->m_cEnchantLevel].insert( mit->second->m_ItemUID );
    }
}

KItemPtr KItemContainer::FindDepotItemByItemID( IN const GCITEMID ItemID_ )
{
    ItemMap::iterator mit = m_mapDepotItems.begin();
    for( ; mit != m_mapDepotItems.end(); ++mit) {
        if( ItemID_ == mit->second->m_ItemID )
            return mit->second;
    }

    return KItemPtr();
}

KItemPtr KItemContainer::FindDepotItemByItemIDandLevel( IN const GCITEMID ItemID_, IN const int nLevel_ )
{
    ItemMap::iterator mit = m_mapDepotItems.begin();
    for ( ; mit != m_mapDepotItems.end(); ++mit) {
        if ( ItemID_ == mit->second->m_ItemID && mit->second->m_sEquipLevel == nLevel_ ) {
            return mit->second;
        }
    }

    return KItemPtr();
}

void KItemContainer::DumpInventory( IN const std::wstring& wstrName_, IN const DWORD dwUID_ )
{
    int nNonOccupySize = 0;
    std::wstringstream stm;

    stm << L"[" << KNC_TIME_STRING << L"] ";
    stm << L"Dumping " << wstrName_ << L"'s inventory items(LoginUID: " << dwUID_ << L" )" << std::endl;

    // typedef std::map< GCITEMUID, KItemPtr > ItemMap;
    BOOST_FOREACH( ItemMap::value_type& rItem, m_mapItems ) {
        bool bNonOccupyItem = false;

        if ( rItem.first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            bNonOccupyItem = true;
        }

        if ( SiKItemManager()->m_kItemConfig.IsNoneSizeItem( rItem.second->m_ItemID ) ) {
            bNonOccupyItem = true;
        }

        if ( rItem.second->CheckItemType( KItem::TYPE_LOOK ) ) {
            bNonOccupyItem = true;
        }

        if ( rItem.second->CheckItemType( KItem::TYPE_AGIT_OBJ ) ) {
            bNonOccupyItem = true;
        }

        if ( false == bNonOccupyItem ) {
            ++nNonOccupySize;
        }

        stm << L"" << rItem.first << L"  " << rItem.second->m_ItemID << L"  " << ( ( bNonOccupyItem ) ? L"NonOccupied" : L"Occupied" ) << std::endl;
    }

    stm << L"Real Size: "           << m_mapItems.size()
        << L", NonOccupySize: "     << nNonOccupySize
        << std::endl;

    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_USER_INVENTORY_ITEM_LIST, wstrName_, dwUID_, 0, stm.str() );

    stm.clear();
    stm.str(L"");
}
void KItemContainer::MakeCharacterInventory( IN const char charType_, std::vector<KItem>& vecList_ )
{    
    vecList_.clear();
    ItemMap::iterator mit;

    for ( mit = m_mapItems.begin() ; mit != m_mapItems.end() ; ++mit ) {
        if ( mit->second->m_cCharType == charType_ || mit->second->m_cCharType == -1 ) {
            vecList_.push_back( *mit->second );
        }
    }

    ClearItem();
    AddItemList( vecList_ );
}