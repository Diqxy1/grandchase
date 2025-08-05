#include "stdafx.h"
#include ".\HardDamageHold.h"
#include "KGCPlayerCommon.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "LatencyTransfer.h"

CHardDamageHold::CHardDamageHold(void)
{
}

CHardDamageHold::~CHardDamageHold(void)
{
}


bool CHardDamageHold::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if ( hitWho < 0 )
    {
        ASSERT_MBOX( "뭥미?" );
        return false;
    }

	if( ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) ) )
	{
		if ( g_MyD3D->MyPlayer[hitWho]->GetHP() == 0.0f &&
			( g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP ) ||
			  g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP ) ||
			  g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER ) ||
			  g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_DOWN_IN_SKY) ) )
			  return false;

		if( pDamageInstance->m_Who < MAX_PLAYER_NUM )
		{
			g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_Who );
		}		
		else
        {
            // Attacker셋팅( 소환된 몬스터가 Attacker라면 플레이어 Index를 셋팅해주자. )
            int iAttacker = pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM;
            if( g_kMonsterManager.IsSummonMonsterByPlayer(pDamageInstance->m_ExtraMonsterNum) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
				if( pMonster ) {
					PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
					if( pPlayer )
						iAttacker = pPlayer->m_iPlayerIndex;
				}
            }
			g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( iAttacker );
        }

        if( pDamageInstance->CheckAlreadyDamaged(hitWho) &&
            ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE ) )
        {
            return false;
        }
        
        if ( m_dwDMAttribute & DA_CANCEL_JUMP_STATE )
        {
            if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )        //점프 상태 해제
                g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;    
        }

		if( g_MyD3D->MyPlayer[hitWho]->IsContact ||
			pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_PLAYER || 
			pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_MONSTER ||
			pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_PLAYER || 
			pDamageInstance->m_What == DT_PET_MAID_THROW_PLAYER || 
            pDamageInstance->m_What == DT_PET_CHRISTMAS_MAID_THROW_PLAYER )
		{	
            g_Push_Count = 0;
            int iStateID = MID_COMMON_HOLD;
			bool bRenderParticle = false;
            if( pDamageInstance->m_What == DT_ARME_SKILL_HOLDBEAM_FIRE ||
                pDamageInstance->m_What == DT_ARME_SKILL_HOLDBEAM_FIRE_LV2 ||
                pDamageInstance->m_What == DT_KARUEL_MON_GUARDIANGOLEM_ATK3_2 ||
                pDamageInstance->m_What == DT_KARUEL_MON_GUARDIANGOLEM_ATK3_2_EFF )
            {
				g_Max_Push = 6;
			}
			else if( pDamageInstance->m_What == DT_SAKU_ATTACK )
			{
				g_Max_Push = 8;
			}
            else if ( pDamageInstance->m_What == DT_PET_RICA_LASER_SHOT || pDamageInstance->m_What == DT_PET_RICA_LASER_SHOT_R)
            {
                g_Max_Push = 8;
            }
            else if( pDamageInstance->m_What == DT_RONAN_PARALYZE_BEAM )
            {
                g_Max_Push = 30;
                return CDamage::CrashPlayer(pDamageInstance,hitWho,fDamageRate);
            }
			else if( pDamageInstance->m_What == DT_BUZZY_BUBLEGUM_BOMB ||  pDamageInstance->m_What == DT_BUZZY_BUBBLEGUM_THROW )
            {
                g_Max_Push = 8;
                iStateID = MID_COMMON_HOLD_UNTIL_ATTACKED;
            }
			else if( pDamageInstance->m_What == DT_THANATOS2_VAMPIRE_R || 
					 pDamageInstance->m_What == DT_THANATOS2_VAMPIRE_L )
			{
				g_Max_Push = 30;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

				if ( pMonster->GetHP() > 0.0f)
				{
#if defined( LEVEL_DESIGN_STAT_CALC )
					pMonster->SetHP( pMonster->GetHP() + m_fDamageToMonster * -1.0f );
#else
					pMonster->SetHP( pMonster->GetHP() + m_fDamageToMonster * -1000.0f );
#endif
				}
			}
			else if( pDamageInstance->m_What == DT_THANATOS3_TAKEUNDERCONTROL )
			{
				g_Max_Push = 30;
			}
			else if(pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_PLAYER )
			{
				g_Max_Push = 20;

				iStateID = MID_COMMON_HOLD_NOEFFECT;
			}
			else if( pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_PLAYER 
				|| pDamageInstance->m_What == DT_PET_MAID_THROW_PLAYER
                || pDamageInstance->m_What == DT_PET_CHRISTMAS_MAID_THROW_PLAYER )
			{
				g_Max_Push = 15;
				
				iStateID = MID_COMMON_HOLD_NOEFFECT;
			}
			else if(pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_MONSTER ||
				pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_MONSTER ||
				pDamageInstance->m_What == DT_PET_MAID_THROW_MONSTER

				)
            {
				pDamageInstance->InsertAlreadyDamaged( hitWho );
				return false;
            }

            else if(pDamageInstance->m_What == DT_PET_SEAMUS_HOLD)
            {
                g_Max_Push = 20;
            }
            else if( pDamageInstance->m_What == DT_HERO_ICEGOLEM_ATK2 )
            {
                iStateID = MID_COMMON_HOLD_NOEFFECT;
                g_Max_Push = 6;
            }
            else if ( pDamageInstance->m_What == DT_HERO_KAMIKI_ATK2_HOLD ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK2_HOLD ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK2_HOLD )
            {
                g_Max_Push = 10;
            }
            else if ( pDamageInstance->m_What == DT_DIO4_DEVIL_EVILSHOCK_TYPE1_LV1_DMG || 
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_1_HOLD ||
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_2_HOLD || 
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_3_HOLD )
            {
                g_Max_Push = 15;
            }
            else if ( pDamageInstance->m_What == DT_DIO4_DEVIL_EVILSHOCK_TYPE1_LV2_DMG || 
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_1_HOLD ||
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_2_HOLD ||
                pDamageInstance->m_What == DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_3_HOLD )
            {
                g_Max_Push = 30;
            }
            else if ( pDamageInstance->m_What == DT_BEIGAS1_SPECIAL_TYPE5_LV1_HOLD_RANGE )
            {
                g_Max_Push = 10;
                iStateID = MID_COMMON_ZERO3_OS_UNIQUE_HOLD;
            }
            else if( pDamageInstance->m_What == DT_PET_TRICKY_PLAYER_ATK )
            {
                g_Max_Push = 10;

                if( (m_iMagicEffect > -1) )
                {
                    if( false == (m_bUniqueMgicEff && g_MyD3D->MyPlayer[ hitWho ]->IsMagicEffect( m_iMagicEffect )) )
                    {
                        DWORD dwAttackUnit = 0;
                        if( OBJ_TYPE_PLAYER == pDamageInstance->m_emWhoType && 
                            true == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
                            false == g_MyD3D->IsSameTeam( pDamageInstance->m_Who, hitWho ) )
                        {
                            dwAttackUnit = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->m_kUserInfo.dwUID;
                        }

                        g_LatencyTransfer.SendMagicEffectSynchPacket( m_iMagicEffect, m_fMagicEffTime , true, hitWho, m_iMagicLevel, dwAttackUnit );
                    }
                }
            }            
            
			g_MyD3D->MyPlayer[hitWho]->SetPlayerState(iStateID);
			g_MyD3D->MyPlayer[hitWho]->cFrame    = 0;			
			g_MyD3D->MyPlayer[hitWho]->m_bIsPushRightKey = g_MyD3D->MyPlayer[hitWho]->bIsRight;
			g_MyD3D->MyPlayer[hitWho]->bIsRight  = true;
			
            g_MyD3D->MyPlayer[hitWho]->m_bIsContinousDamage = pDamageInstance->m_pOriginalDamage->m_bIsContinousDamage;
            g_MyD3D->MyPlayer[hitWho]->m_fContinousDamage = pDamageInstance->m_pOriginalDamage->m_fContinousDamage;
			g_MyD3D->MyPlayer[hitWho]->m_iStoneCurseType = pDamageInstance->m_What;


			pDamageInstance->InsertAlreadyDamaged( hitWho );
            
			//if(pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_PLAYER )
			//	g_MyD3D->MyPlayer[hitWho]->AddDamage(DT_PET_MASHIMARO_PLUNGER_PLAYER,0);				
		}
       
	}

    if ( (m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

	return true;
}
void CHardDamageHold::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	if(pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_MONSTER||
		pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_MONSTER ||
		pDamageInstance->m_What == DT_PET_MAID_THROW_MONSTER)
	{
		CDamage::CrashMonster( pDamageInstance, Monsteri );
		return;
	}
	else if(pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_PLAYER ||
		pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_PLAYER || 
		pDamageInstance->m_What == DT_PET_MAID_THROW_PLAYER || 
        pDamageInstance->m_What == DT_PET_CHRISTMAS_MAID_THROW_PLAYER )
		return;
    if ( (m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

	if ( pDamageInstance->InsertAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ) == false ) {
		return;
	}

    // 몬스터에게 스톤커스.. 
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if( pMonster == NULL ) return;

    // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
    pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
#if defined( LEVEL_DESIGN_STAT_CALC )
    pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
                                            ATTACKTYPE_HOLD, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                            pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#else
    pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
                                            ATTACKTYPE_HOLD, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                            pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#endif

    if (m_iCrashSound > -1)
    {
        char strTemp[15];
        sprintf( strTemp , "%d" , m_iCrashSound);
        g_KDSound.Play( strTemp );
    }
}
