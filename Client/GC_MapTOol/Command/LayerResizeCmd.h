#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"

struct LayerResizeValue
{
	LayerResizeValue()
	{
		m_iLayerIndex = -1;
		m_fNewWidth = 0;
		m_fNewHeight = 0;
		m_fOldWidth = 0;
		m_fOldHeight = 0;
		m_iCurPos = 0;
	}

	int m_iLayerIndex;
	float m_fNewWidth;
	float m_fNewHeight;
	float m_fOldWidth;
	float m_fOldHeight;
	int m_iCurPos;
};

class CLayerResizeCmd
	: public ICommand
{
public:
	CLayerResizeCmd(void);
	virtual ~CLayerResizeCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetReseizeValue(const LayerResizeValue& kValue);

private:
	LayerResizeValue m_kResizeValue;
};
