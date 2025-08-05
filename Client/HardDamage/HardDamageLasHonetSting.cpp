#include "stdafx.h"
#include ".\HardDamageLasHonetSting.h"




CHardDamageLasHonetSting::CHardDamageLasHonetSting(void)
{
}

CHardDamageLasHonetSting::~CHardDamageLasHonetSting(void)
{
}

void CHardDamageLasHonetSting::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	if ( 49 <= pDamageInstance->m_Life && pDamageInstance->m_Life < 51 )
	{
		pDamageInstance->m_pOriginalDamage->m_fDMXSpeed = 0.0f;
		pDamageInstance->m_pOriginalDamage->m_fPushTargetX = 0.0f;
	}
}