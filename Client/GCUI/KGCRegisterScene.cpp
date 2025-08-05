#include "stdafx.h"
#include "KGCRegisterScene.h"
#include "../MyD3D.h"
#include "KGCRegisterDlg.h"
#include "KGCLoginErrorDlg.h"

IMPLEMENT_CLASSNAME(KGCRegisterScene);
IMPLEMENT_WND_FACTORY(KGCRegisterScene);
IMPLEMENT_WND_FACTORY_NAME(KGCRegisterScene, "gc_register_scene");

KGCRegisterScene* g_pRegisterScene = NULL;

KGCRegisterScene::KGCRegisterScene(void)
{
	g_pRegisterScene = this;
	m_bExitProgram = false;
	LINK_CONTROL("black_window", m_pWndBalck);
	LINK_CONTROL("button_exit", m_pWndExit);
	LINK_CONTROL("register_dlg", m_pRegisterDlg);
	LINK_CONTROL("register_error_dlg", m_pErrorDlg);
}

KGCRegisterScene::~KGCRegisterScene(void)
{
}

void KGCRegisterScene::ActionPerformed(const KActionEvent& event)
{

	switch (event.m_dwCode)
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play("30"); return;
		case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play("31"); return;
	}

	if (event.m_pWnd == m_pWndBalck || event.m_pWnd == m_pRegisterDlg)
		return;

	GCWND_MSG_MAP(m_pErrorDlg, KD3DWnd::D3DWE_DIALOG_END, OnEndError)
	GCWND_MSG_MAP(m_pRegisterDlg, KD3DWnd::D3DWE_DIALOG_END, OnEndLogin)
	GCWND_MSG_MAP(m_pWndExit, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit)
}

void KGCRegisterScene::OnError(KD3DWnd * pWnd, const WCHAR * wcsMsg, bool bExitProgram_)
{
	m_bExitProgram = bExitProgram_;
	m_pWndLastDlg = pWnd;
	pWnd->ToggleRender(false);
	m_pErrorDlg->ToggleRender(true);
	m_pErrorDlg->SetMsg(wcsMsg);
	m_pWndBalck->ToggleRender(true);
}

void KGCRegisterScene::OnEndError()
{
	if (m_bExitProgram)
	{
		::PostMessage(g_MyD3D->m_hWnd, WM_CLOSE, 0, 0);
	}
	m_pWndLastDlg->ToggleRender(true);
	m_pErrorDlg->ToggleRender(false);
	m_pErrorDlg->SetMsg(L"");
	m_pWndBalck->ToggleRender(false);
}

void KGCRegisterScene::OnEndLogin()
{
}

void KGCRegisterScene::OnExit()
{
	KP2P::GetInstance()->Send_ExitNormalNot();
	::PostMessage(g_MyD3D->m_hWnd, WM_CLOSE, 0, 0);
}

void KGCRegisterScene::OnCreate(void)
{
	m_pWndBalck->InitState(false, true, this);
	m_pWndExit->InitState(true, true, this);
	m_pRegisterDlg->InitState(true, true, this);
	m_pErrorDlg->InitState(false, true, this);
}

void KGCRegisterScene::ConnectLoginServer()
{
	if ( m_pRegisterDlg != NULL )
	{
		m_pRegisterDlg->ConnectLoginServer();
	}
	else
	{
		KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INITIALIZE_FAIL1 ).c_str(), g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INITIALIZE_FAIL2 ).c_str(), KMsgBox::MBOX_OK );
		SendMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
	}
}

void KGCRegisterScene::SetIDString(const std::wstring & strIDString_)
{
	m_pRegisterDlg->SetIDString(strIDString_);
}

void KGCRegisterScene::SetPWString(const std::wstring & strPWString_)
{
	m_pRegisterDlg->SetPWString(strPWString_);
}

void KGCRegisterScene::SetEmailString(const std::wstring & strEmailString_)
{
	m_pRegisterDlg->SetEmailString( strEmailString_ );
}

void KGCRegisterScene::OnCreateComplete(void)
{
}

void KGCRegisterScene::FrameMoveInEnabledState()
{
}

void KGCRegisterScene::OnEnterScene()
{
	g_pkUIScene->m_pkOnGameDlg->ToggleRender(false);
	g_pkUIScene->m_pkWorldMapContainer->ToggleRender(false);
	g_pkUIScene->m_pkOnGameDlg->ToggleRender(false);
	g_pkUIScene->EnableMessageBox(false);
	g_pkUIScene->m_pkNickNameBox->NickNameRegNameReset();
}

