#pragma once

#include "ICommand.h"
#include "../LayerPart.h"

class CCreateLayerPartCloneCmd
	: public ICommand
{
public:
	CCreateLayerPartCloneCmd(void);
	virtual ~CCreateLayerPartCloneCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetClonePart(CLayerPart* pPart);

private:
	CLayerPart* m_pClonePart;
	bool        m_bNeedDelete;
};
