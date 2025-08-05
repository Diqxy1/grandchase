#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CPasteObejctCmd
	: public ICommand
{
public:
	CPasteObejctCmd(void);
	virtual ~CPasteObejctCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetPasteData(int iLayerIndex, int iZOrder, CLayerPart* pPart);

private:
	CLayerPart* m_pPasteObj;
	int         m_iLayerIndex;
	int         m_iZOrder;
	bool        m_bNeedDelete;
};
