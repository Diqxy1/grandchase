#include "stdafx.h"
#include "Controls.h"
#include ".\HardDamageAstarot.h"
#include "Controls.h"
#include "Buff\KGCBuffManager.h"

CHardDamageAstarot::CHardDamageAstarot( void )
{

}

CHardDamageAstarot::~CHardDamageAstarot( void )
{

}

void CHardDamageAstarot::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	CheckAndDebuff(pDamageInstance);
	CDamage::CrashMonster(pDamageInstance,Monsteri);
	return;
}

void CHardDamageAstarot::CheckAndDebuff(CDamageInstance* pDamageInstance)
{
	if (pDamageInstance->m_What == DT_PET_ASTAROT_ATTACK1 && g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_ASTAROT_SP_UP)==false )
	{
		if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
		{
			g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.4f);
			g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_ASTAROT_SP_UP, 5.0f);
		}
	}
}

bool CHardDamageAstarot::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CheckAndDebuff(pDamageInstance);
	CDamage::CrashPlayer(pDamageInstance, hitWho ,fDamageRate);
	return true;
}