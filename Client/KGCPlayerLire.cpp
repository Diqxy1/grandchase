#include "stdafx.h"
// KGCPlayerLire.cpp: implementation of the KGCPlayerLire class.
//
//////////////////////////////////////////////////////////////////////
//


#include "KDInput.h"

#include "GCCameraOrtho.h"

#include "KGCPlayerLire.h"
#include "DamageManager.h"
#include "Controls.h"


#include "KGCPlayerCommon.h"
#include "Stage.h"

//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float KGCPlayerLire::bodyAngle = 0.0f;

KGCPlayerLire::KGCPlayerLire()
{

}

KGCPlayerLire::~KGCPlayerLire()
{

}

int KGCPlayerLire::OnDoubleJump(PLAYER* pPlayer, bool bKeyProc)
{
	if(bKeyProc)
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_JUMP_ATK);
			pPlayer->NoCheckContact=30;
			pPlayer->x_Speed=0;
			pPlayer->y_Speed=0.01f;
			pPlayer->cFrame=0;
			return 0;
		}		
		return 0;
	}
	if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//���� �ݺ��ϴ� ������..
		pPlayer->cFrame = 0;
	return 0;
}

int KGCPlayerLire::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
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

		//�޺��߿� �� ���� ���� �����ϰ�...
		if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_COMBO_B ) && pPlayer->uiMotion == MID_LIRE_ATK1_2)
		{
			goto ENABLE_ADJUST_ANGLE;
		}
		
		if( pPlayer->uiMotion == MID_LIRE_ATK1_1 )
		{
ENABLE_ADJUST_ANGLE:
			//if( diks[DIK_UP] )
			if( diks[g_MyD3D->MyCtrl->m_keyUp] )
			{
				pPlayer->Body_Angle += 0.1f;
				if(pPlayer->Body_Angle > 1.57f) 
					pPlayer->Body_Angle=1.57f;
			}
			//else if( diks[DIK_DOWN] )
			else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->Body_Angle -= 0.1f;
				if( pPlayer->Body_Angle < -0.8f ) 
					pPlayer->Body_Angle = -0.8f;
			}	

			// Z Ű ������ ������ Ȱ �ȳ����� ����
			//if( diks[DIK_Z] )
			if( pPlayer->uiMotion == MID_LIRE_ATK1_1 && diks[g_MyD3D->MyCtrl->m_keyAttack] )
			{			
				if( pPlayer->cFrame == 11 || pPlayer->cFrame == 12 )
				{
					pPlayer->Old_Frame = pPlayer->cFrame - 2;
					pPlayer->cFrame--;
					if( pPlayer->Next_Attack_Count < 8 )
						pPlayer->Next_Attack_Count = 8;
					pPlayer->Slow_Count = 2;
				}
                if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash || g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                {
                    pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH );
                }
			}
		}

		if ( pPlayer->Next_Attack_Count > 0
			&& ( ( pPlayer->uiMotion == MID_LIRE_ATK1_2 && pPlayer->cFrame > 11 )
				|| pPlayer->uiMotion == MID_LIRE_ATK1_3
				|| pPlayer->uiMotion == MID_LIRE_ATK1_4 ) )
		{
			//====================================================================================
			// 2009.02.18 : Jemitgge
			// Comment : �޺��߿� �������� z 3�� ������ ���~! ���� �����Ҷ� z�ѹ� ��������;;
			if ( g_Hot_Count > 3 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_CONTINUOUS_COMBO_SHOT ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LIRE_CONTINUOUS_COMBO_SHOT );
				pPlayer->cFrame = 0;
				g_Hot_Count = 0;

				return 0;
			}						
#ifdef MATCH_BALLENCE_121217
		}
#endif
	        if ( g_MyD3D->MyCtrl->k_Fresh_Up_Dash )
	        {
	            if ( true == g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_MELEE_COMBO_LV1) )
	            {
	                pPlayer->SetPlayerState( MID_SKILL_LIRE_MELEE_COMBO_LV1 );
	                pPlayer->Body_Angle = 0;
	                pPlayer->cFrame = 0;
	                pPlayer->Next_Attack_Count=0;

	                return 0;
	            }
	            else if ( true == g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_MELEE_COMBO_LV2) )
	            {
	                pPlayer->SetPlayerState( MID_SKILL_LIRE_MELEE_COMBO_LV2 );
	                pPlayer->Body_Angle = 0;
	                pPlayer->cFrame = 0;
	                pPlayer->Next_Attack_Count=0;

	                return 0;
	            }
#ifndef MATCH_BALLENCE_121217
			}
#endif
		}
		

		if(pPlayer->Next_Attack_Count == 0)
			return 0;
		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
		{
			if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
			{
				if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1+4 )	//Critical
				{
					g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
				}
				else //Double
				{
					g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
				}

				if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
					pPlayer->bIsRight = true;
				else 
					pPlayer->bIsRight = false;

				pPlayer->Next_Attack_Count=0;
				pPlayer->Ah_Sound(5);

				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
				pPlayer->cFrame = 0;

				return 0;
			}
		}

		return 0;
	}	
	return 0;
}

int KGCPlayerLire::OnSiegeMode( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		//�޺��߿� �� ���� ���� �����ϰ�...
		if( pPlayer->uiMotion == MID_SKILL_LIRE_SIEGE_WAIT )
		{
			if( g_MyD3D->MyCtrl->k_Fresh_Punch )
			{
				float tmp = pPlayer->Body_Angle;
				if ( g_Hot_Count >= 2 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_DOUBLE_SIEGE_SHOT ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_LIRE_DOUBLE_SIEGE_SHOT );
					pPlayer->cFrame = 0;
					pPlayer->Body_Angle = tmp;
					g_Hot_Count = 0;

					return 0;
				}
				
				pPlayer->SetPlayerState( MID_SKILL_LIRE_SIEGE_ATK );
				pPlayer->cFrame = 0;
				pPlayer->Body_Angle = tmp;
				g_MyD3D->MyCtrl->k_Fresh_Punch = false;
				
				return 0;
			}

			if ( g_MyD3D->MyCtrl->k_Fresh_Left || g_MyD3D->MyCtrl->k_Fresh_Right )
			{
				pPlayer->SetPlayerState( MID_SKILL_LIRE_SIEGE_END );
				pPlayer->Body_Angle = 0;
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;

				return 0;
			}
			//if( diks[DIK_UP] )
			if( diks[g_MyD3D->MyCtrl->m_keyUp] )
			{
				pPlayer->Body_Angle += 0.1f;
				if(pPlayer->Body_Angle > 1.57f) 
					pPlayer->Body_Angle=1.57f;
			}
			//else if( diks[DIK_DOWN] )
			else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
			{
				pPlayer->Body_Angle -= 0.1f;
				if( pPlayer->Body_Angle < -0.8f ) 
					pPlayer->Body_Angle = -0.8f;
			}
			else
			{
				return 0;
			}
		}
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )      //���� �ݺ��ϴ� ������..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerLire::OnSiegeModeAtk( PLAYER* pPlayer, bool bKeyProc )
{
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SIEGE_WAIT );
	}

	return 0;
}

int KGCPlayerLire::OnDodge( PLAYER* pPlayer, bool bKeyProc )
{
	if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 2 &&
		false == pPlayer->m_bJumpDashed )
	{
        D3DXVECTOR3 vTempPos = pPlayer->GetPosition();
		if( false == pPlayer->bIsRight )
		{
			vTempPos.x -= 0.35f;
		}
		else if( true == pPlayer->bIsRight )
		{
			vTempPos.x += 0.35f;
		}
		vTempPos.y += 0.55f;
        pPlayer->SetDirectMove( vTempPos.x, vTempPos.y );
		pPlayer->NoCheckContact = 35;
		pPlayer->m_bJumpDashed = true;
		return 0;
	}
	if( pPlayer->cFrame == (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->m_bJumpDashed = false;
	}
	return 0;
}

int KGCPlayerLire::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		//���� �ٲٱ� �����ϰ� ���� �������� ���� ����
		if( g_MyD3D->MyCtrl->k_Right )
		{
			pPlayer->bIsRight = true;
		}
		else if( g_MyD3D->MyCtrl->k_Left )
		{
			pPlayer->bIsRight = false;
		}

		if (g_MyD3D->MyCtrl->X_Count != 0)
		{	//====================================================================================
			// 2009.02.18 : Jemitgge
			// Comment : ������ ������ ����, ������ �����ϸ� ����~
			if ((g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV1)
				|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV2)
				|| g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE_NEW_AMPLIFIER_LV3))
				&& g_MyD3D->MyCtrl->k_Down)
			{
				pPlayer->SetPlayerState(MID_SKILL_LIRE_DAMAGE_AMPLIFIER);
				if (pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;

				return 0;
			}
		}
		
		if( g_MyD3D->MyCtrl->k_Fresh_Punch )
		{
			if ( g_Hot_Count >= 2 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_JUMPING_DOUBLE_SHOT ) )
			{
				pPlayer->SetPlayerState(MID_SKILL_LIRE_JUMPING_SHOT);
				if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;
				g_Hot_Count = 0;

				return 0;
			}
			else
			{
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
				if(pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact != 0)
					pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
				pPlayer->cFrame = 0;
			}
			
			return 0;
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

		if( g_MyD3D->MyCtrl->k_Up )
        {
            return KGCPlayerCommon::DoubleJumpFunc( pPlayer );			
		}

        pPlayer->SetWindSpeed();
		return 0;
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		//�������� ���� ����
		if( pPlayer->cFrame > 20 )
			pPlayer->cFrame = 20;
	}
	else 
	{
		//if( pPlayer->cFrame < 20 )
			//pPlayer->cFrame = 20;
		//���� �ݺ��ϴ� ������..
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
			pPlayer->m_bFrameLock = true;
	}
	return 0;
}

int KGCPlayerLire::OnDamageAmplifier( PLAYER* pPlayer, bool bKeyProc )
{
	if ( true == pPlayer->IsContact )
	{
		pPlayer->SetPlayerState( MID_LIRE_WAIT );
		pPlayer->cFrame = 0;
		pPlayer->Body_Angle = 0;
		return 0;
	}

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //���� �ݺ��ϴ� ������..
	{
		pPlayer->SetPlayerState( MID_LIRE_JUMP );
		pPlayer->cFrame = pPlayer->GetNowMotionFRM()->GetNum_Frame() -1;
		pPlayer->m_bFrameLock = true;
	}

	return 0;
}

int KGCPlayerLire::OnDashAtk( PLAYER* pPlayer, bool bKeyProc )
{
	//====================================================================================
	// 2009.02.02 : Jemitgge
	// Comment : �뽬 ���� ��
	if ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_DASH_DOUBLE_ATK ) 
		&& pPlayer->cFrame >= 14 )
	{
		if ( bKeyProc )
		{
			if ( g_MyD3D->MyCtrl->k_Fresh_Punch && pPlayer->IsContact )
			{
				pPlayer->Next_Attack_Count=0;
				pPlayer->Ah_Sound(5);

				pPlayer->SetPlayerState( MID_SKILL_LIRE_DOUBLE_DASH_SHOT );
				pPlayer->cFrame = 0;
				//pPlayer->Body_Angle *= -1.0f;
			}
		}
		else
			return 0;
	}

	return 0;
}

int KGCPlayerLire::OnDash( PLAYER* pPlayer, bool bKeyProc )
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

			pPlayer->y_Speed += -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}

		return 0;
	}

	// ������ �ӵ� ����
	if( pPlayer->bIsRight )
	{
		pPlayer->x_Speed -= 0.005f;
		if( pPlayer->x_Speed < 0.0f )
			pPlayer->x_Speed = 0.0f;
	}
	else
	{
		pPlayer->x_Speed += 0.005f;
		if( pPlayer->x_Speed > 0.0f )
			pPlayer->x_Speed = 0.0f;
	}

    pPlayer->SetWindSpeed();

	return 0;
}

int KGCPlayerLire::OnWait( PLAYER* pPlayer, bool bKeyProc )
{	
	if(bKeyProc)
	{
		// ��� ��� �϶��� ��ų�б� ���¸� �ƹ��͵� �ƴѰ����� �Ѵ�. 
		pPlayer->m_iBranchSkill = SKILL_BRANCH_NONE;

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

		if ( g_MyD3D->MyCtrl->X_Count != 0  && pPlayer->IsContact )
		{
			//====================================================================================
			// 2009.02.19 : Jemitgge
			// Comment : Wait���¿��� x �踦 ������ ���� �ߵ�!!
			//			 ���� �ƽ�Ʈ�� �ѵ�;; ������ �ϳ��� ������ �ϰ�, ���� ��Ÿ�� ���� ������Ѵ�..
			if ( g_MyD3D->MyCtrl->k_Up 
				&& ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_MP_REGEN_SPEEDUP ) 
				|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_SHOT_SPEEDUP ) 
				|| g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_MOVE_SPEEDUP ) ) 
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MOVE_SPEEDUP, true ) 
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_SHOT_SPEEDUP, true )
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MP_REGEN_SPEED, true )
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MOVE_SPEEDUP_DUN, true )
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_SHOT_SPEEDUP_DUN, true )
				&& false == pPlayer->IsMagicEffect( EGC_SKILLEFFECT_MP_REGEN_SPEED_DUN, true ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LIRE_IMAGE );
				pPlayer->Body_Angle = 0;
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Up = false;

				return 0;
			}

			//====================================================================================
			// 2009.02.19 : Jemitgge
			// Comment : Wait���¿��� x �鸦 ������ ������ �ߵ�!
			if ( g_MyD3D->MyCtrl->k_Down && ( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_SIEGE ) || g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_DOUBLE_SIEGE_SHOT ) ) )
			{
				pPlayer->SetPlayerState( MID_SKILL_LIRE_SIEGE_START );
				pPlayer->Body_Angle = 0;
				pPlayer->cFrame = 0;
				pPlayer->Next_Attack_Count = 0;
				g_MyD3D->MyCtrl->k_Select_Item = false;
				g_MyD3D->MyCtrl->k_Down = false;

				return 0;
			}
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
				//====================================================================================
				// 2009.02.19 : Jemitgge
				// Comment : ������ ���... Z+X �������� �ǰ� �Ѵ�;
				if ( g_MyD3D->MyCtrl->k_Select_Item && pPlayer->IsContact && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_DODGE ) )
				{
					pPlayer->SetPlayerState( MID_SKILL_LIRE_DODGE_START );
					pPlayer->Body_Angle = 0;
					pPlayer->cFrame = 0;
					pPlayer->Next_Attack_Count = 0;
				}
				else
				{
					pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1);
				}
			}
			//KeyProc�ڿ��� ���� Frameó�� �ϴ� �κ��� �����Ƿ� Frame = 0���� ���ݴϴ�.
			//KeyProc�� �ƴϸ� Frame = -1�� �غ��ô�!

			pPlayer->NoCheckContact = 0;

			pPlayer->cFrame = 0;

			g_MyD3D->MyCtrl->k_Fresh_Punch = false;

			//�����ý��� ��� Ÿ�ݿ� ���������� �޺��� �̾���
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
		else        //Walk���¿��� Wait���·� ���ƿ���...
		{
			if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().WALK )
				pPlayer->cFrame = 0;

			if( pPlayer->GetHP() > 0.0f )
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WAIT);        //Walk�� �������� ����ϱ� ����..�־�� �ڵ���
			else pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
			pPlayer->x_Speed = 0.0f;

		}

		return 0;
	}

	pPlayer->m_bJumpDashed = false;
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() )      //���� �ݺ��ϴ� ������..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerLire::OnJumpDash( PLAYER* pPlayer, bool bKeyProc )
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

		if( pPlayer->IsContact && ( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right
			|| !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left ) )
		{
			pPlayer->SetPlayerState( MID_LIRE_WALK );
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
			pPlayer->NoCheckContact = 20;
			return 0;
		}

		if( pPlayer->cFrame == 0 )
		{
			g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
			g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
		}

		return 0;
	}

	/*if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		if( true == pPlayer->IsContact )
			pPlayer->SetPlayerState( MID_AMY1_DASH_TO_WAIT );
		else
			pPlayer->SetPlayerState( MID_AMY1_DASH_TO_JUMP );
		pPlayer->cFrame = 0;
	}*/

	return 0;
}

int KGCPlayerLire::OnShotMagicCircle( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->cFrame > 5 )
		return 0;

	if ( bKeyProc )
	{
		if( diks[g_MyD3D->MyCtrl->m_keyUp] )
		{
			bodyAngle += 0.1f;
			if( bodyAngle > 1.57f )
				bodyAngle = 1.57f;
		}
		else if( diks[g_MyD3D->MyCtrl->m_keyDown] )
		{
			bodyAngle -= 0.1f;
			if( bodyAngle < -1.57f ) 
				bodyAngle = -1.57f;
		}
	}

	if ( bodyAngle >= 1.1f && bodyAngle <= 1.57f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_90 );
	}
	else if ( bodyAngle >= 0.4f && bodyAngle < 1.1f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_45 );
	}
	else if ( bodyAngle >= -0.4f && bodyAngle < 0.4f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_0 );
	}
	else if ( bodyAngle >= -1.1f && bodyAngle < -0.4f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_315 );
	}
	else if ( bodyAngle >= -1.57f && bodyAngle < -1.1f )
	{
		pPlayer->SetPlayerState( MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_270 );
	}

	return 0;
}

int KGCPlayerLire::OnShotMagicCircleInit( PLAYER* pPlayer, bool bKeyProc )
{
	bodyAngle = 0.0f;

	return 0;
}

int KGCPlayerLire::OnContinuousCombo( PLAYER* pPlayer, bool bKeyProc )
{
	if ( pPlayer->cFrame < 25 )
		return 0;
	
	if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
	{
		if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1+4 )	//Critical
		{
			g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
		}
		else //Double
		{
			g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
		}

		if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
			pPlayer->bIsRight = true;
		else 
			pPlayer->bIsRight = false;

		pPlayer->Next_Attack_Count=0;
		pPlayer->Ah_Sound(5);

		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
		pPlayer->cFrame = 0;

		return 0;
	}

	return 0;
}