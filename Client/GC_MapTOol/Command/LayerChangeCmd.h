#pragma once
//#include "stdafx.h"
#include "../LayerList.h"
#include "../WOGMapTOol.h"
#include "ICommand.h"

class CLayerChangeCmd
	: public ICommand
{
public:
	CLayerChangeCmd(void);
	virtual ~CLayerChangeCmd(void);

	virtual void Undo();
	virtual void Redo();

	void ChangeLayer(int iFirst, int iSecond);

private:
	int         m_iFirst;
	int         m_iSecond;
};
