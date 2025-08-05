#include "stdafx.h"
#include "GCDepotChar.h"

IMPLEMENT_CLASSNAME(KGCDepotChar);
IMPLEMENT_WND_FACTORY(KGCDepotChar);
IMPLEMENT_WND_FACTORY_NAME(KGCDepotChar, "gc_depot_char_info");

KGCDepotChar::KGCDepotChar(void)
{
	for (int i = 0; i < GC_CHAR_NUM; i++) {
		char strChar[128] = { 0, };
		sprintf(strChar, "char%d", i);
		m_mapCharInfo[i] = NULL;
		LINK_CONTROL(strChar, m_mapCharInfo[i]);
	}

	m_mapCharInfo[-1] = NULL;
	LINK_CONTROL("common_info", m_mapCharInfo[-1]);
}

KGCDepotChar::~KGCDepotChar(void)
{
}

void KGCDepotChar::ActionPerformed(const KActionEvent& event)
{
}

void KGCDepotChar::OnCreate(void)
{
	for (std::map<int, KD3DWnd*>::iterator mIt = m_mapCharInfo.begin(); mIt != m_mapCharInfo.end(); mIt++)
	{
		if (mIt->first == 0) {
			mIt->second->InitState(true, false, NULL);
		}
		else {
			mIt->second->InitState(false, false, NULL);
		}
	}
}

void KGCDepotChar::SetChar(int iChar)
{
	for (std::map<int, KD3DWnd*>::iterator mIt = m_mapCharInfo.begin(); mIt != m_mapCharInfo.end(); mIt++)
	{
		if (mIt->first == iChar) {
			mIt->second->InitState(true, false, NULL);
		}
		else {
			mIt->second->InitState(false, false, NULL);
		}
	}
}