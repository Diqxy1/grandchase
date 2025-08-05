#include "stdafx.h"

#include "KGCPlayerRonan3.h"
#include "KGCPlayerCommon.h"
#include "Controls.h"
#include "Stage.h"
#include "Damage.h"

KGCPlayerRonan3::KGCPlayerRonan3()
{

}

KGCPlayerRonan3::~KGCPlayerRonan3()
{

}

int KGCPlayerRonan3::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(MID_RONAN3_DASH_ATK1);
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
                return 0;

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
            ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 30)//정지
        {
            pPlayer->cFrame = 30;
            return 0;
        }
        return 0;
        if( pPlayer->cFrame >= 30 )
        {
            if(g_MyD3D->MyCtrl->k_Fresh_Left_Dash)
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
            if(g_MyD3D->MyCtrl->k_Fresh_Right_Dash)
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        return 0;

    }

    ////무한 반복하는 프레임..
    if(pPlayer->cFrame == 29 )
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 29)
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    pPlayer->SetWindSpeed();

    if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().RUN )
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
                pPlayer->SetPlayerState( MID_RONAN3_RUN_TO_WAIT );
            else
                pPlayer->SetPlayerState( MID_RONAN3_RUN_TO_JUMP );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCPlayerRonan3::OnJump(PLAYER* pPlayer, bool bKeyProc)
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
            if ( g_MyD3D->MyCtrl->k_Up )
            {
                if ( pPlayer->m_nForceNum > 0 /*&& pPlayer->cFrame < 20*/ )
                {
                    pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_JUMP_UP );
                    pPlayer->NoCheckContact = 25;
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }
            else if ( g_MyD3D->MyCtrl->k_Down )
            {
                if ( pPlayer->m_nForceNum > 0 )
                {
                    pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_JUMP_DOWN );
                    pPlayer->NoCheckContact = 25;
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }
            //else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_ATK);
                pPlayer->NoCheckContact = pPlayer->GetCurFormTemplate().iJumpAtkNoCheckContact;
                pPlayer->cFrame = 0;
            }
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
        //내려오는 점프 동작
        if( pPlayer->cFrame > 49 )
            pPlayer->cFrame = 49;
    }
    else 
    {
        if( pPlayer->cFrame > 49 )
            pPlayer->cFrame = 49;
        //무한 반복하는 프레임..
        if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1 )
            pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerRonan3::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if ( g_MyD3D->MyCtrl->k_Fresh_Left && pPlayer->m_nForceNum > 0 )
        {
            pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_WAIT );
            pPlayer->bIsRight = false;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Left = false;

            return 0;
        }
        else if ( g_MyD3D->MyCtrl->k_Fresh_Right && pPlayer->m_nForceNum > 0 )
        {
            pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_WAIT );
            pPlayer->bIsRight = true;
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Fresh_Right = false;

            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if ( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
            }
        }

        // 칼 집어넣는것 까지만 하면 움직여도 된다 이놈아... ㅠ
        switch( pPlayer->uiMotion )
        {
        case MID_RONAN3_ATK1:
            {
                if ( pPlayer->cFrame > 36 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                {
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
                }
            }
        case MID_RONAN3_ATK2:
            {
                if ( pPlayer->cFrame > 32 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            }
        case MID_RONAN3_ATK3:
            {
                if ( pPlayer->cFrame > 50 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            }
        case MID_RONAN3_ATK4:
            {
                if ( pPlayer->cFrame > 46 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            }
        case MID_RONAN3_ATK5:
            {
                if ( pPlayer->cFrame > 46 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            }
        case MID_RONAN3_ATK6:
            {
                if ( pPlayer->cFrame > 45 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                    return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
            }
            break;

        default:
            break;
        }

        if( pPlayer->uiMotion  != MID_RONAN3_ATK1 && (g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash) )
        {
            switch( pPlayer->uiMotion )
            {
            case MID_RONAN3_ATK2:
            case MID_RONAN3_ATK3:
            case MID_RONAN3_ATK4:
            case MID_RONAN3_ATK5:
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                pPlayer->AddDamage(DT_CRITICAL, 0);
                break;
            case MID_RONAN3_ATK6:
                {
                    if ( pPlayer->cFrame < 35 )
                    {
                        pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
                        pPlayer->AddDamage(DT_DOUBLE, 0);
                        break;
                    }
                }

            default:
                return 0;
            }

            if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                pPlayer->bIsRight = true;
            else pPlayer->bIsRight = false;

            pPlayer->cFrame = 0;
            return 0;
        }
        return 0;
    }

    return 0;
}

int KGCPlayerRonan3::OnDash(PLAYER* pPlayer, bool bKeyProc)
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
            if( pPlayer->IsContact )
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            }
            else
            {
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP_DASH_ATK);
            }

            if ( ( g_MyD3D->MyCtrl->k_Right && !pPlayer->bIsRight ) 
                || (g_MyD3D->MyCtrl->k_Left && pPlayer->bIsRight) )
            {
                if ( pPlayer->IsContact == true )
                    pPlayer->SetPlayerState( MID_RONAN3_GUARD );
                else 
                    pPlayer->SetPlayerState( MID_RONAN3_JUMP_GUARD );
            }
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Down)
        {
            g_MyD3D->MyCtrl->k_Down = false;

            if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
                return 0;


            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
                return 0;

#if defined( MATCH_BALANCE_2011_8 )
            pPlayer->y_Speed = -0.01f;
#else
            pPlayer->y_Speed -= 0.01f;
#endif
            pPlayer->NoCheckContact = 20;

            return 0;
        }

        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DASH ||
            pPlayer->uiMotion == pPlayer->GetCurFormTemplate().JUMP_DASH )
        {
            if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
            {
                if( true == pPlayer->IsContact )
                    pPlayer->SetPlayerState( MID_RONAN3_DASH_TO_WAIT );
                else
                    pPlayer->SetPlayerState( MID_RONAN3_DASH_TO_JUMP );

                pPlayer->cFrame = 0;
            }
        }
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerRonan3::OnDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if ( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Next_Attack_Count > 0 )
            {
                pPlayer->Next_Attack_Count = 255;
            }

            if ( pPlayer->IsContact )
            {
                if ( g_MyD3D->MyCtrl->k_Left && pPlayer->m_nForceNum > 0 )
                {
                    pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_WAIT );
                    pPlayer->bIsRight = false;
                    pPlayer->cFrame = 0;
                    g_MyD3D->MyCtrl->k_Left = false;

                    return 0;
                }
                else if ( g_MyD3D->MyCtrl->k_Right && pPlayer->m_nForceNum > 0 )
                {
                    pPlayer->SetPlayerState( MID_RONAN3_PSYCHIC_FORCE_WAIT );
                    pPlayer->bIsRight = true;
                    pPlayer->cFrame = 0;
                    g_MyD3D->MyCtrl->k_Right = false;

                    return 0;
                }
            }
        }
    }

    // 마지막 프레임 이라면
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
        {
            switch ( pPlayer->uiMotion )
            {
            case MID_RONAN3_DASH_ATK1:
                pPlayer->SetPlayerState( MID_RONAN3_DASH_ATK1_TO_WAIT );
                pPlayer->cFrame = 0;
                break;
            case MID_RONAN3_DASH_ATK2:
                pPlayer->SetPlayerState( MID_RONAN3_DASH_ATK2_TO_WAIT );
                pPlayer->cFrame = 0;
                break;
            }
        }
        else
        {
            switch ( pPlayer->uiMotion )
            {
            case MID_RONAN3_DASH_ATK1:
                pPlayer->SetPlayerState( MID_RONAN3_DASH_ATK1_TO_JUMP );
                pPlayer->cFrame = 0;
                break;
            case MID_RONAN3_DASH_ATK2:
                pPlayer->SetPlayerState( MID_RONAN3_DASH_ATK2_TO_JUMP );
                pPlayer->cFrame = 0;
                break;
            }
        }
    }

    return 0;
}

int KGCPlayerRonan3::OnDashToJump( PLAYER* pPlayer, bool bKeyProc )
{
    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 )
    {
        if( true == pPlayer->IsContact )
        {
            pPlayer->SetPlayerState( MID_RONAN3_DASH_TO_WAIT );
            pPlayer->cFrame = 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_RONAN3_JUMP );
            pPlayer->cFrame = 50;
            pPlayer->m_bFrameLock = true;
        }
    }

    return 0;
}

int KGCPlayerRonan3::OnGuard( PLAYER* pPlayer, bool bKeyProc )
{
    if ( false == pPlayer->IsContact && pPlayer->cFrame > 7 )
    {
        pPlayer->SetPlayerState( MID_RONAN3_JUMP_GUARD );
    }
    else if ( true == pPlayer->IsContact && pPlayer->cFrame > 7 )
    {
        pPlayer->SetPlayerState( MID_RONAN3_GUARD );
    }

    if( pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() - 1 && pPlayer->uiMotion == MID_RONAN3_JUMP_GUARD )
    {
        if ( pPlayer->IsContact == false )
        {
            pPlayer->SetPlayerState( MID_RONAN3_JUMP );
            pPlayer->cFrame = 50;
            pPlayer->m_bFrameLock = true;
        }
    }

    if ( bKeyProc )
    {
        if ( pPlayer->cFrame >= 18 && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->bIsRight = !pPlayer->bIsRight;
            pPlayer->SetPlayerState( MID_RONAN3_ATK1 );
            pPlayer->cFrame = 0;
        }
    }

    return 0;
}

int KGCPlayerRonan3::OnPsychicForce( PLAYER* pPlayer, bool bKeyProc )
{
    if ( pPlayer->uiMotion == MID_RONAN3_PSYCHIC_FORCE_JUMP_DOWN 
        && 10 < pPlayer->cFrame && 15 >= pPlayer->cFrame )
    {
        if ( pPlayer->y_Speed < 0 )
        {
            pPlayer->y_Speed = 0;
            pPlayer->y_Speed = pPlayer->y_Speed + 0.0065f;
        }
        else
        {
            pPlayer->y_Speed = pPlayer->y_Speed + 0.0015f;
        }

        if( pPlayer->y_Speed > 0.0065f * 5.0f )
            pPlayer->y_Speed = 0.0065f * 5.0f;
    }

    if ( pPlayer->uiMotion == MID_RONAN3_PSYCHIC_FORCE_JUMP_UP 
        && 10 < pPlayer->cFrame && 15 >= pPlayer->cFrame )
    {
        if ( pPlayer->y_Speed < 0 )
        {
            pPlayer->y_Speed = 0;
            pPlayer->y_Speed = pPlayer->y_Speed + 0.0065f;
        }
        else
        {
            pPlayer->y_Speed = pPlayer->y_Speed + 0.0020f;
        }

        if( pPlayer->y_Speed > 0.0065f * 5.0f )
            pPlayer->y_Speed = 0.0065f * 5.0f;
    }

    if( true == pPlayer->IsContact && 
        pPlayer->uiMotion != MID_RONAN3_PSYCHIC_FORCE_WAIT )
    {
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }

    return 0;
}

int KGCPlayerRonan3::OnDashAtkToWait( PLAYER* pPlayer, bool bKeyProc )
{
    switch ( pPlayer->uiMotion )
    {
    case MID_RONAN3_DASH_ATK1_TO_WAIT:
        {
            if ( pPlayer->cFrame > 9 )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
        }
    case MID_RONAN3_DASH_ATK2_TO_WAIT:
        {
            if ( pPlayer->cFrame > 11 )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
        }
    }
	return 0;
}

int KGCPlayerRonan3::OnCriticalAtk( PLAYER* pPlayer, bool bKeyProc )
{
    if ( bKeyProc )
    {
        if ( pPlayer->cFrame > 38 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
            return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }
	return 0;
}

int KGCPlayerRonan3::OnSpecialAtk( PLAYER* pPlayer, bool bKeyProc )
{
    if ( bKeyProc )
    {
        if ( pPlayer->uiMotion == MID_RONAN3_SWORD_SPECIAL1_POST_ATK )
        {
            if ( pPlayer->cFrame > 39 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
        }
        else if( pPlayer->uiMotion == MID_RONAN3_SWORD_SPECIAL2 )
        {
            if ( pPlayer->cFrame > 106 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
        }
        else if( pPlayer->uiMotion == MID_RONAN3_SWORD_SPECIAL3 )
        {
            if ( pPlayer->cFrame > 46 && ( g_MyD3D->MyCtrl->k_Up || g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash ) )
                return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
        }
    }
	return 0;
}

int KGCPlayerRonan3::OnWait( PLAYER* pPlayer, bool bKeyProc )
{
	if(bKeyProc)
	{
		if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode)
		{
			if ( pPlayer->GetBurnningPoint() >= 3  ){
				pPlayer->SetPlayerState(MID_RONAN3_SHIELD_MODE_1);
				pPlayer->cFrame = 0;
				pPlayer->ChangeBurnningTemplate();
				pPlayer->SetBurnningPoint( 0.0f );
				g_MyD3D->MyCtrl->k_Up = false;
				g_MyD3D->MyCtrl->k_Skill_Key = false;
			}
			return 0;
		}
        /*else if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && !pPlayer->m_bIsBurnningMode && (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON))
        {
            if ( pPlayer->GetBurnningPoint() >= 3 ) {
                pPlayer->SetPlayerState(MID_RONAN3_SHIELD_MODE_2);
                pPlayer->cFrame = 0;
                pPlayer->ChangeBurnningTemplate();
                g_MyD3D->MyCtrl->k_Up = false;
                g_MyD3D->MyCtrl->k_Skill_Key = false;
            }
            return 0;
        }
        else if (g_MyD3D->MyCtrl->k_Fresh_Double_C && !pPlayer->m_bIsBurnningMode && !pPlayer->IsMagicEffect(EGC_EFFECT_RONAN3_HITTED_BLOCK_COOL) && (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON))
        {
            pPlayer->SetPlayerState(MID_RONAN3_HITTED_SHIELD);
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Down = false;
            g_MyD3D->MyCtrl->k_Fresh_Double_C = false;
            
            return 0;
        }
		else if ( g_MyD3D->MyCtrl->k_Skill_Key && g_MyD3D->MyCtrl->k_Up && pPlayer->m_bIsBurnningMode && (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) )
		{
			pPlayer->InitBurnningGauge();
			pPlayer->SetMagicEffect(EGC_EFFECT_RONAN3_SHIELD_MODE_UNEQUIP, 1);
			pPlayer->cFrame = 0;
			g_MyD3D->MyCtrl->k_Up = false;
			g_MyD3D->MyCtrl->k_Skill_Key = false;

			return 0;
		}*/
	}

	return KGCPlayerCommon::OnWait(pPlayer, bKeyProc);
}