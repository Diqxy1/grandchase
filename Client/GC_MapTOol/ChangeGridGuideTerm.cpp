// ChangeGridGuideTerm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "ChangeGridGuideTerm.h"
#include ".\changegridguideterm.h"
#include "./GridGuideline/KGridGuideMgr.h"
#include "./KGCMap.h"

// CChangeGridGuideTerm 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChangeGridGuideTerm, CDialog)
CChangeGridGuideTerm::CChangeGridGuideTerm(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeGridGuideTerm::IDD, pParent)
	, m_strWantChange(_T(""))
	, m_strChangeTerm(_T(""))
{
}

CChangeGridGuideTerm::~CChangeGridGuideTerm()
{
}

void CChangeGridGuideTerm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHANGE_TERM, m_strChangeTerm);
	DDX_Control(pDX, IDC_WANT_CHANGE_TERM, m_eWantChange);
	DDX_Text(pDX, IDC_WANT_CHANGE_TERM, m_strWantChange);
}

BEGIN_MESSAGE_MAP(CChangeGridGuideTerm, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CChangeGridGuideTerm 메시지 처리기입니다.

void CChangeGridGuideTerm::OnBnClickedCancel()
{
	OnCancel();
}

void CChangeGridGuideTerm::OnBnClickedOk()
{
	UpdateData();

	int iWantTerm = strtol(m_strWantChange, NULL, 0);

	if (SiKGridGuideMgr()->GetGuideTerm() == iWantTerm * FOOTHOLDSIZE)
	{
		AfxMessageBox("바꾸려는 인덱스가 같습니다 -_-+\n");
		return;
	}

	SiKGridGuideMgr()->SetGuideTerm(iWantTerm);
	OnOK();
}

BOOL CChangeGridGuideTerm::OnInitDialog()
{
	std::stringstream str;
	str << static_cast<int>(SiKGridGuideMgr()->GetGuideTerm() / FOOTHOLDSIZE) << " 입니다.";
	m_strChangeTerm = str.str().c_str();
	std::stringstream strNum;
	strNum << static_cast<int>(SiKGridGuideMgr()->GetGuideTerm() / FOOTHOLDSIZE);
	m_strWantChange = strNum.str().c_str();

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}