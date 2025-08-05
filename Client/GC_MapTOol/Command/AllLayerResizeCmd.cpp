#include ".\alllayerresizecmd.h"

CAllLayerResizeCmd::CAllLayerResizeCmd(void) : m_iCurPos(0)
{
	m_kResizeValue.clear();
}

CAllLayerResizeCmd::~CAllLayerResizeCmd(void)
{
	m_kResizeValue.clear();
}

void CAllLayerResizeCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	for (int i = 0; i < (int)vecLayer.size(); ++i)
	{
		vecLayer[i]->SetLayerXSize(m_kResizeValue[i].m_fOldWidth);
		vecLayer[i]->SetLayerYSize(m_kResizeValue[i].m_fOldHeight);
	}

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->m_lcLayerList.SetItemState(m_iCurPos, LVIS_SELECTED, LVIS_SELECTED);
	pLayerList->ReloadObjectList(m_iCurPos);

	std::stringstream stm;
	stm << "Undo] All Layer Resized to old size";
	m_strLog = stm.str();
}

void CAllLayerResizeCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	for (int i = 0; i < (int)vecLayer.size(); ++i)
	{
		vecLayer[i]->SetLayerXSize(m_kResizeValue[i].m_fNewWidth);
		vecLayer[i]->SetLayerYSize(m_kResizeValue[i].m_fNewHeight);
	}

	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ReloadLayerList();
	pLayerList->m_lcLayerList.SetItemState(m_iCurPos, LVIS_SELECTED, LVIS_SELECTED);
	pLayerList->ReloadObjectList(m_iCurPos);

	std::stringstream stm;
	stm << "Redo] All Layer " << "Resized to " << m_kResizeValue[0].m_fNewWidth << " * " << m_kResizeValue[0].m_fNewHeight;
	m_strLog = stm.str();
}

void CAllLayerResizeCmd::SetReseizeValue()
{
	std::stringstream stm;
	stm << "All Layer " << " Resized to " << m_kResizeValue[0].m_fNewWidth << " * " << m_kResizeValue[0].m_fNewHeight;
	m_strLog = stm.str();
}