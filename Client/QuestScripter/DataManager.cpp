#include "StdAfx.h"
#include ".\datamanager.h"

CDataManager::CDataManager(void)
{
	m_iCnt = 0;
	m_index = 0;
}

CDataManager::~CDataManager(void)
{
	for (int i = 0; i < (int)m_vtQuestHistory.size(); ++i)
		SAFE_DELETE(m_vtQuestHistory[i]);
	m_vtQuestHistory.clear();
}

void CDataManager::AddQuest(Quest *quest)
{
	// Undo 하다가 데이터를 수정하면, Undo 된 위치 부터 다시 백업한다.
	if ((int)m_vtQuestHistory.size() > (m_index))
	{
		m_iCnt = m_index + 1;
		while (m_iCnt < (int)m_vtQuestHistory.size())
		{
			SAFE_DELETE(m_vtQuestHistory[m_vtQuestHistory.size() - 1]);
			m_vtQuestHistory.pop_back();
		}

		m_iCnt = (int)m_vtQuestHistory.size();
	}

	// 언도는 100개 제한이다.
	if (MAX_UNDO < m_iCnt)
	{
		SAFE_DELETE(m_vtQuestHistory[0]);
		m_vtQuestHistory.erase(m_vtQuestHistory.begin());
		m_index--;
	}

	// 데이터 백업
	Quest *pQuest = new Quest;
	*pQuest = *quest;
	m_vtQuestHistory.push_back(pQuest);
	m_iCnt++;
	m_index++;
}

bool CDataManager::NextQuest(Quest *quest)
{
	if ((int)m_vtQuestHistory.size() <= 0) return false;
	if ((int)m_vtQuestHistory.size() <= (m_index + 1))
		m_index = (int)m_vtQuestHistory.size() - 1;
	else
		m_index++;
	if ((int)m_vtQuestHistory.size() > m_index)
		*quest = *m_vtQuestHistory[m_index];

	// 더 NextQuest 할게 있는지 확인
	if ((m_index + 1) >= m_iCnt)
		return false;

	return true;
}

bool CDataManager::PrevQuest(Quest *quest)
{
	if ((m_index - 1) < 0)
		m_index = 0;
	else
		m_index--;
	if ((int)m_vtQuestHistory.size() > m_index)
		*quest = *m_vtQuestHistory[m_index];

	// 더 PrevQuest 할게 있는지 확인
	if (m_index <= 0)
		return false;

	return true;
}

void CDataManager::Clear(void)
{
	m_vtQuestHistory.clear();
}