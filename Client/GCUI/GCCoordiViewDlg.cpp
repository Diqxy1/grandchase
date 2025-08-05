#include "stdafx.h"
#include "GCCoordiViewDlg.h"
#include "GCCoordiViewChar.h"

IMPLEMENT_CLASSNAME( KGCCoordiViewDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiViewDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiViewDlg, "gc_coordiviewdlg" );

KGCCoordiViewDlg::KGCCoordiViewDlg( void )
: m_pkScrollbar( NULL )
, m_pkBackground( NULL )
, m_pkSearchBar( nullptr )
, m_pkBtnClearLook( NULL )
, m_pkComboItemType( NULL )
, m_pkCoordiCharView( NULL )
{
	m_iCurrentSlot = 0;
	m_mapSlotInfo.clear();
	for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; i++ )
	{
		char strTemp[50];
		sprintf( strTemp, "equip_item%d", i );
		m_vecCoordiViewEquip[i] = NULL;
		LINK_CONTROL( strTemp, m_vecCoordiViewEquip[i] );
	}
	for ( int i = 0; i < COORDI_LOOK_SLOT; i++ )
	{
		char strTemp[50];
		sprintf( strTemp, "slot_look%d", i );
		m_vecCoordiLookSlot[i] = NULL;
		LINK_CONTROL( strTemp, m_vecCoordiLookSlot[i] );
	}
	for ( int i = 0; i < COORDI_LOOK_SLOT; i++ )
	{
		char strTemp[50];
		sprintf( strTemp, "slot_look_ok%d", i );
		m_vecCoordiLookSlotOn[i] = NULL;
		LINK_CONTROL( strTemp, m_vecCoordiLookSlotOn[i] );
	}
	LINK_CONTROL( "back", m_pkBackground );
	LINK_CONTROL( "scroll", m_pkScrollbar );
	LINK_CONTROL( "search_bar", m_pkSearchBar );
	LINK_CONTROL( "back", m_pkBackground );
	LINK_CONTROL( "btn_close", m_pkClose );
	LINK_CONTROL( "btn_clearlook", m_pkBtnClearLook );
	LINK_CONTROL( "coordi_view_char", m_pkCoordiCharView );
	LINK_CONTROL( "itemlist_combobox", m_pkComboItemType );
}

KGCCoordiViewDlg::~KGCCoordiViewDlg( void )
{
	m_iCurrentSlot = 0;
	m_mapSlotInfo.clear();
	m_mapItemInfo.clear();
}

void KGCCoordiViewDlg::OnCreate( void )
{
	for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; i++ )
	{
		if( m_vecCoordiViewEquip[i] )
			m_vecCoordiViewEquip[i]->InitState( true, true, this );
	}
	for ( int i = 0; i < COORDI_LOOK_SLOT; i++ )
	{
		if( m_vecCoordiLookSlot[i] )
			m_vecCoordiLookSlot[i]->InitState( true, true, this );
	}
	for (int i = 0; i < COORDI_LOOK_SLOT; i++)
	{
		if (m_vecCoordiLookSlotOn[i]) 
			m_vecCoordiLookSlotOn[i]->InitState( false, true, this );
	}
	if( m_pkBackground ){
		m_pkBackground->InitState( true );
		m_pkBackground->SetPredraw( true );
	}
	if ( m_pkScrollbar ) 
	{
		m_pkScrollbar->InitState( true, true, this );
		m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );
	}
	if( m_pkSearchBar )
		m_pkSearchBar->InitState( true, true, this );
	if( m_pkBtnClearLook )
		m_pkBtnClearLook->InitState( true, true, this );
	if( m_pkComboItemType ){
		m_pkComboItemType->InitState( true, true, this );
		m_pkComboItemType->SetAlign( DT_CENTER );
	}
	if ( m_pkCoordiCharView ) 
		m_pkCoordiCharView->InitState(true, true, this);
	if (m_pkClose) {
		m_pkClose->InitState( true, true, false );
		m_pkClose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCoordiViewDlg::OnClose );
		m_pkClose->SetHotKey( DIK_ESCAPE );
	}
	CreateInvenSlot();
}

void KGCCoordiViewDlg::OnCreateComplete( void )
{
	m_iScrollpos = 0;
	m_vecCurrentCoordiViewItemList = g_kGlobalValue.GetCoordiViewChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
	SetTypeCombobox();
	FilterItems();
	SetCoordiViewSavedSlot();
	CheckCoordiViewerSlots();
	if ( m_pkSearchBar != nullptr )
		m_pkSearchBar->Init();
}

void KGCCoordiViewDlg::FrameMoveInEnabledState( void )
{
	CheckItemInformation();
}

void KGCCoordiViewDlg::SetCoordiViewSavedSlot()
{
	KP2P::GetInstance()->Send_CoordiViewSlotGetReq( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType, g_kGlobalValue.m_kUserInfo.dwUID );
	Result_ReceiveCoordiviewSlotGetReq = INT_MAX;
	g_MyD3D->WaitForServerAck( Result_ReceiveCoordiviewSlotGetReq, INT_MAX, 3000, INT_MAX );
}

void KGCCoordiViewDlg::CheckCoordiViewerSlots()
{
	for ( int i = 0; i < COORDI_LOOK_SLOT; i++ )
	{
		if ( m_mapSlotInfo.find(i) != m_mapSlotInfo.end() )
		{
			if ( m_mapSlotInfo[i].empty() ) 
			{
				m_vecCoordiLookSlot[i]->ToggleRender(true);
				m_vecCoordiLookSlotOn[i]->ToggleRender(false);
			}
			else 
			{
				m_vecCoordiLookSlot[i]->ToggleRender(false);
				m_vecCoordiLookSlotOn[i]->ToggleRender(true);
			}
		}
		else
		{
			m_vecCoordiLookSlot[i]->ToggleRender( true );
			m_vecCoordiLookSlotOn[i]->ToggleRender( false );
		}
	}
}

void KGCCoordiViewDlg::CreateInvenSlot()
{
	float fBasePosX = 38.0f;
	float fBasePosY = 99.0f;
	if (m_vecCoordiViewList.empty())
	{
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_coordi_view_dlg.stg", "", "coordi_view_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );
		for ( int i = 0; i < (int)vecTemp.size(); ++i )
		{
			float fPosX = fBasePosX + ( ( i % EM_INVEN_COLUMN ) * EM_INVEN_WIDH_SIZE );
			float fPosY = fBasePosY + ( ( i / EM_INVEN_COLUMN ) * EM_INVEN_HEIGHT_SIZE );
			vecTemp[i]->SetWindowPosDirect( D3DXVECTOR2( fPosX, fPosY ) );
			vecTemp[i]->ToggleRender( true );
			vecTemp[i]->InitState( true, true, this );
			m_vecCoordiViewList.push_back( (KGCCoordiViewItemWnd*)vecTemp[i] );
		}
	}
}

void KGCCoordiViewDlg::SetTypeCombobox( void ) 
{
	m_pkComboItemType->DeleteAllItem();
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_TOTAL ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_HELMET ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_JACKET ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_PANTS ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_GLOVES ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_SHOES ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_MANTLE ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_A_UPPER_HELMET ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_A_DOWN_HELMET ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_A_JACKET ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_A_PANTS ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_A_ARM ) );
	m_pkComboItemType->AddString( g_pkStrLoader->GetString( STR_ID_COORDI_WEAPON ) );
	m_pkComboItemType->SetCurSel( 0 );
}

void KGCCoordiViewDlg::FindItemFromName( const WCHAR* strName_ )
{
	if ( strName_ )
		m_strFilter = strName_;
	else
		m_strFilter.clear();
	FilterItems();
}

void KGCCoordiViewDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
	_GCWND_MSG_MAP( m_pkComboItemType, KD3DComboBox::D3DWE_COMBO_SELCHANGE, FilterItems );
	_GCWND_MSG_MAP( m_pkBtnClearLook, KD3DComboBox::D3DWE_BUTTON_CLICK, ClearEquipedItems );
	if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		for ( int i = 0; i < (int)m_vecCoordiViewList.size(); ++i )
		{
			if ( event.m_pWnd == m_vecCoordiViewList[i] )
			{
				if ( m_vecCoordiViewList[i]->GetItemID() > 0 ) 
				{
					SetEquipedSlotItem( i, m_vecCoordiViewList[i]->GetItemID(), m_vecCoordiViewList[i]->GetType() );
					return;
				}
			}
		}
		for (int i = 0; i < COORDI_LOOK_SLOT; ++i)
		{
			if ( event.m_pWnd == m_vecCoordiLookSlotOn[i] || event.m_pWnd == m_vecCoordiLookSlot[i] )
			{
				SetEquippedItemFromSlot( i );
				return;
			}
		}
	}
}

int KGCCoordiViewDlg::GetSelectedCategory( int CurSel ) 
{
	switch ( CurSel ) 
	{
		case  0: return					-1; break;
		case  1: return			ESP_HELMET; break;
		case  2: return			ESP_JACKET; break;
		case  3: return			 ESP_PANTS; break;
		case  4: return			ESP_GLOVES; break;
		case  5: return			 ESP_SHOES; break;
		case  6: return			ESP_MANTLE; break;
		case  7: return ESP_A_UPPER_HELMET; break;
		case  8: return  ESP_A_DOWN_HELMET; break;
		case  9: return		  ESP_A_JACKET; break;
		case 10: return		   ESP_A_PANTS; break;
		case 11: return			 ESP_A_ARM; break;
		case 12: return			ESP_WEAPON; break;
		default: return					-1;
	}
}

int KGCCoordiViewDlg::GetCategorySlot( DWORD CurCat )
{
	switch ( CurCat )
	{
		case 0x00000002: return  0; break;
		case 0x00000004: return  1; break;
		case 0x00000008: return  2; break;
		case 0x00000010: return  3; break;
		case 0x00000020: return  4; break;
		case 0x00000040: return  5; break;
		case 0x00000100: return  6; break;
		case 0x00000200: return  7; break;
		case 0x00000800: return  8; break;
		case 0x00001000: return  9; break;
		case 0x00002000: return 10; break;
		case 0x00000080: return 11; break;
		default:		 return  0;
	}
}

void KGCCoordiViewDlg::UpdateViewListSelectedItems( void )
{
	for ( auto& element : m_vecCoordiViewList )
	{
		element->SelectedItem( false );
		for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; ++i )
			if( m_vecCoordiViewEquip[i]->GetItemID() == element->GetItemID() && m_vecCoordiViewEquip[i]->GetItemID() > 0 )
			{
				element->SelectedItem( true );
			}
	}
}

void KGCCoordiViewDlg::FilterItems( void )
{
	m_vecCurrentCoordiViewItemList.clear();
	int dwCategory = GetSelectedCategory( m_pkComboItemType->GetCurSel() );
	for ( auto& element : g_kGlobalValue.GetCoordiViewChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType ) )
	{
		GCItem* pItem = g_pItemMgr->GetItemData( element.first / 10 );
		if ( dwCategory == -1 )
		{
			if ( m_strFilter.size() > 0 )
			{
				wstring strLowerItemName = pItem->strItemName;
				GCUTIL_STR::ToLower( strLowerItemName );
				GCUTIL_STR::ToLower( m_strFilter );
				if ( strLowerItemName.find( m_strFilter ) != std::wstring::npos )
					m_vecCurrentCoordiViewItemList.push_back( element );
			}
			else 
			{
				m_vecCurrentCoordiViewItemList.push_back( element );
			}
		}
		else
		{
			if ( pItem != NULL && pItem->dwSlotPosition & dwCategory )
			{
				if (m_strFilter.size() > 0)
				{
					wstring strLowerItemName = pItem->strItemName;
					GCUTIL_STR::ToLower( strLowerItemName );
					GCUTIL_STR::ToLower( m_strFilter );
					if ( strLowerItemName.find( m_strFilter ) != std::wstring::npos )
						m_vecCurrentCoordiViewItemList.push_back( element );
				}
				else 
				{
					m_vecCurrentCoordiViewItemList.push_back( element );
				}
			}
		}
	}
	UpdateSlotCoordiViewList();
}

void KGCCoordiViewDlg::OnScrollPos( void )
{
	int iPos = m_pkScrollbar->GetScrollPos();
	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		FilterItems();
	}
}

void KGCCoordiViewDlg::UpdateSlotCoordiViewList( void )
{
	int nItemCount = (int)m_vecCurrentCoordiViewItemList.size();
	if( m_iScrollpos > 0 && nItemCount <= EM_INVEN_COLUMN * EM_INVEN_ROW )
		m_iScrollpos = 0;
	int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;
	m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
	m_pkScrollbar->SetScrollRangeMax( nScrollMax );
	m_pkScrollbar->SetScrollPos( m_iScrollpos );
	UpdateCoordiViewItemList();
	UpdateViewListSelectedItems();
}

void KGCCoordiViewDlg::UpdateCoordiViewItemList( void )
{
	for ( int i = 0; i < (int)m_vecCoordiViewList.size(); ++i )
		m_vecCoordiViewList[i]->ResetItem();
	int nCount = 0;
	for ( int i = 0; i < (int)m_vecCurrentCoordiViewItemList.size(); ++i )
	{
		if ( i < ( m_iScrollpos * EM_INVEN_COLUMN ) )
			continue;
		if ( nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW )
			return;
		m_vecCoordiViewList[nCount]->SetItem( m_vecCurrentCoordiViewItemList[i].first, m_vecCurrentCoordiViewItemList[i].second );
		nCount++;
	}
}

void KGCCoordiViewDlg::SetEquipedSlotItem( int ItemIndex, GCITEMID iItemID, int iType )
{
	GCItem* pItem = g_pItemMgr->GetItemData( m_vecCoordiViewList[ItemIndex]->GetItemID() / 10 );
	int SlotPos = GetCategorySlot( pItem->dwSlotPosition );
	GCItem* tItem = g_pItemMgr->GetItemData(m_vecCoordiViewEquip[SlotPos]->GetItemID() / 10);
	if ( false == m_vecCoordiViewList[ItemIndex]->m_bActive ) 
	{
		GCITEMID TempID = m_vecCoordiViewEquip[SlotPos]->GetItemID();
		for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; i++ )
		{
			if ( TempID > 0 )
				if ( TempID == m_vecCoordiViewEquip[i]->GetItemID() )
					m_vecCoordiViewEquip[i]->ResetItem();
			if( pItem->dwSlotPosition & s_aItemCategory[i] )
				m_vecCoordiViewEquip[i]->SetItem( iItemID, iType );
		}
		if ( m_pkCoordiCharView )
		{
			m_pkCoordiCharView->RemoveItem( tItem );
			m_pkCoordiCharView->SetItem( pItem );
		}
	}
	else 
	{
		if ( m_pkCoordiCharView )
			m_pkCoordiCharView->RemoveItem( pItem );
		for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; i++ )
			if( pItem->dwSlotPosition & s_aItemCategory[i] )
				m_vecCoordiViewEquip[i]->ResetItem();
	}
	UpdateViewListSelectedItems();
}

void KGCCoordiViewDlg::ClearEquipedItems( void ) {
	for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; ++i )
		m_vecCoordiViewEquip[i]->ResetItem();
	for ( int i = 0; i < (int)m_vecCoordiViewList.size(); ++i )
		m_vecCoordiViewList[i]->SelectedItem( false );
	m_pkCoordiCharView->OnViewBasicClick();
}

void KGCCoordiViewDlg::OnClose()
{
	m_strFilter = L"";
	m_iCurrentSlot = 0;
	m_mapSlotInfo.clear();
	m_mapItemInfo.clear();
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

int KGCCoordiViewDlg::GetTypeFromItemSlot( GCITEMID iItemID )
{
	for ( auto& element : m_vecCurrentCoordiViewItemList )
	{
		if ( element.first == iItemID )
			return element.second;
	}
	return 0;
}

void KGCCoordiViewDlg::SetEquippedItemFromSlot( int iSlot ) 
{
	m_iCurrentSlot = iSlot;
	if ( m_pkCoordiCharView->GetItemMap().empty() ) 
	{
		if ( m_mapSlotInfo.find( m_iCurrentSlot ) == m_mapSlotInfo.end() )
			return;
		ClearEquipedItems();
		for ( auto& element : m_mapSlotInfo[ m_iCurrentSlot ] )
		{
			GCItem* pItem = g_pItemMgr->GetItemData( element / 10 );
			for ( int i = 0; i < COORDI_VIEW_EQUIP_MAX; i++ )
				if ( pItem->dwSlotPosition & s_aItemCategory[i] )
					m_vecCoordiViewEquip[i]->SetItem( element, GetTypeFromItemSlot( element ) );
			if ( m_pkCoordiCharView )
				m_pkCoordiCharView->SetItem( pItem );
		}
		UpdateViewListSelectedItems();
	}
	else 
	{
		m_vecCurrentEquipedItems.clear();
		m_vecCurrentEquipedItems = m_pkCoordiCharView->GetEquipedItemIDVector();
		if(!m_vecCurrentEquipedItems.empty())
		{
			if ( m_mapSlotInfo.find( m_iCurrentSlot ) != m_mapSlotInfo.end() )
			{
				if( m_mapSlotInfo[m_iCurrentSlot].empty() )
				{
					std::wstringstream stm;
					stm << m_iCurrentSlot + 1;
					std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_COORDIVIEW_SAVE_MSG, "l", stm.str() );
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strMsg, L"", KGCUIScene::GC_MBOX_COORDIVIEW_SAVE_SLOT, 0, 0, true, true );
				}
				else
				{
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_COORDIVIEW_WARNING ), L"", KGCUIScene::GC_MBOX_COORDIVIEW_SAVE_SLOT, 0, 0, true, true );
				}
			}
			else
			{
				std::wstringstream stm;
				stm << m_iCurrentSlot + 1;
				std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_COORDIVIEW_SAVE_MSG, "l", stm.str() );
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strMsg, L"", KGCUIScene::GC_MBOX_COORDIVIEW_SAVE_SLOT, 0, 0, true, true );
			}
		}
	}
}

void KGCCoordiViewDlg::CheckItemInformation()
{
	for ( int i = 0; i < (int)m_vecCoordiViewList.size(); ++i )
	{
		if ( m_vecCoordiViewList[i]->GetItemID() != 0 && m_vecCoordiViewList[i]->CheckMousePosInWindowBound() )
		{
			GCItem* pItem = g_pItemMgr->GetItemData( m_vecCoordiViewList[i]->GetItemID() / 10 );
			DrawItemInfo(pItem, m_vecCoordiViewList[i]->GetCurrentWindowPos());
			D3DXVECTOR2 vFocusingPos = m_vecCoordiViewList[i]->GetFixedWindowLocalPos();
			vFocusingPos.x -= 2.0f;
			vFocusingPos.y -= 3.0f;
			return;
		}
	}
	for (int i = 0; i < COORDI_VIEW_EQUIP_MAX; ++i)
	{
		if ( m_vecCoordiViewEquip[i]->GetItemID() != 0 && m_vecCoordiViewEquip[i]->CheckMousePosInWindowBound() )
		{
			GCItem* pItem = g_pItemMgr->GetItemData( m_vecCoordiViewEquip[i]->GetItemID() / 10 );
			DrawItemInfo( pItem, m_vecCoordiViewEquip[i]->GetCurrentWindowPos() );
			D3DXVECTOR2 vFocusingPos = m_vecCoordiViewEquip[i]->GetFixedWindowLocalPos();
			vFocusingPos.x -= 2.0f;
			vFocusingPos.y -= 3.0f;
			return;
		}
	}
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCCoordiViewDlg::DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos )
{
	if ( pItem != NULL && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
	{
		vPos.x += EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH;
		vPos.y += ( EM_INVEN_HEIGHT_SIZE / 2 ) * GC_SCREEN_DIV_WIDTH;
		vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
		vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
		if ( vPos.x + g_pkUIScene->m_pkItemInformationBox->GetWidth() > static_cast<float>( GC_SCREEN_WIDTH ) )
		{
			vPos.x -= ( static_cast<float>( EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH ) * m_fWindowScaleX );
			vPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
		}
		GCItem* pGCItem = g_pItemMgr->GetItemData( pItem->dwGoodsID );
		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1 );
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
	}
	else
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}