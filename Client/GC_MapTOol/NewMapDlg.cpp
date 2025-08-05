// NewMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "NewMapDlg.h"
#include ".\newmapdlg.h"
//#include <sstream>
//#include <string>
#include "KGCMap.h"

// CNewMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewMapDlg, CDialog)
CNewMapDlg::CNewMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMapDlg::IDD, pParent)
	, m_strWidth(_T("0"))
	, m_strHeight(_T("0"))
{
}

CNewMapDlg::~CNewMapDlg()
{
}

void CNewMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WORLD_WIDTH, m_strWidth);
	DDX_Text(pDX, IDC_WORLD_HEIGHT, m_strHeight);
}

BEGIN_MESSAGE_MAP(CNewMapDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CNewMapDlg 메시지 처리기입니다.

void CNewMapDlg::OnBnClickedOk()
{
	UpdateData();

	g_AppFormView->DeleteDungeon();

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

	SiKGCMap()->SetWorld(iWidth, iHeight);

	OnOK();
}