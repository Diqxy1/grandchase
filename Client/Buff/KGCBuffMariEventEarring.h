#pragma once

#include "KGCBuff.h"
#include "KncP2PLib/KncCommonDefine.h"
#include "GCUtil\GCRandomObj.h"

class KGCBuffMariEventEarring : public KGCBuff
{
public:
	KGCBuffMariEventEarring(void);
	~KGCBuffMariEventEarring(void);

	//파티클, 에니메이션, 카툰 설정 등등
	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */);
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

	void SetBuffEffectHP(int iPlayerIndex_);
	void SetBuffEffectMP(int iPlayerIndex_);

protected:
	int                 m_iPeriodTime;
	float               m_fHPRecovery;
	float               m_fMPRecovery;
	int                 m_iProbability;

	std::map<int, KncStopWatch>        m_mapStopWatch;
	static GCUTIL::GCRand_Int   ms_kRandom;
};
