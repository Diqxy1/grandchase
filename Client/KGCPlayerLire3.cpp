#include "stdafx.h"
// KGCPlayerLire3.cpp: implementation of the KGCPlayerLire3 class.
//
//////////////////////////////////////////////////////////////////////

//


#include "KDInput.h"


#include "KGCPlayerCommon.h"
#include "KGCPlayerLire3.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"
#include "GCCameraOrtho.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//bool KGCPlayerLire3::isJumpAtked = false;
#define COMBOFRAME (16)
#define LIMITJUMPDOWNATKCOUNT (2)

KGCPlayerLire3::KGCPlayerLire3()
{	
}

KGCPlayerLire3::~KGCPlayerLire3()
{
}

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

int KGCPlayerLire3::OnWait(PLAYER* pPlayer, bool bKeyProc)
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
			//====================================================================================
			// 2009.02.19 : Jemitgge
			// Comment : Wait상태에서 x ↑를 누르면 형상 발동!!
			//			 무언가 아스트랄 한데;; 형상중 하나는 배웠어야 하고, 형상 쿨타임 따위 없어야한다..
			if (g_MyD3D->MyCtrl->k_Up
				&& (g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE1_MP_REGEN_SPEEDUP)
					|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE1_SHOT_SPEEDUP)
					|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE1_MOVE_SPEEDUP))
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MOVE_SPEEDUP, true)
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_SHOT_SPEEDUP, true)
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MP_REGEN_SPEED, true)
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MOVE_SPEEDUP_DUN, true)
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_SHOT_SPEEDUP_DUN, true)
				&& false == pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MP_REGEN_SPEED_DUN, true))
			{
				pPlayer->SetPlayerState( MID_SKILL_LIRE_IMAGE );
				pPlayer->Body_Angle = 0;
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Up = false;

				return 0;
			}
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
#ifdef MATCH_BALLENCE_121217			
                //====================================================================================
                // 2009.02.19 : Jemitgge
                // Comment : 엘프의 몸놀림... Z+X 눌렀을때 되게 한다;
                if ( g_MyD3D->MyCtrl->k_Select_Item && pPlayer->IsContact && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_DODGE ) )
                {
                    pPlayer->SetPlayerState( MID_SKILL_LIRE_DODGE_START );
                    pPlayer->Body_Angle = 0;
                    pPlayer->cFrame = 0;
                    pPlayer->Next_Attack_Count = 0;
                }
                else
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
                }
#else
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
#endif				
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
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerLire3::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( (pPlayer->bIsRight && ISPUSHED(k_Right)) || (!pPlayer->bIsRight && ISPUSHED(k_Left)) )
			{
				pPlayer->SetPlayerState(MID_LIRE3_DASHATK);
				pPlayer->cFrame = 0;
				return 0;
			}
			else if( (pPlayer->bIsRight && !ISPUSHED(k_Right)) || (!pPlayer->bIsRight && !ISPUSHED(k_Left)) )
			{
				pPlayer->SetPlayerState(MID_LIRE3_DASHATK2);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

		if( ISPUSHED(k_Up) )
		{
			if( pPlayer->IsContact )
			{
				if( pPlayer->bIsRight )
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunJumpXSpeed;
				else
					pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunJumpXSpeed;

				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fRunJumpYSpeed;
				pPlayer->cFrame = 0;
				ISPUSHED(k_Up) = false;
				return 0;
			}
		}

		if( ISPUSHED(k_Down) )
		{
			ISPUSHED(k_Down) = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 20;

//			pPlayer->SetPlayerState(pPlayer->m_PlayerTemplate.JUMP);
//			pPlayer->Frame = 0;
			return 0;
		}

		//if( (!pPlayer->IsRight && ISPUSHED(k_Right)) ||
		//	(pPlayer->IsRight && ISPUSHED(k_Left)) )
		//{
		//	if( pPlayer->IsRight )
		//		ISPUSHED(k_Right) = false;
		//	else
		//		ISPUSHED(k_Left) = false;

		//	pPlayer->EffectDust();
		//	pPlayer->IsRight = !pPlayer->IsRight;
		//	pPlayer->SetPlayerState( pPlayer->m_PlayerTemplate.RUN );
		//	pPlayer->Frame = 0;
		//	return 0;
		//}

		if( ( ( pPlayer->bIsRight && !ISPUSHED(k_Right) ) ||
			( !pPlayer->bIsRight && !ISPUSHED(k_Left) ) ) && 
			pPlayer->cFrame < 30 )
		{
			pPlayer->cFrame = 31;
			return 0;
		}

		return 0;
	}

	// 1~29 달리기
	// 30~41 서기

	if( pPlayer->cFrame == 29 )
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame >= 0 && pPlayer->cFrame < 30 )
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}

    pPlayer->SetWindSpeed();

	return 0;
}

int KGCPlayerLire3::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
	{
		if( ISPUSHED(k_Right) )
			pPlayer->bIsRight = true;
		else if( ISPUSHED(k_Left) )
			pPlayer->bIsRight = false;

		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( pPlayer->m_iJumpActionLimitCount <= LIMITJUMPDOWNATKCOUNT )
			{
				if( ISPUSHED(k_Up) )
				{
					pPlayer->SetPlayerState( MID_LIRE3_JUMPATK2 );

					if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
						pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
					pPlayer->cFrame = 0;
					return 0;
				}
				else if( ISPUSHED(k_Down) )
				{				
					pPlayer->SetPlayerState( MID_LIRE3_JUMPATK3 );

					if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
						pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
					pPlayer->cFrame = 0;
					return 0;
				}
				else
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );

					if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
						pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
					pPlayer->cFrame = 0;
					return 0;
				}
			}
			else pPlayer->cFrame = 40;
		}

		if (g_MyD3D->MyCtrl->X_Count != 0 && SiKGCRoomManager()->IsMonsterGameMode() == true)
		{
			//====================================================================================
			// 2009.02.18 : Jemitgge
			// Comment : 데미지 증폭기 배우고, 밑으로 공격하면 나감~
			if ((g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV1)
				|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV2)
				|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV3))
				&& g_MyD3D->MyCtrl->k_Down)
			{
				pPlayer->SetPlayerState(MID_SKILL_LIRE_DAMAGE_AMPLIFIER);
				if (pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;

				return 0;
			}
		}

		return KGCPlayerCommon::OnJump(pPlayer, bKeyProc);
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		if( pPlayer->cFrame > 21 )	//내려오는 점프 동작
			pPlayer->cFrame = 21;
	}
	else 
	{
		if( pPlayer->cFrame < 21 )
			pPlayer->cFrame = 21;
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}
	return 0;
}

int KGCPlayerLire3::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( pPlayer->Next_Attack_Count > 0 )
				pPlayer->Next_Attack_Count = 255;
		}

		if( pPlayer->uiMotion == MID_LIRE3_ATK1_1 )
		{
			//if( diks[DIK_UP] )
			if( diks[g_MyD3D->MyCtrl->m_keyUp] )
			{
				pPlayer->Body_Angle += 0.1f;
				if(pPlayer->Body_Angle > 1.57f) 
					pPlayer->Body_Angle=1.57f;
			}
			//else if( diks[DIK_DOWN] )
			else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->Body_Angle -= 0.1f;
				if( pPlayer->Body_Angle < -0.87f ) 
					pPlayer->Body_Angle = -0.87f;
			}

			//if( diks[DIK_Z] )
			if( diks[g_MyD3D->MyCtrl->m_keyAttack] )
			{
				if( pPlayer->cFrame == COMBOFRAME || pPlayer->cFrame == COMBOFRAME-1 )
				{
					pPlayer->Old_Frame = pPlayer->cFrame - 2;
					pPlayer->cFrame--;
					if( pPlayer->Next_Attack_Count < 8 )
						pPlayer->Next_Attack_Count = 8;
					pPlayer->Slow_Count = 3;                    
				}
                if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash || g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                {
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH );                    
                }
			}
			else
			{
				ISPUSHED(k_Down) = false;
				ISPUSHED(k_Up) = false;
			}
		}

		if( pPlayer->Next_Attack_Count == 0 )
			return 0;

		if( ISPUSHED(k_Up) )
		{
			if( pPlayer->uiMotion == MID_LIRE3_ATK1_3 )
			{
				if( pPlayer->cFrame < COMBOFRAME )
				{
					pPlayer->Next_Attack_Count = 0;
					pPlayer->SetPlayerState( MID_LIRE3_CRITICAL_DASHATK2 );
					pPlayer->cFrame = 0;
					return 0;
				}
			}
		}

		if( ISPUSHED(k_Down) )
		{
			if( pPlayer->uiMotion == MID_LIRE3_ATK1_3 )
			{
				if( pPlayer->cFrame < COMBOFRAME )
				{
					pPlayer->Next_Attack_Count = 0;
					pPlayer->SetPlayerState( MID_LIRE3_CRITICAL_MELEEATK );					
					pPlayer->cFrame = 0;
					return 0;
				}
			}			
		}

		if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash) )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
			{
				if( pPlayer->uiMotion == MID_LIRE3_ATK1_2 ) // Critical
					pPlayer->SetPlayerState( MID_LIRE3_CRITICALATK );
				else
					pPlayer->SetPlayerState( MID_LIRE3_DOUBLEATK );

				if( ISPUSHED(k_Fresh_Right_Dash) )
					pPlayer->bIsRight = true;
				else
					pPlayer->bIsRight = false;

				pPlayer->Next_Attack_Count = 0;
				pPlayer->cFrame = 0;

				return 0;
			}
		}
	}

	return 0;
}
//
//int KGCPlayerLire3::OnLand(PLAYER* pPlayer, bool bKeyProc)
//{
//	if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash) )
//	{
//		pPlayer->SetPlayerState( pPlayer->m_PlayerTemplate.RUN );
//		pPlayer->Frame = 0;
//	}
//	return 0;
//}

int KGCPlayerLire3::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( (pPlayer->bIsRight && ISPUSHED(k_Right)) || (!pPlayer->bIsRight && ISPUSHED(k_Left)) )
			{
				pPlayer->SetPlayerState(MID_LIRE3_DASHATK);
				pPlayer->cFrame = 0;
				return 0;
			}
			else if( (pPlayer->bIsRight && !ISPUSHED(k_Right)) || (!pPlayer->bIsRight && !ISPUSHED(k_Left)) )
			{
				pPlayer->SetPlayerState(MID_LIRE3_DASHATK2);
				pPlayer->cFrame = 0;
				return 0;
			}
		}
		if( ISPUSHED(k_Up) )
		{	
			if( pPlayer->IsContact )
			{			
				if(pPlayer->bIsRight)
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
				else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->cFrame = 0;				
				ISPUSHED(k_Up) = false;
				return 0;
			}			
			pPlayer->NoCheckContact = 0;			
		}
		if( ISPUSHED(k_Down) )
		{
			ISPUSHED(k_Down) = false;

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

		if( ( pPlayer->bIsRight && ISPUSHED(k_Right) ) || (!pPlayer->bIsRight && ISPUSHED(k_Left)) ) 
		{
			if( pPlayer->cFrame >= 10 )
			{
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().RUN );
				pPlayer->cFrame = 0;
				return 0;
			}
		}

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

		return 0;
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	return 0;
}

//int KGCPlayerLire3::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
//{
//	if( bKeyProc )
//	{
//		if( ( pPlayer->IsRight && ISPUSHED(k_Right) ) || (!pPlayer->IsRight && ISPUSHED(k_Left)) ) 
//		{
//			if( pPlayer->Frame >= 20 )
//			{
//				pPlayer->SetPlayerState( pPlayer->m_PlayerTemplate.RUN );
//				pPlayer->Frame = 0;
//				return 0;
//			}
//		}
//		return 0;
//	}
//
//	return 0;
//}

int KGCPlayerLire3::OnSpecial2(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( pPlayer->cFrame >= 20 && pPlayer->cFrame <= 31 )
		{
			if( ISPUSHED(k_Right) )
			{
				pPlayer->bIsRight = true;
				pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;		
				return 0;
			}
			else if( ISPUSHED(k_Left) )
			{
				pPlayer->bIsRight = false;
				pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
				return 0;
			}
			else if( ISPUSHED(k_Down) )
			{
				if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
					return 0;

				pPlayer->y_Speed = -0.01f;
				pPlayer->NoCheckContact = 20;
				return 0;
			}
			else
			{
				pPlayer->x_Speed = 0.0f;
				return 0;
			}
		}
	}
	return 0;
}

int KGCPlayerLire3::OnJumpAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
		return 0;

	if( pPlayer->Old_IsContact == false && pPlayer->IsContact == true && pPlayer->NoCheckContact == 0 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
		return 0;
	}

	if( pPlayer->uiMotion == MID_LIRE3_JUMPATK1 )
	{
		if( pPlayer->cFrame > 1 && pPlayer->cFrame < 4 )
		{
			pPlayer->y_Speed = 0.0f;
			return 0;
		}
	}
	else if( pPlayer->uiMotion == MID_LIRE3_JUMPATK2 )
	{
		if( pPlayer->cFrame > 11 && pPlayer->cFrame < 16 )
		{
			pPlayer->y_Speed = 0.0f;
			return 0;
		}
	}
	else if( pPlayer->uiMotion == MID_LIRE3_JUMPATK3 )
	{
		if( pPlayer->m_iJumpActionLimitCount > LIMITJUMPDOWNATKCOUNT + 1 )
		{
			pPlayer->cFrame = 40;
			pPlayer->SetPlayerState( MID_LIRE3_JUMP );
			return 0;
		}

		if( pPlayer->cFrame > 1 && pPlayer->cFrame <= 15 )
		{
			if( pPlayer->cFrame == 15 )
				pPlayer->m_iJumpActionLimitCount++;

			pPlayer->y_Speed = 0.0f;
		}
		else if( pPlayer->cFrame >= 16 && pPlayer->cFrame <= 18)
			pPlayer->y_Speed += 0.023f;
		else // Frame > 19
		{
			if( pPlayer->cFrame == 19 )
				pPlayer->y_Speed = 0.0f;

			if( pPlayer->cFrame > 19 && ISPUSHED(k_Fresh_Punch) && ISPUSHED(k_Down) )
			{
				if( pPlayer->m_iJumpActionLimitCount <= LIMITJUMPDOWNATKCOUNT )
				{
					pPlayer->m_iJumpActionLimitCount++;
					pPlayer->SetPlayerState( MID_LIRE3_JUMPATK3 );

					if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
						pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
					pPlayer->cFrame = 0;
					return 0;
				}				
			}
		}
		return 0;
	}

	return 0;
}

int KGCPlayerLire3::OnDashAtk1(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
		return 0;

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
		pPlayer->bIsRight = !pPlayer->bIsRight;

	return 0;
}

int KGCPlayerLire3::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( ISPUSHED(k_Up) )
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
		if( ISPUSHED(k_Fresh_Punch) )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			pPlayer->cFrame = 0;
			return 0;
		}
		if( ISPUSHED(k_Down) )
		{
            g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

			pPlayer->NoCheckContact = 20;
			return 0;
		}
	}

	return 0;
}


int KGCPlayerLire3::OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc)
{
	if (true == pPlayer->IsContact)
	{
		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
		pPlayer->cFrame = 0;
		pPlayer->Body_Angle = 0;
		return 0;
	}

	if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1)        //무한 반복하는 프레임..
	{
		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
		pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
		pPlayer->m_bFrameLock = true;
	}

	return 0;
}
