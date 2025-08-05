#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
//#include "../LayerPart.h"

class CLayerPart;
struct DragPosData
{
	int         m_iDragStartPosX;
	int         m_iDragStartPosY;
	int         m_iDragEndPosX;
	int         m_iDragEndPosY;

	DragPosData()
	{
		m_iDragStartPosX = 0;
		m_iDragStartPosY = 0;
		m_iDragEndPosX = 0;
		m_iDragEndPosY = 0;
	}
};

class CDragPartCmd
	: public ICommand
{
public:
	CDragPartCmd(void);
	virtual ~CDragPartCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetDragData(CLayerPart* pPart, const DragPosData& kDragData);

private:
	CLayerPart*     m_pDragPart;
	DragPosData     m_kDragData;
};
