#include "stdafx.h"
//





#include "KGCPlayerCommon.h"
#include ".\kgcplayerlas2.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"


KGCPlayerLas2::KGCPlayerLas2(void)
{
}

KGCPlayerLas2::~KGCPlayerLas2(void)
{
}

int KGCPlayerLas2::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( pPlayer->uiMotion != MID_LAS2_ATK1_1 )
        {		
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
            {
                pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
                pPlayer->cFrame = 0;

                //if( pPlayer->Now_Motion != pPlayer->GetCurFormTemplate().ATK1+3 )	//Critical
                //{
                //    g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->MyNumber);
                //}
                //else //Double
                //{
                //    g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->MyNumber);
                //}

                if( pPlayer->uiMotion == MID_LAS2_ATK1_4 )
                    pPlayer->SetPlayerState(MID_LAS2_DOUBLE_ATK);
                else 
                    pPlayer->SetPlayerState(MID_LAS2_CRITICAL_ATK);
                return 0;
            }
        }

        if(g_MyD3D->MyCtrl->k_Fresh_Punch)
        {
            if( pPlayer->uiMotion != MID_LAS2_ATK1_4)
            {			
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
int KGCPlayerLas2::OnArrowEvasion(PLAYER* pPlayer, bool bKeyProc)
{
    return 0;
}

int KGCPlayerLas2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
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
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if ( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
                pPlayer->SetPlayerState( MID_LAS2_JUMPDASH_ATK );
            else
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            pPlayer->cFrame = 0;
            return 0;
        }
        if ( pPlayer->GetCurFormTemplate().DASH == pPlayer->uiMotion &&  
             ( g_MyD3D->MyCtrl->k_Fresh_Left_Dash || g_MyD3D->MyCtrl->k_Fresh_Right_Dash ) && pPlayer->cFrame > 14 )
        {// Asiroin(전직) 잔상 보여야 된다.
            pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash == true;

			if ( pPlayer->IsMagicEffect( EGC_SKILL_LAS_MOVE_SPEEDUP ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LAS2_DOUBLEDASH );
			}
			else
			{
				pPlayer->SetPlayerState( MID_LAS2_DASH_2 );
			}

            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
            g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
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
        return 0;
    }
    return 0;
}

int KGCPlayerLas2::OnWait( PLAYER* pPlayer, bool bKeyProc )
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
				pPlayer->SetPlayerState( MID_SKILL_LAS2_SUPER_JUMP );
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;

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
					g_Hot_Count = 0;
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