#include "stdafx.h"
#include "GCRoomListDlgS6.h"
#include "GCRoomListS6.h"
#include "GCDungeonInfoS6.h"
#include "GCChampionPopup.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCRoomListDlgS6 );
IMPLEMENT_WND_FACTORY( KGCRoomListDlgS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomListDlgS6, "gc_room_list_dlg_s6" );

KGCRoomListDlgS6::KGCRoomListDlgS6( void )
: m_pkRoomList(NULL)
, m_pkDungeonInfo(NULL)
, m_pkCloseBtn(NULL)
, m_pkCreateRoomBtn(NULL)
, m_pkQuickJoinBtn(NULL)
, m_pkDungeonSelBtn(NULL)
, m_pkHelpBtn(NULL)
, m_pkHeroHelpText(NULL)
, m_pkChampionPopup(NULL)
{
	LINK_CONTROL("room_list_s6", m_pkRoomList);
	LINK_CONTROL("dungeon_info_s6", m_pkDungeonInfo);
	LINK_CONTROL("end_btn", m_pkCloseBtn);
	LINK_CONTROL( "create_room_btn",m_pkCreateRoomBtn );
	LINK_CONTROL( "quick_join_btn",m_pkQuickJoinBtn );
	LINK_CONTROL( "dungeon_sel_btn", m_pkDungeonSelBtn);
	LINK_CONTROL( "btn_help", m_pkHelpBtn );
	LINK_CONTROL( "hero_monster", m_pkHeroHelpText );
	LINK_CONTROL( "champion_popup", m_pkChampionPopup );
}

KGCRoomListDlgS6::~KGCRoomListDlgS6( void )
{
}

void KGCRoomListDlgS6::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if ( event.m_pWnd == m_pkHelpBtn )
		{
			if ( !m_pkChampionPopup->IsRenderOn() )
				m_pkChampionPopup->ToggleRender( true );
		}
		else
		{
			if ( m_pkChampionPopup->IsRenderOn() )
				m_pkChampionPopup->ToggleRender( false );
		}
	}
}

void KGCRoomListDlgS6::OnCreate( void )
{
	m_pkRoomList->SetRoomListType( KGCRoomListS6::KRL_ROOM_LIST_TYPE_DUNGEON );
	g_pkUIMgr->DisconnectAllEventProcedure(GetUID());
	m_pkRoomList->InitState(true, true, this);
	m_pkDungeonInfo->InitState(true, true, this);
	m_pkCloseBtn->InitState(true, true, this);
	m_pkHelpBtn->InitState(false, true, this);
	m_pkHeroHelpText->InitState(false, true, this);
	m_pkChampionPopup->InitState(false, true, this);
	m_pkCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListDlgS6::OnClickBtnClose);
	m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_CREATE);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_CREATE, this, &KGCRoomListDlgS6::OnClickBtnClose);
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_JOIN);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_JOIN, this, &KGCRoomListDlgS6::OnClickBtnClose);
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KD3DWnd::D3DWE_SCENE_CHANGE);
	g_pkUIMgr->ConnectGlobalEventProcedure(KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCRoomListDlgS6::OnClickBtnClose);
	m_pkCreateRoomBtn->InitState(true,true,this);
	m_pkQuickJoinBtn->InitState(true,true,this);
	m_pkDungeonSelBtn->InitState(true,true,this);
	m_pkCreateRoomBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCRoomListDlgS6::OnClickBtnCreateRoom);
	m_pkQuickJoinBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCRoomListDlgS6::OnClickBtnQuickJoinRoom);
	m_pkDungeonSelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this,&KGCRoomListDlgS6::OnClickBtnDungeonSelect);
}

void KGCRoomListDlgS6::OnCreateComplete( void )
{
	if( SiKGCWorldMapManager()->IsHellMode( SiKGCWorldMapManager()->GetGameMode() ) || SiKGCWorldMapManager()->IsBreakMode( SiKGCWorldMapManager()->GetGameMode() ) )
	{
		m_pkHelpBtn->ToggleRender( true );
		m_pkHeroHelpText->ToggleRender( true );
	}
	else
	{
		m_pkHelpBtn->ToggleRender( false );
		m_pkHeroHelpText->ToggleRender( false );
	}
}

void KGCRoomListDlgS6::OnClickBtnDungeonSelect( void )
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_DUNGEON_SELECT_DLG_S6,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL, 0,0,false,true);
}

void KGCRoomListDlgS6::OnClickBtnClose( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCRoomListDlgS6::OnClickBtnCreateRoom( void )
{
	if( !SiKGCRoomManager()->CheckValidDungeon() )
		return;

	if (SiKGCWorldMapManager()->IsHeroDungeonMode())
	{
		std::pair<int, int> pairTemp = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(SiKGCWorldMapManager()->GetGameMode());
		bool IsHeroTicketUsed = g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeonPlayTicket(SiKGCWorldMapManager()->GetGameMode());

		if (pairTemp.first >= pairTemp.second && !IsHeroTicketUsed)
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_HERO_TICKET_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		else
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_CREATE_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
	}
	else
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_CREATE_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
	}
}

void KGCRoomListDlgS6::OnClickBtnQuickJoinRoom( void )
{
	if( !SiKGCRoomManager()->CheckValidDungeon() )
		return;

	if (SiKGCWorldMapManager()->IsHeroDungeonMode())
	{
		std::pair<int, int> pairTemp = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(SiKGCWorldMapManager()->GetGameMode());
		bool IsHeroTicketUsed = g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeonPlayTicket(SiKGCWorldMapManager()->GetGameMode());

		if (pairTemp.first >= pairTemp.second && !IsHeroTicketUsed)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_HERO_TICKET_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		}
		else
		{
			if (FAILED(SiKGCRoomManager()->JoinRoom(KJoinRoomReqInfo::JOIN_QUICK, -1, L"", SiKGCRoomManager()->IsChannelRoomList())))
			{
				if (Result_Join == EPWET_QUICK_JOIN_TIME_OVER)
					return;

				KRoomOptions options;
				if (FAILED(SiKGCRoomManager()->CreateRoom(KGCRoomManager::m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str(), L"", MAX_PLAYER_NUM, true, options)))
				{
					return;
				}
			}
			OnClickBtnClose();
		}
	}
	else
	{
		if (FAILED(SiKGCRoomManager()->JoinRoom(KJoinRoomReqInfo::JOIN_QUICK, -1, L"", SiKGCRoomManager()->IsChannelRoomList())))
		{
			if (Result_Join == EPWET_QUICK_JOIN_TIME_OVER)
				return;

			KRoomOptions options;
			if (FAILED(SiKGCRoomManager()->CreateRoom(KGCRoomManager::m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str(), L"", MAX_PLAYER_NUM, true, options)))
			{
				return;
			}
		}
		OnClickBtnClose();
	}
}