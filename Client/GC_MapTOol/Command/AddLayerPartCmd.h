#pragma once

#include "ICommand.h"
#include "../LayerPart.h"

class CAddLayerPartCmd
	: public ICommand
{
public:
	CAddLayerPartCmd(void);
	virtual ~CAddLayerPartCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetAddPart(CLayerPart* pPart);

private:
	CLayerPart*     m_pPart;
	bool            m_bNeedDelete;
};
