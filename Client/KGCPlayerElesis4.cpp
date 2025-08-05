#include "stdafx.h"

#include "buff/KGCBuffManager.h"
#include "KGCPlayerElesis4.h"
#include "KGCPlayerCommon.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"


KGCPlayerElesis4::KGCPlayerElesis4()
{

}

KGCPlayerElesis4::~KGCPlayerElesis4()
{

}

int KGCPlayerElesis4::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(MID_ELESIS4_DASH_ATK);
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

        if( ( ( pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Right ) ||
            ( !pPlayer->bIsRight && !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 31)//정지
        {
            pPlayer->cFrame = 31;
            return 0;
        }

        return 0;
    }

    if(pPlayer->cFrame == 30) //무한 반복하는 프레임..
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 30 )
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }    

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerElesis4::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            int iJumpAtkMotion = pPlayer->GetCurFormTemplate().JUMP_ATK;

            if( g_MyD3D->MyCtrl->k_Up )
            {
                g_MyD3D->MyCtrl->k_Up = false;
                iJumpAtkMotion = MID_ELESIS4_JUMP_ATK5;
            }
            else if( g_MyD3D->MyCtrl->k_Down )
            {
                g_MyD3D->MyCtrl->k_Down = false;
                iJumpAtkMotion = MID_ELESIS4_JUMP_ATK3;
            }

            pPlayer->SetPlayerState(iJumpAtkMotion);
            if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
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
        if( pPlayer->cFrame > 25 )   //내려오는 점프 동작
            pPlayer->cFrame = 25;
    }
    else 
    {
        if( pPlayer->cFrame < 25 )
            pPlayer->cFrame = 25;
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )       //무한 반복하는 프레임..
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerElesis4::OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState( MID_ELESIS4_JUMP_ATK2 );
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }

    if( pPlayer->cFrame >= 12 )        //무한 반복하는 프레임..
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerElesis4::OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
        return 0;

    if( pPlayer->cFrame >= 14 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerElesis4::OnJumpAtk5(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame <= 20 && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->Next_Attack_Count = 255;
        }
        return 0;
    }

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )       //무한 반복하는 프레임..
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerElesis4::OnCombo1(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
		if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
			g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
			pPlayer->cFrame = 0;
			return 0;
		}

        if( pPlayer->cFrame <= 22 && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = 255;
        }
        return 0;
    }
    return 0;
}

int KGCPlayerElesis4::OnCombo(PLAYER* pPlayer, bool bKeyProc)
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

            if( pPlayer->uiMotion != MID_ELESIS4_ATK5 )
            {
                pPlayer->SetPlayerState( MID_ELESIS4_CRITICAL_ATK );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                return 0;
            }
            else
            {
                pPlayer->SetPlayerState( MID_ELESIS4_DOUBLE_ATK );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                return 0;
            }
        }

        return 0;
    }
    return 0;
}

int KGCPlayerElesis4::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Right_Sync )
        {
            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
            pPlayer->cFrame = 0;
            return 0;
        }
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }
    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
}

int KGCPlayerElesis4::OnBackStep(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(MID_ELESIS4_BACK_STEP_ATK);
            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }
    return 0;
}

int KGCPlayerElesis4::OnSpecial2(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->cFrame >= 40 && pPlayer->cFrame < ELESIS4_SPECIAL2_SHOOT_FRAME )
        {
            pPlayer->cFrame = ELESIS4_SPECIAL2_SHOOT_FRAME;
            return 0;
        }
        return 0;
    }
    return 0;
}

//int KGCPlayerElesis4::OnCriticalDoubleAtk(PLAYER* pPlayer, bool bKeyProc)
//{
//    if( bKeyProc )
//    {
//        if( g_MyD3D->MyCtrl->k_Fresh_Back_dash || 
//            (pPlayer->IsRight && g_MyD3D->MyCtrl->k_Fresh_Right_Left) ||
//            (!pPlayer->IsRight && g_MyD3D->MyCtrl->k_Fresh_Left_Right) )
//        {
//            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
//            pPlayer->Frame = 0;
//            return 0;
//        }
//        return 0;
//    }
//    return 0;
//}

int KGCPlayerElesis4::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Left_Right_Sync )
        {
            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerElesis4::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->bIsRight && 
            ( !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Left ||
            g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Right ) )
        {
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) )
                g_MyD3D->MyCtrl->k_Fresh_Right = false;
            else
                g_MyD3D->MyCtrl->k_Fresh_Left = false;

            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
            pPlayer->cFrame = 0;
            return 0;
        }
        else if( !pPlayer->bIsRight && 
            ( !g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Right ||
            g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) && g_MyD3D->MyCtrl->k_Fresh_Left ) )
        {
            if( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION ) )
                g_MyD3D->MyCtrl->k_Fresh_Left = false;
            else
                g_MyD3D->MyCtrl->k_Fresh_Right = false;

            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
            pPlayer->cFrame = 0;
            return 0;
        }
        else if( g_MyD3D->MyCtrl->k_Fresh_Left_Right_Sync )
        {
            g_MyD3D->MyCtrl->k_Fresh_Left_Right_Sync = false;
            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
            pPlayer->cFrame = 0;
            return 0;
        }
    }

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

//int KGCPlayerElesis4::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
//{
//    if( bKeyProc )
//    {
//        if( pPlayer->cFrame < 13 && 
//            ( g_MyD3D->MyCtrl->k_Fresh_Back_dash || 
//              (pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Fresh_Right_Left) ||
//              (!pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Fresh_Left_Right)
//            ) )
//        {
//            pPlayer->SetPlayerState( MID_ELESIS4_BACK_STEP );
//            pPlayer->cFrame = 0;
//            return 0;
//        }
//    }
//
//    return 0;
//}
