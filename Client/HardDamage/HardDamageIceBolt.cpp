#include "stdafx.h"
#include ".\harddamageicebolt.h"

HardDamageIceBolt::HardDamageIceBolt(void)
{
}

HardDamageIceBolt::~HardDamageIceBolt(void)
{
}

void HardDamageIceBolt::AngleFrameMove( CDamageInstance* pDamageInstance )
{
	float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
	float Angle = acosf( m_x_Speed / Length );

	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
		pDamageInstance->m_Angle += m_Angle_Speed;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
	}
	else
	{
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
		pDamageInstance->m_Angle -= m_Angle_Speed;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
	}
}