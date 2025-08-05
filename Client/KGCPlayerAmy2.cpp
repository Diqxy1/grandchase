#include "stdafx.h"
// 에이미 1차전직 - 파이팅모드

#include ".\kgcplayeramy2.h"

//





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"
//


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)
//#define COMBO_TEST

KGCPlayerAmy2::KGCPlayerAmy2(void)
{
}

KGCPlayerAmy2::~KGCPlayerAmy2(void)
{
}

int KGCPlayerAmy2::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
#ifndef COMBO_TEST
            if( pPlayer->Next_Attack_Count > 0 )
            {
#endif
                pPlayer->Next_Attack_Count = 255;
#ifndef COMBO_TEST
            }
#endif
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion == MID_AMY2_ATK2 || pPlayer->uiMotion == MID_AMY2_ATK3 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_CRITICAL,0);
                pPlayer->SetPlayerState( MID_AMY2_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( pPlayer->uiMotion == MID_AMY2_ATK4 )
            {
                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;
                pPlayer->AddDamage(DT_DOUBLE,0);
                pPlayer->SetPlayerState( MID_AMY2_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                return 0;
            }
        }
        return 0;
    }

    return 0;
}

int KGCPlayerAmy2::OnRun(PLAYER* pPlayer, bool bKeyProc)
{  
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {            
            
            if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON) 
            {
            
                int randomNum = rand()%10;

                if( randomNum ==0 )
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK2);
                }
                else
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);
                }
            } 
            else 
            {

            pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);

            }

            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }

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
                pPlayer->SetPlayerState( MID_AMY2_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY2_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy2::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            if ( g_MyD3D->MyCtrl->k_Down && pPlayer->cFrame >= 10 && pPlayer->cFrame <= 38 )
            {                
                pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1 );                
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
#ifdef MATCH_BALLENCE_121217
			if( pPlayer->Before_Motion != MID_AMY2_SWAP_TO_PERFORMENCE_JUMP && 
                pPlayer->Before_Motion != MID_AMY2_SWAP_TO_FIGHTMODE_JUMP )
#else
			if(pPlayer->Before_Motion != MID_AMY2_SWAP_TO_PERFORMENCE_JUMP )
#endif				
			{
				KGCPlayerCommon::DoubleJumpFunc( pPlayer );			
				pPlayer->m_bFrameLock = true;            
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

int KGCPlayerAmy2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    // 주의!! 점프 더블대쉬후에는 공격이 안된다.

    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                KGCPlayerCommon::DashJumpFunc( pPlayer );
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {            
            if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
            {

                int randomNum = rand() % 10;

                if (randomNum == 0)
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK2);
                }
                else
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);
                }
            }
            else
            {

                pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);

            }
            
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
            pPlayer->SetPlayerState( MID_AMY2_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY2_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerAmy2::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                KGCPlayerCommon::DashJumpFunc( pPlayer );
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {            
            if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
            {

                int randomNum = rand() % 10;

                if (randomNum == 0)
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK2);
                }
                else
                {
                    pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);
                }
            }
            else
            {

                pPlayer->SetPlayerState(MID_AMY2_DASH_ATK1);

            }

            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }

        if( pPlayer->IsContact && ( pPlayer->bIsRight && ISPUSHED(k_Right)
            || !pPlayer->bIsRight && ISPUSHED(k_Left) ) )
        {
            pPlayer->SetPlayerState( MID_AMY2_RUN );
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
            pPlayer->SetPlayerState( MID_AMY2_DASH );
            pPlayer->cFrame = 0;

            return 0;
        }

        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY2_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY2_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}


int KGCPlayerAmy2::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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
                pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1 );                
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
        if( pPlayer->cFrame >= 33 )
        {
            pPlayer->cFrame = 33;

            //무한 반복하는 프레임..
            if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
                pPlayer->m_bFrameLock = true;
        }
    }
    else 
    {
        if( pPlayer->cFrame < 33 )
            pPlayer->cFrame = 33;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy2::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // 점프 더블 대쉬가 있으면 그걸로...
            pPlayer->SetPlayerState( MID_AMY2_DASH );
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
            else
            {
                if( g_MyD3D->MyCtrl->k_Down )
                {
                    //g_MyD3D->MyCtrl->k_Down = false;
                    pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1 );
                    pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                }
                else
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
                }
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;

                g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                pPlayer->cFrame = 0;

                return 0;
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
            pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_JUMP );
            pPlayer->cFrame = 33;
            return 0;
        }
    }

    return 0;
}


int KGCPlayerAmy2::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
                return KGCPlayerCommon::DashJumpFunc( pPlayer ,MID_AMY2_RUN_TO_DOUBLE_JUMP );                 
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
            pPlayer->cFrame = 33;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy2::OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc)
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
        if( pPlayer->cFrame >= 33 )
        {
            pPlayer->cFrame = 33;

            //무한 반복하는 프레임..
            if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
                pPlayer->m_bFrameLock = true;
        }
    }
    else 
    {
        if( pPlayer->cFrame <= 33 )
            pPlayer->cFrame = 33;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy2::OnUniqueSkill(PLAYER* pPlayer, bool bKeyProc)
{
    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
    {
        if( pPlayer->IsContact )
        {
            pPlayer->x_Speed = 0.0;
            pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1_TO_WAIT );          
            pPlayer->cFrame = 0;            
            return 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1_SKY_TO_JUMP );                      
            pPlayer->cFrame = 0;
            return 0;
        }        
    }
    else
    {
        
        if( pPlayer->cFrame == 0)
            pPlayer->NoCheckContact = 16;


        if( pPlayer->IsContact &&  pPlayer->cFrame >=16 )
        {            
            pPlayer->x_Speed = 0.0;
            pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK1_TO_WAIT );          
            pPlayer->cFrame = 0;            
            return 0;
        }
    }    
    return 0;
}

int KGCPlayerAmy2::OnUniqueSkillAtk(PLAYER* pPlayer, bool bKeyProc)
{

    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_AMY2_SPECIAL_ATK2 );
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( MID_AMY2_DASH_ATK2 == pPlayer->uiMotion )
    {
        if( true == pPlayer->IsContact )
        {
            pPlayer->m_bFrameLock = false;
        }
        else if( 38 == pPlayer->cFrame )
        {
            pPlayer->m_bFrameLock = true;
        }
        return 0;
    }        

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( MID_AMY2_DASH_ATK1 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_AMY2_DASH_ATK1_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY2_DASH_ATK1_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy2::OnDashAtk1ToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->cFrame = 33;
            return 0;
        }
    }

    return 0;
}