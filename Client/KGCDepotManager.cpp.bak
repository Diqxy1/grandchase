#include "KGCDepotManager.h"
#include "./gcui/GCDepotDlg.h"

ImplementSingleton( KGCDepotManager )

KGCDepotManager::KGCDepotManager(void)
: m_iDepotMaxHeight(0)
, m_iDepotMaxSafe(0)
, m_iCurrentSafe(0)
, m_iSlotExtendItemID(0)
, m_iSafeExtendItemID(0)
, m_iCurrentSelectSlot( 0 )
{
}

KGCDepotManager::~KGCDepotManager(void)
{
}

void KGCDepotManager::OnQuantityDepotMove(GCITEMUID m_ItemUID, int DepotStatus)
{
    KItem* pItem = NULL;

    switch (DepotStatus)
    {
    case 4:
        pItem = GetDepotItemInfo(m_ItemUID);
        break;
    default:
        pItem = g_pItemMgr->GetInventoryItem(m_ItemUID);
        break;
    }

    if (NULL == pItem) {
        return;
    }

    KItem kSellItem = *pItem;

    switch (DepotStatus)
    {
        case 2:
        {
            //  창고에 넣을수 있는 아이템인지 확인
            GCItem* pItemData = g_pItemMgr->GetItemData(pItem->m_ItemID);
            if (!IsInsertDepotItem(pItemData, pItem))
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
            }

            bool bOK = InsertDepotItem(pItem, kSellItem.m_nCount);

            //  창고가 가득 찼습니다.
            if (!bOK)
            {
                if (IsRestrictItem(pItem->m_ItemID)) {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
                        KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
                }
                else {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_16), L"",
                        KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
                }
            }

            //  갱신
            Result_DepotUpdateItemReq = INT_MAX;
            KP2P::GetInstance()->Send_DepotDBUpdateReq();
            g_MyD3D->WaitForServerAck(Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE);
            g_pkUIScene->EnableMessageBox(true);
        }
        break;
        case 3:
        {
            //  창고에 넣을수 있는 아이템인지 확인
            GCItem* pItemData = g_pItemMgr->GetItemData(pItem->m_ItemID);
            if (!IsInsertDepotItem(pItemData, pItem))
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
            }

            bool bOK = InsertDepotItemSetectSlot(pItem, kSellItem.m_nCount, GetCurrentSelectSlot());

            //  창고가 가득 찼습니다.
            if (!bOK)
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_02), L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
            }
        }
        break;
        case 4:
        {
            SiKGCDepotManager()->SendDelDepotItem(pItem, kSellItem.m_nCount);
        }
        break;
    }
}

bool KGCDepotManager::IsInsertDepotItem( GCItem* pItem, KItem* pkItem )
{
    if ( !g_pItemMgr->IsPutItemInDepot( pItem, pkItem ) )
        return false;

    if ( IsRestrictItem( pItem->dwGoodsID ) )
        return false;

    return true;
}

void KGCDepotManager::SetAllItemInventoryList()
{
	m_vecAllInventoryItem.clear();

	const KItemContainer::ItemList& vecFullInventory = g_pItemMgr->GetFullInvenItemList();
    KItemContainer::ItemList::const_iterator vitItem = vecFullInventory.begin();

    for( ; vitItem != vecFullInventory.end(); ++vitItem )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( (*vitItem).second.m_ItemID );
        if ( !pItem )
            continue;

        KItem* pkItem = g_pItemMgr->GetInventoryItem( vitItem->first );

        if( false == g_pItemMgr->IsPutItemInDepot( pItem, pkItem ) )
            continue;

        int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
        DWORD dwCharType = g_MyD3D->m_TempPlayer.GetCharTypeDword( iCharType );

        if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), (*vitItem).second.m_ItemUID ) )
        {
            continue;
        }

        std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mitSwapWeapon = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find(iCharType);
        if ( mitSwapWeapon != g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end() )
        {
            if(mitSwapWeapon->second.second.second == (*vitItem).second.m_ItemUID) 
                continue;
        }

        if (g_pItemMgr->IsNonVisibleItem(pItem->dwGoodsID))
            continue;

        if( IsRestrictItem( pItem->dwGoodsID ) ) { 
            continue;
        }

        m_vecAllInventoryItem.push_back( (*vitItem).first );
    }
}

KPetInfo* KGCDepotManager::GetPetInfo(KItem *pItem)
{
	GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( pItem->m_ItemID );
	if( pItemInfo && pItemInfo->eItemKind == GIK_PET && pItemInfo->ePetItemKind == EPET_KIND_PET )
	{
		std::map<GCITEMUID, KPetInfo>::iterator mitInven = g_MyD3D->m_mapPetInfo.find( pItem->m_ItemUID );
		std::map<GCITEMUID, KPetInfo>::iterator mitDepot = g_MyD3D->m_mapDepotPetInfo.find( pItem->m_ItemUID );

		// 펫이지만 펫 정보가 없다면 일반 텍스쳐를 로딩한다
		if( mitInven != g_MyD3D->m_mapPetInfo.end() ){
			return &mitInven->second;
		}
		else if ( mitDepot != g_MyD3D->m_mapDepotPetInfo.end() ){
			return &mitDepot->second;
		}
	}
	return NULL;
}


std::vector<GCITEMUID> KGCDepotManager::GetShowItemInventoryList()
{
	return m_vecAllInventoryItem;
}

std::vector<KItem> KGCDepotManager::GetDepotItemList( int iSafe )
{
    if ( iSafe == -1 )
        iSafe = m_iCurrentSafe;

    std::vector<KItem> vecItem;
    for(std::map<GCITEMUID,KItem>::iterator mit = m_mapDepotItemList.begin();mit != m_mapDepotItemList.end();mit++){
        if( mit->second.m_cTabID == iSafe)
        {
            vecItem.push_back(mit->second);
        }
    }
	return vecItem;
}

std::vector<GCITEMID> KGCDepotManager::CheckItemList(std::vector<GCITEMID> vecItemInfo)
{
    std::vector< GCITEMID > vecResult;
    for(std::map<GCITEMUID,KItem>::iterator mit = m_mapDepotItemList.begin();mit != m_mapDepotItemList.end();mit++){
        if ( std::find(vecItemInfo.begin(), vecItemInfo.end(), mit->second.m_ItemID) != vecItemInfo.end() )
            vecResult.push_back(mit->second.m_ItemID);
    }
    return vecResult;
}


bool KGCDepotManager::CheckDepotItem(GCITEMID iItemID)
{
    std::vector<std::pair<GCITEMID,bool>> vecResult;
    for(std::map<GCITEMUID,KItem>::iterator mit = m_mapDepotItemList.begin();mit != m_mapDepotItemList.end();mit++){
        if ( mit->second.m_ItemID == iItemID ) 
            return true;
    }
    return false;
}

bool KGCDepotManager::CheckDepotItemForLevel(GCITEMID iItemID , int iItemLevel )
{
    std::vector<std::pair<GCITEMID,bool>> vecResult;
    for(std::map<GCITEMUID,KItem>::iterator mit = m_mapDepotItemList.begin();mit != m_mapDepotItemList.end();mit++){
        if ( mit->second.m_ItemID == iItemID && mit->second.m_sEquipLevel == iItemLevel) 
            return true;
    }
    return false;
}

void KGCDepotManager::Send_DepotInfo()
{
	KP2P::GetInstance()->Send_DepotInfoReq();
}

void KGCDepotManager::SetDepotInfo(KEVENT_DEPOT_INFO_ACK kRecv)
{
	if(kRecv.m_nOK != 0) return;
	m_mapCharaterDepotInfo.clear();

	SetDepotMaxHeight(kRecv.m_nMaxSlotRow);
	SetDepotMaxSafe(kRecv.m_nMaxTabSize);

	m_iSafeExtendItemID = kRecv.m_TabExtendItemID/10;
	m_iSlotExtendItemID = kRecv.m_SlotExtendItemID/10;
	
	for(std::map< char, KDepotInfo >::iterator mit = kRecv.m_mapDepotInfo.begin();mit != kRecv.m_mapDepotInfo.end();mit++)
	{
		SetCharacterDepotInfo(mit->second);
	}
}

void KGCDepotManager::SetCharacterDepotInfo(KDepotInfo kDepot)
{
	int iSafe = kDepot.m_cTabID;
	int iSlotHeghit = kDepot.m_nSlotSize;
	m_mapCharaterDepotInfo[iSafe] = iSlotHeghit;
}

void KGCDepotManager::SetCharacterDepotRecv(KDepotInfo kDepot)
{
	int iSafe = kDepot.m_cTabID;
	m_mapCharaterDepotRecv[iSafe] = true;
}


int KGCDepotManager::GetSafeOpenSize()
{
	return m_mapCharaterDepotInfo.size();
}

int KGCDepotManager::GetSlotOpenSize( int iSafe )
{
    if ( iSafe == -1 ) 
        iSafe = m_iCurrentSafe;

    std::map<int,int>::iterator mit = m_mapCharaterDepotInfo.find(iSafe);

    if(mit == m_mapCharaterDepotInfo.end()) return 0;

    return mit->second;
}

void KGCDepotManager::Send_DepotInfoByCharTabReq()
{
    if ( IsRecvDepotData(m_iCurrentSafe) )
        return;

	KDepotInfo kPacket;
	kPacket.m_cTabID = static_cast<char>(m_iCurrentSafe);
	Result_DepotInfoByCharTabReq = INT_MAX;
	KP2P::GetInstance()->Send_DepotInfoByCharTabReq(kPacket);
	g_MyD3D->WaitForServerAck( Result_DepotInfoByCharTabReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCDepotManager::Send_DepotInfoByCharTabReqForGacha()
{
    if ( m_mapDepotItemList.empty() )
    {
        KDepotInfo kPacket;
        kPacket.m_cTabID = static_cast<char>(m_iCurrentSafe);
        Result_DepotInfoByCharTabReq = INT_MAX;
        KP2P::GetInstance()->Send_DepotInfoByCharTabReq(kPacket);
        g_MyD3D->WaitForServerAck( Result_DepotInfoByCharTabReq, INT_MAX, 5000, TIME_OUT_VALUE );
    }
}

void KGCDepotManager::AddDepotItem(KItem kItem,bool DivID)
{
	if(DivID){
		kItem.m_ItemID /= 10;
        kItem.m_DesignCoordiID /= 10;
	}

    // 수량성 아이템이 아니면 바로 넣고 수량성이면 갯수만 조정 가능하게..
    if( kItem.m_nCount == -1 ) { // 영구
        m_mapDepotItemList[kItem.m_ItemUID] = kItem;    
    }
    else {  // 수량
        bool bExistItem = false;
        std::map<GCITEMUID, KItem>::iterator mit = m_mapDepotItemList.begin();
        for( ; mit != m_mapDepotItemList.end() ; ++mit ) { 
            // 이미 있는 아이템이라면 수량 갱신
            if( mit->second.m_ItemID == kItem.m_ItemID ) {  
                mit->second.m_nCount = kItem.m_nCount;
                bExistItem = true;
                break;
            }
        }

        if( false == bExistItem ) { 
            m_mapDepotItemList[kItem.m_ItemUID] = kItem;
        }
    }
}

void KGCDepotManager::DelDepotItem(KItem kItem, const bool bDiv_ /*= true*/, const bool bCountItem /*= false*/ )
{
    if( false == bCountItem ) { 
        m_mapDepotItemList.erase(kItem.m_ItemUID);
    }
    else { 
        if( kItem.m_nCount == 0 ) { 
            m_mapDepotItemList.erase( kItem.m_ItemUID );
        }
        else { 
            if( bDiv_ ) { 
                kItem.m_ItemID /= 10;
            }

            m_mapDepotItemList[ kItem.m_ItemUID ] = kItem;
        }
    }
}

void KGCDepotManager::SwapPetInfo(KItem kItem, std::map<GCITEMUID, KPetInfo> &delList, std::map<GCITEMUID, KPetInfo> &addlist)
{
	GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( kItem.m_ItemID/10 );
	if( pItemInfo && pItemInfo->eItemKind == GIK_PET && pItemInfo->ePetItemKind == EPET_KIND_PET ){
		std::map<GCITEMUID, KPetInfo>::iterator mit = delList.find(kItem.m_ItemUID);
		if(mit != delList.end()){
			addlist.insert(std::pair<GCITEMUID,KPetInfo>(mit->first,mit->second));
			delList.erase(mit);
		}
	}
}

void KGCDepotManager::AddDepotItemList(std::vector<KItem> vecItem)
{
	for(std::vector<KItem>::iterator vIt =  vecItem.begin() ; vIt != vecItem.end();vIt++)
	{
		AddDepotItem(*vIt, true);
	}
}

void KGCDepotManager::AddInvenItem(GCITEMUID dwItemUID)
{
    std::vector<GCITEMUID>::iterator vIt = std::find(m_vecAllInventoryItem.begin(),m_vecAllInventoryItem.end(), dwItemUID);

    if(vIt == m_vecAllInventoryItem.end()) 
        m_vecAllInventoryItem.push_back(dwItemUID);
}

void KGCDepotManager::DelInvenItem(GCITEMUID dwItemUID, const bool bCountItem_ /*= false*/)
{
    std::vector<GCITEMUID>::iterator vIt = std::find(m_vecAllInventoryItem.begin(),m_vecAllInventoryItem.end(), dwItemUID);

    if(vIt != m_vecAllInventoryItem.end()) { 
        if( false == bCountItem_ ) { 
            m_vecAllInventoryItem.erase(vIt);
        }
    }
}

void KGCDepotManager::SendMoveDepotItem(KItem *pItem, int iSlot)
{
	if(pItem == NULL) return;
	KDepotKey oldkey;
	oldkey.m_cTabID = static_cast<char>(m_iCurrentSafe);
	oldkey.m_cSlotID = static_cast<char>(pItem->m_cSlotID);

	KDepotKey newkey;
	newkey.m_cTabID = static_cast<char>(m_iCurrentSafe);
	newkey.m_cSlotID = static_cast<char>(iSlot);


    KEVENT_DEPOT_MOVE_ITEM_REQ kPacket;
    kPacket.m_ItemID = pItem->m_ItemID * 10;
    kPacket.m_ItemUID = pItem->m_ItemUID;
    kPacket.m_kOldPos = oldkey;
    kPacket.m_kNewPos = newkey;

    Result_DepotItemReq = INT_MAX;
    KP2P::GetInstance()->Send_DepotMoveItem(kPacket);
    g_MyD3D->WaitForServerAck( Result_DepotItemReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCDepotManager::SendAddDepotItem(KItem *pItem, int iSlot, int iSafe, const int iAddCount_ /*= -1*/ )
{
    if ( iSafe == -1 )
        iSafe = m_iCurrentSafe;

    if(pItem == NULL) return;

    // 슬롯이 오픈된 슬롯보다 큰 경우 경고 하나 띄우고 아무런 액션 취하지 않음.
    if ( iSlot >= GetSlotOpenSize() ) {
        return;
    }

	KDepotKey key;
    key.m_cTabID = static_cast<char>(iSafe);
	key.m_cSlotID = static_cast<char>(iSlot);
	
	KDepotItem kPacket;
	kPacket.m_ItemID = pItem->m_ItemID * 10;
	kPacket.m_ItemUID = pItem->m_ItemUID;
	kPacket.m_kDepotKey = key;
    kPacket.m_nCount = iAddCount_;

    Result_DepotItemReq = INT_MAX;
    KP2P::GetInstance()->Send_DepotInsertItemReq(kPacket);
    g_MyD3D->WaitForServerAck( Result_DepotItemReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCDepotManager::SendDelDepotItem(KItem *pItem, const int iDellCount /*= -1*/ )
{
    if(pItem == NULL) return;

    //코디아이템일땐 코디 인벤토리가 꽉차 있는지 검사
    if( pItem->m_cItemType == KItem::TYPE_LOOK )
    {
        if( g_pItemMgr->CheckExtraCoordiExist() == false ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString(STR_ID_COORDI_INVENTORY_FULL_ERROR) 
                , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
            return;
        }
    }
    else
    {
        //인벤토리가가 꽉차있을 때는 경고 하나띄우고 아무런 행동도 하지 않는다;
        if( g_pItemMgr->CheckExtraInventoryExist() == false ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1) 
                , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
            return;
        }
    }
    

	KDepotKey key;
	key.m_cTabID = static_cast<char>(m_iCurrentSafe);
	key.m_cSlotID = static_cast<char>(pItem->m_cSlotID);

    KDepotItem kPacket;
    kPacket.m_ItemID = pItem->m_ItemID * 10;
    kPacket.m_ItemUID = pItem->m_ItemUID;
    kPacket.m_kDepotKey = key;
    kPacket.m_nCount = iDellCount;

    Result_DepotItemReq = INT_MAX;
    KP2P::GetInstance()->Send_DepotDeleteItemReq(kPacket);
    g_MyD3D->WaitForServerAck( Result_DepotItemReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

KItem* KGCDepotManager::GetDepotItemInfo(GCITEMUID dwItemUID)
{
	std::map<GCITEMUID, KItem>::iterator mIt = m_mapDepotItemList.find(dwItemUID);
	if(mIt == m_mapDepotItemList.end()) return NULL;
	return &(mIt->second);
}

GCItem* KGCDepotManager::GetDepotItemDataByItemID( const GCITEMID itemID_ )
{
    std::map< GCITEMUID, KItem >::const_iterator mit = m_mapDepotItemList.begin();
    for( ; mit != m_mapDepotItemList.end() ; ++mit ) { 
        if( mit->second.m_ItemID == itemID_ ) { 
            return g_pItemMgr->GetItemData( itemID_ );
        }
    }

    return NULL;
}

void KGCDepotManager::ClearRecvItemList()
{
	m_mapCharaterDepotRecv.clear();
	m_mapDepotItemList.clear();
}

void KGCDepotManager::SetDepotCharTabInfo(KEVENT_DEPOT_CHAR_TAB_INFO_ACK kRecv)
{
    std::map< char, KDepotInfo >::const_iterator mit;
    for ( mit = kRecv.m_mapDepotInfo.begin(); mit != kRecv.m_mapDepotInfo.end(); ++mit )
    {
        SetCharacterDepotInfo(mit->second);
        SetCharacterDepotRecv(mit->second);
    }
    AddDepotItemList(kRecv.m_vecItem);

    for(std::map<GCITEMUID, KPetInfo>::iterator mIter = kRecv.m_mapPetInfo.begin(); mIter != kRecv.m_mapPetInfo.end();mIter++){
        mIter->second.m_dwID /= 10;
    }

    g_MyD3D->m_mapDepotPetInfo = kRecv.m_mapPetInfo;

    g_pItemMgr->m_kInventory.AddItemList(kRecv.m_vecInvenItem, true);
	
}

void KGCDepotManager::SendSafeAdd( )
{
	// 금고 갯수 확인
	if( GetDepotMaxSafe() <= GetSafeOpenSize() )
	{
		// 더이상 확장 할수 없다고~!

		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString(STR_ID_DEPOT_EXTEND_FULL_SAFE) 
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
		return;
	}
	// 금고 확장 필요 아이템 확인
	if ( FindExtendSafeItem() == false )
	{
		BuyItem(m_iSafeExtendItemID);
		// 아이템 없자나 없으니깐 사라~.
		return;
	}
	
	KEVENT_DEPOT_EXTEND_REQ kPacket;

	kPacket.m_nExtendType = KDepotExtend::TAB_EXTEND;
	kPacket.m_TabID = static_cast<char>(m_mapCharaterDepotInfo.size());

	Result_DepotItemReq = INT_MAX;
	KP2P::GetInstance()->Send_DepotExtendReq( kPacket );
	g_MyD3D->WaitForServerAck( Result_DepotItemReq, INT_MAX, 5000, TIME_OUT_VALUE );

    if ( 0 != Result_DepotItemReq ) {
        g_pkUIScene->m_pkDepot->Destroy();
    }
}

void KGCDepotManager::SendSlotAdd( )
{
	// 슬롯 갯수 확인
	if( GetDepotMaxSlot() <= GetSlotOpenSize() )
	{
		// 더이상 확장 할수 없다고~!
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString(STR_ID_DEPOT_EXTEND_FULL_SLOT) 
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
		return;
	}
	// 금고 확장 필요 아이템 확인
	if ( FindExtendSlotItem() == false )
	{
		BuyItem(m_iSlotExtendItemID);
		// 아이템 없자나 없으니깐 사라~.
		return;
	}

	KEVENT_DEPOT_EXTEND_REQ kPacket;
	kPacket.m_nExtendType = KDepotExtend::SLOT_EXTEND;
	kPacket.m_TabID = static_cast<char>(m_iCurrentSafe);

	Result_DepotItemReq = INT_MAX;
	KP2P::GetInstance()->Send_DepotExtendReq( kPacket );
	g_MyD3D->WaitForServerAck( Result_DepotItemReq, INT_MAX, 5000, TIME_OUT_VALUE );

    if ( 0 != Result_DepotItemReq ) {
        g_pkUIScene->m_pkDepot->Destroy();
    }
}

bool KGCDepotManager::FindExtendSafeItem( )
{
	if ( g_pItemMgr->GetInventoryItemFromID( m_iSafeExtendItemID ) == NULL )
		return false;
	return true;
}

bool KGCDepotManager::FindExtendSlotItem( )
{
	if ( g_pItemMgr->GetInventoryItemFromID( m_iSlotExtendItemID ) == NULL )
		return false;
	return true;
}

void KGCDepotManager::BuyItem(GCITEMID buyItemID )
{
	GCItem* pItem = g_pItemMgr->GetItemData( buyItemID );

	if( pItem->eMoneyType == EMT_CASH )
	{
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( buyItemID, true );
#else
    #ifdef GAME_CASH
		g_pkUIScene->EnableBuyCashItemWnd( buyItemID, false, false, true );
    #else
		g_MyD3D->m_kItemMgr.BuyItem( buyItemID, pItem->iItemTypeValue  );
    #endif
#endif
	}
	else
	{

		std::wostringstream strmText1;
		std::wostringstream strmText2;

#if defined ( _PORTUGUESE ) || defined( _SPANISH )
		strmText1 << pItem->strItemName;
		strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH ) || defined (NATION_THAILAND)
		strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
		strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
		strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif

		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
			strmText1.str(), strmText2.str(),
			KGCUIScene::GC_MBOX_USE_BUY, buyItemID, pItem->iItemTypeValue, true, true );
	}
}

bool KGCDepotManager::InsertDepotItem( KItem *pItem, const int iInsertCount_ /*= -1*/ )
{
    int nSafeSize = GetSafeOpenSize();

    for ( int i = 0; i < nSafeSize; ++i ) 
    {
        if ( !IsRecvDepotData( i ) )
        {
            KDepotInfo kPacket;
            kPacket.m_cTabID = static_cast<char>(0);
            Result_DepotInfoByCharTabReq = INT_MAX;
            KP2P::GetInstance()->Send_DepotInfoByCharTabReq(kPacket);
            g_MyD3D->WaitForServerAck( Result_DepotInfoByCharTabReq, INT_MAX, 5000, TIME_OUT_VALUE );
        }

        // 현재 탭의 슬롯 사이즈 / 아이템 리스트 
        int nSlotSize = GetSlotOpenSize( i );
        std::vector<KItem> vecItem = GetDepotItemList( i );

        // 동일한 수량 아이템이 이미 창고에 있는가?
        bool bIsExistItem = false;
        GCItem* pDepotItemData = GetDepotItemDataByItemID( pItem->m_ItemID );
        if( pDepotItemData && ( pDepotItemData->dwGoodsID == pItem->m_ItemID ) ) {   
            bIsExistItem = true;
        }

        // 동일 수량 아이템의 슬롯 번호 
        int iExistItemSlot = -1;
        for( std::vector< KItem >::iterator vit = vecItem.begin() ; vit != vecItem.end() ; ++vit ) { 
            ++iExistItemSlot;
            if( pDepotItemData && ( pDepotItemData->dwGoodsID == vit->m_ItemID ) ) { 
                break;
            }
        }

        //  사이즈가 같다는거는 더이상 넣을때가 없다는 거이다.
        if ( false == bIsExistItem && nSlotSize == (int)vecItem.size() )
            continue;

        std::set<int> setSlotList;
        for ( int j = 0; j < (int)vecItem.size(); ++j ) {
            setSlotList.insert( vecItem[j].m_cSlotID );
        }

        if( bIsExistItem && -1 != iExistItemSlot ) { 
            SendAddDepotItem( pItem, iExistItemSlot, i, iInsertCount_ );
            return true;
        }
        else {
            for ( int nSlot = 0; nSlot < nSlotSize; ++nSlot )
            {
                if ( setSlotList.find( nSlot ) == setSlotList.end() )
                {
                    //  해당 슬롯에 아이템을 넣는다.
                    SendAddDepotItem( pItem, nSlot, i, iInsertCount_ ); 
                    return true;
                }
            }
        }
    }

    return false;
}

bool KGCDepotManager::InsertDepotItemSetectSlot( KItem* pItem, const int iInsertCount_ /*= -1*/, int iSlot_ /*= -1 */ )
{
    if( 0 == GetSafeOpenSize() ) { 
        return false;
    }

    if ( !IsRecvDepotData( m_iCurrentSafe ) )
    {
        KDepotInfo kPacket;
        kPacket.m_cTabID = static_cast<char>(0);
        Result_DepotInfoByCharTabReq = INT_MAX;
        KP2P::GetInstance()->Send_DepotInfoByCharTabReq(kPacket);
        g_MyD3D->WaitForServerAck( Result_DepotInfoByCharTabReq, INT_MAX, 5000, TIME_OUT_VALUE );
    }

    //  해당 슬롯에 아이템을 넣는다.
    SendAddDepotItem( pItem, iSlot_, m_iCurrentSafe, iInsertCount_ ); 

    return true;
}


bool KGCDepotManager::IsRecvDepotData( int iSafe )
{
    if ( iSafe == -1 ) 
        iSafe = m_iCurrentSafe;

    std::map<int,bool>::iterator mapIterSafe = m_mapCharaterDepotRecv.find(iSafe);

    if(mapIterSafe == m_mapCharaterDepotRecv.end())
        return false;

    return true;

}

void KGCDepotManager::Initialize()
{
    m_mapDepotItemList.clear();
    m_mapCharaterDepotRecv.clear();
}

void KGCDepotManager::SetDepotRestrictItem( const std::set< GCITEMID >& setList_ )
{
    m_setDpotRestrictItem.clear();
    m_setDpotRestrictItem = setList_;
}

bool KGCDepotManager::IsRestrictItem( const GCITEMID itemID )
{
    return ( m_setDpotRestrictItem.find( itemID * 10 ) != m_setDpotRestrictItem.end() );
}