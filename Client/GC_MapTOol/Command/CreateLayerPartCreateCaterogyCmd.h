#pragma once

#include "stdafx.h"
#include "../KCreateLayerPartDlg.h"
#include "ICommand.h"

class CCreateLayerPartCreateCaterogyCmd
	: public ICommand
{
public:
	CCreateLayerPartCreateCaterogyCmd(void);
	virtual ~CCreateLayerPartCreateCaterogyCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetCategoryname(const std::string& strCategory);

private:
	std::string     m_strCategory;
};
