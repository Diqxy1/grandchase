#include "stdafx.h"
#include ".\KGCBuffArmeHolyStrike.h"
#include "MyD3D.h"

KGCBuffHolyStrike::KGCBuffHolyStrike(void)
{
}

KGCBuffHolyStrike::~KGCBuffHolyStrike(void)
{
}

bool KGCBuffHolyStrike::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		if (pPlayer->GetHP() <= 0.0f && pBuffInst_->m_bPostCoolTime == false &&
			pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) && pPlayer->cFrame > 19)
		{
			if (pPlayer->IsMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP)) {
				pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.1f * (float)(pBuffInst_->m_iLevel + 1));
				pPlayer->SetSuper("resurrection", 20, 0);
				pBuffInst_->m_fRemainTime = 0.1f;

				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_ARME_HOLYSTRIKE_LIFEKEEP, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_ARME_HOLYSTRIKE_LIFEKEEP);
				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP);
			}
			else {
				bool bLv1 = true;
				int iMagic = EGC_EFFECT_ARME_HOLYSTRIKE_LIFEKEEP;

				if (bLv1 && (rand() % 100 >= 70))
				{
					pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.3f);
					pPlayer->SetSuper("", 0, 0);
					pPlayer->SetMagicEffect(EGC_EFFECT_ARME_HOLYSTRIKE_LIFEKEEP_COOL, 60);
				}

				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, iMagic, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP);
				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
			}
		}

		return true;
	}
	return false;
}