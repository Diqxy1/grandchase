#pragma once

#include "ICommand.h"
#include "../LayerPart.h"
#include "../ToolObject.h"

class CCreateLayerPartModifyFH
	: public ICommand
{
public:
	CCreateLayerPartModifyFH(void);
	virtual ~CCreateLayerPartModifyFH(void);

	virtual void Undo();
	virtual void Redo();

	void SetModifyData(int iNewWidth, int iNewHeight, CLayerPart* pPart);

private:
	CLayerPart* m_pPart;
	int m_iOldWidth;
	int m_iOldHeight;
	int m_iNewWidth;
	int m_iNewHeight;
	std::vector< CToolObject > m_vecToolObject;
};
