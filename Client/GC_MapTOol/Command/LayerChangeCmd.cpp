#include ".\layerchangecmd.h"
//#include <sstream>

CLayerChangeCmd::CLayerChangeCmd(void)
{
}

CLayerChangeCmd::~CLayerChangeCmd(void)
{
}

void CLayerChangeCmd::Undo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ChangeLayer(m_iSecond, m_iFirst);
	std::stringstream stm;
	stm << "Undo] Layer Changed " << m_iSecond << " to " << m_iFirst;
	m_strLog = stm.str();
}

void CLayerChangeCmd::Redo()
{
	CLayerList* pLayerList = g_AppFormView->GetLayerList();
	pLayerList->ChangeLayer(m_iFirst, m_iSecond);
	std::stringstream stm;
	stm << "Redo] Layer Changed " << m_iFirst << " to " << m_iSecond;
	m_strLog = stm.str();
}

void CLayerChangeCmd::ChangeLayer(int iFirst, int iSecond)
{
	m_iFirst = iFirst;
	m_iSecond = iSecond;
	std::stringstream stm;
	stm << "Layer Changed " << iFirst << " to " << iSecond;
	m_strLog = stm.str();
}