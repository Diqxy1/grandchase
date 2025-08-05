// FootHoldValue.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "FootHoldValue.h"
//#include <sstream>
#include "ToolObject.h"
#include "Command/CommandRecv.h"
#include "Command/ModifyFHValueCmd.h"

// CFootHoldValue 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFootHoldValue, CDialog)
CFootHoldValue::CFootHoldValue(CWnd* pParent /*=NULL*/)
	: CDialog(CFootHoldValue::IDD, pParent)
	, m_strFHValue(_T(""))
{
}

CFootHoldValue::~CFootHoldValue()
{
}

void CFootHoldValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FHVALUE, m_eFHValue);
	DDX_Text(pDX, IDC_FHVALUE, m_strFHValue);
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
}

BEGIN_MESSAGE_MAP(CFootHoldValue, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
END_MESSAGE_MAP()

// CFootHoldValue 메시지 처리기입니다.

void CFootHoldValue::OnBnClickedOk()
{
	return;
	OnOK();
}

void CFootHoldValue::OnBnClickedCancel()
{
	return;
	OnCancel();
}

void CFootHoldValue::OnBnClickedApply()
{
	UpdateData();

	if (g_AppFormView->m_vecCurObj.empty())
		return;

	std::string strValue = m_strFHValue.GetBuffer();
	if (strValue.compare("") == 0)
		return;

	std::vector< CToolObject* > vecHead;
	std::vector< CToolObject* >::iterator vit;
	for (vit = g_AppFormView->m_vecCurObj.begin(); vit != g_AppFormView->m_vecCurObj.end(); ++vit)
	{
		CToolObject* pObj = (*vit);
		CToolObject* pObjHead = &g_AppFormView->m_ppTObj[pObj->GetIndexX()][pObj->GetIndexY()];
		if (pObjHead != NULL && pObjHead->GetType() == CFootHold::FHT_HEAD)
		{
			vecHead.push_back(pObjHead);
		}
	}

	std::stringstream stm;
	stm << "현재 선택되어 있는 발판 : " << (int)vecHead.size() << " 개의 속성을\n"
		<< "\"" << strValue << "\" 으로 하시겠습니까?";

	int nRet = AfxMessageBox(stm.str().c_str(), MB_YESNO);
	switch (nRet)
	{
	case IDYES:
	{
		// 적용
		VecFootHoldValue vecUndo;
		VecFootHoldValue vecRedo;
		std::vector< CToolObject* >::iterator vit;
		for (vit = vecHead.begin(); vit != vecHead.end(); ++vit)
		{
			std::pair<int, int> indexPair = std::make_pair((*vit)->GetIndexX(), (*vit)->GetIndexY());
			vecUndo.push_back(std::make_pair(indexPair, (*vit)->GetValue()));
			(*vit)->SetValue(strValue);
			vecRedo.push_back(std::make_pair(indexPair, strValue));
		}
		CModifyFHValueCmd* pCmd = new CModifyFHValueCmd();
		pCmd->SetUndoData(vecUndo);
		pCmd->SetRedoData(vecRedo);
		g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
		g_AppFormView->AddLog(pCmd->GetLogStr());
	}
	break;
	}
}

void CFootHoldValue::DisableAllProperty(BOOL bDisable/* = TRUE*/)
{
	if (bDisable == TRUE)
	{
		m_strFHValue = "";
		UpdateData(FALSE);
	}
	m_eFHValue.EnableWindow(!bDisable);
	m_btnApply.EnableWindow(!bDisable);
}

void CFootHoldValue::SetValue(const std::vector< CToolObject* >& vecObj)
{
	if (vecObj.size() == 1)
	{
		std::vector< CToolObject* >::const_iterator vit;
		vit = vecObj.begin();
		CToolObject* pObj = (*vit);
		CToolObject* pObjHead = &g_AppFormView->m_ppTObj[pObj->GetIndexX()][pObj->GetIndexY()];
		if (pObjHead != NULL)
		{
			m_strFHValue = pObjHead->GetValue().c_str();
			UpdateData(FALSE);
		}
	}
	else
	{
		DisableAllProperty();
	}
}