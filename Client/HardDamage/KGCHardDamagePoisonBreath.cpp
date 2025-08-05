#include "stdafx.h"
#include ".\kgcharddamagepoisonbreath.h"

#include "DamageManager.h"
static int siBreathIndex = 0;

KGCHardDamagePoisonBreath::KGCHardDamagePoisonBreath(void)
{
	m_iBreathIndex = ++siBreathIndex;
}

KGCHardDamagePoisonBreath::~KGCHardDamagePoisonBreath(void)
{
}

bool KGCHardDamagePoisonBreath::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	//패킷 꾸미기
	if ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
	{
		g_kPoisonBreath->iBreathIndex = m_iBreathIndex;
		g_kPoisonBreath->fPosX = pDamageInstance->m_X;
		g_kPoisonBreath->fPosY = pDamageInstance->m_Y;
		//독구름 생성 시키기..
		for( int i = 0 ; i < 10 ; i++ )
			g_MyD3D->m_pDamageManager->MonsterAddWithLocate( DT_RENASIEN_GAS, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X ,pDamageInstance->m_Y );		
	}	
	CDamage::CrashPlayer( pDamageInstance, hitWho );

	return true;
}
