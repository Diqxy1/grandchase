#pragma once

#include "ICommand.h"

class CLayerPart;
class CToolObject;
class CCreateLayerPartUnStampObj
	: public ICommand
{
public:
	CCreateLayerPartUnStampObj(void);
	virtual ~CCreateLayerPartUnStampObj(void);

	virtual void Undo();
	virtual void Redo();

	void SetUnStampObj(CLayerPart* pPart, std::vector< CToolObject >& vecCurObj);

private:
	CLayerPart* m_pPart;
	std::vector< CToolObject > m_vecCurObj;
};
