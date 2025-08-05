#include "stdafx.h"
#include "GCOnGameDlg.h"
//
//
//



//
//
//
//
#include "GCTutorialDlg.h"
#include "GCArenaBattleDlg.h"

/*
사실은 게임 내에서 여러 Dialog를 컨트롤 하려고 만들었는데
튜토리얼의 Custom한 Dialog가 되고 말았다-_-;;
*/
IMPLEMENT_CLASSNAME( KGCOnGameDlg );
IMPLEMENT_WND_FACTORY( KGCOnGameDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCOnGameDlg, "gc_ongame_dlg" );

KGCOnGameDlg*   g_pkOnGameDlg       = NULL;

KGCOnGameDlg::KGCOnGameDlg( void )
{
    g_pkOnGameDlg                   = this;
    m_pkTutorialDlg                 = NULL;
    m_pkArenaBattleDlg              = NULL;
    LINK_CONTROL( "tutorialdlg",         m_pkTutorialDlg );
    LINK_CONTROL( "arenabattledlg",      m_pkArenaBattleDlg );
}

KGCOnGameDlg::~KGCOnGameDlg( void )
{
}

void KGCOnGameDlg::OnInitialize()
{
    m_pkArenaBattleDlg->ToggleRender( false );
    m_pkTutorialDlg->ToggleRender( false );
    m_pkArenaBattleDlg->Init();
    m_pkTutorialDlg->Init();
}

void KGCOnGameDlg::OnCreate( void )
{
}

void KGCOnGameDlg::OnDestroy( void )
{
}

void KGCOnGameDlg::FrameMoveInEnabledState( void )
{
}

void KGCOnGameDlg::PostChildDraw( void )
{
}
