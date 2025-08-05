#include "stdafx.h"
#include "GCPresentFriendList.h"

IMPLEMENT_CLASSNAME( KGCPresentFriendList );
IMPLEMENT_WND_FACTORY( KGCPresentFriendList );
IMPLEMENT_WND_FACTORY_NAME( KGCPresentFriendList, "gc_present_friend_list" );

KGCPresentFriendList::KGCPresentFriendList( void )
{
	m_pkCloseBtn = NULL;
	m_pkOkBtn = NULL;
	LINK_CONTROL( "btn_close",			m_pkCloseBtn );
	LINK_CONTROL( "btn_ok",				m_pkOkBtn );

	m_pkFriendTree = NULL;
	LINK_CONTROL( "friend_list_tree",	m_pkFriendTree );

	m_pkRefreshBtn = NULL;
	LINK_CONTROL( "btn_refresh",		m_pkRefreshBtn );
}

KGCPresentFriendList::~KGCPresentFriendList( void )
{
}

void KGCPresentFriendList::OnCreate( void )
{
	m_pkCloseBtn->InitState( true, true, this );
	m_pkCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentFriendList::OnClickCloseBtn );
	m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

	m_pkOkBtn->InitState( true, true, this );
	m_pkOkBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentFriendList::OnClickOkBtn );
	m_pkOkBtn->SetHotKey( DIK_RETURN );
	m_pkOkBtn->SetHotKey( DIK_NUMPADENTER );

	m_pkRefreshBtn->InitState( true, true, this );
	m_pkRefreshBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPresentFriendList::OnClickRefreshBtn );
	m_pkRefreshBtn->SetHotKey( DIK_F5 );

	m_pkFriendTree->InitState( true, true, this );
}

void KGCPresentFriendList::ActionPerformed( const KActionEvent& event )
{
}

void KGCPresentFriendList::OnClickCloseBtn( void )
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE) );
}

void KGCPresentFriendList::OnClickOkBtn( void )
{
	g_pkUIScene->m_pkPresentCashItemWnd->SetNickname( m_pkFriendTree->GetSelectedNickname() );
	if( m_pkFriendTree->GetSelectedNickname().size() != 0 )
		g_pkUIScene->m_pkPresentCashItemWnd->SetCheckedNickname( true ); // 친구 리스트에서 닉네임 선택하면, 닉네임 확인 필요없음
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE) );
}

void KGCPresentFriendList::OnClickRefreshBtn( void )
{
	m_pkFriendTree->UpdateData();
}