#include "stdafx.h"
#include ".\KGCBuffRyanLifeForce.h"
#include "MyD3D.h"

KGCBuffLifeForce::KGCBuffLifeForce(void)
{
}

KGCBuffLifeForce::~KGCBuffLifeForce(void)
{
}

bool KGCBuffLifeForce::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
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

				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1);
				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2);
				g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
				pPlayer->ClearMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP);
			}
			else {
				bool bLv1 = true;
				int iMagic = EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1;

				if (pPlayer->IsMagicEffect(EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2))
				{
					bLv1 = false;
					iMagic = EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2;
				}

				if (bLv1 && (rand() % 100 >= 80))
				{
					pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.2f);
					pPlayer->SetSuper("", 0, 0);
				}
				else if (!bLv1 && rand() % 100 >= 50)
				{
					pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.3f);
					pPlayer->SetSuper("", 0, 0);
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