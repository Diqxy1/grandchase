//#include "stdafx.h"
//#include ".\harddamagemine.h"
//
//#include "DamageManager.h"
//#include "Spark.h"
//
//
//CHardDamageMine::CHardDamageMine(void)
//{
//}
//
//CHardDamageMine::~CHardDamageMine(void)
//{
//}
//
//bool CHardDamageMine::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
//{
//	// 폭발 이펙트를 발생시킨다.
//	g_KDSound.Play( "43" );
//
//	g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_FIRE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
//	g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_WAVE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
//	g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
//
//	pDamageInstance->m_Life = 1;
//	return true;
//}
//
//void CHardDamageMine::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
//{
//	// 폭발 이펙트를 발생시킨다.
//	g_KDSound.Play( "43" );
//
//	g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_FIRE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
//	g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_WAVE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
//	g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
//
//	pDamageInstance->m_Life = 1;
//}
//
//void CHardDamageMine::NotAngleFrameMove(CDamageInstance* pDamageInstance )
//{
//	CDamage::NotAngleFrameMove(pDamageInstance);
//
//	if ( pDamageInstance->m_Life <= -m_Life_Change ) 
//	{
//		// 폭발 이펙트를 발생시킨다.
//		g_KDSound.Play( "43" );
//
//		g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_FIRE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
//		g_MyD3D->m_pDamageManager->AddWithLocate( DT_BOMB_WAVE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
//		g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
//
//	}
//}
//
