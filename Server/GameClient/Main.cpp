#include "Main.h"
#include "stdafx.h"
#include "MainDlg.h"

BEGIN_MESSAGE_MAP(CBotMainApp,CWinApp)
    ON_COMMAND(ID_HELP,CWinApp::OnHelp)
END_MESSAGE_MAP()

CBotMainApp::CBotMainApp() {
}

CBotMainApp theApp;

BOOL CBotMainApp::InitInstance() {
    MainDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
    if( nResponse == IDOK ){

    } else if( nResponse == IDCANCEL ) {

    }

    return FALSE;
}
