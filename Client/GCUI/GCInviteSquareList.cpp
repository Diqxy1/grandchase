#include "stdafx.h"
#include ".\GCInviteSquareList.h"
//
//
#include "../Square/GCSquare.h"

#include "GCInviteSquareInfo.h"
//
#include "GCInviteRequestBox.h"
//

IMPLEMENT_CLASSNAME( KGCInviteSquareList );
IMPLEMENT_WND_FACTORY( KGCInviteSquareList );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteSquareList, "gc_invite_square_list" );

KGCInviteSquareList::KGCInviteSquareList(void)
: m_pkBack(NULL)
, m_pkTitle(NULL)
{
    for( int i = 0; i < MAX_INVITE_SQUARE_NUM; ++i )
    {
        m_pkSquareInfo[i] = NULL;
    }

    LINK_CONTROL( "back",           m_pkBack );
    LINK_CONTROL( "title",          m_pkTitle );

    char strName[MAX_PATH];
    for( int i = 0; i < MAX_INVITE_SQUARE_NUM; ++i )
    {
        sprintf( strName, "square_info%d", i+1 );
        LINK_CONTROL( strName, m_pkSquareInfo[i] );
    }
}

KGCInviteSquareList::~KGCInviteSquareList(void)
{

}

void KGCInviteSquareList::ActionPerformed( const KActionEvent& event )
{
    for( int i = 0; i < MAX_INVITE_SQUARE_NUM; ++i )
    {
        GCWND_WND_CODE_MAP( m_pkSquareInfo[i], OnSquareInfo );
    }
}

void KGCInviteSquareList::OnCreate( void )
{
    m_pkBack->InitState( true );
    m_pkTitle->InitState( true );
    m_pkTitle->SetAlign( DT_CENTER );
    m_pkTitle->SetText( L"" );

    for( int i = 0; i < MAX_INVITE_SQUARE_NUM; ++i )
    {
        m_pkSquareInfo[i]->InitState( true, true, this );
    }
}

void KGCInviteSquareList::SetTitle( const std::wstring& strTitle )
{
    m_pkTitle->SetText( strTitle );
}

void KGCInviteSquareList::SetSquareList()
{
    InitSquareList();
    std::vector< KSquareInfo >& vecInfo = SiGCSquare()->GetSquareInfo();
    ASSERT( (int)vecInfo.size() <= MAX_INVITE_SQUARE_NUM );
    for( int i = 0; i < (int)vecInfo.size(); ++i )
    {
        m_pkSquareInfo[i]->SetSquareInfo( vecInfo[i] );
    }
}

void KGCInviteSquareList::OnSquareInfo( KD3DWnd* pWnd, DWORD dwCode )
{
    KGCInviteSquareInfo* pEventWnd = static_cast<KGCInviteSquareInfo*>( pWnd );
    if( pEventWnd->IsEmpty() )
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
        SetSquareList();
        pEventWnd->SetBackMode( D3DWM_SELECT );
        pEventWnd->SetArrow( true );

        // [2/22/2008] breadceo. UserList 갱신
        SiKGCInviteManager()->UpdateSquareUserList( pEventWnd->GetSquareID(), pEventWnd->GetSquareName() );
        if (g_pkUIScene->m_pkInviteRequestBox)
            g_pkUIScene->m_pkInviteRequestBox->SetUserListTitle( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_USER_LIST ) );
        break;
    }
}

void KGCInviteSquareList::OnCreateComplete()
{
}

void KGCInviteSquareList::InitSquareList()
{
    for( int i = 0; i < MAX_INVITE_SQUARE_NUM; ++i )
    {
        m_pkSquareInfo[i]->InitSquareInfo();
    }
}