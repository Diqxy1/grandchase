#pragma once

#include "KGCBuff.h"

class KGCBuffSpit : public KGCBuff
{
public:
	KGCBuffSpit(void);
	~KGCBuffSpit(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
};

class KGCBuffDevilPet : public KGCBuff
{
public:
	KGCBuffDevilPet(void);
	~KGCBuffDevilPet(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
};
