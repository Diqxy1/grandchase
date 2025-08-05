#pragma once

#include "KGCBuff.h"

class KGCBuffLifeForce : public KGCBuff
{
public:
	KGCBuffLifeForce(void);
	~KGCBuffLifeForce(void);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};
