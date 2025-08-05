#include "stdafx.h"
//
#include "MyD3D.h"




#include "KGCPlayerRonan2.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"


KGCPlayerRonan2::KGCPlayerRonan2()
{

}

KGCPlayerRonan2::~KGCPlayerRonan2()
{

}

int KGCPlayerRonan2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(MID_RONAN2_DASH_ATK1);
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
            ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 30)//정지
        {
            pPlayer->cFrame = 30;
            return 0;
        }

        return 0;
    }

    ////무한 반복하는 프레임..
    if(pPlayer->cFrame == 29 )
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

int KGCPlayerRonan2::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
                pPlayer->SetPlayerState( MID_RONAN2_JUMP_DOWN_ATK );
                pPlayer->NoCheckContact = 25;
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

int KGCPlayerRonan2::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 || pPlayer->uiMotion == MID_RONAN2_ATK1 || pPlayer->uiMotion == MID_RONAN2_ATK2 )
            {
                pPlayer->Next_Attack_Count = 255;
            }

        }     

        if( pPlayer->uiMotion  != MID_RONAN2_ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_RONAN2_ATK2:
            case MID_RONAN2_ATK3:
            case MID_RONAN2_ATK4:
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                break;
            case MID_RONAN2_ATK5:
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
                break;
            default:
                return 0;
            }

            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else pPlayer->bIsRight = false;

            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }

    return 0;
}

int KGCPlayerRonan2::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( pPlayer->IsContact )
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
            }
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->cFrame = 0;
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

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerRonan2::OnBackStep(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->Next_Attack_Count = 255;
            return 0;
        }
        return 0;
    }
    return 0;
}


int KGCPlayerRonan2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
            }			
        }
    }

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
        {
            switch ( pPlayer->uiMotion )
            {
                case MID_RONAN2_DASH_ATK1:
                    pPlayer->SetPlayerState( MID_RONAN2_DASH_ATK1_END_TO_WAIT );
                    pPlayer->cFrame = 0;
                    break;
                case MID_RONAN2_DASH_ATK2:
                    pPlayer->SetPlayerState( MID_RONAN2_DASH_ATK2_END_TO_WAIT );
                    pPlayer->cFrame = 0;
                    break;
            }
        }
        else
        {
            switch ( pPlayer->uiMotion )
            {
                case MID_RONAN2_DASH_ATK1:
                    pPlayer->SetPlayerState( MID_RONAN2_DASH_ATK1_END_TO_JUMP );
                    pPlayer->cFrame = 0;
                    break;
                case MID_RONAN2_DASH_ATK2:
                    pPlayer->SetPlayerState( MID_RONAN2_DASH_ATK2_END_TO_JUMP );
                    pPlayer->cFrame = 0;
                    break;
            }

        }
        
    }   
 
    return 0;
}


int KGCPlayerRonan2::OnBackStepAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame > 19 )
        {
            if( pPlayer->IsContact )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            else
                return KGCPlayerCommon::OnJump( pPlayer, bKeyProc );
        }
    }

    return 0;
}

int KGCPlayerRonan2::OnJumpDownAtk(PLAYER* pPlayer, bool bKeyProc)
{
    //모션의 끝까지 왔으면
    
    if( 28 <= pPlayer->cFrame )
        if( !pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RONAN2_JUMP_DOWN_ATK_END_TO_JUMP );
            pPlayer->cFrame = 0;
        }

        
    
    return 0;
}

int KGCPlayerRonan2::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode)
		{
			if ( pPlayer->GetBurnningPoint() >= 3  ){
				pPlayer->SetPlayerState(MID_RONAN2_SPEAR_MODE);
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
			pPlayer->SetMagicEffect(EGC_EFFECT_RONAN2_SPEAR_MODE_UNEQUIP, 1);
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			g_MyD3D->MyCtrl->k_Skill_Key = false;

			return 0;
		}
	}

	return KGCPlayerCommon::OnWait(pPlayer, bKeyProc);
}