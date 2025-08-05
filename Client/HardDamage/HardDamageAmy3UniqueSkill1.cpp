#include "stdafx.h"
#include ".\HardDamageAmy3UniqueSkill1.h"
#include "Stage.h"



#include "KGCRoomManager.h"
#include "MapSystem/GCFantasticMap.h"

#define PI 3.141592f

CHardDamageAmy3UniqueSkill1::CHardDamageAmy3UniqueSkill1(void)
{
	m_fPrevPosY = -9999.0f;
	m_fPosY = 0.0f;
}

CHardDamageAmy3UniqueSkill1::~CHardDamageAmy3UniqueSkill1(void)
{
}

void CHardDamageAmy3UniqueSkill1::AngleFrameMove(CDamageInstance* pDamageInstance )
{
	if( pDamageInstance->m_What == DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1_1 || 
		pDamageInstance->m_What == DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2_1 || 
		pDamageInstance->m_What == DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3_1 )
	{
		if( pDamageInstance->m_Life == 255 )
		{
            if( !g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who) )
                return;

			if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight() )
			{
				pDamageInstance->m_pOriginalDamage->m_x_Speed = -0.02f;
			}
			else
			{
				pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.02f;
			}
		}		
	}

	if( SiKGCRoomManager()->IsMonsterGameMode() )
	{
        if( m_fPrevPosY > pDamageInstance->m_Y) {
            D3DXVECTOR2 dxvCrashPos;
			SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
            m_fPosY = dxvCrashPos.y;
	}
	}
	else
	{
		if( m_fPrevPosY > pDamageInstance->m_Y)
			m_fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
	}

	if( m_fPosY != -1.0f && m_fPrevPosY > pDamageInstance->m_Y )
	{
		if( pDamageInstance->m_What == DT_AMY3_DANCE_UNIQUE_SKILL_PET1 && pDamageInstance->m_Life > 245 )
		{
			float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
			float Angle = acosf( m_x_Speed / Length );

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
		else
		{
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
				pDamageInstance->CreateSequence("Amy03_Pet_Out_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			g_KDSound.Play("1075");
			pDamageInstance->m_Y = m_fPosY;
			pDamageInstance->m_Angle = 0;
			pDamageInstance->m_Life = 0;
		}
	}
	else
	{
		float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
		float Angle = acosf( m_x_Speed / Length );

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

	m_fPrevPosY = pDamageInstance->m_Y;
}
