#include "stdafx.h"
#include ".\KGCBuffMariEventEarring.h"

#include "Controls.h"
#include "Damage.h"
#include "KGCRoomManager.h"

GCUTIL::GCRand_Int KGCBuffMariEventEarring::ms_kRandom;

const int nEffectSize = 3;
const std::string strHealEffect[nEffectSize] =
{
	"Mari_Earing_Heal_01",
	"Mari_Earing_Heal_02",
	"Mari_Earing_Heal_03",
};

KGCBuffMariEventEarring::KGCBuffMariEventEarring(void)
	:m_iPeriodTime(0)
	, m_fHPRecovery(0.0f)
	, m_fMPRecovery(0.0f)
	, m_iProbability(0)
{
}

KGCBuffMariEventEarring::~KGCBuffMariEventEarring(void)
{
}

bool KGCBuffMariEventEarring::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID) && pBuffInst_)
	{
		m_mapStopWatch.insert(std::pair<int, KncStopWatch>(iPlayerIndex_, KncStopWatch()));
		m_mapStopWatch[iPlayerIndex_].SetStopWatch(m_iPeriodTime);

		MAP_HEROITEM_ITER mit = g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.find(pBuffInst_->m_iBuffEnum);

		if (mit != g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.end())
		{
			std::vector<float> vecCondition;
			if (!g_pItemMgr->GetHeroItemCondition(mit->second, &vecCondition))
				return false;

			if (vecCondition.size() > 3)
			{
				m_iPeriodTime = static_cast<int>(vecCondition[0]);
				m_fHPRecovery = vecCondition[1];
				m_fMPRecovery = vecCondition[2];
				m_iProbability = static_cast<int>(vecCondition[3]);
			}
		}

		return true;
	}

	return false;
}

bool KGCBuffMariEventEarring::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
		return false;

	if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return false;

	//  버프는 각자 계산해서 알려준ㄴ다.
	if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		return false;

	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (m_mapStopWatch[iPlayerIndex_].IsStop())
		{
			srand((unsigned)time(NULL));
			int randomNum = ms_kRandom.rand() % 100;

			if (randomNum < m_iProbability)
			{
				SetBuffEffectHP(iPlayerIndex_);
				SetBuffEffectMP(iPlayerIndex_);

				for (int i = 0; i < nEffectSize; ++i)
				{
					g_ParticleManager->CreateSequence(strHealEffect[i], g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.x - 1.0f,
						g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.y - 0.35f, 0.5f);
				}
			}

			m_mapStopWatch[iPlayerIndex_].SetStopWatch(m_iPeriodTime * 1000);
		}
	}

	return true;
}

bool KGCBuffMariEventEarring::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	m_mapStopWatch.erase(iPlayerIndex_);
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
		return true;

	return false;
}

void KGCBuffMariEventEarring::SetBuffEffectHP(int iPlayerIndex_)
{
	if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return;

	if (m_fHPRecovery == 0.0f)
		return;

	PLAYER* pPlyer = g_MyD3D->MyPlayer[iPlayerIndex_];
	if (!pPlyer)
		return;

	float fHP_Point = pPlyer->m_fAbility[ABILITY_HP];

	if (pPlyer->GetHP() == fHP_Point)
		return;

	if (pPlyer->GetHP() == 0.0f)
		return;

	int nLevel = pPlyer->GetCurrentChar().iLevel;

	float fRecoveryRate = static_cast<float>(m_fHPRecovery - ((static_cast<float>(nLevel * 1.5) / 100)) * fHP_Point);

	if (fRecoveryRate > fHP_Point)
		fRecoveryRate = fHP_Point;

	const float fFivePerfect = 5.0f * fHP_Point / 100.0f;     // 총 HP의 5퍼센트 이하이면 5퍼센트 채운다.
	if (fRecoveryRate < fFivePerfect)
		fRecoveryRate = fFivePerfect;

	pPlyer->IncHP_inFrameMove(fRecoveryRate);

	return;
}

void KGCBuffMariEventEarring::SetBuffEffectMP(int iPlayerIndex_)
{
	if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return;

	if (m_fMPRecovery == 0.0f)
		return;

	PLAYER* pPlyer = g_MyD3D->MyPlayer[iPlayerIndex_];
	if (!pPlyer)
		return;

	if (pPlyer->GetMP() == 3.0f)
		return;

	if (pPlyer->GetHP() == 0.0f)
		return;

	float fRecoveryRate = 0.0f;

	int nLevel = pPlyer->GetCurrentChar().iLevel;

	fRecoveryRate = static_cast<float>((m_fMPRecovery - ((static_cast<float>(nLevel) * 1.5) / 100)) * 3.0f);

	if (fRecoveryRate > 3.0f)
		fRecoveryRate = 3.0f;
	if (fRecoveryRate < 0.15f)
		fRecoveryRate = 0.15f;

	pPlyer->IncMP_inFrameMove(fRecoveryRate);

	return;
}