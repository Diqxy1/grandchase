#include "stdafx.h"
#include ".\KGCBuffLireMoveUp.h"
#include "MyD3D.h"
#include "KGCBuffManager.h"

KGCBuffLireMoveUp::KGCBuffLireMoveUp(void)
{
}

KGCBuffLireMoveUp::~KGCBuffLireMoveUp(void)
{
}

bool KGCBuffLireMoveUp::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL)
		{
			switch (g_MyD3D->MyPlayer[iPlayerIndex_]->Extra_Char_Num)
			{
			case CID_LIRE1:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LIRE1_DASH;
				break;
			//case CID_LIRE2:
				//g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LIRE2_DASH;
				//break;
			case CID_LIRE3:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LIRE3_DASH;
				break;
			case CID_LIRE4:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LIRE4_DASH;
				break;
			default:
				break;
			}
		}

		return true;
	}

	return false;
}

void KGCBuffLireMoveUp::DestroyBuffEffect(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_)
{
	KGCBuff::DestroyBuffEffect(iPlayerIndex_, pBuffInst_, bOnDie_);

	switch (g_MyD3D->MyPlayer[iPlayerIndex_]->Extra_Char_Num)
	{
	case CID_LIRE1:
		g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LIRE_DASH;
		break;
	//case CID_LIRE2:
		//g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LIRE2_DASH;
		//break;
	case CID_LIRE3:
		g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LIRE3_DASH;
		break;
	case CID_LIRE4:
		g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LIRE4_DASH;
		break;
	default:
		break;
	}
}