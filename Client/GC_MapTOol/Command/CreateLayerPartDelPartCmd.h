#pragma once

#include "stdafx.h"
#include "../KCreateLayerPartDlg.h"
#include "../LayerManager.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CCreateLayerPartDelPartCmd
	: public ICommand
{
public:
	CCreateLayerPartDelPartCmd(void);
	virtual ~CCreateLayerPartDelPartCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetDelPart(CLayerPart* pPart);

private:
	CLayerPart* m_pDelPart;
	bool        m_bNeedDelete;
};
