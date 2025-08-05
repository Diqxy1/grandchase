// ParticleListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "ParticleListDlg.h"
#include "LayerManager.h"
#include "LayerList.h"
#include ".\particlelistdlg.h"
#include "EffectProp.h"
//#include <sstream>

// CParticleListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CParticleListDlg, CDialog)
CParticleListDlg::CParticleListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleListDlg::IDD, pParent)
{
	m_pCloneLayerPart = NULL;
}

CParticleListDlg::~CParticleListDlg()
{
}

void CParticleListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD_PARTICLE, m_kAddButton);
	DDX_Control(pDX, IDC_BUTTON_DELETE_PARTICLE, m_kDeleteButton);
	DDX_Control(pDX, IDC_LIST_PARTICLE, m_kListParticle);
}

void CParticleListDlg::SetControlValue(CLayerPart* pPart)
{
	m_pCloneLayerPart = pPart;
}

BOOL CParticleListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_kListParticle.InsertColumn(0, "Particle Name", LVCFMT_LEFT, 100);
	m_kListParticle.InsertColumn(1, "X Offset", LVCFMT_LEFT, 50);
	m_kListParticle.InsertColumn(2, "Y Offset", LVCFMT_LEFT, 50);

	UpdateData();

	return TRUE;
}
BEGIN_MESSAGE_MAP(CParticleListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARTICLE, OnBnClickedButtonAddParticle)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PARTICLE, OnBnClickedButtonDeleteParticle)
END_MESSAGE_MAP()

// CParticleListDlg 메시지 처리기입니다.

void CParticleListDlg::OnBnClickedButtonAddParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEffectProp dlg;
	dlg.SetControlValue(m_pCloneLayerPart);
	dlg.DoModal();
	UpdateData();
}

void CParticleListDlg::OnBnClickedButtonDeleteParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = m_kListParticle.GetFirstSelectedItemPosition();

	int iSel = m_kListParticle.GetNextSelectedItem(pos);
	m_kListParticle.DeleteItem(iSel);
	m_pCloneLayerPart->DeleteParticle(iSel);
}

BOOL CParticleListDlg::UpdateData(BOOL bSaveAndValidate /*= TRUE */)
{
	BOOL ret = CWnd::UpdateData(bSaveAndValidate);

	if (m_kListParticle.GetItemCount())
	{
		m_kListParticle.DeleteAllItems();
	}
	for (unsigned int i = 0; i < m_pCloneLayerPart->GetParticleCount(); i++)
	{
		int nItem = m_kListParticle.GetItemCount();
		m_kListParticle.InsertItem(nItem, m_pCloneLayerPart->GetParticleData(i).strParticleName.c_str(), 0);
		std::stringstream str;
		str << m_pCloneLayerPart->GetParticleData(i).fOffsetX;
		m_kListParticle.SetItemText(nItem, 1, str.str().c_str());
		str.str("");
		str << m_pCloneLayerPart->GetParticleData(i).fOffsetY;
		m_kListParticle.SetItemText(nItem, 2, str.str().c_str());
	}

	return ret;
}