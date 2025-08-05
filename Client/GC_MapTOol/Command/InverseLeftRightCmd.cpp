#include ".\inverseleftrightcmd.h"

CInverseLeftRightCmd::CInverseLeftRightCmd(void)
	: m_pPart(NULL)
{
}

CInverseLeftRightCmd::~CInverseLeftRightCmd(void)
{
}

void CInverseLeftRightCmd::Undo()
{
	m_pPart->Inverse(true);
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Undo] 좌우반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CInverseLeftRightCmd::Redo()
{
	m_pPart->Inverse(true);
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Redo] 좌우반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CInverseLeftRightCmd::SetPart(CLayerPart* pPart)
{
	m_pPart = pPart;

	std::stringstream stm;
	stm << "좌우반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}