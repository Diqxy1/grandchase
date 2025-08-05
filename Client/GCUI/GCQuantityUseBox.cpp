#include "stdafx.h"
#include "GCQuantityUseBox.h"
#include "GCCoordiViewItemWnd.h"

IMPLEMENT_CLASSNAME( KGCQuantityUseBox );
IMPLEMENT_WND_FACTORY( KGCQuantityUseBox );
IMPLEMENT_WND_FACTORY_NAME( KGCQuantityUseBox, "gc_quantity_chest_use" );

KGCQuantityUseBox::KGCQuantityUseBox( void )
: m_pkClose( NULL )
, m_pkBtnUp( NULL )
, m_pkQntback( NULL )
, m_pkBtnDown( NULL )
, m_pkItemName( NULL )
, m_pkItemImage( NULL )
, m_pkBackground( NULL )
, m_pkTitleChest( NULL )
, m_pkBtnConfirm( NULL )
, m_pkTitleCoupon( NULL )
, m_pkQuantityDesc( NULL )
, m_pkCount( NULL )
{
	m_uiQuantity = 0;

	LINK_CONTROL( "up_btn", m_pkBtnUp );
	LINK_CONTROL( "close_btn", m_pkClose );
	LINK_CONTROL( "down_btn", m_pkBtnDown );
	LINK_CONTROL( "item_name", m_pkItemName );
	LINK_CONTROL( "btn_get", m_pkBtnConfirm );
	LINK_CONTROL( "box_item", m_pkItemImage );
	LINK_CONTROL( "background", m_pkBackground );
	LINK_CONTROL( "quantity_back", m_pkQntback );
	LINK_CONTROL( "title_chest", m_pkTitleChest );
	LINK_CONTROL( "title_coupon", m_pkTitleCoupon );
	LINK_CONTROL( "quantity_desc", m_pkQuantityDesc );
	LINK_CONTROL( "quantity_text", m_pkCount );
}

KGCQuantityUseBox::~KGCQuantityUseBox( void )
{
	g_kGlobalValue.m_pkCurrentUseItemID = 0;
	g_kGlobalValue.m_bIsCurrentUsechest = false;
}

void KGCQuantityUseBox::OnCreate( void )
{
	m_pkClose->InitState( true, true, this );
	m_pkBtnUp->InitState( true, true, this );
	m_pkQntback->InitState( true, true, this );
	m_pkBtnDown->InitState( true, true, this );
	m_pkItemImage->InitState( true, true, this );
	m_pkBackground->InitState( true, true, this );
	m_pkTitleChest->InitState( true, true, this );
	m_pkBtnConfirm->InitState( true, true, this );
	m_pkTitleCoupon->InitState( true, true, this );
	m_pkBtnUp->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityUseBox::OnUp );
	m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityUseBox::OnClose );
	m_pkBtnDown->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityUseBox::OnDown );
	m_pkBtnConfirm->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityUseBox::OnConfirm );
}

void KGCQuantityUseBox::OnCreateComplete( void )
{
	GCItem* pItemData = g_pItemMgr->GetItemData( g_kGlobalValue.m_pkCurrentUseItemID );
	std::wstringstream TempItemName;
	TempItemName << g_pItemMgr->GetItemGradeColorKey( pItemData->cItemGrade ) << pItemData->strItemName << L"#cX";
	m_pkItemImage->SetItem( g_kGlobalValue.m_pkCurrentUseItemID * 10, 0 );
	m_pkItemName->InitState( true, true, this );
	m_pkItemName->SetFontColor( D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	m_pkItemName->SetAlign( DT_CENTER );
	m_pkItemName->SetText( TempItemName.str() );
	m_pkQuantityDesc->InitState( true, true, this );
	m_pkQuantityDesc->SetFontColor( D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	m_pkQuantityDesc->SetAlign( DT_LEFT );
	m_pkQuantityDesc->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_DESC_QUANTITY_USE, "l", to_wstring( GetAvailableNum() ) ) );
	m_pkCount->InitState( true, true, this );
	m_pkCount->SetFontColor( D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	m_pkCount->SetEditType(KD3DEdit::EDIT_NUMBER);
	m_uiQuantity = 1;
	SetUseNum( 1 );
}

void KGCQuantityUseBox::FrameMoveInEnabledState( void )
{
	if( true == g_kGlobalValue.m_bIsCurrentUsechest )
	{
		m_pkTitleChest->ToggleRender( true );
		m_pkTitleCoupon->ToggleRender( false );
	}
	else
	{
		m_pkTitleChest->ToggleRender( false );
		m_pkTitleCoupon->ToggleRender( true );
	}
	OnTextEnter();
}

void KGCQuantityUseBox::ActionPerformed( const KActionEvent& event )
{
	switch ( event.m_dwCode )
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER:
			if ( event.m_pWnd == m_pkCount )
				m_pkCount->SetSelection( 0, m_pkCount->GetTextSize() );
		break;
		case KD3DWnd::D3DWE_BUTTON_PRESS:
		{
			if ( ++m_iPressCount > 45 )
			{
				if ( event.m_pWnd == m_pkBtnUp )
				{
					int nMaxCount = GetAvailableNum();
					if ( m_uiQuantity < nMaxCount )
					{
						int nPlusCount = ( m_iPressCount / 100 ) * 10;
						nPlusCount = nPlusCount == 0 ? 1 : nPlusCount;
						if ( m_uiQuantity + nPlusCount > nMaxCount )
							nPlusCount = nMaxCount - m_uiQuantity;
						if ( CheckAvailableNum( m_uiQuantity + nPlusCount ) )
						{
							m_uiQuantity += nPlusCount;
							SetUseNum( m_uiQuantity );
						}
					}
				}
				else if ( event.m_pWnd == m_pkBtnDown )
				{
					if ( m_uiQuantity > 0 )
					{
						int nMinusCount = ( m_iPressCount / 100 ) * 10;
						nMinusCount = nMinusCount == 0 ? 1 : nMinusCount;
						if ( m_uiQuantity - nMinusCount < 0 )
							nMinusCount = m_uiQuantity;
						m_uiQuantity -= nMinusCount;
						if ( m_uiQuantity <= 0 )
							m_uiQuantity = 1;
						SetUseNum( m_uiQuantity );
					}
				}
			}
		}
		break;
	}
}

void KGCQuantityUseBox::OnTextEnter()
{
	int count = static_cast<int>(_wtoi(m_pkCount->GetText()));

	if (count == m_uiQuantity)
	{
		return;
	}

	int iMaxQuantity = GetAvailableNum();

	if (count > iMaxQuantity) {
		count = iMaxQuantity;
	}
	else if (count <= 0) {
		count = 1;
	}

	SetUseNum(count);
	m_uiQuantity = count;
}

void KGCQuantityUseBox::OnUp() 
{
	m_iPressCount = 0;
	m_uiQuantity++;
	int iMaxQuantity = GetAvailableNum();
	if (m_uiQuantity > iMaxQuantity)
		m_uiQuantity = iMaxQuantity;
	SetUseNum( m_uiQuantity );
}

void KGCQuantityUseBox::OnDown()
{
	m_iPressCount = 0;
	m_uiQuantity--;
	if (m_uiQuantity <= 0)
		m_uiQuantity = 1;
	SetUseNum( m_uiQuantity );
}

void KGCQuantityUseBox::OnClose()
{
	g_KDSound.Play( "31" );
	g_kGlobalValue.m_bIsCurrentUsechest = false;
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCQuantityUseBox::OnConfirm()
{
	if ( true == g_kGlobalValue.m_bIsCurrentUsechest )
	{
		int FinalCount = 0;
		if (m_uiQuantity > GetAvailableNum())
			FinalCount = GetAvailableNum();
		else
			FinalCount = m_uiQuantity;
		std::wstringstream ItemNameTemp;
		KItem* pItemData = g_pItemMgr->m_kInventory.FindItemByItemID( g_kGlobalValue.m_pkCurrentUseItemID );
		ItemNameTemp << FinalCount << "x " << g_pItemMgr->GetItemName( g_kGlobalValue.m_pkCurrentUseItemID );
		OnClose();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_QUANTITY_USE_CONFIRM, "l", ItemNameTemp.str() ), L"", KGCUIScene::GC_MBOX_USE_KAIRO_ONESHOT, pItemData->m_ItemUID, FinalCount, true, true, false, false );
	}
	else
	{
		int FinalCount = 0;
		if (m_uiQuantity > GetAvailableNum())
			FinalCount = GetAvailableNum();
		else
			FinalCount = m_uiQuantity;
		std::wstringstream ItemNameTemp;
		KItem* pItemData = g_pItemMgr->m_kInventory.FindItemByItemID(g_kGlobalValue.m_pkCurrentUseItemID);
		ItemNameTemp << FinalCount << "x " << g_pItemMgr->GetItemName(g_kGlobalValue.m_pkCurrentUseItemID);
		OnClose();
		if ( g_pItemMgr->IsGPCoupon( g_kGlobalValue.m_pkCurrentUseItemID ) )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_QUANTITY_USE_CONFIRM, "l", ItemNameTemp.str() ), L"", KGCUIScene::GC_MBOX_USE_CHANGE_COUPON, pItemData->m_ItemUID, FinalCount, true, true, false, false );
		}
		else if ( g_pItemMgr->IsVPCoupon( g_kGlobalValue.m_pkCurrentUseItemID ) )
		{
			g_pkUIScene->MessageBox (KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_QUANTITY_USE_CONFIRM, "l", ItemNameTemp.str() ), L"", KGCUIScene::GC_MBOX_USE_VIRTUAL_CASH_COUPON, pItemData->m_ItemUID, FinalCount, true, true, false, false );
		}
	}
}

int KGCQuantityUseBox::GetAvailableNum()
{
	int iDuration = g_pItemMgr->GetInventoryItemDuration( g_kGlobalValue.m_pkCurrentUseItemID );
	if ( iDuration < 1 )
	{
		OnClose();
		return 0;
	}
	if ( iDuration > 100 )
		return 100;
	return iDuration;
}

bool KGCQuantityUseBox::CheckAvailableNum(int iNum)
{
	KItem* pItemData = g_pItemMgr->m_kInventory.FindItemByItemID( g_kGlobalValue.m_pkCurrentUseItemID );
	return  pItemData->m_nInitCount >= iNum;
}

void KGCQuantityUseBox::SetUseNum( int iNum )
{
	std::wostringstream strmItemNum;
	strmItemNum << iNum;
	m_pkCount->SetText( strmItemNum.str().data(), false );
}