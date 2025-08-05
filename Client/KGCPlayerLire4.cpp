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

        //�޺��߿� �� ���� ���� �����ϰ�...
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

            // Z Ű ������ ������ Ȱ �ȳ����� ����
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

        // 3�޺��� �� + z �ϸ� ���Ӽ�Ʈű!
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

        // 3~4�޺� ���̿� �� + z �ϸ� ���� ����
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
        // �뽬 ����
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
            ( !pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 31 )//����
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
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
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
            // Comment : ������ ������ ����, ������ �����ϸ� ����~
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

    if( pPlayer->y_Speed >= 0.0f ) // �ö� ��..
    {
        if( pPlayer->cFrame > 20 )
            pPlayer->cFrame = 20;
    }
    else // �����ö�..
    {
        if( pPlayer->cFrame < 20 )
            pPlayer->cFrame = 20;

        // ������ �����ӿ��� �����
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }

    return 0;
}

int KGCPlayerLire4::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		// ��� ��� �϶��� ��ų�б� ���¸� �ƹ��͵� �ƴѰ����� �Ѵ�. 
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
			// Comment : Wait���¿��� x �踦 ������ ���� �ߵ�!!
			//			 ���� �ƽ�Ʈ�� �ѵ�;; ������ �ϳ��� ������ �ϰ�, ���� ��Ÿ�� ���� ������Ѵ�..
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
                // Comment : ������ ���... Z+X �������� �ǰ� �Ѵ�;
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
			//KeyProc�ڿ��� ���� Frameó�� �ϴ� �κ��� �����Ƿ� Frame = 0���� ���ݴϴ�.
			//KeyProc�� �ƴϸ� Frame = -1�� �غ��ô�!

			pPlayer->NoCheckContact = 0;

			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;

			//�����ý��� ��� Ÿ�ݿ� ���������� �޺��� �̾���
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
		else        //Walk���¿��� Wait���·� ���ƿ���...
		{
			if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
				pPlayer->cFrame = 0;

			if( pPlayer->GetHP() > 0.0f )
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk�� �������� ����ϱ� ����..�־�� �ڵ���
			else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
			pPlayer->x_Speed = 0.0f;

		}
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //���� �ݺ��ϴ� ������..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerLire4::OnSpecial3( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame >= 26 && pPlayer->cFrame <= 115 )
        {
            //�޺��߿� �� ���� ���� �����ϰ�...

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

    if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1)        //���� �ݺ��ϴ� ������..
    {
        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
        pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
        pPlayer->m_bFrameLock = true;
    }

    return 0;
}
