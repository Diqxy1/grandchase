#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CInverseUpDownCmd
	: public ICommand
{
public:
	CInverseUpDownCmd(void);
	virtual ~CInverseUpDownCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetPart(CLayerPart* pPart);

private:
	CLayerPart*         m_pPart;
};
