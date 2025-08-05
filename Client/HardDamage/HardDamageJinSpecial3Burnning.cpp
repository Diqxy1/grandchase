#include "stdafx.h"
#include ".\HardDamageJinSpecial3Burnning.h"



#include "DamageManager.h"
#include "Monster.h"

CHardDamageJinSpecial3Burnning::CHardDamageJinSpecial3Burnning(void)
{
}

CHardDamageJinSpecial3Burnning::~CHardDamageJinSpecial3Burnning(void)
{
}


void CHardDamageJinSpecial3Burnning::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	if( pDamageInstance->m_What == DT_JIN_SPECIAL3_BURNNING_BALL_EFFECT || pDamageInstance->m_What == DT_JIN_SPECIAL3_BURNNING_BALL_EFFECT_DRAMA )
	{
		if( pDamageInstance->m_Life == 25 )
		{
			pDamageInstance->m_Life = 26;
		}

		if(pDamageInstance->m_IsMonsterDamage)
		{
			MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
			if( pMonster == NULL ) return;

			if(pMonster->GetState() !="SPECIAL3_1" &&
				pMonster->GetState() !="SPECIAL3_2" &&
				pMonster->GetState() !="SPECIAL3_3")		
			{
				pDamageInstance->m_Life = 0;
			}

			if( pMonster->GetState() =="SPECIAL3_3" )
			{
				if( pMonster->m_aiFrame[0]  == 3 )
				{

					g_MyD3D->m_pDamageManager->MonsterAddWithLocate( DT_JIN_SPECIAL3_BURNNING_BALL_FIRE_DRAMA , pDamageInstance->m_ExtraMonsterNum,  pDamageInstance->m_X, pDamageInstance->m_Y);
				}

				if( pMonster->m_aiFrame[0]  == 8 )
				{
					pDamageInstance->m_Life = 1;			
				}
			}
		}
		else
		{
			if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_JIN_SPECIAL3_FORCED && 
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_JIN_SPECIAL3_FORCED2 &&
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_JIN_SPECIAL3_FORCED3 )
			{
				pDamageInstance->m_Life = 0;
			}

			if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_JIN_SPECIAL3_FORCED3 )
			{
				if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 3 ) 
				{
					g_MyD3D->m_pDamageManager->AddWithLocate( DT_JIN_SPECIAL3_BURNNING_BALL_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
				}

				if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 8 ) //발사때 프레임 수 적어주자!!
				{
					pDamageInstance->m_Life = 1;			
				}
			}
		}
	}
	else if( pDamageInstance->m_What == DT_JIN1_SPECIAL3_FINAL_EFFECT ) 
	{
		if( pDamageInstance->m_Life == 25 )
		{
			pDamageInstance->m_Life = 26;
		}	

		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_JIN1_SPECIAL3 &&
			g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_JIN1_SPECIAL3_READY )
		{
			pDamageInstance->m_Life = 0;
		}

		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_JIN1_SPECIAL3 )
		{
			if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 100 )
			{
				pDamageInstance->m_Life = 1;
			}
		}
	}
}
