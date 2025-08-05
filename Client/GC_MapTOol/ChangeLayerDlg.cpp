// ChangeLayerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "ChangeLayerDlg.h"
#include ".\changelayerdlg.h"
//#include <sstream>

// CChangeLayerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChangeLayerDlg, CDialog)
CChangeLayerDlg::CChangeLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeLayerDlg::IDD, pParent)
	, m_strLayerNum(_T("0"))
	, m_strCurrentLayerNum(_T("0"))
{
}

CChangeLayerDlg::~CChangeLayerDlg()
{
}

void CChangeLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CURRENT_LAYER, m_strCurrentLayerNum);
	DDX_Text(pDX, IDC_WANT_CHANGE, m_strLayerNum);
}

BEGIN_MESSAGE_MAP(CChangeLayerDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CChangeLayerDlg 메시지 처리기입니다.

INT_PTR CChangeLayerDlg::DoModal()
{
	std::stringstream str;
	str << g_AppFormView->GetLayerIndex() << " 입니다.";
	m_strCurrentLayerNum = str.str().c_str();
	std::stringstream strNum;
	strNum << g_AppFormView->GetLayerIndex();
	m_strLayerNum = strNum.str().c_str();

	return CDialog::DoModal();
}

void CChangeLayerDlg::OnBnClickedOk()
{
	UpdateData();

	int ChangeLayerNum = strtol(m_strLayerNum, NULL, 0);

	if (g_AppFormView->GetLayerIndex() != ChangeLayerNum)
		g_AppFormView->SetChangeLayerIndex(ChangeLayerNum);
	else
	{
		AfxMessageBox("바꾸려는 인덱스가 같습니다 -_-+\n");
		return;
	}

	OnOK();
}