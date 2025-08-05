#include "stdafx.h"
#include ".\addlayerpartcmd.h"
#include "../WOGMapTOol.h"
#include "../LayerManager.h"
#include "../LayerList.h"

CAddLayerPartCmd::CAddLayerPartCmd(void)
	: m_pPart(NULL)
	, m_bNeedDelete(false)
{
}

CAddLayerPartCmd::~CAddLayerPartCmd(void)
{
	if (m_bNeedDelete)
		SAFE_DELETE(m_pPart);
}

void CAddLayerPartCmd::Undo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_pPart->GetLayer()]->EraseLayerObject(m_pPart);
	vecLayer[m_pPart->GetLayer()]->ResetZOrder(m_pPart->GetZOrder());
	vecLayer[m_pPart->GetLayer()]->SortByZOrder();

	g_AppFormView->GetLayerList()->ReloadLayerList();
	g_AppFormView->GetLayerList()->ReloadObjectList(m_pPart->GetLayer());

	m_bNeedDelete = true;

	std::stringstream stm;
	stm << "Undo] Part removed " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CAddLayerPartCmd::Redo()
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[m_pPart->GetLayer()]->AddLayerPartIndex(m_pPart, m_pPart->GetZOrder());
	vecLayer[m_pPart->GetLayer()]->SortByZOrder();

	g_AppFormView->GetLayerList()->ReloadLayerList();
	g_AppFormView->GetLayerList()->ReloadObjectList(m_pPart->GetLayer());

	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Redo] Part Added " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CAddLayerPartCmd::SetAddPart(CLayerPart* pPart)
{
	m_pPart = pPart;
	m_bNeedDelete = false;

	std::stringstream stm;
	stm << "Part Added " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}