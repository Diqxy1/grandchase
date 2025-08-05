#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
//#include <vector>
#include "../LayerPart.h"

class CLayerDelCmd
	: public ICommand
{
public:
	CLayerDelCmd(void);
	~CLayerDelCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetDeleteVector(int iDeletedIndex, const std::vector< CLayerPart* >& vecDelete);

private:
	std::vector< CLayerPart* >  m_vecDeletedPart;
	int                         m_iDeletedIndex;
	bool                        m_bNeedDelete;
};
