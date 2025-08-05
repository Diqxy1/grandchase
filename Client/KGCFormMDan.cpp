#include "stdafx.h"
#include ".\KGCFormMDan.h"





#include "KGCPlayerCommon.h"
#include "DamageManager.h"
#include "Controls.h"

//
#include "Stage.h"


#include "GCCameraOrtho.h"

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCFormMDan::KGCFormMDan(void)
{
}

KGCFormMDan::~KGCFormMDan(void)
{
}

int KGCFormMDan::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Select_Item )
        {
            pPlayer->ProcessMetamorphosisNormalForm();
            pPlayer->Direct_Motion_Input( MID_RYAN4_WAIT );
            pPlayer->cFrame = 0;
            pPlayer->GetMetaForm(FORM_NORMAL)->SetWorldMat( pPlayer->GetMetaForm(FORM_MDAN)->GetWorldMat() );
            g_MyD3D->MyCtrl->k_Select_Item = false;
            return 0;
        }
        return KGCPlayerCommon::OnWait( pPlayer, true );
    }

    return KGCPlayerCommon::OnWait( pPlayer, false );
}

int KGCFormMDan::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->uiMotion == MID_RYAN4_MDAN_ATK3 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
            else
            {
                if( pPlayer->Next_Attack_Count > 0 )
                {
                    pPlayer->Next_Attack_Count = 255;
                }
            }

            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_RYAN4_MDAN_ATK2 ||
                pPlayer->uiMotion == MID_RYAN4_MDAN_ATK3 ||
                pPlayer->uiMotion == MID_RYAN4_MDAN_ATK4 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_CRITICAL_ATK );
                pPlayer->cFrame = 0;
            }
            return 0;
        }
        return 0;
    }

    return 0;
}

int KGCFormMDan::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            ISPUSHED(k_Fresh_Punch) = false;
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_ATK1 );
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

                pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
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
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && ISPUSHED(k_Right) )
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( pPlayer->uiMotion == MID_RYAN4_MDAN_RUN )
        {
            if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
                ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < pPlayer->GetNowMotionFRM()->GetNum_Frame()-1 )//정지
            {
                pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame();
                return 0;
            }
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == pPlayer->GetNowMotionFRM()->GetNum_Frame()-1 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < pPlayer->GetNowMotionFRM()->GetNum_Frame()-1 )
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }


    if( pPlayer->uiMotion == MID_RYAN4_MDAN_RUN )
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
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_RUN_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
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
            pPlayer->SetPlayerState(MID_RYAN4_MDAN_JUMP_ATK);
            pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPlayer->bIsRight = true;
            pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH);
            pPlayer->cFrame = 0;
            return 0;
        }
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPlayer->bIsRight = false;
            pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH);
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

    if( pPlayer->y_Speed >= 0.0f )
    {
        //내려오는 점프 동작
        if( pPlayer->cFrame > 23 )
            pPlayer->cFrame = 23;
    }
    else 
    {
        if( pPlayer->cFrame < 23 )
            pPlayer->cFrame = 23;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCFormMDan::OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 31 )
        pPlayer->m_bFrameLock = true;

    if( pPlayer->IsContact )
    {
        switch( pPlayer->MetamorphosisForm )
        {
        case FORM_NORMAL:
            pPlayer->SetPlayerState( MID_RYAN4_WAIT );
            break;
        case FORM_MDAN:
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_WAIT );
            break;
        }
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCFormMDan::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            ISPUSHED(k_Fresh_Punch) = false;
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }

		// 2007/2/7. iridology. 일명 스텝이 되게 하는 코드임.
		if( ISPUSHED(k_Down) )
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

        return 0;
    }	

    if( pPlayer->uiMotion == MID_RYAN4_MDAN_DASH )
    {
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
	// 2007/2/6. iridology. 혹 대쉬가 무디면 여기다가 처리를 해줘야함..

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

                pPlayer->SetPlayerState(MID_RYAN4_MDAN_JUMP);
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( (g_MyD3D->MyCtrl->k_Left && pPlayer->GetIsRight() == true) ||
                (g_MyD3D->MyCtrl->k_Right && pPlayer->GetIsRight() == false) )
            {
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_BACK_DASH_ATK );
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                pPlayer->cFrame = 0;
                return 0;
            }

            ISPUSHED(k_Fresh_Punch) = false;
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_ATK1 );
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
        
        if( ((ISPUSHED(k_Right) && pPlayer->bIsRight) || ISPUSHED(k_Left) && !pPlayer->bIsRight)
            && pPlayer->cFrame > 14
            )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_RUN );
            pPlayer->cFrame = 0;
            return 0;
        }

        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    if( pPlayer->uiMotion == MID_RYAN4_MDAN_DASH )
    {
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashToJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( false == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnJumpAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( false == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnRunToJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( false == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashAtk1ToJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( false == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashAtk2ToJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( false == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Right) || ISPUSHED(k_Fresh_Left) )
        {
            if( MID_RYAN4_MDAN_DASH_ATK1 == pPlayer->uiMotion
                && pPlayer->cFrame > 11
                )
            {
                if( (pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Fresh_Right)
                    || (!pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Fresh_Left) )
                {
                    pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2);

                if( g_MyD3D->MyCtrl->k_Fresh_Right )
                {
                    g_MyD3D->MyCtrl->k_Fresh_Right = false;
                    pPlayer->SetIsRight(true);
                }
                else
                {
                    g_MyD3D->MyCtrl->k_Fresh_Left = false;
                    pPlayer->SetIsRight(false);
                }
                }
                else
                {
                    pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2_BACK);

                    if( g_MyD3D->MyCtrl->k_Fresh_Right )
                    {
                        g_MyD3D->MyCtrl->k_Fresh_Right = false;
                        pPlayer->SetIsRight(false);
                    }
                    else
                    {
                        g_MyD3D->MyCtrl->k_Fresh_Left = false;
                        pPlayer->SetIsRight(true);
                    }
                }

                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                pPlayer->cFrame = 0;
                return 0;
            }
        }
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( MID_RYAN4_MDAN_DASH_ATK1 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_ATK1_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RYAN4_MDAN_DASH_ATK1_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
        else if( MID_RYAN4_MDAN_DASH_ATK2 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_WAIT);
            else
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_JUMP);
            pPlayer->cFrame = 0;

            return 0;
        }
        else if( MID_RYAN4_MDAN_DASH_ATK2_BACK == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_WAIT);
            else
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_JUMP);
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashToWait(PLAYER* pPlayer, bool bKeyProc)
{
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
    

    return 0;
}
int KGCFormMDan::OnDashAtk2BackToJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        pPlayer->SetIsRight( !pPlayer->GetIsRight() );

        if( pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;            
            return 0;
        }
    }

    return 0;
}

int KGCFormMDan::OnDashAtk2BackToWait(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        pPlayer->SetIsRight( !pPlayer->GetIsRight() );

        if( pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_RYAN4_MDAN_JUMP );
            pPlayer->cFrame = 23;            
            return 0;
        }
    }

    return 0;
}

//쓰러졌다가 일어서는 ...
int KGCFormMDan::OnDownAndStandUp(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //땅에 누워서 하는 데쉬 -> Tiger Rolling Attack이 발동할 수 있음
        // MID_COMMON_FRONTDOWN_AND_STANDUP 에서도 그라운드다운 데쉬가 발동할 수 있어야함
        if( ( 0.0f != pPlayer->m_fAbility[ABILITY_ROLLING] ) && pPlayer->IsContact && pPlayer->GetHP() > 0 && pPlayer->cFrame > 14 && pPlayer->cFrame < 29 )
            //pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH != -1 && )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
            {
                pPlayer->bIsRight = true;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;                 
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
            {
                pPlayer->bIsRight = false;
                if( g_kCamera.CheckDashZoom() )
                    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
                pPlayer->cFrame = 0;
                pPlayer->SetPlayerState(MID_COMMON_TIGER_ROLLING_ACTION);
                //pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().GROUND_DOWN_DASH);
                return 0;
            }
        }

        if( MID_RYAN4_MDAN_STANDUP_AND_ATK != -1 && pPlayer->GetHP() != 0.0f)
        {           
            if(pPlayer->cFrame > 16 && pPlayer->cFrame < 30 && g_MyD3D->MyCtrl->k_Fresh_Punch)
            {
                pPlayer->m_bReserveWake = true;
                return 0;
            }
            if(pPlayer->cFrame == 30 && pPlayer->m_bReserveWake)
            {
                pPlayer->SetPlayerState(MID_RYAN4_MDAN_STANDUP_AND_ATK);
                pPlayer->cFrame = 0;
                pPlayer->m_bReserveWake = false;
            }
        }
    }
    if( pPlayer->cFrame == 16 )
    {
        pPlayer->m_bReserveWake = false;
    }
    return 0;
}