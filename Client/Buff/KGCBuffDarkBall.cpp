#include "stdafx.h"
#include ".\KGCBuffDarkBall.h"
#include "MyD3D.h"
#include "Controls.h"

KGCBuffDarkBall::KGCBuffDarkBall()
{
	m_eCartoonTexture = CARTOON_BLACK;
	m_iCoolTime = 0;
}

KGCBuffDarkBall::~KGCBuffDarkBall(void)
{
}

bool KGCBuffDarkBall::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (!KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		return false;
	}

	if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return false;

	PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

	if (!pPlayer->IsLocalPlayer())
		return true;

	if (pPlayer->m_kUserInfo.CheckState(GC_RUS_PLAYING) && pPlayer->m_cLife > 0 && pPlayer->GetHP() >= 0.0f &&
		pPlayer->uiMotion != pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
	{
		//if( 0 < m_iCoolTime )
		//{
		//    --m_iCoolTime;
		//    return true;
		//}

		//if( g_MyD3D->MyCtrl->k_Fresh_Punch_Charging )
		if (g_MyD3D->MyCtrl->k_Fresh_Punch_Curse || g_MyD3D->MyCtrl->k_Fresh_Punch)
		{
			g_MyD3D->MyCtrl->k_Fresh_Punch_Curse = false;
			int iLastKillmePlayer = pPlayer->GetLastKillmePlayer();
			PLAYER::DamageInfo damage;
			damage.SetDamage(pPlayer->m_fAbility[ABILITY_HP] / -100.0f);
			damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_MONSTER, 0);
			damage.SetCriticalCheck(false);
			damage.SetAbilityEffect(false, true);
			damage.SetIgnoreDefence(true);
			pPlayer->Change_HP(damage);
			pPlayer->SetLastKillmePlayer(iLastKillmePlayer);
			//m_iCoolTime = DEFAULT_COOL;
		}
	}

	return true;
}