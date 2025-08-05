// MoveObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "MoveObjectDlg.h"
#include ".\moveobjectdlg.h"
//#include <sstream>

// CMoveObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMoveObjectDlg, CDialog)
CMoveObjectDlg::CMoveObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveObjectDlg::IDD, pParent)
	, m_strLayerNum(_T("0"))
	, m_strCurrentLayerNum(_T("0"))
{
}

CMoveObjectDlg::~CMoveObjectDlg()
{
}

void CMoveObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LAYER_NUMBER, m_strCurrentLayerNum);
	DDX_Text(pDX, IDC_WANT_MOVE, m_strLayerNum);
}

BEGIN_MESSAGE_MAP(CMoveObjectDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CMoveObjectDlg 메시지 처리기입니다.

void CMoveObjectDlg::OnBnClickedOk()
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

INT_PTR CMoveObjectDlg::DoModal()
{
	std::stringstream str;
	str << g_AppFormView->GetLayerIndex() << " 입니다.";
	m_strCurrentLayerNum = str.str().c_str();
	std::stringstream strNum;
	strNum << g_AppFormView->GetLayerIndex();
	m_strLayerNum = strNum.str().c_str();

	return CDialog::DoModal();
}