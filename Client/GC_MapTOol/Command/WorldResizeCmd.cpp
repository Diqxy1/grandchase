#include ".\worldresizecmd.h"

//#include <sstream>

CWorldResizeCmd::CWorldResizeCmd(void)
	: m_ppOldTObj(NULL)
	, m_ppNewTObj(NULL)
	, m_iNewX(0)
	, m_iNewY(0)
	, m_iOldX(0)
	, m_iOldY(0)
{
}

CWorldResizeCmd::~CWorldResizeCmd(void)
{
	ClearWorld(m_ppNewTObj, m_iNewX);
	ClearWorld(m_ppOldTObj, m_iOldX);
}

void CWorldResizeCmd::SetNewWorld(int iX, int iY, CToolObject** ppNewWorld)
{
	ClearWorld(m_ppNewTObj, m_iNewX);

	m_iNewX = iX;
	m_iNewY = iY;

	m_ppNewTObj = new CToolObject*[m_iNewX];
	for (int i = 0; i < m_iNewX; i++)
		m_ppNewTObj[i] = new CToolObject[m_iNewY];

	for (int i = 0; i < m_iNewX; ++i)
	{
		for (int j = 0; j < m_iNewY; ++j)
		{
			m_ppNewTObj[i][j] = ppNewWorld[i][j];
		}
	}

	std::stringstream stm;
	stm << "World resized to " << m_iNewX << " X " << m_iNewY;
	m_strLog = stm.str();
}

void CWorldResizeCmd::SetOldWorld(int iX, int iY, CToolObject** ppOldWorld)
{
	ClearWorld(m_ppOldTObj, m_iOldX);

	m_iOldX = iX;
	m_iOldY = iY;

	m_ppOldTObj = new CToolObject*[m_iOldX];
	for (int i = 0; i < m_iOldX; i++)
		m_ppOldTObj[i] = new CToolObject[m_iOldY];

	for (int i = 0; i < m_iOldX; ++i)
	{
		for (int j = 0; j < m_iOldY; ++j)
		{
			m_ppOldTObj[i][j] = ppOldWorld[i][j];
		}
	}
}

void CWorldResizeCmd::ClearWorld(CToolObject** pClear, int iX)
{
	if (pClear != NULL)
	{
		for (int i = 0; i < iX; i++)
		{
			if (pClear[i] != NULL)
				SAFE_DELETE_ARRAY(pClear[i]);
		}
	}

	SAFE_DELETE_ARRAY(pClear);
}

void CWorldResizeCmd::Undo()
{
	CToolObject** ppDel = g_AppFormView->m_ppTObj;
	CToolObject** ppWorld = new CToolObject*[m_iOldX];
	for (int i = 0; i < m_iOldX; i++)
		ppWorld[i] = new CToolObject[m_iOldY];

	for (int i = 0; i < m_iOldX; ++i)
	{
		for (int j = 0; j < m_iOldY; ++j)
		{
			ppWorld[i][j] = m_ppOldTObj[i][j];
		}
	}

	g_AppFormView->m_ppTObj = ppWorld;
	ClearWorld(ppDel, SiKGCMap()->GetBlockWidth());

	SiKGCMap()->SetBlockWidth(m_iOldX);
	SiKGCMap()->SetBlockHeight(m_iOldY);
	g_AppFormView->SetWorldVertex();
	g_AppFormView->ReloadLayer();

	std::stringstream stm;
	stm << "Undo] World resized to " << m_iOldX << " X " << m_iOldY;
	m_strLog = stm.str();
}

void CWorldResizeCmd::Redo()
{
	CToolObject** ppDel = g_AppFormView->m_ppTObj;
	CToolObject** ppWorld = new CToolObject*[m_iNewX];
	for (int i = 0; i < m_iNewX; i++)
		ppWorld[i] = new CToolObject[m_iNewY];

	for (int i = 0; i < m_iNewX; ++i)
	{
		for (int j = 0; j < m_iNewX; ++j)
		{
			ppWorld[i][j] = m_ppNewTObj[i][j];
		}
	}

	g_AppFormView->m_ppTObj = ppWorld;
	ClearWorld(ppDel, SiKGCMap()->GetBlockWidth());

	SiKGCMap()->SetBlockWidth(m_iNewX);
	SiKGCMap()->SetBlockHeight(m_iNewY);
	g_AppFormView->SetWorldVertex();
	g_AppFormView->ReloadLayer();

	std::stringstream stm;
	stm << "Redo] World resized to " << m_iNewX << " X " << m_iNewY;
	m_strLog = stm.str();
}