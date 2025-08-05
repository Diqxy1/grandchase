#include "stdafx.h"
// KGCPlayerElesis2.cpp: implementation of the KGCPlayerElesis2 class.
//
//////////////////////////////////////////////////////////////////////
//




#include "KGCPlayerElesis2.h"
#include "DamageManager.h"
#include "Controls.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerElesis2::KGCPlayerElesis2()
{

}

KGCPlayerElesis2::~KGCPlayerElesis2()
{

}

int KGCPlayerElesis2::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->Next_Attack_Count > 0 )
			{
				pPlayer->Next_Attack_Count = 255;
			}			
		}
		if(pPlayer->Next_Attack_Count == 0)
			return 0;
		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
			{
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1+4 )	//Critical
				{
					g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
				}
				else //Double
				{
					g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
				}
				if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
					pPlayer->bIsRight = true;
				else pPlayer->bIsRight = false;

				pPlayer->Next_Attack_Count=0;
				pPlayer->Ah_Sound(5);
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
				pPlayer->cFrame = 0;

				return 0;
			}
		}
		return 0;
	}	
	return 0;
}
