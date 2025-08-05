#include "stdafx.h"
#include ".\HardDamageRonan2MagicSpecial1.h"

#include "Buff/KGCBuffManager.h"

CHardDamageRonan2MagicSpecial1::CHardDamageRonan2MagicSpecial1()
{

}

CHardDamageRonan2MagicSpecial1::~CHardDamageRonan2MagicSpecial1()
{

}

bool CHardDamageRonan2MagicSpecial1::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */)
{
    if( false == g_MyD3D->IsPlayerIndex( hitWho ) )
        return false;

    g_pMagicEffect->SetMagicEffect(hitWho, EGC_EFFECT_RONAN_HEAL, 7.0f );
    
    return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );
}
