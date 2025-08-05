// LayerResizing.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "LayerResizing.h"
#include ".\layerresizing.h"
//#include <sstream>

// CLayerResizing 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayerResizing, CDialog)
CLayerResizing::CLayerResizing(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerResizing::IDD, pParent)
	, m_strWidth(_T(""))
	, m_strHeight(_T(""))
	, m_bCheckAll(FALSE)
{
}

CLayerResizing::~CLayerResizing()
{
}

void CLayerResizing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strWidth);
	DDX_Text(pDX, IDC_EDIT2, m_strHeight);
	DDX_Control(pDX, IDC_CHECK_ALL, m_btnAllLayer);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bCheckAll);
}

BEGIN_MESSAGE_MAP(CLayerResizing, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CLayerResizing 메시지 처리기입니다.

void CLayerResizing::OnBnClickedOk()
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

	if (iWidth < 800 || iHeight < 600)
	{
		AfxMessageBox("최소 사이즈는 W:800 H:600 입니다.");
		return;
	}

	OnOK();
}

void CLayerResizing::SetSize(int Width, int Height)
{
	// 귀찮네 -.,-;

	std::stringstream str1, str2;
	str1 << Width;
	m_strWidth = str1.str().c_str();
	str2 << Height;
	m_strHeight = str2.str().c_str();
}

BOOL CLayerResizing::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}