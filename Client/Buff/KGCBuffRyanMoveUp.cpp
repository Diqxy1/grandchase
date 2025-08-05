#include "stdafx.h"
#include ".\KGCBuffRyanMoveUp.h"
#include "MyD3D.h"
#include "KGCBuffManager.h"

KGCBuffRyanMoveUp::KGCBuffRyanMoveUp(void)
	: m_iOrigianlRun(-1)
	, m_iOrigianlDash(-1)
	, m_iCharPromotion(-1)
{
}

KGCBuffRyanMoveUp::~KGCBuffRyanMoveUp(void)
{
}

bool KGCBuffRyanMoveUp::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		m_iOrigianlDash = g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH;
		m_iOrigianlRun = g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN;
		m_iCharPromotion = g_MyD3D->MyPlayer[iPlayerIndex_]->GetPromotion(GC_CHAR_RYAN);

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_RYAN_DASH;
			g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = MID_SKILL_RYAN_RUN;
		}

		return true;
	}

	return false;
}

bool KGCBuffRyanMoveUp::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL && m_iCharPromotion == g_MyD3D->MyPlayer[iPlayerIndex_]->GetPromotion(GC_CHAR_RYAN))
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = m_iOrigianlDash;
			g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = -m_iOrigianlRun;
		}

		return true;
	}
	return false;
}

bool KGCBuffRyanMoveUp::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (true == pBuffInst_->m_bPostCoolTime && m_iCharPromotion == g_MyD3D->MyPlayer[iPlayerIndex_]->GetPromotion(GC_CHAR_RYAN))
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = m_iOrigianlDash;
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = m_iOrigianlRun;
			}
		}
	}

	return true;
}