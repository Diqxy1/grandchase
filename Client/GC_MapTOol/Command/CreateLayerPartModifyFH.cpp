#include "stdafx.h"
#include ".\createlayerpartmodifyfh.h"

CCreateLayerPartModifyFH::CCreateLayerPartModifyFH(void)
	: m_pPart(NULL)
	, m_iNewWidth(0)
	, m_iNewHeight(0)
	, m_iOldWidth(0)
	, m_iOldHeight(0)
{
	m_vecToolObject.clear();
}

CCreateLayerPartModifyFH::~CCreateLayerPartModifyFH(void)
{
	m_vecToolObject.clear();
}

void CCreateLayerPartModifyFH::Undo()
{
	m_pPart->SetFHWidth(m_iOldWidth);
	m_pPart->SetFHHeight(m_iOldHeight);
	m_pPart->SetFootHoldVector(m_vecToolObject);

	std::stringstream stm;
	stm << "Undo] Old FootHold Size : " << m_iOldWidth << " x " << m_iOldHeight
		<< " ( ToolObject Size : " << m_vecToolObject.size() << " )";
	m_strLog = stm.str();
}

void CCreateLayerPartModifyFH::Redo()
{
	m_pPart->SetFHWidth(m_iNewWidth);
	m_pPart->SetFHHeight(m_iNewHeight);
	m_pPart->InitTOVector();

	std::stringstream stm;
	stm << "Redo] New FootHold Size : " << m_iNewWidth << " x " << m_iNewHeight
		<< " ( ToolObject Size : " << m_vecToolObject.size() << " )";
	m_strLog = stm.str();
}

void CCreateLayerPartModifyFH::SetModifyData(int iNewWidth, int iNewHeight, CLayerPart* pPart)
{
	m_iNewWidth = iNewWidth;
	m_iNewHeight = iNewHeight;
	m_pPart = pPart;
	m_iOldWidth = m_pPart->GetSBlockWidth();
	m_iOldHeight = m_pPart->GetSBlockHeight();
	m_vecToolObject = m_pPart->GetToolObjectVector();

	std::stringstream stm;
	stm << "New FootHold Size : " << m_iNewWidth << " x " << m_iNewHeight
		<< " ( ToolObject Size : " << m_vecToolObject.size() << " )";
	m_strLog = stm.str();
}