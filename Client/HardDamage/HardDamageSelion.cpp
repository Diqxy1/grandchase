#include "stdafx.h"
#include "Controls.h"
#include "..\DamageManager.h"
#include "..\KGCRoomManager.h"
#include ".\HardDamageSelion.h"
#include "..\Buff\KGCBuffManager.h"

CHardDamageSelion::CHardDamageSelion( void )
{

}

CHardDamageSelion::~CHardDamageSelion( void )
{

}

void CHardDamageSelion::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	CheckAndDebuff(pDamageInstance);
	CDamage::CrashMonster(pDamageInstance,Monsteri);
	return;
}

void CHardDamageSelion::CheckAndDebuff(CDamageInstance* pDamageInstance)
{
    if ( pDamageInstance->m_What == DT_PET_SELION_ATTACK ||
        pDamageInstance->m_What == DT_PET_SELION_ATTACK_R ||
        pDamageInstance->m_What == DT_PET_SELION_CRASH )				
    {
#if defined(NATION_PHILIPPINE)
        return;
#endif
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ) { 
            if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_SELION_SP_UP)==false)
            {
                //마족 케릭터만 SP찹니당.
                if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
                {
#if defined( NATION_KOREA )
                    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(1.25f);
#else
                    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.5f);
#endif
                    g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_SELION_SP_UP, 5.0f);
                }						
            }
        }
    }
    else if ( pDamageInstance->m_What == DT_PET_BLACKDRAGON_ATTACK1 || 
        pDamageInstance->m_What == DT_PET_BLACKDRAGON_ATTACK2 || 
        pDamageInstance->m_What == DT_PET_BLACKDRAGON_ATTACK3 ) { 
            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ) { 
                if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLACK_DRAGON_SP_UP)==false)
                {
                    //마족 케릭터만 SP찹니당.
                    if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
                    {
                        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(1.25f);
                        g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLACK_DRAGON_SP_UP, 5.0f);
                    }						
                }					
            }
    }
	else if ( pDamageInstance->m_What == DT_PET_BERKAS_ATTACK1 || 
		pDamageInstance->m_What == DT_PET_BERKAS_ATTACK2 || 
		pDamageInstance->m_What == DT_PET_BERKAS_ATTACK3 ) { 
			if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ) { 
				if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_PET_BERKAS_SP_UP) == false)
				{
					//마족 케릭터만 SP찹니당.
					if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
					{
						g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.9f);
						g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_PET_BERKAS_SP_UP, 5.0f);
					}						
				}					
			}
	}
}

bool CHardDamageSelion::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CheckAndDebuff(pDamageInstance);
	CDamage::CrashPlayer(pDamageInstance, hitWho ,fDamageRate);
	return true;
}