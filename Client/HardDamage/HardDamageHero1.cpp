#include "stdafx.h"
#include ".\HardDamageHero1.h"
#include "Monster.h"
#include "DamageManager.h"


CHardDamageKamikiLightning::CHardDamageKamikiLightning( void )
:dwTime(0)
{

}

CHardDamageKamikiLightning::~CHardDamageKamikiLightning( void )
{

}

void CHardDamageKamikiLightning::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove(pDamageInstance);

    if( false == g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) )
        return;

    if ( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPlayerState() == PS_DEAD )
    {
        pDamageInstance->m_Life = 1;
    }

    if ( dwTime++ % (GC_FPS_LIMIT * 3) == 0 )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
		g_MyD3D->m_pDamageManager->AddWithLocate(DT_HERO_KAMIKI_ATK3_DOT, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, 0, NULL, pDamageInstance->m_HitWho );

    }
}

CHardDamageKamikiLightningStarter::CHardDamageKamikiLightningStarter( void )
{

}

CHardDamageKamikiLightningStarter::~CHardDamageKamikiLightningStarter( void )
{

}

bool CHardDamageKamikiLightningStarter::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ )
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = g_MyD3D->m_pDamageManager->GetDamageInstancePool().find( pDamageInstance->m_What );

    if( itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end() ) {
        std::vector<CDamageInstance*>::iterator vit;
        for (vit=itor->second.begin() ;vit !=itor->second.end() ;vit++)
        {
            CDamageInstance* pDamageOld = *vit;

            // 저 데미지고.. 이미 한번 저 데미지를 맞았었다면.. 이전 데미지는 사라져!
            if ( pDamageOld->m_What == DT_HERO_KAMIKI_ATK3_2 && pDamageOld->m_HitWho == hitWho )
            {
                pDamageOld->m_Life = 1;
            }
        }
    }

    return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );
}