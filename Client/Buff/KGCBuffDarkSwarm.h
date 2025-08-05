#pragma once

#include "KGCBuff.h"

class KGCBuffDarkSwarm : public KGCBuff
{
public:
	KGCBuffDarkSwarm(void);
	~KGCBuffDarkSwarm(void);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};
