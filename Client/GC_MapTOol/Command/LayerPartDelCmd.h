#pragma once

#include "../WOGMapTool.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CLayerPartDelCmd
	: public ICommand
{
public:
	CLayerPartDelCmd(void);
	virtual ~CLayerPartDelCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetDeletePart(int iLayerIndex, int iZOrder, CLayerPart* pPart);

private:
	CLayerPart*     m_pDeletePart;
	int             m_iLayerIndex;
	int             m_iZOrder;
	bool            m_bNeedDelete;
};
