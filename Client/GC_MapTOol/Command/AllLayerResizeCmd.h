#pragma once

#include "stdafx.h"
#include "../LayerList.h"
#include "../WOGMapTOol.h"
#include "../LayerManager.h"
#include "ICommand.h"
//#include <vector>

struct AllLayerResizeValue
{
	AllLayerResizeValue()
	{
		m_fNewWidth = 0;
		m_fNewHeight = 0;
		m_fOldWidth = 0;
		m_fOldHeight = 0;
	}

	float m_fNewWidth;
	float m_fNewHeight;
	float m_fOldWidth;
	float m_fOldHeight;
};

class CAllLayerResizeCmd
	: public ICommand
{
public:
	CAllLayerResizeCmd(void);
	virtual ~CAllLayerResizeCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetReseizeValue();
	void push_back(const AllLayerResizeValue& kValue) { m_kResizeValue.push_back(kValue); }
	void SetCurPos(int iPos) { m_iCurPos = iPos; }

private:
	std::vector<AllLayerResizeValue> m_kResizeValue;
	int m_iCurPos;
};
