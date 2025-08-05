#include ".\layerpartdelcmd.h"
//#include <sstream>
#include "../LayerManager.h"
#include "../LayerList.h"

CLayerPartDelCmd::CLayerPartDelCmd(void)
	: m_pDeletePart(NULL)
	, m_bNeedDelete(false)
{
}

CLayerPartDelCmd::~CLayerPartDelCmd(void)
{
	if (m_bNeedDelete)
		SAFE_DELETE(m_pDeletePart);
}

void CLayerPartDelCmd::Undo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->AddLayerPartIndex(m_pDeletePart, m_iZOrder);
	vecLayer[m_iLayerIndex]->SortByZOrder();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Undo] Restored " << m_iLayerIndex << " Layer : " << m_pDeletePart->GetObjectName()
		<< "( " << m_pDeletePart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CLayerPartDelCmd::Redo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->EraseLayerObject(m_pDeletePart);
	vecLayer[m_iLayerIndex]->ResetZOrder(m_iZOrder);
	vecLayer[m_iLayerIndex]->SortByZOrder();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Redo] Delete " << m_iLayerIndex << " Layer : " << m_pDeletePart->GetObjectName()
		<< "( " << m_pDeletePart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CLayerPartDelCmd::SetDeletePart(int iLayerIndex, int iZOrder, CLayerPart* pPart)
{
	m_pDeletePart = pPart;
	m_iLayerIndex = iLayerIndex;
	m_iZOrder = iZOrder;

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Delete " << m_iLayerIndex << " Layer : " << m_pDeletePart->GetObjectName()
		<< "( " << m_pDeletePart->GetZOrder() << " )";
	m_strLog = stm.str();
}