#include "stdafx.h"
//
#include "KGCLoginScene.h"
//

#include "../MyD3D.h"

//

#include "KGCLoginDlg.h"


#include "KGCLoginErrorDlg.h"
#include "KGCLoginEventDlg.h"



IMPLEMENT_CLASSNAME(KGCLoginScene);
IMPLEMENT_WND_FACTORY(KGCLoginScene);
IMPLEMENT_WND_FACTORY_NAME(KGCLoginScene, "gc_login_scene");

KGCLoginScene* g_pLoginScene = NULL;

KGCLoginScene::KGCLoginScene(void)
{
	g_pLoginScene = this;
	m_bExitProgram = false;
	LINK_CONTROL("black_window", m_pWndBalck);
	LINK_CONTROL("button_event", m_pWndGoEvent);
	LINK_CONTROL("button_shop", m_pWndGoWebShop);
	LINK_CONTROL("button_home", m_pWndGoHomePage);
	LINK_CONTROL("button_exit", m_pWndExit);

	LINK_CONTROL("login_dlg", m_pLoginDlg);

	LINK_CONTROL("login_error_dlg", m_pErrorDlg);
	LINK_CONTROL("login_event_dlg", m_pEventLoginDlg);

	m_pClickCheck = NULL;
	LINK_CONTROL("click_check", m_pClickCheck);
}

KGCLoginScene::~KGCLoginScene(void)
{
}

void KGCLoginScene::ActionPerformed(const KActionEvent& event)
{

	switch (event.m_dwCode)
	{
	case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play("30"); return;
	case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play("31"); return;
	}

	if (m_pClickCheck && event.m_pWnd == m_pClickCheck && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		if (m_pLoginDlg->IsRenderSmartPanel())
		{
			m_pLoginDlg->SetSelectedID();
			m_pClickCheck->ToggleRender(false);
		}

		return;
	}

	if (event.m_pWnd == m_pWndBalck || event.m_pWnd == m_pLoginDlg)
		return;

	GCWND_MSG_MAP(m_pErrorDlg, KD3DWnd::D3DWE_DIALOG_END, OnEndError)
	GCWND_MSG_MAP(m_pEventLoginDlg, KD3DWnd::D3DWE_DIALOG_END, OnEndEventLoginDlg)
	GCWND_MSG_MAP(m_pLoginDlg, KD3DWnd::D3DWE_DIALOG_END, OnEndLogin)
	GCWND_MSG_MAP(m_pWndGoHomePage, KD3DWnd::D3DWE_BUTTON_CLICK, OnGoHomePage)
	GCWND_MSG_MAP(m_pWndGoWebShop, KD3DWnd::D3DWE_BUTTON_CLICK, OnGoWebShop)
	GCWND_MSG_MAP(m_pWndGoEvent, KD3DWnd::D3DWE_BUTTON_CLICK, OnGoEvent)
	GCWND_MSG_MAP(m_pWndExit, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit)
}

void KGCLoginScene::OnError(KD3DWnd * pWnd, const WCHAR * wcsMsg, bool bExitProgram_)
{
	m_bExitProgram = bExitProgram_;
	m_pWndLastDlg = pWnd;
#if defined(NATION_THAILAND)
	m_pLoginDlg->ToggleRender(false);
	//	m_pLoginDlg2->ToggleRender(false);
#else
	pWnd->ToggleRender(false);
#endif

#if defined(NATION_TAIWAN)
	m_pLoginDlg->ToggleRender(true);
#endif

	m_pErrorDlg->ToggleRender(true);
	m_pErrorDlg->SetMsg(wcsMsg);
	m_pWndBalck->ToggleRender(true);
}

void KGCLoginScene::OnEndError()
{
	if (m_bExitProgram)
	{
		::PostMessage(g_MyD3D->m_hWnd, WM_CLOSE, 0, 0);
	}
#if defined(NATION_THAILAND)
	m_pLoginDlg->ToggleRender(true);
	//m_pLoginDlg2->ToggleRender(true);
#else
	m_pWndLastDlg->ToggleRender(true);
#endif
	m_pErrorDlg->ToggleRender(false);
	m_pErrorDlg->SetMsg(L"");
	m_pWndBalck->ToggleRender(false);

}

void KGCLoginScene::OnEndLogin()
{
}

void KGCLoginScene::OnEndEventLoginDlg()
{
	m_pWndBalck->ToggleRender(false);
	if (m_pEventLoginDlg->m_iEndCode == KGCLoginEventDlg::EC_CANCEL)
	{
	}
	//OnError( m_pLoginDlg, L"그걸 왜 눌렀니??-_-;");
}

void KGCLoginScene::OnGoEvent()
{
#if defined ( NATION_BRAZIL ) || defined ( NATION_PHILIPPINE ) || defined( NATION_EUROPE ) || defined( NATION_HONGKONG) || defined(NATION_THAILAND) || defined( NATION_TAIWAN ) || defined( NATION_LATINAMERICA ) || defined( NATION_CHINA ) || defined( NATION_USA )
	PopUpWebPage(HOMEPAGE_EVENT_LINK);
#endif
	/*
	m_pLoginDlg->ToggleRender( false );
	m_pEventLoginDlg->DoModal();
	m_pWndBalck->ToggleRender( true );
	*/
	//::PostMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );	
	//ShellExecute(NULL, L"open", L"http://www.daum.net", NULL, NULL, SW_SHOWNORMAL);
}

void KGCLoginScene::OnGoWebShop()
{
#if !defined( NATION_IDN ) && !defined(NATION_CHINA)
	PopUpWebPage(HOMAPAGE_SHOP_LINK);
#endif
	// 중국은 이버튼을 회원가입 버튼으로 사용
#if defined( NATION_CHINA )
	PopUpWebPage(HOMEPAGE_JOIN);
#endif
}

void KGCLoginScene::OnGoHomePage()
{
	PopUpWebPage(HOMEPAGE_LINK);
}

void KGCLoginScene::OnExit()
{
	KP2P::GetInstance()->Send_ExitNormalNot();
	::PostMessage(g_MyD3D->m_hWnd, WM_CLOSE, 0, 0);
}


void KGCLoginScene::OnCreate(void)
{
	//ToggleRender(false);
	m_pWndBalck->InitState(false, true, this);
	m_pWndExit->InitState(true, true, this);
	m_pLoginDlg->InitState(true, true, this);
#if defined(NATION_THAILAND)
	m_pLoginDlg->SetFunBoxPostfix(-1);
	//m_pLoginDlg2->SetFunBoxPostfix(1);
	//m_pLoginDlg2->InitState( true, true, this );
	m_pWndGoHomePage->InitState(false, false, NULL);
	m_pWndGoWebShop->InitState(false, false, NULL);
	m_pWndGoEvent->InitState(false, false, NULL);
#elif defined( NATION_LATINAMERICA ) || defined (NATION_BRAZIL)
	m_pWndGoWebShop->InitState(false, false, NULL);
	m_pWndGoHomePage->InitState(true, true, this);
	m_pWndGoEvent->InitState(true, true, this);
#else
	m_pWndGoHomePage->InitState(true, true, this);
	m_pWndGoWebShop->InitState(true, true, this);
	m_pWndGoEvent->InitState(true, true, this);
#endif

	m_pErrorDlg->InitState(false, true, this);
	m_pEventLoginDlg->InitState(false, true, this);

	if (m_pClickCheck)
		m_pClickCheck->InitState(false, true, this);
#if defined(NATION_CHINA)
	m_pWndGoEvent->SetWndMode(D3DWM_LOCK);
#endif
}


void KGCLoginScene::ConnectLoginServer()
{
	if (m_pLoginDlg != NULL)
	{
		m_pLoginDlg->ConnectLoginServer();
	}
	else
	{
		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INITIALIZE_FAIL1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INITIALIZE_FAIL2).c_str(),
			KMsgBox::MBOX_OK);
		SendMessage(g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0);
	}
}

void KGCLoginScene::SetIDString(const std::wstring & strIDString_)
{
	m_pLoginDlg->SetIDString(strIDString_);
}

void KGCLoginScene::SetPWString(const std::wstring & strPWString_)
{
	m_pLoginDlg->SetPWString(strPWString_);
}

void KGCLoginScene::OnCreateComplete(void)
{
#ifdef AGREEMENT
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_AGREEMENT);
#endif
}

void KGCLoginScene::FrameMoveInEnabledState()
{
	if (m_pClickCheck)
	{
		if (!m_pLoginDlg->IsRenderSmartPanel())
			m_pClickCheck->ToggleRender(false);
		else if (m_pLoginDlg->IsCusorInVK())
			m_pClickCheck->ToggleRender(false);
		else
			m_pClickCheck->ToggleRender(true);
	}
}

void KGCLoginScene::OnEnterScene()
{
	g_pkUIScene->m_pkOnGameDlg->ToggleRender(false);
	g_pkUIScene->m_pkWorldMapContainer->ToggleRender(false);

	g_pkUIScene->m_pkOnGameDlg->ToggleRender(false);
	g_pkUIScene->EnableMessageBox(false);

	g_pkUIScene->m_pkNickNameBox->NickNameRegNameReset();

}

