#include "stdafx.h"
#include ".\KGCBuffSpit.h"
#include "MyD3D.h"

KGCBuffSpit::KGCBuffSpit(void)
{
}

KGCBuffSpit::~KGCBuffSpit(void)
{
}

bool KGCBuffSpit::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID);

	g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = (short)(GC_FPS_LIMIT * fBuffTime);

	return false;
}

KGCBuffDevilPet::KGCBuffDevilPet(void)
{
}

KGCBuffDevilPet::~KGCBuffDevilPet(void)
{
}

bool KGCBuffDevilPet::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID);

	g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = (short)(GC_FPS_LIMIT * fBuffTime);

	g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticlePos("Devil_Haste_01", 0, 0.0f, 0.04f);
	g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticlePos("Devil_Haste_02", 0, 0.0f, 0.04f);
	g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticlePos("Devil_Haste_03", 0, 0.0f, 0.04f);
	g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticlePos("Devil_Haste_04", 0, 0.0f, 0.04f);
	g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticlePos("Devil_Haste_05", 0, 0.0f, 0.04f);

	return false;
}