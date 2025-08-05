#include "stdafx.h"
//
#include ".\GCInviteUserList.h"
//
//
#include "GCInviteUserInfo.h"

//
#include "GCInviteRequestBox.h"

IMPLEMENT_CLASSNAME( KGCInviteUserList );
IMPLEMENT_WND_FACTORY( KGCInviteUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteUserList, "gc_invite_user_list" );

KGCInviteUserList::KGCInviteUserList(void)
: m_pkBack(NULL)
, m_pkTitle(NULL)
, m_pkPrevBtn(NULL)
, m_pkNextBtn(NULL)
, m_pkInviteBtn(NULL)
{
    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        m_pkInviteUserInfo[i] = NULL;
    }

    LINK_CONTROL( "back",       m_pkBack );
    LINK_CONTROL( "title",      m_pkTitle );
    LINK_CONTROL( "btn_prev",   m_pkPrevBtn );
    LINK_CONTROL( "btn_next",   m_pkNextBtn );
    LINK_CONTROL( "btn_invite", m_pkInviteBtn );

    char strName[MAX_PATH];
    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        sprintf( strName, "user_info%d", i+1 );
        LINK_CONTROL( strName, m_pkInviteUserInfo[i] );
    }
}

KGCInviteUserList::~KGCInviteUserList(void)
{

}

void KGCInviteUserList::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkPrevBtn, D3DWE_BUTTON_CLICK, OnPrevBtn );
    GCWND_MSG_MAP( m_pkNextBtn, D3DWE_BUTTON_CLICK, OnNextBtn );
    GCWND_MSG_MAP( m_pkInviteBtn, D3DWE_BUTTON_CLICK, OnInviteBtn );

    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        GCWND_WND_CODE_MAP( m_pkInviteUserInfo[i], OnUserList );
    }
}

void KGCInviteUserList::OnCreate( void )
{
    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        m_pkInviteUserInfo[i]->InitState( true, true, this );
    }

    m_pkBack->InitState( true, true, this );
    m_pkTitle->InitState( true, true, this );
    m_pkTitle->SetAlign( DT_CENTER );
    m_pkPrevBtn->InitState( true, true, this );
    m_pkNextBtn->InitState( true, true, this );
    m_pkInviteBtn->InitState( true, true, this );
}

void KGCInviteUserList::OnPrevBtn()
{
    int nCurPage = SiKGCInviteManager()->GetCurrentPage();
    SiKGCInviteManager()->SetCurrentPage( SiKGCInviteManager()->GetCurrentPage() - 1 );
    if( nCurPage !=  SiKGCInviteManager()->GetCurrentPage() )
    {
		if (SiKGCInviteManager()->GetInviteType()==KGCInviteManager::INVITE_TYPE_FRIENDS)
		{
			SetFriendUserList();
		}
        else
		{
			SiKGCInviteManager()->UpdateUserList();
		}
    }
}

void KGCInviteUserList::OnNextBtn()
{
    int nCurPage = SiKGCInviteManager()->GetCurrentPage();
    SiKGCInviteManager()->SetCurrentPage( SiKGCInviteManager()->GetCurrentPage() + 1 );
    if( nCurPage !=  SiKGCInviteManager()->GetCurrentPage() )
    {
		if (SiKGCInviteManager()->GetInviteType()==KGCInviteManager::INVITE_TYPE_FRIENDS)
		{
			SetFriendUserList();
		}
		else
		{
			SiKGCInviteManager()->UpdateUserList();
		}
    }
}

void KGCInviteUserList::OnInviteBtn()
{
    std::vector<DWORD> vecInvite;

    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        if( true == m_pkInviteUserInfo[i]->GetCheck() )
        {
            vecInvite.push_back( m_pkInviteUserInfo[i]->GetUID() );
        }
    }

    if( vecInvite.empty() )
        return;

	//길드전 방일 때는 길드전용 초대패킷을 보낸다.
	if( SiKGCRoomManager()->IsChannelRoomList() )
		KP2P::GetInstance()->Send_InviteGuildUserReq( vecInvite );
	else
		KP2P::GetInstance()->Send_InviteReq( vecInvite );

    if (g_pkUIScene->m_pkInviteRequestBox)
        g_pkUIScene->m_pkInviteRequestBox->OnClose();
}

void KGCInviteUserList::OnUserList( KD3DWnd* pWnd, DWORD dwCode )
{
    KGCInviteUserInfo* pEventWnd = static_cast<KGCInviteUserInfo*>( pWnd );
    if( pEventWnd->IsLock() )
        return;

    switch( dwCode )
    {
    case D3DWE_CURSOR_ENTER:
        if( pEventWnd->GetBackMode() != D3DWM_SELECT )
        {
            pEventWnd->SetBackMode( D3DWM_HOVER );
        }
        break;
    case D3DWE_CURSOR_LEAVE:
        if( pEventWnd->GetBackMode() != D3DWM_SELECT )
        {
            pEventWnd->SetBackMode( D3DWM_DEFAULT );
        }
        break;
    case D3DWE_BUTTON_CLICK:
        // [2/22/2008] breadceo. SetCheck
        pEventWnd->SetCheck( !pEventWnd->GetCheck() );
        pEventWnd->SetBackMode( pEventWnd->GetCheck() ? D3DWM_SELECT : D3DWM_DEFAULT );
        break;
    }
}

void KGCInviteUserList::InitUserList()
{
    for( int i = 0; i < INVITE_USER_NUM; ++i )
    {
        m_pkInviteUserInfo[i]->InitUserInfo();
    }
}

void KGCInviteUserList::SetTitle( const std::wstring& strTitle )
{
    m_pkTitle->SetText( strTitle );
}

void KGCInviteUserList::SetFriendUserList()
{
	InitUserList();
	const std::vector<KInviteFriendUserInfo>& vecUserList = SiKGCInviteManager()->GetInviteFriendUserList();
	ASSERT( (int)vecUserList.size() <= INVITE_USER_NUM );
	int iPage = SiKGCInviteManager()->GetCurrentPage();
	
	for( int i = iPage * INVITE_USER_NUM; i < (iPage + 1) * INVITE_USER_NUM; ++i )
	{
		if (i >= (int)vecUserList.size()) {
			break;
		}
		m_pkInviteUserInfo[i % INVITE_USER_NUM]->SetFriendUserInfo( vecUserList[i] );
	}	
}
void KGCInviteUserList::SetUserList()
{
    InitUserList();
    const std::vector<KInviteUserInfo>& vecUserList = SiKGCInviteManager()->GetInviteUserList();
    ASSERT( (int)vecUserList.size() <= INVITE_USER_NUM );
    for( int i = 0; i < (int)vecUserList.size(); ++i )
    {
        m_pkInviteUserInfo[i]->SetUserInfo( vecUserList[i] );
    }
}
