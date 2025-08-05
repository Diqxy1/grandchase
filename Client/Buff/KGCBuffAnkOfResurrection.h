#pragma once

#include "KGCBuff.h"

class KGCBuffAnkOfResurrection : public KGCBuff
{
public:
	KGCBuffAnkOfResurrection(void);
	~KGCBuffAnkOfResurrection(void);

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

private:
	bool bLv2;
};

class KGCBuffDioLifeKeep : public KGCBuff
{
public:
	KGCBuffDioLifeKeep(void);
	~KGCBuffDioLifeKeep(void);

	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
};

class KGCBuffZeroAmidSuper : public KGCBuff
{
public:
	KGCBuffZeroAmidSuper(void) {};
	~KGCBuffZeroAmidSuper(void) {};

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

private:
	bool m_bFirstCheckFatalDOG;
};

class KGCBuffZeroDSReflection : public KGCBuff
{
public:
	KGCBuffZeroDSReflection(void) {};
	~KGCBuffZeroDSReflection(void) {};

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);
};

class KGCBuffLeyHauntCurse : public KGCBuff
{
public:
	KGCBuffLeyHauntCurse(void) {};
	~KGCBuffLeyHauntCurse(void) {};

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

private:
	int m_iWho;
};