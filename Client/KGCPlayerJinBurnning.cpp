#include "stdafx.h"
//





#include "KGCPlayerJinBurnning.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"

#include "Damage.h"

KGCPlayerJinBurnning::KGCPlayerJinBurnning()
{

}

KGCPlayerJinBurnning::~KGCPlayerJinBurnning()
{

}

int KGCPlayerJinBurnning::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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

		//if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN )
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

    pPlayer->SetWindSpeed();

	//무한 반복하는 프레임..
	if( pPlayer->cFrame == 24 )
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame < 24 )
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}

	//if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN )
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
				pPlayer->SetPlayerState( MID_JIN_RUN_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_RUN_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}

int KGCPlayerJinBurnning::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
			if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
				|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
			{
				pPlayer->SetPlayerState( MID_JIN_UNIQUE_SPINKICK );
			}
			else
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);				
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
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
			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

		return 0;
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
		pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
	{
		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_JIN_DASH_TO_WAIT );
			else
				pPlayer->SetPlayerState( MID_JIN_DASH_TO_JUMP );
			pPlayer->cFrame = 0;
		}
	}

	return 0;
}