#include "stdafx.h"
#include ".\HardDamageFireLance.h"
#include "Spark.h"

CHardDamageFireLance::CHardDamageFireLance(void)
{
}

CHardDamageFireLance::~CHardDamageFireLance(void)
{
}

void CHardDamageFireLance::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);

	if ( !pDamageInstance->m_pOriginalDamage->m_vecChildParticle.empty() )
	{
		return;
	}
	

	float x, y;

	if( pDamageInstance->m_Life == 255 )
	{
		x = pDamageInstance->m_X - 1.0f;
		y = pDamageInstance->m_Y - 0.2f;
		pDamageInstance->Add( LANCE_MAGIC_CIRCLE, x, y, 0, 1.5f, x, y, 0.0f, pDamageInstance->m_IsRight );
	}
	if( !o5 ) 
	{
		x = pDamageInstance->m_X - 1.0f;
		y = pDamageInstance->m_Y - 0.2f;
		pDamageInstance->Add( LANCE_MOVE_CIRCLE, x, y, 0, 0.5f, x, y, 0.0f, pDamageInstance->m_IsRight );
	}
	if( !o3 ) 
	{
		x = pDamageInstance->m_X - 1.0f;
		y = pDamageInstance->m_Y - 0.2f;
		pDamageInstance->Add( LANCE_SPRAY, x, y, 0, 1.6f, x, y, 0.0f, pDamageInstance->m_IsRight );
	}

}