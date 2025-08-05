#include "stdafx.h"
// KGCPlayerElesis3.cpp: implementation of the KGCPlayerElesis3 class.
//
//////////////////////////////////////////////////////////////////////
//




#include "KGCPlayerElesis3.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"

#include "KGCPlayerCommon.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerElesis3::KGCPlayerElesis3()
{

}

KGCPlayerElesis3::~KGCPlayerElesis3()
{

}

int KGCPlayerElesis3::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(MID_ELESIS3_DASHATK);
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

		if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
			( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 36)		//정지
		{
			pPlayer->cFrame = 37;
			return 0;
		}

		return 0;
	}

	if(pPlayer->cFrame == 35)		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame < 36)
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}
	else
	{
		//정지하는 프레임
		/*
		if( pPlayer->IsRight && g_MyD3D->MyCtrl->k_Right)
		pPlayer->x_Speed = 0.007f;
		else pPlayer->x_Speed = -0.007f;
		*/
	}

    pPlayer->SetWindSpeed();

	return 0;
}

int KGCPlayerElesis3::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
			if( g_MyD3D->MyCtrl->k_Up )				
			{
				g_MyD3D->MyCtrl->k_Fresh_Punch;
				g_MyD3D->MyCtrl->k_Up = false;
				pPlayer->SetPlayerState(MID_ELESIS3_JUMPATK_UP);
			}
			else if( g_MyD3D->MyCtrl->k_Down )
			{
				pPlayer->SetPlayerState(MID_ELESIS3_JUMPATK_DOWN);
				pPlayer->NoCheckContact = 25;
				g_MyD3D->MyCtrl->k_Down = false;
			}
			else
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
			
			pPlayer->cFrame = 0;
			return 0;
		}

		return KGCPlayerCommon::OnJump(pPlayer,true);		
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		if( pPlayer->cFrame > 25 )	//내려오는 점프 동작
			pPlayer->cFrame = 25;
	}
	else 
	{
		if( pPlayer->cFrame < 25 )
			pPlayer->cFrame = 25;
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}	
	
	return 0;
}

int KGCPlayerElesis3::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->Next_Attack_Count > 0 )
			{
				pPlayer->Next_Attack_Count = 255;
			}

			if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 )
			{
				if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
					g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
					pPlayer->cFrame = 0;
					return 0;
				}
			}
		}

		if( pPlayer->Next_Attack_Count != 0 )
		{		
			if( g_MyD3D->MyCtrl->k_Up)
			{								
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
				{
					g_MyD3D->MyCtrl->k_Up = false;
					pPlayer->SetPlayerState(MID_ELESIS3_SHADOW_ATK);
					pPlayer->cFrame = 0;
					return 0;				
				}
			}

			if(pPlayer->Next_Attack_Count == 0)
				return 0;

			if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
			{
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
				{
					if( pPlayer->uiMotion == MID_ELESIS3_ATK1_2 )	//Critical
					{						
						pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
					}
					else //Double
					{						
						pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
					}

					if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
						pPlayer->bIsRight = true;
					else pPlayer->bIsRight = false;

					pPlayer->Next_Attack_Count=0;
					//pPlayer->Ah_Sound(5);					
					pPlayer->cFrame = 0;

					return 0;
				}
			}
		}		
		return 0;
	}	
	return 0;
}
