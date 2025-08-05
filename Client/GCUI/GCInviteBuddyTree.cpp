#include "stdafx.h"
#include ".\GCInviteBuddyTree.h"
#include "GCInviteBuddyChild.h"
//

#include "BuddySystem/KFriendShip.h"
#include "../GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( GCInviteBuddyTree );
IMPLEMENT_WND_FACTORY( GCInviteBuddyTree );
IMPLEMENT_WND_FACTORY_NAME( GCInviteBuddyTree, "gc_invite_buddy_tree" );

GCInviteBuddyTree::GCInviteBuddyTree(void)
: m_iTopIndex(0)
, m_pkScroll(NULL)

{
	for( int i = 0; i < MAX_INVITE_CHILD_NUM; ++i )
	{
		m_pkChild[i] = NULL;
	}

	char strWndName[MAX_PATH];
	for( int i = 0; i < MAX_INVITE_CHILD_NUM; ++i )
	{
		sprintf( strWndName, "child%d", i+1 );
		LINK_CONTROL( strWndName, m_pkChild[i] );
	}

	LINK_CONTROL( "scroll", m_pkScroll );
}

GCInviteBuddyTree::~GCInviteBuddyTree(void)
{

}

void GCInviteBuddyTree::OnCreate()
{
	for( int i = 0; i < MAX_CHILD_NUM; ++i )
	{
		m_pkChild[i]->InitState( false, true, this );
	}

	m_pkScroll->InitState( true, false, this );
	m_pkScroll->SetScrollRangeMax( 0 );
}

void GCInviteBuddyTree::OnCreateComplete()
{
	SetMode( 0, GCNateOn::FRIEND_MODE );
	UpdateData();
}

void GCInviteBuddyTree::ActionPerformed( const KActionEvent& event )
{
	for( int i = 0; i < MAX_INVITE_CHILD_NUM; ++i )
	{
		GCWND_WND_MAP( m_pkChild[i], KD3DWnd::D3DWE_BUTTON_CLICK, OnChild );
		//GCWND_WND_MAP( m_pkChild[i], KD3DWnd::D3DWE_RBUTTON_CLICK, OnChildRClick );

		if( m_pkChild[i] == event.m_pWnd )
		{
			if( GCInviteBuddyChild::IBC_BUDDY_CHILD_CHECK == event.m_dwCode )
			{
				m_mapClickList.erase( static_cast<DWORD>( event.m_dwlParam ));
				if( 0 != event.m_dwlParam2 )
				{
					m_mapClickList.insert( std::map< DWORD, bool >::value_type( static_cast<DWORD>( event.m_dwlParam ), true ) );
				}
			}
		}
	}

	if( event.m_pWnd == m_pkScroll )
	{
		/*if( event.m_dwCode == D3DWE_SCROLLBAR_WHEELUP ||
			event.m_dwCode == D3DWE_SCROLLBAR_WHEELDOWN ||
			event.m_dwCode == D3DWE_SCROLLBAR_DRAG ||
			event.m_dwCode == D3DWE_SCROLLBAR_UPBTN ||
			event.m_dwCode == D3DWE_SCROLLBAR_DOWNBTN
			)
		{*/
			OnScroll( event.m_dwCode );
		//}
	}
}

void GCInviteBuddyTree::UpdateData()
{
	std::vector< FRIEND_UI_DATA > vecUIData;
	SiKFriendShip()->GetFriendShipData( m_eMode, m_iTopIndex, vecUIData, MAX_INVITE_CHILD_NUM, false );

	for( int i = 0; i < MAX_INVITE_CHILD_NUM; ++i )
	{
		m_pkChild[i]->ToggleRender( false );
	}
	for( int i = 0; i < (int)vecUIData.size(); ++i )
	{
		m_pkChild[i]->SetData( vecUIData[i] );
		m_pkChild[i]->ToggleRender( true );
	}

	m_pkScroll->SetScrollPageSize( vecUIData.size() );
	int iRangeMax = SiKFriendShip()->GetDataCount( m_eMode );
	if( false == SiKGCCoupleSystem()->Am_I_Solo() )
		--iRangeMax;
	m_pkScroll->SetScrollRangeMax( iRangeMax );
	m_pkScroll->SetScrollPos( m_iTopIndex );
}

void GCInviteBuddyTree::SetMode( int iIdx, GCNateOn::E_NATEON_MODE eMode )
{
	m_iTopIndex = iIdx;
	m_eMode = eMode;
	m_pkScroll->SetScrollPos( m_iTopIndex );
}

void GCInviteBuddyTree::OnChild( KD3DWnd* pkWnd )
{
	GCInviteBuddyChild* pkChild = static_cast<GCInviteBuddyChild*>( pkWnd );
	FRIEND_UI_DATA kData;
	pkChild->GetData( kData );
	if( true == kData.m_bGroup )
	{
		SiKFriendShip()->ToggleGroupOpen( kData.m_nGroupID );
		UpdateData();
	}
}

void GCInviteBuddyTree::OnScroll( DWORD dwEventCode )
{
	//GCNateOn* pkParent = static_cast<GCNateOn*>( this->GetParent() );
	//if( pkParent->IsPopupOn() )
	//{
	//m_pkScroll->SetScrollPos( m_iTopIndex );
	//	return;
	//}

	int iTop = m_pkScroll->GetScrollPos();
	if( m_iTopIndex != iTop )
	{
		g_KDSound.Play( "73" );
		m_iTopIndex = iTop;
		UpdateData();
	}
}

void GCInviteBuddyTree::OnChildRClick( KD3DWnd* pkWnd )
{
	GCInviteBuddyChild* pkChild = static_cast<GCInviteBuddyChild*>( pkWnd );
	FRIEND_UI_DATA kData;
	pkChild->GetData( kData );
	GCNateOn* pkParent = static_cast<GCNateOn*>( this->GetParent() );
	pkParent->OnTreeRButtonClick( kData );
}

void GCInviteBuddyTree::GetInviteList( std::set<DWORD>& setInviteList )
{
	std::map< DWORD, bool >::iterator itBT;

	for( itBT = m_mapClickList.begin();itBT != m_mapClickList.end();++itBT)
	{
		if( itBT->second )
		{
			setInviteList.insert( itBT->first );
		}
	}
}