#include "stdafx.h"
//





#include "KGCPlayerJin2_2.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"


#include "Damage.h"

KGCPlayerJin2_2::KGCPlayerJin2_2()
{

}

KGCPlayerJin2_2::~KGCPlayerJin2_2()
{

}

int KGCPlayerJin2_2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			//if( !pPlayer->IsContact )
			//{
			//	if ( g_MyD3D->MyCtrl->k_Up )
			//	{                
			//		pPlayer->SetPlayerState( MID_JIN_DRAGON_KICK );
			//		g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			//		g_MyD3D->MyCtrl->k_Up = false;
			//		pPlayer->cFrame = 0;
			//		return 0;
			//	}
			//}
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Up )
		{
			if( pPlayer->IsContact )
			{
				if(pPlayer->bIsRight)
					pPlayer->x_Speed = 0.025f;
				else pPlayer->x_Speed = -0.025f;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->y_Speed = 0.03f;
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Up = false;
				return 0;
			}
			pPlayer->NoCheckContact = 0;
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
			pPlayer->NoCheckContact = 20;
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

		if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN )
		{
			if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
				( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 28 )//정지
			{
				pPlayer->cFrame = 29;
				return 0;
			}
		}

		return 0;
	}

	//무한 반복하는 프레임..
	if( pPlayer->cFrame == 28 )
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame < 28 )
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN )
	{
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left )
			{
				g_MyD3D->MyCtrl->k_Left = false;
			}

			if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right )
			{
				g_MyD3D->MyCtrl->k_Right = false;
			}

			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN2_2_RUN_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN2_2_RUN_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}

int KGCPlayerJin2_2::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		//방향 바꾸기 가능하고 점프 어텍으로 갈수 있음
		if( g_MyD3D->MyCtrl->k_Right )
		{
			pPlayer->bIsRight = true;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			pPlayer->bIsRight = false;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			//if ( g_MyD3D->MyCtrl->k_Up )
			//{                
			//	pPlayer->SetPlayerState( MID_JIN_DRAGON_KICK );                
			//}
			//else
			//{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
			//}   
			pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			g_MyD3D->MyCtrl->k_Up = false;
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
			pPlayer->cFrame = 0;
			return 0;
		}
		if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
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

		return 0;
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		//내려오는 점프 동작
		if( pPlayer->cFrame > 50 )
			pPlayer->cFrame = 50;
	}
	else 
	{
		if( pPlayer->cFrame > 50 )
			pPlayer->cFrame = 50;
		//무한 반복하는 프레임..
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
			pPlayer->m_bFrameLock = true;
	}
	return 0;
}

int KGCPlayerJin2_2::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 ||
				pPlayer->uiMotion == MID_JIN2_2_ATK3 )
            {
                pPlayer->Next_Attack_Count = 255;                
            }
			return 0;
        }

		if( g_MyD3D->MyCtrl->k_Down )
		{	
			if( pPlayer->cFrame <= 12 )
			{
				pPlayer->SetPlayerState( MID_JIN2_2_UNIQUE_EVASION );
				pPlayer->cFrame = 0;				
				return 0;
			}
		}

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) && 
			pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 &&
			pPlayer->cFrame <= 12 && ( pPlayer->m_fBurnningPoint >= 0.6f || pPlayer->m_bIsBurnningMode ))
		{	
			if(	pPlayer->GetIsRight() )
			{
				if( g_MyD3D->MyCtrl->k_Fresh_Left )
					pPlayer->SetIsRight(false);
			}
			else
			{
				if( g_MyD3D->MyCtrl->k_Fresh_Right )
					pPlayer->SetIsRight(true);
			}
			pPlayer->SetPlayerState( MID_JIN2_2_UNIQUE_HUJIN );				
			pPlayer->cFrame = 0;
			return 0;
		}

		if( pPlayer->uiMotion  == pPlayer->GetCurFormTemplate().ATK1 )
		{
			if( g_MyD3D->MyCtrl->k_Up )
			{	
				if( pPlayer->cFrame <= 8 )
				{
					if( pPlayer->m_fBurnningPoint >= 0.75f || pPlayer->m_bIsBurnningMode )
						pPlayer->SetPlayerState( MID_JIN2_2_UNIQUE_HANDWIND );
					else
						pPlayer->SetPlayerState( MID_JIN2_2_UNIQUE_HANDWIND_BP_EMPTY );
					pPlayer->cFrame = 0;				
					return 0;	
				}
			}	
		}

        if( pPlayer->uiMotion  != pPlayer->GetCurFormTemplate().ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_JIN2_2_ATK2:
            case MID_JIN2_2_ATK3:
            case MID_JIN2_2_ATK4:			
				pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                break;
			case MID_JIN2_2_ATK5:
				pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
                break;
            default:
                return 0;
            }

            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else pPlayer->bIsRight = false;

            pPlayer->Next_Attack_Count=0;
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }
    return 0;
}

int KGCPlayerJin2_2::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
			if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
				|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
			{
				if( pPlayer->m_bIsBurnningMode || pPlayer->m_fBurnningPoint >= 0.6f )
					pPlayer->SetPlayerState( MID_JIN2_2_UNIQUE_SPINKICK );
				else
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				
			}
			else
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				

			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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
			pPlayer->NoCheckContact = 20;
			return 0;
		}

		return 0;
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
		pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
	{
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}

int KGCPlayerJin2_2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if(  pPlayer->cFrame >= 5 && pPlayer->cFrame <= 15  )
			{
				pPlayer->m_iBranchSkill = SKILL_BRANCH_1;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}			
			else if(  15 < pPlayer->cFrame && pPlayer->cFrame <= 24 )
			{
				pPlayer->m_iBranchSkill = -1;
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_1 );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}
			else
				return 0;
		}

		if( g_MyD3D->MyCtrl->k_Up )
		{
			if(  pPlayer->cFrame >= 5 && pPlayer->cFrame <= 15  )
			{
				pPlayer->m_iBranchSkill = SKILL_BRANCH_2;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}			
			else if(  15 < pPlayer->cFrame && pPlayer->cFrame <= 24 )
			{
				pPlayer->m_iBranchSkill = -1;
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_2 );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}
			else
				return 0;
		}		
	}

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}


int KGCPlayerJin2_2::OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
			pPlayer->cFrame = 0;
		}
		else
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
			pPlayer->cFrame = 50;
			pPlayer->m_bFrameLock = true;
		}
	}
	return 0;
}

//int KGCPlayerJin2_2::OnDragonKick(PLAYER* pPlayer, bool bKeyProc)
//{
//	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
//	{
//		if( true == pPlayer->IsContact )
//		{
//			if( pPlayer->cFrame <= 11 )
//			{
//				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
//				pPlayer->cFrame = 0;
//			}			
//		}
//		else
//		{
//			pPlayer->SetPlayerState( MID_JIN_DRAGON_KICK_TO_JUMP );
//			pPlayer->cFrame = 50;
//			pPlayer->m_bFrameLock = true;
//		}
//	}
//	return 0;
//}

//int KGCPlayerJin2_2::OnDragonKickToJump(PLAYER* pPlayer, bool bKeyProc)
//{
//	if(bKeyProc)
//	{
//		//방향 바꾸기 가능하고 점프 어텍으로 갈수 있음
//		if( g_MyD3D->MyCtrl->k_Right )
//		{
//			pPlayer->bIsRight = true;
//		}
//		else if( g_MyD3D->MyCtrl->k_Left )
//		{
//			pPlayer->bIsRight = false;
//		}
//
//		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
//		{
//			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
//			pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
//			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
//			g_MyD3D->MyCtrl->k_Up = false;
//			pPlayer->cFrame = 0;
//			return 0;
//		}
//
//		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
//		{
//			pPlayer->bIsRight = true;
//			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
//			pPlayer->cFrame = 0;
//			return 0;
//		}
//		if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
//		{
//			pPlayer->bIsRight = false;
//			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
//			pPlayer->cFrame = 0;
//			return 0;
//		}
//		if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
//		{
//			pPlayer->Jump();
//			g_MyD3D->MyCtrl->k_Up = false;
//			pPlayer->m_iJumpPossibleFrame = 0 ;
//			return 0;
//		}
//		return 0;
//	}
//	if( pPlayer->y_Speed >= 0.0f )
//	{
//		//내려오는 점프 동작
//		if( pPlayer->cFrame > 50 )
//			pPlayer->cFrame = 50;
//	}
//	else 
//	{
//		if( pPlayer->cFrame > 50 )
//			pPlayer->cFrame = 50;
//		//무한 반복하는 프레임..
//		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
//			pPlayer->m_bFrameLock = true;
//	}
//	return 0;	
//}

int KGCPlayerJin2_2::OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( pPlayer->uiMotion == MID_JIN2_2_DASH_ATK_1 )
		{
			if( pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_1_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_1_TO_JUMP );
			pPlayer->cFrame = 0;
		}		
		else if( pPlayer->uiMotion == MID_JIN2_2_DASH_ATK_2 )
		{
			if( pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_2_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN2_2_DASH_ATK_2_TO_JUMP );
			pPlayer->cFrame = 0;
		}
		else
			return 0;
	}
	return 0;
}

int KGCPlayerJin2_2::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{		pPlayer->bIsRight = true;
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

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
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
			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			{
				if( pPlayer->Next_Attack_Count != 255 )
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
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

		if( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BURNNING_MODE ) //진
		{			
			if( g_MyD3D->MyCtrl->Burnning_X_Count == 10 && 
				!pPlayer->m_bIsBurnningMode && !pPlayer->m_bIsMovedAfterCharge )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().BURNNING_CHARGE);
				pPlayer->cFrame = 0;
			}
		}

		if ( g_MyD3D->MyCtrl->k_Fresh_Double_X && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_SUMMON_BONG ) )
		{
			g_MyD3D->MyCtrl->k_Fresh_Double_X = false;
			pPlayer->ProcessSwapWeapon();
			pPlayer->cFrame = 0;

			return 0;
		}

		return 0;
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}