#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../ToolObject.h"

class CUnStampObjectCmd
	: public ICommand
{
public:
	CUnStampObjectCmd(void);
	virtual ~CUnStampObjectCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetUnStampObj(const std::vector< CToolObject* >& vecCurObj);

private:
	std::vector< CToolObject > m_vecCurObj;
};
