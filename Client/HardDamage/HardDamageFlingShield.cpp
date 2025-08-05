#include "stdafx.h"
#include ".\harddamageflingshield.h"

CHardDamageFlingShield::CHardDamageFlingShield(void)
{
}

CHardDamageFlingShield::~CHardDamageFlingShield(void)
{
}

void CHardDamageFlingShield::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);
	pDamageInstance->m_X += (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - pDamageInstance->m_X) * 0.1f;
	pDamageInstance->m_Y += (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + g_MyD3D->MyPlayer[pDamageInstance->m_Who]->y_Ex + 0.07f - pDamageInstance->m_Y) * 0.1f;
}

void CHardDamageFlingShield::End(CDamageInstance* pDamageInstance )
{
	g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Shield = 50;
	CDamage::End(pDamageInstance);
}
