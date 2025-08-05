#include "stdafx.h"
#include ".\HardDamageElesis4Special2Shoot.h"




#define DA_NOT(p) (0xFFFFFFFF - p)

CHardDamageElesis4Special2Shoot::CHardDamageElesis4Special2Shoot(void)
{
}

CHardDamageElesis4Special2Shoot::~CHardDamageElesis4Special2Shoot(void)
{
}

void CHardDamageElesis4Special2Shoot::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    // Scale
    if( pDamageInstance->m_Life <= 33 )
    {
        if( pDamageInstance->m_Life > 25 && pDamageInstance->m_Life <= 33 )
        {
            pDamageInstance->m_pOriginalDamage->m_Scale_Speed = 1.06f;
        }
        else if( pDamageInstance->m_Life == 25 )
        {
            pDamageInstance->m_pOriginalDamage->m_Scale_Speed = 1.00f;
        }

        if( 0.32f < pDamageInstance->m_Scale_x )
        {
            pDamageInstance->m_Scale_x = 0.32f;
        }
    }

    // Speed
    if( pDamageInstance->m_Life == 33 )
    {
        pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.07f;
    }
    else if( pDamageInstance->m_Life < 33 )
    {
        pDamageInstance->m_pOriginalDamage->m_x_Speed *= 0.92f;

        if( pDamageInstance->m_Life == 29 )
        {
            pDamageInstance->m_pOriginalDamage->m_IsEnableDamage = true;
#if defined( LEVEL_DESIGN_STAT_CALC )
            pDamageInstance->m_pOriginalDamage->m_fDamageToMonster = -60.f;
            pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer = -43.f;
#else
            pDamageInstance->m_pOriginalDamage->m_fDamageToMonster = -1.25f;
            pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer = -0.6f;
#endif
        }
        else if( pDamageInstance->m_Life == 15 )
        {
            pDamageInstance->m_pOriginalDamage->m_Scale_Speed = 0.99f;
        }
    }
}