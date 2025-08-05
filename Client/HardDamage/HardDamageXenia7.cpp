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
        // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

	return;
}

bool CHardDamageHolyPole::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	MONSTER* pMonster;

    //  ���Ͼ� ��� �ŷ�
    //  �̰� ������ ���� �ǰ� ���� ���� �� �Ǵ� Ǯ�� ����.

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
    //  ���� ������ ���� ƨ�� ���ش�.

	CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

	D3DXVECTOR3 m_vPos;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

	if(pMonster == NULL ) return false;

	// ��!
	m_vPos = pMonster->GetBonePos( 9 );

	// ���� ��ġ�� ���� ���ư��°� ������ �ְڴ�
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

// HardDamage �����۾� ����
//  ���� �ڵ� ����
//  Ư�� ��ǿ����� ������ �±�
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
// 		// ���� �ݺ� �����̶�..
//		g_KDSound.Stop( "996006" );
// //		return;
// //	}
// }
// 
// void CHardDamageDivineTreeAcidRain::End( CDamageInstance* pDamageInstance )
// {
// 	CDamage::End( pDamageInstance );
// 	// ���� �ݺ� �����̶�..
//	g_KDSound.Stop( "996006" );
// }
// HardDamage �����۾� ��

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