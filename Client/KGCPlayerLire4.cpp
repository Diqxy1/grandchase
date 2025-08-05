#include "stdafx.h"
////


#include "KDInput.h"


#include "KGCPlayerCommon.h"
#include "KGCPlayerLire4.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"
#include "GCCameraOrtho.h"
#include "buff/KGCBuffManager.h"


#define LIMIT_JUMP_DOWN_ATK_COUNT (1)

KGCPlayerLire4::KGCPlayerLire4(void)
{
}

KGCPlayerLire4::~KGCPlayerLire4(void)
{
}

int KGCPlayerLire4::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch &&
            pPlayer->m_iJumpActionLimitCount < LIMIT_JUMP_DOWN_ATK_COUNT )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP_ATK );
            pPlayer->NoCheckContact = 30;
            pPlayer->x_Speed = 0.0f;
            pPlayer->y_Speed = 0.01f;
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        pPlayer->cFrame = 0;

    return 0;
}

int KGCPlayerLire4::OnComboAtk( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
        }

        //콤보중에 몸 각도 조절 가능하게...
        if( pPlayer->uiMotion == MID_LIRE4_ATK1_1 )
        {
            if( diks[g_MyD3D->MyCtrl->m_keyUp] )
            {
                pPlayer->Body_Angle += 0.1f;
                if( pPlayer->Body_Angle > 1.57f )
                    pPlayer->Body_Angle=1.57f;
            }
            else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
            {
                pPlayer->Body_Angle -= 0.1f;
                if( pPlayer->Body_Angle < -0.8f ) 
                    pPlayer->Body_Angle = -0.8f;
            }

            // Z 키 누르고 있으면 활 안나가게 수정
            if( diks[g_MyD3D->MyCtrl->m_keyAttack] )
            {
                if( pPlayer->cFrame == 7 || pPlayer->cFrame == 8 )
                {
                    pPlayer->Old_Frame = pPlayer->cFrame - 2;
                    pPlayer->cFrame--;
                    if( pPlayer->Next_Attack_Count < 8 )
                        pPlayer->Next_Attack_Count = 8;
                    pPlayer->Slow_Count = 2;

                    
                }

                if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash || g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                {
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH );
                    
                }


            }
        }

        if( pPlayer->Next_Attack_Count == 0 )
            return 0;

        // 3콤보에 ↑ + z 하면 섬머솔트킥!
        if( pPlayer->uiMotion == MID_LIRE4_ATK1_3 )
        {
            if( g_MyD3D->MyCtrl->k_Up )
            {
                g_MyD3D->MyCtrl->k_Up = false;
                pPlayer->SetPlayerState( MID_LIRE4_DOUBLE_ATK2 );
                pPlayer->cFrame = 0;
				pPlayer->Body_Angle = 0.0f;
                return 0;
            }
        }

        // 3~4콤보 사이에 ↓ + z 하면 쉐도 어택
        if( pPlayer->uiMotion == MID_LIRE4_ATK1_3 ||
            pPlayer->uiMotion == MID_LIRE4_ATK1_4 ||
            pPlayer->uiMotion == MID_LIRE4_ATK1_5 )
        {
            if( g_MyD3D->MyCtrl->k_Down )
            {
                g_MyD3D->MyCtrl->k_Down = false;
                pPlayer->SetPlayerState( MID_LIRE4_SHADOW_ATTACK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
            {
                if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1+4 )	//Critical
                {
                    g_MyD3D->m_pDamageManager->Add( DT_CRITICAL, pPlayer->m_iPlayerIndex );
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
                    pPlayer->cFrame = 0;
                }
                else //Double
                {
                    g_MyD3D->m_pDamageManager->Add( DT_DOUBLE, pPlayer->m_iPlayerIndex );
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_ATK );
                    pPlayer->cFrame = 0;
                }

                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else
                    pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count = 0;
                pPlayer->Ah_Sound(5);

                return 0;
            }
        }
        return 0;
    }
    return 0;
}

int KGCPlayerLire4::OnRun( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        // 대쉬 어택
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_LIRE4_DASH_ATK );
            pPlayer->cFrame = 0;
            pPlayer->x_Speed = 0.0f;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = 0.025f;
                else pPlayer->x_Speed = -0.025f;

                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
                pPlayer->y_Speed = 0.03f;
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;
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
        if( pPlayer->IsContact )
        {
            if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left )
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WALK );
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right )
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WALK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( ( ( pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Right ) ||
            ( !pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 31 )//정지
        {
            pPlayer->cFrame = 31;
            return 0;
        }
        return 0;
    }

    if( pPlayer->cFrame == 30 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 30 )
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerLire4::OnDash( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->bIsRight && 
            ( ( !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Left ) ||
              ( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Right ) ) )
        {
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) )
                g_MyD3D->MyCtrl->k_Fresh_Right = false;
            else
                g_MyD3D->MyCtrl->k_Fresh_Left = false;

			if ( pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MOVE_SPEEDUP ) || pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MOVE_SPEEDUP_DUN) )
				pPlayer->SetPlayerState( MID_SKILL_LIRE4_DASH_TO_BACK );
			else
				pPlayer->SetPlayerState( MID_LIRE4_DASH_BACK );
            pPlayer->cFrame = 0;
            return 0;
        }
        else if( !pPlayer->bIsRight && 
            ( ( !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Right ) ||
              ( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Left ) ) )
        {
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) )
                g_MyD3D->MyCtrl->k_Fresh_Left = false;
            else
                g_MyD3D->MyCtrl->k_Fresh_Right = false;

			if ( pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MOVE_SPEEDUP ) || pPlayer->IsMagicEffect(EGC_SKILLEFFECT_MOVE_SPEEDUP_DUN) )
				pPlayer->SetPlayerState( MID_SKILL_LIRE4_DASH_TO_BACK );
			else
				pPlayer->SetPlayerState( MID_LIRE4_DASH_BACK );
            pPlayer->cFrame = 0;
            return 0;
        }
        //else if( g_MyD3D->MyCtrl->k_Fresh_Left_Right )
        //{
        //    g_MyD3D->MyCtrl->k_Fresh_Left_Right = false;
        //    pPlayer->SetPlayerState( MID_LIRE4_DASH_BACK );
        //    pPlayer->cFrame = 0;
        //    return 0;
        //}
    }

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerLire4::OnDashBack( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_LIRE4_DASH_BACK_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }
    return 0;
}

int KGCPlayerLire4::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        //방향 바꾸기 가능하고 점프 어택으로 갈수 있음
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
            int iJumpAttackMotion = pPlayer->GetCurFormTemplate().JUMP_ATK;

            if( g_MyD3D->MyCtrl->k_Down &&
                pPlayer->m_iJumpActionLimitCount < LIMIT_JUMP_DOWN_ATK_COUNT )
            {
                iJumpAttackMotion = MID_LIRE4_JUMP_ATK2;
                ++pPlayer->m_iJumpActionLimitCount;
            }

            pPlayer->SetPlayerState( iJumpAttackMotion );
            pPlayer->cFrame = 0;

            if( pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0 )
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;

            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
        {
            pPlayer->Jump();
            g_MyD3D->MyCtrl->k_Up = false;
            pPlayer->m_iJumpPossibleFrame = 0 ;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up &&
            pPlayer->GetCurFormTemplate().DOUBLE_JUMP != -1 &&
            pPlayer->m_iJumpActionLimitCount < LIMIT_JUMP_DOWN_ATK_COUNT )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
            pPlayer->cFrame = 0;
            pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDoubleJumpYSpeed;
            return 0;
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

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f ) // 올라갈 때..
    {
        if( pPlayer->cFrame > 20 )
            pPlayer->cFrame = 20;
    }
    else // 내려올때..
    {
        if( pPlayer->cFrame < 20 )
            pPlayer->cFrame = 20;

        // 마지막 프레임에서 멈춘다
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }

    return 0;
}

int KGCPlayerLire4::OnWait( PLAYER* pPlayer, bool bKeyProc )
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

int KGCPlayerLire4::OnSpecial3( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame >= 26 && pPlayer->cFrame <= 115 )
        {
            //콤보중에 몸 각도 조절 가능하게...

            if( diks[g_MyD3D->MyCtrl->m_keyUp] )
            {
                pPlayer->Body_Angle += 0.03f;
                if( pPlayer->Body_Angle > 0.9f )
                    pPlayer->Body_Angle = 0.9f;
            }
            else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
            {
                pPlayer->Body_Angle -= 0.03f;
                if( pPlayer->Body_Angle < -0.8f )
                    pPlayer->Body_Angle = -0.8f;
            }
        }
    }

    return 0;
}

int KGCPlayerLire4::OnDoubleAttack2( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame > 38 )
    {
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }
    return 0;
}

int KGCPlayerLire4::OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc)
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
