#pragma once

#include "KGCBuff.h"

class KGCBuffMetamorphosis : public KGCBuff
{
public:
	KGCBuffMetamorphosis(void);
	~KGCBuffMetamorphosis(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);
};
