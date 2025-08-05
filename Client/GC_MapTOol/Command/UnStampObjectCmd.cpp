#include ".\unstampobjectcmd.h"

CUnStampObjectCmd::CUnStampObjectCmd(void)
{
	m_vecCurObj.clear();
}

CUnStampObjectCmd::~CUnStampObjectCmd(void)
{
	m_vecCurObj.clear();
}

void CUnStampObjectCmd::Undo()
{
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i];

		CToolObject* pObj = &g_AppFormView->m_ppTObj[kObj.GetIndexX()][kObj.GetIndexY()];
		pObj->SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_HEAD);
		pObj->SetProp(kObj.GetProp(), "OR");
		pObj->SetValue(kObj.GetValue());
	}

	std::stringstream stm;
	stm << "Undo] Stamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CUnStampObjectCmd::Redo()
{
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kObj = m_vecCurObj[i];

		CToolObject* pObj = &g_AppFormView->m_ppTObj[kObj.GetIndexX()][kObj.GetIndexY()];
		pObj->SetPos(kObj.GetIndexX(), kObj.GetIndexY(), CFootHold::FHT_INIT);
		pObj->SetProp(kObj.GetProp(), "INIT");
		pObj->SetValue("");
	}

	std::stringstream stm;
	stm << "Redo] UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}

void CUnStampObjectCmd::SetUnStampObj(const std::vector< CToolObject* >& vecCurObj)
{
	m_vecCurObj.clear();
	for (int i = 0; i < static_cast<int>(vecCurObj.size()); ++i)
	{
		CToolObject kTObj;
		CToolObject* pObj = &g_AppFormView->m_ppTObj[vecCurObj[i]->GetIndexX()][vecCurObj[i]->GetIndexY()];
		kTObj = *pObj;
		m_vecCurObj.push_back(kTObj);
	}

	std::stringstream stm;
	stm << "UnStamp " << m_vecCurObj.size() << " ToolObjects";
	m_strLog = stm.str();
}