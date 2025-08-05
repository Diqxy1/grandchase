#pragma once

//#include "stdafx.h"
#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../ToolObject.h"
//#include <vector>

typedef std::pair< std::pair<int, int>, std::string > PairFootHoldValue;
typedef std::vector< PairFootHoldValue > VecFootHoldValue;

class CModifyFHValueCmd
	: public ICommand
{
public:
	CModifyFHValueCmd(void);
	virtual ~CModifyFHValueCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetUndoData(const VecFootHoldValue& vecUndo) { m_vecUndo = vecUndo; }
	void SetRedoData(const VecFootHoldValue& vecRedo);

private:
	VecFootHoldValue m_vecUndo;
	VecFootHoldValue m_vecRedo;
};
