// ResizingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "ResizingDlg.h"
#include ".\resizingdlg.h"
//#include <string>
//#include <sstream>

// CResizingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResizingDlg, CDialog)
CResizingDlg::CResizingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizingDlg::IDD, pParent)
	, m_strWidth(_T("0"))
	, m_strHeight(_T("0"))
{
	std::stringstream str;
	str << g_AppFormView->ResizedX;
	m_strWidth = str.str().c_str();
	std::stringstream str2;
	str2 << g_AppFormView->ResizedY;
	m_strHeight = str2.str().c_str();
}

CResizingDlg::~CResizingDlg()
{
}

void CResizingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WORLD_WIDTH, m_strWidth);
	DDX_Text(pDX, IDC_WORLD_HEIGHT, m_strHeight);
}

BEGIN_MESSAGE_MAP(CResizingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CResizingDlg 메시지 처리기입니다.

void CResizingDlg::OnBnClickedOk()
{
	UpdateData();

	std::stringstream str;
	str << m_strWidth.GetBuffer();
	str << " ";
	str << m_strHeight.GetBuffer();
	int iWidth;
	str >> iWidth;
	int iHeight;
	str >> iHeight;

	if (iWidth == 0 || iHeight == 0)
	{
		AfxMessageBox("사이즈를 0이상으로 해주세요");
		return;
	}

	g_AppFormView->ResizedX = iWidth;
	g_AppFormView->ResizedY = iHeight;

	OnOK();
}