#include "stdafx.h"

#include "KGCPlayerRyan.h"
#include ".\kgcplayerryan2.h"
#include "KGCPlayerCommon.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"

#include "KGCRoomManager.h"

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCPlayerRyan2::KGCPlayerRyan2(void)
{
}

KGCPlayerRyan2::~KGCPlayerRyan2(void)
{
}

int KGCPlayerRyan2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    // 2007/2/6. iridology. 혹 대쉬가 무디면 여기다가 처리를 해줘야함..

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerRyan2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
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

        if(pPlayer->IsContact)
        {		
            if( pPlayer->bIsRight && ISPUSHED(k_Left) )
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && ISPUSHED(k_Right) )
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        //정지
        if( ( ( pPlayer->bIsRight &&  !ISPUSHED(k_Right) ) || 
            ( !pPlayer->bIsRight &&  !ISPUSHED(k_Left) ) )&& pPlayer->cFrame < 30 )
        {
            pPlayer->cFrame = 30;
            return 0;
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == 29 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 29)
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();
    return 0;
}

int KGCPlayerRyan2::OnJump(PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            if( ISPUSHED(k_Down) )
            {
                pPlayer->SetPlayerState( MID_RYAN2_JUMP_ATK2 );
                pPlayer->cFrame = 0;
                pPlayer->NoCheckContact = 20;
                ISPUSHED(k_Down) = false;
                return 0;
            }
            else
            {
                pPlayer->SetPlayerState( MID_RYAN2_JUMP_ATK1 );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        return KGCPlayerCommon::OnJump( pPlayer, true );
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerRyan2::OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc )
{
    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerRyan2::OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame == 32)
        pPlayer->cFrame = 20;

    if( pPlayer->cFrame >= 70 )
    {
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }

    return 0;
}

int KGCPlayerRyan2::OnJumpDash(PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerRyan2::OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame >= 13 && pPlayer->cFrame < 21 )
        {
            if( ISPUSHED(k_Fresh_Punch) )
            {
                ISPUSHED(k_Fresh_Punch) = false;
                pPlayer->Next_Attack_Count = 255;
                return 0;
            }
        }
    }

    if( pPlayer->cFrame >= 21 )
    {
        if( pPlayer->Next_Attack_Count == 255 )
        {
            pPlayer->SetPlayerState( MID_RYAN2_JUMP_DASH_ATK );
            pPlayer->Next_Attack_Count = 0;
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerRyan2::OnCombo(PLAYER* pPlayer, bool bKeyProc )
{
    if(bKeyProc)
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

        if( pPlayer->uiMotion == MID_RYAN2_ATK1 ) {
            if( g_MyD3D->MyCtrl->k_Up )
            {	
                if( pPlayer->cFrame <= 8 )
                {
                    int uiMotion = -1;
                    if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_HP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_HP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN2_HP_TOTEM;
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_MP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_MP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN2_MP_TOTEM;                     
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_DP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN2_DP_TOTEM;
                    }
                    else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_AP_TOTEM ) &&
                        false == pPlayer->IsMagicEffect( EGC_EFFECT_AP_TOTEM_COOL ) )
                    {
                        uiMotion = MID_SKILL_RYAN2_AP_TOTEM;
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

        if( pPlayer->uiMotion != MID_RYAN2_ATK1 )
        {		
            if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash))
            {
                pPlayer->bIsRight = ISPUSHED(k_Fresh_Right_Dash);
                pPlayer->cFrame = 0;

                if( pPlayer->uiMotion == MID_RYAN2_ATK6 )
                    pPlayer->SetPlayerState(MID_RYAN2_DOUBLE_ATK);
                else 
                    pPlayer->SetPlayerState(MID_RYAN2_CRITICAL_ATK);

                return 0;
            }
        }

        if( ISPUSHED(k_Fresh_Punch) )
        {
            if( pPlayer->uiMotion != MID_RYAN2_ATK6)
            {
                // 2007/2/9. iridology. 맞아야지만 콤보연결
                if( pPlayer->Next_Attack_Count > 0 )
                {
                    pPlayer->Next_Attack_Count = 255;
                }
            }
        }
        return 0;
    }	
    return 0;
}

int KGCPlayerRyan2::OnFormChange(PLAYER* pPlayer, bool bKeyProc )
{
    //switch( pPlayer->MetamorphosisForm )
    //{
    //case FORM_NEPHILIM:
    //    {
    //        if( pPlayer->Frame == 55 )
    //        {
    //            pPlayer->SetPlayerState( MID_NEPHILLIM_FORM_END );
    //            pPlayer->Frame = 0;
    //            return 0;
    //        }
    //    }
    //    break;
    //}
    return 0;
}

int KGCPlayerRyan2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 30 )
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );

    return 0;
}

int KGCPlayerRyan2::OnSpecial3(PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame == 1 )
    {
        // 2007/2/14. iridology. 파퉤세이 맵에서는 뚫고 들ㄹ어가면 대락 난감
        if( SiKGCRoomManager()->IsMonsterGameMode() && SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST3 )
        {
            //// Jaeho.Ready
            //if( g_MyD3D->m_pkQuestGameMgr->GetCurrentQuest()->GetCurrentQuestStage()->GetMapID() == GC_GS_QUEST3_E )
            //{
            //    pPlayer->m_bCrashCheck = true;
            //}
        }
    }
    return 0;
}

int KGCPlayerRyan2::OnWait( PLAYER* pPlayer, bool bKeyProc )
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

                pPlayer->SetPlayerState( MID_SKILL_RYAN2_STONE_WALL );
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
                g_MyD3D->MyCtrl->k_Right = false;
                g_MyD3D->MyCtrl->k_Left = false;
                g_MyD3D->MyCtrl->k_Skill_Key = false;
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

		if( g_MyD3D->MyCtrl->Skill_Key_Count == 10 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_CHARGING_ATK) )
		{
			pPlayer->SetPlayerState( MID_SKILL_RYAN2_CHARGING_ING );
			pPlayer->cFrame = 0;

			return 0;
		}
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}