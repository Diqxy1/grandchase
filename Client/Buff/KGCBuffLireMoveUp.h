#pragma once

#include "KGCBuff.h"

class KGCBuffLireMoveUp : public KGCBuff
{
public:
	KGCBuffLireMoveUp(void);
	~KGCBuffLireMoveUp(void);

	//파티클, 에니메이션, 카툰 설정 등등
	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);

	//설정 해제
	virtual void DestroyBuffEffect(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_);

protected:
};
