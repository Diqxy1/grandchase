#include "stdafx.h"
#include "GCMsnDlg.h"
//
//

//
//
#include "GCMsnChat.h"
#include "GCChatBtn.h"
//

//
//
#include "GCChatBox.h"
#include "MyD3D.h"
#include "GCShopScene.h"
#include "GCSearchBar.h"
#include "KGCUIHelper.h"
//
#include "../GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCMsnDlg );
IMPLEMENT_WND_FACTORY( KGCMsnDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCMsnDlg, "gc_msn_dlg" );

KGCMsnDlg* g_pMsnDlg = NULL;

KGCMsnDlg::KGCMsnDlg(void)
{
	g_pMsnDlg = this;

	m_pkTitle = NULL;
	m_pkNickName = NULL;
	m_pkClose = NULL;
	m_pkAdd = NULL;
	m_pkPageUp = NULL;
	m_pkPageDown = NULL;
	m_pkFriendList = NULL;
	m_pkMsnChat = NULL;

	for( int i=0 ; i<GC_GRADE_NUM ; ++i )
		m_ppkGrade[i] = NULL;

	for( int i=0 ; i<5 ; ++i )
	{
		m_ppkChatBtn[i] = NULL;
		char strName[MAX_PATH];
		sprintf( strName, "chat_btn%d", i+1 );

		LINK_CONTROL( strName, m_ppkChatBtn[i] );
	}

	LINK_CONTROL( "list_btn", m_pkListBtn );

	LINK_CONTROL( "title_static", m_pkTitle );
	LINK_CONTROL( "nickname_static", m_pkNickName );
	LINK_CONTROL( "close_btn", m_pkClose );
	LINK_CONTROL( "add_btn", m_pkAdd );
	LINK_CONTROL( "page_up", m_pkPageUp );
	LINK_CONTROL( "page_down", m_pkPageDown );
	LINK_CONTROL( "friend_list", m_pkFriendList );
	LINK_CONTROL( "msn_chat_wnd", m_pkMsnChat );

	LINK_CONTROL( "player_grade_holy",		m_ppkGrade[GC_GRADE_HOLY]);
	LINK_CONTROL( "player_grade_royal",		m_ppkGrade[GC_GRADE_ROYAL]);
	LINK_CONTROL( "player_grade_mercenary",	m_ppkGrade[GC_GRADE_MERCENARY]);
	LINK_CONTROL( "player_grade_vagabond",	m_ppkGrade[GC_GRADE_VAGABOND]);
}

KGCMsnDlg::~KGCMsnDlg(void)
{
	m_vecChat.clear();
}

void KGCMsnDlg::OnCreate( void )
{
	m_pkClose->SetHotKey( DIK_ESCAPE );
	m_pkClose->InitState( true, true, this );

	m_pkMsnChat->InitState( false, true, NULL );
	m_pkFriendList->InitState( true, true, NULL );

	m_pkPageUp->InitState( true, true, this );
	m_pkPageDown->InitState( true, true, this );
	m_pkPageUp->Lock( true );
	m_pkPageDown->Lock( true );

	m_pkAdd->InitState( true, true, this );
	m_pkAdd->SetToolTip( g_pkStrLoader->GetString(STR_ID_FRIEND_ADD_TOOLTIP) );
	m_pkNickName->SetText( g_kGlobalValue.m_kUserInfo.strNickName );
	m_pkTitle->SetText( g_pkStrLoader->GetString(STR_ID_FRIEND) );

	m_pkListBtn->InitState( true, true, this );
	m_pkListBtn->Lock( true );
	KD3DStatic* pTitle = (KD3DStatic*)m_pkListBtn->GetChild(0);
	pTitle->SetText( g_pkStrLoader->GetString(STR_ID_FRIEND_LIST) );
	pTitle->SetAlign( DT_CENTER );
	pTitle->SetFontColor( 0xff000000 );

	for( int i=0 ; i<5 ; ++i )
		m_ppkChatBtn[i]->InitState( false, true, this );

	SetGrade();
}

void KGCMsnDlg::OnCreateComplete()
{
	m_iSelectChat = -1;
	m_iTop = 0;
	ResetChat();
}

void KGCMsnDlg::SetGrade()
{
    {
        g_pkUIHelper->SetCharacterGrade( m_ppkGrade, g_kGlobalValue.m_kUserInfo.GetUserGrade() );
    }
}

void KGCMsnDlg::OnClose()
{
	m_iSelectChat = -1;
	m_iTop = 0;
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCMsnDlg::FrameMoveInEnabledState( void )
{
	if( m_pkFriendList->IsRenderOn() )
	{
		m_pkFriendList->SetFriendList();
	}

	if( g_pkInput->IsDown( DIK_RETURN ) )
	{
		if( m_pkMsnChat != NULL )
		{
			KD3DEdit* pEdit = m_pkMsnChat->GetEdit();
			if( pEdit != NULL )
				pEdit->SetFocus();			
		}
	}

    for( int i = 0; i < 5; ++i )
    {
        m_ppkChatBtn[i]->SetAlert( false );
    }

    for( int i=0 ; i<(int)m_vecChat.size() ; ++i )
    {
        if( m_vecChat[i].m_bNew == true )
        {
            DWORD newUID = m_vecChat[i].m_dwFriendsUID;

            for( int i = 0; i < 5; ++i )
            {
                if( m_ppkChatBtn[i]->GetUID() == newUID )
                {
                    m_ppkChatBtn[i]->SetAlert( true );
                }
            }
        }
    }
}

void KGCMsnDlg::ActionPerformed( const KActionEvent& event )
{	
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		m_pkFriendList->RenderSubMenu( false );

		for( int i=0 ; i<5 ; ++i )
		{
			if( event.m_pWnd == m_ppkChatBtn[i] )
			{
				m_iSelectChat = i;
				ResetChat();
				return;
			}
		}

		if( event.m_pWnd == m_pkClose )
		{
			OnClose();

            if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
                g_pkWorldmapChatBox->SetFocus();
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH )
                g_pkLobbyChatBox->SetFocus();
            if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
                g_pkRoomChatBox->SetFocus();
			if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP )
				g_pkUIScene->m_pkShopScene->m_pkSearchBar->SetFocus( true );
		}
		else if( event.m_pWnd == m_pkAdd )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_FRIEND_ADD_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		}
		else if( event.m_pWnd == m_pkListBtn )
		{
			m_iSelectChat = -1;
			ResetChat();
		}
		else if( event.m_pWnd == m_pkPageUp )
		{
			m_iTop -= 5;
			if( m_iTop < 0 )
				m_iTop = 0;

			ResetChat();
		}
		else if( event.m_pWnd == m_pkPageDown )
		{
			int iMove = (int)m_vecChat.size() - (m_iTop + 5);
			if( iMove > 5 )
				iMove = 5;

			m_iTop += iMove;

			ResetChat();
		}
	}
}

void KGCMsnDlg::DeleteFriend( DWORD dwUID )
{
	int iIndex = FindChat( dwUID );
	if( iIndex != -1 )
	{
		// 채팅창을 삭제합니다.
		DeleteChat( iIndex );
	}	
}

void KGCMsnDlg::SetChatBox()
{	
	KFriendInfo* kInfo = KP2P::GetInstance()->m_kMsgServer->GetFriend( m_pkFriendList->GetSelectFriend() );

	if( NULL == kInfo )
		return;

	if( KP2P::GetInstance()->m_kMsgServer->IsFriend( kInfo->m_dwUserUID ) == false )
		return;

	if( kInfo->m_bOnline == false || kInfo->m_bStandby == true )
		return;

	int iFind = FindChat( kInfo->m_dwUserUID );

	// 없으면 추가
	if( iFind == -1 )
	{
		MSNCHAT chat;
		chat.m_dwFriendsUID = kInfo->m_dwUserUID;
		m_vecChat.insert( m_vecChat.begin(), chat );
	}
	else
	{
		MSNCHAT chat = m_vecChat[iFind];
		m_vecChat.erase( m_vecChat.begin()+iFind );
		m_vecChat.insert( m_vecChat.begin(), chat );
	}

	m_iTop = 0;
	m_iSelectChat = 0;
	ResetChat();
}

void KGCMsnDlg::ResetChat( bool bToggleChat )
{
	if( IsCreate() == false )
		return;

	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

	if( bToggleChat == true )
		m_pkMsnChat->ToggleRender( false );

	m_pkListBtn->Lock( false );
	m_pkFriendList->ToggleRender( false );

	for( int i=0 ; i<5 ; ++i )
	{
		m_ppkChatBtn[i]->ToggleRender( false );
		m_ppkChatBtn[i]->Lock( false );
	}

	if( m_iSelectChat == -1 )
	{
		m_pkListBtn->Lock( true );
		m_pkFriendList->ToggleRender( true );
	}

    // ㄷㄷ 초기화
    for( int i=0 ; i<5 ; ++i )
    {
        m_ppkChatBtn[i]->SetUID( (DWORD)ULONG_MAX );
    }

	for( int i=0 ; i<5 ; ++i )
	{
		int iIndex = m_iTop+i;
		if( iIndex > (int)m_vecChat.size()-1 )
			break;

		m_ppkChatBtn[i]->SetNickName( KP2P::GetInstance()->m_kMsgServer->GetFriend( m_vecChat[iIndex].m_dwFriendsUID )->m_strNickName );
        m_ppkChatBtn[i]->SetUID( m_vecChat[iIndex].m_dwFriendsUID );
		m_ppkChatBtn[i]->ToggleRender( true );

		if( m_iSelectChat == i )
		{
			m_ppkChatBtn[i]->Lock( true );
			m_pkMsnChat->ToggleRender( true );
			m_pkMsnChat->Clear();
			m_pkMsnChat->SetInfo( m_vecChat[iIndex].m_dwFriendsUID );
			for( int j=0 ; j<(int)m_vecChat[iIndex].m_vecChat.size() ; ++j )
			{
				m_pkMsnChat->AddChatStr( m_vecChat[iIndex].m_vecChat[j], m_vecChat[iIndex].m_vecColor[j] );
			}

			m_vecChat[iIndex].m_bNew = false;
		}
	}

	m_pkPageUp->Lock( (m_iTop == 0) );
	m_pkPageDown->Lock( (m_iTop+5 >= (int)m_vecChat.size()) );
}

int KGCMsnDlg::FindChat( DWORD dwUID )
{
	for( int i=0 ; i<(int)m_vecChat.size() ; ++i )
	{
		if( m_vecChat[i].m_dwFriendsUID == dwUID )
			return i;
	}

	return -1;
}

void KGCMsnDlg::DeleteChat()
{
	int iDelete = m_iSelectChat + m_iTop;
	if( iDelete == (int)m_vecChat.size()-1 )
		--m_iSelectChat;

	m_vecChat.erase( m_vecChat.begin() + iDelete );

	if( m_iTop > 0 )
		--m_iTop;

	ResetChat();
}

void KGCMsnDlg::DeleteChat( int iIndex )
{
	m_vecChat.erase( m_vecChat.begin() + iIndex );

	if( m_iTop > 0 )
		--m_iTop;

	ResetChat();
}

void KGCMsnDlg::AddChatStr( KFriendMsgInfo& kMsg )
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

	int iFind;
	std::wstring strSenderNick;
	
	if( kMsg.m_dwSenderUID == g_kGlobalValue.m_kUserInfo.dwUID )
	{
		// 내가보낸 메시지라면
		iFind = FindChat( kMsg.m_dwReceiverUID );
		strSenderNick = g_kGlobalValue.m_kUserInfo.strNickName;
	}
	else
	{
		// 보낸이가 내 친구목록에 있는지 체크		
		if( KP2P::GetInstance()->m_kMsgServer->IsFriend( kMsg.m_dwSenderUID ) == false )
			return;

		iFind = FindChat( kMsg.m_dwSenderUID );

		// 없으면 채팅목록 추가
		if( iFind == -1 )
		{
			MSNCHAT chat;
			chat.m_bNew = true;
			chat.m_dwFriendsUID = kMsg.m_dwSenderUID;
			m_vecChat.push_back( chat );
			iFind = (int)m_vecChat.size()-1;
		}

		strSenderNick = KP2P::GetInstance()->m_kMsgServer->GetFriend( m_vecChat[iFind].m_dwFriendsUID )->m_strNickName;

		// 현재 채팅중인 창이 아니라면 표시해주삼
		if( iFind != m_iSelectChat + m_iTop )
        {
			m_vecChat[iFind].m_bNew = true;
        }
	}

	if( iFind != -1 )
	{
		std::wstring strNickName;
		strNickName = g_pkStrLoader->GetReplacedString( STR_ID_CHAT_HEADER, "l", strSenderNick );

        DWORD dwColor = 0xFFFFFFFF;
        if ( kMsg.m_dwSenderUID == g_kGlobalValue.m_kUserInfo.dwUID )
            dwColor = 0xFFCDDFF0;

		m_vecChat[iFind].m_vecChat.push_back( strNickName );
        m_vecChat[iFind].m_vecColor.push_back( 0xFFFFFFFF );
		m_vecChat[iFind].m_vecChat.push_back( kMsg.m_strMessage.c_str() );
        m_vecChat[iFind].m_vecColor.push_back( dwColor );
	}
	
	ResetChat( false );
}

bool KGCMsnDlg::IsExistNewChat()
{
	for( int i=0 ; i<(int)m_vecChat.size() ; ++i )
	{
		if( m_vecChat[i].m_bNew == true )
			return true;
	}

	return false;
}

void KGCMsnDlg::UpdateFriend( DWORD dwUID_ )
{
	// 채팅중인 내용을 찾아내서 수정된 내용으로 업데이트 합니다.
	int iFind = FindChat( dwUID_ );
	if( iFind != -1 )
	{
		m_vecChat[iFind].m_dwFriendsUID = dwUID_;
		ResetChat();
	}
}

bool KGCMsnDlg::IsBlockFriend( DWORD dwUID )
{
	std::set<DWORD>::iterator itor;
	itor = m_setBlockList.find( dwUID );

	if( itor == m_setBlockList.end() )
		return false;
	
	return true;
}

void KGCMsnDlg::SetBlockFriend( DWORD dwUID )
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

	// 이미 차단되어있으면 리턴
	if( IsBlockFriend( dwUID ) == true )
		return;

	// 차단목록에 추가
	m_setBlockList.insert( dwUID );

	// 친구목록에 있으면 갱신	
	if( KP2P::GetInstance()->m_kMsgServer->IsFriend( dwUID ) == true )
		UpdateFriend( dwUID );
}

void KGCMsnDlg::SetUnBlockFriend( DWORD dwUID )
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

	// 차단목록에 없으면 리턴
	std::set<DWORD>::iterator itor;
	itor = m_setBlockList.find( dwUID );
	if( itor == m_setBlockList.end() )
		return;

	// 차단목록에서 해제
	m_setBlockList.erase( itor );

	// 친구목록에 있으면 갱신
	KFriendInfo kInfo;
	if( KP2P::GetInstance()->m_kMsgServer->IsFriend( dwUID ) == true )
		UpdateFriend( dwUID );
}

void KGCMsnDlg::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCMsnDlg::Clear()
{	
	m_setBlockList.clear();
	m_vecChat.clear();
}