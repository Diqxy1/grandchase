#include "stdafx.h"
#include "GCZeroMaskDlg.h"

IMPLEMENT_CLASSNAME( KGCZeroMaskDlg );
IMPLEMENT_WND_FACTORY( KGCZeroMaskDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCZeroMaskDlg, "gc_zeromaskselect" );

KGCZeroMaskDlg::KGCZeroMaskDlg( void )
: m_pkClose( NULL )
, m_pkClear( NULL )
, m_iCurrentSelectedOption( 0 )
{
	LINK_CONTROL( "btn_close", m_pkClose );
	LINK_CONTROL( "reset_btn", m_pkClear );
	for ( int i = 0; i < ZERO_MASK_MAX; ++i )
    {
        m_pMaskSlot[ i ] = NULL;
		m_pMaskImg[ i ] = NULL;
		m_pMaskSelected[ i ] = NULL;
		char str[MAX_PATH];
        sprintf( str, "slot_%d", i );          
        LINK_CONTROL( str,  m_pMaskSlot[ i ] );
		sprintf( str, "mask_img_%d", i );          
        LINK_CONTROL( str,  m_pMaskImg[ i ] );
		sprintf( str, "select_%d", i );          
        LINK_CONTROL( str,  m_pMaskSelected[ i ] );
    }
}

KGCZeroMaskDlg::~KGCZeroMaskDlg( void )
{
}

void KGCZeroMaskDlg::OnCreate( void )
{
	if ( m_pkClose ) 
	{
		m_pkClose->InitState( true, true, false );
		m_pkClose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCZeroMaskDlg::OnClose );
		m_pkClose->SetHotKey( DIK_ESCAPE );
	}
	if ( m_pkClear ) 
	{
		m_pkClear->InitState( true, true, false );
		m_pkClear->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCZeroMaskDlg::OnClear );
	}
	for ( int j = 0; j < ZERO_MASK_MAX; ++j )
	{
		if( m_pMaskImg[ j ] )
			m_pMaskImg[ j ]->InitState( false, true, this );
		if( m_pMaskSlot[ j ] )
			m_pMaskSlot[ j ]->InitState( true, true, this );
		if( m_pMaskSelected[ j ] )
			m_pMaskSelected[ j ]->InitState( false, true, this );
	}
}

void KGCZeroMaskDlg::OnCreateComplete( void )
{
	for ( int k = 0; k < ZERO_MASK_MAX; ++k )
	{
		m_pMaskImg[ k ]->SetItemInfo( g_kGlobalValue.GetZeroMaskItemID( k ) / 10 );
		m_pMaskImg[ k ]->ToggleRender( true );
	}
	zeroConfirm = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
	if( zeroConfirm.iCharType == GC_CHAR_ZERO )
	{
		m_iCurrentSelectedOption = -1;
		if( g_kGlobalValue.GetCurrEquipedZeroMaskID( &g_MyD3D->m_TempPlayer.GetCurrentChar() ) > 0 )
			m_pMaskSelected[ g_kGlobalValue.GetCurrEquipedZeroMaskID( &g_MyD3D->m_TempPlayer.GetCurrentChar() ) - 1 ]->ToggleRender( true );
	}
}

void KGCZeroMaskDlg::FrameMoveInEnabledState( void )
{
	zeroConfirm = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
	if( zeroConfirm.iCharType == GC_CHAR_ZERO )
	{
		int zeroPromotion = zeroConfirm.iPromotionLevel;
		for ( int l = 0; l < ZERO_MASK_MAX; ++l )
		{
			m_pMaskSelected[ l ]->ToggleRender( false );
			if( zeroPromotion < ( ZERO_MASK_MAX -1 ) )
				if ( zeroPromotion < l )
					if ( !m_pMaskImg[ l ]->IsLocked() )
						m_pMaskImg[ l ]->Lock( true );
			else
				if( m_pMaskImg[ l ]->IsLocked() )
					m_pMaskImg[ l ]->Lock( false );
			if( g_kGlobalValue.GetCurrEquipedZeroMaskID( &g_MyD3D->m_TempPlayer.GetCurrentChar() ) == ( l + 1 ) )
				m_pMaskSelected[ l ]->ToggleRender( true );
		}
	}
}

void KGCZeroMaskDlg::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		for ( int o = 0; o < ZERO_MASK_MAX; ++o )
		{
			if ( event.m_pWnd == m_pMaskImg[ o ] )
			{
				m_iCurrentSelectedOption = o;
				OnSelect();
			}
		}
	}
}

void KGCZeroMaskDlg::OnClose( void )
{
	g_KDSound.Play( "31" );
	this->ToggleRender( false );
}

void KGCZeroMaskDlg::OnSelect( void )
{
	g_KDSound.Play( "31" );
	EquipZeroMask( &g_MyD3D->m_TempPlayer.GetCurrentChar(), g_kGlobalValue.GetZeroMaskItemID( m_iCurrentSelectedOption ) );
}

void KGCZeroMaskDlg::OnClear( void )
{
	g_KDSound.Play( "31" );
	int iState = g_MyD3D->m_pStateMachine->GetState();
	m_iCurrentSelectedOption = -1;
	UnequipZeroMask( &g_MyD3D->m_TempPlayer.GetCurrentChar(), true );
}

void KGCZeroMaskDlg::EquipZeroMask( SCharInfo* pCharInfo, int iItemID )
{
	KItem* pInventoryItem = NULL;
	if ( g_pItemMgr->FindInventoryForItemID( iItemID / 10, &pInventoryItem ) )
	{
		if (pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM)
		{
			UnequipZeroMask( pCharInfo, false );
			SUserItem _Item;
			_Item.dwItemUID = pInventoryItem->m_ItemUID;
			_Item.iItemID = pInventoryItem->m_ItemID;
			pCharInfo->vecItemInfo.push_back( _Item );
			g_MyD3D->m_TempPlayer.RemovePlayerObject();
			g_MyD3D->m_TempPlayer.UpdatePlayerResource( true );
			g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
			g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false, UEC_LOOK, 0 );
			return;
		}
		else if (g_pkUIScene != NULL)
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1), g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
	}
}

void KGCZeroMaskDlg::UnequipZeroMask( SCharInfo* pCharInfo, bool bUpdate )
{
	if ( !pCharInfo )
		return;
	for ( auto& element : pCharInfo->vecItemInfo )
		if ( g_kGlobalValue.GetIsZeroMask( element.iItemID * 10 ) )
			g_pItemMgr->UnequipItemForItemID( pCharInfo, element.iItemID );
	if ( bUpdate )
	{
		g_MyD3D->m_TempPlayer.RemovePlayerObject();
		g_MyD3D->m_TempPlayer.UpdatePlayerResource( true );
		g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false, UEC_LOOK, 0 );
	}
}