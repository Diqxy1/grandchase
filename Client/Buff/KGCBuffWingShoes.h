#pragma once

#include "KGCBuff.h"

class KGCBuffWingShoes : public KGCBuff
{
public:
	KGCBuffWingShoes(void);
	~KGCBuffWingShoes(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
};

class KGCBuffMari1MagicShield : public KGCBuff
{
public:
	KGCBuffMari1MagicShield(void) {};
	~KGCBuffMari1MagicShield(void) {};

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};

class KGCBuffNemoMindControl : public KGCBuff
{
public:
	KGCBuffNemoMindControl(void);
	~KGCBuffNemoMindControl(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

private:
	int m_iWhichMagic;
};
