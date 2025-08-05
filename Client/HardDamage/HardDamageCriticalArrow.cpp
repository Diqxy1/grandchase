#include "stdafx.h"
#include ".\HardDamageCriticalArrow.h"
#include "Controls.h"
#include "Headup Display.h"

#include "Spark.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "GCSKT.h"

CHardDamageCriticalArrow::CHardDamageCriticalArrow(void)
{
}

CHardDamageCriticalArrow::~CHardDamageCriticalArrow(void)
{
}

bool CHardDamageCriticalArrow::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if (pDamageInstance->m_What == DT_UP_ARROW ||
        pDamageInstance->m_What == DT_UP_ARROW_LIGHTNING ||
        pDamageInstance->m_What == DT_UP_ARROW_SHINING ||
		pDamageInstance->m_What == DT_UP_ARROW_LOVE ||
        pDamageInstance->m_What == DT_UP_ARROW_TIME ||
        pDamageInstance->m_What == DT_UP_ARROW_DARK_DRAGON_RED ||
        pDamageInstance->m_What == DT_UP_ARROW_DARK_DRAGON_BLUE ||
        pDamageInstance->m_What == DT_UP_ARROW_HALLOWEEN ||
        pDamageInstance->m_What == DT_UP_ARROW_ELIA ||
        pDamageInstance->m_What == DT_UP_ARROW_PEGASUS )
    {
        pDamageInstance->m_Life = 1;
        pDamageInstance->Add( HIT_LIGHT, ( pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->MyBody.z, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
        
        if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) )
        {
            PLAYER::DamageInfo damage;
            float fDamage = m_fDamageToPlayer;
            if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
            }
            damage.SetDamage( fDamage );
            damage.SetDamageInstanceAttacker( pDamageInstance );
            damage.SetWeakToShield( true );
            g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );

            g_MyD3D->MyPlayer[hitWho]->y_Speed += 0.012f;

            if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0.06f )
                g_MyD3D->MyPlayer[hitWho]->y_Speed = 0.06f;

            g_MyD3D->MyPlayer[hitWho]->NoCheckContact = 10;
            g_MyD3D->MyPlayer[hitWho]->x_Speed = 0;
            g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_COMMON_DAMAGED_BY_FLYATK );
            g_MyD3D->MyPlayer[hitWho]->cFrame = 0;
        }

		MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

        if( pDamageInstance->m_Who == MAX_PLAYER_NUM && pMonster != NULL )
        {
            //if( g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->Now_Monster == MON_SKELETON_ARCHER_LIGHTNING )
            //    g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.7f );
        }
        else
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.4f );

        return true;
    }

    pDamageInstance->m_Life = 1;
    if ((g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())) &&
        (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) &&
        (pDamageInstance->m_What != DT_UP_ARROW &&
            pDamageInstance->m_What != DT_UP_ARROW_LIGHTNING &&
            pDamageInstance->m_What != DT_UP_ARROW_SHINING &&
            pDamageInstance->m_What != DT_UP_ARROW_LOVE &&
            pDamageInstance->m_What != DT_DOWN_ARROW &&
            pDamageInstance->m_What != DT_DOWN_ARROW_DUN &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU &&
            pDamageInstance->m_What != DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LIGHTNING &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LIGHTNING_DUN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_SHINING &&
            pDamageInstance->m_What != DT_DOWN_ARROW_SHINING_DUN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LOVE &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LOVE_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_TIME &&
            pDamageInstance->m_What != DT_DOWN_ARROW_TIME &&
            pDamageInstance->m_What != DT_DOWN_ARROW_TIME_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_LIFE &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LIFE &&
            pDamageInstance->m_What != DT_DOWN_ARROW_LIFE_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_DARK_DRAGON_RED &&
            pDamageInstance->m_What != DT_DOWN_ARROW_DARK_DRAGON_RED &&
            pDamageInstance->m_What != DT_DOWN_ARROW_DARK_DRAGON_RED_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_DARK_DRAGON_BLUE &&
            pDamageInstance->m_What != DT_DOWN_ARROW_DARK_DRAGON_BLUE &&
            pDamageInstance->m_What != DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_HALLOWEEN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_HALLOWEEN_DUN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_ELIA &&
            pDamageInstance->m_What != DT_DOWN_ARROW_ELIA_DUN &&
            pDamageInstance->m_What != DT_DOWN_ARROW_PEGASUS &&
            pDamageInstance->m_What != DT_DOWN_ARROW_PEGASUS_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_COLISEU &&
            pDamageInstance->m_What != DT_DOWN_ARROW_COLISEU &&
            pDamageInstance->m_What != DT_DOWN_ARROW_COLISEU_DUN &&
            pDamageInstance->m_What != DT_UP_ARROW_C_MASTER &&
            pDamageInstance->m_What != DT_DOWN_ARROW_C_MASTER &&
            pDamageInstance->m_What != DT_DOWN_ARROW_C_MASTER_DUN))

    {
        if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_ELESIS || 
            ( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV1 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV2 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV1 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV2 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_DIO4_WEAPON_PASSIVE_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_EDEL1_BERSERKER ) ) )
        {
			if( pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LIGHTNING ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_SHINING ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LOVE ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DEATH ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_TIME ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_HALLOWEEN ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_ELIA ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_PEGASUS )
			{
				g_MyD3D->MyHead->SuperPoint += 0.0f;
			}
			else if( IsBolt( pDamageInstance->m_What ) )
				g_MyD3D->MyHead->SuperPoint += 0.0f;
			else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) )
				g_MyD3D->MyHead->SuperPoint += 0.40f;
			else
				g_MyD3D->MyHead->SuperPoint += 0.435f;

			if( g_MyD3D->MyHead->SuperPoint > 1.0f )
			{
				g_MyD3D->MyHead->SuperPoint = 0;

				if( g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_NORMAL )
				{
					bool bModChange = true;
					if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_ELESIS )
					{
						g_KDSound.Play( "133" );
						g_MyD3D->MyHead->Start_Text( "BERSERK" );
						g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_ELESIS123_BERSERK );     
					}
					else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV1 ) ||
						g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV2 ) ||
						g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV3 ) )
					{
						g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY );
					}
					else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV1 ) )
						g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_RONAN_SKILL_SWORD_EYE_LV1 );
					else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV2 ) )
						g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_RONAN_SKILL_SWORD_EYE_LV2 );
					else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV3 ) )
						g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_RONAN_SKILL_SWORD_EYE_LV3 );
                    else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_SIEG1_BERSERK_LV1 ) ||
                        g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_SIEG1_BERSERK_LV2 ) )
                    {
                        g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_SIEG1_SKILL_BERSERK );                        
                    }
                    else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_DIO4_WEAPON_PASSIVE_LV3 ) ) 
                    {
                        g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_DIO4_PASSIVE_LV3_BERSERK );
                    } else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_EDEL1_BERSERKER ) ) {
                        g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_EDEL1_BERSERKER );
                    } else {
						bModChange = false;
                    }

					if(bModChange){
						g_MyD3D->MyPlayer[hitWho]->Super = 55;
						g_MyD3D->MyPlayer[hitWho]->cFrame = 0;
						//g_MyD3D->MyPlayer[hitWho]->bIsRight = true;
						goto SKIPMOTIONPROCESS;
					}
				}				
			}
		}
    }   

    if( pDamageInstance->m_IsRight )
    {
        if( IsArrow( pDamageInstance->m_What ) ||
            IsBolt( pDamageInstance->m_What ) ||
            IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
            pDamageInstance->m_What == DT_SKULL_CRITICAL_ARROW )
            g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.005f;
        else if( IsHugeArrow( pDamageInstance->m_What ) )
            g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.007f;
    }
    else 
    {
        if( IsArrow( pDamageInstance->m_What ) ||
            IsBolt( pDamageInstance->m_What ) ||
            IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
            pDamageInstance->m_What == DT_SKULL_CRITICAL_ARROW )
            g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.005f;
        else if( IsHugeArrow( pDamageInstance->m_What ) )
            g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.007f;
    }

	if( g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD ) 
		|| g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD_NOEFFECT )  )
	{
		goto SKIPMOTIONPROCESS;
	}

    if( g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION &&
        pDamageInstance->m_What != DT_HARPY_DOWN )
    {
        pDamageInstance->m_Who      = hitWho;
        pDamageInstance->m_IsRight  = !pDamageInstance->m_IsRight;
        pDamageInstance->m_Life     = m_Start_Life;
        pDamageInstance->m_iTeam    = g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.iTeam;
        return true;
    }
    else
    {
		BOOL ENABLE_ARROWDEF = g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ENABLE_ARROWDEF;

		if ( g_MyD3D->MyPlayer[hitWho]->m_bHyperArmor )
		{
			ENABLE_ARROWDEF = FALSE;
		}

        if( pDamageInstance->m_What != DT_HARPY_DOWN &&
            ENABLE_ARROWDEF &&
            ( g_MyD3D->MyCtrl->k_Right||g_MyD3D->MyCtrl->k_Left||!g_MyD3D->MyPlayer[hitWho]->IsContact) &&
            ( g_MyD3D->MyPlayer[hitWho]->GetHP() > -0.05f ) &&
            ( GC_GM_TUTORIAL != SiKGCRoomManager()->GetGameMode() ) )
        {
            if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
            {
                if( g_MyD3D->MyPlayer[hitWho]->IsContact )
                {
                    if( IsArrow( pDamageInstance->m_What ) ) 
                        fDamageRate = 0.15f;
                    else if( IsBolt( pDamageInstance->m_What ) )
                        fDamageRate = 0.2f;
                    else if( IsHugeArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.2f;
                    else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LIGHTNING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_SHINING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LOVE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DEATH ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_TIME ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_HALLOWEEN ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_ELIA ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_PEGASUS )
                        fDamageRate = 0.15f;
                    else if( pDamageInstance->m_What == DT_SKULL_CRITICAL_ARROW  )
                        fDamageRate = 0.0f;

                    g_MyD3D->MyCtrl->k_Right = false;
                    g_MyD3D->MyCtrl->k_Left = false;

                    if( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION != -1 
                        && rand() % 100 + 1 < g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().iRefelectionRate )
                    {
                        g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
                    }
                    else 
                        g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND2, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
                }
                else // 공중에서 막기
                {
                    if( IsArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.3f;
                    else if( IsBolt( pDamageInstance->m_What ) )
                        fDamageRate = 0.4f;
                    else if( IsHugeArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.4f;
                    else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LIGHTNING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_SHINING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_LOVE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DEATH || 
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_TIME ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_HALLOWEEN ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_ELIA ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_ARROW_PEGASUS )
                        fDamageRate = 0.3f;
                    else if( pDamageInstance->m_What == DT_SKULL_CRITICAL_ARROW  )
                        fDamageRate = 0.0f;

                    g_MyD3D->MyCtrl->k_Right = false;
                    g_MyD3D->MyCtrl->k_Left = false;

                    // 화살 반사는 안됨
                    g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_SKY, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                }

SKIPMOTIONPROCESS:
                PLAYER::DamageInfo damage;
                float fDamage = pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer * fDamageRate ;
                if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                    fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                }
                damage.SetDamage( fDamage );
                damage.SetDamageInstanceAttacker( pDamageInstance );
                damage.SetWeakToShield( true );
                g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );

                //if( pDamageInstance->m_Who == MAX_PLAYER_NUM && g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum] != NULL )
                //{
                //    if( g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->Now_Monster == MON_SKELETON_ARCHER_LIGHTNING )
                //        g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.7f );
                //}
                //else
                    g_MyD3D->MyPlayer[hitWho]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.4f );

                g_KDSound.Play( "34" );
            }
        }
        else
        {
            return CDamage::CrashPlayer(pDamageInstance,hitWho,fDamageRate);
        }
    }
    return true;
}

void CHardDamageCriticalArrow::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
    if( pDamageInstance->m_ExtraMonsterNum == Monsteri )
        return;

    //ONEHIT데미지라면 한번만 맞게 한다.
	if ( pDamageInstance->InsertAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ) == false ) {
		return;
	}

    //if( pDamageInstance->m_bAlreadyDamaged[ Monsteri + MAX_PLAYER_NUM ] &&
    //    ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE ) )
    //{
    //    return;
    //}
    //else
    //{
    //    pDamageInstance->m_bAlreadyDamaged[ Monsteri + MAX_PLAYER_NUM ] = true;
    //}

    const bool bSpecialPoint = ( DT_GAS != pDamageInstance->m_What );     // 스페셜 포인트(콤보, 공격점수<던전용>) 사용 여부
    // 가스는 연타가 많아서 안쓴다.

    bool UsePower = true;
	if( m_fDamageToMonster != 0.0f ){
		MONSTER* pMonster = g_kMonsterManager.GetMonster(Monsteri);
		if ( pMonster ) {

            // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
            pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
            pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
#if defined( LEVEL_DESIGN_STAT_CALC )
			UsePower = pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What, m_fDamageToMonster * -1.0f,
			(EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
			m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack, 
			pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, bSpecialPoint, true );
#else
			UsePower = pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What, m_fDamageToMonster * -1000.0f,
			(EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
            m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack,
			pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, bSpecialPoint,true );
#endif
		}
	}
    if ( ( m_dwDMAttribute & DA_ONETIME_DAMAGE ) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

    // 07/09/2007. daeuk. 저사양이고 DA_SHOW_DAMAGE_EFFECT 면 기타 이펙트는 안붙여도 될듯 하다. 이미 데미지 이펙트 자체로 부하다
    if( g_MyD3D->m_KGCOption.GetQualityCount() >= 2 || !(m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT) )
    {
        if ( m_dwDMAttribute & DA_HIT_LIGHT )
        {
            pDamageInstance->Add( HIT_LIGHT, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
        }

        if ( m_dwDMAttribute & DA_PARTICLES )
        {
            if( m_vecSpark.size() == 0 )
            {
                g_MyD3D->MySparks->Particles( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 10 + pDamageInstance->m_Life / 40, 15, pDamageInstance->m_Who );
            }
            else
            {
                for( int i = 0 ; i < (int)m_vecSpark.size() ; ++i )
                {
                    switch( m_vecSpark[i].first )
                    {
                    case HE_RANDOM_DIRECTION_SPARK:
                        g_MyD3D->MySparks->Particles( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 10 + pDamageInstance->m_Life / 40, 15, pDamageInstance->m_Who, m_vecSpark[i].second );
                        break;
                    case HE_HIT_EFFECT_SPARK:
                        pDamageInstance->Add(  m_vecSpark[i].second , pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
                        break;
                    case HE_HIT_EFFECT_BIM:
                        g_MyD3D->MySparks->Bim(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, m_vecSpark[i].second);
                        break;
                    }
                }
            }
        }

        if ( m_dwDMAttribute & DA_FIRES )
        {
            g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
        }
    }

    if ( m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT )
    {
		if( pDamageInstance->m_IsMonsterDamage ) {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if ( pMonster ) {
			   pMonster->SetDamageEff( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
			}
		}
		else
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
    }

    if( m_dwDMAttribute & DA_SLOW_MOTION )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count = 8;
    }

    if( m_dwDMAttribute & DA_PHYSICAL )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = 21;
    }


    int iTemp = m_iCrashSound;
    char strTemp[50];
    sprintf( strTemp , "%d" , iTemp);
    g_KDSound.Play( strTemp );
}

bool CHardDamageCriticalArrow::IsArrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_CRITICAL_ARROW:
        case DT_CRITICAL_ARROW_LIGHTNING:
        case DT_CRITICAL_ARROW_SHINING:
		case DT_CRITICAL_ARROW_LOVE:
        case DT_CRITICAL_ARROW_TIME:
        case DT_CRITICAL_ARROW_DARK_DRAGON_RED:
        case DT_CRITICAL_ARROW_DARK_DRAGON_BLUE:
        case DT_CRITICAL_ARROW_HALLOWEEN:
        case DT_CRITICAL_ARROW_ELIA:
        case DT_CRITICAL_ARROW_PEGASUS:
            return true;
    }
    return false;
}

bool CHardDamageCriticalArrow::IsBolt( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_CRITICAL_BOLT:
        case DT_CRITICAL_BOLT_LIGHTNING:
        case DT_CRITICAL_BOLT_SHINING:
		case DT_CRITICAL_BOLT_LOVE:
        case DT_CRITICAL_BOLT_TIME:
        case DT_CRITICAL_BOLT_DARK_DRAGON_RED:
        case DT_CRITICAL_BOLT_DARK_DRAGON_BLUE:
        case DT_CRITICAL_BOLT_HALLOWEEN:
        case DT_CRITICAL_BOLT_ELIA:
        case DT_CRITICAL_BOLT_PEGASUS:
            return true;
    }
    return false;
}

bool CHardDamageCriticalArrow::IsHugeArrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_HUGE_CRITICAL_ARROW:
        case DT_HUGE_CRITICAL_ARROW_LIGHTNING:
        case DT_HUGE_CRITICAL_ARROW_SHINING:
		case DT_HUGE_CRITICAL_ARROW_LOVE:
        case DT_HUGE_CRITICAL_ARROW_TIME:
        case DT_HUGE_COMBO3_ARROW:
        case DT_HUGE_COMBO3_ARROW_LIGHTNING:
        case DT_HUGE_COMBO3_ARROW_SHINING:
		case DT_HUGE_COMBO3_ARROW_LOVE:
        case DT_HUGE_COMBO3_ARROW_TIME:
        case DT_HUGE_RUNATK_ARROW:
        case DT_HUGE_RUNATK_ARROW_LIGHTNING:
        case DT_HUGE_RUNATK_ARROW_SHINING:
		case DT_HUGE_RUNATK_ARROW_LOVE:
        case DT_HUGE_RUNATK_ARROW_TIME:

        case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_RED:
        case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_RED:
        case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_RED:

		case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_BLUE:
		case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_BLUE:
		case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_BLUE:

        case DT_HUGE_CRITICAL_ARROW_HALLOWEEN:
        case DT_HUGE_COMBO3_ARROW_HALLOWEEN:
        case DT_HUGE_RUNATK_ARROW_HALLOWEEN:

        case DT_HUGE_CRITICAL_ARROW_ELIA:
        case DT_HUGE_COMBO3_ARROW_ELIA:
        case DT_HUGE_RUNATK_ARROW_ELIA:

        case DT_HUGE_CRITICAL_ARROW_PEGASUS:
        case DT_HUGE_COMBO3_ARROW_PEGASUS:
        case DT_HUGE_RUNATK_ARROW_PEGASUS:

            return true;
    }
    return false;
}

bool CHardDamageCriticalArrow::IsNovaBolt_Arrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_NOVA_CRITICAL_BOLT:
        case DT_NOVA_CRITICAL_ARROW:
        case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
        case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
        case DT_NOVA_CRITICAL_BOLT_SHINING:
        case DT_NOVA_CRITICAL_ARROW_SHINING:
        case DT_NOVA_CRITICAL_BOLT_DARK:
        case DT_NOVA_CRITICAL_ARROW_DARK:
		case DT_NOVA_CRITICAL_BOLT_LOVE:
		case DT_NOVA_CRITICAL_ARROW_LOVE:
        case DT_NOVA_CRITICAL_BOLT_DEATH:
        case DT_NOVA_CRITICAL_ARROW_DEATH:
        case DT_NOVA_CRITICAL_ARROW_TIME:
        case DT_NOVA_CRITICAL_BOLT_TIME:
		case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
		case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:
		case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
		case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:
        case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
        case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:
        case DT_NOVA_CRITICAL_ARROW_ELIA:
        case DT_NOVA_CRITICAL_BOLT_ELIA:
        case DT_NOVA_CRITICAL_ARROW_PEGASUS:
        case DT_NOVA_CRITICAL_BOLT_PEGASUS:
            return true;
    }
    return false;
}



void CHardDamageCriticalArrow::CustomizeBeforeCreate( CDamageInstance* pDamageInstance )
{
	if( pDamageInstance->m_IsMonsterDamage )
		return;

	if(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_Elemental == ITEM_ELEMENTAL_ROSE
		&& g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_StrongLevel >= 8 )
	{
		if( pDamageInstance->m_pOriginalDamage->m_particleSeqName == "loveItem_arrow01" )
			pDamageInstance->m_pOriginalDamage->m_particleSeqName = "Rozen_Arrow";
	}

}