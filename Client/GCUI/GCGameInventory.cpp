#include "stdafx.h"
#include "GCGameInventory.h"

IMPLEMENT_CLASSNAME( KGCGameInventory );
IMPLEMENT_WND_FACTORY( KGCGameInventory );
IMPLEMENT_WND_FACTORY_NAME( KGCGameInventory, "gc_gameinventory" );

KGCGameInventory::KGCGameInventory( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnPrev = NULL;
    m_pkBtnNext = NULL;
    LINK_CONTROL( "btnClose", m_pkBtnClose );
    LINK_CONTROL( "btnPrev", m_pkBtnPrev );
    LINK_CONTROL( "btnNext", m_pkBtnNext );

    m_pkStaticPage = NULL;
    LINK_CONTROL( "staticPage", m_pkStaticPage );

    m_pkItemCombo = NULL;
    LINK_CONTROL( "comboItem",  m_pkItemCombo );

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) { 
        m_apkItemSlot[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "itemSlot%d", i );
        LINK_CONTROL( szTemp, m_apkItemSlot[i] );
    }
    
    for( int i=0 ; i<NUM_EQUIP_SLOT ; ++i ) { 
        m_apkEquipSlot[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "equipSlot%d", i );
        LINK_CONTROL( szTemp, m_apkEquipSlot[i] );
    }

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkCharImg[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "charImg%d", i );
        LINK_CONTROL( szTemp, m_apkCharImg[i] );

    }

    for( int i=0 ; i<NUM_GRADE ; ++i ) { 
        m_apkBtnGradeDefault[i] = NULL;
        m_apkBtnGradeActive[i] = NULL;
        m_apkStaticGrade[i] = NULL;

        char szDefault[128] = {0, };
        char szActive[128] = {0, };
        char szStatic[128] = {0, };

        sprintf_s( szDefault, 127, "btnGradeDefault%d", i );
        sprintf_s( szActive, 127, "btnGradeActive%d", i );
        sprintf_s( szStatic, 127, "staticGrade%d", i );

        LINK_CONTROL( szDefault, m_apkBtnGradeDefault[i] );
        LINK_CONTROL( szActive, m_apkBtnGradeActive[i] );
        LINK_CONTROL( szStatic, m_apkStaticGrade[i] );
    }

    m_pkStaticBaseTitle = NULL;
    m_pkStaticAttributeTitle = NULL;
    m_pkStaticRank = NULL;
    m_pkStaticBaseDesc = NULL;
    m_pkStaticAttributeDesc = NULL;
    m_pkStaticBaseStat = NULL;
    m_pkStaticAttributeStat = NULL;
    LINK_CONTROL( "staticBaseTitle", m_pkStaticBaseTitle );
    LINK_CONTROL( "staticAttributeTitle", m_pkStaticAttributeTitle );
    LINK_CONTROL( "staticRank", m_pkStaticRank );
    LINK_CONTROL( "staticBaseDesc", m_pkStaticBaseDesc );
    LINK_CONTROL( "staticAttributeDesc", m_pkStaticAttributeDesc );
    LINK_CONTROL( "staticBaseStat", m_pkStaticBaseStat );
    LINK_CONTROL( "staticAttributeStat", m_pkStaticAttributeStat );

    m_vecInvenItem.clear();
    m_vecItemState.clear();
    m_iCurPage = 0;
    m_iTotalPage = 0;

    m_vecHotEquipParticle.clear();
}

KGCGameInventory::~KGCGameInventory( void )
{
}

void KGCGameInventory::OnDestroy( void )
{
    if ( g_ParticleManager )
    {
        g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
    }

    m_vecHotEquipParticle.clear();
}

bool KGCGameInventory::InitDialog( IN const KDialogInfo& kInfo_ )
{
    // 여기서 UI 열 때의 착용 장비 리스트를 저장합시다.
    m_pPlayer = g_MyD3D->GetMyPlayer();
    if( NULL == m_pPlayer ) { 
        return false;
    }

    if( false == GetCurCharacterItemList( m_pPlayer, m_kBeforeEquip ) ) {
        return false;
    }

    InitComboList();
    RefreshItemList();      // 아이템 리스트 만들고 
    InitPage();             // 페이지 초기화 
    SetInventoryItemSlot(); // 인벤토리 슬롯 세팅 
    InitEquipSlot();        // 착용 장비 세팅 

    m_kBeforeStatus.Clear(); // 시작 스탯 
    GetUserStatus( m_pPlayer, m_kBeforeStatus );
    SetStatusStatic();

    SetCharImg( m_pPlayer->GetCurrentChar().iCharType );  // 캐릭터 이미지    
    return true;
}

void KGCGameInventory::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInventory::OnClickBtnClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnPrev->InitState( true, true, this );
    m_pkBtnPrev->DisconnectAllEventProcedure();
    m_pkBtnPrev->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInventory::OnClickBtnPrev );

    m_pkBtnNext->InitState( true, true, this );
    m_pkBtnNext->DisconnectAllEventProcedure();
    m_pkBtnNext->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInventory::OnClickBtnNext );

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) { 
        m_apkItemSlot[i]->InitState( true, true, this );
        m_apkItemSlot[i]->DisconnectAllEventProcedure();
        m_apkItemSlot[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInventory::OnClickInvenItem );
    }

    for( int i=0 ; i<NUM_EQUIP_SLOT ; ++i ) { 
        m_apkEquipSlot[i]->InitState( true, true, this );
        m_apkEquipSlot[i]->DisconnectAllEventProcedure();
        m_apkEquipSlot[i]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGameInventory::OnCiickEquipItem );
    }

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkCharImg[i]->InitState( false );
    }

    for( int i=0 ; i<NUM_GRADE ; ++i ) { 
        m_apkBtnGradeDefault[i]->InitState( false, true, this );
        m_apkBtnGradeActive[i]->InitState( true, true, this );
        m_apkStaticGrade[i]->InitState( true );
        switch (i) {
            default:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_NORMAL));
            case 0:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_NORMAL));
            case 1:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_MAGIC));
            case 2:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_EPIC));
            case 3:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_LEGEND));
            case 4:
                m_apkStaticGrade[i]->SetText(g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_MYSTIC));
        }
    }

    m_pkStaticPage->InitState( true );
    m_pkStaticPage->SetAlign( DT_CENTER );

    m_pkItemCombo->InitState( true, true, this );
    m_pkItemCombo->DisconnectAllEventProcedure();
    m_pkItemCombo->ConnectEventProcedure( KD3DWnd::D3DWE_COMBO_SELCHANGE, this, &KGCGameInventory::OnComboChange );

    m_pkStaticBaseStat->InitState( true );
    m_pkStaticBaseStat->SetMultiLine( true );
    m_pkStaticAttributeStat->InitState( true );
    m_pkStaticAttributeStat->SetMultiLine( true );
    m_pkStaticAttributeStat->SetLineSpace( 1.1f );

    SetStatDesc();
}

void KGCGameInventory::ActionPerformed( const KActionEvent& event )
{
    for( int i=0 ; i<NUM_GRADE ; ++i ) { 
        if( event.m_pWnd == m_apkBtnGradeDefault[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
            OnClickBtnGrade( i );
        }

        if( event.m_pWnd == m_apkBtnGradeActive[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
            OnClickBtnGrade( i );
        }
    }
}

void KGCGameInventory::PostChildDraw( void )
{
    m_pkItemCombo->Render();
}

void KGCGameInventory::OnClickBtnClose()
{
    // 장비가 바꼈나 체크하고 날린다. 
    if( true == CheckItemChange() ) { 
        g_MyD3D->SendEquipItemList( m_pPlayer, &m_kBeforeEquip, true );

        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_ELESIS1_SPECIAL4_CHANGE_MODE, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_ELESIS1_SPECIAL4_CHANGE_MODE ) );
        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_LAS_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_LAS_MOVE_SPEEDUP ) );
        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILLEFFECT_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILLEFFECT_MOVE_SPEEDUP ) );
        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RYAN_MOVE_SPEEDUP, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_RYAN_MOVE_SPEEDUP ) );
        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_JUMP_UP10, false, 0 != g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_JUMP_UP10 ) );
    }
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );

    g_MyD3D->SetEnableKeyInput( true );
    g_pkGameOverlayUI->ShowInvenKey( false );
    g_MyD3D->MyCtrl->k_Game_Inventory = false;
}

bool KGCGameInventory::OnClickInvenItem( const KActionEvent& event )
{
    GCITEMUID itemUID = event.m_dwlParam;
   
    GCItem* pItemData = NULL;
    g_pItemMgr->FindInventory( itemUID, &pItemData );
    if( NULL == pItemData ) { 
        return false;
    }

    int iSlotPos = g_pItemMgr->GetUniqueSlotPosition( pItemData->dwSlotPosition );
    int iEquipSlotIndex = GetEquipSlotIndex( iSlotPos );
    if( EISP_WEAPON == iSlotPos )  {
        GCITEMUID swapWeaponUID = m_pPlayer->GetSwapWeaponUID();
        if( m_apkEquipSlot[ iEquipSlotIndex ]->GetEquipItem() != NULL && swapWeaponUID != 0 && 
            m_apkEquipSlot[ iEquipSlotIndex ]->GetEquipItem()->m_ItemUID == swapWeaponUID ) { 
            return false;
        }
    }

    if( g_pItemMgr->EquipInventoryItem( &m_pPlayer->GetCurrentChar(), itemUID ) ) {
        SetEquipSlot( itemUID );
        RefreshItemList();
        SetPageItem();
        SetStatusStatic( true, m_pPlayer );
    }
    return true;
}

bool KGCGameInventory::OnCiickEquipItem( const KActionEvent& event )
{
    GCITEMUID itemUID = event.m_dwlParam;
    GCITEMUID swapWeaponUID = m_pPlayer->GetSwapWeaponUID();

    if( itemUID == swapWeaponUID ) { 
        return false;
    }

    g_pItemMgr->UnequipItemForItemUID( &m_pPlayer->GetCurrentChar(), itemUID );

    GCItem* pItemData = NULL;
    g_pItemMgr->FindInventory( itemUID, &pItemData );
    if( pItemData ) { 
        int iItemSlot = g_pItemMgr->GetUniqueSlotPosition( pItemData->dwSlotPosition );
        int iSlotIndex = GetEquipSlotIndex( iItemSlot );

        if( 0 <= iSlotIndex && iSlotIndex < NUM_EQUIP_SLOT ) {
            m_apkEquipSlot[ iSlotIndex ]->Clear();
        }
    }

    RefreshItemList();
    SetPageItem();
    SetStatusStatic( true, m_pPlayer );
    return true;
}

void KGCGameInventory::RefreshItemList()
{
    m_vecInvenItem.clear();
    m_vecInvenItem = g_pItemMgr->GetInventoryItemUIDList(
        g_pItemMgr->GetCharFlag( m_pPlayer->GetCurrentChar().iCharType ),
        ESP_WEAPON|ESP_HELMET|ESP_JACKET|ESP_PANTS|
        ESP_GLOVES|ESP_SHOES|ESP_MANTLE|ESP_A_UPPER_HELMET|
        ESP_A_DOWN_HELMET|ESP_A_JACKET|ESP_A_PANTS|ESP_A_ARM, 0 ); 

    g_pItemMgr->FilterBadPCBANGItemList( m_vecInvenItem );
    g_pItemMgr->FilterEquipedItem( &m_pPlayer->GetCurrentChar(), m_vecInvenItem );
    g_pItemMgr->FilterRemoveCoordiItem( m_vecInvenItem );

    FilterSwapWeapon();
    FilterComboItem(); 
    FilterItemGrade();
    FilterNonEquipItem();
    SetItemState();
    SortInvenItem();
}

void KGCGameInventory::FilterComboItem( void )
{
    // 콤보박스 선택한거에 맞게 필터 
    int iEquipSlot = GetComboItemSlot();
    if( -1 == iEquipSlot ) { 
        return;
    }

    if( iEquipSlot != 0 ) { 
        for( std::vector< GCITEMUID >::iterator vit = m_vecInvenItem.begin() ; vit != m_vecInvenItem.end() ; ) {
            GCItem* pItem = NULL;
            g_pItemMgr->FindInventory( *vit, &pItem );
            if( NULL == pItem ) { 
                continue;
            }

            if( g_pItemMgr->GetUniqueSlotPosition( pItem->dwSlotPosition ) != iEquipSlot ) {
                vit = m_vecInvenItem.erase( vit );
            }
            else { 
                ++vit;
            }
        }
    }
}

void KGCGameInventory::FilterItemGrade( void )
{
    // 아이템 등급 선택된 것에 맞게 필터 
    std::vector< int > vecItemGrade;
    for( int i=0 ; i<NUM_GRADE ; ++i ) {
        if( m_apkBtnGradeActive[i]->IsRenderOn() ) {
            vecItemGrade.push_back( i );
        }
    }

    for( std::vector< GCITEMUID >::iterator vit = m_vecInvenItem.begin() ; vit != m_vecInvenItem.end() ; ) { 
        KItem* pItem = NULL;
        g_pItemMgr->FindInventory( *vit, &pItem );

        if( NULL == pItem ) { 
            continue;
        }

        bool bErase = true;
        int  iItemGrade = GetItemGrade( static_cast<KItem::ITEM_GRADE>( pItem->m_cGradeID ) );
        for( std::vector< int >::iterator gradeIter = vecItemGrade.begin() ; gradeIter != vecItemGrade.end() ; ++gradeIter ) { 
            if( *gradeIter ==  iItemGrade ) { 
                bErase = false;
                break;
            }
        }

        if( bErase ) {
            vit = m_vecInvenItem.erase( vit );
        }
        else { 
            ++vit;
        }
    }
}

bool KGCGameInventory::GetCurCharacterItemList( IN PLAYER* pPlayer_, OUT SUserInfo& userInfo_ )
{
    if( NULL == pPlayer_ ) { 
        return false;
    }

    userInfo_ = pPlayer_->m_kUserInfo;
    return true;
}

void KGCGameInventory::InitPage( void )
{
    if( m_vecInvenItem.empty() ) { 
        m_iCurPage = 0;
        m_iTotalPage = 0;
    }
    else {
        int iItemCount = static_cast<int>( m_vecInvenItem.size() );
        m_iCurPage = 1;
        m_iTotalPage = iItemCount / NUM_ITEM_SLOT;
        if( iItemCount % NUM_ITEM_SLOT != 0) { 
            ++m_iTotalPage;
        }
    }

    SetPageStatic();
}

void KGCGameInventory::SetPage( void )
{
    if( m_vecInvenItem.empty() ) {
        m_iCurPage = 0;
        m_iTotalPage = 0;
    }
    else { 
        int iBeforePage = m_iCurPage;
        
        if( iBeforePage == 0 && !m_vecInvenItem.empty() ) { 
            iBeforePage = 1;
            m_iCurPage = 1;
        }

        int iItemCount = static_cast<int>( m_vecInvenItem.size() );
        m_iTotalPage = iItemCount / NUM_ITEM_SLOT;
        if( iItemCount % NUM_ITEM_SLOT != 0 ) { 
            ++m_iTotalPage;
        }

        if( m_iTotalPage<iBeforePage && iBeforePage > 0 ) { 
            --m_iCurPage;
        }
        else if ( m_iTotalPage<iBeforePage ) {
            m_iCurPage = m_iTotalPage;
        }
    }

    SetPageStatic();
}

void KGCGameInventory::OnClickBtnPrev( void )
{
    --m_iCurPage;
    if( m_iCurPage < 1 ) { 
        m_iCurPage = 1;
    }
    SetPageItem();
}

void KGCGameInventory::OnClickBtnNext( void )
{
    ++m_iCurPage;
    if( m_iCurPage > m_iTotalPage ) {
        m_iCurPage = m_iTotalPage;
    }
    SetPageItem();
}

void KGCGameInventory::SetInventoryItemSlot( void )
{
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) { 
        m_apkItemSlot[i]->Clear();
    }

    if( m_iCurPage <= 0 || m_iTotalPage <= 0 || m_vecInvenItem.empty() ) { 
        return;
    }

    int iStartIndex = ( m_iCurPage - 1 ) * NUM_ITEM_SLOT;
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) { 
        int iIndex = iStartIndex + i;
        if( iIndex >= static_cast<int>( m_vecInvenItem.size() ) ) {
            continue;
        }
        m_apkItemSlot[i]->SetItemInfo( m_vecInvenItem[ iIndex ], m_vecItemState[ iIndex ].second, m_vecItemState[ iIndex ].first );
    }
}

void KGCGameInventory::SetPageStatic( void )
{
    if( m_iCurPage < 0 || m_iTotalPage < 0 || m_vecInvenItem.empty() ) {
        m_pkStaticPage->SetText( L"" );
        return;
    }

    std::wostringstream stm;
    stm << ( m_iCurPage ) << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText( stm.str() );
}

void KGCGameInventory::SetPageItem( void )
{
    SetPage();
    SetInventoryItemSlot();
}

void KGCGameInventory::InitEquipSlot( void )
{
    for( int i=0 ; i<NUM_EQUIP_SLOT ; ++i ) { 
        m_apkEquipSlot[i]->Clear();
    }
    
    if( m_kBeforeEquip.cCharIndex == -1 ) { 
        return;
    }

    KCharInfoVector::iterator vit = m_kBeforeEquip.vecCharInfo.begin();
    for( ; vit != m_kBeforeEquip.vecCharInfo.end() ; ++vit ) { 
        if( vit->iCharType == m_pPlayer->GetCurrentChar().iCharType ) {
            KUserItemVector::iterator itemIter = vit->vecItemInfo.begin();
            for( ; itemIter != vit->vecItemInfo.end() ; ++itemIter ) { 
                    SetEquipSlot( itemIter->dwItemUID );
            }
        }
    }
}

void KGCGameInventory::SetEquipSlot( GCITEMUID itemUID_ )
{
    GCItem* pItemData = NULL;
    g_pItemMgr->FindInventory( itemUID_, &pItemData );
    if( pItemData ) { 
        int iItemSlot = g_pItemMgr->GetUniqueSlotPosition( pItemData->dwSlotPosition );
        int iSlotIndex = GetEquipSlotIndex( iItemSlot );

        if( 0 <= iSlotIndex && iSlotIndex < NUM_EQUIP_SLOT ) {
            m_apkEquipSlot[ iSlotIndex ]->Clear();
            m_apkEquipSlot[ iSlotIndex ]->SetItemData( itemUID_ );
        }
    }
}

int KGCGameInventory::GetEquipSlotIndex( int iItemSlot_ )
{
    int iSlotIndex = -1;

    switch( iItemSlot_ ) {
        case EISP_HELMET:           iSlotIndex = 0;  break;
        case EISP_JACKET:           iSlotIndex = 1;  break;
        case EISP_PANTS:            iSlotIndex = 4;  break;
        case EISP_GLOVES:           iSlotIndex = 5;  break;
        case EISP_SHOES:            iSlotIndex = 2;  break;
        case EISP_MANTLE:           iSlotIndex = 3;  break;
        case EISP_WEAPON:           iSlotIndex = 8;  break;
        case EISP_A_UPPER_HELMET:   iSlotIndex = 6;  break;
        case EISP_A_DOWN_HELMET:    iSlotIndex = 9;  break;
        case EISP_A_JACKET:         iSlotIndex = 7;  break;
        case EISP_A_PANTS:          iSlotIndex = 10; break;
        case EISP_A_ARM:            iSlotIndex = 11; break;
    }

    return iSlotIndex;
}

void KGCGameInventory::InitComboList( void )
{
    m_pkItemCombo->DeleteAllItem();
    m_pkItemCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_TOTAL ) );
    m_pkItemCombo->SetCurSel( -1 );

    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_TOTAL ) );           // 전체 
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD ) );       // 투구
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1 ) );      // 상의
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2 ) );      // 하의
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HAND ) );       // 장갑
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_SHOOSE ) );     // 신발
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_MANTLE ) );     // 망토
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_WEAPON ) );     // 무기
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_UP ) );    // 머리위 
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_HEAD_DOWN ) );  // 머리아래
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY1_ACC ) );  // 상의장식
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_BODY2_ACC ) );  // 하의장식
    m_pkItemCombo->AddString( g_pkStrLoader->GetString( STR_ID_ITEM_ARM_ACC ) );    // 팔장식
}

void KGCGameInventory::OnComboChange( void )
{
    std::vector< GCITEMUID > vecResultItem;
    int iCurItemSlot = GetComboItemSlot();
    if( -1 == iCurItemSlot ) { 
        return;
    }

    RefreshItemList();
    FilterComboItem();
    InitPage();
    SetInventoryItemSlot();
}

int KGCGameInventory::GetComboItemSlot( void )
{
    int iSel = m_pkItemCombo->GetCurSel();
    
    switch( iSel ) { 
        case 0:     return 0;
        case 1:     return EISP_HELMET;
        case 2:     return EISP_JACKET;
        case 3:     return EISP_PANTS;
        case 4:     return EISP_GLOVES;
        case 5:     return EISP_SHOES;
        case 6:     return EISP_MANTLE;
        case 7:     return EISP_WEAPON;
        case 8:     return EISP_A_UPPER_HELMET;
        case 9:     return EISP_A_DOWN_HELMET;
        case 10:     return EISP_A_JACKET;
        case 11:    return EISP_A_PANTS;
        case 12:    return EISP_A_ARM;
        default:    return -1;
    }
}

void KGCGameInventory::SetStatDesc( void )
{
    m_pkStaticBaseTitle->InitState( true );
    m_pkStaticAttributeTitle->InitState( true );
    m_pkStaticRank->InitState( true );
    m_pkStaticBaseDesc->InitState( true );
    m_pkStaticAttributeDesc->InitState( true );
    m_pkStaticBaseStat->InitState( true );
    m_pkStaticAttributeStat->InitState( true );

    m_pkStaticRank->SetAlign( DT_CENTER );

    std::wostringstream stmTitle1, stmTitle2;
    stmTitle1 << L"#cD5D3AD" << g_pkStrLoader->GetString( STR_ID_CONTEST_HELP1 ) << L"#cX";
    m_pkStaticBaseTitle->SetText( stmTitle1.str() );

    stmTitle2 << L"#cD5D3AD" << g_pkStrLoader->GetString( STR_ID_ATTRIBUTE_INFO ) << L"#cX";
    m_pkStaticAttributeTitle->SetText( stmTitle2.str() );

    m_pkStaticBaseTitle->SetFontOutline( true, D3DCOLOR_ARGB( 255, 16, 29, 45 ) ); 
    m_pkStaticAttributeTitle->SetFontOutline( true, D3DCOLOR_ARGB( 255, 16, 29, 45 ) );
    

    std::wostringstream stmDesc1, stmDesc2;
    stmDesc1 << L"#c97C8E6" << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L"\n" << L"#cX";
    m_pkStaticBaseDesc->SetMultiLine( true );
    m_pkStaticBaseDesc->SetText( stmDesc1.str() );

    stmDesc2 << L"#c97C8E6" << g_pkStrLoader->GetString( STR_ID_SP_ATTACK_POWER ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_SP_DEFENCE_POWER ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_CRITICAL_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_COUNTER_DEFENCE_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_HP_RECOVERY_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_MP_RECOVERY_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_HP_DRAIN_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_MP_DRAIN_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_GP_PLUS_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_EXP_PLUS_RATE ) << L"\n" <<
                               g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_9 ) << L"#cX";
    m_pkStaticAttributeDesc->SetMultiLine( true );
    m_pkStaticAttributeDesc->SetLineSpace( 1.1f );
    m_pkStaticAttributeDesc->SetText( stmDesc2.str() );
}

void KGCGameInventory::GetUserStatus( IN PLAYER* pPlayer_, OUT USER_STATUS& userStatus_ )
{
    if( NULL == pPlayer_ ) { 
        return;
    }

    userStatus_.Clear();

    // 기본 캐릭터 능력치
    EAbilityData stCharacterAbility;
    pPlayer_->GetLevelAbilityMode( stCharacterAbility );

    // 장비 능력치 얻기
    EAbilityData stEquipItemAbility;
    EItemAbilityData stItemAttribute;

    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) && !( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH ) 
        && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_CAPTAIN && !SiKGCRoomManager()->IsAngelEggModeAbility()
        && !SiKGCRoomManager()->IsDotaModeAbility() )
    {
        g_MyD3D->m_kItemMgr.GetItemAttribute( pPlayer_, pPlayer_->GetCurrentChar().iCharType, false, stItemAttribute);
        g_MyD3D->m_kItemMgr.GetItemRealAbility( pPlayer_, pPlayer_->GetCurrentChar().iCharType, false, stEquipItemAbility ); 

        // 불량유저 PC방 효과등 처리
        pPlayer_->SetSpecialAbility( stCharacterAbility );
        pPlayer_->SetSpecialAbility( stEquipItemAbility );
    }

    // 공, 방, 생
    userStatus_.m_iAtk = static_cast<int>(stCharacterAbility.fEquipItemAbility[ABILITY_ATK] ) + static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_ATK]);
    userStatus_.m_iDef = static_cast<int>(stCharacterAbility.fEquipItemAbility[ABILITY_DEF] ) + static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_DEF]);
    userStatus_.m_iHP = static_cast<int>(stCharacterAbility.fEquipItemAbility[ABILITY_HP] ) + static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_HP]);

    // 필살기 공.방어력
    userStatus_.m_iSPAtk = static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_SPECIAL_ATK]);
    userStatus_.m_iSPDef = static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_SPECIAL_DEF]);

    // 치명타. 카운터 저항
    userStatus_.m_fCritical = (stEquipItemAbility.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] + stCharacterAbility.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE]) * 100.0f;
    userStatus_.m_fCounterDef = (stEquipItemAbility.fEquipItemAbility[ABILITY_COUNTER_DEF] * 100.0f); 

    if( userStatus_.m_fCounterDef >= 100.0f ) {
        userStatus_.m_fCounterDef = 100.0f;
    }

    // HP, MP 회복
    userStatus_.m_fHPRecovery = (stEquipItemAbility.fEquipItemAbility[ABILITY_HP_RECOVERY_RATE] * 100.0f); 
    userStatus_.m_fMPRecovery = (stEquipItemAbility.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE] * 100.0f);

    // HP, MP 흡수
    userStatus_.m_iHPDrain = ( static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_HP_DRAIN_RATE]) ); 
    userStatus_.m_iMPDrain = ( static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_MP_DRAIN_RATE]) );

    // GP, EXP 추가 획득
    userStatus_.m_fGPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE];
    userStatus_.m_fEXPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE];

    // 헬스피어
    userStatus_.m_iHellSpear = static_cast<int>(stEquipItemAbility.fEquipItemAbility[ABILITY_DARK_DAMAGE_ATK_POINT]);

    float fCriticalTemp = 0.0f;
    int iPromotion = static_cast< int >( pPlayer_->GetCurrentChar().iPromotionLevel );

    g_kGlobalValue.GetPromotionBonusStatus( iPromotion, userStatus_.m_iAtk, userStatus_.m_iDef, userStatus_.m_iHP, fCriticalTemp, pPlayer_ );
    userStatus_.m_fCritical += fCriticalTemp * 100.f;

    //종합 공격력 공식 = (( (1-크리티컬확률)+크리티컬확률*(1.2+크리티컬 데미지) ) * ( (공격력*27)+(( 공격력+필살기 공격력)*20) * ( 1+MP회복력 ) ))/33.75
    float fCritialDamageRate = (stCharacterAbility.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP] + stEquipItemAbility.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP])/100.0f;
    float fTotalAck = ( ((1-userStatus_.m_fCritical/100.0f)+(userStatus_.m_fCritical/100.0f)*(2.2f+fCritialDamageRate)) * ((userStatus_.m_iAtk*30)+((userStatus_.m_iAtk+userStatus_.m_iSPAtk)*25.0f)) )/33.75f;

    userStatus_.m_iTotalAtk = static_cast<int>(fTotalAck);

}

void KGCGameInventory::SetStatusStatic( bool bCompare_ /*= false*/, IN PLAYER* pComparePlayer_ /*= NULL */ )
{
    if( bCompare_ && NULL == pComparePlayer_ ) { 
        return;
    }

    m_pkStaticBaseStat->SetText( L"" );
    m_pkStaticAttributeStat->SetText( L"" );
    m_pkStaticRank->SetText( L"" );

    if( false == bCompare_ ) {
        std::wostringstream stmStat1, stmStat2;
        stmStat1 << m_kBeforeStatus.m_iAtk << L"\n" << m_kBeforeStatus.m_iDef << L"\n" << m_kBeforeStatus.m_iHP ;
        m_pkStaticBaseStat->SetText( stmStat1.str() );


        stmStat2 << m_kBeforeStatus.m_iSPAtk << L"\n" << m_kBeforeStatus.m_iSPDef << L"\n" << 
                    m_kBeforeStatus.m_fCritical << L"%\n" << m_kBeforeStatus.m_fCounterDef << L"%\n" <<
                    m_kBeforeStatus.m_fHPRecovery << L"%\n" << m_kBeforeStatus.m_fMPRecovery << L"%\n" <<
                    m_kBeforeStatus.m_iHPDrain << L"\n" << m_kBeforeStatus.m_iMPDrain << L"\n" <<
                    m_kBeforeStatus.m_fGPAdd << L"%\n" << m_kBeforeStatus.m_fEXPAdd << L"%\n" <<
                    m_kBeforeStatus.m_iHellSpear;
        m_pkStaticAttributeStat->SetText( stmStat2.str() );

        m_pkStaticRank->SetText( g_kGlobalValue.GetStatusGrade( m_kBeforeStatus.m_iTotalAtk ) );
    }
    else {
        USER_STATUS compareStatus;
        GetUserStatus( pComparePlayer_, compareStatus );

        std::wostringstream stmStat1, stmStat2;
        stmStat1 << GetStatCompareString( m_kBeforeStatus.m_iAtk, compareStatus.m_iAtk ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iDef, compareStatus.m_iDef ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iHP, compareStatus.m_iHP );
        m_pkStaticBaseStat->SetText( stmStat1.str() );

        stmStat2 << GetStatCompareString( m_kBeforeStatus.m_iSPAtk, compareStatus.m_iSPAtk ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iSPDef, compareStatus.m_iSPDef ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fCritical, compareStatus.m_fCritical, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fCounterDef, compareStatus.m_fCounterDef, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fHPRecovery, compareStatus.m_fHPRecovery, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fMPRecovery, compareStatus.m_fMPRecovery, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iHPDrain, compareStatus.m_iHPDrain ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iMPDrain, compareStatus.m_iMPDrain ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fGPAdd, compareStatus.m_fGPAdd, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_fEXPAdd, compareStatus.m_fEXPAdd, true ) << L"\n" <<
                    GetStatCompareString( m_kBeforeStatus.m_iHellSpear, compareStatus.m_iHellSpear ) ;
        m_pkStaticAttributeStat->SetText( stmStat2.str() );

        m_pkStaticRank->SetText( g_kGlobalValue.GetStatusGrade( compareStatus.m_iTotalAtk ) );
    }
}

std::wstring KGCGameInventory::GetStatCompareString( int iBeforeStat_, int iNowStat_, bool bPercent_ /*= false */ )
{
    std::wostringstream stm;
    stm << iNowStat_;
    if( bPercent_ ) { 
        stm << L"%";
    }

    if( iBeforeStat_ - iNowStat_ > 0 ) { // 능력치 떨어졌음 
        if( bPercent_ ) { 
            stm << L"#cFF0000(↓" << (iBeforeStat_ - iNowStat_) << L"%)#cX";
        }
        else { 
            stm << L"#cFF0000(↓" << (iBeforeStat_ - iNowStat_) << L")#cX";
        }
    }
    else if( iBeforeStat_ - iNowStat_ < 0 ) { // 능력치 올라갔음 
        if( bPercent_ ) { 
            stm << L"#c00FF00(↑" << (iNowStat_ - iBeforeStat_) << L"%)#cX";
        }
        else {
            stm << L"#c00FF00(↑" << (iNowStat_ - iBeforeStat_) << L")#cX";
        }
    }
    return stm.str();
}

std::wstring KGCGameInventory::GetStatCompareString( float fBeforeStat, float fNowStat_, bool bPercent_ /*= false */ )
{
    std::wostringstream stm;
    stm.setf(std::ios_base::fixed);
    
    stm << setprecision(1) << fNowStat_;
    if( bPercent_ ) { 
        stm << L"%";
    }

    if( fBeforeStat - fNowStat_ > 0.0f ) { // 능력치 떨어졌음 
        if( bPercent_ ) { 
            stm << L"#cFF0000(↓" << setprecision(1) << (fBeforeStat - fNowStat_) << L"%)#cX";
        }
        else { 
            stm << L"#cFF0000(↓" << setprecision(1) << (fBeforeStat - fNowStat_) << L")#cX";
        }
    }
    else if( fBeforeStat - fNowStat_ < 0.0f ) { // 능력치 올라갔음 
        if( bPercent_ ) {
            stm << L"#c00FF00(↑" << setprecision(1) << (fNowStat_ - fBeforeStat) << L"%)#cX";
        }
        else {
            stm << L"#c00FF00(↑" << setprecision(1) << (fNowStat_ - fBeforeStat) << L")#cX";
        }
    }

    return stm.str();
}

void KGCGameInventory::SetCharImg( int iCharIndex_ )
{
    if( iCharIndex_ < 0 || iCharIndex_ >= GC_CHAR_NUM ) { 
        return;
    }

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkCharImg[i]->ToggleRender( false );
    }

    m_apkCharImg[ iCharIndex_ ]->ToggleRender( true );
}

int KGCGameInventory::GetItemGrade( const KItem::ITEM_GRADE eGrade_ )
{
    switch( eGrade_ ) { 
        case KItem::GRADE_NORMAL:   return GRADE0;
        case KItem::GRADE_RARE:     return GRADE1;
        case KItem::GRADE_EPIC:     return GRADE2;
        case KItem::GRADE_LEGEND:   return GRADE3;
        case KItem::GRADE_MYSTIC:   return GRADE4;
        default:                    return GRADE0;
    }
}

void KGCGameInventory::OnClickBtnGrade( int iIndex_ )
{
    if( iIndex_ < GRADE0 || iIndex_ >= NUM_GRADE ) { 
        return;
    }

    m_apkBtnGradeDefault[ iIndex_ ]->ToggleRender( !m_apkBtnGradeDefault[ iIndex_ ]->IsRenderOn() );
    m_apkBtnGradeActive[ iIndex_ ]->ToggleRender( !m_apkBtnGradeActive[ iIndex_ ]->IsRenderOn() );

    RefreshItemList();
    SetPageItem();
}

void KGCGameInventory::SetItemState( void )
{
    // 아이템 리스트의 Hot/New 표시 여부 세팅 
    m_vecItemState.clear();

    if( m_vecInvenItem.empty() ) { 
        return;
    }

    // Hot 아이템 목록 
    std::set< GCITEMUID > setHotItem;
    GetHotItem( setHotItem );

    // 정보 세팅 
    for( int i=0 ; i<static_cast<int>( m_vecInvenItem.size() ) ; ++i ) {
        m_vecItemState.push_back( std::make_pair( false, false ) );

        // Hot
        std::set< GCITEMUID >::iterator sit = setHotItem.find( m_vecInvenItem[i] );
        if( sit != setHotItem.end() ) { 
            m_vecItemState[i].first = true;
            continue;
        }

        // New
        if( true == g_pItemMgr->IsNewItem( m_vecInvenItem[ i ] ) ) { 
            if( true == g_pItemMgr->CheckEquipLevel( m_pPlayer->GetCurrentChar().iLevel, m_vecInvenItem[i] ) ) {
                m_vecItemState[i].second = true;
            }
        }
    }
}

void KGCGameInventory::GetHotItem( OUT std::set< GCITEMUID >& setHotItem_ )
{
    setHotItem_.clear();
    std::map< int, GCITEMUID > mapHotItem;

    for( std::vector< GCITEMUID >::iterator vit = m_vecInvenItem.begin() ; vit != m_vecInvenItem.end() ; ++vit ) { 
        int iNowItemSlotpos = -1;
        int iNowItemAbility = -1;
        
        if( *vit <= KGCItemManager::MAXIMUM_PCBANG_UID ) {  // 피방 아이템 걸름 
            continue;
        }
        
        if( false == g_pItemMgr->GetItemUniqueSlotPosAndAbility( *vit, m_pPlayer->GetCurrentChar(), iNowItemSlotpos, iNowItemAbility ) ) { 
            continue;
        }

        std::map< int, GCITEMUID >::iterator mit = mapHotItem.find( iNowItemSlotpos );
        if( mit == mapHotItem.end() ) { 
            mapHotItem[ iNowItemSlotpos ] = *vit;
            continue;
        }

        int iSavedSlotPos = -1;
        int iSavedAbility = -1;
        g_pItemMgr->GetItemUniqueSlotPosAndAbility( mit->second, m_pPlayer->GetCurrentChar(), iSavedSlotPos, iSavedAbility );
        if( iNowItemAbility > iSavedAbility ) { 
            mapHotItem.erase( mit );
            mapHotItem[ iNowItemSlotpos ] = *vit;
        }
    }

    // 장착 슬롯에 있는 아이템과 비교 
    for( int i=0 ; i<NUM_EQUIP_SLOT ; ++i ) { 
        KItem* pInvenItem = m_apkEquipSlot[i]->GetEquipItem();
        if( NULL == pInvenItem ) { 
            continue;
        }

        int iEquipSlotPos = -1;
        int iEquipAbility = -1;
        if( false == g_pItemMgr->GetItemUniqueSlotPosAndAbility( pInvenItem->m_ItemUID, m_pPlayer->GetCurrentChar(), iEquipSlotPos, iEquipAbility ) ) { 
            continue;
        }
    
        std::map< int, GCITEMUID >::iterator mit = mapHotItem.find( iEquipSlotPos );
        if( mit == mapHotItem.end() ) { 
            continue;
        }
                
        int iSavedSlotPos = -1;
        int iSavedAbility = -1;
        g_pItemMgr->GetItemUniqueSlotPosAndAbility( mit->second, m_pPlayer->GetCurrentChar(), iSavedSlotPos, iSavedAbility );

        if( iEquipAbility > iSavedAbility ) { 
            mapHotItem.erase( mit );
        }
    }

    for( std::map< int, GCITEMUID >::iterator mit = mapHotItem.begin() ; mit != mapHotItem.end() ; ++mit ) { 
        setHotItem_.insert( mit->second );
    }
}

void KGCGameInventory::SortInvenItem( void )
{
    if( m_vecInvenItem.size() != m_vecItemState.size() ) { 
        return;
    }

    std::vector< GCITEMUID > vecHot;
    std::vector< GCITEMUID > vecNew;
    std::vector< GCITEMUID > vecNormal;

    for( int iIndex=0 ; iIndex<static_cast<int>( m_vecItemState.size() ) ; ++iIndex ) { 
        if( true == m_vecItemState[ iIndex ].first ) {      // Hot
            vecHot.push_back( m_vecInvenItem[ iIndex ] );
            continue;
        }

        if( true == m_vecItemState[ iIndex ].second ) {     // New
            vecNew.push_back( m_vecInvenItem[ iIndex ] );
            continue;
        }

        vecNormal.push_back( m_vecInvenItem[ iIndex ] );    // 아무것도 아님 
    }

    m_vecInvenItem.clear();
    m_vecItemState.clear();

    MakeItemListAndStateList( vecHot, true, false );
    MakeItemListAndStateList( vecNew, false, true );
    MakeItemListAndStateList( vecNormal, false, false );
}

void KGCGameInventory::MakeItemListAndStateList( IN const std::vector< GCITEMUID >& vecUID_, IN const bool bHot_, IN const bool bNew_ )
{
    for( std::vector< GCITEMUID >::const_iterator vit = vecUID_.begin() ; vit != vecUID_.end() ; ++vit ) { 
        m_vecInvenItem.push_back( *vit );
        m_vecItemState.push_back( std::make_pair( bHot_, bNew_ ) );
    }
}

void KGCGameInventory::ToggleRender( bool bRender )
{
    KD3DWnd::ToggleRender( bRender );
    ShowItemEffect( bRender );
}

void KGCGameInventory::ShowItemEffect( bool _bShow )
{
    std::string strParticle = "UI_NewEquip_01";

    if ( _bShow ) 
    {
        if ( IsRenderOn() )
        {
            g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
            m_vecHotEquipParticle.clear();

            for(int i=0;i< static_cast<int>( m_apkItemSlot.size() );++i)
            {
                if ( m_apkItemSlot[i] && m_apkItemSlot[i]->IsHotItem() )
                {
                    D3DXVECTOR2 vPos = m_apkItemSlot[i]->GetFixedWindowPos();

                    vPos.x += ( m_apkItemSlot[i]->GetWidth() / 2 ) + 1;
                    vPos.y += ( m_apkItemSlot[i]->GetHeight() / 2 ) + 1;


                    CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( strParticle,PARTICLEPOS_X(vPos.x) ,PARTICLEPOS_Y(vPos.y), 0.5f );
                    g_ParticleManager->SetLayer ( pParticle, GC_LAYER_UI );
                    m_vecHotEquipParticle.push_back( pParticle );
                }
            }
        }
    }
    else 
    {
        g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
        m_vecHotEquipParticle.clear();
    }
}

void KGCGameInventory::FilterSwapWeapon( void )
{
    if( m_vecInvenItem.empty() ) {
        return;
    }

    GCITEMUID swapWeaponUID = m_pPlayer->GetSwapWeaponUID();
    std::vector< GCITEMUID >::iterator vit = std::find( m_vecInvenItem.begin(), m_vecInvenItem.end(), swapWeaponUID );
    if( vit != m_vecInvenItem.end() ) { 
        m_vecInvenItem.erase( vit );
    }
}

bool KGCGameInventory::CheckItemChange( void )
{
    SUserInfo kNowEquip;
    GetCurCharacterItemList( m_pPlayer,kNowEquip );

    if( m_kBeforeEquip.vecCharInfo.size() != kNowEquip.vecCharInfo.size() ) { 
        return false;
    }

    int iCharType = m_pPlayer->GetCurrentChar().iCharType;
    GCITEMUID swapWeaponUID = m_pPlayer->GetSwapWeaponUID();

    KUserItemVector vecBeforeItem;
    KUserItemVector vecNowItem;
    
    for( KCharInfoVector::iterator it = m_kBeforeEquip.vecCharInfo.begin() ; it != m_kBeforeEquip.vecCharInfo.end() ; ++it ) {
        if( it->iCharType == iCharType ) { 
            vecBeforeItem = it->vecItemInfo;
            break;
        }
    }

    for( KCharInfoVector::iterator it = kNowEquip.vecCharInfo.begin() ; it != kNowEquip.vecCharInfo.end() ; ++it ) {
        if( it->iCharType == iCharType ) { 
            vecNowItem = it->vecItemInfo;
            break;
        }
    }

    if( vecBeforeItem.size() != vecNowItem.size() ) { 
        return true;
    }

    bool bChange = false;
    for( int i=0 ; i<static_cast<int>( vecBeforeItem.size() ) ; ++i ) { 
        if( vecBeforeItem[i].dwItemUID != vecNowItem[i].dwItemUID ) { 
            bChange = true;
            break;
        }
    }

    return bChange;
}

void KGCGameInventory::FilterNonEquipItem( void )
{
    for( std::vector< GCITEMUID >::iterator it = m_vecInvenItem.begin() ; it != m_vecInvenItem.end() ; ) { 
        GCItem* pItem = NULL;
        g_pItemMgr->FindInventory( *it, &pItem );
        
        if( NULL == pItem ) { 
            ++it;
            continue;
        }

        if( pItem->eItemKind == GIK_ENCHANT_ASIST_ITEM ) { 
            it = m_vecInvenItem.erase( it );
            continue;
        }

        ++it;
    }
}