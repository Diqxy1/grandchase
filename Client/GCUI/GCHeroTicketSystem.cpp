#include "stdafx.h"
#include "KGCWorldMapManager.h"
#include "GCHeroTicketSystem.h"

IMPLEMENT_CLASSNAME( KGCHeroTicketSystem );
IMPLEMENT_WND_FACTORY( KGCHeroTicketSystem );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroTicketSystem, "gc_heroticket_panel_s6" );

KGCHeroTicketSystem::KGCHeroTicketSystem( void )
: m_pkItemImg( NULL )
, m_pkQuantity( NULL )
, m_pkDescription( NULL )
, m_pkConfirmBtn( NULL )
, m_pkExitBtn( NULL )
, m_pkTicketItem( NULL )
{
	LINK_CONTROL( "item_img",m_pkItemImg );	
	LINK_CONTROL( "ticket_num",m_pkQuantity );	
	LINK_CONTROL( "ticket_explanation",m_pkDescription );	
	LINK_CONTROL( "confirm_btn",m_pkConfirmBtn );	
	LINK_CONTROL( "exit_btn",m_pkExitBtn );	
}

KGCHeroTicketSystem::~KGCHeroTicketSystem( void )
{
}

void KGCHeroTicketSystem::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroTicketSystem::OnCreate( void )
{
	if( m_pkItemImg )
		m_pkItemImg->InitState( true );
	if ( m_pkQuantity )
	{
		m_pkQuantity->InitState( true, true, this );
		m_pkQuantity->SetAlign( DT_CENTER );
	}
	if ( m_pkDescription )
	{
		m_pkDescription->InitState( true, true, this );
		m_pkDescription->SetAlign( DT_CENTER );
	}
	if ( m_pkConfirmBtn )
	{
		m_pkConfirmBtn->InitState( true, true, this );
		m_pkConfirmBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroTicketSystem::OnConfirmButton );
	}
	if ( m_pkExitBtn )
	{
		m_pkExitBtn->InitState( true, true, this );
		m_pkExitBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroTicketSystem::OnExitButton );
	}
}

void KGCHeroTicketSystem::OnCreateComplete( void )
{
	UpdateDialog();
	if ( emState != GS_ROOM )
		if ( m_pkExitBtn )
			m_pkExitBtn->SetHotKey( DIK_ESCAPE );
}

void KGCHeroTicketSystem::UpdateDialog( void )
{
	std::wstringstream strItemDesc, strmItemNum;
	emState = g_MyD3D->m_pStateMachine->GetState();
	if( emState == GS_ROOM )
		emGameMode = SiKGCRoomManager()->GetGameMode();
	else
		emGameMode = SiKGCWorldMapManager()->GetGameMode();
	g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeon( emGameMode, HeroDungeonInfo );
	GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( HeroDungeonInfo.m_dwHeroTicketID / 10 );
	if ( kItem == NULL )
		return;
	m_pkItemImg->SetItemInfo( HeroDungeonInfo.m_dwHeroTicketID / 10 );
	if ( m_pkDescription )
		m_pkDescription->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_HERO_TICKET_SYSTEM_DESC, "l", kItem->strItemName ) );
	m_pkTicketItem = g_MyD3D->m_kItemMgr.GetInventoryItemFromID( HeroDungeonInfo.m_dwHeroTicketID / 10 );
	if( m_pkTicketItem != NULL )
	{
		if ( m_pkConfirmBtn )
			m_pkConfirmBtn->Lock( false );
		strmItemNum << g_pItemMgr->GetInventoryItemDuration( HeroDungeonInfo.m_dwHeroTicketID / 10 );
		if ( m_pkQuantity )
			m_pkQuantity->SetText( strmItemNum.str() );
	}
	else
	{
		if ( m_pkConfirmBtn )
			m_pkConfirmBtn->Lock( true );
		if ( m_pkQuantity )
			m_pkQuantity->SetText( L"0" );
	}
}

void KGCHeroTicketSystem::OnConfirmButton( void )
{
	int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
	KP2P::GetInstance()->Send_UseHeroTicketReq( (int)emGameMode, iCharType, (DWORD)HeroDungeonInfo.m_dwHeroTicketID, (DWORD)m_pkTicketItem->m_ItemUID );
	Result_ReceiveUseHeroTicketReq = INT_MAX;
	g_MyD3D->WaitForServerAck( Result_ReceiveUseHeroTicketReq, INT_MAX, 3000, INT_MAX );
	if( Result_ReceiveUseHeroTicketReq == 0 )
	{
		if ( emState == GS_ROOM )
		{
			g_pkUIScene->m_pkRoomSceneS6->HeroTicketPanelManager( false );
			return;
		}
		else
		{
			OnExitButton();
			return;
		}
	}
	OnExitButton();
}

void KGCHeroTicketSystem::OnExitButton( void )
{
	if ( emState == GS_ROOM )
		g_pkUIScene->m_pkRoomSceneS6->OnExitRoom();
	else
		g_pkUIScene->m_pkHeroTicketDlg->OnExitWindow();
}