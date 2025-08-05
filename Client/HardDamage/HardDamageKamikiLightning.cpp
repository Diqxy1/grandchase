#include "stdafx.h"
#include "HardDamageKamikiLightning.h"

#include "Spark.h"

KHardDamageKamikiLightning::KHardDamageKamikiLightning(void)
{
}

KHardDamageKamikiLightning::~KHardDamageKamikiLightning(void)
{
}


bool KHardDamageKamikiLightning::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

    if( g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP ) &&
        g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP ) &&
        g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER ) )
        g_MyD3D->MyPlayer[hitWho]->Ah_Sound( 0 );

    if( pDamageInstance->m_What ==  DT_MONGBAN_LIGHTING_FIELD )
    {
        if( randf() < 0.3f )
            g_MyD3D->MySparks->Light( g_MyD3D->MyPlayer[hitWho]->MyBody.x + 0.15f, g_MyD3D->MyPlayer[hitWho]->vPos.y - 0.05f, 0, 1.2f );
    }
    else
    {
        g_MyD3D->MySparks->BlueLight( g_MyD3D->MyPlayer[hitWho]->MyBody.x + 0.15f, g_MyD3D->MyPlayer[hitWho]->vPos.y - 0.05f, 0, 2.0f );
    }

    return true;
}
