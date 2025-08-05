#include "stdafx.h"
#include "GCCoordiItemInformationBox.h"

IMPLEMENT_CLASSNAME( KGCCoordiItemInformationBox );
IMPLEMENT_WND_FACTORY( KGCCoordiItemInformationBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiItemInformationBox, "gc_coordiiteminformationbox" );

KGCCoordiItemInformationBox::KGCCoordiItemInformationBox( void )
: m_pkBackground( NULL )
, m_pkCenterBackground( NULL )
, m_pkName( NULL )
, m_pkDefault1( NULL )
, m_pkDefault2( NULL )
//, m_pkAttribute( NULL )
, m_pkSetItem( NULL )
, m_pkDesc( NULL )
, m_pkItem( NULL )

, m_cGrade( -1 )
, m_ItemID( UINT_MAX )
, m_ItemUID( 0 )
, m_iChar( 0 )
, m_iAlpha( 255 )
, m_iCoordiSeasonNum ( UINT_MAX )
{
    for( int i = 0 ; i < EIIG_MAX; ++i )
    {
        char szTemp[ MAX_PATH ] = "";
        sprintf_s( szTemp, MAX_PATH, "item_information_group%d", i );
        LINK_CONTROL( szTemp, m_stItemInfoGroup[i].m_pkItemInfoGroup );
    }

    LINK_CONTROL( "item_information_box_center_background", m_pkCenterBackground );
    LINK_CONTROL( "item_information_box_background", m_pkBackground );

}

KGCCoordiItemInformationBox::~KGCCoordiItemInformationBox( void )
{
}

void KGCCoordiItemInformationBox::OnCreate( void )
{
    m_pkName = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_NAME].m_pkItemInfoGroup->GetChildByName("item_information_name") );
    m_pkDefault1 = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DEFAULT].m_pkItemInfoGroup->GetChildByName("item_information_position_ability") );
    m_pkDefault2 = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DEFAULT].m_pkItemInfoGroup->GetChildByName("item_information_kind_equiplimit_price") );
    m_pkSetItem = static_cast<KD3DListBox*>( m_stItemInfoGroup[EIIG_SET].m_pkItemInfoGroup->GetChildByName("item_information_setitem") );
    m_pkDesc = static_cast<KD3DStatic*>( m_stItemInfoGroup[EIIG_DESC].m_pkItemInfoGroup->GetChildByName("item_information_desc") );

    //DEFAULT_FONT_SIZE
    m_pkName->SetFontOutline( true );
    m_pkName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkName->SetLineSpace( 1.2f );
    m_pkName->SetAlign( DT_CENTER );

    m_pkDefault1->SetLineSpace(1.3f);
    m_pkDefault2->SetLineSpace(1.3f);
    m_pkDefault2->SetAlign( DT_RIGHT );

    m_pkSetItem->SetLineSpace( 1.3f );
    m_pkDesc->SetLineSpace(1.3f);
    m_pkDesc->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));

    for(int i = 0 ; i < EIIG_MAX; ++i)
    {
        m_stItemInfoGroup[i].m_dwExtraSize = 2;
        m_stItemInfoGroup[i].m_dwExtraPlus = 3;
        m_stItemInfoGroup[i].m_pkBackGround = static_cast<KD3DSizingBox*>( m_stItemInfoGroup[i].m_pkItemInfoGroup->GetChildByName("item_information_background") );
    }

    m_stItemInfoGroup[EIIG_NAME].m_dwExtraPlus = 9;
}

void KGCCoordiItemInformationBox::OncreateComplete( void )
{
}

void KGCCoordiItemInformationBox::FrameMoveInEnabledState( void )
{
}

void KGCCoordiItemInformationBox::PostChildDraw()
{
#ifndef __PATH__
    if( GetAsyncKeyState( VK_SHIFT ) && GetAsyncKeyState( VK_CONTROL ) )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        g_pkUIMgr->RenderNumber( "white_num", vPos, g_pItemMgr->GetSameImageIndex(m_ItemID) *10);

        vPos.y += 80;
        g_pkUIMgr->RenderNumber( "white_num", vPos, static_cast<DWORD>(m_ItemUID) );
    }
    else if( GetAsyncKeyState( VK_SHIFT ) )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        g_pkUIMgr->RenderNumber( "white_num", vPos, m_ItemID * 10 );

        vPos.y += 80;
        g_pkUIMgr->RenderNumber( "white_num", vPos, static_cast<DWORD>(m_ItemUID) );
        g_pItemMgr->GetSameImageIndex(m_ItemID);
    }

#endif
}

std::wstring KGCCoordiItemInformationBox::MakeDiffString( int iDiff )
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

void KGCCoordiItemInformationBox::SetBoxState()
{
    m_pkName->SetText( m_strItemName );
    SetHeightGroup( EIIG_NAME, m_pkName );

    SetUIPosition();
}

void KGCCoordiItemInformationBox::SetUIPosition()
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

void KGCCoordiItemInformationBox::ToggleRenderItemInfoGroup( EITEMINFOGROUP eItemInfoGroup_, bool bShow_ )
{
    if( ( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ ) ||
        ( bShow_ && false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->ToggleRender( bShow_ );
}

void KGCCoordiItemInformationBox::SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic_)
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ || ( false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    DWORD dwNewHeight = static_cast<DWORD>( psStanDardStatic_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight );
}

void KGCCoordiItemInformationBox::SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic1_, KD3DStatic* psStanDardStatic2_)
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ || ( false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    DWORD dwNewHeight1 = static_cast<DWORD>( psStanDardStatic1_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic1_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;
    DWORD dwNewHeight2 = static_cast<DWORD>( psStanDardStatic2_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic2_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    if ( dwNewHeight1 < dwNewHeight2 )
    {
        dwNewHeight1 = dwNewHeight2;
    }

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight1 );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight1 );

}

void KGCCoordiItemInformationBox::SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DListBox* psStanDardStatic_)
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ || ( false == m_stItemInfoGroup[ eItemInfoGroup_ ].m_bExist ) )
        return;

    DWORD dwNewHeight = static_cast<DWORD>( psStanDardStatic_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight );
}
void KGCCoordiItemInformationBox::SetPositionGroup( EITEMINFOGROUP eItemInfoGroup_, D3DXVECTOR2 vNewPos_ )
{
    if( eItemInfoGroup_< 0 || EIIG_MAX <= eItemInfoGroup_ )
        return;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetWindowPosDirect( vNewPos_ );
}

std::wstring KGCCoordiItemInformationBox::MakeEquipLimitString(  GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ )
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
    if ( pInven_ && pInven_->m_sEquipLvDown ) 
    {
        nEquipLVDown = pInven_->m_sEquipLvDown;
    }
//     else if ( pUserItem_ && pUserItem_->m_nEquipLvDown > 0 )
//     {
//         nEquipLVDown = pUserItem_->m_nEquipLvDown;
//     }


    EItemAbilityData eItemAbilityData = g_pItemMgr->GetItemAttributeAbilityData( pItem_, pCharInfo->iLevel, pInven_, pUserItem_ );
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

    if( 0 == iNeedForEquipChangeJobLevel)
    {
        if( pItem_->dwSlotPosition & ESP_WEAPON )
        {
            strJobLevel += g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON1 ) + strColorKeyEnd;
            stm << strJobLevel;
        }
    }
    else
    {

#if defined(NATION_THAILAND) || defined( NATION_TAIWAN ) || defined(NATION_PHILIPPINE) //한국은 기본 무기, 1차,2차,3차 무기... 외국(태국)은 1차,2차,3차,4차 무기...
        iNeedForEquipChangeJobLevel += 1;
#endif
        strJobLevel += g_pkStrLoader->GetReplacedString( STR_ID_ITEM_JOB_LEVEL_STRING, "i", iNeedForEquipChangeJobLevel) + strColorKeyEnd;
        stm << strJobLevel;
    }
    return stm.str();		
}

std::vector<std::wstring> KGCCoordiItemInformationBox::MakeSetItemListString( GCITEMID ItemID_ )
{
    std::set< int > setPartsList;
    int iSeasonNum = g_pItemMgr->GetCoordiSeasonNum( ItemID_, m_cGrade, &setPartsList );

    std::vector< KGCItemManager::SetItemAttributeList > vecSetItemAttributeList;
    std::vector<std::wstring> strResult;

    if ( iSeasonNum == UINT_MAX ) 
        return strResult;

    if ( setPartsList.empty() )
        return strResult;

    if ( !g_pItemMgr->GetCoordiSeasonAttribute( m_iCoordiSeasonNum, vecSetItemAttributeList ) )
        return strResult;

    if ( vecSetItemAttributeList.empty() )
        return strResult;


    std::wstring strColorKeyEnd = L"#cX";
    int iPartsCount = setPartsList.size();
    int iEquipSetItemCount = 0;

    //  시즌 정보
    std::wstring strTitleItemName;
    strTitleItemName = g_pItemMgr->GetCoordiSeasonName( m_iCoordiSeasonNum );
    strTitleItemName += L" ";
    
    std::set< int >::iterator vit = setPartsList.begin();
    for(; vit != setPartsList.end(); ++vit)
    {
        COORDI_SLOT_POSION emCoordiSlot = static_cast<COORDI_SLOT_POSION>((*vit));
        
        ESLOTPOSITION emPosition = g_pItemMgr->GetSlotPosion( emCoordiSlot );
        if ( emPosition == ESP_NONE )
            continue;

        std::wstring strPartsColorKey = L"#c909090";

        int iSlotSeasonNum = g_pItemMgr->GetCoordiSlotSeasonNum( 
            &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), emPosition );
        
        if( m_iCoordiSeasonNum == iSlotSeasonNum )
        {
            strPartsColorKey = L"#cffff00";
            ++iEquipSetItemCount;
        }
        
        std::wstring result;
        std::wstring strItemName = strTitleItemName + g_pItemMgr->GetCoordiSlotString(emPosition);

        result += L"  " +strPartsColorKey + strItemName + strColorKeyEnd;
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

    strTitleItemName +=  g_pItemMgr->GetCoordiSlotString( ESP_SET_ITEM );
    
    strTitleItemName = g_pkStrLoader->GetReplacedString( STR_ID_SET_ITEM_TITLE_STRING, "lii", strTitleItemName, iEquipSetItemCount, iPartsCount );
    std::vector<std::wstring> strResult2;

    strResult2.push_back(strTitleItemName);
    for(std::vector<std::wstring>::iterator vIter = strResult.begin();vIter != strResult.end();vIter++)
    {
        strResult2.push_back(*vIter);
    }

    return strResult2;
}

void KGCCoordiItemInformationBox::SetItemInformation( GCItem* pItem_, char cPetPromotion_ /*= -1 */, KItem* pInven_ /*= NULL*/, SUserItem* pUserItem_, KPetInfo* pPetInfo_, char cItemGrade_ /*= -1*/, int iLevel_ /*= -1*/)
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

    if(pInven_)
    {
        NewItemUID = pInven_->m_ItemUID;
        vecNewAttribute = pInven_->m_vecAttribute;
        NewItemGrade = pInven_->m_cGradeID;
    }
    else if( pUserItem_)
    {
        NewItemUID = pUserItem_->dwItemUID;
        vecNewAttribute = pUserItem_->vecItemAttribute;
        NewItemGrade = pUserItem_->cGradeID;
    }
    else
    {
        NewItemUID = 0;
        vecNewAttribute = pItem_->m_vecAttribute;
        NewItemGrade = pItem_->cItemGrade;
    }

    if ( cItemGrade_ != -1 )
        NewItemGrade = cItemGrade_;

    int nReqEquipLevel = g_pItemMgr->GetlItemEquipLevel( pItem_, pInven_, pUserItem_ );

    m_iLevel = iLevel_;
    m_cGrade = NewItemGrade;
    m_ItemID = NewItemID;
    m_ItemUID = NewItemUID;
    m_ItemBoxUID = (NewItemUID!=0)?NewItemUID:m_ItemBoxUID+1;
    m_iCoordiSeasonNum = g_pItemMgr->GetCoordiSeasonNum( m_ItemID, m_cGrade );
    m_iReqEquipLevel = nReqEquipLevel;

    m_vecAttribute.swap( vecNewAttribute );
    m_iChar = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;

    for(int i = 0 ; i < EIIG_MAX; ++i)
    {
        m_stItemInfoGroup[i].m_bExist = true;
    }

    m_strItemName = MakeItemNameString();
    m_pkName->SetText( m_strItemName );
    SetHeightGroup( EIIG_NAME, m_pkName );

    m_pkDefault1->SetTextAutoMultiline( MakeDefault1String( pItem_, pInven_, pUserItem_, pPetInfo_, iLevel_ ) );
    m_pkDefault2->SetTextAutoMultiline( MakeDefault2String( pItem_, pInven_ , pUserItem_, iLevel_ ) );
    SetHeightGroup( EIIG_DEFAULT, m_pkDefault1, m_pkDefault2 );

    if( UINT_MAX == m_iCoordiSeasonNum )
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

    if( ( false == g_pItemMgr->IsEquipmentItem( pItem_->dwGoodsID ) && 0 == (pItem_->dwSlotPosition & ESP_SET_ITEM) ) || GIK_MONSTER_CARD == pItem_->eItemKind )
    {
        m_stItemInfoGroup[ EIIG_SET ].m_bExist = false;
    }

    for( int i = 0 ; i < EIIG_MAX; ++i)
    {
        ToggleRenderItemInfoGroup( (EITEMINFOGROUP)i, m_stItemInfoGroup[ i ].m_bExist );
    }

    SetUIPosition();
}

std::wstring KGCCoordiItemInformationBox::MakeGradeString( GCItem* pItem_, KItem* pInventoryItem_, SUserItem* pUserItem_ )
{
    wostringstream strm;

    std::wstring strGradeColorKey = g_pItemMgr->GetItemGradeColorKey( m_cGrade );
    std::wstring strColorKeyEnd = L"#cX";

    strm << strGradeColorKey << g_pItemMgr->GetItemGradeString( m_cGrade) << strColorKeyEnd;
    return strm.str();
}

void KGCCoordiItemInformationBox::ActionPerformed( const KActionEvent& event )
{

}

std::wstring KGCCoordiItemInformationBox::MakeDefault1String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ )
{
    std::wstring strResult = g_pItemMgr->GetCoordiSlotString( static_cast<ESLOTPOSITION>(pItem_->dwSlotPosition) );


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

    //  원래 코디 아이템만 능력치를 표시한다.
    if ( m_pkItem->eItemKind == GIK_COORDI_ITEM ) 
    {
    strResult +=  L"\n" + MakeAtrributeInfoString( m_vecAttribute );
    }

    return strResult;
}

std::wstring KGCCoordiItemInformationBox::MakeDefault2String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ /*= -1*/)
{
    std::wstring strResult = MakeGradeString( pItem_, pInven_, pUserItem_ );
    if( (g_pItemMgr->IsEquipmentItem( pItem_->dwGoodsID ) || ( pItem_->dwSlotPosition & ESP_SET_ITEM )) && (pItem_->eItemKind != GIK_MONSTER_CARD) )
        strResult += L"\n" + MakeEquipLimitString( pItem_, pInven_, pUserItem_, iLevel_ );

    strResult += L"\n";
    strResult += L"#cffff00" + g_pItemMgr->GetCoordiSeasonName( m_iCoordiSeasonNum );
    strResult += L"\n" + MakePriceString( pItem_);

    return strResult;
}

std::wstring KGCCoordiItemInformationBox::MakePriceString( GCItem* pItem_ )
{
    wstringstream strm;
    strm << g_pItemMgr->GetItemMoneyTypeString( pItem_ ) << L" " << pItem_->dwPrice;
    return strm.str();
}

std::wstring KGCCoordiItemInformationBox::MakeAtrributeInfoString( const std::vector< KAttributeInfo >& vecAttributeInfo_ )
{
    std::wstring strResult;
    std::vector< KAttributeInfo >::const_iterator vitAtrribute = vecAttributeInfo_.begin();

    for( ; vitAtrribute != vecAttributeInfo_.end(); ++vitAtrribute )
    {
        std::wstring strAttribute;
        if( KAttributeInfo::ATS_OPENED == (*vitAtrribute).m_cState )
            continue;
//            strAttribute = g_pkStrLoader->GetString( STR_ID_ITEM_UNKNOWN_STRING );
        else
            strAttribute = g_pItemMgr->GetAtrributeString( (*vitAtrribute).m_cType, (*vitAtrribute).m_fValue );

//         if( (*vitAtrribute).m_cType >= ATTRTYPE_CRITICAL_DAMAGE_RESIST && (*vitAtrribute).m_cType <= ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST )
//         {
//             strResult += L"#cffff00" + g_pkStrLoader->GetReplacedString( STR_ID_ENCHANT_ATTRIBUTE_TITLE, "l", strAttribute) + L"\n";
//         }
//         else
//         {            
             strResult += L"#c00ffff" + g_pkStrLoader->GetReplacedString( STR_ID_ATTRIBUTE__PRE_STRING, "l", strAttribute) + L"\n";
//         }
    }

    return strResult;
}

std::wstring KGCCoordiItemInformationBox::MakeItemNameString()
{
    wstringstream strm;

    DWORD dwCommonCharType = 0; 
    for( int i = 0 ; i < GC_CHAR_NUM; ++i)
    {
        dwCommonCharType = dwCommonCharType | ( 1 << i );
    }

    GCItem* pItem = g_pItemMgr->GetItemData( m_ItemID );
    if ( !pItem )
        return strm.str();

    std::wstring strItemCharType = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );
    if(dwCommonCharType != pItem->dwCharType )
        strItemCharType = GCFUNC::GetCharName( pItem->GetCharType() );


    strm << g_pItemMgr->GetItemGradeColorKey( m_cGrade ) << g_pItemMgr->GetItemName( m_ItemID ) << L"(" << strItemCharType << L")" << L"#cX";
    return strm.str();
}

int KGCCoordiItemInformationBox::PlusPosY( KD3DStatic* pkStatic_ )
{
    int iSpaceSize =  static_cast<int>( pkStatic_->GetLineSpace() * static_cast<float>( pkStatic_->GetFontMgr()->GetHeight() ) );
    return pkStatic_->GetLineNum() * iSpaceSize;
}

int KGCCoordiItemInformationBox::PlusPosY( KD3DListBox* pkStatic_ )
{
    int iSpaceSize =  static_cast<int>(pkStatic_->GetLineSpace() * static_cast<float>(pkStatic_->GetDeltaY(false,false) ) );
    return pkStatic_->GetCount() * iSpaceSize;
}

std::wstring KGCCoordiItemInformationBox::MakeDescString( GCItem* pItem_,  KItem* pInven_, char cPetPromotion_ )
{
    std::wstring strDesc = L"";

    // 창고 이동 불가능 아이템
    if ( !SiKGCDepotManager()->IsInsertDepotItem( pItem_, pInven_ )  )
    {
        strDesc += L"#cff0000" + g_pkStrLoader->GetString( STR_ID_DISABLE_DEPOT_MOVE ) + L"#cX" + L"\n";
    }
    else
    {
        strDesc += L"#c00ccff" + g_pkStrLoader->GetString( STR_ID_ENABLE_DEPOT_MOVE ) + L"#cX" + L"\n";
    }


    KD3DWnd* pWnd = static_cast<KD3DWnd*>( m_stItemInfoGroup[EIIG_DESC].m_pkItemInfoGroup->GetChildByName("img_coordi_compose") );
    if ( pWnd )
        pWnd->ToggleRender( false );

    if ( m_pkItem->eItemKind == GIK_COORDI_ITEM )
    {
        strDesc += g_pItemMgr->GetItemNameDesc(pItem_, cPetPromotion_ );
        if ( pInven_ && pInven_->m_DesignCoordiID != 0 )
        {
            strDesc += L"\n";
            strDesc += L"　　";  // 아이콘때문에 특문 공백을 넣어야 한다.
            strDesc += g_pkStrLoader->GetReplacedString( STR_ID_COORDI_COMPOSE_DESC, "L", g_pItemMgr->GetItemName( pInven_->m_DesignCoordiID ) );

            if ( pWnd )
            {
                pWnd->ToggleRender( true );
            }
        }
    }
    else
    {
        strDesc += g_pkStrLoader->GetString( STR_ID_NOT_COORDI_NOTIFY );
    }

    return strDesc;
}