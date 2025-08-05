#include "stdafx.h"
// 에이미 1차전직 - 연주모드 dolki

#include ".\kgcplayeramy2_2.h"

//





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"
//


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)
//#define COMBO_TEST

KGCPlayerAmy2_2::KGCPlayerAmy2_2(void)
{
}

KGCPlayerAmy2_2::~KGCPlayerAmy2_2(void)
{
}

int KGCPlayerAmy2_2::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 
                || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK1 
                || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK2
                || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK3)
            {
                pPlayer->Next_Attack_Count = 255;
            }
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up)
        {								
            if( pPlayer->uiMotion != MID_AMY2_PERFORMENCE_ATK1)
            {
                g_MyD3D->MyCtrl->k_Up = false;
                pPlayer->SetPlayerState(MID_AMY2_PERFORMENCE_SPECIAL_ATK);
                pPlayer->cFrame = 0;
                return 0;				
            }
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK2 ||
                pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK3 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( pPlayer->uiMotion == MID_AMY2_PERFORMENCE_ATK4 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }
        return 0;    
    }

    return 0;
}

int KGCPlayerAmy2_2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{  
    if(bKeyProc)
    {
        //if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        //{
        //    if( pPlayer->IsContact )
        //        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
        //    else
        //        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);

        //    pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
        //    pPlayer->cFrame = 0;
        //    g_MyD3D->MyCtrl->k_Fresh_Punch = false;
        //    return 0;
        //}

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                KGCPlayerCommon::DashJumpFunc( pPlayer );
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
                pPlayer->SetPlayerState( MID_AMY2_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY2_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerAmy2_2::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
                //g_MyD3D->MyCtrl->k_Down = false;
                pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_JUMPDOWN_ATK );
                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            }
            else
            {
                pPlayer->SetPlayerState(MID_AMY2_PERFORMENCE_JUMP_ATK);

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

#ifdef MATCH_BALLENCE_121217
        if( g_MyD3D->MyCtrl->k_Up )
        {
            if(pPlayer->Before_Motion != MID_AMY2_SWAP_TO_PERFORMENCE_JUMP )
            {
                KGCPlayerCommon::DoubleJumpFunc( pPlayer );			
                pPlayer->m_bFrameLock = true;            
            }
        }
#endif		

        pPlayer->SetWindSpeed();

        if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )// && pPlayer->GetCurFormTemplate().DOUBLE_JUMP != -1 )
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

int KGCPlayerAmy2_2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    // 주의!! 점프 더블대쉬후에는 공격이 안된다.
    if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
    {
        pPlayer->bIsRight = true;
    }
    else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
    {
        pPlayer->bIsRight = false;
    }

    if(bKeyProc)
    {                
        if( g_MyD3D->MyCtrl->k_Fresh_Punch ) 
        {
            if( g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight )
            {
                pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_DASH_CMD_ATK );                
                g_MyD3D->MyCtrl->k_Left = false;
            }
            else if( g_MyD3D->MyCtrl->k_Right && pPlayer->bIsRight == false )
            {
                pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_DASH_CMD_ATK );                                             
                g_MyD3D->MyCtrl->k_Right = false;
            } 
            else
            {
                 pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_DASH_ATK );                                  
            }   
            pPlayer->cFrame = 0;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                KGCPlayerCommon::DashJumpFunc( pPlayer );
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

    return 0;
}

int KGCPlayerAmy2_2::OnModeChange(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            if( MID_AMY2_SWAP_TO_PERFORMENCE_MODE == pPlayer->uiMotion )
                pPlayer->SetPlayerState( MID_AMY2_SWAP_TO_PERFORMENCEMODE_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY2_SWAP_TO_FIGHTMODE_WAIT );
        }
        else
        {
            if( MID_AMY2_SWAP_TO_PERFORMENCE_MODE == pPlayer->uiMotion )
                pPlayer->SetPlayerState( MID_AMY2_SWAP_TO_PERFORMENCE_JUMP );            
            else
                pPlayer->SetPlayerState( MID_AMY2_SWAP_TO_FIGHTMODE_JUMP );            
        }
        pPlayer->cFrame = 0;

        return 0;
    }
    return 0;
}

int KGCPlayerAmy2_2::OnModeChangeToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->SetPlayerState( MID_AMY2_RUN_TO_DOUBLE_JUMP );
            pPlayer->cFrame = 33;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy2_2::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // 점프 더블 대쉬가 있으면 그걸로...
            pPlayer->SetPlayerState( MID_AMY2_PERFORMENCE_DASH );
            pPlayer->cFrame = 0;

            return 0;
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
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
            pPlayer->cFrame = 33;
            return 0;
        }
    }


    return 0;
}


int KGCPlayerAmy2_2::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
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
    }

    return 0;
}


int KGCPlayerAmy2_2::OnSpecial3Atk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if(  pPlayer->bIsRight )
        {
            if( g_MyD3D->MyCtrl->k_Left)
            {
                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_READY )
                {                    
                    pPlayer->m_bIsPushLeftKey = true;
                    pPlayer->m_bIsPushRightKey = false;
                }                
            }

            if( g_MyD3D->MyCtrl->k_Right )
            {
                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_READY )
                {
                    pPlayer->m_bIsPushRightKey = true;
                    pPlayer->m_bIsPushLeftKey = false;
                }
                return 0;
            }
        }
        else
        {
            if( g_MyD3D->MyCtrl->k_Right )
            {
                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_READY )
                {
                    pPlayer->m_bIsPushLeftKey = true;
                    pPlayer->m_bIsPushRightKey = false;                    
                }
                return 0;
            }

            if( g_MyD3D->MyCtrl->k_Left )
            {
                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_READY )
                {
                    pPlayer->m_bIsPushRightKey = true;
                    pPlayer->m_bIsPushLeftKey = false;
                }
                return 0;
            }
        }
        
    }

    if( pPlayer->cFrame == 8 )
    {
        pPlayer->m_bIsPushLeftKey = false;
        pPlayer->m_bIsPushRightKey = false;
    }
    
    return 0;
}