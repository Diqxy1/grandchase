#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CInverseLeftRightCmd
	: public ICommand
{
public:
	CInverseLeftRightCmd(void);
	virtual ~CInverseLeftRightCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetPart(CLayerPart* pPart);

private:
	CLayerPart*         m_pPart;
};
