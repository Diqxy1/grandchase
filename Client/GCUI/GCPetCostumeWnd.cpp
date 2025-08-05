#include "StdAfx.h"
#include "GCPetCostumeWnd.h"
#include "GCItemImgWnd.h"

#include "MyD3D.h"
#include "GCUI/GCMyInfoScene.h"

IMPLEMENT_CLASSNAME( KGCPetCostumeWnd );
IMPLEMENT_WND_FACTORY( KGCPetCostumeWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCPetCostumeWnd, "gc_kgcpetcostumewnd" );

KGCPetCostumeWnd::KGCPetCostumeWnd(void)
: m_dwItemID( 0 )
, m_dwBaseItemID( 0 )
, m_iEvolutionLv( 0 )
, m_bBuybtnClick( false )
{
	m_pkBack = NULL;
	m_pkItemImg = NULL;
	m_pkUnequip = NULL;
	m_pkEquip = NULL;
	m_pkBuy = NULL;
	LINK_CONTROL ( "btn_buy", m_pkBuy );
	LINK_CONTROL ( "btn_equip", m_pkEquip );
	LINK_CONTROL ( "btn_unequip", m_pkUnequip );
	LINK_CONTROL ( "pet_image", m_pkItemImg );
	LINK_CONTROL( "back", m_pkBack );
}

KGCPetCostumeWnd::~KGCPetCostumeWnd(void)
{
}

void KGCPetCostumeWnd::OnCreate( void )
{
	m_pkItemImg->InitState( m_pkItemImg->IsRenderOn() , true );
	m_pkUnequip->InitState( m_pkUnequip->IsRenderOn(), true, this );
	m_pkEquip->InitState( m_pkEquip->IsRenderOn(), true, this );
	m_pkBuy->InitState( m_pkBuy->IsRenderOn(), true, this );
	m_pkBack->InitState( true, true );
}

void KGCPetCostumeWnd::FrameMoveInEnabledState( void )
{
	if( m_dwItemID == 0 ||
		m_pkItemImg->IsRenderOn() == false )
	{
		m_pkBuy->ToggleRender( false );
		m_pkEquip->ToggleRender( false );
		m_pkUnequip->ToggleRender( false );
	}
	else
	{
		if( g_pItemMgr->FindInventoryForItemID( m_dwItemID ) )
		{
			m_pkBuy->ToggleRender( false );
			if( g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID == m_dwItemID )
			{
				m_pkEquip->ToggleRender( false );
				m_pkUnequip->ToggleRender( true );
				m_pkBack->Activate( true );
			}
			else if( g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwID == m_dwItemID )
			{
				m_pkBack->Activate( g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID == 0 );

				m_pkEquip->ToggleRender( g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID != 0 );
				m_pkUnequip->ToggleRender( false );
			}
			else
			{
				m_pkEquip->ToggleRender( true );
				m_pkUnequip->ToggleRender( false );
				m_pkBack->Activate( false );
			}
		}
		else
		{
            GCItem* pItem = g_pItemMgr->GetItemData( m_dwItemID );
            if( pItem == NULL || pItem->bNotBuyItem ) { 
                m_pkBuy->ToggleRender( false );
                m_pkEquip->ToggleRender( false );
                m_pkUnequip->ToggleRender( false );
            }
            else { 
                m_pkBuy->ToggleRender( true );
                m_pkEquip->ToggleRender( false );
                m_pkUnequip->ToggleRender( false );
            }
		}
	}
}

void KGCPetCostumeWnd::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBuy,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickBuy );
	GCWND_MSG_MAP( m_pkEquip,	KD3DWnd::D3DWE_BUTTON_CLICK, OnClickEquip );
	GCWND_MSG_MAP( m_pkUnequip,	KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUnEquip );
}

void KGCPetCostumeWnd::OnClickBuy()
{
	m_bBuybtnClick = true;

	if( g_pItemMgr->CheckExtraInventoryExist() == false )
	{
		int iInvenCapacity = (int)g_kGlobalValue.m_iInvenCapacity;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
		g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
		g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", iInvenCapacity ),
		g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ),
		KGCUIScene::GC_MBOX_USE_NORMAL,
		0,0,true,true);
		return;
	}

	bool bOpenBuyWnd = true;
	GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwItemID );
#if defined( VIRTUAL_CASH ) && !defined( GAME_CASH )
	if( g_kGlobalValue.m_dwVirtualCash >= kItem->dwPrice )
	{
		g_pkUIScene->m_pkAskUseVirtualCash->SetItemInfo( m_dwItemID, -1 );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_VIRTUAL_CASH, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		bOpenBuyWnd = false;
	}
#endif

	if( bOpenBuyWnd )
	{
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( m_dwItemID );
#else
    #ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( m_dwItemID, false, true );
    #else    
        g_MyD3D->m_kItemMgr.BuyItem( m_dwItemID, -1  );
    #endif
#endif
	}
}
void KGCPetCostumeWnd::OnClickEquip()
{
	if( m_dwItemID == m_dwBaseItemID )
	{
		OnClickUnEquip();
		return;
	}
	g_MyD3D->m_TempPlayer.SetMyPlayer();
	g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID = m_dwItemID;
	KItem* pInvenItem = NULL;
	if( g_pItemMgr->FindInventoryForItemID( m_dwItemID, &pInvenItem ) )
	g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwUID = pInvenItem->m_ItemUID;
    g_MyD3D->m_TempPlayer.RemovePlayerObject();
	g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
	g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCPetCostumeWnd::OnClickUnEquip()
{
	g_MyD3D->m_TempPlayer.SetMyPlayer();
	g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID = 0;
	g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwUID = 0;
    g_MyD3D->m_TempPlayer.RemovePlayerObject();
	g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
	g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCPetCostumeWnd::SetItemID( DWORD dwItemID, bool bActive /*= true*/ )
{
	m_dwItemID = dwItemID;
	m_pkItemImg->ToggleRender( true );
	m_pkItemImg->SetPetItemInfo( dwItemID, m_iEvolutionLv, bActive?-1:0x55FFB0B0 );
}

void KGCPetCostumeWnd::HideImage()
{
	m_pkItemImg->ToggleRender( false );
	m_pkBack->Lock( true );
}

void KGCPetCostumeWnd::ShowImage()
{
	m_pkItemImg->ToggleRender( true );
	m_pkBack->Lock( false );
}

void KGCPetCostumeWnd::SetBaseItemID( DWORD dwItemID, int iEvolutionLv )
{
	m_dwBaseItemID = dwItemID;
	m_iEvolutionLv = iEvolutionLv;
}

void KGCPetCostumeWnd::OnClick()
{
	if( m_pkEquip->IsRenderOn() )
		OnClickEquip();
	else if( m_pkUnequip->IsRenderOn() )
		OnClickUnEquip();
	else if( m_pkBuy->IsRenderOn() && !m_bBuybtnClick)
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PET_COSTUME3 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,true,true);
	else
		OnClickUnEquip();


	m_bBuybtnClick = false;
}