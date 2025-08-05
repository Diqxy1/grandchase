#include "stdafx.h"
#include ".\KGCBuffEagleSight.h"
#include "MyD3D.h"
#include "GCCameraOrtho.h"

KGCBuffEagleSight::KGCBuffEagleSight(void)
{
}

KGCBuffEagleSight::~KGCBuffEagleSight(void)
{
}

bool KGCBuffEagleSight::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		//원거리에서 보도록 초기화한다.
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			g_kCamera.SavePrevState();

			g_kCamera.InitCamera();
			g_kCamera.m_fWideMode = 7.0f;
			// 나 자신을 타겟으로 카메라를 잡는다.
			g_kCamera.SetTargetPlayer(iPlayerIndex_);
			//카메라 포지션을 잡는다.
			g_kCamera.InitCameraPosition();
		}
		return true;
	}

	return false;
}

bool KGCBuffEagleSight::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			g_kCamera.BackToPrevState();
			g_kCamera.InitCameraPosition();
		}
	}
	return false;
}