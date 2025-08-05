#include "stdafx.h"
#include ".\HardDamageArrow.h"
#include "Controls.h"
#include "Headup Display.h"

#include "Spark.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "GCSKT.h"
#include "LatencyTransfer.h"
#include "Buff/KGCBuffManager.h"

static bool bChangeSuper = false;
static unsigned char l_o = 0;

CHardDamageArrow::CHardDamageArrow(void)
{
    bChangeSuper = false;
}

CHardDamageArrow::~CHardDamageArrow(void)
{
}

bool CHardDamageArrow::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    bool bDamage = true;

	if ( pDamageInstance->InsertAlreadyDamaged(hitWho) == false ) {
		return false;
	}

	if ( ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE ) && pDamageInstance->m_Life > 1)
	{
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
		pDamageInstance->m_Life = 1;
	}
	
#if defined( LEVEL_DESIGN_STAT_CALC )
    if( pDamageInstance->m_IsMonsterDamage )
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage( hitWho, pDamageInstance->m_ExtraMonsterNum, m_fDamageToPlayer * fDamageRate * 10000.0f);
    else
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage( hitWho, pDamageInstance->m_Who, m_fDamageToPlayer * fDamageRate, pDamageInstance->m_bPetDamage );
#else
    // 데미지 반사
    if( pDamageInstance->m_IsMonsterDamage )
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage( hitWho, pDamageInstance->m_ExtraMonsterNum, m_fDamageToPlayer * fDamageRate * 100.0f);
    else
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage( hitWho, pDamageInstance->m_Who, m_fDamageToPlayer * fDamageRate, pDamageInstance->m_bPetDamage );
#endif

	if ( g_MyD3D->IsPlayerIndex( hitWho ) && !g_MyD3D->MyPlayer[hitWho]->OnDamagePreProcess( pDamageInstance->m_IsMonsterDamage ? pDamageInstance->m_ExtraMonsterNum : pDamageInstance->m_Who,false, pDamageInstance->m_IsMonsterDamage, 
        pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, -1, hitWho ) )
		return false;

    if( pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() && hitWho == g_MyD3D->Get_MyPlayer() && 
        ( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_RONAN3 || g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurrentChar().iCharType == GC_CHAR_LIME ||
        ( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_SIEG && g_MyD3D->MyPlayer[hitWho]->CheckSkill(SID_SIEG1_FURY_TYPE_B_LV3))) && 
        g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_NORMAL && g_MyD3D->MyPlayer[hitWho]->IsDamagedMotion() == false )
    {
        if( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false )
        {
            int i = g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME;
            // 론한2촤 블로킹!!
            if ( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight == false )
            {
                for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                {
                    if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][i] ) break;
                }
            }
            else
            {
                for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                {
                    if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][i] ) break;
                }
            }

            if ( i != g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME )
            {
                PLAYER* pMyPlayer = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()];
                if( pMyPlayer ) {
                    if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_SIEG ) {
                        g_MyD3D->MyPlayer[hitWho]->SendBurnningPoint(hitWho);
                        if( g_MyD3D->MyPlayer[hitWho]->GetBurnningPoint() >= 0.5f ) {
                            if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG4 ) {
                                pMyPlayer->Direct_Motion_Input( MID_SIEG4_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                            } else if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG3 ) {
                                pMyPlayer->Direct_Motion_Input( MID_SIEG3_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                            } else if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG2 ) {
                                pMyPlayer->Direct_Motion_Input( MID_SIEG2_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                            } else {
                                pMyPlayer->Direct_Motion_Input( MID_SIEG1_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                            }
                        }
                    } else if( pMyPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ) { 
                        if( pMyPlayer->IsMagicEffect( EGC_EFFECT_LIME2_SELF_AURA_DEFFENCE ) && pMyPlayer->CheckSkill( SID_LIME2_SELF_AURA_DEFFENSE_PLUS_LV3 ) ) {
                            if( pMyPlayer->GetMP() >= 0.3f ) {
                                pMyPlayer->Direct_Motion_Input( MID_LIME2_COUNTER_ATK );
                            }
                        }
                    } else {
						if(pMyPlayer->IsMagicEffect(EGC_EFFECT_RONAN3_BLOCK_COOL) == false )
						{
							pMyPlayer->Direct_Motion_Input(MID_RONAN3_BLOCKING, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
						}
                    }
                    return false;
                }                
            }
        }
    }    

    if( g_MyD3D->IsPlayerIndex( hitWho ) && ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) ) 
        && !bChangeSuper && pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false )
    {
		if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_ELESIS ||
			( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV1 ) ||
			g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV2 ) ||
			g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_ARME1_SPIRIT_RECOVERY_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV1 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV2 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_RONAN1_SWORD_EYE_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_SIEG1_BERSERK_LV1 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_SIEG1_BERSERK_LV2 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_DIO4_WEAPON_PASSIVE_LV3 ) ||
            g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_EDEL1_BERSERKER ) ) )
		{
			// 버서크 게이지 설정	
			if( pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_SHINING ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LOVE ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_LOVE ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_SHINNING ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_LIGHTNING ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DEATH ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_TIME ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_TIME ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_LIFE ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIFE ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_CONTINUOUS_ARROW_DARK_DRAGON_BLUE ||
				pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_HALLOWEEN ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_ELIA ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_PEGASUS ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_PEGASUS ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_C_MASTER ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_C_MASTER ||
                pDamageInstance->m_What == DT_CONTINUOUS_ARROW_COLISEU ||
                pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_COLISEU ||
				pDamageInstance->m_pOriginalDamage->m_bSpecialAttack )
				g_MyD3D->MyHead->SuperPoint += 0.0f;
			else if( pDamageInstance->m_What == DT_SKULL_ARROW )
				g_MyD3D->MyHead->SuperPoint += 0.23f;
			else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) )
				g_MyD3D->MyHead->SuperPoint += 0.40f;
			else
				g_MyD3D->MyHead->SuperPoint += 0.435f;

			if( g_MyD3D->MyHead->SuperPoint > 1.0f )
			{
				g_MyD3D->MyHead->SuperPoint = 0;
				
                if( g_MyD3D->IsPlayerIndex( hitWho ) && g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_NORMAL )
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
                   }
                   else if( g_kGlobalValue.CheckTrainedSkill( (g_MyD3D->MyPlayer[hitWho]), SID_EDEL1_BERSERKER ) ) {
                        g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_EDEL1_BERSERKER );
                   } else {
					   bModChange = false;
                   }

				   if(bModChange){
					   g_MyD3D->MyPlayer[hitWho]->Super = 55;
					   g_MyD3D->MyPlayer[hitWho]->cFrame = 0;
					   g_MyD3D->MyPlayer[hitWho]->bIsRight = true;
					   bChangeSuper = true;
					   goto SKIPMOTIONPROCESS;
				   }
                }				
			}
		}
    }
    g_MyD3D->MyPlayer[hitWho]->x_Speed = 0;
    pDamageInstance->Add( HIT_LIGHT,
                            ( pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f,
                            g_MyD3D->MyPlayer[hitWho]->vPos.y,
                            0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who
							);

    g_MyD3D->MyPlayer[hitWho]->bIsRight = !pDamageInstance->m_IsRight;

	if( g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD ) 
		|| g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD_NOEFFECT )  )
	{
		goto SKIPMOTIONPROCESS;
	}

    if( g_MyD3D->MyPlayer[hitWho]->uiMotion == g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION )
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

        if( ENABLE_ARROWDEF &&
            ( g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left || !g_MyD3D->MyPlayer[hitWho]->IsContact )
            && ( GC_GM_TUTORIAL != SiKGCRoomManager()->GetGameMode() ) )
        {
            if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
            {
                if( g_MyD3D->MyPlayer[hitWho]->IsContact )
                {
                    // 디펜스 때 화살들은 데미지 0.15배
                    if( IsArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.25f; //0.15f;
                    // 디펜스 때 볼트들은 데미지 0.2배
                    else if( IsBolt( pDamageInstance->m_What ) )
                        fDamageRate = 0.3f; //0.2
                    // 디펜스 때 대궁들은 데미지 0.2배
                    else if( IsHugeArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.3f; //0.2
                    else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_SHINING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DEATH ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LOVE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_TIME ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIFE ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_PEGASUS )

                        fDamageRate = 0.25f;
                    else if( pDamageInstance->m_What == DT_SKULL_ARROW )
                        fDamageRate = 0.0f;

                    g_MyD3D->MyCtrl->k_Right = false;
                    g_MyD3D->MyCtrl->k_Left = false;

                    if( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION != -1 &&
                        rand() % 100 + 1 < g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().iRefelectionRate )
                    {
                        g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROW_REFLECTION, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                    }
                    else
                    {
                        ++l_o;
                        if( l_o > 1 )
                        {
                            l_o = 0;
                            g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND1, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                        }
                        else 
                            g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND2, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                    }
                }
                else 
                {
                    if( IsArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.3f;
                    else if( IsBolt( pDamageInstance->m_What ) )
                        fDamageRate = 0.4f;
                    else if( IsHugeArrow( pDamageInstance->m_What ) )
                        fDamageRate = 0.4f;
                    else if( IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_SHINING ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DEATH ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LOVE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_TIME ||
						pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_LIFE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                        pDamageInstance->m_What == DT_NOVA_SPECIAL1_BOLT_PEGASUS )
                        fDamageRate = 0.3f;
                    else if( pDamageInstance->m_What == DT_SKULL_ARROW )
                        fDamageRate = 0.0f;

                    g_MyD3D->MyCtrl->k_Right = false;
                    g_MyD3D->MyCtrl->k_Left = false;

                    // 화살 반사는 안됨
                    g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_SKY, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                }
                PLAYER::DamageInfo damage;
                float fDamage = m_fDamageToPlayer * fDamageRate;
                if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                    fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                }
                damage.SetDamage( fDamage );
                damage.SetWeakToShield( true ); /*2007.08.22 wony 실드에 약함*/
                damage.SetDamageInstance( pDamageInstance );
                damage.SetSpecialAttack(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
            }
        }
        else
        {
SKIPMOTIONPROCESS:
            g_MyD3D->MySparks->Particles( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 10 + pDamageInstance->m_Life / 40, 15, pDamageInstance->m_Who );

            if( IsBolt( pDamageInstance->m_What ) ||
                IsHugeArrow( pDamageInstance->m_What ) ||
                pDamageInstance->m_What == DT_SKULL_ARROW )
            {
                if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_LIRE ) {
                    float fDamage = m_fDamageToPlayer * 1.11f;
                    if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                        fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                    }
                    PLAYER::DamageInfo damage;
                    damage.SetDamage( fDamage );
                    damage.SetWeakToShield( true );
                    damage.SetDamageInstance( pDamageInstance );
                    damage.SetSpecialAttack(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                    g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );
                }
                else {
                    float fDamage = m_fDamageToPlayer;
                    if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                        fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                    }
                    PLAYER::DamageInfo damage;
                    damage.SetDamage( fDamage );
                    damage.SetWeakToShield( true );
                    damage.SetDamageInstance( pDamageInstance );
                    damage.SetSpecialAttack(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                    g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );
                }

            }
            else
            {
                if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_LIRE ) {
                    float fDamage = m_fDamageToPlayer * 0.98f;
                    if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                        fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                    }

                    PLAYER::DamageInfo damage;
                    damage.SetDamage( fDamage );
                    damage.SetWeakToShield( true );
                    damage.SetDamageInstance( pDamageInstance );
                    damage.SetSpecialAttack(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                    g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );
                }
                else {
                    PLAYER::DamageInfo damage;
                    float fDamage = m_fDamageToPlayer;
                    if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) {
                        fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
                    }
                    damage.SetDamage( fDamage );
                    damage.SetWeakToShield( true );
                    damage.SetDamageInstance( pDamageInstance );
                    damage.SetSpecialAttack(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                    g_MyD3D->MyPlayer[hitWho]->Change_HP( damage/*2007.08.22 wony 실드에 약함*/ );
                }
            }

            if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() ||
                ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) )
                g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( MID_COMMON_SMALLDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
        }
    }

	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

    if( pDamageInstance->m_Who == MAX_PLAYER_NUM && pMonster != NULL )
    {
        //if( g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->Now_Monster == MON_SKELETON_ARCHER_LIGHTNING )
        //    g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.7f );
    }
    else
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.4f );

    g_KDSound.Play( "34" );

    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() ||
        ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) )
    {
        if( pDamageInstance->m_IsRight ) 
        {
            if( IsArrow( pDamageInstance->m_What ) ||
                IsBolt( pDamageInstance->m_What ) ||
                IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                pDamageInstance->m_What == DT_SKULL_ARROW )
                g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.005f;
            else if( IsHugeArrow( pDamageInstance->m_What ) )
                g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.007f;
        }
        else 
        {
            if( IsArrow( pDamageInstance->m_What ) ||
                IsBolt( pDamageInstance->m_What ) ||
                IsNovaBolt_Arrow( pDamageInstance->m_What ) ||
                pDamageInstance->m_What == DT_SKULL_ARROW )
                g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.005f;
            else if( IsHugeArrow( pDamageInstance->m_What ) )
                g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.007f;
        }

        if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )        //점프 상태 해제
            g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;
    }
    return true;
}

bool CHardDamageArrow::IsArrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_ARROW:
        case DT_ARROW_LIGHTNING:
        case DT_ARROW_SHINING:
		case DT_ARROW_LOVE:
        case DT_ARROW_TIME:
		case DT_ARROW_LIFE:
        case DT_ARROW_DARK_DRAGON_RED:
        case DT_ARROW_DARK_DRAGON_BLUE:
        case DT_ARROW_HALLOWEEN:
        case DT_ARROW_ELIA:
        case DT_ARROW_PEGASUS:
        
        case DT_FREEWAY_ARROW:
        case DT_FREEWAY_ARROW_LIGHTNING:
        case DT_FREEWAY_ARROW_SHINING:
		case DT_FREEWAY_ARROW_LOVE:
        case DT_FREEWAY_ARROW_TIME:
		case DT_FREEWAY_ARROW_LIFE:
        case DT_FREEWAY_ARROW_DARK_DRAGON_RED:
        case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE:
        case DT_FREEWAY_ARROW_HALLOWEEN:
        case DT_FREEWAY_ARROW_PEGASUS:
        case DT_FREEWAY_ARROW_DUN:
        case DT_FREEWAY_ARROW_LIGHTNING_DUN:
        case DT_FREEWAY_ARROW_SHINING_DUN:
        case DT_FREEWAY_ARROW_LOVE_DUN:
        case DT_FREEWAY_ARROW_TIME_DUN:
        case DT_FREEWAY_ARROW_LIFE_DUN:
        case DT_FREEWAY_ARROW_DARK_DRAGON_RED_DUN:
        case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE_DUN:
        case DT_FREEWAY_ARROW_HALLOWEEN_DUN:
        case DT_FREEWAY_ARROW_PEGASUS_DUN:
        case DT_FREEWAY_ARROW_C_MASTER_DUN:
        case DT_FREEWAY_ARROW_COLISEU_DUN:
        
            return true;
    }
    return false;
}

bool CHardDamageArrow::IsBolt( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_BOLT:
        case DT_BOLT_LIGHTNING:
        case DT_BOLT_SHINING:
		case DT_BOLT_LOVE:
        case DT_BOLT_TIME:
		case DT_BOLT_LIFE:
        case DT_BOLT_DARK_DRAGON_RED:
        case DT_BOLT_DARK_DRAGON_BLUE:
        case DT_BOLT_HALLOWEEN:
        case DT_BOLT_ELIA:
        case DT_BOLT_PEGASUS:
            return true;
    }
    return false;
}

bool CHardDamageArrow::IsHugeArrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_HUGE_ARROW:
        case DT_HUGE_ARROW_LIGHTNING:
        case DT_HUGE_ARROW_SHINING:
		case DT_HUGE_ARROW_LOVE:
        case DT_HUGE_ARROW_TIME:
		case DT_HUGE_ARROW_LIFE:
        case DT_HUGE_JUMPDOWN_ARROW:
        case DT_HUGE_JUMPDOWN_ARROW_LIGHTNING:
        case DT_HUGE_JUMPDOWN_ARROW_SHINING:
		case DT_HUGE_JUMPDOWN_ARROW_LOVE:
        case DT_HUGE_JUMPDOWN_ARROW_TIME:
		case DT_HUGE_JUMPDOWN_ARROW_LIFE:
        case DT_HUGE_ARROW_DARK_DRAGON_RED:
        case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_RED:
		case DT_HUGE_ARROW_DARK_DRAGON_BLUE:
		case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_BLUE:
        case DT_HUGE_ARROW_HALLOWEEN:
        case DT_HUGE_JUMPDOWN_ARROW_HALLOWEEN:
        case DT_HUGE_ARROW_ELIA:
        case DT_HUGE_JUMPDOWN_ARROW_ELIA:
        case DT_HUGE_ARROW_PEGASUS:
        case DT_HUGE_JUMPDOWN_ARROW_PEGASUS:
            return true;
    }
    return false;
}

bool CHardDamageArrow::IsNovaBolt_Arrow( int iDamageType )
{
    switch( iDamageType )
    {
        case DT_NOVA_BOLT:
        case DT_NOVA_FREEWAY_BOLT:
        case DT_NOVA_FREEWAY_BOLT_DUN:
        case DT_NOVA_ARROW:
        case DT_NOVA_BOLT_LIGHTNING:
        case DT_NOVA_FREEWAY_BOLT_LIGHTNING:
        case DT_NOVA_FREEWAY_BOLT_LIGHTNING_DUN:
        case DT_NOVA_ARROW_LIGHTNING:
        case DT_NOVA_BOLT_SHINING:
        case DT_NOVA_FREEWAY_BOLT_SHINING:
        case DT_NOVA_FREEWAY_BOLT_SHINING_DUN:
        case DT_NOVA_ARROW_SHINING:
        case DT_NOVA_BOLT_DARK:
        case DT_NOVA_FREEWAY_BOLT_DARK:
        case DT_NOVA_FREEWAY_BOLT_DARK_DUN:
        case DT_NOVA_ARROW_DARK:
		case DT_NOVA_BOLT_LOVE:
		case DT_NOVA_FREEWAY_BOLT_LOVE:
        case DT_NOVA_FREEWAY_BOLT_LOVE_DUN:
		case DT_NOVA_ARROW_LOVE:
        case DT_NOVA_DOWN_ARROW:
        case DT_NOVA_DOWN_ARROW_LIGHTNING:
        case DT_NOVA_DOWN_ARROW_SHINING:
        case DT_NOVA_DOWN_ARROW_DARK:
		case DT_NOVA_DOWN_ARROW_LOVE:
        case DT_NOVA_BOLT_DEATH:
        case DT_NOVA_ARROW_DEATH:
        case DT_NOVA_FREEWAY_BOLT_DEATH:
        case DT_NOVA_FREEWAY_BOLT_DEATH_DUN:
        case DT_NOVA_DOWN_ARROW_DEATH:
        case DT_NOVA_BOLT_TIME:
        case DT_NOVA_ARROW_TIME:
        case DT_NOVA_FREEWAY_BOLT_TIME:
        case DT_NOVA_FREEWAY_BOLT_TIME_DUN:
		case DT_NOVA_BOLT_LIFE:
		case DT_NOVA_ARROW_LIFE:
		case DT_NOVA_FREEWAY_BOLT_LIFE:
        case DT_NOVA_FREEWAY_BOLT_LIFE_DUN:

		case DT_NOVA_BOLT_DARK_DRAGON_RED:
		case DT_NOVA_ARROW_DARK_DRAGON_RED:
		case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED:
        case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED_DUN:

		case DT_NOVA_BOLT_DARK_DRAGON_BLUE:
		case DT_NOVA_ARROW_DARK_DRAGON_BLUE:
		case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE:
        case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE_DUN:

        case DT_NOVA_BOLT_HALLOWEEN:
        case DT_NOVA_ARROW_HALLOWEEN:
        case DT_NOVA_FREEWAY_BOLT_HALLOWEEN:
        case DT_NOVA_FREEWAY_BOLT_HALLOWEEN_DUN:

        case DT_NOVA_BOLT_ELIA:
        case DT_NOVA_ARROW_ELIA:
        case DT_NOVA_FREEWAY_BOLT_ELIA:
        case DT_NOVA_FREEWAY_BOLT_ELIA_DUN:

        case DT_NOVA_BOLT_PEGASUS:
        case DT_NOVA_ARROW_PEGASUS:
        case DT_NOVA_FREEWAY_BOLT_PEGASUS:
        case DT_NOVA_FREEWAY_BOLT_PEGASUS_DUN:

        case DT_NOVA_BOLT_C_MASTER:
        case DT_NOVA_ARROW_C_MASTER:
        case DT_NOVA_FREEWAY_BOLT_C_MASTER:
        case DT_NOVA_FREEWAY_BOLT_C_MASTER_DUN:

        case DT_NOVA_BOLT_COLISEU:
        case DT_NOVA_ARROW_COLISEU:
        case DT_NOVA_FREEWAY_BOLT_COLISEU:
        case DT_NOVA_FREEWAY_BOLT_COLISEU_DUN:

            return true;
    }
    return false;
}

void CHardDamageArrow::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
    if( pDamageInstance->m_ExtraMonsterNum == Monsteri )
        return;

    //ONEHIT데미지라면 한번만 맞게 한다.
	if ( pDamageInstance->InsertAlreadyDamaged(Monsteri + MAX_PLAYER_NUM) == false )
		return;
    const bool bSpecialPoint = ( DT_GAS != pDamageInstance->m_What );     // 스페셜 포인트(콤보, 공격점수<던전용>) 사용 여부
    // 가스는 연타가 많아서 안쓴다.

    bool UsePower = true;
	if( m_fDamageToMonster != 0.0f ) {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(Monsteri);

		if ( pMonster ) {
            // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
            pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
            pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
#if defined( LEVEL_DESIGN_STAT_CALC )			
			UsePower = pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What, m_fDamageToMonster * -1.0f,
				(EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
				m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack, 
                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, bSpecialPoint,true );
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
		MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

		if( pDamageInstance->m_IsMonsterDamage && pMonster != NULL)
            pMonster->SetDamageEff( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
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

void CHardDamageArrow::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );

	//====================================================================================
	// 2009.02.22 : Jemitgge
	// Comment : 젠장.. 화살은 하나 만들면.. 수십개 더 만들어야 해서.. 하드코딩으로 
	//			 이거 배웠을때만 이 속성을 추가해주쟈!!

	PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
	if ( pPlayer->IsLocalPlayer() && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_LIRE1_HUNGRY_ARROW ) )
	{
		pDamageInstance->m_pOriginalDamage->m_dwDMAttribute += DA_CANCEL_MISSILE;
	}
}

void CHardDamageArrow::CustomizeBeforeCreate( CDamageInstance* pDamageInstance )
{
	if(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_Elemental == ITEM_ELEMENTAL_ROSE
		&& g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_StrongLevel >= 8 )
	{
		if( pDamageInstance->m_pOriginalDamage->m_particleSeqName == "loveItem_arrow01" )
			pDamageInstance->m_pOriginalDamage->m_particleSeqName = "Rozen_Arrow";
	}

}