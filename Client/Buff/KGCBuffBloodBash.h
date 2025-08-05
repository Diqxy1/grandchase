#pragma once

#include "KGCBuff.h"

class KGCBuffBloodBash : public KGCBuff
{
public:
	KGCBuffBloodBash(void);
	~KGCBuffBloodBash(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

protected:
	virtual void InitMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iWho = -1);
	virtual void FinalizeMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};

class KGCBuffAmyAbracadabra7 : public KGCBuff
{
public:
	KGCBuffAmyAbracadabra7(void) {};
	~KGCBuffAmyAbracadabra7(void) {};

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
};

class KGCBuffCancelation : public KGCBuff
{
public:
	KGCBuffCancelation(void) {};
	~KGCBuffCancelation(void) {};

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
};
