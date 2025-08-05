#pragma once

//#include "stdafx.h"
#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"
#include "../LayerList.h"
#include "../LayerManager.h"

struct KMoveData
{
	int             m_iCurLayerIndex;
	int             m_iCurZOrder;
	int             m_iAfterLayerIndex;
	int             m_iAfterZOrder;

	KMoveData()
	{
		m_iCurLayerIndex = 0;
		m_iCurZOrder = 0;
		m_iAfterLayerIndex = 0;
		m_iAfterZOrder = 0;
	}
};

class CLayerPartMoveCmd
	: public ICommand
{
public:
	CLayerPartMoveCmd(void);
	virtual ~CLayerPartMoveCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetMoveData(const KMoveData& kData, CLayerPart* pPart);

private:
	CLayerPart*     m_pMovePart;
	KMoveData       m_kMoveData;
};
