// GCFileDiff.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GCFileDiff.h"
#include "GCFileDiffDlg.h"
#include "KMsgBox.h"
#include ".\gcfilediff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp

BEGIN_MESSAGE_MAP(CGCFileDiffApp, CWinApp)
	//{{AFX_MSG_MAP(CGCFileDiffApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp construction

CGCFileDiffApp::CGCFileDiffApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGCFileDiffApp object

CGCFileDiffApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp initialization

BOOL CGCFileDiffApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/

    // for CDropEdit!!
    ::CoInitialize(NULL);

	CGCFileDiffDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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

int CGCFileDiffApp::ExitInstance()
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    // for CDropEdit!!
    ::CoUninitialize();

    return CWinApp::ExitInstance();
}
