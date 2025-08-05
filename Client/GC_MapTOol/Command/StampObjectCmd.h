#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../ToolObject.h"

class CStampObjectCmd
	: public ICommand
{
public:
	CStampObjectCmd(void);
	virtual ~CStampObjectCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetStampObj(const std::vector< CToolObject* >& vecCurObj);

private:
	std::vector< std::pair< bool, CToolObject > > m_vecCurObj;
};
