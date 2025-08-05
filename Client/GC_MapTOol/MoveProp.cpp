// MoveProp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "MoveProp.h"
#include ".\moveprop.h"
#include "LayerManager.h"

// CMoveProp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMoveProp, CDialog)
CMoveProp::CMoveProp(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveProp::IDD, pParent)
	, m_bMoveable(FALSE)
	, m_strXSpeed(_T(""))
	, m_strYSpeed(_T(""))
	, m_bPosResetX(FALSE)
	, m_bPosResetY(FALSE)
	, m_bScroolX(FALSE)
	, m_bScroolY(FALSE)
{
	m_fXSpeed = 0;
	m_fYSpeed = 0;
}

CMoveProp::~CMoveProp()
{
}

void CMoveProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MOVEABLE_FALSE, m_bMoveable);
	DDX_Control(pDX, IDC_MOVEABLE_FALSE, m_btnMoveable);
	DDX_Control(pDX, IDC_X_SPEED, m_eXSpeed);
	DDX_Control(pDX, IDC_Y_SPEED, m_eYSpeed);
	DDX_Text(pDX, IDC_X_SPEED, m_strXSpeed);
	DDX_Text(pDX, IDC_Y_SPEED, m_strYSpeed);
	DDX_Control(pDX, IDC_POSITIONRESET_X, m_btnPosResetX);
	DDX_Control(pDX, IDC_POSITIONRESET_Y, m_btnPosResetY);
	DDX_Check(pDX, IDC_POSITIONRESET_X, m_bPosResetX);
	DDX_Check(pDX, IDC_POSITIONRESET_Y, m_bPosResetY);
	DDX_Check(pDX, IDC_SCROOLX, m_bScroolX);
	DDX_Check(pDX, IDC_SCROOLY, m_bScroolY);
}

BEGIN_MESSAGE_MAP(CMoveProp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MOVEABLE_TRUE, OnBnClickedMoveableTrue)
	ON_BN_CLICKED(IDC_MOVEABLE_FALSE, OnBnClickedMoveableFalse)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	//	ON_BN_CLICKED(IDC_SCROOLX, &CMoveProp::OnBnClickedScroolx)
	//	ON_BN_CLICKED(IDC_SCROOLY, &CMoveProp::OnBnClickedScrooly)
END_MESSAGE_MAP()

// CMoveProp 메시지 처리기입니다.

void CMoveProp::OnBnClickedOk()
{
	return;
	OnOK();
}

void CMoveProp::OnBnClickedCancel()
{
	return;
	OnCancel();
}

void CMoveProp::SetControlValue(CLayerPart* pPart)
{
	std::stringstream str;
	bool bMoveable;
	float fSpeedX, fSpeedY;
	pPart->GetMoveProp(bMoveable, fSpeedX, fSpeedY);
	m_bMoveable = bMoveable;
	SETSTR(fSpeedX, m_strXSpeed);
	SETSTR(fSpeedY, m_strYSpeed);
	m_bPosResetX = pPart->GetRepeat_X();
	m_bPosResetY = pPart->GetRepeat_Y();

	if (pPart->GetLayer() == LAST_LAYER - 1 || pPart->GetLayer() == LAST_LAYER - 2 || pPart->GetLayer() == LAST_LAYER - 3)
	{
		m_bScroolX = pPart->GetScroll_X();
		m_bScroolY = pPart->GetScroll_Y();
		GetDlgItem(IDC_SCROOLX)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCROOLY)->EnableWindow(TRUE);
	}
	else
	{
		m_bScroolX = FALSE;
		m_bScroolY = FALSE;
		GetDlgItem(IDC_SCROOLX)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCROOLY)->EnableWindow(FALSE);
	}

	if (bMoveable)
		OnBnClickedMoveableTrue();
	else
		OnBnClickedMoveableFalse();

	UpdateData(FALSE);
}

void CMoveProp::UpdateControlValue()
{
	UpdateData();

	m_fXSpeed = static_cast<float>(strtod(m_strXSpeed, NULL));
	m_fYSpeed = static_cast<float>(strtod(m_strYSpeed, NULL));
}

void CMoveProp::UpdateSelectPart(CLayerPart* pPart)
{
	pPart->SetMoveProp(m_bMoveable, m_fXSpeed, m_fYSpeed);
	pPart->SetRepeat_X(m_bPosResetX);
	pPart->SetRepeat_Y(m_bPosResetY);
	pPart->SetScroll_X(m_bScroolX);
	pPart->SetScroll_Y(m_bScroolY);

	UpdateData(FALSE);
}

void CMoveProp::DisableAllProperty(BOOL bDisable)
{
	GetDlgItem(IDC_MOVEABLE_TRUE)->EnableWindow(!bDisable);
	m_btnMoveable.EnableWindow(!bDisable);
}

void CMoveProp::OnBnClickedMoveableTrue()
{
	m_eXSpeed.EnableWindow();
	m_eYSpeed.EnableWindow();

	m_btnPosResetX.EnableWindow();
	m_btnPosResetY.EnableWindow();
}

void CMoveProp::OnBnClickedMoveableFalse()
{
	m_eXSpeed.EnableWindow(FALSE);
	m_eYSpeed.EnableWindow(FALSE);

	m_btnPosResetX.EnableWindow(FALSE);
	m_btnPosResetY.EnableWindow(FALSE);
}

BOOL CMoveProp::OnInitDialog()
{
	CDialog::OnInitDialog();

	Enable(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMoveProp::Enable(BOOL bEnable)
{
	m_eXSpeed.EnableWindow(bEnable);
	m_eYSpeed.EnableWindow(bEnable);
}

void CMoveProp::OnBnClickedApply()
{
	g_AppFormView->UpdateControlValue();
	g_AppFormView->UpdateSelectPart();
}