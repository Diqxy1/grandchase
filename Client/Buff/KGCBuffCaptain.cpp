#include "stdafx.h"
#include ".\KGCBuffCaptain.h"
#include "MyD3D.h"

KGCBuffCaptain::KGCBuffCaptain(void)
{
}

KGCBuffCaptain::~KGCBuffCaptain(void)
{
}

bool KGCBuffCaptain::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEF] /= 3.0f; // 1/5배

	return KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID);
}

bool KGCBuffCaptain::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEF] *= 3.0f;

		return true;
	}

	return false;
}