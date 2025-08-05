#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../ToolObject.h"
#include "../KGCMap.h"

class CWorldResizeCmd
	: public ICommand
{
public:
	CWorldResizeCmd();
	virtual ~CWorldResizeCmd(void);

	void SetNewWorld(int iX, int iY, CToolObject** ppNewWorld);
	void SetOldWorld(int iX, int iY, CToolObject** ppOldWorld);
	void ClearWorld(CToolObject** pClear, int iX);

	virtual void Undo();
	virtual void Redo();

private:
	CToolObject** m_ppOldTObj;
	CToolObject** m_ppNewTObj;
	int m_iOldX;
	int m_iOldY;
	int m_iNewX;
	int m_iNewY;
};
