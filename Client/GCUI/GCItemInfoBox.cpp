#include "stdafx.h"
#include "GCItemInfoBox.h"
//
#include "../MyD3D.h"

#include "EnchantStrong.h"
#include "KSingleton.h"

#include "KGCEmoticon.h"
IMPLEMENT_CLASSNAME( KGCItemInfoBox );
IMPLEMENT_WND_FACTORY( KGCItemInfoBox );
IMPLEMENT_WND_FACTORY_NAME( KGCItemInfoBox, "gc_item_info_box" );

KGCItemInfoBox::KGCItemInfoBox( void )
{
	m_pkItemName = NULL;
	m_pkItemAbllity = NULL;
	m_pkItemDecs = NULL;

	m_pTextureItem = NULL;

	LINK_CONTROL( "item_info_name",		m_pkItemName );
	LINK_CONTROL( "item_info_abillity",	m_pkItemAbllity );
	LINK_CONTROL( "item_info_decs",		m_pkItemDecs );
	
	m_iItemID = -1;
	m_dwItemUID = 0;
	m_iSetItemNumber = -1;

    m_bUpdate = false;
    m_cPromotion = -1;

    m_pkItem = NULL;


}

KGCItemInfoBox::~KGCItemInfoBox( void )
{
	SAFE_RELEASE( m_pTextureItem );
}

void KGCItemInfoBox::OnCreate( void )
{
	m_pkItemName->SetSelfInputCheck( false );
	m_pkItemAbllity->SetSelfInputCheck( false );
	m_pkItemDecs->SetSelfInputCheck( false );

	m_pkItemAbllity->SetMultiLine( true );
	m_pkItemDecs->SetMultiLine( true );

	//태국어는 줄간격1.5배
#if defined(_THAI_LANG)
	m_pkItemAbllity->SetLineSpace( 1.5f );
#else
	m_pkItemAbllity->SetLineSpace( 1.8f );
#endif
	m_pkItemDecs->SetWidth( 160 );
	m_pkItemDecs->SetLineSpace( 1.2f );
	m_pkItemName->SetAlign( DT_CENTER );

}

void KGCItemInfoBox::PostChildDraw()
{
	// 샵 아이템 이미지를 그린다.
	if ( IsRenderOn() && m_iItemID > -1 )
	{
		D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_iItemID );
        if ( NULL != kTmpEmoticon && !g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( m_iItemID ) ) {
            // 이모티콘 데이터라면!!!!
            kTmpEmoticon->RenderForShop( vPos.x + 28.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, vPos.y + 34.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY, vPos.x + (28.0f + 76.0f)* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, vPos.y + (34.0f + 76.0f)* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY );
        } else {
	        g_pItemMgr->Draw_ShopImage( vPos + D3DXVECTOR2( 28.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, 34.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY), 76.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, 76.0f* GC_SCREEN_DIV_WIDTH * m_fWindowScaleY, m_pTextureItem, m_iSetItemNumber != -1, m_iSetItemNumber,-1,m_iItemID );
        }
	}

//#ifndef __PATH__
//
//    if( GetAsyncKeyState( VK_SHIFT ) )
//    {
//        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
//        g_pkUIMgr->RenderNumber( "white_num", vPos, m_iItemID * 10 );
//    }
//
//#endif
}

std::wstring KGCItemInfoBox::MakeDiffString( int iDiff )
{
    std::wstringstream str;
    if( iDiff == 0 )
    {
        str << L"(-)";
        return str.str();
    }
    std::wstring strColorKey;
    std::wstring strColorKeyEnd = L"#cX";
    if( iDiff < 0 )
    {
        strColorKey = L"#cff0000↓";
    }
    else if( iDiff > 0 )
    {
        strColorKey = L"#c00ff00↑";
    }
    str << L"(" << strColorKey << abs(iDiff) << strColorKeyEnd << L")";
    return str.str();
}

std::wstring KGCItemInfoBox::MakeLvString( int iLv )
{
    std::wstringstream stm;
    SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

    std::wstring strColorKey;
    std::wstring strColorKeyEnd = L"#cX";
    if( pCharInfo->iLevel >= iLv )
    {
        strColorKey = L"#cffffff";
    }
    else
    {
        strColorKey = L"#cff0000";
    }
    stm << strColorKey << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << L" " << iLv << strColorKeyEnd;
    return stm.str();
}

void KGCItemInfoBox::GetItemText( GCItem*& pkItem, KItemText& ItemText, KItem* pkInventory/* = NULL*/ )
{
    bool bDiff = false;
    GCItem* pItem = NULL;
    if( g_pItemMgr->GetMyCharEquipByItemPosition( pkItem->dwSlotPosition, &pItem ) == true && !(pkItem->dwSlotPosition & ESP_SET_ITEM) )
        bDiff = true;
    GCItemAbility kItemAbility = g_pItemMgr->GetItemAbility( pkItem );
    GCItemAbility kItemAbilityDiff;
	GCItemAbility kEnchantItemAbility;
	memset( &kEnchantItemAbility, 0, sizeof( GCItemAbility ) );
	KItem* pkInvenItem = NULL;

	if ( pkItem->dwSlotPosition & ESP_WEAPON || pkItem->dwSlotPosition & ESP_HELMET || pkItem->dwSlotPosition & ESP_JACKET )
	{
        // 장착중인 녀석에게만 보여준다
        if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), pkItem->dwGoodsID ) )
        {
		    DWORD dwSocketSlot = 0;
		    if ( pkItem->dwSlotPosition & ESP_WEAPON )
			    dwSocketSlot = ESP_WEAPON_SOCKET;
		    if ( pkItem->dwSlotPosition & ESP_HELMET )
			    dwSocketSlot = ESP_HELMET_SOCKET;
		    if ( pkItem->dwSlotPosition & ESP_JACKET )
			    dwSocketSlot = ESP_JACKET_SOCKET;
            if ( pkItem->dwSlotPosition & ESP_A_UPPER_HELMET )
                dwSocketSlot = ESP_A_HELMET_SOCKET;
		    if ( g_pItemMgr->GetMyCharEquipByItemPosition( dwSocketSlot, &pkInvenItem ) == true )
		    {
			    kEnchantItemAbility = KSingleton<KEnchantStrong>::GetInstance()->GetAbility( pkInvenItem );
		    }
        }
	}
    else if ( pkItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
    {
        if( pkInventory && pkInventory->m_EnchantEquipItemUID != 0 )
        {
            kItemAbility = g_pItemMgr->GetEnchantItemAbility(pkItem, pkInventory);
        }
        else
        {
        GetEnchantAbility( kItemAbility, pkItem->dwGoodsID, pkInventory );
    }
    }

    if( bDiff )
    {
        GCItemAbility kEquipItemAbility;

        if( pkItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
        {
            if( pkInventory && pkInventory->m_EnchantEquipItemUID != 0 )
            {
                kEquipItemAbility = g_pItemMgr->GetEnchantItemAbility(pkItem, pkInventory);
            }
            else
            {
            GetEnchantAbility( kEquipItemAbility, pkItem->dwGoodsID, pkInventory );
        }
        }
        else
        {
            kEquipItemAbility = g_pItemMgr->GetItemAbility( pItem );
        }

        kItemAbilityDiff.iAtk = kItemAbility.iAtk - kEquipItemAbility.iAtk;
        kItemAbilityDiff.iDef = kItemAbility.iDef - kEquipItemAbility.iDef;
        kItemAbilityDiff.iVit = kItemAbility.iVit - kEquipItemAbility.iVit;
    }
    else
    {
        if( pkItem->dwSlotPosition & ESP_SET_ITEM )
        {
            kItemAbilityDiff.iAtk = 0;
            kItemAbilityDiff.iDef = 0;
            kItemAbilityDiff.iVit = 0;
        }
        else
        {
            kItemAbilityDiff.iAtk = kItemAbility.iAtk;
            kItemAbilityDiff.iDef = kItemAbility.iDef;
            kItemAbilityDiff.iVit = kItemAbility.iVit;
        }
    }
   
    ItemText.strmAbillity << MakeLvString( static_cast<int>(g_pItemMgr->GetlItemEquipLevel(pkItem, pkInventory)) ) << std::endl
		<< g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L" " << kItemAbility.iAtk << MakeSocketItemString( kEnchantItemAbility.iAtk ) << MakeDiffString(kItemAbilityDiff.iAtk) << std::endl
        << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L" " << kItemAbility.iDef << MakeSocketItemString( kEnchantItemAbility.iDef ) << MakeDiffString(kItemAbilityDiff.iDef) <<  std::endl
        << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L" " << kItemAbility.iVit << MakeSocketItemString( kEnchantItemAbility.iVit ) << MakeDiffString(kItemAbilityDiff.iVit) <<  std::endl;

    ItemText.strmName << pkItem->strItemName;
    ItemText.strmDesc << pkItem->strItemDesc;
}

std::wstring KGCItemInfoBox::MakeSocketItemString( int iAbility )
{
	if ( iAbility == 0 )
		return L"";
	std::wstringstream strStream;
	strStream << L"+" << iAbility;
	return strStream.str();
}

void KGCItemInfoBox::SetItemForPet( GCItem* pkItem, char cPromotion )
{
    if( pkItem->eItemKind != GIK_PET )
        return;

    if( pkItem == NULL )
        return;

    if( m_iItemID == pkItem->dwGoodsID && m_bUpdate )
    {
        // 펫이라면 매번 Setitem해주자=ㅅ= 진화한다든지 할 수 있으니..
        // 일반 아이템이면 그냥 GoodsID만 비교해서 같으면 또 아래 과정을 거치지 않아도 된다
        if( ( pkItem->eItemKind != GIK_PET ||
            pkItem->ePetItemKind != EPET_KIND_PET ) &&
            pkItem->eItemKind != GIK_ENCHANT_ENABLE_ITEM )
            return;
    }

    m_bUpdate = true;

    SAFE_RELEASE( m_pTextureItem );

    m_iItemID = pkItem->dwGoodsID;

    KItemText ItemText;
    GetItemText( pkItem, ItemText );

    m_pkItemName->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );

    m_pkItemName->SetText( ItemText.strmName.str() );
	m_pkItemAbllity->SetText( ItemText.strmAbillity.str() );



	int iPetID = g_kGlobalValue.GetPetIDFromItemID( m_iItemID );
	int iPetItemDecsID = -1; 
	if(iPetID != -1) 
		iPetItemDecsID= g_kGlobalValue.GetPetTemplate(iPetID , cPromotion ).iItemDescID;
	// 따로 지정된 설명이 없으면 원래 sort에 있는 설명을 보여준다
	if( iPetItemDecsID == -1 )
	{
		m_pkItemName->SetText( ItemText.strmName.str() );
		m_pkItemDecs->SetTextAutoMultiline( ItemText.strmDesc.str() );
	}
	else
	{
		m_pkItemName->SetText( g_pkStrLoader->GetString( g_kGlobalValue.GetPetTemplate( iPetID, cPromotion ).iNameID ) );
		m_pkItemDecs->SetTextAutoMultiline( g_pkStrLoader->GetString( iPetItemDecsID ) );
	}

    m_pTextureItem = g_pItemMgr->CreatePetItemTexture( m_iItemID, cPromotion );

    m_cPromotion = cPromotion;

}

void KGCItemInfoBox::SetItem( GCItem* pkItem )
{
    if( pkItem == NULL )
        return;

    m_pkItem = pkItem;


    if( m_iItemID == pkItem->dwGoodsID && m_bUpdate )
    {
        // 펫이라면 매번 Setitem해주자=ㅅ= 진화한다든지 할 수 있으니..
        // 일반 아이템이면 그냥 GoodsID만 비교해서 같으면 또 아래 과정을 거치지 않아도 된다
        if( ( pkItem->eItemKind != GIK_PET ||
            pkItem->ePetItemKind != EPET_KIND_PET ) &&
            pkItem->eItemKind != GIK_ENCHANT_ENABLE_ITEM &&
			pkItem->iLimitedLevelOfLevelRatioAbility == -1 )
            return;
    }

    m_bUpdate = true;

    SAFE_RELEASE( m_pTextureItem );

    m_iItemID = pkItem->dwGoodsID;

    if( pkItem->dwSlotPosition & ESP_SET_ITEM )
    {
        m_iSetItemNumber = pkItem->iSetItemNumber;
    }
    else
    {
        m_iSetItemNumber = -1;
    }

    KItemText ItemText;
    GetItemText( pkItem, ItemText );

    m_pkItemName->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );

    m_pkItemName->SetText( ItemText.strmName.str() );
    m_pkItemAbllity->SetText( ItemText.strmAbillity.str() );
    m_pkItemDecs->SetTextAutoMultiline( ItemText.strmDesc.str() );

    if( m_iSetItemNumber == - 1 )
    {
		m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );
    }
    else
    {
        m_pTextureItem = g_pItemMgr->CreateShopSetItemTexture( m_iSetItemNumber,m_iItemID );
    }
}

void KGCItemInfoBox::SetItem( GCItem* pkItem, KItem* pkInventoryItem )
{
    if( pkItem == NULL )
        return;

    m_pkItem = pkItem;

    if( m_iItemID == pkItem->dwGoodsID && m_bUpdate )
    {
        // 펫이라면 매번 Setitem해주자=ㅅ= 진화한다든지 할 수 있으니..
        // 일반 아이템이면 그냥 GoodsID만 비교해서 같으면 또 아래 과정을 거치지 않아도 된다
        if( ( pkItem->eItemKind != GIK_PET ||
            pkItem->ePetItemKind != EPET_KIND_PET ) &&
            pkItem->eItemKind != GIK_ENCHANT_ENABLE_ITEM &&
            g_pItemMgr->GetlItemAbilityLimitLevel(pkItem, pkInventoryItem) == -1 )
            return;
    }

    m_bUpdate = true;

    SAFE_RELEASE( m_pTextureItem );

    m_iItemID = pkItem->dwGoodsID;

    if( pkInventoryItem != NULL )
        m_dwItemUID = pkInventoryItem->m_ItemUID;

    if( pkItem->dwSlotPosition & ESP_SET_ITEM )
        m_iSetItemNumber = pkItem->iSetItemNumber;
    else
        m_iSetItemNumber = -1;

    KItemText ItemText;
    bool bIsPet = false;
    int iPetEvolutionLevel = 0;

    // 펫이고 DB에서 받은 펫의 정보에 펫 정보가 있다면
    if( pkInventoryItem != NULL &&
        pkItem->eItemKind == GIK_PET &&
        pkItem->ePetItemKind == EPET_KIND_PET &&
        g_MyD3D->IsExistPetInfo( pkInventoryItem->m_ItemUID ) )
    {
        KPetInfo& kPetinfo = g_MyD3D->m_mapPetInfo[ pkInventoryItem->m_ItemUID ];

        bIsPet = true;
        iPetEvolutionLevel = kPetinfo.m_cPromotion;

        // 펫일 경우에 펫 이름 표시 & 펫 레벨 표시
        int iLevel = GetPetLevel( kPetinfo.m_dwEXP );
        ItemText.strmName << g_pkStrLoader->GetString( STR_ID_LEVEL ) << " " << iLevel << " " << kPetinfo.m_strName;

        // 레벨, 진화여부에 따라서 능력치 세팅
        int iEvolutionConstant = 1;
        if( iPetEvolutionLevel == 1 )
        {
            iEvolutionConstant = 2;
        }
        else if( iPetEvolutionLevel == 2 )
        {
            iEvolutionConstant = 4;
        }

        int iAttack = (int)( ( 0.2f + iLevel * 0.002f * iEvolutionConstant + 0.0005f ) * 100 );
        int iDeffece = (int)( ( 0.2f + iLevel * 0.001f * iEvolutionConstant + 0.0005f ) * 100 );
        int iHealth = (int)( ( 0.2f + iLevel * 0.001f * iEvolutionConstant + 0.0005f ) * 100 );

        ItemText.strmAbillity << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << L" " << static_cast<int>( g_pItemMgr->GetlItemEquipLevel(pkItem, pkInventoryItem) ) << std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L" " << iAttack << std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L" " << iDeffece << std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L" " << iHealth << std::endl;

        int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPetinfo.m_dwID );
        if( iPetID != -1 )
        {
            int iPetItemDecsID = g_kGlobalValue.GetPetTemplate(iPetID , iPetEvolutionLevel ).iItemDescID;
            // 따로 지정된 설명이 없으면 원래 sort에 있는 설명을 보여준다
            if( iPetItemDecsID == -1 )
            {
                ItemText.strmDesc << pkItem->strItemDesc;
            }
            else
            {
                ItemText.strmDesc << g_pkStrLoader->GetString( iPetItemDecsID );
            }
        }
        else
        {
            ItemText.strmDesc << pkItem->strItemDesc;
        }
    }
    else if( NULL != pkInventoryItem && pkItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
    {
        // [2/14/2008] breadceo. "+ 얼마" 까지만 하면 된다
        ItemText.strmName << L"+" << (int)pkInventoryItem->m_cEnchantLevel << " "/* << pkItem->strItemName*/;
        GetItemText( pkItem, ItemText, pkInventoryItem );
    }
    else
    {
        GetItemText( pkItem, ItemText );
    }

    m_pkItemName->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );
    m_pkItemName->SetText( ItemText.strmName.str() );
    m_pkItemAbllity->SetText( ItemText.strmAbillity.str() );
    m_pkItemDecs->SetTextAutoMultiline( ItemText.strmDesc.str() );

    // 펫이면 진화레벨을 고려해서 텍스쳐를 로딩한다
    if( bIsPet )
    {
        m_pTextureItem = g_pItemMgr->CreatePetItemTexture( m_iItemID, iPetEvolutionLevel );
    }
    else if( m_iSetItemNumber == - 1 )
    {
        m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );
    }
    else
    {
        m_pTextureItem = g_pItemMgr->CreateShopSetItemTexture( m_iSetItemNumber,m_iItemID );
    }

    m_cPromotion = (char)iPetEvolutionLevel;

}

D3DXVECTOR4 KGCItemInfoBox::GetTextureCoords( int iSetNum )
{
	D3DXVECTOR4 m_vTexCoords;

	if ( 0 == iSetNum % 2 )
	{
		m_vTexCoords.x = 0.0f;
		m_vTexCoords.y = 0.0f;
		m_vTexCoords.z = 0.5f;
		m_vTexCoords.w = 1.0f;
	}
	else
	{
		m_vTexCoords.x = 0.5f;
		m_vTexCoords.y = 0.0f;
		m_vTexCoords.z = 1.0f;
		m_vTexCoords.w = 1.0f;
	}

	return m_vTexCoords;
}

void KGCItemInfoBox::GetEnchantAbility( OUT GCItemAbility& kAbility, DWORD dwID, KItem* pkInventory )
{
    // 인벤토리에 없는것들 ( ex : 미션 보상 )
    if( pkInventory == NULL )
    {
        KItem kInvenItem;
        kInvenItem.m_cEnchantLevel = 0; // 미션 보상에 + 몇짜리 주려면 이부분 수정해야 한다
        kInvenItem.m_ItemUID = ULONG_MAX; // 내부적으로 안쓰기 때문에 최대값으로 사용
        kInvenItem.m_ItemID = dwID;
        kAbility = KSingleton<KEnchantStrong>::GetInstance()->GetAbility( &kInvenItem );
    }
    else
    {
        kAbility = KSingleton<KEnchantStrong>::GetInstance()->GetAbility( pkInventory );
    }
}

void KGCItemInfoBox::SetTextColor( D3DCOLOR color )
{
	m_pkItemName->SetFontColor( color );
	m_pkItemAbllity->SetFontColor( color );
	m_pkItemDecs->SetFontColor( color );
}
