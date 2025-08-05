#include "stdafx.h"
// KGCPlayerArme4.cpp: implementation of the KGCPlayerArme4 class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerCommon.h"
#include "KGCPlayerArme4.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Headup Display.h"

#include "KGCRoomManager.h"

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerArme4::KGCPlayerArme4()
{

}

KGCPlayerArme4::~KGCPlayerArme4()
{

}

int KGCPlayerArme4::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash))
        {
            pPlayer->bIsRight = ISPUSHED(k_Fresh_Right_Dash);
            pPlayer->cFrame = 0;
            pPlayer->SetPlayerState(MID_ARME4_CRITICAL_ATK);

            return 0;
        }
    

        if( ISPUSHED(k_Fresh_Punch) )
        {
            if( pPlayer->uiMotion != MID_ARME4_ATK3 )
            {
                if( pPlayer->Next_Attack_Count > 0 )
                {
                    pPlayer->Next_Attack_Count = 255;
                }
            }
        }
        return 0;
    }	
    return 0;
}


int KGCPlayerArme4::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {		
		if( g_MyD3D->MyCtrl->k_Fresh_Double_X 
			&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_CHANGE_HP_TO_MP )
			&& pPlayer->GetHP() > 0 && g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
		{	
			pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_HP_TO_MP);	
			pPlayer->cFrame = 0;				
			return 0;
		}
        return KGCPlayerCommon::OnWait(pPlayer, true);
    }

    if(g_MyD3D->MyHead->Level >= g_kGlobalValue.GetMaxMP() )		//엠피가 가득 찼을경우
    {
        if(pPlayer->cFrame > 69 && pPlayer->cFrame < 80 )
            pPlayer->cFrame = 0;
    }
    else
    {
        if( pPlayer->cFrame > 105)						//엠피가 덜 찼을 경우
        {
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
            {
                g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;
                pPlayer->cFrame = 95;
            }
        }
    }
    return 0;
}

int KGCPlayerArme4::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        KGCPlayerArme4::OnWait(pPlayer,true);

        pPlayer->SetWindSpeed();
        return 0;
    }

    pPlayer->SetWindSpeed();

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
        pPlayer->cFrame = 0;

    return 0;
}

int KGCPlayerArme4::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
		if( pPlayer->Extra_Char_Num == CID_ARME4 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_TELEPORT_VERTICAL ))
		{
			if( g_MyD3D->MyCtrl->k_Up && pPlayer->cFrame <= 10 )
			{
				pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_TELEPORT_UP);
				pPlayer->cFrame = 0;
				return 0;
			}
			else if( g_MyD3D->MyCtrl->k_Fresh_Down_Dash )
			{
				pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

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

            if( g_MyD3D->MyCtrl->k_Down )
            {
                iJumpAtkMotion = MID_ARME4_JUMP_ATK2;
            }
            else
            {
                iJumpAtkMotion = MID_ARME4_JUMP_ATK1;
            }

            pPlayer->SetPlayerState(iJumpAtkMotion);
            if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
            pPlayer->cFrame = 0;
        }

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->m_iJumpPossibleFrame > 0 )
		{
			pPlayer->Jump();
			g_MyD3D->MyCtrl->k_Up = false;
			pPlayer->m_iJumpPossibleFrame = 0 ;
			return 0;
		}
    }

    pPlayer->SetWindSpeed();

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //무한 반복하는 프레임..
        pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() -2;

    return 0;

}

int KGCPlayerArme4::OnJumpAtk2( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame >= 28 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCPlayerArme4::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if ( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
                || (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
            {
                pPlayer->SetPlayerState( MID_ARME4_DASHATK2 );
                g_MyD3D->MyCtrl->k_Fresh_Punch = false;
                pPlayer->cFrame = 0;
                return 0;

            }
        }

		if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_TELEPORT_BACK_MOVE ))
		{
			if( pPlayer->GetIsRight() )
			{   
				if( g_MyD3D->MyCtrl->k_Left)
				{
					pPlayer->SetPlayerState( MID_SKILL_COMMON_ARME_TELEPORT_BACK );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Left = false;
					return 0;
				}           
			}
			else
			{
				if( g_MyD3D->MyCtrl->k_Right)
				{
					pPlayer->SetPlayerState( MID_SKILL_COMMON_ARME_TELEPORT_BACK );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Right = false;
					return 0;
				}           
			}
		}		
    }
    return KGCPlayerCommon::OnDash(pPlayer,bKeyProc);
}

int KGCPlayerArme4::OnBackTeleport(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Up )
		{   
			if( pPlayer->IsContact )
			{           
				if(!pPlayer->bIsRight)
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
	}
	return 0;
}

//int KGCPlayerArme4::OnChangeHpToMp(PLAYER* pPlayer, bool bKeyProc)
//{
//	if(pPlayer->cFrame == 20)	
//	{		
//		float fChangeHpToMp = pPlayer->m_fAbility[ABILITY_HP] * 0.1f;
//		pPlayer->m_fHP -= fChangeHpToMp;
//		pPlayer->DecreaseMP( 0.3 );
//	}
//	else if( pPlayer->cFrame == 35 )
//	{
//		g_MyD3D->MyHead->Add_HP_Changer(pPlayer->m_iPlayerIndex, 3, true, "-MP");
//	}
//	return 0;
//}