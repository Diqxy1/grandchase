#include "stdafx.h"
//
#include "MyD3D.h"




#include "KGCPlayerLas4.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"
#include "GCCameraOrtho.h"


KGCPlayerLas4::KGCPlayerLas4()
{

}

KGCPlayerLas4::~KGCPlayerLas4()
{

}

int KGCPlayerLas4::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame != 0 && 
            ( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
              (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) ) )
        {
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH );
            pPlayer->cFrame = 0;
            return 0;
        }

        return KGCPlayerCommon::OnDash( pPlayer, true );
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_LAS4_DASH_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_LAS4_DASH_END_JUMP );
        pPlayer->cFrame = 0;
        return 0;
    }

    return 0;
}

int KGCPlayerLas4::OnDoubleDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
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
        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_END_JUMP );
        pPlayer->cFrame = 0;
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnCombo1(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
#if defined( MATCH_BALANCE_2011_8 )
        if( pPlayer->cFrame <= 16 && true == g_MyD3D->MyCtrl->k_Up )
#else
        if( //pPlayer->Next_Attack_Count == 0 &&
            pPlayer->cFrame <= 16 &&
            ( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right) ||
             (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left) ) )
#endif
        {
            pPlayer->SetPlayerState( MID_LAS4_CHANGE_BODY_SKILL );
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
        return 0;
    }
    return 0;
}


int KGCPlayerLas4::OnCombo2(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( //pPlayer->Next_Attack_Count == 0 &&
            true == g_MyD3D->MyCtrl->k_Fresh_Up )
        {
            pPlayer->SetPlayerState( MID_LAS4_BOUNCE_ATK );
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
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnCombo3(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
                pPlayer->bIsRight = false;

            pPlayer->SetPlayerState( MID_LAS4_CRITICAL_ATK );
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
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnCombo4(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
                pPlayer->bIsRight = false;

            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame != 0 && (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_LAS4_DASH_ATK_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_LAS4_DASH_ATK_END_JUMP );
        pPlayer->cFrame = 0;
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnDoubleDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_ATK_END_WAIT );
        else
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_ATK_END_JUMP );
        pPlayer->cFrame = 0;
        return 0;
    }
    return 0;
}

int KGCPlayerLas4::OnChangeBodytoLog(PLAYER* pPlayer, bool bKeyProc)
{
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
        vTempPos.y += 0.7f;
        pPlayer->SetDirectMove( vTempPos.x, vTempPos.y );
        pPlayer->NoCheckContact = 50;
        pPlayer->m_bJumpDashed = true;
        return 0;
    }
    if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        pPlayer->m_bJumpDashed = false;
    }
    return 0;
}

int KGCPlayerLas4::OnDashEnd(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame != 0 && (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH );
            pPlayer->cFrame = 0;
            return 0;
        }
        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_LAS4_DASH_ATK );
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
        return 0;
    }

    return 0;
}

int KGCPlayerLas4::OnDoubleDashEnd(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_LAS4_DOUBLE_DASH_ATK );
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

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 40;
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
        return 0;
    }

    return 0;
}

int KGCPlayerLas4::OnDashEndJump(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->IsContact = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
            pPlayer->NoCheckContact = 15;
            pPlayer->cFrame = 0;
            return 0;
        }
        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPlayer->bIsRight = true;
            if( pPlayer->uiMotion == MID_LAS4_DASH_END_JUMP )
                pPlayer->SetPlayerState(MID_LAS4_DOUBLE_DASH);
            else if( pPlayer->uiMotion == MID_LAS4_DOUBLE_DASH_ATK_END_JUMP )
                pPlayer->SetPlayerState(MID_LAS4_DASH);
            pPlayer->cFrame = 0;
            return 0;
        }
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPlayer->bIsRight = false;
            if( pPlayer->uiMotion == MID_LAS4_DASH_END_JUMP )
                pPlayer->SetPlayerState(MID_LAS4_DOUBLE_DASH);
            else if( pPlayer->uiMotion == MID_LAS4_DOUBLE_DASH_ATK_END_JUMP )
                pPlayer->SetPlayerState(MID_LAS4_DASH);
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
    return 0;
}

int KGCPlayerLas4::OnWait( PLAYER* pPlayer, bool bKeyProc )
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
				pPlayer->SetPlayerState( MID_SKILL_LAS4_SUPER_JUMP );
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