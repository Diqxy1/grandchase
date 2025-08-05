#include "stdafx.h"
#include ".\GCCoordiShopScene.h"
#include "../MyD3D.h"

#include "../GCStateMachine.h"
#include "KMci.h"
#include "GCCoordiItemBox.h"
#include "KGCCoordiClearSealDlg.h"
#include "KGCBillboardChatBoard.h"

IMPLEMENT_CLASSNAME( KGCCoordiShopScene );
IMPLEMENT_WND_FACTORY( KGCCoordiShopScene );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiShopScene, "gc_coordi_shop_scene" );

KGCCoordiShopScene::KGCCoordiShopScene( void )
{
    m_bEquipHotCoordi = false;
    m_dwCurHotCoordiID = 0;
    m_pkHotCoordiItemBox = NULL;
    LINK_CONTROL( "hot_coordi_item", m_pkHotCoordiItemBox );

    m_pkSearchBar       = NULL;
    m_pkItemCombo       = NULL;
    m_pkSortCombo       = NULL;
    LINK_CONTROL( "search_bar",     m_pkSearchBar );
    LINK_CONTROL( "item_combobox",  m_pkItemCombo );
    LINK_CONTROL( "sort_combobox",  m_pkSortCombo );
    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        m_apkCoordiItemBox[i] = NULL;

        char szWndName[100] = {0, };
        sprintf_s( szWndName, 99, "coordi_item_box%d", i );
        LINK_CONTROL( szWndName, m_apkCoordiItemBox[i] );
    }

    m_eCurrentTab = COORDI_TAB_ARMOR;
    m_pkBtnArmorDefault = NULL;
    m_pkBtnAccessaryDefault = NULL;
    m_pkBtnPackageDefault = NULL;

    m_pkBtnArmorActive = NULL;
    m_pkBtnAccessaryActive = NULL;
    m_pkBtnPackageActive = NULL;

    LINK_CONTROL( "btn_armor_default",       m_pkBtnArmorDefault );
    LINK_CONTROL( "btn_accessary_default",   m_pkBtnAccessaryDefault );
    LINK_CONTROL( "btn_package_default",   m_pkBtnPackageDefault );

    
    LINK_CONTROL( "btn_armor_active",       m_pkBtnArmorActive );
    LINK_CONTROL( "btn_accessary_active",   m_pkBtnAccessaryActive );
    LINK_CONTROL( "btn_package_active",   m_pkBtnPackageActive );
    
    m_pkCoordiView = NULL;
    m_pkBtnCatalog = NULL;
    LINK_CONTROL( "coordi_char_view", m_pkCoordiView );
    LINK_CONTROL( "btn_catalog", m_pkBtnCatalog );

    m_pkStaticCurrentPage  = NULL;
    m_pkBtnPrevPage = NULL;
    m_pkBtnNextPage = NULL;
    m_pkBtnFirstPage= NULL;
    m_pkBtnEndPage  = NULL;
    m_pkBtnHotCoordiPrev = NULL;
    m_pkBtnHotCoordiNext = NULL;
    m_pkBtnViewPackageList = NULL;
    LINK_CONTROL( "static_current_page", m_pkStaticCurrentPage );
    LINK_CONTROL( "btn_prev_page",  m_pkBtnPrevPage );
    LINK_CONTROL( "btn_next_page",  m_pkBtnNextPage );
    LINK_CONTROL( "btn_first_page", m_pkBtnFirstPage );
    LINK_CONTROL( "btn_end_page",   m_pkBtnEndPage );
    LINK_CONTROL( "btn_hot_coordi_prev", m_pkBtnHotCoordiPrev );
    LINK_CONTROL( "btn_hot_coordi_next", m_pkBtnHotCoordiNext );
    LINK_CONTROL( "coordi_preview_button", m_pkBtnViewPackageList );

    m_iTotalPage = 0;
    m_iCurrentPage = 0;
    m_iTotalHotItem = 0;
    m_iCurrentHotItem = 0;

    for( int i = 0; i < NUM_BUTTON; ++ i )
        m_apkButton[i]  = NULL;

    m_iNPCIndex = 0;
    m_bNowEnter = true;
    m_bUserItemClick = false;
    m_pkStaticNPCSay = NULL;
    LINK_CONTROL( "static_npc_say", m_pkStaticNPCSay );
    LINK_CONTROL("worldbillboardchat", m_pkBillboardChatBoard);
}

KGCCoordiShopScene::~KGCCoordiShopScene( void )
{
}

void KGCCoordiShopScene::OnCreate( void )
{
    m_pkBillboardChatBoard->InitState(false, true, this);

    m_pkSearchBar->InitState( true, true, this );
    m_pkItemCombo->InitState( true, true, this );
    m_pkSortCombo->InitState( true, true, this );
    
    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
        m_apkCoordiItemBox[i]->InitState( true, true, this );

    m_pkHotCoordiItemBox->InitState( true, true, this );
    m_pkHotCoordiItemBox->SetBoxType( KGCCoordiItemBox::COORDI_ITEM_BOX_HOT );

    m_pkBtnArmorDefault->InitState( true, true, this );
    m_pkBtnAccessaryDefault->InitState( true, true, this );
    if(m_pkBtnPackageDefault)
        m_pkBtnPackageDefault->InitState(true,true,this);

    m_pkBtnArmorActive->InitState( false, true, this );
    m_pkBtnAccessaryActive->InitState( false, true, this );
    if(m_pkBtnPackageActive)
        m_pkBtnPackageActive->InitState( false, true, this );

    
    
    m_pkCoordiView->InitState( true, true, this );

#ifdef __PANHO__
    m_pkBtnCatalog->InitState( false );
#else
    m_pkBtnCatalog->InitState( true, true, this );
#endif
    
    m_pkStaticCurrentPage->InitState( true, false, this );
    m_pkStaticCurrentPage->SetAlign( DT_CENTER );
    m_pkBtnPrevPage->InitState( true, true, this );
    m_pkBtnNextPage->InitState( true, true, this );
    m_pkBtnFirstPage->InitState( true, true, this );
    m_pkBtnEndPage->InitState( true, true, this );
    m_pkBtnHotCoordiPrev->InitState( true, true, this );
    m_pkBtnHotCoordiNext->InitState( true, true, this );
    m_pkBtnViewPackageList->InitState( true, true, this );

    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_NEW ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_RECOM ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_NAME ) );

    m_pkStaticNPCSay->InitState( true, false, this );
    m_pkStaticNPCSay->SetAlign(  DT_LEFT );
    m_pkStaticNPCSay->SetMultiLine( true );

    KGCScene::OnCreate();
}

void KGCCoordiShopScene::OnCreateComplete( void )
{

}

void KGCCoordiShopScene::OnDestroy( void )
{

}

void KGCCoordiShopScene::FrameMoveInEnabledState( void )
{
    if( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        if ( false == g_pkUIScene->IsMsgBoxModal() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SHUTDOWN );
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        }
    }

    NPCFrameMove();
    KGCScene::FrameMoveInEnabledState();
    if( KD3DWnd::D3DWS_ENABLED != g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD ) ) { 
        DrawCoordiItemInfo();
    }
}

void KGCCoordiShopScene::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnArmorDefault && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetActiveTab( COORDI_TAB_ARMOR, true );

    if( event.m_pWnd == m_pkBtnAccessaryDefault && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetActiveTab( COORDI_TAB_ACCESSARY, true );

    if(m_pkBtnPackageDefault && event.m_pWnd == m_pkBtnPackageDefault && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetActiveTab( COORDI_TAB_PACKAGE, true );

    if( event.m_pWnd == m_pkBtnFirstPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetPage( PAGE_FIRST );

    if( event.m_pWnd == m_pkBtnPrevPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetPage( PAGE_PREV );

    if( event.m_pWnd == m_pkBtnNextPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetPage( PAGE_NEXT );

    if( event.m_pWnd == m_pkBtnEndPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetPage( PAGE_END );

    if( event.m_pWnd == m_pkBtnHotCoordiPrev && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetHotCoordiView( PAGE_PREV );

    if( event.m_pWnd == m_pkBtnHotCoordiNext && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        SetHotCoordiView( PAGE_NEXT );

    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        if( event.m_pWnd == m_apkCoordiItemBox[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            EquipCoordiItem( i );
            g_KDSound.Play( "31" );
        }
    }

    if( ( event.m_pWnd == m_pkItemCombo || event.m_pWnd == m_pkSortCombo )  && event.m_dwCode == KD3DWnd::D3DWE_COMBO_SELCHANGE )
        ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel() );

    GCWND_MSG_MAP( m_pkBtnCatalog,                  KD3DWnd::D3DWE_BUTTON_CLICK,    OnFashionCatalog );
    GCWND_MSG_MAP( m_pkCoordiView,                  KD3DWnd::D3DWE_BUTTON_CLICK,    OnCharacterChange );
    GCWND_MSG_MAP( m_pkHotCoordiItemBox,            KD3DWnd::D3DWE_BUTTON_CLICK,    EquipHotCooriItem );
    GCWND_MSG_MAP( m_pkBtnViewPackageList,          KD3DWnd::D3DWE_BUTTON_CLICK,    ViewHotCoordiPackageList );
    KGCScene::ActionPerformed( event );
}

void KGCCoordiShopScene::OnInitialize()
{
    SetActiveTab( COORDI_TAB_ARMOR, true );
    m_pkSearchBar->Init();

    m_pkHotCoordiItemBox->Clear();
    SetHotCoordiItemList();

    LoadCoordiShopInfo();
    
    if( g_pkUIScene->m_pkCoordiCatalogDlg )
        g_pkUIScene->m_pkCoordiCatalogDlg->SetCatalogInfo( m_vecCatalogTexture );

    InitPage();
    UpdateItemBox();

    if( g_KMci.GetTrack() != GC_BGM_COORDI_SHOP )
        g_KMci.Play( GC_BGM_COORDI_SHOP, true );
}

void KGCCoordiShopScene::OnLeaveScene( void )
{

}

void KGCCoordiShopScene::OnEnterScene( void )
{
#if defined ( GUILD_RENEW_S4 )
	if (g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
	{
		SetButtonRender( BTN_GUILD_MATCH, true );
		SetButtonRender( BTN_MATCH, false );
	}
	else
	{
		SetButtonRender( BTN_GUILD_MATCH, false );
		SetButtonRender( BTN_MATCH, true );
	}          
#endif


    m_vecEquipItem.clear();
    m_vecHotCoordiItem.clear();
    m_pkCoordiView->SetCoordiView( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
    m_bNowEnter = true;
    m_bUserItemClick = false;
    SetActiveTab( COORDI_TAB_ARMOR, true );
    SetHotCoordiItemList();
    InitPage();
    UpdateItemBox();

    if( !m_vecNPCInfo.empty() )
        NPCFrameMove();
}

void KGCCoordiShopScene::OnFashionCatalog( void )
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FASHION_CATALOG_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_pkCoordiView->GetCurrentChar() );
}

void KGCCoordiShopScene::ExitShop( void )
{   
    g_MyD3D->m_pStateMachine->GoPrev();
}

void KGCCoordiShopScene::SetComboList( bool bResetCategory, bool bResetSortType )
{
    if( bResetSortType )
    {
        m_pkSortCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_ITEMSORT ) );
        m_pkSortCombo->SetCurSel( -1 );
    }

    m_pkItemCombo->DeleteAllItem();
    m_pkItemCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_ITEM_CATEGORY ) );
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_TOTAL ) );

    switch( m_eCurrentTab )
    {
    case COORDI_TAB_ARMOR:
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1 ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2 ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HAND ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_MANTLE ) );
        break;

    case COORDI_TAB_ACCESSARY:
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_UP )  );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_DOWN ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_NECKLACE )  );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1_ACC ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2_ACC ) );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_ARM_ACC )  );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_RING )   );
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE_ACC )  );
        break;
    case COORDI_TAB_PACKAGE:
        m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_RANDOM_BOX )  );
        break;
    }
}

void KGCCoordiShopScene::InitPage( void )
{
    m_iTotalPage = 0;
    m_iCurrentPage = 0;

    int iItemCount = g_pItemMgr->GetNumCoordiItem();
    int iRemainCount = iItemCount % MAX_COORDI_BOX;
    
    if( iRemainCount != 0 )
        m_iTotalPage = ( iItemCount / MAX_COORDI_BOX ) + 1;
    else
        m_iTotalPage = ( iItemCount / MAX_COORDI_BOX );

    m_iCurrentPage = 1;


	WCHAR temp[20] = {0,};
	swprintf( temp, 10, L"%2d / %2d", m_iCurrentPage , m_iTotalPage);
	m_pkStaticCurrentPage->SetText( temp );

    m_pkBtnFirstPage->SetWndMode( D3DWM_LOCK );
    m_pkBtnPrevPage->SetWndMode( D3DWM_LOCK );

    if( m_iTotalPage == 1 )
    {
        m_pkBtnNextPage->SetWndMode( D3DWM_LOCK );
        m_pkBtnEndPage->SetWndMode( D3DWM_LOCK );
    }
    else
    {
        m_pkBtnNextPage->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnEndPage->SetWndMode( D3DWM_DEFAULT );
    }
}

void KGCCoordiShopScene::SetPage( EPageType ePageType_ )
{
    if( ePageType_ < PAGE_FIRST || PAGE_END < ePageType_  )
        return;

    switch( ePageType_ )
    {
    case PAGE_FIRST:
        if( m_iCurrentPage == m_iTotalPage )
        {
            m_pkBtnEndPage->SetWndMode( D3DWM_DEFAULT );
            m_pkBtnNextPage->SetWndMode( D3DWM_DEFAULT );
        }
        m_iCurrentPage = 1;
        m_pkBtnFirstPage->SetWndMode( D3DWM_LOCK );
        m_pkBtnPrevPage->SetWndMode( D3DWM_LOCK );
        break;

    case PAGE_PREV:
        if( 1 < m_iCurrentPage )
        {
            if( m_iCurrentPage == m_iTotalPage )
            {
                m_pkBtnEndPage->SetWndMode( D3DWM_DEFAULT );
                m_pkBtnNextPage->SetWndMode( D3DWM_DEFAULT );
            }
            --m_iCurrentPage;
            if( m_iCurrentPage == 1 )
            {
                m_pkBtnFirstPage->SetWndMode( D3DWM_LOCK );
                m_pkBtnPrevPage->SetWndMode( D3DWM_LOCK );
            }

        }
        break;

    case PAGE_NEXT:
        if( m_iCurrentPage < m_iTotalPage )
        {
            if( m_iCurrentPage == 1 )
            {
                m_pkBtnFirstPage->SetWndMode( D3DWM_DEFAULT );
                m_pkBtnPrevPage->SetWndMode( D3DWM_DEFAULT );
            }
            ++m_iCurrentPage;
            if( m_iCurrentPage == m_iTotalPage )
            {
                m_pkBtnEndPage->SetWndMode( D3DWM_LOCK );
                m_pkBtnNextPage->SetWndMode( D3DWM_LOCK );
            }
        }
        break;

    case PAGE_END:
        if( m_iCurrentPage == 1 )
        {
            m_pkBtnFirstPage->SetWndMode( D3DWM_DEFAULT );
            m_pkBtnPrevPage->SetWndMode( D3DWM_DEFAULT );
        }
        m_iCurrentPage = m_iTotalPage;
        m_pkBtnEndPage->SetWndMode( D3DWM_LOCK );
        m_pkBtnNextPage->SetWndMode( D3DWM_LOCK );
        break;
    }

    UpdateItemBox();

    // 입고 있는 아이템은 입고 있다고 표시 
    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        GCItem* pItem = m_apkCoordiItemBox[i]->GetItemData();
        if( IsEquipItem( pItem ) )
            m_apkCoordiItemBox[i]->SetEquip( true );
    }

	WCHAR temp[20] = {0,};
	swprintf( temp, 10, L"%2d / %2d", m_iCurrentPage , m_iTotalPage);
	m_pkStaticCurrentPage->SetText( temp );
}

void KGCCoordiShopScene::SetHotCoordiView( EPageType ePageType_ )
{
    if( ePageType_ == PAGE_FIRST || ePageType_ == PAGE_END )
        return;

    
    switch( ePageType_ )
    {
    case PAGE_PREV:
        if( m_iCurrentHotItem > 0 )
        {
            --m_iCurrentHotItem;
            m_pkHotCoordiItemBox->Clear();
            m_pkHotCoordiItemBox->SetBoxType( KGCCoordiItemBox::COORDI_ITEM_BOX_HOT );
            m_pkHotCoordiItemBox->SetItemInfo( m_vecHotCoordiItem[ m_iCurrentHotItem ] );
        }
        break;

    case PAGE_NEXT:
        if( m_iTotalHotItem  - 1 > m_iCurrentHotItem )
        {
            ++m_iCurrentHotItem;
            m_pkHotCoordiItemBox->Clear();
            m_pkHotCoordiItemBox->SetBoxType( KGCCoordiItemBox::COORDI_ITEM_BOX_HOT );
            m_pkHotCoordiItemBox->SetItemInfo( m_vecHotCoordiItem[ m_iCurrentHotItem ] );
        }
        break;
    }

    if (m_pkHotCoordiItemBox->GetItemData() == NULL)
        return;

    if( m_pkHotCoordiItemBox->GetItemData()->dwGoodsID == m_dwCurHotCoordiID )
        m_pkHotCoordiItemBox->SetEquip( true );
}

void KGCCoordiShopScene::SetActiveTab( ECoordiShopTab eTabType, bool bInit )
{
    m_eCurrentTab = eTabType;

    if( bInit )
        SetComboList( true, true );
    else
        SetComboList( false, false );

    switch( eTabType )
    {
    case COORDI_TAB_ARMOR:
        m_pkBtnArmorDefault->ToggleRender( false );
        m_pkBtnArmorActive->ToggleRender( true );
        m_pkBtnAccessaryDefault->ToggleRender( true );
        m_pkBtnAccessaryActive->ToggleRender( false );
        if(m_pkBtnPackageDefault)
            m_pkBtnPackageDefault->ToggleRender(true);
        if(m_pkBtnPackageActive)
            m_pkBtnPackageActive->ToggleRender( false );
        break;

    case COORDI_TAB_ACCESSARY:
        m_pkBtnArmorDefault->ToggleRender( true );
        m_pkBtnArmorActive->ToggleRender( false );
        m_pkBtnAccessaryDefault->ToggleRender( false );
        m_pkBtnAccessaryActive->ToggleRender( true );
        if(m_pkBtnPackageDefault)
            m_pkBtnPackageDefault->ToggleRender(true);
        if(m_pkBtnPackageActive)
            m_pkBtnPackageActive->ToggleRender( false );
        break;
    case COORDI_TAB_PACKAGE:
        m_pkBtnArmorDefault->ToggleRender( true );
        m_pkBtnArmorActive->ToggleRender( false );
        m_pkBtnAccessaryDefault->ToggleRender( true );
        m_pkBtnAccessaryActive->ToggleRender( false );
        if(m_pkBtnPackageDefault)
            m_pkBtnPackageDefault->ToggleRender( false );
        if(m_pkBtnPackageActive)
            m_pkBtnPackageActive->ToggleRender( true );
        break;
    }

    ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel() );
    InitPage();
    UpdateItemBox();
}

void KGCCoordiShopScene::ChangeSortType( int iCategory, int iSortType, const WCHAR* strName )
{
    int itemType;
    switch( m_eCurrentTab )
    {
    case COORDI_TAB_ARMOR:
        itemType = 0;
        break;

    case COORDI_TAB_ACCESSARY:
        itemType = 3;
        break;
    case COORDI_TAB_PACKAGE:
        itemType = 4;

        break;

    }

    g_pItemMgr->MakeCoordiShopItemList( static_cast<EGCCharType>( m_pkCoordiView->GetCurrentChar() ), itemType, iCategory, iSortType, strName );
    InitPage();
    UpdateItemBox();
}

void KGCCoordiShopScene::FindItemFromName( const WCHAR* strName )
{
    ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), strName );
}

void KGCCoordiShopScene::OnCharacterChange( void )
{
    m_vecEquipItem.clear();
    ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel() );
    SetHotCoordiItemList();
    InitPage();
    UpdateItemBox();
}

void KGCCoordiShopScene::DrawCoordiItemInfo( void )
{
    GCItem* pItem = NULL;
    D3DXVECTOR2 dxvPos;
    bool bHovered = false;

    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        if( m_apkCoordiItemBox[i]->IsHovered() )
        {
            //KLuaManager luaMgr;
            //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

            //float fX, fY;
            //LUA_GET_VALUE_DEF( "x", fX, 0.0f );
            //LUA_GET_VALUE_DEF( "y", fY, 0.0f );

            pItem  = m_apkCoordiItemBox[i]->GetItemData();
            dxvPos = m_apkCoordiItemBox[i]->GetRelocatedWindowPos();
            dxvPos.x += 100.0f;
            dxvPos.y += 50.0f;

            bHovered = true;
            break;
        }
    }

    if( bHovered && pItem )
    {
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else if (false == g_pkUIScene->GetClearSealDlg()->IsRenderOn()) {}
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}

void KGCCoordiShopScene::EquipCoordiItem(int iIndex_)
{
    GCItem* pItem = m_apkCoordiItemBox[iIndex_]->GetItemData();
    if(pItem == NULL) return;

    if(pItem->eItemKind == GIK_COORDIBOX)
    {
        EquipCooriItemBox(iIndex_);
    }
    else
    {
        EquipCoordiItemParts(iIndex_);
    }
}

void KGCCoordiShopScene::EquipCooriItemBox( int iIndex_ )
{
    GCItem* pBoxItem = m_apkCoordiItemBox[iIndex_]->GetItemData();

    std::vector<int> vecItemList = g_pItemMgr->GetSetItemList(pBoxItem->iSetItemNumber);

    PLAYER* ViewPlayer = m_pkCoordiView->GetPlayer();

    ViewPlayer->SetMyPlayer( -1, true  );

    bool bEquipedItem = false;

    m_bUserItemClick = true;
    bEquipedItem = true;

    for(std::vector<int>::iterator vIter = vecItemList.begin();vIter != vecItemList.end();vIter++)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(*vIter);
        if (pItem->dwSlotPosition <= 1){ 
            continue;
        }

        for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
        {
            if(pItem == *it)
            {
                m_bUserItemClick = false;
                bEquipedItem = false;
                break;
            }
        }

        if(!bEquipedItem) break;
    }

    m_vecEquipItem.clear();
    g_pItemMgr->UnequipItemAll( &( ViewPlayer->GetCurrentChar() ));

    if(bEquipedItem){
        for(std::vector<int>::iterator vIter = vecItemList.begin();vIter != vecItemList.end();vIter++)
        {
            GCItem* pItem = g_pItemMgr->GetItemData(*vIter);
            if (pItem->dwSlotPosition <= 1){ 
                continue;
            }

            m_vecEquipItem.push_back(pItem);
        }        
        for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
            g_pItemMgr->EquipItem( &( ViewPlayer->GetCurrentChar() ), *it, false, 0, true );
    }

    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        m_apkCoordiItemBox[i]->SetEquip( false );
    }

    m_apkCoordiItemBox[iIndex_]->SetEquip( bEquipedItem );

    m_pkHotCoordiItemBox->SetEquip( m_bEquipHotCoordi );
    m_pkCoordiView->SetPlayer( ViewPlayer );
    m_pkCoordiView->UpdateMeshObject();
}


void KGCCoordiShopScene::EquipCoordiItemParts( int iIndex_ )
{
    if( iIndex_ < 0 || MAX_COORDI_BOX <= iIndex_ )
        return;

    PLAYER* ViewPlayer = m_pkCoordiView->GetPlayer();
    //ViewPlayer->SetMyPlayer( m_pkCoordiView->GetCurrentChar(), true  );
    ViewPlayer->SetMyPlayer( -1, true  );

    GCItem* pItem = m_apkCoordiItemBox[iIndex_]->GetItemData();
    if( pItem == NULL ) 
        return;

    if( !m_apkCoordiItemBox[iIndex_]->IsEquip() )   // 장착 
    {
        g_pItemMgr->EquipItem( &( ViewPlayer->GetCurrentChar() ), pItem, false, 0, true );
        m_apkCoordiItemBox[iIndex_]->SetEquip( true );
    }
    else
        m_apkCoordiItemBox[iIndex_]->SetEquip( false );

   if( false == SetEquipData( pItem ) )
    {
        if( m_pkCoordiView->IsShowDefault() )
            g_pItemMgr->UnequipItemForItemID( &( ViewPlayer->GetCurrentChar() ), pItem->dwGoodsID );
        else
        {
            m_pkCoordiView->ResetPlayer();
            ViewPlayer = m_pkCoordiView->GetPlayer();
            //ViewPlayer->SetMyPlayer( -1, true  );
        }
    }   
    else
        m_bUserItemClick = true;

    for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
        g_pItemMgr->EquipItem( &( ViewPlayer->GetCurrentChar() ), *it, false, 0, true );

    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        GCItem* pBoxItem = m_apkCoordiItemBox[i]->GetItemData();
        if( IsEquipItem( pBoxItem ) )
            m_apkCoordiItemBox[i]->SetEquip( true );
        else
            m_apkCoordiItemBox[i]->SetEquip( false );
    }

    m_pkHotCoordiItemBox->SetEquip( m_bEquipHotCoordi );
    m_pkCoordiView->SetPlayer( ViewPlayer );
    m_pkCoordiView->UpdateMeshObject();
}

bool KGCCoordiShopScene::SetEquipData( GCItem* pItem )
{
    if( pItem == NULL )
        return false;

    bool bEquip = true;
    for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
    {
        if( (*it)->dwGoodsID == pItem->dwGoodsID )
        {
            // 장착해제 
            it = m_vecEquipItem.erase( it );
            bEquip = false;
            break;
        }

        if( (*it)->dwSlotPosition == pItem->dwSlotPosition )
        {
            // 핫 코디 아이템 패키지중 하나인지 
            if( IsHotCoordiPackageItem( (*it) ) )
            {
                m_bEquipHotCoordi = false;
                m_dwCurHotCoordiID = 0;
            }

            // 장착해재 
            if( IsEquipItem( pItem ) )
            {
                pItem->dwGoodsID = (*it)->dwGoodsID;
                bEquip = false;
            }
            it = m_vecEquipItem.erase( it );

            break;
        }
    }

    if( bEquip )
        m_vecEquipItem.push_back( pItem );

    return bEquip;
}

bool KGCCoordiShopScene::IsEquipItem( GCItem* pItem )
{
    if( pItem == NULL )
        return false;

    bool bEquip = false;
    for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
    {
        if( ( (*it)->dwSlotPosition == pItem->dwSlotPosition ) && 
            ( (*it)->dwCharType == pItem->dwCharType ) &&
            ( (*it)->strItemName == pItem->strItemName ) )
        {
            bEquip = true;
            break;
        }
    }
    
    return bEquip;
}

void KGCCoordiShopScene::LoadCoordiShopInfo( void )
{
    if( m_vecNPCInfo.empty() && m_vecCatalogTexture.empty() )
    {
        KLuaManager luaMgr;

        if( !GCFUNC::LoadLuaScript( luaMgr, "CoordiShopInfo.stg", 0 ) )
            return;

        LoadNPCInfo( luaMgr );
        LoadCatalogInfo( luaMgr );
    }
}

void KGCCoordiShopScene::LoadNPCInfo( KLuaManager& luaMgr )
{
    luaMgr.GetValue( "NPC_INDEX",  m_iNPCIndex );

    LUA_BEGIN_TABLE( "COORDI_NPC_INFO", return )
    {
        int iNPCNum = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iNPCNum ) ) ) break;
            {
                SCoordiShopNPCInfo tempInfo;

                // 인사 
                if( SUCCEEDED( luaMgr.BeginTable( "GREETING" ) ) )
                {
                    int iGreetingNum = 1;
                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iGreetingNum ) ) ) break;

                        std::pair< int, int > tempPair;
                        luaMgr.GetValue( 1, tempPair.first );
                        luaMgr.GetValue( 2, tempPair.second );
                        tempInfo.m_vecGreetingString.push_back( tempPair );

                        ++iGreetingNum;
                        luaMgr.EndTable(); // 인사 스트링 각 항목 
                    }
                    luaMgr.EndTable(); // 인사 스트링 전체 
                }

                // 랜덤
                if( SUCCEEDED( luaMgr.BeginTable( "RANDOM" ) ) )
                {
                    int iRandomNum = 1;
                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iRandomNum ) ) ) break;

                        std::pair< int, int > tempPair;
                        luaMgr.GetValue( 1, tempPair.first );
                        luaMgr.GetValue( 2, tempPair.second );
                        tempInfo.m_vecRandomString.push_back( tempPair );

                        ++iRandomNum;
                        luaMgr.EndTable(); // 랜덤 스트링 각 항목 
                    }
                    luaMgr.EndTable(); // 랜덤 스트링 전체 
                }

                // 클릭시 
                if( SUCCEEDED( luaMgr.BeginTable( "CLICK" ) ) )
                {
                    int iClickNum = 1;
                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iClickNum ) ) ) break;

                        std::pair< int, int > tempPair;
                        luaMgr.GetValue( 1, tempPair.first );
                        luaMgr.GetValue( 2, tempPair.second );
                        tempInfo.m_vecClickString.push_back( tempPair );

                        ++iClickNum;
                        luaMgr.EndTable(); // 클릭시 스트링 각 항목 
                    }
                    luaMgr.EndTable(); // 클릭시 스트링  
                }

                m_vecNPCInfo.push_back( tempInfo );
                ++iNPCNum;
            }
            luaMgr.EndTable(); // 각 NPC 정보 
        }
    }
    luaMgr.EndTable();  //COORDI_NPC_INFO
}

void KGCCoordiShopScene::LoadCatalogInfo( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE( "COORDI_CATALOG_INFO", return )
    {
        for( int i=1 ; ; ++i )
        {
            std::string texName;

            if( FAILED( luaMgr.GetValue(i, texName) ) )
                break;

            m_vecCatalogTexture.push_back( texName );
        }
    }
    luaMgr.EndTable();  // COORDI_CATALOG_INFO
}

void KGCCoordiShopScene::NPCFrameMove( void )
{
    static int iFrame = 0;

    if( m_bNowEnter )   // 샵에 막 들어왔다면 인사 
    {
        int iRate = 0;
        int iRand = rand() % 101;
        std::vector< std::pair<int, int> > tempVec = m_vecNPCInfo[m_iNPCIndex].m_vecGreetingString;

        for( std::vector< std::pair<int, int> >::iterator it = tempVec.begin() ; it != tempVec.end() ; ++it )
        {
            iRate += it->second;
            if( iRate >= iRand )
            {
                m_pkStaticNPCSay->SetTextAutoMultiline( g_pkStrLoader->GetString(it->first) );
                break;
            }
        }
        m_bNowEnter = false;
    }

    if( iFrame > 275 )  // 55 * 5 = 5초 
    {
        
        if( m_bUserItemClick ) // 물건 클릭시 대사 
        {
            int iRate = 0;
            int iRand = rand() % 101;
            std::vector< std::pair<int, int> > tempVec = m_vecNPCInfo[m_iNPCIndex].m_vecClickString;

            for( std::vector< std::pair<int, int> >::iterator it = tempVec.begin() ; it != tempVec.end() ; ++it )
            {
                iRate += it->second;
                if( iRate >= iRand )
                {
                    m_pkStaticNPCSay->SetTextAutoMultiline( g_pkStrLoader->GetString(it->first) );
                    break;
                }
            }
            m_bUserItemClick = false;
        }
        else     // 아무 행동 없다면 랜덤 대사 
        {
            int iRate = 0;
            int iRand = rand() % 101;
            std::vector< std::pair<int, int> > tempVec = m_vecNPCInfo[m_iNPCIndex].m_vecRandomString;

            for( std::vector< std::pair<int, int> >::iterator it = tempVec.begin() ; it != tempVec.end() ; ++it )
            {
                iRate += it->second;
                if( iRate >= iRand )
                {
                    m_pkStaticNPCSay->SetTextAutoMultiline( g_pkStrLoader->GetString(it->first) );
                    break;
                }
            }
        }
        iFrame = 0;
    }
    else
        ++iFrame;
}

void KGCCoordiShopScene::UpdateItemBox( void )
{
    // 아이템 창 다시 세팅 
    for(int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        m_apkCoordiItemBox[i]->Clear();

        int iIndex = ( m_iCurrentPage - 1 ) * MAX_COORDI_BOX + i ;

        GCItem* pOriginal = NULL;
        GCItem* pPackage7 = NULL;

        g_pItemMgr->GetCoordiItem( COORDI_SHOP_ORIGINAL, iIndex, pOriginal );
        g_pItemMgr->GetCoordiItem( COORDI_SHOP_7PACKAGE, iIndex, pPackage7 );
        KCoordiShopItem itemInfo( pOriginal, pPackage7 );
        m_apkCoordiItemBox[i]->SetItemInfo( itemInfo );
        
        for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
        {
            if(pOriginal&& (*it)->dwGoodsID == pOriginal->dwGoodsID )
                m_apkCoordiItemBox[i]->SetEquip( true );
            if(pPackage7&& (*it)->dwGoodsID == pPackage7->dwGoodsID )
                m_apkCoordiItemBox[i]->SetEquip( true );
        }
    }
}

void KGCCoordiShopScene::SetHotCoordiItemList( void )
{
    m_vecHotCoordiItem.clear();
    g_pItemMgr->MakeHotCoordiShopItemList( static_cast<EGCCharType>( m_pkCoordiView->GetCurrentChar() ), m_vecHotCoordiItem );

    m_pkHotCoordiItemBox->Clear();
    m_pkHotCoordiItemBox->SetBoxType( KGCCoordiItemBox::COORDI_ITEM_BOX_HOT );

    if(m_vecHotCoordiItem.empty() == false)
        m_pkHotCoordiItemBox->SetItemInfo( m_vecHotCoordiItem[0] );

    m_iCurrentHotItem = 0;
    m_iTotalHotItem = static_cast<int>( m_vecHotCoordiItem.size() );
}

void KGCCoordiShopScene::EquipHotCooriItem( void )
{
    GCItem* pItem = m_pkHotCoordiItemBox->GetItemData();
    if (pItem == NULL)
        return;

    std::map<GCITEMID,std::vector<KDropItemInfo>>& mapPackageInfo = g_pItemMgr->GetPackageInfo();

    std::map<GCITEMID,std::vector<KDropItemInfo>>::iterator mit = mapPackageInfo.find( pItem->dwGoodsID * 10 );

    if( mit == mapPackageInfo.end() )
        return;

    PLAYER* ViewPlayer = m_pkCoordiView->GetPlayer();
    //ViewPlayer->SetMyPlayer( m_pkCoordiView->GetCurrentChar(), true  );
    ViewPlayer->SetMyPlayer( -1, true  );

    if( !m_pkHotCoordiItemBox->IsEquip() ) // 장착 중 아닐 때 
    {
        for( std::vector< KDropItemInfo >::iterator vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            GCItem* pTempItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
            if( pTempItem == NULL || ( pTempItem->eItemKind != GIK_COORDI_ITEM && pTempItem->eItemKind != GIK_COORDI_PACKAGE7 ) )
                continue;

            if( !IsEquipItem(pTempItem) )
                SetEquipData( pTempItem );
        }
        m_pkHotCoordiItemBox->SetEquip( true );
        m_bEquipHotCoordi = true;
        m_dwCurHotCoordiID = pItem->dwGoodsID;
    }
    else    // 장착 중 일 때 
    {
        for( std::vector< KDropItemInfo >::iterator vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            GCItem* pTempItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
            if( pTempItem == NULL || ( pTempItem->eItemKind != GIK_COORDI_ITEM && pTempItem->eItemKind != GIK_COORDI_PACKAGE7 ) )
                continue;

            if( false == SetEquipData( pTempItem ) )
            {
                if( m_pkCoordiView->IsShowDefault() )
                    g_pItemMgr->UnequipItemForItemID( &( ViewPlayer->GetCurrentChar() ), pTempItem->dwGoodsID );
                else
                {
                    m_pkCoordiView->ResetPlayer();
                    ViewPlayer = m_pkCoordiView->GetPlayer();
                    //ViewPlayer->SetMyPlayer( -1, true  );
                }
            }   
        }
        m_pkHotCoordiItemBox->SetEquip( false );
        m_bEquipHotCoordi = false;
        m_dwCurHotCoordiID = 0;
    }

    for( std::vector<GCItem*>::iterator it = m_vecEquipItem.begin() ; it != m_vecEquipItem.end() ; ++it )
        g_pItemMgr->EquipItem( &( ViewPlayer->GetCurrentChar() ), *it, false, 0, true );

    for( int i=0 ; i<MAX_COORDI_BOX ; ++i )
    {
        GCItem* pItem = m_apkCoordiItemBox[i]->GetItemData();
        if( IsEquipItem( pItem ) )
            m_apkCoordiItemBox[i]->SetEquip( true );
        else
            m_apkCoordiItemBox[i]->SetEquip( false );
    }

    m_pkCoordiView->SetPlayer( ViewPlayer );
    m_pkCoordiView->UpdateMeshObject();
}

bool KGCCoordiShopScene::IsHotCoordiPackageItem( GCItem* pItem )
{
    GCItem* pHotItem = m_pkHotCoordiItemBox->GetItemData();

    if (pHotItem == NULL)
        return false;

    std::map<GCITEMID,std::vector<KDropItemInfo>>& mapPackageInfo = g_pItemMgr->GetPackageInfo();

    std::map<GCITEMID,std::vector<KDropItemInfo>>::iterator mit = mapPackageInfo.find( pHotItem->dwGoodsID * 10 );

    if( mit == mapPackageInfo.end() )
        return false;

    for( std::vector< KDropItemInfo >::iterator vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    {
        GCItem* pTempItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );

        if( pTempItem->dwGoodsID == pItem->dwGoodsID )
            return true;
    }

    return false;
}

void KGCCoordiShopScene::ClearEquipItemList( void )
{
    m_vecEquipItem.clear();
    m_bEquipHotCoordi = false;
    m_dwCurHotCoordiID = 0;
}

void KGCCoordiShopScene::ViewHotCoordiPackageList( void )
{
    GCItem* pHotCoordiItem = m_pkHotCoordiItemBox->GetItemData();

    if( pHotCoordiItem == NULL )
        return;

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PACKAGE_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, pHotCoordiItem->dwGoodsID * 10, true );
}

void KGCCoordiShopScene::SetChangeCharacter( int iChar_ )
{
    m_pkCoordiView->ResetPlayer();
    m_pkCoordiView->SetCoordiView( iChar_ );
    OnCharacterChange();
    m_pkCoordiView->UpdateMeshObject();
}