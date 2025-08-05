#include ".\dragpartcmd.h"
#include "../LayerPart.h"

CDragPartCmd::CDragPartCmd(void)
{
}

CDragPartCmd::~CDragPartCmd(void)
{
}

void CDragPartCmd::Undo()
{
	KLayerRect<float> rt;
	m_pDragPart->GetLayerRect(rt);
	rt.m_X = static_cast<float>(m_kDragData.m_iDragStartPosX);
	rt.m_Y = static_cast<float>(m_kDragData.m_iDragStartPosY);
	m_pDragPart->SetLayerPos(rt);
	m_pDragPart->SetVertex();

	g_AppFormView->SetControlValue(m_pDragPart);

	std::stringstream stm;
	stm << "Undo] " << m_pDragPart->GetLayer() << " Layer : " << m_pDragPart->GetObjectName()
		<< "( " << m_pDragPart->GetZOrder() << " )" << " Move from "
		<< m_kDragData.m_iDragEndPosX << ", " << m_kDragData.m_iDragEndPosY << " to "
		<< m_kDragData.m_iDragStartPosX << ", " << m_kDragData.m_iDragStartPosY;
	m_strLog = stm.str();
}

void CDragPartCmd::Redo()
{
	KLayerRect<float> rt;
	m_pDragPart->GetLayerRect(rt);
	rt.m_X = static_cast<float>(m_kDragData.m_iDragEndPosX);
	rt.m_Y = static_cast<float>(m_kDragData.m_iDragEndPosX);
	m_pDragPart->SetLayerPos(rt);
	m_pDragPart->SetVertex();

	g_AppFormView->SetControlValue(m_pDragPart);

	std::stringstream stm;
	stm << "Redo] " << m_pDragPart->GetLayer() << " Layer : " << m_pDragPart->GetObjectName()
		<< "( " << m_pDragPart->GetZOrder() << " )" << " Move from "
		<< m_kDragData.m_iDragStartPosX << ", " << m_kDragData.m_iDragStartPosY << " to "
		<< m_kDragData.m_iDragEndPosX << ", " << m_kDragData.m_iDragEndPosY;
	m_strLog = stm.str();
}

void CDragPartCmd::SetDragData(CLayerPart* pPart, const DragPosData& kDragData)
{
	m_pDragPart = pPart;
	m_kDragData = kDragData;

	std::stringstream stm;
	stm << m_pDragPart->GetLayer() << " Layer : " << m_pDragPart->GetObjectName()
		<< "( " << m_pDragPart->GetZOrder() << " )" << " Move from "
		<< m_kDragData.m_iDragStartPosX << ", " << m_kDragData.m_iDragStartPosY << " to "
		<< m_kDragData.m_iDragEndPosX << ", " << m_kDragData.m_iDragEndPosY;
	m_strLog = stm.str();
}