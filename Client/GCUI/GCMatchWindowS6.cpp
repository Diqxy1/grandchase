#include "stdafx.h"
#include "GCRoomListS6.h"
#include "GCMatchWindowS6.h"

IMPLEMENT_CLASSNAME( KGCMatchWindowS6 );
IMPLEMENT_WND_FACTORY( KGCMatchWindowS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchWindowS6, "gc_match_window_s6" );

KGCMatchWindowS6::KGCMatchWindowS6( void )
: m_pkRoomList(NULL)
, m_pkAutoMatchTapBtn(NULL)
, m_pkCustomMatchTapBtn(NULL)
, m_pkCreateRoomBtn(NULL)
, m_pkQuickJoinBtn(NULL)
, m_pkAutoMatch(NULL)
{
	LINK_CONTROL("room_list_s6", m_pkRoomList);
	LINK_CONTROL("auto_matching_s6",m_pkAutoMatch);
	LINK_CONTROL("auto_match_tap", m_pkAutoMatchTapBtn);
	LINK_CONTROL("custom_match_tap", m_pkCustomMatchTapBtn);
	LINK_CONTROL("create_room_btn",m_pkCreateRoomBtn);
	LINK_CONTROL("quick_join_btn",m_pkQuickJoinBtn);
}

KGCMatchWindowS6::~KGCMatchWindowS6( void )
{
}

void KGCMatchWindowS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkAutoMatchTapBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickAutoMatchTap );
	GCWND_MSG_MAP( m_pkCustomMatchTapBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCustomMatchTap );
	GCWND_MSG_MAP( m_pkCreateRoomBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnCreateRoom );
	GCWND_MSG_MAP( m_pkQuickJoinBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnQuickJoin );

}

void KGCMatchWindowS6::OnCreate( void )
{
	m_pkRoomList->SetRoomListType( KGCRoomListS6::KRL_ROOM_LIST_TYPE_MATCH );
	m_pkRoomList->InitState(false,true,this);
	m_pkAutoMatch->InitState(true,true,this);

    if ( GC_GLOBAL_DEFINE::bAutoMatch ) {
        m_pkAutoMatchTapBtn->InitState(true,true,this);
	    m_pkAutoMatchTapBtn->Lock(true);
	    m_pkCustomMatchTapBtn->InitState(true,true,this);
    }
    else
    {
        m_pkAutoMatchTapBtn->InitState(false);
        m_pkCustomMatchTapBtn->InitState(false);
    }
	m_pkCreateRoomBtn->InitState(false,true,this);
	m_pkQuickJoinBtn->InitState(false,true,this);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_PARTY_CREATE);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_PARTY_CREATE, this, &KGCMatchWindowS6::OnClickAutoMatchTap);

}

void KGCMatchWindowS6::OnClickAutoMatchTap( void )
{
	if ( false == SiKGCChannelManager()->GoAutoMatch() )
		return;

	m_pkAutoMatchTapBtn->Lock(true);
	m_pkCustomMatchTapBtn->Lock(false);

	m_pkRoomList->ToggleRender(false);
	m_pkCreateRoomBtn->ToggleRender(false);
	m_pkQuickJoinBtn->ToggleRender(false);
	m_pkAutoMatch->ToggleRender(true);
	m_pkAutoMatch->UpdateState();
}

void KGCMatchWindowS6::OnClickCustomMatchTap( void )
{
	if ( false == SiKGCChannelManager()->GoCustomMatch() )
		return;

	m_pkAutoMatchTapBtn->Lock(false);
	m_pkCustomMatchTapBtn->Lock(true);

	m_pkRoomList->ToggleRender(true);
	m_pkCreateRoomBtn->ToggleRender(true);
	m_pkQuickJoinBtn->ToggleRender(true);
	m_pkAutoMatch->ToggleRender(false);
}

void KGCMatchWindowS6::OnCreateRoom()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CREATE_ROOM_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
}

void KGCMatchWindowS6::OnQuickJoin()
{
	if( FAILED( SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_QUICK, -1, L"", SiKGCRoomManager()->IsChannelRoomList() ) ) )
	{
		KRoomOptions options;
		if (FAILED(SiKGCRoomManager()->CreateRoom(KGCRoomManager::m_vecRandomRoomNameList[rand() % KGCRoomManager::m_vecRandomRoomNameList.size()].c_str(), L"", MAX_PLAYER_NUM, true, options)))
		{
			g_MyD3D->m_pStateMachine->GoMatch();
		}
	}
}

void KGCMatchWindowS6::EnterScene()
{
	if ( SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) )
	{
		m_pkAutoMatchTapBtn->Lock(true);
		m_pkCustomMatchTapBtn->Lock(false);

    	m_pkRoomList->ToggleRender(false);
		m_pkCreateRoomBtn->ToggleRender(false);
		m_pkQuickJoinBtn->ToggleRender(false);
		m_pkAutoMatch->ToggleRender(true);
		m_pkAutoMatch->UpdateState();
	}
	else {
		m_pkAutoMatchTapBtn->Lock(false);
		m_pkCustomMatchTapBtn->Lock(true);

    	m_pkRoomList->ToggleRender(true);
		m_pkCreateRoomBtn->ToggleRender(true);
		m_pkQuickJoinBtn->ToggleRender(true);
		m_pkAutoMatch->ToggleRender(false);
	}

    m_pkAutoMatchTapBtn->ToggleRender(g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP ));
    m_pkCustomMatchTapBtn->ToggleRender(g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP ));
}
