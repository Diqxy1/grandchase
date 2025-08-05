// HardDamage 제거작업 시작
//#include "stdafx.h"
//#include "HardDamageAssassinRanger.h"
//
//CHardDamageAssassinRanger::CHardDamageAssassinRanger( void )
//{
//
//}
//
//CHardDamageAssassinRanger::~CHardDamageAssassinRanger( void )
//{
//
//}
//
//void CHardDamageAssassinRanger::FrameMove( CDamageInstance* pDamageInstance )
//{
//	if( pDamageInstance == NULL )
//	{
//		ASSERT( !"뭥미 포인터가 널이삼" );
//		return;
//	}
//
//	int iLife = pDamageInstance->m_Life;
//	if( iLife == 40 )
//	{
//		pDamageInstance->m_pOriginalDamage->m_Angle_Speed = 0.0f;
//		pDamageInstance->m_pOriginalDamage->m_Start_Angle = 0.0f;
//		pDamageInstance->m_pOriginalDamage->m_fDMXSpeed = 0.0f;
//		pDamageInstance->m_pOriginalDamage->m_fDMYSpeed = 0.0f;
//		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0001f;
//	}
//
//
//	CDamage::FrameMove( pDamageInstance );
//
//}
// HardDamage 제거작업 끝