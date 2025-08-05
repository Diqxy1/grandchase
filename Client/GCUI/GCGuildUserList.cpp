#include "stdafx.h"
#include "GCGuildUserList.h"
#include "GCUserBar.h"
#include "GCGuildMemberBox.h"
//
//
//
//

#include "../Procedure.h"
//

IMPLEMENT_CLASSNAME( KGCGuildUserList );
IMPLEMENT_WND_FACTORY( KGCGuildUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildUserList, "gc_guild_user_list" );

KGCGuildUserList::KGCGuildUserList( void )
{
    m_iTopIndex     = 0;
    m_pkScrollbar   = NULL;

    for( int i = 0 ; i < (int)m_apkUserBar.size() ; ++i )
    {
        m_apkUserBar[i] = NULL;
    }

    LINK_CONTROL( "scroll", m_pkScrollbar );

    LINK_CONTROL( "user_bar0",  m_apkUserBar[0] );
    LINK_CONTROL( "user_bar1",  m_apkUserBar[1] );
    LINK_CONTROL( "user_bar2",  m_apkUserBar[2] );
    LINK_CONTROL( "user_bar3",  m_apkUserBar[3] );
    LINK_CONTROL( "user_bar4",  m_apkUserBar[4] );
    LINK_CONTROL( "user_bar5",  m_apkUserBar[5] );
    LINK_CONTROL( "user_bar6",  m_apkUserBar[6] );
    LINK_CONTROL( "user_bar7",  m_apkUserBar[7] );
    LINK_CONTROL( "user_bar8",  m_apkUserBar[8] );
    LINK_CONTROL( "user_bar9",  m_apkUserBar[9] );
    LINK_CONTROL( "user_bar10", m_apkUserBar[10] );
    LINK_CONTROL( "user_bar11", m_apkUserBar[11] );
    LINK_CONTROL( "user_bar12", m_apkUserBar[12] );
    LINK_CONTROL( "user_bar13", m_apkUserBar[13] );
    LINK_CONTROL( "user_bar14", m_apkUserBar[14] );
    LINK_CONTROL( "user_bar15", m_apkUserBar[15] );
    LINK_CONTROL( "user_bar16", m_apkUserBar[16] );
    LINK_CONTROL( "user_bar17", m_apkUserBar[17] );
}

KGCGuildUserList::~KGCGuildUserList( void )
{
    // empty
}

void KGCGuildUserList::OnCreate( void )
{
    SetSelfInputCheck( true );

    for ( int i = 0; i < NUM_GUILD_USER_LIST; ++i )
    {
        m_apkUserBar[i]->SetSelfInputCheck( true );
        m_apkUserBar[i]->SetGuildUserBar( true );
        m_apkUserBar[i]->ToggleRender( false );
    }

    m_pkScrollbar->AddActionListener( this );
    m_pkScrollbar->SetSelfInputCheck( false );

    m_pkScrollbar->SetScrollPageSize( NUM_GUILD_USER_LIST );
    m_pkScrollbar->SetScrollPos( m_iTopIndex );
}

void KGCGuildUserList::FrameMoveInEnabledState( void )
{
    if ( g_bUserListUpdate )
    {
        UpdateGuildUserList();
        g_bUserListUpdate = false;
    }

    if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
    {
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

        for ( int i = 0; i < NUM_GUILD_USER_LIST; ++i )
        {
            if ( m_apkUserBar[i]->IsRenderOn() )
            {
                if ( m_apkUserBar[i]->CheckPosInWindowBound( vMousePos ) )
                {
                    g_KDSound.Play( "31" );
                    if ( g_pkGuildMemberBox != NULL )
                    {
						KP2P::GetInstance()->Send_DetailUserInfoReq( g_vecUserInfo[m_iTopIndex + i].m_strLogin );
                    }
                    break;
                }
            }
            else break;
        }
    }
}

void KGCGuildUserList::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_pkScrollbar->GetScrollPos();
        if ( m_iTopIndex != iTop )
        {
            g_KDSound.Play( "73" );
            m_iTopIndex = iTop;
            UpdateGuildUserList();
        }
    }
}

void KGCGuildUserList::UpdateGuildUserList( void )
{
    m_pkScrollbar->SetScrollRangeMax( (int)g_vecUserInfo.size() );

    for ( int i = 0; i < NUM_GUILD_USER_LIST; ++i )
    {
        if ( m_iTopIndex + i < (int)g_vecUserInfo.size() )
        {
            m_apkUserBar[i]->ToggleRender( true );
			
			// 2007.02.08 wony
			// 계급표시
			{
				m_apkUserBar[i]->SetUserGrade( g_vecUserInfo[m_iTopIndex + i].m_cKorGrade );
			}

            KUserInfo& kInfo = g_vecUserInfo[m_iTopIndex + i];
            m_apkUserBar[i]->SetUserID( kInfo.m_strLogin );
            m_apkUserBar[i]->SetUserNickName( L"#c" + kInfo.m_strNickColor + kInfo.m_strNickName + L"#cx");
            m_apkUserBar[i]->SetUserStatus( kInfo.m_cState, kInfo.m_usRoomID );
        }
        else
        {
            m_apkUserBar[i]->ToggleRender( false );
        }
    }
}
