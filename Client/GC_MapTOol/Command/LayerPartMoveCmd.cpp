#include ".\layerpartmovecmd.h"

CLayerPartMoveCmd::CLayerPartMoveCmd(void)
	: m_pMovePart(NULL)
{
}

CLayerPartMoveCmd::~CLayerPartMoveCmd(void)
{
}

void CLayerPartMoveCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_kMoveData.m_iAfterLayerIndex]->EraseLayerObject(m_pMovePart);
	vecLayer[m_kMoveData.m_iAfterLayerIndex]->ResetZOrder(m_kMoveData.m_iAfterZOrder);
	vecLayer[m_kMoveData.m_iAfterLayerIndex]->SortByZOrder();

	m_pMovePart->SetLayer(m_kMoveData.m_iCurLayerIndex);
	m_pMovePart->SetZOrder(m_kMoveData.m_iCurZOrder);
	vecLayer[m_kMoveData.m_iCurLayerIndex]->AddLayerPartIndex(m_pMovePart, m_kMoveData.m_iCurZOrder);
	vecLayer[m_kMoveData.m_iCurLayerIndex]->SortByZOrder();

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_kMoveData.m_iCurLayerIndex);

	std::stringstream stm;
	stm << "Undo] " << m_kMoveData.m_iAfterLayerIndex << " Layer : " << m_pMovePart->GetObjectName()
		<< "( " << m_kMoveData.m_iAfterZOrder << " ) Move to "
		<< m_kMoveData.m_iCurLayerIndex << " Layer : ( " << m_kMoveData.m_iCurZOrder << " )";
	m_strLog = stm.str();
}

void CLayerPartMoveCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_kMoveData.m_iCurLayerIndex]->EraseLayerObject(m_pMovePart);
	vecLayer[m_kMoveData.m_iCurLayerIndex]->ResetZOrder(m_kMoveData.m_iCurZOrder);
	vecLayer[m_kMoveData.m_iCurLayerIndex]->SortByZOrder();

	m_pMovePart->SetLayer(m_kMoveData.m_iAfterLayerIndex);
	m_pMovePart->SetZOrder(m_kMoveData.m_iAfterZOrder);
	vecLayer[m_kMoveData.m_iAfterLayerIndex]->AddLayerPartIndex(m_pMovePart, m_kMoveData.m_iAfterZOrder);
	vecLayer[m_kMoveData.m_iAfterLayerIndex]->SortByZOrder();

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_kMoveData.m_iAfterLayerIndex);

	std::stringstream stm;
	stm << "Redo] " << m_kMoveData.m_iCurLayerIndex << " Layer : " << m_pMovePart->GetObjectName()
		<< "( " << m_kMoveData.m_iCurZOrder << " ) Move to "
		<< m_kMoveData.m_iAfterLayerIndex << " Layer : ( " << m_kMoveData.m_iAfterZOrder << " )";
	m_strLog = stm.str();
}

void CLayerPartMoveCmd::SetMoveData(const KMoveData& kData, CLayerPart* pPart)
{
	m_kMoveData = kData;
	m_pMovePart = pPart;

	std::stringstream stm;
	stm << m_kMoveData.m_iCurLayerIndex << " Layer : " << m_pMovePart->GetObjectName()
		<< "( " << m_kMoveData.m_iCurZOrder << " ) Move to "
		<< m_kMoveData.m_iAfterLayerIndex << " Layer : ( " << m_kMoveData.m_iAfterZOrder << " )";
	m_strLog = stm.str();
}