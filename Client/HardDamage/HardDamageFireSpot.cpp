#include "stdafx.h"
#include ".\harddamagefirespot.h"

HardDamageFireSpot::HardDamageFireSpot(void)
{
}

HardDamageFireSpot::~HardDamageFireSpot(void)
{
}

void HardDamageFireSpot::AngleFrameMove( CDamageInstance* pDamageInstance )
{
	float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
	float Angle = acosf( m_x_Speed / Length );

	// 포물선 비스무리하게 해보려고...;;
	if( pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life )
		m_fStartAngle = pDamageInstance->m_Angle;

	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
		if( m_fStartAngle - 2.44f < pDamageInstance->m_Angle )
			pDamageInstance->m_Angle += m_Angle_Speed;			
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
	}
	else
	{
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
		if( m_fStartAngle + 2.44f > pDamageInstance->m_Angle )
			pDamageInstance->m_Angle -= m_Angle_Speed;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
	}
}