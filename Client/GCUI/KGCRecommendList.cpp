#include "stdafx.h"
#include "GCReplayBox.h"
#include "../MyD3D.h"
#include "KGCCheckuser_ListElement.h"
#include "../Replay.h"

IMPLEMENT_CLASSNAME( KGCRecommendList );
IMPLEMENT_WND_FACTORY( KGCRecommendList );
IMPLEMENT_WND_FACTORY_NAME( KGCRecommendList, "gc_recommend_list" );

KGCRecommendList::KGCRecommendList( void )
{
    m_iTopIndex     = 0;
    m_bNew          = false;
    for( int i = 0 ; i < (int)m_pkUserList.size() ; ++i )
    {
        m_pkUserList[i] = NULL;
    }

    m_pkScrollbar   = NULL;
    m_pkExit		= NULL; 
    m_pkStaticTitle = NULL;
    m_pkBtnAccept   = NULL;;
    m_pkBtnReject   = NULL;;

    LINK_CONTROL( "scroll",         m_pkScrollbar );
    LINK_CONTROL( "exit",           m_pkExit );
    LINK_CONTROL( "static_title",   m_pkStaticTitle);

    char str[MAX_PATH];
    for(int i=0; i < MAX_LIST_NUM; i++)
    {
        m_pkUserList[i] = NULL;
        sprintf( str, "checkuser_listitem%d", i );
        LINK_CONTROL( str, m_pkUserList[i] );
    }

    LINK_CONTROL( "btn_accept",   m_pkBtnAccept );
    LINK_CONTROL( "btn_reject",   m_pkBtnReject );
    
}

KGCRecommendList::~KGCRecommendList( void )
{
    // empty
}

void KGCRecommendList::OnCreate( void )
{
    m_bNew = false;
    m_pkScrollbar->SetSelfInputCheck( false );
    m_pkScrollbar->AddActionListener( this );
    m_pkScrollbar->SetScrollPageSize( MAX_LIST_NUM );
    m_pkScrollbar->SetScrollPos( m_iTopIndex );

    m_pkExit->SetSelfInputCheck( true );
    m_pkExit->AddActionListener( this );
    m_pkExit->SetHotKey( DIK_ESCAPE );
    m_pkExit->SetHotKey( DIK_RETURN );

    for ( int i = 0; i < MAX_LIST_NUM; ++i )
    {
        m_pkUserList[i]->InitState( false, true, this );
    }
    m_pkBtnAccept->InitState( true, true, this );
    m_pkBtnReject->InitState( true, true, this );

    m_pkStaticTitle->SetText( g_pkStrLoader->GetString(STR_ID_RECOMMEND_LIST_WINDOW));

    RequestList();
}

void KGCRecommendList::OnCreateComplete( void )
{
    SpeakToActionListener( KActionEvent( this, IRR_CREATED ) );
}

void KGCRecommendList::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, IRR_DESTROYED ) );
}

void KGCRecommendList::ActionPerformed( const KActionEvent& event )
{


    for ( int i = 0; i < MAX_LIST_NUM; i++ )
    {
        if( m_pkUserList[i] == event.m_pWnd )
        {
            if( event.m_dwlParam == 100 )
                m_setCheckedID.insert( m_pkUserList[i]->GetUserUID() );
            else if( event.m_dwlParam == 200 )
                m_setCheckedID.erase( m_pkUserList[i]->GetUserUID() );
        }
    }

    // 스크롤바를 조작했다면 리스트를 갱신한다.
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_pkScrollbar->GetScrollPos();
        if ( m_iTopIndex != iTop )
        {
            g_KDSound.Play( "73" );
            m_iTopIndex = iTop;
            UpdateList();
        }
        return;
    }
    else if ( event.m_pWnd == m_pkExit )  //닫기 버튼에서 이벤트가 발생 했다면
    {
        switch ( event.m_dwCode )
        {
        case KD3DWnd::D3DWE_BUTTON_DOWN:
            {
                g_KDSound.Play( "31" );
                return;
            }
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            {
                SpeakToActionListener( KActionEvent( this, IRR_CLOSE ) );
                return;
            }
        }
    }
    else if( event.m_pWnd == m_pkBtnAccept )
    {
        switch ( event.m_dwCode )
        {
        case KD3DWnd::D3DWE_BUTTON_DOWN:
            {
                g_KDSound.Play( "31" );
                return;
            }
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            {
                if(m_setCheckedID.empty() )
                    return;

                std::vector<DWORD> vecAccept;
                for( std::set<DWORD>::iterator it = m_setCheckedID.begin(); it != m_setCheckedID.end(); it++ )
                {
                    vecAccept.push_back( *it );
                }
                
                int iMyState = SiKFriendShip()->GetMyRecommendState();
                iMyState = ( iMyState == 1)? 1: 0;

                if( static_cast<int>( m_setCheckedID.size() + SiKFriendShip()->m_vecContactFriendList.size()) <= RECOMMENDER_MAX_NUM + iMyState )   // 다 수락했을때 12명 넘으면
                {
                    KP2P::GetInstance()->Send_AcceptRecommenderReq( vecAccept );

                    SpeakToActionListener( KActionEvent( this, IRR_CLOSE ) );
                }
                else
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_ACCEPT_BECAUSE_TOO_MANY ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                }
                return;
            }
        }
        
    }
    else if( event.m_pWnd == m_pkBtnReject )
    {
        switch ( event.m_dwCode )
        {
        case KD3DWnd::D3DWE_BUTTON_DOWN:
            {
                g_KDSound.Play( "31" );
                return;
            }
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            {
                if(m_setCheckedID.empty() )
                    return;

                std::vector<DWORD> vecDeny;
                for( std::set<DWORD>::iterator it = m_setCheckedID.begin(); it != m_setCheckedID.end(); it++ )
                {
                    vecDeny.push_back( *it );
                }

                KP2P::GetInstance()->Send_RecommenderDenyReq( vecDeny );

                SpeakToActionListener( KActionEvent( this, IRR_CLOSE ) );

				if( SiKFriendShip()->m_vecWaitRecommenders.size() <= 1 )
				{
					g_pkUIScene->m_pkNateOn->SetNew( false );
					SetNew( false );
				}

                return;
            }
        }
    }
}

void KGCRecommendList::RequestList( void )
{
    m_setCheckedID.clear();

    m_pkScrollbar->SetScrollRangeMax( SiKFriendShip()->m_vecWaitRecommenders.size()); 
    m_pkScrollbar->SetScrollPos(0);

    UpdateList();	
}

void KGCRecommendList::UpdateList( void )
{
	m_pkScrollbar->SetScrollRangeMax( SiKFriendShip()->m_vecWaitRecommenders.size());

	int iRecommendersSize = 0;

	if( SiKFriendShip()->m_vecWaitRecommenders.size() < MAX_LIST_NUM )
		iRecommendersSize = SiKFriendShip()->m_vecWaitRecommenders.size();
	else
		iRecommendersSize = MAX_LIST_NUM;

	if( m_iTopIndex  > static_cast<int>( SiKFriendShip()->m_vecWaitRecommenders.size() ) - iRecommendersSize)
	{
		m_iTopIndex = static_cast<int>( SiKFriendShip()->m_vecWaitRecommenders.size() ) - iRecommendersSize;
		m_pkScrollbar->SetScrollPos(m_iTopIndex);
	}

	int vecIndex = m_iTopIndex;

	for ( int i = 0; i < iRecommendersSize; i++,vecIndex++ )
	{
		KRecommenderInfo recom = SiKFriendShip()->m_vecWaitRecommenders[ vecIndex ];
		//등급마크를 그릴려면 등급정보를 받아와야지 이전처럼 0만 집어넣고 있음 안되지...
		m_pkUserList[i]->SetUserInfo( recom.m_strNick, -1, recom.m_dwLoginUID );
		m_pkUserList[i]->ToggleRender( true );                
		m_pkUserList[i]->SetCheck( m_setCheckedID.find( recom.m_dwLoginUID ) != m_setCheckedID.end() );
	}
}

bool KGCRecommendList::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    this->RequestList();
    return true;
}