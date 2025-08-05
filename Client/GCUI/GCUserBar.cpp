#include "stdafx.h"
#include "GCUserBar.h"
#include "GCChatBox.h"
//
//
//
//
#include "../MyD3D.h"




#include "GCAddFriendMenu.h"
#include "KGCUIHelper.h"

#include "../GCStateMachine.h"
#include "gcui/KGCGuildLobbyDlg.h"

IMPLEMENT_CLASSNAME( KGCUserBar );
IMPLEMENT_WND_FACTORY( KGCUserBar );
IMPLEMENT_WND_FACTORY_NAME( KGCUserBar, "gc_user_bar" );

KGCUserBar::KGCUserBar( void )
: m_usRoomID(0xFFFF)
{
    m_bGuildUserBar = false;
    m_iMark         = 0;
    m_bGuildChannel = false;
    m_bJoin         = false;
    m_cStatus       = -1;
    m_eUserStatus   = US_WAIT;
    m_eUserGrade    = UG_VAGABOND;
    m_strLoginID.erase();

    m_pkWait        = NULL;
    m_pkPlay        = NULL;
    m_pkLobby       = NULL;
    m_pkJoin        = NULL;
    m_pkUserID      = NULL;
    m_pkSquare      = NULL;

    for( int i = 0 ; i < (int)m_apkGrade.size() ; ++i )
    {
        m_apkGrade[i] = NULL;
    }

    LINK_CONTROL( "user_wait",   m_pkWait );
    LINK_CONTROL( "user_play",   m_pkPlay );
    LINK_CONTROL( "user_lobby",  m_pkLobby );
    LINK_CONTROL( "user_join",   m_pkJoin );
    LINK_CONTROL( "user_bar_id", m_pkUserID );
    LINK_CONTROL( "user_square", m_pkSquare );

    LINK_CONTROL( "grade_vagabond_small",   m_apkGrade[0] );
    LINK_CONTROL( "grade_mercenary_small",  m_apkGrade[1] );
    LINK_CONTROL( "grade_royal_small",      m_apkGrade[2] );
    LINK_CONTROL( "grade_holy_small",       m_apkGrade[3] );
}

KGCUserBar::~KGCUserBar( void )
{
    // empty
}

void KGCUserBar::OnCreate( void )
{
	SetSelfInputCheck( true );
	AddActionListener( this );

    m_pkWait->SetSelfInputCheck( false );
    m_pkPlay->SetSelfInputCheck( false );
    m_pkLobby->SetSelfInputCheck( false );
    m_pkJoin->SetSelfInputCheck( false );
    m_pkUserID->SetSelfInputCheck( false );

    m_pkWait->ToggleRender( false );
    m_pkPlay->ToggleRender( false );
    m_pkLobby->ToggleRender( false );
    m_pkJoin->ToggleRender( false );

    m_pkSquare->InitState( false );

    for ( int i = 0; i < NUM_USER_GRADE; ++i )
    {
        m_apkGrade[i]->SetSelfInputCheck( false );
        m_apkGrade[i]->ToggleRender( false );
    }

    SetUserStatus( US_WAIT );
    SetUserGrade( 0 );
}

void KGCUserBar::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK
		|| event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_CLICK )
		g_pkAddFriendMenu->ToggleRender( false );

	if( event.m_dwCode == D3DWE_RBUTTON_CLICK )
	{
		if( g_kGlobalValue.m_kUserInfo.strNickName != m_pkUserID->GetText() )
		{
			g_KDSound.Play( "31" );

			g_pkAddFriendMenu->ToggleRender( true );

			POINT pt = g_pkInput->GetMousePos();
			D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
			D3DXVECTOR2 vPos( event.m_pWnd->GetFixedWindowPos() );
			D3DXVECTOR2 vPos2( (float)0.0, (float)0.0 );

			// 현재 클릭 된 userbar의 인덱스를 얻어와서 ( erase( 0, 8 ) )
			// 높이만큼 곱해서 위치를 잡아준다
			std::string stdWindowName = event.m_pWnd->GetWindowName();
			vPos2.y = (float)( atoi( stdWindowName.erase( 0, 8 ).c_str() ) * 25 );

			g_pkAddFriendMenu->SetWindowPosDirect( vMousePos - vPos + vPos2 );
			g_pkAddFriendMenu->SetFriendNickName( m_pkUserID->GetText() );

			return;
		}
	}

	if( event.m_pWnd == this && event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		g_KDSound.Play( "31" );
		if( US_WAIT != m_eUserStatus )
			return;
		
		if ( m_bGuildUserBar )
		{
            //StringCchCopyW( g_Selected_Session_Name, MAX_PATH, g_pkStrLoader->GetString( STR_ID_ROOM_GUILD_MEMBER ).c_str() );
            SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_GUILD, -1,  m_strLoginID );
		}
		else
		{
            // [1/28/2008] breadceo. m_cStatus 안에 RoomID 까지 있었는데 광장이 들어가면서
            // m_usRoomID 라는거에 담도록 바뀌었다.
            // 나중에 필요하게 되면 여기에 적용하셈.
			//// 해당 유저가 소속된 방으로 조인한다.
			//if ( -1 != m_cStatus || -2 != m_cStatus )
			//{
			//	SiKGCRoomManager()->GetRoomInfo().usRoomID = m_cStatus;

            //  std::vector<KSimpleRoomInfo>& vecRoomList = SiKGCRoomManager()->GetRoomList();

			//	for ( UINT i = 0; i < vecRoomList.size(); ++i )
			//	{
			//		if ( m_cStatus == vecRoomList[i].m_usRoomID )
			//		{
			//			if ( vecRoomList[i].m_bPublic )
			//			{
			//				g_MyD3D->JoinRoom( KJoinRoomReqInfo::JOIN_NORMAL, SiKGCRoomManager()->GetRoomInfo().usRoomID );
			//			}
			//			else
			//			{
			//				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PASSWORD );
			//			}
			//			break; // found
			//		}
			//	}

			//	if ( i == vecRoomList.size() ) // not found
			//	{
			//		g_MyD3D->JoinRoom( KJoinRoomReqInfo::JOIN_NORMAL, SiKGCRoomManager()->GetRoomInfo().usRoomID );
			//	}
			//}
		}
		return;		
	}
}


void KGCUserBar::FrameMoveInEnabledState( void )
{
    if ( US_WAIT == m_eUserStatus && KD3DWnd::D3DWM_HOVER == GetWndMode() )
    {
        if ( false == m_bJoin )
        {
            g_KDSound.Play( "30" );
        }

        m_pkWait->ToggleRender( false );
        m_pkPlay->ToggleRender( false );
        m_pkLobby->ToggleRender( false );
        m_pkSquare->ToggleRender( false );
        m_pkJoin->ToggleRender( true );

        m_bJoin = true;
    }
    else
    {
        if ( true == m_bJoin )
        {
            // empty
        }

        if ( KD3DWnd::D3DWM_HOVER == GetWndMode() ) SetWndMode( D3DWM_DEFAULT );

        SetUserStatus( m_eUserStatus );
        m_pkJoin->ToggleRender( false );

        m_bJoin = false;
    }

    if ( g_pkInput->BtnUp( KInput::MBRIGHT ) )
    {
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

        if ( CheckPosInWindowBound( vMousePos ) )
        {
            std::wstring strText( L"/t " );
            strText += m_pkUserID->GetText();
            strText += ' ';

            switch ( g_MyD3D->m_pStateMachine->GetState() )
            {
                case GS_MATCH:
                {
                    if ( g_pkLobbyChatBox != NULL ) g_pkLobbyChatBox->SetText( strText );
                    break;
                }
                case GS_ROOM:
                {
                    if ( g_pkRoomChatBox != NULL ) g_pkRoomChatBox->SetText( strText );
                    break;
                }
                case GS_GUILD:
                {
                    if ( g_pkGuildChatBox != NULL ) g_pkGuildChatBox->SetText( strText );
                    break;
                }
            }

            if(g_pkUIScene->m_pkGuildLobbyDlg->IsRenderOn() && g_pkGuildLobbyChatBox != NULL )
                g_pkGuildLobbyChatBox->SetText( strText );
        }
    }
}

void KGCUserBar::PostDraw( void )
{
    //if ( m_bGuildChannel )
    //{
		
    //    D3DXVECTOR2 vPos( GetFixedWindowPos() );

    //    float x = ( vPos.x / 400.0f ) - 1.0f;
    //    float y = ( -vPos.y / 300.0f * 0.75f ) + 0.75f;

    //    // 길드 마크를 그린다.
    //    g_MyD3D->m_KGCGuildMark.Render( ((BYTE*)(&m_iMark))[0],
    //                                      ((BYTE*)(&m_iMark))[1],
    //                                      ((BYTE*)(&m_iMark))[2],
    //                                      x + 0.134f, y - 0.028f, 0.078f );
    //}
}

void KGCUserBar::SetUserStatus( char cStatus, USHORT usRoomID )
{
    // channel: 1, playing: 2, room: 3, square: 4
    // 상태가 룸이면 RoomID가 의미 있음.
    m_cStatus = cStatus;
    m_usRoomID = usRoomID;

    switch( m_cStatus )
    {
    case 1: // lobby
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( true );
            m_pkSquare->ToggleRender( false );
            m_eUserStatus = US_LOBBY;
        }
        break;
    case 2: // play
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( true );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( false );
            m_eUserStatus = US_PLAY;
        }
        break;
    case 3: // wait
        {
            m_pkWait->ToggleRender( true );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( false );
            m_eUserStatus = US_WAIT;
        }
        break;
    case 4: // square
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( false );
            m_eUserStatus = US_SQUARE;
        }
        break;
    default:
        {
            ASSERT( !"Invalid Status" );
        }
        break;
    }
}

void KGCUserBar::SetUserStatus( EUserStatus eStatus )
{
    switch ( m_eUserStatus = eStatus )
    {
    case US_LOBBY:
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( true );
            m_pkSquare->ToggleRender( false );
        }
        break;
    case US_PLAY:
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( true );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( false );
        }
        break;
    case US_WAIT:
        {
            m_pkWait->ToggleRender( true );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( false );
        }
        break;
    case US_SQUARE:
        {
            m_pkWait->ToggleRender( false );
            m_pkPlay->ToggleRender( false );
            m_pkLobby->ToggleRender( false );
            m_pkSquare->ToggleRender( true );
        }
        break;
    }
}

void KGCUserBar::SetUserGrade( char cGrade )
{
    g_pkUIHelper->SetCharacterGrade( m_apkGrade, (int)cGrade );
}

void KGCUserBar::SetUserID( const std::wstring& strUserID )
{
    m_strLoginID = strUserID;
}

void KGCUserBar::SetUserNickName( const std::wstring& strUserNickName )
{
    m_pkUserID->SetText( strUserNickName );
}

void KGCUserBar::SetGuildChannel( bool bGuildChannel )
{
    if ( m_bGuildChannel = bGuildChannel ) // NOT '=='
    {
        SetUserGrade( 0 );
    }
}

void KGCUserBar::SetMark( int iMark )
{
    m_iMark = iMark;
}

void KGCUserBar::SetGuildUserBar( bool bGuildUserBar )
{
    m_bGuildUserBar = bGuildUserBar;
}
