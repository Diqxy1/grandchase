#include "stdafx.h"
#include "GCForgeManager.h"
#include "GCUI/KGCNewForgeDlg.h"


ImplementSingleton(GCForgeManager);

GCForgeManager::GCForgeManager(void)
: m_iCurrentCharacter(0)
, m_iCurrentSlot(0)
, m_funcRecipeSort(NULL)
, m_Ascending(false)
, m_Result_ManufactureS3_Material_Get(0)
, m_bEnableBreakUp(false)
, m_fBreakUpMaterialRatioMin(1.0f)
, m_fBreakUpMaterialRatioMax(1.0f)
{
    m_mapBreakUpMaterialItemID.clear();
    m_mapBreakUpMaterialInfo.clear();
    m_mapBreakUpSpecificBreakupInfo.clear();
}

GCForgeManager::~GCForgeManager(void)
{
}

void GCForgeManager::SetRecipeAllList( std::vector<KManufactureItem> &vecRecipeAllList_ )
{
    m_vecRecipeAllList.clear();
    m_vecRecipeAllList.swap(vecRecipeAllList_);

    for(std::vector<KManufactureItem>::iterator vit = m_vecRecipeAllList.begin();vit != m_vecRecipeAllList.end();vit++){
        (*vit).m_ItemID /= 10;
    }
}

void GCForgeManager::Send_GetItemCatalog()
{
    if(!m_vecRecipeAllList.empty()) return;

    m_Result_ManufactureS3_Material_Get = INT_MAX;
    KP2P::GetInstance()->Send_GetItemCatalog();
    g_MyD3D->WaitForServerAck( m_Result_ManufactureS3_Material_Get, INT_MAX, 3000, TIME_OUT_VALUE );

}

bool GCForgeManager::CheckRecipeContinentType(IN std::vector<KManufactureItem>::iterator vitRecipe )
{
    std::vector<EGCWorldMap> veceRecipeContinentType; 

    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_BERMESIA_SERDIN) )
        veceRecipeContinentType.push_back(WORLDMAP_BERMESIA_SERDIN);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_BERMESIA_CANAVAN) )
        veceRecipeContinentType.push_back(WORLDMAP_BERMESIA_CANAVAN);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_SILVERLAND) )
        veceRecipeContinentType.push_back(WORLDMAP_SILVERLAND);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_ELIA) )
        veceRecipeContinentType.push_back(WORLDMAP_ELIA);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_XENIA) )
        veceRecipeContinentType.push_back(WORLDMAP_XENEA);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_ATUM) )
        veceRecipeContinentType.push_back(WORLDMAP_ATUM);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_ACHAEMEDIA) )
        veceRecipeContinentType.push_back(WORLDMAP_ACHAEMEDIA);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_FLOSLAND) )
        veceRecipeContinentType.push_back(WORLDMAP_FLOSLAND);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_DEVILDOM) )
        veceRecipeContinentType.push_back(WORLDMAP_DEVILDOM);
    if(  (*vitRecipe).CheckContinentFlag(KManufactureItem::ECF_ELYOS) )
        veceRecipeContinentType.push_back(WORLDMAP_ELYOS);

    for( std::vector<EGCWorldMap>::iterator vitRecipeContinentType = veceRecipeContinentType.begin(); vitRecipeContinentType!= veceRecipeContinentType.end(); ++vitRecipeContinentType )
    {
        if( (*vitRecipeContinentType) == SiKGCWorldMapManager()->GetCurrentWorldMap() )
		{
            return true;
		}
	}
    
	return false;
}
void GCForgeManager::SetShowRecipeList()
{
    m_vecShowRecipeList.clear();
    std::set< PAIR_USHORT_DWORD > setSendItemIDList;
    PAIR_USHORT_DWORD pairItemInfo;
    
    m_iCurrentCharacter = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    
    for( std::vector<KManufactureItem>::iterator vitRecipe = m_vecRecipeAllList.begin(); vitRecipe != m_vecRecipeAllList.end(); ++vitRecipe )
	{
        GCItem* pItem = g_pItemMgr->GetItemData( (*vitRecipe).m_ItemID );

        if( !pItem || !(pItem->dwCharType & ( 1 << m_iCurrentCharacter)) ) 
		{
            continue;
		}

        bool bShowRecipe = false;
       
		if( m_iCurrentSlot == ESFB_ALL )
		{
			bShowRecipe = true;
		}
        else if (((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_EVENT) && ESFB_EVENT == m_iCurrentSlot)
		{
            bShowRecipe = true;
        }
        else if (((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_VIP) && ESFB_VIP == m_iCurrentSlot)
        {
            bShowRecipe = true;
        }
        else if ((!((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_EVENT) && !((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_VIP) && ESFB_CARD == m_iCurrentSlot && GIK_MONSTER_CARD == pItem->eItemKind))
        {
            bShowRecipe = true;
        }
        else if ( ( !((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_VIP) && !((*vitRecipe).m_dwItemFlag & KManufactureItem::IF_EVENT) && ( ( ESFB_ETC == m_iCurrentSlot && 0 == pItem->dwSlotPosition ) || !(ESFB_CARD == m_iCurrentSlot && GIK_MONSTER_CARD == pItem->eItemKind) && (pItem->dwSlotPosition & (GetSlotFrag())))) )
		{
			bShowRecipe = true;
        }

        if ( bShowRecipe )
		{
            if( !CheckRecipeContinentType( vitRecipe ) )
			{
                continue;
			}

            RecipeSimpleInfo stRecipeSimpleInfo;
            if( (*vitRecipe).m_nLevel != -1 )
			{ 
                stRecipeSimpleInfo.iItemLevel = (*vitRecipe).m_nLevel;
            }
            else
			{
                stRecipeSimpleInfo.iItemLevel = pItem->iNeedLevelForEquippingItem;
            }


            stRecipeSimpleInfo.iPrice = (*vitRecipe).m_nPrice;
            stRecipeSimpleInfo.strItemName = pItem->strItemName;
            stRecipeSimpleInfo.ItemID = pItem->dwGoodsID;
            stRecipeSimpleInfo.usIndex = (*vitRecipe).m_Index;
            stRecipeSimpleInfo.bHasDiscount = (*vitRecipe).m_bIsOnDiscount;

			if( (*vitRecipe).m_nGrade == -1)
			{
				(*vitRecipe).m_nGrade = pItem->cItemGrade;
			}
            
			stRecipeSimpleInfo.cGrade = static_cast<char>((*vitRecipe).m_nGrade);
            stRecipeSimpleInfo.cFactor = ( (*vitRecipe).m_nDuration != -1 ) ? (*vitRecipe).m_nDuration : 1;
            m_vecShowRecipeList.push_back( stRecipeSimpleInfo );

            pairItemInfo.first = stRecipeSimpleInfo.usIndex;
            pairItemInfo.second = stRecipeSimpleInfo.ItemID;

            if(m_mapAllMeterialList.find(pairItemInfo) == m_mapAllMeterialList.end())
            {
                setSendItemIDList.insert(pairItemInfo);
            }
        }
    }

    Send_GetMeterials(setSendItemIDList);
    std::sort( m_vecShowRecipeList.begin(), m_vecShowRecipeList.end(), boost::bind(m_funcRecipeSort, _1, _2, m_Ascending ) );
}

void GCForgeManager::Send_GetMeterials(std::set< PAIR_USHORT_DWORD >& setItemList)
{
    if(setItemList.empty()) return;
    
    m_Result_ManufactureS3_Material_Get = INT_MAX;
    KP2P::GetInstance()->Send_GetMaterials( setItemList );
    g_MyD3D->WaitForServerAck( m_Result_ManufactureS3_Material_Get, INT_MAX, 3000, TIME_OUT_VALUE );
}

void GCForgeManager::AddMaterialList( MAP_FM_MATERIAL_LIST& mapAddMaterialList_ )
{
    MAP_FM_MATERIAL_LIST::iterator mit = mapAddMaterialList_.begin();

    for(; mit != mapAddMaterialList_.end(); ++mit )
    {
        PAIR_USHORT_DWORD pairTmp = std::make_pair( mit->first.first, mit->first.second / 10 );
        std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >::iterator mitFind = m_mapAllMeterialList.find( pairTmp );

        std::vector<KDropItemInfo>::iterator vitMaterialList = mit->second.begin();
        for( ; vitMaterialList != mit->second.end(); ++vitMaterialList )
        {
            (*vitMaterialList).m_ItemID /= 10;
        }

        if( m_mapAllMeterialList.end() != mitFind )
		{
            mitFind->second.swap( mit->second );
		}
        else
		{
            m_mapAllMeterialList.insert( MAP_FM_MATERIAL_LIST::value_type( pairTmp, mit->second) );
		}
	}
}

void GCForgeManager::InitSort()
{
    m_funcRecipeSort = GCForgeManager::SortByItemLevel;
    m_Ascending = false;
}

void GCForgeManager::SetSort(int iSortType, bool Ascending_)
{
    m_Ascending = Ascending_;

    switch(iSortType)
	{
        case ERSB_ITEMLEVEL :
            m_funcRecipeSort = GCForgeManager::SortByItemLevel;
            break;
        case ERSB_ITEMNAME :
            m_funcRecipeSort = GCForgeManager::SortByItemName;
            break;
        case ERSB_PRICE :
            m_funcRecipeSort = GCForgeManager::SortByPrice;
            break;
		case ERSB_ITEMGRADE:
			m_funcRecipeSort = GCForgeManager::SortByGrade;
			break;

        default:
            m_funcRecipeSort = GCForgeManager::SortByItemLevel;
    }

    std::sort( m_vecShowRecipeList.begin(), m_vecShowRecipeList.end(), boost::bind(m_funcRecipeSort, _1, _2, m_Ascending ) );
}


bool GCForgeManager::SortByItemLevel( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ )
{
    if( bAscending_ )
	{
        if( lhs.iItemLevel == rhs.iItemLevel )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.iItemLevel < rhs.iItemLevel;
    }
    else
	{   
        if( lhs.iItemLevel == rhs.iItemLevel )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.iItemLevel > rhs.iItemLevel;
    }
}

bool GCForgeManager::SortByItemName( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ )
{
    if( bAscending_ )
	{
        if( lhs.strItemName == rhs.strItemName )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.strItemName < rhs.strItemName;
    }
    else
	{   
        if( lhs.strItemName == rhs.strItemName )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.strItemName > rhs.strItemName;
    }
}

bool GCForgeManager::SortByPrice( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ )
{
    if( bAscending_ )
	{
        if( lhs.iPrice == rhs.iPrice )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.iPrice < rhs.iPrice;
    }
    else
	{   
        if( lhs.iPrice == rhs.iPrice )
            return lhs.ItemID < rhs.ItemID;
        else
            return lhs.iPrice > rhs.iPrice;
    }
}

bool GCForgeManager::SortByGrade( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ )
{
	if( bAscending_ )
	{
		if( lhs.cGrade == rhs.cGrade )
			return lhs.ItemID < rhs.ItemID;
		else
			return lhs.cGrade < rhs.cGrade;
	}
	else
	{   
		if( lhs.cGrade == rhs.cGrade )
			return lhs.ItemID < rhs.ItemID;
		else
			return lhs.cGrade > rhs.cGrade;
	}
}

int GCForgeManager::GetSlotFrag( )
{
    switch(m_iCurrentSlot)
    {
	case ESFB_ALL:
		return ESP_ALL;
    case ESFB_EQUIPMENT:
        return ESP_WEAPON | ESP_HELMET | ESP_JACKET | ESP_PANTS | ESP_GLOVES | ESP_SHOES | ESP_MANTLE;
    case ESFB_ACCESSORY:
        return ESP_A_UPPER_HELMET | ESP_A_DOWN_HELMET | ESP_A_JACKET | ESP_A_PANTS | ESP_A_ARM | ESP_A_FINGER | ESP_A_SHOES;
    case ESFB_ETC:
        return ~(ESP_WEAPON | ESP_HELMET | ESP_JACKET | ESP_PANTS | ESP_GLOVES | ESP_SHOES | ESP_MANTLE | ESP_A_UPPER_HELMET | ESP_A_DOWN_HELMET | ESP_A_JACKET | ESP_A_PANTS | ESP_A_ARM | ESP_A_FINGER | ESP_A_SHOES | ESP_HELMET_SOCKET | ESP_WEAPON_SOCKET | ESP_JACKET_SOCKET | ESP_A_HELMET_SOCKET);
    default:
        return 0;
    }
    return 0;
}

void GCForgeManager::MakeAllBreakUpItemList()
{
    m_vecAllBreakUpItemList.clear();
    const KItemContainer::ItemList& vecFullInventory = g_pItemMgr->GetFullInvenItemList();
    KItemContainer::ItemList::const_iterator vitItem = vecFullInventory.begin();

    int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    DWORD dwCharType = g_MyD3D->m_TempPlayer.GetCharTypeDword(iCharType);

    for( ; vitItem != vecFullInventory.end(); ++vitItem )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( (*vitItem).second.m_ItemID );

        if( NULL == pItem || false == g_pItemMgr->IsBreakUpItem( pItem->dwGoodsID ) )
		{
            continue;
		}

        if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), (*vitItem).second.m_ItemUID ))
        {
            continue;
        }

        m_vecAllBreakUpItemList.push_back( (*vitItem).first  );
    }

    g_pItemMgr->FilterRemoveCoordiItem( m_vecAllBreakUpItemList );   
    g_pItemMgr->FilterRemoveOtherChar( m_vecAllBreakUpItemList, iCharType);
}

void GCForgeManager::SetShowBreakUpItemList()
{
    m_vecShowBreakUpItemList.clear();
    m_vecShowBreakUpItemList = m_vecAllBreakUpItemList;
    int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    struct ExceptItem 
	{
        const std::set<GCITEMUID>& m_setItemUID;
        ExceptItem( const std::set<GCITEMUID>& setItemUID_ ):m_setItemUID(setItemUID_) {};
        ~ExceptItem() {};

        bool operator() ( GCITEMUID arg ) 
		{
            return m_setItemUID.find( arg ) != m_setItemUID.end();
        }
    };

    m_vecShowBreakUpItemList.erase( std::remove_if( m_vecShowBreakUpItemList.begin(), m_vecShowBreakUpItemList.end(), ExceptItem(m_setWaitItemList) ), m_vecShowBreakUpItemList.end() );
    m_vecShowBreakUpItemList.erase( std::remove_if( m_vecShowBreakUpItemList.begin(), m_vecShowBreakUpItemList.end(), ExceptItem(m_setRemovedItemList) ), m_vecShowBreakUpItemList.end() );

    for( auto& element : g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo )
    {
        g_pItemMgr->FilterEquipedItem( &element, m_vecShowBreakUpItemList );
        g_pItemMgr->FilterEquipedItem( &element, m_vecShowBreakUpItemList, true );
    }

    g_pItemMgr->FilterRemoveOtherChar( m_vecAllBreakUpItemList, iCharType );
}

bool GCForgeManager::GetMaterialList(int iIndex,GCITEMID giItemID, OUT std::vector<SMaterialInfo>& vInfo)
{
    bool isDiscountOn = true;
    int totalDiscount = 0;
    g_kGlobalValue.currentTotalForgeDiscount = 0;
    for( auto element : m_vecShowRecipeList )
    {
        if ( element.ItemID == giItemID )
        {
            isDiscountOn = element.bHasDiscount;
            break;
        }
    }
    if( isDiscountOn )
    {
        if ( g_pItemMgr->FindInventoryForItemID( ITEM_BLACKSMITH_DISCOUNT ) )
            g_kGlobalValue.currentTotalForgeDiscount = 0.15;
        if ( g_pItemMgr->FindInventoryForItemID( ITEM_VIP_TEMPORARY_ITEM ) )
            g_kGlobalValue.currentTotalForgeDiscount = 0.30;
    }

    MAP_FM_MATERIAL_LIST::iterator mIt = m_mapAllMeterialList.find( PAIR_USHORT_DWORD( iIndex,giItemID ) );
    if( mIt == m_mapAllMeterialList.end() ) return false;

    std::vector< KDropItemInfo>& vecTemp = mIt->second;

    vInfo.clear();
    std::set<GCITEMUID> setUsedUID;
    const KItemContainer::ItemList& vecInvenList = g_pItemMgr->GetFullInvenItemList();
    std::vector<KDropItemInfo>::const_iterator vit;

    for( vit = vecTemp.begin() ; vit != vecTemp.end() ; ++vit ) 
	{
        if ( isDiscountOn && g_kGlobalValue.currentTotalForgeDiscount > 0 && (*vit).m_nDuration > 0 )
            totalDiscount = (int)ceil( (*vit).m_nDuration - ( (*vit).m_nDuration * g_kGlobalValue.currentTotalForgeDiscount ) );
        else
            totalDiscount = (*vit).m_nDuration;
        SMaterialInfo sMaterial;
        sMaterial.m_ItemID = (*vit).m_ItemID;
        sMaterial.m_nDuration = totalDiscount;
		sMaterial.m_nLevel = (*vit).m_cEquipLevel;
        sMaterial.m_nPeriod = (*vit).m_nPeriod;
        KItemContainer::ItemList::const_iterator itorInven = vecInvenList.begin();
        for( ; itorInven != vecInvenList.end() ; ++itorInven ) 
		{
            if( setUsedUID.find( itorInven->first ) != setUsedUID.end() )
                continue;

            if( itorInven->second.m_ItemID != (*vit).m_ItemID )
                continue;

            setUsedUID.insert( itorInven->first );
            sMaterial.m_bExist = true;
            break;
        }

        vInfo.push_back( sMaterial );
    }

    return true;
}

int GCForgeManager::GetMaxCreateCount(int iIndex,GCITEMID giItemID )
{
    std::vector<SMaterialInfo> vInfo;
    if(GetMaterialList(iIndex, giItemID, vInfo) == false) return 1;

    int iMaxCount = INT_MAX;

    for( std::vector<SMaterialInfo>::iterator vIt = vInfo.begin();vIt != vInfo.end();++vIt ){
        KItem *pItem = NULL;
        g_pItemMgr->FindInventoryForItemID( vIt->m_ItemID,&pItem );
        if( pItem == NULL ) continue;
        if( vIt->m_nDuration < 1 ) continue;

        int iMax = pItem->m_nCount / vIt->m_nDuration;

        if( iMax < iMaxCount )
            iMaxCount = iMax;
    }

    if( iMaxCount == INT_MAX || iMaxCount < 1 )
        iMaxCount = 1;
    return iMaxCount;
}


bool GCForgeManager::GetMaterialColor( GCITEMID giItemID ,int nItemDuration, int nLevel, DWORD& imgColor, DWORD& fontcolor)
{
    KItem* kItem = NULL;

    if( g_pItemMgr->FindInventoryForItemID( giItemID, &kItem ) == false )
	{
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

	if( kItem->m_sEquipLevel < nLevel )
	{
		imgColor = 0xff666666;
		fontcolor = 0xffff0000;
		return false;
	}

    if(kItem->m_nCount < nItemDuration && nItemDuration != -1)
	{
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

    if( kItem->m_cItemType != KItem::TYPE_NORMAL ) 
	{ 
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

    imgColor = 0xffffffff;
    fontcolor = 0xffffffff;
    return true;
}

bool GCForgeManager::GetMaterialColorByUID( GCITEMUID itemmUID_ ,int nItemDuration, int nLevel, DWORD& imgColor, DWORD& fontcolor )
{
    KItem* pInvenItem = NULL;
    if( false == g_pItemMgr->FindInventory( itemmUID_, &pInvenItem ) ) { 
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

    if( pInvenItem->m_nCount < nItemDuration && nItemDuration != -1 ) {
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

	if( pInvenItem->m_cEnchantLevel < nLevel )
	{
		imgColor = 0xff666666;
		fontcolor = 0xffff0000;
		return false;
	}

    if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), itemmUID_ ) ) 
	{
        imgColor = 0xff666666;
        fontcolor = 0xffff0000;
        return false;
    }

    imgColor = 0xffffffff;
    fontcolor = 0xffffffff;
    return true;
}

bool GCForgeManager::CreateItem( RecipeSimpleInfo &info ,int iFactor )
{
    if (info.iPrice > g_kGlobalValue.GetCurrentCharacterGP())
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CREATION_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , false, true );
        return false;
    }
    else
    {
        std::vector< GCITEMUID > vecMaterialUID;
        m_Result_ManufactureS3_Material_Get = INT_MAX;
        KP2P::GetInstance()->Send_ItemCreationReq( info.usIndex, info.ItemID, iFactor, vecMaterialUID );
        g_MyD3D->WaitForServerAck( m_Result_ManufactureS3_Material_Get, INT_MAX, 3000, TIME_OUT_VALUE );
    }

    return true;
}

bool GCForgeManager::CreateItem( RecipeSimpleInfo &info ,int iFactor, const IN vector<GCITEMUID>& vecMaterialList_ )
{
    if (info.iPrice > g_kGlobalValue.GetCurrentCharacterGP())
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CREATION_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , false, true );
        return false;
    }
    else if( vecMaterialList_.empty() ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CREATION_ERROR3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , false, true );
        return false;
    }

    //  중복된 아이를 찾는다.
    std::vector<GCITEMUID>::const_iterator iter = vecMaterialList_.begin();
    for ( ; iter != vecMaterialList_.end(); ++iter )
    {
        GCITEMUID _itemUID = *iter;
        int num = std::count( vecMaterialList_.begin(), vecMaterialList_.end(), _itemUID );

        if ( num > 1 ) 
        {   
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CREATION_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 , false, true );            
            return false;
        }
    }

    m_Result_ManufactureS3_Material_Get = INT_MAX;
    KP2P::GetInstance()->Send_ItemCreationReq( info.usIndex, info.ItemID, iFactor, vecMaterialList_ );
    g_MyD3D->WaitForServerAck( m_Result_ManufactureS3_Material_Get, INT_MAX, 3000, TIME_OUT_VALUE );

    return true;
}

void GCForgeManager::SetResultItem( std::vector< KItem >& vecItem )
{
    m_kResultItemList.clear();
    m_kResultItemList = vecItem;
}

bool GCForgeManager::AddWaitItem( GCITEMUID guItemUID, GCITEMID giItemID )
{
    if(m_setWaitItemList.size() >= 8)
        return false;

    bool m_bBypassForgeVerify = false;
    KItem* tempItemForge = g_pItemMgr->m_kInventory.FindItemByItemUID( guItemUID );

    if ( tempItemForge->m_cEnchantLevel > 0 )
    {
        m_bBypassForgeVerify = true;
    }

    if ( !tempItemForge->m_vecSocket.empty() )
    {
        for ( auto& socket : tempItemForge->m_vecSocket )
        {
            if ( socket.m_cState == KSocketInfo::STS_USING )
            {
                m_bBypassForgeVerify = true;
            }
        }
    }

    if ( m_bBypassForgeVerify )
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_FORGE_CONFIRM_CONFIRM), L"", KGCUIScene::GC_MBOX_USE_BREAKUP_CONFIRM, guItemUID, giItemID, false, true);
    }
    else
    {
        m_setWaitItemList.insert(guItemUID);
        m_vecWaitItemList.push_back(std::pair<GCITEMUID, GCITEMID>(guItemUID, giItemID));
    }
    return true;
}

void GCForgeManager::EraseWaitItem( GCITEMUID guItemUID )
{
    m_setWaitItemList.erase(guItemUID);
    std::vector<std::pair<GCITEMUID, GCITEMID>>::iterator vit = m_vecWaitItemList.begin();
    for(;vit != m_vecWaitItemList.end();vit++){
        if(vit->first == guItemUID){
            m_vecWaitItemList.erase(vit);
            break;
        }
    }
}

void GCForgeManager::InitWaitItem( )
{
    m_setWaitItemList.clear();
    m_vecWaitItemList.clear();
}

void GCForgeManager::SetEnableBreakUp(bool bEnable)
{
	m_bEnableBreakUp = bEnable;
}

bool GCForgeManager::BreakUpItem( GCITEMUID guItemUID )
{
    m_Result_ManufactureS3_Material_Get = INT_MAX;
    KP2P::GetInstance()->Send_ItemBreakupReq( guItemUID );
    g_MyD3D->WaitForServerAck( m_Result_ManufactureS3_Material_Get, INT_MAX, 3000, TIME_OUT_VALUE );
    EraseWaitItem(guItemUID);
    return true;
}

RecipeSimpleInfo GCForgeManager::GetCraftRecipeInfo( DWORD dwItemID )
{
    RecipeSimpleInfo stRecipeSimpleInfo;
    for( std::vector<KManufactureItem>::iterator vitRecipe = m_vecRecipeAllList.begin(); vitRecipe != m_vecRecipeAllList.end(); ++vitRecipe )
    {
        if( dwItemID != (*vitRecipe).m_ItemID )
            continue;

        GCItem* pItem = g_pItemMgr->GetItemData( (*vitRecipe).m_ItemID );

        if( NULL == pItem )
            continue;

        if( (*vitRecipe).m_nLevel != -1 )
        { 
            stRecipeSimpleInfo.iItemLevel = (*vitRecipe).m_nLevel;
        }
        else
        {
            stRecipeSimpleInfo.iItemLevel = pItem->iNeedLevelForEquippingItem;
        }

        stRecipeSimpleInfo.iPrice = (*vitRecipe).m_nPrice;
        stRecipeSimpleInfo.strItemName = pItem->strItemName;
        stRecipeSimpleInfo.ItemID = pItem->dwGoodsID;
        stRecipeSimpleInfo.usIndex = (*vitRecipe).m_Index;
        stRecipeSimpleInfo.cGrade = static_cast<char>((*vitRecipe).m_nGrade);
        stRecipeSimpleInfo.cFactor = ( (*vitRecipe).m_nDuration != -1 ) ? (*vitRecipe).m_nDuration : 1;
        break;
    }

    return stRecipeSimpleInfo;
}

void GCForgeManager::SetItemBreakUpInfo( KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kRecv_ )
{
    m_fBreakUpMaterialRatioMax = kRecv_.m_fMaterialRatioMax;
    m_fBreakUpMaterialRatioMin = kRecv_.m_fMaterialRatioMin;

    m_mapBreakUpMaterialItemID.swap( kRecv_.m_mapMaterialItemID );
    m_mapBreakUpMaterialInfo.swap( kRecv_.m_mapMaterialInfo );
    m_mapBreakUpSpecificBreakupInfo.swap( kRecv_.m_mapSpecificBreakupInfo );
}

void GCForgeManager::Send_GetItemBreakupInfo()
{
    if(!m_mapBreakUpMaterialItemID.empty()) return;

    KP2P::GetInstance()->Send_GetItemBreakupInfoReq();
}

bool GCForgeManager::IsSpecificBreakupItem( IN PAIR_INT& paitItemInfo )
{
    std::map< PAIR_INT, std::vector< KDropItemInfo > >::iterator it = m_mapBreakUpSpecificBreakupInfo.find( paitItemInfo );

    if( it != m_mapBreakUpSpecificBreakupInfo.end() )
        return true;
    else
        return false;
}

void GCForgeManager::GetSpecificBreakupItem( IN PAIR_INT& pairItemInfo, OUT std::vector< KDropItemInfo >& vecItemInfo )
{
    std::map< PAIR_INT, std::vector< KDropItemInfo > >::iterator it = m_mapBreakUpSpecificBreakupInfo.find( pairItemInfo );

    if( it != m_mapBreakUpSpecificBreakupInfo.end() )
	{
        vecItemInfo = (*it).second;
    }
}

void GCForgeManager::GetBreakupItem( IN PAIR_INT& pairItemInfo, OUT std::map< GCITEMID, PAIR_INT >& mapResultItemInfo )
{
    std::map< PAIR_INT, std::map< int, int > >::iterator iterBreakupInfo = m_mapBreakUpMaterialInfo.find( pairItemInfo );

    if( iterBreakupInfo != m_mapBreakUpMaterialInfo.end() ){
        // 인덱스랑 수량으로 된 맵에서 갯수를 판별
        for( std::map< int, int >::iterator it = (*iterBreakupInfo).second.begin(); it != (*iterBreakupInfo).second.end(); ++it ) {

            // 인덱스 번호로 아이템아이디 찾기
            std::map< int, GCITEMID >::iterator iterID = m_mapBreakUpMaterialItemID.find( (*it).first );

            GCITEMID dwItemID = 0;
            if( iterID != m_mapBreakUpMaterialItemID.end() ){
                dwItemID = (*iterID).second;
            }

            // 수량계산
            int nCount = (*it).second;
            int nCountMin = (int)(ceilf( nCount * m_fBreakUpMaterialRatioMin ) );
            int nCountMax = (int)(ceilf( nCount * m_fBreakUpMaterialRatioMax ) );

            // 추가
            mapResultItemInfo.insert( std::make_pair( dwItemID, std::make_pair( nCountMin, nCountMax ) ) );
        }
    }
}

void GCForgeManager::ClearRecipeAndBreakInfo()
{
    m_vecRecipeAllList.clear();

    m_mapBreakUpMaterialItemID.clear();
    m_mapBreakUpMaterialInfo.clear();
    m_mapBreakUpSpecificBreakupInfo.clear();

}
bool GCForgeManager::MakeItemOnFirstSlot( IN DWORD dwItemID )
{
    std::vector<GCITEMUID>::iterator iter = m_vecAllBreakUpItemList.begin();

    for( ; iter != m_vecAllBreakUpItemList.end(); ++iter ){
        KItem* pkItem = g_pItemMgr->GetInventoryItem( (*iter) );

        if( NULL == pkItem ) { 
            continue;
        }

        if( pkItem->m_ItemID == dwItemID ){
            std::iter_swap( iter, m_vecAllBreakUpItemList.begin() );
            return true;
        }
    }

    return false;
}

bool GCForgeManager::MakeRecipeOnFirstBar( IN DWORD dwItemID )
{
    std::vector< RecipeSimpleInfo >::iterator itSRL = m_vecShowRecipeList.begin();

    for ( ; itSRL != m_vecShowRecipeList.end(); ++itSRL ) {
        if ( (*itSRL).ItemID == dwItemID ){
            std::iter_swap( itSRL, m_vecShowRecipeList.begin() );
            g_pkUIScene->m_pkForgeDlg->UpdateRecipeUI();
            return true;
        }
    }

    return false;
}