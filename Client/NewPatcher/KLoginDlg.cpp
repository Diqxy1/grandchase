// KLoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "KLoginDlg.h"
#include ".\klogindlg.h"
#include "KGCStringLoader.h"

// KLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLoginDlg, CDialog)
KLoginDlg::KLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KLoginDlg::IDD, pParent)
    , m_kLoginID(_T(""))
{
}

KLoginDlg::~KLoginDlg()
{
}

void KLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LOGINID, m_kLoginID);
}


BEGIN_MESSAGE_MAP(KLoginDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// KLoginDlg 메시지 처리기입니다.

void KLoginDlg::OnBnClickedOk()
{
    UpdateData();
    OnOK();
}

char* KLoginDlg::GetID()
{
    return (char*)GCStrWideToChar( m_kLoginID.GetBuffer() );
}
void KLoginDlg::OnBnClickedCancel()
{
    OnCancel();
}
