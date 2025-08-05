#include "stdafx.h"
#include ".\HardDamageSinFly.h"
#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"

CHardDamageSinFly::CHardDamageSinFly(void)
{
	m_fStartY = 0.0f;
	m_iTime   = 0;
	m_fCyclopseY = -1.0f;
}

CHardDamageSinFly::~CHardDamageSinFly(void)
{
}


void CHardDamageSinFly::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	if (m_iTime == 0)
	{
		m_fStartY = pDamageInstance->m_Y;
	}
	m_iTime++;               //1프레임을 1초로 계산하자.

	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_X += m_x_Speed;
		pDamageInstance->m_Angle += m_Angle_Speed;
	}
	else
	{
		pDamageInstance->m_X -= m_x_Speed;
		pDamageInstance->m_Angle -= m_Angle_Speed;
	}

	if( pDamageInstance->m_What == DT_CONFUSION )
	{
		float fDiff = sinf( (float)pDamageInstance->m_Life / (D3DX_PI*2.0f) );
		pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f);
	}	
	else if ( pDamageInstance->m_What == DT_DARK_SWORM_FLY)
	{
		float fDiff;
		if( rand()%2 == 0 )
            fDiff = sinf( (float)pDamageInstance->m_Life / (D3DX_PI*2.0f) );
		else
			fDiff = cosf( (float)pDamageInstance->m_Life / (D3DX_PI*2.0f) );
        
		pDamageInstance->m_Y = m_fStartY + (fDiff / 40.0f);
	}	
	else if ( pDamageInstance->m_What == DT_CYCLOPSE_STONE )
	{
		pDamageInstance->m_Y = m_fStartY + m_y_Speed * m_iTime - 0.000514f*m_iTime*m_iTime;
		
		//한 번 땅에 떨어진 이후에는 계속 그 y좌표로 구른다.
		if(m_fCyclopseY!= -1.0f)
			pDamageInstance->m_Y = m_fCyclopseY;
		else
		{
			float fPosY;
            if( SiKGCRoomManager()->IsMonsterGameMode() )
            {
                D3DXVECTOR2 dxvCrashPos;
			    SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
                fPosY = dxvCrashPos.y;
            }
            else
            {
                fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
            }
            if( fPosY != -1.0f )
            {
                m_fCyclopseY = fPosY + 0.2f;
            }
		}	
	}
	else  // s =s0 + vt + 1/2 at^2 공식을 활용하였다. 중력 가속도는 우선 적당한 값을 넣어 만들었다만...
	{
		pDamageInstance->m_Y = m_fStartY + m_y_Speed * m_iTime - 0.000686f*m_iTime*m_iTime;
	}
}
