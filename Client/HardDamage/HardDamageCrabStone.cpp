#include "StdAfx.h"
#include "HardDamageCrabStone.h"
#include "Monster.h"
#include "mapsystem/GCFantasticMap.h"

HardDamageCrabStone::HardDamageCrabStone(void)
{
}

HardDamageCrabStone::~HardDamageCrabStone(void)
{
}

void HardDamageCrabStone::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    if( pDamageInstance->m_pOriginalDamage->m_bIsTotalRange )
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }
    GCCollisionRect<float> rtFire;
    rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x*fRangeX - 1.0f;
    rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x*fRangeX - 1.0f;
    rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
    rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
	if( pMonster  == NULL ) 
		return;

    for( int i = 0 ; i < pMonster->GetAttackBoundRectNum(); i++ )
    {
        GCCollisionRect<float> rtMonster = pMonster->GetAttackBoundRect(i);

        rtMonster.m_Right -= 0.33f;
        rtMonster.m_Left += 0.33f;

        if( rtMonster.CheckCollision( rtFire, NULL ) )
        {
            pDamageInstance->m_Life = 1;
            g_KDSound.Play( 43, 0 );
            pMonster->SetFlashEffectFrame( 4 );
            g_ParticleManager->CreateSequence("Rock_Shock_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f , 0.5f );
            g_ParticleManager->CreateSequence("Rock_Shock_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f , 0.5f );

            break;
        }
        else if ( SiKGCFantasticMap()->IsLastBottomPosition(pDamageInstance->m_X, pDamageInstance->m_Y ) )
        {
            pDamageInstance->m_Life = 1;
            g_KDSound.Play( 43, 0 );        
            g_ParticleManager->CreateSequence("Rock_Shock_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f , 0.5f );
            g_ParticleManager->CreateSequence("Rock_Shock_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f , 0.5f );

            break;
        }        


    }

}