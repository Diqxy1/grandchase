#include "stdafx.h"
#include ".\createlayerpartunstampobj.h"
#include "../ToolObject.h"
#include "../LayerPart.h"

CCreateLayerPartUnStampObj::CCreateLayerPartUnStampObj(void)
	: m_pPart(NULL)
{
	m_vecCurObj.clear();
}

CCreateLayerPartUnStampObj::~CCreateLayerPartUnStampObj(void)
{
	m_vecCurObj.clear();
}

void CCreateLayerPartUnStampObj::Undo()
{
	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i];

		CToolObject& kTargetObj = vecTO[kObj.GetIndexY() * m_pPart->GetFHWidth() + kObj.GetIndexX()];
		kTargetObj.SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
		kTargetObj.SetProp(kObj.GetProp(), "OR");
		kTargetObj.SetValue(kObj.GetValue());
	}

	std::stringstream stm;
	stm << "Undo] Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CCreateLayerPartUnStampObj::Redo()
{
	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i];

		CToolObject& kTargetObj = vecTO[kObj.GetIndexY() * m_pPart->GetFHWidth() + kObj.GetIndexX()];
		kTargetObj.SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_INIT);
		kTargetObj.SetProp(kObj.GetProp(), "INIT");
		kTargetObj.SetValue("");
	}

	std::stringstream stm;
	stm << "Redo] UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CCreateLayerPartUnStampObj::SetUnStampObj(CLayerPart* pPart, std::vector< CToolObject >& vecCurObj)
{
	m_pPart = pPart;
	m_vecCurObj.clear();
	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < static_cast<int>(vecCurObj.size()); ++i)
	{
		CToolObject kTObj;
		kTObj = vecTO[vecCurObj[i].GetIndexY() * m_pPart->GetFHWidth() + vecCurObj[i].GetIndexX()];
		m_vecCurObj.push_back(kTObj);
	}

	std::stringstream stm;
	stm << "UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}