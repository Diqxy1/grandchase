#pragma once

#include "ICommand.h"
#include "../LayerPart.h"
#include "../ToolObject.h"

class CCreateLayerPartStampObjCmd
	: public ICommand
{
public:
	CCreateLayerPartStampObjCmd(void);
	virtual ~CCreateLayerPartStampObjCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetStampObj(CLayerPart* pPart, std::vector< CToolObject > vecCurObj);

private:
	CLayerPart* m_pPart;
	std::vector< std::pair< bool, CToolObject > > m_vecCurObj;
};
