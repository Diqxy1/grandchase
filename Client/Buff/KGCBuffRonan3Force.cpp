#include "stdafx.h"
#include ".\KGCBuffRonan3Force.h"
#include "MyD3D.h"

KGCBuffRonan3Force::KGCBuffRonan3Force(void)
{
	m_bAlwaysDescShow = true;
}

KGCBuffRonan3Force::~KGCBuffRonan3Force(void)
{
}

bool KGCBuffRonan3Force::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	pBuffInst_->m_iLevel = g_MyD3D->MyPlayer[iPlayerIndex_]->m_nForceNum;
	pBuffInst_->m_fRemainTime = 9999999.0f;
	//KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID );

	//if ( pBuffInst_->m_iLevel > 3 )
	//    pBuffInst_->m_iLevel = 3;

	for (std::vector<CParticleEventSeqPTR>::iterator vit = pBuffInst_->m_vecParticle.begin();
		vit != pBuffInst_->m_vecParticle.end();
		vit++)
	{
		g_ParticleManager->DeleteSequence(*vit);
	}

	if (pBuffInst_->m_iLevel == 0)
	{
		pBuffInst_->m_fRemainTime = 0.0f;
		return false;
	}

	pBuffInst_->m_vecParticle.clear();

	CParticleEventSeqPTR pRonan3Force, pRonan3Force1, pRonan3Force2;

	switch (pBuffInst_->m_iLevel)
	{
	case 1:
		pRonan3Force = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff01", 0, 0, 0.5f);
		pRonan3Force1 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff01_01", 0, 0, 0.5f);
		pRonan3Force2 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff01_02", 0, 0, 0.5f);

		break;
	case 2:
		pRonan3Force = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff02", 0, 0, 0.5f);
		pRonan3Force1 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff02_01", 0, 0, 0.5f);
		pRonan3Force2 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff02_02", 0, 0, 0.5f);

		break;
	case 3:
		pRonan3Force = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff03", 0, 0, 0.5f);
		pRonan3Force1 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff03_01", 0, 0, 0.5f);
		pRonan3Force2 = g_ParticleManager->CreateSequence("Ronan3_SPECIAL2-1_buff03_02", 0, 0, 0.5f);
		break;
	default:
		break;
	}

	if (pBuffInst_->m_iLevel > 0)
	{
		TRACE_INFO stTrace;
		stTrace.m_pvPos = ComputePosDesc(g_MyD3D->MyPlayer[iPlayerIndex_]->m_posDesc, EPT_BODY_CENTER);

		stTrace.m_fParticleTraceTime = 5.0f;
		if (pRonan3Force != NULL && g_ParticleManager->IsLiveInstance(pRonan3Force) == true)
		{
			pRonan3Force->SetTrace(&stTrace);
			pBuffInst_->m_vecParticle.push_back(pRonan3Force);
		}
		if (pRonan3Force1 != NULL && g_ParticleManager->IsLiveInstance(pRonan3Force1) == true)
		{
			pRonan3Force1->SetTrace(&stTrace);
			pBuffInst_->m_vecParticle.push_back(pRonan3Force1);
		}
		if (pRonan3Force2 != NULL && g_ParticleManager->IsLiveInstance(pRonan3Force2) == true)
		{
			pRonan3Force2->SetTrace(&stTrace);
			pBuffInst_->m_vecParticle.push_back(pRonan3Force2);
		}
	}
	return true;
}

bool KGCBuffRonan3Force::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_);

	return true;
}

std::wstring KGCBuffRonan3Force::GetHudString(KGCBuffInstance* pBuffInst_, int iStringIndex_)
{
	if (iStringIndex_ == 1)
	{
		return g_pkStrLoader->GetReplacedString(STR_ID_RONAN3_FORCE, "i", pBuffInst_->m_iLevel);
	}
	else
		return L"";
}