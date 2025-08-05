#include "stdafx.h"
#include "GCInviteReplayBar.h"
//
//
//
#include "../MyD3D.h"

#include "KGCUIHelper.h"
IMPLEMENT_CLASSNAME( KGCInviteReplayBar );
IMPLEMENT_WND_FACTORY( KGCInviteReplayBar );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteReplayBar, "gc_invite_replay_bar" );

KGCInviteReplayBar::KGCInviteReplayBar( void )
{
    m_bGuildChannel = false;
    m_bInviteMode   = true;
    m_iMark         = 0;
    m_cGrade        = 0;
    m_eUserGrade    = UG_VAGABOND;
    m_strNickName.erase();

    m_pkUserID          = NULL;
    m_pkInviteReplayBtn = NULL;

    for( int i = 0 ; i < (int)m_apkGrade.size() ; ++i )
    {
        m_apkGrade[i] = NULL;
    }

    LINK_CONTROL( "invite_replay_grade_vagabond",   m_apkGrade[0] );
    LINK_CONTROL( "invite_replay_grade_mercenary",  m_apkGrade[1] );
    LINK_CONTROL( "invite_replay_grade_royal",      m_apkGrade[2] );
    LINK_CONTROL( "invite_replay_grade_holy",       m_apkGrade[3] );

    LINK_CONTROL( "invite_replay_static",           m_pkUserID );
    LINK_CONTROL( "invite_replay_button",           m_pkInviteReplayBtn );
}

KGCInviteReplayBar::~KGCInviteReplayBar( void )
{
    // empty
}

void KGCInviteReplayBar::OnCreate( void )
{
    for ( int i = 0; i < NUM_USER_GRADE; ++i )
    {
        m_apkGrade[i]->SetSelfInputCheck( false );
    }

    m_pkUserID->SetSelfInputCheck( false );
    m_pkInviteReplayBtn->SetSelfInputCheck( false );
    m_pkInviteReplayBtn->ToggleRender( !m_bInviteMode );
}

void KGCInviteReplayBar::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if ( CheckPosInWindowBound( vMousePos ) )
    {
        m_pkUserID->SetFontColor( 0xff00ff00 );

        if ( m_bInviteMode && g_pkInput->BtnUp( KInput::MBLEFT ) )
        {
            g_KDSound.Play( "31" );
            m_pkInviteReplayBtn->ToggleRender( !m_pkInviteReplayBtn->IsRenderOn() );
            SpeakToActionListener( KActionEvent( this ) );
        }
    }
    else m_pkUserID->SetFontColor( 0xffffffff );

    if ( !m_bInviteMode && m_pkInviteReplayBtn->CheckPosInWindowBound( vMousePos ) )
    {
        if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
        {
            g_KDSound.Play( "31" );
        }
        else if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
        {
            SpeakToActionListener( KActionEvent( this ) );
        }
    }
}

void KGCInviteReplayBar::PostDraw( void )
{
    //if ( m_bInviteMode && m_bGuildChannel )
    //{		

    //    D3DXVECTOR2 vPos( GetFixedWindowPos() );

    //    float x = ( vPos.x / 400.0f ) - 1.0f;
    //    float y = ( -vPos.y / 300.0f * 0.75f ) + 0.75f;

    //    // 길드 마크를 그린다.
    //    g_MyD3D->m_KGCGuildMark.Render( ((BYTE*)(&m_iMark))[0],
    //                                      ((BYTE*)(&m_iMark))[1],
    //                                      ((BYTE*)(&m_iMark))[2],
    //                                      x + 0.035f, y - 0.028f, 0.078f );
    //}
}

void KGCInviteReplayBar::SetOnOff( bool bOnOff )
{
    ASSERT( m_pkInviteReplayBtn != NULL );
    m_pkInviteReplayBtn->ToggleRender( bOnOff );
}

void KGCInviteReplayBar::SetGuildChannel( bool bGuildChannel )
{
    m_bGuildChannel = bGuildChannel;
}

void KGCInviteReplayBar::SetIndex( int iIndex )
{
    m_iIndex = iIndex;
}

void KGCInviteReplayBar::SetMark( int iMark )
{
    m_iMark = iMark;

    for ( int i = 0; i < NUM_USER_GRADE; ++i )
    {
        m_apkGrade[i]->ToggleRender( false );
    }
}

void KGCInviteReplayBar::SetUserGrade( char cGrade )
{
    g_pkUIHelper->SetCharacterGrade( m_apkGrade, (int)cGrade );
	m_cGrade = cGrade;
}

void KGCInviteReplayBar::SetUserNickName( const std::wstring& strNickName )
{
    m_pkUserID->SetText( m_strNickName = strNickName );
}

void KGCInviteReplayBar::SetMode( bool bInviteMode )
{
    if ( m_bInviteMode = bInviteMode ) // NOT '=='
    {
        SetUserGrade( m_cGrade );
        m_pkUserID->SetOffset( D3DXVECTOR2( 0.0f, 0.0f ) );
        m_pkInviteReplayBtn->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
    }
    else
    {
        for ( int i = 0; i < NUM_USER_GRADE; ++i )
        {
            m_apkGrade[i]->ToggleRender( false );
        }
        m_pkUserID->SetOffset( D3DXVECTOR2( -20.0f, 0.0f ) );
        m_pkInviteReplayBtn->SetWndMode( KD3DWnd::D3DWM_SELECT );
    }
}
