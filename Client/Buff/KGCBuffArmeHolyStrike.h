#pragma once

#include "KGCBuff.h"

class KGCBuffHolyStrike : public KGCBuff
{
public:
	KGCBuffHolyStrike(void);
	~KGCBuffHolyStrike(void);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};
