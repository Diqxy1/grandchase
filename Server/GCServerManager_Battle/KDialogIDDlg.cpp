// KDialogIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KDialogIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KDialogIDDlg dialog


KDialogIDDlg::KDialogIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KDialogIDDlg::IDD, pParent)
    , m_iNickname(0)
{
	//{{AFX_DATA_INIT(KDialogIDDlg)
	m_cstrUserID = _T("");
	//}}AFX_DATA_INIT
}


void KDialogIDDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(KDialogIDDlg)
    DDX_Text(pDX, IDC_EDIT_USER_ID, m_cstrUserID);
    DDX_Radio(pDX, IDC_LOGINID, m_iNickname);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KDialogIDDlg, CDialog)
	//{{AFX_MSG_MAP(KDialogIDDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KDialogIDDlg message handlers
