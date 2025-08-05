#include "stdafx.h"
#include ".\KGCBuffBloodBash.h"
#include "MyD3D.h"

KGCBuffBloodBash::KGCBuffBloodBash(void)
{
	BuffParitcle Partlcle;
	Partlcle.m_strParticle = "BloodBash";
	Partlcle.m_fDirPosX = -0.15f;

	m_vecParticle.push_back(Partlcle);
}

KGCBuffBloodBash::~KGCBuffBloodBash(void)
{
}

bool KGCBuffBloodBash::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = false;

		return true;
	}
	return false;
}

bool KGCBuffBloodBash::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = true;

		return true;
	}

	return false;
}

void KGCBuffBloodBash::InitMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iWho)
{
	KGCBuff::InitMonsterStatus(iPlayerIndex_, pBuffInst_, fBuffTime, iWho);

	if (!SiKGCRoomManager()->IsMonsterGameMode())
		return;

	int iIndex = iPlayerIndex_;
	g_kMonsterManager.m_mapMonster[iIndex]->m_fHP_Recover = 0.0f;
}

void KGCBuffBloodBash::FinalizeMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (!SiKGCRoomManager()->IsMonsterGameMode() || iPlayerIndex_ < 0)
		return;

	int iIndex = iPlayerIndex_;
	g_kMonsterManager.m_mapMonster[iIndex]->m_fHP_Recover = 0.02f;

	KGCBuff::FinalizeMonsterStatus(iPlayerIndex_, pBuffInst_);
}

bool KGCBuffBloodBash::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = false;
		return true;
	}
	return false;
}

bool KGCBuffAmyAbracadabra7::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		PLAYER* pPlayer = g_MyD3D->GetPlayer(iPlayerIndex_);

		if (pPlayer->GetHP() > pPlayer->m_fAbility[ABILITY_HP] * 0.02f)
			pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.02f);

		return true;
	}
	return false;
}

bool KGCBuffCancelation::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	bool bAbnormal = false;
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		// 석화 & 빙결 & 잡힌거 풀기
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_STONECURSE || g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_PIXIE ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_ENT || g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_LICH ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_STONECURSE_BY_MEDUSA)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->Direct_Motion_Input(g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().WAIT);
			bAbnormal = true;
		}

		// 저주 풀기
		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_DARKBALL))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DARKBALL, true);
			bAbnormal = true;
		}

		// 컨퓨전 풀기!
		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION, true);
			bAbnormal = true;
		}
		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION2))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION2, true);
			bAbnormal = true;
		}

		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION_REINFORCEMENT))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION_REINFORCEMENT, true);
			bAbnormal = true;
		}

		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_ASSITANT_AURA_ALL_MANA_DOWN))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_ASSITANT_AURA_ALL_MANA_DOWN, true);
			bAbnormal = true;
		}

		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_LEY3_HAUNT_CURSE03))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LEY3_HAUNT_CURSE03, true);
			bAbnormal = true;
		}

		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_OVEJAR_40FORM_DEBUFF_ON_CHAR))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_OVEJAR_40FORM_DEBUFF_ON_CHAR, true);
			bAbnormal = true;
		}

		if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_MON_NEMOPHILLA_FALL))
		{
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_MON_NEMOPHILLA_FALL, true);
			bAbnormal = true;
		}

		// 감전 풀기!
		std::map< int, std::vector<CDamageInstance*> >::iterator itor;
		for (itor = g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end(); ++itor)
		{
			std::vector<CDamageInstance*>::iterator vit;
			for (vit = itor->second.begin(); vit != itor->second.end(); vit++)
			{
				CDamageInstance* pDamageOld = *vit;

				if (pDamageOld->m_HitWho == iPlayerIndex_)
				{
					if (pDamageOld->m_What == DT_HERO_KAMIKI_ATK3_2 ||
						pDamageOld->m_What == DT_SCORPIS_CRITICAL_INJECTION_STARTER ||
						pDamageOld->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK3_2 ||
						pDamageOld->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK3_2
						)
					{
						pDamageOld->m_Life = 1;
					}
				}
			}
		}
	}

	return bAbnormal;
}