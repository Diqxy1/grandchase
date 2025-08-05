#include "stdafx.h"
#include ".\KGCBuffCaptainDisplay.h"
#include "MyD3D.h"

KGCBuffCaptainDisplay::KGCBuffCaptainDisplay(void)
{
}

KGCBuffCaptainDisplay::~KGCBuffCaptainDisplay(void)
{
}

bool KGCBuffCaptainDisplay::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATK] *= 1.5f;
	g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEF] *= 2.0f;

	g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] *= 2.0f;
	g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() * 2.0f);

	return KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID);
}