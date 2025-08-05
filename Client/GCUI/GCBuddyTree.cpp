#include "stdafx.h"
#include ".\GCBuddyTree.h"
//
#include "GCBuddyChild.h"
#include "BuddySystem/KFriendShip.h"

//
#include "../GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( GCBuddyTree );
IMPLEMENT_WND_FACTORY( GCBuddyTree );
IMPLEMENT_WND_FACTORY_NAME( GCBuddyTree, "gc_buddy_tree" );

GCBuddyTree::GCBuddyTree(void)
: m_iTopIndex(0)
, m_iChildSize( MAX_CHILD_NUM - 1 )
, m_bAlignTop( false )
, m_pkScroll(NULL)
, m_bCheckGuildOfflineMember(false)
, m_bCheckGuildBlockMember(false)
{
    for( int i = 0; i < MAX_CHILD_NUM; ++i )
    {
        m_pkChild[i] = NULL;
		m_pkContactFriendMakr[i] = NULL;
    }

    char strWndName[MAX_PATH];
    for( int i = 0; i < MAX_CHILD_NUM; ++i )
    {
        sprintf( strWndName, "child%d", i+1 );
        LINK_CONTROL( strWndName, m_pkChild[i] );
		
		sprintf( strWndName, "contact_friend_mark%d", i+1 );
		LINK_CONTROL( strWndName, m_pkContactFriendMakr[i] );
    }

    LINK_CONTROL( "scroll", m_pkScroll );
}

GCBuddyTree::~GCBuddyTree(void)
{
}

void GCBuddyTree::OnCreate()
{
    for( int i = 0; i < MAX_CHILD_NUM; ++i )
    {
        m_pkChild[i]->InitState( false, true, this );
        m_pkContactFriendMakr[i]->InitState(false, true, this );
    }
    m_pkScroll->InitState( true, true, this );
    m_pkScroll->SetScrollRangeMax( 0 );
    UpdateData();
}

void GCBuddyTree::ActionPerformed( const KActionEvent& event )
{
    for( int i = 0; i < MAX_CHILD_NUM; ++i )
    {
        GCWND_WND_MAP( m_pkChild[i], KD3DWnd::D3DWE_BUTTON_CLICK, OnChild );
        GCWND_WND_MAP( m_pkChild[i], KD3DWnd::D3DWE_RBUTTON_CLICK, OnChildRClick );
    }
    GCWND_CODE_MAP( m_pkScroll, OnScroll );
}

void GCBuddyTree::UpdateData()
{
    std::vector< FRIEND_UI_DATA > vecUIData;
    SiKFriendShip()->GetFriendShipData( m_eMode, m_iTopIndex, vecUIData, m_iChildSize );

 	//std::sort( vecUIData.begin(), vecUIData.end(), SortByContactFriend() );

    for( int i = 0; i < MAX_CHILD_NUM; ++i )
    {
        m_pkChild[i]->ToggleRender( false );
		m_pkContactFriendMakr[i]->ToggleRender( false );
    }
    for( int i = 0; i < (int)vecUIData.size(); ++i )
    {
		ASSERT( i < MAX_CHILD_NUM );
        m_pkChild[GetChildBegin() + i]->SetData( vecUIData[i] );
        m_pkChild[GetChildBegin() + i]->ToggleRender( true );

		m_pkChild[GetChildBegin() + i]->SetGuildGradeMark( KNGuildUserInfo::GL_NORMAL );
		//길드메신저 등급표시
		if( m_eMode == GCNateOn::GUILD_MODE )
		{	
			KBuddy kGuildBuddyData;
			if( SiKFriendShip()->GetGuildBuddy(vecUIData[i].m_dwUserUID, kGuildBuddyData) )
				m_pkChild[GetChildBegin() + i]->SetGuildGradeMark( kGuildBuddyData.GetData().m_nGuildUserLevel );
			else
				m_pkChild[GetChildBegin() + i]->SetGuildGradeMark( KNGuildUserInfo::GL_NORMAL );

			if( vecUIData[i].m_bOnline == false )
				m_pkChild[GetChildBegin() + i]->ToggleRender(m_bCheckGuildOfflineMember);

			KBuddy kBlockBuddyData;
			if( m_bCheckGuildBlockMember && SiKFriendShip()->GetBlockBuddy(vecUIData[i].m_dwUserUID, kBlockBuddyData) )
				m_pkChild[GetChildBegin() + i]->SetTextColor( D3DCOLOR_RGBA( 255, 0, 0, 255) );
		}

		if( SiKFriendShip()->FindUIDInContactFriendList(vecUIData[i].m_dwUserUID) )
			m_pkContactFriendMakr[GetChildBegin() + i]->ToggleRender( true );
	}

	D3DXVECTOR2 vScrollPos = m_pkScroll->GetFixedWindowLocalPos();
	m_pkScroll->SetWindowPosDirect( D3DXVECTOR2( vScrollPos.x, m_pkChild[GetChildBegin()]->GetFixedWindowLocalPos().y ) );
	m_pkScroll->SetHeight( (DWORD)(m_pkChild[GetChildEnd()-1]->GetFixedWindowLocalPos().y - m_pkChild[GetChildBegin()]->GetFixedWindowLocalPos().y + m_pkChild[GetChildBegin()]->GetHeight() ));

    m_pkScroll->SetScrollPageSize( vecUIData.size() );

	//길드에서 온라인 유저만 보는 경우 스크롤바 범위 재 설정.
    int iRangeMax = SiKFriendShip()->GetDataCount( m_eMode);
	if (m_eMode==GCNateOn::GUILD_MODE)
	{
		iRangeMax = SiKFriendShip()->GetDataUserCount( m_eMode , !m_bCheckGuildOfflineMember);
	}

	if( false == SiKGCCoupleSystem()->Am_I_Solo() )
		--iRangeMax;
    m_pkScroll->SetScrollRangeMax( iRangeMax );
    m_pkScroll->SetScrollPos( m_iTopIndex );
}

void GCBuddyTree::SetMode( int iIdx, GCNateOn::E_NATEON_MODE eMode )
{
    m_iTopIndex = iIdx;
    m_eMode = eMode;
    m_pkScroll->SetScrollPos( m_iTopIndex );
}

void GCBuddyTree::OnChild( KD3DWnd* pkWnd )
{
    GCBuddyChild* pkChild = static_cast<GCBuddyChild*>( pkWnd );
    FRIEND_UI_DATA kData;
    pkChild->GetData( kData );
    if( true == kData.m_bGroup )
    {
        SiKFriendShip()->ToggleGroupOpen( kData.m_nGroupID );
        UpdateData();
    }
}

void GCBuddyTree::OnScroll( DWORD dwEventCode )
{
    GCNateOn* pkParent = static_cast<GCNateOn*>( this->GetParent() );
    if( pkParent->IsPopupOn() )
    {
        m_pkScroll->SetScrollPos( m_iTopIndex );
        return;
    }

    int iTop = m_pkScroll->GetScrollPos();
    if( m_iTopIndex != iTop )
    {
        g_KDSound.Play( "73" );
        m_iTopIndex = iTop;
        UpdateData();
    }
}

void GCBuddyTree::OnChildRClick( KD3DWnd* pkWnd )
{
    GCBuddyChild* pkChild = static_cast<GCBuddyChild*>( pkWnd );
    FRIEND_UI_DATA kData;
    pkChild->GetData( kData );
    GCNateOn* pkParent = static_cast<GCNateOn*>( this->GetParent() );
    pkParent->OnTreeRButtonClick( kData );
}

void GCBuddyTree::SetChildSize( int iSize, bool bAlignTop /*= true*/ )
{
	if( iSize > MAX_CHILD_NUM )
	{
		ASSERT( !"Too Big Friend size" );
		iSize = MAX_CHILD_NUM;
	}

	if( iSize < 1 )
	{
		ASSERT( !"Too Small Friend size" );
		iSize = 0;
	}

	m_bAlignTop = bAlignTop;
	m_iChildSize = iSize;
}

int GCBuddyTree::GetChildBegin()
{
	if( m_bAlignTop )
		return 0;
	else
		return MAX_CHILD_NUM - m_iChildSize;
}

int GCBuddyTree::GetChildEnd()
{
	if( m_bAlignTop )
		return m_iChildSize;
	else
		return MAX_CHILD_NUM;
}