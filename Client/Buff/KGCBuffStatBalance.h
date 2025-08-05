#pragma once

#include "KGCBuff.h"

class KGCBuffStatBalance : public KGCBuff
{
public:
	KGCBuffStatBalance(void);
	~KGCBuffStatBalance(void);

	//파티클, 에니메이션, 카툰 설정 등등
	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);

protected:
};
class KGCBuffConfusion : public KGCBuff
{
public:
	KGCBuffConfusion(void);
	~KGCBuffConfusion(void);

	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);
};
