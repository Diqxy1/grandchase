#include "stdafx.h"
#include "Headup Display.h"
#include "..\DamageManager.h"
#include ".\HardDamageXenia8.h"
#include "Monster.h"

CHardDamageDarkTemplarDoubleFrame::CHardDamageDarkTemplarDoubleFrame( void )
{

}

CHardDamageDarkTemplarDoubleFrame::~CHardDamageDarkTemplarDoubleFrame( void )
{

}

bool CHardDamageDarkTemplarDoubleFrame::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if ( CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate ) == false )
		return false;

	if ( pDamageInstance->m_What == DT_DARKTEMPLAR_DOUBLEFRAME )
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
		if( pMonster  == NULL ) return false;
		D3DXVECTOR3 m_vPos = pMonster->GetBonePos( 4 );

		// ÇÇ Èí¼ö
		pMonster->m_fHP += (pMonster->m_fHPPoint * 0.02f);
		if ( pMonster->m_fHP > pMonster->m_fHPPoint )
			pMonster->m_fHP = pMonster->m_fHPPoint;

		D3DXVECTOR3 m_vParticlePos = pMonster->GetBonePos( 4 );
		// ÀÌÆåÆ®
		g_ParticleManager->CreateSequence( "Dark_Templar_Soul_Drain_Absorb_01", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f );
		g_ParticleManager->CreateSequence( "Dark_Templar_Soul_Drain_Absorb_02", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f );
		g_ParticleManager->CreateSequence( "Dark_Templar_Soul_Drain_Absorb_03", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f );

		if ( g_MyD3D->IsPlayerIndex( hitWho ) && g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
		{
			float fDiff = m_fDamageToMonster * -10.0f * pMonster->m_fAttackPoint /  g_MyD3D->MyPlayer[hitWho]->m_fAbility[ABILITY_DEF];
			g_MyD3D->MyPlayer[hitWho]->SetHP( g_MyD3D->MyPlayer[hitWho]->GetHP() - fDiff );

			g_MyD3D->MyHead->Level /= g_kGlobalValue.GetMaxMP();
		}
	}
	else
	{
		if ( g_MyD3D->IsPlayerIndex( hitWho ) && g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
		{
			g_MyD3D->MyHead->Level = 0.0f;
		}
	}

	return true;
}


CHardDamageIblisLaser::CHardDamageIblisLaser( void )
{
	pMonster = NULL;
}

CHardDamageIblisLaser::~CHardDamageIblisLaser( void )
{

}

void CHardDamageIblisLaser::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate(pDamageInstance, fStartX, fStartY);

	vecLiveIndex.clear();
	iPlayerNum = 0;

	for(int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && ((char)g_MyD3D->MyPlayer[i]->m_cLife) > 0)
		{
			vecLiveIndex.push_back( i );
			iPlayerNum++;
		}
	}

	pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

	return;
}

void CHardDamageIblisLaser::NotAngleFrameMove( CDamageInstance* pDamageInstance )
{
	if ( vecLiveIndex.size() <= 0 )
		return;

	float fAngle = 0.0f;

	if ( pMonster->m_bDie || pMonster->m_fHP < 0.0f )
	{
		pDamageInstance->m_Life = 1;
		return;
	}

	for ( unsigned int i = 0; i < vecLiveIndex.size() ; ++i )
	{
		pMonster->SetPlayerTargetManual( vecLiveIndex[i] );
		D3DXVECTOR3 vFirePos = pMonster->GetBonePos( 7 );

		float fOffsetX = vFirePos.x + ( pMonster->GetIsRight() ? 0.15f : -0.15f ) - pMonster->GetX();
		float fOffsetY = vFirePos.y - pMonster->GetY();

		float fX = pMonster->GetTargetX() - ( pMonster->GetX() + fOffsetX );
		float fY = pMonster->GetTargetY() + 0.15f - ( pMonster->GetY() + fOffsetY );

		float fLength = ASMsqrt( powf( fX, 2.0f ) + powf( fY, 2.0f ) );
		float fAngle = asinf( fY / fLength );

		fAngle = fAngle * (pMonster->GetIsRight() ? 1.0f : -1.0f);
		if ( ( pMonster->GetX() > pMonster->GetTargetX() && pMonster->GetIsRight() ) ||
			( pMonster->GetX() < pMonster->GetTargetX() && !pMonster->GetIsRight() ) )
		{
			fAngle = fAngle * -1.0f + D3DX_PI;
		}

        if( pDamageInstance->m_What == DT_IBLIS_LASER_SHOOTER ) { 
		    pMonster->AddDamageWithLocateAngle( DT_IBLIS_LASER, vFirePos.x + (pMonster->GetIsRight() ? 0.15f : -0.15f), vFirePos.y, true, fAngle );
        }
        else if( pDamageInstance->m_What == DT_SUPERMON_IBLIS_LASER_SHOOTER ) { 
            pMonster->AddDamageWithLocateAngle( DT_SUPERMON_IBLIS_LASER, vFirePos.x + (pMonster->GetIsRight() ? 0.15f : -0.15f), vFirePos.y, true, fAngle );
        }
	}

	vecLiveIndex.clear();
}