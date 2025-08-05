//#include "stdafx.h"
//#include ".\HardDamageGorgosFire.h"
//
//CHardDamageGorgosFire::CHardDamageGorgosFire(void)
//{
//}
//
//CHardDamageGorgosFire::~CHardDamageGorgosFire(void)
//{
//}
//
//
//void CHardDamageGorgosFire::Begin(CDamageInstance* pDamageInstance )
//{
//	float fStartAngle = 1.57f * 0.02f * (float)(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 69);
//	float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));
//	float Angle = 0.0f;
//	Angle = fStartAngle * 0.8f + asinf( m_Start_Locate_y / Length );
//
//	if( pDamageInstance->m_IsRight )
//	{
//		pDamageInstance->m_Angle =  m_Start_Angle + fStartAngle;			
//	}
//	else
//	{
//		pDamageInstance->m_Angle = -m_Start_Angle - fStartAngle;				
//	}
//	pDamageInstance->m_X		= g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x;
//	pDamageInstance->m_Y		= g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y;
//	pDamageInstance->m_Scale_x	= m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.04f;	
//
//
//	BeginAnim(pDamageInstance);
//	BeginParticle(pDamageInstance);
//
//
//}
