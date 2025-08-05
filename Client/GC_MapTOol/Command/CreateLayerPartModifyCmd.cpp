#include ".\createlayerpartmodifycmd.h"

CCreateLayerPartModifyCmd::CCreateLayerPartModifyCmd(void)
{
}

CCreateLayerPartModifyCmd::~CCreateLayerPartModifyCmd(void)
{
}

void CCreateLayerPartModifyCmd::Undo()
{
	g_LayerToolDlg->SetClonePart(&m_OldPart);
	g_LayerToolDlg->UpdateControlData();
	g_LayerToolDlg->OnPaint();

	std::stringstream stm;
	stm << "Undo] Modify : " << m_OldPart.GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartModifyCmd::Redo()
{
	g_LayerToolDlg->SetClonePart(&m_NewPart);
	g_LayerToolDlg->UpdateControlData();
	g_LayerToolDlg->OnPaint();

	std::stringstream stm;
	stm << "Redo] Modify : " << m_NewPart.GetPartName();
	m_strLog = stm.str();
}

void CCreateLayerPartModifyCmd::SetModifyData(CLayerPart* pOldPart, CLayerPart* pNewPart)
{
	m_OldPart = *pOldPart;
	m_NewPart = *pNewPart;

	std::stringstream stm;
	stm << "Modify : " << m_OldPart.GetPartName();
	m_strLog = stm.str();
}