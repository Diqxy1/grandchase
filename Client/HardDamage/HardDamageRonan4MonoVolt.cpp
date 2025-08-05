#include "stdafx.h"
#include ".\HardDamageRonan4MonoVolt.h"
#include "Stage.h"



#include "KGCRoomManager.h"
#include "MapSystem/GCFantasticMap.h"

CHardDamageRonan4MonoVolt::CHardDamageRonan4MonoVolt(void)
{
	m_isOn = false;
}

CHardDamageRonan4MonoVolt::~CHardDamageRonan4MonoVolt(void)
{
}

void CHardDamageRonan4MonoVolt::AngleFrameMove(CDamageInstance* pDamageInstance )
{

	if( SiKGCRoomManager()->IsMonsterGameMode() )
	{
		D3DXVECTOR2 dxvCrashPos;
		SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
        m_fPosY = dxvCrashPos.y;
	}
	else
	{
		m_fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
	}

	if( m_fPosY != -1.0f )
	{
		if( pDamageInstance->m_What == DT_RONAN4_MONOVOLT_ATK2_1 || 
			pDamageInstance->m_What == DT_RONAN4_MONOVOLT_ATK2_2 ||
			pDamageInstance->m_What == DT_RONAN4_MONOVOLT_ATK2_3 )
		{
			pDamageInstance->m_Y = m_fPosY;
			pDamageInstance->m_Angle = 0;
			pDamageInstance->m_pOriginalDamage->m_IsEnableDamage = true;
			if( !m_isOn )
			{
				g_KDSound.Play( "904" );
				m_isOn = true;
			}
		}
	}
	else
	{
		float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
		float Angle = acosf( m_x_Speed / Length );

		// 포물선 비스무리하게 해보려고...;;
		if( pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life )
			m_fStartAngle = pDamageInstance->m_Angle;

		if( pDamageInstance->m_IsRight )
		{
			pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
			if( m_fStartAngle - 1.7f < pDamageInstance->m_Angle )
				pDamageInstance->m_Angle += m_Angle_Speed;			
			pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
		}
		else
		{
			pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
			if( m_fStartAngle + 1.7f > pDamageInstance->m_Angle )
				pDamageInstance->m_Angle -= m_Angle_Speed;
			pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
		}
	}
}
