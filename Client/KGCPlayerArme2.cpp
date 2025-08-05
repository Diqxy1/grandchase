#include "stdafx.h"
// KGCPlayerArme2.cpp: implementation of the KGCPlayerArme2 class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerCommon.h"
#include "KGCPlayerArme2.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Headup Display.h"
#include "KGCRoomManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerArme2::KGCPlayerArme2()
{

}

KGCPlayerArme2::~KGCPlayerArme2()
{

}

int KGCPlayerArme2::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
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
		if(pPlayer->Next_Attack_Count == 0)
			return 0;		

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
			{
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )	//Critical
				{
					g_MyD3D->m_pDamageManager->Add( DT_CRITICAL, pPlayer->m_iPlayerIndex );
				}
				else //Double
				{
					g_MyD3D->m_pDamageManager->Add( DT_DOUBLE, pPlayer->m_iPlayerIndex );
				}
				if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
					pPlayer->bIsRight = true;
				else pPlayer->bIsRight = false;

				pPlayer->Next_Attack_Count=0;

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
				pPlayer->cFrame = 0;

				return 0;
			}
		}
		return 0;
	}	
	return 0;
}

int KGCPlayerArme2::OnWait(PLAYER* pPlayer, bool bKeyProc)
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
#ifdef MATCH_BALLENCE_121217
    if(g_MyD3D->MyHead->Level >= g_kGlobalValue.GetMaxMP() )		//¿¥ÇÇ°¡ °¡µæ Ã¡À»°æ¿ì
    {
        if( pPlayer->cFrame == 201 )
            pPlayer->cFrame = 130;			
    }
    else //¿¥ÇÇ°¡ ´ú Ã¡À» °æ¿ì
    {

        if( pPlayer->cFrame > 59 && pPlayer->cFrame <= 201 ) {
            pPlayer->cFrame = 202;
        } else {
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() ) {
                if( pPlayer->cFrame >= 219 ) {
                    g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;
                    pPlayer->cFrame = 209;
                }
            }            
        }
    }	
#else
    if(g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
	{
        if(pPlayer->cFrame > 149)
        {
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
            {
                g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;               

                pPlayer->cFrame=139;
            }
        }
		else if( pPlayer->cFrame > 59 && pPlayer->cFrame < 69 ) 
			pPlayer->cFrame=129;
    }
	else if( pPlayer->cFrame > 128 ) 
		pPlayer->cFrame=128;
#endif
	return 0;
}

int KGCPlayerArme2::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		KGCPlayerArme2::OnWait(pPlayer,true);
        pPlayer->SetWindSpeed();
		return 0;
	}

    pPlayer->SetWindSpeed();

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//¹«ÇÑ ¹Ýº¹ÇÏ´Â ÇÁ·¹ÀÓ..
		pPlayer->cFrame = 0;

	return 0;
}

//int KGCPlayerArme2::OnChangeHpToMp(PLAYER* pPlayer, bool bKeyProc)
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