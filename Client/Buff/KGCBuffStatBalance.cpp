#include "stdafx.h"
#include ".\KGCBuffStatBalance.h"
#include "MyD3D.h"

KGCBuffStatBalance::KGCBuffStatBalance(void)
{
}

KGCBuffStatBalance::~KGCBuffStatBalance(void)
{
}

bool KGCBuffStatBalance::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	//스텟 벨런스는 걸때마다 다시 능력치 셋팅을 해준다. 시간만 갱신하는게 아니다.

	int iWorstPlayer = -1;
	//능력치가 가장 낮은 아이를 찾는다.
	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
		{
			if (iWorstPlayer == -1)
			{
				iWorstPlayer = i;
			}

			//WorstPlayer 보다 더 못한놈이면.
			if (g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_ATK] + g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_DEF] + g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_HP]
	> g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK] + g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_DEF] + g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP])
			{
				// 니가 월스트 플레이어다!
				iWorstPlayer = i;
			}
		}
	}

	if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATK] = (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATK] - g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_ATK]) * 0.3f
			+ g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_ATK];

		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEF] = (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEF] - g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_DEF]) * 0.3f
			+ g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_DEF];

		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] = (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] - g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_HP]) * 0.3f
			+ g_MyD3D->MyPlayer[iWorstPlayer]->m_fAbility[ABILITY_HP];
	}

	return KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID);
}

KGCBuffConfusion::KGCBuffConfusion(void)
{
}

KGCBuffConfusion::~KGCBuffConfusion(void)
{
}

bool KGCBuffConfusion::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_);

	g_MyD3D->MyCtrl->InitDirectionKey();

	return true;
}