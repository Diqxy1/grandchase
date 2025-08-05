#include "stdafx.h"
#include "./KGCBuffWingShoes.h"
#include "MyD3D.h"
#include "Controls.h"
#include "Headup Display.h"

KGCBuffWingShoes::KGCBuffWingShoes()
{
}

KGCBuffWingShoes::~KGCBuffWingShoes(void)
{
}

bool KGCBuffWingShoes::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_WING] = static_cast<int>(fBuffTime) * GC_FPS_LIMIT;
		return true;
	}
	else
		return false;
}

bool KGCBuffMari1MagicShield::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (g_MyD3D->Get_MyPlayer() == iPlayerIndex_)
		{
			if (g_MyD3D->MyHead->Level < 0.1f && pBuffInst_->m_fRemainTime > 0.1f)
			{
				if (!pBuffInst_->m_bPostCoolTime)	// KWAK1 쿨타임 해제 방지
					pBuffInst_->m_fRemainTime = 0.1f;
			}
		}
	}
	return false;
}

KGCBuffNemoMindControl::KGCBuffNemoMindControl(void)
{
	m_iWhichMagic = EGC_EFFECT_MON_NEMOPHILLA_MIND;
}

KGCBuffNemoMindControl::~KGCBuffNemoMindControl(void)
{
}

bool KGCBuffNemoMindControl::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID)
{
	if (false == KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
		return false;

	int iMonType = 0;
	if (pBuffInst_->m_iBuffEnum == EGC_HELL_HOWLING_GHOST) {
		iMonType = MON_HELL_HAMMERMON_TRANS;
	}
	else if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_MON_NEMOPHILLA_MIND) {
		iMonType = MON_WEAK_GAUNT;
	}
	m_iWhichMagic = pBuffInst_->m_iBuffEnum;

	pBuffInst_->m_fReserve1 = 0.0f;
	int iIndex = iPlayerIndex_;
	PLAYER* pPlayer = g_MyD3D->MyPlayer[iIndex];

	pPlayer->ToggleRenderPlayer(true);
	pPlayer->m_bRender = false;
	pPlayer->m_bRenderQuestDrama = false;
	pPlayer->m_bCrashCheck = false;
	pPlayer->Item_Remain_Time[GC_GAME_ITEM_SUPER] = INT_MAX;

	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first))
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

		if (pMonster == NULL) continue;

		if (pMonster->m_bLive && pMonster->m_fHP > 0.0f && pMonster->GetPlayerTarget() == iIndex)
			pMonster->ReleaseTarget();
	}
	//pMonster->Release();
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iIndex);
	if (pMonster == NULL) {
		g_kMonsterManager.CreateMonster(iIndex);
		pMonster = g_kMonsterManager.GetMonster(iIndex);
	}
	pMonster->m_bLive = false;
	pMonster->Wait_Room(iMonType, pPlayer->GetPosition().x, pPlayer->GetPosition().y, pPlayer->GetIsRight(), true, pMonster->GetLevel());
	//pMonster->InitBuffer( pPlayer->GetPosition().x, pPlayer->GetPosition().y, pPlayer->GetIsRight() );

	pMonster->m_pObject->SetRender(true);

	if (m_iWhichMagic == EGC_EFFECT_MON_NEMOPHILLA_MIND)
		pMonster->SetMagicEffect(iIndex, EGC_EFFECT_MON_NEMOPHILLA_MIND_SIGN, fBuffTime, 1, -1, false);
	else
		pMonster->SetMagicEffect(iIndex, EGC_HELL_HOWLING_GHOST_SIGN, fBuffTime, 1, -1, false);

	pMonster->AddParticle("Nemo_Change_Boom_05", 0.0f, 0.18f);

	pPlayer->m_bNotTargeting = true;
	if (pPlayer->IsLocalPlayer())
	{
		g_kCamera.SetTargetMonster(iIndex);
		g_MyD3D->SetEnableKeyInput(false);
		pPlayer->m_bFrameLock = true;
	}

	return true;
}

bool KGCBuffNemoMindControl::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (false == KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
		return false;

	if (iPlayerIndex_ != -1)
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bFrameLock = true;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bCrashCheck = false;
		// 으?! 그 몹이 죽었따.
		MONSTER* pMonster = g_kMonsterManager.GetMonster(iPlayerIndex_);
		if (pMonster == NULL) return false;

		if (pMonster->m_fHP <= 0.0f)
		{
			pBuffInst_->m_fRemainTime = GC_ELAPSED_TIME;
			//죽었따는걸 요딴식으로 표시.
			pBuffInst_->m_fReserve1 = -1.0f;

			if (pMonster->m_bRender)
			{
				pMonster->EndMonster(false, true);
			}
		}
	}

	return true;
}

bool KGCBuffNemoMindControl::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (false == KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
		return false;

	int iIndex = iPlayerIndex_;
	PLAYER* pPlayer = g_MyD3D->MyPlayer[iIndex];
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iIndex);
	if (pMonster == NULL) return false;

	pPlayer->vPos = D3DXVECTOR3(pMonster->GetX(), pMonster->GetY(), pPlayer->vPos.z);
	pPlayer->ToggleRenderPlayer(false);
	pPlayer->m_bRender = true;
	pPlayer->m_bRenderQuestDrama = true;
	pPlayer->m_bCrashCheck = true;
	pPlayer->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 0;
	pPlayer->m_bNotTargeting = false;
	pPlayer->AddDamage(DT_HERO_NEMOPHILLA_MIND_CONTROL_END, 0);

	if (pPlayer->IsLocalPlayer())
	{
		g_kCamera.InitCamera();
		g_MyD3D->SetEnableKeyInput(true);
		pPlayer->m_bFrameLock = false;
		//죽었나?
		if (pBuffInst_->m_fReserve1 == -1.0f &&
			m_iWhichMagic == EGC_EFFECT_MON_NEMOPHILLA_MIND)
		{
			g_MyD3D->GetMyPlayer()->SetHP(0.0f);
			g_MyD3D->GetMyPlayer()->Change_Motion(MID_COMMON_DOWN_AND_STANDUP);
		}

		if (m_iWhichMagic == EGC_HELL_HOWLING_GHOST)
		{
			g_MyD3D->GetMyPlayer()->Change_Motion(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().WAIT);
		}
	}

	pMonster->SetHP(0.0f);
	pMonster->EndMonster(true, true, true);

	return true;
}