#include "stdafx.h"
#include "GCSocketIcon.h"

IMPLEMENT_CLASSNAME( KGCSocketIcon );
IMPLEMENT_WND_FACTORY( KGCSocketIcon );
IMPLEMENT_WND_FACTORY_NAME( KGCSocketIcon, "gc_socket_icon" );

KGCSocketIcon::KGCSocketIcon( void )
: m_eItemSocketState( KSocketInfo::STS_CLOSED )
{
    for( int i = 0 ; i < KSocketInfo::STS_NUM; ++i )
    {
        m_pkSocketIcon[i] = NULL;

        char szTemp[MAX_PATH] = "";
        sprintf_s( szTemp, MAX_PATH, "socket_icon_state%d", i );
        LINK_CONTROL( szTemp, m_pkSocketIcon[i] );
    }
}

KGCSocketIcon::~KGCSocketIcon( void )
{
}

void KGCSocketIcon::OnCreate( void )
{
    for( int i = 0 ; i < KSocketInfo::STS_NUM; ++i )
    {
        m_pkSocketIcon[i]->InitState( false, true, this);
    }
}

void KGCSocketIcon::OnCreateComplete( void )
{
}

void KGCSocketIcon::ActionPerformed( const KActionEvent& event_ )
{
    for( int i = 0; i < KSocketInfo::STS_NUM; ++i)
    {
        if( event_.m_pWnd == m_pkSocketIcon[i] )
            SpeakToActionListener( KActionEvent( this, event_.m_dwCode) );
    }
}

void KGCSocketIcon::SetSocketState( KSocketInfo::SOCKET_STATE eItemSocketState_ )
{
    m_pkSocketIcon[ m_eItemSocketState ]->ToggleRender( false );
    m_pkSocketIcon[ eItemSocketState_ ]->ToggleRender( true );
    m_eItemSocketState = eItemSocketState_;
}

void KGCSocketIcon::AllSelfInputCheck( bool bUpdate_ )
{
    for(int i = 0 ; i < KSocketInfo::STS_NUM; ++i )
    {
        m_pkSocketIcon[i]->SetSelfInputCheck( bUpdate_ );
    }
}