#include "stdafx.h"
#include "KGCChannelBar.h"


IMPLEMENT_CLASSNAME( KGCChannelBar );
IMPLEMENT_WND_FACTORY( KGCChannelBar );
IMPLEMENT_WND_FACTORY_NAME( KGCChannelBar, "gc_channel_bar" );

KGCChannelBar::KGCChannelBar( void )
{
    m_pkChannelName = NULL;
    m_pkProgressBar = NULL;

    LINK_CONTROL( "channel_name",   m_pkChannelName );
    LINK_CONTROL( "progress",       m_pkProgressBar );
}

KGCChannelBar::~KGCChannelBar( void )
{
}

void KGCChannelBar::OnCreate( void )
{
    m_pkChannelName->InitState( true );
    m_pkChannelName->SetWordWrap( true );
    m_pkProgressBar->InitState( true );

    m_vTextureCoords = m_pkProgressBar->GetWndTexCoords();
}

void KGCChannelBar::FrameMoveInEnabledState( void )
{
}

void KGCChannelBar::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play( "31" ); return;
    }
}

#define MAX_USER_IN_CHANNEL ( 500 )
void KGCChannelBar::SetChannelInfo( KChannelInfo& kChannelInfo, D3DCOLOR kChannelNameColor )
{
    m_kChannelInfo = kChannelInfo;

    //m_pkChannelName->SetText( m_kChannelInfo.m_strChannelName );
    m_pkChannelName->SetForceWordWrapText( m_kChannelInfo.m_strChannelName , true );
    m_pkChannelName->SetToolTip( m_kChannelInfo.m_strChannelName );
    m_pkChannelName->SetFontColor( kChannelNameColor );

    float fPercent = (float)m_kChannelInfo.m_usUsers / (float)MAX_USER_IN_CHANNEL;

    if( fPercent < (float)( MIN_PERCENT / 100.0f ) ) // 사람이 아무도 없어도 찔끔 보여준다
        fPercent = (float)( MIN_PERCENT / 100.0f );
    else if( fPercent > 0.94f ) // 채널에 제한이 없어질거라서.. 꽉차도 찼다고 보여주지 않겠음
        fPercent = 0.94f;

    m_pkProgressBar->SetWndTexCoords( D3DXVECTOR4( m_vTextureCoords.x, m_vTextureCoords.y,
                                      m_vTextureCoords.x + fPercent * ( m_vTextureCoords.z - m_vTextureCoords.x ), m_vTextureCoords.w ) );

    m_pkProgressBar->SetWidth( (DWORD)( fPercent * MAX_CHANNEL_BAR_WIDTH ) );
}