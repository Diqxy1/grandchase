#pragma once

#include "KGCBuff.h"

class KGCBuffCaptainDisplay : public KGCBuff
{
public:
	KGCBuffCaptainDisplay(void);
	~KGCBuffCaptainDisplay(void);

	//파티클, 에니메이션, 카툰 설정 등등
	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);

protected:
};
