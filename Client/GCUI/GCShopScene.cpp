#include "stdafx.h"
#include ".\GCCashShopScene.h"
#include ".\GCGPShopScene.h"
//
#include "../MyD3D.h"

//
#include "GCUI/GCShopTab.h"



//
#include "GCUI/GCInfoPanel.h"
#include "GCUI/KGCStatusInfoPanel.h"
#include "GCUI/GCCharWnd.h"
//
#include "GCUI/GCItemInfoBox.h"

#include "GCSearchBar.h"





#include "../BuddySystem/KFriendShip.h"
#include "../GCStateMachine.h"
#include "GCUI/GCItemInformationBoxPair.h"

IMPLEMENT_CLASSNAME( KGCCashShopScene );
IMPLEMENT_WND_FACTORY( KGCCashShopScene );
IMPLEMENT_WND_FACTORY_NAME( KGCCashShopScene, "gc_cash_shop_scene" );

KGCCashShopScene::KGCCashShopScene(void)
: m_pkBtnPointItemToggle(NULL)
, m_pkBtnCashItemToggle(NULL)
, m_bPointItemToggle(false)
, m_iCurTabCategory(TAB_ARMOR)
{
    m_pkItemBox         = NULL;
    m_pkCurActiveTab    = NULL;
    m_ucCharNum         = 0;
    m_pkUserInfo        = NULL;
    m_pkStatusPanel     = NULL;
    m_pkSearchBar       = NULL;

    for( int i = 0; i < NUM_BUTTON; ++ i )
        m_apkButton[i]  = NULL;

    for( int i = 0; i < NUM_TAB; ++i )
        m_apkShopTab[i] = NULL;

    m_ShopTabType = TAB_SET;
    m_iCurCategory      = -1;
    m_iCurSortType      = -1;
    m_iCurMoneyType     = -1;

    LINK_CONTROL("shop_item_box",m_pkItemBox);

    LINK_CONTROL("armor_tab",           m_apkShopTab[TAB_ARMOR]);
    LINK_CONTROL("set_tab",             m_apkShopTab[TAB_SET]);
    LINK_CONTROL("weapon_tab",          m_apkShopTab[TAB_WEAPON]);
    LINK_CONTROL("accessory_tab",       m_apkShopTab[TAB_ACC]);
	LINK_CONTROL("accessory2_tab",      m_apkShopTab[TAB_ACC2]);
    LINK_CONTROL("training_tab",        m_apkShopTab[TAB_TRAIN]);
    LINK_CONTROL("pet_tab",             m_apkShopTab[TAB_PET]);
    LINK_CONTROL("pet_item_tab",        m_apkShopTab[TAB_COUPLE]);
    LINK_CONTROL("pointitem_tab",       m_apkShopTab[TAB_POINTITEM]);

    LINK_CONTROL("item_combobox",       m_pkItemCombo );
    LINK_CONTROL("sort_combobox",       m_pkSortCombo );
    LINK_CONTROL("moneytype_combobox",  m_pkMoneyTypeCombo );

    //LINK_CONTROL("user_status_view",    m_pkUserStatusInfo);
    LINK_CONTROL("user_info_view",      m_pkUserInfo);
    LINK_CONTROL("search_bar",          m_pkSearchBar );

    m_pkWelcomeBack_btn = NULL;
    LINK_CONTROL( "welcomeback_btn",            m_pkWelcomeBack_btn );

    m_bShowUserInfo = false;
    m_bShowStatusInfo = false;

	LINK_CONTROL("user_status_info_view", m_pkStatusPanel);

    LINK_CONTROL("pointitem_toggle", m_pkBtnPointItemToggle);
    LINK_CONTROL("cashItem_toggle", m_pkBtnCashItemToggle);

	m_pkBntUserInfo = NULL;
	LINK_CONTROL("user_info_button", m_pkBntUserInfo);

	m_pkBntUserStatus = NULL;
	LINK_CONTROL("user_status_button", m_pkBntUserStatus);

	m_pkCharWnd = NULL;
	LINK_CONTROL( "character_window", m_pkCharWnd );
	
}

KGCCashShopScene::~KGCCashShopScene(void)
{
}

void KGCCashShopScene::OnCreate()
{
    KGCScene::OnCreate();

    for( int i = 0 ; i < NUM_TAB ; ++i )
    {
        m_apkShopTab[i]->InitState( true, true, this );
    }

	m_apkShopTab[TAB_ACC2]->ToggleRender(false);

#if !defined( COUPLE_SYSTEM ) && !(defined( NATION_IDN ) || defined(NATION_THAILAND)) && !defined(NATION_LATINAMERICA) || defined( NATION_CHINA )
	m_apkShopTab[TAB_COUPLE]->InitState( false );
#endif
	

    // 포인트 시스템 사용여부에 따라 포인트 샵을 보이지 않게 한다
    #if !defined( ENABLE_POINT_SYSTEM )
    {
        m_apkShopTab[TAB_POINTITEM]->InitState( false );

        for( int i = 0 ; i < NUM_TAB ; ++i )
        {
            if( i == TAB_POINTITEM )
                continue;

            D3DXVECTOR2 vPos = m_apkShopTab[i]->GetFixedWindowLocalPos();
            vPos.x -= m_apkShopTab[TAB_POINTITEM]->GetWidth();

            m_apkShopTab[i]->SetFixedWindowLocalPos( vPos );
        }
    }
    #endif
    
    g_MyD3D->m_TempPlayer.m_kUserInfo = g_kGlobalValue.m_kUserInfo;

    m_pkItemBox->AddActionListener(this);
    m_pkItemCombo->AddActionListener( this );
    m_pkSortCombo->AddActionListener( this );
    m_pkMoneyTypeCombo->AddActionListener( this );

    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_NEW ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_RECOM ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_HIGH_LV ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_LOW_LV ) );
    m_pkSortCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEMSORT_NAME ) );

    m_pkMoneyTypeCombo->AddString( g_pkStrLoader->GetString( STR_ID_TOTAL ) );
    m_pkMoneyTypeCombo->AddString( g_pkStrLoader->GetString( STR_ID_MONEYTYPE_GP ) );
#if defined ( WEB_CASH ) || defined ( GAME_CASH )
    m_pkMoneyTypeCombo->AddString( g_pkStrLoader->GetString( STR_ID_MONEYTYPE_CASH_POINT ) );
#endif

#if !defined(NATION_CHINA)
    m_pkMoneyTypeCombo->AddString( g_pkStrLoader->GetString( STR_ID_MONEYTYPE_CRYSTAL ) );
#endif

    m_pkSearchBar->InitState( true, true, this );

#ifdef WELCOMEBACK_FRIEND
    m_pkWelcomeBack_btn->InitState( true, true, this );
#else
    m_pkWelcomeBack_btn->InitState( false );
#endif

	m_pkBntUserInfo->InitState(true,true,this);
	m_pkBntUserStatus->InitState(true,true,this);

    SetInfoButtonState(true, false);

    g_pkUIMgr->DisconnectEventProcedure_Code( GetUID(), KGCUIScene::D3DWE_CHANGE_CHAR_INFO );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_CHANGE_CHAR_INFO, this, &KGCCashShopScene::OnChangeItemInfo );


#if defined(BUY_CASH_ITEM_WITH_POINT)
    m_pkBtnPointItemToggle->InitState(true, true, this);
    m_pkBtnCashItemToggle->InitState(false, true, this);
#endif

	m_pkCharWnd->AddActionListener(this);
	//m_pkCharWnd->SetUseRoomObject(true);

}

void KGCCashShopScene::OnEnterScene()
{
    m_pkCharWnd->UpdateMeshObject();

#if !defined( USE_COORDI_SHOP )
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 610.0f, 0.0f ) );
#else
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 383.0f, 0.0f ) );
#endif
    g_pkUIScene->m_pkInventoryFullMsg->ToggleRender( true );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );

	if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH ) {
		if (g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
			SetButtonRender( BTN_GUILD_MATCH, true );
			SetButtonRender( BTN_MATCH, false );
		}
		else {
			SetButtonRender( BTN_GUILD_MATCH, false );
			SetButtonRender( BTN_MATCH, true );
		}        
	}

    g_pkUIScene->EnableMessageBox( true );

}

void KGCCashShopScene::InitPointshopTap()
{
#if defined(BUY_CASH_ITEM_WITH_POINT)
    m_iCurTabCategory = static_cast<int>(TAB_ARMOR);
    m_bPointItemToggle = false;
    m_pkBtnPointItemToggle->ToggleRender(true);
    m_pkBtnCashItemToggle->ToggleRender(false);

	m_apkShopTab[TAB_ACC2]->ToggleRender(false);
	m_apkShopTab[TAB_ACC]->ToggleRender(true);

    m_apkShopTab[TAB_TRAIN]->ToggleRender(true);
    m_apkShopTab[TAB_PET]->ToggleRender(true);
    m_apkShopTab[TAB_COUPLE]->ToggleRender(true);

#endif
}

void KGCCashShopScene::OnLeaveScene()
{
    ResetEquipItems();
    m_pkCharWnd->DeleteMeshObject();

    InitPointshopTap();
}

void KGCCashShopScene::OnDestroy()
{
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
}
void KGCCashShopScene::OnCreateComplete()
{    
}

void KGCCashShopScene::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        if( m_pkBtnPointItemToggle == event.m_pWnd )
        {
            //포인트 샵에서 보이는 탭들이 아닐 경우엔 포인트 카테고리로 강제 이동한다.
            if( !(
                static_cast<ShopTabType>(m_iCurTabCategory) == TAB_ARMOR ||
                static_cast<ShopTabType>(m_iCurTabCategory) == TAB_SET ||
                static_cast<ShopTabType>(m_iCurTabCategory) == TAB_WEAPON ||
                static_cast<ShopTabType>(m_iCurTabCategory) == TAB_ACC ||
                static_cast<ShopTabType>(m_iCurTabCategory) == TAB_POINTITEM
                ) )
            {
                SetActiveShopTab(TAB_POINTITEM , false);
                m_iCurTabCategory = static_cast<int>(TAB_ARMOR);
            }

            m_bPointItemToggle = true;

            m_pkBtnCashItemToggle->ToggleRender(true);
            m_pkBtnPointItemToggle->ToggleRender(false);


			m_apkShopTab[TAB_ACC]->ToggleRender(false);
			m_apkShopTab[TAB_ACC2]->ToggleRender(true);

            m_apkShopTab[TAB_TRAIN]->ToggleRender(false);
            m_apkShopTab[TAB_PET]->ToggleRender(false);
            m_apkShopTab[TAB_COUPLE]->ToggleRender(false);

            RefreshtemBox();
        }
        else if ( m_pkBtnCashItemToggle == event.m_pWnd )
        {
            m_bPointItemToggle = false;

            m_pkBtnCashItemToggle->ToggleRender(false);
            m_pkBtnPointItemToggle->ToggleRender(true);

			m_apkShopTab[TAB_ACC]->ToggleRender(true);
			m_apkShopTab[TAB_ACC2]->ToggleRender(false);

            m_apkShopTab[TAB_TRAIN]->ToggleRender(true);
            m_apkShopTab[TAB_PET]->ToggleRender(true);
            m_apkShopTab[TAB_COUPLE]->ToggleRender(true);
            
            RefreshtemBox();
        }

        if( m_pkCharWnd == event.m_pWnd )
        {
            //캐릭터가 바뀜
            //g_kGlobalValue.m_kUserInfo = g_MyD3D->m_TempPlayer.m_kUserInfo;
            SetCharItem( (EGCCharType)((int)g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) );

            m_pkStatusPanel->ResetInfo();
            m_pkStatusPanel->SetStartUserInfo();
            m_pkStatusPanel->SetChangeUserInfo();
            m_pkStatusPanel->SetText();
        }

        //나가기
//         if ( event.m_pWnd == m_apkButton[BTN_SHOP] )
//         {
//             ResetEquipItems();
//             g_MyD3D->m_pStateMachine->GoPrev();            
//             return;
//         }
    }    

    if( event.m_dwCode == D3DWE_COMBO_SELCHANGE )
    {
        ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), m_pkMoneyTypeCombo->GetCurSel() );
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {    
        for( UINT i = 0 ; i < NUM_TAB ; i++ )
        {        
            if (event.m_pWnd == m_apkShopTab[i])
            {
                SetActiveShopTab((ShopTabType)i , false);
                m_iCurTabCategory = i;
                return;
            }
        }
    }

	KGCScene::ActionPerformed( event );

	if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK)
		return;

    if ( event.m_pWnd == m_pkWelcomeBack_btn )
    {
#if defined( WELCOMEBACK_FRIEND_NEW )
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_WELCOMEBACK_DLG_NEW );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_WELCOMEBACK_DLG );
#endif
        return;
    }
    if( event.m_pWnd == m_pkBntUserStatus )
        SetInfoButtonState(false, true);
    if( event.m_pWnd == m_pkBntUserInfo )
        SetInfoButtonState(true, false);

}

void KGCCashShopScene::ResetEquipItems()
{
    //g_MyD3D->m_kItemMgr.CheckItemConflict( true );
    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
    
}

void KGCCashShopScene::FrameMoveInEnabledState( void )
{
    if( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        if ( false == g_pkUIScene->IsMsgBoxModal() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ESC_MENU_DLG );
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        }
    }

    DrawInfoWindow();

    SiKFriendShip()->CheckAlramAddFriend();

    KGCScene::FrameMoveInEnabledState();
}

void KGCCashShopScene::OnActivateComplete( void )
{
}

void KGCCashShopScene::SetActiveShopTab( ShopTabType TabType,bool bInit_ )
{
    if ( TabType < TAB_ARMOR || TabType >= NUM_TAB ) 
        return;

    m_apkShopTab[TabType]->Activate(true);

    if( TabType == m_ShopTabType )
        return;

    m_apkShopTab[m_ShopTabType]->Activate(false);
    m_ShopTabType = (ShopTabType)TabType;

    if( g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex == -1 )
        return;

    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
    m_pkItemBox->SetShopItemView((EGCCharType)iCharType, TabType, -1, -1 );

    //Combo Box 항목 바꾸기
    if( bInit_ )
    {
        SetComboPopupList();
    }
    else
    {
        SetComboPopupList( true, false, false );
    }
    ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), m_pkMoneyTypeCombo->GetCurSel() );
}

void KGCCashShopScene::FilterGoodsItem( ShopTabType TabType )
{
    m_ShopTabType = TabType;
    m_pkItemBox->ResetScrollbar( 0 );
}

void KGCCashShopScene::SetCharItem( EGCCharType iChar )
{
    m_pkItemBox->SetShopItemView(iChar, m_ShopTabType, -1, -1 );
    //SetComboPopupList();
    ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), m_pkMoneyTypeCombo->GetCurSel() );
    UpdateUserInfo();

	//m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
}

void KGCCashShopScene::UpdateUserInfo()
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_SHOP_CASH ) { 
        return;
    }

    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
    m_pkUserInfo->SetUserInfo();

	//m_pkCharWnd->UpdateMeshObject();
	//m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );

    KPetInfo &kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
    int iPetEvolutionLevel = (int)kPet.m_cPromotion;
    int iPetLevel = kPet.m_iLevel;

    bool bPetExistFlag = false;

    if( kPet.m_dwUID != 0 )
    {
        // 만약 인벤토리에 없는놈을 장착하고 있다면
        if( !g_pItemMgr->FindInventory( kPet.m_dwUID ) )
        {
            kPet.Init();
            bPetExistFlag = false;
        }
        // 인벤토리에 있다면
        else
        {
            // 펫 정보가 있는지 확인
            if( g_MyD3D->IsExistPetInfo( kPet.m_dwUID ) && iPetID != -1)
            {
                if( iPetID != m_pkCharWnd->m_pkPet->m_iPetID ||
                    iPetEvolutionLevel != m_pkCharWnd->m_pkPet->m_iEvolutionLevel ||
                    iPetLevel != m_pkCharWnd->m_pkPet->m_iLevel )
                {
                    m_pkCharWnd->m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID );
                    m_pkCharWnd->m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
                }
                bPetExistFlag = true;
            }
            // 펫정보가 없다
            else
            {
                bPetExistFlag = false;
            }
        }
    }
    // 펫 UID가 0이다
    else
    {
        bPetExistFlag = false;
    }

    // 펫이 없다
    if( !bPetExistFlag )
    {
        m_pkCharWnd->m_pkPet->EndPet();
    }

}

void KGCCashShopScene::ChangeSortType( int iCategory, int iSortType, int iMoneyType )
{
#if defined ( WEB_CASH ) || defined ( GAME_CASH )
#else
    if ( iMoneyType == 2 )
    {
        iMoneyType = 3;
    }    
#endif    
    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

    m_pkItemBox->SetShopItemView((EGCCharType)iCharType, m_ShopTabType, iCategory, iSortType, iMoneyType );
    m_iCurCategory = iCategory;
    m_iCurSortType = iSortType;
    m_iCurMoneyType = iMoneyType;
}
void KGCCashShopScene::FindItemFromName( const WCHAR* strName_ )
{
    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;

    m_pkItemBox->SetShopItemView((EGCCharType)iCharType, m_ShopTabType, m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), m_pkMoneyTypeCombo->GetCurSel(), strName_ );
}

void KGCCashShopScene::SetComboPopupList( bool bResetCategory_ , bool bResetSortType_ , bool bResetMoneyType_ )
{
    
    if ( bResetSortType_ )
    {
        m_pkSortCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_ITEMSORT ) );
        m_pkSortCombo->SetCurSel( -1 );
    }
    

    if ( bResetMoneyType_ )
    {
        m_pkMoneyTypeCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_MONEYTYPE ) );
        m_pkMoneyTypeCombo->SetCurSel( -1 );
    }

    if ( !bResetCategory_ )
    {
        return;
    }
    

    m_pkItemCombo->DeleteAllItem();
    m_pkItemCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_ITEM_CATEGORY ) );
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_TOTAL ) );

    switch ( m_ShopTabType )
    {
    case TAB_ARMOR:
        {
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1 ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2 ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HAND ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_MANTLE ) );
            break;
        }
    case TAB_WEAPON:
        {
            if( g_kGlobalValue.IsCharacterOpenFromServer() )
            {
                m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON1 ) );

                int iMaxOpenJob = g_kGlobalValue.GetOpenedCharacterNum();

                if (iMaxOpenJob >=1 ) m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON2 ) );
                if (iMaxOpenJob >=2 ) m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON3 ) );
                if (iMaxOpenJob >=3 ) m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON4 ) );
			}
			else
			{
				m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON1 ) );
#if defined( USE_ELESIS2 ) || defined( USE_LIRE2 ) || defined( USE_ARME2 ) || defined( USE_LAS2 ) || defined( USE_RYAN2 )
				m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON2 ) );
#endif
#if defined( USE_ELESIS3 ) || defined( USE_LIRE3 ) || defined( USE_ARME3 ) || defined( USE_LAS3 ) || defined( USE_RYAN3 )
				m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON3 ) );
#endif
#if defined( USE_ELESIS4 ) || defined( USE_LIRE4 ) || defined( USE_ARME4 ) || defined( USE_LAS4 ) || defined( USE_RYAN4 )
				m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON4 ) );
#endif
			}
            break;
        }
    case TAB_ACC:
        {
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_UP )  );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_DOWN ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_NECKLACE )  );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1_ACC ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2_ACC ) );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_ARM_ACC )  );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_RING )   );
            m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE_ACC )  );
            break;
        }
	case TAB_COUPLE:
		{
			m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COUPLE_ITEM0 ) );
			m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COUPLE_ITEM1 ) );
			m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COUPLE_ITEM2 ) );
			m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COUPLE_ITEM3 ) );
			m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_COUPLE_ITEM4 ) );
			break;
		}
    }
}

void KGCCashShopScene::OnInitialize()
{
    // [2/17/2008] breadceo. 내 케릭터 정보 업데이트 안됨
    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);

    // 초기 능력치
    m_pkStatusPanel->SetStartUserInfo();

    int nType = GC_GLOBAL_DEFINE::nShopBeginShowTab;

    SetActiveShopTab(static_cast<ShopTabType>( nType ));
    UpdateUserInfo();

 
    SetCharItem( (EGCCharType)((int)g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) );

    m_pkSearchBar->Init();
}

void KGCCashShopScene::DrawInfoWindow()
{
    GCItem* kItem = NULL;
    D3DXVECTOR2 vecPos( 0.0f, 0.0f );
    if ( !m_pkItemBox->GetCurrentHoveredItem( kItem, vecPos ) )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return;
    }

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );

}

void KGCCashShopScene::UpdateItemView()
{
	ChangeSortType( m_pkItemCombo->GetCurSel(), m_pkSortCombo->GetCurSel(), m_pkMoneyTypeCombo->GetCurSel() );
}

void KGCCashShopScene::SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_)
{
    m_bShowUserInfo = bUserInfo_;
	m_pkBntUserInfo->Activate(bUserInfo_);
    m_pkUserInfo->ToggleRender(bUserInfo_);

    m_bShowStatusInfo = bStatusInfo_;
    m_pkBntUserStatus->Activate(bStatusInfo_);
    m_pkStatusPanel->ToggleRender(m_bShowStatusInfo);

    if( bUserInfo_)
        m_pkWelcomeBack_btn->ToggleRender( true );
    else
        m_pkWelcomeBack_btn->ToggleRender( false );
}

void KGCCashShopScene::OnChangeItemInfo()
{
    bool bEquip = false;
    if( m_ShopTabType == TAB_SET ) {    
        for(UINT i=0 ; i<KGCShopItemBox::NUM_SET_ITEM_WND ; ++i) {
            if( m_pkItemBox->IsEquip(i) ) {
                bEquip = true;
                m_pkStatusPanel->SetChangeUserInfo();
                m_pkStatusPanel->SetText();
                break;
            }
        }
    }
    else {
        for(UINT i=0 ; i<KGCShopItemBox::NUM_ITEM_WND ; ++i) {
            if( m_pkItemBox->IsEquip(i) ) {
                bEquip = true;
                m_pkStatusPanel->SetChangeUserInfo();
                m_pkStatusPanel->SetText();
                break;
            }
        }
    }
    if( !bEquip ) {
        m_pkStatusPanel->SetChangeUserInfo();
        m_pkStatusPanel->SetText();
    }
}

void KGCCashShopScene::UpdateUserPanel()
{
    m_pkStatusPanel->SetChangeUserInfo();
    m_pkStatusPanel->SetText();
}
