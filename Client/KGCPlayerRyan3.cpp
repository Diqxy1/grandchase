#include "stdafx.h"

#include "MyD3D.h"
#include "KGCPlayerRyan.h"
#include "KGCPlayerRyan3.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"




KGCPlayerRyan3::KGCPlayerRyan3()
{

}

KGCPlayerRyan3::~KGCPlayerRyan3()
{

}

int KGCPlayerRyan3::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(MID_RYAN3_DASH_ATK);
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

        if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
            ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 31)//정지
        {
            pPlayer->cFrame = 32;
            return 0;
        }
        return 0;
        if( pPlayer->cFrame >= 32 )
        {
            if(g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
            if(g_MyD3D->MyCtrl->k_Fresh_Right_Dash)
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if(pPlayer->cFrame == 31)
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 31)
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerRyan3::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            if( g_MyD3D->MyCtrl->k_Up && pPlayer->cFrame >= 2 )
            {
                pPlayer->SetPlayerState(MID_RYAN3_JUMP_UP_ATK);
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
            }
            return 0;
        }        

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPlayer->bIsRight = true;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            return 0;
        }
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPlayer->bIsRight = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
        {
            pPlayer->Jump();
            g_MyD3D->MyCtrl->k_Up = false;
            pPlayer->m_iJumpPossibleFrame = 0 ;
            return 0;
        }
        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f )
    {
        //내려오는 점프 동작
        if( pPlayer->cFrame > 20 )
            pPlayer->cFrame = 20;
    }
    else 
    {
        if( pPlayer->cFrame < 20 )
            pPlayer->cFrame = 20;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerRyan3::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( (true == g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight == true) ||
                (true == g_MyD3D->MyCtrl->k_Right && pPlayer->bIsRight == false) )
            {
                pPlayer->SetPlayerState(MID_RYAN3_BACKDASH_ATK);
            }
            else
            {
                if( pPlayer->IsContact ) {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
                } else {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
                }
            }
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
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

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN3_DASH_END_WAIT );
        }
        else
        {
            pPlayer->SetPlayerState( MID_RYAN3_DASH_END_JUMP );
        }
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerRyan3::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if ( g_MyD3D->MyCtrl->k_Skill_Key && pPlayer->IsContact ) {
            if ( g_MyD3D->MyCtrl->k_Down
                && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_WILD_HOWLING ) 
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_WILD_HOWLING ) )
            {
                pPlayer->SetPlayerState( MID_SKILL_RYAN_WILD_HOWLING );
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Select_Item = false;
                g_MyD3D->MyCtrl->k_Down = false;

                return 0;
            }
        }

        if( pPlayer->uiMotion == MID_RYAN3_ATK1 ) {
            if( g_MyD3D->MyCtrl->k_Up )
            {	
                if( pPlayer->cFrame <= 8 )
                {
                    int uiMotion = -1;
                    if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_HP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_HP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN3_HP_TOTEM;
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_MP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_MP_TOTEM_COOL ))
                    {
                        uiMotion = MID_SKILL_RYAN3_MP_TOTEM;
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_DP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN3_DP_TOTEM;
                    }
                    else if (g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_AP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_AP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN3_AP_TOTEM;
                    }

                    if ( uiMotion != -1 )
                    {
                        pPlayer->SetPlayerState( uiMotion );
                        pPlayer->cFrame = 0;
                        return 0;
                    }
                }
                g_MyD3D->MyCtrl->k_Up = false;
            } else if( g_MyD3D->MyCtrl->k_Down ) {
                g_MyD3D->MyCtrl->k_Down = false;
                if( pPlayer->cFrame <= 8 ) {
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT );
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }            
        }

        if( pPlayer->uiMotion != MID_RYAN3_LEVEL_ATK1 &&
            ( (pPlayer->bIsRight == false && g_MyD3D->MyCtrl->k_Fresh_Right) ||
            (pPlayer->bIsRight == true && g_MyD3D->MyCtrl->k_Fresh_Left) ) )
        {
            pPlayer->SetPlayerState(MID_RYAN3_LEVEL_ATK1);
            pPlayer->Next_Attack_Count=0;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
                return 0;
            }
        }

        if( pPlayer->Next_Attack_Count == 0 && pPlayer->uiMotion != MID_RYAN3_ATK1 && pPlayer->uiMotion != MID_RYAN3_ATK4 )
            return 0;

        if( pPlayer->uiMotion  != MID_RYAN3_ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_RYAN3_ATK2:
            case MID_RYAN3_ATK3:
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                break;
            case MID_RYAN3_ATK4:
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

int KGCPlayerRyan3::OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_RYAN3_JUMP_DASH_ATK_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_RYAN3_JUMP_DASH_ATK_END_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerRyan3::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_RYAN3_DASH_ATK_WAIT );
        else
            pPlayer->SetPlayerState( MID_RYAN3_DASH_ATK_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerRyan3::OnDashEnd( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {   
        if( g_MyD3D->MyCtrl->k_Up )
        {   
            if( pPlayer->IsContact )
            {           
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed + 0.005f;
                else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed + 0.005f;

                pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }           
            pPlayer->NoCheckContact = 0;
        }

        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->uiMotion == MID_RYAN3_DASH_END_JUMP )
            {
                if( g_MyD3D->MyCtrl->k_Up )
                {
                    pPlayer->SetPlayerState(MID_RYAN3_JUMP_UP_ATK);
                    pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
                    pPlayer->cFrame = 0;
                }
                else
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
                    pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
                    pPlayer->cFrame = 0;
                }
                return 0;
            }
            else if( pPlayer->uiMotion == MID_RYAN3_DASH_END_WAIT )
            {
                pPlayer->SetPlayerState(MID_RYAN3_DASH_ATK);
                pPlayer->cFrame = 0;
                return 0;
            }
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

    return 0;
}

int KGCPlayerRyan3::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		// 대기 모션 일때는 스킬분기 상태를 아무것도 아닌것으로 한다. 
		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

		if ( g_MyD3D->MyCtrl->k_Skill_Key && pPlayer->IsContact )
		{
			if ( g_MyD3D->MyCtrl->k_Up )
			{
				if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV1 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1 ) &&
                    false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_ANKH_OF_RESURRECTION ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_STRONGLY_LIFE_FORCE_LV1 );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Select_Item = false;
					g_MyD3D->MyCtrl->k_Up = false;

					return 0;
				}
				else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV2 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2 ) &&
                    false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_ANKH_OF_RESURRECTION ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_STRONGLY_LIFE_FORCE_LV2 );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Select_Item = false;
					g_MyD3D->MyCtrl->k_Up = false;

					return 0;
				}                
			}

			if ( g_MyD3D->MyCtrl->k_Down
				&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_WILD_HOWLING ) 
				&& false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_WILD_HOWLING ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_RYAN_WILD_HOWLING );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Down = false;

				return 0;
			}

            if ( ( ( g_MyD3D->MyCtrl->k_Fresh_Right && pPlayer->bIsRight )||( g_MyD3D->MyCtrl->k_Fresh_Left && !pPlayer->bIsRight ) )
                && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STONE_WALL )
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STONEWALL ) )
            {
                if ( ( pPlayer->GetIsRight() && g_MyD3D->MyCtrl->k_Fresh_Left ) &&
                    ( !pPlayer->GetIsRight() && g_MyD3D->MyCtrl->k_Fresh_Right ) )
                {
                    pPlayer->bIsRight = !pPlayer->bIsRight;
                }

                pPlayer->SetPlayerState( MID_SKILL_RYAN3_STONE_WALL );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;

                return 0;
            }
		}

        if ( g_MyD3D->MyCtrl->k_Fresh_Double_C && pPlayer->IsContact&&  pPlayer->uiMotion != MID_COMMON_CATCH_BY_RYAN )
        {
            if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_RYAN_NATURESHIELD )
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_NATURESHIELD ) )
            {
                pPlayer->Direct_Motion_Input( MID_SKILL_RYAN_NATURESHIELD );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                g_MyD3D->MyCtrl->k_Skill_Key = false;
                g_MyD3D->MyCtrl->k_Fresh_Left = false;
                g_MyD3D->MyCtrl->k_Fresh_Right = false;
                KGCPlayerRyan::m_dwSuperTime = timeGetTime();
                //pPlayer->Shield = INT_MAX;
                pPlayer->ConfirmUseSheild( CID_RYAN1 );
                pPlayer->Super = 220;
                pPlayer->x_Speed = 0.0f;
                pPlayer->y_Speed = 0.0f;

                return 0;
            }
        }

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
			//if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
			//	g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			//{
			//	if( pPlayer->Next_Attack_Count != 255 )
			//	{
			//		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
			//	}
			//}
			//else
			//{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
			//}
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

		if( g_MyD3D->MyCtrl->Skill_Key_Count == 10 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_CHARGING_ATK) )
		{
			pPlayer->SetPlayerState( MID_SKILL_RYAN3_CHARGING_ING );
			pPlayer->cFrame = 0;

			return 0;
		}
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}