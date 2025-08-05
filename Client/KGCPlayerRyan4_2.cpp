#include "stdafx.h"

#include "MyD3D.h"
#include "KGCPlayerRyan.h"
#include "KGCPlayerRyan4_2.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"

KGCPlayerRyan4_2::KGCPlayerRyan4_2()
{

}

KGCPlayerRyan4_2::~KGCPlayerRyan4_2()
{

}

int KGCPlayerRyan4_2::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
		if ( g_MyD3D->MyCtrl->k_Skill_Key && pPlayer->IsContact )
		{
			if ( g_MyD3D->MyCtrl->k_Up )
			{
				if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV1 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1 ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_STRONGLY_LIFE_FORCE_LV1 );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Select_Item = false;
					g_MyD3D->MyCtrl->k_Up = false;

					return 0;
				}
				else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV2 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2 ) )
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

                pPlayer->SetPlayerState( MID_SKILL_RYAN4_2_STONE_WALL );
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

		if( g_MyD3D->MyCtrl->Skill_Key_Count == 10 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_CHARGING_ATK) )
		{
			pPlayer->SetPlayerState( MID_SKILL_RYAN4_1_CHARGING_ING );
			pPlayer->cFrame = 0;

			return 0;
		}

        if( g_MyD3D->MyCtrl->k_Select_Item )
        {
            pPlayer->SetPlayerState( MID_RYAN4_SWAP_TO_TWO_HAND );
            pPlayer->cFrame = 0;
            return 0;
        }

        return KGCPlayerCommon::OnWait( pPlayer, true );
    }
    return KGCPlayerCommon::OnWait( pPlayer, false );
}

int KGCPlayerRyan4_2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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
                ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 30 )//정지
            {
                pPlayer->cFrame = 31;
                return 0;
            }
        }

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == 30 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 30 )
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

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
                pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_RUN_END_WAIT );
            else
                pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_RUN_END_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCPlayerRyan4_2::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( (g_MyD3D->MyCtrl->k_Left && pPlayer->GetIsRight() == true) ||
                (g_MyD3D->MyCtrl->k_Right && pPlayer->GetIsRight() == false) ) {
                pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_BACK_DASH_ATK );
            } else {
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
            pPlayer->y_Speed -= 0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();
        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    //if( pPlayer->uiMotion == pPlayer->m_PlayerTemplate.DASH ||
    //    pPlayer->uiMotion == pPlayer->m_PlayerTemplate.JUMP_DASH )
    //{
    //    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    //    {
    //        if( true == pPlayer->IsContact )
    //            pPlayer->SetPlayerState( MID_RYAN4_DASH_TO_WAIT );
    //        else
    //            pPlayer->SetPlayerState( MID_RYAN4_DASH_TO_JUMP );
    //        pPlayer->cFrame = 0;
    //    }
    //}
    return 0;
}

int KGCPlayerRyan4_2::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
            pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
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

int KGCPlayerRyan4_2::OnDashAtk( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_DASH_ATK_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_DASH_ATK_END_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

//int KGCPlayerRyan4_2::OnBackDashAtk( PLAYER* pPlayer, bool bKeyProc )
//{
//    if( bKeyProc )
//    {
//        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
//        {
//            pPlayer->Next_Attack_Count = 255;
//            return 0;
//        }
//        if( g_MyD3D->MyCtrl->k_Down)
//        {
//            g_MyD3D->MyCtrl->k_Down = false;
//            pPlayer->y_Speed = -0.01f;
//            pPlayer->NoCheckContact = 20;
//            return 0;
//        }
//        return 0;
//    }
//    return 0;
//}

int KGCPlayerRyan4_2::OnCombo( PLAYER* pPlayer, bool bKeyProc )
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

        if( pPlayer->uiMotion == MID_RYAN4_ONE_HAND_ATK1 ) {
            if( g_MyD3D->MyCtrl->k_Up )
            {	
                if( pPlayer->cFrame <= 8 )
                {
                    int uiMotion = -1;
                    if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_HP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_HP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN4_2_HP_TOTEM;				
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_MP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_MP_TOTEM_COOL ))
                    {
                        uiMotion = MID_SKILL_RYAN4_2_MP_TOTEM;				
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_DP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN4_2_DP_TOTEM;				
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_AP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_AP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN4_2_AP_TOTEM;
                    }

                    if ( uiMotion != -1 )
                    {
                        pPlayer->SetPlayerState( uiMotion );
                        pPlayer->cFrame = 0;
                        return 0;
                    }
                }

                g_MyD3D->MyCtrl->k_Up = false;
            }            
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
#ifndef COMBO_TEST
            if( pPlayer->Next_Attack_Count > 0 )
            {
#endif
                pPlayer->Next_Attack_Count = 255;
#ifndef COMBO_TEST
            }
#endif
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_RYAN4_ONE_HAND_ATK2 ||
                pPlayer->uiMotion == MID_RYAN4_ONE_HAND_ATK3 ||
                pPlayer->uiMotion == MID_RYAN4_ONE_HAND_ATK4 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->SetPlayerState( MID_RYAN4_ONE_HAND_CRITICAL_ATK );
                pPlayer->cFrame = 0;
            }
            return 0;
        }
        return 0;
    }

    return 0;
}

int KGCPlayerRyan4_2::OnFormChange(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame == 55 )
    {
        pPlayer->Direct_Motion_Input( MID_RYAN4_ONE_HAND_FORM_END );
        pPlayer->cFrame = 0;

        // 변신 모델의 위치를 기본 모델의 위치로 맞춘다.
        pPlayer->GetMetaForm(FORM_MDAN)->SetWorldMat( pPlayer->GetMetaForm(FORM_NORMAL)->GetWorldMat() );

        return 0;
    }

    return 0;
}

int KGCPlayerRyan4_2::OnFormChangeComplete(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->IsLocalPlayer() )
    {
        pPlayer->ProcessMetamorphosisFormChange( FORM_MDAN );
        pPlayer->Direct_Motion_Input( MID_RYAN4_MDAN_WAIT );
        pPlayer->cFrame = 0;
        pPlayer->SetSkillType( pPlayer->m_iSkillType );

        return 0;
    }

    return 0;
}
