#include "stdafx.h"
//





#include "KGCPlayerCommon.h"
#include ".\kgcplayerlas3.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"
#include "GCCameraOrtho.h"


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCPlayerLas3::KGCPlayerLas3(void)
{
}

KGCPlayerLas3::~KGCPlayerLas3(void)
{
}

int KGCPlayerLas3::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Right_Dash) )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 10;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Left_Dash) )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 10;
			return 0;
		}

		if ( g_MyD3D->MyCtrl->X_Count != 0  && pPlayer->IsContact )
		{
			if ( g_MyD3D->MyCtrl->k_Up 
				&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SUPER_SPEED ) 
				&& false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_SKILL_LAS_MOVE_SPEEDUP ) )
			{
				g_KDSound.Play( "1501" );
				pPlayer->SetPlayerState( MID_SKILL_LAS_SUPER_SPEED );
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Up = false;

				return 0;
			}

			if ( g_MyD3D->MyCtrl->k_Down
				&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SHADOW ) 
				&& false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_SKILL_HIDE ) )
			{
				g_KDSound.Play( "1500" );
				pPlayer->SetPlayerState( MID_SKILL_LAS_SHADOW );
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Down = false;

				return 0;
			}

			if ( ( g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left )
				&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SUPER_JUMP )
				&& false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_SKILL_LAS_SUPER_JUMP ) )
			{
				g_KDSound.Play( "1499" );
				pPlayer->SetPlayerState( MID_SKILL_LAS3_SUPER_JUMP );
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;

				return 0;
			}
		}

		return KGCPlayerCommon::OnWait( pPlayer, true );
	}

	pPlayer->m_bJumpDashed = false;
	return KGCPlayerCommon::OnWait( pPlayer, false );
}

int KGCPlayerLas3::OnDash( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		return KGCPlayerCommon::OnDash( pPlayer, true );
	}

	if( pPlayer->cFrame == 26 )
	{
		pPlayer->SetPlayerState( MID_LAS3_WAIT );
		pPlayer->cFrame = 0;
		return 0;
	}
	return KGCPlayerCommon::OnDash( pPlayer, true );
}

int KGCPlayerLas3::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;
			pPlayer->SetPlayerState( MID_LAS3_JUMP_ATK );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Left_Dash) || ISPUSHED(k_Fresh_Right_Dash) )
		{
			ISPUSHED(k_Fresh_Left_Dash) = false;
			ISPUSHED(k_Fresh_Right_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_FRONT_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( pPlayer->m_bJumpDashed )
			return 0;

		if( ISPUSHED(k_Fresh_Up_Dash) )
		{
			ISPUSHED(k_Fresh_Up_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_UP_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Down_Dash) )
		{
			ISPUSHED(k_Fresh_Down_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_DOWN_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

        pPlayer->SetWindSpeed();

		return KGCPlayerCommon::OnJump( pPlayer, true );
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		if( pPlayer->cFrame > 24 )	//내려오는 점프 동작
			pPlayer->cFrame = 24;
	}
	else 
	{
		if( pPlayer->cFrame < 24 )
			pPlayer->cFrame = 24;
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}

	return 0;
}

int KGCPlayerLas3::OnJumpDash( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Right) )
			pPlayer->bIsRight = true;
		else if( ISPUSHED(k_Left) )
			pPlayer->bIsRight = false;

		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;
			pPlayer->SetPlayerState( MID_LAS3_JUMP_ATK );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down )
		{
			g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

			pPlayer->y_Speed = -0.03f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}

		return 0;
	}

	if( pPlayer->cFrame == 0 )
		pPlayer->m_bJumpDashed = true;

	if( pPlayer->uiMotion == MID_LAS3_UP_JUMPDASH )
	{
		if( pPlayer->cFrame == 1 )
		{
			pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fJumpYSpeed * 1.3f;
			return 0;
		}
	}

	//if( pPlayer->Now_Motion == MID_LAS3_FRONT_JUMPDASH )
	//{
	//	if( pPlayer->Frame == 1 )
	//	{
	//		if( pPlayer->IsRight == true )
	//			pPlayer->x_Speed = 0.04f;
	//		else
	//			pPlayer->x_Speed = -0.04f;
	//	}
	//}

	if( pPlayer->uiMotion == MID_LAS3_DOWN_JUMPDASH )
	{
		if( pPlayer->cFrame == 1 )
		{
			pPlayer->y_Speed = -0.05f;
			return 0;
		}
	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		pPlayer->m_bFrameLock = true;

	return 0;
}

int KGCPlayerLas3::OnCombo1( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;
#ifndef MATCH_BALLENCE_121217
			if( pPlayer->Next_Attack_Count > 0 )
#endif
				pPlayer->Next_Attack_Count = 255;
		}
		return 0;
	}
	return 0;
}

int KGCPlayerLas3::OnCombo2( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;
#ifndef MATCH_BALLENCE_121217
			if( pPlayer->Next_Attack_Count > 0 )
#endif
				pPlayer->Next_Attack_Count = 255;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Up_Dash) )
		{
			ISPUSHED(k_Fresh_Up_Dash) = false;
			pPlayer->cFrame = 0;
			pPlayer->SetPlayerState( MID_LAS3_ATK3_2 );
			return 0;
		}
		return 0;
	}
	return 0;
}

int KGCPlayerLas3::OnCombo3( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( pPlayer->bIsRight == true )
		{
			if( ISPUSHED(k_Fresh_Right_Dash) )
			{				
				ISPUSHED(k_Fresh_Right_Dash) = false;
				if( pPlayer->Next_Attack_Count > 0 )
					pPlayer->Next_Attack_Count = 255;
			}

			if( ISPUSHED(k_Fresh_Left_Dash) )
			{
				ISPUSHED(k_Fresh_Left_Dash) = false;
				pPlayer->bIsRight = false;
				if( pPlayer->Next_Attack_Count > 0 )
					pPlayer->Next_Attack_Count = 255;
			}
		}

		if( pPlayer->bIsRight == false )
		{
			if( ISPUSHED(k_Fresh_Right_Dash) )
			{				
				ISPUSHED(k_Fresh_Right_Dash) = false;
				pPlayer->bIsRight = true;
				if( pPlayer->Next_Attack_Count > 0 )
					pPlayer->Next_Attack_Count = 255;
			}

			if( ISPUSHED(k_Fresh_Left_Dash) )
			{
				ISPUSHED(k_Fresh_Left_Dash) = false;
				if( pPlayer->Next_Attack_Count > 0 )
					pPlayer->Next_Attack_Count = 255;
			}
		}
		return 0;
	}
	if( pPlayer->cFrame < 10 ) 
		pPlayer->cFrame = 11;
	return 0;
}

int KGCPlayerLas3::OnLanding( PLAYER* pPlayer, bool bKeyProc )
{
	return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
}

int KGCPlayerLas3::OnJumpAtk( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		//if( ISPUSHED(k_Right) )
		//{
		//	pPlayer->IsRight = true;
		//	pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
		//}
		//else if( ISPUSHED(k_Left) )
		//{
		//	pPlayer->IsRight = false;
		//	pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
		//}

		if( ISPUSHED(k_Fresh_Left_Dash) || ISPUSHED(k_Fresh_Right_Dash) )
		{
			ISPUSHED(k_Fresh_Left_Dash) = false;
			ISPUSHED(k_Fresh_Right_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_FRONT_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( pPlayer->m_bJumpDashed )
			return 0;

		if( ISPUSHED(k_Fresh_Up_Dash) )
		{
			ISPUSHED(k_Fresh_Up_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_UP_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Down_Dash) )
		{
			ISPUSHED(k_Fresh_Down_Dash) = false;
			pPlayer->SetPlayerState( MID_LAS3_DOWN_JUMPDASH );
			pPlayer->cFrame = 0;
			return 0;
		}

		return 0;
	}
	return 0;
}

int KGCPlayerLas3::OnSpecial1( PLAYER* pPlayer, bool bKeyProc )
{
	if( pPlayer->cFrame == 21 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerLas3::OnDashCombo1( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;			
			pPlayer->Next_Attack_Count = 255;
		}
		return 0;
	}
	return 0;
}

int KGCPlayerLas3::OnDashCombo2( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			ISPUSHED(k_Fresh_Punch) = false;
			pPlayer->Next_Attack_Count = 255;
		}
		return 0;
	}
	return 0;
}
