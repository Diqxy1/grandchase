#include "stdafx.h"
#include "HardAI_Dota.h"
#include "../Monster.h"
#include "../Damage.h"

// 도타코어 시작================================================
void HardAI_Core::Action_Wait(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->GetInitAI() )
            {
                if( TEAM_SERDIN == pMonster->m_iTeam )
                {
                    pMonster->AddTraceParticleToBone( "dota_core_serdin", 1.0f, 0, 6, 6, 0.0f, -0.19f, false );
                    pMonster->AddTraceParticleToBone( "Dota_CoreR_01", 1.0f, 0, 6, 6, 0.04f, 0.15f, false );
                    pMonster->AddTraceParticleToBone( "Dota_CoreR_02", 1.0f, 0, 6, 6, 0.04f, 0.15f, false );
                    pMonster->AddTraceParticleToBone( "Dota_Core", 1.0f, 0, 6, 6, 0.04f, 0.15f, false );
                }
                else
                {
                    pMonster->AddTraceParticleToBone( "dota_core_canaban", 1.0f, 0, 6, 6, 0.0f, -0.19f, false );
                    pMonster->AddTraceParticleToBone( "Dota_CoreB_01", 1.0f, 0, 6, 6, -0.04f, 0.15f, false );
                    pMonster->AddTraceParticleToBone( "Dota_CoreB_02", 1.0f, 0, 6, 6, -0.04f, 0.15f, false );
                    pMonster->AddTraceParticleToBone( "Dota_Core", 1.0f, 0, 6, 6, -0.04f, 0.15f, false );
                }

                pMonster->SetInitAI( false );
            }

            if( (iFrame % 4) == 0 )
            {
                pMonster->AddDamage( DT_DOTA_CORE_HP_RECOVER );
                pMonster->AddDamage( DT_DOTA_CORE_HP_DRAIN );

                if( 0 == pMonster->m_iTeam )
                {
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_01 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_02 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_03 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_04 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_05 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_06 );
                }
                else
                {
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_01 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_02 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_03 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_04 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_05 );
                    pMonster->AddDamage( DT_DOTA_SKY_DMG_ZONE_07 );
                }
            }

            if( iFrame == 3 )
                pMonster->SetIsRight( true );
            else if( iFrame >= pMonster->GetLastFrame() )
                pMonster->SetState( "WAIT" );
            }
void HardAI_Core::Action_Die(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1)
                pMonster->SetInvincible( true );
            else if( iFrame == pMonster->GetLastFrame() - 1 )
            {
                if( 0 == pMonster->m_iTeam )
                {
                    pMonster->AddParticle("Dota_CoreR_Boom_01", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_02", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_03", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreR_Boom_04", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreR_Boom_05", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreR_Boom_06", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreR_Boom_07", 0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreR_Boom_08", 0.03f, 0.27f );
                }
                else
                {
                    pMonster->AddParticle("Dota_CoreB_Boom_01", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_02", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_03", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_04", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_05", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_06", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_07", -0.03f, 0.27f );
                    pMonster->AddParticle("Dota_CoreB_Boom_08", -0.03f, 0.27f );
                }
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->EndMonster( false );
                pMonster->SetFrameLock( true );
            }
    }

void HardAI_Core::CallBack_Die(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetState( "DIE" );
        }

void HardAI_Core::CallBack_Damage(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetSpeedX( 0.0f );
            pMonster->SetSpeedY( 0.0f );
            pMonster->SetIsRight( true );
        }
// 도타코어 끝================================================

// 고대병기 시작================================================
void HardAI_Ancient::Action_WaitFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame == 15 )
            {
                pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WALK" );
                else
                    pMonster->SetState( "WALK_R" );
            }
        }
void HardAI_Ancient::Action_WalkFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( ( pMonster->IsTargetMonsterType( MON_DOTA_GUARD_TOWER ) ||
                pMonster->IsTargetMonsterType( MON_DOTA_GUARDIAN_TOWER ) ||
                pMonster->IsTargetMonsterType( MON_DOTA_CORE ) ) &&
                pMonster->IsInvincible() == false &&
                pMonster->CheckTargetInRange( 0, -60, 10, 60, -10 ) == true )
            {
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "ATTACK02" );
                else
                    pMonster->SetState( "ATTACK02_R" );
            }

            pMonster->SetSpeedX( pMonster->GetRunSpeed() );

            if( iFrame >= pMonster->GetLastFrame() )
            {
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WAIT" );
                else
                    pMonster->SetState( "WAIT_R" );
            }
        }
void HardAI_Ancient::Ancient_DieFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1 )
                pMonster->SetInvincible( true );
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->EndMonster(false, true);
                pMonster->SetFrameLock( true );
            }
        }
void HardAI_Ancient::Ancient_Attack01Func(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame == 40 )
            {
                pMonster->PlaySound("984026");
                if( 0 == pMonster->m_iTeam )
                    pMonster->AddDamage( DT_DOTA_ANCIENT_SERDIN_TARGET );
                else
                    pMonster->AddDamage( DT_DOTA_ANCIENT_CANABAN_TARGET );
            }
            else if( iFrame == 43 )
            {
                if( 0 == pMonster->m_iTeam )
                {
                    if( pMonster->GetIsRight() )
                    {
                        float fPosX = 0.16f;
                        float fPosY = 0.40f;
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_01", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_02", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_03", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_04", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_05", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_06", fPosX, fPosY);
                    }
                    else
                    {
                        float fPosX = -0.16f;
                        float fPosY = 0.40f;
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_01_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_02_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_03",   fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_04_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_05_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterR_Beam_06_R", fPosX, fPosY);
                    }
                }
                else
                {
                    if( pMonster->GetIsRight() )
                    {
                        float fPosX = 0.16f;
                        float fPosY = 0.40f;
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_01", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_02", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_03", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_04", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_05", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_06", fPosX, fPosY);
                    }
                    else
                    {
                        float fPosX = -0.16f;
                        float fPosY = 0.40f;
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_01_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_02_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_03",   fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_04_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_05_R", fPosX, fPosY);
                        pMonster->AddParticleNoDirection("Dota_MonsterB_Beam_06_R", fPosX, fPosY);
                    }
                }
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->ResetDelay();
                if( false == pMonster->IsTarget() )
                    pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );

                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WAIT" );
                else
                    pMonster->SetState( "WAIT_R" );
            }
    }
void HardAI_Ancient::Ancient_Attack02Func(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame == pMonster->GetLastFrame() - 1 )
            {
                pMonster->PlaySound("984027");
                pMonster->AddDamage( DT_DOTA_ANCIENT_BOOM );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                if( false == pMonster->IsTarget() )
                    pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
                pMonster->EndMonster(false, true);
            }
        }
void HardAI_Ancient::Ancient_JumpDownFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame >= pMonster->GetLastFrame() )
                pMonster->SetFrameLock( true );
        }
void HardAI_Ancient::Ancient_JumpToWait(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->GetIsContact() == true )
            {
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WALK" );
                else
                    pMonster->SetState( "WALK_R" );
            }
        }
void HardAI_Ancient::Ancient_StoneCurseFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1 )
            {
                pMonster->AddTraceParticle( "StoneCurse_small", 3 );
                pMonster->AddParticle( "StoneCurse_small_smog01", 0.0f, 0.0f );
                pMonster->AddParticle( "StoneCurse_small_tick", 0.0f, 0.0f );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->AddParticle( "StoneCurse_small_smog01", 0.0f, 0.0f );
                pMonster->AddParticle( "StoneCurse_small_tick", 0.0f, 0.0f );
                pMonster->SetInvincible( false );
                pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WAIT" );
                else
                    pMonster->SetState( "WAIT_R" );

                if(  pMonster->GetHP() <= 0  )
                {
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "DIE" );
                    else
                        pMonster->SetState( "DIE_R" );
                }
            }
        }
void HardAI_Ancient::Ancient_IceCurseFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1 )
            {
                pMonster->AddTraceParticle( "IceCurse_small", 3 );
                pMonster->AddParticle( "IceCurse_small_smog01", 0.0f, 0.0f );
                pMonster->AddParticle( "IceCurse_small_tick", 0.0f, 0.0f );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->AddParticle( "IceCurse_small_smog01", 0.0f, 0.0f );
                pMonster->AddParticle( "IceCurse_small_tick", 0.0f, 0.0f );
                pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WAIT" );
                else
                    pMonster->SetState( "WAIT_R" );

                if( pMonster->GetHP() <= 0 )
                {
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "DIE" );
                    else
                        pMonster->SetState( "DIE_R" );
                }
            }
        }
void HardAI_Ancient::Ancient_HoldFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1 )
            {
                pMonster->AddParticle( "Arme01_Hold_Beam_effect_01_Small", 0.0f, 0.12f );
                pMonster->AddParticle( "Arme01_Hold_Beam_effect_02_Small", 0.0f, 0.12f );
                pMonster->AddParticle( "Arme01_Hold_Beam_effect_03_Small", 0.0f, 0.12f );
                pMonster->AddParticle( "Arme01_Hold_Beam_effect_04_Small", 0.0f, 0.12f );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "WAIT" );
                else
                    pMonster->SetState( "WAIT_R" );

                if( pMonster->GetHP() <= 0 )
                {
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "DIE" );
                    else
                        pMonster->SetState( "DIE_R" );
                }
            }
        }
void HardAI_Ancient::Trigger_Check_Targeting(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->SetTarget( 0, -300, 100, 300, -100 ) == true )
            {
                pMonster->TurnToTarget();
                pMonster->ResetDelay();
            }
        }
void HardAI_Ancient::Trigger_Check_Attack01(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->CheckTargetInRange( 50, -150, 10, 150, -10 ) == true &&
                pMonster->GetTargetIsContact() == true )
            {
                if( pMonster->IsTarget() == true )
                {
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "ATTACK01" );
                    else
                        pMonster->SetState( "ATTACK01_R" );
                }
            }
        }
void HardAI_Ancient::Trigger_Check_Attack02(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->CheckTargetInRange( 0, -50, 100, 50, -100 ) == true &&
                pMonster->IsTargetMonster() == true )
            {
                if( pMonster->IsTargetMonsterType( MON_DOTA_GUARD_TOWER ) ||
                    pMonster->IsTargetMonsterType( MON_DOTA_GUARDIAN_TOWER ) ||
                    pMonster->IsTargetMonsterType( MON_DOTA_CORE ) )
                {
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "ATTACK02" );
                    else
                        pMonster->SetState( "ATTACK02_R" );
                    pMonster->ResetDelay();
                }
            }
        }
void HardAI_Ancient::Trigger_Check_Attack03(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->CheckTargetInRange( -50, -100, 10, 100, -10 ) == true && 
                pMonster->GetTargetIsContact() == true )
            {
                if( pMonster->IsTarget() == true )
                {
                    pMonster->TurnToTarget();
                    if( pMonster->GetIsRight() == true )
                        pMonster->SetState( "ATTACK01" );
                    else
                        pMonster->SetState( "ATTACK01_R" );
                }
            }
        }
void HardAI_Ancient::CallBack_Land(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetSpeedX( pMonster->GetSpeedX() * (2.0f / 3.0f) );

            if( pMonster->CheckState( "JUMP_DOWN" ) || pMonster->CheckState( "JUMP_DOWN_R" ) == true )
            {
                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "JUMP_LANDING" );
                else
                    pMonster->SetState( "JUMP_LANDING_R" );
            }
        }
void HardAI_Ancient::CallBack_Die(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->TurnToTarget();
            if( pMonster->GetIsRight() == true )
                pMonster->SetSpeedX( -0.02f );
            else
                pMonster->SetSpeedX( 0.02f );

            pMonster->SetInvincible( true );
            if( pMonster->GetIsRight() == true )
                pMonster->SetState( "DIE" );
            else
        pMonster->SetState( "DIE_R" );
        }
void HardAI_Ancient::Action_WalkFailFunc(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->GetIsContact() == true )
            {
                if( pMonster->CheckSideBlock() == true )
                    pMonster->SetIsRight( false == pMonster->GetIsRight() );
                else
                    pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );

                pMonster->SetSpeedX( pMonster->GetWalkSpeed() * 1.5f );
                pMonster->SetSpeedY( 0.015f );
                pMonster->SetNoCheckContact(10);

                if( pMonster->GetIsRight() == true )
                    pMonster->SetState( "JUMP_DOWN" );
                else
                    pMonster->SetState( "JUMP_DOWN_R" );

                pMonster->ResetDelay();
            }
        }
// 고대병기 끝================================================

// 가디언타워 시작================================================
void HardAI_GuardianTower::Action_WaitFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->GetInitAI() )
            {
                if( 0 == pMonster->m_iTeam )
                {
            pMonster->AddParticleModel("GuardianTower_Serdin_1", -0.04f, 0.34f, 0, 0.6f, 1.0f, false);
            pMonster->AddParticleModel("GuardianTower_Serdin_2", -0.04f, 0.34f, 0, 0.3f, 0.6f, false);
            pMonster->AddParticleModel("GuardianTower_Serdin_3", -0.04f, 0.34f, 0, 0.0f, 0.3f, false);
                }
                else
                {
            pMonster->AddParticleModel("GuardianTower_Canaban_1", -0.04f, 0.34f, 0, 0.6f, 1.0f, false);
            pMonster->AddParticleModel("GuardianTower_Canaban_2", -0.04f, 0.34f, 0, 0.3f, 0.6f, false);
            pMonster->AddParticleModel("GuardianTower_Canaban_3", -0.04f, 0.34f, 0, 0.0f, 0.3f, false);
                }
            pMonster->SetInitAI( false );
        }

    pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
        if( iFrame >= pMonster->GetLastFrame() )
        {
            pMonster->ReleaseTarget();
            pMonster->SetState( "WAIT" );
        }
    }
void HardAI_GuardianTower::Action_DieFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1)
                pMonster->SetInvincible( true );
            else if( iFrame == pMonster->GetLastFrame() - 1 )
            {
                pMonster->AddParticle("Dota_Tower_Boom_01", 0.0f, 0.5f );
                pMonster->AddParticle("Dota_Tower_Boom_02", 0.0f, 0.5f );
                pMonster->AddParticle("Dota_Tower_Boom_04", 0.0f, 0.5f );
                pMonster->AddParticle("Dota_Tower_Boom_04", 0.0f, 1.0f );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->EndMonster(false, true);
                pMonster->SetFrameLock( true );
            }
        }
void HardAI_GuardianTower::Action_AttackFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            float fPosY;
            float fAngle;
            pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );
            pMonster->AddCountValue(-1);

            if( iFrame == 5 )
            {
                fPosY = 1.0f;
                pMonster->PlaySound("984028");
                if( 0 == pMonster->m_iTeam )
                {
                    pMonster->AddParticle("Dota_ShotR_01", 0.0f, fPosY);
                    pMonster->AddParticle("Dota_ShotR_04", 0.0f, fPosY);
                }
                else
                {
                    pMonster->AddParticle("Dota_ShotB_01", 0.0f, fPosY);
                    pMonster->AddParticle("Dota_ShotB_04", 0.0f, fPosY);
                }
                pMonster->ComputeAngleToTarget( 0.0f, 1.0f );
            }
            else if( iFrame == 6 )
            {
                fAngle = pMonster->GetAngleToTarget();
                if( pMonster->GetX() > pMonster->GetTargetX() )
                    fAngle = fAngle * -1.0f;
                else
                    fAngle = fAngle - 3.141592f;

                if( 0 == pMonster->m_iTeam )
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARDIAN_SERDIN, 0.0f, 1.0f, false, fAngle );
                else
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARDIAN_CANABAN, 0.0f, 1.0f, false, fAngle );
            }
            else if( iFrame == 55 )
            {
                fPosY = 1.0f;
                pMonster->PlaySound("984028");
                if( 0 == pMonster->m_iTeam )
                {
                    pMonster->AddParticle("Dota_ShotR_01", 0.0f, fPosY);
                    pMonster->AddParticle("Dota_ShotR_04", 0.0f, fPosY);
                }
                else
                {
                    pMonster->AddParticle("Dota_ShotB_01", 0.0f, fPosY);
                    pMonster->AddParticle("Dota_ShotB_04", 0.0f, fPosY);
                }
            }
            else if( iFrame == 56 )
            {
                pMonster->SetTarget( 0, -1000, 500, 1000, -400 );
                pMonster->ComputeAngleToTarget( 0.0f, 1.0f );
                fAngle = pMonster->GetAngleToTarget();
                if( pMonster->GetX() > pMonster->GetTargetX() )
                    fAngle = fAngle * -1.0f;
                else
                    fAngle = fAngle - 3.141592f;

                if( 0 == pMonster->m_iTeam )
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARDIAN_SERDIN, 0.0f, 1.0f, false, fAngle );
                else
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARDIAN_CANABAN, 0.0f, 1.0f, false, fAngle );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->ResetDelay();
                pMonster->SetState( "WAIT" );
        }
    }
void HardAI_GuardianTower::Trigger_Check_Targeting(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->SetTarget( 0, -1000, 1000, 1000, -500 ) == true )
        pMonster->ResetDelay();
        }
void HardAI_GuardianTower::Trigger_Check_Attack(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->SetTarget( 0, -900, 500, 900, -400 ) == true )
            {
                pMonster->SetCountValue(9);
                pMonster->SetState( "ATTACK01" );
                pMonster->ResetDelay();
            }
        }
void HardAI_GuardianTower::CallBack_Die(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetState( "DIE" );
        }
void HardAI_GuardianTower::CallBack_Damage(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetSpeedX( 0.0 );
            pMonster->SetSpeedY( 0.0 );
            pMonster->SetIsRight( true );
        }
// 가디언타워 끝================================================

// 가드타워 시작================================================
void HardAI_GuardTower::Action_WaitFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->GetInitAI() )
            {
                if( 0 == pMonster->m_iTeam )
                {
            pMonster->AddParticleModel("GuardTower_Serdin_1", 0.0f, -0.035f, 0, 0.6f, 1.0f, false);
            pMonster->AddParticleModel("GuardTower_Serdin_2", 0.0f, -0.035f, 0, 0.3f, 0.6f, false);
            pMonster->AddParticleModel("GuardTower_Serdin_3", 0.0f, -0.035f, 0, 0.0f, 0.3f, false);
                }
                else
                {
            pMonster->AddParticleModel("GuardTower_Canaban_1", 0.0f, -0.035f, 0, 0.6f, 1.0f, false);
            pMonster->AddParticleModel("GuardTower_Canaban_2", 0.0f, -0.035f, 0, 0.3f, 0.6f, false);
            pMonster->AddParticleModel("GuardTower_Canaban_3", 0.0f, -0.035f, 0, 0.0f, 0.3f, false);
                }
                pMonster->SetInitAI( false );
            }
            if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->ReleaseTarget();
                pMonster->SetState( "WAIT" );
            }
        }
void HardAI_GuardTower::Action_DieFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( iFrame <= 1)
                pMonster->SetInvincible( true );
            else if( iFrame == pMonster->GetLastFrame() - 1 )
            {
                pMonster->AddParticle("Dota_Tower_Boom_Small_01", 0.0f, 0.3f );
                pMonster->AddParticle("Dota_Tower_Boom_Small_02", 0.0f, 0.3f );
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->EndMonster(false, true);
                pMonster->SetFrameLock( true );
            }
        }
void HardAI_GuardTower::Action_AttackFunc(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            float fAngle;

            pMonster->SetIsRight( pMonster->GetDotaGateIsRight() );

            if( iFrame == 19 )
                pMonster->ComputeAngleToTarget( 0.0f, 0.0f );
            else if( iFrame == 20 )
            {
                pMonster->PlaySound("984028");
                fAngle = pMonster->GetAngleToTarget();
                if( pMonster->GetX() > pMonster->GetTargetX() )
                    fAngle = fAngle * -1.0f;
                else
                    fAngle = fAngle * 1.0f - 3.141592f;

                if( 0 == pMonster->m_iTeam )
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARD_SERDIN, 0.0f, 0.0f, false, fAngle );
                else
                    pMonster->AddDamageWithLocateAngle( DT_DOTA_STATIC_GUARD_CANABAN, 0.0f, 0.0f, false, fAngle );
            }
            else if( iFrame == 23 )
            {
                if( 0 == pMonster->m_iTeam )
                {
                    pMonster->AddParticle("Dota_ShotR_01", 0.0f, 0.35f );
                    pMonster->AddParticle("Dota_ShotB_04", 0.0f, 0.35f );
                }
                else
                {
                    pMonster->AddParticle("Dota_ShotB_01", 0.0f, 0.35f );
                    pMonster->AddParticle("Dota_ShotB_04", 0.0f, 0.35f );
                }
            }
            else if( iFrame >= pMonster->GetLastFrame() )
            {
                pMonster->ResetDelay();
                pMonster->SetState( "WAIT" );
        }
    }
void HardAI_GuardTower::Trigger_Check_Targeting(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->SetTarget( 0, -1000, 500, 1000, -600 ) == true )
                pMonster->ResetDelay();
        }
void HardAI_GuardTower::Trigger_Check_Attack(int iFrame, int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            if( pMonster->CheckTargetInRange( 0, -450, 500, 450, -200 ) == true )
                pMonster->SetState( "ATTACK01" );

        }
void HardAI_GuardTower::CallBack_Die(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetState( "DIE" );
        }
void HardAI_GuardTower::CallBack_Damage(int iMonIdx)
{
    if( false == g_kMonsterManager.IsMonsterIndex( iMonIdx ) )
        return;

        MONSTER *pMonster = g_kMonsterManager.GetMonster( iMonIdx );
    if( NULL == pMonster )
        return;

            pMonster->SetSpeedX( 0.0f );
            pMonster->SetSpeedY( 0.0f );
            pMonster->SetIsRight( true );

        }
// 가드타워 끝================================================