#include "stdafx.h"
#include ".\KGCBuffBombInfect.h"

#include "DamageManager.h"

KGCBuffBombInfect::KGCBuffBombInfect(void)
{
}

KGCBuffBombInfect::~KGCBuffBombInfect(void)
{
}

bool KGCBuffBombInfect::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		std::string strMesh;

		switch (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm)
		{
		case FORM_NEPHILIM:
			strMesh = "nephilim_bomb.p3m";
			break;
		case FORM_WOLF:
			strMesh = "wolf_bomb.p3m";
			break;
		case FORM_MDAN:
			strMesh = "mdan_bomb.p3m";
			break;
		default:
			strMesh = "player_bomb.p3m";
			break;
		}

		g_MyD3D->MyPlayer[iPlayerIndex_]->AddMeshObject(strMesh, "bomb.dds");

		CParticleEventSeqPTR pParticle;
		pParticle = g_ParticleManager->CreateSequence("BombFlash");

		if (pParticle == NULL)
			return false;

		pParticle->SetKillTime(fBuffTime);

		TRACE_INFO stTrace;
		stTrace.m_pvPos = &g_MyD3D->MyPlayer[iPlayerIndex_]->m_posDesc.HeadBombSpark;
		stTrace.m_fParticleTraceTime = pParticle->GetKillTime();                        //킬 타임을 설정했으므로 생성시키고 잊어버려도 된다
		pParticle->SetTrace(&stTrace);
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &g_MyD3D->MyPlayer[iPlayerIndex_]->bIsRight;
		pParticle->SetDirectInfo(&stDirect);
		pParticle->SetShow(&g_MyD3D->MyPlayer[iPlayerIndex_]->m_bRender);

		return true;
	}
	return false;
}

bool KGCBuffBombInfect::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (pBuffInst_->m_fRemainTime <= 0.0f)
	{
		return false;
	}

	pBuffInst_->m_fRemainTime -= GC_ELAPSED_TIME;

	if (pBuffInst_->m_fRemainTime <= 0.0f)
	{
		Destroy(iPlayerIndex_, pBuffInst_, false, false);

		if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			return false;

		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		std::vector< std::string > vBombList;
		vBombList.push_back("nephilim_bomb.p3m");
		vBombList.push_back("wolf_bomb.p3m");
		vBombList.push_back("mdan_bomb.p3m");
		vBombList.push_back("player_bomb.p3m");

		for (int i = 0; i < FORM_NUM; ++i)
		{
			if (NULL == pPlayer->GetMetaForm(i))
				continue;

			for (int j = 0; j < (int)vBombList.size(); ++j)
			{
				GCDeviceMeshP3M* pP3M = g_pGCDeviceManager2->CreateMeshP3M(vBombList[j]);
				if (pP3M)
				{
					pPlayer->GetMetaForm(i)->RemoveMesh(pP3M);
					pP3M->Release();
				}
			}
		}

		g_MyD3D->m_pDamageManager->MonsterAddWithLocate(DT_BOMBMAN_BOOM, 0, pPlayer->GetPosition().x, pPlayer->GetPosition().y + 0.3f, true);

		g_KDSound.Play("43");

		return false;
	}

	return true;
}