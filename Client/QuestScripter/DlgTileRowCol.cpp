// DlgTileRowCol.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTileRowCol.h"
#include ".\dlgtilerowcol.h"

// CDlgTileRowCol 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTileRowCol, CDialog)
CDlgTileRowCol::CDlgTileRowCol(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTileRowCol::IDD, pParent)
{
	m_iRow = 0;
	m_iCol = 0;
}

CDlgTileRowCol::~CDlgTileRowCol()
{
}

void CDlgTileRowCol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_ebRow);
	DDX_Control(pDX, IDC_EDIT2, m_ebCol);

	CString strRow;
	CString strCol;
	strRow.Format(L"%d", m_iRow);
	strCol.Format(L"%d", m_iCol);
	m_ebRow.SetWindowText(strRow);
	m_ebCol.SetWindowText(strCol);
}

BEGIN_MESSAGE_MAP(CDlgTileRowCol, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

// CDlgTileRowCol 메시지 처리기입니다.

void CDlgTileRowCol::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strRow;
	CString strCol;
	m_ebRow.GetWindowText(strRow);
	m_ebCol.GetWindowText(strCol);
	m_iRow = _wtoi(strRow);
	m_iCol = _wtoi(strCol);

	OnOK();
}

void CDlgTileRowCol::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}