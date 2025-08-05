#include "stdafx.h"


#include "MyD3D.h"
#include "KGCPlayerRonan.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"



KGCPlayerRonan::KGCPlayerRonan()
{

}

KGCPlayerRonan::~KGCPlayerRonan()
{

}

int KGCPlayerRonan::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if (g_MyD3D->MyCtrl->k_Fresh_Punch)
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            pPlayer->cFrame = 0;
            return 0;
        }
        
        if (g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_RONAN1_RENEW_X_DASH_ATK) )
        {
            if ( g_MyD3D->MyCtrl->k_Fresh_Skill_Key && !pPlayer->m_bIsBurnningMode )
            {
                pPlayer->SetPlayerState(MID_RONAN_DASH_X_ATK);
                g_MyD3D->MyCtrl->k_Fresh_Skill_Key = false;
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if (g_MyD3D->MyCtrl->k_Up)
        {
            if (pPlayer->IsContact)
            {
                return KGCPlayerCommon::DashJumpFunc(pPlayer);
            }
            pPlayer->NoCheckContact = 0;
        }

        if (g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;

            if (KGCPlayerCommon::CheckPortal(pPlayer, bKeyProc) == true)
                return 0;

            if (pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x, pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if (pPlayer->IsContact)
        {
            if (pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left)
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if (!pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right)
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if (((pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Right) ||
            (!pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Left)) && pPlayer->cFrame < 31)//정지
        {
            pPlayer->cFrame = 32;
            return 0;
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if (pPlayer->cFrame == 31)
        pPlayer->cFrame = 0;

    if (pPlayer->cFrame < 31)
    {
        if (pPlayer->bIsRight)
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();

    return 0;
}

/*
int KGCPlayerRonan::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( true == g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->uiMotion == MID_RONAN_ATK3 )
            {
                if( g_MyD3D->MyCtrl->k_Up )
                    pPlayer->Next_Motion = MID_RONAN_ATK_UP1;
                else
                    pPlayer->Next_Motion = MID_RONAN_ATK4;
            }
            else if( pPlayer->uiMotion == MID_RONAN_ATK4 )
            {
                pPlayer->Next_Attack_Count = 255;
                return 0;
            }
            else
            {
                if( pPlayer->Next_Attack_Count > 0 )
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }
            }
        }

        if( pPlayer->Next_Attack_Count == 0 && pPlayer->uiMotion != MID_RONAN_ATK4 && pPlayer->uiMotion != MID_RONAN_ATK5 )
            return 0;

        if( pPlayer->uiMotion  != MID_RONAN_ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_RONAN_ATK2:
            case MID_RONAN_ATK3:
            case MID_RONAN_ATK4:
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                break;
            case MID_RONAN_ATK5:
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
    if( pPlayer->uiMotion == MID_RONAN_ATK3 )
    {
        if( 26 == pPlayer->cFrame && -1 != pPlayer->Next_Motion )
        {
            pPlayer->SetPlayerState( pPlayer->Next_Motion );
            pPlayer->cFrame = 0;
            return 0;
        }
    }
    return 0;
}*/

int KGCPlayerRonan::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if (g_MyD3D->MyCtrl->k_Up)
        {
            if (pPlayer->IsContact)
            {
                return KGCPlayerCommon::DashJumpFunc(pPlayer);
            }
            pPlayer->NoCheckContact = 0;
        }
        if (g_MyD3D->MyCtrl->k_Fresh_Punch)
        {
            if (pPlayer->IsContact)
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
            }
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
            return 0;
        }
        /*else if (g_MyD3D->MyCtrl->k_Fresh_Skill_Key && !pPlayer->m_bIsBurnningMode && pPlayer->CheckSkill(SID_RONAN1_RENEW_X_DASH_ATK))
        {
            pPlayer->SetPlayerState(MID_RONAN_DASH_X_ATK);
            g_MyD3D->MyCtrl->k_Fresh_Skill_Key = false;
            pPlayer->cFrame = 0;
            return 0;
        }*/

        if ((g_MyD3D->MyCtrl->k_Fresh_Left && pPlayer->GetIsRight() == true) ||
            (g_MyD3D->MyCtrl->k_Fresh_Right && pPlayer->GetIsRight() == false))
        {
            pPlayer->SetPlayerState(MID_RONAN_BACK_STEP);
            pPlayer->cFrame = 0;
            return 0;
        }

        if (g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;

            if (KGCPlayerCommon::CheckPortal(pPlayer, bKeyProc) == true)
                return 0;

            if (pPlayer->uiMotion == MID_RONAN_DASH)
            {
                if (pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x, pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
                {
                    return 0;
                }

                pPlayer->y_Speed += -0.01f;
                pPlayer->NoCheckContact = 20;
            }
            return 0;
        }

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();
    return 0;
}

int KGCPlayerRonan::OnBackStep(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if (g_MyD3D->MyCtrl->k_Fresh_Punch)
        {
            pPlayer->Next_Attack_Count = 255;
            return 0;
        }
        return 0;
    }
    return 0;
}


int KGCPlayerRonan::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    // 마지막 프레임 이라면
    if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1)
    {
        if (true == pPlayer->IsContact)
            pPlayer->SetPlayerState(MID_RONAN_DASH_ATK_END_WAIT);
        else
            pPlayer->SetPlayerState(MID_RONAN_DASH_ATK_END_JUMP);
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerRonan::OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    // 마지막 프레임 이라면
    if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1)
    {
        if (true == pPlayer->IsContact)
            pPlayer->SetPlayerState(MID_RONAN_JUMP_DASH_ATK_END_WAIT);
        else
            pPlayer->SetPlayerState(MID_RONAN_JUMP_DASH_ATK_END_JUMP);
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCPlayerRonan::OnBackStepAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if (pPlayer->cFrame > 19)
        {
            if (pPlayer->IsContact)
                return KGCPlayerCommon::OnWait(pPlayer, bKeyProc);
            else
                return KGCPlayerCommon::OnJump(pPlayer, bKeyProc);
        }
    }

    return 0;
}

/*int KGCPlayerRonan::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

        if (g_MyD3D->MyCtrl->k_Fresh_Right_Dash)
        {
            pPlayer->bIsRight = true;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if ( g_kCamera.CheckDashZoom() )
                g_kCamera.SetZoom(0.75f, g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }
        else if (g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
        {
            pPlayer->bIsRight = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if (g_kCamera.CheckDashZoom())
                g_kCamera.SetZoom(0.75f, g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }

        /*if (g_MyD3D->MyCtrl->k_Fresh_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode)
        {
            if (pPlayer->GetBurnningPoint() >= 3 && pPlayer->CheckSkill(SID_RONAN1_RENEW_MAGIC_SWORD_MODE)) {
                pPlayer->SetPlayerState(MID_RONAN_CHANGE_MAGIC_SWORD);
                pPlayer->cFrame = 0;
                pPlayer->ChangeBurnningTemplate();
                g_MyD3D->MyCtrl->k_Up = false;
                g_MyD3D->MyCtrl->k_Fresh_Skill_Key = false;
            }
            return 0;
        }
        if (g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && pPlayer->m_bIsBurnningMode)
        {
            pPlayer->InitBurnningGauge();
            pPlayer->SetMagicEffect(EGC_EFFECT_RONAN_MAGICSWORD_MODE_UNEQUIP, 1);
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            g_MyD3D->MyCtrl->k_Skill_Key = false;

            return 0;
        }

        if (g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if (g_kCamera.CheckJumpZoom())
                g_kCamera.SetZoom(0.75f, g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            return 0;
        }

        if (g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;
            if (KGCPlayerCommon::CheckPortal(pPlayer, bKeyProc) == true)
                return 0;

            if (pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x, pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 40;
            return 0;
        }

        if (g_MyD3D->MyCtrl->k_Fresh_Punch)
        {
            if (pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 &&
                g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount)
            {
                if (pPlayer->Next_Attack_Count != 255)
                {
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
                }
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
            }
            //KeyProc뒤에는 현재 Frame처리 하는 부분이 있으므로 Frame = 0으로 해줍니다.
            //KeyProc이 아니면 Frame = -1로 해봅시다!

            pPlayer->NoCheckContact = 0;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;

            return 0;
        }
        
        /*if (g_MyD3D->MyCtrl->k_Fresh_Skill_Key && !pPlayer->m_bIsBurnningMode)
        {
            if (pPlayer->CheckSkill(SID_RONAN1_RENEW_MAGIC_ATK_FIX))
            {
                pPlayer->SetPlayerState(MID_RONAN_X_COMBO_1);
                pPlayer->NoCheckContact = 0;
                pPlayer->cFrame = 0;
            }
            g_MyD3D->MyCtrl->k_Fresh_Skill_Key = false;
            return 0;
        }

        pPlayer->x_Speed = 0.0f;

        if (g_MyD3D->MyCtrl->k_Right)
        {
            if (pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
                pPlayer->cFrame = 0;
            pPlayer->bIsRight = true;
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
            return 0;
        }
        else if (g_MyD3D->MyCtrl->k_Left)
        {
            if (pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
                pPlayer->cFrame = 0;
            pPlayer->bIsRight = false;
            pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
            return 0;
        }
        else        //Walk상태에서 Wait상태로 돌아오는...
        {
            if (pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK)
                pPlayer->cFrame = 0;

            if (pPlayer->GetHP() > 0.0f)
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk와 공용으로 사용하기 위해..넣어둔 코드임
            else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
            pPlayer->x_Speed = 0.0f;

        }

        if (g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BURNNING_MODE) //진
        {
            if (g_MyD3D->MyCtrl->Burnning_X_Count == 10 &&
                !pPlayer->m_bIsBurnningMode && !pPlayer->m_bIsMovedAfterCharge)
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().BURNNING_CHARGE);
                pPlayer->cFrame = 0;
            }
        }

        return 0;
    }

    pPlayer->m_bJumpDashed = false;
    if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame())      //무한 반복하는 프레임..
        pPlayer->cFrame = 0;

    return 0;
}*/


int KGCPlayerRonan::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        /*if (g_MyD3D->MyCtrl->k_Fresh_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode)
        {
            if (pPlayer->GetBurnningPoint() >= 3 && pPlayer->CheckSkill(SID_RONAN1_RENEW_MAGIC_SWORD_MODE)) {
                pPlayer->SetPlayerState(MID_RONAN_CHANGE_MAGIC_SWORD);
                pPlayer->cFrame = 0;
                pPlayer->ChangeBurnningTemplate();
                g_MyD3D->MyCtrl->k_Up = false;
                g_MyD3D->MyCtrl->k_Fresh_Skill_Key = false;
            }
            return 0;
        }*/
        if (g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && pPlayer->m_bIsBurnningMode)
        {
            pPlayer->InitBurnningGauge();
            pPlayer->SetMagicEffect(EGC_EFFECT_RONAN_MAGICSWORD_MODE_UNEQUIP, 1);
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            g_MyD3D->MyCtrl->k_Skill_Key = false;

            return 0;
        }
    }

    return KGCPlayerCommon::OnWait(pPlayer, bKeyProc);
}

int KGCPlayerRonan::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        OnWait(pPlayer, true);
        return 0;
    }

    pPlayer->SetWindSpeed();

    if (pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1)      //무한 반복하는 프레임..
        pPlayer->cFrame = 0;

    return 0;
}