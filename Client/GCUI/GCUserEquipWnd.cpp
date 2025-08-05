#include "stdafx.h"
//

//

//
//
//
//
#include "../MyD3D.h"



//



#include "GCUserEquipWnd.h"
#include "GCMyInfoScene.h"
#include "KGCEmoticon.h"
#include "GCUINumber.h"
#include "GCItemImgWnd.h"
//

IMPLEMENT_CLASSNAME( KGCUserEquipWnd );
IMPLEMENT_WND_FACTORY( KGCUserEquipWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCUserEquipWnd, "gc_user_equip_wnd" );

KGCUserEquipWnd::KGCUserEquipWnd( void ) : m_kItemInfo(NULL), m_dwUID( 0 )
{
    m_bShow             = false;
    m_bMouseHoverPrev   = false;
    m_bAlreadyPatternUp = true;
    m_bEnchant          = false;
    m_bEnchantBreak     = false;
    m_bNotSaleForLevel  = false;
    m_bEvent            = false;
    m_bPeriodItem       = false;
    m_bShortCut         = false;
    m_bEmoticon         = false;
    m_bSelect           = false;
    m_iMyIndex          = -1;

    m_pkItemName    = NULL;
    m_pkRemainTime  = NULL;
    m_pTextureItem  = NULL;

    m_pkActiveFrame = NULL;
    m_pkItemImg = NULL;
    m_pkDuration = NULL;
    m_pkStrong = NULL;
    m_pkEquipEnchantItemLv = NULL;

    m_pkImgCoordiCompose = NULL;

    LINK_CONTROL( "item_active_frame",                  m_pkActiveFrame );

    LINK_CONTROL( "item_name_static",                   m_pkItemName );
    LINK_CONTROL( "remain_time",                        m_pkRemainTime );
    LINK_CONTROL( "item_img",                           m_pkItemImg );
    LINK_CONTROL( "num_duration",                       m_pkDuration );
    LINK_CONTROL( "num_strong",                         m_pkStrong );

    LINK_CONTROL( "number_equip_enchant_item",          m_pkEquipEnchantItemLv );
    LINK_CONTROL( "img_coordi_compose",                 m_pkImgCoordiCompose );

	m_pkImgDrawRect = NULL;
	LINK_CONTROL( "img_draw_rect",                         m_pkImgDrawRect );
}

KGCUserEquipWnd::~KGCUserEquipWnd( void )
{
    // empty
    SAFE_RELEASE(m_pTextureItem);
}

void KGCUserEquipWnd::OnCreate( void )
{
    if( m_pkActiveFrame != NULL ) {
        m_pkActiveFrame->InitState( false );
    }
    if ( m_pkItemName ) 
    {
    m_pkItemName->SetSelfInputCheck( false );
    m_pkItemName->SetOffset( D3DXVECTOR2( 0.0f, 1.0f ) );
	m_pkItemName->SetWordWrap( true );
	

#if defined(NO_SHOW_ITEM_NAME)
    m_pkItemName->ToggleRender(false);
#endif
    }

    if( m_pkItemImg != NULL ) {
        m_pkItemImg->InitState( true );
    }

    if( m_pkEquipEnchantItemLv != NULL ) {
        m_pkEquipEnchantItemLv->InitState( true );
        m_pkEquipEnchantItemLv->SetStringType( "normal_num_enchant" );
        m_pkEquipEnchantItemLv->SetWriteToRight(false);
	}

	m_pkImgDrawRect->InitState(false);

    if ( m_pkImgCoordiCompose )
        m_pkImgCoordiCompose->InitState( false );
}

void KGCUserEquipWnd::FrameMoveInEnabledState( void )
{
    POINT   pt = g_pkInput->GetMousePos();
    bool    bMouseHover = CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );

    if( m_pkActiveFrame != NULL ) {
        m_pkActiveFrame->SetWndMode( m_eMode );
        m_pkActiveFrame->ToggleRender( bMouseHover || m_bSelect );
    }

    if( m_bPeriodItem ) {
        // 실시간으로 시간의 변화를 보여주기 위함.
        // Render 하는 곳에 있던 것을 FrameMove로 옮김.
        KItem* pInvenItem = g_pItemMgr->GetInventoryItem( m_dwUID );
        if( pInvenItem != NULL ) {
            m_strRemainTime = g_pItemMgr->GetRemainTime( *pInvenItem );

            if ( m_pkRemainTime )
            m_pkRemainTime->SetText( m_strRemainTime );
        }
    }
}

void KGCUserEquipWnd::PostDraw( void )
{
    KItem* pInvenItem = NULL;

    if ( m_bShow && m_iItemID > -1 )
    {	
		D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
		vPos.x += m_pkImgDrawRect->GetFixedWindowLocalPos().x ;
		vPos.y += m_pkImgDrawRect->GetFixedWindowLocalPos().y ;

		float fInterpolationNumX = GC_SCREEN_INTERPOLATION_WIDTH;
		float fInterpolationNumY = GC_SCREEN_INTERPOLATION_HEIGHT;

        // 샵 아이템 이미지를 그린다.
        if( m_pkItemImg == NULL ) {
            g_pGCDeviceManager2->PushState();
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
            g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			//if( g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_SHORTCUT_SLOT || 
   //             g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_EQUIP2 )
            {
#if !defined(USE_HERO_DUNGEON)
                if( g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_SHORTCUT_SLOT )
#else
                if( g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_SHORTCUT_SLOT ||
                    g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_EQUIP2 )
#endif
                {
                    KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_iItemID );
                    if ( NULL != kTmpEmoticon && !g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( m_iItemID ) )
                    {
                        // 이모티콘 데이터라면!!!!
                        kTmpEmoticon->RenderForShop( vPos.x , vPos.y , (vPos.x + m_pkImgDrawRect->GetWidth() * m_fWindowScaleX) , (vPos.y+ m_pkImgDrawRect->GetHeight() * m_fWindowScaleY));
                        m_bEmoticon = true;
                    }
                    else
                    {
                        g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
													 vPos.x, vPos.y, (vPos.x + m_pkImgDrawRect->GetWidth()* m_fWindowScaleX), (vPos.y+ m_pkImgDrawRect->GetHeight()* m_fWindowScaleY) , 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);		
                        m_bEmoticon = false;
                    }
                }
                else
                {

                    g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
													vPos.x, vPos.y, (vPos.x + m_pkImgDrawRect->GetWidth()* m_fWindowScaleX), (vPos.y+ m_pkImgDrawRect->GetHeight()* m_fWindowScaleY), 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
                }
            }
            g_pGCDeviceManager2->PopState();
        }

        if( m_pkDuration == NULL ) {
            int iDuration = -1;
            // 인벤토리에 있는 수량 제한 아이템이라면 남은 수량을 표시한다.
            if( m_bShortCut == true )
            {
                if ( m_bEmoticon && !g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( m_iItemID ) )
                    iDuration = g_pItemMgr->GetVolumeInventoryItem( m_iItemID );
                else
                    iDuration = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(m_iMyIndex);
            }
            else
                iDuration = m_iDuration;

            if ( iDuration > -1 )
            {
                g_pGCDeviceManager2->PushState();
                g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
                g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
				D3DXVECTOR2 vNumPos = vPos;
				vNumPos.y += m_pkImgDrawRect->GetHeight() * m_fWindowScaleY;
				//g_pkUIMgr->GetNumInfor().find("small_num");
				vNumPos.y -= 18;

                if( m_kItemInfo->ePetItemKind == EPET_KIND_PET || m_kItemInfo->ePetItemKind == EPET_KIND_FOOD || m_kItemInfo->ePetItemKind == EPET_KIND_EQUIP_ITEM )
                    vNumPos.x -= 28;
                g_pkUIMgr->RenderNumber( "small_num", vNumPos , iDuration, true, false, -1.0f,true );
                g_pGCDeviceManager2->PopState();
            }
        }

        // 인벤토리에 있는 유니크아이디 표시 아이템이라면
        if ( m_bUnique )
        { 
			D3DXVECTOR2 vNumPos = vPos;
			vNumPos.y += m_pkImgDrawRect->GetHeight() * m_fWindowScaleY;
			//g_pkUIMgr->GetNumInfor().find("small_num");
			vNumPos.y -= 18;
			g_pkUIMgr->RenderNumber( "big_num", vNumPos,static_cast<DWORD>(m_dwUID), true, false,-1.0f,true );
        }

        // 강화레벨이 있는 아이템이면 강화표시를 한다
//         float xPos = 95.0f;
//         float yPos = 51.0f;
//         if( g_pkUIScene->m_pkMyInfoScene->m_pkUserEquipBox->GetCurrentCategory() == UEC_EQUIP2 )
//         {
//             xPos = 75.0f;
//             yPos = 80.0f;
//         }
		D3DXVECTOR2 vNumPos = vPos;
		vNumPos.x += m_pkImgDrawRect->GetWidth() * m_fWindowScaleX;
		vNumPos.x -= 12.0f;
		vNumPos.y += m_pkImgDrawRect->GetHeight() * m_fWindowScaleY;
		//g_pkUIMgr->GetNumInfor().find("small_num");
		vNumPos.y -= 16;

        if( m_kItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM ){
        if( m_pkStrong == NULL ) {
            if (m_kItemInfo->iStrongLevel != 0 || m_kItemInfo->eItemKind == GIK_STRONG_ITEM )
            {
                g_pkUIMgr->RenderNumber( "yellow_num", vNumPos,
                    m_iStrong, true, false ,-1.0f,true);
            }
        }
    }
}
}

void KGCUserEquipWnd::ActionPerformed( const KActionEvent& event )
{
}

KItem* KGCUserEquipWnd::GetItemData( void )
{
    KItem* pInvenItem = NULL;

    g_pItemMgr->FindInventory(m_dwUID , &pInvenItem);

    return pInvenItem;
}

void KGCUserEquipWnd::ShowItem( bool bShow )
{
    m_bShow = bShow;

    if ( m_pkItemName )
    {
    m_pkItemName->ToggleRender( bShow );
    }

    if ( m_pkRemainTime )
    {
    if( !m_bPeriodItem )
        m_pkRemainTime->ToggleRender( false );
    else
        m_pkRemainTime->ToggleRender( bShow );
    }

    if( m_pkEquipEnchantItemLv && !bShow )
        m_pkEquipEnchantItemLv->ToggleRender(false);

    if ( m_pkImgCoordiCompose && !bShow )
        m_pkImgCoordiCompose->ToggleRender( false );
}

void KGCUserEquipWnd::SetItem()
{
    SAFE_RELEASE( m_pTextureItem );
    m_iItemID = -1;
    m_dwUID = 0;
    m_bPeriodItem = false;
    ShowItem( false );
    m_kItemInfo = NULL;

    if( m_pkItemImg != NULL )
        m_pkItemImg->ToggleRender( false );
    if( m_pkDuration != NULL )
        m_pkDuration->ToggleRender( false );
    if( m_pkStrong != NULL )
        m_pkStrong->ToggleRender( false );
    if( m_pkEquipEnchantItemLv != NULL )
        m_pkEquipEnchantItemLv->ToggleRender( false );

    if( m_pkImgCoordiCompose != NULL )
        m_pkImgCoordiCompose->ToggleRender( false );
}

void KGCUserEquipWnd::SetItem(KItem*& pInventoryItem)
{
    SAFE_RELEASE( m_pTextureItem );
    if(pInventoryItem == NULL)
    {
        m_iItemID = -1;
        ShowItem( false );
        m_dwUID = 0;
        return;
    }

    if( g_MyD3D->m_kItemMgr.IsBaseItem(pInventoryItem->m_ItemID) )
    {
        m_iItemID = -1;
        ShowItem(false);
        m_dwUID = 0;
        return;
    }

    m_kItemInfo = g_MyD3D->m_kItemMgr.GetItemData(pInventoryItem->m_ItemID);
    if( m_kItemInfo == NULL )
    {
        m_iItemID = -1;
        ShowItem(false);
        m_dwUID = 0;
        return;
    }

    if( m_pkEquipEnchantItemLv != NULL )
    {
        if( pInventoryItem->m_EnchantEquipItemUID != 0 )
        {
            m_pkEquipEnchantItemLv->ToggleRender(true);

            if( m_kItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
            {
                if( pInventoryItem->m_cGradeID == KItem::GRADE_NORMAL )
                    m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                else if( pInventoryItem->m_cGradeID == KItem::GRADE_EPIC )
                    m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
            }
            else
            {
                if( pInventoryItem->m_cEnchantEquipGradeID == KItem::GRADE_NORMAL )
                    m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                else if( pInventoryItem->m_cEnchantEquipGradeID == KItem::GRADE_EPIC )
                    m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
            }

            m_pkEquipEnchantItemLv->SetNumber(pInventoryItem->m_cEnchantLevel);

        }
        else
        {
            m_pkEquipEnchantItemLv->ToggleRender(false);
        }
    }


    if( m_kItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM && pInventoryItem->m_EnchantEquipItemUID == 0 ){
    if( m_pkStrong != NULL ){
        if(m_kItemInfo->iStrongLevel != 0 || m_kItemInfo->eItemKind == GIK_STRONG_ITEM ) {
            m_pkStrong->SetNumber( m_kItemInfo->iStrongLevel );
            m_pkStrong->ToggleRender( true );
        }
        else {
            m_pkStrong->ToggleRender( false );
        }
    }
    m_iStrong = m_kItemInfo->iStrongLevel;
    }    


    if( m_pkDuration != NULL ) {
        if( pInventoryItem->m_nCount > -1 ) {
            m_pkDuration->SetNumber( pInventoryItem->m_nCount );
            m_pkDuration->ToggleRender( true );
        }
        else {
            m_pkDuration->ToggleRender( false );
        }
    }
    m_iDuration = pInventoryItem->m_nCount;

    m_iItemID = pInventoryItem->m_ItemID;
    m_dwUID = pInventoryItem->m_ItemUID;

    if ( m_pkItemName )
    {
		m_pkItemName->SetFontColor( g_MyD3D->m_kItemMgr.GetItemStringColor( m_kItemInfo->eItemKind ) );
		m_pkItemName->SetShadow( true );
		//m_pkItemName->SetFontOutline( true );
    }

    // 2007.04.11 wony
    // 펫 아이템일 경우 샵 이미지를 보여줄 때..
    // 진화 레벨에 따라서.. 다른 이미지를 보여준다..
    // ex) 블루페어리 SBTA3881.dds
    //     아이스픽시 SBTA3881_1.dds ( 1차진화 )
    if( m_kItemInfo->eItemKind == GIK_PET && m_kItemInfo->ePetItemKind == EPET_KIND_PET )
    {
        if( m_pkItemImg != NULL ) {
            std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( pInventoryItem->m_ItemUID );

            // 펫이지만 펫 정보가 없다면 일반 텍스쳐를 로딩한다
            if( mit == g_MyD3D->m_mapPetInfo.end() ) {
                m_pkItemImg->SetPetItemInfo( m_iItemID, 0 );
            }
            else {
                m_pkItemImg->SetPetItemInfo( m_iItemID, mit->second.m_cPromotion );
            }
        }
        else {
            std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( pInventoryItem->m_ItemUID );

            // 펫이지만 펫 정보가 없다면 일반 텍스쳐를 로딩한다
            if( mit == g_MyD3D->m_mapPetInfo.end() )
                m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID - 1, 0 );

            // 펫 정보가 있다면 펫의 진화레벨에 따라서 다른 이미지를 보여준다
            KPetInfo kPetInfo = mit->second;
            if( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwUID != 0 ) //g_kGlobalValue.IsPetCostumeItem( m_iItemID ) )
                m_pTextureItem = g_pItemMgr->CreatePetItemTexture( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID
                , g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_cPromotion );
            else
                m_pTextureItem = g_pItemMgr->CreatePetItemTexture( kPetInfo.m_dwID, kPetInfo.m_cPromotion );
        }
    }
    else
    {
        //Texture Loading
        if( m_pkItemImg != NULL ) {
            m_pkItemImg->SetItemInfo( m_iItemID, -1, false, -1, 0, -1, pInventoryItem->m_DesignCoordiID );
            m_pkItemImg->ToggleRender( true );
        }
        else {
            GCITEMID itemID = m_iItemID;
            if ( pInventoryItem->m_DesignCoordiID != 0 )
            {
                itemID = pInventoryItem->m_DesignCoordiID;
            }

            m_pTextureItem = g_pItemMgr->CreateItemTexture( itemID, 0 );
        }
    }

    m_bEnchant = m_kItemInfo->eItemKind == GIK_STRONG_ITEM;
    m_bUnique = m_kItemInfo->eItemKind == GIK_PRINT_UID;

    if ( m_pkRemainTime )
    {
    if( m_kItemInfo->eItemType == GCIT_PERIOD && pInventoryItem->m_nPeriod != -1 ) {
        m_bPeriodItem = true;
        m_strRemainTime = g_pItemMgr->GetRemainTime( *pInventoryItem );
        m_pkRemainTime->SetText( m_strRemainTime );
    }
    else {
        m_bPeriodItem = false;
        m_strRemainTime.clear();
        m_pkRemainTime->SetText( m_strRemainTime );
    }

    if( m_bPeriodItem )
        m_pkRemainTime->SetText( g_pItemMgr->GetRemainTime( *pInventoryItem ) );
    }

    WCHAR szItemName[MAX_PATH] = { 0, };
    std::size_t len = std::wcslen( m_kItemInfo->strItemName.c_str() );
    for ( std::size_t i = 0; i < len; ++i )
    {
        szItemName[i] = m_kItemInfo->strItemName[i];

        if ( g_pkFontMgr->GetWidth( szItemName ) > (int)( GetWidth() * m_fWindowScaleX) && i > 0 )
        {
            szItemName[i-1] = szItemName[i] = L'\0';
            break;
        }
    }

    if ( m_pkItemName )
    {
		m_pkItemName->SetFontColor(g_pItemMgr->GetItemGradeColorRGB( pInventoryItem->m_cGradeID ));
		m_pkItemName->SetForceWordWrapText( szItemName ,true );
    }

    // 펫일 경우에 펫 이름 표시 & 펫 레벨 표시
    if( g_MyD3D->IsExistPetInfo( pInventoryItem->m_ItemUID ) )
    {
        if( m_kItemInfo->eItemKind == GIK_PET && m_kItemInfo->ePetItemKind == EPET_KIND_PET )
        {
            KPetInfo& kPetinfo = g_MyD3D->m_mapPetInfo[ pInventoryItem->m_ItemUID ];

            int iLevel = GetPetLevel( kPetinfo.m_dwEXP );
            std::wostringstream strmName;
            strmName <<g_pkStrLoader->GetString( STR_ID_LEVEL ) << " " << iLevel << " " << kPetinfo.m_strName << L"#cX";
				//버그란다 전국가 적용시키자
//#if defined(NATION_USA) || defined(NATION_THAILAND) || defined(NATION_BRAZIL) || defined(NATION_KOREA) || defined(NATION_IDN)
//#else
//				szItemName << 
//#endif
				
            if ( m_pkItemName )
				m_pkItemName->SetForceWordWrapText( strmName.str() ,true);
        }
    }

    
    if ( m_pkImgCoordiCompose )
    {
        if ( pInventoryItem->m_DesignCoordiID != 0 )
        {
            m_pkImgCoordiCompose->ToggleRender( true );
        }
        else
        {
            m_pkImgCoordiCompose->ToggleRender( false );
        }

    }

    m_bShortCut = false;

    ShowItem(true);
}

void KGCUserEquipWnd::SetItem( int _index, bool _bMode )
{
    m_bShortCut = false;
    SAFE_RELEASE(m_pTextureItem);

    // Item인 경우 ItemID 확인
    if( _bMode == true )
    {
        if((m_iItemID = g_MyD3D->GetMyShortCutSlot()->GetItemID(_index)) == KItem::INVALID_ITEM )
        {
            ShowItem(false);
            return;
        }
    }
    // Emoticon인 경우 ItemID 확인
    else
    {
        if((m_iItemID = g_MyD3D->GetMyShortCutSlot()->GetEmotID(_index - 5)) == KItem::INVALID_ITEM )
        {
            ShowItem(false);
            return;
        }
    }

    // GCItem 구조체 받아오기
    m_kItemInfo = NULL;
	if(_bMode == true)
	{
		m_kItemInfo = g_MyD3D->m_kItemMgr.GetItemData( g_MyD3D->GetMyShortCutSlot()->GetItemID(_index) );
         m_dwUID = g_MyD3D->GetMyShortCutSlot()->GetItemUID( _index );
	}
	else
	{
		m_kItemInfo = g_MyD3D->m_kItemMgr.GetItemData( g_MyD3D->GetMyShortCutSlot()->GetEmotID(_index - 5) );
        m_dwUID = g_MyD3D->GetMyShortCutSlot()->GetEmotUID( _index - 5 );
	}
    if( m_kItemInfo == NULL )
    {
        SetItem();
        return;
    }

    m_iStrong = 0;
    if( m_pkStrong != NULL ) {
        m_pkStrong->ToggleRender( false );
    }

    if( m_pkDuration != NULL ) {

        if ( m_bEmoticon )
            m_pkDuration->SetNumber( g_pItemMgr->GetVolumeInventoryItem( m_iItemID ) );
        else
            m_pkDuration->SetNumber( g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(m_iMyIndex) );

        m_pkDuration->ToggleRender( true );
    }

    if ( m_pkItemName )
    {
    m_pkItemName->SetFontColor( g_MyD3D->m_kItemMgr.GetItemStringColor( GIK_SHORTCUT_SLOT ) );
    m_pkItemName->SetShadow( true );
    }

    m_bEnchant = false;
    m_bUnique = false;

    WCHAR szItemName[MAX_PATH] = { 0, };
    std::size_t len = std::wcslen( m_kItemInfo->strItemName.c_str() );
    for ( std::size_t i = 0; i < len; ++i )
    {
        szItemName[i] = m_kItemInfo->strItemName[i];

        if ( g_pkFontMgr->GetWidth( szItemName ) > (int)( GetWidth() ) && i > 0 )
        {
            szItemName[i-1] = szItemName[i] = L'\0';
            break;
        }
    }

    if ( m_pkItemName )
		m_pkItemName->SetForceWordWrapText( szItemName , true );

    if( m_pkItemImg == NULL ) {
        m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );
    }
    else {
        m_pkItemImg->SetItemInfo( m_iItemID );
    }

    m_bShortCut = true;
    m_iMyIndex = _index;

    ShowItem(true);
}
