#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CLayerZOrderCmd
	: public ICommand
{
public:
	CLayerZOrderCmd(void);
	virtual ~CLayerZOrderCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetZOrder(int iAfterZOrder, CLayerPart* pPart);

private:
	CLayerPart*     m_pPart;
	int             m_iCurZOrder;
	int             m_iAfterZOrder;
	int             m_iLayerIndex;
};
