#include "stdafx.h"
#include ".\KGCBuffLireShotSpeedUp.h"

#include "KGCBuffManager.h"

KGCBuffLireShotSpeedUp::KGCBuffLireShotSpeedUp(void)
{
}

KGCBuffLireShotSpeedUp::~KGCBuffLireShotSpeedUp(void)
{
}

bool KGCBuffLireShotSpeedUp::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		return true;
	}

	return false;
}

bool KGCBuffLireShotSpeedUp::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		//g_pMagicEffect->SetMagicEffect( iPlayerIndex_, EGC_BUFF_COOLTIME_NOTIFIER_92, 30.0f );
	}
	return false;
}