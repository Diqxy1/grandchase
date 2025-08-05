#include "stdafx.h"
// KGCFormWolf.cpp: implementation of the KGCPlayerArme2 class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerCommon.h"
#include "KGCFormWolf.h"
#include "DamageManager.h"
#include "Controls.h"

//
#include "GCCameraOrtho.h"

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCFormWolf::KGCFormWolf()
{

}

KGCFormWolf::~KGCFormWolf()
{

}

int KGCFormWolf::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
	if (bKeyProc)
	{
		if (g_MyD3D->MyCtrl->k_Select_Item)
		{
			pPlayer->ProcessMetamorphosisNormalForm();
			pPlayer->Direct_Motion_Input(pPlayer->GetCurFormTemplate().WAIT);
			pPlayer->cFrame = 0;
			pPlayer->GetMetaForm(FORM_NORMAL)->SetWorldMat(pPlayer->GetMetaForm(FORM_WOLF)->GetWorldMat());
			pPlayer->ClearMagicEffect(EGC_EFFECT_METAMORPHOSIS_WOLF);
			pPlayer->ClearMagicEffect(EGC_EFFECT_METAMORPHOSIS_WOLF_MOON_LIGHT);
			pPlayer->ClearMagicEffect(EGC_EFFECT_METAMORPHOSIS_WOLF_AURA_SELF);
			g_MyD3D->MyCtrl->k_Select_Item = false;
			return 0;
		}
		return KGCPlayerCommon::OnWait(pPlayer, true);
	}

	return KGCPlayerCommon::OnWait(pPlayer, false);
}

int KGCFormWolf::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Right) )
			pPlayer->bIsRight = true;
		else if( ISPUSHED(k_Left) )
			pPlayer->bIsRight = false;

		if( ISPUSHED(k_Fresh_Right_Dash) )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(MID_FORM_WOLF_DASH);				
			pPlayer->cFrame = 0;
			return 0;
		}
		if( ISPUSHED(k_Fresh_Left_Dash) )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(MID_FORM_WOLF_DASH);
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( ISPUSHED(k_Down) )
			{
				pPlayer->SetPlayerState( MID_FORM_WOLF_JUMP_ATK2 );
				pPlayer->cFrame = 0;
				pPlayer->NoCheckContact = 25;
				ISPUSHED(k_Down) = false;
				return 0;
			}
			else
			{
				pPlayer->SetPlayerState( MID_FORM_WOLF_JUMP_ATK1 );
				pPlayer->cFrame = 0;
				return 0;
			}
		}

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
		{
			pPlayer->Jump();
			g_MyD3D->MyCtrl->k_Up = false;
			pPlayer->m_iJumpPossibleFrame = 0 ;
			return 0;
		}
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		if( pPlayer->cFrame > 16 )	//내려오는 점프 동작
			pPlayer->cFrame = 16;
	}
	else 
	{
		if( pPlayer->cFrame < 16 )
			pPlayer->cFrame = 16;
		if(pPlayer->cFrame >= 35 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}

	return 0;
}

int KGCFormWolf::OnDash( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( pPlayer->cFrame > 16 )
		{
			if( ISPUSHED(k_Fresh_Right_Dash) )
			{
				pPlayer->bIsRight = true;
				pPlayer->SetPlayerState(MID_FORM_WOLF_DASH);
                if( g_kCamera.CheckDashZoom() )
				    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
				pPlayer->cFrame = 0;
				return 0;
			}
			else if( ISPUSHED(k_Fresh_Left_Dash) )
			{
				pPlayer->bIsRight = false;
				pPlayer->SetPlayerState(MID_FORM_WOLF_DASH);
                if( g_kCamera.CheckDashZoom() )
				    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
				pPlayer->cFrame = 0;
				return 0;
			}
		}
	}

	return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCFormWolf::OnCombo1( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if( pPlayer->uiMotion == MID_FORM_WOLF_ATK1 )
		{
			if( g_MyD3D->MyCtrl->k_Up )
			{
				if( pPlayer->cFrame <= 8 )
				{
					if ( ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_HP_TOTEM ) && false == pPlayer->IsMagicEffect( EGC_EFFECT_HP_TOTEM_COOL ) ) ||
						( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_MP_TOTEM ) && false == pPlayer->IsMagicEffect( EGC_EFFECT_MP_TOTEM_COOL ) ) ||
						( g_kGlobalValue.CheckTrainedSkill (pPlayer, SID_RYAN1_DP_TOTEM ) && false == pPlayer->IsMagicEffect( EGC_EFFECT_DP_TOTEM_COOL ) ) ||
						( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_AP_TOTEM ) && false == pPlayer->IsMagicEffect( EGC_EFFECT_AP_TOTEM_COOL ) ) )
					{
						pPlayer->SetPlayerState( MID_SKILL_RYAN_HP_TOTEM_FOR_WOLF );
						pPlayer->cFrame = 0;
						return 0;
					}
				}
			}
		}

		if( pPlayer->uiMotion != MID_FORM_WOLF_ATK1 )
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
			{
				pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
				pPlayer->cFrame = 0;

				pPlayer->SetPlayerState( MID_FORM_WOLF_CRITICAL );

				return 0;
			}
		}

		if(g_MyD3D->MyCtrl->k_Fresh_Punch)
		{
			//if( pPlayer->uiMotion != MID_FORM_WOLF_ATK4 )
			//{			
				//if( pPlayer->Next_Attack_Count > 0 ||
					//pPlayer->uiMotion != MID_FORM_WOLF_ATK1 )
				//{
					pPlayer->Next_Attack_Count = 255;
				//}
			//}
		}
		return 0;
	}	
	return 0;	
}

int KGCFormWolf::OnJumpAtk2( PLAYER* pPlayer, bool bKeyProc )
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if ( !pPlayer->IsContact )
		{
			pPlayer->SetPlayerState( MID_FORM_WOLF_JUMP_ATK2_JUMP );
		}
		
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCFormWolf::OnWolfFormEnd( PLAYER* pPlayer, bool bKeyProc )
{
	if( pPlayer->cFrame == 11 )
	{
		pPlayer->SetPlayerState( MID_FORM_WOLF_WAIT );
		pPlayer->cFrame = 0;
		//pPlayer->All_Latency_Equal();
	}

	return 0;
}
