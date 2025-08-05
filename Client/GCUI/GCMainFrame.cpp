#include "stdafx.h"
#include "GCMainFrame.h"
//
#include "../MyD3D.h"

#include "KMci.h"

//

IMPLEMENT_CLASSNAME( KGCMainFrame );
IMPLEMENT_WND_FACTORY( KGCMainFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCMainFrame, "gc_main_frame" );

KGCMainFrame::KGCMainFrame( void )
{
    m_pkMinimize    = NULL;
    m_pkMaximize    = NULL;
    m_pkClose       = NULL;
	m_pkAgeLimit	= NULL;

    LINK_CONTROL( "main_frame_minimize",    m_pkMinimize );
    LINK_CONTROL( "main_frame_maximize",    m_pkMaximize );
    LINK_CONTROL( "main_frame_close",       m_pkClose );
	LINK_CONTROL( "main_frame_age_limit",	m_pkAgeLimit );
}

KGCMainFrame::~KGCMainFrame( void )
{
    // empty
}

void KGCMainFrame::OnCreate( void )
{
#if defined( NATION_KOREA )
	m_pkAgeLimit->InitState( true, true, this );
#else
	m_pkAgeLimit->InitState( false );
#endif
    m_pkMinimize->SetSelfInputCheck( true );
    m_pkMaximize->SetSelfInputCheck( true );
    m_pkClose->SetSelfInputCheck( true );

    m_pkMinimize->AddActionListener( this );
    m_pkMaximize->AddActionListener( this );
    m_pkClose->AddActionListener( this );
}

void KGCMainFrame::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
        case KD3DWnd::D3DWE_BUTTON_CLICK:
        {
            if ( event.m_pWnd == m_pkMinimize )
            {
                ::PostMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
            }
            else if ( event.m_pWnd == m_pkMaximize )
            {
                ::PostMessage( g_MyD3D->m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );
            }
            else if ( event.m_pWnd == m_pkClose )
            {
#if defined(USE_LOGOUT)
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_LOGOUT_BOX);
#else
// 				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
// 					, KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, true );
                if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER || g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
                    , KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false );
                else
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L"", KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false);
#endif
            }
			else if ( event.m_pWnd == m_pkAgeLimit )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GRADE_SHOW );
			}
            break;
        }
    }
}
