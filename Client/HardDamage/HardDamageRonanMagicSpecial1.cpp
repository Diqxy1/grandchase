// HardDamage 제거작업 시작
// #include "stdafx.h"
// #include ".\HardDamageRonanMagicSpecial1.h"
// 
// #include "Buff/KGCBuffManager.h"


// bool CHardDamageRonanMagicSpecial1::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */)
// {
//     bool bRet;
//     if( false == g_MyD3D->IsPlayerIndex( hitWho ) )
//         return false;
// 
//     if( bRet = CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate ) )
//         g_pMagicEffect->SetMagicEffect(hitWho, EGC_EFFECT_RONAN_MAGIC_SWORD, 25.0f );
//     return bRet;
// }



// bool CHardDamageRonanMagicSpecial1Lv2::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */)
// {
//     bool bRet;
//     if( false == g_MyD3D->IsPlayerIndex( hitWho ) )
//         return false;
// 
//     if( bRet = CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate ) )
//         g_pMagicEffect->SetMagicEffect(hitWho, EGC_EFFECT_RONAN_MAGIC_SWORD, 25.0f, g_pMagicEffect->GetBuffLevel( hitWho, EGC_EFFECT_RONAN_MAGIC_SWORD ) + 1);
//     return bRet;
// }
// 
// 
// bool CHardDamageRonanMagicSpecial1Lv3::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */)
// {
//     bool bRet;
//     if( false == g_MyD3D->IsPlayerIndex( hitWho ) )
//         return false;
// 
//     if( bRet = CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate ) )
//         g_pMagicEffect->SetMagicEffect(hitWho, EGC_EFFECT_RONAN_MAGIC_SWORD, 25.0f, g_pMagicEffect->GetBuffLevel( hitWho, EGC_EFFECT_RONAN_MAGIC_SWORD ) + 1);
//     return bRet;
// }
// HardDamage 제거작업 끝