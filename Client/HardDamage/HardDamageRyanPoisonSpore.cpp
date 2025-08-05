#include "stdafx.h"
#include "..\KGCRoomManager.h"
#include ".\HardDamageRyanPoisonSpore.h"
#include "KGCPlayerCommon.h"
#include "Monster.h"
#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGC3DObject.h"

CHardDamageRyanPoisonSpore::CHardDamageRyanPoisonSpore(void)
{
	m_isOn = false;
}

CHardDamageRyanPoisonSpore::~CHardDamageRyanPoisonSpore(void)
{
}

void CHardDamageRyanPoisonSpore::End( CDamageInstance* pDamageInstance )
{
	CDamage::End( pDamageInstance );
}

bool CHardDamageRyanPoisonSpore::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ )
{
	PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
	if ( pPlayer->IsLocalPlayer() && false == pDamageInstance->CheckAlreadyDamaged(hitWho))
	{
#if defined (NATION_USA )
        pPlayer->SetHP( pPlayer->GetHP() + pPlayer->m_fAbility[ABILITY_HP] * 0.025f );
#else
#if !defined( MATCH_BALANCE_2011_8 )
		pPlayer->SetHP( pPlayer->GetHP() + pPlayer->m_fAbility[ABILITY_HP] * 0.05f );
#endif
#endif
	}

	return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );
}

void CHardDamageRyanPoisonSpore::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{

	PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
	if ( pPlayer->IsLocalPlayer() && false == pDamageInstance->CheckAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ))
	{
#if defined (NATION_USA )
		pPlayer->SetHP( pPlayer->GetHP() + pPlayer->m_fAbility[ABILITY_HP] * 0.025f );
#else
#if !defined( MATCH_BALANCE_2011_8 )
		pPlayer->SetHP( pPlayer->GetHP() + pPlayer->m_fAbility[ABILITY_HP] * 0.05f );
#endif
#endif
	}

	CDamage::CrashMonster( pDamageInstance, Monsteri );
	return;
}

void CHardDamageRyanPoisonSpore::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );
}

void CHardDamageRyanPoisonSpore::AngleFrameMove( CDamageInstance* pDamageInstance )
{
	float fPosY = -1.0f;

	if ( pDamageInstance->m_Life < 195 )
	{
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
	}

	if( fPosY != -1.0f )
	{
		pDamageInstance->m_Y = fPosY;
		pDamageInstance->m_Angle = 0;
		pDamageInstance->m_pOriginalDamage->m_IsEnableDamage = true;
		pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0f;
		pDamageInstance->m_pOriginalDamage->m_Angle_Speed = 0.0f;
		pDamageInstance->m_pOriginalDamage->m_IsFixedAngleMove = false;

		if( !m_isOn )
		{
			g_KDSound.Play( "40006" );
			m_isOn = true;
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

void CHardDamageRyanPoisonSpore::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	pDamageInstance->m_p3DObject->SetCartoon( (EGCCartoonTexture)CARTOON_SLIM );
	CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );
}


