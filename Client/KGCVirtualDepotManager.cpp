#include "KGCVirtualDepotManager.h"

ImplementSingleton( KGCVirtualDepotManager )

KGCVirtualDepotManager::KGCVirtualDepotManager( void )
:m_iCurrentChar(0)
{
    m_bIsRecvPack = false;
    m_bIsUpdateUI = false;
    m_vecDepotItemList.clear();
    m_vecDepotCommonItemList.clear();
    m_vecDepotExclusiveItemList.clear();
    m_vecDepotInComplateComonItemList.clear();
    m_setDpotRestrictItem.clear();
    m_bFirstLogOn = true;
    m_bCloseDlg = false;
}

KGCVirtualDepotManager::~KGCVirtualDepotManager(void)
{

}

void KGCVirtualDepotManager::SetDepotItemList( IN KEVENT_VIRTUAL_DEPOT_ITEM_LIST_NOT& kRecv )
{
    m_vecDepotItemList = kRecv;

    std::vector<KItem>::iterator itor;

    for( itor = m_vecDepotItemList.begin(); itor != m_vecDepotItemList.end(); ) {
        itor->m_ItemID = itor->m_ItemID/10;
        if( itor->m_ItemID == 0 )
        {
            itor = m_vecDepotItemList.erase(itor);
        }
        else
            itor++;
    }

}

void KGCVirtualDepotManager::SetDepotRestrictList( IN KEVENT_DEPOT_RESTRICT_ITEM_NOT& kRecv )
{
    m_setDpotRestrictItem.clear();

    if( kRecv.empty() )
        return;

    m_setDpotRestrictItem = kRecv;

}

void KGCVirtualDepotManager::SendMoveItemData()
{
    if( m_mapMoveMyInvenItemList.empty() )
        return;

    int iMaxOpenChar = g_kGlobalValue.GetCharOpenType().size();

    for( int i = 0; i < iMaxOpenChar; i++ )
    {
        std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( i );
        if( mit != m_mapMoveMyInvenItemList.end() )
        {
            std::vector< KItem >::iterator vit;
            for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
            {
                vit->m_ItemID = vit->m_ItemID * 10;
            }
        }
    }

    Result_VirtualDepotItemListMoveInven = INT_MAX;
    KP2P::GetInstance()->Send_MoveItemToInventoryFromVirtualDepot( m_mapMoveMyInvenItemList );
    g_MyD3D->WaitForServerAck( Result_VirtualDepotItemListMoveInven, INT_MAX, 30000, TIME_OUT_VALUE );
    
}

KPetInfo* KGCVirtualDepotManager::GetPetInfo(IN KItem *pItem)
{
    GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( pItem->m_ItemID );

    if( pItemInfo ==  NULL )
        return NULL;

    if( pItemInfo && pItemInfo->eItemKind == GIK_PET && pItemInfo->ePetItemKind == EPET_KIND_PET )
    {
        std::map<GCITEMUID, KPetInfo>::iterator mitInven = g_MyD3D->m_mapPetInfo.find( pItem->m_ItemUID );
        std::map<GCITEMUID, KPetInfo>::iterator mitDepot = g_MyD3D->m_mapDepotPetInfo.find( pItem->m_ItemUID );

        if( mitInven != g_MyD3D->m_mapPetInfo.end() ){
            return &mitInven->second;
        }
        else if ( mitDepot != g_MyD3D->m_mapDepotPetInfo.end() ){
            return &mitDepot->second;
        }
    }
    return NULL;
}

KItem* KGCVirtualDepotManager::CommonFindGetItem( IN const GCITEMUID nItemUID )
{
    std::vector <KItem>::iterator vitItem;

    for( vitItem = m_vecDepotCommonItemList.begin(); vitItem != m_vecDepotCommonItemList.end(); vitItem++ )
    {
        if( vitItem->m_ItemUID == nItemUID )
        {
            return &(*vitItem);
        }
    }

    return NULL;

}

KItem* KGCVirtualDepotManager::ExclusiveFindGetItem( IN const GCITEMUID nItemUID )
{
    std::vector <KItem>::iterator vitItem;

    for( vitItem = m_vecDepotExclusiveItemList.begin(); vitItem != m_vecDepotExclusiveItemList.end(); vitItem++ )
    {
        if( vitItem->m_ItemUID == nItemUID )
        {
            return &(*vitItem);
        }
    }

    return NULL;
}

std::vector<GCITEMUID> KGCVirtualDepotManager::GetDepotCommonItemList()
{
    std::vector<GCITEMUID> vecDepotCommonItemUIDList;
    std::vector<KItem>::iterator vecItor;

    for( vecItor = m_vecDepotCommonItemList.begin(); vecItor != m_vecDepotCommonItemList.end(); vecItor++ )
    {
        vecDepotCommonItemUIDList.push_back( vecItor->m_ItemUID );
    }

    return vecDepotCommonItemUIDList;
}

std::vector<GCITEMUID> KGCVirtualDepotManager::DepotExclusiveItemList()
{
    std::vector<GCITEMUID> vecDepotExclusiveUIDList;
    std::vector<KItem>::iterator vecItor = m_vecDepotExclusiveItemList.begin();

    for( vecItor = m_vecDepotExclusiveItemList.begin(); vecItor != m_vecDepotExclusiveItemList.end(); vecItor++ )
    {
        vecDepotExclusiveUIDList.push_back( vecItor->m_ItemUID );
    }

    return vecDepotExclusiveUIDList;
}


bool KGCVirtualDepotManager::FindDepotInComplateCommonItem( IN const GCITEMUID nItemUID )
{
    // 빅헤드만 적용
    if( m_iCurrentChar < GC_CHAR_LIME )
        return false;

    std::vector<KItem>::iterator vecItor;

    for( vecItor = m_vecDepotInComplateComonItemList.begin(); vecItor != m_vecDepotInComplateComonItemList.end(); vecItor++ )
    {
        if( vecItor->m_ItemUID == nItemUID )
            return true;
    }

    return false;
}

std::vector< KItem > KGCVirtualDepotManager::GetCurrentInvenList( void  )
{
    std::vector< KItem > vecCurrentCharInven;
    vecCurrentCharInven.clear();

    std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( m_iCurrentChar );

    if( mit == m_mapMoveMyInvenItemList.end() )
        return vecCurrentCharInven;

    return mit->second;

}

KItem* KGCVirtualDepotManager::InvenFindGetItem( IN const GCITEMUID nItemUID )
{
    std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( m_iCurrentChar );

    if( mit == m_mapMoveMyInvenItemList.end() )
        return NULL;

    std::vector< KItem >::iterator vit;
    for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
    {
        if( vit->m_ItemUID == nItemUID )
            return &(*vit);
    }

    return NULL;
}


void KGCVirtualDepotManager::MoveDepotToInvenItem( KItem kItem, int nCount, bool bCountItem )
{
    if( bCountItem )
        kItem.m_nCount = nCount;

    if( m_mapMoveMyInvenItemList.empty() || m_mapMoveMyInvenItemList[m_iCurrentChar].empty() )
        m_mapMoveMyInvenItemList[m_iCurrentChar].push_back(kItem);
    else
    {
        std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( m_iCurrentChar );

        if( mit == m_mapMoveMyInvenItemList.end() )
            return;

        // 동일한 수량 아이템이 이미 인벤에 있는지에 대한 처리
        bool bIsInvenExistItem = false;
        if( bCountItem )
        {
            std::vector<KItem>::iterator vit;
            for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
            {
                if( vit->m_ItemUID == kItem.m_ItemUID )
                {
                    vit->m_nCount += kItem.m_nCount;
                    bIsInvenExistItem = true;
                }
            }
        }

        if( !bIsInvenExistItem )
            m_mapMoveMyInvenItemList[m_iCurrentChar].push_back(kItem);
    }


   //아이템 창고 리스트에서 수령 감소 및 삭제 해주기

    if( m_iCurrentTab == 1 )    // 공용 아이템 탭이면
    {
        std::vector<KItem>::iterator vit;
        for( vit = m_vecDepotCommonItemList.begin(); vit != m_vecDepotCommonItemList.end(); )
        {
            if( vit->m_ItemUID == kItem.m_ItemUID )
            {
                if( bCountItem )
                {
                    vit->m_nCount -= kItem.m_nCount;

                    if( vit->m_nCount <= 0 )
                        vit =  m_vecDepotCommonItemList.erase( vit );
                }
                else
                    vit = m_vecDepotCommonItemList.erase( vit );
                break;
            }
            else
                vit++;
        }
    }
    else        // 캐릭터 전용 아이템 탭이면
    {
        std::vector<KItem>::iterator vit;
        for( vit = m_vecDepotExclusiveItemList.begin(); vit != m_vecDepotExclusiveItemList.end(); )
        {
            if( vit->m_ItemUID == kItem.m_ItemUID )
            {
                if( bCountItem )
                {
                    vit->m_nCount -= kItem.m_nCount;

                    if( vit->m_nCount <= 0 )
                        vit =  m_vecDepotExclusiveItemList.erase( vit );
                }
                else
                    vit = m_vecDepotExclusiveItemList.erase( vit );
                break;
            }
            else
                vit++;
        }
    }


    m_bIsUpdateUI = true;

}

void KGCVirtualDepotManager::MoveInvenToDepotCommon( KItem kItem, int nCount, bool bCountItem )
{

    if( IsExclusiveItem( kItem ) )
        return;

    if( bCountItem )
        kItem.m_nCount = nCount;

    bool bIsDepotExistItem = false;

    if( bCountItem )
    {
        std::vector<KItem>::iterator vit;
        for( vit = m_vecDepotCommonItemList.begin(); vit != m_vecDepotCommonItemList.end(); vit++ )
        {
            if( vit->m_ItemUID == kItem.m_ItemUID )
            {
                vit->m_nCount += kItem.m_nCount;
                bIsDepotExistItem = true;
            }
        }
    }

    if( !bIsDepotExistItem )
        m_vecDepotCommonItemList.push_back( kItem );


    std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( m_iCurrentChar );

    if( m_mapMoveMyInvenItemList.empty() )
        return;

    if( mit == m_mapMoveMyInvenItemList.end() )
        return;
    
    std::vector<KItem>::iterator vit;
    for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
    {
        if( vit->m_ItemUID == kItem.m_ItemUID )
        {
            if( bCountItem )
            {
                vit->m_nCount -= kItem.m_nCount;
                if( vit->m_nCount <= 0 )
                    vit = m_mapMoveMyInvenItemList[m_iCurrentChar].erase( vit );
            }
            else
                vit = m_mapMoveMyInvenItemList[m_iCurrentChar].erase( vit );
            break;
        }
    }

    m_bIsUpdateUI = true;
}

void KGCVirtualDepotManager::MoveInvenToDepotCharItem( KItem kItem, int nCount, bool bCountItem )
{
    if( !IsExclusiveItem( kItem ) )
        return;

    if( bCountItem )
        kItem.m_nCount = nCount;

    bool bIsDepotExistItem = false;

    if( bCountItem )
    {
        std::vector<KItem>::iterator vit;
        for( vit = m_vecDepotExclusiveItemList.begin(); vit != m_vecDepotExclusiveItemList.end(); vit++ )
        {
            if( vit->m_ItemUID == kItem.m_ItemUID )
            {
                vit->m_nCount += kItem.m_nCount;
                bIsDepotExistItem = true;
            }
        }
    }

    if( !bIsDepotExistItem )
        m_vecDepotExclusiveItemList.push_back( kItem );


    std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( m_iCurrentChar );

    if( mit == m_mapMoveMyInvenItemList.end() )
        return;

    std::vector<KItem>::iterator vit;
    for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
    {
        if( vit->m_ItemUID == kItem.m_ItemUID )
        {
            if( bCountItem )
            {
                vit->m_nCount -= kItem.m_nCount;
                if( vit->m_nCount <= 0 )
                    vit = m_mapMoveMyInvenItemList[m_iCurrentChar].erase( vit );
            }
            else
                vit = m_mapMoveMyInvenItemList[m_iCurrentChar].erase( vit );
            break;
        }
    }

    m_bIsUpdateUI = true;
}

bool KGCVirtualDepotManager::IsEmptyItemList( void )
{
    if( m_vecDepotItemList.empty() )
        return true;
    else
        return false;
}

bool KGCVirtualDepotManager::IsEmptyMyInvenItemList( void )
{
    int iMaxOpenChar = g_kGlobalValue.GetCharOpenType().size();
    int iInvensize = 0;

    for( int i = 0; i <= iMaxOpenChar; i++ )
    {
        std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( i );

        if( mit != m_mapMoveMyInvenItemList.end() )
        {
            iInvensize += mit->second.size();
        }
    }

    if( iInvensize == 0 )
        return true;

    return false;
}

void KGCVirtualDepotManager::MoveDataResualt( IN KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_ACK& kRecv )
{
    if( !kRecv.m_vecUpdateItem.empty() )
    {
        std::vector< KItem >::iterator vit = kRecv.m_vecUpdateItem.begin();
        while( vit != kRecv.m_vecUpdateItem.end() )
        {
            GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
            if( NULL == pItem ) { 
                continue;
            }

            g_pItemMgr->m_kInventory.AddItem( *vit, true );

            ++vit;
        }
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MSG1), L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_DEOPT_EXIT, 0, 0, false, true, true);
    }

    SetDepotItemList( kRecv.m_vecVirtualDepot );
}

void KGCVirtualDepotManager::SetItemCategory( void )
{
    m_vecDepotCommonItemList.clear();
    m_vecDepotInComplateComonItemList.clear();
    m_vecDepotExclusiveItemList.clear();

    SortItemList();

    std::vector< KItem >    m_vecTempDepotItemList;

    m_vecTempDepotItemList = m_vecDepotItemList;

    if( m_vecTempDepotItemList.empty() )
        return;

    std::vector< KItem >::iterator vecItor;
    for( vecItor = m_vecTempDepotItemList.begin(); vecItor != m_vecTempDepotItemList.end(); )
    {
        std::set< GCITEMID >::iterator sit;
        bool bFind = false;
        for( sit = m_setDpotRestrictItem.begin(); sit != m_setDpotRestrictItem.end(); sit++ )
        {
            if( vecItor->m_ItemID * 10 == *sit )
            {
                m_vecDepotExclusiveItemList.push_back( *vecItor );
                vecItor = m_vecTempDepotItemList.erase(vecItor);
                bFind = true;
                break;
            }
        }

        if( bFind == false )
            vecItor++;
    }

    std::vector< KItem >::iterator vit;
    for( vit = m_vecTempDepotItemList.begin(); vit != m_vecTempDepotItemList.end(); vit++ )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID );

        if( pItem == NULL )
            continue;

        if( ( pItem->dwCharType == ECT_ALL ) &&
            ( pItem->dwSlotPosition & ESP_A_UPPER_HELMET ) && 
            ( pItem->dwSlotPosition & ESP_A_DOWN_HELMET ) &&
            ( pItem->dwSlotPosition & ESP_HELMET ) )
        {
            m_vecDepotInComplateComonItemList.push_back( *vit );
        }
    }

    m_vecDepotCommonItemList = m_vecTempDepotItemList;
}

bool KGCVirtualDepotManager::IsExclusiveItem( KItem kItem )
{

    std::set< GCITEMID >::iterator sit;
    for( sit = m_setDpotRestrictItem.begin(); sit != m_setDpotRestrictItem.end(); sit++ )
    {
        if( kItem.m_ItemID * 10 == *sit )
        {
            return true;
        }
    }

    return false;

}

void KGCVirtualDepotManager::SortItemList( void )
{
    std::map< int, std::vector< KItem > > mapSortItemList;
    std::vector< KItem > vecTempITemList;

    if( m_vecDepotItemList.empty() )
        return;

    std::vector<KItem>::iterator itor;
    for( itor = m_vecDepotItemList.begin(); itor != m_vecDepotItemList.end(); itor++ ) 
    {
        GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( itor->m_ItemID );
        if ( !pItemInfo )
            continue;

        switch( pItemInfo->eItemKind )
        {
        case GIK_EQUIP:
            mapSortItemList[SORT_TYPE_EQUIP].push_back( *itor );
            break;
        case GIK_COORDI_ITEM:
            mapSortItemList[SORT_TYPE_COORDISHOP].push_back( *itor );
            break;
        case GIK_SELECT_LEVEL_ITEM:
            mapSortItemList[SORT_TYPE_SELECT_LEVEL_ITEM].push_back( *itor );
            break;
        case GIK_PET:
            mapSortItemList[SORT_TYPE_PET].push_back( *itor );
            break;
        case GIK_STRONG_ITEM:
            mapSortItemList[SORT_TYPE_STRONG_ITEM].push_back( *itor );
            break;
        case GIK_STRONG_ITEM_NOT_STRONG:
            mapSortItemList[SORT_TYPE_STRONG_ITEM_NOT_STRONG].push_back( *itor );
            break;
        case GIK_MONSTER_CARD:
            mapSortItemList[SORT_TYPE_MONSTER_CARD].push_back( *itor );
            break;
        case GIK_MISSION_SCROLL:
            mapSortItemList[SORT_TYPE_MISSION_SCROLL].push_back( *itor );
            break;
        case GIK_SHORTCUT_SLOT:
            mapSortItemList[SORT_SHORTCUT_SLOT].push_back( *itor );
            break;
        case GIK_TREASUREBOX_ITEM:
            mapSortItemList[SORT_TYPE_TREASUREBOX_ITEM].push_back( *itor );
            break;
        default:
            mapSortItemList[SORT_DEFAULT_ITEM].push_back( *itor );
            break;
        }
    }

    for( int index = 0; index <= SORT_DEFAULT_ITEM; index++ )
    {
        std::map< int, std::vector< KItem >>::iterator mit = mapSortItemList.find( index );
        if( mit != mapSortItemList.end() )
        {
            std::vector< KItem >::iterator vit;
            for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
            {
                vecTempITemList.push_back( *vit );
            }
        }
    }

    m_vecDepotItemList.swap( vecTempITemList );
}

void KGCVirtualDepotManager::LimitMoveItems( void )
{
    if( m_vecDepotCommonItemList.empty() )
        return;

    int iLimitSize = 0;
    std::vector< KItem >    m_vecCopyDepotCommonItemList = m_vecDepotCommonItemList;
    std::vector<KItem>::iterator itor;
    for( itor = m_vecCopyDepotCommonItemList.begin(); itor != m_vecCopyDepotCommonItemList.end(); itor++ ) 
    {
        if( iLimitSize >= MAX_MOVE_INVEN_MAX_SIZE )
            break;

        if( !FindDepotInComplateCommonItem(itor->m_ItemUID ) )
        {
            GCItem* pItemData = g_pItemMgr->GetItemData( itor->m_ItemID );
            if( pItemData != NULL )
            {
                if( pItemData->eItemType == GCIT_COUNT ) { 
                    MoveDepotToInvenItem( *itor, itor->m_nCount, true );
                }
                else
                {
                    MoveDepotToInvenItem( *itor, -1, false );
                }
                iLimitSize++;
            }
        }
    }
}

int KGCVirtualDepotManager::InvenLimitSize( void )
{
    int iMaxOpenChar = g_kGlobalValue.GetCharOpenType().size();
    int iInvensize = 0;

    for( int i = 0; i <= iMaxOpenChar; i++ )
    {
        std::map< int, std::vector< KItem >>::iterator mit = m_mapMoveMyInvenItemList.find( i );

        if( mit != m_mapMoveMyInvenItemList.end() )
        {
            iInvensize += mit->second.size();
        }
    }

    return iInvensize;
}

bool KGCVirtualDepotManager::IsVirtualDepotItem( IN const KItem kItem )
{
    std::vector<KItem>::iterator itor;

    for( itor = m_vecDepotItemList.begin(); itor != m_vecDepotItemList.end(); itor++ ) 
    {
        if( itor->m_ItemUID == kItem.m_ItemUID )
        {
            return true;
        }
    }
    
    return false;
}
