#include ".\ModifyLayerPartCmd.h"

CModifyLayerPartCmd::CModifyLayerPartCmd(void)
	: m_pOrigPart(NULL)
	, m_pBackPart(NULL)
{
}

CModifyLayerPartCmd::~CModifyLayerPartCmd(void)
{
	SAFE_DELETE(m_pBackPart);
}

void CModifyLayerPartCmd::Undo()
{
	if (m_pOrigPart != NULL && m_pBackPart != NULL)
	{
		CLayerPart* pPartBack = m_pOrigPart->Clone();
		*m_pOrigPart = *m_pBackPart;
		SAFE_DELETE(m_pBackPart);
		m_pBackPart = pPartBack;
		std::stringstream stm;
		stm << "Undo] Modify " << m_pBackPart->GetLayer() << " Layer : " << m_pBackPart->GetObjectName()
			<< "( " << m_pBackPart->GetZOrder() << " )";
		m_strLog = stm.str();
		g_AppFormView->SetControlValue(m_pOrigPart);
	}
}

void CModifyLayerPartCmd::Redo()
{
	if (m_pOrigPart != NULL && m_pBackPart != NULL)
	{
		CLayerPart* pPartBack = m_pOrigPart->Clone();
		*m_pOrigPart = *m_pBackPart;
		SAFE_DELETE(m_pBackPart);
		m_pBackPart = pPartBack;
		std::stringstream stm;
		stm << "Redo] Modify " << m_pBackPart->GetLayer() << " Layer : " << m_pBackPart->GetObjectName()
			<< "( " << m_pBackPart->GetZOrder() << " )";
		m_strLog = stm.str();
		g_AppFormView->SetControlValue(m_pOrigPart);
	}
}

void CModifyLayerPartCmd::StoreOrigPart(CLayerPart* pPart)
{
	m_pBackPart = pPart->Clone();
	std::stringstream stm;
	stm << "Modify " << m_pBackPart->GetLayer() << " Layer : " << m_pBackPart->GetObjectName()
		<< "( " << m_pBackPart->GetZOrder() << " )";
	m_strLog = stm.str();
}