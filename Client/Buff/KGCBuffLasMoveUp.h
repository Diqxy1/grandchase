#pragma once

#include "KGCBuff.h"

class KGCBuffLasSpeedUp : public KGCBuff
{
public:
	KGCBuffLasSpeedUp(void);
	~KGCBuffLasSpeedUp(void);

	//파티클, 에니메이션, 카툰 설정 등등
	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

	//설정 해제
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

protected:
	bool bEnd;
};
