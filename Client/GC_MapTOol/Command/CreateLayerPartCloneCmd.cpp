#include "stdafx.h"
#include ".\createlayerpartclonecmd.h"
#include "../KCreateLayerPartDlg.h"
#include "../LayerManager.h"

CCreateLayerPartCloneCmd::CCreateLayerPartCloneCmd(void)
	: m_bNeedDelete(false)
	, m_pClonePart(NULL)
{
}

CCreateLayerPartCloneCmd::~CCreateLayerPartCloneCmd(void)
{
	if (m_bNeedDelete)
		SAFE_DELETE(m_pClonePart);
}

void CCreateLayerPartCloneCmd::Undo()
{
	CLayerPart* pTempPart = m_pClonePart->Clone();
	g_LayerToolDlg->GetLayerManager()->EraseLayerPart(m_pClonePart);
	m_pClonePart = pTempPart;
	g_LayerToolDlg->SetCategoryMember(m_pClonePart->GetPartCategory());
	g_LayerToolDlg->m_lbPartsList.SetCurSel(g_LayerToolDlg->m_lbPartsList.FindString(-1, m_pClonePart->GetPartName().c_str()));
	g_LayerToolDlg->m_btnAdd.EnableWindow(FALSE);

	g_LayerToolDlg->SetClonePart(m_pClonePart);
	g_LayerToolDlg->UpdateControlData();
	g_LayerToolDlg->OnPaint();

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Undo] Part Deleted " << m_pClonePart->GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartCloneCmd::Redo()
{
	g_LayerToolDlg->GetLayerManager()->AddLayerPart(m_pClonePart);
	g_LayerToolDlg->SetCategoryMember(m_pClonePart->GetPartCategory());
	g_LayerToolDlg->m_lbPartsList.SetCurSel(g_LayerToolDlg->m_lbPartsList.FindString(-1, m_pClonePart->GetPartName().c_str()));
	g_LayerToolDlg->m_btnAdd.EnableWindow(FALSE);

	g_LayerToolDlg->SetClonePart(m_pClonePart);
	g_LayerToolDlg->UpdateControlData();
	g_LayerToolDlg->OnPaint();

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Redo] Part Cloned " << m_pClonePart->GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartCloneCmd::SetClonePart(CLayerPart* pPart)
{
	m_pClonePart = pPart;

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Part Cloned " << m_pClonePart->GetPartName();
	m_strLog = stm.str();
}