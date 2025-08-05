#pragma once

#include "../WOGMapTool.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CLayerPartKeyMoveCmd
	: public ICommand
{
public:
	CLayerPartKeyMoveCmd(void);
	virtual ~CLayerPartKeyMoveCmd(void);

	enum MOVETYPE
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
	};

	virtual void Undo();
	virtual void Redo();

	void SetMove(MOVETYPE eType, CLayerPart* pPart);

private:
	MOVETYPE        m_eMoveType;
	CLayerPart*     m_pPart;

	std::string     GetMoveTypeStr(MOVETYPE eType);
};
