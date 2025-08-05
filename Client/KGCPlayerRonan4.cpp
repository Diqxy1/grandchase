#include "stdafx.h"
//





#include "KGCPlayerRonan4.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"

#include "Damage.h"

KGCPlayerRonan4::KGCPlayerRonan4()
{

}

KGCPlayerRonan4::~KGCPlayerRonan4()
{

}

int KGCPlayerRonan4::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
		if( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->IsMagicEffect( EGC_EFFECT_RONAN4_SPECIAL4_BUFF ) == false )
		{
			pPlayer->SetPlayerState(MID_RONAN4_DASH_ATK_A1);
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Up )
		{
			if( pPlayer->IsContact )
			{
				if(pPlayer->bIsRight)
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
				else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;
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
                return 0;

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
			( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 31 )//정지
		{
			pPlayer->cFrame = 31;
			return 0;
		}

        return 0;
    }

    ////무한 반복하는 프레임..
    if(pPlayer->cFrame == 30 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 30)
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

            if( pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_RONAN4_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RONAN4_RUN_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCPlayerRonan4::OnJump(PLAYER* pPlayer, bool bKeyProc)
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

		if( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->IsMagicEffect( EGC_EFFECT_RONAN4_SPECIAL4_BUFF ) == false )
		{
			if ( g_MyD3D->MyCtrl->k_Down)
			{                
				pPlayer->SetPlayerState( MID_RONAN4_JUMP_ATK_B );                
				pPlayer->cFrame = 0;
			}
			else
			{
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
				pPlayer->cFrame = 0;
			}   
			pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			g_MyD3D->MyCtrl->k_Down = false;			
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

        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f )
    {
        //내려오는 점프 동작
        if( pPlayer->cFrame > 49 )
            pPlayer->cFrame = 49;
    }
    else 
    {
        if( pPlayer->cFrame > 49 )
            pPlayer->cFrame = 49;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerRonan4::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc && pPlayer->IsMagicEffect( EGC_EFFECT_RONAN4_SPECIAL4_BUFF ) == false )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->uiMotion != MID_RONAN4_ATK4 )
        {
            if ( pPlayer->Next_Attack_Count > 0 ||
				 pPlayer->uiMotion == MID_RONAN4_ATK3)
            {
				if(	pPlayer->uiMotion == MID_RONAN4_ATK3 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Down ) )
					pPlayer->Next_Attack_Count = 0;
				else
					pPlayer->Next_Attack_Count = 255;
            }
        }	

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_bChargeMonoVolt == false &&
			pPlayer->uiMotion == MID_RONAN4_ATK1 )
		{	
			if( pPlayer->cFrame <= 15 )
			{
				pPlayer->SetPlayerState(MID_RONAN4_MONOVOLT_READY);				
				pPlayer->cFrame = 0;				
				return 0;
			}
		}

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) 
			&& pPlayer->m_bChargeMonoVolt && pPlayer->uiMotion == MID_RONAN4_ATK1 &&
			pPlayer->cFrame <= 15 )
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
            pPlayer->m_bChargeMonoVolt = false;
            pPlayer->SetChargeMonoVolt(false);
			pPlayer->SetPlayerState( MID_RONAN4_MONOVOLT_ATK1 );				
			pPlayer->cFrame = 0;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down && pPlayer->m_bChargeMonoVolt &&
			pPlayer->uiMotion == MID_RONAN4_ATK1 )
		{
            pPlayer->m_bChargeMonoVolt = false;
            pPlayer->SetChargeMonoVolt(false);
			pPlayer->SetPlayerState( MID_RONAN4_MONOVOLT_ATK2 );
			g_MyD3D->MyCtrl->k_Down = false;
			pPlayer->cFrame = 0;
			return 0;
		}

		if (g_MyD3D->MyCtrl->k_Up && pPlayer->m_bChargeMonoVolt &&
			pPlayer->uiMotion == MID_RONAN4_ATK1 && false == pPlayer->IsMagicEffect(EGC_EFFECT_RONAN4_UNIQUE1_COOL) )
		{
			pPlayer->m_bChargeMonoVolt = false;
			pPlayer->SetChargeMonoVolt(false);
			pPlayer->SetPlayerState(MID_RONAN4_FORCE_PULL_ATK);
			g_MyD3D->MyCtrl->k_Up = false;
			pPlayer->cFrame = 0;
			return 0;
		}

		
		else if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_RONAN4_ATK2 || pPlayer->uiMotion == MID_RONAN4_ATK3 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_RONAN4_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( pPlayer->uiMotion == MID_RONAN4_ATK4 && pPlayer->cFrame >= 11 && pPlayer->cFrame < 30 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState( MID_RONAN4_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

		if( g_MyD3D->MyCtrl->k_Up)
		{		
			if( pPlayer->uiMotion == MID_RONAN4_ATK2 )
			{
				if( pPlayer->cFrame >= 12 )
				{
					pPlayer->SetPlayerState(MID_RONAN4_AERIAL_COMBO_A1);                                    			
					pPlayer->cFrame = 0;
				}
			}
			else if( pPlayer->uiMotion == MID_RONAN4_ATK3 )
			{
				if( pPlayer->cFrame >= 37 )
				{
					pPlayer->SetPlayerState(MID_RONAN4_AERIAL_COMBO_A1);                                    			
					pPlayer->cFrame = 0;
				}
			}
			return 0;		
		}

		else if( g_MyD3D->MyCtrl->k_Down)
		{		
			if( pPlayer->uiMotion == MID_RONAN4_ATK2 )
			{
				if( pPlayer->cFrame >= 12 )
				{
					pPlayer->SetPlayerState(MID_RONAN4_MAGIC_COMBO1);                                    			
					pPlayer->cFrame = 0;
				}
			}
			else if( pPlayer->uiMotion == MID_RONAN4_ATK3 )
			{
				if( pPlayer->cFrame >= 37 )
				{
					pPlayer->SetPlayerState(MID_RONAN4_MAGIC_COMBO1);                                    			
					pPlayer->cFrame = 0;
				}
			}
		}
	}
    return 0;
}

int KGCPlayerRonan4::OnDash(PLAYER* pPlayer, bool bKeyProc)
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

		if( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->IsMagicEffect( EGC_EFFECT_RONAN4_SPECIAL4_BUFF ) == false )
		{
			if( pPlayer->IsContact )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
			}

			if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
				|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
			{
				pPlayer->SetPlayerState( MID_RONAN4_UNIQUE_ATK_A1 );
			}
			pPlayer->cFrame = 0;
			//pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			return 0;
		}

        if( g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;

            if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
                return 0;


            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
                return 0;

#if defined( MATCH_BALANCE_2011_8 )
            pPlayer->y_Speed = -0.01f;
#else
            pPlayer->y_Speed -= 0.01f;
#endif
            pPlayer->NoCheckContact = 20;

            return 0;
        }

        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
            pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
        {
            if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
            {
                if( true == pPlayer->IsContact )
                    pPlayer->SetPlayerState( MID_RONAN4_DASH_TO_WAIT );
                else
                    pPlayer->SetPlayerState( MID_RONAN4_DASH_TO_JUMP );

                pPlayer->cFrame = 0;
            }
        }
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();
    return 0;
}

int KGCPlayerRonan4::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch && !g_MyD3D->MyCtrl->k_Up)
		{
			if ( pPlayer->Next_Attack_Count > 0 &&
				pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A1 )
			{			
				pPlayer->Next_Attack_Count = 255;
			}
		}	

		if( g_MyD3D->MyCtrl->k_Up )
		{
			if ( pPlayer->Next_Attack_Count > 0 &&
				pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A1 )
			{			
				pPlayer->Next_Attack_Count = 255;
			}
		}	
	}

	if( g_MyD3D->MyCtrl->k_Up && pPlayer->Next_Attack_Count == 255)
	{		
		if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A1 )
		{
			if( pPlayer->cFrame >= 10 )
			{
				pPlayer->SetPlayerState(MID_RONAN4_DASH_ATK_B1);                                    			
				pPlayer->cFrame = 0;
			}
		}		
		return 0;
	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( pPlayer->IsContact )
		{
			if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A1 )
			{
				pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_A2_TO_WAIT );
				pPlayer->cFrame = 0;
			}

			if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A3 )
			{
				pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_A3_TO_WAIT );
				pPlayer->cFrame = 0;
			}
		}		
		else
		{
			if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A1 )
			{
				pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_A2_TO_JUMP );
				pPlayer->cFrame = 0;
			}

			if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_A3 )
			{
				pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_A3_TO_JUMP );
				pPlayer->cFrame = 0;
			}
		}
	}
    return 0;
}

int KGCPlayerRonan4::OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if ( pPlayer->Next_Attack_Count > 0 )
			{
				pPlayer->Next_Attack_Count = 255;
			}
		}
	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_B1 )
		{
			pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_B1_TO_JUMP );
			pPlayer->cFrame = 0;
		}		

		//if( pPlayer->uiMotion == MID_RONAN4_DASH_ATK_B2)
		//{
		//	pPlayer->SetPlayerState( MID_RONAN4_DASH_ATK_B2_TO_JUMP );
		//	pPlayer->cFrame = 0;
		//}
	}
	return 0;
}

int KGCPlayerRonan4::OnDashToJump( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RONAN4_DASH_TO_WAIT );
            pPlayer->cFrame = 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_RONAN4_JUMP );
            pPlayer->cFrame = 50;
            pPlayer->m_bFrameLock = true;
        }
    }

    return 0;
}

int KGCPlayerRonan4::OnDashAtkToWait( PLAYER* pPlayer, bool bKeyProc )
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
	}
	return 0;
}

int KGCPlayerRonan4::OnDashAtkToJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
		pPlayer->cFrame = 50;
	}
	return 0;
}

int KGCPlayerRonan4::OnJumpAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Down &&
			pPlayer->uiMotion == MID_RONAN4_JUMP_ATK_A1 )
		{
			if(g_MyD3D->MyCtrl->k_Fresh_Punch)
			{
				if ( pPlayer->Next_Attack_Count > 0 )
				{
					pPlayer->Next_Attack_Count = 255;
				}					
			}			
		}	
	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 && !pPlayer->IsContact )
	{
		pPlayer->SetPlayerState( MID_RONAN4_JUMP );
		pPlayer->cFrame = 50;
	}
	return 0;
}
	
int KGCPlayerRonan4::OnUniqueAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if ( pPlayer->Next_Attack_Count > 0 ||
				pPlayer->uiMotion == MID_RONAN4_UNIQUE_ATK_A1 )
			{
				pPlayer->Next_Attack_Count = 255;
			}	
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash && pPlayer->cFrame >= 25 )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
			pPlayer->cFrame = 0;
			return 0;
		}
		if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash && pPlayer->cFrame >= 25 )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
			pPlayer->cFrame = 0;
			return 0;
		}
	}
	return 0;
}

int KGCPlayerRonan4::OnSpecialCombo(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if ( pPlayer->Next_Attack_Count > 0 ||
				pPlayer->uiMotion == MID_RONAN4_AERIAL_COMBO_A2 ||
				pPlayer->uiMotion == MID_RONAN4_MAGIC_COMBO1 ||
				pPlayer->uiMotion == MID_RONAN4_CRITICAL_ATK ||
				pPlayer->uiMotion == MID_RONAN4_DOUBLE_ATK )
			{
				pPlayer->Next_Attack_Count = 255;
			}	
		}
	}
	return 0;
}

int KGCPlayerRonan4::OnMonoVolt(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame == 0 )
	{
		pPlayer->m_bChargeMonoVolt = true;
	}	
	return 0;
}

int KGCPlayerRonan4::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode)
		{
			if ( pPlayer->GetBurnningPoint() >= 3  ){
				pPlayer->SetPlayerState(MID_RONAN4_BURNING_MODE);
				pPlayer->cFrame = 0;
				pPlayer->ChangeBurnningTemplate();
				g_MyD3D->MyCtrl->k_Up = false;
				g_MyD3D->MyCtrl->k_Skill_Key = false;
			}
			return 0;
		}
		else if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && pPlayer->m_bIsBurnningMode)
		{
			pPlayer->InitBurnningGauge();
			pPlayer->SetMagicEffect(EGC_EFFECT_RONAN4_BURNING_MODE_UNEQUIP, 1);
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			g_MyD3D->MyCtrl->k_Skill_Key = false;

			return 0;
		}
	}

	return KGCPlayerCommon::OnWait(pPlayer, bKeyProc);
}