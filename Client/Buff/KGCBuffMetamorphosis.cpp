#include "stdafx.h"
#include ".\KGCBuffMetamorphosis.h"
#include "MyD3D.h"

KGCBuffMetamorphosis::KGCBuffMetamorphosis(void)
{
}

KGCBuffMetamorphosis::~KGCBuffMetamorphosis(void)
{
}

bool KGCBuffMetamorphosis::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			g_MyD3D->ActionForSpecialCheck(CMyD3DApplication::METAMORPHOSIS_WOLF);
			g_MyD3D->bEverSpecialCheck = true;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bNormalForm = false;
		}
	}

	return false;
}

bool KGCBuffMetamorphosis::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bNormalForm = true;
		return true;
	}

	return false;
}