// EffectProp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "EffectProp.h"
#include ".\effectprop.h"
#include "LayerManager.h"
#include "LayerList.h"

// CEffectProp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffectProp, CDialog)
CEffectProp::CEffectProp(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectProp::IDD, pParent)
	, m_strEffectName(_T(""))
	, m_strEffectX(_T(""))
	, m_strEffectY(_T(""))
	, m_iEffectX(0)
	, m_iEffectY(0)
	, m_bUseEffect(FALSE)
{
}

CEffectProp::~CEffectProp()
{
}

void CEffectProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECT_NAME, m_eEffectName);
	DDX_Text(pDX, IDC_EFFECT_NAME, m_strEffectName);
	DDX_Control(pDX, IDC_EFFECT_TERM_X, m_eEffectX);
	DDX_Control(pDX, IDC_EFFECT_TERM_Y, m_eEffectY);
	DDX_Text(pDX, IDC_EFFECT_TERM_X, m_strEffectX);
	DDX_Text(pDX, IDC_EFFECT_TERM_Y, m_strEffectY);
}

BEGIN_MESSAGE_MAP(CEffectProp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
END_MESSAGE_MAP()

// CEffectProp 메시지 처리기입니다.

void CEffectProp::OnBnClickedOk()
{
	if (!m_strEffectName.IsEmpty())
	{
		std::stringstream str;
		str << m_strEffectX.GetBuffer();
		str << " ";
		str << m_strEffectY.GetBuffer();
		float fOffsetX;
		str >> fOffsetX;
		float fOffsetY;
		str >> fOffsetY;

		m_pPartClone->AddParticle(m_strEffectName.GetString(), fOffsetX, fOffsetY);
	}

	OnOK();
}

void CEffectProp::OnBnClickedCancel()
{
	OnCancel();
}

void CEffectProp::SetControlValue(CLayerPart* pPart)
{
	m_pPartClone = pPart;
	//m_bUseEffect = pPart->GetUseEffect();

	//std::stringstream str;

	//m_strEffectName = pPart->GetEffectName().c_str();
	//int EffX, EffY;
	//pPart->GetEffectPos( EffX, EffY );
	//SETSTR( EffX,m_strEffectX );
	//SETSTR( EffY,m_strEffectY );

	//UpdateData( FALSE );
}

void CEffectProp::UpdateControlValue()
{
	UpdateData();

	m_iEffectX = strtol(m_strEffectX, NULL, 0);
	m_iEffectY = strtol(m_strEffectY, NULL, 0);
}

void CEffectProp::UpdateSelectPart(CLayerPart* pPart)
{
	//pPart->SetUseEffect( m_bUseEffect );
	//pPart->CreateChildEffect( m_strEffectName.GetBuffer(), m_iEffectX, m_iEffectY );

	//UpdateData( FALSE );
}

void CEffectProp::DisableAllProperty(BOOL bDisable)
{
	m_btnUseEffect.EnableWindow(!bDisable);
	GetDlgItem(IDC_USE_EFFECT_YES)->EnableWindow(!bDisable);
}

BOOL CEffectProp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_eEffectX.SetWindowText("0");
	m_eEffectY.SetWindowText("0");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEffectProp::OnBnClickedApply()
{
	UpdateControlValue();
	if (!m_strEffectName.IsEmpty())
	{
		std::stringstream str;
		str << m_strEffectX.GetBuffer();
		str << " ";
		str << m_strEffectY.GetBuffer();
		float fOffsetX;
		str >> fOffsetX;
		float fOffsetY;
		str >> fOffsetY;

		m_pPartClone->AddParticle(m_strEffectName.GetString(), fOffsetX, fOffsetY);
	}

	CDialog::OnOK();
}

void CEffectProp::OnOK()
{
	OnBnClickedApply();
}