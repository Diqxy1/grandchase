#include "stdafx.h"
#include "GCUserList.h"
#include "GCUserBar.h"
//
//
//
//


#include "../Procedure.h"
#include "GCUI/GCAddFriendMenu.h"
//
#include "KGCChannelManager.h"

IMPLEMENT_CLASSNAME( KGCUserList );
IMPLEMENT_WND_FACTORY( KGCUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCUserList, "gc_user_list" );

KGCUserList::KGCUserList( void )
{
    m_iTopIndex      = 0;
    m_bToggleButton  = false;
    m_bShowAll       = true;
    m_bGuildChannel  = false;
    m_pkScrollbar    = NULL;

    ::ZeroMemory( m_apkUserBar, sizeof( m_apkUserBar ) );

    LINK_CONTROL( "scroll",                     m_pkScrollbar );

	for( int i=0 ; i<NUM_USER_LIST ; ++i )
	{
		char strName[MAX_PATH];
		sprintf( strName, "user_bar%d", i );

		LINK_CONTROL( strName,  m_apkUserBar[i] );
	}
}

KGCUserList::~KGCUserList( void )
{
    // empty
}

void KGCUserList::OnCreate( void )
{
    for ( int i = 0; i < NUM_USER_LIST; ++i )
    {        
        m_apkUserBar[i]->SetGuildUserBar( false );
        m_apkUserBar[i]->ToggleRender( false );
    }

    m_pkScrollbar->SetSelfInputCheck( false );
    m_pkScrollbar->AddActionListener( this );

    m_pkScrollbar->SetScrollPageSize( NUM_USER_LIST );
    m_pkScrollbar->SetScrollPos( m_iTopIndex );
}

void KGCUserList::OnActivateComplete( void )
{
    // 리스트를 초기화한다.
    ToggleButton( true );
    UpdateUserList();
}

void KGCUserList::FrameMoveInEnabledState( void )
{
    m_bToggleButton = false;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
	bool bCheckBound = CheckPosInWindowBound( vMousePos );

	if( !bCheckBound )
	{
		if( g_pkAddFriendMenu->IsRenderOn() )
            g_pkAddFriendMenu->ToggleRender( false );
	}

    // 채널 타입(일반/길드)이 갱신되었는지 확인한다.
    if ( true == m_bGuildChannel && false == SiKGCChannelManager()->CheckChannel( CT_GUILD ) )
    {
        // 일반 채널에서는 모두/길드 토글 버튼 기본 상태로 고정시킨다.
        m_bGuildChannel = SiKGCChannelManager()->CheckChannel( CT_GUILD );
        ToggleButton( true );
        UpdateUserList();
    }
    else if ( false == m_bGuildChannel && true == SiKGCChannelManager()->CheckChannel( CT_GUILD ) )
    {
        // 길드 채널에서는 모두/길드 토글 버튼을 '길드 보기'로 초기화한다.
        m_bGuildChannel = SiKGCChannelManager()->CheckChannel( CT_GUILD ) ;
        ToggleButton( false );
        UpdateUserList();
    }

    m_bGuildChannel = SiKGCChannelManager()->CheckChannel( CT_GUILD );

    // 새로운 유저 리스트를 받았다면 갱신한다.
    if ( g_bUserListUpdate )
    {
        UpdateUserList();
        g_bUserListUpdate = false;
    }
}

void KGCUserList::ActionPerformed( const KActionEvent& event )
{
    // 스크롤바를 조작했다면 리스트를 갱신한다.
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_pkScrollbar->GetScrollPos();
        if ( m_iTopIndex != iTop )
        {
            g_KDSound.Play( "73" );
            m_iTopIndex = iTop;
            UpdateUserList();
        }
    }
}

void KGCUserList::ToggleButton( bool bShowAll )
{
    m_bToggleButton = true;
    m_bShowAll      = bShowAll;
}

void KGCUserList::UpdateUserList( void )
{
    if ( m_bGuildChannel ) // 길드 채널일 경우
    {
        std::vector<KUserInfo> vecUserInfo;

        if ( m_bShowAll ) // 전체 보기일 경우
        {
            // 길드 채널 유저 리스트를 그대로 복사한다.
            vecUserInfo = g_vecUserInfo;
        }
        else // 길드 멤버 보기일 경우
        {
            // 길드 마크가 동일한 것들을 필터링한다.
            for ( int i = 0; i < (int)g_vecUserInfo.size(); ++i )
            {
                if ( g_kGlobalValue.m_kUserInfo.aiGuildMark[0] == ((BYTE*)( &g_vecUserInfo[i].m_iGuildMark ))[0] &&
                     g_kGlobalValue.m_kUserInfo.aiGuildMark[1] == ((BYTE*)( &g_vecUserInfo[i].m_iGuildMark ))[1] &&
                     g_kGlobalValue.m_kUserInfo.aiGuildMark[2] == ((BYTE*)( &g_vecUserInfo[i].m_iGuildMark ))[2] )
                {
                    vecUserInfo.push_back( g_vecUserInfo[i] );
                }
            }
        }

        // 스크롤바의 최대치를 조정한다.
        m_pkScrollbar->SetScrollRangeMax( (int)vecUserInfo.size() );

        // 디스플레이될 유저 바에 길드 채널 유저 리스트 정보를 입력한다.
        for ( int i = 0; i < NUM_USER_LIST; ++i )
        {
            if ( m_iTopIndex + i < (int)vecUserInfo.size() ) // 표시 범위 안
            {
                KUserInfo& kInfo = vecUserInfo[m_iTopIndex + i];

                m_apkUserBar[i]->ToggleRender( true );
                m_apkUserBar[i]->SetGuildChannel( true );
                m_apkUserBar[i]->SetMark( kInfo.m_iGuildMark );
                m_apkUserBar[i]->SetUserID( kInfo.m_strLogin );
                m_apkUserBar[i]->SetUserNickName( kInfo.m_strNickName );
                m_apkUserBar[i]->SetUserStatus( kInfo.m_cState, kInfo.m_usRoomID );
            }
            else // 표시 범위 밖
            {
                m_apkUserBar[i]->ToggleRender( false );
            }
        }
    }
    else // 일반 채널일 경우
    {
        // 스크롤바의 최대치를 조정한다.
        m_pkScrollbar->SetScrollRangeMax( (int)g_vecUserInfo.size() );

        // 디스플레이될 유저 바에 일반 유저 리스트 정보를 입력한다.
        for ( int i = 0; i < NUM_USER_LIST; ++i )
        {
            if ( m_iTopIndex + i < (int)g_vecUserInfo.size() ) // 표시 범위 안
            {
                m_apkUserBar[i]->ToggleRender( true );
                m_apkUserBar[i]->SetGuildChannel( false );

				// 2007.02.08 wony
				// 계급표시
				{
					m_apkUserBar[i]->SetUserGrade( g_vecUserInfo[m_iTopIndex + i].m_cKorGrade );
				}

                KUserInfo& kInfo = g_vecUserInfo[m_iTopIndex + i];
                m_apkUserBar[i]->SetUserID( kInfo.m_strLogin );
                m_apkUserBar[i]->SetUserNickName( kInfo.m_strNickName );
                m_apkUserBar[i]->SetUserStatus( kInfo.m_cState, kInfo.m_usRoomID );
            }
            else // 표시 범위 밖
            {
                m_apkUserBar[i]->ToggleRender( false );
            }
        }
    }
}
