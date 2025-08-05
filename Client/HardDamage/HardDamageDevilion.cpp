#include "stdafx.h"
#include "Controls.h"
#include ".\HardDamageDevilion.h"
#include "KGCRoomManager.h"
#include "Buff/KGCBuffManager.h"

CHardDamageDevilion::CHardDamageDevilion( void )
{

}

CHardDamageDevilion::~CHardDamageDevilion( void )
{

}

void CHardDamageDevilion::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	CheckAndDebuff(pDamageInstance);
	CDamage::CrashMonster(pDamageInstance,Monsteri);
	return;
}

void CHardDamageDevilion::CheckAndDebuff(CDamageInstance* pDamageInstance)
{
	if ( pDamageInstance->m_What == DT_PET_DEVILION_ATK1 || pDamageInstance->m_What == DT_PET_DEVILION_ATK2 ) 
	{ 
		if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ) { 
			if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who, EGC_EFFECT_DEVILION_SP_UP) == false)
			{
				//마족 케릭터만 SP찹니당.
				if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
				{
					g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.5f);
					g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_DEVILION_SP_UP, 5.0f);
				}						
			}					
		}
	}
}

bool CHardDamageDevilion::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	CheckAndDebuff(pDamageInstance);
	CDamage::CrashPlayer(pDamageInstance, hitWho ,fDamageRate);
	return true;
}