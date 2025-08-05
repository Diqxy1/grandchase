#include "stdafx.h"
#include "KGCReplayBar.h"

IMPLEMENT_CLASSNAME( KGCReplayBar );
IMPLEMENT_WND_FACTORY( KGCReplayBar );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayBar, "gc_replay_bar" );

KGCReplayBar::KGCReplayBar( void )
: m_pkFileName(NULL)
{
    LINK_CONTROL("filename", m_pkFileName);
}

KGCReplayBar::~KGCReplayBar( void )
{
}

void KGCReplayBar::OnCreate()
{
    m_pkFileName->SetText(L"");
    m_pkFileName->SetAlign(DT_CENTER);
}


void KGCReplayBar::ActionPerformed(const KActionEvent& event )
{

}

void KGCReplayBar::FrameMoveInEnabledState()
{

}

void KGCReplayBar::SetBarText( std::wstring strFileName )
{
    m_pkFileName->SetForceWordWrapText(strFileName, true);
    m_pkFileName->SetToolTip(strFileName);
}

void KGCReplayBar::CursorEnter( )
{
    m_pkFileName->SetFontColor( 0xffffff00 );
}

void KGCReplayBar::CursorLeave( )
{
    m_pkFileName->SetFontColor( 0xffffffff );
}
