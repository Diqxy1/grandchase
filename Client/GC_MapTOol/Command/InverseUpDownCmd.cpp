#include ".\InverseUpDownCmd.h"

CInverseUpDownCmd::CInverseUpDownCmd(void)
	: m_pPart(NULL)
{
}

CInverseUpDownCmd::~CInverseUpDownCmd(void)
{
}

void CInverseUpDownCmd::Undo()
{
	m_pPart->Inverse(false);
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Undo] 상하반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CInverseUpDownCmd::Redo()
{
	m_pPart->Inverse(false);
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Redo] 상하반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CInverseUpDownCmd::SetPart(CLayerPart* pPart)
{
	m_pPart = pPart;

	std::stringstream stm;
	stm << "상하반전 " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}