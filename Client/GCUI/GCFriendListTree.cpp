#include "stdafx.h"
#include "GCFriendListTree.h"

IMPLEMENT_CLASSNAME( KGCFriendListTree );
IMPLEMENT_WND_FACTORY( KGCFriendListTree );
IMPLEMENT_WND_FACTORY_NAME( KGCFriendListTree, "gc_friend_list_tree" );

KGCFriendListTree::KGCFriendListTree( void )
{
	char szTemp[128];
	for( int i = 0; i < MAX_FRIEND_LIST_CHILD; ++i )
	{
		m_pkChild[i] = NULL;
		sprintf(szTemp, "friend_child%d", i);
		LINK_CONTROL( szTemp, m_pkChild[i] );
	}

	m_pkScrollbar = NULL;
	LINK_CONTROL( "scroll",				m_pkScrollbar);

	m_iMaxScrollSize = MAX_FRIEND_LIST_CHILD;
	m_iScrollPos = 0;
	m_iSelectedIndex = -1;
	m_iSelectedUserUID = -1;
}

KGCFriendListTree::~KGCFriendListTree( void )
{
}

void KGCFriendListTree::OnCreate( void )
{
	for( int i = 0; i < MAX_FRIEND_LIST_CHILD; ++i )
	{
		m_pkChild[i]->InitState( true, true, this );
	}

	m_pkScrollbar->InitState( true, true, this );
	m_pkScrollbar->SetScrollPos( m_iScrollPos );
	m_pkScrollbar->SetScrollRangeMin( 0 );
	m_pkScrollbar->SetScrollGap( 1 );
	m_pkScrollbar->SetScrollPageSize( MAX_FRIEND_LIST_CHILD );
	m_pkScrollbar->SetScrollRangeMax( m_iMaxScrollSize );

	m_iSelectedIndex = -1;
	m_iSelectedUserUID = -1;
}

void KGCFriendListTree::OnCreateComplete( void )
{
	m_iScrollPos = 0;
	m_pkScrollbar->SetScrollPos( m_iScrollPos );
	m_iMaxScrollSize = MAX_FRIEND_LIST_CHILD;
	m_pkScrollbar->SetScrollRangeMax( m_iMaxScrollSize );
	UpdateData();
}

void KGCFriendListTree::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK || event.m_dwCode == KD3DWnd::D3DWE_DOUBLE_CLICK )
	{
		for( int i = 0; i < MAX_FRIEND_LIST_CHILD; ++i )
		{
			if( event.m_pWnd == m_pkChild[i] && m_pkChild[i]->IsRenderOn() )
			{
				if( m_pkChild[i]->IsGroupTitle() )
				{
					m_pkChild[i]->ToggleFolder();
					SiKFriendShip()->ToggleGroupOpen( m_pkChild[i]->GetGroupId() );
					UpdateData();
				}
				else
				{
					SetSelection( i );
					if( event.m_dwCode == KD3DWnd::D3DWE_DOUBLE_CLICK )
						g_pkUIScene->m_pkPresentFriendList->OnClickOkBtn();
				}
				
			}
		}
	}

	if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER || event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
	{
		for( int i = 0; i < MAX_FRIEND_LIST_CHILD; ++i )
		{
			if( event.m_pWnd == m_pkChild[i] && m_pkChild[i]->IsRenderOn() )
			{
				if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
					m_pkChild[i]->OnCursor( true );
				else
					m_pkChild[i]->OnCursor( false );

			}
		}
	}

	if ( event.m_pWnd == m_pkScrollbar )
	{
		int iPos = m_pkScrollbar->GetScrollPos();

		if ( m_iScrollPos != iPos )
		{
			g_KDSound.Play( "73" );
			m_iScrollPos = iPos;
			UpdateDisplayData( m_iScrollPos );
		}
	}
}

void KGCFriendListTree::UpdateData( void )
{
	m_vecFriendListData.clear();
	SiKFriendShip()->GetFriendListData( m_vecFriendListData );
	SetMaxScrollSize( m_vecFriendListData.size() );
	UpdateDisplayData();
	RefreshSelection();
}

void KGCFriendListTree::UpdateDisplayData( int iScrollPos/*=-1*/ )
{
	if( iScrollPos == -1 )
		iScrollPos = m_pkScrollbar->GetScrollPos();

	for( int i = 0; i < MAX_FRIEND_LIST_CHILD; ++i )
	{
		m_pkChild[i]->ToggleRender( false );
	}

	for( int i = 0; i < MAX_FRIEND_LIST_CHILD && i+iScrollPos < (int)m_vecFriendListData.size(); ++i )
	{
		m_pkChild[i]->ToggleRender( true );
		m_pkChild[i]->SetData( m_vecFriendListData[i+iScrollPos] );
	}
}

void KGCFriendListTree::SetSelection( int iSelectedChildIndex )
{
	int iNewSelectedIndex = iSelectedChildIndex + m_pkScrollbar->GetScrollPos();

	if( m_iSelectedIndex >= 0 ) // 기존 선택 해제
	{
		m_vecFriendListData[m_iSelectedIndex].m_bSelected = false;
		int iChildIndex = m_iSelectedIndex - m_pkScrollbar->GetScrollPos();
		if( iChildIndex >= 0 && iChildIndex < MAX_FRIEND_LIST_CHILD )
			m_pkChild[iChildIndex]->SetSelection( false );
	}

	//새로 선택
	m_vecFriendListData[iNewSelectedIndex].m_bSelected = true;
	if( iSelectedChildIndex >= 0 && iSelectedChildIndex < MAX_FRIEND_LIST_CHILD )
		m_pkChild[iSelectedChildIndex]->SetSelection( true );

	m_iSelectedIndex = iNewSelectedIndex;
	m_iSelectedUserUID = m_vecFriendListData[iNewSelectedIndex].m_dwUserUID;
}

void KGCFriendListTree::RefreshSelection( void )
{
	if( m_iSelectedIndex < 0 || m_iSelectedUserUID < 0 )
	{
		m_iSelectedIndex = -1;
		m_iSelectedUserUID = -1;
		return;
	}
	
	for( int i = 0; i < (int)m_vecFriendListData.size() ; ++i )
	{
		if( m_vecFriendListData[i].m_dwUserUID == m_iSelectedUserUID )
		{
			m_iSelectedIndex = i;
			SetSelection( m_iSelectedIndex - m_pkScrollbar->GetScrollPos() );
			return;
		}
	}
}

void KGCFriendListTree::SetMaxScrollSize( int iMaxScrollSize )
{
	if( m_iMaxScrollSize < iMaxScrollSize )
		m_iMaxScrollSize = iMaxScrollSize;
	m_pkScrollbar->SetScrollRangeMax( m_iMaxScrollSize );
}

std::wstring KGCFriendListTree::GetSelectedNickname( void )
{
	if( m_iSelectedIndex < 0 )
		return L"";

	return m_vecFriendListData[m_iSelectedIndex].m_strNickname;
}