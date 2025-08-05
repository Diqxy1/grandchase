#include ".\layerzordercmd.h"
#include "../LayerList.h"
#include "../LayerManager.h"

CLayerZOrderCmd::CLayerZOrderCmd(void)
	: m_pPart(NULL)
	, m_iCurZOrder(0)
	, m_iAfterZOrder(0)
{
}

CLayerZOrderCmd::~CLayerZOrderCmd(void)
{
}

void CLayerZOrderCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->EraseLayerObject(m_pPart);
	vecLayer[m_iLayerIndex]->ResetZOrder();
	vecLayer[m_iLayerIndex]->SortByZOrder();
	m_pPart->SetZOrder(m_iCurZOrder);
	vecLayer[m_iLayerIndex]->AddLayerPartIndex(m_pPart, m_iCurZOrder);
	vecLayer[m_iLayerIndex]->SortByZOrder();

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	std::stringstream stm;
	stm << "Undo] ZOrder Changed " << m_iLayerIndex << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_iAfterZOrder << " ) to " << m_iCurZOrder;
	m_strLog = stm.str();
}

void CLayerZOrderCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->EraseLayerObject(m_pPart);
	vecLayer[m_iLayerIndex]->ResetZOrder();
	vecLayer[m_iLayerIndex]->SortByZOrder();
	m_pPart->SetZOrder(m_iAfterZOrder);
	vecLayer[m_iLayerIndex]->AddLayerPartIndex(m_pPart, m_iAfterZOrder);
	vecLayer[m_iLayerIndex]->SortByZOrder();

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	std::stringstream stm;
	stm << "Redo] ZOrder Changed " << m_iLayerIndex << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_iCurZOrder << " ) to " << m_iAfterZOrder;
	m_strLog = stm.str();
}

void CLayerZOrderCmd::SetZOrder(int iAfterZOrder, CLayerPart* pPart)
{
	m_pPart = pPart;
	m_iLayerIndex = m_pPart->GetLayer();
	m_iCurZOrder = pPart->GetZOrder();
	m_iAfterZOrder = iAfterZOrder;

	std::stringstream stm;
	stm << "ZOrder Changed " << m_iLayerIndex << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_iCurZOrder << " ) to " << m_iAfterZOrder;
	m_strLog = stm.str();
}