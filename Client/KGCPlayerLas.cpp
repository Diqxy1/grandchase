#include "stdafx.h"
// KGCPlayerLas.cpp: implementation of the KGCPlayerLas class.
//
//////////////////////////////////////////////////////////////////////
//
#include "MyD3D.h"

#include "KDInput.h"


#include "KGCPlayerLas.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
//
#include "Stage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
float KGCPlayerLas::bodyAngle = 0.0f;

KGCPlayerLas::KGCPlayerLas()
{

}

KGCPlayerLas::~KGCPlayerLas()
{

}


//Elesis의 달리기와 유사하나..
//정확한 수치 조절을 위해 밖으로 때냄
int KGCPlayerLas::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			if(pPlayer->bIsRight)
				pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
			else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

			pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);			
			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Up = false;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down)
		{
			g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 40;
			return 0;
		}
		if(pPlayer->IsContact)
		{		
			if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left)
			{
				pPlayer->bIsRight = false;
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
				pPlayer->cFrame = 0;
				return 0;
			}

			if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right)
			{
				pPlayer->bIsRight = true;
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

		if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
			( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) ) && pPlayer->cFrame < 20)		//정지
		{
			pPlayer->cFrame = 20;
			return 0;
		}		

		return 0;
	}

	if(pPlayer->cFrame == 19)		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame < 20)
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}
	else
	{
	}

    pPlayer->SetWindSpeed();
	return 0;
}

int KGCPlayerLas::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left && !g_MyD3D->MyCtrl->k_Right)
		{
			pPlayer->bIsRight = false;
			return 0;
		}
		if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right && !g_MyD3D->MyCtrl->k_Left)
		{
			pPlayer->bIsRight = true;
			return 0;
		}
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			//if( pPlayer->y_Speed > 0 && diks[DIK_DOWN] )
			if( pPlayer->y_Speed > 0 && diks[g_MyD3D->MyCtrl->m_keyDown] )
				pPlayer->SetPlayerState(MID_LAS_DOUBLEJUMP_ATK);
			else 
				pPlayer->SetPlayerState(MID_LAS_JUMPATK);
			pPlayer->cFrame = 0;
		}
		if ( g_MyD3D->MyCtrl->k_Fresh_Down_Dash && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DROPPING ) )
		{
			pPlayer->SetPlayerState( MID_SKILL_LAS_DROPPING );
			pPlayer->cFrame = 0;
		}

		return 0;
	}
	else
	{
		//마지막 프레임 유지
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
			pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame()-2;
		return 0;
	}
}

int KGCPlayerLas::OnDashCombo1(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( pPlayer->uiMotion != MID_LAS_DASHATK1 )
		{		
			if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
			{
				pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
				pPlayer->cFrame = 0;

				if( pPlayer->uiMotion == MID_LAS_DASHATK4 )
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_ATK );
					//g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
				}
				else 
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
					//g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
				}
				
				return 0;
			}
		}

		if(g_MyD3D->MyCtrl->k_Fresh_Punch)
		{
			if( pPlayer->uiMotion != MID_LAS_DASHATK4)
			{			
				if( pPlayer->Next_Attack_Count > 0 )
				{
					pPlayer->Next_Attack_Count = 255;
				}
			}
		}
		

		return 0;
	}
	else
	{
		return 0;
	
	}
}

int KGCPlayerLas::OnEvasion(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		return 0;
	}
	else
	{
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)		//무한 반복하는 프레임..
			pPlayer->bIsRight = !pPlayer->bIsRight;
		return 0;
	}
}

int KGCPlayerLas::OnSpiningSlash( PLAYER* pPlayer, bool bKeyProc )
{
	if ( bKeyProc )
	{
		if ( pPlayer->cFrame > 39 || pPlayer->cFrame <= 15 || pPlayer->Slow_Count > 0 )
			return 0;

		if( diks[g_MyD3D->MyCtrl->m_keyLeft] )
		{
			pPlayer->vPos.x -= 0.02f;
			if ( pPlayer->uiMotion == MID_SKILL_LAS_SPININGSLASH_LV1 )
				pPlayer->bIsRight = false;
		}
		else if( diks[g_MyD3D->MyCtrl->m_keyRight] )
		{
			pPlayer->vPos.x += 0.02f;
			if ( pPlayer->uiMotion == MID_SKILL_LAS_SPININGSLASH_LV1 )
				pPlayer->bIsRight = true;
		}
	}

	return 0;
}

int KGCPlayerLas::OnThunderSlash( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->cFrame > 5 )
		return 0;

	if ( bKeyProc )
	{
		if( diks[g_MyD3D->MyCtrl->m_keyUp] )
		{
			bodyAngle += 0.1f;
			if( bodyAngle > 1.57f )
				bodyAngle = 1.57f;
		}
		else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
		{
			bodyAngle -= 0.1f;
			if( bodyAngle < -1.57f ) 
				bodyAngle = -1.57f;
		}
	}

	if ( bodyAngle >= 0.1f && bodyAngle < 1.1f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_THUNDER_SLASH_UP );
	}
	else if ( bodyAngle >= -0.1f && bodyAngle < 0.1f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_THUNDER_SLASH_MIDDLE );
	}
	else if ( bodyAngle >= -1.1f && bodyAngle < -0.1f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_THUNDER_SLASH_DOWN );
	}

	return 0;
}

int KGCPlayerLas::OnThunderSlashInit( PLAYER* pPlayer, bool bKeyProc )
{
	bodyAngle = 0.0f;

	return 0;
}

int KGCPlayerLas::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		// 대기 모션 일때는 스킬분기 상태를 아무것도 아닌것으로 한다. 
		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
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
				pPlayer->SetPlayerState( MID_SKILL_LAS_SUPER_JUMP );
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;

				return 0;
			}
		}

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			if ( g_MyD3D->MyCtrl->k_Fresh_Punch && ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_STUN_SURIKEN ) ) 
				&& !g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_SKILL_STUN_SURIKEN ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LAS_STUN_SURIKEN );
				pPlayer->cFrame = 0;
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
                if( g_kCamera.CheckJumpZoom() )
				    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			}
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
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

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 40;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			static DWORD m_dwFastAtkTime = timeGetTime();

			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount 
#ifdef MATCH_BALLENCE_121217
				&& timeGetTime() - m_dwFastAtkTime > 2000 )
#else
				&& timeGetTime() - m_dwFastAtkTime > 4000 )
#endif				
			{
				if( pPlayer->Next_Attack_Count != 255 )
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
					g_Hot_Count = 0;
					m_dwFastAtkTime = timeGetTime();
				}
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
			}
			//KeyProc뒤에는 현재 Frame처리 하는 부분이 있으므로 Frame = 0으로 해줍니다.
			//KeyProc이 아니면 Frame = -1로 해봅시다!

			pPlayer->NoCheckContact = 0;

			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;

			//엘리시스의 경우 타격에 성공했을때 콤보가 이어짐
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			return 0;
		}
		pPlayer->x_Speed = 0.0f;

		if( g_MyD3D->MyCtrl->k_Right )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = true;
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = false;
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else        //Walk상태에서 Wait상태로 돌아오는...
		{
			if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
				pPlayer->cFrame = 0;

			if( pPlayer->GetHP() > 0.0f )
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk와 공용으로 사용하기 위해..넣어둔 코드임
			else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
			pPlayer->x_Speed = 0.0f;

		}

		return 0;
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerLas::OnDash( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Up )
		{   
			if( pPlayer->IsContact )
			{           
				if(pPlayer->bIsRight)
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
				else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Up = false;
				return 0;
			}           
			pPlayer->NoCheckContact = 0;
		}
		
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;

			if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
				|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
			{
#if defined( MATCH_BALANCE_2011_8 )
                if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DOUBLE_SURIKEN ) )
#else
				if ( pPlayer->IsContact == true && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DOUBLE_SURIKEN ) )
#endif
				{
					pPlayer->SetPlayerState( MID_SKILL_LAS_DOUBLE_SURIKEN );
					pPlayer->cFrame = 0;
				}
			}

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

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

		return 0;
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	return 0;
}

int KGCPlayerLas::OnNewDashAtk( PLAYER* pPlayer, bool bKeyProc )
{
	static bool bNextMotion = false;

	if(bKeyProc)
	{
		if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DASHATK_LV3) && pPlayer->cFrame <= 23 )
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Punch && diks[g_MyD3D->MyCtrl->m_keyUp])
			{
				bNextMotion = true;
			}
		}

		return 0;
	}

	if ( bNextMotion && pPlayer->cFrame >= 18 && pPlayer->cFrame <= 25 )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_DASHATK_LV3 );
		pPlayer->cFrame = 0;
		bNextMotion = false;
	}

	return 0;
}

int KGCPlayerLas::OnJumpAtk( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left && !g_MyD3D->MyCtrl->k_Right)
		{
			pPlayer->bIsRight = false;
		}
		if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right && !g_MyD3D->MyCtrl->k_Left)
		{
			pPlayer->bIsRight = true;
		}
		if( g_MyD3D->MyCtrl->k_Fresh_Punch && !pPlayer->IsContact && pPlayer->cFrame >= 15 
			&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SURIKEN_LV2 ) 
			&& pPlayer->uiMotion == MID_LAS_DOUBLEJUMP_ATK )
		{
			if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->SetPlayerState(MID_SKILL_LAS_JUMP_ATK_LV2);
				pPlayer->cFrame = 0;
			}
			
		}
		if ( g_MyD3D->MyCtrl->k_Fresh_Down_Dash && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DROPPING ) )
		{
			pPlayer->SetPlayerState( MID_SKILL_LAS_DROPPING );
			pPlayer->cFrame = 0;
		}
		return 0;
	}
	else
	{
		//마지막 프레임 유지
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
			pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame()-2;
		return 0;
	}
}

int KGCPlayerLas::OnNewComboAtk( PLAYER* pPlayer, bool bKeyProc )
{
	static DWORD m_dwEvasionTime = timeGetTime();
	static DWORD m_dwTrapTime = timeGetTime();

	if(bKeyProc)
	{
		if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
		{		
			if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
			{
				pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
				pPlayer->cFrame = 0;

				if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATKEND )
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_ATK );
					//g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
				}
				else 
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
					//g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
				}

				return 0;
			}

			if( g_MyD3D->MyCtrl->k_Up )
			{
				if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_FLY_COMBO_LV1 ) 
					|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_FLY_COMBO_LV2 ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_LAS_FLYATK_LV1 );
					pPlayer->cFrame = 0;
				}

				g_MyD3D->MyCtrl->k_Up = false;
				return 0;
			}						
#ifndef MATCH_BALLENCE_121217
			if ( g_MyD3D->MyCtrl->k_Down && timeGetTime() - m_dwEvasionTime > 2000)
			{
				if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DODGE_COMBO_LV1 ) 
					|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DODGE_COMBO_LV2 ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_LAS_EVASION_LV1 );
					pPlayer->cFrame = 0;
					m_dwEvasionTime = timeGetTime();
				}

				g_MyD3D->MyCtrl->k_Down = false;
				return 0;
			}
#endif
		}
		else
		{
#ifdef MATCH_BALLENCE_121217
			if ( pPlayer->IsContact && g_MyD3D->MyCtrl->k_Up 
#else
			if ( pPlayer->IsContact && g_MyD3D->MyCtrl->k_Up && pPlayer->cFrame < 7 
#endif
				&& ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_STUN_SURIKEN ) ) 
				&& !g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_SKILL_STUN_SURIKEN ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LAS_STUN_SURIKEN );
				pPlayer->cFrame = 0;

				return 0;
			}
#ifdef MATCH_BALLENCE_121217
			if ( pPlayer->IsContact && g_MyD3D->MyCtrl->k_Down 
				&& ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SPIKE_TRAP ) 
				|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_CANON_TRAP ) ) )
#else
			if ( pPlayer->IsContact && g_MyD3D->MyCtrl->k_Down && pPlayer->cFrame < 7 
				&& ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_SPIKE_TRAP ) 
				|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_CANON_TRAP ) )
				&& timeGetTime() - m_dwTrapTime > 3000 )
#endif
			{
				pPlayer->SetPlayerState( MID_SKILL_LAS_SPIKE_TRAP );
				pPlayer->cFrame = 0;
				m_dwTrapTime = timeGetTime();

				return 0;
			}
		}
#ifdef MATCH_BALLENCE_121217
        if ( g_MyD3D->MyCtrl->k_Down && timeGetTime() - m_dwEvasionTime > 2000)
        {
            if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DODGE_COMBO_LV1 ) 
                || g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DODGE_COMBO_LV2 ) )
            {
                if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left && !g_MyD3D->MyCtrl->k_Right)
                {
                    pPlayer->bIsRight = false;
                }
                if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right && !g_MyD3D->MyCtrl->k_Left)
                {
                    pPlayer->bIsRight = true;
                }

                pPlayer->SetPlayerState( MID_SKILL_LAS_EVASION_LV1 );
                pPlayer->cFrame = 0;
                m_dwEvasionTime = timeGetTime();
            }

            g_MyD3D->MyCtrl->k_Down = false;
            return 0;
        }
#endif
		if(g_MyD3D->MyCtrl->k_Fresh_Punch)
		{
#ifdef MATCH_BALLENCE_121217
            static DWORD m_dwFastAtkTime = timeGetTime();
            if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
                g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount 
                && timeGetTime() - m_dwFastAtkTime > 2000 )
            {
                if( pPlayer->Next_Attack_Count != 255 )
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
                    g_Hot_Count = 0;
                    m_dwFastAtkTime = timeGetTime();
                    return 0;
                }
            }
#endif
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )
			{			
				if( pPlayer->Next_Attack_Count > 0 )
				{
					pPlayer->Next_Attack_Count = 255;
					TRACE( L"PLAYERLAS : Motion %d, Next_A_Count = %d\n", (int)pPlayer->uiMotion, (int)pPlayer->Next_Attack_Count );
				}
			}
		}
		return 0;
	}	
	return 0;	
}

int KGCPlayerLas::OnDropping( PLAYER* pPlayer, bool bKeyProc )
{
	pPlayer->y_Speed = -0.06f;

	if(bKeyProc)
	{
		if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left && !g_MyD3D->MyCtrl->k_Right)
		{
			pPlayer->bIsRight = false;
		}
		if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right && !g_MyD3D->MyCtrl->k_Left)
		{
			pPlayer->bIsRight = true;
		}
	}

	if ( pPlayer->IsContact )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_DROPPING_TO_LAND );
		pPlayer->cFrame = 0;
	}
	else if ( pPlayer->cFrame == pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->SetPlayerState( MID_SKILL_LAS_DROPPING_TO_JUMP );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerLas::OnDodge( PLAYER* pPlayer, bool bKeyProc )
{
	static bool bIsPushDown = false;

	if(bKeyProc)
	{
		if ( g_MyD3D->MyCtrl->k_Down && ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LAS1_DODGE_COMBO_LV2 ) ) )
		{
			bIsPushDown = true;
		}

		if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 2 &&
			false == pPlayer->m_bJumpDashed )
		{
            D3DXVECTOR3 vTempPos = pPlayer->GetPosition();
			if( false == pPlayer->bIsRight )
			{
				vTempPos.x -= 0.35f;
			}
			else if( true == pPlayer->bIsRight )
			{
				vTempPos.x += 0.35f;
			}
			vTempPos.y += 0.25f;
			pPlayer->NoCheckContact = 25;
			pPlayer->m_bJumpDashed = true;

            pPlayer->SetDirectMove( vTempPos.x, vTempPos.y );

			if ( bIsPushDown && pPlayer->uiMotion == MID_SKILL_LAS_EVASION_LV1 )
			{
				pPlayer->SetPlayerState( MID_SKILL_LAS_EVASION_LV2 );
				pPlayer->cFrame = 0;

				bIsPushDown = false;
				pPlayer->m_bJumpDashed = false;

				return 0;
			}

			return 0;
		}

		if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			pPlayer->m_bJumpDashed = false;
			bIsPushDown = false;
		}

		return 0;
	}

	return 0;
}

int KGCPlayerLas::OnTradWarm( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if ( pPlayer->cFrame >= 10 && pPlayer->cFrame <= 28 
			&& g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState( MID_SKILL_LAS_TRADWARM_LV1_ATK );
			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;

			return 0;
		}
	}

	return 0;
}

int KGCPlayerLas::OnSuperJump( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->cFrame > 7 )
	{
		if(bKeyProc)
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Punch )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
				if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;
				return 0;
			}

			if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
			{
				pPlayer->Jump();
				g_MyD3D->MyCtrl->k_Up = false;
				pPlayer->m_iJumpPossibleFrame = 0 ;
				return 0;
			}

            if( g_MyD3D->MyCtrl->k_Up )
	        {
	            return KGCPlayerCommon::DoubleJumpFunc( pPlayer );			
	        }

			
			if( pPlayer->IsContact )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
				pPlayer->cFrame = 0;

				return 0;
			}
			else if ( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->cFrame = 0;

				return 0;
			}
		}

		return 0;
	}
	
	return 0;
}