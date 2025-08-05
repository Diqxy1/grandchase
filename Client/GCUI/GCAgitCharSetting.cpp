#include "stdafx.h"
#include "PetAI.h"
#include "KGCObjectEgo.h"
#include "./animation/KGCAnimManager.h"

#include "GCAgitCharSetting.h"
#include "SlangFilter.h"
#include "KGCAgitGameManager.h"

IMPLEMENT_CLASSNAME( KGCAgitCharSetting );
IMPLEMENT_WND_FACTORY( KGCAgitCharSetting );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitCharSetting, "gc_agit_char_setting" );

KGCAgitCharSetting::KGCAgitCharSetting( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;
    m_pkWndCharView = NULL;
    LINK_CONTROL( "btnClose",   m_pkBtnClose );
    LINK_CONTROL( "btnOK",      m_pkBtnOK );
    LINK_CONTROL( "btnCancle",  m_pkBtnCancle );
    LINK_CONTROL( "wndView",    m_pkWndCharView );

    m_pkStaticActionSetting = NULL;
    m_pkStaticMentSetting = NULL;
    m_pkStaticMent0 = NULL;
    m_pkStaticMent1 = NULL;
    m_pkStaticMent2 = NULL;
    LINK_CONTROL( "staticActionSetting",    m_pkStaticActionSetting );
    LINK_CONTROL( "staticMentSetting",      m_pkStaticMentSetting );
    LINK_CONTROL( "staticMent0",            m_pkStaticMent0 );
    LINK_CONTROL( "staticMent1",            m_pkStaticMent1 );
    LINK_CONTROL( "staticMent2",            m_pkStaticMent2 );

    for( int i=0 ; i<NUM_CHAR_MENT ; ++i ) {
        m_apkEditMent[i] = NULL;

        char szMent[128] = { 0, };
        sprintf_s( szMent, 127, "editMent%d", i );
        LINK_CONTROL( szMent, m_apkEditMent[i] );
    }

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        m_apkEquipSlot[i] = NULL;
        m_apkItemSlot[i] = NULL;
        
        char szEquip[128] = { 0, };
        char szItem[128] = { 0, };

        sprintf_s( szEquip, 127, "equipSlot%d", i );
        sprintf_s( szItem, 127, "itemSlot%d", i );

        LINK_CONTROL( szEquip, m_apkEquipSlot[i] );
        LINK_CONTROL( szItem,  m_apkItemSlot[i] );
    }

    m_iItemScrollPage = 0;
    m_iItemScrollPos = 0;
    m_iBeforeItemScrollPos = 0;
    m_pkItemListScroll = NULL;
    LINK_CONTROL( "itemlistScroll", m_pkItemListScroll );

    m_vecInvenItemList.clear();

    m_pPlayer = NULL;
    m_pViewMotion = NULL;

    m_iCharType = GC_CHAR_INVALID;
    m_iMotionCount = 0;
    m_iFrame = 0;
    m_fCharPosX = 0.0f;
    m_fCharPosY = 0.0f;

    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) {
        m_apkActionSlot[i] = NULL;

        char szAction[128] = { 0, };
        sprintf_s( szAction, 127, "actionslot%d", i);
        LINK_CONTROL( szAction, m_apkActionSlot[i] );
    }

    m_iActionScrollPage = 0;
    m_iActionScrollPos = 0;
    m_iBeforeItemScrollPos = 0;
    m_iCurSelActionIndex = -1;
    m_pkActionListScroll = NULL;
    LINK_CONTROL( "actionlistScroll", m_pkActionListScroll );
}

KGCAgitCharSetting::~KGCAgitCharSetting( void )
{
}

void KGCAgitCharSetting::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkWndCharView->InitState( true );

    m_pkStaticActionSetting->InitState( true );
    m_pkStaticMentSetting->InitState( true );
    m_pkStaticMent0->InitState( true );
    m_pkStaticMent0->InitState( true );
    m_pkStaticMent0->InitState( true );
    m_pkStaticActionSetting->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_ACTION_SETTING ) );
    m_pkStaticMentSetting->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_SETTING ) );
    m_pkStaticActionSetting->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticMentSetting->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticActionSetting->SetFontSpace( 0.0f );
    m_pkStaticMentSetting->SetFontSpace( 0.0f );
    m_pkStaticMent0->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_NORMAL ) );
    m_pkStaticMent1->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_CLOSE ) );
    m_pkStaticMent2->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_CLICK ) );
    m_pkStaticMent0->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticMent1->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticMent2->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );

    for( int i=0 ; i<NUM_CHAR_MENT ; ++i ) {
        m_apkEditMent[i]->InitState( true, true, this );
        m_apkEditMent[i]->SetLimitText( 30 );
    }

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        m_apkEquipSlot[i]->InitState( true, true, this );
        m_apkItemSlot[i]->InitState( true, true, this );
    }

    m_pkItemListScroll->InitState( true, true, this );
    InitItemListScroll();

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProjection = matLook * matScale * matTran;

    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) {
        m_apkActionSlot[i]->InitState( true, true, this );
    }
    m_pkActionListScroll->InitState( true, true, this );

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCAgitCharSetting::ActionPerformed( const KActionEvent& event )
{
    // 아이템 리스트 스크롤 
    if ( event.m_pWnd == m_pkItemListScroll && 
        ( event.m_dwCode >= KD3DWnd::D3DWE_SCROLLBAR_WHEELUP || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_PRESS || event.m_dwCode == 0 ) ) { 
            OnItemListScroll();
            return;
    }

    // 모션 리스트 스크롤 
    if ( event.m_pWnd == m_pkActionListScroll && 
        ( event.m_dwCode >= KD3DWnd::D3DWE_SCROLLBAR_WHEELUP || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_PRESS || event.m_dwCode == 0 ) ) {

            OnActionListScroll();
            return;
    }

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( event.m_pWnd == m_apkItemSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
            OnClickItemSlot( i );
            break;
        }

        GCWND_MSG_MAP( m_apkEquipSlot[i],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEquipSlot );
    }

    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) { 
        if( event.m_pWnd == m_apkActionSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
            OnClickActionSlot( i );
            break;
        }
    }

    GCWND_MSG_MAP( m_pkBtnClose,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnOK );
    GCWND_MSG_MAP( m_pkBtnOK,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnOK );
    GCWND_MSG_MAP( m_pkBtnCancle,   KD3DWnd::D3DWE_BUTTON_CLICK,    Onclose );
}

void KGCAgitCharSetting::PostChildDraw( void )
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_ViewPort );
    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProjection );
    

    // 배경
    m_pkWndCharView->Render();

    // 캐릭터 출력 
    if( m_pViewMotion != NULL ) {
        if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_pPlayer->Extra_Char_Num || 
            NULL == m_pPlayer->m_pObject ) {
            int iCharType = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
            g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iCharType );
            SetViewCharacter( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType );
        }

        if ( m_iFrame > m_pViewMotion->GetNum_Frame() - 1 ) {
            m_iFrame = 0;
        }

        float fPosX = m_fCharPosX * CHAR_SCALE_RATIO;
        float fPosY = m_pViewMotion->GetFrameData(m_iFrame).Pos_y * 0.1f - 0.06f;
        float fPosZ = 0.303f;

        D3DXMatrixScaling( &matScale , 0.25f, 0.25f, 0.25f );
        D3DXMatrixRotationY( &matRot, 0.0f );
        D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

        matWorld = matScale * matRot * matTrans;

        m_pPlayer->m_pObject->SetWorldMat( &matWorld );
        m_pPlayer->m_pObject->SetFrame( &m_pViewMotion->GetFrameData( m_iFrame ) );
        g_RenderManager->Render( false ,m_pPlayer->m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
        m_iFrame++;
    }

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCAgitCharSetting::FrameMoveInEnabledState( void )
{
    if( m_strCurMotion == "NPC_cleaning_man.frm" || m_strCurMotion == "NPC_cleaning_girl.frm" ||
        m_strCurMotion == "NPC_ZZZ_man.frm" || m_strCurMotion == "NPC_ZZZ_girl.frm" ) {
            m_pPlayer->ToggleExtraMesh( "AGIT_BROOM", true );
    }
    
    if( m_pPlayer->m_pObject )
    {
        m_pPlayer->SetRender( true );
        m_pPlayer->EgoItemFrameMove( UCHAR(m_iFrame) );
        m_pPlayer->m_pObject->ChildFrameMove();
    }
}

void KGCAgitCharSetting::InitUI( int iCharType_ )
{
    if( iCharType_ <= GC_CHAR_INVALID || iCharType_ >= GC_CHAR_NUM ) {
        return;
    }

    m_iCharType = iCharType_;
    m_iCurSelActionIndex = SiKGCAgitGameManager()->GetCharacterMotionID( m_iCharType );

    SetItemList( m_iCharType );
    InitItemListScroll();
    ClearAllSlotList();
    SetItemSlotList();
    InitEquipSlotPosition();
    SetViewCharacter( m_iCharType );
    InitEquipSlotList();

    SetMotionListVector();
    InitActionListScroll();

    InitCharacterMent();
}

void KGCAgitCharSetting::OnOK( void )
{
    SaveCoordiInfo();   // 장착 코디 정보 저장
    SaveActionIndex();  // 지정 모션 정보 저장 
    SaveCharacterMent();// 대사 정보 저장 

    this->ToggleRender( false );
}

void KGCAgitCharSetting::Onclose( void )
{
    m_strCurMotion.clear();
    this->ToggleRender( false );
}

void KGCAgitCharSetting::SetItemList( int iCharType_ )
{
    ClearItemList();

    // 현재 캐릭터 장착 가능 아이템 리스트 만듬
    std::vector<GCITEMUID> vecCoordiList = g_pItemMgr->GetInventoryCoordiItemUIDList( g_pItemMgr->GetCharFlag( iCharType_ ) );
    for( std::vector< GCITEMUID >::iterator vit = vecCoordiList.begin() ; vit != vecCoordiList.end() ; ++vit ) {
        KItem* pItem = NULL;
        g_pItemMgr->FindInventory( *vit, &pItem );
        if( NULL == pItem ) { 
            continue;
        }

        // 미니펫을 코디로 만든 경우 -> 리스트에서 제외 
        if( true == g_pkPetAI->IsOldPet( pItem->m_ItemID ) ) { 
            continue;
        }

        // 사인보드를 코디로 만든경우 리스트에서 제외 
        if( true == g_AnimManager.IsItemAnim( pItem->m_ItemID ) ) {
            continue;
        }

        // 움직이는 아이템일 경우 리스트에서 제외
        if( true == SiKGCObjectEgoMgr()->IsEgoItem( pItem->m_ItemID ) ) {
            continue;
        }

        m_vecInvenItemList.push_back( pItem );
    }
}

void KGCAgitCharSetting::ClearItemList( void )
{
    m_vecInvenItemList.clear();
}

void KGCAgitCharSetting::SetItemSlotList( void )
{
    // 아이템 리스트 보고 체워 넣기 
    int iIndex = m_iItemScrollPos * COLS;
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( (iIndex + i) >= static_cast<int>( m_vecInvenItemList.size() ) ) {
            break;
        }

        m_apkItemSlot[i]->SetSlot( m_vecInvenItemList[iIndex + i]->m_ItemUID );
    }
}

void KGCAgitCharSetting::ClearItemSlotList( void )
{
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        m_apkItemSlot[i]->Clear();
    }
}

void KGCAgitCharSetting::ClearEquipSlotList( void )
{
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        m_apkEquipSlot[i]->Clear(); 
    }
}

void KGCAgitCharSetting::ClearAllSlotList( void )
{
    ClearItemSlotList();
    ClearEquipSlotList();
}

void KGCAgitCharSetting::InitItemListScroll( void )
{
    m_iItemScrollPage = 0;
    m_iItemScrollPos = 0;
    m_iBeforeItemScrollPos = 0;
    if( m_vecInvenItemList.empty() ) { 
        return;
    }

    int iNumItem = static_cast<int>( m_vecInvenItemList.size() );
    int iItemTotalRows = iNumItem / COLS;
    if( iNumItem % COLS != 0 ) { 
        ++iItemTotalRows;
    }

    int iPage = iItemTotalRows - ROWS;
    if( iPage <= 0 ) {
        m_iItemScrollPage = 1;
    }
    else {
        m_iItemScrollPage = iPage + 1;
    }

    m_pkItemListScroll->SetScrollPos( 0 );
    m_pkItemListScroll->SetScrollGap( 1 );
    m_pkItemListScroll->SetThumbYGab( 0 );
    m_pkItemListScroll->SetScrollPageSize( 1 );
    m_pkItemListScroll->SetScrollRangeMin( 0 );
    m_pkItemListScroll->SetScrollRangeMax( m_iItemScrollPage );
    m_pkItemListScroll->ResizeBtn();
}

void KGCAgitCharSetting::OnItemListScroll( void )
{
    // 현재 착용 중인 것 
    std::vector< GCITEMUID > vecEquipItem;
    GetEquipItemList( vecEquipItem );

    // 스크롤 위치 
    int iTemp = m_pkItemListScroll->GetScrollPos();

    if( m_pkItemListScroll->IsRenderOn() )
    {
        if( iTemp == 0 ) {
            m_iItemScrollPos = 0;
        }
        else if( iTemp == m_pkItemListScroll->GetScrollRangeMax() ) {
            m_iItemScrollPos = m_pkItemListScroll->GetScrollRangeMax();
        }
        else
            if( (m_iBeforeItemScrollPos>iTemp) && (m_iItemScrollPos>0) ) {
                --m_iItemScrollPos;
            }
            else if( (m_iBeforeItemScrollPos<iTemp) && (m_iItemScrollPos<m_iItemScrollPage) ) {
                ++m_iItemScrollPos;
            }
            m_iBeforeItemScrollPos = iTemp;
    }
    else if( m_iItemScrollPos != iTemp )
    {
        m_iItemScrollPos = iTemp;
        g_KDSound.Play( "73" );
    }
    ClearItemSlotList();
    SetItemSlotList();

    // 장착 표시 
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( vecEquipItem.end() != std::find( vecEquipItem.begin(), vecEquipItem.end(), m_apkItemSlot[i]->GetItemUID() ) ) {
            m_apkItemSlot[i]->SetEquipState( true );
        }
        else {
            m_apkItemSlot[i]->SetEquipState( false );
        }
    }
}

void KGCAgitCharSetting::OnClickItemSlot( const int iIndex_ )
{
    UpdateItemList( iIndex_ );    
    UpdateEquipItem();   
}

void KGCAgitCharSetting::OnClickEquipSlot( void )
{
    // 현재 착용 중인 것 
    std::vector< GCITEMUID > vecEquipItem;
    GetEquipItemList( vecEquipItem );

    // 장착 표시 
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( vecEquipItem.end() != std::find( vecEquipItem.begin(), vecEquipItem.end(), m_apkItemSlot[i]->GetItemUID() ) ) {
            m_apkItemSlot[i]->SetEquipState( true );
        }
        else {
            m_apkItemSlot[i]->SetEquipState( false );
        }
    }

    // 미리보기 
    UnequipAllItem();
    EquipItem( vecEquipItem );
}

void KGCAgitCharSetting::UpdateItemList( const int iIndex_ )
{
    if( false == m_apkItemSlot[ iIndex_ ]->GetEquipState() ) {
        return;
    }

    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        // 장착상태이고, 아이템장착부위가 같고,
        // 아이템ID가 다르거나 아이템ID 같고, UID가 다르면 빼자
        if( m_apkItemSlot[i]->GetEquipState() && 
            (m_apkItemSlot[i]->GetItemData()->dwSlotPosition == m_apkItemSlot[ iIndex_ ]->GetItemData()->dwSlotPosition) &&
            ( (m_apkItemSlot[i]->GetItemID() != m_apkItemSlot[iIndex_]->GetItemID()) ||
              ((m_apkItemSlot[i]->GetItemID() == m_apkItemSlot[iIndex_]->GetItemID()) &&
               (m_apkItemSlot[i]->GetItemUID() != m_apkItemSlot[iIndex_]->GetItemUID()))) ){
            m_apkItemSlot[i]->Onclick();
            break;
        }
    }
}

void KGCAgitCharSetting::UpdateEquipItem( void )
{
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        
        GCItem* pItem = m_apkItemSlot[i]->GetItemData();
        if( NULL == pItem ) { 
            continue;
        }

        int iEquipIndex = GetEquipSlotIndex( pItem->dwSlotPosition );
        if( -1 == iEquipIndex ) {
            continue;
        }

        if( m_apkItemSlot[i]->GetEquipState() ) {   // 장착 
            m_apkEquipSlot[ iEquipIndex ]->Clear();
            m_apkEquipSlot[ iEquipIndex ]->SetSlot( m_apkItemSlot[i]->GetItemUID() );
        }
        else {  // 해제 
            if( m_apkEquipSlot[ iEquipIndex ]->GetItemUID() == m_apkItemSlot[i]->GetItemUID() ) {
                m_apkEquipSlot[ iEquipIndex ]->Clear();
            }
        }
    }

    // 미리보기 
    std::vector< GCITEMUID > vecItemUID;
    GetEquipItemList( vecItemUID );
    UnequipAllItem();
    EquipItem( vecItemUID );
}

void KGCAgitCharSetting::InitEquipSlotPosition( void )
{
    m_apkEquipSlot[ SLOT_HELMET ]->SetSlotPosition( ESP_HELMET );
    m_apkEquipSlot[ SLOT_JACKET ]->SetSlotPosition( ESP_JACKET );
    m_apkEquipSlot[ SLOT_PANTS ]->SetSlotPosition( ESP_PANTS );
    m_apkEquipSlot[ SLOT_GLOVES ]->SetSlotPosition( ESP_GLOVES );
    m_apkEquipSlot[ SLOT_SHOES ]->SetSlotPosition( ESP_SHOES );
    m_apkEquipSlot[ SLOT_MANTLE ]->SetSlotPosition( ESP_MANTLE );
    m_apkEquipSlot[ SLOT_A_UPPER_HELMET ]->SetSlotPosition( ESP_A_UPPER_HELMET );
    m_apkEquipSlot[ SLOT_A_DOWN_HELMET ]->SetSlotPosition( ESP_A_DOWN_HELMET );
    m_apkEquipSlot[ SLOT_A_JACKET ]->SetSlotPosition( ESP_A_JACKET );
    m_apkEquipSlot[ SLOT_A_PANTS ]->SetSlotPosition( ESP_A_PANTS );
    m_apkEquipSlot[ SLOT_A_ARM ]->SetSlotPosition( ESP_A_ARM );
    m_apkEquipSlot[ SLOT_A_SHOES ]->SetSlotPosition( ESP_A_SHOES );
}

void KGCAgitCharSetting::InitEquipSlotList( void )
{
    KAgitCharacterPos charInfo = SiKGCAgitGameManager()->GetAgitCharacter( m_iCharType );

    for( std::map<DWORD,KSimpleCoordiItem>::iterator mit = charInfo.m_mapCoordi.begin() ; mit != charInfo.m_mapCoordi.end() ; ++mit ) { 
        m_apkEquipSlot[ GetEquipSlotIndex( mit->first ) ]->SetSlot( mit->second.m_ItemUID );
    }
    
    OnClickEquipSlot();
}

int KGCAgitCharSetting::GetEquipSlotIndex( DWORD dwSlotPosition_ )
{
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( m_apkEquipSlot[i]->GetSlotPosition() & dwSlotPosition_ ) {
            return i;
        }
    }

    return -1;
}

void KGCAgitCharSetting::SetViewCharacter( int iCharType_ )
{
    m_pPlayer = &(g_MyD3D->m_TempPlayer);
    m_pPlayer->m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    m_pPlayer->m_kUserInfo.SetCurrentChar( iCharType_ );
    m_pPlayer->AutoSetExtra();

    m_iMotionCount = 0;
    m_iFrame = 0;
    m_fCharPosX = 0.0f;
    m_fCharPosY = 0.0f;
    
    // 모션
    SetViewMotion();

    D3DXVECTOR2 vPos = m_pkWndCharView->GetFixedWindowLocalPos();
    m_ViewPort.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X * GC_SCREEN_DIV_WIDTH + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_ViewPort.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y * GC_SCREEN_DIV_HEIGHT + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_ViewPort.Width = static_cast<DWORD>( VIEWPORT_WIDTH * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX() );
    m_ViewPort.Height = static_cast<DWORD>( VIEWPORT_HEIGHT * GC_SCREEN_DIV_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY() );

    m_ViewPort.MinZ = 0.0f;
    m_ViewPort.MaxZ = 1.0f;

    UnequipAllItem();
}

void KGCAgitCharSetting::UpdateMeshObject( void )
{
    m_pPlayer->ReleasePlayerResource();
    m_pPlayer->SetMyPlayer( -1, true );
    m_pPlayer->UpdatePlayerResource( true );

    if( m_pPlayer->m_pObject ) {
        m_pPlayer->m_pObject->SetShellMatrixScale( 0.25f );
    }

    if( m_strCurMotion == "NPC_cleaning_man.frm" || m_strCurMotion == "NPC_cleaning_girl.frm" ) { 
        m_pPlayer->SetChildMeshMotion( "AGIT_BROOM", "NPC_broom.frm", true, true, false, true );
    }
    else if( m_strCurMotion == "NPC_ZZZ_man.frm" || m_strCurMotion == "NPC_ZZZ_girl.frm" ) {
        int iCharSex = g_kGlobalValue.IsSexCharacter( m_pPlayer->GetCurrentChar().iCharType );
        if( SEX_MAN == iCharSex ) { 
            m_pPlayer->SetChildMeshMotion( "AGIT_BROOM", "NPC_broom_ZZZ_man.frm", true, true, false, true );
        }
        else {
            m_pPlayer->SetChildMeshMotion( "AGIT_BROOM", "NPC_broom_ZZZ_girl.frm", true, true, false, true );
        }
    }
}

void KGCAgitCharSetting::EquipItem( std::vector< GCITEMUID >& vecItemList_ )
{
    if( NULL == m_pPlayer->m_pObject ) { 
        return;
    }

    if( vecItemList_.empty() ) { 
        return;
    }

    for( std::vector< GCITEMUID >::iterator vit = vecItemList_.begin() ; vit != vecItemList_.end() ; ++vit ) { 
        
        GCItem* pItem = NULL;
        g_pItemMgr->FindInventory( *vit, &pItem );
        if( NULL == pItem ) { 
            continue;
        }


        KItem* pKItem = NULL;
        g_pItemMgr->FindInventory( *vit, &pKItem );
        if( NULL == pKItem ) { 
            continue;
        }

        if( false == pKItem->CheckItemType( KItem::TYPE_LOOK ) ) {
            continue;
        }

        g_pItemMgr->EquipInventoryItem(&( m_pPlayer->GetCurrentChar() ), *vit, true );
    }

    g_pItemMgr->UnEquipWeapon( &( m_pPlayer->GetCurrentChar() ) );


    UpdateMeshObject();
}

void KGCAgitCharSetting::UnequipAllItem( void )
{
    SCharInfo charInfo = m_pPlayer->GetCurrentChar();

    // 장비
    for( std::vector< SUserItem >::iterator vit = charInfo.vecItemInfo.begin() ; vit != charInfo.vecItemInfo.end() ; ++vit ) {
        g_pItemMgr->UnequipItemForItemUID( &( m_pPlayer->GetCurrentChar() ), vit->dwItemUID );
    }

    // 코디 
    for( std::vector< SUserItem >::iterator vit = charInfo.vecLookInfo.begin() ; vit != charInfo.vecLookInfo.end() ; ++vit ) { 
        g_pItemMgr->UnequipItemForItemUID( &( m_pPlayer->GetCurrentChar() ), vit->dwItemUID );
    }

    // 무기 
    g_pItemMgr->UnEquipWeapon( &( m_pPlayer->GetCurrentChar() ) );

    UpdateMeshObject();
}

bool KGCAgitCharSetting::GetEquipItemList( OUT std::vector< GCITEMUID >& vecItemList_ )
{
    vecItemList_.clear();
    for( int i=0 ; i<NUM_ITEM_SLOT ; ++i ) {
        if( 0 != m_apkEquipSlot[i]->GetItemUID() ) { 
            vecItemList_.push_back( m_apkEquipSlot[i]->GetItemUID() );
        }
    }

    if( vecItemList_.empty() )
        return false;

    return true;
}

void KGCAgitCharSetting::SaveCoordiInfo( void )
{
    std::vector< GCITEMUID > vecItemUID;
    GetEquipItemList( vecItemUID );

    std::map< DWORD, GCITEMUID > mapEquipInfo;
    for( std::vector< GCITEMUID >::iterator vit = vecItemUID.begin(); vit != vecItemUID.end() ; ++vit ) { 
        GCItem* pItem = NULL;
        g_pItemMgr->FindInventory( *vit, &pItem );
        if( NULL == pItem ) { 
            continue;
        }

        mapEquipInfo[ pItem->dwSlotPosition ] = *vit;
    }

    // 이전 정보와 비교해서 같으면 안보낸다 
    bool bDiff = false;
    KAgitCharacterPos beforeInfo = SiKGCAgitGameManager()->GetAgitCharacter( m_iCharType );

    if( beforeInfo.m_mapCoordi.empty() ) {
        bDiff = true; 
    }
    else if( mapEquipInfo.size() != beforeInfo.m_mapCoordi.size() ) {
        bDiff = true;
    }
    else {
        for( std::map<DWORD,KSimpleCoordiItem>::iterator mit = beforeInfo.m_mapCoordi.begin() ; mit != beforeInfo.m_mapCoordi.end() ; ++mit ) { 
            std::map< DWORD, GCITEMUID >::iterator findIter = mapEquipInfo.find( mit->first );
            if( findIter == mapEquipInfo.end() ||
                findIter->second != mit->second.m_ItemUID ) {
                    bDiff = true;
                    break;
            }
        }
    }

    if( true == bDiff ) {
        KP2P::GetInstance()->Send_SetAgitCharCoordiReq( m_iCharType, mapEquipInfo );
    }
}

void KGCAgitCharSetting::SetMotionListVector( void )
{
    m_vecCharacterMotionList.clear();
    std::vector< KAgitCharacterMotion > vecMotionInfo = SiKGCAgitGameManager()->GetCharMotionList();

    for( std::vector< KAgitCharacterMotion >::iterator vit = vecMotionInfo.begin() ; vit != vecMotionInfo.end() ; ++vit ) { 
        m_vecCharacterMotionList.push_back( std::make_pair( vit->m_iIndex, vit->m_iStringID ) );
    }
}

void KGCAgitCharSetting::InitActionListScroll( void )
{
    m_iActionScrollPage = 0;
    m_iActionScrollPos = 0;
    m_iBeforeActionScrollPos = 0;
    
    if( m_vecCharacterMotionList.empty() ) { 
        return;
    }

    int iNumMotion = static_cast<int>( m_vecCharacterMotionList.size() );
    int iPage = iNumMotion - NUM_ACTION_SLOT;

    if( iPage <= 0 ) {
        m_iActionScrollPage = 1;
    }
    else { 
        m_iActionScrollPage = iPage + 1;
    }

    m_pkActionListScroll->SetScrollPos( 0 );
    m_pkActionListScroll->SetScrollGap( 1 );
    m_pkActionListScroll->SetThumbYGab( 0 );
    m_pkActionListScroll->SetScrollPageSize( 1 );
    m_pkActionListScroll->SetScrollRangeMin( 0 );
    m_pkActionListScroll->SetScrollRangeMax( m_iActionScrollPage );
    m_pkActionListScroll->ResizeBtn();    

    UpdateActionList( m_iActionScrollPos );
}   

void KGCAgitCharSetting::OnActionListScroll( void )
{
    // 스크롤 위치 
    int iTemp = m_pkActionListScroll->GetScrollPos();

    if( m_pkActionListScroll->IsRenderOn() )
    {
        if( iTemp == 0 ) {
            m_iActionScrollPos = 0;
        }
        else if( iTemp == m_pkActionListScroll->GetScrollRangeMax() ) {
            m_iActionScrollPos = m_pkActionListScroll->GetScrollRangeMax();
        }
        else
            if( (m_iBeforeActionScrollPos>iTemp) && (m_iActionScrollPos>0) ) {
                --m_iActionScrollPos;
            }
            else if( (m_iBeforeActionScrollPos<iTemp) && (m_iActionScrollPos<m_iActionScrollPage) ) {
                ++m_iActionScrollPos;
            }
            m_iBeforeActionScrollPos = iTemp;
    }
    else if( m_iActionScrollPos != iTemp )
    {
        m_iActionScrollPos = iTemp;
        g_KDSound.Play( "73" );
    }

    UpdateActionList( m_iActionScrollPos );
}

void KGCAgitCharSetting::UpdateActionList( int iIndex_ )
{
    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) { 
        m_apkActionSlot[i]->Clear();
    }

    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) {
        if( iIndex_ + i >= static_cast<int>( m_vecCharacterMotionList.size() ) ) {
            break;
        }

        m_apkActionSlot[i]->SetSlot( m_vecCharacterMotionList[ iIndex_ + i ].first, m_vecCharacterMotionList[ iIndex_ + i ].second );
        
        if( m_iCurSelActionIndex == m_apkActionSlot[i]->GetActionIndex() ) {
            m_apkActionSlot[i]->SetActive( true );
        }
    }
}

void KGCAgitCharSetting::OnClickActionSlot( int iIndex_ )
{
    if( m_apkActionSlot[iIndex_]->IsActiveSlot() ) { 
        return;
    }

    for( int i=0 ; i<NUM_ACTION_SLOT ; ++i ) { 
        m_apkActionSlot[i]->SetActive( false );
    }
    
    m_apkActionSlot[iIndex_]->SetActive( true );
    m_iCurSelActionIndex = m_apkActionSlot[iIndex_]->GetActionIndex();
    SetViewMotion();
    
    m_pPlayer->ToggleExtraMesh( "AGIT_BROOM", false );
    UpdateMeshObject();
}

void KGCAgitCharSetting::SaveActionIndex( void )
{
    int iBeforeAction = SiKGCAgitGameManager()->GetCharacterMotionID( m_iCharType );
    
    if( iBeforeAction == m_iCurSelActionIndex ) {
        return;
    }

    KP2P::GetInstance()->Send_SetAgitCharMotionReq( m_iCharType, m_iCurSelActionIndex );
}

void KGCAgitCharSetting::SetViewMotion( void )
{
    std::string strMotion;
    if( true == SiKGCAgitGameManager()->GetAgitCharacterMotion( m_iCharType, m_iCurSelActionIndex, strMotion ) ) {
        m_pViewMotion = g_pGCDeviceManager2->CreateMotionFRM( strMotion );
    }
    else {
        int iCharSex = g_kGlobalValue.IsSexCharacter( m_iCharType );
        strMotion = SEX_MAN == iCharSex ? "NPC_wait_man.frm" : "NPC_wait_girl.frm" ;
        m_pViewMotion = g_pGCDeviceManager2->CreateMotionFRM( strMotion );
        m_iCurSelActionIndex = 0;
    }

    m_strCurMotion = strMotion;
}

void KGCAgitCharSetting::InitCharacterMent( void )
{
    for( int i=0 ; i<NUM_CHAR_MENT ; ++i ) {
        InitCharacterMentEdit( i );
    }
}

void KGCAgitCharSetting::InitCharacterMentEdit( int iMentType_ )
{
    if( iMentType_ < 0 || iMentType_ >= NUM_CHAR_MENT ) {
        iMentType_ = 0;
    }

    std::wstring strMent = SiKGCAgitGameManager()->GetAgitCharacterMent( m_iCharType, iMentType_ );
    if( strMent.empty() ) {
        m_apkEditMent[ iMentType_ ]->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_DEFAULT0 + iMentType_ ).c_str() );
    }
    else {
        m_apkEditMent[ iMentType_ ]->SetText( strMent.c_str() );
    }
}

void KGCAgitCharSetting::SaveCharacterMent( void )
{
    std::map< USHORT, std::wstring > mapMent;

    bool bDiff = false;
    for( int i=0 ; i<NUM_CHAR_MENT ; ++i ) { 
        std::wstring strBeforeMent = SiKGCAgitGameManager()->GetAgitCharacterMent( m_iCharType, i );
        std::wstring strNowMent = m_apkEditMent[i]->GetText();

        if( false == SiKSlangFilter()->FilterCheck( strNowMent ) ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_CHAR_MENT_SET_FAIL11 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
            return;
        }

        if( false == bDiff && strBeforeMent != strNowMent ) { 
            bDiff = true;
        }

        mapMent[ static_cast<USHORT>(i) ] = strNowMent;
    }

    if( bDiff ) { 
        KP2P::GetInstance()->Send_SetAgitCharSpeechReq( m_iCharType, mapMent );
    }
}

void KGCAgitCharSetting::InitMent( void )
{
    InitCharacterMent();
    SaveCharacterMent();// 대사 정보 저장 
}
