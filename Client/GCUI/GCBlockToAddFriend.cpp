#include "stdafx.h"
#include ".\GCBlockToAddFriend.h"
//
//

//

//


//

IMPLEMENT_CLASSNAME( GCBlockToAddFriend );
IMPLEMENT_WND_FACTORY( GCBlockToAddFriend );
IMPLEMENT_WND_FACTORY_NAME( GCBlockToAddFriend, "gc_nateon_block_to_add_friend" );

GCBlockToAddFriend::GCBlockToAddFriend(void)
: m_pkTitle(NULL)
, m_pkOK(NULL)
, m_pkCancel(NULL)
, m_pkDesc(NULL)
, m_pkDesc2(NULL)
{
	LINK_CONTROL( "message_static1", m_pkDesc );
	LINK_CONTROL( "message_static2", m_pkDesc2 );
	LINK_CONTROL( "ok_button", m_pkOK );
	LINK_CONTROL( "cancel_button", m_pkCancel );
	LINK_CONTROL( "title_static", m_pkTitle );
}

GCBlockToAddFriend::~GCBlockToAddFriend(void)
{
}

void GCBlockToAddFriend::OnCreate()
{
	m_pkOK->InitState( true, true, this );
	m_pkOK->SetHotKey( DIK_RETURN );
	m_pkOK->SetHotKey( DIK_NUMPADENTER );
	m_pkCancel->InitState( true, true, this );
	m_pkCancel->SetHotKey( DIK_ESCAPE );

	m_pkTitle->InitState( true );
	m_pkTitle->SetAlign( DT_CENTER );

	m_pkDesc->InitState( true, true, this );
	m_pkDesc2->InitState( true, true, this );

	m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_NATEON_ADD_BUDDY ) );

	//m_pkDesc->SetText( g_pkStrLoader->GetString( STR_ID_UNBLOCK_FRIEND_1 ) );
	m_pkDesc2->SetText( g_pkStrLoader->GetString( STR_ID_UNBLOCK_FRIEND_2 ) );
}

void GCBlockToAddFriend::SetFriendNickName( const DWORD dwUID, const std::wstring wstrNickName )
{	
	m_pkDesc->SetText( g_pkStrLoader->GetReplacedString( STR_ID_UNBLOCK_FRIEND_1, "L", wstrNickName ) );

	m_wstrNickName = wstrNickName;
	m_dwUID = dwUID;
}

void GCBlockToAddFriend::OnDestroyComplete()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_DESTROYED ) );
}

void GCBlockToAddFriend::OnCancel()
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );
	KP2P::GetInstance()->m_kMsgServer->Send_UnBlockFriendReq_S2( m_dwUID );
	//g_pkUIScene->m_pkNateOn->GetMultiMenu()->UnBlockFriendAck();

	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void GCBlockToAddFriend::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
	GCWND_MSG_MAP( m_pkCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );

	if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
	{
		OnOK();
	}
	else if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_ESCAPE )
	{
		OnCancel();
	}
}

void GCBlockToAddFriend::FrameMoveInEnabledState()
{

}

void GCBlockToAddFriend::OnOK()
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );
	KP2P::GetInstance()->m_kMsgServer->Send_UnBlockMoveFriendReq_S2( m_dwUID );
	//g_pkUIScene->m_pkNateOn->GetMultiMenu()->UnBlockFriendAck();

	SpeakToActionListener( KActionEvent( this, KD3DWnd::D3DWE_EDIT_RETURN ) );
}

void GCBlockToAddFriend::OnCreateComplete()
{

}

bool GCBlockToAddFriend::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    this->SetFriendNickName(static_cast<DWORD>(kInfo_.m_lParam), kInfo_.m_strText1 );
    return true;
}