#include "stdafx.h"
#include ".\KGCBuffBackStep.h"
#include "MyD3D.h"

KGCBuffBackStep::KGCBuffBackStep(void)
{
}

KGCBuffBackStep::~KGCBuffBackStep(void)
{
}

bool KGCBuffBackStep::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->IsMyTeam(iPlayerIndex_))
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_BACK_STEP);

			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
			{
				g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOn(MS_BACK_STEP);
			}
		}
		else
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_BACK_STEP_ENEMY);

			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
			{
				g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOn(MS_BACK_STEP_ENEMY);
			}
		}

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle != NULL)
		{
			g_ParticleManager->DeleteSequence(g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle);
		}

		return true;
	}
	return false;
}

bool KGCBuffBackStep::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOff(MS_BACK_STEP | MS_BACK_STEP_ENEMY);

		if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
		{
			g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOff(MS_BACK_STEP | MS_BACK_STEP_ENEMY);
		}
		return true;
	}

	return false;
}

bool KGCBuffInvisible::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->IsMyTeam(iPlayerIndex_))
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_INVISIBLE);

			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
			{
				g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOn(MS_INVISIBLE);
			}
		}
		else
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_INVISIBLE_ENEMY);

			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
			{
				g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOn(MS_INVISIBLE_ENEMY);
			}
		}
		return true;
	}
	return false;
}

bool KGCBuffInvisible::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOff(MS_INVISIBLE | MS_INVISIBLE_ENEMY);

		if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject)
		{
			g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->MeshStateOff(MS_INVISIBLE | MS_INVISIBLE_ENEMY);
		}
		return true;
	}

	return false;
}