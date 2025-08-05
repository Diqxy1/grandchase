#include "stdafx.h"
//////
//#include "KGCServerListScene.h"
////
//
////
//
//
//
//
//IMPLEMENT_CLASSNAME( KGCServerListScene );
//IMPLEMENT_WND_FACTORY( KGCServerListScene );
//IMPLEMENT_WND_FACTORY_NAME( KGCServerListScene, "gc_serverlist_scene" );
//
//KGCServerListScene* g_pServerListScene = NULL;
//
//KGCServerListScene::KGCServerListScene(void)
//{
//	g_pServerListScene = this;
//	
//		
//		
//	LINK_CONTROL("main_frame_minimize",	m_pWndMinimize	);
//	LINK_CONTROL("main_frame_maximize",	m_pWndMaximize	);
//	LINK_CONTROL("main_frame_close",	m_pWndExit		);
//}
//
//KGCServerListScene::~KGCServerListScene(void)
//{
//
//}
//
//void KGCServerListScene::ActionPerformed( const KActionEvent& event )
//{
//	GCWND_MSG_MAP( m_pWndMinimize,	KD3DWnd::D3DWE_BUTTON_CLICK, OnMinimize )
//	GCWND_MSG_MAP( m_pWndMaximize,	KD3DWnd::D3DWE_BUTTON_CLICK, OnMaximize )
//	GCWND_MSG_MAP( m_pWndExit,		KD3DWnd::D3DWE_BUTTON_CLICK, OnExit )
//}
//
//void KGCServerListScene::Initialize()
//{
//	g_pServerListDlg->UpdateServerList();
//	KD3DWnd::Initialize();
//}
//
//void KGCServerListScene::OnExit()
//{
//	//::PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
//	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
//		, KGCUIScene::GC_MBOX_USE_GAMEEXIT );
//}
//
//void KGCServerListScene::OnMinimize()
//{
//	::PostMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
//}
//
//void KGCServerListScene::OnMaximize()
//{
//	::PostMessage( g_MyD3D->m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );	
//}
//
//void KGCServerListScene::OnCreate( void )
//{
//	m_pWndMinimize->InitState(true,true,this);
//	m_pWndMaximize->InitState(true,true,this);
//	m_pWndExit->InitState(true,true,this);	
//}
//
