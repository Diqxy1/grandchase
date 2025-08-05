#include "stdafx.h"
// KGCPlayerArme.cpp: implementation of the KGCPlayerArme class.
//
//////////////////////////////////////////////////////////////////////
//


#include "KDInput.h"


#include "KGCPlayerCommon.h"
#include "KGCPlayerArme.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Headup Display.h"
#include "KGCRoomManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerArme::KGCPlayerArme()
{

}

KGCPlayerArme::~KGCPlayerArme()
{

}

int KGCPlayerArme::OnSpecial2Black(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( pPlayer->cFrame > 49 && pPlayer->cFrame < 179)
		{
			if( diks[g_MyD3D->MyCtrl->m_keyUp] )
			{
				pPlayer->Body_Angle += 0.01f;
				if(pPlayer->Body_Angle > 0.8f) 
					pPlayer->Body_Angle=0.8f;
			}
			else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->Body_Angle -= 0.01f;
				if( pPlayer->Body_Angle < -0.8f ) 
					pPlayer->Body_Angle = -0.8f;
			}
		}

		return 0;
	}
	return 0;
}

int KGCPlayerArme::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->Next_Attack_Count > 0 
				|| pPlayer->uiMotion == MID_SKILL_ARME1_NEWCOMBO_ATK3 
				|| pPlayer->uiMotion == MID_SKILL_ARME1_NEWCOMBO_ATK4)				
			{
				pPlayer->Next_Attack_Count = 255;
			}
			//return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down && pPlayer->GetCurFormTemplate().ATK1 &&
			pPlayer->uiMotion != MID_SKILL_ARME1_NEWCOMBO_ATK2 &&
			pPlayer->uiMotion != MID_SKILL_ARME1_NEWCOMBO_ATK3 &&
			pPlayer->uiMotion != MID_SKILL_ARME1_NEWCOMBO_ATK4 &&
			pPlayer->uiMotion != MID_ARME_ATK1_2 &&
			pPlayer->uiMotion != MID_ARME_ATK1_3 &&
			pPlayer->uiMotion != MID_ARME_ATK1_4 )
		{	
			if( pPlayer->cFrame <= 15 )
			{				
				if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_WIZARD_DOLL ) )
				{
					pPlayer->SetPlayerState(MID_SKILL_ARME1_WIZARD_DOLL);				
					pPlayer->cFrame = 0;				
					return 0;
				}				
			}
		}

		if( (g_MyD3D->MyCtrl->k_Fresh_Right || g_MyD3D->MyCtrl->k_Fresh_Left) && pPlayer->m_bIsChargedShield && pPlayer->Shield > 0 )
		{	
			if( pPlayer->cFrame <= 15 )
			{
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

				if( pPlayer->m_bIsCountAttackShield )
				{
#ifndef MATCH_BALLENCE_121217			
					if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_WHITE1_PROTECTIONFIRE_LV2 ) )
					{
						pPlayer->SetPlayerState(MID_SKILL_ARME1_W1_PROTECTION_LV2_FIRE);				
						pPlayer->cFrame = 0;				
						return 0;
					}
#endif					
				}				
				else
				{	
					if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_WHITE1_BASE_MGSHIELD_LV2 ) )
					{
						pPlayer->SetPlayerState( MID_SKILL_ARME1_MAGICSHIELD_FIRE );				
						pPlayer->cFrame = 0;
						return 0;
					}
				}					
			}									
		}

		if(pPlayer->Next_Attack_Count == 0)
			return 0;

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
			{

				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )	//Critical
				{
					g_MyD3D->m_pDamageManager->Add( DT_CRITICAL, pPlayer->m_iPlayerIndex );
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
					pPlayer->cFrame = 0;
				}
				else //Double
				{
					g_MyD3D->m_pDamageManager->Add( DT_DOUBLE, pPlayer->m_iPlayerIndex );
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
					pPlayer->cFrame = 0;
				}
				if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
					pPlayer->bIsRight = true;
				else pPlayer->bIsRight = false;

				pPlayer->Next_Attack_Count=0;

				return 0;
			}
		}

		if( pPlayer->Next_Attack_Count != 0 )
		{		
			if( g_MyD3D->MyCtrl->k_Up )
			{								
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
				{
					pPlayer->Next_Attack_Count=0;
					g_MyD3D->MyCtrl->k_Up = false;
					pPlayer->SetPlayerState(MID_ARME_EARTHQUAKE);
					pPlayer->cFrame = 0;
					return 0;
				}
			}
		}		
		return 0;
	}	
	return 0;
}

int KGCPlayerArme::OnWait(PLAYER* pPlayer, bool bKeyProc)
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
#ifdef MATCH_BALLENCE_121217
		if( pPlayer->cFrame == 150)
			pPlayer->cFrame = 50;
#else
		if(pPlayer->cFrame > 49 && pPlayer->cFrame < 59)
			pPlayer->cFrame = 49;			
#endif						
	}
	else //엠피가 덜 찼을 경우
	{
#ifdef MATCH_BALLENCE_121217
        if( pPlayer->cFrame > 59 && pPlayer->cFrame <= 150 ) {
            pPlayer->cFrame = 151;
        } else {
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() ) {
                if( pPlayer->cFrame >= 183 ) {
                    g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;
                    pPlayer->cFrame = 173;
                }
            }            
		}
#else
		if(pPlayer->cFrame > 89)						//엠피가 덜 찼을 경우
		{
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
            {
                // 판타스틱 맵이라는건 몬스터가 나온다는 말이고 몬스터가 나올 때는 MP 적게 차게 해 준다
                g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;
                pPlayer->cFrame = 79;
            }
		}
#endif		
	}
	return 0;
}

int KGCPlayerArme::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		KGCPlayerArme::OnWait(pPlayer,true);
        pPlayer->SetWindSpeed();
		return 0;
	}

    pPlayer->SetWindSpeed();

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerArme::OnBlackMagicFear(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( 0 <= pPlayer->cFrame && pPlayer->cFrame <= 29 )
		{
			if(bKeyProc)
			{
				if( g_MyD3D->MyCtrl->k_Left)
				{
					if( pPlayer->Next_Attack_Count > 0 
						|| pPlayer->uiMotion == MID_SKILL_ARME1_B1_FEAR_LV1 
						|| pPlayer->uiMotion == MID_SKILL_ARME1_B1_FEAR_LV2 )
					{                    
						pPlayer->m_bIsPushLeftKey = true;
						pPlayer->m_bIsPushRightKey = false;
					}                
				}

				if( g_MyD3D->MyCtrl->k_Right )
				{
					if( pPlayer->Next_Attack_Count > 0 
						|| pPlayer->uiMotion == MID_SKILL_ARME1_B1_FEAR_LV1
						|| pPlayer->uiMotion == MID_SKILL_ARME1_B1_FEAR_LV2 )
					{
						pPlayer->m_bIsPushRightKey = true;
						pPlayer->m_bIsPushLeftKey = false;
					}
					return 0;
				}
			}
		}
	}
	
	return 0;
}

int KGCPlayerArme::OnSpecialW2IceTrap(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			//if( pPlayer->cFrame >= 15 && pPlayer->cFrame <= 21 ) 
				pPlayer->m_bIsPushLeftKey = true;
		}
	}
	return 0;
}


int KGCPlayerArme::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_TELEPORT_BACK_MOVE ) )
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

    if( pPlayer->Extra_Char_Num == CID_ARME4 ) {
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
    }

    return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}

int KGCPlayerArme::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_TELEPORT_VERTICAL ))
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
    }
    return KGCPlayerCommon::OnJump( pPlayer, bKeyProc);

}