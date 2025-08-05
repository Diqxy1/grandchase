#include "stdafx.h"
#include ".\HardDamageMermairing.h"


void CHardDamageMermairing::FrameMove( CDamageInstance* pDamageInstance )
{
	if( pDamageInstance->m_Life == 33 )
	{
		pDamageInstance->m_pOriginalDamage->m_fDMXSpeed = 0.015f;
		pDamageInstance->m_pOriginalDamage->m_fPushTargetX = 0.025f;
	}
	if( pDamageInstance->m_Life == 30 )
	{
		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.f;
		pDamageInstance->m_pOriginalDamage->m_fDMXSpeed = 0.f;
		pDamageInstance->m_pOriginalDamage->m_fPushTargetX = 0.f;
	}
	if( pDamageInstance->m_Life <= 8 )
	{
		pDamageInstance->m_pOriginalDamage->m_fDamageToMonster = 0.f;
		pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer = 0.f;

	}
	

	return CDamage::FrameMove( pDamageInstance );
}