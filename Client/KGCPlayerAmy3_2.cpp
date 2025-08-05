#include "stdafx.h"
#include ".\kgcplayeramy3_2.h"





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

// 댄싱모드

KGCPlayerAmy3_2::KGCPlayerAmy3_2(void)
{
}

KGCPlayerAmy3_2::~KGCPlayerAmy3_2(void)
{
}

int KGCPlayerAmy3_2::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )                
            {
                pPlayer->Next_Attack_Count = 255;
            }
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_AMY3_DANCE_ATK2 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_AMY3_DANCE_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( pPlayer->uiMotion == MID_AMY3_DANCE_ATK3 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState( MID_AMY3_DANCE_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) && 
			pPlayer->cFrame <= 15 && pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1)
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
			pPlayer->SetPlayerState( MID_AMY3_DANCE_UNIQUE_ATK );				
			pPlayer->cFrame = 0;
			return 0;
		}
    }

    pPlayer->SetWindSpeed();
    return 0;
}

int KGCPlayerAmy3_2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{  
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->IsContact )
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            else
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);

            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer );
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

            pPlayer->y_Speed += -0.01f;
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
                pPlayer->SetPlayerState( MID_AMY3_DANCE_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY3_DANCE_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy3_2::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            if( g_MyD3D->MyCtrl->k_Down )
            {
                pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE_SKILL1 );
                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            }
            else
            {
                pPlayer->SetPlayerState(MID_AMY3_DANCE_JUMP_ATK);

                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
            }
            
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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

        if( g_MyD3D->MyCtrl->k_Up )
        {
			if(pPlayer->Before_Motion != MID_AMY3_SWAP_TO_FIGHT_MODE )
			{
				KGCPlayerCommon::DoubleJumpFunc( pPlayer );			
				pPlayer->m_bFrameLock = true;
				return 0;
			}
        }

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f )
    {
        //내려오는 점프 동작
        if( pPlayer->cFrame > 40 )
            pPlayer->cFrame = 40;
    }
    else 
    {
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy3_2::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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
			if( g_MyD3D->MyCtrl->k_Down )
			{
				pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE_SKILL1 );
				pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);

				pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
			}

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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
		return 0;
	}

	if( pPlayer->y_Speed >= 0.0f )
	{
		//내려오는 점프 동작
		if( pPlayer->cFrame >= 34 )
		{
			pPlayer->cFrame = 34;

			//무한 반복하는 프레임..
			if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
				pPlayer->m_bFrameLock = true;
		}
	}
	else 
	{
		if( pPlayer->cFrame < 34 )
			pPlayer->cFrame = 34;
		//무한 반복하는 프레임..
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
			pPlayer->m_bFrameLock = true;
	}
	return 0;
}

int KGCPlayerAmy3_2::OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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

		return 0;
	}

	if( pPlayer->y_Speed >= 0.0f )
	{
		//내려오는 점프 동작
		if( pPlayer->cFrame >= 34 )
		{
			pPlayer->cFrame = 34;

			//무한 반복하는 프레임..
			if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
				pPlayer->m_bFrameLock = true;
		}
	}
	else 
	{
		if( pPlayer->cFrame <= 34 )
			pPlayer->cFrame = 34;
		//무한 반복하는 프레임..
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
			pPlayer->m_bFrameLock = true;
	}

	return 0;
}

int KGCPlayerAmy3_2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {                
        if( g_MyD3D->MyCtrl->k_Fresh_Punch ) 
        {
            pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_ATK );                                              
            pPlayer->cFrame = 0;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer );
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
            pPlayer->y_Speed += -0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if( pPlayer->cFrame == 0 )
        {
            g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
            g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
        }

        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_TO_WAIT );
		else
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_TO_JUMP );
		pPlayer->cFrame = 0;
	}

    return 0;
}

int KGCPlayerAmy3_2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_ATK_TO_WAIT );
		else
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_ATK_TO_JUMP );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerAmy3_2::OnDashAtkToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( pPlayer->IsContact )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
			pPlayer->cFrame = 0;
			return 0;
		}
		else
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
			pPlayer->cFrame = 34;
			return 0;
		}
	}

	return 0;
}

int KGCPlayerAmy3_2::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Up )
		{
			if( pPlayer->IsContact )
			{
				return KGCPlayerCommon::DashJumpFunc( pPlayer );
			}
			pPlayer->NoCheckContact = 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			return 0;
		}

		if( pPlayer->IsContact && ( pPlayer->bIsRight && ISPUSHED(k_Right)
			|| !pPlayer->bIsRight && ISPUSHED(k_Left) ) )
		{
			pPlayer->SetPlayerState( MID_AMY3_DANCE_RUN );
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
			pPlayer->y_Speed += -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}

		if( pPlayer->cFrame == 0 )
		{
			g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
			g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
		}

		if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
			(false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
		{
			// 점프 더블 대쉬가 있으면 그걸로...
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH );
			pPlayer->cFrame = 0;

			return 0;
		}

		return 0;
	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
			pPlayer->SetPlayerState( MID_AMY3_DANCE_DASH_TO_WAIT );
		else
			pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_DASH_TO_JUMP );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerAmy3_2::OnModeChange(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            if( MID_AMY3_SWAP_TO_DANCE_MODE == pPlayer->uiMotion )
                pPlayer->SetPlayerState( MID_AMY3_SWAP_TO_DANCEMODE_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY3_SWAP_TO_FIGHTMODE_WAIT );
        }
        else
        {
            if( MID_AMY3_SWAP_TO_DANCE_MODE == pPlayer->uiMotion )
                pPlayer->SetPlayerState( MID_AMY3_SWAP_TO_DANCEMODE_JUMP );            
            else
                pPlayer->SetPlayerState( MID_AMY3_SWAP_TO_FIGHTMODE_JUMP );            
        }
        pPlayer->cFrame = 0;

        return 0;
    }

    return 0;
}

int KGCPlayerAmy3_2::OnModeChangeToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
        else
        {
			if( pPlayer->uiMotion == MID_AMY3_SWAP_TO_DANCEMODE_JUMP ) 
				pPlayer->SetPlayerState( MID_AMY3_DANCE_RUN_TO_DOUBLE_JUMP );								
			else if( pPlayer->uiMotion == MID_AMY3_SWAP_TO_FIGHTMODE_JUMP ) 
				pPlayer->SetPlayerState( MID_AMY3_RUN_TO_DOUBLE_JUMP );
            pPlayer->cFrame = 34;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy3_2::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
        else
        {
			if( pPlayer->uiMotion == MID_AMY3_DANCE_JUMP_DASH_TO_JUMP )
			{
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
				pPlayer->cFrame = 34;
			}
			else
			{
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
				pPlayer->cFrame = 33;
			}
            return 0;
        }
    }
    return 0;
}


int KGCPlayerAmy3_2::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer );
            }
			else
			{
                return KGCPlayerCommon::DashJumpFunc( pPlayer, MID_AMY3_DANCE_RUN_TO_DOUBLE_JUMP );				
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

            pPlayer->y_Speed += -0.01f;
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
    }

    // 서서히 속도 감소
    if( pPlayer->bIsRight )
    {
        pPlayer->x_Speed -= 0.005f;
        if( pPlayer->x_Speed < 0.0f )
            pPlayer->x_Speed = 0.0f;
    }
    else
    {
        pPlayer->x_Speed += 0.005f;
        if( pPlayer->x_Speed > 0.0f )
            pPlayer->x_Speed = 0.0f;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
		else
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
			pPlayer->cFrame = 34;
			return 0;
		}
    }

    return 0;
}