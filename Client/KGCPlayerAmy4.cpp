#include "stdafx.h"
#include ".\kgcplayeramy4.h"





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"



// 파이팅 모드

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCPlayerAmy4::KGCPlayerAmy4(void)
{
}

KGCPlayerAmy4::~KGCPlayerAmy4(void)
{
}

int KGCPlayerAmy4::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 || 
				pPlayer->uiMotion == MID_AMY4_ATK5 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 && pPlayer->uiMotion != MID_AMY4_ATK2 &&
				pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_AMY4_CRITICAL_ATK );
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
                pPlayer->SetPlayerState( MID_AMY4_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) && 
			pPlayer->cFrame <= 10 && pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1)
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
			pPlayer->SetPlayerState( MID_AMY4_UNIQUE_SKILL1 );				
			pPlayer->cFrame = 0;
			return 0;
		}

        return 0;
    }

    return 0;
}

int KGCPlayerAmy4::OnRun(PLAYER* pPlayer, bool bKeyProc)
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
                ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < pPlayer->GetNowMotionFRM()->GetNum_Frame() )//정지
            {
                pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame();
                return 0;
            }
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
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
                pPlayer->SetPlayerState( MID_AMY4_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY4_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy4::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
                pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL1 );                
                g_MyD3D->MyCtrl->k_Down = false;
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
			if(pPlayer->Before_Motion != MID_AMY4_SWAP_TO_MELO_MODE )
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

int KGCPlayerAmy4::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->SetPlayerState( MID_AMY4_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY4_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerAmy4::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->SetPlayerState( MID_AMY4_RUN );
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
            pPlayer->SetPlayerState( MID_AMY4_DASH );
            pPlayer->cFrame = 0;

            return 0;
        }

        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY4_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY4_JUMP_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}


int KGCPlayerAmy4::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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
                pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL1 );                
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

int KGCPlayerAmy4::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // 점프 더블 대쉬가 있으면 그걸로...
            pPlayer->SetPlayerState( MID_AMY4_DASH );
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

    if( pPlayer->IsContact )
    {
        pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
        pPlayer->cFrame = 0;
        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
		pPlayer->cFrame = 34;
        return 0;    
    }


    return 0;
}


int KGCPlayerAmy4::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
                return KGCPlayerCommon::DashJumpFunc( pPlayer, MID_AMY4_RUN_TO_DOUBLE_JUMP );
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
int KGCPlayerAmy4::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{		
            pPlayer->Next_Attack_Count = 255;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
		}

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( MID_AMY4_DASH_ATK1 == pPlayer->uiMotion && 21 <= pPlayer->cFrame )
            {
                pPlayer->SetPlayerState( MID_AMY4_DASH_ATK2 );
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;

            }            
            return 0;
        }
	}

   
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
     
        //if( MID_AMY4_DASH_ATK1 == pPlayer->uiMotion )
        //{
        //    if( true == pPlayer->IsContact )
        //        pPlayer->SetPlayerState( MID_AMY4_DASH_ATK1_TO_WAIT );
        //    else
        //        pPlayer->SetPlayerState( MID_AMY4_DASH_ATK1_TO_JUMP );
        //    pPlayer->cFrame = 0;

        //    return 0;
        //}

        if( !pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_AMY4_JUMP );
            pPlayer->cFrame = 0;
        }
      /*  else if( MID_AMY4_DASH_ATK2 == pPlayer->uiMotion )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_AMY4_DASH_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_AMY4_DASH_TO_JUMP );
			pPlayer->cFrame = 0;

			return 0;
        }
        else if( MID_AMY4_DASH_ATK3 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_AMY4_DASH_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY4_DASH_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }*/
    }

    return 0;
}

int KGCPlayerAmy4::OnDashAtkToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->IsContact && MID_AMY4_DASH_ATK1_TO_JUMP == pPlayer->uiMotion || MID_AMY4_DASH_ATK2_TO_JUMP == pPlayer->uiMotion ||
		MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_JUMP == pPlayer->uiMotion || MID_AMY4_DASH_ATK3_TO_JUMP == pPlayer->uiMotion )
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

int KGCPlayerAmy4::OnJumpUniqueSkill(PLAYER* pPlayer, bool bKeyProc)
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
					pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL2 );
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

    // 이전 처럼 총질 가능하려면 제한 프레임 제거하면 됨.
	if(pPlayer->cFrame >= 33 && pPlayer->IsContact ) 
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerAmy4::OnJumpUniqueSkill2(PLAYER* pPlayer, bool bKeyProc)
{
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
	{
		if( pPlayer->IsContact )
		{
			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_WAIT );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_MELO_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_MELO_JUMP_UNIQUE2_TO_WAIT );
			}			
			pPlayer->cFrame = 0;
			pPlayer->x_Speed = 0.0f;
			return 0;
		}
		else
		{
			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_JUMP );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_MELO_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_MELO_JUMP_UNIQUE2_TO_JUMP );
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

			if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_WAIT );
			}
			else if( pPlayer->GetCurFormTemplate().WAIT == MID_AMY4_MELO_WAIT )
			{
				pPlayer->SetPlayerState( MID_AMY4_MELO_JUMP_UNIQUE2_TO_WAIT );
			}			
			pPlayer->cFrame = 0;            
			return 0;
		}
	}    
	return 0;
}

