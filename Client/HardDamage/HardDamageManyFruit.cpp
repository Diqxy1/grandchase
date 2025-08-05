#include "stdafx.h"
#include ".\harddamagemanyfruit.h"

CHardDamageManyFruit::CHardDamageManyFruit(void)
{
}

CHardDamageManyFruit::~CHardDamageManyFruit(void)
{
}

void CHardDamageManyFruit::AngleFrameMove(CDamageInstance* pDamageInstance )
{
	float fTempYSpeed = ( pDamageInstance->m_Life - m_Angle_Speed ) * 0.00098f;
	float fTempXSpeed = m_x_Speed * 0.3f;

	if( fTempYSpeed < -0.1f )
		fTempYSpeed = -0.1f;

	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_X += fTempXSpeed;
		pDamageInstance->m_Y += fTempYSpeed;
	}
	else
	{
		pDamageInstance->m_X -= fTempXSpeed;
		pDamageInstance->m_Y += fTempYSpeed;
	}
}