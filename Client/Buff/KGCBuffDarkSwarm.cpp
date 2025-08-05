#include "stdafx.h"
#include ".\KGCBuffDarkSwarm.h"
#include "MyD3D.h"
#include "KGCRoomManager.h"
#include "Controls.h"
#include "KDInput.h"

KGCBuffDarkSwarm::KGCBuffDarkSwarm(void)
{
	m_eCartoonTexture = CARTOON_BLACK;
}

KGCBuffDarkSwarm::~KGCBuffDarkSwarm(void)
{
}

bool KGCBuffDarkSwarm::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (g_MyD3D->EnableKeyInput() == false)
			return true;

		if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()
			&& !g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
			return true;

		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		if (pPlayer->m_kUserInfo.CheckState(GC_RUS_PLAYING) && pPlayer->m_cLife > 0 && pPlayer->GetHP() >= 0.0f &&
			pPlayer->uiMotion != pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
		{
#if defined( MATCH_BALANCE_2011_8 )
			if (diks[g_MyD3D->MyCtrl->m_keyAttack] && !Old_diks[g_MyD3D->MyCtrl->m_keyAttack])
#else
			if (g_MyD3D->MyCtrl->k_Fresh_Punch)
#endif
			{
				PLAYER::DamageInfo damage;
#if defined( MATCH_BALANCE_2011_8 )
				damage.SetDamage(-1.0f);
#else
				damage.SetDamage(-0.03f);
#endif

				if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_MEDUSA_ATK) {
					damage.SetDamage(0.0f);
				}

				damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_MONSTER, 0);
				damage.SetAbilityEffect(false, true);
				pPlayer->Change_HP(damage);
				pPlayer->Ah_Sound(0);

				if (rand() % 10 == 0)
				{
					pPlayer->Direct_Motion_Input(MID_COMMON_BIGDAMAGE);
				}
			}
		}

		return true;
	}

	return false;
}