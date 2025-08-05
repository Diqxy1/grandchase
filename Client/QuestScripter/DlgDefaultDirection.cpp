// DlgDefaultDirection.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgDefaultDirection.h"
#include ".\dlgdefaultdirection.h"
#include "struct.h"

// CDlgDefaultDirection 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDefaultDirection, CDialog)
CDlgDefaultDirection::CDlgDefaultDirection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefaultDirection::IDD, pParent)
{
	m_iDirection = 0;
	m_bDirectionMode = 0;
}

CDlgDefaultDirection::~CDlgDefaultDirection()
{
}

void CDlgDefaultDirection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_RIGHT, m_btRight);
	DDX_Control(pDX, IDC_CHECK_DOWN, m_btBottom);
	DDX_Control(pDX, IDC_CHECK_LEFT, m_btLeft);
	DDX_Control(pDX, IDC_CHECK_UP, m_bttop);
	DDX_Control(pDX, IDC_RADIO_DEL, m_rdDel);
	DDX_Control(pDX, IDC_RADIO_DIRECTION, m_rdDirection);
}

BEGIN_MESSAGE_MAP(CDlgDefaultDirection, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_DIRECTION, OnBnClickedRadioDirection)
	ON_BN_CLICKED(IDC_RADIO_DEL, OnBnClickedRadioDel)
END_MESSAGE_MAP()

// CDlgDefaultDirection 메시지 처리기입니다.

void CDlgDefaultDirection::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iRight = 0x00000000;
	int iBottom = 0x00000000;
	int iLeft = 0x00000000;
	int iTop = 0x00000000;

	if (m_btRight.GetCheck())
	{
		iRight = 0x00000001;
		iRight <<= 0;  // 1
	}
	if (m_btBottom.GetCheck())
	{
		iBottom = 0x00000001;
		iBottom <<= 1;  // 2
	}
	if (m_btLeft.GetCheck())
	{
		iLeft = 0x00000001;
		iLeft <<= 2;  // 4
	}
	if (m_bttop.GetCheck())
	{
		iTop = 0x00000001;
		iTop <<= 3;  // 8
	}

	m_iDirection = iRight | iBottom | iLeft | iTop;
	m_bDirectionMode = m_rdDirection.GetCheck();

	OnOK();
}

void CDlgDefaultDirection::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CDlgDefaultDirection::OnBnClickedRadioDirection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_btRight.EnableWindow(true);
	m_btBottom.EnableWindow(true);
	m_btLeft.EnableWindow(true);
	m_bttop.EnableWindow(true);
}

void CDlgDefaultDirection::OnBnClickedRadioDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_btRight.SetCheck(0);
	m_btBottom.SetCheck(0);
	m_btLeft.SetCheck(0);
	m_bttop.SetCheck(0);

	m_btRight.EnableWindow(false);
	m_btBottom.EnableWindow(false);
	m_btLeft.EnableWindow(false);
	m_bttop.EnableWindow(false);
}

BOOL CDlgDefaultDirection::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_rdDel.SetCheck(0);
	m_rdDirection.SetCheck(true);
	m_bDirectionMode = m_rdDirection.GetCheck();

	m_btRight.SetCheck(0);
	m_btBottom.SetCheck(0);
	m_btLeft.SetCheck(0);
	m_bttop.SetCheck(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}