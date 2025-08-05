#include "stdafx.h"
//





#include "KGCPlayerJin.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"


#include "Damage.h"

KGCPlayerJin::KGCPlayerJin()
{

}

KGCPlayerJin::~KGCPlayerJin()
{

}

int KGCPlayerJin::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			//if( !pPlayer->IsContact )
			//{
			//	if ( g_MyD3D->MyCtrl->k_Up )
			//	{                
			//		pPlayer->SetPlayerState( MID_JIN_DRAGON_KICK );
			//		g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			//		g_MyD3D->MyCtrl->k_Up = false;
			//		pPlayer->cFrame = 0;
			//		return 0;
			//	}
			//}
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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
				pPlayer->SetPlayerState( MID_JIN_RUN_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_RUN_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}

int KGCPlayerJin::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
			//if ( g_MyD3D->MyCtrl->k_Up )
			//{                
			//	pPlayer->SetPlayerState( MID_JIN_DRAGON_KICK );                
			//}
			//else
			//{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );                
			//}   
			pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;                
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			g_MyD3D->MyCtrl->k_Up = false;
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
		if( pPlayer->cFrame > 50 )
			pPlayer->cFrame = 50;
	}
	else 
	{
		if( pPlayer->cFrame > 50 )
			pPlayer->cFrame = 50;
		//무한 반복하는 프레임..
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
			pPlayer->m_bFrameLock = true;
	}
	return 0;
}

int KGCPlayerJin::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            //if( pPlayer->Next_Attack_Count > 0 ||
			//	pPlayer->uiMotion == MID_JIN_ATK3 )
            //{
                pPlayer->Next_Attack_Count = 255;                
            //}
			return 0;
        }

		if( g_MyD3D->MyCtrl->k_Fresh_Down )
		{	
			if( pPlayer->cFrame <= 12 && !pPlayer->IsOldMotion( 3025 ) )
			{
				pPlayer->SetPlayerState( MID_JIN_UNIQUE_EVASION );
				pPlayer->cFrame = 0;				
				return 0;
			}
		}

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) && 
			pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 && pPlayer->cFrame <= 12 )
		{	
            if( pPlayer->m_fBurnningPoint >= 0.6f || pPlayer->m_bIsBurnningMode || pPlayer->IsMagicEffect( EGC_EFFECT_JIN1_SPECIAL4_BUFF ) ) {
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
			    pPlayer->SetPlayerState( MID_JIN_UNIQUE_HUJIN );				
			    pPlayer->cFrame = 0;
			    return 0;
            }
		}

		if( pPlayer->uiMotion  == pPlayer->GetCurFormTemplate().ATK1 )
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Up )
			{	
				if( pPlayer->cFrame <= 8 )
				{
					if( pPlayer->m_fBurnningPoint >= 0.75f || pPlayer->m_bIsBurnningMode || pPlayer->IsMagicEffect( EGC_EFFECT_JIN1_SPECIAL4_BUFF ) )
						pPlayer->SetPlayerState( MID_JIN_UNIQUE_HANDWIND );
					else
						pPlayer->SetPlayerState( MID_JIN_UNIQUE_HANDWIND_BP_EMPTY );
					pPlayer->cFrame = 0;				
					return 0;	
				}
			}	
		}

        if( pPlayer->uiMotion  != pPlayer->GetCurFormTemplate().ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_JIN_ATK2:
            case MID_JIN_ATK3:
            case MID_JIN_ATK4:			
				pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                break;
			case MID_JIN_ATK5:
				pPlayer->AddDamage(DT_DOUBLE,0);
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

int KGCPlayerJin::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
			if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
				|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
			{
				if( pPlayer->m_bIsBurnningMode || pPlayer->m_fBurnningPoint >= 0.6f || pPlayer->IsMagicEffect( EGC_EFFECT_JIN1_SPECIAL4_BUFF ) )
					pPlayer->SetPlayerState( MID_JIN_UNIQUE_SPINKICK );
				else
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				
			}
			else
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				

			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();
		return 0;
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
		pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
	{
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN_DASH_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_DASH_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}

int KGCPlayerJin::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if(  pPlayer->cFrame >= 5 && pPlayer->cFrame <= 15  )
			{
				pPlayer->m_iBranchSkill = SKILL_BRANCH_1;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}			
			else if(  15 < pPlayer->cFrame && pPlayer->cFrame <= 24 )
			{
				pPlayer->m_iBranchSkill = -1;
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_1 );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}
			else
				return 0;
		}

		if( g_MyD3D->MyCtrl->k_Up )
		{
			if(  pPlayer->cFrame >= 5 && pPlayer->cFrame <= 15  )
			{
				pPlayer->m_iBranchSkill = SKILL_BRANCH_2;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}			
			else if(  15 < pPlayer->cFrame && pPlayer->cFrame <= 24 )
			{
				pPlayer->m_iBranchSkill = -1;
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_2 );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				return 0;
			}
			else
				return 0;
		}		
	}

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_JIN_DASH_ATK_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_JIN_DASH_ATK_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}


int KGCPlayerJin::OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
			pPlayer->cFrame = 0;
		}
		else
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
			pPlayer->cFrame = 50;
			pPlayer->m_bFrameLock = true;
		}
	}
	return 0;
}

int KGCPlayerJin::OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( pPlayer->uiMotion == MID_JIN_DASH_ATK_1 )
		{
			if( pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_1_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_1_TO_JUMP );
			pPlayer->cFrame = 0;
		}		
		else if( pPlayer->uiMotion == MID_JIN_DASH_ATK_2 )
		{
			if( pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_2_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_DASH_ATK_2_TO_JUMP );
			pPlayer->cFrame = 0;
		}
		else
			return 0;
	}
	return 0;
}