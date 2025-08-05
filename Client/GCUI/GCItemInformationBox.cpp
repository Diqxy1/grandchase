#include "stdafx.h"
#include "GCItemInformationBox.h"
//
#include "../MyD3D.h"

#include "EnchantStrong.h"
#include "KSingleton.h"

#include "KGCEmoticon.h"
#include "KGCMultipleLanguages.h"

IMPLEMENT_CLASSNAME( KGCItemInformationBox );
IMPLEMENT_WND_FACTORY( KGCItemInformationBox );
IMPLEMENT_WND_FACTORY_NAME( KGCItemInformationBox, "gc_item_information_box" );

KGCItemInformationBox::KGCItemInformationBox( void )
: m_pkBackground( NULL )
, m_pkCenterBackground( NULL )
, m_pkName( NULL )
, m_pkDefault1( NULL )
, m_pkDefault2( NULL )
, m_pkSocket( NULL )
, m_pkAttribute( NULL )
, m_pkSetItem( NULL )
, m_pkDesc( NULL )
, m_pkExtraDesc( NULL )
, m_pkItem( NULL )
, m_eItemInformationBoxState( EIIBS_ALONE )
, m_cGrade( -1 )
, m_ItemID( UINT_MAX )
, m_ItemUID( 0 )
, m_iChar( 0 )
, m_iAlpha( 255 )
, m_bShowExtraInfo( false )
, m_bEquippedItem( false )
{
    for( int i = 0 ; i < EIIG_MAX; ++i )
    {
        char szTemp[ MAX_PATH ] = "";
        sprintf_s( szTemp, MAX_PATH, "item_information_group%d", i );
        LINK_CONTROL( szTemp, m_stItemInfoGroup[i].m_pkItemInfoGroup );
    }

    for( int i = 0; i < EICI_MAX; ++i)
    {
        char szTemp[ MAX_PATH ] = "";
        sprintf_s( szTemp, MAX_PATH, "compare_icon%d", i);
        LINK_CONTROL( szTemp, m_pkCompareIcon[i] );
    }

    LINK_CONTROL( "item_information_box_center_background", m_pkCenterBackground );
    LINK_CONTROL( "item_information_box_background", m_pkBackground );
    m_vecExtraInfoGroup.push_back( EIIG_SET );

    m_strEquipState[EIIBS_ALONE] = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE1 );
    m_strEquipState[EIIBS_EQUIPPED] = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE2 );
    m_strEquipState[EIIBS_POINTED] = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE3 );

    m_strHideExtraInfo = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC1 );
    m_strShowExtraInfo = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC2 );
    m_strExtraDescString = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC3 );
}

KGCItemInformationBox::~KGCItemInformationBox( void )
{
}

void KGCItemInformationBox::OnCreate( void )
{
    m_pkName = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_NAME].m_pkItemInfoGroup->GetChildByName("item_information_name") );
    m_pkDefault1 = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DEFAULT].m_pkItemInfoGroup->GetChildByName("item_information_position_ability") );
    m_pkDefault2 = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DEFAULT].m_pkItemInfoGroup->GetChildByName("item_information_kind_equiplimit_price") );
    m_pkSocket = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_SOCKET].m_pkItemInfoGroup->GetChildByName("item_information_socket") );
    m_pkAttribute = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_ATTRIBUTE].m_pkItemInfoGroup->GetChildByName("item_information_attribute") );
    m_pkSetItem = static_cast<KD3DListBox*>( m_stItemInfoGroup[EIIG_SET].m_pkItemInfoGroup->GetChildByName("item_information_setitem") );
    m_pkDesc = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DESC].m_pkItemInfoGroup->GetChildByName("item_information_desc") );
    m_pkExtraDesc = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_EXTRADESC].m_pkItemInfoGroup->GetChildByName("item_information_extra_desc") );

    //DEFAULT_FONT_SIZE
    m_pkName->SetFontOutline( true );
    m_pkName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkName->SetLineSpace( 1.2f );
    m_pkName->SetAlign( DT_CENTER );
    m_pkName->SetAutoScroll(true);

    m_pkDefault1->SetLineSpace(1.3f);
    m_pkDefault2->SetLineSpace(1.3f);
    m_pkDefault2->SetAlign( DT_RIGHT );

    m_pkSocket->SetLineSpace( 1.3f);
    m_pkAttribute->SetLineSpace(1.3f);

#if defined(NATION_IDN)
    m_pkSetItem->SetLineSpace( 1.2f );
#else
    m_pkSetItem->SetLineSpace( 1.3f );
#endif

    m_pkDesc->SetLineSpace(1.3f);
    m_pkDesc->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));
    m_pkExtraDesc->SetLineSpace(1.3f);
    m_pkExtraDesc->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255) );
    //m_pkExtraDesc->SetAlign( DT_CENTER );

    for(int i = 0 ; i < EIIG_MAX; ++i)
    {
        m_stItemInfoGroup[i].m_dwExtraSize = 2;
        m_stItemInfoGroup[i].m_dwExtraPlus = 3;
        m_stItemInfoGroup[i].m_pkBackGround = static_cast<KD3DSizingBox*>( m_stItemInfoGroup[i].m_pkItemInfoGroup->GetChildByName("item_information_background") );
    }

    m_stItemInfoGroup[EIIG_NAME].m_dwExtraPlus = 9;

    //SetAlignType( WNDUIAT_POSZEROBASE );
    //SetAllWindowColor( D3DCOLOR_ARGB(160, 255, 255, 255) );
}

void KGCItemInformationBox::OncreateComplete( void )
{
}

void KGCItemInformationBox::FrameMoveInEnabledState( void )
{
}

void KGCItemInformationBox::PostChildDraw()
{
#ifndef __PATH__
    if( GetAsyncKeyState( VK_SHIFT ) && GetAsyncKeyState( VK_CONTROL ) )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        g_pkUIMgr->RenderNumber( "white_num", vPos, g_pItemMgr->GetSameImageIndex(m_ItemID) *10);

        vPos.y += 80;
        g_pkUIMgr->RenderNumber( "white_num", vPos, m_ItemUID );
    }
    else if( GetAsyncKeyState( VK_SHIFT ) )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        g_pkUIMgr->RenderNumber( "white_num", vPos, m_ItemID * 10 );

        vPos.y += 80;
        g_pkUIMgr->RenderNumber( "white_num", vPos, m_ItemUID );
        g_pItemMgr->GetSameImageIndex(m_ItemID);
    }

#endif
}

std::wstring KGCItemInformationBox::MakeDiffString( int iDiff )
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

std::wstring KGCItemInformationBox::MakeSocketItemString( int iAbility )
{
    if ( iAbility == 0 )
        return L"";

    std::wstringstream strStream;
    strStream << L"+" << iAbility;
    return strStream.str();
}

std::wstring KGCItemInformationBox::MakeAbilityString( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ )
{
   std::wostringstream strmAbility;

    EItemAbilityData stItemAbilityData = g_pItemMgr->GetItemAbilityData( pItem_, g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iLevel, pInven_, pUserItem_, pPetInfo_, iLevel_ );
    GCItemAbility kItemAbility;

    kItemAbility.iAtk = static_cast<int>(stItemAbilityData.fAtk);
    kItemAbility.iDef = static_cast<int>(stItemAbilityData.fDef);
    kItemAbility.iVit = static_cast<int>(stItemAbilityData.fHP);

    int iItemAttributeAtk = static_cast<int>( stItemAbilityData.fItemAttribute[ ATTRTYPE_ATK_POINT ] );
    int iItemAttributeDef = static_cast<int>( stItemAbilityData.fItemAttribute[ ATTRTYPE_DEF_POINT ] );
    int iItemAttributeHP  = static_cast<int>( stItemAbilityData.fItemAttribute[ ATTRTYPE_HP_POINT ] );

    GCItemAbility kEnchantItemAbility;
    GCItemAbility kItemAbilityDiff;
    
    m_bEquippedItem = false;
    if( ( pInven_ && g_pItemMgr->IsEquippedSameItem( &g_MyD3D->m_TempPlayer, pItem_, pInven_->m_ItemUID) ) ||
        ( pUserItem_ && g_pItemMgr->IsEquippedSameItem( &g_MyD3D->m_TempPlayer, pItem_, pUserItem_->dwItemUID ) ) )
    {
        m_bEquippedItem = true;
        DWORD dwSocketSlot = 0;

        if ( pItem_->dwSlotPosition & ESP_WEAPON )
            dwSocketSlot |= ESP_WEAPON_SOCKET;

        if ( pItem_->dwSlotPosition & ESP_HELMET )
            dwSocketSlot |= ESP_HELMET_SOCKET;

        if ( pItem_->dwSlotPosition & ESP_JACKET )
            dwSocketSlot |= ESP_JACKET_SOCKET;

        if ( pItem_->dwSlotPosition & ESP_A_UPPER_HELMET )
            dwSocketSlot |= ESP_A_HELMET_SOCKET;

        if( dwSocketSlot)
        {
            KItem* pkInvenItem = NULL;
            if ( g_pItemMgr->GetMyCharEquipByItemPosition( dwSocketSlot, &pkInvenItem ) == true )
            {
                kEnchantItemAbility = KSingleton<KEnchantStrong>::GetInstance()->GetAbility( pkInvenItem );
            }
        }
    }
    else if( !(pItem_->dwSlotPosition & ESP_SET_ITEM ) )
    {
        EItemAbilityData stEquipItemAbilityData;
        SUserItem* pEquipUserItem = NULL;
        GCItem* pEquipItem = NULL;
        KPetInfo* pEquipPetInfo = NULL;

        if( g_pItemMgr->GetEquipItemOfSamePosition( &g_MyD3D->m_TempPlayer, pItem_, &pEquipItem, &pEquipUserItem, &pEquipPetInfo) )
                stEquipItemAbilityData= g_pItemMgr->GetItemAbilityData( pEquipItem, g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iLevel, NULL, pEquipUserItem, pEquipPetInfo );

#if defined( USE_ONGAME_INVENTORY )
        kItemAbilityDiff.iAtk = kItemAbility.iAtk - static_cast<int>(stEquipItemAbilityData.fAtk) - static_cast<int>( stEquipItemAbilityData.fItemAttribute[ ATTRTYPE_ATK_POINT ] );
        kItemAbilityDiff.iDef = kItemAbility.iDef - static_cast<int>(stEquipItemAbilityData.fDef) - static_cast<int>( stEquipItemAbilityData.fItemAttribute[ ATTRTYPE_DEF_POINT ] );
        kItemAbilityDiff.iVit = kItemAbility.iVit - static_cast<int>(stEquipItemAbilityData.fHP) - static_cast<int>( stEquipItemAbilityData.fItemAttribute[ ATTRTYPE_HP_POINT ] );
#else
        kItemAbilityDiff.iAtk = kItemAbility.iAtk - static_cast<int>(stEquipItemAbilityData.fAtk);
        kItemAbilityDiff.iDef = kItemAbility.iDef - static_cast<int>(stEquipItemAbilityData.fDef);
        kItemAbilityDiff.iVit = kItemAbility.iVit - static_cast<int>(stEquipItemAbilityData.fHP);
#endif

        if ( pInven_ && pInven_->CheckItemType(KItem::TYPE_LOOK) )
        {
            kItemAbilityDiff.iAtk = 0;
            kItemAbilityDiff.iDef = 0;
            kItemAbilityDiff.iVit = 0;
        }
    }

#if defined( USE_ONGAME_INVENTORY )
    if( false == m_bEquippedItem ) { 
        strmAbility << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L" " << kItemAbility.iAtk << MakeSocketItemString( kEnchantItemAbility.iAtk ) << MakeDiffString(kItemAbilityDiff.iAtk + iItemAttributeAtk) << std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L" " << kItemAbility.iDef << MakeSocketItemString( kEnchantItemAbility.iDef ) << MakeDiffString(kItemAbilityDiff.iDef + iItemAttributeDef) <<  std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L" " << kItemAbility.iVit << MakeSocketItemString( kEnchantItemAbility.iVit ) << MakeDiffString(kItemAbilityDiff.iVit + iItemAttributeHP);
    }
    else {
        strmAbility << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L" " << kItemAbility.iAtk << MakeSocketItemString( kEnchantItemAbility.iAtk ) << MakeDiffString(kItemAbilityDiff.iAtk) << std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L" " << kItemAbility.iDef << MakeSocketItemString( kEnchantItemAbility.iDef ) << MakeDiffString(kItemAbilityDiff.iDef) <<  std::endl
            << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L" " << kItemAbility.iVit << MakeSocketItemString( kEnchantItemAbility.iVit ) << MakeDiffString(kItemAbilityDiff.iVit);
    }
#else
    strmAbility << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L" " << kItemAbility.iAtk << MakeSocketItemString( kEnchantItemAbility.iAtk ) << MakeDiffString(kItemAbilityDiff.iAtk) << std::endl
        << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L" " << kItemAbility.iDef << MakeSocketItemString( kEnchantItemAbility.iDef ) << MakeDiffString(kItemAbilityDiff.iDef) <<  std::endl
        << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L" " << kItemAbility.iVit << MakeSocketItemString( kEnchantItemAbility.iVit ) << MakeDiffString(kItemAbilityDiff.iVit);
#endif

    return strmAbility.str();
}

void KGCItemInformationBox::SetUIPosition()
{
    int iAlpah;
    DWORD dwStarPosY = 10;
    DWORD dwCenterBackgroundStartY = 10;

    if( m_stItemInfoGroup[EIIG_NAME].m_pkItemInfoGroup->IsRenderOn() )
    {
        dwCenterBackgroundStartY += m_stItemInfoGroup[EIIG_NAME].GetHeight() + 4;
        m_pkCenterBackground->SetWindowPosDirect( D3DXVECTOR2( m_pkCenterBackground->GetFixedWindowLocalPos().x, static_cast<float>(dwCenterBackgroundStartY ) ) );
    }

    iAlpah = ((m_iAlpha-155) < 0) ? 0 : (m_iAlpha-155);
    for( int i = 0 ; i < EIIG_MAX; ++i )
    {
        if( m_stItemInfoGroup[i].m_pkItemInfoGroup->IsRenderOn() )
        {
            SetPositionGroup( (EITEMINFOGROUP)i ,D3DXVECTOR2( m_stItemInfoGroup[i].m_pkItemInfoGroup->GetFixedWindowLocalPos().x, static_cast< float >(dwStarPosY) ) );
            dwStarPosY += m_stItemInfoGroup[i].GetHeight() + m_stItemInfoGroup[i].m_dwExtraPlus;
            m_stItemInfoGroup[i].m_pkBackGround->SetAllWindowAlpha( iAlpah );
        }
    }

    dwStarPosY += 21;
    m_pkCenterBackground->SetHeight( dwStarPosY - dwCenterBackgroundStartY - 13);
    iAlpah = ((m_iAlpha-65) < 0) ? 0 : (m_iAlpha-65);
    m_pkCenterBackground->SetAllWindowAlpha( iAlpah );
    m_pkBackground->SetHeight( dwStarPosY);
    iAlpah = ((m_iAlpha-85) < 0) ? 0 : (m_iAlpha-85);
    m_pkBackground->SetAllWindowAlpha( iAlpah );
    SetHeightDirect(dwStarPosY );
}

void KGCItemInformationBox::ToggleRenderItemInfoGroup( EITEMINFOGROUP eItemInfoGroup_, bool bShow_ )
{
    if( ( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ ) ||
        ( bShow_ && false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->ToggleRender( bShow_ );
}

void KGCItemInformationBox::SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic_)
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ || ( false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    DWORD dwNewHeight = static_cast<DWORD>( psStanDardStatic_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight );
}
void KGCItemInformationBox::SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DListBox* psStanDardStatic_)
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ || ( false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    DWORD dwNewHeight = static_cast<DWORD>( psStanDardStatic_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight );
}
void KGCItemInformationBox::SetPositionGroup( EITEMINFOGROUP eItemInfoGroup_, D3DXVECTOR2 vNewPos_ )
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ )
        return;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetWindowPosDirect( vNewPos_ );
}

std::wstring KGCItemInformationBox::MakeEquipLimitString(  GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ )
{
    std::wstring strResult;
    std::wstringstream stm;
    SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

    std::wstring strLevelColorKey = L"#cffffff";
    std::wstring strChangeJobColorKey = L"#cffffff";
    std::wstring strColorKeyEnd = L"#cX";

    int iNeedForEquipLevel;
    if( iLevel_ != -1 )
        iNeedForEquipLevel = iLevel_;
    else
        iNeedForEquipLevel = g_pItemMgr->GetlItemEquipLevel(pItem_, pInven_);

    int nEquipLVDown = 0;
    if ( pInven_ && pInven_->m_sEquipLvDown > 0  ) 
    {
        nEquipLVDown =  pInven_->m_sEquipLvDown ;
    }


    EItemAbilityData eItemAbilityData = g_pItemMgr->GetItemAbilityData( pItem_, pCharInfo->iLevel, pInven_, pUserItem_ );
    if( pCharInfo->iLevel + static_cast<int>( eItemAbilityData.fItemAttribute[ ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN ] ) + nEquipLVDown < iNeedForEquipLevel )
        strLevelColorKey = L"#cff0000";

    stm << strLevelColorKey << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << L" " << iNeedForEquipLevel << strColorKeyEnd; 
        
    if ( nEquipLVDown > 0 )
    {
        stm << L" " << g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_DOWN_NUMBER, "i", nEquipLVDown ) << std::endl;
    }
    else
    {
        stm << std::endl;
    }


    
    int iNeedForEquipChangeJobLevel = pItem_->iNeedChangeJobLevelForEquippingItem;

    if( pCharInfo->iPromotionLevel < iNeedForEquipChangeJobLevel )
        strChangeJobColorKey = L"#cff0000";

    std::wstring strJobLevel = strChangeJobColorKey;

    switch ( iNeedForEquipChangeJobLevel )
    {
    case 0:
        {
            if( pItem_->dwSlotPosition & ESP_WEAPON )
            {
                strJobLevel += g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON1 ) + strColorKeyEnd;
                stm << strJobLevel;
            }
            break;
        }
    case 1:
        {
            strJobLevel += g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON2 ) + strColorKeyEnd;
            stm << strJobLevel;
            break;
        }
    case 2:
        {
            strJobLevel += g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON3 ) + strColorKeyEnd;
            stm << strJobLevel;
            break;
        }
    case 3:
        {
            strJobLevel += g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON4 ) + strColorKeyEnd;
            stm << strJobLevel;
            break;
        }

    }
    return stm.str();		
}

std::vector<std::wstring> KGCItemInformationBox::MakeSetItemListString( GCITEMID ItemID_ )
{
    std::vector< int > vecSetEffectItem;
    std::map< int, std::vector< int > > mapSetEffectItem;
    std::set<int> setSetItemName = g_pItemMgr->GetSetEffectItemName( ItemID_, &mapSetEffectItem );
    int iFindSetNumber = -1;
    int iCurrentCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
    for( std::set<int>::iterator sit = setSetItemName.begin(); sit != setSetItemName.end(); ++sit )
    {
        GCItem* pSetItem = g_pItemMgr->GetItemData( (*sit) );
        if( NULL == pSetItem || g_pItemMgr->GetUniqueChartype( pSetItem->dwCharType ) != iCurrentCharType )
            continue;

        iFindSetNumber = (*sit);

        std::map< int, std::vector< int > >::iterator mit = mapSetEffectItem.find( iFindSetNumber );

        if( mapSetEffectItem.end() != mit )
            vecSetEffectItem = mit->second;

        break;
    }

    std::vector< KGCItemManager::SetItemAttributeList > vecSetItemAttributeList;
    std::vector<std::wstring> strResult;

    if( -1 == iFindSetNumber || vecSetEffectItem.empty() || 
        false == g_pItemMgr->GetEquipSetItemAttributeBySetName( iFindSetNumber, vecSetItemAttributeList ) || vecSetItemAttributeList.empty() )
    {
        return strResult;
    }


    std::wstring strColorKeyEnd = L"#cX";
    int iPartsCount = vecSetEffectItem.size();
    int iEquipSetItemCount = 0;
    std::vector< int >::iterator vit = vecSetEffectItem.begin();
    for(; vit != vecSetEffectItem.end(); ++vit)
    {
        GCItem* pItem = g_pItemMgr->GetItemData( (*vit) );
        if( NULL == pItem)
            continue;

        std::wstring strPartsColorKey = L"#c909090";
        if( g_pItemMgr->CheckEquipItemForItemID( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), (*vit), true ) )
        {
            strPartsColorKey = L"#cffff00";
            ++iEquipSetItemCount;
        }
        std::wstring result;
        result += L"       " +strPartsColorKey + pItem->strItemName + strColorKeyEnd;
        strResult.push_back(result);
    }

    std::vector< KGCItemManager::SetItemAttributeList >::iterator vitAttributeList = vecSetItemAttributeList.begin();

    for(; vitAttributeList != vecSetItemAttributeList.end(); ++vitAttributeList )
    {
        std::wstring strAttributeColorKey = L"#c909090";
        if( iEquipSetItemCount >= (*vitAttributeList).iEquipCount )
            strAttributeColorKey = L"#cffff00";

        std::wstring strEquipCount = g_pkStrLoader->GetReplacedString( STR_ID_SET_ITEM_ATTRIBUTE_EQUIP_COUNT, "i", (*vitAttributeList).iEquipCount );
        int iSpaceSize = g_pkFontMgr->GetWidth( strEquipCount.c_str() ) / g_pkFontMgr->GetWidth(L' ');

        std::wstring strSpace;
        for(int i = 0 ; i < iSpaceSize; ++i )
            strSpace += L' ';

        std::vector< KGCItemManager::SetItemAttribute >::iterator vitAttribute = (*vitAttributeList).vecSetItemAttribute.begin();
        for(; vitAttribute != (*vitAttributeList).vecSetItemAttribute.end(); ++ vitAttribute )
        {
            if( vitAttribute != (*vitAttributeList).vecSetItemAttribute.begin() )
                strEquipCount += strSpace;
            strEquipCount += L' ';

            strEquipCount += g_pItemMgr->GetAtrributeString( (*vitAttribute).eAttributeType, (*vitAttribute).fAbility );

            std::wstring result;
            result = strAttributeColorKey + strEquipCount + strColorKeyEnd;
            strResult.push_back(result); 
            strEquipCount = L"";
        }
    }

    GCItem* pTitleItem = g_pItemMgr->GetItemData( iFindSetNumber );
    std::wstring strTitleItemName;
    if( pTitleItem )
        strTitleItemName = pTitleItem->strItemName;
    strTitleItemName = g_pkStrLoader->GetReplacedString( STR_ID_SET_ITEM_TITLE_STRING, "lii", strTitleItemName, iEquipSetItemCount, iPartsCount );
    std::vector<std::wstring> strResult2;

    strResult2.push_back(strTitleItemName);
    for(std::vector<std::wstring>::iterator vIter = strResult.begin();vIter != strResult.end();vIter++)
    {
        strResult2.push_back(*vIter);
    }

    return strResult2;
}

void KGCItemInformationBox::SetItemInformation( GCItem* pItem_, char cPetPromotion_ /*= -1 */, KItem* pInven_ /*= NULL*/, SUserItem* pUserItem_, KPetInfo* pPetInfo_, char cItemGrade_ /*= -1*/, int iLevel_ /*= -1*/)
{
    if( NULL == pItem_)
    {
        m_ItemID = UINT_MAX;
        m_ItemUID = 0;
        return;
    }
    m_pkItem = pItem_;

    GCITEMID NewItemID = pItem_->dwGoodsID;
    GCITEMUID NewItemUID = 0;
    char NewItemGrade = cItemGrade_;
    std::vector< KAttributeInfo > vecNewAttribute;
    std::vector< KSocketInfo >  vecNewSocket;

    //  강화된 속성 인덱스
    std::set<int> setEnchantAtt;

    if(pInven_)
    {
        NewItemUID = pInven_->m_ItemUID;
        vecNewAttribute = pInven_->m_vecAttribute;

        if( pItem_->eItemKind == GIK_MONSTER_CARD )
        {
            KGCAttributeTable attributetable;
            g_pItemMgr->GetAttributeTable(g_pItemMgr->GetAttributeTableID( NewItemID ), pInven_->m_cGradeID, attributetable);
            const std::vector<KGCAttributeTable::SAttribute>    &vecAttributelist = attributetable.GetContainer();

            std::vector<KGCAttributeTable::SAttribute>::const_iterator vIter = vecAttributelist.begin();
            for(;vIter != vecAttributelist.end(); vIter++)
            {
                KAttributeInfo buf;
                buf.m_cType = static_cast<char>(vIter->m_iType);
                buf.m_fValue = vIter->m_fValue;
                vecNewAttribute.push_back( buf );
            }

        }


        if( pInven_->m_EnchantEquipItemUID != 0 && pItem_ && pItem_->eItemKind != GIK_ENCHANT_ENABLE_ITEM )
        {
            KItem* pkEnchantItem = g_pItemMgr->GetInventoryItem(pInven_->m_EnchantEquipItemUID);

            if( pkEnchantItem )
            {
                for( int i = 0; i < static_cast<int>(pkEnchantItem->m_vecAttribute.size()); i++ )
                {
                    vecNewAttribute.push_back(pkEnchantItem->m_vecAttribute[i]);
                    setEnchantAtt.insert( vecNewAttribute.size() - 1 );
                }
            }
        }            
        vecNewSocket = pInven_->m_vecSocket;
    }
    else if( pUserItem_)
    {
        NewItemUID = pUserItem_->dwItemUID;
        vecNewAttribute = pUserItem_->vecItemAttribute;
        vecNewSocket = pUserItem_->vecItemSocket;
    }
    else
    {
        NewItemUID = 0;
        vecNewAttribute = pItem_->m_vecAttribute;
        vecNewSocket = pItem_->m_vecSocketInfo;
    }

    int nReqEquipLevel = g_pItemMgr->GetlItemEquipLevel( pItem_, pInven_,pUserItem_, true );

    if( m_ItemID == NewItemID && m_ItemUID == NewItemUID && m_vecAttribute == vecNewAttribute && 
        m_vecSocket == vecNewSocket && m_iChar == g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType && 
        m_cGrade == NewItemGrade && m_iLevel == iLevel_ && m_iNeedEquipLevel == nReqEquipLevel && pItem_->eItemKind != GIK_MONSTER_CARD)
        return;

    m_iLevel = iLevel_;
    m_cGrade = NewItemGrade;
    m_ItemID = NewItemID;
    m_ItemUID = NewItemUID;
    m_ItemBoxUID = (NewItemUID!=0)?NewItemUID:m_ItemBoxUID+1;
    m_iNeedEquipLevel = nReqEquipLevel;

    m_vecAttribute.swap( vecNewAttribute );
    m_vecSocket.swap( vecNewSocket );
    m_iChar = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;

    for(int i = 0 ; i < EIIG_MAX; ++i)
    {
        m_stItemInfoGroup[i].m_bExist = true;
    }


    m_strItemName = MakeItemNameString( pItem_, cPetPromotion_, pInven_, pUserItem_, pPetInfo_, cItemGrade_);
    m_pkName->SetText( m_strItemName );
    SetHeightGroup( EIIG_NAME, m_pkName );

    m_pkDefault1->SetTextAutoMultiline( MakeDefault1String( pItem_, pInven_, pUserItem_, pPetInfo_, iLevel_ ) );
    m_pkDefault2->SetTextAutoMultiline( MakeDefault2String( pItem_, pInven_ , pUserItem_, cItemGrade_, iLevel_ ) );
    SetHeightGroup( EIIG_DEFAULT, m_pkDefault2 );

    std::map< int, KGCSocketIcon*>::iterator mit = m_mapSocketIcon.begin();
    for(; mit != m_mapSocketIcon.end(); ++mit )
    {
        mit->second->ToggleRender( false );
    }


    m_stItemInfoGroup[EIIG_SOCKET].m_bExist = !m_vecSocket.empty();

    m_pkSocket->SetText();

    if( m_stItemInfoGroup[EIIG_SOCKET].m_bExist )
    {
        std::vector< KSocketInfo >::iterator vitSocket = m_vecSocket.begin();
        wstringstream strm;

#if defined ( NATION_PHILIPPINE )
        D3DXVECTOR2 vStartPos( 8.0f, m_pkSocket->GetFixedWindowLocalPos().y + 2.0f);
#else
        D3DXVECTOR2 vStartPos( 8.0f, m_pkSocket->GetFixedWindowLocalPos().y -2.0f );
#endif

        for( ; vitSocket != m_vecSocket.end(); ++vitSocket )
        {
            float fMoveY = static_cast<float>( PlusPosY( m_pkSocket ) );

            if( KSocketInfo::STS_USING == (*vitSocket).m_cState )
            {
                std::wstring strColorKey = L"#cffff00";
                std::wstring strColorKeyEnd = L"#cX";
                GCItem* pMonsterCardItem = g_pItemMgr->GetItemData( (*vitSocket).m_CardItemID / 10);
                if( pMonsterCardItem )
                {
                    std::vector< KAttributeInfo > vecAttribute = pMonsterCardItem->m_vecAttribute;
                    std::vector< KAttributeInfo >::iterator vitAttribute = pMonsterCardItem->m_vecAttribute.begin();

                    for(; vitAttribute != pMonsterCardItem->m_vecAttribute.end(); ++vitAttribute )
                    {
                        strm << g_pItemMgr->GetAtrributeString( (*vitAttribute).m_cType, (*vitAttribute).m_fValue ) << std::endl;;
                    }
                }
            }
            else
            {
                strm << g_pItemMgr->GetSocketStateString( (*vitSocket).m_cState ) << std::endl;
            }

            m_pkSocket->SetTextAutoMultiline( strm.str() );
            std::map< int, KGCSocketIcon* >::iterator mit = m_mapSocketIcon.find( (*vitSocket).m_cSlotID );

            if( mit == m_mapSocketIcon.end() )
            {
                char szMakeWndName[MAX_PATH] ="";
                sprintf_s( szMakeWndName, "socket_icon%d", (*vitSocket).m_cSlotID );

                KGCSocketIcon* pkSocketIcon = (KGCSocketIcon*)( g_pkUIMgr->CreateTemplateUIByFindName( m_stItemInfoGroup[EIIG_SOCKET].m_pkItemInfoGroup, "ui_item_information_box_pair.stg", szMakeWndName, "item_information_box_pair\\socket_icon" ) );

                pkSocketIcon->SetSocketState( static_cast< KSocketInfo::SOCKET_STATE>( ( *vitSocket ).m_cState ) );
                pkSocketIcon->ToggleRender( true );
                pkSocketIcon->SetWindowPosDirect(  D3DXVECTOR2( vStartPos.x, vStartPos.y + fMoveY) );
                m_mapSocketIcon.insert( std::map< int, KGCSocketIcon* >::value_type( (*vitSocket).m_cSlotID, pkSocketIcon ) );
            }
            else
            {
                mit->second->SetSocketState( static_cast< KSocketInfo::SOCKET_STATE>( ( *vitSocket ).m_cState ) );
                mit->second->ToggleRender( true );
                mit->second->SetWindowPosDirect( D3DXVECTOR2( vStartPos.x, vStartPos.y + fMoveY) );
            }
        }
    }

    SetHeightGroup( EIIG_SOCKET, m_pkSocket );

    //속성 처리
    ///////////////////////////////////////////////////////////////////////////////////////
    m_stItemInfoGroup[EIIG_ATTRIBUTE].m_bExist = !m_vecAttribute.empty();

    if( m_stItemInfoGroup[EIIG_ATTRIBUTE].m_bExist)
    {
        //m_pkAttribute->SetFontColor( 0xff00ffff );

        //if( pInven_ && pInven_->m_EnchantEquipItemUID )
        //    m_pkAttribute->SetFontColor( 0xffffff00 );

        if( pItem_->eItemKind == GIK_MONSTER_CARD )
        {
            std::vector< KAttributeInfo >::iterator vitAttribute = m_vecAttribute.begin();
            for(; vitAttribute != m_vecAttribute.end(); ++vitAttribute )
            {
                m_pkAttribute->SetTextAutoMultiline( g_pItemMgr->GetAtrributeString( (*vitAttribute).m_cType, (*vitAttribute).m_fValue ) );
                m_pkAttribute->SetFontColor(0xff00ffff);
            }
        }
        else
            m_pkAttribute->SetTextAutoMultiline( MakeAtrributeInfoString( m_vecAttribute, setEnchantAtt ) );
    }
    SetHeightGroup( EIIG_ATTRIBUTE, m_pkAttribute );

    int iSetItemNumber = -1;
    if( !(ESP_SET_ITEM & pItem_->dwSlotPosition) )
        iSetItemNumber = pItem_->iSetItemNumber;

    if( -1 == iSetItemNumber)
    {
        m_stItemInfoGroup[ EIIG_SET ].m_bExist = false;
    }
    else
    {
        std::vector<std::wstring> strSetItemList = MakeSetItemListString( pItem_->dwGoodsID );
        m_stItemInfoGroup[ EIIG_SET ].m_bExist = !strSetItemList.empty();
        if(!strSetItemList.empty()){
            m_pkSetItem->Clear();
            for(std::vector<std::wstring>::iterator vIter = strSetItemList.begin();vIter != strSetItemList.end();vIter++)
            {
                m_pkSetItem->AddString(*vIter);
            }
        }
    }
    SetHeightGroup( EIIG_SET, m_pkSetItem );
    



#if defined( NATION_BRAZIL )
    m_pkDesc->SetWordWrap( true );
#endif
    m_pkDesc->SetTextAutoMultiline( MakeDescString( pItem_, pInven_, cPetPromotion_ ) );
    SetHeightGroup( EIIG_DESC, m_pkDesc );
    

    //if( GIK_EQUIP !=  pItem_->eItemKind )
    if( ( false == g_pItemMgr->IsEquipmentItem( pItem_->dwGoodsID ) && 0 == (pItem_->dwSlotPosition & ESP_SET_ITEM) ) )
    {
        m_stItemInfoGroup[ EIIG_SOCKET ].m_bExist = false;
        m_stItemInfoGroup[ EIIG_ATTRIBUTE ].m_bExist = false;
        m_stItemInfoGroup[ EIIG_SET ].m_bExist = false;
        m_stItemInfoGroup[ EIIG_EXTRADESC ].m_bExist = false;
    }

#if defined( DISABLE_ITEM_SOCKET )
    m_stItemInfoGroup[ EIIG_SOCKET ].m_bExist = false;
#endif

    for( int i = 0 ; i < EIIG_MAX; ++i)
    {
        ToggleRenderItemInfoGroup( (EITEMINFOGROUP)i, m_stItemInfoGroup[ i ].m_bExist );
    }

    SetShowExtraInfo( m_bShowExtraInfo );

}

std::wstring KGCItemInformationBox::MakeGradeString( GCItem* pItem_, KItem* pInventoryItem_, SUserItem* pUserItem_, char cItemGrade_ /*= -1*/ )
{
    wostringstream strm;

    char cItemGrade = 0;

    if ( cItemGrade_ != -1 )
        cItemGrade = cItemGrade_;
    else
    {
        if( pInventoryItem_ )
        {
            cItemGrade = pInventoryItem_->m_cGradeID;
        }
        else if( pUserItem_)
        {
            cItemGrade = pUserItem_->cGradeID;
        }
        else if( pItem_ )
        {
            cItemGrade = pItem_->cItemGrade;
        }
    }

    std::wstring strGradeColorKey = g_pItemMgr->GetItemGradeColorKey( cItemGrade );
    std::wstring strColorKeyEnd = L"#cX";

    strm << strGradeColorKey << g_pItemMgr->GetItemGradeString( cItemGrade) << strColorKeyEnd;
    return strm.str();
}

void KGCItemInformationBox::ActionPerformed( const KActionEvent& event )
{

}

std::wstring KGCItemInformationBox::MakeSocketInfoString( const std::vector< KSocketInfo >& vecSocketInfo_ )
{
    wstringstream strm;
    std::vector< KSocketInfo >::const_iterator cvit = vecSocketInfo_.begin();

    for(; cvit != vecSocketInfo_.end(); ++cvit )
    {
        if( KSocketInfo::STS_USING == (*cvit).m_cState )
        {
            GCItem* pItem = g_pItemMgr->GetItemData( ( *cvit).m_CardItemID / 10 );
            std::vector< KAttributeInfo > vecAttributeInfo = pItem->m_vecAttribute;
            std::vector< KAttributeInfo >::iterator vitAttribute = vecAttributeInfo.begin();
            std::wstring strColorKey = L"#c00ffff";
            std::wstring strColorKeyEnd = L"#cX";

            if( vecAttributeInfo.end() != vitAttribute )
            {
                strm << strColorKey << g_pItemMgr->GetAtrributeString( (*vitAttribute).m_cType, (*vitAttribute).m_fValue );
                ++vitAttribute;
            }

            for(; vitAttribute != vecAttributeInfo.end(); ++vitAttribute )
            {
                strm << L" / " << g_pItemMgr->GetAtrributeString( (*vitAttribute).m_cType, (*vitAttribute).m_fValue );
            }

            strm << strColorKeyEnd << std::endl;
        }
        else
        {
            strm << g_pItemMgr->GetSocketStateString( (*cvit).m_cState ) << std::endl;
        }
    }

    return strm.str();
}

std::wstring KGCItemInformationBox::MakeDefault1String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ )
{
    std::wstring strResult = g_pItemMgr->GetItemTypeString( pItem_ );

    //기간제 아이템은 시간을 표시해 줍니다.
    if( pInven_ != NULL ) {
        std::wstringstream stm;
        if( pInven_->m_nPeriod != KItem::UNLIMITED_ITEM ) {
            stm << " (" << g_pkStrLoader->GetString( STR_ID_ITEM_EXTRA_PERIOD ) << g_pItemMgr->GetRemainTime( *pInven_ ) << ")";
        }
        else if( pInven_->m_nCount != KItem::UNLIMITED_ITEM ) {
            stm << " (" << g_pkStrLoader->GetString( STR_ID_ITEM_EXTRA_DURATION )  << pInven_->m_nCount << ")";
        }
        strResult += stm.str();
    }

    if( (g_pItemMgr->IsEquipmentItem( pItem_->dwGoodsID ) || ( pItem_->dwSlotPosition & ESP_SET_ITEM )) && (pItem_->eItemKind != GIK_MONSTER_CARD) )
        strResult += L"\n" + MakeAbilityString( pItem_, pInven_, pUserItem_, pPetInfo_, iLevel_ );

    return strResult;
}

std::wstring KGCItemInformationBox::MakeDefault2String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, char cItemGrade_ /*= -1*/, int iLevel_ /*= -1*/)
{
    std::wstring strResult = MakeGradeString( pItem_, pInven_, pUserItem_, cItemGrade_ );
    if( (g_pItemMgr->IsEquipmentItem( pItem_->dwGoodsID ) || ( pItem_->dwSlotPosition & ESP_SET_ITEM )) && (pItem_->eItemKind != GIK_MONSTER_CARD) )
        strResult += L"\n" + MakeEquipLimitString( pItem_, pInven_, pUserItem_, iLevel_ );

    strResult += L"\n" + MakePriceString( pItem_);

    return strResult;
}

std::wstring KGCItemInformationBox::MakePriceString( GCItem* pItem_ )
{
    wstringstream strm;
    strm << g_pItemMgr->GetItemMoneyTypeString( pItem_ ) << L" " << pItem_->dwPrice;
    return strm.str();
}

std::wstring KGCItemInformationBox::MakeAtrributeInfoString( const std::vector< KAttributeInfo >& vecAttributeInfo_, const std::set<int>& setEnchantAtt )
{
    std::wstring strResult;
    std::vector< KAttributeInfo >::const_iterator vitAtrribute = vecAttributeInfo_.begin();

    for( ; vitAtrribute != vecAttributeInfo_.end(); ++vitAtrribute )
    {
        std::wstring strAttribute;
        if( KAttributeInfo::ATS_OPENED == (*vitAtrribute).m_cState )
            strAttribute = g_pkStrLoader->GetString( STR_ID_ITEM_UNKNOWN_STRING );
        else
            strAttribute = g_pItemMgr->GetAtrributeString( (*vitAtrribute).m_cType, (*vitAtrribute).m_fValue );

//      if( (*vitAtrribute).m_cType >= ATTRTYPE_CRITICAL_DAMAGE_RESIST && (*vitAtrribute).m_cType <= ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST )
        if ( setEnchantAtt.find( vitAtrribute -  vecAttributeInfo_.begin() ) != setEnchantAtt.end() )
        {
            strResult += L"#cffff00" + g_pkStrLoader->GetReplacedString( STR_ID_ENCHANT_ATTRIBUTE_TITLE, "l", strAttribute) + L"\n";
        }
        else
        {            
            strResult += L"#c00ffff" + g_pkStrLoader->GetReplacedString( STR_ID_ATTRIBUTE__PRE_STRING, "l", strAttribute) + L"\n";
        }
    }

    return strResult;
}

void KGCItemInformationBox::SetBoxState( EITEMINFORMATIONBOXSTATE eItemInformationboxState_ )
{
    if( m_eItemInformationBoxState < 0 || m_eItemInformationBoxState >= EIIBS_MAX )
        return;

    m_eItemInformationBoxState = eItemInformationboxState_;
    std::wstring strNewName = m_strEquipState[ m_eItemInformationBoxState] + m_strItemName;
    m_pkName->SetText( strNewName );
    SetHeightGroup( EIIG_NAME, m_pkName );

    if( EIIBS_EQUIPPED == m_eItemInformationBoxState )
    {
        ToggleRenderItemInfoGroup( EIIG_EXTRADESC, false );
    }

    //EIIG_EXTRADESC 맨 마지막에 있는애라 안해도 되지만.. 순서를 바꾸게된다면 해줘야된다.
    SetUIPosition();
}

void KGCItemInformationBox::SetCompareIcon( EITEMCOMPAREICON eItemCompareIcon_ )
{
    for( int i= 0; i < EICI_MAX; ++i )
    {
        m_pkCompareIcon[i]->ToggleRender( false );
    }

    if( eItemCompareIcon_ < 0 || eItemCompareIcon_>= EICI_MAX )
        return;

    m_pkCompareIcon[ eItemCompareIcon_]->ToggleRender( true );
}

std::wstring KGCItemInformationBox::MakeItemNameString( GCItem* pItem_, char cPetPromotion_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, char cItemGrade_ /*= -1*/ )
{
    wstringstream strm;
    char cItemGrade = 0;

    if ( cItemGrade_ != -1 )
        cItemGrade = cItemGrade_;
    else
    {
        if( pInven_ )
            cItemGrade = pInven_->m_cGradeID;
        else if( pUserItem_ )
            cItemGrade = pUserItem_->cGradeID;
        else if( pItem_ )
            cItemGrade = pItem_->cItemGrade;
    }

    DWORD dwCommonCharType = 0; 
    for( int i = 0 ; i < GC_CHAR_NUM; ++i)
    {
        dwCommonCharType = dwCommonCharType | ( 1 << i );
    }

    std::wstring strItemCharType = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );
    
    if(dwCommonCharType != pItem_->dwCharType ) {
        strItemCharType = GCFUNC::GetCharName( pItem_->GetCharType() );
    }

#if defined( USE_BIND_PET )
    if( pItem_->eItemKind == GIK_PET && pItem_->ePetItemKind == EPET_KIND_PET ) { 
        if( pInven_ == NULL ) { 
            strItemCharType = L"";
        }
        else { 
            std::map< GCITEMUID, KPetInfo >::const_iterator petIter = g_MyD3D->m_mapPetInfo.find( pInven_->m_ItemUID );
            if( petIter == g_MyD3D->m_mapPetInfo.end() ) { 
                strItemCharType = L"";
            }
            else if( petIter->second.m_cCharType != -1 ) {
                strItemCharType = g_kGlobalValue.GetCharName( static_cast<int>( petIter->second.m_cCharType ) );
            }
        }
    }
#endif

    if( strItemCharType.empty() ) { 
        strm << g_pItemMgr->GetItemGradeColorKey( cItemGrade ) << g_pItemMgr->GetItemName( pItem_, cPetPromotion_, pInven_, pUserItem_, pPetInfo_ ) << L"#cX";
    }
    else { 
        strm << g_pItemMgr->GetItemGradeColorKey( cItemGrade ) << g_pItemMgr->GetItemName( pItem_, cPetPromotion_, pInven_, pUserItem_, pPetInfo_ ) << L"(" << strItemCharType << L")" << L"#cX";
    }
    return strm.str();
}

int KGCItemInformationBox::PlusPosY( KD3DStatic* pkStatic_ )
{
    int iSpaceSize =  static_cast<int>( pkStatic_->GetLineSpace() * static_cast<float>( pkStatic_->GetFontMgr()->GetHeight() ) );
    return pkStatic_->GetLineNum() * iSpaceSize;
}

int KGCItemInformationBox::PlusPosY( KD3DListBox* pkStatic_ )
{
    int iSpaceSize =  static_cast<int>(pkStatic_->GetLineSpace() * static_cast<float>(pkStatic_->GetDeltaY(false,false) ) );
    return pkStatic_->GetCount() * iSpaceSize;
}

void KGCItemInformationBox::SetShowExtraInfo( bool bShow_ )
{
    m_bShowExtraInfo = bShow_;

    std::wstring strExtraDesc;
    if( m_bEquippedItem && false == IsExistExtraInfoGroup() )
    {
        m_stItemInfoGroup[ EIIG_EXTRADESC ].m_bExist = false;
        ToggleRenderItemInfoGroup( EIIG_EXTRADESC, false );
    }
    else
    {
        if( IsExistExtraInfoGroup() )
        {
            if( m_bShowExtraInfo )
                strExtraDesc = m_strShowExtraInfo;
            else
                strExtraDesc = m_strHideExtraInfo;

            if( false == m_bEquippedItem )
                strExtraDesc += L"\n";
        }

        if( false == m_bEquippedItem )
            strExtraDesc += m_strExtraDescString;

        m_pkExtraDesc->SetTextAutoMultiline( strExtraDesc );
        SetHeightGroup( EIIG_EXTRADESC, m_pkExtraDesc );
    }

    ShowExtraInfo( m_bShowExtraInfo );
    SetUIPosition();
}

bool KGCItemInformationBox::IsExistExtraInfoGroup()
{
    std::vector< EITEMINFOGROUP>::iterator vit =  m_vecExtraInfoGroup.begin();

    for( ; vit != m_vecExtraInfoGroup.end(); ++vit )
    {
        int iIndex = static_cast<int>( *vit );
        if( iIndex < 0 || iIndex >= EIIG_MAX )
            continue;

        if( m_stItemInfoGroup[ iIndex ].m_bExist )
            return true;
    }

    return false;
}

void KGCItemInformationBox::ShowExtraInfo( bool bShow_)
{
    std::vector< EITEMINFOGROUP>::iterator vit =  m_vecExtraInfoGroup.begin();

    for( ; vit != m_vecExtraInfoGroup.end(); ++vit )
    {
        ToggleRenderItemInfoGroup( (*vit), bShow_ );
    }
}


void KGCItemInformationBox::SetKeyText( int iStringID )
{
    m_strExtraDescString = g_pkStrLoader->GetString( iStringID );
    SetShowExtraInfo( m_bShowExtraInfo );
}

std::wstring KGCItemInformationBox::MakeDescString( GCItem* pItem_, KItem* pInven_, char cPetPromotion_ )
{
    std::wstring strDesc = L"";

    // 창고 이동 불가능 아이템
    if ( !SiKGCDepotManager()->IsInsertDepotItem( pItem_, pInven_ ) )
    {
        strDesc += L"#cff0000" + g_pkStrLoader->GetString( STR_ID_DISABLE_DEPOT_MOVE ) + L"#cX" + L"\n";
    }
    else
    {
        strDesc += L"#c00ccff" + g_pkStrLoader->GetString( STR_ID_ENABLE_DEPOT_MOVE ) + L"#cX" + L"\n";
    }

    strDesc += g_pItemMgr->GetItemNameDesc( pItem_, cPetPromotion_ );
    return strDesc;
}