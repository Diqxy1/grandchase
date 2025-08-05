#include "stdafx.h"
// KGCPlayerSieg1.cpp: implementation of the KGCPlayerArme2 class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerCommon.h"
#include "KGCPlayerSieg.h"
#include "DamageManager.h"
#include "Controls.h"

//

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerSieg1::KGCPlayerSieg1()
{

}

KGCPlayerSieg1::~KGCPlayerSieg1()
{

}

int KGCPlayerSieg1::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
            return 0;
        }

        if( pPlayer->Next_Attack_Count == 0 )
            return 0;

        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else pPlayer->bIsRight = false;

            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 &&
                pPlayer->uiMotion < pPlayer->GetCurFormTemplate().ATKEND )
            {
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                return 0;
            }           

            if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATKEND )
            {
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_ATK );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                return 0;
            }
        }

        //if( g_MyD3D->MyCtrl->k_Up )
        //{
        //    g_MyD3D->MyCtrl->k_Up = false;

        //    if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 &&
        //        pPlayer->uiMotion < pPlayer->GetCurFormTemplate().ATKEND )
        //    {
        //        pPlayer->SetPlayerState( MID_SIEG1_AERIAL );
        //        pPlayer->cFrame = 0;
        //        pPlayer->Next_Attack_Count = 0;
        //        return 0;
        //    }           
        //}
    }
    return 0;
}

int KGCPlayerSieg1::OnJumpAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {    
        if( ISPUSHED(k_Fresh_Punch) )
            pPlayer->Next_Attack_Count = 255;
    }
   
    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //무한 반복하는 프레임..
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerSieg1::OnDash(PLAYER* pPlayer, bool bKeyProc)
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

            //if( pPlayer->IsContact )
            {
                //pPlayer->y_Speed -= 0.015f;
				pPlayer->y_Speed = -0.01f;
            }
            pPlayer->NoCheckContact = 35;
            return 0;
        }        
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
        pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
    {
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_SIEG1_DASH_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_SIEG1_DASH_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}


int KGCPlayerSieg1::OnRun( PLAYER* pPlayer, bool bKeyProc )
{
    KGCPlayerCommon::OnRun( pPlayer, bKeyProc );

    if( bKeyProc )
    {
        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH_ATK )
            pPlayer->x_Speed /= 2.0f;

    }
    return 0;
}

int KGCPlayerSieg1::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
    KGCPlayerCommon::OnWait( pPlayer, bKeyProc );

    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Select_Item )
        {
            if( pPlayer->m_fBurnningPoint > 2.0f &&
                !pPlayer->m_bIsBurnningMode )
            {
                pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().BURNNING_ACTIVE );
                pPlayer->ChangeBurnningTemplate();
                
                pPlayer->cFrame = 0;
            }
        }
    }

    return 0;
}

int KGCPlayerSieg1::OnSpecial3( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame < 29 )
        {
            if( g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight )
            {
                pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed * 2.8f;
            }
            else if( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight )
            {
                pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed * 2.8f;
            }
        }
    }
    return 0;

}

int KGCPlayerSieg2::OnCombo( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Up )
        {
            g_MyD3D->MyCtrl->k_Up = false;
        }
    }

    return KGCPlayerSieg1::OnCombo( pPlayer, bKeyProc );

}

int KGCPlayerSieg2::OnDash( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
        pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
    {
        if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
        {
            if( true == pPlayer->IsContact )
                pPlayer->SetPlayerState( MID_SIEG2_DASH_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_SIEG2_DASH_TO_JUMP );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return KGCPlayerSieg1::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerSieg2::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Down && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_SIEG2_JUMP_ATK2 );
            pPlayer->cFrame = 0;

            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            g_MyD3D->MyCtrl->k_Down = false;
        }        
    }

    return KGCPlayerCommon::OnJump( pPlayer, bKeyProc );
}

int KGCPlayerSieg2::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Up) )
        {
            pPlayer->Next_Attack_Count = 255;
            return 0;
        }

        if( ISPUSHED(k_Down) && pPlayer->IsContact)
        {
            pPlayer->SetPlayerState( MID_SIEG2_DASH_POLE_JUMP );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return 0;
}

int KGCPlayerSieg2::OnPole( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {    
        if( ISPUSHED(k_Fresh_Punch) )
            pPlayer->Next_Attack_Count = 255;
    }
    return 0;
}

int KGCPlayerSieg2::OnJumpDownAtk( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame >= 24 && !pPlayer->IsContact )
    {
        pPlayer->m_bFrameLock = true;
        pPlayer->y_Speed = -0.12f;
    }

    if( pPlayer->IsContact )
        pPlayer->m_bFrameLock = false;

    return 0;
}


    