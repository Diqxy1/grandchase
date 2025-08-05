// WGameServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerUIDlg.h"
#include "ServerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
END_MESSAGE_MAP()

using namespace std;

// CServerApp construction

CServerApp::CServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CServerApp object

CServerApp theApp;

// CServerApp initialization

BOOL CServerApp::InitInstance()
{
	CWinApp::InitInstance();

	CServerUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
