#include "stdafx.h"
#include ".\KGCBuffLireFireArrow.h"

KGCBuffLireFireArrow::KGCBuffLireFireArrow(void)
{
}

KGCBuffLireFireArrow::~KGCBuffLireFireArrow(void)
{
}

bool KGCBuffLireFireArrow::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		g_KDSound.Play("1291");

		return true;
	}

	return false;
}