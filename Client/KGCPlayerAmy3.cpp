#include "stdafx.h"
#include ".\kgcplayeramy3.h"





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"



// 파이팅 모드

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCPlayerAmy3::KGCPlayerAmy3(void)
{
}

KGCPlayerAmy3::~KGCPlayerAmy3(void)
{
}

int KGCPlayerAmy3::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 || 
				pPlayer->uiMotion == MID_AMY3_ATK5 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 && pPlayer->uiMotion != MID_AMY3_ATK2 &&
				pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_AMY3_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATKEND )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState( MID_AMY3_DOUBLE_ATK );
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
			pPlayer->SetPlayerState( MID_AMY3_UNIQUE_SKILL1 );				
			pPlayer->cFrame = 0;
			return 0;
		}

        return 0;
    }

    return 0;
}

int KGCPlayerAmy3::OnRun(PLAYER* pPlayer, bool bKeyProc)
{  
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {            
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
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
                ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 24 )//정지
            {
                pPlayer->cFrame = 25;
                return 0;
            }
        }
        return 0;
    }

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == 24 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 24 )
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
                pPlayer->SetPlayerState( MID_AMY3_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY3_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy3::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            if ( g_MyD3D->MyCtrl->k_Down )
            {                
                pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL1 );                
            }
            else
            {
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
            }   
            pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            g_MyD3D->MyCtrl->k_Down = false;
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
			if(pPlayer->Before_Motion != MID_AMY3_SWAP_TO_DANCE_MODE )
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

int KGCPlayerAmy3::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    // 주의!! 점프 더블대쉬후에는 공격이 안된다.

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
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);                      
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
            pPlayer->y_Speed += -0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if( pPlayer->cFrame == 0 )
        {
            g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
            g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
        }

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY3_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY3_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerAmy3::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->SetPlayerState( MID_AMY3_RUN );
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
            pPlayer->SetPlayerState( MID_AMY3_DASH );
            pPlayer->cFrame = 0;

            return 0;
        }

        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY3_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY3_JUMP_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}


int KGCPlayerAmy3::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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
            if ( g_MyD3D->MyCtrl->k_Down )
            {                
                pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL1 );                
            }
            else
            {
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
            }   
            pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
            
            pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;

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

int KGCPlayerAmy3::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // 점프 더블 대쉬가 있으면 그걸로...
            pPlayer->SetPlayerState( MID_AMY3_DASH );
            pPlayer->cFrame = 0;

            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->IsContact )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Punch )
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
                    g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                    pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }
        }
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
			if( pPlayer->uiMotion == MID_AMY3_JUMP_DASH_TO_JUMP )
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


int KGCPlayerAmy3::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
                return KGCPlayerCommon::DashJumpFunc( pPlayer , MID_AMY3_RUN_TO_DOUBLE_JUMP);
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

int KGCPlayerAmy3::OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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

int KGCPlayerAmy3::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{			
			if( MID_AMY3_DASH_ATK1 == pPlayer->uiMotion && 17 >= pPlayer->cFrame)
			{
				pPlayer->Next_Attack_Count = 255;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}			

			if( MID_AMY3_DASH_ATK1 == pPlayer->uiMotion && 17 < pPlayer->cFrame && pPlayer->cFrame <= 25 )
			{
				pPlayer->SetPlayerState( MID_AMY3_DASH_ATK2 );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}
		}
	}

    //if( MID_AMY3_DASH_ATK1 == pPlayer->uiMotion )
    //{
    //    if( true == pPlayer->IsContact )
    //    {
    //        pPlayer->m_bFrameLock = false;
    //    }
    //    else if( 38 == pPlayer->cFrame )
    //    {
    //        pPlayer->m_bFrameLock = true;
    //    }
    //    return 0;
    //}        

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( MID_AMY3_DASH_ATK1 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_AMY3_DASH_ATK1_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY3_DASH_ATK1_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
		else if( MID_AMY3_DASH_ATK2 == pPlayer->uiMotion )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_AMY3_DASH_ATK2_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_AMY3_DASH_ATK2_TO_JUMP );
			pPlayer->cFrame = 0;

			return 0;
		}
    }

    return 0;
}

int KGCPlayerAmy3::OnDashAtkToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->IsContact && MID_AMY3_DASH_ATK1_TO_JUMP == pPlayer->uiMotion || MID_AMY3_DASH_ATK2_TO_JUMP == pPlayer->uiMotion ||
		MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_JUMP == pPlayer->uiMotion )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
		return 0;
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

int KGCPlayerAmy3::OnJumpUniqueSkill(PLAYER* pPlayer, bool bKeyProc)
{	
	if( bKeyProc )
	{
		// 프레임 락 걸린 동안은 좌우 방향 바꿀 수 있게 한다.
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
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
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( false == pPlayer->IsContact )
			{
				if( 14 < pPlayer->cFrame )
				{
					if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_WAIT )
					{
						pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL2 );
					}
					else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_DANCE_WAIT )
					{
						pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE_SKILL2 );
					}
					else
					{
						return 0;
					}
					pPlayer->cFrame = 0;
					pPlayer->NoCheckContact = 25;
					return 0;
				}
			}
		}
	}

	//무한 반복하는 프레임..
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
		pPlayer->m_bFrameLock = true;

	if( pPlayer->IsContact ) 
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerAmy3::OnJumpUniqueSkill2(PLAYER* pPlayer, bool bKeyProc)
{
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
	{
		if( pPlayer->IsContact )
		{
			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_WAIT );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_DANCE_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE2_TO_WAIT );
			}			
			pPlayer->cFrame = 0;
			pPlayer->x_Speed = 0.0f;
			return 0;
		}
		else
		{
			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_JUMP );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_DANCE_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE2_TO_JUMP );
			}			
			pPlayer->cFrame = 0;
			pPlayer->x_Speed = 0.0f;
			return 0;
		}
	}
	else
	{
		if( pPlayer->cFrame == 0)
			pPlayer->NoCheckContact = 18;
		if( pPlayer->IsContact &&  pPlayer->cFrame >=18 )
		{            
			pPlayer->x_Speed = 0.0;

			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_WAIT );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY3_DANCE_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY3_DANCE_JUMP_UNIQUE2_TO_WAIT );
			}			
			pPlayer->cFrame = 0;            
			return 0;
		}
	}    
	return 0;
}