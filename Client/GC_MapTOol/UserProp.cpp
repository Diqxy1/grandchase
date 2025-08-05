// UserProp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "UserProp.h"
#include ".\userprop.h"
#include "LayerManager.h"
#include "LayerList.h"

// CUserProp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserProp, CDialog)
CUserProp::CUserProp(CWnd* pParent /*=NULL*/)
	: CDialog(CUserProp::IDD, pParent)
	, m_strObjectName(_T(""))
	, m_strUserValue(_T(""))
	, m_csUserString(_T(""))
{
	m_iUserValue = 0;
}

CUserProp::~CUserProp()
{
}

void CUserProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_UNIQUE_NAME, m_eObjectName);
	DDX_Text(pDX, IDC_OBJECT_UNIQUE_NAME, m_strObjectName);
	DDX_Control(pDX, IDC_OBJECT_NAME_VERIFY, m_btnObjectNameVerify);
	DDX_Control(pDX, IDC_USERVALUE, m_eUserValue);
	DDX_Text(pDX, IDC_USERVALUE, m_strUserValue);
	DDX_Control(pDX, IDC_USERSTRING, m_eUserString);
	DDX_Text(pDX, IDC_USERSTRING, m_csUserString);
}

BEGIN_MESSAGE_MAP(CUserProp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_OBJECT_NAME_VERIFY, OnBnClickedObjectNameVerify)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
END_MESSAGE_MAP()

// CUserProp 메시지 처리기입니다.

void CUserProp::OnBnClickedOk()
{
	// 항상 띄우고 싶어혀
	return;
	OnOK();
}

void CUserProp::OnBnClickedCancel()
{
	// 항상 띄우고 싶어혀
	return;
	OnCancel();
}

void CUserProp::OnBnClickedObjectNameVerify()
{
	int iCurSel;
	CLayerPart* pPart = g_AppFormView->GetLayerList()->GetCurItemData(&iCurSel);

	if (pPart == NULL)
		return;

	UpdateData();
	m_strUserString = m_csUserString.GetBuffer();

	std::string strObjName = m_strObjectName.GetBuffer();
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
	{
		std::vector< CLayerPart* >& PartVector = vecLayer[i]->GetPartVector();
		for (int j = 0; j < (int)PartVector.size(); ++j)
		{
			if (PartVector[j] == pPart && i == g_AppFormView->GetLayerIndex())
				continue;

			if (PartVector[j]->GetObjectName() == strObjName)
			{
				AfxMessageBox("중복되는 이름이 있습니다.\n중복안되게 해주세요~~\n");
			}
		}
	}
	AfxMessageBox("중복되는 이름이 없습니다.\n사용할 수 있어혀~~\n");
}

void CUserProp::DisableAllProperty(BOOL bDisable)
{
	m_eObjectName.EnableWindow(!bDisable);
	m_btnObjectNameVerify.EnableWindow(!bDisable);
	m_eUserValue.EnableWindow(!bDisable);
	m_eUserString.EnableWindow(!bDisable);
}

void CUserProp::SetControlValue(CLayerPart* pPart)
{
	std::stringstream str;

	SETSTR(pPart->GetUserValue(), m_strUserValue);
	m_csUserString = pPart->GetUserString().c_str();
	m_strObjectName = pPart->GetObjectName().c_str();

	UpdateData(FALSE);
}

void CUserProp::UpdateSelectPart(CLayerPart* pPart)
{
	pPart->SetObjectName(m_strObjectName.GetBuffer());
	pPart->SetUserString(m_strUserString);
	pPart->SetUserValue(m_iUserValue);

	UpdateData(FALSE);
}

void CUserProp::UpdateControlValue()
{
	UpdateData();

	m_iUserValue = strtol(m_strUserValue, NULL, 0);
	m_strUserString = m_csUserString.GetBuffer();
}

void CUserProp::OnBnClickedApply()
{
	int iCurSel;
	CLayerPart* pPart = g_AppFormView->GetLayerList()->GetCurItemData(&iCurSel);

	if (pPart == NULL)
		return;

	g_AppFormView->UpdateControlValue();
	g_AppFormView->UpdateSelectPart();
	std::stringstream str;
	str << pPart->GetFileName() << " : " << m_strObjectName.GetBuffer() << " (" << pPart->GetZOrder() << ")";
	g_AppFormView->GetLayerList()->GetObjectControl()->SetItemText(iCurSel, 0, str.str().c_str());
	g_AppFormView->GetLayerList()->GetObjectControl()->SetItemState(iCurSel, LVIS_SELECTED, LVIS_SELECTED);
}