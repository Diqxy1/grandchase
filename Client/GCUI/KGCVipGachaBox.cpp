#include "stdafx.h"
//#include "P2P.H"
#include "KGCVipGachaBox.h"
#include "../uifx/KInput.h"
#include "../uifx/D3DStatic.h"
#include "../KDSound.h"
//#include "KGCStringLoader.h"
#include "MyD3D.h"
#include "GCUI/GCCharWnd.h"
//#include "GCUIScene.h"
#include "GCItemInfoBox.h"
#include "KGCVipGachaItemBox.h"
#include "GCStateMachine.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCInfoPanel.h"


IMPLEMENT_CLASSNAME( KGCVipGachaBox );
IMPLEMENT_WND_FACTORY( KGCVipGachaBox );
IMPLEMENT_WND_FACTORY_NAME( KGCVipGachaBox, "gc_vip_gacha_box" );

KGCVipGachaBox::KGCVipGachaBox( void )
:m_pkClose( NULL )
,m_pkBack( NULL )
,m_pkCharWnd( NULL )
,m_pkNextBtn( NULL )
,m_pkPrevBtn( NULL )
,m_pkPage( NULL )
,m_pkVipBadgeNumber( NULL )
,m_pkVipBadgeDesc( NULL )
,m_pkItemInfoBox( NULL )
,m_iCharType( -1 )
,m_iCurPage( 0 )
,m_iTotalPage( 0 )
{
    LINK_CONTROL( "back",               m_pkBack );
	LINK_CONTROL( "close_btn",          m_pkClose );
    LINK_CONTROL( "character_window",   m_pkCharWnd );
    LINK_CONTROL( "btn_next_page",      m_pkNextBtn );
    LINK_CONTROL( "btn_prev_page",      m_pkPrevBtn );
    LINK_CONTROL( "static_page",        m_pkPage );
    LINK_CONTROL( "vip_badge_num",      m_pkVipBadgeNumber );
    LINK_CONTROL( "desc",               m_pkVipBadgeDesc );
    LINK_CONTROL( "item_info_box",      m_pkItemInfoBox );

    char str[256] = "";
    for ( int i = 0; i < VIP_ITEM_MAX; ++i )
    {
        m_pkItem[i] = NULL;
        sprintf( str, "item%d", i );
        LINK_CONTROL( str, m_pkItem[i] );
    }
}

KGCVipGachaBox::~KGCVipGachaBox( void )
{
}

void KGCVipGachaBox::OnCreate( void )
{
     m_pkBack->InitState( true );

     m_pkClose->InitState( true, true, this );
     m_pkClose->SetHotKey( DIK_ESCAPE );

     m_pkCharWnd->InitState( true, true, this );

     m_pkNextBtn->InitState( true, true, this );
     m_pkNextBtn->SetHotKey( DIK_RIGHT );

     m_pkPrevBtn->InitState( true, true, this );
     m_pkPrevBtn->SetHotKey( DIK_LEFT );

     m_pkPage->InitState( true );
     m_pkPage->SetAlign( DT_CENTER );

     m_pkVipBadgeNumber->InitState( true );
     m_pkVipBadgeNumber->SetAlign( DT_CENTER );
     m_pkVipBadgeNumber->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );

     m_pkVipBadgeDesc->InitState( true );
     m_pkVipBadgeDesc->SetText( g_pkStrLoader->GetString( STR_ID_VIP_GACHA_DESC1 ) );
     m_pkVipBadgeDesc->SetToolTip( g_pkStrLoader->GetString( STR_ID_VIP_GACHA_DESC2 ), 0.0f, 50.0f );

     m_pkItemInfoBox->InitState( false );
 
     for ( int i = 0; i < VIP_ITEM_MAX; ++i )
     {
         m_pkItem[i]->InitState( true, true, this );
         m_pkItem[i]->SetIndex( i );
     }
}

void KGCVipGachaBox::OnCreateComplete( void )
{
	if( g_kGlobalValue.m_kUserInfo.dwUID == 0)
		return;

	UpdateBadgeNum();
    m_pkCharWnd->UpdateMeshObject();

	m_pkCharWnd->SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
}

void KGCVipGachaBox::OnDestroyComplete( void )
{
    m_iCharType = -1;
    m_pkCharWnd->UpdateMeshObject();

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCVipGachaBox::FrameMoveInEnabledState( void )
{
    if ( o5 == 0 )
    {
        int temp = m_iCharType;
        m_iCharType = m_pkCharWnd->GetExtraNum() % GC_CHAR_NUM;

        if ( temp != m_iCharType )
        {
			m_iCurPage = 0; // 새로운 곳으로 가면 ~ 항상 현재 페이지는 초기화이다!!
            m_iTotalPage = (int)( m_ItemList[m_iCharType].size() - 1 ) / VIP_ITEM_MAX;
            RefreshPage();

			//====================================================================================
			// 2009.02.03 : Stone
			// Comment : // 뒷창의 정보들도 갱신해주자!!
			if ( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH )
			{
				g_pkUIScene->m_pkCashShopScene->UpdateUserInfo();
				g_pkUIScene->m_pkCashShopScene->Update();
			}
            else if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
            {
                g_pkUIScene->m_pkGPShopScene->UpdateUserInfo();
                g_pkUIScene->m_pkGPShopScene->Update();
            }

			if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO )
				g_pkUIScene->m_pkMyInfoScene->m_pkInfoPanel->SetUserInfo();
        }
    }

    ToggleItemInfoBox();
}

void KGCVipGachaBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

    GCWND_MSG_MAP( m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
    GCWND_MSG_MAP( m_pkPrevBtn, KD3DWnd::D3DWE_BUTTON_CLICK, PrevPage );
    GCWND_MSG_MAP( m_pkNextBtn, KD3DWnd::D3DWE_BUTTON_CLICK, NextPage );

    for ( int i = 0; i < VIP_ITEM_MAX; ++i )
    {
        if ( m_pkItem[i] == event.m_pWnd )
        {
            if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
                m_pkItem[i]->SetMouseOver( true );

            if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
                m_pkItem[i]->SetMouseOver( false );

            if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            {
                int ItemId = m_pkItem[i]->GetItemID();
                if( ItemId == -1 )	return;

                GCItem* ItemInfo = g_pItemMgr->GetItemData( (DWORD)ItemId );
                int iSetNum = ItemInfo->iSetItemNumber;

                g_KDSound.Play( "31" );
                g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);

                if ( m_pkItem[i]->IsEquiped() ) // 장착 중이라면 입혀보기 해제
                {
                    if ( ItemInfo->dwSlotPosition != ESP_SET_ITEM )
                    {
                        g_pItemMgr->UnequipItemForItemID( &g_MyD3D->m_TempPlayer.GetCurrentChar(), ItemId );
                    }
                    else
                    {
                        g_pItemMgr->UnequipSetItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), iSetNum );
                    }

                    m_pkItem[i]->SetEquiped( false );
                }
                else if( ItemInfo->dwSlotPosition != 0  )
                {
                    if ( ItemInfo->dwSlotPosition == ESP_SET_ITEM )
                    {
                        g_pItemMgr->EquipSetItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), iSetNum );
                    }
                    else
                    {
                        g_pItemMgr->EquipItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), ItemInfo );
                    }

					// 같은 슬랏 포지션에 있는 놈들은 다 벗기쟈 ㅠㅠ;;
					for ( int j = 0; j < VIP_ITEM_MAX; ++j )
					{
						int tempID = m_pkItem[j]->GetItemID();
						if ( tempID == -1 ) continue;

						GCItem* tmpItemInfo = g_pItemMgr->GetItemData( (DWORD)tempID );
						if ( i != j && ItemInfo->dwSlotPosition == tmpItemInfo->dwSlotPosition )
						{
							m_pkItem[j]->SetEquiped( false );
						}
					}

                    m_pkItem[i]->SetEquiped( true );
                }
                g_KDSound.Play( "31" );

                g_MyD3D->m_TempPlayer.AutoSetExtra();
                g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
            }
        }
    }
}

void KGCVipGachaBox::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	ReleaseCapture();
	//====================================================================================
	// 2009.02.03 : Stone
	// Comment : // 뒷창의 정보들도 갱신해주자!!
	if ( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH )
		g_pkUIScene->m_pkCashShopScene->UpdateItemView();
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
        g_pkUIScene->m_pkGPShopScene->UpdateItemView();
	if ( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO )
		g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCVipGachaBox::PostChildDraw()
{
}

void KGCVipGachaBox::SetItemList( KEVENT_VIP_ITEM_LIST_NOT kRecv )
{
    m_ItemList.clear();
    m_ItemList = kRecv;

    RefreshPage();
}

void KGCVipGachaBox::NextPage( void )
{
    if( m_iTotalPage < ( m_iCurPage + 1 ) ) return;
    ++m_iCurPage;

    RefreshPage();
}

void KGCVipGachaBox::PrevPage( void )
{
    if( 0 > ( m_iCurPage - 1 ) ) return;
    --m_iCurPage;

    RefreshPage();
}

void KGCVipGachaBox::RefreshPage( void )
{
    // 현재 페이지 세팅!
    if ( m_ItemList.count( m_iCharType ) == 0 ) return;

    for ( int i = 0; i < VIP_ITEM_MAX; ++i )
    {
        // 캐릭 바뀔때는 모든아이템 다 해제한다!
        m_pkItem[i]->SetEquiped( false );

        const int iCurIndex = m_iCurPage * VIP_ITEM_MAX + i;
        // 비는 아이템들 세팅
        if( iCurIndex >= (int)m_ItemList[m_iCharType].size() )
        {
            m_pkItem[i]->SetItemInfo( -1, -1 );
        }
        // 있는 아이템들 세팅
        else
        {
            std::map< DWORD, int >::iterator itCurItem = m_ItemList[m_iCharType].begin();
            for ( int k = 0; k < iCurIndex; ++k ) itCurItem++;       // 인덱스 만큼 더한다.

            m_pkItem[i]->SetItemInfo( (int)itCurItem->first / 10, itCurItem->second );
        }
    }

    // 페이지 세팅
    WCHAR wszPage[256];
    wsprintf( wszPage, L"%d / %d", m_iCurPage + 1, m_iTotalPage + 1 );
    m_pkPage->SetText( wszPage );
}

void KGCVipGachaBox::ToggleItemInfoBox( void )
{
    // 아놔 짜증나 이거 ㅎㅎ

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    if( m_pkBack->CheckPosInWindowBound( vMousePos ) )
    {
        bool bCheck = false;
        for(int i = 0; i < VIP_ITEM_MAX; ++i )
        {
            if( m_pkItem[i]->CheckPosInWindowBoundWithChild( vMousePos ) )
            {
                int iItemID = m_pkItem[i]->GetItemID();
                if( -1 != iItemID )
                {
                    // 아이템 정보 표시창 위치 보정.
                    D3DXVECTOR2 vecPos = m_pkItem[i]->GetFixedWindowLocalPos();
                    vecPos.x += 80;
                    vecPos.y -= ( m_pkItemInfoBox->GetHeight() / 2.0f ) - ( m_pkItem[i]->GetHeight() / 2.0f );

                    if ( vecPos.y + m_pkItemInfoBox->GetHeight() > GC_SCREEN_HEIGHT )
                        vecPos.y = (float)( GC_SCREEN_HEIGHT - m_pkItemInfoBox->GetHeight() );

                    m_pkItemInfoBox->SetWindowPosDirect( vecPos );

                    if( iItemID != m_pkItemInfoBox->GetItemID() )
                        m_pkItemInfoBox->SetItem( g_pItemMgr->GetItemData( iItemID ) );

                    m_pkItemInfoBox->ToggleRender( true );

                    bCheck = true;
                    break;
                }
            }
        }

        if( !bCheck )
            m_pkItemInfoBox->ToggleRender(false);
    }
    else
    {
        m_pkItemInfoBox->ToggleRender(false);
    }
}

void KGCVipGachaBox::UpdateBadgeNum( void )
{
    std::wstringstream strNum;
    strNum << g_MyD3D->m_kItemMgr.GetInventoryItemDuration(KGCItemManager::ITEM_GACHA_VIP);
    m_pkVipBadgeNumber->SetText( strNum.str() );
}

void KGCVipGachaBox::BuyItemProcedure( int index )
{
    m_pkItem[index]->BuyItemProcedure();
}

void KGCVipGachaBox::Dismount( void )
{
    for ( int i = 0; i < VIP_ITEM_MAX; ++i )
    {
        // 캐릭 바뀔때는 모든아이템 다 해제한다!
        m_pkItem[i]->SetEquiped( false );
    }
}