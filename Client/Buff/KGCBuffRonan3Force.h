#pragma once

#include "KGCBuff.h"

class KGCBuffRonan3Force : public KGCBuff
{
public:
	KGCBuffRonan3Force(void);
	~KGCBuffRonan3Force(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);

	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

	virtual std::wstring GetHudString(KGCBuffInstance* pBuffInst_, int iStringIndex_);
	virtual int GetHudStringNum(KGCBuffInstance* pBuffInst_) { return 2; }
};
