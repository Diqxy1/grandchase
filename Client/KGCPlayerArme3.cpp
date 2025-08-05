#include "stdafx.h"
//





#include "KGCPlayerCommon.h"
#include ".\kgcplayerarme3.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"
#include "GCCameraOrtho.h"

#include "Headup Display.h"

KSafeArray<PLAYERSTATEFUNCTION,SUMMONCOUNT + 1> KGCPlayerArme3::m_pSummonfunc;
#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

#define DOSUMMON(p, q)  { ISPUSHED(p) = false;						\
						g_MyD3D->GetMyPlayer()->m_iSummoning = 0;	\
						pPlayer->cFrame = 0;							\
						pPlayer->SetPlayerState(q); }

KGCPlayerArme3::KGCPlayerArme3(void)
{	
}

KGCPlayerArme3::~KGCPlayerArme3(void)
{
}

int KGCPlayerArme3::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( g_MyD3D->MyCtrl->k_Fresh_Double_X 
			&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_CHANGE_HP_TO_MP )
			&& pPlayer->GetHP() > 0 && g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
		{	
			pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_HP_TO_MP);	
			pPlayer->cFrame = 0;				
			return 0;
		}
	}
#ifdef MATCH_BALLENCE_121217
    if(g_MyD3D->MyHead->Level >= g_kGlobalValue.GetMaxMP() )		//엠피가 가득 찼을경우
    {
        if( pPlayer->cFrame == 122 )
            pPlayer->cFrame = 0;			
    }
    else //엠피가 덜 찼을 경우
    {

        if( pPlayer->cFrame > 70 && pPlayer->cFrame <= 122 ) {
            pPlayer->cFrame = 123;
        } else {
            if ( g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() ) {
                if( pPlayer->cFrame >= 169 ) {
                    g_MyD3D->ChargeLocalPlayerMana( SiKGCRoomManager()->IsMonsterGameMode() ? 0.018f :0.03f ) ;
                    pPlayer->cFrame = 159;
                }
            }            
        }
    }	
#endif
	return m_pSummonfunc[g_MyD3D->GetMyPlayer()->m_iSummoning](pPlayer, bKeyProc);
}

int KGCPlayerArme3::OnWalk( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		KGCPlayerArme3::OnWait(pPlayer,true);
        pPlayer->SetWindSpeed();
		return 0;
	}

    pPlayer->SetWindSpeed();

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerArme3::OnJumpReady( PLAYER* pPlayer, bool bKeyProc )
{
	if( g_MyD3D->GetMyPlayer()->m_iSummoning == 0 )
	{
		pPlayer->GetCurFormTemplate().JUMP = MID_ARME3_JUMP;
		return KGCPlayerCommon::OnEmpty(pPlayer, bKeyProc);
	}
	else
	{
		pPlayer->GetCurFormTemplate().JUMP = MID_ARME3_SUMMON_JUMP;
		return KGCPlayerArme3::OnSummonJumpReady(pPlayer, bKeyProc);
	}
}

int KGCPlayerArme3::OnJump( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( pPlayer->Extra_Char_Num == CID_ARME3 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_TELEPORT_VERTICAL ))
		{
			if( g_MyD3D->MyCtrl->k_Up && pPlayer->cFrame <= 10 )
			{
				pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_TELEPORT_UP);
				pPlayer->cFrame = 0;
				return 0;
			}
			else if( g_MyD3D->MyCtrl->k_Fresh_Down_Dash)
			{
				pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
				pPlayer->cFrame = 0;
				return 0;
			}
		}

		if( ISPUSHED(k_Fresh_Punch) )
		{
			if( ISPUSHED(k_Down) )
			{
				ISPUSHED(k_Down) = false;
				pPlayer->SetPlayerState( MID_ARME3_JUMPATK1 );
				pPlayer->cFrame = 0;
				g_MyD3D->GetMyPlayer()->m_iSummoning = 0;
				return 0;
			}
			else
			{
				pPlayer->SetPlayerState( MID_ARME3_JUMPATK2 );				
				pPlayer->cFrame = 0;
				g_MyD3D->GetMyPlayer()->m_iSummoning = 0;
				return 0;
			}
            return 0;
		}

        return KGCPlayerCommon::OnJump( pPlayer, true );
	}

    pPlayer->SetWindSpeed();

	if( pPlayer->y_Speed >= 0.0f )
	{
		if( pPlayer->cFrame > 16 )	//내려오는 점프 동작
			pPlayer->cFrame = 16;
	}
	else 
	{
		if( pPlayer->cFrame < 16 )
			pPlayer->cFrame = 16;
		if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
			pPlayer->m_bFrameLock = true;
	}

	return 0;
}

//int KGCPlayerArme3::OnDash( PLAYER* pPlayer, bool bKeyProc )
//{
//	if( bKeyProc )
//	{
//		// Dash Cancel
//		//if( pPlayer->IsRight && ISPUSHED(k_Left) )
//		//{
//		//	ISPUSHED(k_Left) = false;
//		//	pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
//		//	pPlayer->Frame = 0;
//		//	return 0;
//		//}
//		//if( !pPlayer->IsRight && ISPUSHED(k_Right) )
//		//{
//		//	ISPUSHED(k_Right) = false;
//		//	pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
//		//	pPlayer->Frame = 0;
//		//	return 0;
//		//}
//
//		// 대쉬 어택에 콤보 있음
//		if( ISPUSHED(k_Fresh_Punch) )
//		{
//			ISPUSHED(k_Fresh_Punch) = false;
//			pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().DASH_ATK );
//			pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
//			pPlayer->Frame = 0;
//			return 0;
//		}
//
//		return KGCPlayerCommon::OnDash(pPlayer, true);
//	}
//	return 0;
//}

int KGCPlayerArme3::OnDashCombo1( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) )
		{
			// 다음 대쉬 콤보
			if( pPlayer->Next_Attack_Count > 0 )
				pPlayer->Next_Attack_Count = 255;
		}
		return 0;
	}

	return 0;
}

int KGCPlayerArme3::OnDashCombo2( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED( k_Fresh_Up_Dash ) )
		{
			ISPUSHED(k_Fresh_Up_Dash) = false;
			// 다음 대쉬 콤보
			if( pPlayer->Next_Attack_Count > 0 )
				pPlayer->Next_Attack_Count = 255;
		}
		return 0;
	}
	return 0;
}

// 키 입력 바로바로 먹기 위해
int KGCPlayerArme3::OnAtkReady( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{				
		// 하급 소환일 경우
		if( g_MyD3D->GetMyPlayer()->m_iSummoning == 1 )
		{	
			if( ISPUSHED(k_Fresh_Punch) )
			{
				// 중급소환 상태로 가자
				g_MyD3D->GetMyPlayer()->m_iSummoning++;				
				ISPUSHED(k_Fresh_Punch) = false;
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().ATKEND );
				pPlayer->NoCheckContact = 0;
				pPlayer->cFrame = 0;
				return 0;
			}
			else
				KGCPlayerArme3::OnLowSummoning( pPlayer, bKeyProc, false );
		}
		else if(g_MyD3D->GetMyPlayer()->m_iSummoning == 2 )
		{
			KGCPlayerArme3::OnMiddleSummoning( pPlayer, bKeyProc, false );
		}

	}
	return 0;
}

int KGCPlayerArme3::OnNormal( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) && g_MyD3D->GetMyPlayer()->m_iSummoning < SUMMONCOUNT )
		{
			if( g_MyD3D->GetMyPlayer()->m_iSummoning == 0 )
			{
				g_MyD3D->GetMyPlayer()->m_iSummoning++;
				// 하급소환 상태
				ISPUSHED(k_Fresh_Punch) = false;
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().ATK1 );
				pPlayer->NoCheckContact = 0;
				pPlayer->cFrame = 0;			
				return 0;
			}
		}

		if( ISPUSHED(k_Fresh_Right_Dash) )
		{
			pPlayer->bIsRight = true;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Fresh_Left_Dash) )
		{
			pPlayer->bIsRight = false;
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            if( g_kCamera.CheckDashZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			return 0;
		}

		if( ISPUSHED(k_Up) && pPlayer->IsContact)
		{
			pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			pPlayer->cFrame = 0;
			ISPUSHED(k_Up) = false;
			return 0;
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
			pPlayer->NoCheckContact = 40;

			//pPlayer->cFrame = 16;
			//pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
			return 0;
		}

		if( ISPUSHED(k_Right) )
		{
			if( pPlayer->uiMotion != MID_ARME3_WALK )
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = true;
			pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState( MID_ARME3_WALK );
			return 0;
		}
		else if( ISPUSHED(k_Left) )
		{
			if( pPlayer->uiMotion !=  MID_ARME3_WALK )
				pPlayer->cFrame = 0;
			pPlayer->bIsRight = false;
			pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
			pPlayer->SetPlayerState( MID_ARME3_WALK );
			return 0;
		}
		else
		{
			if( pPlayer->uiMotion == MID_ARME3_WALK )
				pPlayer->cFrame = 0;

			if( pPlayer->GetHP() > 0.0f )
				pPlayer->SetPlayerState( MID_ARME3_WAIT );
			else 
				pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().FATAL_WAIT);
			pPlayer->x_Speed = 0.0f;			
		}
		return 0;
	}

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerArme3::OnLowGradeSummon( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Punch) && g_MyD3D->GetMyPlayer()->m_iSummoning < SUMMONCOUNT )
		{
			if( g_MyD3D->GetMyPlayer()->m_iSummoning == 1 )
			{
				g_MyD3D->GetMyPlayer()->m_iSummoning++;
				// 중급소환 상태
				ISPUSHED(k_Fresh_Punch) = false;
				pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().ATKEND );
				pPlayer->NoCheckContact = 0;
				pPlayer->cFrame = 0;				
				return 0;
			}
		}
		return KGCPlayerArme3::OnLowSummoning( pPlayer, bKeyProc );
	}

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerArme3::OnMiddleGradeSummon( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
		return KGCPlayerArme3::OnMiddleSummoning( pPlayer, bKeyProc );

	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )		//무한 반복하는 프레임..
		pPlayer->cFrame = 0;

	return 0;
}

int KGCPlayerArme3::OnSummonJumpReady( PLAYER* pPlayer, bool bKeyProc )
{
	if( bKeyProc )
	{
		if( ISPUSHED(k_Fresh_Up_Dash) )
		{
			// 샐러맨더 소환
			if( g_MyD3D->GetMyPlayer()->m_iSummoning == 1 )
				DOSUMMON( k_Fresh_Up_Dash, MID_ARME3_DASHATK3 )
			//// 샐리스트 소환
			else if( g_MyD3D->GetMyPlayer()->m_iSummoning == 2 )
				DOSUMMON( k_Fresh_Up_Dash, MID_ARME3_SUMMON_SALIST )

			return 0;
		}
		
		if( !g_MyD3D->GetMyPlayer()->m_bJumpZoomed )
		{
            if( g_kCamera.CheckJumpZoom() )
			    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
			g_MyD3D->GetMyPlayer()->m_bJumpZoomed = true;
		}
		return KGCPlayerCommon::OnEmpty(pPlayer, bKeyProc);
	}
	return KGCPlayerCommon::OnEmpty(pPlayer, bKeyProc);
}

int KGCPlayerArme3::OnSpecial1( PLAYER* pPlayer, bool bKeyProc )
{
	g_MyD3D->GetMyPlayer()->m_iSummoning = 0;

	if( bKeyProc )
	{
		return 0;
	}
    
	if( pPlayer->cFrame < 17 )
		pPlayer->cFrame = 18;

	return 0;
}

int KGCPlayerArme3::OnSpecial2( PLAYER* pPlayer, bool bKeyProc )
{
	g_MyD3D->GetMyPlayer()->m_iSummoning = 0;

	if( bKeyProc )
	{
		return 0;
	}

	if(pPlayer->cFrame >= 100 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
		return 0;
	}

	return 0;
}

int KGCPlayerArme3::OnSpecial3( PLAYER* pPlayer, bool bKeyProc )
{
	g_MyD3D->GetMyPlayer()->m_iSummoning = 0;

	if( bKeyProc )
	{
		return 0;
	}

	return 0;
}

int KGCPlayerArme3::OnLowSummoning( PLAYER* pPlayer, bool bKeyProc, bool bForceChange/* = true*/ )
{
	if( ISPUSHED(k_Up) && pPlayer->IsContact)
	{
		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
		pPlayer->cFrame = 0;
		ISPUSHED(k_Up) = false;
		g_MyD3D->GetMyPlayer()->m_bJumpZoomed = false;
		return 0;
	}

	if( g_MyD3D->MyCtrl->k_Up 
		&& g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ARME1_CHANGE_HP_TO_MP )
		&& pPlayer->GetHP() > 0 && g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() )
	{	
		if( pPlayer->cFrame <= 15 )
		{
			pPlayer->SetPlayerState(MID_SKILL_COMMON_ARME_HP_TO_MP);				
			pPlayer->cFrame = 0;				
			if( g_MyD3D->GetMyPlayer()->m_iSummoning > 0 )
				g_MyD3D->GetMyPlayer()->m_iSummoning = 0;
			return 0;
		}			
	}

	// 실프 소환
	if( pPlayer->bIsRight && ISPUSHED(k_Fresh_Left_Dash) )
	{
		DOSUMMON( k_Fresh_Left_Dash, MID_ARME3_SUMMON_SHYLPH );
		return 0;
	}
	else if( !pPlayer->bIsRight && ISPUSHED(k_Fresh_Right_Dash) )
	{
		DOSUMMON( k_Fresh_Right_Dash, MID_ARME3_SUMMON_SHYLPH );
		return 0;
	}

	// 운디네 소환
	if( pPlayer->bIsRight && ISPUSHED(k_Fresh_Right_Dash) )
	{
		pPlayer->bIsRight = true;
		DOSUMMON( k_Fresh_Right_Dash, MID_ARME3_SUMMON_ENDINE );
		return 0;
	}
	else if( !pPlayer->bIsRight && ISPUSHED(k_Fresh_Left_Dash) )
	{
		pPlayer->bIsRight = false;
		DOSUMMON( k_Fresh_Left_Dash, MID_ARME3_SUMMON_ENDINE );
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
		//pPlayer->cFrame = 16;
		//pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
		return 0;
	}

	if( ISPUSHED(k_Right) )
	{
		if( pPlayer->uiMotion != MID_ARME3_SUMMON_MOVE )
			pPlayer->cFrame = 0;			
		pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
		pPlayer->SetPlayerState( MID_ARME3_SUMMON_MOVE );
		return 0;
	}
	else if( ISPUSHED(k_Left) )
	{
		if( pPlayer->uiMotion != MID_ARME3_SUMMON_MOVE )
			pPlayer->cFrame = 0;
		pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
		pPlayer->SetPlayerState( MID_ARME3_SUMMON_MOVE );
		return 0;
	}
	else
	{
		if( bForceChange == true )
		{
			if( pPlayer->uiMotion == MID_ARME3_SUMMON_MOVE )
				pPlayer->cFrame = 0;

			pPlayer->SetPlayerState( MID_ARME3_SUMMON_WAIT );
			pPlayer->x_Speed = 0.0f;
		}
	}

	return 0;
}

int KGCPlayerArme3::OnUnSummon( PLAYER* pPlayer, bool bKeyProc )
{
	if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
	{
		pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().WAIT );
		pPlayer->cFrame = 0;
		return 0;
	}

	return 0;
}

int KGCPlayerArme3::OnMiddleSummoning( PLAYER* pPlayer, bool bKeyProc, bool bForceChange/* = true*/ )
{
	if( ISPUSHED(k_Up) && pPlayer->IsContact)
	{
		pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_READY);
        if( g_kCamera.CheckJumpZoom() )
		    g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
		pPlayer->cFrame = 0;
		ISPUSHED(k_Up) = false;
		g_MyD3D->GetMyPlayer()->m_bJumpZoomed = false;
		return 0;
	}

	// 소환해제
	if( pPlayer->bIsRight && ISPUSHED(k_Fresh_Left_Dash) )
	{
		pPlayer->bIsRight = true;
		DOSUMMON( k_Fresh_Left_Dash, MID_ARME3_UNSUMMON )
		return 0;
	}
	else if( !pPlayer->bIsRight && ISPUSHED(k_Fresh_Right_Dash) )
	{
		pPlayer->bIsRight = false;
		DOSUMMON( k_Fresh_Right_Dash, MID_ARME3_UNSUMMON )
		return 0;
	}

	// 운다인 소환
	if( pPlayer->bIsRight && ISPUSHED(k_Fresh_Right_Dash) )
	{
		pPlayer->bIsRight = true;
		DOSUMMON( k_Fresh_Right_Dash, MID_ARME3_SUMMON_UNDINE )
		return 0;
	}
	else if( !pPlayer->bIsRight && ISPUSHED(k_Fresh_Left_Dash) )
	{
		pPlayer->bIsRight = false;
		DOSUMMON( k_Fresh_Left_Dash, MID_ARME3_SUMMON_UNDINE )
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
		//pPlayer->cFrame = 16;
		//pPlayer->SetPlayerState( pPlayer->GetCurFormTemplate().JUMP );
		return 0;
	}

	if( ISPUSHED(k_Right) )
	{
		if( pPlayer->uiMotion != MID_ARME3_SUMMON_MOVE )
			pPlayer->cFrame = 0;
		pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fWalkSpeed;
		pPlayer->SetPlayerState( MID_ARME3_SUMMON_MOVE );
		return 0;
	}
	else if( ISPUSHED(k_Left) )
	{
		if( pPlayer->uiMotion != MID_ARME3_SUMMON_MOVE )
			pPlayer->cFrame = 0;
		pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fWalkSpeed;
		pPlayer->SetPlayerState( MID_ARME3_SUMMON_MOVE );
		return 0;
	}
	else
	{
		if( bForceChange == true )
		{
			if( pPlayer->uiMotion == MID_ARME3_SUMMON_MOVE )
				pPlayer->cFrame = 0;

			pPlayer->SetPlayerState( MID_ARME3_SUMMON_WAIT );
			pPlayer->x_Speed = 0.0f;
		}
	}

	return 0;
}

//int KGCPlayerArme3::OnChangeHpToMp(PLAYER* pPlayer, bool bKeyProc)
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