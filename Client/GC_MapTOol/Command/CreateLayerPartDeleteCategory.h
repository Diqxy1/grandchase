#pragma once

#include "stdafx.h"
#include "ICommand.h"

class CCreateLayerPartDeleteCategory
	: public ICommand
{
public:
	CCreateLayerPartDeleteCategory(void);
	virtual ~CCreateLayerPartDeleteCategory(void);

	virtual void Undo();
	virtual void Redo();

	void SetCategoryName(const std::string& strCategory);

private:
	std::string m_strCategory;
};
