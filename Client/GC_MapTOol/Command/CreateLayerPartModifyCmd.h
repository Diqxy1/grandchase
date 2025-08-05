#pragma once

#include "../stdafx.h"
#include "../KCreateLayerPartDlg.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CCreateLayerPartModifyCmd
	: public ICommand
{
public:
	CCreateLayerPartModifyCmd(void);
	virtual ~CCreateLayerPartModifyCmd(void);

	virtual void Undo();
	virtual void Redo();

	void SetModifyData(CLayerPart* pOldPart, CLayerPart* pNewPart);

private:
	CLayerPart  m_OldPart;
	CLayerPart  m_NewPart;
};
