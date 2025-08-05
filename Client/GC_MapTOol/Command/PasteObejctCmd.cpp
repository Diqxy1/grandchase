#include ".\pasteobejctcmd.h"
#include "../LayerList.h"
#include "../LayerManager.h"

CPasteObejctCmd::CPasteObejctCmd(void)
	: m_pPasteObj(NULL)
	, m_iLayerIndex(0)
	, m_iZOrder(0)
	, m_bNeedDelete(false)
{
}

CPasteObejctCmd::~CPasteObejctCmd(void)
{
	if (m_bNeedDelete)
		SAFE_DELETE(m_pPasteObj);
}

void CPasteObejctCmd::Undo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->EraseLayerObject(m_pPasteObj);
	vecLayer[m_iLayerIndex]->ResetZOrder();
	vecLayer[m_iLayerIndex]->SortByZOrder();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Undo] Unpaste " << m_iLayerIndex << " Layer : " << m_pPasteObj->GetObjectName()
		<< "( " << m_iZOrder << " )";
	m_strLog = stm.str();
}

void CPasteObejctCmd::Redo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_iLayerIndex]->AddLayerPartIndex(m_pPasteObj, m_iZOrder);
	vecLayer[m_iLayerIndex]->SortByZOrder();
	pLayerList->ReloadLayerList();
	pLayerList->ReloadObjectList(m_iLayerIndex);

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Redo] Paste " << m_iLayerIndex << " Layer : " << m_pPasteObj->GetObjectName()
		<< "( " << m_iZOrder << " )";
	m_strLog = stm.str();
}

void CPasteObejctCmd::SetPasteData(int iLayerIndex, int iZOrder, CLayerPart* pPart)
{
	m_iLayerIndex = iLayerIndex;
	m_iZOrder = iZOrder;
	m_pPasteObj = pPart;
	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Paste " << m_iLayerIndex << " Layer : " << m_pPasteObj->GetObjectName()
		<< "( " << m_iZOrder << " )";
	m_strLog = stm.str();
}