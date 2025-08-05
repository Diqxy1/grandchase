#include ".\createlayerpartdelpartcmd.h"

CCreateLayerPartDelPartCmd::CCreateLayerPartDelPartCmd(void)
	: m_pDelPart(NULL)
	, m_bNeedDelete(false)
{
}

CCreateLayerPartDelPartCmd::~CCreateLayerPartDelPartCmd(void)
{
	if (m_bNeedDelete)
		SAFE_DELETE(m_pDelPart);
}

void CCreateLayerPartDelPartCmd::Undo()
{
	g_LayerToolDlg->GetLayerManager()->AddLayerPart(m_pDelPart);
	g_LayerToolDlg->SetCategoryMember(m_pDelPart->GetPartCategory());
	g_LayerToolDlg->m_lbPartsList.SetCurSel(g_LayerToolDlg->m_lbPartsList.FindString(-1, m_pDelPart->GetPartName().c_str()));
	g_LayerToolDlg->m_btnAdd.EnableWindow(FALSE);

	g_LayerToolDlg->SetClonePart(m_pDelPart);
	g_LayerToolDlg->UpdateControlData();
	g_LayerToolDlg->OnPaint();

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Undo] Part recovered " << m_pDelPart->GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartDelPartCmd::Redo()
{
	m_bNeedDelete = true;

	CLayerPart* pTempPart = m_pDelPart->Clone();
	g_LayerToolDlg->GetLayerManager()->EraseLayerPart(m_pDelPart);
	m_pDelPart = pTempPart;

	std::vector< std::string > CategoryList = g_LayerToolDlg->GetLayerManager()->GetCategoryList();
	g_LayerToolDlg->SetCategoryMember(CategoryList[g_LayerToolDlg->m_lbCategoryList.GetCurSel()]);
	g_LayerToolDlg->InitControlData();
	g_LayerToolDlg->OnPaint();

	std::stringstream stm;
	stm << "Redo] Part Deleted " << m_pDelPart->GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartDelPartCmd::SetDelPart(CLayerPart* pPart)
{
	// [8/22/2007 breadceo] pPart는 지워진다
	m_pDelPart = pPart->Clone();
	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Part Deleted " << m_pDelPart->GetPartName();
	m_strLog = stm.str();
}