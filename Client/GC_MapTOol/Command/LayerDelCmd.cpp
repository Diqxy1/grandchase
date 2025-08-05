#include ".\layerdelcmd.h"
#include "../LayerManager.h"

CLayerDelCmd::CLayerDelCmd(void)
{
	m_vecDeletedPart.clear();
}

CLayerDelCmd::~CLayerDelCmd(void)
{
	if (m_bNeedDelete)
	{
		std::vector< CLayerPart* >::iterator vit;
		for (vit = m_vecDeletedPart.begin(); vit != m_vecDeletedPart.end(); ++vit)
		{
			SAFE_DELETE((*vit));
		}
	}
	m_vecDeletedPart.clear();
}

void CLayerDelCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	std::vector< CLayerPart* >& vecPart = vecLayer[m_iDeletedIndex]->GetPartVector();
	vecPart.clear();
	vecPart = m_vecDeletedPart;

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Undo] Layer " << m_iDeletedIndex << " restored ( " << m_vecDeletedPart.size() << " Parts )";
	m_strLog = stm.str();
}

void CLayerDelCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	std::vector< CLayerPart* >& vecPart = vecLayer[m_iDeletedIndex]->GetPartVector();
	vecPart.clear();

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Redo] Layer " << m_iDeletedIndex << " deleted ( " << m_vecDeletedPart.size() << " Parts )";
	m_strLog = stm.str();
}

void CLayerDelCmd::SetDeleteVector(int iDeletedIndex, const std::vector< CLayerPart* >& vecDelete)
{
	m_vecDeletedPart = vecDelete;
	m_iDeletedIndex = iDeletedIndex;
	std::stringstream stm;
	stm << "Layer " << iDeletedIndex << " deleted ( " << vecDelete.size() << " Parts )";
	m_strLog = stm.str();

	m_bNeedDelete = true;
}