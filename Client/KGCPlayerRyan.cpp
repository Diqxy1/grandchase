#include "stdafx.h"
// KGCPlayerRyan.cpp: implementation of the KGCPlayerArme2 class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerCommon.h"
#include "KGCPlayerRyan.h"
#include "DamageManager.h"
#include "Controls.h"

//

#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)
float KGCPlayerRyan::m_fChargeElapsedTime = 0.0f;
DWORD KGCPlayerRyan::m_dwSuperTime = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerRyan::KGCPlayerRyan()
{
	m_fChargeElapsedTime = 0.0f;
}

KGCPlayerRyan::~KGCPlayerRyan()
{

}

int KGCPlayerRyan::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
	if( bKeyProc )
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
			if( ISPUSHED(k_Down) )
			{
				pPlayer->SetPlayerState( MID_RYAN_JUMP_ATK2 );
				pPlayer->cFrame = 0;
				pPlayer->NoCheckContact = 25;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				ISPUSHED(k_Down) = false;
				return 0;
			}
			else
			{
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

		if( pPlayer->GetCurFormTemplate().JUMP_DASH != -1 )
		{
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
		if( pPlayer->cFrame > 21 )	//내려오는 점프 동작
			pPlayer->cFrame = 21;
	}
	else 
	{
		if( pPlayer->cFrame < 21 )
			pPlayer->cFrame = 21;
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}

	return 0;
}

int KGCPlayerRyan::OnCombo1(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
        if ( g_MyD3D->MyCtrl->k_Skill_Key && pPlayer->IsContact ) {
            if ( g_MyD3D->MyCtrl->k_Down
                && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_WILD_HOWLING ) 
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_WILD_HOWLING ) )
            {
                pPlayer->SetPlayerState( MID_SKILL_RYAN_WILD_HOWLING );
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Select_Item = false;
                g_MyD3D->MyCtrl->k_Down = false;

                return 0;
            }
        }

		if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
		{		
			if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
			{
				pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
				pPlayer->cFrame = 0;

				if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATKEND )
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DOUBLE_ATK );
					g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
				}
				else 
				{
					pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
					g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
				}

				return 0;
			}

			if( g_MyD3D->MyCtrl->k_Up )
			{
				if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_COMBO_UP ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_COMBO_UP_1 );
					pPlayer->cFrame = 0;
				}

				g_MyD3D->MyCtrl->k_Up = false;
				return 0;
			}

			
			if ( g_MyD3D->MyCtrl->k_Down )
			{
				if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_COMBO_DOWN ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_COMBO_DOWN );
					pPlayer->cFrame = 0;
				}

				g_MyD3D->MyCtrl->k_Down = false;
				return 0;
			}
		}
		else
		{
			if( g_MyD3D->MyCtrl->k_Up )
			{	
				if( pPlayer->cFrame <= 8 )
				{
					int uiMotion = -1;
					if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_HP_TOTEM ) &&
						false == pPlayer->IsMagicEffect( EGC_EFFECT_HP_TOTEM_COOL ) )
					{
                        uiMotion = MID_SKILL_RYAN_HP_TOTEM;
					}
					else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_MP_TOTEM ) &&
						false == pPlayer->IsMagicEffect( EGC_EFFECT_MP_TOTEM_COOL ) )
					{
                        uiMotion = MID_SKILL_RYAN_MP_TOTEM;
					}
					else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DP_TOTEM ) &&
						false == pPlayer->IsMagicEffect( EGC_EFFECT_DP_TOTEM_COOL ) )
					{
                        uiMotion = MID_SKILL_RYAN_DP_TOTEM;
					}
					else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_AP_TOTEM ) &&
						false == pPlayer->IsMagicEffect( EGC_EFFECT_AP_TOTEM_COOL ) )
					{
						uiMotion = MID_SKILL_RYAN_AP_TOTEM;
					}

					if ( uiMotion != -1 )
					{
						pPlayer->SetPlayerState( uiMotion );
						pPlayer->cFrame = 0;
						return 0;
					}
				}

				g_MyD3D->MyCtrl->k_Up = false;
			}

			if ( g_MyD3D->MyCtrl->k_Down )
			{
				if( pPlayer->cFrame <= 8 )
				{
					if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_LEOPARD_IMAGE ) &&
						false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_MOVE_SPEEDUP ) )
					{
						pPlayer->SetPlayerState( MID_SKILL_RYAN_LEOPARD_IMAGE );
						pPlayer->cFrame = 0;

						return 0;
					}
				}

				g_MyD3D->MyCtrl->k_Down = false;
			}			
		}

		if(g_MyD3D->MyCtrl->k_Fresh_Punch)
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )
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

int KGCPlayerRyan::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
	static bool bIsPushBack = false;
	static bool bIsJumpAtk = false;

	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
			pPlayer->Next_Attack_Count = 255;
		if ( pPlayer->uiMotion == MID_RYAN_DASH_ATK1 && pPlayer->cFrame >= 17 )
		{
			bIsPushBack = true;
		}

		if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DASH_ATK_LV3 ) && 
			 pPlayer->uiMotion == MID_RYAN_DASH_ATK1 && g_MyD3D->MyCtrl->k_Fresh_Up )
		{
			bIsJumpAtk = true;
		}

		if ( bIsPushBack && ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
			|| (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) ) )
		{
			pPlayer->SetPlayerState( MID_SKILL_RYAN_DASHATK_CANCEL );
			pPlayer->cFrame = 0;
			bIsPushBack = false;

			return 0;
		}


		if ( bIsJumpAtk && pPlayer->cFrame >= 25 )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP_ATK );
			if ( pPlayer->IsContact )
				pPlayer->y_Speed = 0.022f;
			else
				pPlayer->y_Speed = 0.00f;

			pPlayer->x_Speed = 0.015f;
			if ( !pPlayer->bIsRight )
				pPlayer->x_Speed *= -1.0f;

			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Up = false;
			bIsJumpAtk = false;

			return 0;
		}
		if( ( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
		{
			pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;
			pPlayer->cFrame = 0;

			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().CRITICAL_ATK );
			g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);

			return 0;
		}
	}



	return 0;
}

int KGCPlayerRyan::OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= 31 )
		pPlayer->m_bFrameLock = true;

	if ( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DOWNWARD_CHOP_LV2 ) )
		{
			if( ISPUSHED(k_Down) )
			{
				pPlayer->SetPlayerState( MID_RYAN_JUMP_ATK2 );
				pPlayer->cFrame = 0;
				pPlayer->NoCheckContact = 25;
				ISPUSHED(k_Down) = false;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;

				return 0;
			}
		}

		if ( ( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) &&
			 g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_JUMP_ATTACK_LV2 ) )
		{
			pPlayer->bIsRight = g_MyD3D->MyCtrl->k_Fresh_Right_Dash;

			pPlayer->SetPlayerState(MID_SKILL_RYAN_JUMP_DASH);
			g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
			g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
			pPlayer->cFrame = 0;

			return 0;
		}
	}

	return 0;
}

int KGCPlayerRyan::OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= 26)
		pPlayer->m_bFrameLock = true;

	static bool bIsPushAtk = false;

	if ( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_DOWNWARD_CHOP_LV3 ) )
		{
			bIsPushAtk = true;
		}
	}

	if ( pPlayer->cFrame >= 25 && bIsPushAtk )
	{
		pPlayer->SetPlayerState( MID_SKILL_RYAN_DOWNWARD_CHOP_LV3 );
		pPlayer->cFrame = 0;
		bIsPushAtk = false;

		return 0;
	}
	

	return 0;
}

int KGCPlayerRyan::OnJumpAtk2Land(PLAYER* pPlayer, bool bKeyProc)
{
	if( pPlayer->cFrame >= 25 )
	{
		pPlayer->SetPlayerState( MID_RYAN_WAIT );
		pPlayer->cFrame = 0;
	}

	return 0;
}

int KGCPlayerRyan::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			pPlayer->cFrame = 0;

			return 0;
		}
		if( g_MyD3D->MyCtrl->k_Down )
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
	}

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

	if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
	{
		if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
		{
			if( true == pPlayer->IsContact )
				pPlayer->SetPlayerState( MID_RYAN_WAIT );
			else
				pPlayer->SetPlayerState( MID_RYAN_JUMP );
			pPlayer->cFrame = 0;
			return 0;
		}
		else
		{
			pPlayer->SetPlayerState( MID_RYAN_WAIT );
			pPlayer->cFrame = 0;
			return 0;
		}
	}

	return 0;
}

int KGCPlayerRyan::OnFormChange(PLAYER* pPlayer, bool bKeyProc)
{
	switch( pPlayer->MetamorphosisForm )
	{
	case FORM_WOLF:
		{
			if( pPlayer->cFrame == 55 )
			{
				pPlayer->SetPlayerState( MID_WOLF_FORM_END );
				pPlayer->cFrame = 0;
				return 0;
			}
		}
		break;
	}

	return 0;
}

int KGCPlayerRyan::OnReturnToJumpOrWait( PLAYER* pPlayer, bool bKeyProc )
{
	if ( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}	

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if ( pPlayer->IsContact )
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
				pPlayer->NoCheckContact = 0;
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);

				if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
			}

			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Fresh_Punch = false;
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;

			return 0;
		}

	}

	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
			pPlayer->cFrame = 0;
		}
		else
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
			pPlayer->cFrame = 50;
			pPlayer->m_bFrameLock = true;
		}
	}

	return 0;
}

int KGCPlayerRyan::OnDashAtkCancel( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->cFrame >= 18 )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}		

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down )
		{
			g_MyD3D->MyCtrl->k_Down = false;
			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

			if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
			{
				return 0;
			}

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 40;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			{
				if( pPlayer->Next_Attack_Count != 255 )
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

			//엘리시스의 경우 타격에 성공했을때 콤보가 이어짐
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			return 0;
		}
		pPlayer->x_Speed = 0.0f;

		if( g_MyD3D->MyCtrl->k_Right )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = true;
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = false;
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
	}

	return 0;
}

int KGCPlayerRyan::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		// 대기 모션 일때는 스킬분기 상태를 아무것도 아닌것으로 한다. 
		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

		if ( g_MyD3D->MyCtrl->k_Skill_Key && pPlayer->IsContact )
		{
			if ( g_MyD3D->MyCtrl->k_Up )
			{
				if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV1 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1 ) &&
					false == g_pMagicEffect->IsMagicEffect(pPlayer->m_iPlayerIndex, EGC_EFFECT_ANKH_OF_RESURRECTION ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_STRONGLY_LIFE_FORCE_LV1 );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Select_Item = false;
					g_MyD3D->MyCtrl->k_Up = false;

					return 0;
				}
				else if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STRONGLY_LIFE_FORCE_LV2 ) &&
					false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2 ) &&
					false == g_pMagicEffect->IsMagicEffect(pPlayer->m_iPlayerIndex, EGC_EFFECT_ANKH_OF_RESURRECTION ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_RYAN_STRONGLY_LIFE_FORCE_LV2 );
					pPlayer->cFrame = 0;
					g_MyD3D->MyCtrl->k_Select_Item = false;
					g_MyD3D->MyCtrl->k_Up = false;

					return 0;
				}
			}

			if ( g_MyD3D->MyCtrl->k_Down
				&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_WILD_HOWLING ) 
				&& false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_WILD_HOWLING ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_RYAN_WILD_HOWLING );
				pPlayer->cFrame = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Down = false;

				return 0;
			}

            if ( ( ( g_MyD3D->MyCtrl->k_Fresh_Right && pPlayer->bIsRight )||( g_MyD3D->MyCtrl->k_Fresh_Left && !pPlayer->bIsRight ) )
                && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_STONE_WALL )
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_STONEWALL ) )
            {
                if ( ( pPlayer->GetIsRight() && g_MyD3D->MyCtrl->k_Fresh_Left ) &&
                    ( !pPlayer->GetIsRight() && g_MyD3D->MyCtrl->k_Fresh_Right ) )
                {
                    pPlayer->bIsRight = !pPlayer->bIsRight;
                }

                pPlayer->SetPlayerState( MID_SKILL_RYAN_STONE_WALL );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;

                return 0;
            }
		}

        if ( g_MyD3D->MyCtrl->k_Fresh_Double_C && pPlayer->IsContact &&  pPlayer->uiMotion != MID_COMMON_CATCH_BY_RYAN )
        {
            if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_RYAN_NATURESHIELD )
                && false == g_pMagicEffect->IsMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_NATURESHIELD ) )
            {
                pPlayer->Direct_Motion_Input( MID_SKILL_RYAN_NATURESHIELD );
                pPlayer->cFrame = 0;
                pPlayer->Next_Attack_Count = 0;
                g_MyD3D->MyCtrl->k_Skill_Key = false;
                g_MyD3D->MyCtrl->k_Fresh_Left = false;
                g_MyD3D->MyCtrl->k_Fresh_Right = false;
                m_dwSuperTime = timeGetTime();
                //pPlayer->Shield = INT_MAX;
                pPlayer->ConfirmUseSheild( CID_RYAN1 );
                pPlayer->Super = 220;
                pPlayer->x_Speed = 0.0f;
                pPlayer->y_Speed = 0.0f;

                return 0;
            }
        }

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}		

		if( g_MyD3D->MyCtrl->k_Up && pPlayer->IsContact)
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Down )
		{
			g_MyD3D->MyCtrl->k_Down = false;
			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

			if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
			{
				return 0;
			}

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 40;
			return 0;
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
				g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
			{
				if( pPlayer->Next_Attack_Count != 255 )
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

			//엘리시스의 경우 타격에 성공했을때 콤보가 이어짐
			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
			return 0;
		}
		pPlayer->x_Speed = 0.0f;

		if( g_MyD3D->MyCtrl->k_Right )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = true;
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().WALK)
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = false;
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
			return 0;
		}
		else        //Walk상태에서 Wait상태로 돌아오는...
		{
			if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
				pPlayer->cFrame = 0;

			if( pPlayer->GetHP() > 0.0f )
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk와 공용으로 사용하기 위해..넣어둔 코드임
			else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
			pPlayer->x_Speed = 0.0f;
		}

		if( g_MyD3D->MyCtrl->Skill_Key_Count == 10 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_RYAN1_CHARGING_ATK) )
		{
			pPlayer->SetPlayerState( MID_SKILL_RYAN_CHARGING_ING );
			pPlayer->cFrame = 0;

			return 0;
		}		

		return 0;
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerRyan::OnChargeX( PLAYER* pPlayer, bool bKeyProc )
{
	static bool bFull = false;
	int uiMotion = -1;

	if ( g_MyD3D->MyCtrl->k_Skill_Key || m_fChargeElapsedTime < 0.4f )
	{
		m_fChargeElapsedTime += GC_ELAPSED_TIME * 0.6f;
		if ( m_fChargeElapsedTime >= 1.0f )
		{
			m_fChargeElapsedTime = 1.0f;
		}

		if ( g_MyD3D->GetMyPlayer()->IsLocalPlayer() )
		{
			g_MyD3D->MyHead->SuperPoint = m_fChargeElapsedTime;
		}

		if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )        //무한 반복하는 프레임..
			pPlayer->cFrame = (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -2;

		if ( !bFull && m_fChargeElapsedTime >= 1.0f )
		{
			bFull = true;
			g_KDSound.Play("40066");
		}

		if ( m_fChargeElapsedTime <= 0.1f && o10 < 6 )
		{
			g_KDSound.Play("280");
		}
	}
	else
	{
		bFull = false;
		g_KDSound.Stop("40066");

		if ( m_fChargeElapsedTime <= 0.5f )
		{
			switch ( pPlayer->Extra_Char_Num )
			{
			case CID_RYAN1:
				uiMotion = MID_SKILL_RYAN_CHARGING_ATK;
				break;
			case CID_RYAN2:
				uiMotion = MID_SKILL_RYAN2_CHARGING_ATK;
				break;
			case CID_RYAN3:
				uiMotion = MID_SKILL_RYAN3_CHARGING_ATK;
				break;
			case CID_RYAN4:
				if ( pPlayer->m_iWeaponType )
					uiMotion = MID_SKILL_RYAN4_1_CHARGING_ATK;
				else
					uiMotion = MID_SKILL_RYAN4_2_CHARGING_ATK;
				break;
			}
		}
		else if ( m_fChargeElapsedTime > 0.5f && m_fChargeElapsedTime < 0.8f ) 
		{
			switch ( pPlayer->Extra_Char_Num )
			{
			case CID_RYAN1:
				uiMotion = MID_SKILL_RYAN_CHARGING_ATK_MIDIUM;
				break;
			case CID_RYAN2:
				uiMotion = MID_SKILL_RYAN2_CHARGING_ATK_MIDIUM;
				break;
			case CID_RYAN3:
				uiMotion = MID_SKILL_RYAN3_CHARGING_ATK_MIDIUM;
				break;
			case CID_RYAN4:
				if ( pPlayer->m_iWeaponType )
					uiMotion = MID_SKILL_RYAN4_1_CHARGING_ATK_MIDIUM;
				else
					uiMotion = MID_SKILL_RYAN4_2_CHARGING_ATK_MIDIUM;
				break;
			}
			
		}
		else
		{
			switch ( pPlayer->Extra_Char_Num )
			{
			case CID_RYAN1:
				uiMotion = MID_SKILL_RYAN_CHARGING_ATK_MAX;
				break;
			case CID_RYAN2:
				uiMotion = MID_SKILL_RYAN2_CHARGING_ATK_MAX;
				break;
			case CID_RYAN3:
				uiMotion = MID_SKILL_RYAN3_CHARGING_ATK_MAX;
				break;
			case CID_RYAN4:
				if ( pPlayer->m_iWeaponType )
					uiMotion = MID_SKILL_RYAN4_1_CHARGING_ATK_MAX;
				else
					uiMotion = MID_SKILL_RYAN4_2_CHARGING_ATK_MAX;
				break;
			}
		}
		
		pPlayer->SetPlayerState( uiMotion );
		pPlayer->cFrame = 0;
		g_MyD3D->MyCtrl->k_Select_Item = false;
		m_fChargeElapsedTime = 0.0f;
		if ( g_MyD3D->GetMyPlayer()->IsLocalPlayer() )
		{
			g_MyD3D->MyHead->SuperPoint = 0.f;
		}
    }
    return 0;
}

int KGCPlayerRyan::OnNatureShield( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->uiMotion == MID_SKILL_RYAN_NATURESHIELD_STANDUP || pPlayer->uiMotion == MID_SKILL_RYAN2_NATURESHIELD_STANDUP ||
        pPlayer->uiMotion == MID_SKILL_RYAN3_NATURESHIELD_STANDUP || pPlayer->uiMotion == MID_SKILL_RYAN4_NATURESHIELD_STANDUP ||
        pPlayer->uiMotion == MID_SKILL_RYAN4_2_NATURESHIELD_STANDUP ) {
		    pPlayer->Shield = 0;
		    pPlayer->Super = 20;
		    return KGCPlayerRyan::OnDashAtkCancel( pPlayer, bKeyProc );
    } else {
        if ( timeGetTime() - m_dwSuperTime > 4000 ) {
            m_dwSuperTime = timeGetTime();
            if( pPlayer->Extra_Char_Num == CID_RYAN2 ) {
                pPlayer->SetPlayerState( MID_SKILL_RYAN2_NATURESHIELD_STANDUP );
            } else if( pPlayer->Extra_Char_Num == CID_RYAN3 ) {                
                pPlayer->SetPlayerState( MID_SKILL_RYAN3_NATURESHIELD_STANDUP );
            } else if( pPlayer->Extra_Char_Num == CID_RYAN4 ) {
                if( pPlayer->GetCurFormTemplate().WAIT == MID_RYAN4_WAIT ) {
                    pPlayer->SetPlayerState( MID_SKILL_RYAN4_NATURESHIELD_STANDUP );
                } else {
                    pPlayer->SetPlayerState( MID_SKILL_RYAN4_2_NATURESHIELD_STANDUP );
                }
            } else {
                pPlayer->SetPlayerState( MID_SKILL_RYAN_NATURESHIELD_STANDUP );
            }
            pPlayer->cFrame = 0;
            pPlayer->Shield = 0;

            return 0;
        } else {
            if( pPlayer->cFrame >= 37 )        //무한 반복하는 프레임..
                pPlayer->cFrame = 36;
        }
    }
	
	return 0;
}

int KGCPlayerRyan::OnRun( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH_ATK );
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Up) )
		{
			if( pPlayer->IsContact )
			{
				if( pPlayer->bIsRight )
					pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunJumpXSpeed;
				else
					pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunJumpXSpeed;

				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
				pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fRunJumpYSpeed;
				pPlayer->cFrame = 0;
				ISPUSHED(k_Up) = false;
				return 0;
			}
			pPlayer->NoCheckContact = 0;
		}

		if( ISPUSHED(k_Down) )
		{
			ISPUSHED(k_Down) = false;

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
			if( pPlayer->bIsRight && ISPUSHED(k_Left) )
			{
				pPlayer->bIsRight = false;
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
				pPlayer->cFrame = 0;
				return 0;
			}

			if( !pPlayer->bIsRight && ISPUSHED(k_Right) )
			{
				pPlayer->bIsRight = true;
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

        pPlayer->SetWindSpeed();

		//정지
		if( ( ( pPlayer->bIsRight &&  !ISPUSHED(k_Right) ) || 
			( !pPlayer->bIsRight &&  !ISPUSHED(k_Left) ) )&& pPlayer->cFrame < 31 )
		{
			pPlayer->cFrame = 31;
			return 0;
		}

		return 0;
	}

    pPlayer->SetWindSpeed();


	//무한 반복하는 프레임..
	if( pPlayer->cFrame == 30 )
		pPlayer->cFrame = 0;

	if( pPlayer->cFrame < 31)
	{
		if( pPlayer->bIsRight )
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
		else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
	}

	return 0;
}
