#include "stdafx.h"
#include ".\KGCBuffLirePoisonArrow.h"

#include "Controls.h"
#include "Damage.h"
#include "KGCRoomManager.h"

KGCBuffLirePoisonArrow::KGCBuffLirePoisonArrow(void)
	:m_iPushCount(0)
{
}

KGCBuffLirePoisonArrow::~KGCBuffLirePoisonArrow(void)
{
}

bool KGCBuffLirePoisonArrow::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (bIsCrashPlayer)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = GC_FPS_LIMIT;
			m_iPushCount = 0;
		}

		return true;
	}

	return false;
}

bool KGCBuffLirePoisonArrow::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (SiKGCRoomManager()->IsMonsterGameMode() || (MAX_PLAYER_NUM < iPlayerIndex_))
			return true;

		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		if (g_MyD3D->Get_MyPlayer() == iPlayerIndex_)
		{
			g_MyD3D->ChargeLocalPlayerMana(-0.0035f);
		}

		if (g_MyD3D->MyCtrl->k_Fresh_Left || g_MyD3D->MyCtrl->k_Fresh_Right)
		{
			++m_iPushCount;
		}

		//====================================================================================
		// 2009.02.18 : Jemitgge
		// Comment : 이상하게.. o10인데 20프레임 마다로.. 되어있다..
		if ((o10 == 0 || o10 == 10) && g_MyD3D->Get_MyPlayer() == iPlayerIndex_)
		{
			int iLeftRightEffect = DT_JOYSTICK_LEFT_HIGH + ((int)o10 % 3);
			pPlayer->AddDamage(iLeftRightEffect, 0, true);
		}

		if (m_iPushCount >= 5)
		{
			//KGCBuffLirePoisonArrow::Destroy( iPlayerIndex_, pBuffInst_, false, false );

			pPlayer->ClearMagicEffect(EGC_SKILLEFFECT_POISONARROW_DOTDAMAGE);
			m_iPushCount = 0;
			return false;
		}
	}

	return true;
}

bool KGCBuffLirePoisonArrow::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = 0;
		}

		return true;
	}

	return false;
}