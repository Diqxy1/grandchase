#include ".\stampobjectcmd.h"

CStampObjectCmd::CStampObjectCmd(void)
{
	m_vecCurObj.clear();
}

CStampObjectCmd::~CStampObjectCmd(void)
{
	m_vecCurObj.clear();
}

void CStampObjectCmd::Undo()
{
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i].second;
		bool bInit = m_vecCurObj[i].first;

		CToolObject* pObj = &g_AppFormView->m_ppTObj[kObj.GetIndexX()][kObj.GetIndexY()];
		if (bInit == true)
		{
			pObj->SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_INIT);
			pObj->SetProp(kObj.GetProp(), "INIT");
		}
		else
		{
			pObj->SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
			pObj->SetProp(kObj.GetProp(), "XOR");
		}
	}

	std::stringstream stm;
	stm << "Undo] UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CStampObjectCmd::Redo()
{
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i].second;

		CToolObject* pObj = &g_AppFormView->m_ppTObj[kObj.GetIndexX()][kObj.GetIndexY()];
		pObj->SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
		pObj->SetProp(kObj.GetProp(), "OR");
	}

	std::stringstream stm;
	stm << "Redo] Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CStampObjectCmd::SetStampObj(const std::vector< CToolObject* >& vecCurObj)
{
	m_vecCurObj.clear();
	for (int i = 0; i < static_cast<int>(vecCurObj.size()); ++i)
	{
		bool bInit = false;
		CToolObject* pObj = &g_AppFormView->m_ppTObj[vecCurObj[i]->GetIndexX()][vecCurObj[i]->GetIndexY()];
		if (pObj->GetType() == CFootHold::FHT_INIT)
			bInit = true;

		pObj->SetPos(vecCurObj[i]->GetIndexX(), vecCurObj[i]->GetIndexY(), CFootHold::FHT_HEAD);
		pObj->SetProp(vecCurObj[i]->GetProp(), "OR");

		CToolObject kTObj;
		kTObj = *vecCurObj[i];

		m_vecCurObj.push_back(std::make_pair(bInit, kTObj));
	}

	std::stringstream stm;
	stm << "Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}