#include "stdafx.h"
#include ".\HardDamageLas4BounceAtk.h"



//

CHardDamageLas4BounceAtk::CHardDamageLas4BounceAtk(void)
{
}

CHardDamageLas4BounceAtk::~CHardDamageLas4BounceAtk(void)
{
}

void CHardDamageLas4BounceAtk::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    if( pDamageInstance->m_Life <= 3 )
        return;

    if( pDamageInstance->m_pOriginalDamage->m_Y_per_x > 0.3f )
    {
        pDamageInstance->m_pOriginalDamage->m_Y_per_x -= 0.2f;
        pDamageInstance->m_pOriginalDamage->m_Start_Locate_y += 0.06f;
    }
}
