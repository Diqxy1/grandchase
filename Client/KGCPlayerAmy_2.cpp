#include "stdafx.h"
#include ".\KGCPlayerAmy_2.h"

//





#include "KGCPlayerCommon.h"

#include "DamageManager.h"
#include "Controls.h"

#include "Stage.h"
//


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)
//#define COMBO_TEST

KGCPlayerAmy_2::KGCPlayerAmy_2(void)
{
}

KGCPlayerAmy_2::~KGCPlayerAmy_2(void)
{
}

int KGCPlayerAmy_2::OnRun(PLAYER* pPlayer, bool bKeyProc)
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
            else
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer ,MID_AMY1_RUN_TO_DOUBLE_JUMP );                
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
                ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 22 )//����
            {
                pPlayer->cFrame = 23;
                return 0;
            }
        }

        return 0;
    }

    //���� �ݺ��ϴ� ������..
    if( pPlayer->cFrame == 22 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 22 )
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
                pPlayer->SetPlayerState( MID_AMY1_DANCE_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY1_RUN_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy_2::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
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
                pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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
			if(pPlayer->Before_Motion != MID_AMY1_SWAP_TO_JUMP )
			{
				KGCPlayerCommon::DoubleJumpFunc( pPlayer );
				return 0;
			}
        }

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->y_Speed >= 0.0f )
    {
        //�������� ���� ����
        if( pPlayer->cFrame > 40 )
            pPlayer->cFrame = 40;
    }
    else 
    {
        //���� �ݺ��ϴ� ������..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy_2::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    // ����!! ���� ����뽬�Ŀ��� ������ �ȵȴ�.

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
            if( pPlayer->IsContact )
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            else
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);

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

        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // ���� ���� �뽬�� ������ �װɷ�...
            pPlayer->SetPlayerState( MID_AMY1_DOUBLE_DASH );
            pPlayer->cFrame = 0;

            return 0;
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
            pPlayer->SetPlayerState( MID_AMY1_DANCE_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY1_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerAmy_2::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( pPlayer->IsContact )
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            else
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);

            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( pPlayer->IsContact && ( pPlayer->bIsRight && ISPUSHED(k_Right)
            || !pPlayer->bIsRight && ISPUSHED(k_Left) ) )
        {
            pPlayer->SetPlayerState( MID_AMY1_RUN );
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
            // ���� ���� �뽬�� ������ �װɷ�...
            pPlayer->SetPlayerState( MID_AMY1_DOUBLE_DASH );
            pPlayer->cFrame = 0;

            return 0;
        }

        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY1_DANCE_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY1_DASH_TO_JUMP );
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerAmy_2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( MID_AMY1_DASH_ATK1 == pPlayer->uiMotion
                && 11 <= pPlayer->cFrame )
            {
                pPlayer->SetPlayerState( MID_AMY1_DASH_ATK2 );
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;

                return 0;
            }
        }
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( MID_AMY1_DASH_ATK1 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_AMY1_DANCE_DASH_ATK1_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY1_DASH_ATK1_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }
        else if( MID_AMY1_DASH_ATK2 == pPlayer->uiMotion )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_AMY1_DANCE_DASH_ATK2_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY1_DASH_ATK2_TO_JUMP );
            pPlayer->cFrame = 0;

            return 0;
        }        
    }

    return 0;
}

int KGCPlayerAmy_2::OnSpecial1Atk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        // ������ �� �ɸ� ������ �¿� ���� �ٲ� �� �ְ� �Ѵ�.
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
        {
            //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
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
                if( 10 < pPlayer->cFrame )
                {
                    pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK2 );
                    pPlayer->cFrame = 0;
                    pPlayer->NoCheckContact = 25;
                    return 0;
                }
            }
        }
    }

    //���� �ݺ��ϴ� ������..
    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerAmy_2::OnSpecial2Atk(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->IsContact && pPlayer->cFrame > 17 )
    {
        // �ٷ� ���� �ع���
        pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {

            pPlayer->SetPlayerState( MID_AMY1_DANCE_UNIQUE_ATK2_TO_WAIT );
            pPlayer->cFrame = 0;
            pPlayer->x_Speed = 0.0f;
            return 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK2_TO_JUMP );
            pPlayer->cFrame = 0;
            pPlayer->x_Speed = 0.0f;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy_2::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
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
                pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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
        //�������� ���� ����
        if( pPlayer->cFrame >= 33 )
        {
            pPlayer->cFrame = 33;

            //���� �ݺ��ϴ� ������..
            if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
                pPlayer->m_bFrameLock = true;
        }
    }
    else 
    {
        if( pPlayer->cFrame < 33 )
            pPlayer->cFrame = 33;
        //���� �ݺ��ϴ� ������..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy_2::OnDoubleDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( pPlayer->IsContact )
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                pPlayer->cFrame = 0;
                return 0;
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
                pPlayer->cFrame = 0;
                return 0;
            }
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

        return 0;
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
            pPlayer->SetPlayerState( MID_AMY1_DANCE_DOUBLE_DASH_TO_WAIT );
        else
            pPlayer->SetPlayerState( MID_AMY1_DOUBLE_DASH_TO_JUMP );
        pPlayer->cFrame = 0;

        g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
        g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
    }

    return 0;
}

int KGCPlayerAmy_2::OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( (true == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Right_Dash) ||
            (false == pPlayer->bIsRight && true == g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            // ���� ���� �뽬�� ������ �װɷ�...
            pPlayer->SetPlayerState( MID_AMY1_DOUBLE_DASH );
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
                    pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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

int KGCPlayerAmy_2::OnDoubleDashToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
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
                    pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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

int KGCPlayerAmy_2::OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
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
                    pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer );
            }
            else
            {
                return KGCPlayerCommon::DashJumpFunc( pPlayer ,MID_AMY1_RUN_TO_DOUBLE_JUMP );                
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

    // ������ �ӵ� ����
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

int KGCPlayerAmy_2::OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
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
                pPlayer->SetPlayerState( MID_AMY1_UNIQUE_ATK1 );
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

        return 0;
    }

    if( pPlayer->y_Speed >= 0.0f )
    {
        //�������� ���� ����
        if( pPlayer->cFrame >= 33 )
        {
            pPlayer->cFrame = 33;

            //���� �ݺ��ϴ� ������..
            if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
                pPlayer->m_bFrameLock = true;
        }
    }
    else 
    {
        if( pPlayer->cFrame < 33 )
            pPlayer->cFrame = 33;
        //���� �ݺ��ϴ� ������..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerAmy_2::OnSpecialAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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
            pPlayer->m_bFrameLock = true;
            pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerAmy_2::OnDashAtk1ToWaitJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( 15 > pPlayer->cFrame )
            {
                pPlayer->SetPlayerState( MID_AMY1_DASH_ATK2 );
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;

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

int KGCPlayerAmy_2::OnDashAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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

int KGCPlayerAmy_2::OnSpecial1(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        //���� �ٲٱ� �����ϰ� ���� �������� ���� ����
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
        }
    }

    return 0;
}

int KGCPlayerAmy_2::OnModeChange(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( pPlayer->IsContact )
        {
            if( MID_AMY1_SWAP_TO_DANCE_MODE == pPlayer->uiMotion )
                pPlayer->SetPlayerState( MID_AMY1_SWAP_TO_DANCEMODE_WAIT );
            else
                pPlayer->SetPlayerState( MID_AMY1_SWAP_TO_FIGHTMODE_WAIT );
        }
        else
        {
            pPlayer->SetPlayerState( MID_AMY1_SWAP_TO_JUMP );            
        }
        pPlayer->cFrame = 0;

        return 0;
    }
    return 0;
}

int KGCPlayerAmy_2::OnModeChangeToWaitJump(PLAYER* pPlayer, bool bKeyProc)
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