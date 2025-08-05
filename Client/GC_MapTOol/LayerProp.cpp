// LayerProp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "LayerProp.h"
#include ".\LayerProp.h"
#include "LayerManager.h"

// CLayerProp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayerProp, CDialog)
CLayerProp::CLayerProp(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerProp::IDD, pParent)
	, m_strLayerX(_T(""))
	, m_strLayerY(_T(""))
	, m_strLayerWidth(_T(""))
	, m_strLayerHeight(_T(""))
{
	m_fLayerX = 0;
	m_fLayerY = 0;
	m_fLayerWidth = 0;
	m_fLayerHeight = 0;
}

CLayerProp::~CLayerProp()
{
}

void CLayerProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYER_X, m_eLayerX);
	DDX_Control(pDX, IDC_LAYER_Y, m_eLayerY);
	DDX_Control(pDX, IDC_LAYER_WIDTH, m_eLayerWidth);
	DDX_Control(pDX, IDC_LAYER_HEIGHT, m_eLayerHeight);
	DDX_Text(pDX, IDC_LAYER_X, m_strLayerX);
	DDX_Text(pDX, IDC_LAYER_Y, m_strLayerY);
	DDX_Text(pDX, IDC_LAYER_WIDTH, m_strLayerWidth);
	DDX_Text(pDX, IDC_LAYER_HEIGHT, m_strLayerHeight);
}

BEGIN_MESSAGE_MAP(CLayerProp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
END_MESSAGE_MAP()

// CLayerProp 메시지 처리기입니다.

void CLayerProp::OnBnClickedOk()
{
	return;
	OnOK();
}

void CLayerProp::OnBnClickedCancel()
{
	return;
	OnCancel();
}

void CLayerProp::SetControlValue(CLayerPart* pPart)
{
	KLayerRect<float> kRect;
	pPart->GetLayerRect(kRect);

	std::stringstream str;
	SETSTR(kRect.m_X, m_strLayerX);
	SETSTR(kRect.m_Y, m_strLayerY);
	SETSTR(kRect.m_Width, m_strLayerWidth);
	SETSTR(kRect.m_Height, m_strLayerHeight);

	UpdateData(FALSE);
}

void CLayerProp::UpdateControlValue()
{
	UpdateData();

	m_fLayerX = static_cast<float>(strtod(m_strLayerX, NULL));
	m_fLayerY = static_cast<float>(strtod(m_strLayerY, NULL));
	m_fLayerWidth = static_cast<float>(strtod(m_strLayerWidth, NULL));
	m_fLayerHeight = static_cast<float>(strtod(m_strLayerHeight, NULL));
}

void CLayerProp::UpdateSelectPart(CLayerPart* pPart)
{
	KLayerRect<float> kRect;
	pPart->GetLayerRect(kRect);
	pPart->SetLayerPos(KLayerRect<float>(m_fLayerX, m_fLayerY, kRect.m_Width, kRect.m_Height));
	pPart->SetVertex();

	UpdateData(FALSE);
}

void CLayerProp::DisableAllProperty(BOOL bDisable)
{
	m_eLayerX.EnableWindow(!bDisable);
	m_eLayerY.EnableWindow(!bDisable);
	//m_eLayerWidth.EnableWindow( !bDisable );
	//m_eLayerHeight.EnableWindow( !bDisable );
}

void CLayerProp::OnBnClickedApply()
{
	g_AppFormView->UpdateControlValue();
	g_AppFormView->UpdateSelectPart();
}