#include "stdafx.h"
#include ".\KGCBuffLireMpRegenUp.h"
#include "MyD3D.h"
#include "KGCBuffManager.h"

KGCBuffLireMpRegenUp::KGCBuffLireMpRegenUp(void)
{
}

KGCBuffLireMpRegenUp::~KGCBuffLireMpRegenUp(void)
{
}

bool KGCBuffLireMpRegenUp::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		//g_pMagicEffect->SetMagicEffect( iPlayerIndex_, EGC_BUFF_COOLTIME_NOTIFIER_93, 20.0f );

		return true;
	}
	return false;
}