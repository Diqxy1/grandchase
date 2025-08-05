#include "stdafx.h"
#include ".\createlayerpartstampobjcmd.h"

CCreateLayerPartStampObjCmd::CCreateLayerPartStampObjCmd(void)
	: m_pPart(NULL)
{
	m_vecCurObj.clear();
}

CCreateLayerPartStampObjCmd::~CCreateLayerPartStampObjCmd(void)
{
	m_vecCurObj.clear();
}

void CCreateLayerPartStampObjCmd::Undo()
{
	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i].second;
		bool bInit = m_vecCurObj[i].first;

		CToolObject& kTargetObj = vecTO[kObj.GetIndexY() * m_pPart->GetFHWidth() + kObj.GetIndexX()];
		if (bInit == true)
		{
			kTargetObj.SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_INIT);
			kTargetObj.SetProp(kObj.GetProp(), "INIT");
		}
		else
		{
			kTargetObj.SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
			kTargetObj.SetProp(kObj.GetProp(), "XOR");
		}
	}

	std::stringstream stm;
	stm << "Undo] UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CCreateLayerPartStampObjCmd::Redo()
{
	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i].second;

		CToolObject& kTargetObj = vecTO[kObj.GetIndexY() * m_pPart->GetFHWidth() + kObj.GetIndexX()];
		kTargetObj.SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
		kTargetObj.SetProp(kObj.GetProp(), "OR");
	}

	std::stringstream stm;
	stm << "Redo] Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CCreateLayerPartStampObjCmd::SetStampObj(CLayerPart* pPart, std::vector< CToolObject > vecCurObj)
{
	m_pPart = pPart;
	m_vecCurObj.clear();

	std::vector< CToolObject >& vecTO = m_pPart->GetToolObjectVector();
	for (int i = 0; i < static_cast<int>(vecCurObj.size()); ++i)
	{
		bool bInit = false;
		CToolObject& kObj = vecTO[vecCurObj[i].GetIndexY() * m_pPart->GetFHWidth() + vecCurObj[i].GetIndexX()];
		if (kObj.GetType() == CFootHold::FHT_INIT)
			bInit = true;

		kObj.SetPos(vecCurObj[i].GetIndexX(), vecCurObj[i].GetIndexY(), CFootHold::FHT_HEAD);
		kObj.SetProp(vecCurObj[i].GetProp(), "OR");

		CToolObject kTObj;
		kTObj = vecCurObj[i];

		m_vecCurObj.push_back(std::make_pair(bInit, kTObj));
	}

	std::stringstream stm;
	stm << "Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}