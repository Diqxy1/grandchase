#include "stdafx.h"
#include "GCshopItemBox.h"
#include "GCShopItemWnd.h"
#include "GCSetItemWnd.h"

//
//
//
#include "../MyD3D.h"


//

//

IMPLEMENT_CLASSNAME( KGCShopItemBox );
IMPLEMENT_WND_FACTORY( KGCShopItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCShopItemBox, "gc_shop_item_box" );

KGCShopItemBox::KGCShopItemBox( void ) : m_kCurInfoItem(NULL)
{
	m_bSetItemView  = false;
	
	m_pkScrollBtnFirst	= NULL;
	m_pkScrollBtnPrev	= NULL;
	m_pkScrollBtnNext	= NULL;
	m_pkScrollBtnLast	= NULL;
	m_pkStaticPage		= NULL;

	m_iTotalPage = 0;
	m_iCurrentPage = 0;

    for( int i = 0 ; i < (int)m_apItemWnd.size() ; ++i )
    {
        m_apItemWnd[i] = NULL;
    }
    for( int i = 0 ; i < (int)m_apSetItemWnd.size() ; ++i )
    {
        m_apSetItemWnd[i] = NULL;
    }
	
	LINK_CONTROL( "page_first",		m_pkScrollBtnFirst	);
	LINK_CONTROL( "page_prev",		m_pkScrollBtnPrev	);
	LINK_CONTROL( "page_next",		m_pkScrollBtnNext	);
	LINK_CONTROL( "page_last",		m_pkScrollBtnLast	);
	LINK_CONTROL( "page_current",   m_pkStaticPage		);
	
	m_kCurInfoItemKey =-1;
}

KGCShopItemBox::~KGCShopItemBox( void )
{
	// empty
}

void KGCShopItemBox::OnActivateComplete( void )
{
	for ( UINT i = 0; i < NUM_ITEM_WND; ++i )
	{
		m_apItemWnd[i]->Activate( false );
	}

	for ( UINT i = 0; i < NUM_SET_ITEM_WND; ++i )
	{
		m_apSetItemWnd[i]->Activate( false );
	}
}

void KGCShopItemBox::OnCreate( void )
{
	SetSelfInputCheck( true );

	std::vector<KD3DWnd*> vecItemWndList = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_cash_shop_scene.stg", "", "cash_shop_scene\\shop_item_box\\box_item", true, NUM_ITEM_WND );
	if (!vecItemWndList.empty())
	{
		std::vector<KD3DWnd*>::iterator iterItemWnd = vecItemWndList.begin();
		DWORD dwWidth = (*iterItemWnd)->GetWidth();
		DWORD dwHeight = (*iterItemWnd)->GetHeight();
		for (int i = 0 ;iterItemWnd!=vecItemWndList.end() ; ++iterItemWnd , i++)
		{
			m_apItemWnd[i] = static_cast<KGCShopItemWnd*>(*iterItemWnd);
			m_apItemWnd[i]->SetIndex( i );
			m_apItemWnd[i]->SetSelfInputCheck(true);
			m_apItemWnd[i]->AddActionListener(this);
			D3DXVECTOR2 vPos;
			vPos.x = static_cast<float>(START_GAP + (i%MAX_ITEMWND_ROW)*(dwWidth+BETWEEN_X_GAP));
			vPos.y = static_cast<float>(START_GAP + (i/MAX_ITEMWND_ROW)*(dwHeight+BETWEEN_Y_GAP));
			m_apItemWnd[i]->SetFixedWindowLocalPos(vPos);
		}
	}

	

	std::vector<KD3DWnd*> vecSetItemWndList = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_cash_shop_scene.stg", "", "cash_shop_scene\\shop_item_box\\set_item", true, NUM_SET_ITEM_WND );
	if (!vecSetItemWndList.empty())
	{
		std::vector<KD3DWnd*>::iterator iterSetItemWnd = vecSetItemWndList.begin();
		DWORD dwWidth = (*iterSetItemWnd)->GetWidth();
		DWORD dwHeight = (*iterSetItemWnd)->GetHeight();
		for (int i = 0 ;iterSetItemWnd!=vecSetItemWndList.end() ; ++iterSetItemWnd , i++)
		{
			m_apSetItemWnd[i] = static_cast<KGCSetItemWnd*> (*iterSetItemWnd);

			m_apSetItemWnd[i]->SetIndex( i );
			m_apSetItemWnd[i]->InitState( false, true, this );

			D3DXVECTOR2 vPos;
			vPos.x =static_cast<float> (START_GAP + (i%MAX_ITEMWND_ROW)*(dwWidth+BETWEEN_X_GAP));
			vPos.y =static_cast<float> (START_GAP + (i/MAX_ITEMWND_ROW)*(dwHeight+BETWEEN_Y_GAP + 8)); //시즌 5 UI 개편 이후 간격 조절 
			m_apSetItemWnd[i]->SetFixedWindowLocalPos(vPos);
		}
	}
	
	m_pkScrollBtnFirst->InitState(true, true, this);
	m_pkScrollBtnPrev->InitState(true, true, this);
	m_pkScrollBtnNext->InitState(true, true, this);
	m_pkScrollBtnLast->InitState(true, true, this);

	m_pkStaticPage->SetAlign( DT_CENTER );

}

void KGCShopItemBox::FrameMoveInEnabledState( void )
{
}

void KGCShopItemBox::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if ( event.m_pWnd == m_pkScrollBtnFirst )
		{
			m_iCurrentPage = 0;
			SetPage(m_iCurrentPage);
			return;
		}
		if ( event.m_pWnd == m_pkScrollBtnLast )
		{
			m_iCurrentPage = m_iTotalPage - 1;			
			SetPage(m_iCurrentPage);
			return;
		}
		if ( event.m_pWnd == m_pkScrollBtnPrev )
		{
			m_iCurrentPage -= 1;
			if( m_iCurrentPage < 0 ) 
				m_iCurrentPage = 0;
			SetPage(m_iCurrentPage);
			return;
		}
		if( event.m_pWnd == m_pkScrollBtnNext )
		{
			m_iCurrentPage += 1;
			if( m_iCurrentPage > (m_iTotalPage-1)) 
				m_iCurrentPage = m_iTotalPage-1;
			SetPage(m_iCurrentPage);
			return;
		}
	}
	
	/*
	if( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{		
		int iItemWnd = -1;

		for(int i = 0 ; i < NUM_ITEM_WND; i++)
		{
			if( event.m_pWnd == m_apItemWnd[i] )
			{
				iItemWnd = i;
				break;
			}		
		}

		if(iItemWnd != -1)
		{
			for ( UINT i = 0; i < NUM_ITEM_WND; ++i )
			{
				m_apItemWnd[i]->Activate( i == iItemWnd );	
				Equip( i , IsEquip(i));
			}			
		}
	}
	*/

	for ( UINT i = 0; i < NUM_ITEM_WND; ++i )
	{		
		if(m_apItemWnd[i] == event.m_pWnd)
		{
			if( event.m_dwCode == D3DWE_BUTTON_CLICK )
			{
				GCItem* ItemInfo = m_apItemWnd[i]->GetItemInfo();
				int ItemId = m_apItemWnd[i]->GetItemID();
				if( ItemId == -1 )	return;

				if( ItemInfo->eItemKind == GIK_MISSION_SCROLL )
				{
					//g_pkUIScene->m_pkMissionDlg->SetReviewMode( ItemId * 10 );
					//g_pkUIScene->MessageBox( KGCUIScene::GC_MISSION_BOX );
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, ItemId * 10, true );
					return;
				}

                // 먼저 해당 아이템이 패키지인지 판별을 한다
                bool bPackage = g_pItemMgr->IsPackageItemID( static_cast< GCITEMID >( ItemId ) * 10 );
                std::vector< KDropItemInfo > vecItems;
                g_pItemMgr->GetPackageElements( static_cast< GCITEMID >( ItemId ) * 10, vecItems );

				if ( IsEquip( i ) ) // 장착 중이라면 입혀보기 해제
				{
                    if ( bPackage )     // 패키지면 구성품 다 벗기자
                    {
                        std::vector< KDropItemInfo >::iterator vecIter = vecItems.begin();
                        for ( ; vecIter != vecItems.end(); ++vecIter ) {
                            g_pItemMgr->UnequipItemForItemID( &g_MyD3D->m_TempPlayer.GetCurrentChar(), vecIter->m_ItemID / 10 );
                        }
                    }
                    else     // 일반 장비이면 해당 장비만 벗기자
                    {
					    g_pItemMgr->UnequipItemForItemID( &g_MyD3D->m_TempPlayer.GetCurrentChar(), ItemId );
                    }
					m_apItemWnd[i]->Equip( false );
				}
				else if ( ItemInfo->dwSlotPosition != 0 )
				{
					g_pItemMgr->EquipItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), ItemInfo ); // 요거랑(@1)
					
					UpdateEquip();      // 요 함수 안에서 m_apItemWnd[]에 Equip() 호출하여 장착정보를 풀로 갱신하고 있다
				}
                else if ( bPackage )    // 패키지면 구성품 다 입히자
                {
                    std::vector< KDropItemInfo >::iterator vecIter = vecItems.begin();
                    for ( ; vecIter != vecItems.end(); ++vecIter ) {

                        // 요거 사용하면.. 패키지에 장착 불가능한 장비가 포함된 경우 착용불가 메시지창이 뜬다.. 물론 나머지는 입어짐
                        //g_pItemMgr->EquipShopItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), vecIter->first.m_ItemID / 10 );

                        GCItem* pkInnerItem = g_pItemMgr->GetItemData( vecIter->m_ItemID / 10 );
                        CONTINUE_NIL( pkInnerItem );

                        SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.GetCurrentChar();
                        if ( g_pItemMgr->GetUniqueChartype( pkInnerItem->dwCharType ) != pCharInfo->iCharType ) {
                            // 지금 선택중인 캐릭터의 장비가 아니면 입히지 않는다
                            // 공용이면 pkInnerItem->dwCharType이 32767로 넘어온다
                            // GCItem.h의 ECHARTYPE 참조
                            continue;
                        }

                        if ( pkInnerItem != NULL && pkInnerItem->dwSlotPosition != 0 ) {
                            g_pItemMgr->EquipItem( pCharInfo, pkInnerItem, false, 0, pkInnerItem->eItemKind == GIK_COORDI_ITEM );  // 요거(@2) 에서 우측하단 캐릭터뷰에 옷입힌다
                        }
                    }
					UpdateEquip();
				}

				g_KDSound.Play( "31" );

				g_MyD3D->m_TempPlayer.AutoSetExtra();
                g_MyD3D->m_TempPlayer.RemovePlayerObject();
				g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
			}

			return;
		}		
	}

	for( int i = 0; i < NUM_SET_ITEM_WND; ++i )
	{
		if( m_apSetItemWnd[i] == event.m_pWnd )
		{
			if( event.m_dwCode == D3DWE_BUTTON_CLICK )
			{
				GCItem* ItemInfo = m_apSetItemWnd[i]->GetItemInfo();
				int ItemId = m_apSetItemWnd[i]->GetItemID();
				if( ItemId == -1 )	return;
				int iSetNum = ItemInfo->iSetItemNumber;
		
				if( !m_apSetItemWnd[i]->IsEquip())
				{
					g_pItemMgr->EquipSetItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(),iSetNum);
					//g_MyD3D->m_kItemMgr.EquipItem( g_MyD3D->m_TempPlayer.m_UserInfo.iSelectChar, m_eCharType, iNum , 0, 10, false );
				}
				else
				{
					g_pItemMgr->UnequipSetItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(),iSetNum);
					//g_MyD3D->m_kItemMgr.UnequipItem( m_eCharType, iNum );
				}

				UpdateEquip();
				g_KDSound.Play( "31" );
                g_MyD3D->m_TempPlayer.RemovePlayerObject();
				g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
			}
			return;		
		}
	}
}

GCItem*& KGCShopItemBox::GetItemInfo( int iIndex )
{
	if ( m_bSetItemView )
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_SET_ITEM_WND );
		return m_apSetItemWnd[iIndex]->GetItemInfo();
	}
	else
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_ITEM_WND );
		return m_apItemWnd[iIndex]->GetItemInfo();
	}
}

DWORD KGCShopItemBox::GetItemRecomInfo( int iIndex )
{
	if ( m_bSetItemView )
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_SET_ITEM_WND );
		return g_pItemMgr->GetItemRecomData(m_apSetItemWnd[iIndex]->GetItemInfo()->dwGoodsID) ;
	}
	else
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_ITEM_WND );
		return g_pItemMgr->GetItemRecomData(m_apItemWnd[iIndex]->GetItemInfo()->dwGoodsID);
	}
}


void KGCShopItemBox::SetSetItemView( bool bSetItemView )
{
	m_bSetItemView = bSetItemView;

	for ( UINT i = 0; i < NUM_ITEM_WND; ++i )
	{
		m_apItemWnd[i]->SetSelfInputCheck( !m_bSetItemView );
		m_apItemWnd[i]->ToggleRender( !m_bSetItemView );
	}

	for ( int i = 0; i < NUM_SET_ITEM_WND; ++i )
	{
		m_apSetItemWnd[i]->SetSelfInputCheck( m_bSetItemView );
		m_apSetItemWnd[i]->ToggleRender( m_bSetItemView );
	}

	
}

void KGCShopItemBox::ResetScrollbar( int iPos )
{
	//m_pkScrollbar->SetScrollPos( iPos );
}

bool KGCShopItemBox::IsEquip( int iIndex ) const
{
	if ( m_bSetItemView )
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_SET_ITEM_WND );
		return m_apSetItemWnd[iIndex]->IsEquip();
	}
	else
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_ITEM_WND );
		return m_apItemWnd[iIndex]->IsEquip();
	}
}
/*
void KGCShopItemBox::Equip( int iIndex, bool bEquip )
{
	if ( m_bSetItemView )
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_SET_ITEM_WND );
		m_apSetItemWnd[iIndex]->Equip( bEquip );
	}
	else
	{
		ASSERT( iIndex >= 0 && iIndex < NUM_ITEM_WND );
		m_apItemWnd[iIndex]->Equip( bEquip );
	}
}
*/
void KGCShopItemBox::SetShopItemView( EGCCharType CharType, ShopTabType TabType,
									 int iCategory, int iSortType, int iMoneyType,
									 const WCHAR* strName_ )
{
	m_eCharType = CharType;
	//어떤 종류 아이템? 
	g_MyD3D->m_kItemMgr.MakeShopItemList(CharType, TabType, iCategory, iSortType, iMoneyType, strName_ );

	m_bSetItemView = (TabType == TAB_SET);

	//Scroll초기화 pos = 0, 최대 몇 페이지까지인가?... 페이지 셋팅...
	GetTotalPage();

	for(int i = 0; i < NUM_SET_ITEM_WND; i++)
	{	
		m_apSetItemWnd[i]->ToggleRender( m_bSetItemView );
	}

	for( int i = 0; i < NUM_ITEM_WND; i++)
	{
		m_apItemWnd[i]->ToggleRender(!m_bSetItemView);
	}

	m_iCurrentPage = 0;
	SetPage(m_iCurrentPage);
}

void KGCShopItemBox::RefreshList()
{
    SetPage(m_iCurrentPage);
}

//각 아이템윈도우에 알맞는 아이템정보를 넣어주자...
void KGCShopItemBox::SetPage(int iPage)
{
	int iItemIndex;

	if(!m_bSetItemView)
	{	
		for(int i = 0; i < NUM_ITEM_WND; i++)
		{	
			iItemIndex = iPage * NUM_ITEM_WND + i;
			//페이지도 적용해야함..
			if( iItemIndex < g_MyD3D->m_kItemMgr.GetNumShopItem() )
				m_apItemWnd[i]->SetItemInfo(iItemIndex);
			else 
				m_apItemWnd[i]->SetItemInfo(-1);
		}
	}
	else
	{		
		for(int i = 0; i < NUM_SET_ITEM_WND; i++)
		{	
			iItemIndex = iPage * NUM_SET_ITEM_WND + i;
			//페이지도 적용해야함..
			if( iItemIndex < g_MyD3D->m_kItemMgr.GetNumShopItem() )
				m_apSetItemWnd[i]->SetItemInfo(iItemIndex);
			else 
				m_apSetItemWnd[i]->SetItemInfo(-1);
		}
	}
	UpdateEquip();

	UpdateMarks();
	
	WCHAR temp[20] = {0,};
	swprintf( temp, 10, L"%2d / %2d", m_iCurrentPage + 1 , m_iTotalPage);
	m_pkStaticPage->SetText( temp );

	m_pkScrollBtnFirst->Lock(m_iCurrentPage == 0);
	m_pkScrollBtnPrev->Lock(m_iCurrentPage == 0);
	m_pkScrollBtnNext->Lock(m_iCurrentPage == m_iTotalPage - 1);
	m_pkScrollBtnLast->Lock(m_iCurrentPage == m_iTotalPage - 1);
	
	/*
	if( m_bSetItemView )
		g_MyD3D->m_kItemMgr.m_ShopPitch = iPage * NUM_SET_ITEM_WND;
	else
		g_MyD3D->m_kItemMgr.m_ShopPitch = iPage * NUM_ITEM_WND;
	*/
}

void KGCShopItemBox::GetTotalPage()
{
	int iPage;
	int iItemNum = g_MyD3D->m_kItemMgr.GetNumShopItem();
	int iWndNum;
	if( m_bSetItemView )
	{
		iWndNum = NUM_SET_ITEM_WND; 
	}
	else
	{
		iWndNum = NUM_ITEM_WND; 
	}

	iPage = iItemNum/iWndNum;

	if( (iItemNum%iWndNum) > 0) 
		iPage += 1;

	m_iTotalPage = iPage;
}

void KGCShopItemBox::UpdateEquip()
{
	//현재 보이는 창중에서
	if( m_bSetItemView )
	{
		for(int iWnd = 0; iWnd < NUM_SET_ITEM_WND; ++iWnd )
		{
			m_apSetItemWnd[iWnd]->Equip( false );

			if( m_apSetItemWnd[iWnd]->GetItemID() == -1 )			
				continue;
			
			if( g_pItemMgr->CheckEquipSetItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), m_apSetItemWnd[iWnd]->GetItemInfo()->iSetItemNumber ) )
				m_apSetItemWnd[iWnd]->Equip( true );
		}
	}
	else
	{
		SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.GetCurrentChar();
		if( pCharInfo == NULL )
			g_kGlobalValue.m_strNullFunc =__FUNCTION__;

		for(int iWnd = 0; iWnd < NUM_ITEM_WND; ++iWnd )
		{
			m_apItemWnd[iWnd]->Equip( false );
			
            // 먼저 해당 아이템이 패키지인지 판별을 한다
            int nItemId = m_apItemWnd[ iWnd ]->GetItemID();
            bool bPackage = g_pItemMgr->IsPackageItemID( static_cast< GCITEMID >( nItemId ) * 10 );
            std::vector< KDropItemInfo > vecItems;
            g_pItemMgr->GetPackageElements( static_cast< GCITEMID >( nItemId ) * 10, vecItems );

			for( int j = 0; j < (int)pCharInfo->vecItemInfo.size(); ++j )
			{
                if ( bPackage ) {
                    std::vector< KDropItemInfo >::iterator vecIter = vecItems.begin();
                    for ( ; vecIter != vecItems.end(); ++vecIter ) {
                        if ( vecIter->m_ItemID / 10 == pCharInfo->vecItemInfo[ j ].iItemID )
                        {
                            m_apItemWnd[ iWnd ]->Equip( true );
                            break;
                            // 여기에서는 m_apItemWnd에 bEquip을 true로 설정 해주는 것이 목적이므로 패키지 벡터를 끝까지 순회할 필요는 없다
                        }
                    }
                }
				else
                {
                    if ( nItemId == pCharInfo->vecItemInfo[ j ].iItemID )
				    {
					    m_apItemWnd[ iWnd ]->Equip( true );
					    break;
				    }
                }
			}
		}
	}
}

void KGCShopItemBox::UpdateMarks( void )
{
	int iIndex = 0;
	
	if( m_bSetItemView )
	{
		for ( int i = 0; i < NUM_SET_ITEM_WND; ++i )
		{
			GCITEMID tempID = m_apSetItemWnd[i]->GetItemID();
			if (m_apSetItemWnd[i]->GetItemID() == -1)
				continue;

			m_apSetItemWnd[i]->SetMask(GetItemRecomInfo(i));
			m_apSetItemWnd[i]->SetVIP(g_kGlobalValue.IsItemInVIPList(tempID));
		}
	}
	else
	{
		for ( int i = 0; i < NUM_ITEM_WND; ++i )
		{			
			GCITEMID tempID = m_apItemWnd[i]->GetItemID();
			if (tempID == -1)
				continue;
			m_apItemWnd[i]->SetMask(GetItemRecomInfo(i));
			m_apItemWnd[i]->SetVIP(g_kGlobalValue.IsItemInVIPList(tempID));
		}
	}
}

bool KGCShopItemBox::GetCurrentHoveredItem( GCItem*& pItem, D3DXVECTOR2 &vecPos )
{
	POINT   pt = g_pkInput->GetMousePos();
	if (!m_bSetItemView)
	{
		for( int i = 0 ; i < NUM_ITEM_WND ; ++i )
		{
			if ( m_apItemWnd[i]->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) ) )
			{
				if( m_apItemWnd[i]->GetItemID() == -1 )
					return false;
				if ( m_apItemWnd[i]->GetItemKey() == m_kCurInfoItemKey )
				{
					pItem = m_kCurInfoItem;
                    _RETURN_NIL(pItem, false);

					vecPos = m_apItemWnd[i]->GetCurrentWindowPos();
					vecPos.x += ( m_apItemWnd[i]->GetWidth() );
					vecPos.y += ( m_apItemWnd[i]->GetHeight() / 2.0f );
					vecPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
					vecPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
					return true;
				}
				pItem = g_pItemMgr->GetItemData(m_apItemWnd[i]->GetItemKey());
                _RETURN_NIL(pItem, false);

				m_kCurInfoItem = pItem;
				m_kCurInfoItemKey = m_apItemWnd[i]->GetItemKey();

				vecPos = m_apItemWnd[i]->GetCurrentWindowPos();
				vecPos.x += ( m_apItemWnd[i]->GetWidth() );
				vecPos.y += ( m_apItemWnd[i]->GetHeight() / 2.0f );
				vecPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
				vecPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
				return true;
			}
		}
	}
	else
	{
		for( int i = 0 ; i < NUM_SET_ITEM_WND ; ++i )
		{
			if ( m_apSetItemWnd[i]->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) ) )
			{
				if( m_apSetItemWnd[i]->GetItemID() == -1 )
					return false;
				if ( m_apSetItemWnd[i]->GetItemKey() == m_kCurInfoItemKey )
				{
					pItem = m_kCurInfoItem;
                    _RETURN_NIL(pItem, false);

					vecPos = m_apSetItemWnd[i]->GetCurrentWindowPos();
					vecPos.x += ( m_apSetItemWnd[i]->GetWidth() );
					vecPos.y += ( m_apSetItemWnd[i]->GetHeight() / 2.0f );
					vecPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
					vecPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

					return true;
				}
				pItem = g_pItemMgr->GetItemData(m_apSetItemWnd[i]->GetItemKey());
                _RETURN_NIL(pItem, false);
				m_kCurInfoItem = pItem;
				m_kCurInfoItemKey = m_apSetItemWnd[i]->GetItemKey();

				vecPos = m_apSetItemWnd[i]->GetCurrentWindowPos();
				vecPos.x += ( m_apSetItemWnd[i]->GetWidth() );
				vecPos.y += ( m_apSetItemWnd[i]->GetHeight() / 2.0f );
				vecPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
				vecPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

				return true;
			}
		}

	}
	return false;
}