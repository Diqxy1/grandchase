//#include "stdafx.h"
//#include ".\HardDamageSiegeDeco.h"
//
//
//
//
//
//CHardDamageSiegeDeco::CHardDamageSiegeDeco(void)
//{
//
//}
//
//CHardDamageSiegeDeco::~CHardDamageSiegeDeco(void)
//{
//}
//
//void CHardDamageSiegeDeco::FrameMove( CDamageInstance* pDamageInstance )
//{
//	CDamage::FrameMove( pDamageInstance );
//	
//	UINT curMotion = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion;
//	if ( curMotion != MID_SKILL_LIRE_SIEGE_START && curMotion != MID_SKILL_LIRE_SIEGE_WAIT 
//		&& curMotion != MID_SKILL_LIRE_SIEGE_ATK && curMotion != MID_SKILL_LIRE_DOUBLE_SIEGE_SHOT )
//	{
//		pDamageInstance->m_Life = 0;
//	}
//	else if ( curMotion == MID_SKILL_LIRE_SIEGE_END )
//	{
//		pDamageInstance->m_Life = 0;
//	}
//	else 
//	{
//		pDamageInstance->m_Life = 500;
//	}
//}