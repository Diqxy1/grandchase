#include "stdafx.h"
#include ".\KGCBuffrenciaShield.h"

#include "Controls.h"
#include "Damage.h"

KGCBuffRenciaShield::KGCBuffRenciaShield(void)
	: m_iShieldType(EGC_EFFECT_RENCIA_SHIELD)
	, m_iShieldTime(0)
	, m_bSetShield(false)
{
}

KGCBuffRenciaShield::~KGCBuffRenciaShield(void)
{
}

bool KGCBuffRenciaShield::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID) && pBuffInst_)
	{
		if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_PET_CHOUCRING)
		{
			m_iShieldType = EGC_EFFECT_CHOUCRING_SHIELD;
		}
		else if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_PET_RENCIA)
		{
			m_iShieldType = EGC_EFFECT_RENCIA_SHIELD;
		}
		else if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_PET_DOKKAEBIE)
		{
			m_iShieldType = EGC_EFFECT_DOKKAEBIE_SHIELD;
		}

		m_iShieldTime = static_cast<int>(pBuffInst_->m_fRemainTime);
		m_bSetShield = false;
		return true;
	}

	return false;
}

bool KGCBuffRenciaShield::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return false;

	//  버프는 각자 계산해서 알려준다.
	if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		return false;

	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (!m_bSetShield && iPlayerIndex_ > -1) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetMagicEffect(m_iShieldType, static_cast<float>(m_iShieldTime));
			m_bSetShield = true;
		}
	}

	return true;
}

bool KGCBuffRenciaShield::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_)) {
		m_bSetShield = false;
		return true;
	}

	return false;
}