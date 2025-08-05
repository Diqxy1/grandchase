#include "stdafx.h"
#include "KGCItemInformationBreakupBox.h"
//
#include "../MyD3D.h"

#include "EnchantStrong.h"
#include "KSingleton.h"

#include "KGCEmoticon.h"
IMPLEMENT_CLASSNAME( KGCItemInformationBreakupBox );
IMPLEMENT_WND_FACTORY( KGCItemInformationBreakupBox );
IMPLEMENT_WND_FACTORY_NAME( KGCItemInformationBreakupBox, "gc_item_information_breakup_box" );

KGCItemInformationBreakupBox::KGCItemInformationBreakupBox( void )
: m_pkBackground( NULL )
, m_pkCenterBackground( NULL )
, m_pkItem( NULL )
{
    // 텍스쳐 초기화
    m_vecpTexBreakUpItem.clear();
    for(int iLoop=0; iLoop<MATERIAL_MAX; ++iLoop)
    {
        m_iTextureIndex[ iLoop ] = 0;
    }

#if defined( ITEM_BREAKUP_INFO_BOX )
    // 백 그라운드
    LINK_CONTROL( "item_information_box_center_background", m_pkCenterBackground );
    LINK_CONTROL( "item_information_box_background", m_pkBackground );

    // 이름
    LINK_CONTROL( "item_information_name", m_stItemInfoGroup[ EITEM_NAME ].m_pkItemInfoGroup );

    // 설명
    LINK_CONTROL( "item_information_info", m_stItemInfoGroup[ EITEM_INFO ].m_pkItemInfoGroup );

    // 해체아이템
    LINK_CONTROL( "item_information_breakup_1", m_stItemInfoGroup[ EITEM_BREAKUP1 ].m_pkItemInfoGroup );
    LINK_CONTROL( "item_information_breakup_2", m_stItemInfoGroup[ EITEM_BREAKUP2 ].m_pkItemInfoGroup );
    LINK_CONTROL( "item_information_breakup_3", m_stItemInfoGroup[ EITEM_BREAKUP3 ].m_pkItemInfoGroup );

    // 스크립트 로드
    //LoadScript( "InitBreakUp.lua" ); /// 서버에서 받음
#endif
}

KGCItemInformationBreakupBox::~KGCItemInformationBreakupBox( void )
{
    ReleaseItemTexture();
}

void KGCItemInformationBreakupBox::OnCreate( void )
{
#if defined( ITEM_BREAKUP_INFO_BOX )
    // UI연결
    m_pkName = static_cast<KD3DStatic*>( m_stItemInfoGroup[EITEM_NAME].m_pkItemInfoGroup->GetChildByName("item_information_name") );
    m_pkCost = static_cast<KD3DStatic*>( m_stItemInfoGroup[EITEM_INFO].m_pkItemInfoGroup->GetChildByName("item_information_cost") );
    m_pkInfo = static_cast<KD3DStatic*>( m_stItemInfoGroup[EITEM_INFO].m_pkItemInfoGroup->GetChildByName("item_information_info") );
    m_stItemInfoGroup[ EITEM_NAME ].m_pkBackGround = static_cast<KD3DSizingBox*>( m_stItemInfoGroup[ EITEM_NAME ].m_pkItemInfoGroup->GetChildByName("item_information_background") );
    m_stItemInfoGroup[ EITEM_INFO ].m_pkBackGround = static_cast<KD3DSizingBox*>( m_stItemInfoGroup[ EITEM_INFO ].m_pkItemInfoGroup->GetChildByName("item_information_background") );

    for(int iLoop=EITEM_BREAKUP1; iLoop<EITEM_MAX; ++iLoop)
    {
        m_pkBreakUpItem[ (iLoop-EITEM_BREAKUP1) ] = static_cast<KD3DStatic*>( m_stItemInfoGroup[iLoop].m_pkItemInfoGroup->GetChildByName("item_information_breakup") );
        m_pkMyItem[ (iLoop-EITEM_BREAKUP1) ] = static_cast<KD3DStatic*>( m_stItemInfoGroup[iLoop].m_pkItemInfoGroup->GetChildByName("item_information_my") );
        m_pkBreakUpItem[ (iLoop-EITEM_BREAKUP1) ]->SetLineSpace( 1.3f );
        m_pkBreakUpItem[ (iLoop-EITEM_BREAKUP1) ]->SetAlign( DT_LEFT );
        m_pkMyItem[ (iLoop-EITEM_BREAKUP1) ]->SetLineSpace( 1.3f );
        m_pkMyItem[ (iLoop-EITEM_BREAKUP1) ]->SetAlign( DT_LEFT );
    }

    // String글꼴설정
    m_pkName->SetFontOutline( true );
    m_pkName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkName->SetLineSpace( 1.0f );
    m_pkName->SetAlign( DT_CENTER );
    m_pkCost->SetLineSpace(1.3f);
    m_pkCost->SetAlign( DT_LEFT );
    m_pkInfo->SetLineSpace(1.3f);
    m_pkInfo->SetAlign( DT_LEFT );

    // UI간격설정
    m_stItemInfoGroup[ EITEM_NAME ].m_dwExtraPlus = 2;
    m_stItemInfoGroup[ EITEM_INFO ].m_dwExtraPlus = 5;
    m_stItemInfoGroup[ EITEM_BREAKUP1 ].m_dwExtraPlus = 1;
    m_stItemInfoGroup[ EITEM_BREAKUP2 ].m_dwExtraPlus = 1;
    m_stItemInfoGroup[ EITEM_BREAKUP3 ].m_dwExtraPlus = 5;

    // Static크기
    for(int iLoop=0; iLoop<EITEM_MAX; ++iLoop)
    {
        m_stItemInfoGroup[ iLoop ].m_dwExtraSize = 2;
    }

    // 텍스쳐로드
    //for( unsigned int iLoop=0; iLoop<m_vecBreakupGoosID.size(); ++iLoop)
    //{
    //        m_vecpTexBreakUpItem.push_back( g_pItemMgr->CreateShopItemTexture( m_vecBreakupGoosID[ iLoop ] / 10 ) );
    //}
    
#endif
}

void KGCItemInformationBreakupBox::FrameMoveInEnabledState( void ){}
void KGCItemInformationBreakupBox::ActionPerformed( const KActionEvent& event ){}

void KGCItemInformationBreakupBox::PostChildDraw()
{
#if defined( ITEM_BREAKUP_INFO_BOX )
    D3DXVECTOR2 vPos, vScale;
    vPos = D3DXVECTOR2(0.0, 0.0f);
    vScale = D3DXVECTOR2(50.0f, 50.0f);
    int nIndex = 0;

    for( std::vector<GCDeviceTexture*>::iterator it = m_vecpTexBreakUpItem.begin(); it != m_vecpTexBreakUpItem.end(); ++it ){
        if( (*it) != NULL ){
            vPos = m_stItemInfoGroup[ nIndex + EITEM_BREAKUP1 ].m_pkItemInfoGroup->GetCurrentWindowPos();
            vPos.x -= 2.0f; vPos.y += 8.0f;
            g_pItemMgr->Draw_ShopImage( vPos, vScale.x, vScale.y, (*it), false);
            ++nIndex;
        }
    }
#endif
}

bool KGCItemInformationBreakupBox::SetInfoString( GCItem* pkItem, GCITEMUID itemUID_ )
{
    if( NULL == pkItem ) return false;
    m_pkItem = pkItem;

    std::wstring str1;
    GCItem* pGCItem = NULL;
    KItem* pInven = NULL;
    if( !g_pItemMgr->FindInventory( itemUID_, &pInven, &pGCItem ) )
        return false;

    // 아이템 제목
    m_pkName->SetText( MakeItemNameString() );

    // 설명
    int iPrice = 0;// g_pkUIScene->m_pkItemBreakUpDlg->GetBreakCost( pInven->m_ItemUID );
    str1 = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC5, "i", iPrice );
    m_pkCost->SetText( str1 );
    str1 = g_pkStrLoader->GetString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC6 );
    m_pkInfo->SetText( str1 );

    m_pkBreakUpItem[MATERIAL_1]->SetText( L"" );
    m_pkBreakUpItem[MATERIAL_2]->SetText( L"" );
    m_pkBreakUpItem[MATERIAL_3]->SetText( L"" );

    // 해체 아이템 정보 Renew
    ReleaseItemTexture();
    PAIR_INT pairItemInfo;        // 지금 분해하려는 아이템 정보(레벨, 등급 or ID, 등급)
    // 분해 결과물이 정의된 아이템인지 부터 확인
    pairItemInfo.first = static_cast<int>( pGCItem->dwGoodsID * 10 );
    pairItemInfo.second = static_cast<int>( pInven->m_cGradeID );

    if( SiGCForgeManager()->IsSpecificBreakupItem( pairItemInfo ) ){
        // 분해 결과물이 정의된 아이템이면 정보를 받아오자
        std::vector< KDropItemInfo > vecItemInfo;
        SiGCForgeManager()->GetSpecificBreakupItem( pairItemInfo, vecItemInfo );

        // 아이템 정보 세팅
        int nIndex = 0;
        for( std::vector< KDropItemInfo >::iterator it =  vecItemInfo.begin(); it != vecItemInfo.end(); ++it ){
            if( nIndex < MATERIAL_MAX ){
                // 아이템 이미지
                m_vecpTexBreakUpItem.push_back( g_pItemMgr->CreateShopItemTexture( (*it).m_ItemID / 10 ) );

                // 수량
                str1 = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC9, "li", g_pItemMgr->GetItemName( (*it).m_ItemID / 10 ), (*it).m_nDuration );
                m_pkBreakUpItem[nIndex++]->SetText( str1 );
            }
        }
    } else { // 레벨 등급별 해체
        // 해체 아이템정보
        pairItemInfo.first = pGCItem->iNeedLevelForEquippingItem;
        pairItemInfo.second = static_cast<int>( pInven->m_cGradeID );

        // 레벨 등급별 결과물을 받아오자
        std::map< GCITEMID, PAIR_INT > mapResultItemInfo;

        mapResultItemInfo.clear();
        SiGCForgeManager()->GetBreakupItem( pairItemInfo, mapResultItemInfo );

        if( !mapResultItemInfo.empty() ){
            int nIndex = 0;
            for( std::map< GCITEMID, PAIR_INT >::iterator it = mapResultItemInfo.begin(); it != mapResultItemInfo.end(); ++it ){
                if( nIndex < MATERIAL_MAX ){
                    // 아이템 이미지
                    m_vecpTexBreakUpItem.push_back( g_pItemMgr->CreateShopItemTexture( (*it).first / 10 ) );

                    // 수량
                    str1 = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC7, "lii", g_pItemMgr->GetItemName( (*it).first / 10 ), (*it).second.first, (*it).second.second );
                    m_pkBreakUpItem[nIndex++]->SetText( str1 );
                }
            }
        }
    }

    // 윈도우 높이 설정
    SetHeightGroup( EITEM_NAME, m_pkName);
    SetHeightGroup( EITEM_INFO, m_pkCost);
    SetHeightGroup( EITEM_INFO, m_pkInfo);

    return true;
}

std::wstring KGCItemInformationBreakupBox::MakeItemNameString()
{
    wstringstream strm;
    if( NULL == m_pkItem )  return strm.str();

    DWORD dwCommonCharType = 0; 
    for( int i = 0 ; i < GC_CHAR_NUM; ++i)
    {
        dwCommonCharType = dwCommonCharType | ( 1 << i );
    }

    std::wstring strItemCharType = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );
    if(dwCommonCharType != m_pkItem->dwCharType )
        strItemCharType = GCFUNC::GetCharName( m_pkItem->GetCharType() );

    strm << g_pItemMgr->GetItemGradeColorKey( m_pkItem->cItemGrade ) << g_pItemMgr->GetItemName( m_pkItem->dwGoodsID ) << L"(" << strItemCharType << L")" << L"#cX";
    return strm.str();
}

void KGCItemInformationBreakupBox::SetUIPosition()
{
    DWORD dwStarPosY = 10;
    DWORD dwCenterBackgroundStartY = 10;

    // 벡그라운드 위치설정
    if( m_stItemInfoGroup[EITEM_NAME].m_pkItemInfoGroup->IsRenderOn() )
    {
        dwCenterBackgroundStartY += m_stItemInfoGroup[EITEM_NAME].GetHeight() + 4;
        m_pkCenterBackground->SetWindowPosDirect( D3DXVECTOR2( m_pkCenterBackground->GetFixedWindowLocalPos().x, static_cast<float>(dwCenterBackgroundStartY ) ) );
    }

    // 각 정보 레이아웃 위치설정
    for( int iLoop=0; iLoop<EITEM_MAX; ++iLoop)
    {
        if( m_stItemInfoGroup[iLoop].m_pkItemInfoGroup->IsRenderOn() )
        {
            m_stItemInfoGroup[ iLoop ].m_pkItemInfoGroup->SetWindowPosDirect( D3DXVECTOR2( m_stItemInfoGroup[iLoop].m_pkItemInfoGroup->GetFixedWindowLocalPos().x, static_cast< float >(dwStarPosY) ) );
            dwStarPosY += m_stItemInfoGroup[iLoop].GetHeight() + m_stItemInfoGroup[iLoop].m_dwExtraPlus;
        }
    }
    m_stItemInfoGroup[ EITEM_NAME ].m_pkBackGround->SetAllWindowAlpha( 100 );
    m_stItemInfoGroup[ EITEM_INFO ].m_pkBackGround->SetAllWindowAlpha( 100 );

    dwStarPosY += 21;
    m_pkCenterBackground->SetHeight( dwStarPosY - dwCenterBackgroundStartY - 13);
    m_pkCenterBackground->SetAllWindowAlpha( 190 );
    m_pkBackground->SetHeight( dwStarPosY );
    m_pkBackground->SetAllWindowAlpha( 170 );
    SetHeightDirect( dwStarPosY );
}

void KGCItemInformationBreakupBox::SetHeightGroup( GROUPENUM eItemInfoGroup_, KD3DStatic* psStanDardStatic_)
{
    if( eItemInfoGroup_< 0 || EITEM_MAX <= eItemInfoGroup_ )
        return;

    DWORD dwNewHeight = static_cast<DWORD>( psStanDardStatic_->GetFixedWindowLocalPos().y ) + static_cast<DWORD>( PlusPosY( psStanDardStatic_ ) ) + m_stItemInfoGroup[ eItemInfoGroup_ ].m_dwExtraSize;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkBackGround->SetHeight( dwNewHeight );
    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetHeightDirect( dwNewHeight );
}

void KGCItemInformationBreakupBox::SetPositionGroup( GROUPENUM eItemInfoGroup_, D3DXVECTOR2 vNewPos_ )
{
    if( eItemInfoGroup_< 0 || EITEM_MAX <= eItemInfoGroup_ )
        return;

    m_stItemInfoGroup[ eItemInfoGroup_ ].m_pkItemInfoGroup->SetWindowPosDirect( vNewPos_ );
}

int KGCItemInformationBreakupBox::PlusPosY( KD3DStatic* pkStatic_ )
{
    int iSpaceSize =  static_cast<int>( pkStatic_->GetLineSpace() * static_cast<float>( pkStatic_->GetFontMgr()->GetHeight() ) );
    return pkStatic_->GetLineNum() * iSpaceSize;
}

bool KGCItemInformationBreakupBox::LoadScript( std::string strFileName )
{
    KLuaManager luaMgr;
    if( !GCFUNC::LoadLuaScript( luaMgr, strFileName, false ) )
    {  return false;   }

    // MaterialItemID
    LUA_BEGIN_TABLE( "MaterialItemID", return false )
    {
        int itemID;
        for(int iLoop=1; ; ++iLoop)
        {
            LUA_GET_VALUE_NOASSERT( iLoop, itemID, break );
            m_vecBreakupGoosID.push_back(itemID);
        }
    }
    LUA_END_TABLE( return false );

    float fMaterialRatioMin = 0.0f;
    float fMaterialRatioMax = 0.0f;
    LUA_GET_VALUE( "MaterialRatioMin", fMaterialRatioMin, return false );
    LUA_GET_VALUE( "MaterialRatioMax", fMaterialRatioMax, return false );

    // MaterialCountRatio => Lv, Grade, 
    LUA_BEGIN_TABLE( "MaterialCountRatio", return false )
    {
        int iGrade, iLevel;
        BreakUpItemInfo pBreakItemInfo;

        for(int iLoop=0; ; ++iLoop)
        {
            LUA_BEGIN_TABLE( iLoop+1, break );
            {
                LUA_GET_VALUE( 1, iLevel, return false );
                LUA_GET_VALUE( 2, iGrade, return false );

                int j = 3;
                int iIndex = 0;
                for ( ; ; ++j )
                {
                    int count;
                    LUA_GET_VALUE_NOASSERT( j, count, break; );
                    if ( count > -1 )
                    {
                        int iMin = (int)(ceilf(count * fMaterialRatioMin));
                        if ( count > 0 && iMin < 1 )
                            iMin = 1;
                        int iMax = (int)(ceilf(count * fMaterialRatioMax));

                        pBreakItemInfo.m_iMaterial_min[iIndex].first = j - 3;
                        pBreakItemInfo.m_iMaterial_min[iIndex].second = iMin;
                        pBreakItemInfo.m_iMaterial_max[iIndex].first = j - 3;
                        pBreakItemInfo.m_iMaterial_max[iIndex].second = iMax;
                        iIndex++;
                    }
                }

                m_mapBreakupInfo[ iGrade ].insert( std::make_pair(iLevel, pBreakItemInfo) );
            }
            LUA_END_TABLE( break );
        }
    }
    LUA_END_TABLE( return false );

    return true;
}

void KGCItemInformationBreakupBox::ReleaseItemTexture()
{
    for(unsigned int iLoop=0; iLoop<m_vecpTexBreakUpItem.size(); ++iLoop)
    {
        if( NULL != m_vecpTexBreakUpItem[ iLoop ] )
        {
            SAFE_RELEASE( m_vecpTexBreakUpItem[ iLoop ] );
        }
    }

    m_vecpTexBreakUpItem.clear();
}