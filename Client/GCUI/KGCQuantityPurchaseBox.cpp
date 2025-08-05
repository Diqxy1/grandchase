#include "stdafx.h"
#include "KGCQuantityPurchaseBox.h"

IMPLEMENT_CLASSNAME( KGCQuantityPurchaseBox );
IMPLEMENT_WND_FACTORY( KGCQuantityPurchaseBox );
IMPLEMENT_WND_FACTORY_NAME( KGCQuantityPurchaseBox, "gc_quantity_purchase_box" );

KGCQuantityPurchaseBox::KGCQuantityPurchaseBox() 
: m_pkBtnUp( NULL )
, m_pkBtnDown( NULL )
, m_pkBtnPurchase( NULL )
, m_pkBtnClose( NULL )
, m_pkPrice( NULL )
, m_pkTotal( NULL )
, m_pkItemNameStatic( NULL )
, m_pkPurchaseNumStatic( NULL )
, m_pkTotalPriceStatic( NULL )
, m_pkCount( NULL )
, m_pkItemImg( NULL )
, m_pkItem( NULL )
, m_uiPrice( 0 )
, m_uiQuantity( 1 )
, m_iPressCount( 0 )
, m_eMoneyType( EMT_GAMEPOINT )
{
	LINK_CONTROL("up_btn", m_pkBtnUp);
	LINK_CONTROL("down_btn", m_pkBtnDown);
	LINK_CONTROL("purchase_btn", m_pkBtnPurchase);
	LINK_CONTROL("close_btn", m_pkBtnClose);
	LINK_CONTROL("price", m_pkPrice);
	LINK_CONTROL("total_price", m_pkTotal);
	LINK_CONTROL("itemname_static", m_pkItemNameStatic);
	LINK_CONTROL("num_purchase_static", m_pkPurchaseNumStatic);
	LINK_CONTROL("total_price_static", m_pkTotalPriceStatic);
	LINK_CONTROL("item_img", m_pkItemImg);
	LINK_CONTROL("count", m_pkCount);

	char strTemp[50];
	for(int i = 0; i < 2 ; i++)
	{
		m_saGPMark[i] = NULL;
		sprintf( strTemp, "gp_mark%d", i + 1 );
		LINK_CONTROL( strTemp, m_saGPMark[i] );

		m_saVPMark[i] = NULL;
		sprintf( strTemp, "vp_mark%d", i + 1 );
		LINK_CONTROL( strTemp, m_saVPMark[i] );

		m_saDiaMark[i] = NULL;
		sprintf( strTemp, "dia_mark%d", i + 1 );
		LINK_CONTROL( strTemp, m_saDiaMark[i] );

		m_saPPMark[i] = NULL;
		sprintf( strTemp, "point_mark%d", i + 1 );
		LINK_CONTROL( strTemp, m_saPPMark[i] );

		m_saHeroMark[i] = NULL;
		sprintf( strTemp, "hero_mark%d", i + 1 );
		LINK_CONTROL( strTemp, m_saHeroMark[i] );
	}
}

KGCQuantityPurchaseBox::~KGCQuantityPurchaseBox()
{
}

void KGCQuantityPurchaseBox::ActionPerformed( const KActionEvent& event )
{
	switch( event.m_dwCode )
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER:
		{
			if(event.m_pWnd == m_pkCount)
			{
				m_pkCount->SetSelection(0, m_pkCount->GetTextSize());
			}
		}
		break;
		case KD3DWnd::D3DWE_BUTTON_PRESS:
		{
			if( ++m_iPressCount > 45 )
			{
				if( event.m_pWnd == m_pkBtnUp )
				{
					int nMaxCount = GetAvailableNum();
					if( m_uiQuantity < nMaxCount )
					{
						int nPlusCount = ( m_iPressCount / 100 ) * 10;
						nPlusCount = nPlusCount == 0 ? 1 : nPlusCount;
						if( m_uiQuantity + nPlusCount > nMaxCount )
						{
							nPlusCount = nMaxCount - m_uiQuantity;
						}
						if( CheckAvailableNum(m_uiQuantity + nPlusCount) )
						{
							m_uiQuantity += nPlusCount;
							SetPurchaseNum( m_uiQuantity );
						}
					}
				}
				else if( event.m_pWnd == m_pkBtnDown )
				{
					if( m_uiQuantity > 0 )
					{
						int nMinusCount = ( m_iPressCount / 100 ) * 10;
						nMinusCount = nMinusCount == 0 ? 1 : nMinusCount;
						if( m_uiQuantity - nMinusCount < 0 )
						{
							nMinusCount = m_uiQuantity;
						}
						m_uiQuantity -= nMinusCount;
						if( m_uiQuantity <= 0 )
						{
							m_uiQuantity = 1;
						}
						SetPurchaseNum( m_uiQuantity );
					}
				}
			}
		}
		break;
	}
}

void KGCQuantityPurchaseBox::FrameMoveInEnabledState()
{
	if( m_eMoneyType == KBuySellItemReq::EMT_GAMEPOINT )
	{
		for(int i = 0; i < 2; i++)
		{
			m_saGPMark[i]->ToggleRender( true );
			m_saVPMark[i]->ToggleRender( false );
			m_saPPMark[i]->ToggleRender( false );
			m_saDiaMark[i]->ToggleRender( false );
			m_saHeroMark[i]->ToggleRender( false );
		}
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_CRYSTAL )
	{
		for (int i = 0; i < 2; i++)
		{
			m_saGPMark[i]->ToggleRender( false );
			m_saVPMark[i]->ToggleRender( false );
			m_saPPMark[i]->ToggleRender( false );
			m_saDiaMark[i]->ToggleRender( true );
			m_saHeroMark[i]->ToggleRender( false );
		}
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_VIRTUALCASH )
	{
		for (int i = 0; i < 2; i++)
		{
			m_saGPMark[i]->ToggleRender( false );
			m_saVPMark[i]->ToggleRender( true );
			m_saPPMark[i]->ToggleRender( false );
			m_saDiaMark[i]->ToggleRender( false );
			m_saHeroMark[i]->ToggleRender( false );
		}
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_GEM )
	{
		for (int i = 0; i < 2; i++)
		{
			m_saGPMark[i]->ToggleRender( false );
			m_saVPMark[i]->ToggleRender( false );
			m_saPPMark[i]->ToggleRender( false );
			m_saDiaMark[i]->ToggleRender( false );
			m_saHeroMark[i]->ToggleRender( true );
		}
	}
	else
	{
		for(int i = 0; i < 2; i++)
		{
			m_saGPMark[i]->ToggleRender( false );
			m_saVPMark[i]->ToggleRender( false );
			m_saPPMark[i]->ToggleRender( true );
			m_saDiaMark[i]->ToggleRender( false );
			m_saHeroMark[i]->ToggleRender( false );
		}
	}

	OnTextEnter();
}

void KGCQuantityPurchaseBox::OnCreateComplete()
{
	m_uiQuantity = 1;
	m_uiPrice = 0;

	if( m_pkItem ) 
	{
		m_pkItemImg->SetItemInfo(m_pkItem->dwGoodsID);
		m_pkItemImg->ToggleRender(true);
		m_pkItemNameStatic->SetText(m_pkItem->strItemName);
		m_pkItemNameStatic->ToggleRender(true);

		DWORD dwPrice = GetPrice();
		m_uiPrice = dwPrice * m_uiQuantity;
		m_pkPrice->SetText(dwPrice);
		
		m_pkCount->SetText(L"1", false);
		m_pkTotal->SetText(m_uiPrice);
	}

	for (int i = 0; i < 2; i++)
	{
		m_saHeroMark[i]->SetItemInfo( g_kGlobalValue.m_pkCurrentUseItemCurrency );
	}
}

void KGCQuantityPurchaseBox::OnCreate()
{
	if( m_pkTotalPriceStatic )
	{
		m_pkTotalPriceStatic->InitState( true, true, this );
	}

	if( m_pkPurchaseNumStatic )
	{
		m_pkPurchaseNumStatic->InitState( true, true, this );
	}

	if( m_pkItemNameStatic )
	{
		m_pkItemNameStatic->InitState( true, true, this );
	}

	if( m_pkTotal )
	{
		m_pkTotal->InitState( true, true, this );
	}

	if( m_pkPrice )
	{
		m_pkPrice->InitState( true, true, this );
	}

	if( m_pkBtnClose )
	{
		m_pkBtnClose->InitState( true, true, this );
		m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityPurchaseBox::OnExit );
		m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCQuantityPurchaseBox::OnEnterCursor );
		m_pkBtnClose->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkBtnUp )
	{
		m_pkBtnUp->InitState( true, true, this );
		m_pkBtnUp->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityPurchaseBox::OnUp );
		m_pkBtnUp->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCQuantityPurchaseBox::OnEnterCursor );
	}

	if( m_pkBtnDown )
	{
		m_pkBtnDown->InitState( true, true, this );
		m_pkBtnDown->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityPurchaseBox::OnDown );
		m_pkBtnDown->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCQuantityPurchaseBox::OnEnterCursor );
	}

	if( m_pkBtnPurchase )
	{
		m_pkBtnPurchase->InitState( true, true, this );
		m_pkBtnPurchase->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCQuantityPurchaseBox::OnBuy );
		m_pkBtnPurchase->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCQuantityPurchaseBox::OnEnterCursor );
	}

	if( m_pkCount )
	{
		m_pkCount->InitState( true, true, this );
		m_pkCount->SetEditType(KD3DEdit::EDIT_NUMBER);
	}

	for (int i = 0; i < 2; i++)
	{
		m_saGPMark[i]->InitState(false, true, this);
		m_saVPMark[i]->InitState(false, true, this);
		m_saPPMark[i]->InitState(false, true, this);
		m_saDiaMark[i]->InitState(false, true, this);
	}
}

bool KGCQuantityPurchaseBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
	DWORD dwGoodsID = static_cast<DWORD>( kInfo_.m_lParam );
	KBuySellItemReq::EMONEYTYPE  eMoneyType = static_cast<KBuySellItemReq::EMONEYTYPE>( kInfo_.m_lParam2 );

	if( dwGoodsID == -1 ) { 
		return false;
	}

	m_eMoneyType = eMoneyType;

	m_pkItem = g_MyD3D->m_kItemMgr.GetItemData( dwGoodsID );
	if(m_pkItem == NULL)
		return false;

	m_iPressCount = 0;

	return true;
}

void KGCQuantityPurchaseBox::OnDown()
{
	m_iPressCount = 0;

	m_uiQuantity--;
	
	if( m_uiQuantity <= 0 ) {
		m_uiQuantity = 1;
	}

	SetPurchaseNum( m_uiQuantity );
}

void KGCQuantityPurchaseBox::OnUp()
{
	m_iPressCount = 0;

	m_uiQuantity++;
	int iMaxQuantity = GetAvailableNum();

	if( m_uiQuantity > iMaxQuantity )
	{
		m_uiQuantity = iMaxQuantity;
	}

	SetPurchaseNum( m_uiQuantity );
}

void KGCQuantityPurchaseBox::OnEnterCursor()
{
	g_KDSound.Play( "30" );
}

void KGCQuantityPurchaseBox::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCQuantityPurchaseBox::OnBuy()
{
	m_iPressCount = 0;

	if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANT_BUY_IN_ROOM), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

	if( false == g_pItemMgr->CheckExtraInventoryExist() )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

	std::wostringstream strmText1;
	std::wostringstream strmText2;

	if ( m_eMoneyType == KBuySellItemReq::EMT_GAMEPOINT )
	{
		strmText1 << m_pkItem->strItemName;
		strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", m_uiPrice );
		int iQuantity = m_pkItem->iItemTypeValue > 0 ? ( m_pkItem->iItemTypeValue * m_uiQuantity ) : m_uiQuantity;
		OnExit();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_BUY, m_pkItem->dwGoodsID, iQuantity, true);
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_CRYSTAL )
	{
		strmText1 << m_pkItem->strItemName;
		strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_THE_NUMBER, "iI", m_uiPrice, STR_ID_CRYSTAL_TEXT );
		int iQuantity = m_pkItem->iItemTypeValue > 0 ? ( m_pkItem->iItemTypeValue * m_uiQuantity ) : m_uiQuantity;
		OnExit();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_BUY_FOR_CRYSTAL, m_pkItem->dwGoodsID, iQuantity, true);
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_VIRTUALCASH || m_eMoneyType == KBuySellItemReq::EMT_CASH )
	{
		strmText1 << g_pkStrLoader->GetString(STR_ID_AGIT_MENU_SHOP) << L" " << m_uiQuantity << L"x " << m_pkItem->strItemName << L" ?";
		strmText2 << m_uiPrice << L" " << g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_4);
		int iQuantity = m_pkItem->iItemTypeValue > 0 ? (m_pkItem->iItemTypeValue * m_uiQuantity) : m_uiQuantity;
		OnExit();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), strmText2.str(), KGCUIScene::GC_MBOX_USE_BUY_FOR_VP_AMMOUNT, m_pkItem->dwGoodsID, iQuantity, true);
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_GEM )
	{
		int pQuantity = m_uiQuantity * g_kGlobalValue.m_iCurrentUseItemMultiplier;
		strmText1 << pQuantity << L"x " << m_pkItem->strItemName;
		g_kGlobalValue.m_iCurrentUseItemMultiplier = m_uiQuantity;
		GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( g_kGlobalValue.m_pkCurrentUseItemCurrency );
		OnExit();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
			g_pkStrLoader->GetReplacedString( STR_ID_BUY_HERO_ITEM_COIN_MATERIAL, "lil", strmText1.str(), ( g_kGlobalValue.m_iCurrentUseItemPrice * m_uiQuantity ), kItem->strItemName ), L"",
			KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM, g_kGlobalValue.m_pkCurrentUseItemUID, g_kGlobalValue.m_iCurrentUseItemQuantity, true, true, true );
	}
	else
	{
		strmText1 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_POINT_ITEM, "li", m_pkItem->strItemName, m_uiPrice );
		OnExit();
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, strmText1.str(), L"", KGCUIScene::GC_MBOX_USE_BUY_POINT_ITEM, m_pkItem->dwGoodsID, m_uiQuantity );
	}
}

void KGCQuantityPurchaseBox::OnTextEnter()
{
	int count = static_cast<int>( _wtoi ( m_pkCount->GetText() ) );

	if( count == m_uiQuantity )
	{
		return;
	}

	int iMaxQuantity = GetAvailableNum();

	if( count > iMaxQuantity ) {
		count = iMaxQuantity;
	} else if( count <= 0 ) {
		count = 1;
	}

	SetPurchaseNum( count );
	m_uiQuantity = count;
}

void KGCQuantityPurchaseBox::SetPurchaseNum( int iNum )
{
	std::wostringstream strmItemNum;
	std::wostringstream strmGPPrice;

	m_uiPrice = iNum * GetPrice();

	strmItemNum << iNum;
	strmGPPrice << m_uiPrice;

	m_pkCount->SetText( strmItemNum.str().data(), false );
	m_pkTotal->SetText( strmGPPrice.str().data() );
}

int KGCQuantityPurchaseBox::GetAvailableNum()
{
	DWORD dwPrice = GetPrice();
	int nCount = 0;

	if( m_eMoneyType == KBuySellItemReq::EMT_GAMEPOINT )
		nCount = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() / dwPrice;
	else if ( m_eMoneyType == KBuySellItemReq::EMT_CRYSTAL )
		nCount = g_pItemMgr->GetInventoryItemDuration( CRYSTAL_ITEM_ID ) / dwPrice;
	else if ( m_eMoneyType == KBuySellItemReq::EMT_GEM )
		nCount = g_pItemMgr->GetInventoryItemDuration( g_kGlobalValue.m_pkCurrentUseItemCurrency ) / dwPrice;
	else if ( m_eMoneyType == KBuySellItemReq::EMT_VIRTUALCASH || m_eMoneyType == KBuySellItemReq::EMT_CASH )
		nCount = g_kGlobalValue.m_dwVirtualCash / dwPrice;
	else
		nCount = (int)g_kGlobalValue.m_iGCPoint / dwPrice;

	if( nCount == 0 )
		nCount = 1;

	return nCount;
}

bool KGCQuantityPurchaseBox::CheckAvailableNum( int iNum )
{
	int iTotalPrice = GetPrice() * iNum;

	if( m_eMoneyType == KBuySellItemReq::EMT_GAMEPOINT )
		return g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() >= iTotalPrice;

	if( m_eMoneyType == KBuySellItemReq::EMT_CRYSTAL )
		return g_pItemMgr->GetInventoryItemDuration( CRYSTAL_ITEM_ID ) >= iTotalPrice;

	if( m_eMoneyType == KBuySellItemReq::EMT_GEM )
		return g_pItemMgr->GetInventoryItemDuration( g_kGlobalValue.m_pkCurrentUseItemCurrency ) >= iTotalPrice;

	if ( m_eMoneyType == KBuySellItemReq::EMT_VIRTUALCASH || m_eMoneyType == KBuySellItemReq::EMT_CASH )
		return g_kGlobalValue.m_dwVirtualCash >= iTotalPrice;
	
	return  (int)g_kGlobalValue.m_iGCPoint >= iTotalPrice;
}

DWORD KGCQuantityPurchaseBox::GetPrice()
{
	if( m_eMoneyType == KBuySellItemReq::EMT_GAMEPOINT || m_eMoneyType == KBuySellItemReq::EMT_CRYSTAL || m_eMoneyType == KBuySellItemReq::EMT_VIRTUALCASH || m_eMoneyType == KBuySellItemReq::EMT_CASH )
	{
		return m_pkItem->dwPrice;
	}
	else if ( m_eMoneyType == KBuySellItemReq::EMT_GEM )
	{
		return g_kGlobalValue.m_iCurrentUseItemPrice;
	}
	else
	{
		int iPrice = g_pItemMgr->GetPointItemPrice( m_pkItem->dwGoodsID );
		return iPrice;
	}
}