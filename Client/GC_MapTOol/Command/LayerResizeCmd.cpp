#include ".\layerresizecmd.h"
#include "../LayerList.h"
#include "../LayerManager.h"

CLayerResizeCmd::CLayerResizeCmd(void)
{
}

CLayerResizeCmd::~CLayerResizeCmd(void)
{
}

void CLayerResizeCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	vecLayer[m_kResizeValue.m_iLayerIndex]->SetLayerXSize(m_kResizeValue.m_fOldWidth);
	vecLayer[m_kResizeValue.m_iLayerIndex]->SetLayerYSize(m_kResizeValue.m_fOldHeight);

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->m_lcLayerList.SetItemState(m_kResizeValue.m_iCurPos, LVIS_SELECTED, LVIS_SELECTED);
	pLayerList->ReloadObjectList(m_kResizeValue.m_iCurPos);

	std::stringstream stm;
	stm << "Undo] Layer " << m_kResizeValue.m_iLayerIndex
		<< " Resized to " << m_kResizeValue.m_fOldWidth << " * " << m_kResizeValue.m_fOldHeight;
	m_strLog = stm.str();
}

void CLayerResizeCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	vecLayer[m_kResizeValue.m_iLayerIndex]->SetLayerXSize(m_kResizeValue.m_fNewWidth);
	vecLayer[m_kResizeValue.m_iLayerIndex]->SetLayerYSize(m_kResizeValue.m_fNewHeight);

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->m_lcLayerList.SetItemState(m_kResizeValue.m_iCurPos, LVIS_SELECTED, LVIS_SELECTED);
	pLayerList->ReloadObjectList(m_kResizeValue.m_iCurPos);

	std::stringstream stm;
	stm << "Redo] Layer " << m_kResizeValue.m_iLayerIndex
		<< " Resized to " << m_kResizeValue.m_fNewWidth << " * " << m_kResizeValue.m_fNewHeight;
	m_strLog = stm.str();
}

void CLayerResizeCmd::SetReseizeValue(const LayerResizeValue& kValue)
{
	m_kResizeValue = kValue;
	std::stringstream stm;
	stm << "Layer " << kValue.m_iLayerIndex << " Resized to " << kValue.m_fNewWidth << " * " << kValue.m_fNewHeight;
	m_strLog = stm.str();
}