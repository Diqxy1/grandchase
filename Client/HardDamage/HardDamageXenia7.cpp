#include "stdafx.h"
#include ".\HardDamageXenia7.h"
#include "Monster.h"

CHardDamageHolyPole::CHardDamageHolyPole( void )
{

}

CHardDamageHolyPole::~CHardDamageHolyPole( void )
{

}

void CHardDamageHolyPole::Begin( CDamageInstance* pDamageInstance )
{
	
}

void CHardDamageHolyPole::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( Monsteri );
    if( pMonster )
    {
        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

	return;
}

bool CHardDamageHolyPole::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	MONSTER* pMonster;

    //  제니아 대륙 신록
    //  이걸 먹으면 몬스터 피가 조금 차고 내 피는 풀로 찬다.

	if ( g_kGlobalValue.m_kUserInfo.bHost )
	{
		for ( int i = 0; i < 5; ++i )
		{
			pMonster = g_kMonsterManager.GetMonster(i);
			if ( pMonster->m_bDie || pMonster->m_fHP <= 0.0f )
				continue;

			pMonster->m_fHP += (pMonster->m_fHPPoint * 0.05f);

			if ( pMonster->m_fHP > pMonster->m_fHPPoint )
				pMonster->m_fHP = pMonster->m_fHPPoint;
		}
	}

	if ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
	{
		g_MyD3D->MyPlayer[hitWho]->SetHPFull();
	}

	pDamageInstance->m_Life = 1;

	return true;
}



CHardDamageDivineTreeSonicBomb::CHardDamageDivineTreeSonicBomb( void )
{

}

CHardDamageDivineTreeSonicBomb::~CHardDamageDivineTreeSonicBomb( void )
{

}

bool CHardDamageDivineTreeSonicBomb::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    //  맞은 각도에 따라서 튕겨 내준다.

	CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

	D3DXVECTOR3 m_vPos;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

	if(pMonster == NULL ) return false;

	// 입!
	m_vPos = pMonster->GetBonePos( 9 );

	// 맞은 위치에 따라서 날아가는걸 조절해 주겠다
	float fX = m_vPos.x - g_MyD3D->MyPlayer[hitWho]->vPos.x;
	float fY = ASMfabs(m_vPos.y - g_MyD3D->MyPlayer[hitWho]->vPos.y);
	float fLength = ASMsqrt( powf( fX, 2.0f ) + powf( fY, 2.0f ) );
	float fAngle = asinf( fY / fLength );

	if( pDamageInstance->m_X + 0.05f < g_MyD3D->MyPlayer[hitWho]->vPos.x )
		g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.07f * cosf( fAngle );
	else
		g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.07f * cosf( fAngle );

	g_MyD3D->MyPlayer[hitWho]->y_Speed = -0.2f * sinf( fAngle ) + 0.03f;

	g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_COMMON_DOWN_AND_STANDUP );

	return true;
}

// HardDamage 제거작업 시작
//  사운드 자동 종료
//  특정 모션에서만 데미지 맞기
// 
// CHardDamageDivineTreeAcidRain::CHardDamageDivineTreeAcidRain( void )
// {
// 	pMonster = NULL;
// }
// 
// CHardDamageDivineTreeAcidRain::~CHardDamageDivineTreeAcidRain( void )
// {
// 
// }
// 
// void CHardDamageDivineTreeAcidRain::Begin( CDamageInstance* pDamageInstance )
// {
// 	CDamage::Begin( pDamageInstance );
// //	pMonster = g_kMonsterManager.GetMonster( 0 );
// 
// //	if ( pMonster->m_bDie || pMonster->m_fHP <= 0.0f )
// //	{
// //		pDamageInstance->m_Life = 1;
// //		return;
// //	}
// }
// 
// void CHardDamageDivineTreeAcidRain::NotAngleFrameMove( CDamageInstance* pDamageInstance )
// {
//   
// 	CDamage::NotAngleFrameMove( pDamageInstance );
// 
// //	if ( pMonster && (pMonster->m_bDie || pMonster->m_fHP <= 0.0f) )
// //	{
// //		pDamageInstance->m_Life = 1;
// 		// 무한 반복 음원이라..
//		g_KDSound.Stop( "996006" );
// //		return;
// //	}
// }
// 
// void CHardDamageDivineTreeAcidRain::End( CDamageInstance* pDamageInstance )
// {
// 	CDamage::End( pDamageInstance );
// 	// 무한 반복 음원이라..
//	g_KDSound.Stop( "996006" );
// }
// HardDamage 제거작업 끝

CHardDamageSunkenArea::CHardDamageSunkenArea( void )
{

}

CHardDamageSunkenArea::~CHardDamageSunkenArea( void )
{

}

bool CHardDamageSunkenArea::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if ( ( g_MyD3D->MyPlayer[hitWho]->m_ePlayerState == PS_ALIVE || g_MyD3D->MyPlayer[hitWho]->m_ePlayerState == PS_FATAL ) && 
		   g_MyD3D->MyPlayer[hitWho]->IsContact )
	{
		CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

		return true;
	}

	return false;
}